#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "vector.h"

struct big_integer {

    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(unsigned int a);
    explicit big_integer(bool new_sign, vector<unsigned int> const &new_data);
    explicit big_integer(std :: string const& str);


    big_integer& operator=(big_integer const& other);


    friend big_integer operator-(big_integer const&, big_integer const&);
    friend big_integer operator+(big_integer const&, big_integer const&);
    friend big_integer operator*(big_integer const&, big_integer const&);
    friend big_integer operator/(big_integer const&, big_integer const&);
    friend big_integer operator%(big_integer const&, big_integer const&);
    friend int operator%(big_integer const&, int);

    friend big_integer operator/(big_integer const&, int);
    friend big_integer operator/(big_integer const&, unsigned int);



    friend big_integer operator&(big_integer const&, big_integer const&);
    friend big_integer operator|(big_integer const&, big_integer const&);
    friend big_integer operator^(big_integer const&, big_integer const&);


    friend big_integer operator-(big_integer const&);
    friend big_integer operator+(big_integer const&);
    friend big_integer operator~(big_integer const&);
    friend big_integer operator++(big_integer&);


    friend big_integer operator<<(big_integer const&, unsigned int);
    friend big_integer operator>>(big_integer const&, unsigned int);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);


    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);


    big_integer& operator/=(int rhs);
    big_integer& operator%=(int rhs);


    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    friend std::string to_string(big_integer const& arg);
    void swap(big_integer& other) noexcept;
    bool is_negative() const;
    void set_sign(bool s);
    size_t length() const;
    int get_sign() const;
    unsigned int get_digit(size_t i) const;
    unsigned int get_inf_digit(size_t i) const;
    big_integer absolute_value() const;

private:
    bool sign;
    vector <unsigned int> data;
    void normalize();

};

#endif
