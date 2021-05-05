#include <vector>
#include <string>
#include <cmath>
#define utils_print
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "../kFramework/kRenderer.h"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

namespace kNgine
{
  Camera::Camera() : Camera(1, 1920, 1080)
  {
  }
  Camera::Camera(f32 fov, u32 windowWidth, u32 windowHeight)
  {
    this->position = v3(0, 0, 1);
    this->posMapper = mapper(v3(0,0,0),v3(0,0,1),v3(-1,-1,-1),v3(1,1,1));
    if (windowHeight < windowWidth)
    {
      posMapper.min.x = -fov / 2;
      posMapper.max.x = fov / 2;
      posMapper.min.y = -((f32)windowHeight) / windowWidth * fov / 2;
      posMapper.max.y = ((f32)windowHeight) / windowWidth * fov / 2;
    }
    else
    {
      posMapper.min.y = -fov / 2;
      posMapper.max.y = fov / 2;
      posMapper.min.x = -((f32)windowWidth) / windowHeight * fov / 2;
      posMapper.max.x = ((f32)windowWidth) / windowHeight * fov / 2;
    }
    this->fov = fov;
    this->flags|=ObjectFlags::RENDERER_LAYER;
  }
  Camera::~Camera() {}

  void Camera::init(std::vector<EngineObject *> objects){
    this->engineInfo = {(u64 *)callEvent("getEngineObjectsSize"), (EngineObject **)callEvent("getEngineObjects")};
    this->layer = layerO(*(LayerOrder *)callEvent("getEngineRendererLayers"), DEFAULT_LAYER);
  }

  void Camera::updateWindowSize(u32 windowWidth, u32 windowHeight)
  {
    if (fovType == MAX_WH)
    {
      if (windowHeight < windowWidth)
      {
        posMapper.min.x = -fov / 2;
        posMapper.max.x =  fov / 2;
        posMapper.min.y = -((f32)windowHeight) / windowWidth * fov / 2;
        posMapper.max.y =  ((f32)windowHeight) / windowWidth * fov / 2;
      }
      else
      {
        posMapper.min.y = -fov / 2;
        posMapper.max.y = fov / 2;
        posMapper.min.x = -((f32)windowWidth) / windowHeight * fov / 2;
        posMapper.max.x =  ((f32)windowWidth) / windowHeight * fov / 2;
      }
    }
    else if (fovType == MIN_WH)
    {
      if (windowWidth < windowHeight)
      {
        posMapper.min.x = -fov / 2;
        posMapper.max.x =  fov / 2;
        posMapper.min.y = -((f32)windowHeight) / windowWidth * fov / 2;
        posMapper.max.y =  ((f32)windowHeight) / windowWidth * fov / 2;
      }
      else
      {
        posMapper.min.y = -fov / 2;
        posMapper.max.y =  fov / 2;
        posMapper.min.x = -((f32)windowWidth) / windowHeight * fov / 2;
        posMapper.max.x =  ((f32)windowWidth) / windowHeight * fov / 2;
      }
    }
    else if (fovType == WIDTH)
    {
      posMapper.min.x = -fov / 2;
      posMapper.max.x = fov / 2;
      posMapper.min.y = -((f32)windowHeight) / windowWidth * fov / 2;
      posMapper.max.y = ((f32)windowHeight) / windowWidth * fov / 2;
    }
    else if (fovType == HEIGHT)
    {
      posMapper.min.y = -fov / 2;
      posMapper.max.y = fov / 2;
      posMapper.min.x = -((f32)windowWidth) / windowHeight * fov / 2;
      posMapper.max.x = ((f32)windowWidth) / windowHeight * fov / 2;
    }
    else if (fovType == DIAGONAL)
    {
      f32 diagonal = sqrtf(windowWidth*windowWidth+windowHeight*windowHeight);
      posMapper.min.x = -windowWidth / diagonal * fov;
      posMapper.max.x =  windowWidth / diagonal * fov;
      posMapper.min.y = -windowHeight / diagonal * fov;
      posMapper.max.y =  windowHeight / diagonal * fov;
    }
    else
    {
      if (windowWidth > windowHeight)
      {
        posMapper.min.x = -fov / 2;
        posMapper.max.x = fov / 2;
        posMapper.min.y = -((f32)windowWidth) / windowHeight * fov / 4;
        posMapper.max.y = ((f32)windowWidth) / windowHeight * fov / 4;
      }
      else
      {
        posMapper.min.y = -fov / 2;
        posMapper.max.y = fov / 2;
        posMapper.min.x = -((f32)windowHeight) / windowWidth * fov / 4;
        posMapper.max.x = ((f32)windowHeight) / windowWidth * fov / 4;
      }
    }
  }

