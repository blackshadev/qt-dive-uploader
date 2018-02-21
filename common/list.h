#ifndef LIST_H
#define LIST_H

template <class TData>
class List {
public:
    unsigned int length = 0;
    TData* data;

    List();
    ~List();

    void resize(unsigned int len);
};

#include "list.hpp"
#endif // LIST_H
