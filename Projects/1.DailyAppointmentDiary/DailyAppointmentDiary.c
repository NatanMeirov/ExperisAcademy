/* Includes: */
#include <stdlib.h> /* Used for size_t, malloc, calloc, realloc, free */
#include <stdio.h> /* Used for FILE and printf */
#include "Meeting.h"
#include "DailyAppointmentDiary.h"


/* Defines: */
#define MAGIC_NUM 9817642
#define IS_FULL 1
#define IS_NOT_FULL 0


struct DailyAppointmentDiary
{
	Meeting** m_meetingsArray;
	size_t m_meetingsArraySize;
	size_t m_todaysMeetingsCount;
	float m_todayFirstMeetingTime;
	float m_todayLastMeetingTime;
	int m_magicNumber;
};


/* Validation Functions Declarations: */
DADStatus ValidateNotNullDailyAppointmentDiary(DailyAppointmentDiary* _ad);
DADStatus ValidateNotNullMeetingPtrPtr(Meeting** _mPtrPtr);
DADStatus ValidateNotNullMeetings(Meeting* _m);
DADStatus ValidateNotEmptyDailyAppointmentDiary(DailyAppointmentDiary* _ad);
DADStatus ValidateNotClashMeetingsInTime(DailyAppointmentDiary* _ad, Meeting* _m);


/* Helper Functions Declarations: */
DailyAppointmentDiary* InitializeNewDailyAppointmentDiary(DailyAppointmentDiary* _ad);
int IsFullMeetingsArray(DailyAppointmentDiary* _ad);
DADStatus DoubleSizeOfMeetingsArray(DailyAppointmentDiary* _ad);
size_t FindCorrectInsertPlace(DailyAppointmentDiary* _ad, Meeting* _m);
void ShiftMeetingsRight(DailyAppointmentDiary* _ad, size_t _indexOfMeeting);
void SwapMeetings(Meeting** _m1, Meeting** _m2);
int SearchForMeeting(DailyAppointmentDiary* _ad, float _meetingTime);
void ShiftMeetingsLeft(DailyAppointmentDiary* _ad, size_t _indexOfMeeting);
int CompaireMeeting(Meeting* _m1, Meeting* _m2);
void WriteDailyAppointmentDiaryData(DailyAppointmentDiary* _ad, FILE* _f);
void WriteMeetingsData(DailyAppointmentDiary* _ad, FILE* _f);
void ReadDailyAppointmentDiaryData(DailyAppointmentDiary* _ad, FILE* _f);
void ReadMeetingsData(DailyAppointmentDiary* _ad, FILE* _f);


/*---------------------------------------------------- Main API Functions: ---------------------------------------------------------------*/

DailyAppointmentDiary* CreateNewDailyAppointmentDiary()
{
	DailyAppointmentDiary* ad = NULL;
	
	ad = malloc(sizeof(DailyAppointmentDiary));
	
	if(ValidateNotNullDailyAppointmentDiary(ad) == NULL_PTR_ERROR) /* ALLOCATION_FAILED_ERROR */
	{
		return NULL;
	}

	ad->m_meetingsArray = calloc(sizeof(Meeting*) * 2); /* Initialized with 2 places */
	
	if(ValidateNotNullMeetingsPtrPtr(ad->m_meetingsArray) == NULL_PTR_ERROR) /* ALLOCATION_FAILED_ERROR */
	{
		return NULL;
	}

	return InitializeNewDailyAppointmentDiary(ad);
}


DADStatus DestroyDailyAppointmentDiary(DailyAppointmentDiary* _ad)
{
	size_t i;
	
	if(ValidateNotNullDailyAppointmentDiary(_ad) != NULL_PTR_ERROR && _ad->m_magicNumber == MAGIC_NUM)
	{
		for(i = 0; i < _ad->m_todaysMeetingsCount; i++)
		{
			DeleteMeeting(_ad->m_meetingsArray[i]);
		}
	
		free(_ad->m_meetingsArray);	
		_ad->m_magicNumber = 0;
		free(_ad);
	}
}


