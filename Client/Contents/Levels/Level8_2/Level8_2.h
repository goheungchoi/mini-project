#pragma once
#include "Contents/Levels/GameLevel.h"
#include "GameFramework/World/World.h"

// ��� 2���� ��������� ��
class Level8_2 : public GameLevel
{
public:
  Level8_2(const std::string& name) : GameLevel(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();

  void TriggerAction();
};
