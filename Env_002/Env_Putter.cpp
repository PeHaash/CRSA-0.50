#include <iostream>
#include <iomanip>

#include "Env_Putter.h"

Env_Putter::Env_Putter() {
	// should make the Shared!!
	shared_data = Shared(static_cast<void*>(this), &InputGrid[0][0], &SubspaceGrid[0][0], &RoomAreaCount[0], &UsedSpaceCount);

}


int32_t Env_Putter::Render()
{
	std::cout << '\n';
	for (int j = 1; j <= MAX_Y; j++) {
		for (int i = 1; i <= MAX_X; i++) {
			if (SubspaceGrid[i][j] == -1) {
				std::cout << std::setw(2) << "-";
			}
			else {
				std::cout << std::setw(2) << SubspaceGrid[i][j];
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
