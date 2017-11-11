#include "big_integer.h"
#include <algorithm>
#include <stdexcept>
#include <math.h>
#include <string>

typedef unsigned int ui;
typedef unsigned long long ull;
typedef long long ll;

//using namespace std;

const ll BASE = 32;
const ll MAX_DIGIT = 1ll << BASE;
const ull base_10 = 10;

template <typename T>
bool is_neg(T x) {
    return x < 0;
}

vector <ull> to_number(std::string const& str) {
    vector <ull> ret;
    int left;
    if(str[0] == '-') {
        left = 1;
    } else {
        left = 0;
    }
    for(size_t ii = str.size() ; ii != (size_t) left ; --ii) {
        size_t i = ii - 1;
        if(str[i] < '0' || str[i] > '9') {
            throw std::runtime_error("incorrect char");
        }
        ret.push_back(str[i] - '0');
    }
    return ret;
}

ui get_digit_from_vector(vector<ull>& data, ull base) {
    if (base == 0) {
        throw std::runtime_error("base equals zero");
    }
    ull carry = 0;
    for (size_t ii = data.size() ; ii != 0 ; --ii) {
        size_t i = ii - 1;
        ull cur = data[i] + carry * base_10;
        data[i] = cur / base;
        carry = cur % base;
    }
    while (data.size() > 0 && data.back() == 0) {
        data.pop_back();
    }
    return static_cast<ui>(carry);
}

big_integer big_integer::absolute_value() const {
    return is_negative() ? -*this : *this;
}

std::string to_string(big_integer const& arg) {
    std::string result = "";
    big_integer abs_arg = arg.absolute_value();
    bool is_negat = arg.is_negative();
    while (abs_arg.length() > 0) {
        const int block = (int) 1e9;
        const int pow_10_block = 9;
        int t = abs_arg % block;
        abs_arg = abs_arg / block;
        for (int i = 0; i < pow_10_block; i++) {
            result.push_back('0' + t % 10);
            t /= 10;
        }
    }
    while (result.size() > 1 && result.back() == '0') {
        result.pop_back();
    }
    if (result.empty()) {
        result.push_back('0');
    }
    if (is_negat) {
        result.push_back('-');
    }
    for (size_t i = 0; i < result.size() / 2; i++) {
    	std::swap(result[i], result[result.size() - 1 - i]);
    }
    return result;
}

big_integer::big_integer() {
    sign = false;
}

big_integer::big_integer(big_integer const& other) {
    data = other.data;
    sign = other.sign;
}

big_integer::big_integer(int a) {
    if (a == 0) {
        sign = false;
    }
    else {
        sign = is_neg(a);
        data = vector<ui>(1, a & (MAX_DIGIT - 1));
        normalize();
    }
}

big_integer::big_integer(ui a) {
    sign = false;
    data = vector<ui>(1, a);
}

big_integer::big_integer(bool new_sign, vector<ui> const& new_data) : sign(new_sign), data(new_data) {
    normalize();
}

big_integer::big_integer(std::string const& str) {
    if (str.size() == 0) {
        throw std::runtime_error("empty string");
    }
    bool is_negat = (str[0] == '-');
    vector<ull> temp = to_number(str);
    vector<ui> temp_data;
    while (temp.size() > 0) {
        temp_data.push_back(get_digit_from_vector(temp, MAX_DIGIT));
    }
    sign = false;
    data = temp_data;
    set_sign(is_negat);
    normalize();
}

void big_integer::set_sign(bool s) {
    if (s != is_negative()) {
        *this = -*this;
    }
}

big_integer& big_integer::operator=(big_integer const& other) {
    big_integer temp(other);
    swap(temp);
    return *this;
}

size_t big_integer::length() const {
    return data.size();
}

void big_integer::swap(big_integer& other) noexcept {
    std::swap(sign, other.sign);
    std::swap(data, other.data);
}

int big_integer::get_sign() const {
    return sign ? -1 : 1;
}

ui big_integer::get_digit(size_t i) const {
    return data[i];
}

ui big_integer::get_inf_digit(size_t i) const {
    if (i < length()) {
        return get_digit(i);
    }
    if (get_sign() == -1) {
        return MAX_DIGIT - 1;
    }
    return 0;
}

void big_integer::normalize() {
    while (data.size() > 0 && ((get_sign() == -1 && data.back() == MAX_DIGIT - 1) || (get_sign() == 1 && data.back() == 0))) {
        data.pop_back();
    }
}

big_integer operator+(big_integer const& first, big_integer const& second) {
    ull carry = 0;
    size_t size = std::max(first.length(), second.length()) + 2;
    vector<ui> temp(size);
    for (size_t i = 0; i < size; i++) {
        ull val = carry + first.get_inf_digit(i) + second.get_inf_digit(i);
        carry = val >> BASE;
        temp[i] = val & (MAX_DIGIT - 1);
    }
    bool sgn = ((temp[size - 1] >> (BASE - 1)) > 0);
    return big_integer(sgn, temp);
}

