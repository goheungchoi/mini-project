#include "GameFramework/UI/UIAnim/UIAnim.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"
#include "Resource2DManager/Resource2DManager.h"
UIAnim::UIAnim(World* world) : UIImage(world) {}

UIAnim::~UIAnim() {}

void UIAnim::Update(float dt)
{
  if (_pCurrAnimInfo == nullptr)
  {
    return;
  }
  _pCurrFrameInfo = _pCurrAnimInfo->Frames[_curFrameIndex];
  const FRAME_INFO& frame = _pCurrFrameInfo;
  _lastFrameIndex = _pCurrAnimInfo->Frames.size() - 1; // ������ Index ��ȣ
  int maxFrameCnt = _lastFrameIndex + 1; // �ִϸ��̼��� frame �� ����

  ///////////////
  // m_FrameTime�� fTimeElapsed�� ��������  Frame.Duration���� ũ��
  // m_FrameIndexCurr�� ������Ű�� m_FrameTime�� 0���� �ʱ�ȭ�Ѵ�.
  if (frame.duration == 0 && _curFrameIndex == _lastFrameIndex)
  {
    return;
  }

  _frameTime += dt;

  if (_frameTime >= frame.duration)
  {
    if (_bLoop)
    {
      _curFrameIndex = (_curFrameIndex + 1) % maxFrameCnt;
    }
    else
    {
      _curFrameIndex = std::min(_curFrameIndex + 1, _lastFrameIndex);
    }
    _frameTime -= frame.duration;
  }

  // �̹��������� ������ ����
  _srcRect = {frame.frameRect.x, frame.frameRect.y,
              frame.frameRect.z, frame.frameRect.w};

}

void UIAnim::Render()
{
  if (_pCurrAnimInfo == nullptr) return;
  if (_status == EStatus_Active)
  {
    _pCurrSprite->Render(_srcRect);
  }
}

void UIAnim::SetCurrentAnimSprite(std::string AnimName)
{
  auto* animsprite = _sprites.find(AnimName)->second;
  _pCurrSprite = animsprite->_pSprite;
  _pCurrAnimInfo = animsprite->_pAnimInfo;
  _curFrameIndex = 0;
  _pCurrFrameInfo = _pCurrAnimInfo->Frames[_curFrameIndex];
  _lastFrameIndex = _pCurrAnimInfo->Frames.size() - 1;

}

void UIAnim::LoadAnimSprite(LPCSTR SpritePath, LPCSTR animCSV)
{
  AnimSprite* newAnimSprite = new AnimSprite();
  newAnimSprite->_pSprite = LoadSprite(SpritePath);
  newAnimSprite->_pAnimInfo = LoadAnim2DAsset(animCSV);
  _sprites.insert({newAnimSprite->_pAnimInfo->name, newAnimSprite});
  
}

Sprite* UIAnim::LoadSprite(LPCSTR animSpritePath)
{
  return Resource2DManager::GetInstance()->GetSprite(animSpritePath).get();
}

ANIMATION_INFO* UIAnim::LoadAnim2DAsset(LPCSTR animCSV)
{
  ANIMATION_INFO* newAnimInfo = new ANIMATION_INFO();
  std::filesystem::path filepath(animCSV);
  filepath.replace_extension("");
  newAnimInfo->name = filepath.filename().string();


  std::ifstream file(animCSV);
  if (!file.is_open())
  {
    std::cout << "animation csv file not opened : " << animCSV << std::endl;
    return nullptr;
  }
  std::string line;
  int framecnt = 0;
  {
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> framecnt;
  }
  for (UINT i = 0; i < framecnt; i++)
  {
    getline(file, line);
    std::stringstream ss(line);
    std::string token;
    {
      FRAME_INFO tempFrame;
      getline(ss, token, ','); // ss�� ������ ,�� �������� ���ڿ��� �и�
      tempFrame.frameRect.x = stof(token);
      getline(ss, token, ',');
      tempFrame.frameRect.y = stof(token);
      getline(ss, token, ',');
      tempFrame.frameRect.z = stof(token);
      getline(ss, token, ',');
      tempFrame.frameRect.w = stof(token);
      getline(ss, token, ',');
      tempFrame.center.x = stof(token);
      getline(ss, token, ',');
      tempFrame.center.y = stof(token);
      newAnimInfo->Frames.push_back(tempFrame);
    }

  }
  return newAnimInfo;
}


void UIAnim::SetAnim2D(std::wstring animName, bool bLoop) {}
