#define engine_2D

#include <vector>
#include <string>
#include <cmath>
#include "utils.hpp"
#include "EngineObjects.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Physics.hpp"
#include "SpriteUtils.hpp"

namespace kNgine{
  Camera::Camera():Camera(1,1920,1080) {
  }
  Camera::Camera(float fov, int windowWidth, int windowHeight) {
    this->position = v3(0, 0, 1);
    if (windowWidth < windowHeight) {
      float widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper =
          mapper(v2(-widthFOV / 2, -fov / 2), v2(widthFOV / 2, fov / 2),
                 v2(0, windowHeight), v2(windowHeight, 0));
    } else {
      float heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper =
          mapper(v2(-fov / 2, -heightFOV / 2), v2(fov / 2, heightFOV / 2),
                 v2(0, windowHeight), v2(windowWidth, 0));
    }
    this->fov = fov;
    this->flags.push_back(objectFlags::CAMERA);
  }
  Camera::~Camera() {}

  void Camera::updateWindowSize(int windowWidth, int windowHeight) {
    this->posMapper.targetMax = v2(windowWidth, 0);
    this->posMapper.targetMin = v2(0, windowHeight);
    if (windowWidth < windowHeight)
    {
      float widthFOV = ((float)windowWidth) / windowHeight * fov;
      this->posMapper.min.x = -widthFOV / 2;
      this->posMapper.max.x = widthFOV / 2;
    }
    else
    {
      float heightFOV = ((float)windowHeight) / windowWidth * fov;
      this->posMapper.min.y = -heightFOV / 2;
      this->posMapper.max.y = heightFOV / 2;
    }
  }

  void showLines(ComponentGameObject *obj, mapper posMapper,v3 position) {
    physics::PhysicsBodyComponent *body =
        obj->findComponent<physics::PhysicsBodyComponent>("[physics_body]");
    if (body) {
      // for (float a = 0; a < 360; a += 1) {
      //   if (0)  // draw max dist
      //   {
      //     renderer::setDrawColor(255, 125, 0, 255);
      //     v2 maxPoint = posMapper.map(
      //         obj->position.toV2() - position.toV2() +
      //         v2(fast_cos(degreeToRad(a)) * body->body->maxDistance,
      //            fast_sin(degreeToRad(a)) * body->body->maxDistance));
      //     renderer::drawPoint(maxPoint);
      //   }
      //   renderer::setDrawColor(255, 0, 0, 255);
      //   v2 hitbox = posMapper.map(
      //       obj->position.toV2() - position.toV2() +
      //       v2(fast_cos(degreeToRad(a)) *
      //              body->body->distanceAtAngle(a + obj->rotation.z),
      //          fast_sin(degreeToRad(a)) *
      //              body->body->distanceAtAngle(a + obj->rotation.z)));
      //   renderer::drawPoint(hitbox);
      // }
      v2 velLine[2] = {
          posMapper.map(obj->position.toV2() - position.toV2()),
          posMapper.map(obj->position.toV2() - position.toV2() + body->velocity)};
      renderer::setDrawColor(0, 0, 255, 255);
      renderer::drawLine(velLine);
    }
  }

  void Camera::render(std::vector<ComponentGameObject *> objects) {
    objects = orderObjectsByZ<ComponentGameObject>(
        std::vector<GameObject *>(objects.begin(), objects.end()));
    for (int i = 0; i < objects.size() && objects[i]->position.z<position.z; i++) {
      if (!isBlacklist(objects[i]->labels)) {
        SpriteAccessor *compn =
            objects[i]->findComponent<SpriteAccessor>("[sprite]");
        v2 spriteDimensions = posMapper.map(objects[i]->position.toV2() +
                                            compn->getSpriteDimensions()) -
                              posMapper.map(objects[i]->position.toV2());
        unsigned char *colorMap = compn->getSprite()->colorMap.data();

        v2 spriteOffset = compn->getSpriteOffset();
        spriteOffset.x *= spriteDimensions.x;
        spriteOffset.y *= spriteDimensions.y;
        renderer::drawColorMap(
            colorMap,
            posMapper.map(objects[i]->position.toV2() - position.toV2()) +
                spriteOffset,
            spriteDimensions.x, spriteDimensions.y, compn->getSprite()->width,
            compn->getSprite()->height, compn->getSprite()->numChannels,objects[i]->rotation);
        if (showDebugHitBox) showLines(objects[i], posMapper,position);
      }
    }
  }
  void Camera::renderObject(ComponentGameObject* object){
    if (!isBlacklist(object->labels)) {
      SpriteAccessor *compn =
          object->findComponent<SpriteAccessor>("[sprite]");
      v2 spriteDimensions = posMapper.map(object->position.toV2() +
                                          compn->getSpriteDimensions()) -
                            posMapper.map(object->position.toV2());
      spriteDimensions.y*=-1;
      unsigned char *colorMap = compn->getSprite()->colorMap.data();
      v2 spriteOffset = compn->getSpriteOffset();
      spriteOffset.x *= spriteDimensions.x;
      spriteOffset.y *= spriteDimensions.y;
      if(compn->hasToSave()){
        renderer::drawColorMap(
            colorMap,
            posMapper.map(object->position.toV2() - position.toV2()) +
                spriteOffset,
            spriteDimensions.x, spriteDimensions.y, compn->getSprite()->width,
            compn->getSprite()->height, compn->getSprite()->numChannels,object->rotation);
      }else{
        SpriteMapAccessor *ref = (SpriteMapAccessor *)compn;
        renderer::drawTexture(
            ref->spriteList->texIndex[ref->getMapIndex()],
            posMapper.map(object->position.toV2() - position.toV2()) +
                spriteOffset,spriteDimensions.x,spriteDimensions.y,object->rotation);
      }
      if(showDebugHitBox) showLines(object, posMapper,position);
    }
  }
}
