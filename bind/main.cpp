#include <iostream>
#include <string>
#include "bind.h"
#include <assert.h>
#include <functional>

void f(int a, int b, int c)
{
    std::cout << a << b << c << std::endl;
}

int g(int a, int b)
{
    return a + b;
}

int main()
{
    bind(&f, 1, 2, 3)();
    bind(&f, _1, _3, _2)(4, 6, 5);
    bind(&f, _1, _1, _2)(7, 8);
    bind(&f, _1, bind(&g, _1, 5), _2)(2, 3);
    return 0;
}