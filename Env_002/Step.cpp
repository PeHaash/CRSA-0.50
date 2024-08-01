#include "Env_Putter.h"


int32_t Env_Putter::Step() {
	// code is 1-based, but the input is 0 based, so... +1,
	// ++ to keep 1-index
	int x1 = shared_data.x1 + 1, x2 = shared_data.x2 + 1, y1 = shared_data.y1 + 1, y2 = shared_data.y2 + 1, type = shared_data.type;
	if (x2 < x1) std::swap(x1, x2);
	if (y2 < y1) std::swap(y1, y2); // TODO: maybe better to get rid of these two lines for swapping

	Penalized = false;

	if (type == 0 || type == 1) {
		AddCirculation(x1, y1, x2, y2);
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
	shared_data.Terminated = (StepCount >= MAX_STEPS);
	shared_data.Truncated = false;
	return 0;
}




void Env_Putter::AddCirculation(int x1, int y1, int x2, int y2)
{
	// TODO: Add Door

	// Good spaces: inside Rooms or Acsess, not part of furnitures 
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (InputGrid[i][j] == ACCESS || (RoomGrid[i][j] != EMPTY && FurnitureGrid[i][j] == EMPTY)) {
				// fine, circulation in access or emptry parts of rooms
			}
			else {
				Penalized = true;
				return;
			}
		}
	}

	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (CirculationGrid[i][j] == NO_CIRCULATION) NumberOfGridsUsedInCirulation++;
			CirculationGrid[i][j] = IS_CIRCULATION;
			CirculationConnections.Join(i, j, x1, y1); // connecting all together
			if(InputGrid[i][j] == ACCESS) CirculationAccessToOutside = true;
			RoomConnectedToCirculation[RoomGrid[i][j]] = true;
		}
	}
	// connecting to borders:
	for (int i = x1; i <= x2; i++) {
		if (CirculationGrid[i][y1 - 1] == IS_CIRCULATION) CirculationConnections.Join(i, y1 - 1, x1, y1);
		if (CirculationGrid[i][y2 + 1] == IS_CIRCULATION) CirculationConnections.Join(i, y2 + 1, x1, y1);
	}
	for (int j = y1; j <= y2; j++) {
		if (CirculationGrid[x1 - 1][j] == IS_CIRCULATION) CirculationConnections.Join(x1 - 1, j, x1, y1);
		if (CirculationGrid[x2 + 1][j] == IS_CIRCULATION) CirculationConnections.Join(x2 + 1, j, x1, y1);
	}


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
	// check if we put this before
	if (Subspaces[subspace_id].x1 != -1) {
		// we have put it before
		Penalized = true;
		return;
	}

	// check if the ss is covering other things
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (SubspaceGrid[i][j] != EMPTY) {
				Penalized = true;
				return;
			}
		}
	}

	// no problem, let's put it
	Subspaces[subspace_id] = { x1,y1,x2,y2 };
	int room_id = subspace_id / MAX_SS_PER_ROOM;
	int room_first_ss_id = room_id * MAX_SS_PER_ROOM;
	int Area = (x2 - x1 + 1) * (y2 - y1 + 1);

	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++)
		{
			SubspaceGrid[i][j] = subspace_id;
			RoomGrid[i][j] = room_id;
		}

	UsedSpaceCount += Area;
	RoomAreaCount[room_id] += Area;
	NumberOfSubspacesMade[room_id]++;
	// adding walls of the new subspace:
	UpdateWallsOfSubspace(subspace_id);
	// add connections between ss of the same room
	for (int i = room_first_ss_id; i < room_first_ss_id + MAX_SS_PER_ROOM; i++) {
		if (i != subspace_id && Subspaces[subspace_id].SharesAWallWith(Subspaces[i])) {
			// connected subspace!
			UpdateWallsOfSubspace(i);
			SubspaceConnections[room_id].Join(i - room_first_ss_id, subspace_id - room_first_ss_id);
		}
	}



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




