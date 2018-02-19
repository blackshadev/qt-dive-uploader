#include "dive.h"
template <typename T>
void get_field_value(dc_parser_t* parser, dc_field_type_t type, unsigned int flag, optional<T>* field) {
    auto st = dc_parser_get_field(parser, type, flag, &field->value);
    field->has_value = st == DC_STATUS_SUCCESS;
}

Dive::Dive(dc_parser_t* parser, const unsigned char* data, size_t size)
{

    dc_parser_set_data(parser, data, size);
    dc_parser_get_datetime(parser, &datetime);

    dc_parser_get_field(parser, DC_FIELD_MAXDEPTH, 0, &maxDepth);
    dc_parser_get_field(parser, DC_FIELD_DIVETIME, 0, &divetime);

    get_field_value(parser, DC_FIELD_TEMPERATURE_MAXIMUM, 0, &minTemperature);
    get_field_value(parser, DC_FIELD_TEMPERATURE_MINIMUM, 0, &maxTemperature);


}
