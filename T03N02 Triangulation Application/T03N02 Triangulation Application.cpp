#include "stdafx.h"
#include "T03N02 Triangulation Application.h"

// Load Dynamic Library
#define TYPE_INCLUDE_ONLY
#include "../T03N02_Triangulation_Library/triangulation.h"

// Load DLL Functions
HMODULE hmTriangulation = nullptr;

typedef LineList(*LPTriangulation)(const PointList&);
LPTriangulation Triangulate = nullptr;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Solution variables
PointList Points;
LineList Triangulation;

// Solution constants
const SHORT PointRadius = 3;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	hmTriangulation = LoadLibrary(L"triangulation.dll");
	Triangulate = //LPTriangulation(0x000116AE);
		(LPTriangulation)GetProcAddress(hmTriangulation,
		"?Triangulate@@YA?AV?$vector@ULINE@@V?$allocator@ULINE@@@std@@@std@@ABV?$vector@U_COORD@@V?$allocator@U_COORD@@@std@@@2@@Z");

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.
	MSG msg;
	DispatchMessage(&msg);
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_T03N02TRIANGULATIONAPPLICATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_T03N02TRIANGULATIONAPPLICATION));

	// Main message loop:
	BOOL ret = 1;
	while (ret)
	{
		ret = GetMessage(&msg, NULL, 0, 0);
		if ((int)ret != -1)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		return(ret);
	}
	return (int) msg.wParam;
}

//Registers the window class.

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_T03N02TRIANGULATIONAPPLICATION));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_T03N02TRIANGULATIONAPPLICATION);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}


//  Saves instance handle and creates main window

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   CreateWindow(L"Button", L"Triangulate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   8, 8, 100, 40, hWnd, HMENU(IDM_TRIANGULATE), hInst, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	COORD Point;
	RECT Rect;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_TRIANGULATE:
			// Generating Triangulation

			Triangulation.clear();
			Triangulation = Triangulate(Points);
			// Error message if we have too few points
			if (Triangulation.empty())
				MessageBox(hWnd, L"Too few points", L"Error", MB_OK | MB_ICONERROR);

			// Repainting window
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		Point.X = GET_X_LPARAM(lParam);
		Point.Y = GET_Y_LPARAM(lParam);
		Points.push_back(Point);

		// Repainting area nearest click-point
		Rect.left   = Point.X - 2 * PointRadius;
		Rect.top    = Point.Y - 2 * PointRadius;
		Rect.right  = Point.X + 2 * PointRadius;
		Rect.bottom = Point.Y + 2 * PointRadius;
		InvalidateRect(hWnd, &Rect, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		// Drawing Lines
		for each (LINE Line in Triangulation)
		{
			MoveToEx(hdc, Line.From.X, Line.From.Y, NULL);
			LineTo(hdc, Line.To.X, Line.To.Y);
		}

		// Drawing points
		SetDCPenColor(hdc, RGB(0xFF, 0, 0));
		SetDCBrushColor(hdc, RGB(0xFF, 0, 0));
		for each (Point in Points)
		{
			Ellipse(hdc,
				Point.X - PointRadius,
				Point.Y - PointRadius,
				Point.X + PointRadius,
				Point.Y + PointRadius);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}