void Env_Putter::UpdateWallsOfSubspace(int subspace_id) {
	Subspace ss = Subspaces[subspace_id];
	int room_id = subspace_id / MAX_SS_PER_ROOM;

	// Add walls for the top and bottom
	for (int x = ss.x1 + 1; x <= ss.x2 - 1; x++) {
		bool top_has_different_room_neighbour =
			(RoomGrid[x - 1][ss.y1 - 1] != room_id || RoomGrid[x][ss.y1 - 1] != room_id || RoomGrid[x + 1][ss.y1 - 1] != room_id);
		WallGrid[x][ss.y1] = top_has_different_room_neighbour ? IS_WALL : NO_WALL;

		bool bottom_has_different_room_neighbour =
			(RoomGrid[x - 1][ss.y2 + 1] != room_id || RoomGrid[x][ss.y2 + 1] != room_id || RoomGrid[x + 1][ss.y2 + 1] != room_id);
		WallGrid[x][ss.y2] = bottom_has_different_room_neighbour ? IS_WALL : NO_WALL;
	}

	// Add walls for the left and right
	for (int y = ss.y1 + 1; y <= ss.y2 - 1; y++) {
		bool left_has_different_room_neighbour =
			(RoomGrid[ss.x1 - 1][y - 1] != room_id || RoomGrid[ss.x1 - 1][y] != room_id || RoomGrid[ss.x1 - 1][y + 1] != room_id);
		WallGrid[ss.x1][y] = left_has_different_room_neighbour ? IS_WALL : NO_WALL;

		bool right_has_different_room_neighbour =
			(RoomGrid[ss.x2 + 1][y - 1] != room_id || RoomGrid[ss.x2 + 1][y] != room_id || RoomGrid[ss.x2 + 1][y + 1] != room_id);
		WallGrid[ss.x2][y] = right_has_different_room_neighbour ? IS_WALL : NO_WALL;
	}


	// Add walls for the corners
	bool top_left_corner_has_different_room_neighbour =
		(RoomGrid[ss.x1 - 1][ss.y1 - 1] != room_id || RoomGrid[ss.x1 - 1][ss.y1] != room_id || RoomGrid[ss.x1 - 1][ss.y1 + 1] != room_id ||
			RoomGrid[ss.x1][ss.y1 - 1] != room_id || RoomGrid[ss.x1 + 1][ss.y1 - 1] != room_id);
	WallGrid[ss.x1][ss.y1] = top_left_corner_has_different_room_neighbour ? IS_WALL : NO_WALL;

	bool top_right_corner_has_different_room_neighbour =
		(RoomGrid[ss.x2 + 1][ss.y1 - 1] != room_id || RoomGrid[ss.x2 + 1][ss.y1] != room_id || RoomGrid[ss.x2 + 1][ss.y1 + 1] != room_id ||
			RoomGrid[ss.x2][ss.y1 - 1] != room_id || RoomGrid[ss.x2 - 1][ss.y1 - 1] != room_id);
	WallGrid[ss.x2][ss.y1] = top_right_corner_has_different_room_neighbour ? IS_WALL : NO_WALL;

	bool bottom_left_corner_has_different_room_neighbour =
		(RoomGrid[ss.x1 - 1][ss.y2 - 1] != room_id || RoomGrid[ss.x1 - 1][ss.y2] != room_id || RoomGrid[ss.x1 - 1][ss.y2 + 1] != room_id ||
			RoomGrid[ss.x1][ss.y2 + 1] != room_id || RoomGrid[ss.x1 + 1][ss.y2 + 1] != room_id);
	WallGrid[ss.x1][ss.y2] = bottom_left_corner_has_different_room_neighbour ? IS_WALL : NO_WALL;

	bool bottom_right_corner_has_different_room_neighbour =
		(RoomGrid[ss.x2 + 1][ss.y2 - 1] != room_id || RoomGrid[ss.x2 + 1][ss.y2] != room_id || RoomGrid[ss.x2 + 1][ss.y2 + 1] != room_id ||
			RoomGrid[ss.x2][ss.y2 + 1] != room_id || RoomGrid[ss.x2 - 1][ss.y2 + 1] != room_id);
	WallGrid[ss.x2][ss.y2] = bottom_right_corner_has_different_room_neighbour ? IS_WALL : NO_WALL;
}