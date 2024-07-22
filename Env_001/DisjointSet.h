#pragma once

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
};
