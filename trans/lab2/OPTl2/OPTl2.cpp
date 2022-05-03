#include "ut.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const string progfile = "program.pas";
const bool FILE_MODE = false;

int main()
{
    RunAll();

    if (FILE_MODE)
    {
        ifstream in("program.pas");
        stringstream progtext;
        progtext << in.rdbuf();
        CheckProgram(std::cout, "File test", progtext.str());
    }
}