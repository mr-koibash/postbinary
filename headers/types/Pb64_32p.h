#pragma once
#include "../abstracts/Fractional.h"

namespace Postbinary {
    class Pb64_32p : public Abstracts::Fractional {
    public:

        // constructors

        Pb64_32p() : Abstracts::Fractional(
            Constants::TetralogicalDigitCapacity::T_32,
            (unsigned int) Constants::FractionalFieldSize::Exponent::E_8,
            (unsigned int) Constants::FractionalFieldSize::Mantissa::M_21
        ) {
            // set identificator
            this->_setTetrit(0, Constants::TetralogicalState::FALSE);  // bits: 01
            this->_setTetrit(1, Constants::TetralogicalState::M);      // bits: 11  
        }

        template <typename Type>
        Pb64_32p(Type number) : Abstracts::Fractional(
            Constants::TetralogicalDigitCapacity::T_32,
            (unsigned int) Constants::FractionalFieldSize::Exponent::E_8,
            (unsigned int) Constants::FractionalFieldSize::Mantissa::M_21
        ) {
            float _number = (float)number;
            this->convertFromFloat(&_number);

            // set identificator
            this->_setTetrit(0, Constants::TetralogicalState::FALSE);  // bits: 01
            this->_setTetrit(1, Constants::TetralogicalState::M);      // bits: 11
        }

        template <typename Type1, typename Type2>
        Pb64_32p(Type1 number1, Type2 number2) : Abstracts::Fractional(
            Constants::TetralogicalDigitCapacity::T_32,
            (unsigned int) Constants::FractionalFieldSize::Exponent::E_8,
            (unsigned int) Constants::FractionalFieldSize::Mantissa::M_21) {
            float _number1 = (float)number1;
            float _number2 = (float)number2;
            this->convertFromFloat(&_number1, &_number2);

            // set identificator
            this->_setTetrit(0, Constants::TetralogicalState::FALSE);  // 01
            this->_setTetrit(1, Constants::TetralogicalState::M);      // 11
        }


        // get range methods

        void getMinimumFloatRange(float& f1, float& f2) {
            this->_getMinimumRange((void*)&f1, (void*)&f2, sizeof(float));
        }

        void getMaximumFloatRange(float& f1, float& f2) {
            this->_getMaximumRange((void*)&f1, (void*)&f2, sizeof(float));
        }

        // convert methods

        void convertFromFloat(float* number) {
            Abstracts::Fractional::_convertFromBinaryNumber(number, sizeof(float));
        }

        void convertFromFloat(float* number1, float* number2) {
            Abstracts::Fractional::_convertFromBinaryRange(number1, number2, sizeof(float));
        }
    };
}
