/* Daily Appointment Diary header file */
#ifndef __DAILYAPPOINTMENTDIARY_H__
#define __DAILYAPPOINTMENTDIARY_H__

/* Defines: */

typedef struct DailyAppointmentDiary DailyAppointmentDiary;


typedef enum DADStatus {OK, NULL_PTR_ERROR, ALLOCATION_FAILED_ERROR, REALLOCATION_FAILED_ERROR, UNDERFLOW_ERROR, CLASH_MEETINGS_ERROR, MEETING_NOT_FOUND_ERROR, FILE_NOT_CREATED_ERROR, FILE_NOT_FOUND_ERROR} DADStatus;



/*
- Description: Creates and returns a pointer to a new DailyAppointmentDiary

- Input: None

- Output: A pointer to the newly created DailyAppointmentDiary, or NULL if error

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL),  ALLOCATION_FAILED_ERROR (if allocation failed)
*/
DailyAppointmentDiary* CreateNewDailyAppointmentDiary();






/*
- Description: Destroys a DailyAppointmentDiary (releasing its memory), validates not double free attempts

- Input: [_ad:] a pointer to a DailyAppointmentDiary to free

- Output: None

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL)
*/
DADStatus DestroyDailyAppointmentDiary(DailyAppointmentDiary* _ad);






/*
- Description: Inserts a new Meeting to DailyAppointmentDiary in order (sorted by meeting time), validates no clashing Meetings

- Input: [_ad:] a pointer to a DailyAppointmentDiary, [_newMeeting:] a pointer to the new Meeting to insert

- Output: OK if succeed, else error

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL), REALLOCATION_FAILED_ERROR (if failed to reallocate more memory), CLASH_MEETINGS_ERROR (if the given Meeting is clashing with other meetings)
*/
DADStatus InsertNewDailyAppointmentToDiary(DailyAppointmentDiary* _ad, Meeting* _newMeeting);







/*
- Description: Removes a Meeting from DailyAppointmentDiary by its starting time, keeping the order of the meetings

- Input: [_ad:] a pointer to a DailyAppointmentDiary, [_meetingStartingTime:] meeting starting time (to remove), [_meetingToRemove:] a reference to a location of a pointer to store the removed meeting

- Output: (DADStatus types:) OK if succeed, else error

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL), MEETING_NOT_FOUND_ERROR (if didnt found a match by meeting starting time), UNDERFLOW_ERROR (if there are no meetings in the DailyAppointmentDiary)
*/
DADStatus RemoveDailyAppointmentFromDiary(DailyAppointmentDiary* _ad, float _meetingStartingTime, Meeting** _meetingToRemove);






/*
- Description: Searches for a Meeting in DailyAppointmentDiary, by meeting starting time, and returns its index in the DailyAppointmentDiary, if the meeting starting time does not match to any Meeting, returns -1

- Input: [_ad:] a pointer to a DailyAppointmentDiary, [_meetingStartingTime:] meeting starting time

- Output: Index of the found Meeting, or -1 if did not found

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL), MEETING_NOT_FOUND_ERROR (if didnt found a match by meeting starting time)
*/
int FindDailyAppointmentInDiary(DailyAppointmentDiary* _ad, float _meetingStartingTime);






/*
- Description: Prints all daily meetings in order (sorted by meeting times)

- Input: [_ad:] a pointer to a DailyAppointmentDiary

- Output: None

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL)
*/
void PrintAllDailyMeetings(DailyAppointmentDiary* _ad);




/*
- Description: Saves a DailyAppointmentDiary data to a file

- Input: [_ad:] a pointer to a DailyAppointmentDiary

- Output: OK if succeed, else error

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL), FILE_NOT_CREATED_ERROR (if file creation failed)
*/
DADStatus SaveDailyAppointmentDiaryToFile(DailyAppointmentDiary* _ad);




/*
- Description: Loads a DailyAppointmentDiary data from a file

- Input: [_ad:] a pointer to a DailyAppointmentDiary

- Output: OK if succeed, else error

- Errors: (DADStatus types:) NULL_PTR_ERROR (if pointer is NULL), FILE_NOT_FOUND_ERROR (if failed to find the data file), ALLOCATION_FAILED_ERROR (if allocation failed when trying to allocate memory to store the saved data)
*/
DADStatus LoadDailyAppointmentDiaryFromFile(DailyAppointmentDiary* _ad)


#endif /* #ifndef __DAILYAPPOINTMENTDIARY_H__ */
