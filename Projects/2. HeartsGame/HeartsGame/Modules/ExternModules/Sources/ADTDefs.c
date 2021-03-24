#include "../../HeartsGame/Modules/ExternModules/Headers/ADTDefs.h"

const char* ErrDescription[] = 
{
	"OK",
	"General error",
	"Initialization error",
	"Allocation error",
	"ReAllocation error",
	"Underflow error",
	"Overflow error",
	"Wrong index",
	"Wrong size"
};


const char* HandleError(ADTErr _errNum)
{
	return ErrDescription[_errNum];
}
