/* Main application file: */

#include <stdlib.h>
#include <stdio.h>
#include "Meeting.h"
#include "DailyAppointmentDiary.h"

typedef enum Option {CREATE_AD = 1, CREATE_MEETING, INSERT_APPOINTMENT, REMOVE_APPOINTMENT, FIND_APPOINTMENT, DESTROY_AD, PRINT_AD, QUIT} Option;

void RunDailyAppointmentDiaryApp(void); /* Main application engine */
void ShowMenu(void);
void HandleCreateAD(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandleCreateMeeting(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandleInsertAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandleRemoveAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandleFindAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandleDestroyAD(DailyAppointmentDiary** _ad, Meeting** _meeting);
void HandlePrintAD(DailyAppointmentDiary** _ad, Meeting** _meeting);


int main(void)
{
	RunDailyAppointmentDiaryApp();
	return 0;
}


void RunDailyAppointmentDiaryApp(void)
{
	Option opt;
	int inputNum;
	int isQuitOption = 0; /* A bool flag */
	Meeting* meeting = NULL;
	DailyAppointmentDiary* ad = NULL;
	
	
	while(!isQuitOption)
	{
		ShowMenu();

		scanf("%d", &inputNum);
		opt = inputNum;

		switch(opt)
		{
			case CREATE_AD:
			{
				HandleCreateAD(&ad, &meeting);
				break;
			}
			
			case CREATE_MEETING:
			{
				HandleCreateMeeting(&ad, &meeting);
				break;
			}
			
			case INSERT_APPOINTMENT:
			{
				HandleInsertAppointment(&ad, &meeting);
				break;
			}
			
			case REMOVE_APPOINTMENT:
			{
				HandleRemoveAppointment(&ad, &meeting);
				break;
			}
			
			case FIND_APPOINTMENT:
			{
				HandleFindAppointment(&ad, &meeting);
				break;
			}
			
			case DESTROY_AD:
			{
				HandleDestroyAD(&ad, &meeting);
				break;
			}
			
			case PRINT_AD:
			{
				HandlePrintAD(&ad, &meeting);
				break;
			}
			
			case QUIT:
			{
				isQuitOption = 1; /* True */
				break;
			}
			
			default:
			{
				printf("Error: not a valid input...\n");
				break;
			}
		}
	}
}


void ShowMenu(void)
{

}


void HandleCreateAD(DailyAppointmentDiary** _ad, Meeting** _meeting)
{

}


void HandleCreateMeeting(DailyAppointmentDiary** _ad, Meeting** _meeting)
{

}


void HandleInsertAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting)
{
	/* TODO */
}


void HandleRemoveAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting)
{
	/* TODO */
}


void HandleFindAppointment(DailyAppointmentDiary** _ad, Meeting** _meeting)
{
	/* TODO */
}


void HandleDestroyAD(DailyAppointmentDiary** _ad, Meeting** _meeting)
{
	/* TODO */
}


void HandlePrintAD(DailyAppointmentDiary** _ad, Meeting** _meeting)
{
	/* TODO */
}




