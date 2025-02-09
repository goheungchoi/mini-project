#include "DialogUI.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "GameFramework/UI/UIButton/UIButton.h"
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
    _dialogTextBox->SetScale({1.f,0.8f});
    _dialogTextBox->SetSize(_dialogTextBox->GetTextureSize());
    _dialogTextBox->SetCenterPos({960, 850});
    _dialogTextBox->SetOpacity(.6f);
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

    _dialogBtnImage = CreateUI<UIImage>(L"dialogBtnImage");
    _dialogBtnImage->SetSprite("2D\\UI\\UI_Textbox_Button.png");
    _dialogBtnImage->SetCenterPos({1600, 900});
    _dialogButton = CreateUI<UIButton>(L"dialogButton");
    _dialogButton->SetSize(_dialogBtnImage->GetSize());
    _dialogButton->SetCenterPos({1600, 900});
    _dialogButton->AddOnClickHandler([=]()
        {
      if (!bPlayerSelection)NextStep();
    });
    _dialogButton->AddOnHoveredHandler([=]()
        {
      _dialogBtnImage->SetOpacity(0.7f);
    });
    _dialogButton->AddOnUnHoveredHandler([=]()
        {
      _dialogBtnImage->SetOpacity(1.f);
    });

    _speakerNameText = CreateUI<UIText>(L"speakerName");
    _speakerNameText->SetSize({300, 100});
    _speakerNameText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _speakerNameText->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _speakerNameText->SetColor(Color(1, 1, 1, 1));
    _speakerNameText->SetOpacity(1.f);
    _speakerNameText->SetCenterPos({350, 775});
    _speakerNameText->SetFont(L"PT Noeul");
    _speakerNameText->SetFontSize(47.5f);

    _speakerClassText = CreateUI<UIText>(L"speakerClass");
    _speakerClassText->SetSize({100, 100});
    _speakerClassText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _speakerClassText->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _speakerClassText->SetColor(Color(0.871f, 0.620f, 0.478f, 1));
    _speakerClassText->SetOpacity(1.f);
    _speakerClassText->SetCenterPos({230, 775});
    _speakerClassText->SetFont(L"PT Noeul");
    _speakerClassText->SetFontSize(47.5f);
    _speakerClassText->SetText(L"부관");

    _playerSelectBtnImage1 = CreateUI<UIImage>(L"playerbuttonimage1");
    _playerSelectBtnImage1->SetSprite("2D\\UI\\UI_Selectbox_01.png");
    _playerSelectBtnImage1->SetScale({1.5f, 1.5f});
    _playerSelectBtnImage1->SetCenterPos({1600, 600});
    _playerSelectBtnImage1->SetOpacity(1.f);

    _playerSelectButton1 = CreateUI<UIButton>(L"playerbutton1");
    _playerSelectButton1->SetSize({_playerSelectBtnImage1->GetSize().x * 1.3f,
                                   _playerSelectBtnImage1->GetSize().y});
    _playerSelectButton1->SetCenterPos({1550, 600});
    _playerSelectButton1->AddOnClickHandler([=]() {
      HideUI(L"playerbuttonimage1");
      HideUI(L"playerbutton1");
      HideUI(L"playerbuttontext1");
      HideUI(L"playerbuttonimage2");
      HideUI(L"playerbutton2");
      HideUI(L"playerbuttontext2");
      NextStep();
    });
    _playerSelectButton1->AddOnHoveredHandler([=]() 
        { _playerSelectBtnImage1->SetOpacity(0.5f);
    });
    _playerSelectButton1->AddOnUnHoveredHandler([=]() 
        { _playerSelectBtnImage1->SetOpacity(1.f);
    });

    _playerSelectBtnText1 = CreateUI<UIText>(L"playerbuttontext1");
    _playerSelectBtnText1->SetSize(_playerSelectBtnImage1->GetSize());
    _playerSelectBtnText1->SetCenterPos({1550, 600});
    _playerSelectBtnText1->SetFont(L"PT Noeul");

    _playerSelectBtnText1->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _playerSelectBtnText1->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _playerSelectBtnText1->SetColor(Color(1, 1, 1, 1));
    _playerSelectBtnText1->SetOpacity(1.f);

    _playerSelectBtnImage2 = CreateUI<UIImage>(L"playerbuttonimage2");
    _playerSelectBtnImage2->SetSprite("2D\\UI\\UI_Selectbox_01.png");
    _playerSelectBtnImage2->SetScale({1.5f, 1.5f});
    _playerSelectBtnImage2->SetCenterPos({1600, 750});
    _playerSelectBtnImage2->SetOpacity(1.f);

    _playerSelectButton2 = CreateUI<UIButton>(L"playerbutton2");
    _playerSelectButton2->SetSize({_playerSelectBtnImage2->GetSize().x*1.3f,
                                   _playerSelectBtnImage2->GetSize().y});
    _playerSelectButton2->SetCenterPos({1550, 750});
    _playerSelectButton2->AddOnClickHandler([=]() {
      HideUI(L"playerbuttonimage1");
      HideUI(L"playerbutton1");
      HideUI(L"playerbuttontext1");
      HideUI(L"playerbuttonimage2");
      HideUI(L"playerbutton2");
      HideUI(L"playerbuttontext2");
      NextStep();
    });
    _playerSelectButton2->AddOnHoveredHandler([=]() {
          _playerSelectBtnImage2->SetOpacity(0.5f);
        });
    _playerSelectButton2->AddOnUnHoveredHandler([=]() {
          _playerSelectBtnImage2->SetOpacity(1.f);
        });

    _playerSelectBtnText2 = CreateUI<UIText>(L"playerbuttontext2");
    _playerSelectBtnText2->SetSize(_playerSelectBtnImage1->GetSize());
    _playerSelectBtnText2->SetFont(L"PT Noeul");
    _playerSelectBtnText2->SetCenterPos({1550, 750});
    _playerSelectBtnText2->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _playerSelectBtnText2->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
    _playerSelectBtnText2->SetColor(Color(1, 1, 1, 1));
    _playerSelectBtnText2->SetOpacity(1.f);

    HideUI(L"playerbuttonimage1");
    HideUI(L"playerbutton1");
    HideUI(L"playerbuttontext1");
    HideUI(L"playerbuttonimage2");
    HideUI(L"playerbutton2");
    HideUI(L"playerbuttontext2");

    //test
    SetPrevBattleResult(eBattleResult::PerfectWin);
  }
  {
    _actionList.push_back([=]() { ElizaDialogStep(_prevBattleResult); });
    for (int i = 6; i < _dialogList.size(); i++)
      _actionList.push_back([=]() { ElizaDialogStep(i); });
    _actionList.push_back([=]() {
      bPlayerSelection = true;
      PlayerSelectDialogStep();
    });
    _actionList.push_back([=]() {
      bPlayerSelection = false;
      ElizaDialogStep(5);
    });
  }
}
void DialogUI::Update(float dt)
{
  UIPanel::Update(dt);
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
    _speakerNameText->SetTextAlignment(TextAlignment::CENTERAlIGN);
    _speakerClassText->SetOpacity(1.f);
    _Eliza->SetMasking(Color(1, 1, 1, 1));
    _Eliza->SetOpacity(_dialogList[idx].ElizaAnim, 1.f);
  }
  else
  {
    _speakerNameText->SetTextAlignment(TextAlignment::LEFTAlIGN);
    _speakerClassText->SetOpacity(0.f);
    _Eliza->SetMasking(Color(0.5f, 0.5f, 0.5f, 1));
    _Eliza->SetOpacity(_dialogList[idx].ElizaAnim, 0.8f);
  }
}
void DialogUI::PlayerSelectDialogStep()
{
  int lastIdx = _dialogList.size() - 1;
  _dialogText->SetText(_dialogList[lastIdx].dialogtext.c_str());
  _Eliza->SetCurrentAnimSprite(_dialogList[lastIdx].ElizaAnim);
  _speakerNameText->SetText(_dialogList[lastIdx].IsElizaSpeaking ? _nameEliza
                                                             : _namePlayer);
  _speakerNameText->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _speakerClassText->SetOpacity(0.f);
  _Eliza->SetMasking(Color(0.5f, 0.5f, 0.5f, 1));
  _Eliza->SetOpacity(_dialogList[lastIdx].ElizaAnim, 0.8f);

  if (_dialogList[4].dialogtext.length()>0)
  {
    _playerSelectBtnText1->SetText(_dialogList[3].dialogtext.c_str());
    _playerSelectBtnText2->SetText(_dialogList[4].dialogtext.c_str());
    ShowUI(L"playerbuttonimage1");
    ShowUI(L"playerbutton1");
    ShowUI(L"playerbuttontext1");
    ShowUI(L"playerbuttonimage2");
    ShowUI(L"playerbutton2");
    ShowUI(L"playerbuttontext2");
  }
  else
  {

    _playerSelectBtnText1->SetText(_dialogList[3].dialogtext.c_str());
    ShowUI(L"playerbuttonimage1");
    ShowUI(L"playerbutton1");
    ShowUI(L"playerbuttontext1");
  }


  
}
