#pragma once
#include "../../nclgl/Vector2.h"
enum TileType {
	FLOOR,
	WALL,
	RUBBLE,
	BOULDER,
	HOARD,
	POOL,
	INVALID_TILE,

};

class Map{
	public:
		static const int GRID_SIZE;
		static const int TILES_X = 59;
		static const int TILES_Y = 38;
		static TileType tileMap[Map::TILES_X *  Map::TILES_Y];
		static const Vector2 MAP_IMAGE_BORDER_SIZE;
		static const float MAP_IMAGE_HALF_WIDTH;
		static const float MAP_IMAGE_HALF_HEIGHT;
		static const float MAP_WIDTH;
		static const float MAP_HEIGHT;
		static const Vector2 MAP_BOTTOM_LEFT;
		
		static int worldToGridPositionX(float positionX) {
			return (positionX - MAP_BOTTOM_LEFT.x - 1) / GRID_SIZE;
		}
		static int worldToGridPositionY(float positionY){
			return  (positionY - MAP_BOTTOM_LEFT.y + 1) / GRID_SIZE;
		}

		static Vector2 gridToWorldPosition(const int gridX, const int gridY){
			//just trust me on this
			return Vector2(((gridX + 1) * Map::GRID_SIZE) + MAP_BOTTOM_LEFT.x, ((gridY - 1) * Map::GRID_SIZE) + MAP_BOTTOM_LEFT.y);
		}

		static int getIndex(int x, int y) {
			return x + y * Map::TILES_X;
		}

		//static TileType getTileAt(int x, int y) {
		//	if (x >= 0 && y >= 0 && x < Map::TILES_X && y < Map::TILES_Y) {
		//		int flippedY = Map::TILES_Y - y;
		//		return tileMap[flippedY * Map::TILES_X + x];
		//	}
		//	return INVALID_TILE;


		//}
		

		static TileType charToTile(char charTile);

	private:
		
};

