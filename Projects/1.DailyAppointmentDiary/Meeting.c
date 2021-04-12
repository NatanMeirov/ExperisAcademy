/* Includes: */
#include <stdlib.h> /* Used for malloc, free */
#include "Meeting.h"

#define MAGIC_NUM 3758311

/* Defines: */
typedef struct Meeting
{
	float m_meetingStartingTime;
	float m_meetingEndingTime;
	unsigned int m_meetingRoomNumber;
	int m_magicNumber;
} Meeting;


/* Validation Functions Declarations: */
MeetingStatus ValidateMeetingTimes(float _meetingStartingTime, float _meetingEndingTime);
MeetingStatus ValidateSuccessfulAllocation(Meeting* _meeting);
MeetingStatus ValidateNotNullMeetingPtr(Meeting* _meeting);

/* Helper Functions Declarations: */
Meeting* InitializeNewMeeting(Meeting* _newMeeting, float _meetingStartingTime, float _meetingEndingTime, unsigned int _meetingRoomNumber);


/*------------------------------------------------------ Main API Functions --------------------------------------------------------------*/

Meeting* CreateNewMeeting(float _meetingStartingTime, float _meetingEndingTime, int _meetingRoomNumber)
{
	Meeting* newMeeting = NULL;
	
	if(ValidateMeetingTimes(_meetingStartingTime, _meetingEndingTime) == WRONG_TIME_MEETING_ERROR)
	{
		return NULL;
	}
	
	newMeeting = malloc(sizeof(Meeting));
	
	if(ValidateSuccessfulAllocation(newMeeting) == ALLOCATION_FAILED_ERROR)
	{
		return NULL;
	}
	
	return InitializeNewMeeting(newMeeting, _meetingStartingTime, _meetingEndingTime, _meetingRoomNumber);
}


void DeleteMeeting(Meeting* _m)
{
	if(ValidateNotNullPtr(_m) != NULL_PTR_ERROR && _m->m_magicNumber == MAGIC_NUM)
	{
		_m->m_magicNumber = 0;
		free(_m);
	}
}

/* Getters: */
float GetMeetingStartingTime(Meeting* _m)
{
	if(ValidateNotNullMeetingPtr(_m) == NULL_PTR_ERROR)
	{
		return -1;
	}
	
	return _m->m_meetingStartingTime;
}


float GetMeetingEndingTime(Meeting* _m)
{
	if(ValidateNotNullMeetingPtr(_m) == NULL_PTR_ERROR)
	{
		return -1;
	}
	
	return _m->m_meetingEndingTime;
}


int GetMeetingRoomNumber(Meeting* _m)
{
	if(ValidateNotNullMeetingPtr(_m) == NULL_PTR_ERROR)
	{
		return -1;
	}
	
	return _m->m_meetingRoomNumber;
}


/*-------------------------------------------------- End of Main API Functions -----------------------------------------------------------*/


/* Validation Functions: */
MeetingStatus ValidateMeetingTimes(float _meetingStartingTime, float _meetingEndingTime)
{
	if((0.0 <= _meetingStartingTime && _meetingStartingTime <= 23.99) && (0.0 <= _meetingEndingTime && _meetingEndingTime <= 23.99) && (_meetingStartingTime < _meetingEndingTime))
	{
		return OK;
	}
	else
	{
		return WRONG_TIME_MEETING_ERROR;
	}
}

MeetingStatus ValidateSuccessfulAllocation(Meeting* _meeting)
{
	return (!_meeting) ? ALLOCATION_FAILED_ERROR : OK;
}


MeetingStatus ValidateNotNullMeetingPtr(Meeting* _meeting)
{
	return (!_meeting) ? NULL_PTR_ERROR : OK;
}

/* Helper Functions: */
Meeting* InitializeNewMeeting(Meeting* _newMeeting, float _meetingStartingTime, float _meetingEndingTime, unsigned int _meetingRoomNumber)
{
	_newMeeting->m_meetingStartingTime = _meetingStartingTime;
	_newMeeting->m_meetingEndingTime = _meetingEndingTime;
	_newMeeting->m_meetingRoomNumber = _meetingRoomNumber;
	_newMeeting->m_magicNumber = MAGIC_NUM;
	
	return _newMeeting;
}







