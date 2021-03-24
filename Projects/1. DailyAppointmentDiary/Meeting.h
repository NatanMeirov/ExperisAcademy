/* Meeting header file */
#ifndef __MEETING_H__
#define __MEETING_H__

/* Defines: */
typedef struct Meeting Meeting;

typedef enum MeetingStatus {OK, ALLOCATION_FAILED_ERROR, WRONG_TIME_MEETING_ERROR, NULL_PTR_ERROR} MeetingStatus;


/*
- Decription: Creates and return a pointer to a new Meeting, initialized with wanted data

- Input: [_meetingStartingTime:] meeting starting time (float), [_meetingEndingTime:] meeting ending time (float), [_meetingRoomNumber:] meeting room number (unsigned int)

- Output: A pointer to the newly created Meeting, or NULL if error

- Errors: (MeetingStatus types:) ALLOCATION_FAILED_ERROR (if failed to allocate memory), WRONG_TIME_MEETING_ERROR (if the time is not between 0.0 to 23.99 or starting meeting time is higher then the ending meeting time)
*/
Meeting* CreateNewMeeting(float _meetingStartingTime, float _meetingEndingTime, unsigned int _meetingRoomNumber);


/*
- Decription: Deletes a Meeting (releasing its memory), validates a single use only of free

- Input: [_m:] a pointer to a Meeting to delete (to free)

- Output: None

- Errors: (MeetingStatus types:) NULL_PTR_ERROR (if NULL pointer)
*/
void DeleteMeeting(Meeting* _m);


/*------------------------------------------------------------ Getters: -------------------------------------------------------------------*/

/*
- Decription: Returns a meeting starting time

- Input: [_m:] a pointer to a Meeting

- Output: The meeting starting time (float), or -1 if error

- Errors: (MeetingStatus types:) NULL_PTR_ERROR (if NULL pointer)
*/
float GetMeetingStartingTime(Meeting* _m);


/*
- Decription: Returns a meeting ending time

- Input: [_m:] a pointer to a Meeting

- Output: The meeting ending time (float), or -1 if error

- Errors: (MeetingStatus types:) NULL_PTR_ERROR (if NULL pointer)
*/
float GetMeetingEndingTime(Meeting* _m);


/*
- Decription: Returns a meeting room number

- Input: [_m:] a pointer to a Meeting

- Output: The meeting room number (int), or -1 if error

- Errors: (MeetingStatus types:) NULL_PTR_ERROR (if NULL pointer)
*/
int GetMeetingRoomNumber(Meeting* _m);


#endif /* #ifndef __MEETING_H__ */
