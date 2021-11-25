#include "pending_coords_updater.hpp"
#include "drawing.h"


namespace nm
{

	bool UpperSideUpdatingStrategy::IsValidSideToUpdate(int y) const
	{
		return y > 0;
	}


	int UpperSideUpdatingStrategy::TransformY(int y) const
	{
		return --y; // Checking y - 1 (upper side of computer graphics)
	}


	bool DownSideUpdatingStrategy::IsValidSideToUpdate(int y) const
	{
		return y < Height - 1;
	}


	int DownSideUpdatingStrategy::TransformY(int y) const
	{
		return ++y; // Checking y + 1 (down side of computer graphics)
	}


	bool SideUpdatingValidator::IsValidSideUpdating(int y) const
	{
		return m_sidesUpdater->IsValidSideToUpdate(y);
	}
	
	
	int SideUpdatingValidator::UpdateY(int y) const
	{
		return m_sidesUpdater->TransformY(y);
	}


	bool DriftingRightCheckStrategy::IsValidDrifting(int driftingX, int endingX) const
	{
		return driftingX <= endingX;
	}


	int DriftingRightCheckStrategy::Offset() const
	{
		return 1;
	}


	bool DriftingLeftCheckStrategy::IsValidDrifting(int driftingX, int endingX) const
	{
		return driftingX >= endingX;
	}


	int DriftingLeftCheckStrategy::Offset() const
	{
		return -1;
	}


	bool PendingCoordinatesUpdater::HasAnotherCoordinatesToCheck(int driftingX, int endingX) const
	{
		return m_driftingChecker->IsValidDrifting(driftingX, endingX);
	}


	int PendingCoordinatesUpdater::Offset() const
	{
		return m_driftingChecker->Offset();
	}

}