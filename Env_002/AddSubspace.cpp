#include "Env_Putter.h"


void Env_Putter::AddSubspace(int x1, int y1, int x2, int y2, int subspace_id)
{

#ifndef NO_WALLS
	// check if the space is thin
	if (x2 - x1 <= 1 || y2 - y1 <= 1) {
		// too thin, can cause problem with the walls
		Penalized = true;
		return;
	}
#endif // !NO_WALLS

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
		if (i != subspace_id && Subspaces[i].x1 != -1 && Subspaces[subspace_id].SharesAWallWith(Subspaces[i])) {
			// connected subspace!
			UpdateWallsOfSubspace(i);
			SubspaceConnections[room_id].Join(i - room_first_ss_id, subspace_id - room_first_ss_id);
		}

	}

}



#ifndef NO_WALLS
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


#else
void Env_Putter::UpdateWallsOfSubspace(int subspace_id){}
#endif // !NO_WALLS

