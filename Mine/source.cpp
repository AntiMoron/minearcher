#include <windows.h>
#include"TileSet.hpp"
using namespace std;
using namespace MEOM;
RECT g_rect = {0};
HINSTANCE   g_hInst = NULL;
HWND        hwnd 	= NULL;

TileSet ts(10, 10, 10);

HRESULT InitWindow(HINSTANCE hInstance,int nCmdShow);
LRESULT CALLBACK  WndProc(HWND,UINT,WPARAM,LPARAM);
void Render(HDC buf);

int main()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

    if(FAILED(InitWindow(hInstance, true)))
        return 0;
    MSG msg = {0};
	Renderer::getInstance().initialize(hwnd);
	GetClientRect(hwnd,&g_rect);
	ts.setLeftTop({ 100, 100 });
	ts.setupTiles();
	while (WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ts.Render();
			RENDERER.render();
		}
	}
	return (int)msg.wParam;
}

HRESULT InitWindow(HINSTANCE hInstance,int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, NULL);
    wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "CrystalMiao";
    wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
    if(!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = {0,0,800,600};
    AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW,FALSE);
    hwnd = CreateWindow("CrystalMiao","Crystal Miao",WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,CW_USEDEFAULT,rc.right - rc.left,
                           rc.bottom - rc.top,NULL,NULL,hInstance,NULL);
    if(!hwnd)
        return E_FAIL;

    ShowWindow(hwnd,nCmdShow);
    return S_OK;
}

bool isDown = false;

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			ts.clickOnTileCoord(HIWORD(lParam), LOWORD(lParam));
			break;
		case WM_KEYDOWN:
			break;
		case WM_SIZE:
			Renderer::getInstance().updateRenderSize();
			break;
        default:
            return DefWindowProc(hWnd,message,wParam,lParam);
    }
    return 0;
}

HPEN pen = CreatePen(PS_SOLID,1,RGB(255,255,0));
HBRUSH brush = CreateSolidBrush(RGB(0,255,0));
