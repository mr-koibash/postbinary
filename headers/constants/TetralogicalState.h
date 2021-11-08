#pragma once

namespace Postbinary { namespace Constants {

    // Tetralogical casting to bit pair
    enum class TetralogicalState
    {
        A,      //0 = 00
        FALSE,  //1 = 01
        TRUE,   //2 = 10
        M       //3 = 11
    };
}}