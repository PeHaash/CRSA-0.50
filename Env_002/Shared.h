#pragma once

struct Shared {
	void* CustEnv;
	// Reset Options: 
	int Grid_id;
	int* InputGrid;
	// Action:
	int x1, x2, y1, y2, type;
	// Observations:
	int* Grids;
	int* RoomAreaCount;
	int* UsedSpaceCount;

	// Score, Terminations:
	double Score;
	bool Terminated, Truncated;


	Shared(void* cust_env, int* input_grid, int* grid, int* room_area_count, int* used_space_count) :
		CustEnv(cust_env), InputGrid(input_grid), Grids(grid), RoomAreaCount(room_area_count), UsedSpaceCount(used_space_count) {
		Grid_id = 0;
		x1 = y1 = x2 = y2 = type = 0;

		Terminated = Truncated = false;
		Score = 0;
	}
	Shared() = default;
};
