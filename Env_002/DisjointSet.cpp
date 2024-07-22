#include <vector>
#include <numeric>			// for std::accumulate
#include <algorithm>		// for std::max_element

#include "DisjointSet.h"

DisjointSet::DisjointSet(int n){
	Par = std::vector<int> (n,-1);
	NumberOfSets = n;
	NumberOfElements = n;
}

int DisjointSet::FindParent(int p){
	if (Par[p] == -1) return p;
	return Par[p]=FindParent(Par[p]);
}

void DisjointSet::Join(int a, int b){
	if (FindParent(a) != FindParent(b)){
		Par[FindParent(a)] = FindParent(b);
		NumberOfSets--;
	}
}

int DisjointSet::GetNumOfSets()const{
	return NumberOfSets;
}

double DisjointSet::GetDisjointnessOfElements(std::vector<int> indexes) {
	std::vector<int> dsize(NumberOfElements, 0);
	for (unsigned int i = 0; i < indexes.size(); i++) {
		dsize[FindParent(indexes[i])]++;
	}
	return  (double)(*std::max_element(dsize.begin(), dsize.end())) / std::accumulate(dsize.begin(), dsize.end(), 0);
}

double DisjointSet::GetDisjointness() {
	std::vector<int> dsize(NumberOfElements, 0);
	for (int i = 0; i < NumberOfElements; i++) {
		dsize[FindParent(i)]++;
	}

	return (double)(*std::max_element(dsize.begin(), dsize.end())) / NumberOfElements ;
}

int DisjointSet::BiggestSetSize()
{
	std::vector<int> dsize(NumberOfElements, 0);
	for (int i = 0; i < NumberOfElements; i++) {
		dsize[FindParent(i)]++;
	}
	return *std::max_element(dsize.begin(), dsize.end());
}




// very useful function! (inja mitoonim bebinim ye zirgorooh az disjoint set e ma aya beham vaslan ya na)
/*
int DisjointSet::GetDisjointnessOfElements(std::vector<int> indexes) {
	std::vector<int> dsize (NumberOfElements, 0);
	for(unsigned int i = 0; i < indexes.size(); i++){
		dsize[FindParent(indexes[i])]++;
	}
	return std::accumulate(dsize.begin(),dsize.end(),0) - *std::max_element(dsize.begin(),dsize.end());
}

int DisjointSet::GetDisjointness(){
	std::vector<int> dsize (NumberOfElements, 0);
		for(int i = 0; i < NumberOfElements; i++){
		dsize[FindParent(i)]++;
	}

	return NumberOfElements - *std::max_element(dsize.begin(),dsize.end());	
}
*/

