#include "ColorConsole.h"
#include <iostream>

bool CC::CustomColor(HANDLE hConsole, int color)
{
	return(SetConsoleTextAttribute(hConsole, color));
}

bool CC::Black(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 0));
}
bool CC::DarkBlue(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 1));
}
bool CC::DarkGreen(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 2));
}
bool CC::BabyBlue(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 3));
}
bool CC::Red(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 4));
}
bool CC::Purple(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 5));
}
bool CC::Gold(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 6));
}
bool CC::LightGray(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 7));
}
bool CC::Gray(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 8));
}
bool CC::SkyBlue(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 9));
}
bool CC::LightGreen(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 10));
}
bool CC::Cyan(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 11));
}
bool CC::Peach(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 12));
}
bool CC::Magenta(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 13));
}
bool CC::Khaki(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 14));
}
bool CC::White(HANDLE hConsole)
{
	return(SetConsoleTextAttribute(hConsole, 15));
}


void Console::PrintTime(time_t time)
{
	tm timeInfo;
	localtime_s(&timeInfo, &time);
	printf("%02u:%02u:%02u", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
}
