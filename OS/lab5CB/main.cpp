#include <iostream>
#include <windows.h>

using namespace std;
typedef void (*Func)(char*, char*);

int main() {
	HMODULE hLib;
	Func cutter;

	hLib = LoadLibraryEx(TEXT("C:\\prog\\6term\\OS\\lab5CB\\CutterLib.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);

	if (hLib != NULL)
	{
		cutter = (Func)GetProcAddress(hLib, "cutter");

		if (cutter != NULL) {
			char inp[100];
            char out[100];

            cout << "Input string: ";
            cin.getline(inp, 100);

            cutter(inp, out);

            cout << "Result: " << out;
		}
		else {
			cout << "Function not found" << endl;
		}
	}
	else {
		cout << "Library not found" << endl;
	}
	FreeLibrary(hLib);
}
