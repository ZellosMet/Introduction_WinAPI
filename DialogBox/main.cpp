#include<Windows.h>
#include<commctrl.h>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
			//HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			//SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"������� ��� ������������");
			//SetFocus(GetDlgItem(hwnd, IDC_EDIT_LOGIN));
			Edit_SetCueBannerTextFocused(GetDlgItem(hwnd, IDC_EDIT_LOGIN), L"fsdf", TRUE);
		}
		break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_COPY:
				{
					//������ ������ ������:
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE] = {};
					//�������� ���������� ���������� �����
					HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //������� GetDlgItem() �� ID-������� ��������� ���� ���������� HWND ���������������� ��������� ����
					HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD); //������� GetDlgItem() �� ID-������� ��������� ���� ���������� HWND ���������������� ��������� ����
					//������ ����� �� ���������� ����
					SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
					//��������� ������������ ���������� ������ � ���� 'Password'
					SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
				}
				break;
				case IDOK:		MessageBox(hwnd, "���� ������ ������ OK", "Info", MB_OK | MB_ICONINFORMATION); break;
				case IDCANCEL:	EndDialog(hwnd, 0);
			}
		break;
		case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}