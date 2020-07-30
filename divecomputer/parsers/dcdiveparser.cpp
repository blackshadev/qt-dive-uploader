#include "dcdiveparser.h"
#include "dcdivesampleparser.h"

template <typename T>
inline dc_status_t _getField(dc_parser_t* parser, dc_field_type_t type, T* field, unsigned int flag = 0)
{
    return dc_parser_get_field(parser, type, flag, field);
}

template <typename T>
inline dc_status_t setField(dc_parser_t* parser, dc_field_type_t type, std::function<void(T)> setItem, unsigned int flag = 0)
{
    T value;
    auto st = dc_parser_get_field(parser, type, flag, &value);
    if (st == DC_STATUS_SUCCESS) {
        setItem(value);
    }

    return st;
}

inline void emptyDCData(dc_tank_t &tank)
{
    tank.type = DC_TANKVOLUME_NONE;
    tank.gasmix = 0;
    tank.volume = 0;
    tank.endpressure = 0;
    tank.workpressure = 0;
    tank.beginpressure = 0;
}
inline void emptyDCData(dc_gasmix_t &mix)
{
    mix.helium = 0;
    mix.oxygen = 0;
    mix.nitrogen = 0;
}

template <typename TData>
inline void setListFields(dc_parser_t* parser, dc_field_type_t length_type, dc_field_type_t data_type,  std::function<void(TData)> addItem)
{
    unsigned int count;
    auto st = _getField(parser, length_type, &count);

    if (st != DC_STATUS_SUCCESS) {
        return;
    }

    TData data;
    emptyDCData(data);
    for (unsigned int i = 0; i < count; i++) {
        _getField(parser, data_type, &data, i);
        addItem(data);
    }
}

DCDiveParser::DCDiveParser()
{
    parser = NULL;
    sampleParser = NULL;
}

DCDiveParser::~DCDiveParser()
{
    if (sampleParser) {
        delete sampleParser;
        sampleParser = NULL;
    }
    if (parser) {
        dc_parser_destroy(parser);
        parser = NULL;
    }
}

DCDiveParser *DCDiveParser::setDevice(DCDeviceInterface *dev)
{
    reset();
    device = dev;
    return this;
}

DCDiveParser *DCDiveParser::setContext(DCContextInterface *ctx)
{
    context = ctx;
    return this;
}

DCDive *DCDiveParser::parseDive(rawdivedata_t &divedata)
{
    if (!initialized) {
        initialize();
    }
    if (parser == NULL) {
        throw new std::runtime_error("Parser not yet initialized, call setDevice first");
    }
    auto dive = createDive();
    dive->setFingerprint(divedata.fingerprint);

    dc_parser_set_data(parser, divedata.data, divedata.size);

    dc_datetime_t datetime;
    dc_parser_get_datetime(parser, &datetime);
    dive->setDatetime(parseDatetime(datetime));

    setField<double>(parser, DC_FIELD_MAXDEPTH, [dive](double depth) { dive->setMaxDepth(depth); });
    setField<unsigned int>(parser, DC_FIELD_DIVETIME, [dive](unsigned int time) { dive->setDivetime(time); });
    setField<double>(parser, DC_FIELD_TEMPERATURE_MAXIMUM, [dive](double temp) { dive->setMaxTemperature(temp); });
    setField<double>(parser, DC_FIELD_TEMPERATURE_MINIMUM, [dive](double temp) { dive->setMaxTemperature(temp); });
    setField<double>(parser, DC_FIELD_TEMPERATURE_SURFACE, [dive](double temp) { dive->setSurfaceTemperature(temp); });
    setField<double>(parser, DC_FIELD_ATMOSPHERIC, [dive](double temp) { dive->setAtmosphericPressure(temp); });
    setField<dc_salinity_t>(parser, DC_FIELD_SALINITY, [dive](dc_salinity_t salinity) { dive->setSalinity(salinity); });
    setField<dc_divemode_t>(parser, DC_FIELD_DIVEMODE, [dive](dc_divemode_t mode) { dive->setDivemode(mode); });

    std::vector<dc_tank_t> tanks;
    std::vector<dc_gasmix_t> gasmixes;

    setListFields<dc_tank_t>(
        parser,
        DC_FIELD_TANK_COUNT,
        DC_FIELD_TANK,
        [&tanks](dc_tank_t tank) { tanks.push_back(tank); }
    );
    setListFields<dc_gasmix_t>(
        parser,
        DC_FIELD_GASMIX_COUNT,
        DC_FIELD_GASMIX,
        [&gasmixes](dc_gasmix_t mix) { gasmixes.push_back(mix); }
    );

    for (unsigned int iX = 0; iX < std::max(tanks.size(), gasmixes.size() ); iX++) {
        tank_t tank;
        if (iX < tanks.size()) {
            tank.pressures = tanks.at(iX);
        }
        if (iX < gasmixes.size()) {
            tank.gasmix = gasmixes.at(iX);
        }
        dive->addTank(tank);
    }

    sampleParser->setCallback([dive](DCDiveSample *sample) {
        dive->addSample(sample);
    });
    
    dc_parser_samples_foreach(parser, [](dc_sample_type_t type, dc_sample_value_t value, void *userdata) {
        auto sampleParser = (DiveSampleParser *)userdata;
        sampleParser->addSample({ type, value });
    }, sampleParser);

    sampleParser->finalize();

    finalize(dive);

    return dive;
}

