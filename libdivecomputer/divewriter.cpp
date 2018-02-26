#include "divewriter.h"


// source: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
// source: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;
}


DiveWriter::DiveWriter()
{
}

DiveWriter::~DiveWriter()
{
    if(file.isOpen()) {
        close();
    }

}

void DiveWriter::set_filename(QString path)
{
    file.setFileName(path);
}

void DiveWriter::write_tank(QJsonArray* tanksArray, Dive *dive)
{

    dc_tank_t* tank;
    dc_gasmix_t* gas;

    int gasMixCount = dive->gasMixures.length();

    for(int i = 0; i < dive->tankPressures.length(); i++)
    {
        QJsonObject tankObj;
        QJsonObject presObj;

        gas = dive->gasMixures.itemPtr(gasMixCount > i ? i : 0);
        tank = dive->tankPressures.itemPtr(i);

        tankObj["volume"] = tank->volume;
        tankObj["oxygen"] = gas->oxygen * 100;

        presObj["begin"] = tank->beginpressure;
        presObj["end"] = tank->endpressure;

        switch(tank->type) {
            case DC_TANKVOLUME_IMPERIAL:
                presObj["type"] = "psi";
            break;
            case DC_TANKVOLUME_METRIC:
                presObj["type"] = "bar";
            break;
            default:
                presObj["type"] = "bar";
            break;
        }

        tankObj["pressure"] = presObj;
        tanksArray->append(tankObj);
    }

}

void DiveWriter::write_samples(QJsonArray *tanksArray, Dive *dive)
{
    for(auto sample : dive->samples) {
        QJsonObject obj;
        obj["Time"] = sample->time;
        obj["Depth"] = sample->depth;
        obj["Temperature"] = sample->temperature;
        tanksArray->append(obj);
    }
}

void DiveWriter::write(Dive* dive)
{

    QJsonObject json;
    char dt[25];
    std::sprintf(
        dt,
        "%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d",
        dive->datetime.year,
        dive->datetime.month,
        dive->datetime.day,
        dive->datetime.hour,
        dive->datetime.minute,
        dive->datetime.second,
        dive->datetime.timezone / 3600,
        dive->datetime.timezone % 3600
    );
    json["Datetime"] = dt;
    json["Divetime"] = (int)dive->divetime;

    json["Fingerprint"] = QString::fromStdString(base64_encode(dive->fingerprint.data(), dive->fingerprint.length()));
    json["MaxDepth"] = dive->maxDepth;
    json["MinTemperature"] = dive->minTemperature;
    json["MaxTemperature"] = dive->maxTemperature;

    QJsonArray tankArr;
    write_tank(&tankArr, dive);

    json["Tanks"] = tankArr;

    QJsonArray sampleArr;
    write_samples(&sampleArr, dive);
    json["Samples"] = sampleArr;

    jsonDives.append(json);
}

void DiveWriter::open() {
    if(file.isOpen()) {
        throw std::runtime_error("File was already opened");
    }
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Couldn't open save file.");
    }
}

void DiveWriter::end() {

    jsonObject["dives"] = jsonDives;

    QJsonDocument saveDoc(jsonObject);
    file.write(saveDoc.toJson());
}


void DiveWriter::close() {
    if(!file.isOpen()) {
        throw std::runtime_error("File was not yet opened");
    }
    end();
    file.close();
}
