#include "DialogUI.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"

DialogUI::DialogUI(class World* world) : UIPanel(world)
{
  _Eliza = CreateUI<UIAnim>(L"Eliza");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Initiative_Gunfire.png", "kProjectDir/,Library\\CSV\\Eliza_Initiative_Gunfire.csv");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Noble.png", "kProjectDir\\Library\\CSV\\Eliza_Noble.csv");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair30.png", "kProjectDir\\Library\\CSV\\Eliza_Resistance_Despair30.csv");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair70.png", "kProjectDir\\Library\\CSV\\Eliza_Resistance_Despair70.csv");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Joy.png", "kProjectDir\\Library\\CSV\\Eliza_Resistance_Joy.csv");
  _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Sad.png", "kProjectDir\\Library\\CSV\\Eliza_Resistance_Sad.csv");

  _Eliza->SetCurrentAnimSprite("Eliza_Noble");
  _Eliza->SetCenterPos({960, 540});


}
