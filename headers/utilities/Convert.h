// Class provides convert methods between binary and tetralogical digits

#pragma once
#include <string>
#include "ByteOrder.h"
#include "../Constants.h"
#include "../Service.h"


namespace Postbinary { namespace Utilities {

    
    class Convert {
    public:
        static Constants::TetralogicalState boolToTetralogicalState(bool value);
    
        // Set M and A for range of two numbers: M is [0; 1] and A is [1; 0]
        static void tetritToMinimumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary);
        // Set M and A for range of two numbers: M is [0; 1] and A is [0; 1]
        static void tetritToMaximumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary);

        static std::string binaryToString(void* number, unsigned int numberOfBytes);
        static void bytesToFloat(
            float* number,
            Postbinary::byte b1,
            Postbinary::byte b2,
            Postbinary::byte b3,
            Postbinary::byte b4
        );

    private:
        static void _tetritToRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary, bool isMinimumRange);
        inline static bool _getBit(byte targetByte, unsigned int bitNumber);
    };
} }
