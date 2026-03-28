#pragma once
#include "../abstracts/Number.h"


namespace Postbinary {
    class Package : public Abstracts::Number {
    public:
        Package(unsigned int numberOfBytes);
        ~Package() { }
    };
}
