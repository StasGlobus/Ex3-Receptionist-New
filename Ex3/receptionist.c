

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
	guest->checked_in = (bool*)malloc(1, sizeof(bool));
	guest->designated_room = (Room*)malloc(sizeof(Room));


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
	Guest *guest = lpParam;
	
	
		/* Sleep for the purposes of the demonstration */
		Sleep(PRODUCER_WAIT_TIME_IN_MILISECONDS);

		wait_res = WaitForSingleObject(room_sem, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram(); // if succeds, the semphore then room has space
		//printf("%s\n",wait_res);
		while(TRUE) 
		{
			check_out(guest);
			if (guest->money)
				check_in(guest);
			else
			{
				release_res = ReleaseSemaphore(
					room_sem,
					1, 		/* Signal that exactly one cell was emptied */
					&previous_count);
				if (release_res == FALSE) ReportErrorAndEndProgram();
				break;
			}
			
		}
		//consume_item(item);
	//	printf("Consumer used one item. Previous count is: %ld\n", previous_count);

	

	return 0;
}

int main()
{
	Room *rooms=NULL;
	char path[MAX_STRING] = { '\0' };
	rooms = (Room*)malloc(sizeof(Room));
	strcpy(path,"C:\\Users\\stani\\OneDrive\\Documents\\Limudim\\OS");
	init_rooms(path, rooms);
	printf("%s\n", rooms->name);
	//reception();
	getchar(); // stops the cmd window from closing
}

static void reception()
{
	
	BOOL an_error_occured = FALSE;
	//int   p_action_list[] = { 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 };
	LPTHREAD_START_ROUTINE p_thread_routine = (LPTHREAD_START_ROUTINE)ChkRoomStatus;
	HANDLE p_thread_handles[NUM_OF_THREADS];
	int i = 0;
	//int   p_thread_indices[] = { 1,2,3,4,5,6,7,8,9,10 };
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
	guest->checked_in = FALSE;
	guest->designated_room = room;
	guest->day = 1;

	hotel = (Hotel*)malloc(sizeof(Hotel));
	Hotel_default(hotel);
	hotel->room_ptr = room;
	hotel->guest_ptr = guest;

	//------------------------------------------------------------------------
	room_sem = CreateSemaphore(
		NULL,	/* Default security attributes */
		N,		/* Initial Count - all slots are room_sem currently 2 should be set according to room size */
		N,		/* Maximum Count should be set according to room size*/
		NULL); /* un-named */

	if (room_sem == NULL) { an_error_occured = TRUE; goto Main_Cleanup_0; }



	/* create the threads */
	for (i = 0; i < NUM_OF_THREADS; i++) { // num of threads shoud be num of guests?
		p_thread_handles[i] = CreateThread(
			NULL,                                /* default security attributes */
			0,                                   /* use default stack size */
			p_thread_routine, /* thread function */
			guest,//&p_thread_indices[i],                /* argument to thread function  */
			0,                                   /* use default creation flags */
			&thread_id                           /* returns the thread identifier  */
		);

		if (p_thread_handles[i] == NULL) {
			printf("CreateThread %s failed.\n", hotel->guest_ptr->name);
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



static void check_in(Guest *guest)
{
	if (!(guest->checked_in))
	{
		guest->checked_in = TRUE;
		printf("\n%s %s IN %d\n", guest->designated_room->name, guest->name, guest->day);
	}
	guest->money = (guest->money) - (guest->designated_room->price);	
	(guest->day)++;
}

static void check_out(Guest *guest)
{
	if (guest->checked_in)
	{
		if (!(guest->money))
			printf("\n%s %s OUT %d\n", guest->designated_room->name, guest->name, guest->day);
	}
}


static void init_rooms(char path[], Room *rooms) // create a semaphore for each room
{
	//Room *rooms;
	FILE *ptr;
	int i = 0;
	char adress[MAX_STRING] = { '\0' };
	//rooms = (Room*)malloc(sizeof(Room));// multiply by the amount of ID's in the filr
	Room_default(rooms);
	memcpy(adress, path, MAX_STRING);
	strcat(adress, "\\rooms.txt");
	ptr = fopen(adress, "r");
	if (NULL == ptr) {
		perror("Could not open rooms.txt");
			return 1;
	  }
	else {
		while (!feof(ptr)) {
			char price[5] = { '\o' }, size[5] = { '\o' };
			fscanf(ptr, "%s %s %s", (rooms + i)->name, price, size );
			(rooms + i)->price= atoi(price);
			(rooms + i)->size = atoi(size);
			i++;
			rooms = (Room*)realloc(rooms, sizeof(Room));
			Room_default(rooms + i);
		}
	}
	
	
	//return rooms;
}

static Guest* init_guests(char path) // create a thread for each guest
{
	Guest *guests;

	return guests;
}
static Hotel* init_hotel(Room *rooms, Guest *guest) // match designated room for each guest
{
	Hotel *hotel;

	return hotel;
	
}