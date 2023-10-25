#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Window Class"; //��� ������ ����

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParem, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// ������� ���� �������� � 3 �����:
	// 1) ����������� ����;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc); //cb - Count Bytes (���������� ������)
	wc.cbWndExtra = 0; // �������������� ����� ����
	wc.cbClsExtra = 0; // �������������� ����� ������ ����
	wc.style = 0;
	
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_RECYCLE)); // ������ � ��������� ����
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_WALLPAPER)); //Sm -Small. ������ � ���� ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	wc.hInstance = hInstance; //hInstance - ��� ��������� ������������ ����� ��������� � ������
	// � ������� WinMain()  ��������� hInstance ��� ��������, � ������� � ���� ���� ������ ������, 
	// � ����� ������ ������� hInstance ������ ����� �������� � ������� ������� GetModuleHandle(NULL)
	wc.lpfnWndProc = WndProc; //��������� �� ��������� ����
	wc.lpszMenuName = NULL; //�������� �� ���� ����(NULL - ��� ���� �����������)
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, ("Call Register failed!"), ("Error"), MB_OK | MB_ICONERROR);
		return 0;
	}
	// 2) �������� ����;
	HWND hwnd = CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW, //�������� ������������ ����� ����(�� ����������)
		g_sz_WINDOW_CLASS, //��� ����������
		g_sz_WINDOW_CLASS, //����� ������������ � ������ ��������� ����
		WS_OVERLAPPEDWINDOW, //��� ������������ ����, � �������� ���� ������ ����� ����� �����
		
		CW_USEDEFAULT, //��������� ������� ���� �� �
		CW_USEDEFAULT, //��������� ������� �� Y
		CW_USEDEFAULT, //��������� ������� ���� �� �
		CW_USEDEFAULT, //����� ����
		
		NULL, //�������� ���� ������� ����
		NULL, //hMenu - ������ ���� ����( NULL - �� �����). ��� ��������� ����, ������� ��������� ��������� ������� ����, � hMenu ��������� ID ������� ����� ��������.
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

	// 3) ������ ����� ���������;
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
		case WM_CREATE:
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:	PostQuitMessage(0); break;
		case WM_CLOSE:		DestroyWindow(hwnd); break;
		default:			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}