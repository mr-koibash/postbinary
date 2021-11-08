#include "../../headers/Utilities/Convert.h"

namespace Postbinary { namespace Utilities { 
	Constants::TetralogicalState Convert::boolToTetralogicalState(bool value)
	{
		return value ? Constants::TetralogicalState::TRUE : Constants::TetralogicalState::FALSE;
	}
	




	// Set M and A for range of two numbers: M is [0; 1] and A is [1; 0]
	void Convert::tetritToMinimumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary)
	{
		bool isMimimumRange = true;
		Convert::_tetritToRange(tetralogicalState, leftBoundary, rightBoundary, isMimimumRange);
	}
	
	// Set M and A for range of two numbers: M is [0; 1] and A is [1; 0]
	void Convert::tetritToMaximumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary)
	{
		bool isMimimumRange = false;
		Convert::_tetritToRange(tetralogicalState, leftBoundary, rightBoundary, isMimimumRange);
	}

	//// Set M and A for range of two numbers: M is [0; 1] and A is [0; 1]
	//void Convert::tetritToMaximumRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary)
	//{
	//	switch (tetralogicalState)
	//	{
	//	case Constants::TetralogicalState::A:
	//		leftBoundary = false;
	//		rightBoundary = true;
	//		break;
	//	case Constants::TetralogicalState::FALSE:
	//		leftBoundary = false;
	//		rightBoundary = false;
	//		break;
	//	case Constants::TetralogicalState::TRUE:
	//		leftBoundary = true;
	//		rightBoundary = true;
	//		break;
	//	case Constants::TetralogicalState::M:
	//		leftBoundary = false;
	//		rightBoundary = true;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	
	void Convert::_tetritToRange(const Constants::TetralogicalState& tetralogicalState, bool& leftBoundary, bool& rightBoundary, bool isMinimumRange)
	{
		switch (tetralogicalState)
		{
		case Constants::TetralogicalState::A:
			leftBoundary = isMinimumRange;
			rightBoundary = !isMinimumRange;
			break;
		case Constants::TetralogicalState::FALSE:
			leftBoundary = false;
			rightBoundary = false;
			break;
		case Constants::TetralogicalState::TRUE:
			leftBoundary = true;
			rightBoundary = true;
			break;
		case Constants::TetralogicalState::M:
			leftBoundary = false;
			rightBoundary = true;
			break;
		default:
			break;
		}
	}

} }