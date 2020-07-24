#ifndef VECTOR_H
#define VECTOR_H

#define FREE_VECTOR(V) V->clear(); delete V;
#define FREE_VECTOR_PTR(V) FREE_VECTOR_ITEMS(*V) FREE_VECTOR(V)
#define FREE_VECTOR_ITEMS(V)  for (auto item : V) { delete item; }

#endif // VECTOR_H
