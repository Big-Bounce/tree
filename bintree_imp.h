#ifndef BINTREE_IMP_H_INCLUDED
#define BINTREE_IMP_H_INCLUDED

#include <algorithm>
#include <stack>
#include <math.h>
#include <iostream>
#include <cassert>
#include <queue>
#include <sstream>
#include <climits>

template <typename T>
bintree<T>::description::description():
    height(0),
    maxpathnode(0x0),
    minpath(UINT_MAX),
    minpathnode(0x0),
    leafcount(0),
    onechildcount(0),
    onechildnodes(0),
    level_nodes(0)
{
}

template <typename T>
inline void bintree<T>::description::prepare()
{
    *this = bintree<T>::description();
}

template <typename T>
bintree<T>::node::node(): up(0x0), left(0x0), right(0x0), level(0)
{
}

template <typename T>
bintree<T>::node::node(node const& arg): up(0x0), left(0x0), right(0x0), level(0)
{
    value = arg.value;
}

template <typename T>
inline typename bintree<T>::node& bintree<T>::node::operator= (node const &arg)
{
    value = arg.value;
    return *this;
}

template <typename T>
bintree<T>::node::~node()
{
    if (left != 0x0)
        delete left;
    if (right != 0x0)
        delete right;
}

template <typename T>
bintree<T>::node::node(T const& val, node* parent): value(val), level(0), up(parent), left(0x0), right(0x0)
{
}

template <typename T>
void bintree<T>::node::create_children(std::vector<T>& data, node* caller)
{
    if (caller == up)
    {
        if (left == 0x0 && right == 0x0)
        {
            //if (!data.empty())
                left = new node(data.back(), this);
                data.pop_back();
            if (!data.empty())
            {
                right = new node(data.back(), this);
                data.pop_back();
            }
            if  (!data.empty())
            {
                if (up == 0x0)
                    left -> create_children(data, this);

                else
                    up -> create_children(data, this);
            }
        }
        else
            left -> create_children(data, this);

    }

    if (caller == left)
        right -> create_children(data, this);

    if (caller == right)
    {
        if (up == 0x0)
            left -> create_children(data, this);
        else
            up -> create_children(data, this);
    }
}

template <typename T>
void bintree<T>::node::show(description &arg)
{
    show_me(value);
}

template <typename T>
inline void bintree<T>::node::describe(description &arg)
{
    //node count
    if (level < arg.level_nodes.size())
        ++arg.level_nodes[level];
    else
        arg.level_nodes.push_back(1);

    //level and height
    if (left != 0x0)
        arg.height = (left -> level = level + 1) >= arg.height ? (arg.maxpathnode = left, left -> level) : arg.height;
    if (right != 0x0)
        arg.height = (right -> level = level + 1) >= arg.height ? (arg.maxpathnode = right, right -> level) : arg.height;

    //minimum path
    if (left == 0x0 && right == 0x0)
    {
        ++arg.leafcount;
        if (level < arg.minpath)
        {
            arg.minpath = level;
            arg.minpathnode = this;
        }

    }

    //one-child node count
    if ((left == 0x0 && right != 0x0) || (left != 0x0 && right == 0x0))
    {
        arg.onechildnodes.push_back(this);
        ++arg.onechildcount;
    }

}

template <typename T>
void bintree<T>::node::replace_with_child (bintree<T>::node *arg)
{
    assert(arg != 0x0); //zero node
    assert(arg == left || arg == right); //not my child
    assert (!(arg == left && right != 0x0 && arg -> right != 0x0)); //right subtree fail
    assert (!(arg == right && left != 0x0 && arg -> left != 0x0)); //left subtree fail

    if (arg == left || left == 0x0)
        left = arg -> left;
    if (arg == right || right == 0x0)
        right = arg -> right;
    value = arg -> value;
    arg -> left = 0x0;
    arg -> right = 0x0;
    delete arg;

}

template <typename T>
void bintree<T>::node::rotation (bintree<T>::node *child, bintree<T>::node **root)
{
    assert(child != 0x0); //zero node
    assert(child == left || child == right); //not my child

    if (child == left)
    {
        left = child -> right;
        if (left != 0x0)
            left -> up = this;
        child -> right = this;
    }
    else
    {
        right = child -> left;
        if (right != 0x0)
            right -> up = this;
        child -> left = this;
    }

    child -> up = up;
    up = child;
    if (*root == this)
        *root = child;
    else
    {
        if (child -> up -> left == this)
            child -> up -> left = child;
        else
            child -> up -> right = child;
    }
}

//============================================================

template <typename T>
bintree<T>::bintree(): _root(0x0), _features(), _traversal(flag_bsf)
{
}

template <typename T>
bintree<T>::bintree(bintree const &arg): _root(0x0), _features(), _traversal(flag_bsf)
{
}

template <typename T>
inline bintree<T>& bintree<T>::operator= (bintree const &arg)
{
    return *this;
}

template <typename T>
bintree<T>::~bintree()
{
    if (_root != 0x0)
        delete _root;
}

template <typename T>
bintree<T>::bintree(std::vector<T> & arg): _root(0x0), _features(), _traversal(flag_bsf)
{
    if (arg.empty())
        return;

    std::vector<T> rev(arg.size());
    reverse_copy(arg.begin(), arg.end(), rev.begin());
    _features.height = log2(rev.size());

    if (arg.size() <= REC_LIMIT)
    {
        T help = rev.back();
        _root = new node(help);
        rev.pop_back();
        if (!rev.empty())
            _root -> create_children(rev);
    }

    else
    {
        std::queue<node*> nodes;
        nodes.push(_root = new node(rev.back()));
        rev.pop_back();
        while (!rev.empty())
        {
            nodes.push(nodes.front() -> left = new node(rev.back(), nodes.front()));
            rev.pop_back();

            if (!rev.empty())
            {
                nodes.push(nodes.front() -> right = new node(rev.back(), nodes.front()));
                rev.pop_back();
            }

            nodes.pop();
        }
    }
}

