#include<Windows.h>

INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	MessageBox(NULL, "Hello Windows!", "Header", MB_YESNOCANCEL | MB_ICONINFORMATION | MB_HELP | MB_ICONERROR);
	return 0;

}