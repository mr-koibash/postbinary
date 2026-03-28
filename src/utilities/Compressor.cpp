#include "../../headers/utilities/Compressor.h"
#include "../../headers/types/Package.h"    
#include "../../headers/types/Pb64_32p.h"
#include "../../headers/types/Pb128_32ip.h"

namespace Postbinary { namespace Utilities { 
    Postbinary::Package* Compressor::compress(Pb64_32p& number, unsigned int K) {
        const unsigned int identifierSize = 2;

        // init package (number of insignificant digits = K [0 to K - 1], minus 2 per identifier)
        unsigned int insignificantBytes = (K - identifierSize) / Compressor::TETRITS_IN_BYTE;
        unsigned int packageSize = number.sizeInBytes() - insignificantBytes; 
        Package* package = new Package(packageSize);                     
        
        // set Pb64_32p identifier
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 1, Constants::TetralogicalState::M);
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 2, Constants::TetralogicalState::FALSE);

        // transfer main digits
        int offset = insignificantBytes * Compressor::TETRITS_IN_BYTE + identifierSize;
        for (int i = number.sizeInTetrits() - 1; i >= K; i--) {
            package->setTetrit(i - offset, number.getTetrit(i));
        }

        // fill remaining digits in package with A-uncertainty state
        for (int i = K - offset - 1; i >= 0; i--) {
            package->setTetrit(i, Constants::TetralogicalState::A);
        }
        
        return package;
    }

    Postbinary::Package* Compressor::compress(Pb128_32ip& number, unsigned int K) {
        const unsigned int identifierSize = 4;

        // init package (number of insignificant digits = K [0 to K - 1], minus 2 per identifier)
        unsigned int insignificantBytes = (K - identifierSize) / Compressor::TETRITS_IN_BYTE;
        unsigned int packageSize = number.sizeInBytes() / 2 - insignificantBytes; 
        Package* package = new Package(packageSize);

        // set Pb64_32p identifier
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 1, Constants::TetralogicalState::A);
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 2, Constants::TetralogicalState::TRUE);
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 3, Constants::TetralogicalState::TRUE);
        package->setTetrit(packageSize * Compressor::TETRITS_IN_BYTE - 4, Constants::TetralogicalState::M);

        // transfer main digits
        Postbinary::Pb64_32p* left = number.getLeft();
        Postbinary::Pb64_32p* right = number.getRight();
        int offset = insignificantBytes * Compressor::TETRITS_IN_BYTE + identifierSize;
        for (int i = number.sizeInTetrits() / 2 - 1; i >= K; i--) {
            Constants::TetralogicalState t1 = left->getTetrit(i);
            Constants::TetralogicalState t2 = right->getTetrit(i);

            if (t1 == t2) {
                package->setTetrit(i - offset, t2);
            } else if (t2 == Constants::TetralogicalState::TRUE) {
                package->setTetrit(i - offset, Constants::TetralogicalState::M);
            } else {
                package->setTetrit(i - offset, Constants::TetralogicalState::A);
            }
        }

        // fill remaining digits in package with A-uncertainty state
        for (int i = K - offset - 1; i >= 0; i--) {
            package->setTetrit(i, Constants::TetralogicalState::A);
        }

        return package;
    }
        

    Postbinary::Pb64_32p* Compressor::decompressAsPb64(Package& package) {
        const unsigned int identifierSize = 2;
        
        Pb64_32p* number = new Pb64_32p();

        int offset = number->sizeInTetrits() - package.sizeInTetrits() + identifierSize;
        for (int i = package.sizeInTetrits() - identifierSize; i >= 0; i--) {
            number->setTetrit(i + offset, package.getTetrit(i));
        }

        // fill remaining tetrits
        for (int i = offset - 1; i > identifierSize - 1; i--) {
            number->setTetrit(i, Constants::TetralogicalState::A);
        }

        return number;
    }

    Postbinary::Pb128_32ip* Compressor::decompressAsPb128(Package& package) {
        const unsigned int identifierSize = 4;
        const unsigned int rangeDigits = 30; // range between digits of left and right numbers

        Pb128_32ip* number = new Pb128_32ip();

        int offset = number->sizeInTetrits() - package.sizeInTetrits() + identifierSize;
        for (int i = package.sizeInTetrits() - identifierSize; i >= 0; i--) {
            Constants::TetralogicalState t = package.getTetrit(i);
            if (t == Constants::TetralogicalState::TRUE || t == Constants::TetralogicalState::FALSE) {
                number->setTetrit(i + offset, t);
                number->setTetrit(i + offset - rangeDigits, t);
            } else if (t == Constants::TetralogicalState::M) {
                number->setTetrit(i + offset, Constants::TetralogicalState::FALSE);
                number->setTetrit(i + offset - rangeDigits, Constants::TetralogicalState::TRUE);
            } else {
                number->setTetrit(i + offset, Constants::TetralogicalState::TRUE);
                number->setTetrit(i + offset - rangeDigits, Constants::TetralogicalState::FALSE);
            }  
        }

        // fill remaining tetrits
        const int lastLeftMantissaDigit = 34;
        for (int i = offset - 1; i >= lastLeftMantissaDigit; i--) {
            number->setTetrit(i, Constants::TetralogicalState::A);
            number->setTetrit(i - rangeDigits, Constants::TetralogicalState::A);
        }

        return number;
    }
} }
