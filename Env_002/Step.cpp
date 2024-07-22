#include "Env_Putter.h"


int32_t Env_Putter::Step() {
	// code is 1-based, but the input is 0 based, so... +1,
	// ++ to keep 1-index
	int x1 = shared_data.x1 + 1, x2 = shared_data.x2 + 1, y1 = shared_data.y1 + 1, y2 = shared_data.y2 + 1, type = shared_data.type;
	if (x2 < x1) std::swap(x1, x2);
	if (y2 < y1) std::swap(y1, y2); // TODO: maybe better to get rid of these two lines for swapping

	Penalized = false;

	if (type == 0 || type == 1) {
		AddDoor(x1, y1, x2, y2);
	}
	else if (type == 2) {
		AddWindow(x1, y1, x2, y2);
	}
	else if (type == 3) {
		AddEntrance(x1, y1, x2, y2);
	}
	else if (type < 4 + MAX_SUBSPACE) {
		AddSubspace(x1, y1, x2, y2, type - 4);
	}
	else {
		AddFurniture(x1, y1, x2, y2, type - (4 + MAX_SUBSPACE));
	}
	if (Penalized == false) {
		UpdateScores();
		double NewScore = Objectives::Average(Scores, Weights);
		double delta = NewScore - PreviousScore;
		PreviousScore = NewScore;
		shared_data.Score = delta;
	}
	else {
		shared_data.Score = PENALIZED_CONSTANT;
	}

	StepCount++;
	shared_data.Terminated = (StepCount >= MAX_STEPS); // Sol
	shared_data.Truncated = false;
	return 0;
}




void Env_Putter::AddDoor(int x1, int y1, int x2, int y2)
{
	// TODO: Add Door
}

void Env_Putter::AddWindow(int x1, int y1, int x2, int y2)
{
	// TODO: Add Window
}

void Env_Putter::AddEntrance(int x1, int y1, int x2, int y2)
{
	// TODO: Add Entrance
}

void Env_Putter::AddSubspace(int x1, int y1, int x2, int y2, int subspace_id)
{

	if (Subspaces[subspace_id].x1 != -1) {
		// we have put it before
		Penalized = true;
		return;
	}
	// check if the ss in covering other things
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (Grid[i][j] != -1) {
				Penalized = true;
				return;
			}
		}
	}
	// no problem!
	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++)
			Grid[i][j] = subspace_id;

	Subspaces[subspace_id] = { x1,y1,x2,y2 };
	int room = subspace_id / MAX_SS_PER_ROOM;
	int room_first_ss_id = room * MAX_SS_PER_ROOM;
	//std::cout <<"--<<___>> "<<room<<std::endl;
	int Area = (x2 - x1 + 1) * (y2 - y1 + 1);

	UsedSpaceCount += Area;
	RoomAreaCount[room] += Area;
	NumberOfSubspacesMade[room]++;
	for (int i = room_first_ss_id; i < room_first_ss_id + MAX_SS_PER_ROOM; i++) {
		if (i != subspace_id && Subspaces[subspace_id].SharesAWallWith(Subspaces[i])) {
			SubspaceConnections[room].Join(i - room_first_ss_id, subspace_id - room_first_ss_id);
		}
	}

}

void Env_Putter::AddFurniture(int x1, int y1, int x2, int y2, int type)
{
	// TODO: AddFurniture
}
