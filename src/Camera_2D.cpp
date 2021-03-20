#include <vector>
#include <string>
#include <cmath>
#include "utils.h"
#include "EngineObjects.hpp"
#include "kRenderer.h"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

#include <iostream>

namespace kNgine
{
  Camera::Camera() : Camera(1, 1920, 1080)
  {
  }
  Camera::Camera(f32 fov, i32 windowWidth, i32 windowHeight)
  {
    this->position = v3(0, 0, 1);
    if (windowWidth < windowHeight)
    {
      f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper =
          mapper(v2(-widthFOV / 2, -fov / 2), v2(widthFOV / 2, fov / 2),
                 v2(0, windowHeight), v2(windowHeight, 0));
    }
    else
    {
      f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper =
          mapper(v2(-fov / 2, -heightFOV / 2), v2(fov / 2, heightFOV / 2),
                 v2(0, windowHeight), v2(windowWidth, 0));
    }
    this->fov = fov;
    this->flags|=ObjectFlags::RENDERER_LAYER;
  }
  Camera::~Camera() {}

  void Camera::init(std::vector<EngineObject *> objects){
    this->engineInfo = {(u64 *)callEvent("getEngineObjectsSize"), (EngineObject **)callEvent("getEngineObjects")};
    this->layer = layerO(*(LayerOrder *)callEvent("getEngineRendererLayers"), DEFAULT_LAYER);
  }

  void Camera::updateWindowSize(i32 windowWidth, i32 windowHeight)
  {
    this->posMapper.targetMax = v4(windowWidth, 0, 0, 0);
    this->posMapper.targetMin = v4(0, windowHeight, 0, 0);
    if (fovType == MAX_WH)
    {
      if (windowWidth < windowHeight)
      {
        f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
        this->posMapper.min.y = -heightFOV / 2;
        this->posMapper.max.y = heightFOV / 2;
        this->posMapper.min.x = -fov / 2;
        this->posMapper.max.x = fov / 2;
      }
    }
    else if (fovType == MIN_WH)
    {
      if (windowWidth > windowHeight)
      {
        f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
        this->posMapper.min.y = -heightFOV / 2;
        this->posMapper.max.y = heightFOV / 2;
        this->posMapper.min.x = -fov / 2;
        this->posMapper.max.x = fov / 2;
      }
    }
    else if (fovType == WIDTH)
    {
      f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper.min.x = -widthFOV / 2;
      this->posMapper.max.x = widthFOV / 2;
      this->posMapper.min.y = -fov / 2;
      this->posMapper.max.y = fov / 2;
    }
    else if (fovType == HEIGHT)
    {
      f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper.min.y = -heightFOV / 2;
      this->posMapper.max.y = heightFOV / 2;
      this->posMapper.min.x = -fov / 2;
      this->posMapper.max.x = fov / 2;
    }
    else if (fovType == DIAGONAL)
    {
      // f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
      // this->posMapper.min.x = -widthFOV / 2;
      // this->posMapper.max.x = widthFOV / 2;
      // f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
      // this->posMapper.min.y = -heightFOV / 2;
      // this->posMapper.max.y = heightFOV / 2;
    }
    else
    {
      if (windowWidth < windowHeight)
      {
        f32 widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        f32 heightFOV = ((float)windowHeight) / windowWidth * fov;
        this->posMapper.min.y = -heightFOV / 2;
        this->posMapper.max.y = heightFOV / 2;
        this->posMapper.min.x = -fov / 2;
        this->posMapper.max.x = fov / 2;
      }
    }
  }

  void Camera::render()
  {
    std::vector<ComponentGameObject *> objects = findObject<ComponentGameObject>(this->engineInfo.engineObjects, *this->engineInfo.engineObjectLength, ObjectFlags::SPRITE);
    objects = findObjectBlacklist<ComponentGameObject>(orderObjectsByZ<ComponentGameObject>(objects),"");
    for(u32 i=0;i<objects.size();i++){
      render(objects[i]);
    }
  }

  void Camera::render(ComponentGameObject *object)
  {
    SpriteAccessor *compn =
        object->findComponent<SpriteAccessor>("[sprite]");
    i32 numSprite = 1;
    bool isSpriteList = false;
    if (!(compn))
    {
      compn = object->findComponent<SpriteList>("[sprite_list]");
      numSprite = ((SpriteList*)compn)->getSpriteListLength();
      isSpriteList = true;
    }
    for (i32 i = 0; i < numSprite; i++)
    {
      SpriteAccessor*accessor=compn;
      if (isSpriteList)
      {
        accessor = ((SpriteList*)compn)->getSpriteList()[i];
      }
      v2 spriteDimensions = V2MinusV2(posMapper.map(V2AddV2(toV2(object->position),
                                                            accessor->getSpriteDimensions())),
                                      posMapper.map(toV2(object->position)));
      u8 *buffer = accessor->getSprite()->buffer;
      v2 spriteOffset = accessor->getSpriteLocation();
      spriteOffset.x *= spriteDimensions.x;
      spriteOffset.y *= spriteDimensions.y;

      if (accessor->hasToSave())
      {
        v2 pos = V2AddV2(posMapper.map(V2MinusV2(V2AddV2(toV2(object->position), accessor->offset), toV2(position))),spriteOffset);
        kRenderer_drawBuffer_defaultShader(buffer, accessor->getSprite()->width, accessor->getSprite()->height, accessor->getSprite()->numChannels,
                                           v3(pos.x, pos.y, object->position.z), spriteDimensions.x, spriteDimensions.y, object->rotation);
      }
      else
      {
        v2 pos = V2AddV2(posMapper.map(V2MinusV2(V2AddV2(toV2(object->position), accessor->offset), toV2(position))), spriteOffset);
        SpriteMapAccessor *ref = (SpriteMapAccessor *)accessor;
        kRenderer_drawStoredTexture_defaultShader(ref->spriteList->texIndex[ref->getMapIndex()],
                                                  v3(pos.x, pos.y, object->position.z), spriteDimensions.x, spriteDimensions.y, object->rotation);
      }
    }
    if (showDebugHitBox)
    {
      // showLines(object, posMapper, position);
    }
  }
} // namespace kNgine
