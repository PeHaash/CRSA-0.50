#include "Env_Putter.h"


#define E_LOW(x)  (((x-1)/ CIRCULATION_GRID_SIZE) * CIRCULATION_GRID_SIZE + 1)
#define E_HIGH(x) (((x-1)/ CIRCULATION_GRID_SIZE) * CIRCULATION_GRID_SIZE + CIRCULATION_GRID_SIZE)


int32_t Env_Putter::Step() {
	// code is 1-based, but the input is 0 based, so... +1,
	// ++ to keep 1-index
	int x1 = shared_data.x1 + 1, x2 = shared_data.x2 + 1, y1 = shared_data.y1 + 1, y2 = shared_data.y2 + 1, type = shared_data.type;
	if (x2 < x1) std::swap(x1, x2);
	if (y2 < y1) std::swap(y1, y2); // TODO: maybe better to get rid of these two lines for swapping

	Penalized = false;

	if (type == 0) {
		AddWindow(x1, y1, x2, y2);
	}
	else if (type == 1) {
		AddCirculation(E_LOW(x1), E_LOW(y1), E_HIGH(x2), E_HIGH(y2));
	}
	//else if (type == 3) {
	//	AddEntrance(x1, y1, x2, y2);
	//}
	else if (type < 2 + MAX_SUBSPACE) {
		AddSubspace(x1, y1, x2, y2, type - 2);
	}
	else {
		AddFurniture(x1, y1, x2, y2, type - (2 + MAX_SUBSPACE));
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
	shared_data.Terminated = (StepCount >= MAX_STEPS);
	shared_data.Truncated = false;
	return 0;
}




void Env_Putter::AddWindow(int x1, int y1, int x2, int y2)
{
	// TODO: Add Window
}



void Env_Putter::AddFurniture(int x1, int y1, int x2, int y2, int furniture_id)
{
	// TODO: AddFurniture
	// check if we put this before
	if (Furnitures[furniture_id].x1 != -1) {
		// we have put it before
		Penalized = true;
		return;
	}

	// check if the furniture is out of rooms, is on the wall, or other furnitures
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (RoomGrid[i][j] == EMPTY || WallGrid[i][j] != NO_WALL || FurnitureGrid[i][j] != EMPTY) {
				Penalized = true;
				return;
			}
		}
	}

	// no problem, let's put it
	Furnitures[furniture_id] = { x1,y1,x2,y2 };
	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++)
		{
			FurnitureGrid[i][j] = furniture_id;
		}

}


