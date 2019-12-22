#ifndef __RECEPTIONIST_H__
#define __RECEPTIONIST_H__
#define _CRT_SECURE_NO_WARNINGS
// Includes --------------------------------------------------------------------
#include "windows.h"
#include <stdlib.h>
#include <stdio.h>

//structs ----------------------------------------------------------------------
typedef struct {

	char  *name; // name of the room
	int price; // price for single person for single night
	int size; // The amount of pepople the room can hold at the same time
	
}Room; 
typedef struct {

	char  *name; // name of the guest
	int money; // amount of monet guest has 
	 

}Guest;

typedef struct {

	Room  *room_ptr; // name of the guest
	Guest *guest_ptr; // amount of monet guest has 


}Hotel;

// Constants  ------------------------------------------------------------------

#define NUM_OF_THREADS 3
#define MAX_STRING 100
static const int TIMEOUT_IN_MILLISECONDS = 10 * 1000;
static const int PRODUCER_WAIT_TIME_IN_MILISECONDS = 250;
static const int CONSUMER_WAIT_TIME_IN_MILISECONDS = 500; /* Unequal wait times make the demonstration clearer */
static const int N = 2; // integration number of places in one room_sem

// Variables -------------------------------------------------------------------

static HANDLE room_sem; //semaphoe
//static HANDLE full;//semaphore
//static HANDLE mutex;
static int items = 20;

// Function Declarations -------------------------------------------------------

static void reception();
static DWORD Producer(LPVOID Dummy);
static void ReportErrorAndEndProgram();
static int produce_item();
static void insert_item(int item);
static int remove_item();
static void consume_item(int item);


#endif //__RECEPTIONIST_H__