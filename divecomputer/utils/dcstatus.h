#ifndef DCSTATUS_H
#define DCSTATUS_H
#include <libdivecomputer/common.h>

const char* statusToMessage(dc_status_t status);
void checkDCStatus(dc_status_t status);

#endif // DCSTATUS_H
