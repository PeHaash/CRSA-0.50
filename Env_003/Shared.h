#pragma once


struct Shared {
	void* CustEnv;
	// Reset Options: 
	int Grid_id;
	int* InputGrid;
	// Action:
#ifdef RECEIVES_POSITIONS_IN_FLOAT
	float x1, x2, y1, y2;
#else
	int x1, x2, y1, y2;
#endif
	int type;
	// Observations:
	int* SubspaceGrid;
	int* RoomGrid;
	int* FurnitureGrid;
	int* WallGrid;
	int* CirculationGrid;
	int* RoomAreaCount;
	int* UsedSpaceCount;
	int* NumberOfSubspacesMade;
	bool* RoomConnectedToCirculation;
	double* Objectives;
	int ObjectivesCount;
	// Score, Terminations:
	double Score;
	bool Terminated, Truncated;

	Shared(void* cust_env, int* input_grid, int* subspace_grid, int* room_grid, int* furniture_grid, int* wall_grid, int* circulation_grid,
	int* room_area_count, int* used_space_count, int* number_of_ss_made, bool* room_connected_to_circulation, double* objectives, int obj_count) :
		CustEnv(cust_env), InputGrid(input_grid), SubspaceGrid(subspace_grid), RoomGrid(room_grid), FurnitureGrid(furniture_grid),
		WallGrid(wall_grid), CirculationGrid(circulation_grid), RoomAreaCount(room_area_count), UsedSpaceCount(used_space_count), 
		NumberOfSubspacesMade(number_of_ss_made), RoomConnectedToCirculation(room_connected_to_circulation), Objectives(objectives),
		ObjectivesCount(obj_count){
		
		Grid_id = 0;
		x1 = y1 = x2 = y2 = 0;
		type = 0;
		Terminated = Truncated = false;
		Score = 0;
	}
	Shared() = default; 
};
