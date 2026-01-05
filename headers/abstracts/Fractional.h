#pragma once
#include "Number.h"

namespace Postbinary { namespace Abstracts {
    class Fractional : public Number {
    protected:
        unsigned int _sizeOfExponent;
        unsigned int _sizeOfMantissa;

    public:
        Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa);

        Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa,
            void* number1, void* number2, unsigned int numberOfBytes);

        Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa,
            void* number, unsigned int numberOfBytes);

        ~Fractional() { }

        unsigned int getSizeOfExponent();
        unsigned int getSizeOfMantissa();

    protected:
        void _getMinimumRange(void* number1, void* number2, unsigned int numberOfBytes);

        void _getMaximumRange(void* number1, void* number2, unsigned int numberOfBytes);

        void _convertFromBinaryNumber(void* number, unsigned int numberOfBytes);

        void _convertFromBinaryRange(void* number1, void* number2, unsigned int numberOfBytes);

    private:
        /// <summary>
        /// Get range from current tetralogical number and set it to two numbers. 
        /// Method is used by getMinimumRange and getMaximumRange
        /// </summary>
        /// <param name="number1">Left boundary of range</param>
        /// <param name="number2">Right boundary of range</param>
        /// <param name="numberOfBytes">Bytes number of number1/number2</param>
        /// <param name="isMimimumRange">flag for computatuion minimum or maximum range</param>
        void _getRange(void* number1, void* number2, unsigned int numberOfBytes, bool isMimimumRange);
    };
} }


