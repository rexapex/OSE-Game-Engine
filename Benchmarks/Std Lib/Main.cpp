#include <iostream>
#include "ReferenceWrapperBM.h"

int main()
{
	// Results show reference_wrapper is slower is debug mode but faster in release mode ?
	ReferenceWrapperBM bm1;
	bm1.DereferenceSpeed(1000000000);
	getchar();
	return 0;
}
