#pragma once

struct Shared {
	void* CustEnv;
	// Reset Options: 
	int* InputGrid;
	// Action:
	int x1, x2, y1, y2, type;
	// Observations:

	// Score, Terminations:
	double Score;
	bool Terminated, Truncated;
};
