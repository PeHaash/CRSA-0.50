#pragma once
// Macros for ENV_002 : Putter
#define __CUST_ENV__ Env_Putter

#ifdef _MSC_VER
	#define ROOM_COUNT 3
	#define MAX_SS_PER_ROOM 4
	#define FURNITURE_COUNT 4
	#define MAX_X 10
	#define MAX_Y 20
	#define GOAL_AREAS { 0.3,0.4,0.3 }
	#define PENALIZED_CONSTANT -5
	#define ALPHA 1
	#define BETA 0.2
	#define MAX_STEPS 10 // TODO: Make max-steps + termination better
	#define RESET_GRIDS_COUNT 5 /// DONT FORGET TO ADD GRID_ID TO THE WHOLE SHARED STUFF
#endif

