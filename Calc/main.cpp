#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

// Название окна
CONST CHAR g_sz_CLASSNAME[] = "Calculator";
//Начальная позиция Edit
CONST INT START_X = 10;
CONST INT START_Y = 10;
//Размеры кнопок
CONST INT BUTTON_SIZE = 80;
CONST INT INTERVAL = 5;
CONST INT BUTTON_DOUBLE_SIZE = BUTTON_SIZE * 2 + INTERVAL;
//Размеры окна
CONST INT SCREEN_WIDTH = (BUTTON_SIZE) * 5 + INTERVAL * 4;
//Размер Edit
CONST INT SCREEN_HEIGHT = 50;
//Настройки стиля окна
CONST CHAR DISPLAY_FONT[] = "Tahoma";
CONST INT DISPLAY_FONT_HEIHGT = SCREEN_HEIGHT - 2;
CONST INT DISPLAY_FONT_WIDTH = DISPLAY_FONT_HEIHGT / 2.5;
//Начальное положение кнопок
CONST INT BUTTON_START_X = START_X;
CONST INT BUTTON_START_Y = START_Y + SCREEN_HEIGHT + INTERVAL * 2;
//Массив операций
CONST CHAR* OPERATIONS[] = { "/", "*", "-", "+" };
//Массив названия файлов
CONST CHAR* BUTTON_NAMES[] = { "point", "plus", "minus", "aster", "slash", "equal", "clear", "back" };

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR sz_skin[]);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmsLine, INT nCmdShow)
{
	//Класс окна
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;

	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);

	wc.hInstance = hInstance;
	wc.lpszClassName = g_sz_CLASSNAME;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK);
		return 0;
	}
	//Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASSNAME,
		g_sz_CLASSNAME,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(BUTTON_SIZE + INTERVAL) * 5 + BUTTON_START_X * 3, SCREEN_HEIGHT + (BUTTON_SIZE + INTERVAL) * 5 + INTERVAL * 2,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//Цикл сообщений
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DOUBLE a = 0, b = 0; //Переменные для расчёта
	static INT operation = 0; // ID-операций
	static BOOL input = false; //Ввод чисел
	static BOOL input_operation = false; //Ввод операций
	static CHAR sz_style_button[FILENAME_MAX] = {}; //Путь к файлам стилей
	static COLORREF crBackground  = RGB(0, 0, 255); //Задание цвета фона окна

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//Создание Edit
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "Screen",
			WS_CHILDWINDOW | WS_VISIBLE | /*WS_BORDER |*/ ES_RIGHT /*| ES_READONLY*/,
			START_X, START_Y,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		//Создание стиля окна
		HFONT hFont = CreateFont
		(
			DISPLAY_FONT_HEIHGT, DISPLAY_FONT_WIDTH,
			GM_ADVANCED, 0, 600,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,
			ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			DISPLAY_FONT
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

		/////////////////////////////////////////////////////////////////////////////////
		//Создание цифровых кнопок 1-9
		int digit = 0;
		char sz_digit[2] = "";
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				digit++;
				sz_digit[0] = digit + 48;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * j,
					BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * i / 3,
					BUTTON_SIZE, BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		//Создание 0
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X, BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * 3,
			BUTTON_DOUBLE_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
		);
		//Создание точки
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + BUTTON_DOUBLE_SIZE + INTERVAL, BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * 3,
			BUTTON_SIZE, BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_POINT),
			GetModuleHandle(NULL),
			NULL
		);
		//Создание операций
		for (int i = 0; i < sizeof(OPERATIONS) / sizeof(OPERATIONS[0]); i++)
		{
			CreateWindowEx
			(
				NULL, "Button", OPERATIONS[i],
				WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 3, BUTTON_START_Y + (BUTTON_SIZE + INTERVAL) * i,
				BUTTON_SIZE, BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_SLASH - i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		//создание Backspace
		CreateWindowEx
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
		//Создание очистки
		CreateWindowEx
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
		//Создание равно
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_START_X + (BUTTON_SIZE + INTERVAL) * 4, BUTTON_START_Y + BUTTON_DOUBLE_SIZE + INTERVAL,
			BUTTON_SIZE, BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		SetSkin(hwnd, "square_blue"); //Функция задание стиля кнопок
	}
	break;
	case WM_PAINT:
	{
		HBRUSH hbrBackground = CreateSolidBrush(crBackground);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(ps.hdc, &ps.rcPaint, hbrBackground);
		EndPaint(hwnd, &ps);
	}
	break;
	//Событие изменения стиля
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, RGB(0, 0, 100));
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SetTextColor(hdc, RGB(255, 0, 0));
		return (LRESULT)hBrush;
	}
	break;
	//Событие команд
	case WM_COMMAND:
	{
		//Смена фокуса с Edit на окно
		if (LOWORD(wParam) == IDC_EDIT && HIWORD(wParam) == EN_SETFOCUS)SetFocus(hwnd);

		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		//Опрос кнопок 0-9 и точки
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9 || LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (!input)SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
			input = true;
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			if (strcmp(sz_buffer, "Screen") == 0)
			{
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
				sz_buffer[0] = 0;
			}
			CHAR sz_digit[2]{};
			if (LOWORD(wParam) == IDC_BUTTON_POINT)
			{
				if (strchr(sz_buffer, '.'))break;
				sz_digit[0] = '.';
			}
			else sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48;
			strcat(sz_buffer, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		//Опрос кнопок операция
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
			{
				CHAR sz_buffer[MAX_PATH]{};
				SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
				b = atof(sz_buffer);
				input = false;
				if (a == 0)a = b;
			}
			if (input_operation)SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);
			operation = LOWORD(wParam);
			input_operation = true;
		}
		//Опрос равно
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)
			{
				CHAR sz_buffer[MAX_PATH]{};
				SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
				b = atof(sz_buffer);
				input = false;
				if (a == 0)a = b;
			}
			switch (operation)
			{
				case IDC_BUTTON_PLUS:	a += b;	break;
				case IDC_BUTTON_MINUS:	a -= b;	break;
				case IDC_BUTTON_ASTER:	a *= b;	break;
				case IDC_BUTTON_SLASH:	a /= b;	break;
			}
			input_operation = false;
			CHAR sz_buffer[MAX_PATH]{};
			sprintf(sz_buffer, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		//Опрос Backspace
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			CHAR sz_buffer[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			if (sz_buffer[0])sz_buffer[strlen(sz_buffer) - 1] = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		//Опрос очистки
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			a = b = 0;
			operation = 0;
			input = false;
			input_operation = false;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
		}
	}
	break;
	//Событие нажатия клавишь
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT) < 0)
			if (wParam == 0x38)SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);
		else if (LOWORD(wParam) >= 0x30 && LOWORD(wParam) <= 0x39)
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x30 + IDC_BUTTON_0, 0);
		if (LOWORD(wParam) >= 0x60 && LOWORD(wParam) <= 0x69)
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x60 + IDC_BUTTON_0, 0);
		switch (LOWORD(wParam))
		{
			case VK_OEM_PLUS:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_PLUS, 0);	break;
			case VK_OEM_MINUS:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MINUS, 0); break;
			case VK_MULTIPLY:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0); break;
			case VK_OEM_2:
			case VK_DIVIDE:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_SLASH, 0); break;
			case VK_OEM_PERIOD:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_POINT, 0); break;
			case VK_RETURN:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0); break;
			case VK_ESCAPE:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_CLEAR, 0); break;
			case VK_BACK:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_BSP, 0);	break;
		}
	}
	break;
	//Событие контекстного меню
	case WM_CONTEXTMENU:
	{
		HMENU hContextMenu = CreatePopupMenu();
		InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, IDC_CLOSE, "Close");
		InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, IDC_SQUARE, "Style Square Buttons");
		InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, IDC_ROUND, "Style Round Buttons");
		InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, IDC_COOL, "Style Cool Buttons");
		HBRUSH hbrBackground = CreateSolidBrush(RGB(0,0,240));
		switch (TrackPopupMenuEx(hContextMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, LOWORD(lParam), HIWORD(lParam), hwnd, NULL))
		{
			case IDC_COOL:		strcpy(sz_style_button, "cool_button");	hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); break;
			case IDC_ROUND:		strcpy(sz_style_button, "round_green");	hbrBackground = CreateSolidBrush(RGB(0, 240, 0)); break;
			case IDC_SQUARE:	strcpy(sz_style_button, "square_blue");	hbrBackground = CreateSolidBrush(RGB(0, 240, 0)); break;
			case IDC_CLOSE:		SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		SetSkin(hwnd, sz_style_button);
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hbrBackground);
	}
	break;
	//События закрытия окна
	case WM_DESTROY:PostQuitMessage(0); break;
	case WM_CLOSE:	DestroyWindow(hwnd); break;
	default:		return DefWindowProc(hwnd, uMsg, wParam, lParam); break;
	}
	return NULL;
}
//Функция смены стиля кнопок
VOID SetSkin(HWND hwnd, CONST CHAR sz_skin[])
{
	CONST INT SIZE = 10;
	CHAR sz_filename[FILENAME_MAX] = {};
	//Цифровые кнопки
	for (int i = 0; i < SIZE; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
		sprintf(sz_filename, "ButtonsBMP\\%s\\button_%i.bmp", sz_skin, i);
		HBITMAP hBitmap = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			i > 0 ? BUTTON_SIZE : BUTTON_DOUBLE_SIZE, BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
	}
	//Остальное
	for (int i = 0; i < sizeof(BUTTON_NAMES) / sizeof(BUTTON_NAMES[0]); i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_POINT + i);
		sprintf(sz_filename, "ButtonsBMP\\%s\\button_%s.bmp", sz_skin, BUTTON_NAMES[i]);
		HBITMAP hBitmap = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			BUTTON_SIZE, i == 5 ? BUTTON_DOUBLE_SIZE : BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
	}
}
