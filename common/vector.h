#ifndef VECTOR_H
#define VECTOR_H

#define FREE_VECTOR(V) V->clear(); delete V;
#define FREE_VECTOR_PTR(V) for(int __iX = 0; __iX < V->size(); __iX++) { delete V->at(__iX); } FREE_VECTOR(V)


#endif // VECTOR_H
