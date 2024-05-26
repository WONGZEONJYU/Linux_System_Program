#ifndef _CONINT_H_
#define _CONINT_H_

using ConInt = void*;

ConInt ConInt_Create(int v);
void ConInt_Destroy(ConInt);
int ConInt_Value(ConInt);
ConInt ConInt_Add(ConInt,int);
ConInt ConInt_Dec(ConInt,int);

#endif

