#include <iostream>
#include "persistent_set.h"
#include <gtest/gtest.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <set>

int main() {
    persistent_set<int, my_linked_ptr> q, p;
    q.insert(1);
    q.insert(2);
    q.insert(3);
    q.insert(4);

    p = q;
    p.erase(p.find(3));
    std:: cout << (q.find(3) != q.end());

    persistent_set<int> r;
    r.insert(5);
    r.insert(6);
    auto e = r.begin();
    ++e;
    std::cout << (*e) << '\n';
    e = r.end();
    --e;
    std::cout << (*e) << '\n';

    srand(time(0));
    std::vector <int> v;
    for (int i = 1; i <= 15; ++i) {
        v.push_back(i);
    }
    int tests = 0;
    for(int it = 0; it < 1000000; ++it) {
        ++tests;
        if (tests % 100000 == 0) {
            std::cout << "done : " << tests << '\n';
        }
        std::random_shuffle(v.begin(), v.end());
        persistent_set<int> s;
        for (int x : v) {
            s.insert(x);
        }

        for (int i = 1; i <= 15; ++i) {
            assert(s.find(i) != s.end());
        }

        int k = 1;
        for (auto it = s.begin(); it != s.end(); ++it) {
            assert((*it) == k);
            ++k;
        }
    }
    std::cout << "INSERT IS GOOD\n";

    tests = 0;
    for (int it = 0; it < 1000000; ++it) {
        ++tests;
        if (tests % 100000 == 0) {
            std::cout << "done : " << tests << '\n';
        }
        std::random_shuffle(v.begin(), v.end());
        std::vector<int> erase_order = v;
        std::random_shuffle(erase_order.begin(), erase_order.end());

        persistent_set<int> s;
        std::set<int> ss;

        for (int x : v) {
            s.insert(x);
            ss.insert(x);
        }

        for (int x : erase_order) {
            s.erase(s.find(x));
            ss.erase(ss.find(x));

            auto it1 = s.begin();
            auto it2 = ss.begin();
            for(; it2 != ss.end(); ++it2) {
                assert((*it1) == (*it2));
                ++it1;
            }
        }
    }
    std::cout << "ERASE IS GOOD\n";
    return 0;
}