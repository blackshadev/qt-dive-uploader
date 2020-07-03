#include "diveparser.h"
#define GET_SET_FIELD(parser, type, target)
#define GET_SET_OPTIONAL_FIELD()
#define GET_SET_LIST_FIELD()

template <typename T>
dc_status_t getField(dc_parser_t* parser, dc_field_type_t type, T* field, unsigned int flag = 0)
{
    return dc_parser_get_field(parser, type, flag, field);
}

template <typename T>
dc_status_t setField(dc_parser_t* parser, dc_field_type_t type, void (*setItem)(T), unsigned int flag = 0)
{
    T value;
    auto st = dc_parser_get_field(parser, type, flag, &value);
    if(st == DC_STATUS_SUCCESS) {
        setItem(value);
    }
    return st;
}

template <typename T>
dc_status_t setOptionalField(dc_parser_t* parser, dc_field_type_t type,  void (*setItem)(optional_t<T>), unsigned int flag = 0)
{
    T value;
    auto st = getField(parser, type, &value, flag);
    if(st == DC_STATUS_SUCCESS) {
        setItem(value);
    }
    return st;
}

template <typename TData>
void setListField(dc_parser_t* parser, dc_field_type_t length_type, dc_field_type_t data_type,  void (*addItem)(TData))
{
    unsigned int count;
    getField(parser, length_type, &count);

    TData data;
    for(unsigned int i = 0; i < count; i++) {
        getField(parser, data_type, &data, i);
        addItem(data);
    }
}

DiveParser::DiveParser()
{
    parser = NULL;
}

void DiveParser::setDevice(DCDevice *device)
{
    if(parser != NULL) {
        dc_parser_destroy(parser);
    }
    dc_parser_new(&parser, device->getNative());
}

Dive *DiveParser::parseDives(const unsigned char *data, unsigned int size)
{
    if(parser == NULL) {
        throw new std::logic_error("Parser not yet initialized, call setDevice first");
    }
    auto dive = new Dive;

    divedata_t divedata;

    dc_parser_set_data(parser, data, size);

    dc_parser_get_datetime(parser, &divedata.datetime);
    dive->setDatetime(divedata.datetime);

//    setField(parser, DC_FIELD_MAXDEPTH, dive->setMaxDepth);
//    setField(parser, DC_FIELD_DIVETIME, &divedata.divetime);

//    getOptionalField(parser, DC_FIELD_TEMPERATURE_MAXIMUM, &divedata.maxTemperature);
//    getOptionalField(parser, DC_FIELD_TEMPERATURE_MINIMUM, &divedata.minTemperature);
//    getOptionalField(parser, DC_FIELD_TEMPERATURE_SURFACE, &divedata.surfaceTemperature);
//    getOptionalField(parser, DC_FIELD_ATMOSPHERIC, &divedata.atmosphericPressure);
//    getOptionalField(parser, DC_FIELD_SALINITY, &divedata.salinity);
//    getOptionalField(parser, DC_FIELD_DIVEMODE, &divedata.divemode);


//    getListField(parser, DC_FIELD_TANK_COUNT, DC_FIELD_TANK, dive->set);
//    get_list_field(parser, DC_FIELD_GASMIX_COUNT, DC_FIELD_GASMIX, &divedata.gasMixures);


    return dive;
}

