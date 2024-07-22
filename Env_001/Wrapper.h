// these are for the debug purposes, these flags should be defined in the compiler, in the google colab
//#define DualGridCNN
//#define FrenchHamlets

#include <cstdint>

#include "ENV_Painter.h"



extern "C" {
	Shared* InstantiateCustEnv();
	int32_t Reset(void*);
	int32_t Step(void*);
	int32_t Render(void*);
	int32_t Close(void*);
	int32_t DeleteCustEnv(void*);
}

