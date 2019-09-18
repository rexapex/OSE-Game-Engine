#pragma once
#include <iostream>
#include <chrono>

class ReferenceWrapperBM
{
public:
	void DereferenceSpeed(int const num_iters = 10000000)
	{
		int * p = new int;
		int & r = *p;
		std::reference_wrapper<int> rw = std::ref(*p);

		// Test the speed of dereferencing raw pointers
		auto start1 = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < num_iters; i++) {
			*p = i;
		}
		auto stop1 = std::chrono::high_resolution_clock::now();

		// Test the speed of dereferencing c++ references
		auto start2 = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < num_iters; i++) {
			r = i;
		}
		auto stop2 = std::chrono::high_resolution_clock::now();

		// Test the speed of dereferencing std::reference_wrappers
		auto start3 = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < num_iters; i++) {
			rw.get() = i;
		}
		auto stop3 = std::chrono::high_resolution_clock::now();

		// Output the results
		std::cout << "ReferenceWrapperBM::DereferenceSpeed" << std::endl;
		std::cout << "Num Iterations: " << num_iters << std::endl;
		std::cout << "Values: " << *p << ", " << r << ", " << rw.get() << std::endl;
		std::cout << "Raw Pointers: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << "ms" << std::endl;
		std::cout << "C++ References: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2).count() << "ms" << std::endl;
		std::cout << "Standard reference_wrappers: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop3 - start3).count() << "ms" << std::endl;
	}
};
