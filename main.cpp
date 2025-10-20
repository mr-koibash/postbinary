#include <iostream>

#include "Postbinary.h"



// TODO
//добавить враппер, методы getSizeOfExponent и getSizeOfMantissa во Fractional.h и Fractional.cpp, и добавить метод в строку битов




namespace Postbinary {

    class PostbinaryFractionalWrapper : Abstracts::Fractional {
    public:
        PostbinaryFractionalWrapper(Abstracts::Fractional& fractional) : Abstracts::Fractional(
            (Postbinary::Constants::TetralogicalDigitCapacity) fractional.sizeInTetrits(),
            fractional.getSizeOfExponent(),
            fractional.getSizeOfMantissa()
        ) {
            // copy fractional number bytes to wrapper
            int size;
            char* fractionalBegin;
            fractional.toBytes(&fractionalBegin, size);
            this->fromBytes(fractionalBegin);
        }

    #if defined(__linux__) || defined(_WIN32)
        std::string toString() {
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

    };


    inline bool _getBit(byte targetByte, unsigned int bitNumber) {
        return (targetByte >> bitNumber) & 1;
    }



    // convert float to string of bits
    std::string foo(void* number, unsigned int numberOfBytes)
    {   
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
                //bool bitOfNumber = _get(*byteOfNumber >> bitCounter) & 1;
                bool bitOfNumber = _getBit(*byteOfNumber, bitCounter);
                

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


    // b1 - high order byte, b4 - low order byte
    void convertBitsToFloat(float* number, Postbinary::byte b1, Postbinary::byte b2, Postbinary::byte b3, Postbinary::byte b4) {
        


        // 0 10000011 000100000011011011100
        // 01000001 10001000 00011011 01110000

        

        
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

        // casting a binary bits from fractional number (sign, exponenta, mantissa) to postbinary
        // cycle over each byte and each bit in the number
        std::string s;
        for (int byteCounter = numberOfBytes - 1; byteCounter >= 0; byteCounter--)
        {

            

            // for (int bitCounter = (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE - 1; bitCounter >= 0; bitCounter--)
            // {
            //     // calculate the absolute index of the bit in the number
            //     int bitIdx = byteCounter * (int)Constants::DigitNumbers::BITS_NUMBER_IN_BYTE + bitCounter;
                
            //     // get current bits of both numbers
            //     //bool bitOfNumber = _get(*byteOfNumber >> bitCounter) & 1;
            //     bool bitOfNumber = _getBit(*byteOfNumber, bitCounter);
                

            //     if (bitOfNumber) {
            //         s.push_back('1');
            //     } else {
            //         s.push_back('0');
            //     }
            // }

            // s.push_back(' ');
            // byteOfNumber += offset;
        }

        //return s;







}


}



// namespace Postbinary {

//     class PostbinaryFractionalWrapper : Abstracts::Fractional {
//     private:
        

//     public:
//         PostbinaryNumberWrapper(Abstracts::Number number) : Abstracts::Number(number.sizeInBits()) {

//         }
//     };


// }






int main() {


    // 01000010 00111111 01100111 00111000 
    // 01000010 01000100 10100100 10101000

    Postbinary::byte bl1 = 0b01000010;
    Postbinary::byte bl2 = 0b00111111;
    Postbinary::byte bl3 = 0b01100111;
    Postbinary::byte bl4 = 0b00111000;


    float lowIntervalBorder = 0;
    Postbinary::convertBitsToFloat(&lowIntervalBorder, bl1, bl2, bl3, bl4);


    Postbinary::byte bh1 = 0b01000010;
    Postbinary::byte bh2 = 0b01000100;
    Postbinary::byte bh3 = 0b10100100;
    Postbinary::byte bh4 = 0b10101000;

    float highIntervalBorder = 0;
    Postbinary::convertBitsToFloat(&highIntervalBorder, bh1, bh2, bh3, bh4);



    float a2 = 0;

    /*
    00100101
    00111111

    00011010
    


    11.001001 00001101 00010000 11110101
    11.001001 00001111 11011010 11010011

    11.001001 000011MM MMMAAAAA AAAAAAAA


    ---



    Гипотеза: что если будут единицы
    0 10000011 0001 000 0001101101110001
    0 10000011 0001 111 0000011000100101
    Postbinary: 
    0 10000011 0001 MMM AAAAAAAAAAAAAAM0

    0 10000011 0001 000 1111111111111111 - больше нужного
    0 10000011 0001 111 0000000000000001 - меньше нужного


    0 10000011 0001 000 1110000010001110
    0 10000011 0001 111 1111010111011010

    0 10000011 0001 MMM MMMMAAAAAAAAAAM0

    0 10000011 0001 000 0000111111111111 - всё ок
    0 10000011 0001 111 1111000000000001 - меньше нужного

    Решение: после первого совпадения - все дальнейшие должны быть М вплоть до первого несовпадения ВКЛЮЧИТЕЛЬНО
    начиная с первого несовпадения+1 дальнейшие значения - А (исключая конец числа) 
    
    // апдейт - в таком варианте как выше будет факап, т.к. там и так 111, а остальная часть обнулится и итоговое число будет меньше


    

    Проблемы по низу:
        - если было 0000011, а ставлю МММААААА, то при переходе будет больше необходимого: 0001111111

    Т.е. в меньшем числе серия М либо:
        - должна включать внутри себя число 1
    В большем числе серия М должна:
        - включать внутри себя число 0

    Пока у обоих чисел внутри серии М нет 0 и 1, то продолжать алгоритм



    // временное решение для статьи - подбирать эмпирически, сначала по правилу выше, а затем перебором до нужного результата (если диапазон неправильный)


    3M 
    4M Minimum range: [17.0625; 17.9375]
    5M Minimum range: [17.0312; 17.9688]
    6M Minimum range: [17.0156; 17.9844]
    7M Minimum range: [17.0078; 17.9922] !!!

    8M Minimum range: [17.0039; 17.9961] - уже более избыточно

    */
    





    float a = 17.0078;
    // float b = 49.1608;
    float b = 17.9922;

    // float a = 3.1414225;
    // float b = 3.1415927;
    std::cout << "Initial numbers: " << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;

    std::cout << "Numbers bits: " << std::endl;
    std::cout << Postbinary::foo(&a, 4) << std::endl;
    std::cout << Postbinary::foo(&b, 4) << std::endl;

    Postbinary::Pb64_32p p;
    p.convertFromFloat(&a, &b);



    // p._setTetrit(10, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(9, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(8, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(7, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(6, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(5, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(4, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(3, Postbinary::Constants::TetralogicalState::A);
    // p._setTetrit(2, Postbinary::Constants::TetralogicalState::A);





    Postbinary::PostbinaryFractionalWrapper pw(p);
    std::string s = pw.toString();
    std::cout << "Postbinary: " << std::endl << s << std::endl; 

    float minRangeStart = 0;
    float minRangeEnd = 0;
    p.getMinimumFloatRange(minRangeStart, minRangeEnd);
    std::cout << "Minimum range: [" << minRangeStart << "; " << minRangeEnd << "]" << std::endl;


    float maxRangeStart = 0;
    float maxRangeEnd = 0;
    p.getMaximumFloatRange(maxRangeStart, maxRangeEnd);
    std::cout << "Maximum range: [" << maxRangeStart << "; " << maxRangeEnd << "]" << std::endl;
    
}
