#include "dive.h"

Dive::Dive(dc_parser_t* parser, const unsigned char* data, size_t size)
{

    dc_parser_set_data(parser, data, size);
    dc_parser_get_datetime(parser, &datetime);

    dc_parser_get_field(parser, DC_FIELD_MAXDEPTH, 0, &maxDepth);
    dc_parser_get_field(parser, DC_FIELD_DIVETIME, 0, &divetime);

//    dc_parser_get_field(parser, DC_FIELD_TEMPERATURE_MAXIMUM, 0, &minTemperature);
//    dc_parser_get_field(parser, DC_FIELD_TEMPERATURE_MINIMUM, 0, &maxTemperature);


}
