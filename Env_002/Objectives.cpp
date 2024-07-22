#include <iostream>
#include "Objectives.h"


Objectives::Objectives(double t) {
	double* ss = reinterpret_cast<double*>(this);
	for (int i = 0; i < OBJ_COUNT; i++) {
		ss[i] = t;
	}
}


double Objectives::Average(Objectives s, Objectives w) {
	double sum = 0, sumOfws = 0;
	double* ss = reinterpret_cast<double*>(&s);
	double* ww = reinterpret_cast<double*>(&w);

	for (int i = 0; i < OBJ_COUNT; i++) {
		sum += ss[i] * ww[i];
		sumOfws += ww[i];
	}
	return sum / sumOfws;
}
void Objectives::Print() const
{
	std::cout << "\n Rooms: ";
	for (int i = 0; i < ROOM_COUNT; i++) std::cout << RoomsHaveDesiredArea[i] << ", ";
	std::cout << "\n Available spaces:" << AllAvailableSpacesAreUsed << '\n';
	std::cout << " Rooms Connectedness:";
	for (int i = 0; i < ROOM_COUNT; i++) std::cout << RoomsInternalConnectedness[i] << ", ";
	std::cout << "\n";
}

double Objectives::MappedScore(double min_num, double goal_num, double max_num, double status) {
	double ans1 = (goal_num != min_num) ? (status - goal_num) / (goal_num - min_num) : 2;
	double ans2 = (goal_num != max_num) ? (goal_num - status) / (max_num - goal_num) : 2;
	return std::min(ans1, ans2) + 1;
}