inline datetime_t DCDiveParser::parseDatetime(dc_datetime_t datetime)
{
    return (datetime_t)datetime;
}

inline salinity_t DCDiveParser::parseSalinity(dc_salinity_t salinity)
{
    return (salinity_t)salinity;
}

inline tank_t DCDiveParser::parseTank(optional_t<dc_tank_t> pressure, optional_t<dc_gasmix_t> gasmix)
{
    tank_t tank;

    if (pressure.has_value) {
        tank.pressures = pressure.value;
    }
    if(gasmix.has_value) {
        tank.gasmix = gasmix.value;
    }

    return tank;
}

DCDive *DCDiveParser::createDive()
{
    return new DCDive();
}

DiveSampleParser *DCDiveParser::createSampleParser()
{
    return new DiveSampleParser();
}

void DCDiveParser::initialize()
{
    if (initialized) {
        throw std::runtime_error("Parser already initialized");
    }
    if (!context || !device) {
        throw std::runtime_error("Unable to initialize parser, some properties are missing");
    }

    sampleParser = createSampleParser();
    dc_parser_new(&parser, device->getNative(context));
    initialized = true;
}

void DCDiveParser::finalize(DCDive *dive)
{
    fixTankPressuresFromSamples(dive);
}

template<typename TIterator>
void findPressureSample(
        double &out,
        unsigned int iTank,
        TIterator sampleStart,
        TIterator sampleEnd
) {
    DCDiveSample *sample;
    for(TIterator it = sampleStart; it != sampleEnd; ++it) {
        sample = *it;
        for(auto const& pressure: *(sample->getPressures())) {
            if(pressure.tank == iTank) {
                out = pressure.pressure;
                return;
            }
        }
    }
}

void DCDiveParser::fixTankPressuresFromSamples(DCDive *dive)
{
    auto samples = dive->getSamples();
    auto tanks = dive->getTanks();

    if (!samples->size()) {
        return;
    }

    const unsigned int tankCount = dive->getTanks()->size();
    unsigned int iTank = 0;
    DCDiveSample *sample;

    for (iTank = 0; iTank < tanks->size(); iTank++) {
        tank_t* tank = tanks[iTank].data();

        if (!tank->pressures.has_value) {
            continue;
        }

        auto fixBeginPressure = tank->pressures.has_value || tank->pressures.value.beginpressure == 0;
        auto fixEndPressure = tank->pressures.has_value || tank->pressures.value.endpressure == 0;

        if (fixBeginPressure) {
            findPressureSample(tank->pressures.value.beginpressure, iTank, samples->begin(), samples->end());
        }

        if (fixEndPressure) {
            findPressureSample(tank->pressures.value.endpressure, iTank, samples->rbegin(), samples->rend());
        }
        tank->pressures.has_value = true;
    }
}

void DCDiveParser::reset()
{
    if (!initialized) {
        return;
    }

    if (sampleParser) {
        delete sampleParser;
        sampleParser = NULL;
    }
    if (parser) {
        dc_parser_destroy(parser);
        parser = NULL;
    }
    initialized = false;
}