DADStatus InsertNewDailyAppointmentToDiary(DailyAppointmentDiary* _ad, Meeting* _newMeeting)
{
	DADStatus statusCode;
	size_t indexOfPlaceToInsert;
	
	if((statusCode = ValidateNotNullDailyAppointmentDiary(_ad)) != OK || (statusCode = ValidateNotNullMeetings(_newMeeting)) != OK)
	{
		return statusCode;
	}
	if(IsFullMeetingsArray)
	{
		if((statusCode = DoubleSizeOfMeetingsArray(_ad)) != OK)
		{
			return statusCode;
		}
	}
	if(_ad->m_todayFirstMeetingTime <= GetMeetingEndingTime(_newMeeting) || _ad->m_todayLastMeetingTime >= GetMeetingStartingTime(_newMeeting)) /* Need to check if meetings are clashing */
	{
		if((statusCode = ValidateNotClashMeetingsInTime(_ad)) != OK)
		{
			return statusCode;
		}
	}
	
	indexOfPlaceToInsert = FindCorrectInsertPlace(_ad, _newMeeting);
	ShiftMeetingsRight(_ad, indexOfPlaceToInsert);
	_ad->m_meetingsArray[indexOfPlaceToInsert] = _newMeeting;
	_ad->m_todaysMeetingsCount++;
	
	if(CompaireMeeting(_ad->m_meetingsArray[0], _newMeeting)) /* Checks if the new meeting is the first meeting for today */
	{
		_ad->m_todayFirstMeetingTime = GetMeetingStartingTime(_newMeeting); /* Updating */
	}
	
	if(CompaireMeeting(_ad->m_meetingsArray[_ad->m_todaysMeetingsCount - 1], _newMeeting)) /* Checks if the new meeting is the last meeting for today */
	{
		_ad->m_todayLastMeetingTime = GetMeetingEndingTime(_newMeeting); /* Updating */
	}
	
	return OK;
}


DADStatus RemoveDailyAppointmentFromDiary(DailyAppointmentDiary* _ad, float _meetingStartingTime, Meeting** _meetingToRemove)
{
	size_t indexOfMeeting;
	DADStatus statusCode;
	
	if((statusCode = ValidateNotNullDailyAppointmentDiary(_ad)) != OK || (statusCode = ValidateNotNullMeetingPtrPtr(_meetingToRemove)) != OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotEmptyDailyAppointmentDiary(_ad)) != OK
	{
		return statusCode;
	}
	
	indexOfMeeting = SearchForMeeting(_ad, _meetingStartingTime);
	
	if(indexOfMeeting == -1) /* Not Found */
	{
		return MEETING_NOT_FOUND_ERROR;
	}
	
	*_meetingToRemove = _ad->m_meetingsArray[indexOfMeeting];
	ShiftMeetingsLeft(_ad, indexOfMeeting);
	_ad->m_todaysMeetingsCount--;
	
	if(indexOfMeeting == 0) /* Checks if the meeting was the first meeting for today */
	{
		_ad->m_todayFirstMeetingTime = GetMeetingStartingTime(_ad->m_meetingsArray[0]); /* Updating */
	}
	
	if(indexOfMeeting == _ad->m_todaysMeetingsCount) /* Checks if the meeting was the last meeting for today */
	{
		_ad->m_todayLastMeetingTime = GetMeetingEndingTime(_newMeeting); /* Updating */
	}
	
	return OK;	
}


int FindDailyAppointmentInDiary(DailyAppointmentDiary* _ad, float _meetingStartingTime)
{
	size_t indexOfMeeting;
	DADStatus statusCode;
	
	if((statusCode = ValidateNotNullDailyAppointmentDiary(_ad)) != OK)
	{
		return statusCode;
	}
	
	return SearchForMeeting(_ad, _meetingStartingTime);
}


void PrintAllDailyMeetings(DailyAppointmentDiary* _ad)
{
	size_t i;
	
	if(ValidateNotNullDailyAppointmentDiary(_ad) != OK)
	{
		return; /* NULL_PTR_ERROR */
	}
	
	printf("\t\t\t|********************************** Today's Meetings **********************************\n|\n");
	
	for(i = 0; i < _ad->m_todaysMeetingsCount; i++)
	{
		printf("\t\t\t|\t->\t[Meeting %d] Starting time: %f, Ending time: %f, Room number: %d\n", (i + 1), GetMeetingStartingTime(_ad->m_meetingsArray[i]), GetMeetingEndingTime(_ad->m_meetingsArray[i]), GetMeetingRoomNumber(_ad->m_meetingsArray[i]));
	}
}

/*-------------------------------------------------- End of Main API Functions -----------------------------------------------------------*/


/* Validation Functions: */
DADStatus ValidateNotNullDailyAppointmentDiary(DailyAppointmentDiary* _ad)
{
	return (!_ad) ? NULL_PTR_ERROR : OK;
}

