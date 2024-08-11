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
				RoomsConnections.Join(RoomGrid[i][j], RoomGrid[x1][y1]);
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

	// Connecting the rooms, in the adjacency matrix


}
