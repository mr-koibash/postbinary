#pragma once
#include "../abstracts/Number.h"
#include "Pb64_32p.h"


namespace Postbinary {
    class Pb128_32ip : public Abstracts::Number {
    protected:
        static const unsigned int _sizeOfExponent = (unsigned int) Constants::FractionalFieldSize::Exponent::E_8;
        static const unsigned int _sizeOfMantissa = (unsigned int) Constants::FractionalFieldSize::Mantissa::M_21;

    private:
        static const size_t identificatorSize = 4;
        static const size_t borderIdentificatorSize = 2;
        static const size_t fractionalNumberSize = 1 + _sizeOfExponent + _sizeOfMantissa;


    public:
        Pb128_32ip() : Abstracts::Number(
            Constants::TetralogicalDigitCapacity::T_64
        ) {
            // set identificator
            this->_setTetrit(3, Constants::TetralogicalState::A);       // bits: 00
            this->_setTetrit(2, Constants::TetralogicalState::TRUE);    // bits: 10
            this->_setTetrit(1, Constants::TetralogicalState::TRUE);    // bits: 10
            this->_setTetrit(0, Constants::TetralogicalState::M);       // bits: 11
        }

        template <typename Type1, typename Type2, typename Type3, typename Type4>
        Pb128_32ip(Type1 number1, Type2 number2, Type3 number3, Type4 number4) : Abstracts::Number(
            Constants::TetralogicalDigitCapacity::T_64
        ) {
            float _number1 = (float)number1;
            float _number2 = (float)number2;
            Pb64_32p _left = Pb64_32p(_number1, _number2);

            float _number3 = (float)number3;
            float _number4 = (float)number4;
            Pb64_32p _right = Pb64_32p(_number3, _number4);

            for (size_t i = (size_t) Constants::TetralogicalDigitCapacity::T_64; i --> this->fractionalNumberSize + this->identificatorSize;) {
                Constants::TetralogicalState tetrit = _left.getTetrit(i - (size_t) Constants::TetralogicalDigitCapacity::T_32);
                this->_setTetrit(i, tetrit);
            }
            for (size_t i = this->fractionalNumberSize + this->identificatorSize; i --> this->identificatorSize;) {
                Constants::TetralogicalState tetrit = _right.getTetrit(i - this->borderIdentificatorSize);
                this->_setTetrit(i, tetrit);
            }

            // set identificator
            this->_setTetrit(3, Constants::TetralogicalState::A);       // bits: 00
            this->_setTetrit(2, Constants::TetralogicalState::TRUE);    // bits: 10
            this->_setTetrit(1, Constants::TetralogicalState::TRUE);    // bits: 10
            this->_setTetrit(0, Constants::TetralogicalState::M);       // bits: 11
        }

        Pb64_32p* getLeft() {
            Pb64_32p* _left = new Pb64_32p();
            for (size_t i = (size_t) Constants::TetralogicalDigitCapacity::T_64; i --> this->fractionalNumberSize + this->identificatorSize;) {
                size_t tetritIdx = i - (size_t) Constants::TetralogicalDigitCapacity::T_32;
                _left->setTetrit(tetritIdx, this->getTetrit(i));
            }

            _left->setTetrit(1, Constants::TetralogicalState::M);
            _left->setTetrit(0, Constants::TetralogicalState::FALSE);

            return _left;
        }

        Pb64_32p* getRight() {
            Pb64_32p* _right = new Pb64_32p();
            for (size_t i = this->fractionalNumberSize + this->identificatorSize; i --> this->identificatorSize;) {
                size_t tetritIdx = i - this->borderIdentificatorSize;
                _right->setTetrit(tetritIdx, this->getTetrit(i));
            }

            _right->setTetrit(1, Constants::TetralogicalState::M);
            _right->setTetrit(0, Constants::TetralogicalState::FALSE);

            return _right;
        }
    };
}
