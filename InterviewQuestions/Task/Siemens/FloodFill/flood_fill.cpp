#include "flood_fill.hpp"
#include <iostream>
#include <stack>
#include <utility> // std::pair
#include <memory> // std::make_shared
#include "filling_indicator.hpp"
#include "filling_condition.hpp"
#include "pending_coords_updater.hpp"
#include "point.hpp"


namespace nm // Natan Meirov
{
    void updatePendingCoords(int startingX, int endingX, int y, FillingIndicator::Direction currentDir, FillingIndicator::Side fillingSide, Color colorToReplace, std::stack<FillingIndicator>& pendingCoordsToHandle)
    {      
        SideUpdatingValidator validator;
        if (fillingSide == FillingIndicator::Up)
        {
            validator = SideUpdatingValidator(std::make_shared<UpperSideUpdatingStrategy>());
        }
        else // FillingIndicator::Down
        {
            validator = SideUpdatingValidator(std::make_shared<DownSideUpdatingStrategy>());
        }

        if (!validator.IsValidSideUpdating(y))
        {
            return;
        }

        y = validator.UpdateY(y);
        PendingCoordinatesUpdater updater;

        if (currentDir == FillingIndicator::Right)
        {
            updater = PendingCoordinatesUpdater(std::make_shared<DriftingRightCheckStrategy>());
        }
        else // FillingIndicator::Left)
        {
            updater = PendingCoordinatesUpdater(std::make_shared<DriftingLeftCheckStrategy>());
        }

        bool hasCandidateCoordToPushForCurInterval = false;
        FillingIndicator::Direction nextDir = currentDir == FillingIndicator::Right ? FillingIndicator::Left : FillingIndicator::Right;
        register int driftingX = startingX;
        register int lastCandidateXToPush;

        while (updater.HasAnotherCoordinatesToCheck(driftingX, endingX))
        {
            if (GetPixel(driftingX, y) == colorToReplace)
            {
                lastCandidateXToPush = driftingX; // Replace the last candidate x coord - the same line would be drawn with the new candidate x coord
                hasCandidateCoordToPushForCurInterval = true;
            }
            else
            {
                if (hasCandidateCoordToPushForCurInterval)
                {
                    pendingCoordsToHandle.push(FillingIndicator({ lastCandidateXToPush, y }, nextDir, fillingSide));
                }

                hasCandidateCoordToPushForCurInterval = false;
            }

            driftingX += updater.Offset();
        }

        // Make sure to check if there is another waiting candidate after the loop's finish
        if (hasCandidateCoordToPushForCurInterval)
        {
            pendingCoordsToHandle.push(FillingIndicator({ lastCandidateXToPush, y }, nextDir, fillingSide));
        }
    }


    void fillLine(const FillingIndicator& fillingIndicator, Color color, Color colorToReplace, std::stack<FillingIndicator>& pendingCoordsToHandle)
    {
        int x = fillingIndicator.Coordinates().X();
        int y = fillingIndicator.Coordinates().Y();
        FillingIndicator::Side fillingSide = fillingIndicator.FillingSide();
        FillingIndicator::Direction fillingDirection = fillingIndicator.FillingDirection();
        register int deltaX;
        FillingCondition cond;

        if (fillingDirection == FillingIndicator::Right)
        {
            cond = FillingCondition(std::make_shared<FillingRightStrategy>());
        }
        else // FillingIndicator::Left
        {
            cond = FillingCondition(std::make_shared<FillingLeftStrategy>());
        }

        for (deltaX = x; cond.CheckCondition(deltaX, y, colorToReplace); deltaX += cond.Offset())
        {
            SetPixel(deltaX, y, color);
        }

        deltaX -= cond.Offset();

        FillingIndicator::Side oppositeSide = fillingSide == FillingIndicator::Up ? FillingIndicator::Down : FillingIndicator::Up;

        // First, push to the stack FillingIndicator that goes to the opposite side, and then to the current side:
        updatePendingCoords(x, deltaX, y, fillingDirection, oppositeSide, colorToReplace, pendingCoordsToHandle);
        updatePendingCoords(x, deltaX, y, fillingDirection, fillingSide, colorToReplace, pendingCoordsToHandle);
    }

} // nm


void floodFill(int x, int y, Color color)
{
    Color colorToReplace = GetPixel(x, y);
    std::stack<nm::FillingIndicator> pendingCoordsToHandle;

    // Init stage:
    pendingCoordsToHandle.push(nm::FillingIndicator({ x, y }, nm::FillingIndicator::Right, nm::FillingIndicator::Up));
    
    // Main loop:
    while (!pendingCoordsToHandle.empty())
    {
        nm::FillingIndicator fillingIndicator = pendingCoordsToHandle.top();
        pendingCoordsToHandle.pop();
        fillLine(fillingIndicator, color, colorToReplace, pendingCoordsToHandle);
    }
}
