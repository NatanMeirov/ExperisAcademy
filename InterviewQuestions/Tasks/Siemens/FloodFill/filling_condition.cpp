#include "filling_condition.hpp"
#include "drawing.h"


namespace nm
{
	bool FillingCondition::CheckCondition(int x, int y, Color colorToReplace)
	{
		return m_fillingCondition->Check(x) && GetPixel(x, y) == colorToReplace;
	}


	int FillingCondition::Offset() const
	{
		return m_fillingCondition->Offset();
	}


	bool FillingRightStrategy::Check(int x) const
	{
		return x < Width;
	}


	int FillingRightStrategy::Offset() const
	{
		return 1;
	}


	bool FillingLeftStrategy::Check(int x) const
	{
		return x >= 0;
	}


	int FillingLeftStrategy::Offset() const
	{
		return -1;
	}

} // nm
