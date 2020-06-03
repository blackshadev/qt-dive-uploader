# Usage

## Overview

Using libdivecomputer you can extract data from various divecomputer devices. The general process follows these x steps:

1. Creating a context for logging and error handling
2. Selecting the descriptor for your dive computer
3. Selecting and the transport to interface with your device
4. Selecting a transport device and opening communication with it
5. Downloading data from your device
6. Parsing the the data to a more usable state

## Common patterns

This section describes some common patterns used throughout this library.

### `dc_status_t` as return value

Most functions return a status of the `dc_status_t`, this indicates whenever or not the operation succeeded. It can hold the following values

- `DC_STATUS_SUCCESS`: the operation succeeded
- `DC_STATUS_DONE`: End of an iterator
- `DC_STATUS_UNSUPPORTED`: Operation not supported by current device
- `DC_STATUS_INVALIDARGS`: Invalid arguments given to the function
- `DC_STATUS_NOMEMORY`: Memory allocation failed
- `DC_STATUS_NODEVICE` Device not found
- `DC_STATUS_NOACCESS`Access denied to low level communication device
- `DC_STATUS_TIMEOUT` Communication to device timed out most of the time this indicated a transport issue or the device could not be found
- `DC_STATUS_IO` An IO error
- `DC_STATUS_PROTOCOL` Encountered unexpected data while communicating
- `DC_STATUS_DATAFORMAT` Encountered unexpected data while interpreting data contents
- `DC_STATUS_CANCELLED` Operation cancelled by user cancellation

### Iterators

A common pattern in libdivecomputer is the use of iterators to iterate over a sets of data. Examples are iterating over the supported device descriptors, or available devices in a transport. Commonly you have a function which initializes a iterator by reference, which you can use to iterate with `dc_status_t dc_iterator_next(dc_iterator_t *iter, void *data)`, the `data` parameter is filled with the data you try to iterate and it's type depends on the type of iterator. It is set by reference so do note to give it a reference. After the iteration is done you can clean it up with `dc_status_t dc_iterator_free(dc_iterator_t *iter)`

The return value of `dc_iterator_next` is `dc_status_t` as described earlier. On the last value it will return `DC_STATUS_DONE` to indicate the iterator is done.

A common pattern is to iterate the iterator in a while look shown in the example

```c
/**
 * Based on: https://github.com/libdivecomputer/libdivecomputer/blob/master/examples/dctool_list.c
 */
dc_iterator_t *iterator = NULL;
dc_descriptor_t *descriptor = NULL;
dc_status_t status;
dc_descriptor_iterator (&iterator);
while ((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
    printf(
        "%s %s\n",
        dc_descriptor_get_vendor (descriptor),
        dc_descriptor_get_product (descriptor)
    );
    dc_descriptor_free (descriptor);
}
dc_iterator_free (iterator);

return status != DC_STATUS_DONE;

```

## Creating context

One of the first steps to using libdivecomputer is creating a `dc_context_t` for logging. It is a common required parameter for many functions and thus a logical step to start. You can create a new context with `dc_status_t dc_context_new(dc_contect **context_ptr)` and clean it up afterwards with `dc_status_t dc_context_free(dc_context_t *context_ptr)`. It is not required to provide the same `dc_context_t` to seperate functions and you can change the loglevel dynamicly as seen below.

### Log level

To change the log verbosity call `dc_status_t dc_context_set_loglevel(dc_context_t *context_ptr, dc_loglevel_t loglevel)` which accepts the `dc_context_t` to set the loglevel of and a loglevel `dc_loglevel_t.
The `dc_loglevel_t` this can have the following values

- `DC_LOGLEVEL_NONE` 
- `DC_LOGLEVEL_ERROR`
- `DC_LOGLEVEL_WARNINGS`
- `DC_LOGLEVEL_INFO`
- `DC_LOGLEVEL_DEBUG`
- `DC_LOGLEVEL_ALL`

These values each increase the logging, to when setting it to `DC_LOGLEVEL_INFO`the context will also provide logging for warnings and errors as well as info messages.

### Log function

To receive the log messages a callback function needs to be set with `dc_status_t dc_context_set_logfunc(dc_context_t *ctx, dc_logfunc_t *func, void *userdata)`where the context_ptr is of type `dc_context_t *` and refers to the context to set the loglevel of. The func is a function pointer of type `func*(dc_context_t *ctx, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *message, void *userdata)`and lastly the userdata is a pointer to any data you want to access in you logfunc callback, this userdatais given as `userdata`.

### Example

```c
/**
 * Based on https://github.com/libdivecomputer/libdivecomputer/blob/master/examples/common.c
 */
#include <libdivecomputer/context.h>

static void
logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
	const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};

	if (loglevel == DC_LOGLEVEL_ERROR || loglevel == DC_LOGLEVEL_WARNING) {
		printf ("%s: %s [in %s:%d (%s)]\n", loglevels[loglevel], msg, file, line, function);
	} else {
		printf ("%s: %s\n", loglevels[loglevel], msg);
	}
}

