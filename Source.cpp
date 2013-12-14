#include <windows.h>
#include <string>
#include <vector>
#include <ShlObj.h>

using namespace std;

HWND		ghMainWnd = 0;
HINSTANCE	ghAppInst = 0;

struct textObj
{
	string s;
	POINT  p;

};

vector<textObj> gTextObjs;

struct line
{
	POINT p0;
	POINT p1;
};

vector<line> gLineObjs;
line gLineObj;

bool gMouseDown = false;

LRESULT	CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND DesktopHandle = FindWindow("ProgMan", 0);
	DesktopHandle = GetWindow(DesktopHandle, GW_CHILD);
	DesktopHandle = GetWindow(DesktopHandle, GW_CHILD);
	HDC hDC_Desktop = GetWindowDC(DesktopHandle);

	// objects for painting
	HDC hdc = 0;
	PAINTSTRUCT ps;

	textObj to;
	int x = 0;
	int y = 0;
	

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		
		
		// string
		to.s = "Hello, World.";
		
		// point x,y position
		to.p.x = LOWORD(lParam);
		to.p.y = HIWORD(lParam);
		
		// add to global list of text objects
		gTextObjs.push_back(to);

		InvalidateRect(hWnd, 0, false);
		

		SetCapture(hWnd);
		gMouseDown = true;

		// point we  clicked
		gLineObj.p0.x = LOWORD(lParam);
		gLineObj.p0.y = HIWORD(lParam);

		return 0;

	case WM_MOUSEMOVE:
		
		if(gMouseDown)
		{
			// current mouse position
			gLineObj.p1.x = LOWORD(lParam);
			gLineObj.p1.y = HIWORD(lParam);

			InvalidateRect(hWnd, 0, true);

		}

		return 0;

	case WM_LBUTTONUP:

		// release the captured mouse when you release mouse button
		ReleaseCapture();
		gMouseDown = false;

		// current mouse position is stored in lParam
		gLineObj.p1.x = LOWORD(lParam);
		gLineObj.p1.y = HIWORD(lParam);

		gLineObjs.push_back(gLineObj);

		
		InvalidateRect(hWnd, 0, true);

		return 0;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(DesktopHandle, &ps);

		
		for(int i = 0; i < gTextObjs.size();++i)
			TextOut(hDC_Desktop, gTextObjs[i].p.x, gTextObjs[i].p.y, gTextObjs[i].s.c_str(), gTextObjs[i].s.size());
		

		if(gMouseDown)
		{
			MoveToEx(hdc, gLineObj.p0.x, gLineObj.p0.y, 0);
			LineTo(hdc, gLineObj.p1.x, gLineObj.p1.y);
		}

		for(int i = 0; i < gLineObjs.size(); ++i)
		{
			MoveToEx(hdc, gLineObjs[i].p0.x, gLineObjs[i].p0.y, 0);
			LineTo(hdc, gLineObjs[i].p1.x, gLineObjs[i].p1.y);

		}


		
	
		
		
		for(int i = 0; i < 3; ++i)
		{
		x = rand() % 1023;
		y = rand() % 767;
		SendMessage(DesktopHandle, LVM_SETITEMPOSITION, i, MAKELPARAM(x, y));
		SendMessage(DesktopHandle, LVM_UPDATE, 0, 0);
		Sleep(50);
		}
		
		

		EndPaint(DesktopHandle, &ps);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
			
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ghAppInst;
	wc.hIcon			= ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground		= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName		= "My Window";

	RegisterClass(&wc);

	ghMainWnd = ::CreateWindow("My Window", "MyWindow", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, 0, 0, ghAppInst, 0);

	if(ghMainWnd == 0)
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;



}

/*

// Lesson 6 load bitmap
HWND		ghMainWnd = 0;
HINSTANCE	ghAppInst = 0;
HBITMAP ghBitmap = 0;
BackBuffer *gBackBuffer = 0;

bool gMouseDown = false;
int  Run();

LRESULT	CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	// objects for painting
	HDC hdc = 0;
	HDC bmHDC = 0;
	PAINTSTRUCT ps;

	BITMAP bitmap = {0};

	static int bmWidth = 0;
	static int bmHeight = 0;

	HBITMAP oldBM = 0;

	BackBuffer *gBackBuffer = 0;

	const int gClientWidth	= 640;
	const int gClientHeight = 480;

	switch( msg )
	{
	case WM_CREATE:
	
		gBackBuffer = new BackBuffer(hWnd, gClientWidth, gClientHeight);

		ghBitmap = LoadBitmap(ghAppInst, MAKEINTRESOURCE(IDB_BITMAP1));

		GetObject(ghBitmap, sizeof(BITMAP), &bitmap);

		bmWidth  = bitmap.bmWidth;
		bmHeight = bitmap.bmHeight;

		return 0;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// create a system memory device context.
		bmHDC = CreateCompatibleDC(hdc);

		// hook up the bitmap to the bmHDC
		oldBM = (HBITMAP)SelectObject(bmHDC, ghBitmap);
		BitBlt(hdc, 0, 0, bmWidth, bmHeight, bmHDC, 0, 0,  SRCCOPY);

		// Select the orginal loaded bitmap
		SelectObject(bmHDC,  oldBM);

		// delete the system memory device context
		DeleteDC(bmHDC);
		

		EndPaint(hWnd, &ps);

		return 0;

	case WM_DESTROY:
		DeleteObject(ghBitmap);
		delete gBackBuffer;
		PostQuitMessage(0);
		return 0;
			
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ghAppInst;
	wc.hIcon			= ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground		= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName		= "My Window";

	RegisterClass(&wc);

	ghMainWnd = ::CreateWindow("My Window", "MyWindow", WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, 0, ghAppInst, 0);

	if(ghMainWnd == 0)
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	return true;
}

int Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT )
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			HDC	bbDC = gBackBuffer->getDC();



		}
	}
	

	return (int)msg.wParam;



}

*/