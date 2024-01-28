#pragma once
#include <windows.h>

namespace CC
{
	bool CustomColor(HANDLE hConsole, int color);

	bool Black(HANDLE hConsole);
	bool DarkBlue(HANDLE hConsole);
	bool DarkGreen(HANDLE hConsole);
	bool BabyBlue(HANDLE hConsole);
	bool Red(HANDLE hConsole);
	bool Purple(HANDLE hConsole);
	bool Gold(HANDLE hConsole);
	bool LightGray(HANDLE hConsole);
	bool Gray(HANDLE hConsole);
	bool SkyBlue(HANDLE hConsole);
	bool LightGreen(HANDLE hConsole);
	bool Cyan(HANDLE hConsole);
	bool Peach(HANDLE hConsole);
	bool Magenta(HANDLE hConsole);
	bool Khaki(HANDLE hConsole);
	bool White(HANDLE hConsole);
}

namespace Console
{
	void PrintTime(time_t time);
}