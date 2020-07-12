#include "dcstatus.h"
#include "../../common/string_format.h"
#include <stdexcept>

const char *statusToMessage(dc_status_t status)
{

    switch (status) {
        case DC_STATUS_SUCCESS:
            return "Success";
        case DC_STATUS_DONE:
            return "Done";
        case DC_STATUS_UNSUPPORTED:
            return "Unsupported operation";
        case DC_STATUS_INVALIDARGS:
            return "Invalid arguments";
        case DC_STATUS_NOMEMORY:
            return "Out of memory";
        case DC_STATUS_NODEVICE:
            return "No device found";
        case DC_STATUS_NOACCESS:
            return "Access denied";
        case DC_STATUS_IO:
            return "Input/output error";
        case DC_STATUS_TIMEOUT:
            return "Timeout";
        case DC_STATUS_PROTOCOL:
            return "Protocol error";
        case DC_STATUS_DATAFORMAT:
            return "Data format error";
        case DC_STATUS_CANCELLED:
            return "Cancelled";
        default:
            return "Unknown error";
    }
}

void checkDCStatus(dc_status_t status) {
    if(status != DC_STATUS_SUCCESS && status != DC_STATUS_DONE) {
        std::string test = string_format("Invalid status code %s", statusToMessage(status));
        throw std::runtime_error(
            test
        );
    }
}
