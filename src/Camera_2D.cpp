#define engine_2D

#include <vector>
#include <string>
#include <cmath>
// #define vectorObjects
#include "utils.h"
#include "EngineObjects.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

namespace kNgine
{
  Camera::Camera() : Camera(1, 1920, 1080)
  {
  }
  Camera::Camera(float fov, int windowWidth, int windowHeight)
  {
    this->position = v3(0, 0, 1);
    if (windowWidth < windowHeight)
    {
      float widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper =
          mapper(v2(-widthFOV / 2, -fov / 2), v2(widthFOV / 2, fov / 2),
                 v2(0, windowHeight), v2(windowHeight, 0));
    }
    else
    {
      float heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper =
          mapper(v2(-fov / 2, -heightFOV / 2), v2(fov / 2, heightFOV / 2),
                 v2(0, windowHeight), v2(windowWidth, 0));
    }
    this->fov = fov;
    this->flags.push_back(objectFlags::CAMERA);
  }
  Camera::~Camera() {}

  void Camera::updateWindowSize(int windowWidth, int windowHeight)
  {
    this->posMapper.targetMax = v4(windowWidth, 0,0,0);
    this->posMapper.targetMin = v4(0, windowHeight,0,0);
    if (fovType == MAX_WH)
    {
      if (windowWidth < windowHeight)
      {
        float widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        float heightFOV = ((float)windowHeight) / windowWidth * fov;
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
        float widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        float heightFOV = ((float)windowHeight) / windowWidth * fov;
        this->posMapper.min.y = -heightFOV / 2;
        this->posMapper.max.y = heightFOV / 2;
        this->posMapper.min.x = -fov / 2;
        this->posMapper.max.x = fov / 2;
      }
    }
    else if (fovType == WIDTH)
    {
      float widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper.min.x = -widthFOV / 2;
      this->posMapper.max.x = widthFOV / 2;
      this->posMapper.min.y = -fov / 2;
      this->posMapper.max.y = fov / 2;
    }
    else if (fovType == HEIGHT)
    {
      float heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper.min.y = -heightFOV / 2;
      this->posMapper.max.y = heightFOV / 2;
      this->posMapper.min.x = -fov / 2;
      this->posMapper.max.x = fov / 2;
    }
    else if (fovType == DIAGONAL)
    {
      // float widthFOV = ((float)windowWidth) / windowHeight * fov;
      // this->posMapper.min.x = -widthFOV / 2;
      // this->posMapper.max.x = widthFOV / 2;
      // float heightFOV = ((float)windowHeight) / windowWidth * fov;
      // this->posMapper.min.y = -heightFOV / 2;
      // this->posMapper.max.y = heightFOV / 2;
    }
    else
    {
      if (windowWidth < windowHeight)
      {
        float widthFOV = ((float)windowWidth) / windowHeight * fov;
        this->posMapper.min.x = -widthFOV / 2;
        this->posMapper.max.x = widthFOV / 2;
        this->posMapper.min.y = -fov / 2;
        this->posMapper.max.y = fov / 2;
      }
      else
      {
        float heightFOV = ((float)windowHeight) / windowWidth * fov;
        this->posMapper.min.y = -heightFOV / 2;
        this->posMapper.max.y = heightFOV / 2;
        this->posMapper.min.x = -fov / 2;
        this->posMapper.max.x = fov / 2;
      }
    }
  }

  void Camera::render(std::vector<ComponentGameObject *> objects)
  {
    objects = orderObjectsByZ<ComponentGameObject>(
        std::vector<GameObject *>(objects.begin(), objects.end()));
    for (int i = 0; i < objects.size() && objects[i]->position.z < position.z; i++)
    {
      if (!isBlacklist(objects[i]->labels))
      {
        SpriteAccessor *compn =
            objects[i]->findComponent<SpriteAccessor>("[sprite]");
        v2 spriteDimensions = V2MinusV2(posMapper.map(V2AddV2(toV2(objects[i]->position),
                                            toV2(compn->getSpriteDimensions()))),
                              posMapper.map(toV2(objects[i]->position)));
        unsigned char *colorMap = compn->getSprite()->colorMap.data();

        v2 spriteOffset = compn->getSpriteLocation();
        spriteOffset.x *= spriteDimensions.x;
        spriteOffset.y *= spriteDimensions.y;
        renderer::drawColorMap(
            colorMap,
            V2AddV2(posMapper.map(V2MinusV2(toV2(objects[i]->position), toV2(position))),
                spriteOffset),
            spriteDimensions.x, spriteDimensions.y, compn->getSprite()->width,
            compn->getSprite()->height, compn->getSprite()->numChannels, objects[i]->rotation);
        if (showDebugHitBox){
          // showLines(objects[i], posMapper, position);
        }
      }
    }
  }
  void Camera::renderObject(ComponentGameObject *object)
  {
    if (!isBlacklist(object->labels))
    {
      SpriteAccessor *compn =
          object->findComponent<SpriteAccessor>("[sprite]");
      int numSprite=1;
      bool isSpriteList=false;
      if(!(compn)){
        numSprite = object->findComponent<SpriteList>("[sprite_list]")->getSpriteListLength();
        isSpriteList=true;
      }

      for(int i=0;i<numSprite;i++){
        if (isSpriteList)
        {
          compn = object->findComponent<SpriteList>("[sprite_list]")->getSpriteList()[i];
        }
        v2 spriteDimensions = V2MinusV2(posMapper.map(V2AddV2(toV2(object->position),
                                            compn->getSpriteDimensions())),
                              posMapper.map(toV2(object->position)));
        spriteDimensions.y *= -1;
        unsigned char *colorMap = compn->getSprite()->colorMap.data();
        v2 spriteOffset = compn->getSpriteLocation();
        spriteOffset.x *= spriteDimensions.x;
        spriteOffset.y *= spriteDimensions.y;
        if (compn->hasToSave())
        {
          renderer::drawColorMap(
              colorMap,
              V2AddV2(posMapper.map(V2MinusV2(V2AddV2(toV2(object->position), compn->offset), toV2(position))),
                  spriteOffset),
              spriteDimensions.x, spriteDimensions.y, compn->getSprite()->width,
              compn->getSprite()->height, compn->getSprite()->numChannels, object->rotation);
        }
        else
        {
          SpriteMapAccessor *ref = (SpriteMapAccessor *)compn;
          renderer::drawTexture(
              ref->spriteList->texIndex[ref->getMapIndex()],
              V2AddV2(posMapper.map(V2MinusV2(V2AddV2(toV2(object->position), compn->offset), toV2(position))),
                  spriteOffset),
              spriteDimensions.x, spriteDimensions.y, object->rotation);
        }
      }
      if (showDebugHitBox){
        // showLines(object, posMapper, position);
      }
    }
  }
} // namespace kNgine

#undef vectorObjects