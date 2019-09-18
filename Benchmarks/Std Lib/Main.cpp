#include <iostream>
#include "ReferenceWrapperBM.h"

void main()
{
	// Results show reference_wrapper is slower is debug mode but faster in release mode ?
	ReferenceWrapperBM bm1;
	bm1.DereferenceSpeed(1000000000);
	getchar();
}
