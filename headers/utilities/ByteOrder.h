// Class provides methods bound with byte order in different computer arhitectures
// i.e. Intel has reversal byte order (high order byte situated at the end of number)

#pragma once
#include "../Constants.h"
#include "../Service.h"

namespace Postbinary { namespace Utilities {

    class ByteOrder
    {
    public:
        /// <summary>
        /// Select high order byte of the numbers depending on the little-endian/big-endian of arhitecture
        /// </summary>
        /// <param name="number">Pointer to target number</param>
        /// <param name="numberOfBytes">Size of the number</param>
        /// <returns>Pointer to the high order byte</returns>
        static pointer getHighOrderByteInNumber(void* number, unsigned int numberOfBytes);
    
        // check byte order in memory: big endian or little endian
        static bool isLittleEndian();
    };
} }