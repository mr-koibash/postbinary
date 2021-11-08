// Abstract postbinary number class that provides memory allocation  
// and operations on tetralogical bits (tetrits) operations. 
// Left tetrits are elders, right are minors
//
// Class supports the following:
// * allocation the memory area for a given postbinary "bitness" - tetritness
// * set any postbinary "bit" (tetrit) in postbinary state: A, False, True or M
// * set all tetrits in any postbinary state

#pragma once
#include <cstring>
#include "../Constants.h"
#include "../Service.h"
#include "../Utilities.h"

namespace Postbinary { namespace Abstracts
{
    class Number
    {
    private:
        pointer _begin;	// start of memory area
        pointer _end;	// end of memory area

    public:
        Number(Constants::BinaryDigitCapacity bitness);
        Number(Constants::TetralogicalDigitCapacity tetritness);
        virtual ~Number() = 0;

        // --------Get main pointers--------

        pointer begin();
        pointer end();

        // --------From/to raw bytes methods--------

        /// <summary>
        /// Set postbinary numbers from raw bytes in memory
        /// </summary>
        /// <param name="source">begin of memory block</param>
        void fromBytes(char* source);

        /// <summary>
        /// Get raw bytes from postbinary number
        /// </summary>
        /// <param name="begin">begin of memory block</param>
        /// <param name="size">size of memory block</param>
        void toBytes(char** begin, int& size);

        // --------Size methods--------

        /// <summary>
        /// Calculate the size of tetralogical number in the binary memory in bytes
        /// </summary>
        /// <returns>number of binary bytes</returns>
        int sizeInBytes();

        /// <summary>
        /// Calculate the size of tetralogical number in the binary memory in bits
        /// </summary>
        /// <returns>number of binary bits</returns>
        int sizeInBits();

        /// <summary>
        /// Calculate the size of tetralogical number in the binary memory in 
        /// tetralogical "bits" - tetrits
        /// </summary>
        /// <returns>number of tetrits</returns>
        int sizeInTetrits();

        // --------Operator overload--------

        Number& operator= (Number& number);

    protected:

        // --------get-set methods--------

        Constants::TetralogicalState _getTetrit(unsigned int idx);

        /// <summary>
        /// Set tetrit to postbinary state
        /// </summary>
        /// <param name="tetritIndex">Index of tetrit in the quaternary number</param>
        /// <param name="value">Value in one of quaternary states: A, FALSE, TRUE, M</param>
        void _setTetrit(int tetritIndex, Constants::TetralogicalState value);

        /// <summary>
        /// Set all tetrits in the number to postbinary state
        /// </summary>
        /// <param name="value">Value in one of quaternary states: A, FALSE, TRUE, M</param>
        void _setAllTetrits(Constants::TetralogicalState value);

        bool _getBit(byte targetByte, unsigned int bitNumber);
        void _setBit(byte& number, unsigned int index, bool value);

    private:

        /// <summary>
        /// Set tetrit to postbinary state without input validation 
        /// </summary>
        /// <param name="tetritIndex">Index of tetrit in the quaternary number</param>
        /// <param name="value">Value in one of quaternary states: A, FALSE, TRUE, M</param>
        void _setTetritUnsafe(unsigned int tetritIndex, Constants::TetralogicalState value);
    };
} }