bool big_integer::is_negative() const {
    return sign;
}

big_integer operator~(big_integer const& first) {
    size_t size = first.length();
    vector<ui> temp(size);
    for (size_t i = 0; i < size; i++) {
        temp[i] = ~first.get_inf_digit(i) & static_cast<ui>(MAX_DIGIT - 1);
    }
    return big_integer(!first.is_negative(), temp);
}

big_integer operator-(big_integer const& first) {
    return (~first) + 1;
}

big_integer operator-(big_integer const& first, big_integer const& second) {
    return first + (-second);
}

big_integer operator+(big_integer const& first) {
    return big_integer(first);
}

big_integer operator++(big_integer& first) {
    big_integer temp = first + 1;
    first.swap(temp);
    return temp;
}

int operator%(big_integer const& first, int second) {
    if (second == 0) {
        throw std::runtime_error("mod by zero");
    }
    big_integer abs_first(first.absolute_value());
    size_t size = first.length();
    int carry = 0;
    for (size_t ii = size ; ii != 0; ii--) {
        size_t i = ii - 1;
        ull cur = abs_first.get_digit(i) + carry * MAX_DIGIT;
        carry = static_cast<int>(cur % second);
    }
    return carry;
}

big_integer operator*(big_integer const& first, big_integer const& second) {
    bool sign = first.is_negative() ^ second.is_negative();

    big_integer abs_first(first.absolute_value());
    big_integer abs_second(second.absolute_value());

    if (abs_first.length() > abs_second.length()) {
        abs_first.swap(abs_second);
    }

    vector<ui> temp(abs_first.length() + abs_second.length() + 1);
    for (size_t i = 0; i < abs_first.length(); i++) {
        ull carry = 0;
        for (size_t j = 0; j < abs_second.length(); j++) {
            size_t k = i + j;
            ull tmp = static_cast<ull>(abs_first.get_digit(i)) * abs_second.get_digit(j);
            ull e = temp[k] + (tmp & (MAX_DIGIT-1)) + carry;
            temp[k] = e & (MAX_DIGIT - 1);
            carry = (tmp >> BASE) + (e >> BASE);
        }
        if (carry != 0) {
            temp[i + abs_second.length()] += static_cast<ui>(carry);
        }
    }
    big_integer result(false, temp);
    result.set_sign(sign);
    return result;
}

big_integer get_higher_digits(big_integer const& number, size_t n) {
    vector<ui> temp(n);
    for (size_t i = 0; i < temp.size(); i++) {
        if (i < number.length()) {
            temp[temp.size() - i - 1] = number.get_digit(number.length() - i - 1);
        }
    }
    return big_integer(false, temp);
}

ui get_next_digit(big_integer const& first, big_integer const& second) {
    if (second == 0) {
        throw std::runtime_error("division by zero");
    }
    big_integer abs_first = get_higher_digits(first, 4);
    big_integer abs_second = get_higher_digits(second, 3);
    if (abs_first < abs_second) {
        return 0;
    }
    abs_second = abs_second << BASE;
    for (ui i = 0; i <= 1; i++) {
        ui dig = 0;
        if (abs_first >= abs_second) {
            for (ui jj = BASE ; jj != 0; jj--) {
                ui j = jj - 1;
                ui nw = dig + (1ll << j);
                if (nw * abs_second <= abs_first) {
                    dig = nw;
                }
            }
            abs_first = abs_first - dig * abs_second;
        }
        if (dig > 0) {
            return dig;
        }
        abs_second = abs_second >> BASE;
    }
    return 0;
}

big_integer operator/(big_integer const& first, big_integer const& second) {
    if (second == 0) {
        throw std::runtime_error("division by zero");
    }
    bool sign = first.is_negative() ^ second.is_negative();
    big_integer abs_first(first.absolute_value());
    big_integer abs_second(second.absolute_value());
    if (abs_first < abs_second) {
        return big_integer(0);
    }
    if (abs_second.length() == 1) {
        big_integer result = abs_first / abs_second.get_digit(0);
        if (sign) {
            result = -result;
        }
        return result;
    }
    size_t t = abs_first.length() - abs_second.length();
    if (abs_first < abs_second << static_cast<ui>(t) * BASE) {
        t--;
    }
    vector<ui> temp;
    for (size_t ii = t + 1; ii != 0; ii--) {
        size_t i = ii - 1;
        ui dig = get_next_digit(abs_first, abs_second);
        big_integer tmp = dig * abs_second;
        ll carry = 0;
        for (size_t j = i; j < abs_first.length(); j++) {
            ll res = carry + abs_first.data[j];
            if (j - i < tmp.length()) {
                res -= tmp.data[j - i];
            }
            if (res < 0) {
                carry = -1;
                res += MAX_DIGIT;
            }
            else {
                carry = 0;
            }
            abs_first.data[j] = static_cast<ui>(res);
        }
        abs_first.normalize();
        temp.push_back(dig);
    }
    for (size_t i = 0; i < temp.size() / 2; i++) {
    	std::swap(temp[i], temp[temp.size() - 1 - i]);
    }
    big_integer result(false, temp);
    result.set_sign(sign);
    return result;
}

