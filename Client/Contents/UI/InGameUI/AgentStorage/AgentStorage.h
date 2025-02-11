#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/GameObjects/Map/Characters/Character.h"

class AgentStorage : public UIPanel
{
public:
  AgentStorage(class World* world);
  virtual ~AgentStorage() = default;

  void Update(float dt) override;
  void BeginLevel() override;
  void ResetAgent();


  class Map* mapptr = nullptr;
  bool IsAgent3 = false;

  class UIImage* BrawlerDefaultImage{nullptr};
  class UIImage* BrawlerHoveredImage{nullptr};
  class UIImage* BrawlerInactiveImage{nullptr};
  class UIButton* BrawlerButton{nullptr};
  bool IsBrawlerUsing = false;
  class UIImage* SlasherDefaultImage{nullptr};
  class UIImage* SlasherHoveredImage{nullptr};
  class UIImage* SlasherInactiveImage{nullptr};
  class UIButton*SlasherButton{nullptr};
  bool IsSlasherUsing = false;

  class UIImage* Gunman1DefaultImage{nullptr};
  class UIImage* Gunman1HoveredImage{nullptr};
  class UIImage* Gunman1InactiveImage{nullptr};
  class UIButton*Gunman1Button{nullptr};
  bool IsGunman1Using = false;

  class UIImage* Gunman2DefaultImage{nullptr};
  class UIImage* Gunman2HoveredImage{nullptr};
  class UIImage* Gunman2InactiveImage{nullptr};
  class UIButton*Gunman2Button{nullptr};
  bool IsGunman2Using = false;


};