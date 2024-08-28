#include "Env_Room_Putter.h"

// new version, room_id / no walls
void Env_Room_Putter::AddSubspace(int x1, int y1, int x2, int y2, int room_id)
{
	// check if we reach the maximum count of subspaces for that room
	if (NumberOfSubspacesMade[room_id] == MAX_SS_PER_ROOM) {
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
	int subspace_id = room_id * MAX_SS_PER_ROOM + NumberOfSubspacesMade[room_id];
	Subspaces[subspace_id] = { x1,y1,x2,y2 };
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

	// add connections between ss of the same room
	for (int i = room_first_ss_id; i < room_first_ss_id + MAX_SS_PER_ROOM; i++) {
		if (i != subspace_id && Subspaces[i].x1 != -1 && Subspaces[subspace_id].SharesAWallWith(Subspaces[i])) {
			// connected subspace!

			SubspaceConnections[room_id].Join(i - room_first_ss_id, subspace_id - room_first_ss_id);
		}

	}

}


/*
* Old version: with subspace_id,
void Env_Room_Putter::AddSubspace(int x1, int y1, int x2, int y2, int subspace_id)
{

	// NO_WALLS

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

void Env_Room_Putter::UpdateWallsOfSubspace(int subspace_id){}
// !NO_WALLS

*/


