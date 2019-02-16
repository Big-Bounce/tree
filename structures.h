#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define COMPARISION_ACCURACY 0.001
#include <cmath>
#include <string>

template <typename T>
struct one_value
{
    bool is_value;
    T first;
    one_value(): is_value(false), first(T()) {}
    one_value(T const& arg1): is_value(true), first(arg1) {}
    ~one_value(){}
};

template <typename T>
struct two_values
{
    bool is_value;
    T first;
    T second;
    two_values(): is_value(false), first(T()), second(T()) {}
    two_values(T const& arg1, T const& arg2): is_value(true), first(arg1), second(arg2) {}
    ~two_values(){}
};

//======================================================
//PRIORITY QUEUE'S DATA TYPE AND OUTPUT OPERATOR

template <typename T, typename U>
struct different_values
{
    T first;
    U second;
    different_values(): first(T()), second(U()) {}
    different_values(T const& arg1, U const& arg2): first(arg1), second(arg2) {}
};

template <typename T, typename U>
std::ostream& operator<< (std::ostream& stream, different_values<T,U> const &arg)
{
    return stream << "@Data: " << arg.first << " Priority: " << arg.second << "@";
}

//======================================================
//COMPARISION TYPES

struct eq
{
    eq(){}
    template <typename T>
        bool compare(T const& arg1, T const& arg2)
        {return arg1 == arg2;}
    ~eq(){}
};

template <>
bool eq::compare<double> (double const& arg1, double const& arg2)
{
    return fabs(arg1 - arg2) < COMPARISION_ACCURACY;
}

struct asc
{
    asc(){}
    template <typename T>
        bool compare(T const& arg1, T const& arg2)
        {return arg1 < arg2;}
    template <typename T, typename U>
        bool compare(different_values<T,U> const& arg1, different_values<T,U> const& arg2)
        {return compare(arg1.second, arg2.second);}
    ~asc(){}
};

template<>
bool asc::compare(std::string const& arg1, std::string const& arg2)
{
    for (typename std::string::size_type i = 0; i < arg1.size() && i < arg2.size(); ++i)
    {
        if (arg1[i] < arg2[i])
            return true;
        if (arg1[i] > arg2[i])
            return false;
    }

    if (arg1.size() < arg2.size())
        return true;
    return false;
}

struct dsc
{
    dsc(){}
    template <typename T>
        bool compare(T const& arg1, T const& arg2)
        {return arg1 > arg2;}
    template <typename T, typename U>
        bool compare(different_values<T,U> const& arg1, different_values<T,U> const& arg2)
        {return compare(arg1.second, arg2.second);}
    ~dsc(){}
};

template<>
bool dsc::compare(std::string const& arg1, std::string const& arg2)
{
    for (typename std::string::size_type i = 0; i < arg1.size() && i < arg2.size(); ++i)
    {
        if (arg1[i] > arg2[i])
            return true;
        if (arg1[i] < arg2[i])
            return false;
    }

    if (arg1.size() > arg2.size())
        return true;
    return false;
}

#endif // STRUCTURES_H_INCLUDED
