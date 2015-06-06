#include<ctime>
#include <windows.h>
#include"TileSet.hpp"
using namespace std;
using namespace MEOM;
RECT g_rect = {0};
HINSTANCE   g_hInst = NULL;
HWND        hwnd 	= NULL;
bool gamePlaying = true;
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
	std::srand((unsigned int)std::time(nullptr));
	RENDERER.initialize(hwnd);
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
			std::string markedBomb = "Bombs marked : " + std::to_string(ts.getMarkedTiles());
			std::string totalBomb = "Bombs in total : " + std::to_string(ts.getTotalBombs());
			RENDERER.drawText("Press \'R\' to restart.", 26, { 0, 0 }, { 500, 26 });
			RENDERER.drawText(markedBomb.c_str(), 26, { 0, 26 }, { 500, 52 });
			RENDERER.drawText(totalBomb.c_str(), 26, { 0, 52 }, { 500, 78 });
			if (gamePlaying == false)
			{
				if (ts.getGameStatus() == GAME_WIN)
				{
					RENDERER.drawText("You win", 26, { 0, 78 }, { 500, 104 });
				}
				else
					RENDERER.drawText("You lose", 26, { 0, 78 }, { 500, 104 });
			}
			else
			{
				gamePlaying = (ts.getGameStatus() == GAME_PLAYING);
			}
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

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		Tile::releaseSource();
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		IOCONTROL.setMousePosition({ HIWORD(lParam), LOWORD(lParam) });
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		if (gamePlaying)
			ts.clickOnTileCoord(HIWORD(lParam), LOWORD(lParam));
		break;
	case WM_RBUTTONUP:
		if (gamePlaying)
			ts.markWithMousePosition(HIWORD(lParam), LOWORD(lParam));
		break;
	case WM_MBUTTONUP:
		if (gamePlaying)
			ts.autoClickWithMousePosition(HIWORD(lParam), LOWORD(lParam));
		break;
	case WM_KEYDOWN:
		if (wParam == 'R')
		{
			ts.setupTiles();
			gamePlaying = true;
		}
		break;
	case WM_SIZE:
		RENDERER.updateRenderSize();
		break;
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
    }
    return 0;
}
