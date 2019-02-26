//
//  existence.cpp
//
//  Created by Joe Taylor on 15/02/2019.
//
#include <iostream>
#include <fstream>
#include <vector>

// Global constants
const int num_n = 2;
const int len_f = 1000; 

// Print vector to console
void print(std::vector<double> const &input)
{
	for (auto const& i: input) {
		std::cout << i << "\n";
	}
}

int main()
{
    std::vector<double> f11, f12;
    std::ifstream f("f1.txt");
	
	int count = 0;
	double y, dummy, compare;
	
	// Read in length of PRC values and store in arrays
    while (f >> dummy >> y && count < len_f)
    {
        f11.push_back(y);
		f12.push_back(y);
		count++;
    }
	
	// Check for identical value in other PRC
	for (auto& p : f11)
	{
		for (auto& f : f12)	
		{
			if (f == p)
			{
				std::cout << (&f - &f12[0]) << '\n';
			}
		}
	}
	
	return 0;
}