int main(int argc, char *argv) {
    dc_context_t *ctx = NULL;
    dc_status_t status;
    int return_value = 0;

    status = dc_context_new(&ctx);
    if(status != DC_STATUS_SUCCESS) {
        return_value = 1;
        goto cleanup;
    }

    status = dc_context_set_loglevel(ctx, DC_LOGLEVEL_WARNINGS);
    if(status != DC_STATUS_SUCCESS) {
        return_value = 1;
        goto cleanup;
    }

    status = dc_context_set_logfunc(ctx, logfunc, NULL)
    if(status != DC_STATUS_SUCCESS) {
        return_value = 1;
        goto cleanup;
    }

    // other code

cleanup:
    dc_context_free(context);

    return return_value;
}
```

## Selecting descriptors

After creating a context next up is selecting a device you would like to download dives from. Initially this is done by selecting a descriptor of type `dc_descriptor_t`. This is a descriptive representation of the device supported by libdivecomputer. One way of selecting it is by iterating through the supported devices and selecting the one you want.

The descriptor can be used to display certain information about the device, for instance getting the vendor name with `const char* dc_descriptor_get_vendor(dc_descriptor_t *descr)`, or the name of the divecomputer with `const char* dc_descriptor_get_product(dc_descriptor_t *descr)` . And for the next step, getting the supported transports with `unsigned int dc_descriptor_get_transports(dc_descriptor_t *descr)`.

### Example

This is a modified version of the example seen in the iterators section

```c
#include <string.h>
#include <libdivecomputer/descriptor.h>

/**
 * Based on https://github.com/libdivecomputer/libdivecomputer/blob/master/examples/common.c
 */
dc_status_t get_descriptor(
    dc_descriptor_t **out,
    const char *name
) {
    dc_iterator_t *iterator = NULL;
    dc_descriptor_t *descriptor = NULL;
    dc_descriptor_t *current = NULL;
    dc_status_t status;

    dc_descriptor_iterator (&iterator);
    while ((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
        const char *vendor = dc_descriptor_get_vendor (descriptor);
        const char *product = dc_descriptor_get_product (descriptor);
        
        size_t n = strlen (vendor);
		if (
            strncasecmp (name, vendor, n) == 0 && name[n] == ' ' &&
			strcasecmp (name + n + 1, product) == 0
        ) {
            current = descriptor;
            // do not free the descriptor you want to use after this function
        } else {
            dc_descriptor_free (descriptor);
        }
    }
    dc_iterator_free (iterator);

    *out = current;

    return status;
}
```

## Selecting transport

After creating a logging context and selecting a target device descriptor, you want to select a transport type to download the data from the device of. 
First of, not all devices support all transports. To determine which transports are usable for a given descriptor you use `unsigned int dc_descriptor_get_transports(dc_descriptor_t *descr)` which will give you a combined flag of all supported transports. To check if a device supports for instance a USBHID transport you use `dc_descriptor_get_transports(dc_descriptor_t *descr) & DC_TRANSPORT_USBHID` where `DC_TRANSPORT_USBHID` is of type `dc_transport_t`. 

Available transports are:

- DC_TRANSPORT_USBHID
- DC_TRANSPORT_USB
- DC_TRANSPORT_BLUETOOTH
- DC_TRANSPORT_BLE
- DC_TRANSPORT_IRDA 
- DC_TRANSPORT_SERIAL
- DC_TRANSPORT_NONE


## Selecting source Device

After selecting a transport you need to list the available sources. If you have multiple devices plugged in, or if the transport type doesn't support the detection of the specific device. 

### Iterating transport devices

Each type of transport has it's own iterator function, all variants have the same arguments `fn(dc_iterator_t **iterator, dc_context_t *ctx, dc_descriptor_t *descriptor)` but each iterator returns a different device type. 
 
- USBHID: `dc_usbhid_iterator_new` iterates over `dc_usbhid_device_t` devices
- Serial: `dc_serial_iterator_new` iterates over `dc_serial_device_t` devices
- IRDA `dc_irda_iterator_new` iterates over `dc_irda_device_t` devices
- bluetooth `dc_bluetooth_iterator_new` iterates over `dc_bluetooth_device_t` devices

### Interfacing functions for different transports

Each device type has it's own set of interfacing functions
- `dc_usbhid_device_t` has `int dc_usbhid_device_t(dc_usbhid_device_t *dev)` and  `int dc_usbhid_device_get_vid(dc_usbhid_device_t *dev)`
- `dc_irda_device_t` has  `const char* dc_irda_device_get_name(dc_irda_device_t *device)` and `unsigned int dc_irda_device_get_address(dc_irda_device_t *device)`
- `dc_serial_device_t` has `const char* dc_serial_device_get_name(dc_serial_device_t* device)`
- `dc_bluetooth_device_t` has `const char* dc_bluetooth_device_get_name(dc_bluetooth_device_t* device)` and `dc_bluetooth_address_t dc_bluetooth_device_get_address(dc_bluetooth_device_t *)`

Some of these functions are purely for displaying, others are also required when connecting to the source.

### Connecting to the source

As with the iterating of transport devices for each type of transport there is a different open function. The firsty to parameters are the same for each function. The first is the output parameter setting a `dc_iostream_t`, the second is the `dc_context_t` for logging. The other parameters differ between transports. 

- USBHID: `dc_status_t dc_usbhid_open(dc_iostream_t** strm, dc_context_t* ctx, dc_usbhid_device_it*dev)`
- BLUETOOTH: `dc_status_t dc_bluebooth_open(dc_iostream_t** strm, dc_context_t* ctx, dc_bluebooth_address_t addr, unsigned int port)`


## Downloading Dives

## Parsing Dives

## Cleanup