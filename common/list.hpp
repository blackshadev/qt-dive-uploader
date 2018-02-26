#include "list.h"
#include <cstring>
#include <stdexcept>

template <class TData>
List<TData>::List() {
    m_length = 0;
    m_data = new TData[0];
}

template <class TData>
List<TData>::~List() {
    delete[] m_data;
    m_data = NULL;
    m_length = 0;
}

template <class TData>
void List<TData>::resize(unsigned int new_length) {
    unsigned int min_length = new_length < m_length ? new_length : m_length;
    TData* new_data = NULL;
    if(new_length > 0)  {
        new_data = new TData[new_length];
    }
    if(new_length > 0 && m_length > 0) {
        std::memcpy(new_data, m_data, min_length * sizeof(TData));
    }
    if(m_length > 0) {
        delete[] m_data;
    }

    m_length = new_length;
    m_data = new_data;
}

template <class TData>
TData* List<TData>::data() {
    return m_data;
}

template <class TData>
TData List<TData>::item(int idx) {
    if(idx < 0 || idx > this->m_length) {
        throw std::invalid_argument("Given index out of bound");
    }
    return m_data[idx];
}

template <class TData>
TData* List<TData>::itemPtr(int idx) {
    if(idx < 0 || idx > this->m_length) {
        throw std::invalid_argument("Given index out of bound");
    }
    return &m_data[idx];
}

template <class TData>
unsigned int List<TData>::length() {
    return m_length;
}

template <class TData>
void List<TData>::set(TData *data, unsigned int len) {
    resize(len);
    std::memcpy(m_data, data, len * sizeof(TData));
}
