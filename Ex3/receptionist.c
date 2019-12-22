

// Includes --------------------------------------------------------------------

#include "receptionist.h"





// Function Definitions --------------------------------------------------------

void* Room_default(Room *room) {

	room->name = (char*)malloc(MAX_STRING * sizeof(char));
	room->price = (int*)calloc(1, sizeof(int));
	room->price = (int*)calloc(1, sizeof(int));
	room->size = (int*)calloc(1, sizeof(int));

} 
void* Guest_default(Guest *guest) {

	guest->name = (char*)malloc(MAX_STRING * sizeof(char));
	guest->money = (int*)calloc(1, sizeof(int));

}
void* Hotel_default(Hotel *hotel) {

	hotel->room_ptr = (Room*)malloc(sizeof(Room));
	hotel->guest_ptr = (Guest*)malloc(sizeof(Guest));

}
static DWORD ChkRoomStatus(LPVOID lpParam)
{
	DWORD wait_res;
	BOOL release_res;
	LONG previous_count;
	while (TRUE)
	{
		/* Sleep for the purposes of the demonstration */
		Sleep(PRODUCER_WAIT_TIME_IN_MILISECONDS);

		wait_res = WaitForSingleObject(room_sem, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram(); // if succeds, the semphore then room has space
		printf("%s\n",wait_res);
		
		//produce_item(); // need to change it to a realfumction

		/* Start Critical Section */

		//insert_item(items);// 

		/* End Critical Section */
		
	

		release_res = ReleaseSemaphore(
			room_sem,
			1, 		/* Signal that exactly one cell was emptied */
			&previous_count);
		if (release_res == FALSE) ReportErrorAndEndProgram();

		//consume_item(item);
		printf("Consumer used one item. Previous count is: %ld\n", previous_count);

	}

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
	Guest *guest;
	Hotel *hotel;

	//Integration code -------------------------------------------------------
	room = (Room*)malloc(sizeof(Room));// multiply by the amount of ID's in the filr
	Room_default(room);
	strcpy(room->name, "Twin-Room");
	room->price = 10;
	room->size = 2;

	guest = (Guest*)malloc(sizeof(Guest));
	Guest_default(guest);
	strcpy(guest->name, "John Doe");
	guest->money = 50;

	hotel = (Hotel*)malloc(sizeof(Hotel));




	//------------------------------------------------------------------------
	room_sem = CreateSemaphore(
		NULL,	/* Default security attributes */
		N,		/* Initial Count - all slots are room_sem currently 2 */
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

static void check_in(int item)
{
	/* dummy implementation, supplied just to make the program compile */
}

static void check_out()
{
	/* dummy implementation, supplied just to make the program compile */
	return 0;
}

static void consume_item(int item)
{
	/* dummy implementation, supplied just to make the program compile */
}
