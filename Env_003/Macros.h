#pragma once
// Macros for ENV_002 : Putter
#define __CUST_ENV__ Env_Room_Putter

#ifdef _MSC_VER
	#define ROOM_COUNT 3
	#define MAX_SS_PER_ROOM 3
	#define FURNITURE_COUNT 30
	#define MAX_X 32
	#define MAX_Y 32
	#define GOAL_AREAS { 0.3,0.4,0.3 }
	#define PENALIZED_CONSTANT -5
	#define ALPHA 1
	#define BETA 0.2
	#define MAX_STEPS 50 // TODO: Make max-steps + termination better
	#define RESET_GRIDS_COUNT 30 /// DONT FORGET TO ADD GRID_ID TO THE WHOLE SHARED STUFF
	#define CIRCULATION_GRID_SIZE 4
	#define WEIGHTS {1.0,5,40,2.0,-1}
	#define ADJACENCY_MATRIX_GOAL {{0, 1, -1}, {1, 0, 1}, { -1, 1, 0}};
#endif

#define RECEIVES_POSITIONS_IN_FLOAT
#define NO_WALLS
//#undef RECEIVES_POSITIONS_IN_FLOAT
