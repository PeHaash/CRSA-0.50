#include "Env_Putter.h"
#include "InputGridArrays.h"

int32_t Env_Putter::Reset() {
	// >> to avoid memory leak, we do not have any constructors of structs/classes here. they can cause problems

	// For things that are per room:
	for (int i = 0; i < ROOM_COUNT; i++) {
		RoomAreaCount[i] = 0;
		SubspaceConnections[i].Reset();
		NumberOfSubspacesMade[i] = 0;
		RoomConnectedToCirculation[i] = false;
	}

	// only for InputGrid
	AllAvailableSpace = 0;
	if (shared_data.Grid_id == -1) {
		// customized input from the notebook
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				SubspaceGrid[i][j] = (InputGrid[i][j] == FREE) ? EMPTY : UNAVAILABLE;
				AllAvailableSpace += (InputGrid[i][j] == FREE);
			}
		}
	}
	else {
		// fixed input from the code we have:
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				SubspaceGrid[i][j] = (PresetInputArrays[shared_data.Grid_id][i][j] == FREE) ? EMPTY : UNAVAILABLE;
				AllAvailableSpace += (PresetInputArrays[shared_data.Grid_id][i][j] == FREE);
			}
		}
	}

	// For grids:
	for (int i = 0; i <= MAX_X + 1; i++) {
		for (int j = 0; j <= MAX_Y + 1; j++) {
			FurnitureGrid[i][j] = EMPTY;
			WallGrid[i][j] = NO_WALL;
			RoomGrid[i][j] = EMPTY;
			CirculationGrid[i][j] = NO_CIRCULATION;
		}
	}

	// for subspaces:
	for (int i = 0; i < MAX_SUBSPACE; i++) {
		Subspaces[i] = { -1 };
	}

	// for furniture
	for (int i = 0; i < FURNITURE_COUNT; i++) {
		Furnitures[i] = { -1 };
	}

	
	RoomsConnections.Reset();
	CirculationConnections.Reset();
	CirculationAccessToOutside = false;

	NumberOfGridsUsedInCirulation = 0;
	UsedSpaceCount = 0;
	StepCount = 0;
	PreviousScore = 0;
	Scores.Reset(0);
	//Weights = Objectives(WEIGHTS); // TODO: Weights are constant, get set in the constructor, or initialization, from MACROS
	return 0;
}