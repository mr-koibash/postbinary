#include "../../headers/utilities/Convert.h"

namespace Postbinary { namespace Utilities { 
    Constants::TetralogicalState Convert::boolToTetralogicalState(bool value) {
        return value ? Constants::TetralogicalState::TRUE : Constants::TetralogicalState::FALSE;
    }

    // Set M and A for range of two numbers: M is [0; 1] and A is [1; 0]
    void Convert::tetritToMinimumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary)    {
        bool isMimimumRange = true;
        Convert::_tetritToRange(tetralogicalState, leftBoundary, rightBoundary, isMimimumRange);
    }

    // Set M and A for range of two numbers: M is [0; 1] and A is [1; 0]
    void Convert::tetritToMaximumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary)    {
        bool isMimimumRange = false;
        Convert::_tetritToRange(tetralogicalState, leftBoundary, rightBoundary, isMimimumRange);
    }

#if defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
    std::string Convert::binaryToString(void* number, unsigned int numberOfBytes) {
        pointer byteOfNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number, numberOfBytes);
        int offset = Utilities::ByteOrder::isLittleEndian() ? -1 : 1;

        // casting a binary bits from fractional number (sign, exponenta, mantissa) to postbinary
        // cycle over each byte and each bit in the number
        std::string s;
        for (int byteCounter = numberOfBytes - 1; byteCounter >= 0; byteCounter--)
        {
            for (int bitCounter = (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1; bitCounter >= 0; bitCounter--)
            {
                // calculate the absolute index of the bit in the number
                int bitIdx = byteCounter * (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE + bitCounter;

                // get current bits of both numbers
                bool bitOfNumber = Convert::_getBit(*byteOfNumber, bitCounter);


                if (bitOfNumber) {
                    s.push_back('1');
                } else {
                    s.push_back('0');
                }
            }

            s.push_back(' ');
            byteOfNumber += offset;
        }

        return s;
    }
#endif

    // b1 - high order byte (e.g. sign & exponenta), b4 - low order byte (e.g. mantissa)
    void Convert::bytesToFloat(float* number, Postbinary::byte b1, Postbinary::byte b2, Postbinary::byte b3, Postbinary::byte b4) {
        int numberOfBytes = 4;

        pointer byteOfNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number, numberOfBytes);
        int offset = Utilities::ByteOrder::isLittleEndian() ? -1 : 1;

        *byteOfNumber = *byteOfNumber & 0b00000000;
        *byteOfNumber = *byteOfNumber | b1;
        byteOfNumber += offset;

        *byteOfNumber = *byteOfNumber & 0b00000000;
        *byteOfNumber = *byteOfNumber | b2;
        byteOfNumber += offset;

        *byteOfNumber = *byteOfNumber & 0b00000000;
        *byteOfNumber = *byteOfNumber | b3;
        byteOfNumber += offset;

        *byteOfNumber = *byteOfNumber & 0b00000000;
        *byteOfNumber = *byteOfNumber | b4;
        byteOfNumber += offset;
    }


    void Convert::_tetritToRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary, bool isMinimumRange) {
        switch (tetralogicalState) {
        case Constants::TetralogicalState::A:
            leftBoundary = isMinimumRange;
            rightBoundary = !isMinimumRange;
            break;
        case Constants::TetralogicalState::FALSE:
            leftBoundary = false;
            rightBoundary = false;
            break;
        case Constants::TetralogicalState::TRUE:
            leftBoundary = true;
            rightBoundary = true;
            break;
        case Constants::TetralogicalState::M:
            leftBoundary = false;
            rightBoundary = true;
            break;
        default:
            break;
        }
    }

    inline bool Convert::_getBit(byte targetByte, unsigned int bitNumber) {
        return (targetByte >> bitNumber) & 1;
    }
} }
