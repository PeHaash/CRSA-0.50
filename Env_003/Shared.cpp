#include "Shared.h"
#include "Env_Room_Putter.h"

Shared::Shared(Env_Room_Putter& env, int objectives_count) {
	CustEnv = static_cast<void*>(&env);
	Grid_id = 0;
	InputGrid = &env.InputGrid[0][0];
	SubspaceGrid = &env.SubspaceGrid[0][0];
	RoomGrid = &env.RoomGrid[0][0];
	FurnitureGrid = &env.FurnitureGrid[0][0];
	CirculationGrid = &env.CirculationGrid[0][0];
	RoomAreaCount = &env.RoomAreaCount[0];
	UsedSpaceCount = &env.UsedSpaceCount;
	NumberOfSubspacesMade = &env.NumberOfSubspacesMade[0];
	RoomConnectedToCirculation = &env.RoomConnectedToCirculation[0];
	Objectives = reinterpret_cast<double*> (&env.Scores);
	ObjectivesCount = objectives_count;
	x1 = y1 = x2 = y2 = 0;
	type = 0;
	Terminated = Truncated = false;
	Score = 0;
}