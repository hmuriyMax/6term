#include <iostream>
#include <fstream>
#include "pas_synt.h"

using std::ifstream;

int main()
{
    ifstream in_file;
    in_file.open("program.pas");
    Pas_synt examp;
    examp.TestProgram(in_file);
}
