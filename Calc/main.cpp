#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<stdio.h>
#include<string>

CONST CHAR g_sz_CLASSNAME[] = "Calc";

CONST INT START_X = 10;
CONST INT START_Y = 10;


CONST INT BUTTON_SIZE = 50;
CONST INT INTERVAL = 5;
CONST INT BUTTON_DOUBLE_SIZE = BUTTON_SIZE * 2 + INTERVAL;

CONST INT SCREEN_WIDTH = BUTTON_SIZE * 5 + INTERVAL * 4;
CONST INT SCREEN_HEIGHT = 20;

CONST INT BUTTON_START_X = START_X;
CONST INT BUTTON_START_Y = START_Y + SCREEN_HEIGHT + INTERVAL * 2;

CONST CHAR* OPERATIONS[] = { "/","*","-","+" };

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParem, LPARAM lParam);

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
	wc.lpszClassName = g_sz_CLASSNAME;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, ("Call Register failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW,
		g_sz_CLASSNAME,
		g_sz_CLASSNAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(BUTTON_SIZE+INTERVAL)*5+BUTTON_START_X*3,
		SCREEN_HEIGHT + (BUTTON_SIZE+INTERVAL)*5 + INTERVAL*2,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, ("Window Create failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		
	{
		//Строка ввода
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0", 
			WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT| ES_READONLY | WS_BORDER, 
			START_X, START_X, SCREEN_WIDTH, SCREEN_HEIGHT,
			hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL
		);
		//Создание цифровой панели
		int digit = 0;
		char sz_digit[2] = "";
		for(int i=6; i>=0; i-=3)
			for (int j = 0; j < 3; j++)
			{
				digit++;
				sz_digit[0] = digit + 48;
				CreateWindowEx
				(
					NULL, "Button", (LPCSTR)sz_digit, 
					WS_CHILDWINDOW | WS_VISIBLE |BS_PUSHBUTTON, 
					BUTTON_START_X + (BUTTON_SIZE+INTERVAL)*j, 
					BUTTON_START_Y + (BUTTON_SIZE+INTERVAL)*i/3, 
					BUTTON_SIZE, BUTTON_SIZE, 
					hwnd, 
					(HMENU)(IDC_BUTTON_0 + digit), 
					GetModuleHandle(NULL), NULL
				);
			}
		//Создание нуля
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_START_X, BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * 3,
			BUTTON_DOUBLE_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_0),
			GetModuleHandle(NULL), NULL
		);
		//Создание точки
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_START_X + BUTTON_DOUBLE_SIZE + INTERVAL,
			BUTTON_START_Y + (BUTTON_SIZE + INTERVAL)*3,
			BUTTON_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_POINT),
			GetModuleHandle(NULL), NULL
		);
		//Создане операций
		for (size_t i = 0; i < sizeof(OPERATIONS)/sizeof(OPERATIONS[0]); i++)
		{
			CreateWindowEx
			(
				NULL, "Button", OPERATIONS[i],
				WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
				BUTTON_START_X + (BUTTON_SIZE + INTERVAL)*3,
				BUTTON_START_Y + (BUTTON_SIZE + INTERVAL)*i,
				BUTTON_SIZE, BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_SLASH-i),
				GetModuleHandle(NULL), NULL
			);
		}
		//Кнопка очистки
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4,
			BUTTON_START_Y,
			BUTTON_SIZE, BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_CLEAR),
			GetModuleHandle(NULL), NULL
		);
		//Кнопка равно
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4,
			BUTTON_START_Y + BUTTON_DOUBLE_SIZE+INTERVAL,
			BUTTON_SIZE, BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL), NULL
		);
	}
	break;
	case WM_COMMAND:	
	{
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9 || LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			if (strcmp(sz_buffer, "0") == 0)
			{
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
				sz_buffer[0] = 0;
			}
			CHAR sz_digit[2]{};
			if (LOWORD(wParam) == IDC_BUTTON_POINT)
			{
				if (strchr(sz_buffer, '.')) break;
				sz_digit[0] = '.';
			}
			else sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48;
			strcat(sz_buffer, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
	}
	break;
	case WM_DESTROY:	PostQuitMessage(0); break;
	case WM_CLOSE:		DestroyWindow(hwnd); break;
	default:			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}