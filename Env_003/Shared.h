#pragma once

class Env_Room_Putter;

struct Shared {
	void* CustEnv;
	// Reset Options: 
	int Grid_id;
	int* InputGrid;
	// Action:
	float x1, x2, y1, y2;
	int type;
	// Observations:
	int* SubspaceGrid;
	int* RoomGrid;
	int* FurnitureGrid;
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

	Shared(Env_Room_Putter& env, int objectives_count);
	Shared() = default; 
};
