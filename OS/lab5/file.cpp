#include <iostream>
#include "Header.h" 

using namespace std;

int main()
{
	char inp[100];
	char out[100];

	cout << "Input string: ";
	cin.getline(inp, 100);

	cutter(inp, out);

	cout << "Result: " << out;
}