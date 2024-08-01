#include "Env_Putter.h"


void Env_Putter::UpdateScores()
{
	// TODO: UpdateScores + sub functions
	/*
	* A. Area types
	*	- We have the room					----
	*	- We Use All the Proper spaces		DONE
	*	- Areas in the good proportions		DONE
	* B. Connections Between Subspaces
	*	- Rooms Internal Integrity			!!!!
	*	- Rooms Circulation Connection
	*	- Rooms Sharing Wall
	* C. Circulation
	*	- One Integrated Circulation
	*	- Circulation Connected to the entrance
	*
	*
	*/
	//double WeHaveRoom[ROOM_COUNT]; // 0 or 1
	//double AllAvailableSpacesAreUsed; // 0 to 1
	//double RoomsHaveDesiredArea[ROOM_COUNT];
	if (
		//CheckRoomAvailability() &&
		CheckAvailableSpaceUsed() &&
		CheckRoomsCorrectArea() &&
		CheckRoomsInternalConnectedness() &&
		CheckCirculationsConnectivity() &&
		CheckCirculationAccessToOutside() &&
		CheckAllRoomsConnectedToCirculation() &&
		CheckAllRoomsConnectedTogether()
		) {
	}

}


bool Env_Putter::CheckAvailableSpaceUsed() {
	Scores.AllAvailableSpacesAreUsed = Objectives::MappedScore(0, AllAvailableSpace, AllAvailableSpace, UsedSpaceCount) * ALPHA + BETA;
	return true;
}

bool Env_Putter::CheckRoomsCorrectArea() {
	for (int i = 0; i < ROOM_COUNT; i++) {
		// best number is (UsedSpaceCount * RoomAreaGoal[i])
		Scores.RoomsHaveDesiredArea[i] =
			Objectives::MappedScore(0, UsedSpaceCount * RoomAreaGoal[i], AllAvailableSpace, RoomAreaCount[i]) * ALPHA + BETA;
	}
	return true;
}

bool Env_Putter::CheckRoomsInternalConnectedness() {
	for (int i = 0; i < ROOM_COUNT; i++) {
		Scores.RoomsInternalConnectedness[i] =
			(RoomAreaCount[i] == 0) ? 0 : (((double)SubspaceConnections[i].BiggestSetSize() / (double)NumberOfSubspacesMade[i]) * ALPHA + BETA);
	}
	return true;
}


bool Env_Putter::CheckCirculationsConnectivity(){
	Scores.AllCirculationsAreConnected = ((double)CirculationConnections.BiggestSetSize() / (double)NumberOfGridsUsedInCirulation) * ALPHA + BETA;
	return true;
}

bool Env_Putter::CheckCirculationAccessToOutside() {
	Scores.CirculationAccessToOutside = (CirculationAccessToOutside? ALPHA : 0) + BETA;
	return true;
}

bool Env_Putter::CheckAllRoomsConnectedToCirculation(){
	int rooms_connected_to_circulation = 0;
	for(int i = 0; i < ROOM_COUNT; i++) rooms_connected_to_circulation += RoomConnectedToCirculation[i]?1:0;
	Scores.AllRoomsConnectedToCirculation = ((double)rooms_connected_to_circulation / (double)ROOM_COUNT) * ALPHA + BETA;
	return true;
}

bool Env_Putter::CheckAllRoomsConnectedTogether() {
	Scores.AllRoomsConnectedTogether = ((double)RoomsConnections.BiggestSetSize() / (double)ROOM_COUNT) * ALPHA + BETA;
}
