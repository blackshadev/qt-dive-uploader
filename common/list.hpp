#include "list.h"
#include <cstring>

template <class TData>
List<TData>::List() {
    length = 0;
    data = new TData[0];
}

template <class TData>
List<TData>::~List() {
    delete[] data;
    data = NULL;
    length = 0;
}

template <class TData>
void List<TData>::resize(unsigned int new_length) {
    unsigned int min_length = new_length < length ? new_length : length;
    TData* new_data = NULL;
    if(new_length > 0)  {
        new_data = new TData[new_length];
    }
    if(new_length > 0 && length > 0) {
        std::memcpy(new_data, data, min_length * sizeof(TData));
    }
    if(length > 0) {
        delete[] data;
    }

    length = new_length;
    data = new_data;
}
