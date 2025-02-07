#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Types.h"

enum ObstacleType
{
  ObstacleType_Stool,
  ObstacleType_Box01,
  ObstacleType_Box02,
  ObstacleType_Drum,
  ObstacleType_DrumOld,
  ObstacleType_VBox,
  ObstacleType_Lion,
  ObstacleType_Sofa,
};

class Obstacle : public GameObject
{

  class GridObject* grid{nullptr};
  class Map* map{nullptr};

  ObstacleType type;

  bool bGridLocationChanged{false};
  uint32_t grid_w{0}, grid_h{0};

  bool bDirectionChanged{false};
  Direction dir{kEast};

public:

	Obstacle(class World* world);

  void SetObstacleType(ObstacleType type);
  ObstacleType GetObstacleType();

  void SetDirection(Direction direction);
  Direction GetDirection();

  void SetGridLocation(uint32_t w, uint32_t h);
  std::pair<uint32_t, uint32_t> GetGridLocation();

  std::pair<int, int> GetGridFrontDirection();

  void OnAwake() override;
  void Update(float dt) override;

private:

  // Update logic
  void ApplyChangedGridLocation();
  void ApplyChangedDirection();

};
