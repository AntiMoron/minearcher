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
		static Texture* defaultTexture;
		static Texture* clickTexture;
		static Texture* rollOverTexture;
		static Texture* bombTexture;
		static Texture* emptyTexture;
		static Texture* markedTexture;
	public:
		const static size_t tileLength = 36;
		static void releaseSource()
		{
			delete defaultTexture;
			delete clickTexture;
			delete rollOverTexture;
			delete bombTexture;
			delete emptyTexture;
			delete markedTexture;
		}
		Tile()
		{
			aroundBombCount = 0;
			bIsClick = false;
			bIsBomb = false;
			bIsMark = false;
		}
		void setCovered()
		{
			bIsMark = false;
			bIsClick = false;
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
		void mark()
		{
			if (bIsClick)
				return;
			bIsMark = !bIsMark;
		}
		bool isMarked() const
		{
			return bIsMark;
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
				if (bIsBomb)
				{
					RENDERER.drawTexture(*bombTexture, leftTop);
				}
				else if (aroundBombCount > 0)
				{
					std::string str = std::to_string(aroundBombCount);
					RENDERER.drawText(str.c_str(), Tile::tileLength, leftTop,
					{ leftTop.x + Tile::tileLength,
					leftTop.y + Tile::tileLength });
				}				
			}
			else if (bIsMark == true)
			{
				RENDERER.drawTexture(*markedTexture, leftTop);
			}
			else
			{
				RENDERER.drawTexture(*defaultTexture, leftTop);
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
		friend class TileSet;
		Point leftTop;
		TileStatus tileStatus;
		bool bIsClick;
		bool bIsBomb;
		bool bIsMark;
		int aroundBombCount;
	};
}
#endif // TILE_HPP

