#include <iostream>
#include "debug_set.h"

int main() {



    debug_set<int> t;
    t.insert(6);
    t.insert(7);
    t.insert(8);

    debug_set<int> s;
    s = t;
    std::cout << *s.begin();
    return 0;
}