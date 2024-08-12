#include "Env_Putter.h"


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
			if (InputGrid[i][j] == ACCESS){
				// connected to entrance
				CirculationAccessToOutside = true;
			}
			else {
				// part of a room
				RoomConnectedToCirculation[RoomGrid[i][j]] = true;
				// 
			}

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

	// Connecting the rooms, in the adjacency matrix / also, updates the RoomConnections disjoint set
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			if (i == x1) {
				UpdateAdjacencyMatrix(i, j, i - 1, j);
			}
			if (j == y1) {
				UpdateAdjacencyMatrix(i, j, i, j - 1);
			}
			UpdateAdjacencyMatrix(i, j, i + 1, j);
			UpdateAdjacencyMatrix(i, j, i, j + 1);
		}
	}


}


void Env_Putter::UpdateAdjacencyMatrix(int x1, int y1, int x2, int y2) {
	if(RoomGrid[x1][y1] != EMPTY && 
		RoomGrid[x2][y2] != EMPTY && 
		CirculationGrid[x2][y2] != NO_CIRCULATION &&
		CirculationGrid[x1][y1] != NO_CIRCULATION && // not needed really, guaranteed to be true
		RoomGrid[x1][y1] != RoomGrid[x2][y2]) {
			AdjacencyMatrix[RoomGrid[x1][y1]][RoomGrid[x2][y2]] = 1;
			AdjacencyMatrix[RoomGrid[x2][y2]][RoomGrid[x1][y1]] = 1;
			RoomsConnections.Join(RoomGrid[x2][y2], RoomGrid[x1][y1]);
	}
}