#include "Env_Putter.h"
#include "InputGridArrays.h"

int32_t Env_Putter::Reset() {
	for (int i = 0; i < ROOM_COUNT; i++) {
		RoomAreaCount[i] = 0;
		SubspaceConnections[i] = DisjointSet(MAX_SS_PER_ROOM);
		NumberOfSubspacesMade[i] = 0;
	}
	AllAvailableSpace = 0;
	if (shared_data.Grid_id == -1) {
		// customized input from the notebook
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				SubspaceGrid[i][j] = (InputGrid[i][j] == 0) ? -1 : -2;
				FurnitureGrid[i][j] = -1;
				AllAvailableSpace += (InputGrid[i][j] == 0);
			}
		}
	}
	else {
		// fixed input from the code we have:
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				SubspaceGrid[i][j] = (PresetInputArrays[shared_data.Grid_id][i][j] == 0) ? -1 : -2;
				WallGrid[i][j] = 0;
				RoomGrid[i][j] = -1;
				FurnitureGrid[i][j] = -1;
				AllAvailableSpace += (InputGrid[i][j] == 0);
			}
		}
	}

	for (int i = 0; i < FURNITURE_COUNT; i++) {
		Furnitures[i] = { -1 };
	}

	for (int i = 0; i < MAX_SUBSPACE; i++) {
		Subspaces[i] = { -1 };
	}
	RoomsConnections = DisjointSet(ROOM_COUNT);

	UsedSpaceCount = 0;
	StepCount = 0;
	PreviousScore = 0;
	Scores = 0;
	Weights = 1; // TODO: Weights are constant, get set in the constructor, or initialization, from MACROS
	return 0;
}