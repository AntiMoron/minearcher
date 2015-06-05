#ifndef TILESET_HPP
#define TILESET_HPP
#include<queue>
#include<cstdlib>
#include"Tile.hpp"
#include"Point.hpp"
namespace MEOM
{
	class TileSet
	{
	private:
		int totalBombs;
		int columnCount;
		int rowCount;
		Point leftTop;
		Tile* tileEntities;
	public:
		TileSet(int c, int r, int b) : 
			columnCount(c), rowCount(r), totalBombs(b)
		{
			tileEntities = new Tile[c * r];
		}
		~TileSet()
		{
			if (tileEntities != nullptr)
			{
				delete [] tileEntities;
				tileEntities = nullptr;
			}
		}
		Tile& getTile(int x,int y)
		{
			return tileEntities[x + (y * columnCount)];
		}
		void setupTiles()
		{
			//init all tiles
			for (int i = 0; i < columnCount; i++)
			{
				for (int j = 0; j < rowCount; j++)
				{
					Tile& currentTile = getTile(i, j);
					currentTile.setLeftTop(
						{ leftTop.x + i * Tile::tileLength,
						leftTop.y + j * Tile::tileLength });
					currentTile.setBomb(false);
				}
			}
			//set up bombs
			int cnt = totalBombs;
			int count = columnCount * rowCount;
			while (cnt--)
			{
				int i = rand() % count;
				if (tileEntities[i].isBomb() == true)
				{
					cnt++;
				}
				else
				{
					tileEntities[i].setBomb(true);
				}
			}
			//set up numbers
			for (int i = 0; i < columnCount; i++)
			{
				for (int j = 0; j < rowCount; j++)
				{
					//检查周围8个格子有几个雷
					int bnum = 0;
					Tile& currentTile = getTile(i, j);
					for (int dx = -1; dx < 2; dx++)
					{
						for (int dy = -1; dy < 2; dy++)
						{
							int newX = i + dx;
							int newY = j + dy;
							if (newX >= 0 && newX < columnCount &&
								newY >= 0 && newY < rowCount)
							{
								getTile(newX, newY).isBomb() ? bnum++ : bnum += 0;
							}
						}
					}
					//设定雷数
					currentTile.setAroundBombCount(bnum);
				}
			}
		}
		void setLeftTop(Point pt)
		{
			leftTop = pt;
			//update all the tiles.
			for (int i = 0; i < columnCount; i++)
			{
				for (int j = 0; j < rowCount; j++)
					tileEntities[i + (j * columnCount)].setLeftTop(
				{leftTop.x + i * Tile::tileLength,
				leftTop.y + j * Tile::tileLength});
			}
		}
		const Point& getLeftTop()const
		{
			return leftTop;
		}
		void clickOnTileCoord(int mousex, int mousey)
		{
			std::pair<int, int> coord = getCoordWithMousePosition(mousex, mousey);
			bfsHelperFlipTiles(coord.first, coord.second);
		}
		void Render()
		{
			int count = columnCount * rowCount;
			for (int i = 0; i < count; i++)
			{
				tileEntities[i].Render();
			}
		}
	private:
		std::pair<int, int> getCoordWithMousePosition(int x, int y)
		{
			x -= leftTop.x;
			y -= leftTop.y;
			return std::pair<int, int>(y / Tile::tileLength, 
				x / Tile::tileLength);
		}
		void bfsHelperFlipTiles(int x, int y)
		{
			if (x < 0 || y < 0 ||
				x >= columnCount || y >= rowCount)
			{
				return ;
			}
			tileEntities[y * columnCount + x].flip();
			std::queue<Point> que;
			que.push({ x, y });
			const static Point moveDir[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
			while (!que.empty())
			{
				Point pt = que.front();
				que.pop();
				for (const Point& dir : moveDir)
				{
					Point next = { pt.x + dir.x,
						pt.y + dir.y };
					if (next.x >= 0 && next.y >= 0
						&& next.x < columnCount && next.y < rowCount)
					{
						Tile& currentTile = tileEntities[next.y * columnCount + next.x];
						if (currentTile.isBomb())
						{
							continue;
						}
						if (currentTile.getAroundBombCount() > 0)
						{
							currentTile.flip();
							continue;
						}
						if (!currentTile.isClicked())
						{
							currentTile.flip();
							que.push(next);
						}
					}
				}
			}
		}
		//
	}; 
}
#endif //TILESET_HPP
