#ifndef BINTREE_H_INCLUDED
#define BINTREE_H_INCLUDED

#define REC_LIMIT 20
#define BINTREE_CHECK if (start == 0x0) start = _root; if (start == 0x0) return

#include <vector>
#include <string>

template <typename T>
class binheap;

template <typename T>
class bsttree;

template <typename T>
class bintree
{
    friend class binheap<T>;
    friend class bsttree<T>;
    struct node;

    struct description
    {
        unsigned height;
        node* maxpathnode;
        unsigned minpath;
        node* minpathnode;
        unsigned leafcount;
        unsigned onechildcount;
        std::vector<node*> onechildnodes;
        std::vector<unsigned> level_nodes;

        description();
        void prepare();
    };

    struct node
    {
        T value;
        unsigned level;
        node* up;
        node* left;
        node* right;

        node();
        node(node const& arg);
        node& operator= (node const& arg);
        ~node();

        node(T const& val, node* parent = 0x0);
        void create_children (std::vector<T> &data, node *caller = 0x0);
        void show(description &arg);
        void describe(description &arg);
    };

public:
    //for choosing traversal method
    enum traversal
    {
        flag_dsf_pre_order = 1,
        flag_dsf_in_order = 2,
        flag_dsf_post_order = 3,
        flag_bsf = 4
    };

private:

    node* _root;
    description _features;
    traversal _traversal;

    //aliases
    typedef void (node::*process) (description&);

    //traversal methods
    void dsf_pre_order(process fun, node* start = 0x0);
    void dsf_in_order(process fun, node* start = 0x0);
    void dsf_post_order(process fun, node* start = 0x0);
    void bsf(process fun, node* start = 0x0);

public:
    bintree();
    bintree(bintree const& arg);
    bintree& operator= (bintree const& arg);
    ~bintree();

    //working constructors
    bintree(std::vector<T> &arg);

    //functional methods
    void show();
    std::string& describe(std::string& arg);

    //setting traversal type
    void set_traversal (traversal arg);

};

#include "bintree_imp.h"

#endif // BINTREE_H_INCLUDED
