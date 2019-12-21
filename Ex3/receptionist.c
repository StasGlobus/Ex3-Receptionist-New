// ProducerConsumerDemonstration.c

/*
* This is a c implementation for the problem discussed in the lecture.
* We have a producer thread, which creates messages, and a consumer thread,
* which reads them.
*/


/*
// Pseudo-code of the bottom example:
// ---------------------------------------

const int N = 100;
typedef int semaphore;
semaphore mutex = 1;
semaphore empty = N;
semaphore full = 0;

void producer()
{
	int item;
	while(true)
	{
		item = produce_item();
		down(&empty);
		down(&mutex);
		insert_item(item);
		up(&mutex);
		up(&full);
	}
}

 void consumer()
 {
	 int item;
	 while (true)
	 {
		 down(&full);
		 down(&mutex);
		 item = remove_item();
		 up(&mutex);
		 up(&empty);
		 consume_item(item);
	 }
}
*/

// Includes --------------------------------------------------------------------

#include "windows.h"
#include <stdlib.h>
#include <stdio.h>

// Constants  ------------------------------------------------------------------

#define NUM_OF_THREADS 2
static const int TIMEOUT_IN_MILLISECONDS = 10 * 1000;
static const int PRODUCER_WAIT_TIME_IN_MILISECONDS = 250;
static const int CONSUMER_WAIT_TIME_IN_MILISECONDS = 500; /* Unequal wait times make the demonstration clearer */
static const int N = 10;

// Variables -------------------------------------------------------------------

static HANDLE empty;
static HANDLE full;
static HANDLE mutex;
static int items = 20;

// Function Declarations -------------------------------------------------------

static void ProducerConsumerUsingSemaphores();
static DWORD Producer(LPVOID Dummy);
static DWORD Consumer(LPVOID Dummy);
static void ReportErrorAndEndProgram();
static int produce_item();
static void insert_item(int item);
static int remove_item();
static void consume_item(int item);

// Function Definitions --------------------------------------------------------

int main()
{
	ProducerConsumerUsingSemaphores();
}

static void ProducerConsumerUsingSemaphores()
{
	BOOL an_error_occured = FALSE;
	int   p_action_list[] = { 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 };
	LPTHREAD_START_ROUTINE p_thread_routines[] =
	{ (LPTHREAD_START_ROUTINE)Consumer,(LPTHREAD_START_ROUTINE)Producer };
	HANDLE p_thread_handles[NUM_OF_THREADS];
	int i = 0;
	int   p_thread_indices[] = { 1,2,3,4,5,6,7,8,9,10 };
	DWORD thread_id;

	empty = CreateSemaphore(
		NULL,	/* Default security attributes */
		N,		/* Initial Count - all slots are empty */
		N,		/* Maximum Count */
		NULL); /* un-named */

	if (empty == NULL) { an_error_occured = TRUE; goto Main_Cleanup_0; }

	full = CreateSemaphore(
		NULL,	/* Default security attributes */
		0,		/* Initial Count - no slots are full */
		N,		/* Maximum Count */
		NULL); /* un-named */

	if (full == NULL) { an_error_occured = TRUE; goto Main_Cleanup_1; }

	mutex = CreateMutex(
		NULL,   /* default security attributes */
		FALSE,	/* don't lock mutex immediately */
		NULL); /* un-named */

	if (mutex == NULL) { an_error_occured = TRUE; goto Main_Cleanup_2; }


	/* create the threads */
	for (i = 0; i < NUM_OF_THREADS; i++) {
		p_thread_handles[i] = CreateThread(
			NULL,                                /* default security attributes */
			0,                                   /* use default stack size */
			p_thread_routines[p_action_list[i]], /* thread function */
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
	CloseHandle(mutex);
Main_Cleanup_2:
	CloseHandle(full);
Main_Cleanup_1:
	CloseHandle(empty);
Main_Cleanup_0:
	if (an_error_occured) ReportErrorAndEndProgram();
}

static DWORD Producer(LPVOID Dummy)
{
	DWORD wait_res;
	BOOL release_res;
	LONG previous_count;

	while (TRUE)
	{
		/* Sleep for the purposes of the demonstration */
		Sleep(PRODUCER_WAIT_TIME_IN_MILISECONDS);

		produce_item();

		wait_res = WaitForSingleObject(empty, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram();

		wait_res = WaitForSingleObject(mutex, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram();

		/* Start Critical Section */

		insert_item(items);

		/* End Critical Section */

		release_res = ReleaseMutex(mutex);
		if (release_res == FALSE) ReportErrorAndEndProgram();

		release_res = ReleaseSemaphore(
			full,
			1, 		/* Signal that exactly one cell was filled */
			&previous_count);
		if (release_res == FALSE) ReportErrorAndEndProgram();
		printf("Producer inserted one item. Previous count is: %ld\n", previous_count);
	}

	return 0;
}

static DWORD Consumer(LPVOID Dummy)
{
	DWORD wait_res;
	BOOL release_res;
	LONG previous_count;

	int item;
	while (TRUE)
	{
		/* Sleep for the purposes of the demonstration */
		Sleep(CONSUMER_WAIT_TIME_IN_MILISECONDS);

		wait_res = WaitForSingleObject(full, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram();

		wait_res = WaitForSingleObject(mutex, INFINITE);
		if (wait_res != WAIT_OBJECT_0) ReportErrorAndEndProgram();

		/* Start Critical Section */

		item = remove_item(items);

		/* End Critical Section */

		release_res = ReleaseMutex(mutex);
		if (release_res == FALSE) ReportErrorAndEndProgram();

		release_res = ReleaseSemaphore(
			empty,
			1, 		/* Signal that exactly one cell was emptied */
			&previous_count);
		if (release_res == FALSE) ReportErrorAndEndProgram();

		consume_item(item);
		printf("Consumer used one item. Previous count is: %ld\n", previous_count);
	}

	return 0;
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
