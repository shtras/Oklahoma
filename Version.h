#pragma once

namespace Version {
#include "hash.txt"

    enum Version
    {
        MAJOR = 0,
        MINOR = 1,
        REVISION =
#include "rev.txt"
    };
}
