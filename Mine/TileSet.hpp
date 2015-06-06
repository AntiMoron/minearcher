#ifndef TILESET_HPP
#define TILESET_HPP
#include<queue>
#include<cstdlib>
#include"Tile.hpp"
#include"IOControl.hpp"
#include"GameStatus.hpp"
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
		bool bIsGameOver;
	public:
		TileSet(int c, int r, int b) :
			columnCount(c), rowCount(r), totalBombs(b)
		{
			tileEntities = new Tile[c * r];
			bIsGameOver = false;
		}
		~TileSet()
		{
			if (tileEntities != nullptr)
			{
				delete[] tileEntities;
				tileEntities = nullptr;
			}
		}
		Tile& getTile(int x, int y)
		{
			return tileEntities[x + (y * columnCount)];
		}
		void setupTiles()
		{
			bIsGameOver = false;
			//init all tiles
			for (int i = 0; i < columnCount; i++)
			{
				for (int j = 0; j < rowCount; j++)
				{
					Tile& currentTile = getTile(i, j);
					currentTile.setCovered();
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
				{ leftTop.x + i * Tile::tileLength,
				leftTop.y + j * Tile::tileLength });
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
		void markWithMousePosition(int mousex, int mousey)
		{
			std::pair<int, int> coord = getCoordWithMousePosition(mousex, mousey);
			getTile(coord.first, coord.second).mark();
		}
		void autoClickWithMousePosition(int mousex, int mousey)
		{
			std::pair<int, int> coord = getCoordWithMousePosition(mousex, mousey);
			if (getTile(coord.first, coord.second).getAroundBombCount() <= 0)
			{
				return;
			}
			//雷数正好就点击开所有的未点击格子
			int cnt = getTile(coord.first, coord.second).getAroundBombCount();
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (isWithinTiles(coord.first + i, coord.second + j) &&
						getTile(coord.first + i, coord.second + j).isMarked())
					{
						cnt--;
					}
				}
			}
			if (cnt == 0)
			{
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (isWithinTiles(coord.first + i, coord.second + j) &&
							!getTile(coord.first + i, coord.second + j).isMarked())
						{
							bfsHelperFlipTiles(coord.first + i, coord.second + j);
						}
					}
				}
			}
			//雷数不正好但是所有未点击格子数正好
			else
			{
				int cnt2 = 0;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (isWithinTiles(coord.first + i, coord.second + j) &&
							!getTile(coord.first + i, coord.second + j).isClicked() &&
							!getTile(coord.first + i, coord.second + j).isMarked())
						{
							cnt2++;
						}
					}
				}
				if (cnt == cnt2)
				{
					for (int i = -1; i < 2; i++)
					{
						for (int j = -1; j < 2; j++)
						{
							if (isWithinTiles(coord.first + i, coord.second + j) &&
								!getTile(coord.first + i, coord.second + j).isClicked() &&
								!getTile(coord.first + i, coord.second + j).isMarked())
							{
								getTile(coord.first + i, coord.second + j).mark();
							}
						}
					}
				}
			}
		}
		void Render()
		{
			Point mousePosition = IOCONTROL.getMousePosition();
			std::pair<int, int> pos = getCoordWithMousePosition(mousePosition.x, mousePosition.y);
			int count = columnCount * rowCount;
			for (int i = 0; i < count; i++)
			{
				if ((i % columnCount) == pos.first &&
					(i / columnCount) == pos.second &&
					tileEntities[i].isClicked() == false &&
					tileEntities[i].isMarked() == false)
				{
					RENDERER.drawTexture(*Tile::rollOverTexture,
						tileEntities[i].getLeftTop());
				}
				else
					tileEntities[i].Render();
			}
		}
		GAME_STATUS getGameStatus() const
		{
			//正确的雷数
			int cnt = 0;
			int count = columnCount * rowCount;
			for (int i = 0; i < count; i++)
			{
				if (tileEntities[i].isBomb() && tileEntities[i].isClicked())
					return GAME_LOSE;
				if (tileEntities[i].isMarked() &&
					tileEntities[i].isBomb())
				{
					cnt++;
				}
			}
			if (cnt == totalBombs)
			{
				return GAME_WIN;
			}
			return GAME_PLAYING;
		}
		int getTotalBombs()const
		{
			return totalBombs;
		}
		int getMarkedTiles()const
		{
			int ret = 0;
			int count = columnCount * rowCount;
			for (int i = 0; i < count; i++)
			{
				if (tileEntities[i].isMarked())
				{
					ret++;
				}
			}
			return ret;
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
			Tile& startTile = tileEntities[y * columnCount + x];
			if (startTile.isClicked() || 
				startTile.isMarked())
				return;
			startTile.flip();
			if (startTile.getAroundBombCount() > 0)
			{
				return ;
			}
			std::queue<Point> que;
			que.push({ x, y });
			const static Point moveDir[] = { { 1, 0 }, { -1, 0 },
			{ 0, 1 }, { 0, -1 },
			{ 1, 1 }, { -1, -1 },
			{ 1, -1 }, { -1, 1 } };
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
		bool isWithinTiles(int x,int y)
		{
			if (x < 0) return false;
			if (y < 0) return false;
			if (x >= columnCount) return false;
			if (y >= rowCount) return false;
			return true;
		}
	}; 
}
#endif //TILESET_HPP
