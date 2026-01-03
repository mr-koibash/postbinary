#include "../../headers/abstracts/Number.h"

namespace Postbinary { namespace Abstracts {
    Number::Number(Constants::BinaryDigitCapacity bitness) {
        // calculate number of bytes in binary form
        unsigned int _number_of_bytes = (unsigned int)bitness / (unsigned int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE;

        // allocate the memory
        this->_begin = new byte[_number_of_bytes];
        this->_end = this->_begin + _number_of_bytes;

        // set number tetrits to A
        this->_setAllTetrits(Constants::TetralogicalState::A);
    }

    Number::Number(Constants::TetralogicalDigitCapacity tetritness) :
        Number::Number(Constants::BinaryDigitCapacity((unsigned int)tetritness * 2)) {   }

    Number::~Number() {
        delete[] this->_begin;
    }

    pointer Number::begin() {
        return this->_begin;
    }

    pointer Number::end() {
        return this->_end;
    }


    // --------From/to raw bytes methods--------

    void Number::fromBytes(char* source) {
        std::memcpy(this->_begin, source, (unsigned int)this->sizeInBytes());
    }

    void Number::toBytes(char** begin, int& size) {
        *begin = (char*)this->begin();
        size = this->sizeInBytes();
    }


    // --------Size methods--------

    int Number::sizeInBytes() {
        return (int)(this->_end - this->_begin);
    }

    int Number::sizeInBits() {
        return sizeInBytes() * (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE;
    }

    int Number::sizeInTetrits() {
        return sizeInBytes() * (int)Constants::DigitNumbers::TETRITS_NUMBER_IN_BYTE;
    }

    // --------Operator overload--------

    Number& Number::operator= (Number& number) {
        std::memcpy(this->_begin, number._begin, (unsigned int)this->sizeInBytes());
        return *this;
    }

    // --------Presentation methods--------

    size_t Number::toString(char* buffer, size_t bufferSize) {
        size_t tetritsCount = this->sizeInTetrits();

        // validate space (including null-terminator)
        if (bufferSize <= tetritsCount) {
            return 0; // Error: too small buffer
        }

        size_t charIndex = 0;

            for (std::size_t i = tetritsCount; i --> 0 ;) {
                Constants::TetralogicalState tetrit = this->_getTetrit(i);
                char c = '?';

                switch(tetrit) {
                    case Constants::TetralogicalState::A:     c = 'A'; break;
                    case Constants::TetralogicalState::FALSE: c = '0'; break;
                    case Constants::TetralogicalState::TRUE:  c = '1'; break;
                    case Constants::TetralogicalState::M:     c = 'M'; break;
                }
                buffer[charIndex++] = c;
            }

            buffer[charIndex] = '\0'; // add null to end of string
            return charIndex;
    }

    #if defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
        std::string Number::toString() {
            std::string s;
            s.reserve(this->sizeInTetrits());

            for (std::size_t i = this->sizeInTetrits(); i --> 0 ;) {
                Constants::TetralogicalState tetrit = this->_getTetrit(i);
                switch(tetrit) {
                    case Constants::TetralogicalState::A:
                        s.push_back('A');
                        break;

                    case Constants::TetralogicalState::FALSE:
                        s.push_back('0');
                        break;

                    case Constants::TetralogicalState::TRUE:
                        s.push_back('1');
                        break;

                    case Constants::TetralogicalState::M:
                        s.push_back('M');
                        break;
                }
            }
            return s;
        }
    #endif

    // --------get-set methods--------

    Constants::TetralogicalState Number::_getTetrit(unsigned int tetritIndex) {
        // check input variable
        bool out_of_range = tetritIndex < 0 || tetritIndex >= this->sizeInTetrits();
        if (out_of_range) {
            return Constants::TetralogicalState::A;
        }

        // shift temp pointer to requiered byte
        byte _shift = (byte)(tetritIndex / (unsigned int)Constants::DigitNumbers::TETRITS_NUMBER_IN_BYTE);
        pointer _current = (this->_end - 1) - _shift;

        // calculate the required pair of bits and set it
        unsigned int _bitPairNumber = tetritIndex % (unsigned int)Constants::DigitNumbers::TETRITS_NUMBER_IN_BYTE;
        unsigned int _rightBitIdx = _bitPairNumber * 2;
        unsigned int _leftBitIdx = _rightBitIdx + 1;

        bool _leftBit = this->_getBit(*_current, _leftBitIdx);
        bool _rightBit = this->_getBit(*_current, _rightBitIdx);

        // 00 - A, 01 - FALSE, 10 - TRUE, 11 - M
        Constants::TetralogicalState tetralogicalState = Constants::TetralogicalState::A;
        if (!_leftBit) {
            if (!_rightBit) {
                tetralogicalState = Constants::TetralogicalState::A;
            }
            else {
                tetralogicalState = Constants::TetralogicalState::FALSE;
            }
        }
        else {
            if (!_rightBit) {
                tetralogicalState = Constants::TetralogicalState::TRUE;
            }
            else {
                tetralogicalState = Constants::TetralogicalState::M;
            }
        }

        return tetralogicalState;
    }

    void Number::_setTetrit(int tetritIndex, Constants::TetralogicalState value) {
        // check input variable
        bool out_of_range = tetritIndex < 0 || tetritIndex >= this->sizeInTetrits();
        if (out_of_range) {
            return;
        }

        this->_setTetritUnsafe(tetritIndex, value);
    }

    void Number::_setAllTetrits(Constants::TetralogicalState value) {
        byte byteValue = 0;

        switch (value) {
        case Constants::TetralogicalState::A:
            byteValue = 0x00;   // 00 00 00 00
            break;
        case Constants::TetralogicalState::FALSE:
            byteValue = 0x55;   // 01 01 01 01
            break;
        case Constants::TetralogicalState::TRUE:
            byteValue = 0xAA;   // 10 10 10 10
            break;
        case Constants::TetralogicalState::M:
            byteValue = 0xFF;   // 11 11 11 11
            break;
        default:
            break;
        }

        // set every byte in number to complexMask
        pointer _last = this->_end - 1;
        while (this->_begin <= _last) {
            *_last = byteValue;
            _last--;
        }
    }

    bool Number::_getBit(byte targetByte, unsigned int bitNumber) {
        return (targetByte >> bitNumber) & 1;
    }

    void Number::_setBit(byte& targetByte, unsigned int index, bool value) {
        if (value) {
            targetByte |= (1 << index);
        }
        else {
            targetByte &= ~(1 << index);
        }

    }

    void Number::_setTetritUnsafe(unsigned int tetritIndex, Constants::TetralogicalState value) {
        // shift temp pointer to requiered byte
        byte _shift = (byte)(tetritIndex / (unsigned int)Constants::DigitNumbers::TETRITS_NUMBER_IN_BYTE);
        pointer _current = (this->_end - 1) - _shift;

        // calculate the required pair of bits and set it
        unsigned int _bitPairNumber = tetritIndex % (unsigned int)Constants::DigitNumbers::TETRITS_NUMBER_IN_BYTE;
        unsigned int _rightBitIdx = _bitPairNumber * 2;
        unsigned int _leftBitIdx = _rightBitIdx + 1;

        switch (value) {
        case Constants::TetralogicalState::A:
            this->_setBit(*_current, _leftBitIdx, 0);
            this->_setBit(*_current, _rightBitIdx, 0);
            break;
        case Constants::TetralogicalState::FALSE:
            this->_setBit(*_current, _leftBitIdx, 0);
            this->_setBit(*_current, _rightBitIdx, 1);
            break;
        case Constants::TetralogicalState::TRUE:
            this->_setBit(*_current, _leftBitIdx, 1);
            this->_setBit(*_current, _rightBitIdx, 0);
            break;
        case Constants::TetralogicalState::M:
            this->_setBit(*_current, _leftBitIdx, 1);
            this->_setBit(*_current, _rightBitIdx, 1);
            break;
        default:
            break;
        }
    }
} }