  void Camera::update(std::vector<msg>msgs){
    iv2 windowsize=iv2(1920,1080);
    for(msg m:msgs){
      if(m.msgType==msg::WINDOW_SIZE){
        windowsize=m.window_size;
        break;
      }
    }
    updateWindowSize(windowsize.x,windowsize.y);
  }

  void Camera::render()
  {
    std::vector<ComponentGameObject *> objects = findObject<ComponentGameObject>(this->engineInfo.engineObjects, *this->engineInfo.engineObjectLength, ObjectFlags::RENDERABLE);
    objects = findObjectBlacklist<ComponentGameObject>(orderObjectsByZ<ComponentGameObject>(objects),"");
    for(u32 i=0;i<objects.size();i++){
      render(objects[i]);
    }
  }

  void Camera::render(ComponentGameObject *object)
  {
    Renderable*compn=object->findComponent<Renderable>(ObjectFlags::RENDERABLE);
    if(compn->label=="[Rend_SYS]"){
      compn=((RenderableSystem*)compn)->active;
    }
    u32 numSprite = 1;
    bool isSpriteList = false;
    if(compn->specialAccessor)
    {
      if (compn->label == "[RendererObject]")
      {
        RendererObject_base *rendObject = (RendererObject_base *)compn;
        m4 matrix = M4InitDiagonal(1);
        matrix = M4MultiplyM4(matrix, posMapper.toM4());
        matrix = M4MultiplyM4(matrix, M4TranslateV3(v3(this->position.x, this->position.y,0)*-1));

        
        v4 pos1=V4MultiplyM4(v4xyz(rendObject->object->position,1),matrix);
        v4 pos3=V4MultiplyM4(v4(10,5,1,1),matrix);
        v3 pos4 = posMapper.map(v3(10,5,1));
        v3 pos2=posMapper.map(rendObject->object->position);

        rendObject->updatePoints(matrix);
        std::vector<std::vector<u32>>objSpritesIndexes = rendObject->getSpriteIndexes();
        if(objSpritesIndexes.size()){
          u32 **spriteIndexes = new u32 *[objSpritesIndexes.size()];
          for (u32 i = 0; i < objSpritesIndexes.size(); i++)
          {
            spriteIndexes[i] = new u32[objSpritesIndexes[i].size()];
            for(u32 j=0;j<objSpritesIndexes[i].size();j++){
              spriteIndexes[i][j] = objSpritesIndexes[i][j];
            }
          }
          kRenderer_drawObjectWithTexture(rendObject->rendererObjectIndex, spriteIndexes);
          for (u32 i = 0; i < objSpritesIndexes.size(); i++){
            delete[] spriteIndexes[i];
          }
          delete[] spriteIndexes;
        }else{
          kRenderer_drawObject(rendObject->rendererObjectIndex);
        }
        return;
      }
      else if(compn->label=="[sprite_list]")
      {
        numSprite = ((SpriteList *)compn)->getSpriteListLength();
        isSpriteList = true;
      }
    }
    for (u32 i = 0; i < numSprite; i++)
    {
      SpriteAccessor*accessor=(SpriteAccessor*)compn;
      if (isSpriteList)
      {
        accessor = ((SpriteList*)compn)->getSpriteList()[i];
      }
      v2 spriteDimensions = posMapper.map(toV2(object->position)+
                                                            accessor->getSpriteDimensions())-
                                      posMapper.map(toV2(object->position));
      u8 *buffer = accessor->getSprite()->buffer;
      v2 spriteOffset = accessor->getSpriteLocation();
      spriteOffset.x *= spriteDimensions.x;
      spriteOffset.y *= spriteDimensions.y;

      if (accessor->hasToSave())
      {
        v2 pos = toV2(posMapper.map((object->position + accessor->offset) - position))+spriteOffset;
        kRenderer_drawBuffer_defaultShader(buffer, accessor->getSprite()->width, accessor->getSprite()->height, accessor->getSprite()->numChannels,
                                           v3(pos.x, pos.y, object->position.z), spriteDimensions.x, spriteDimensions.y, object->rotation);
      }
      else
      {
        v2 pos = toV2(posMapper.map((object->position + accessor->offset) - position)) + spriteOffset;
        SpriteMapAccessor *ref = (SpriteMapAccessor *)accessor;
        kRenderer_drawStoredTexture_defaultShader(ref->spriteList->texIndex[ref->getMapIndex()],
                                                  v3(pos.x, pos.y, object->position.z), spriteDimensions.x, spriteDimensions.y, object->rotation);
      }
    }
  }
} // namespace kNgine