template <typename T>
void bintree<T>::dsf_pre_order (process fun, node* start)
{
    BINTREE_CHECK;

    std::vector<node*> dummy (_features.height + 1);
    dummy.clear();
    std::stack<node*, std::vector<node*> > nodes (dummy);
    nodes.push(start);
    while (!nodes.empty())
    {
        start = nodes.top();
        nodes.pop();
        (start->*fun)(_features);
        if (start -> right != 0x0)
            nodes.push(start -> right);
        if (start -> left != 0x0)
            nodes.push(start -> left);
    }
}

template <typename T>
void bintree<T>::dsf_in_order (process fun, node* start)
{
    BINTREE_CHECK;

    std::vector<node*> dummy (_features.height + 1);
    dummy.clear();
    std::stack<node*, std::vector<node*> > nodes (dummy);
    while (!nodes.empty() || start != 0x0)
    {
        if (start != 0x0)
        {
            nodes.push(start);
            start = start -> left;
            continue;
        }

        else
        {
            start = nodes.top();
            nodes.pop();
            (start->*fun)(_features);
            start = start -> right;
        }
    }
}

template <typename T>
void bintree<T>::dsf_post_order (process fun, node* start)
{
    BINTREE_CHECK;

    std::vector<node*> dummy (_features.height + 1);
    dummy.clear();
    std::stack<node*, std::vector<node*> > nodes (dummy);
    node* previous = 0x0;
    nodes.push(start);
    while (!nodes.empty())
    {
        start = nodes.top();

        if (previous == 0x0 || previous -> left == start || previous -> right == start)
        {
            if (start -> left != 0x0)
                nodes.push(start -> left);
            else if (start -> right != 0x0)
                nodes.push(start -> right);
            previous = start;
        }

        else if (start -> left == previous && start -> right != 0x0)
        {
            nodes.push(start -> right);
            previous = start;
        }

        else
        {
            (start->*fun)(_features);
            nodes.pop();
            previous = start;
        }


    }
}

template <typename T>
void bintree<T>::bsf(process fun, node* start)
{
    BINTREE_CHECK;

    std::queue<node*> nodes;
    nodes.push(start);
    while (!nodes.empty())
    {
        start = nodes.front();
        nodes.pop();
        (start->*fun)(_features);
        if (start -> left != 0x0)
            nodes.push(start -> left);
        if (start -> right != 0x0)
            nodes.push(start -> right);
    }
}

template <typename T>
void bintree<T>::show()
{
    if (_root == 0x0)
        return;

    switch (_traversal)
   {
      case flag_dsf_pre_order: dsf_pre_order(node::show); break;
      case flag_dsf_in_order: dsf_in_order(node::show); break;
      case flag_dsf_post_order: dsf_post_order(node::show); break;
      case flag_bsf: bsf(node::show); break;
      default: flag_dsf_pre_order: dsf_pre_order(node::show);
   }

   std::cout<<std::endl;

}

template <typename T>
std::string& bintree<T>::describe(std::string& arg)
{
    arg.clear();
    _features.prepare();
    if (_root != 0x0)
        switch (_traversal)
        {
            case flag_dsf_pre_order: dsf_pre_order(node::describe); break;
            case flag_dsf_in_order: dsf_in_order(node::describe); break;
            case flag_dsf_post_order: dsf_post_order(node::describe); break;
            case flag_bsf: bsf(node::describe); break;
            default: flag_dsf_pre_order: dsf_pre_order(node::describe);
        }

    std::ostringstream result;

    arg += "\nHeight: ";
    result << _features.height;
    arg += result.str();
    result.str(std::string());

    arg += "\nMinimum path: ";
    result << _features.minpath;
    arg += result.str();
    result.str(std::string());

    arg += "\nNumber of leaves: ";
    result << _features.leafcount;
    arg += result.str();
    result.str(std::string());

    arg += "\nNumber of one-child nodes: ";
    result << _features.onechildcount;
    arg += result.str();
    result.str(std::string());

    arg += "\nNumber of individual levels' nodes:";
    for (std::vector<unsigned>::size_type i = 0; i < _features.level_nodes.size(); ++i)
    {
        arg += result.str() += "\n\tlevel ";
        result << i;
        arg += result.str();
        result.str(std::string());

        arg += ": ";
        result << _features.level_nodes[i];
        arg += result.str();
        result.str(std::string());
    }

    arg += "\n\n";

    return arg;
}

template <typename T>
inline void bintree<T>::set_traversal (traversal arg)
{
    _traversal = arg;
}

template <typename T>
void bintree<T>::describe()
{
    _features.prepare();
    if (_root != 0x0)
        switch (_traversal)
        {
            case flag_dsf_pre_order: dsf_pre_order(node::describe); break;
            case flag_dsf_in_order: dsf_in_order(node::describe); break;
            case flag_dsf_post_order: dsf_post_order(node::describe); break;
            case flag_bsf: bsf(node::describe); break;
            default: flag_dsf_pre_order: dsf_pre_order(node::describe);
        }
}

#endif // BINTREE_IMP_H_INCLUDED
