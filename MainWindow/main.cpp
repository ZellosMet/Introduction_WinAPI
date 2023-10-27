#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Window Class"; //Имя класса окна

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParem, LPARAM lParam);
void MoveSizeEvent(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{

	// Главное окно создаётся в 3 этапа:
	// 1) Регистрация окна;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc); //cb - Count Bytes (Количество байтов)
	wc.cbWndExtra = 0; // Дополнительный байты окна
	wc.cbClsExtra = 0; // Дополнительный байты класса окна
	wc.style = 0;
	
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_RECYCLE)); // Иконка в панели задач
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_WALLPAPER)); //Sm -Small. Иконка в заголовке окна
	wc.hIcon = ExtractIcon(hInstance, "napster.ico", 0);
	wc.hIconSm = ExtractIcon(hInstance, "mailchimp.ico", 0);


	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Link_Select.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wc.hInstance = hInstance; //hInstance - это экземпляр исполняемого файла программы в памяти
	// В функции WinMain()  принимает hInstance как параметр, и поэтому к нему есть прямой доступ, 
	// в любой другой функции hInstance всегда можно получить с помощью функции GetModuleHandle(NULL)
	wc.lpfnWndProc = WndProc; //Указатель на процедуру окна
	wc.lpszMenuName = NULL; //Указание на меню окна(NULL - нет меню поумолчанию)
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, ("Call Register failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	// 2) Создание окна;
	HWND hwnd = CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW, //Параметр расширенного стяли окна(не обязателен)
		g_sz_WINDOW_CLASS, //Имя приложения
		g_sz_WINDOW_CLASS, //Текст отображаемый в строке заголовка окна
		WS_OVERLAPPEDWINDOW, //Тип создаваемого окна, у главного окна всегда будет такой стиль
		
		CW_USEDEFAULT, //Начальная позиция окна по Х
		CW_USEDEFAULT, //Начальная позиция по Y
		GetSystemMetrics(SM_CXSCREEN)*3/4,
		GetSystemMetrics(SM_CYSCREEN)*3/4,
		//CW_USEDEFAULT, //Ширина окна
		//CW_USEDEFAULT, //Длина окна
		
		NULL, //Родитель окна данного окна
		NULL, //hMenu - Строка меню окна( NULL - не имеет). Для дочернего окна, который являкется элементом другого окна, в hMenu передаётся ID ресурса этого элемента.
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

	// 3) Запуск цикла сообщений;
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
	switch(uMsg)
	{
		case WM_CREATE:		break;
		case WM_SIZE:		MoveSizeEvent(hwnd); break;
		case WM_MOVE:		MoveSizeEvent(hwnd); break;
		case WM_COMMAND:	break;
		case WM_DESTROY:	PostQuitMessage(0); break;
		case WM_CLOSE:		DestroyWindow(hwnd); break;
		default:			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}

void MoveSizeEvent(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE] = {};
	sprintf(sz_buffer, "%s: Положение окна (%i x %i) Размер окна (%i x %i)", g_sz_WINDOW_CLASS, rect.left, rect.top, width, height);
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
}
