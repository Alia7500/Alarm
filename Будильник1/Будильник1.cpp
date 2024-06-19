#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  
#define WM_USER_UPDATE_NOTE (WM_USER + 1) // Определение пользовательского сообщения для обновления заметки
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib,"Comctl32.lib")

#include "resource.h"
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <wchar.h>
#include <iostream>
#include <SDKDDKVer.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <ctime>
#include <chrono>


HINSTANCE hInst;
HBRUSH hBrushEditTopic;
HWND hClild;
HWND hUpd;
HWND hParent;
HWND hHour;
HWND hMin;
HWND hSec;
HWND hNoteText;
HWND hButton1{};
HWND hButton2{};
HWND hButton3{};
HWND hButton4{};
HWND hButton5{};
HWND hButton6{};
HWND hButton[6] = { hButton1, hButton2, hButton3, hButton4, hButton5, hButton6 };
int selectslot;

struct Alarm {
	UINT ID = 9;
	wchar_t min[3];
	wchar_t hour[3];
	wchar_t sec[3];
}al;
Alarm Alarms[6];

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcD(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{};
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Родитель";
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(300, 198, 242));
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	hInst = hInstance;
	if (!RegisterClassEx(&wc)) return EXIT_FAILURE;
	hwnd = CreateWindow(wc.lpszClassName, L"Будильник", WS_OVERLAPPED | WS_SYSMENU, 200, 210, 315, 240, nullptr, nullptr, wc.hInstance, nullptr);
	hParent = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)(msg.wParam);
}
INT CALLBACK CreateWindowChild(HWND hwnd) {
	WNDCLASSEX wcChild{};
	wcChild.cbSize = sizeof(WNDCLASSEX);
	wcChild.style = CS_HREDRAW | CS_VREDRAW;
	wcChild.lpfnWndProc = WndProcD;
	wcChild.cbClsExtra = 0;
	wcChild.cbWndExtra = 0;
	wcChild.hInstance = hInst;
	wcChild.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcChild.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(230, 230, 250));
	wcChild.lpszClassName = L"ChildClass";
	if (!RegisterClassEx(&wcChild))
		return EXIT_FAILURE;
	HWND hChildWnd = CreateWindow(wcChild.lpszClassName, L"Создать", WS_OVERLAPPED | WS_SYSMENU, 200, 100, 167, 130, hwnd, nullptr, nullptr, nullptr);
	hClild = hChildWnd;
	UpdateWindow(hChildWnd);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	//обработка сообщений главного окна
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		for (int i = 0; i < 6; i++) {
			hButton[i] = CreateWindowEx(0, L"BUTTON", L"Пусто",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, i * 30, 300, 30, hwnd, (HMENU)(200+i), hInst, NULL);
		}
		
		CreateWindowChild(hwnd);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	return 0;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
		{
			DestroyWindow(hwnd);
		}
		break;
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		}
		break;
		case 201:
		{
			if (Alarms[1].ID == 9) {
				selectslot = 1;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[1].ID = 9;
					SetWindowText(hButton[1], L"Пусто");
					KillTimer(hwnd, 1);
				}
			}
		}
		break;
		case 202:
		{
			if (Alarms[2].ID == 9) {
				selectslot = 2;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[2].ID = 9;
					SetWindowText(hButton[2], L"Пусто");
					KillTimer(hwnd, 2);
				}
			}
		}
		break;
		case 203:
		{
			if (Alarms[3].ID == 9) {
				selectslot = 3;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[3].ID = 9;
					SetWindowText(hButton[3], L"Пусто");
					KillTimer(hwnd, 3);
				}
			}
		}
		break;
		case 204:
		{
			if (Alarms[4].ID == 9) {
				selectslot = 4;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[4].ID = 9;
					SetWindowText(hButton[4], L"Пусто");
					KillTimer(hwnd, 4);
				}
			}
		}
		break;
		case 205:
		{
			if (Alarms[5].ID == 9) {
				selectslot = 5;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[5].ID = 9;
					SetWindowText(hButton[5], L"Пусто");
					KillTimer(hwnd, 5);
				}
			}
		}
		break;
		case 200:
		{
			if (Alarms[0].ID == 9) {
				selectslot = 0;
				ShowWindow(hClild, SW_RESTORE);
			}
			else {
				int result = MessageBox(hwnd, L"Вы хотите удалить будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Alarms[0].ID = 9;
					SetWindowText(hButton[0], L"Пусто");
					KillTimer(hwnd, 0);
				}
			}
		}
		break;
		}
	}
	return 0;
	case WM_TIMER:
	{
		int ID_Alarm = wParam;
		KillTimer(hwnd, ID_Alarm);
		MessageBox(hwnd, L"Будильник сработал!", L"Будильник", MB_OK);
		Alarms[ID_Alarm].ID = 9;
		SetWindowText(hButton[ID_Alarm], L"Пусто");
	}
	return 0;
	case WM_DESTROY:
	{
		for (int i = 0; i < 6; i++) {
			if (Alarms[i].ID != 9) {
				KillTimer(hwnd, i);
			}
		}
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK WndProcD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	//обработка сообщений главного окна
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		CreateWindowEx(0, L"STATIC", L"Введите час:",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 130, 20, hwnd, NULL, hInst, NULL);
		hHour = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 130, 0, 20, 20, hwnd, (HMENU)ID_HOUR, hInst, NULL);
		CreateWindowEx(0, L"STATIC", L"Введите минуту:",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 20, 130, 20, hwnd, NULL, hInst, NULL);
		hMin = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 130, 20, 20, 20, hwnd, (HMENU)ID_MINUTE, hInst, NULL);
		CreateWindowEx(0, L"STATIC", L"Введите секунду:",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 40, 130, 20, hwnd, NULL, hInst, NULL);
		hSec = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 130, 40, 20, 20, hwnd, (HMENU)ID_SECOND, hInst, NULL);
		HWND hButton = CreateWindowEx(0, L"BUTTON", L"Создать",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 60, 150, 30, hwnd, (HMENU)ID_ADDALARM, hInst, NULL);
		break;
	}
	return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
		{
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hParent, SW_RESTORE);
		}
		break;
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		}
		break;
		case ID_ADDALARM:
		{
			int hour, minute, second;
			GetWindowText(hHour, al.hour, sizeof(al.hour));
			GetWindowText(hMin, al.min, sizeof(al.min));
			GetWindowText(hSec, al.sec, sizeof(al.sec));
			hour = std::stoi(al.hour);
			minute = std::stoi(al.min);
			second = std::stoi(al.sec);
			int result = MessageBox(hwnd, L"Вы действительно хотите создать будильник?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) {
				al.ID = selectslot;
				wchar_t times[9];
				times[0] = al.hour[0];
				times[1] = al.hour[1];
				times[2] = L':';
				times[3] = al.min[0];
				times[4] = al.min[1];
				times[5] = L':';
				times[6] = al.sec[0];
				times[7] = al.sec[1];
				times[8] = L'\0';
				Alarms[selectslot] = al;
				auto now = std::chrono::system_clock::now();
				time_t now_time_t = std::chrono::system_clock::to_time_t(now);
				tm now_tm = *std::localtime(&now_time_t);
				int now_seconds = now_tm.tm_hour * 3600 + now_tm.tm_min * 60 + now_tm.tm_sec;
				int alarm_second = hour * 3600 + minute * 60 + second;
				int difference_seconds = alarm_second - now_seconds;
				if (difference_seconds < 0) {
					difference_seconds += 24 * 3600; // добавляем количество секунд в одном дне
				}
				int difference_milliseconds = difference_seconds * 1000;
				SetTimer(hParent, selectslot, difference_milliseconds, NULL);
				ShowWindow(hClild, SW_HIDE);  // Закрываем дочернее окно после сохранения
				SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowText(hHour, L"");
				SetWindowText(hMin, L"");
				SetWindowText(hSec, L"");
				SetWindowText(hButton[selectslot], times);
			}
		}
		break;
		}
	}
	return 0;
	case WM_CLOSE:
	{
		ShowWindow(hwnd, SW_HIDE);
		SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowText(hHour, L"");
		SetWindowText(hMin, L"");
		SetWindowText(hSec, L"");
	}
	return 0;
	case WM_DESTROY:
	{
		DeleteObject(hBrushEditTopic);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

