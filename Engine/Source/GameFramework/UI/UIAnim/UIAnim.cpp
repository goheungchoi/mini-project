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
  _bLoop = _pCurrAnimInfo->bIsLoop;
  const FRAME_INFO& frame = _pCurrFrameInfo;
  _lastFrameIndex = _pCurrAnimInfo->Frames.size() - 1; // 마지막 Index 번호
  int maxFrameCnt = _lastFrameIndex + 1; // 애니메이션의 frame 총 갯수

  if (frame.duration == 0 && _curFrameIndex == _lastFrameIndex+1)
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

  // 이미지에서의 프레임 영역
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

void UIAnim::LoadAnimSprite(LPCSTR SpritePath, std::string animCSV)
{
  AnimSprite* newAnimSprite = new AnimSprite();
  newAnimSprite->_pSprite = LoadSprite(SpritePath);
  newAnimSprite->_pAnimInfo = LoadAnim2DAsset(animCSV);
  _sprites.insert({newAnimSprite->_pAnimInfo->name, newAnimSprite});
  
}

Sprite* UIAnim::LoadSprite(LPCSTR animSpritePath)
{
  _spriteList.push_back(Resource2DManager::GetInstance()->GetSprite(animSpritePath).get());
  return Resource2DManager::GetInstance()->GetSprite(animSpritePath).get();
}

ANIMATION_INFO* UIAnim::LoadAnim2DAsset(std::string animCSV)
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
      getline(ss, token, ','); // ss의 내용을 ,를 기준으로 문자열을 분리
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

void UIAnim::SetScale(std::string spritename,
    ::DirectX::SimpleMath::Vector2 scale)
{
  _sprites.find(spritename)->second->_pSprite->SetScale(scale);
}

void UIAnim::SetScale(::DirectX::SimpleMath::Vector2 scale)
{
  for (auto sprite : _spriteList)
  {
    sprite->SetScale(scale);
  }
}

void UIAnim::SetCenterPos(::DirectX::SimpleMath::Vector2 pos)
{
  UIElement::SetCenterPos(pos);
  _position = {pos.x - (_size.x / 2), pos.y - (_size.y / 2)};
  for (auto sprite:_spriteList)
  {
    sprite->SetPos(_position);
  }
}

void UIAnim::SetCenterPos(std::string spritename, Vector2 pos)
{
  _sprites.find(spritename)->second->_pSprite->SetPos(pos);
}
