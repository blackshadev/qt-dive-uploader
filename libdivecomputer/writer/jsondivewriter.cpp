#include "jsondivewriter.h"
#include "../../common/base64.h"

void JsonDiveWriter::write_tank(QJsonArray &tanksArray, Dive *dive)
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
        tanksArray.append(tankObj);
    }

}

void JsonDiveWriter::write_samples(QJsonArray &sampleArray, Dive *dive)
{
    for(auto sample : dive->samples) {
        QJsonObject obj;
        obj["Time"] = sample->time;
        optional_write(&obj, "Depth", sample->depth);
        optional_write(&obj, "Temperature", sample->temperature);
        optional_write(&obj, "CNS", sample->cns);
        optional_write(&obj, "SetPoint", sample->setpoint);
        optional_write(&obj, "PPO2", sample->ppo2);
        optional_write(&obj, "Gasmix", sample->gasmix);
        optional_write(&obj, "Heartbeat", sample->heartbeat);
        optional_write(&obj, "RBT", sample->rbt);

        if(sample->events.size()) {
            QJsonArray eventArr;
            for(auto evt : sample->events) {
                QJsonObject evObj;
                evObj["Type"] = evt.type_name;
                evObj["Value"] = (int)evt.value;
                eventArr.append(evObj);
            }
            obj["Events"] = eventArr;
        }

        if(sample->pressures.size()) {
            QJsonArray pressArr;
            for(auto press : sample->pressures) {
                QJsonObject presObj;
                presObj["Tank"] = (int)press.tank;
                presObj["Pressure"] = press.pressure;
                pressArr.append(presObj);
            }
            obj["Pressure"] = pressArr;
        }

        sampleArray.append(obj);
    }
}

void JsonDiveWriter::write_dive(QJsonObject &json, Dive *dive)
{
    json["date"] = QString::fromStdString(format_datetime(&dive->datetime));
    json["divetime"] = (int)dive->divetime;

    json["fingerprint"] = base64_encode(dive->fingerprint.data(), dive->fingerprint.length());
    json["max_depth"] = dive->maxDepth;
    json["min_temperature"] = dive->minTemperature;
    json["max_temperature"] = dive->maxTemperature;

    QJsonArray tankArr;
    write_tank(tankArr, dive);

    json["tanks"] = tankArr;

    QJsonArray sampleArr;
    write_samples(sampleArr, dive);
    json["samples"] = sampleArr;

}

void JsonDiveWriter::write_computer(QJsonObject &json, dc_descriptor_t *device, uint serial)
{
    json["serial"] =  (int)(serial);
    json["vendor"] = QString(dc_descriptor_get_vendor(device));
    json["model"] = (int)(dc_descriptor_get_model(device));
    json["type"] = (int)(dc_descriptor_get_type(device));
    json["name"] = QString(dc_descriptor_get_product(device));

}
