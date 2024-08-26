#include <iostream>
#include <iomanip>

#include "Env_Room_Putter.h"

Env_Room_Putter::Env_Room_Putter():
	Weights(WeightsInArray),
	RoomsConnections(DisjointSet(ROOM_COUNT)) {

	CirculationConnections = DisjointSet2D(MAX_X + 1, MAX_Y + 1);
	
	Scores = Objectives(0.0);

	for (int i = 0; i < ROOM_COUNT; i++) {
		SubspaceConnections[i] = DisjointSet(MAX_SS_PER_ROOM);
	}
	
	// should make the Shared!!

	shared_data = Shared(
		static_cast<void*>(this), 
		&InputGrid[0][0], 
		&SubspaceGrid[0][0], 
		&RoomGrid[0][0], 
		&FurnitureGrid[0][0],
#ifndef NO_WALLS
		& WallGrid[0][0],
#else
		& RoomGrid[0][0], // just a measure, be careful
#endif // !NO_WALLS
		&CirculationGrid[0][0], 
		&RoomAreaCount[0], 
		&UsedSpaceCount, 
		&NumberOfSubspacesMade[0], 
		&RoomConnectedToCirculation[0],
		reinterpret_cast<double*> (&Scores), 
		OBJ_COUNT);
}


int32_t Env_Room_Putter::Render()
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
	Weights.Print();
	return 0;
}

int32_t Env_Room_Putter::Close()
{
	return 0;
}
