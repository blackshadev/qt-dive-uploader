#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T>
struct optional_t {
    bool has_value = false;
    T   value;
};

#endif // OPTIONAL_H
