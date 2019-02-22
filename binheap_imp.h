#ifndef BINHEAP_IMP_H_INCLUDED
#define BINHEAP_IMP_H_INCLUDED

#include <exception>
#include <iostream>

template <typename T, typename Direction>
binheap<T, Direction>::binheap(): _tree()
{
}

template <typename T, typename Direction>
binheap<T, Direction>::binheap(std::vector<T>& arg): _tree()
{
    while (!arg.empty())
    {
        push(arg.back());
        arg.pop_back();
    }
}

template <typename T, typename Direction>
inline void binheap<T, Direction>::show()
{
    _tree.show();
}

template <typename T, typename Direction>
inline std::string& binheap<T, Direction>::describe(std::string& arg)
{
    return _tree.describe(arg);
}

template <typename T, typename Direction>
void binheap<T, Direction>::push(const T& arg)
{
try
{
    if (_tree._root == 0x0)
        _tree._root = new typename bintree<T>::node(arg);

    else
    {
        _tree.describe();
        typename bintree<T>::node* temp;

        if (_tree._features.onechildnodes.size() == 1)
        {
            temp = _tree._features.onechildnodes.back();
            temp -> right = new typename bintree<T>::node(arg, temp);
            temp = temp -> right;
        }
        else
        {
            temp = _tree._features.minpathnode;
            temp -> left = new typename bintree<T>::node(arg, temp);
            temp = temp -> left;
        }

        while (temp != _tree._root && Direction().compare(temp -> value, temp -> up -> value))
        {
            T help = temp -> value;
            temp -> value = temp -> up -> value;
            temp -> up -> value = help;
            temp = temp -> up;
        }

    }

}

catch (std::bad_alloc const &exc)
{
    std::cout << exc.what();
}

}

template <typename T, typename Direction>
void binheap<T, Direction>::pop()
{
    if (_tree._root == 0x0)
        return;

    _tree.describe();
    typename bintree<T>::node* temp = _tree._root;

    if (_tree._features.onechildnodes.size() > 0)
    {
        temp = _tree._features.onechildnodes.back();
        _tree._root -> value = temp -> left -> value;
        delete temp -> left;
        temp -> left = 0x0;
    }

    else if (_tree._features.maxpathnode != 0x0)
    {
        temp = (_tree._features.maxpathnode) -> up;
        _tree._root -> value = temp -> right -> value;
        delete temp -> right;
        temp -> right = 0x0;
    }
    else
    {
        delete _tree._root;
        _tree._root = 0x0;
        return;
    }

    temp = _tree._root;
    while (temp -> left != 0x0 || temp -> right != 0x0)
    {
        typename bintree<T>::node* help;
        if (temp -> right != 0x0)
            help = Direction().compare(temp -> left -> value, temp -> right -> value) ? temp -> left : temp -> right;
        else
            help = temp -> left;

        if (Direction().compare(help -> value, temp -> value))
        {
            T help_value = help -> value;
            help -> value = temp -> value;
            temp -> value = help_value;
            temp = help;
        }
        else
            break;
    }

}

template <typename T, typename Direction>
inline void binheap<T, Direction>::set_traversal (typename bintree<T>::traversal arg)
{
    _tree.set_traversal(arg);
}

#endif // BINHEAP_IMP_H_INCLUDED
