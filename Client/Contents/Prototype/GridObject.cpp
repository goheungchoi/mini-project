#include "GridObject.h"

#include "GameFramework/World/World.h"

void GridObject::CreateGrid(uint32_t width, uint32_t height, float actualCellSize)
{
  this->actualCellSize = actualCellSize;
  this->width = width;
  this->height = height;

  uint32_t len = width * height;
  
	grid.resize(len);
	for (int i = 0; i < len; ++i)
  {
    CellObject* cell = world->CreateGameObject<CellObject>();
		AddChild(cell);

		float pos_x = actualCellSize * (i % width) + actualCellSize / 2;
    float pos_z = actualCellSize * (i / height) + actualCellSize / 2;

		cell->SetScaling(actualCellSize);
		cell->SetTranslation(pos_x, 0, pos_z);

    grid[i] = cell;
	}
}
