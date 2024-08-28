#include <iostream>

#include "Objectives.h"


Objectives::Objectives(double t) {
	double* ss = reinterpret_cast<double*>(this);
	for (int i = 0; i < OBJ_COUNT; i++) {
		ss[i] = t;
	}
}

void Objectives::Reset(double t) {
	double* ss = reinterpret_cast<double*>(this);
	for (int i = 0; i < OBJ_COUNT; i++) {
		ss[i] = t;
	}
}

/*
Objectives::Objectives(std::initializer_list<double> array) {
	double* ss = reinterpret_cast<double*>(this);
	for (int i = 0; i < OBJ_COUNT; i++) {
		ss[i] = 1;
	}
	int index = 0;
	for (double element : array) {
		if (element != -1.0)
			ss[index] = element;
		else 
			break;
		index++;
	}
}*/

Objectives::Objectives(const double Array[]) {
	double* ss = reinterpret_cast<double*>(this);
	bool we_saw_the_minus_one = false;
	for (int i = 0; i < OBJ_COUNT; i++) {
		if(Array[i] == -1) we_saw_the_minus_one = true;
		ss[i] = we_saw_the_minus_one? 1 : Array[i];
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
	std::cout << RoomsHaveDesiredArea << ", ";
	std::cout << "\n Available spaces:" << AllAvailableSpacesAreUsed << '\n';
	std::cout << " Rooms Connectedness:";
	std::cout << RoomsInternalConnectedness << ", ";
	std::cout << "\n";
}

double Objectives::MappedScore(double min_num, double goal_num, double max_num, double status) {
	double ans1 = (goal_num != min_num) ? (status - goal_num) / (goal_num - min_num) : 2;
	double ans2 = (goal_num != max_num) ? (goal_num - status) / (max_num - goal_num) : 2;
	return std::min(ans1, ans2) + 1;
}
