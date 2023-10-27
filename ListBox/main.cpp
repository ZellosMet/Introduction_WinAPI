#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<cstdio>

//CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AddProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		//for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
			//SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_ADD:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)AddProc, 0);
		}
		break;
		case IDC_DELETE:
		{
			if (SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCURSEL, 0, 0) != LB_ERR)
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_DEL), hwnd, (DLGPROC)DelProc, 0);
		}
		break;
		case IDOK:	
		{
			const int SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			int i = SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE]{};
			sprintf(sz_message, "Вы выбрали элемент № %i, со значением \"%s\"", i, sz_buffer);
			if (SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCURSEL, 0, 0) != LB_ERR)
				MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK AddProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ELEMENT);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			EndDialog(hwnd, 0);
		}
		break;
		case IDC_BUTTON_CANCEL: EndDialog(hwnd, 0); break;
		}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0); 
	}
	return FALSE;
}

BOOL CALLBACK DelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	
	{
		const int SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		HWND hParent = GetParent(hwnd);
		int i = SendMessage(GetDlgItem(hParent, IDC_LIST1), LB_GETCURSEL, 0, 0);
		SendMessage(GetDlgItem(hParent, IDC_LIST1), LB_GETTEXT, i, (LPARAM)sz_buffer);
		CHAR sz_message[SIZE]{};
		sprintf(sz_message, "Удалить элемент \"%s\"?", sz_buffer);
		SetWindowTextA(GetDlgItem(hwnd, IDC_TEXT), sz_message);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, i, (LPARAM)sz_buffer);
		}
		break;
		}
	case IDCANCEL: EndDialog(hwnd, 0);
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}