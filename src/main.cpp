#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

/* entrypoint file, don't use main, only WinMain() ! */

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdLine, int cmdShow)
{
	return MessageBox(NULL, "Hello Windows!", "Caption", 0);
}