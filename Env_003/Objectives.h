#pragma once
#include <initializer_list>
#include "Macros.h"

struct Objectives {
	// rooms sharing one grid: Action Masking
	// No rooms more than the MaxSSperRoom: Action Masking

	//double WeHaveRoom[ROOM_COUNT]; // 0 or 1
	double AllAvailableSpacesAreUsed = 0; // 0 to 1
	double RoomsHaveDesiredArea =  0 ;

	double RoomsInternalConnectedness = 0 ; // 0 to 1

	double AllCirculationsAreConnected = 0;
	double CirculationAccessToOutside = 0;
	double AllRoomsConnectedToCirculation = 0;
	double AllRoomsConnectedTogether = 0;
	double CorrectAdjacencyMatrix = 0;

	//double FurniturePositions[FURNITURE_COUNT];
	//double Connections[ROOM_COUNT * ROOM_COUNT];

	// many many more!

	explicit Objectives(double t);
	Objectives() = default;
	explicit Objectives(const double Array[]);
	void Reset(double t);
	static double Average(Objectives s, Objectives w);
	static double MappedScore(double min_num, double goal_num, double max_num, double status);
	void Print() const;

};


constexpr int OBJ_COUNT = sizeof(Objectives) / sizeof(double);

