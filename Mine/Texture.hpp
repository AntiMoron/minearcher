#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include<windows.h>
#include "Point.hpp"

namespace MEOM
{
	class Texture
	{
	public:
		Texture(const char* fileName)
		{
			bitmap = reinterpret_cast<HBITMAP>(LoadImageA(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			BITMAP bmpHeader;
			GetObject(bitmap, sizeof(BITMAP), reinterpret_cast<LPBYTE>(&bmpHeader));
		}
		~Texture(){}
		void renderTextureOnDc(HWND hwnd,HDC dc,const Point& p)
		{
			HDC tmpDc = CreateCompatibleDC(nullptr);
			SelectObject(tmpDc, bitmap);
			BitBlt(dc, p.x, p.y, 35, 35, tmpDc, 0, 0, SRCCOPY);
			ReleaseDC(hwnd, tmpDc);
		}
	private:
		int width;
		int height;
		HBITMAP bitmap;
	};

	
}
#endif