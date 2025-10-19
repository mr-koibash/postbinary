// Class provides convert methods between binary and tetralogical digits

#pragma once
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

    private:
        static void _tetritToRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary, bool isMinimumRange);
    
        
    
    };
} }
