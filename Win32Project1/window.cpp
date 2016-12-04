#include <windows.h>
#include "windowsx.h"
#include <iostream>
#include <string>

int Schet = 0;
int xC = 0, yC = 0;

// объ€вление функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
bool justONE = true;
HINSTANCE hinst;

////////////////////////////////////////////////////////////////////////// 
// функци€ вхождений программы WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{

	// им€ будущего класса
	LPCTSTR lpzClass = TEXT("My Window Class!");

	// регистраци€ класса
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	// вычисление координат центра экрана
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	// создание диалогового окна
	HWND hWnd = CreateWindow(lpzClass, TEXT("Title"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, 300, 150, NULL, NULL,
		hInstance, NULL);

	// если окно не создано, описатель будет равен 0
	if (!hWnd) return 2;

	// цикл сообщений приложени€
	MSG msg = { 0 };    // структура сообщени€
	int iGetOk = 0;   // переменна€ состо€ни€
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // цикл сообщений
	{
		if (iGetOk == -1) return 3;  // если GetMessage вернул ошибку - выход
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;  // возвращаем код завершени€ программы
}

////////////////////////////////////////////////////////////////////////// 
// функци€ регистрации класса окон
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// адрес ф-ции обработки сообщений
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// стиль окна
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// дискриптор экземпл€ра приложени€
	wcWindowClass.hInstance = hInst;
	// название класса
	wcWindowClass.lpszClassName = lpzClassName;
	// загрузка курсора
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// загрузка цвета окон
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // регистраци€ класса
}

////////////////////////////////////////////////////////////////////////// 
// функци€ обработки сообщений
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
	int x = screen_rect.right / 2 - rand()%250;
	int y = screen_rect.bottom / 2 - rand() %275;

	// выборка и обработка сообщений
	switch (message)
	{
	case WM_LBUTTONUP:
		// реакци€ на сообщение
		if (justONE) {
			SetWindowText(hWnd, L"changed Title");
			SetWindowPos(hWnd, HWND_TOP, x, y, 400, 200, NULL);
			justONE = false;
			xC = x + 100; yC = y + 100;
		}
		else if(Schet < 5){
			WNDCLASS w;
			LPCTSTR lpzClass = TEXT("My Window Class!");
			memset(&w, 0, sizeof(WNDCLASS));
			w.lpfnWndProc = ChildProc;
			w.hInstance = hinst;
			w.hbrBackground = GetStockBrush(WHITE_BRUSH);
			w.lpszClassName = TEXT("ChildWClass");
			w.hCursor = LoadCursor(NULL, IDC_CROSS);
			
			RegisterClass(&w);
			HWND child;

			child = CreateWindow(lpzClass, TEXT("ChildWClass"),	WS_OVERLAPPEDWINDOW | WS_VISIBLE, xC , yC, 400, 200, NULL, NULL, hinst, NULL);
			ShowWindow(child, SW_NORMAL);
			xC += 100; yC += 100;
			Schet++;
			UpdateWindow(child);
		}
		break;

	case WM_RBUTTONUP:
		Schet--;
		xC -= 100; yC -= 100;
		DefWindowProc(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_CLOSE:  
		if (IDOK == MessageBox(hWnd, L"Exit programm?", L"Close", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)) {
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);   
			PostQuitMessage(NULL);
		}
		break;
	default:
		// все сообщени€ не обработанные обработает сам Windows
		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	return 0;
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	if (Message == WM_DESTROY)
	{
		return 0;
	}
	return DefWindowProc(hwnd, Message, wparam, lparam);
}