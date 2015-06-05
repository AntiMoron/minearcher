#ifndef TILE_HPP
#define TILE_HPP
#include<string>
#include"Renderer.hpp"
#include"TileStatus.hpp"
#include"Texture.hpp"
namespace MEOM
{
	class Tile
	{
	public:
		const static size_t tileLength = 36;
		static Texture* defaultTexture;
		static Texture* clickTexture;
		static Texture* rollOverTexture;
		static Texture* bombTexture;
		static Texture* emptyTexture;
		Tile()
		{
			aroundBombCount = 0;
			bIsClick = false;
			bIsBomb = false;
		}
		void setBomb(bool b)
		{
			bIsBomb = b;
		}
		bool isBomb() const
		{
			return bIsBomb;
		}
		void setAroundBombCount(int around)
		{
			aroundBombCount = around;
		}
		int getAroundBombCount()const
		{
			return aroundBombCount;
		}
		void flip()
		{
			bIsClick = true;
		}
		bool isClicked() const
		{
			return bIsClick;
		}
		void Render()
		{
			if (bIsClick == true)
			{
				RENDERER.drawTexture(*emptyTexture, leftTop);
				if (aroundBombCount > 0)
				{
					std::string str = std::to_string(aroundBombCount);
					RENDERER.drawText(str.c_str(), 10, leftTop,
					{ leftTop.x + Tile::tileLength,
					leftTop.y + Tile::tileLength });
				}
			}
			else
			{
				Renderer::getInstance().drawTexture(*defaultTexture, leftTop);
			}
		}
		void setTileStatus(TileStatus val)
		{
			tileStatus = val;
		}
		TileStatus getTileStatus()const
		{
			return tileStatus;
		}
		void setLeftTop(Point pt)
		{
			leftTop = pt;
		}
		const Point& getLeftTop()const
		{
			return leftTop;
		}
	private:
		Point leftTop;
		TileStatus tileStatus;
		bool bIsClick;
		bool bIsBomb;
		int aroundBombCount;
	};
}
#endif // TILE_HPP

