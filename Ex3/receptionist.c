

// Includes --------------------------------------------------------------------

#include "receptionist.h"





// Function Definitions --------------------------------------------------------

void* Room_default(Room *room) {

	room->name = (char*)malloc(MAX_STRING * sizeof(char));
	room->price = (int*)clloc(sizeof(int));
	room->price = (int*)clloc(sizeof(int));
	room->size = (int*)calloc(1, sizeof(int));

} 

static DWORD ChkRoomStatus(LPVOID lpParam)
{


	return 0;
}

int main()
{
	reception();
}

static void reception()
{
	
	BOOL an_error_occured = FALSE;
	//int   p_action_list[] = { 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 };
	LPTHREAD_START_ROUTINE p_thread_routine = (LPTHREAD_START_ROUTINE)ChkRoomStatus;
	HANDLE p_thread_handles[NUM_OF_THREADS];
	int i = 0;
	int   p_thread_indices[] = { 1,2,3,4,5,6,7,8,9,10 };
	DWORD thread_id;
	Room *room;

	//Integration code -------------------------------------------------------
	room = (Room*)malloc(sizeof(Room));// multiply by the amount of ID's in the filr
	Room_default(room);
	strcpy(room->name, "Twin-Room");
	room->price = 10;
	room->size = 2;
	//------------------------------------------------------------------------
	room_sem = CreateSemaphore(
		NULL,	/* Default security attributes */
		N,		/* Initial Count - all slots are room_sem */
		N,		/* Maximum Count */
		NULL); /* un-named */

	if (room_sem == NULL) { an_error_occured = TRUE; goto Main_Cleanup_0; }



	/* create the threads */
	for (i = 0; i < NUM_OF_THREADS; i++) {
		p_thread_handles[i] = CreateThread(
			NULL,                                /* default security attributes */
			0,                                   /* use default stack size */
			p_thread_routine, /* thread function */
			&p_thread_indices[i],                /* argument to thread function  */
			0,                                   /* use default creation flags */
			&thread_id                           /* returns the thread identifier  */
		);

		if (p_thread_handles[i] == NULL) {
			printf("CreateThread %d failed.\n", p_thread_indices[i]);
			an_error_occured = TRUE; goto Main_Cleanup_3;
		}
	}


	DWORD wait_code;
	wait_code = WaitForMultipleObjects(
		NUM_OF_THREADS,
		p_thread_handles,
		TRUE,			/* wait for all threads to finish */
		TIMEOUT_IN_MILLISECONDS);


Main_Cleanup_3:
	for (i = 0; i < NUM_OF_THREADS; i++) {
		CloseHandle(p_thread_handles[i]);
	}
Main_Cleanup_1:
	CloseHandle(room_sem);
Main_Cleanup_0:
	if (an_error_occured) ReportErrorAndEndProgram();
}







static void ReportErrorAndEndProgram()
{
	printf("Encountered error, ending program. Last Error = 0x%x\n", GetLastError());
	exit(1);
}

static int produce_item()
{
	/* dummy implementation, supplied just to make the program compile */
	return 0;
}

static void insert_item(int item)
{
	/* dummy implementation, supplied just to make the program compile */
}

static int remove_item()
{
	/* dummy implementation, supplied just to make the program compile */
	return 0;
}

static void consume_item(int item)
{
	/* dummy implementation, supplied just to make the program compile */
}
