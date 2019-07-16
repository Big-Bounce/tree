#ifndef BSTTREE_H_INCLUDED
#define BSTTREE_H_INCLUDED

#include "bintree.h"
#include <vector>

template <typename T, typename Direction = dsc>
class bsttree
{
    bintree<T> _tree;
    void _pop(typename bintree<T>::node *arg);

    //searching
    typename bintree<T>::node * find_key (T key, typename bintree<T>::node * start = 0x0);
    typename bintree<T>::node * find_left_optimum (typename bintree<T>::node * start = 0x0);
    typename bintree<T>::node * find_right_optimum (typename bintree<T>::node * start = 0x0);
    typename bintree<T>::node * find_next (typename bintree<T>::node *arg);
    typename bintree<T>::node * find_prev (typename bintree<T>::node *arg);

public:
    bsttree();
    bsttree(std::vector<T> &arg);
    void show();
    std::string& describe(std::string& arg);

    void push(T const &arg);
    void pop(T key);
    void pop_all(T key);
    void balance();
};

#include "bsttree_imp.h"

#endif // BSTTREE_H_INCLUDED
