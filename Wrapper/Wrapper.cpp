#include "Wrapper.h"
// Havaset bashe: Wrapper ha ba reference nemigiran!!!!

/*
void* c_MakeDTI(HyperParameters hp){
	_DETENVI_* m = new _DETENVI_(hp);
	return static_cast<void*> (m);
}

// void c_SetFeature(void* p, Features fet){
// 	(static_cast<FrenchHamlets*>(p))->SetFeature(fet);
// }


int32_t c_ImplementAndEvaluate(void* DTI,  Input InputData, Score Scores){
	return (static_cast<_DETENVI_*>(DTI))->ImplementAndEvaluate(InputData, Scores);
}

int32_t c_ImplementAndExport(void* DTI, Input InputData, Score Scores, Export ExportGrid){
	return (static_cast<_DETENVI_*>(DTI))->ImplementAndExport(InputData, Scores, ExportGrid);
}

int32_t c_DeleteDTI(void* p){
	if (p == nullptr)
		return 1;
	delete static_cast<_DETENVI_*>(p);
	return 0;
}
*/

Shared* InstantiateCustEnv()
{
	__CUST_ENV__* m = new __CUST_ENV__();
	//return static_cast<void*> (m);
	return &m->shared_data;
}

int32_t Reset(void* CustEnv)
{
	return (static_cast<__CUST_ENV__*>(CustEnv))->Reset();
}

int32_t Step(void* CustEnv)
{
	return (static_cast<__CUST_ENV__*>(CustEnv))->Step();
}

int32_t Render(void* CustEnv)
{
	return (static_cast<__CUST_ENV__*>(CustEnv))->Render();
}

int32_t Close(void* CustEnv)
{
	return (static_cast<__CUST_ENV__*>(CustEnv))->Close();
}

int32_t DeleteCustEnv(void* CustEnv)
{
	delete static_cast<__CUST_ENV__*>(CustEnv);
	return 0;
}