DADStatus ValidateNotNullMeetingPtrPtr(Meeting** _mPtrPtr)
{
	return (!_mArr) ? NULL_PTR_ERROR : OK;
}


DADStatus ValidateNotNullMeetings(Meeting* _m)
{
	return (!_m) ? NULL_PTR_ERROR : OK;
}


DADStatus ValidateNotEmptyDailyAppointmentDiary(DailyAppointmentDiary* _ad)
{
	return (_ad->m_todaysMeetingsCount == 0) ? UNDERFLOW_ERROR : OK;
}


DADStatus ValidateNotClashMeetingsInTime(DailyAppointmentDiary* _ad, Meeting* _m)
{
	size_t i;
	float startingTime, endingTime;
	float newMeetingStartingTime = GetMeetingStartingTime(_m);
	float newMeetingEndingTime = GetMeetingEndingTime(_m);

	for(i = 0; i < _ad->m_todaysMeetingsCount; i++)
	{
		startingTime = GetMeetingStartingTime(_ad->m_meetingsArray[i]);
		endingTime = GetMeetingEndingTime(_ad->m_meetingsArray[i]);
		
		if((startingTime <= newMeetingStartingTime && newMeetingStartingTime <= endingTime) || (startingTime <= newMeetingEndingTime && newMeetingEndingTime <= endingTime) || (newMeetingStartingTime < startingTime && endingTime < newMeetingEndingTime))
		{
			return CLASH_MEETINGS_ERROR;
		}
	}
	
	return OK;
}


/* Helper Functions: */
DailyAppointmentDiary* InitializeNewDailyAppointmentDiary(DailyAppointmentDiary* _ad)
{
	_ad->m_meetingsArraySize = 1;
	_ad->m_todaysMeetingsCount = 0;
	_ad->m_todaysFirstMeetingTime = 0.0;
	_ad->m_todaysLastMeetingTime = 0.0;
	_ad->m_magicNumber = MAGIC_NUM;
	
	return _ad;
}


int IsFullMeetingsArray(DailyAppointmentDiary* _ad)
{
	return (_ad->m_meetingsArraySize == _ad->m_todaysMeetingsCount) ? IS_FULL : IS_NOT_FULL;
}


DADStatus DoubleSizeOfMeetingsArray(DailyAppointmentDiary* _ad)
{
	Meeting** temp = NULL;
	
	temp = realloc((_ad->m_meetingsArray * 2) * sizeof(Meeting*));
	
	
	if(ValidateNotNullMeetingsArray(temp) != OK)
	{
		return REALLOCATION_FAILED_ERROR;
	}
	
	_ad->m_meetingsArray = temp;
	_ad->m_meetingsArraySize *= 2; /* Doubled its size */
	
	return OK;
}


size_t FindCorrectInsertPlace(DailyAppointmentDiary* _ad, Meeting* _m)
{
	size_t i;
	float currentMeetingEndingTime, nextMeetingStartingTime, newMeetingStartingTime = GetMeetingStartingTime(_m), newMeetingEndingTime = GetMeetingEndingTime(_m);
	
	for(i = 0; i < _ad->m_todaysMeetingsCount - 1; i++)
	{
		currentMeetingEndingTime = GetMeetingEndingTime(_ad->m_meetingsArray[i]);
		nextMeetingStartingTime = GetMeetingStartingTime(_ad->m_meetingsArray[i + 1]);
		
		if(currentMeetingEndingTime < newMeetingStartingTime && newMeetingEndingTime < nextMeetingStartingTime)
		{
			return i;	
		}
	}
}


void ShiftMeetingsRight(DailyAppointmentDiary* _ad, size_t _indexOfMeeting)
{
	size_t i;
	
	for(i = _ad->m_todaysMeetingsCount; i >= _indexOfMeeting; i--)
	{
		_ad->m_meetingsArray[i] = _ad->m_meetingsArray[i - 1];
	}

	_ad->m_meetingsArray[_indexOfMeeting] = NULL; /* Not a must */
}


int SearchForMeeting(DailyAppointmentDiary* _ad, float _meetingTime) /* Using binary search - Returns index of the Meeting, else -1 if not found */
{
	float currentMeetingTime;
	size_t mid, start = 0, end = _ad->m_todaysMeetingsCount - 1;
	
	while(start <= end)
	{
		mid = (start + end) / 2;
		currentMeetingTime = GetMeetingStartingTime(_ad->m_meetingsArray[mid]);
		
		if(currentMeetingTime == _meetingTime)
		{
			return mid;
		}
		else if(currentMeetingTime < _meetingTime)
		{
			start = mid + 1;
		}
		else /* currentMeetingTime > _meetingTime */
		{
			end = mid - 1;
		}
	}
	
	return -1;
}


