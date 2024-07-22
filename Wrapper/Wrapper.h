// these are for the debug purposes, these flags should be defined in the compiler, in the google colab
//#define DualGridCNN
//#define FrenchHamlets

#include <cstdint>

#ifdef _MSC_VER
	#include "./../Env_002/Env_Putter.h"
#endif

#ifdef CUST_ENV_IS_PUTTER
	#include "Env_Putter.h"
#endif



extern "C" {
	Shared* InstantiateCustEnv();
	int32_t Reset(void*);
	int32_t Step(void*);
	int32_t Render(void*);
	int32_t Close(void*);
	int32_t DeleteCustEnv(void*);
}

