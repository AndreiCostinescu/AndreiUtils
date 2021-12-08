//
// Created by andrei on 06.12.21.
//

#include <AndreiUtils/utilsError.h>
#include <cassert>

using namespace std;

void AndreiUtils::error(const char *s) {
    perror(s);
    assert(0);
    exit(EXIT_FAILURE);
}

void AndreiUtils::error(const string &s) {
    error(s.c_str());
}