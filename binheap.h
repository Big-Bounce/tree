#ifndef BINHEAP_H_INCLUDED
#define BINHEAP_H_INCLUDED

#include "bintree.h"
#include "structures.h"

template <typename T, typename Direction>
class binheap
{
    bintree<T> _tree;

public:
    binheap();
    binheap(std::vector<T>& arg);
    void show();
    std::string& describe(std::string& arg);
    void push(const T& arg);
    void pop();
    void set_traversal (typename bintree<T>::traversal arg);
};

#include "binheap_imp.h"

#endif // BINHEAP_H_INCLUDED
