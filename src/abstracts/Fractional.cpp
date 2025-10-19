#include "../../headers/abstracts/Fractional.h"

namespace Postbinary { namespace Abstracts {
    Fractional::Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa) :
        Number(tetritness), _sizeOfExponent(sizeOfExponent), _sizeOfMantissa(sizeOfMantissa) {

    }

    Fractional::Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa,
        void* number1, void* number2, unsigned int numberOfBytes) :
        Number(tetritness), _sizeOfExponent(sizeOfExponent), _sizeOfMantissa(sizeOfMantissa) {
            this->_convertFromBinaryRange(number1, number2, numberOfBytes);
    }

    Fractional::Fractional(Constants::TetralogicalDigitCapacity tetritness, unsigned int sizeOfExponent, unsigned int sizeOfMantissa,
        void* number, unsigned int numberOfBytes) :
        Number(tetritness), _sizeOfExponent(sizeOfExponent), _sizeOfMantissa(sizeOfMantissa)  {
            this->_convertFromBinaryNumber(number, numberOfBytes);
    }

    unsigned int Fractional::getSizeOfExponent() {
        return this->_sizeOfExponent;
    }

    unsigned int Fractional::getSizeOfMantissa() {
        return this->_sizeOfMantissa;
    }


    void Fractional::_getMinimumRange(void* number1, void* number2, unsigned int numberOfBytes) {
        bool isMimimumRange = true;
        this->_getRange(number1, number2, numberOfBytes, isMimimumRange);
    }

    void Fractional::_getMaximumRange(void* number1, void* number2, unsigned int numberOfBytes) {
        bool isMimimumRange = false;
        this->_getRange(number1, number2, numberOfBytes, isMimimumRange);
    }

    // number1 is the left boundary and number2 is the right boundary of range [number1; number2]
    void Fractional::_getRange(void* number1, void* number2, unsigned int numberOfBytes, bool isMimimumRange) {
        // ----- loop variables begin ----- 
        // select first bytes of both numbers
        pointer byteOfFirstNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number1, numberOfBytes);
        pointer byteOfSecondNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number2, numberOfBytes);
        int offset = Utilities::ByteOrder::isLittleEndian() ? -1 : 1;

        unsigned int exponentCounter = this->_sizeOfExponent;
        unsigned int mantissaCounter = this->_sizeOfMantissa;
        // ----- loop variables end ----- 

        // loop over each byte and each bit in the number
        for (int byteCounter = numberOfBytes - 1; byteCounter >= 0; byteCounter--) {
            for (int bitCounter = (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1; bitCounter >= 0; bitCounter--) {
                // calculate the absolute index of the bit in the number
                int bitIdx = byteCounter * (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE + bitCounter;

                Constants::TetralogicalState tetrit = this->_getTetrit(bitIdx);

                // sign block
                // sign bit is first byte and 7th bit
                bool isSignBit = byteCounter == numberOfBytes - 1 && bitCounter == (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1;
                if (isSignBit) {
                    // calculate bits depending on tetralogical state and minimum/maximum range
                    bool leftBoundary, rightBoundary;
                    isMimimumRange ? Utilities::Convert::tetritToMinimumRange(tetrit, leftBoundary, rightBoundary) :
                        Utilities::Convert::tetritToMaximumRange(tetrit, leftBoundary, rightBoundary);

                    this->_setBit(*byteOfFirstNumber, bitCounter, leftBoundary);
                    this->_setBit(*byteOfSecondNumber, bitCounter, rightBoundary);

                    continue;
                }

                // exponenta block: M -> [0; 1] and A -> [1; 0]
                if (exponentCounter > 0) {
                    // calculate bits depending on tetralogical state and minimum/maximum range
                    bool leftBoundary, rightBoundary;
                    isMimimumRange ? Utilities::Convert::tetritToMinimumRange(tetrit, leftBoundary, rightBoundary) :
                        Utilities::Convert::tetritToMaximumRange(tetrit, leftBoundary, rightBoundary);

                    this->_setBit(*byteOfFirstNumber, bitCounter, leftBoundary);
                    this->_setBit(*byteOfSecondNumber, bitCounter, rightBoundary);

                    exponentCounter--;
                    continue;
                }

                // mantissa block: M -> [0; 1] and A -> [0; 1]
                if (mantissaCounter > 0) {
                    // calculate bits depending on tetralogical state and minimum/maximum range
                    bool leftBoundary, rightBoundary;
                    isMimimumRange ? Utilities::Convert::tetritToMinimumRange(tetrit, leftBoundary, rightBoundary) :
                        Utilities::Convert::tetritToMaximumRange(tetrit, leftBoundary, rightBoundary);

                    this->_setBit(*byteOfFirstNumber, bitCounter, leftBoundary);
                    this->_setBit(*byteOfSecondNumber, bitCounter, rightBoundary);

                    mantissaCounter--;
                    continue;
                }
            }
            byteOfFirstNumber += offset;
            byteOfSecondNumber += offset;
        }
    }

    void Fractional::_convertFromBinaryNumber(void* number, unsigned int numberOfBytes) {
        this->_convertFromBinaryRange(number, number, numberOfBytes);
    }

    void Fractional::_convertFromBinaryRange(void* number1, void* number2, unsigned int numberOfBytes) {
        // ----- loop variables begin ----- 
        // select first bytes of both numbers
        pointer byteOfFirstNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number1, numberOfBytes);
        pointer byteOfSecondNumber = Utilities::ByteOrder::getHighOrderByteInNumber(number2, numberOfBytes);
        int offset = Utilities::ByteOrder::isLittleEndian() ? -1 : 1;

        unsigned int exponentCounter = this->_sizeOfExponent;
        unsigned int mantissaCounter = this->_sizeOfMantissa;

        // set true if difference between two exponent/mantissa in numbers was found
        bool isExponentAlreadyDifferent = false;
        bool isMantissaAlreadyDifferent = false;

        // interval validity conditions
        bool isLowNumberSerieHasTrue = false;
        bool isHighNumberSerieHasFalse = false;
        bool isSerieMCoincidenceFound = false;

        // there is a neccessary of the second M tetrit state after first, where difference was found
        bool isSecondMRequired = true;

        // series of M at the mantissa: if first difference between bits was found, set M serie while bit pair not equal 
        bool isMSerieEnd = false;
        // ----- loop variables end ----- 

        // casting a binary bits from fractional number (sign, exponenta, mantissa) to postbinary
        // cycle over each byte and each bit in the number
        for (int byteCounter = numberOfBytes - 1; byteCounter >= 0; byteCounter--) {
            for (int bitCounter = (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1; bitCounter >= 0; bitCounter--) {
                // calculate the absolute index of the bit in the number
                int bitIdx = byteCounter * (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE + bitCounter;

                // get current bits of both numbers
                bool bitOfFirstNumber = this->_getBit(*byteOfFirstNumber, bitCounter);
                bool bitOfSecondNumber = this->_getBit(*byteOfSecondNumber, bitCounter);

                // check bits match of two numbers
                bool isBitDifferent = bitOfFirstNumber ^ bitOfSecondNumber;

                // sign block
                // sign bit is first byte and 7th bit
                bool isSignBit = byteCounter == numberOfBytes - 1 && bitCounter == (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1;
                if (isSignBit) {
                    // set M if different or cast numbers bit to postbinary tetrit
                    Constants::TetralogicalState tetralogicalState = 
                        isBitDifferent ? Constants::TetralogicalState::M : Utilities::Convert::boolToTetralogicalState(bitOfFirstNumber);
                    this->_setTetrit(bitIdx, tetralogicalState);
                    continue;
                }

                // exponenta block
                if (exponentCounter > 0) {
                    exponentCounter--;

                    // set all rest exponenta part to AAA... if the difference was found
                    if (isExponentAlreadyDifferent) {
                        this->_setTetrit(bitIdx, Constants::TetralogicalState::A);
                        continue;
                    }

                    isExponentAlreadyDifferent = isBitDifferent;
                    Constants::TetralogicalState tetralogicalState = isBitDifferent ? Constants::TetralogicalState::M : Utilities::Convert::boolToTetralogicalState(bitOfFirstNumber);
                    this->_setTetrit(bitIdx, tetralogicalState);
                    continue;
                }

                // mantissa block
                if (mantissaCounter > 0) {
                    mantissaCounter--;

                    // set all rest exponenta part to AAA... if the difference was found
                    if (isMantissaAlreadyDifferent) {


                        if (!isBitDifferent) {
                            isSerieMCoincidenceFound = true;
                        }

                        // if (isSecondMRequired) {
                        //     // set second M after first M tetrit
                        //     this->_setTetrit(bitIdx, Constants::TetralogicalState::M);
                        //     isSecondMRequired = false;
                        //     continue;
                        // }

                        // set M while logical states of numbers is different
                        bool isConditionMet = isLowNumberSerieHasTrue && isHighNumberSerieHasFalse;
                        if (!isConditionMet || !isSerieMCoincidenceFound) {
                            this->_setTetrit(bitIdx, Constants::TetralogicalState::M);

                            // low number M serie should have bit "1"
                            if (bitOfFirstNumber) {
                                isLowNumberSerieHasTrue = true;
                            }
                            // high number M serie should have bit "0"
                            if (!bitOfSecondNumber) {
                                isHighNumberSerieHasFalse = true;
                            }

                            continue;

                            // else
                            // {
                            //     isMSerieEnd = true;
                            // }
                        }






                        // if (!isMSerieEnd)
                        // {
                        //     if (isBitDifferent || !(isLowNumberSerieHasTrue && isHighNumberSerieHasFalse))
                        //     {
                        //         this->_setTetrit(bitIdx, Constants::TetralogicalState::M);
                        //         continue;
                        //     }
                        //     else
                        //     {
                        //         isMSerieEnd = true;
                        //     }
                        // }


                        this->_setTetrit(bitIdx, Constants::TetralogicalState::A);
                        continue;
                    }

                    isMantissaAlreadyDifferent = isBitDifferent;
                    Constants::TetralogicalState tetralogicalState = isBitDifferent ? Constants::TetralogicalState::M : Utilities::Convert::boolToTetralogicalState(bitOfFirstNumber);
                    this->_setTetrit(bitIdx, tetralogicalState);

                    if (tetralogicalState == Constants::TetralogicalState::M) {
                        if (bitOfFirstNumber) {
                            isLowNumberSerieHasTrue = true;
                        }
                        if (!bitOfSecondNumber) {
                            isHighNumberSerieHasFalse = true;
                        }
                    }

                    continue;
                }
            }

            byteOfFirstNumber += offset;
            byteOfSecondNumber += offset;
        }
    }
} }
