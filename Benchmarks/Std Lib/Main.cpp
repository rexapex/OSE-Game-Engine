#include <iostream>
#include "ReferenceWrapperBM.h"

void main()
{
	// Results show reference_wrapper to be significantly slower that raw pointer / reference
	ReferenceWrapperBM bm1;
	bm1.DereferenceSpeed();
	getchar();
}
