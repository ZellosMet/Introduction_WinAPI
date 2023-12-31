﻿#define _CRT_SECURE_NO_WARNINGS
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
CONST INT SCREEN_HEIGHT = 50;

CONST CHAR DISPLAY_FONT[] = "Tahoma";
CONST INT DISPLAY_FONT_HEIHGT = SCREEN_HEIGHT - 2;
CONST INT DISPLAY_FONT_WIDTH = DISPLAY_FONT_HEIHGT / 2.5;

CONST INT BUTTON_START_X = START_X;
CONST INT BUTTON_START_Y = START_Y + SCREEN_HEIGHT + INTERVAL * 2;

CONST CHAR* OPERATIONS[] = { "/","*","-","+" };

CONST CHAR* BUTTON_IMAGES_PATH = "Button_images\\";
CONST CHAR* BUTTON_OPERATIONS_IMAGES_PATH[] = { "Divide.bmp", "Multiply.bmp", "Minus.bmp", "Plus.bmp" };

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
	wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 0));
	//wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
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
		//WS_OVERLAPPEDWINDOW,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(BUTTON_SIZE + INTERVAL) * 5 + BUTTON_START_X * 3,
		SCREEN_HEIGHT + (BUTTON_SIZE + INTERVAL) * 5 + INTERVAL * 2,
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
	static DOUBLE a = 0, b = 0; //переменные
	static INT operation = 0;
	static BOOL input = false; //Отслеживает ввод чисел
	static BOOL input_operation = false; //отслеживает ввож операций

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//Строка ввода
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILDWINDOW | WS_VISIBLE /*| WS_BORDER*/ | ES_RIGHT | ES_READONLY,
			START_X, START_Y,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);

		// Создаём шрифт для Edit
		HFONT hFont = CreateFont
		(
			DISPLAY_FONT_HEIHGT, DISPLAY_FONT_WIDTH,
			GM_ADVANCED, 0 , 600,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, DISPLAY_FONT
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

		///////////////////////////////////////////////////////////////////////
		//Создание цифровой панели
		CHAR sz_bitmap_path[MAX_PATH]{};
		HBITMAP hBitmap_image[9];
		for (int i = 0; i < sizeof(hBitmap_image) / sizeof(hBitmap_image[0]); i++)
		{
			sprintf(sz_bitmap_path, "%s%i.bmp", BUTTON_IMAGES_PATH, i+1);
			hBitmap_image[i] = (HBITMAP)LoadImage(NULL, sz_bitmap_path, IMAGE_BITMAP, BUTTON_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		}

		int digit = 0;
		char sz_digit[2] = "";
		HWND hBitmap_button[9];
		for (int i = 6, k=0; i >= 0; i -= 3)
			for (int j = 0; j < 3; j++)
			{
				digit++;
				sz_digit[0] = digit + 48;
				hBitmap_button[k] = CreateWindowEx
				(
					NULL, "Button", (LPCSTR)sz_digit,
					WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * j,
					BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * i / 3,
					BUTTON_SIZE, BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit),
					GetModuleHandle(NULL), NULL
				);
				SendMessage(hBitmap_button[k], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_image[k]);
				k++;
			}

		//Создание нуля
		HBITMAP hBitmap_0 = (HBITMAP)LoadImage(NULL, "Button_images\\0.bmp", IMAGE_BITMAP, BUTTON_DOUBLE_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		HWND hButton_0 = CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X, BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * 3,
			BUTTON_DOUBLE_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_0),
			GetModuleHandle(NULL), NULL
		);
		SendMessage(hButton_0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_0);

		//Создание точки
		HBITMAP hBitmap_Point = (HBITMAP)LoadImage(NULL, "Button_images\\Point.bmp", IMAGE_BITMAP, BUTTON_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		HWND hButton_Point = CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + BUTTON_DOUBLE_SIZE + INTERVAL,
			BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * 3,
			BUTTON_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_POINT),
			GetModuleHandle(NULL), NULL
		);
		SendMessage(hButton_Point, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_Point);

		//Создане операций
		HBITMAP hBitmap_operations[4];
		HWND hButton_operations[4];
		for (int i = 0; i < sizeof(hBitmap_operations) / sizeof(hBitmap_operations[0]); i++)
		{
			sprintf(sz_bitmap_path, "%s%s", BUTTON_IMAGES_PATH, BUTTON_OPERATIONS_IMAGES_PATH[i]);
			hBitmap_operations[i] = (HBITMAP)LoadImage(NULL, sz_bitmap_path, IMAGE_BITMAP, BUTTON_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		}
		for (size_t i = 0; i < sizeof(OPERATIONS) / sizeof(OPERATIONS[0]); i++)
		{
			hButton_operations[i] = CreateWindowEx
			(
				NULL, "Button", OPERATIONS[i],
				WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 3,
				BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * i,
				BUTTON_SIZE, BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_SLASH - i),
				GetModuleHandle(NULL), NULL
			);
			SendMessage(hButton_operations[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_operations[i]);
		}

		//Кнопка очистки
		HBITMAP hBitmap_clear = (HBITMAP)LoadImage(NULL, "Button_images\\Clear.bmp", IMAGE_BITMAP, BUTTON_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		HWND hButton_clear = CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4, BUTTON_START_Y + BUTTON_SIZE + INTERVAL,
			BUTTON_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLEAR,
			GetModuleHandle(NULL),
			NULL
		);
		SendMessage(hButton_clear, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_clear);

		//Кнопка равно
		HBITMAP hBitmap_equal = (HBITMAP)LoadImage(NULL, "Button_images\\Equals.bmp", IMAGE_BITMAP, BUTTON_SIZE, BUTTON_DOUBLE_SIZE,  LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		HWND hButton_equal = CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4,
			BUTTON_START_Y + BUTTON_DOUBLE_SIZE + INTERVAL,
			BUTTON_SIZE, BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL), NULL
		);
		SendMessage(hButton_equal, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_equal);

		//Кнопка Backspace
		HBITMAP hBitmap_BSP = (HBITMAP)LoadImage(NULL, "Button_images\\BSP.bmp", IMAGE_BITMAP, BUTTON_SIZE, BUTTON_SIZE, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		HWND hButton_BSP = CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4, BUTTON_START_Y,
			BUTTON_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		SendMessage(hButton_BSP, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap_BSP);
	}
	break;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, RGB(0, 0, 100));
		HBRUSH hBrush  = CreateSolidBrush(RGB(250, 0, 0));
		SetTextColor(hdc, RGB(255, 0, 0));
		return (LRESULT)hBrush;
	}
	break;

	case WM_COMMAND:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		// Цифры с точкой
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9 || LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (!input) SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
			input = true;
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
		// Операторы
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			a = b = 0;
			operation = 0;
			input = false;
			input_operation = false;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
		}

		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			if (strcmp(sz_buffer, "0") == 0 || strlen(sz_buffer) == 0) break;
			sz_buffer[strlen(sz_buffer) - 1] = 0;
			if (strlen(sz_buffer) == 0) SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
			else SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}

		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
			{
				CHAR sz_buffer[MAX_PATH]{};
				SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
				b = atof(sz_buffer);
				input = false;
				if (a == 0) a = b;
			}
			if (input_operation) SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);
			operation = LOWORD(wParam);
			input_operation = true;
		}

		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)
			{
				CHAR sz_buffer[MAX_PATH]{};
				SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
				b = atof(sz_buffer);
				input = false;
				if (a == 0) a = b;
			}
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH: a /= b; break;
			}
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			input_operation = false;
			sprintf(sz_buffer, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT) < 0)
		{
			if (wParam == 0x38)SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);
		}
		else if (LOWORD(wParam) >= 0x30 && LOWORD(wParam) <= 0x39)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x30 + IDC_BUTTON_0, 0);
		}
		if (LOWORD(wParam) >= 0x60 && LOWORD(wParam) <= 0x69)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x60 + IDC_BUTTON_0, 0);
		}
		switch (wParam)
		{
		case VK_OEM_PLUS:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_PLUS, 0);	break;
		case VK_OEM_MINUS:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MINUS, 0);	break;
		case VK_MULTIPLY:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);	break;
		case VK_OEM_2:
		case VK_DIVIDE:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_SLASH, 0);	break;
		case VK_OEM_PERIOD:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_POINT, 0);	break;
		case VK_BACK:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_BSP, 0);	break;
		case VK_ESCAPE:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_CLEAR, 0);	break;
		case VK_RETURN:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);	break;
		}
	}
	break;
	case WM_DESTROY:	PostQuitMessage(0); break;
	case WM_CLOSE:		DestroyWindow(hwnd); break;
	default:			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}