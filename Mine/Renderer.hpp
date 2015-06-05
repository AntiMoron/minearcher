#ifndef RENDERER_HPP
#define RENDERER_HPP
#include"Texture.hpp"
namespace MEOM
{
	class Renderer
	{
	private:
		Renderer(){}
	public:
		static Renderer& getInstance()
		{
			static Renderer inst;
			return inst;
		}
		~Renderer()
		{
			ReleaseDC(windowHandle, windowDC);
			DeleteObject(bufferBitmap);
			DeleteDC(bufferDC);
		}
		void initialize(HWND hwnd)
		{
			windowHandle = hwnd;
			windowDC = GetDC(hwnd);
			bufferDC = CreateCompatibleDC(windowDC);
			GetClientRect(hwnd, &rect);
			bufferBitmap = CreateCompatibleBitmap(windowDC, rect.right, rect.bottom);
		}
		void updateRenderSize()
		{
			DeleteObject(bufferBitmap);
			DeleteDC(bufferDC);
			bufferDC = CreateCompatibleDC(windowDC);
			GetClientRect(windowHandle, &rect);
			bufferBitmap = CreateCompatibleBitmap(windowDC, rect.right, rect.bottom);
		}
		void drawCircle(int x, int y, int r)
		{
			SelectObject(bufferDC, bufferBitmap);
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
			SelectObject(bufferDC, pen);
			Ellipse(bufferDC, x - r, y - r, x + r, y + r);
			DeleteObject(pen);
		}
		void drawRectangle()
		{
			SelectObject(bufferDC, bufferBitmap);
		}
		void drawTexture(const Texture& tex,Point leftTop)
		{
			SelectObject(bufferDC, bufferBitmap);
			//Add your render code here.
			tex.renderTextureOnDc(bufferDC, leftTop);
		}
		void drawText(const char* text,int size,Point leftTop,Point rightBottom)
		{
			SelectObject(bufferDC, bufferBitmap);
			SetBkMode(bufferDC, TRANSPARENT);
			SetTextColor(bufferDC, RGB(255, 0, 0));
			RECT r;
			r.left = leftTop.x;
			r.right = rightBottom.x;
			r.top = leftTop.y;
			r.top = rightBottom.y;			
			//Add your render code here.
			DrawTextA(bufferDC, text, -1, &r, DT_CENTER | DT_VCENTER);
		}
		void render() const
		{
			BitBlt(windowDC, 0, 0,
				rect.right - rect.left,
				rect.bottom - rect.top,
				bufferDC, 0, 0,
				SRCCOPY);
		}
	private:
		HWND windowHandle;
		RECT rect;
		HBITMAP bufferBitmap;
		HDC bufferDC;
		HDC windowDC;
	};
}

#define RENDERER (Renderer::getInstance())
#endif // RENDERER_HPP

