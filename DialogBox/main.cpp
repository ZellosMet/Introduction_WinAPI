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
			SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"Введите имя пользователя");
			SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)"Введите пароль");
			//SetFocus(GetDlgItem(hwnd, IDC_EDIT_LOGIN));
		}
		break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_COPY:
				{
					//Создаём буффер чтения:
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE] = {};
					//Получаем обработчик текстового полей
					HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //Функция GetDlgItem() по ID-резурса дочернего окна возвращает HWND соответствующего дочернего окна
					HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD); //Функция GetDlgItem() по ID-резурса дочернего окна возвращает HWND соответствующего дочернего окна
					//Читаем текст из текстового поля
					SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
					//Загружаем содержимоего текстового буфера в поле 'Password'
					if (strcmp(sz_buffer, "Введите имя пользователя") != 0)
						SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
				}break;

				case IDC_EDIT_LOGIN:
				{
					//Edit_SetCueBannerTextFocused(GetDlgItem(hwnd, IDC_EDIT_LOGIN), (LPARAM)"Введите имя пользователя", TRUE);
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE] = {};
					SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
					if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, "Введите имя пользователя") == 0)
						SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"");
					if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
						SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"Введите имя пользователя");
				}break;

				case IDC_EDIT_PASSWORD:
				{
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE] = {};
					SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
					if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, "Введите пароль") == 0)
						SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)"");
					if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
						SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)"Введите пароль");
				}break;

				case IDOK:		MessageBox(hwnd, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION); break;
				case IDCANCEL:	EndDialog(hwnd, 0); break;
			}
		break;
		case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}