#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<windows.h>

namespace MEOM
{
class Renderer
{
public:
    Renderer(){}
    ~Renderer(){}
private:
	HBITMAP bufferBitmap;
};
}
#endif // RENDERER_HPP

