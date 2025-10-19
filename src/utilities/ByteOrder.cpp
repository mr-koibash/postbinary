#include "../../headers/utilities/ByteOrder.h"

namespace Postbinary { namespace Utilities {
    /// <summary>
    /// Select high order byte of the numbers depending on the little-endian/big-endian of arhitecture
    /// </summary>
    /// <param name="number">Pointer to target number</param>
    /// <param name="numberOfBytes">Size of the number</param>
    /// <returns>Pointer to the high order byte</returns>
    pointer ByteOrder::getHighOrderByteInNumber(void* number, unsigned int numberOfBytes) {
        return ByteOrder::isLittleEndian() ? (pointer)number + (numberOfBytes - 1) : (pointer)number;
    }
    
    // check byte order in memory: big endian or little endian
    bool ByteOrder::isLittleEndian() {
        unsigned short x = 0x0001;
        return *((unsigned char*)&x);
    }
} }
