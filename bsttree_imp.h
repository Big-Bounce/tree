#ifndef BSTTREE_IMP_H_INCLUDED
#define BSTTREE_IMP_H_INCLUDED

#include <iostream>

//=============================================
//CONSTRUCTORS

template <typename T, typename Direction>
bsttree<T, Direction>::bsttree(): _tree()
{
}

template <typename T, typename Direction>
bsttree<T, Direction>::bsttree(std::vector<T> &arg): _tree()
{
    while (!arg.empty())
    {
        push(arg.back());
        arg.pop_back();
    }
}

//=============================================
//FINDING

template <typename T, typename Direction>
typename bintree<T>::node* bsttree<T, Direction>::find_key (T key, typename bintree<T>::node * start)
{
    if (start == 0x0)
        start = _tree._root;
    if (start == 0x0)
        return 0x0;

    while (start != 0x0 && (Direction().compare(key, start -> value) || Direction().compare(start -> value, key)))
    {
        if (Direction().compare(start -> value, key))
            start = start -> left;
        else
            start = start -> right;
    }

    return start;
}

template <typename T, typename Direction>
typename bintree<T>::node* bsttree<T, Direction>::find_left_optimum(typename bintree<T>::node * start)
{
    if (start == 0x0)
        start = _tree._root;
    if (start == 0x0)
        return 0x0;
    while (start -> left != 0x0)
        start = start -> left;
    return start;
}

template <typename T, typename Direction>
typename bintree<T>::node* bsttree<T, Direction>::find_right_optimum(typename bintree<T>::node * start)
{
    if (start == 0x0)
        start = _tree._root;
    if (start == 0x0)
        return 0x0;
    while (start -> right != 0x0)
        start = start -> right;
    return start;
}

template <typename T, typename Direction>
typename bintree<T>::node* bsttree<T, Direction>::find_next (typename bintree<T>::node * start)
{
    if (start == 0x0)
        return start;

    if (start -> right != 0x0)
        return find_left_optimum (start -> right);
    else
        while (start -> up != 0x0 && start -> up -> left != start)
            start = start -> up;
    return start -> up;
}

template <typename T, typename Direction>
typename bintree<T>::node* bsttree<T, Direction>::find_prev (typename bintree<T>::node * start)
{
    if (start == 0x0)
        return start;

    if (start -> left != 0x0)
        return find_right_optimum (start -> left);
    else
        while (start -> up != 0x0 && start -> up -> right != start)
            start = start -> up;
    return start -> up;
}

//=============================================
//ALTERING

template <typename T, typename Direction>
void bsttree<T, Direction>::push (T const &arg)
{
    if (_tree._root == 0x0)
    {
        _tree._root = new typename bintree<T>::node(arg);
        return;
    }

    typename bintree<T>::node* parent = 0x0;
    typename bintree<T>::node* child = _tree._root;
    do
    {
        parent = child;
        if (Direction().compare(parent -> value, arg))
        {
            if ((child = parent -> left) == 0x0)
                parent -> left = new typename bintree<T>::node(arg, parent);
        }

        else
            if ((child = parent -> right) == 0x0)
                parent -> right = new typename bintree<T>::node(arg, parent);
    } while (child != 0x0);

}


template <typename T, typename Direction>
void bsttree<T, Direction>::_pop(typename bintree<T>::node *arg)
{
    if (arg == 0x0)
        return;

    if (arg -> left == 0x0 && arg -> right == 0x0)
    {

        if (arg == _tree._root)
        {
            delete _tree._root;
            _tree._root = 0x0;
            return;
        }

        if (arg == arg -> up -> left)
            arg -> up -> left = 0x0;
        else
            arg -> up -> right = 0x0;
        delete arg;
        return;
    }

    if ((arg -> left == 0 && arg -> right != 0x0) || (arg -> left != 0 && arg -> right == 0))
    {
            if (arg -> left == 0x0 && arg -> right != 0x0)
                arg -> replace_with_child(arg -> right);
            else
                arg -> replace_with_child(arg -> left);
            return;
    }

    if (arg -> left != 0x0 && arg -> right != 0x0)
    {
        typename bintree<T>::node * next = find_next(arg);
        arg -> value = next -> value;
        if (next -> right == 0x0)
        {
            if (next == next -> up -> left)
                next -> up -> left = 0x0;
            else
                next -> up -> right = 0x0;
            delete next;
        }
        else
                next -> replace_with_child(next -> right);
    }
}

template <typename T, typename Direction>
void bsttree<T, Direction>::pop(T key)
{
    if (_tree._root != 0x0)
        _pop(find_key(key));
}

template <typename T, typename Direction>
void bsttree<T, Direction>::pop_all(T key)
{
    for (typename bintree<T>::node* temp = find_key(key); _tree._root != 0x0 && temp != 0x0; temp = find_key(key))
        _pop(temp);
}

//========================================================
//DESCRIBING

template <typename T, typename Direction>
void bsttree<T, Direction>::show()
{
    _tree.show();
}

template <typename T, typename Direction>
std::string& bsttree<T, Direction>::describe(std::string& arg)
{
    _tree.describe(arg);
}

//========================================================
//BALANCING

template <typename T, typename Direction>
void bsttree<T, Direction>::balance()
{
    if (_tree._root == 0x0)
        return;

    //degenerating
    unsigned nodecount = 0;
    typename bintree<T>::node *temp = _tree._root;
    while (temp != 0x0)
    {
        if (temp -> left != 0x0)
        {
            temp -> rotation(temp -> left, &_tree._root);
            temp = temp -> up;
        }
        else
        {
            ++nodecount;
            temp = temp -> right;
        }
    }

    //find number of future leaves
    unsigned leavescount = 1, help = nodecount + 1;
    help >>= 1;
    while (help)
    {
        leavescount <<= 1;
        help >>= 1;
    }
    leavescount = nodecount + 1 - leavescount;

    //balancing
    temp = _tree._root;
    for (unsigned i = 0; i < leavescount; ++i)
    {
        temp -> rotation(temp -> right, &_tree._root);
        temp = temp -> up -> right;
    }

    nodecount -= leavescount;
    while (nodecount > 1)
    {
        nodecount >>= 1;
        temp = _tree._root;
        for (unsigned i = 0; i < nodecount; ++i)
        {
                temp -> rotation(temp -> right, &_tree._root);
                temp = temp -> up -> right;
        }
    }

}



#endif // BSTTREE_IMP_H_INCLUDED
