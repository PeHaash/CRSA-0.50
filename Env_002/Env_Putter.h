#pragma once
#include "cstdint"

#include "Macros.h"
#include "Shared.h"
#include "Objectives.h"
#include "DisjointSet.h"


constexpr int MAX_SUBSPACE = ROOM_COUNT * MAX_SS_PER_ROOM;

struct Subspace {
	int x1, y1, x2, y2;
	bool SharesAWallWith(Subspace b)const {
		if (this->x1 + 1 == b.x2 || this->x2 == b.x1 + 1) // vertical connections
			if (this->y1 <= b.y2 && this->y2 >= b.y1) return true;
		if (this->y1 + 1 == b.y2 || this->y2 == b.y1 + 1) // horizantal
			if (this->x1 <= b.y1 && this->x2 >= b.x1) return true;
		return false;
	}
};

class Env_Putter {
public:
	Shared shared_data; // data shared between the front & back
private:

	int RoomAreaCount[ROOM_COUNT] = { 0 };
	const double RoomAreaGoal[ROOM_COUNT] = GOAL_AREAS;
	int InputGrid[MAX_X + 1][MAX_Y + 1] = { 0 };
	int Grid[MAX_X + 1][MAX_Y + 1] = { -1 }; // -1: empty, -2: unavailable
	int FurnitureGrid[MAX_X + 1][MAX_Y + 1] = { -1 }; // -1: empty, -2: unavailable
	int NumberOfSubspacesMade[ROOM_COUNT] = { 0 };
	DisjointSet RoomsConnections; // = {ROOM_COUNT};
	// keep subspaces positions
	Subspace Subspaces[MAX_SUBSPACE] = { -1 };
	DisjointSet SubspaceConnections[ROOM_COUNT];// = {MAX_SS_PER_ROOM}; 
	// keep door positions
	// keep windows positions
	int UsedSpaceCount, AllAvailableSpace;
	int StepCount = 0;
	double PreviousScore = 0; // we only send Delta!
	Objectives Weights, Scores;
	bool Penalized;
private:
	void UpdateScores();
	// Evaluator functions
	// bool CheckRoomAvailability(); deprecated
	bool CheckAvailableSpaceUsed();
	bool CheckRoomsCorrectArea();
	bool CheckRoomsInternalConnectedness();
private:
	// step subprocesses
	void AddDoor(int x1, int y1, int x2, int y2);
	void AddWindow(int x1, int y1, int x2, int y2);
	void AddEntrance(int x1, int y1, int x2, int y2);
	void AddSubspace(int x1, int y1, int x2, int y2, int type);
	void AddFurniture(int x1, int y1, int x2, int y2, int type);

public:
	Env_Putter();
	int32_t Reset();
	int32_t Step();
	//(int x1, int y1, int x2, int y2, int type);
	// gymnasium.Env.step(self, action: ActType) --> tuple[ObsType, SupportsFloat, bool, bool, dict[str, Any]]
	// Observation, Reward, Terminated, Turncated, info = Env.step(action)

	int32_t Render(); // TODO: implement Render
	int32_t Close(); // WATCH: implement close

};
