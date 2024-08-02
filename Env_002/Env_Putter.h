#pragma once
#include "cstdint"

#include "Macros.h"
#include "Shared.h"
#include "Objectives.h"
#include "DisjointSet.h"

constexpr int UNAVAILABLE = -2;
constexpr int EMPTY = -1;
constexpr int NO_WALL = 0;
constexpr int IS_WALL = 1;
constexpr int IS_WINDOW = 2;

// grids
constexpr int FREE = 0;
constexpr int BLOCKED = -1;
constexpr int ACCESS = 1;
constexpr int OUTSIDE = 2;
// 0: Free, -1: Blocked, 1: Acsess, 2: Outside

constexpr int NO_CIRCULATION = 0, IS_CIRCULATION = 1;

constexpr int MAX_SUBSPACE = ROOM_COUNT * MAX_SS_PER_ROOM;


struct Subspace {
	int x1, y1, x2, y2;
	bool SharesAWallWith(Subspace b)const {
		if (this->x1 - 1 == b.x2 || this->x2 == b.x1 - 1) // vertical connections
			if (this->y1 <= b.y2 && this->y2 >= b.y1) return true;
		if (this->y1 - 1 == b.y2 || this->y2 == b.y1 - 1) // horizantal
			if (this->x1 <= b.y1 && this->x2 >= b.x1) return true;
		return false;
	}
};

struct Furniture {};

class Env_Putter {
public:
	Shared shared_data; // data shared between the front & back
private:

	int RoomAreaCount[ROOM_COUNT] = { 0 };
	const double RoomAreaGoal[ROOM_COUNT] = GOAL_AREAS;
	int InputGrid[MAX_X + 2][MAX_Y + 2] = { 0 }; // 0: Free, -1: Blocked, 1: Acsess, 2: Outside
	int SubspaceGrid[MAX_X + 2][MAX_Y + 2] = { -1 }; // -1: empty, -2: unavailable
	int FurnitureGrid[MAX_X + 2][MAX_Y + 2] = { -1 }; // -1: empty, -2: unavailable
	int RoomGrid[MAX_X + 2][MAX_Y + 2] = { -1 }; // -1: empty
	int WallGrid[MAX_X + 2][MAX_Y + 2]; // 0: no wall, 1: wall, 2: window
	int CirculationGrid[MAX_X + 2][MAX_Y + 2]; // 0: no circulation, 1: circulation
	int NumberOfSubspacesMade[ROOM_COUNT] = { 0 };
	bool RoomConnectedToCirculation[ROOM_COUNT];
	DisjointSet RoomsConnections; // = {ROOM_COUNT};
	// keep subspaces positions
	Subspace Subspaces[MAX_SUBSPACE] = { -1 };
	Subspace Furnitures[FURNITURE_COUNT] = { -1 };
	DisjointSet SubspaceConnections[ROOM_COUNT];// = {MAX_SS_PER_ROOM}; 
	// keep door positions
	DisjointSet2D CirculationConnections;
	int NumberOfGridsUsedInCirulation;
	bool CirculationAccessToOutside;
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
	bool CheckCirculationsConnectivity();
	bool CheckCirculationAccessToOutside();
	bool CheckAllRoomsConnectedToCirculation();
	bool CheckAllRoomsConnectedTogether();

private:
	// step subprocesses
	void AddCirculation(int x1, int y1, int x2, int y2);
	void AddWindow(int x1, int y1, int x2, int y2);
	void AddEntrance(int x1, int y1, int x2, int y2);
	void AddSubspace(int x1, int y1, int x2, int y2, int subspace_id);
	void AddFurniture(int x1, int y1, int x2, int y2, int furniture_id);
private:
	// helper functions for step
	void UpdateWallsOfSubspace(int subspace_id);

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

