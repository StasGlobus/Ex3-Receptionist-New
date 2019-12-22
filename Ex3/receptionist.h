#ifndef __RECEPTIONIST_H__
#define __RECEPTIONIST_H__
#define _CRT_SECURE_NO_WARNINGS
// Includes --------------------------------------------------------------------
#include "windows.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//structs ----------------------------------------------------------------------
typedef struct {

	char  *name; // name of the room
	int price; // price for single person for single night
	int size; // The amount of pepople the room can hold at the same time
	
}Room; 
typedef struct {

	char *name; // name of the guest
	int money; // amount of monet guest has 
	bool checked_in;
	Room *designated_room;
	int	day;
	 

}Guest;

typedef struct {

	Room  *room_ptr; // name of the guest
	Guest *guest_ptr; // amount of monet guest has 


}Hotel;

// Constants  ------------------------------------------------------------------

#define NUM_OF_THREADS 1
#define MAX_STRING 500
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
static void ReportErrorAndEndProgram();
static void check_in(Guest *guest);
static void check_out(Guest *guest);
static Hotel* init_hotel(char path);
static void init_rooms(char path, Room *rooms);
static Guest* init_guests(Room *rooms, Guest *guest);
#endif //__RECEPTIONIST_H__