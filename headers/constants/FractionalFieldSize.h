#pragma once

namespace Postbinary {	namespace Constants {

    class FractionalFieldSize
    {
    public:
        enum class Exponent
        {
            E_5 = 5,
            E_8 = 8,
            E_11 = 11
        };

        enum class Mantissa
        {
            M_9 = 9,
            M_21 = 21,
            M_48 = 48
        };
    };
} }