void ShiftMeetingsLeft(DailyAppointmentDiary* _ad, size_t _indexOfMeeting)
{
	size_t i;
	
	for(i = _ad->m_todaysMeetingsCount - 1; i > _indexOfMeeting; i--)
	{
		_ad->m_meetingsArray[i - 1] = _ad->m_meetingsArray[i];
	}

	_ad->m_meetingsArray[_ad->m_todaysMeetingsCount - 1] = NULL;
}


int CompaireMeeting(Meeting* _m1, Meeting* _m2)
{
	return (_m1 == _m2) /* Compairing the pointers */ ? 1 /* True */ : 0; /* False */
}


DADStatus SaveDailyAppointmentDiaryToFile(DailyAppointmentDiary* _ad)
{
	FILE* fdad = NULL;
	FILE* fm = NULL;
	DADStatus statusCode;
	
	if((statusCode = ValidateNotNullDailyAppointmentDiary(_ad)) != OK)
	{
		return statusCode;
	}

	if(!(fdad = fopen("DailyAppointmentDiaryFile.txt", "w")))
	{
		return FILE_NOT_CREATED_ERROR;
	}
	
	if(!(fm = fopen("MeetingsFile.txt", "w")))
	{
		return FILE_NOT_CREATED_ERROR;
	}
	
	WriteDailyAppointmentDiaryData(_ad, fdad);
	WriteMeetingsData(_ad, fm);
	
	fclose(fdad);
	fclose(fm);
}


DADStatus LoadDailyAppointmentDiaryFromFile(DailyAppointmentDiary* _ad)
{
	FILE* fdad = NULL;
	FILE* fm = NULL;
	DADStatus statusCode;
	
	if((statusCode = ValidateNotNullDailyAppointmentDiary(_ad)) != OK)
	{
		return statusCode;
	}

	if(!(fdad = fopen("DailyAppointmentDiaryFile.txt", "r")))
	{
		return FILE_NOT_FOUND_ERROR;
	}
	
	if(!(fm = fopen("MeetingsFile.txt", "r")))
	{
		return FILE_NOT_FOUND_ERROR;
	}
	
	ReadDailyAppointmentDiaryData(_ad, fdad);
	
	ad->m_meetingsArray = calloc(sizeof(Meeting*) * _ad->m_meetingsArraySize);
	if((statusCode = ValidateNotNullMeetingsPtrPtr(ad->m_meetingsArray)) != OK)
	{
		return statusCode;
	}
	
	ReadMeetingsData(_ad, fm);
	
	fclose(fdad);
	fclose(fm);
}


void WriteDailyAppointmentDiaryData(DailyAppointmentDiary* _ad, FILE* _f)
{
	fprintf(_f, "%ld %ld %f %f %d", _ad->m_meetingsArraySize, _ad->m_todaysMeetingsCount, _ad->m_todayFirstMeetingTime, _ad->m_todayLastMeetingTime, _ad->m_magicNumber);
}


void WriteMeetingsData(DailyAppointmentDiary* _ad, FILE* _f)
{
	size_t i;
	
	for(i = 0; i < _ad->m_todaysMeetingsCount; i++)
	{
		fprintf(_f, "%f %f %d\n", GetMeetingStartingTime(_ad->m_meetingsArray[i]), GetMeetingEndingTime(_ad->m_meetingsArray[i]), GetMeetingRoomNumber(_ad->m_meetingsArray[i]);
	}
}


void ReadDailyAppointmentDiaryData(DailyAppointmentDiary* _ad, FILE* _f)
{
	fscanf(_f, "%ld %ld %f %f %d", &(_ad->m_meetingsArraySize), &(_ad->m_todaysMeetingsCount), &(_ad->m_todayFirstMeetingTime), &(_ad->m_todayLastMeetingTime), &(_ad->m_magicNumber));
}


void ReadMeetingsData(DailyAppointmentDiary* _ad, FILE* _f)
{
	size_t i;
	float start, end;
	unsigned int room;
	
	for(i = 0; i < _ad->m_meetingsArraySize, i++)
	{
		fscanf(_f, "%f %f %d", &start, &end, &room);
		_ad->m_meetingsArray[i] = CreateNewMeeting(start, end, room);
	}
}



