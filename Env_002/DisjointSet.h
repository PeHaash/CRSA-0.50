#pragma once
#include <vector>

class DisjointSet{
	private:
		std::vector<int> Par;
		int NumberOfSets;
		int NumberOfElements;
	public:
		DisjointSet() = default;  // vase in ke betoonam alaki tarifesh konam ke ba'dan kamel she
		DisjointSet(int n);
		int FindParent(int p);
		void Join(int a, int b);
		int GetNumOfSets()const;
		double GetDisjointnessOfElements(std::vector<int> indexes);
		double GetDisjointness();
		int BiggestSetSize();
};


class DisjointSet2D {
	private:
		DisjointSet disjointset;
		int Width, Height;
	public:
		DisjointSet2D(int width, int height);
		DisjointSet2D() = default;
		int FindParent(int x, int y);
		void Join(int x1, int y1, int x2, int y2);
		int BiggestSetSize();
};
