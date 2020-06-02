# Usage

Using libdivecomputer you can extract dat afrom various divecomputer devices. The general process follows these x steps:

1. Creating a context for logging and error handling
2. Selecting the descriptor for your dive computer
3. Selecting and the transport to interface with your device
4. Downloading data from your device
5. Parsing the the data to a more usable state

## Status codes

Most functions return a status of the `dc_status_t`, this indicates whenever or not the operation succeeded. It can hold the following values

- `DC_STATUS_SUCCESS`: the opertion succeeded
- `DC_STATUS_DONE`: End of an iterator
- `DC_STATUS_UNSUPPORTED`: Operation not supported by current device
- `DC_STATUS_INVALIDARGS`: Invalid arguments given to the function
- `DC_STATUS_NOMEMORY`: Memory allocation failed
- `DC_STATUS_NODEVICE` Device not found
- `DC_STATUS_NOACCESS`Access denied to low level communication device
- `DC_STATUS_TIMEOUT` Communication to device timedout most of the time this indicated a transport issue or the device could not be found
- `DC_STATUS_IO` An IO error
- `DC_STATUS_PROTOCOL` Encountered unexpected data while communicating
- `DC_STATUS_DATAFORMAT` Encountered unexpected data while interpreting data contents
- `DC_STATUS_CANCELLED` Operation cancelled by user canculation

## Creating context

A common parameter for many functions is the `dc_context_t` it is libdivecomputers means of providing logging. You can create a new context with `dc_context_new(&context_ptr)` and clean it up afterwards with `dc_context_free(context_ptr)`. It is not required to provide the same `dc_context_t` to seperate functions.

### Log level

To change the log verbosity call `dc_context_set_loglevel(context_ptr, loglevel)` which accepts the `dc_context_t` to set the loglevel of and a loglevel `dc_loglevel_t.
The `dc_loglevel_t` this can have the following values

- `DC_LOGLEVEL_NONE` 
- `DC_LOGLEVEL_ERROR`
- `DC_LOGLEVEL_WARNINGS`
- `DC_LOGLEVEL_INFO`
- `DC_LOGLEVEL_DEBUG`
- `DC_LOGLEVEL_ALL`

These values each increase the logging, to when setting it to `DC_LOGLEVEL_INFO`the context will also provide logging for warnings and errors as well as info messages.

### Log function

To receive the log messages a callback function needs to be set with `dc_context_set_logfunc(context_ptr, func, userdata)`where the context_ptr is of type `dc_context_t*` and refers to the context to set the loglevel of. The func is a function pointer of type `(*dc_logfunc_t)(dc_context_t* ctx, dc_loglevel_t loglevel, const char* file, unsigned int line, const char* function, const char* message, void* userdata)`and lastly the userdata is a pointer to any data you want to access in you logfunc callback. it is given as `userdata`.

### Example


## Selecting descriptors

## Selecting transport

## Downloading

## Parsing

## Cleanup