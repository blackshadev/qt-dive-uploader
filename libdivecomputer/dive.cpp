#include "dive.h"

Dive::Dive(dc_parser_t* parser, const unsigned char* data, size_t size)
{

    dc_parser_set_data(parser, data, size);
    dc_parser_get_datetime(parser, &datetime);

    get_field(parser, DC_FIELD_MAXDEPTH, &maxDepth);
    get_field(parser, DC_FIELD_DIVETIME, &divetime);

    get_optional_field(parser, DC_FIELD_TEMPERATURE_MAXIMUM, &minTemperature);
    get_optional_field(parser, DC_FIELD_TEMPERATURE_MINIMUM, &maxTemperature);
    get_optional_field(parser, DC_FIELD_TEMPERATURE_SURFACE, &surfaceTemperature);
    get_optional_field(parser, DC_FIELD_ATMOSPHERIC, &atmosphericPressure);
    get_optional_field(parser, DC_FIELD_SALINITY, &salinity);
    get_optional_field(parser, DC_FIELD_DIVEMODE, &divemode);

    get_list_field(parser, DC_FIELD_TANK_COUNT, DC_FIELD_TANK, &tankPressures);
    get_list_field(parser, DC_FIELD_GASMIX_COUNT, DC_FIELD_GASMIX, &gasMixures);

}

template <typename T>
dc_status_t Dive::get_optional_field(dc_parser_t* parser, dc_field_type_t type,  optional_t<T>* field, unsigned int flag) {
    auto st = get_field(parser, type, &field->value, flag);
    field->has_value = st == DC_STATUS_SUCCESS;
    return st;
}

template <typename T>
dc_status_t Dive::get_field(dc_parser_t* parser, dc_field_type_t type, T* field, unsigned int flag) {
    return dc_parser_get_field(parser, type, flag, field);
}
template <typename TData>
void Dive::get_list_field(dc_parser_t* parser, dc_field_type_t length_type, dc_field_type_t data_type, List<TData>* field) {
    unsigned int count;
    get_field(parser, length_type, &count);

    field->resize(count);

    for(unsigned int i = 0; i < count; i++) {
        get_field(parser, data_type, &field->data[i], i);
    }

}
