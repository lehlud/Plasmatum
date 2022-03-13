#pragma once

inline unsigned long hash_string(const char *str) {
    unsigned long hash = 5381;

    char c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    return hash;
}
