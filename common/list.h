#ifndef LIST_H
#define LIST_H

template <class TData>
class List {
public:

    List();
    ~List();

    void resize(unsigned int len);
    TData* data();
    unsigned int length();
    void set(TData* data, unsigned int len);
private:
    unsigned int m_length;
    TData* m_data;
};

#include "list.hpp"
#endif // LIST_H
