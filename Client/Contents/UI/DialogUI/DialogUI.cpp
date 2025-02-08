#include "DialogUI.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "Shared/Config/Config.h"

DialogUI::DialogUI(class World* world) : UIPanel(world)
{
  {
    _PhotoPanel = CreateUI<UIImage>(L"PhotoPanel");
    _PhotoPanel->SetSprite("Textures\\Picture\\PhotoPanel.png");
    _PhotoPanel->SetCenterPos({960, 540});
  }
  {
    fs::path p = ns::kDataDir;
    std::string rootpath = p.string();
    _Eliza = CreateUI<UIAnim>(L"Eliza");
    std::string gunfire = "\\Eliza_Animation\\Eliza_Initiative_Gunfire.csv";
    std::string noble = "\\Eliza_Animation\\Eliza_Noble.csv";
    std::string d30 = "\\Eliza_Animation\\Eliza_Resistance_Despair30.csv";
    std::string d70 = "\\Eliza_Animation\\Eliza_Resistance_Despair70.csv";
    std::string joy = "\\Eliza_Animation\\Eliza_Resistance_Joy.csv";
    std::string sad = "\\Eliza_Animation\\Eliza_Resistance_Sad.csv";
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Noble.png", rootpath + noble);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Initiative_Gunfire.png",
                           rootpath + gunfire);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair30.png",
                           rootpath + d30);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair70.png",
                           rootpath + d70);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Joy.png",
                           rootpath + joy);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Sad.png",
                           rootpath + sad);
    _Eliza->SetCenterPos("Eliza_Noble", {100, 70});
    _Eliza->SetScale("Eliza_Noble", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Despair30", {35, 155});
    _Eliza->SetScale("Eliza_Resistance_Despair30", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Despair70", {28, 161});
    _Eliza->SetScale("Eliza_Resistance_Despair70", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Joy", {35, 152.5f});
    _Eliza->SetScale("Eliza_Resistance_Joy", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Sad", {35, 156});
    _Eliza->SetScale("Eliza_Resistance_Sad", {.88f, .88f});
    _Eliza->SetCurrentAnimSprite("Eliza_Noble");
  }

  {
    _dialogTextBox = CreateUI<UIImage>(L"DialogTextBox");
    _dialogTextBox->SetSprite("2D\\UI\\UI_Textbox_01.png");
    _dialogTextBox->SetCenterPos({960, 900});
    _dialogTextBox->SetOpacity(1.f);
    _dialogText = CreateUI<UIText>(L"DialogText");
    //_dialogText->SetDebugDraw(true);
    _dialogText->SetSize(
        {_dialogTextBox->GetSize().x, _dialogTextBox->GetSize().y});
    _dialogText->SetCenterPos({960, 900});
    _dialogText->SetFont(L"PT Noeul");
    _dialogText->SetOpacity(1.f);
    _dialogText->SetColor(Color(1, 1, 1, 1));
    _dialogText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _dialogText->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    ParseDialogScript();
    SetStageDialogIndex(4);

    _speakerNameText = CreateUI<UIText>(L"speakerName");
    // _speakerNameText->SetDebugDraw(true);
    _speakerNameText->SetSize({300, 100});
    _speakerNameText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _speakerNameText->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _speakerNameText->SetColor(Color(1, 1, 1, 1));
    _speakerNameText->SetOpacity(1.f);
    _speakerNameText->SetCenterPos({400, 800});
    _speakerNameText->SetFont(L"PT Noeul");

    _speakerClassText = CreateUI<UIText>(L"speakerClass");
    _speakerClassText->SetSize({100, 100});
    _speakerClassText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _speakerClassText->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _speakerClassText->SetColor(Color(0.871f, 0.620f, 0.478f, 1));
    _speakerClassText->SetOpacity(1.f);
    _speakerClassText->SetCenterPos({325, 800});
    _speakerClassText->SetFont(L"PT Noeul");
    _speakerClassText->SetText(L"부관");
    SetPrevBattleResult(eBattleResult::PerfectWin);
  }

  {

    _actionList.push_back([=]() { ElizaDialogStep(_prevBattleResult); });
    for (int i = 6; i < _dialogList.size(); i++)
      _actionList.push_back([=]() { ElizaDialogStep(i); });
    _actionList.push_back([=]() { PlayerSelectDialogStep(); });
    _actionList.push_back([=]() { ElizaDialogStep(5); });
  }
}
void DialogUI::Update(float dt)
{
  UIPanel::Update(dt);

  // if (isLClicked)
  //   _clickTimer -= dt;
  // if (INPUT.IsKeyPress(MouseState::LB))
  //{
  //   isLClicked = true;
  //   if (!isClickActioned)
  //   {
  //     NextStep();
  //     isClickActioned = true;
  //   }
  // }
  // if (_clickTimer <= 0)
  //{
  //   _clickTimer = _clickThreshold;
  //   isLClicked = false;
  //   isClickActioned = false;
  // }

  if (INPUT.IsKeyPress(MouseState::LB))
    NextStep();

  if (INPUT.IsKeyPress(Key::D1))
    _dialogTextBox->FadeIn(1.0f);
  if (INPUT.IsKeyPress(Key::D2))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Despair30");
  if (INPUT.IsKeyPress(Key::D3))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Despair70");
  if (INPUT.IsKeyPress(Key::D4))
    _Eliza->SetCurrentAnimSprite("Eliza_Noble");
  if (INPUT.IsKeyPress(Key::D5))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Sad");
  if (INPUT.IsKeyPress(Key::D6))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Joy");

  if (isCurrentActionFinished && _currentActionIndex <= _actionList.size() - 1)
  {
    isCurrentActionFinished = false;
    _actionList[_currentActionIndex]();
  }
}

void DialogUI::NextStep()
{
  _currentActionIndex++;
  if (_currentActionIndex >= _actionList.size())
    _currentActionIndex = _actionList.size() - 1;
  isCurrentActionFinished = true;
}

void DialogUI::PrevStep()
{
  _currentActionIndex--;
  if (_currentActionIndex < 0)
    _currentActionIndex = 0;
}

void DialogUI::ParseDialogScript()
{
  fs::path p = ns::kDataDir;
  std::string rootpath = p.string();
  std::string csvpath = "\\Dialog_Text\\Dialog_csv.csv";
  std::string dialogscriptpath = rootpath + csvpath;

  _csvlist.resize(9);
  std::ifstream csvlistfile(dialogscriptpath);
  if (!csvlistfile.is_open())
  {
    std::cout << "script csv file not opened : " << dialogscriptpath
              << std::endl;
    return;
  }
  std::string line;
  {
    for (int i = 0; i < 9; i++)
    {
      std::getline(csvlistfile, line);
      std::stringstream ss(line);
      std::string token;

      getline(ss, token, ',');
      int idx = stoi(token) - 1;
      getline(ss, token, ',');
      _csvlist[idx] = token;
    }
  }
}

void DialogUI::SetStageDialogIndex(int StageIdx)
{
  fs::path p = ns::kDataDir;
  std::string rootpath = p.string();
  _dialogList.clear();

  // std::locale::global(std::locale("ko_KR.UTF-8"));
  std::wifstream scriptfile(rootpath + _csvlist[StageIdx - 1]);
  scriptfile.imbue(std::locale("ko_KR")); // UTF-8 강제 적용[3][19]

  if (!scriptfile.is_open())
    return;

  wstring line, token;
  getline(scriptfile, line);

  {
    wstringstream ss(line);
    getline(ss, token, L',');
    int stage = _wtoi(token.c_str());
    if (StageIdx != stage)
      return;

    getline(ss, token, L',');
  }
  int lineCnt = stoi(token.c_str());
  _dialogList.resize(lineCnt);
  for (int i = 0; i < lineCnt; i++)
  {
    getline(scriptfile, line);
    wstringstream ss(line);

    getline(ss, token, L',');
    _dialogList[i].idx = _wtoi(token.c_str());

    getline(ss, token, L',');
    _dialogList[i].IsElizaSpeaking = stoi(token);

    getline(ss, token, L',');
    _dialogList[i].ElizaAnim = std::string().assign(token.begin(), token.end());

    getline(ss, token);
    size_t pos = 0;
    while ((pos = token.find(L"\\n", pos)) != std::wstring::npos)
    {
      token.replace(pos, 2, L"\n");
      pos += 1;
    }
    _dialogList[i].dialogtext = token;
  }
}

void DialogUI::ElizaDialogStep(int idx)
{
  _dialogText->SetText(_dialogList[idx].dialogtext.c_str());
  _Eliza->SetCurrentAnimSprite(_dialogList[idx].ElizaAnim);
  _speakerNameText->SetText(_dialogList[idx].IsElizaSpeaking
                                ? _nameEliza
                                : _namePlayer);
  if (_dialogList[idx].IsElizaSpeaking)
  {
    _speakerClassText->SetOpacity(1.f);
    _Eliza->SetMasking(Color(1, 1, 1, 1));
    _Eliza->SetOpacity(_dialogList[idx].ElizaAnim, 1.f);
  }
  else
  {
    _speakerClassText->SetOpacity(0.f);
    _Eliza->SetMasking(Color(0.5f, 0.5f, 0.5f, 1));
    _Eliza->SetOpacity(_dialogList[idx].ElizaAnim, 0.8f);
  }
}

void DialogUI::PlayerSelectDialogStep()
{
  
}
