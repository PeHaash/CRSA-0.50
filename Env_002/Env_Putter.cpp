#include <iostream>
#include <iomanip>

#include "Env_Putter.h"
#include "InputGridArrays.h"

Env_Putter::Env_Putter() {
	// should make the Shared!!
	shared_data = Shared(static_cast<void*>(this), &InputGrid[0][0], &Grid[0][0], &RoomAreaCount[0], &UsedSpaceCount);

}

int32_t Env_Putter::Reset() {
	for (int i = 0; i < ROOM_COUNT; i++) {
		RoomAreaCount[i] = 0;
		SubspaceConnections[i] = DisjointSet(MAX_SS_PER_ROOM);
		NumberOfSubspacesMade[i] = 0;
	}
	AllAvailableSpace = 0;
	if(shared_data.Grid_id == -1){
		// customized input from the notebook
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				Grid[i][j] = (InputGrid[i][j] == 0) ? -1 : -2;
				FurnitureGrid[i][j] = -1;
				AllAvailableSpace += (InputGrid[i][j] == 0);
			}
		}
	}
	else {
		// fixed input from the code we have:
		for (int i = 0; i < MAX_X + 1; i++) {
			for (int j = 0; j < MAX_Y + 1; j++) {
				Grid[i][j] = (PresetInputArrays[shared_data.Grid_id][i][j] == 0) ? -1 : -2;
				FurnitureGrid[i][j] = -1;
				AllAvailableSpace += (InputGrid[i][j] == 0);
			}
		}
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

int32_t Env_Putter::Render()
{
	std::cout << '\n';
	for (int j = 1; j <= MAX_Y; j++) {
		for (int i = 1; i <= MAX_X; i++) {
			if (Grid[i][j] == -1) {
				std::cout << std::setw(2) << "-";
			}
			else {
				std::cout << std::setw(2) << Grid[i][j];
			}
			//std::cout <<(Grid[i][j]==-1? "-": (const char*)(Grid[i][j])) << "";
		}
		std::cout << '\n';
	}

	Scores.Print();
	std::cout << '\n';
	//Weights.Print();
	return 0;
}

int32_t Env_Putter::Close()
{
	return 0;
}