big_integer operator%(big_integer const& first, big_integer const& second) {
    return first - (first / second) * second;
}

bool operator==(big_integer const& a, big_integer const& b) {
    return a.data == b.data && a.sign == b.sign;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& first, big_integer const& second) {
    if (first.get_sign() != second.get_sign()) {
        return first.get_sign() < second.get_sign();
    }
    if (first.length() != second.length()) {
        return first.length() < second.length();
    }
    for (size_t ii = first.length() ; ii != 0 ; ii--) {
        size_t i = ii - 1;
        if (first.get_digit(i) != second.get_digit(i)) {
            return first.get_digit(i) < second.get_digit(i);
        }
    }
    return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
    return b < a;
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    big_integer T = *this + rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    big_integer T = *this - rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    big_integer T = *this * rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    big_integer T = *this / rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    big_integer T = *this % rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator/=(int rhs) {
    big_integer T = *this / rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator%=(int rhs) {
    big_integer T = *this % rhs;
    swap(T);
    return *this;
}

template <typename E>
big_integer div_big_by_small(big_integer const& first, E second) {
    if (second == 0) {
        throw std::runtime_error("division by zero");
    }
    big_integer abs_first(first.absolute_value());
    bool is_negat = first.is_negative() ^ is_neg(second);
    ull div = abs(static_cast<ll>(second));
    size_t size = first.length();
    vector<ui> temp(size);
    ull carry = 0;
    for (size_t ii = size ; ii != 0; ii--) {
        size_t i = ii - 1;
        ull cur = abs_first.get_digit(i) + carry * MAX_DIGIT;
        temp[i] = static_cast<ui>(cur / div);
        carry = cur % div;
    }
    big_integer T(false, temp);
    T.set_sign(is_negat);
    return T;
}

big_integer operator/(big_integer const& first, int second) {
    return div_big_by_small<int>(first, second);
}

big_integer operator/(big_integer const& first, ui second) {
    return div_big_by_small<ui>(first, second);
}

enum type_bit_operation {
    XOR,
    OR,
    AND
};

template <typename T>
T do_bit_operation(T a, T b, type_bit_operation type) {
    switch (type) {
        case XOR:
            return a ^ b;
        case OR:
            return a | b;
        case AND:
            return a & b;
    }
    return 0;
}

big_integer bit_operation(big_integer const& first, big_integer const& second, type_bit_operation type) {
    size_t size = std::max(first.length(), second.length());
    vector<ui> temp(size);
    for (size_t i = 0; i < size; i++) {
        temp[i] = do_bit_operation(first.get_inf_digit(i), second.get_inf_digit(i), type);
    }
    return big_integer(do_bit_operation(first.is_negative(), second.is_negative(), type), temp);
}

big_integer operator^(big_integer const& first, big_integer const& second) {
    return bit_operation(first, second, XOR);
}

big_integer operator|(big_integer const& first, big_integer const& second) {
    return bit_operation(first, second, OR);
}

big_integer operator&(big_integer const& first, big_integer const& second) {
    return bit_operation(first, second, AND);
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
    big_integer temp = *this & rhs;
    swap(temp);
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
    big_integer temp = *this | rhs;
    swap(temp);
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
    big_integer temp = *this ^ rhs;
    swap(temp);
    return *this;
}

big_integer operator>>(big_integer const& first, ui shift) {
    if (shift == 0) {
        return big_integer(first);
    }
    ull mod = shift % BASE;
    ull div = shift / BASE;
    size_t size = div < first.length() ? first.length() - div : 0;
    vector<ui> temp(size);
    for (size_t i = 0; i < size; i++) {
        temp[i] = (static_cast<ull>(first.get_inf_digit(i + div)) >> mod) | 
		  (static_cast<ull>(first.get_inf_digit(i + div + 1)) << (BASE - mod));
    }
    return big_integer(first.is_negative(), temp);
}

big_integer operator<<(big_integer const& first, ui shift) {
    if (shift == 0) {
        return big_integer(first);
    }
    ull mod = shift % BASE;
    ull div = shift / BASE;
    size_t size = first.length() + div + 1;
    vector<ui> temp(size);
    temp[div] = (first.get_inf_digit(0) << mod);
    for (size_t i = div; i < size; i++) {
        temp[i] = (first.get_inf_digit(i - div) << mod)
                  | static_cast<ull>(first.get_inf_digit(i - div - 1)) >> (BASE - mod);
    }
    return big_integer(first.is_negative(), temp);
}

big_integer& big_integer::operator<<=(int rhs) {
    big_integer T = *this << rhs;
    swap(T);
    return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
    big_integer T = *this >> rhs;
    swap(T);
    return *this;
}

