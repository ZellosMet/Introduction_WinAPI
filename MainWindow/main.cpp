#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include<cstdlib>


#define IDC_BUTTON_1	1001
#define IDC_BUTTON_2	1002
#define IDC_BUTTON_3	1003
#define IDC_BUTTON_4	1004
#define IDC_BUTTON_5	1005
#define IDC_BUTTON_6	1006
#define IDC_BUTTON_7	1007
#define IDC_BUTTON_8	1008
#define IDC_BUTTON_9	1009
#define IDC_BUTTON_0	1010
#define IDC_BUTTON_ADD	1011
#define IDC_BUTTON_SUB	1012
#define IDC_BUTTON_MUL	1013
#define IDC_BUTTON_DIV	1014
#define IDC_BUTTON_RES	1015
#define IDC_BUTTON_C	1016
#define IDC_EDIT		1017

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParem, LPARAM lParam);
CHAR* Enter(HWND hwnd, const CHAR* symbol);
INT ConvertEditTextToInt(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.style = 0;
	wc.hIcon = ExtractIcon(hInstance, "calculation.ico", 0);
	wc.hIconSm = ExtractIcon(hInstance, "calculation.ico", 0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Ñalculator";

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, ("Call Register failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW,
		"Ñalculator",
		"Ñalculator",
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		255,
		320,		
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if(hwnd == NULL)
	{
		MessageBox(NULL, ("Window create failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton[17], hEdit;
	static CHAR sign;

	switch(uMsg)
	{
		case WM_CREATE:		
		{			
			hEdit = CreateWindowEx(NULL, "Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 215, 25, hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);
			hButton[1] = CreateWindowEx(NULL, "Button", "1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 50, 50, 50, hwnd, (HMENU)IDC_BUTTON_1, GetModuleHandle(NULL), NULL);
			hButton[2] = CreateWindowEx(NULL, "Button", "2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 65, 50, 50, 50, hwnd, (HMENU)IDC_BUTTON_2, GetModuleHandle(NULL), NULL);
			hButton[3] = CreateWindowEx(NULL, "Button", "3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 50, 50, 50, hwnd, (HMENU)IDC_BUTTON_3, GetModuleHandle(NULL), NULL);
			hButton[4] = CreateWindowEx(NULL, "Button", "4", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 105, 50, 50, hwnd, (HMENU)IDC_BUTTON_4, GetModuleHandle(NULL), NULL);
			hButton[5] = CreateWindowEx(NULL, "Button", "5", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 65, 105, 50, 50, hwnd, (HMENU)IDC_BUTTON_5, GetModuleHandle(NULL), NULL);
			hButton[6] = CreateWindowEx(NULL, "Button", "6", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 105, 50, 50, hwnd, (HMENU)IDC_BUTTON_6, GetModuleHandle(NULL), NULL);
			hButton[7] = CreateWindowEx(NULL, "Button", "7", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 160, 50, 50, hwnd, (HMENU)IDC_BUTTON_7, GetModuleHandle(NULL), NULL);
			hButton[8] = CreateWindowEx(NULL, "Button", "8", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 65, 160, 50, 50, hwnd, (HMENU)IDC_BUTTON_8, GetModuleHandle(NULL), NULL);
			hButton[9] = CreateWindowEx(NULL, "Button", "9", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 160, 50, 50, hwnd, (HMENU)IDC_BUTTON_9, GetModuleHandle(NULL), NULL);
			hButton[0] = CreateWindowEx(NULL, "Button", "0", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 65, 215, 50, 50, hwnd, (HMENU)IDC_BUTTON_0, GetModuleHandle(NULL), NULL);
			hButton[10] = CreateWindowEx(NULL, "Button", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 50, 50, 50, hwnd, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);
			hButton[11] = CreateWindowEx(NULL, "Button", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 105, 50, 50, hwnd, (HMENU)IDC_BUTTON_SUB, GetModuleHandle(NULL), NULL);
			hButton[12] = CreateWindowEx(NULL, "Button", "*", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 160, 50, 50, hwnd, (HMENU)IDC_BUTTON_MUL, GetModuleHandle(NULL), NULL);
			hButton[13] = CreateWindowEx(NULL, "Button", "/", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 215, 50, 50, hwnd, (HMENU)IDC_BUTTON_DIV, GetModuleHandle(NULL), NULL);
			hButton[14] = CreateWindowEx(NULL, "Button", "=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 215, 50, 50, hwnd, (HMENU)IDC_BUTTON_RES, GetModuleHandle(NULL), NULL);
			hButton[15] = CreateWindowEx(NULL, "Button", "C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 215, 50, 50, hwnd, (HMENU)IDC_BUTTON_C, GetModuleHandle(NULL), NULL);
		}
		break;
		case WM_COMMAND:	
		{
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_1: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "1")); break;
				case IDC_BUTTON_2: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "2")); break;
				case IDC_BUTTON_3: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "3")); break;
				case IDC_BUTTON_4: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "4")); break;
				case IDC_BUTTON_5: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "5")); break;
				case IDC_BUTTON_6: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "6")); break;
				case IDC_BUTTON_7: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "7")); break;
				case IDC_BUTTON_8: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "8")); break;
				case IDC_BUTTON_9: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "9")); break;
				case IDC_BUTTON_0: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "0")); break;
				case IDC_BUTTON_ADD: 
				{
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "+"));
					sign = '+';
				}
				break;
				case IDC_BUTTON_SUB: 
				{
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "-"));
					sign = '-';
				}
				break;
				case IDC_BUTTON_MUL: 
				{
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "*")); 
					sign = '*';
				}
				break;
				case IDC_BUTTON_DIV: 
				{
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)Enter(hwnd, "/"));
					sign = '/';
				}
				break;
				case IDC_BUTTON_C: SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)""); break;
				case IDC_BUTTON_RES: 
				{
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE] = {};
					DOUBLE value[2];
					HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
					SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
					CHAR delim[5] = {'+', '-', '*', '/'};
					value[0] = std::atof(strtok(sz_buffer, delim));
					value[1] = std::atof(strtok(NULL, delim));
					switch (sign)
					{
						case '+':
						{							
							CHAR* result = _itoa((value[0]+value[1]), sz_buffer, 10);
							SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)result);
						} 
						break;
						case '-':
						{
							CHAR* result = _itoa((value[0]-value[1]), sz_buffer, 10);
							SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)result);
						} 
						break;
						case '*':
						{
							CHAR* result = _itoa((value[0]*value[1]), sz_buffer, 10);
							SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)result);
						} 
						break;
						{
						case '/':
							CHAR* result = _itoa((value[0]/value[1]), sz_buffer, 10);
							SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)result);
						}
						break;
					}
				}
				break;
			}
		}
		break;
		case WM_DESTROY:	PostQuitMessage(0); break;
		case WM_CLOSE:		DestroyWindow(hwnd); break;
		default:			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}

CHAR* Enter(HWND hwnd, const CHAR* symbol)
{
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE] = {};
	SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
	if ((strcmp(sz_buffer, "") == 0) && (strcmp(symbol, "0") == 0)) return sz_buffer;
	return strcat(sz_buffer, symbol);
}