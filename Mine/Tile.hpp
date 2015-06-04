#ifndef TILE_HPP
#define TILE_HPP

namespace MEOM{

class Tile
{
public:
    Tile()
    {
        isClick = false;
    }
    ~Tile(){}

private:
    bool isClick,isBomb;
    int aroundBombCount;
};
}
#endif // TILE_HPP

