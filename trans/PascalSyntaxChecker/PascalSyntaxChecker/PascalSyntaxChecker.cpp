#include <iostream>
#include <fstream>
#include "pas_synt.h"

using std::ifstream;

int main()
{
    ifstream in_file;
    for (int i = 1; i < 10; i++) {
		Pas_synt examp;
		string fname = "test";
		char k[10];
		_itoa_s(i, k, _countof(k), 10);
		fname += k;
		fname += ".pas";
        in_file.open(fname);
		std::cout << fname << ": ";
		if (examp.TestProgram(in_file))
			std::cout << "Correct";
		std::cout << std::endl;
		in_file.close();
    }
}
