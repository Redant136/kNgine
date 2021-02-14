#define engine_2D

#include <cmath>
#include "utils.hpp"
#include "EngineObjects.hpp"
#include "Physics.hpp"

namespace kNgine{
  namespace physics {
  HitBox::~HitBox() {}
  void HitBox::setMaxDistance() {
    for (float a = 0; a < 360; a += 0.001) {
      float dist = distanceAtAngle(a);
      if (maxDistance < dist) {
        maxDistance = dist;
      }
    }
  }
  bool HitBox::isInside(v2 point) { return false; }
  float HitBox::squareDistanceAtAngle(float angle) { return 0; }
  float HitBox::distanceAtAngle(float angle) { return 0; }
  v2 HitBox::getPointAtAngle(float angle) {
    float dist = distanceAtAngle(angle);
    return getPointAroundCircleDegree(angle) * dist;
  }
  v2 HitBox::getPointAtAngleWithRotation(float angle, float rotation) {
    float dist = distanceAtAngle(angle + rotation);
    return getPointAroundCircleDegree(angle) * dist;
  }
  std::vector<v2> HitBox::getCorners() {
    std::vector<v2> corners = std::vector<v2>();
    corners.push_back(v2(0, 0));
    return corners;
  }

  rectHitBox::rectHitBox() {
    width = 0;
    height = 0;
  }
  rectHitBox::rectHitBox(float width, float height) {
    this->width = width;
    this->height = height;
    setMaxDistance();
  }
  rectHitBox::~rectHitBox() {}
  void rectHitBox::setMaxDistance() {
    this->maxDistance =
        std::sqrt(width / 2 * width / 2 + height / 2 * height / 2);
  }
  bool rectHitBox::isInside(v2 point) {
    return point.x <= width / 2 && point.x >= -width / 2 &&
          point.y <= height / 2 && point.y >= -height / 2;
  }
  float rectHitBox::squareDistanceAtAngle(float angle) {
    return distanceAtAngle(angle) * distanceAtAngle(angle);
  }
  float rectHitBox::distanceAtAngle(float angle) {
    float t_angle = fmod(angle, 360);
    if (t_angle < 0) t_angle += 360;
    if (t_angle > 180) t_angle -= 360;
    if (t_angle == 90 || t_angle == -90) {
      return height / 2.0;
    }

    float cornerAngle = radToDegree(fast_atan(height / width));
    float res = 0;
    if (t_angle <= cornerAngle && t_angle > -cornerAngle) {
      res = width / 2 / fast_cos(degreeToRad(angle));
    } else if (t_angle > 180 - cornerAngle || t_angle <= -(180 - cornerAngle)) {
      res = -width / 2 / fast_cos(degreeToRad(angle));
    } else if (t_angle > cornerAngle && t_angle <= 180 - cornerAngle) {
      res = height / 2 / fast_sin(degreeToRad(angle));
    } else if (t_angle <= -cornerAngle && t_angle > -(180 - cornerAngle)) {
      res = -height / 2 / fast_sin(degreeToRad(angle));
    }
    return res;
  }
  std::vector<v2> rectHitBox::getCorners() {
    std::vector<v2> res;
    res.push_back(v2(width / 2.0, height / 2.0));
    res.push_back(v2(width / 2.0, -height / 2.0));
    res.push_back(v2(-width / 2.0, -height / 2.0));
    res.push_back(v2(-width / 2.0, height / 2.0));

    return res;
  }

  circularHitBox::circularHitBox() { radius = 0; }
  circularHitBox::circularHitBox(float radius) {
    this->radius = radius;
    setMaxDistance();
  }
  circularHitBox::~circularHitBox() {}
  bool circularHitBox::isInside(v2 point) {
    return point.x * point.x + point.y * point.y <= radius * radius;
  }
  float circularHitBox::squareDistanceAtAngle(float angle) {
    return radius * radius;
  }
  float circularHitBox::distanceAtAngle(float angle) { return radius; }
  // v2 circularHitBox::getCorners(float angle){return
  // v2(fast_cos(angle)*radius,fast_sin(angle)*radius);}

  HitBoxComponent::HitBoxComponent(ComponentGameObject *base)
      : ObjectComponent(base) {
    this->label = "[HitBox]";
    base->flags.push_back(objectFlags::Physics);
    base->labels.push_back(label);
  }
  void HitBoxComponent::update(std::vector<msg> msgs) {}

  PhysicsBodyComponent::PhysicsBodyComponent(ComponentGameObject *base)
      : ObjectComponent(base) {
    this->label = "[physics_body]";
    base->flags.push_back(objectFlags::Physics);
    base->labels.push_back(label);
    this->lastPosition = object->position;
    this->body = new HitBox();
    this->weight = -1;
    this->elasticity = 0;
  }
  PhysicsBodyComponent::PhysicsBodyComponent(HitBox *body,
                                            ComponentGameObject *base)
      : ObjectComponent(base) {
    this->label = "[physics_body]";
    base->flags.push_back(objectFlags::Physics);
    base->labels.push_back(label);
    this->lastPosition = object->position;
    this->body = body;
    this->weight = 1;
    this->elasticity = 1;
  }
  PhysicsBodyComponent::PhysicsBodyComponent(HitBox *body, float weight,
                                            ComponentGameObject *base)
      : ObjectComponent(base) {
    this->label = "[physics_body]";
    base->flags.push_back(objectFlags::Physics);
    base->labels.push_back(label);
    this->lastPosition = object->position;
    this->body = body;
    this->weight = weight;
    this->elasticity = 1;
  }
  PhysicsBodyComponent::PhysicsBodyComponent(HitBox *body, float weight,
                                            float elasticity,
                                            ComponentGameObject *base)
      : ObjectComponent(base) {
    this->label = "[physics_body]";
    base->flags.push_back(objectFlags::Physics);
    base->labels.push_back(label);
    this->lastPosition = object->position;
    this->body = body;
    this->weight = weight;
    this->elasticity = elasticity;
  }
  void PhysicsBodyComponent ::update(std::vector<msg> msgs) {
    lastPosition = object->position;
    if (!engineIncluded) {
      float timeElapsed;
      for (msg mess : msgs) {
        if (mess.msgType == msg::TIME_ELAPSED) {
          timeElapsed = mess.msgBody.time;
          break;
        }
      }
      velocity += acceleration * timeElapsed;
      object->position += velocity * timeElapsed;
      rotationVel += rotationAcc * timeElapsed;
      object->rotation += rotationVel * timeElapsed;
      if (velocity.x != 0 && velocity.y != 0) {
        touchOtherBody = false;
      }
    }
  }

  bool collision_SAT(v2 hitbox1Pos, HitBox *hitbox1, v2 hitbox2Pos,
                    HitBox *hitbox2, float rotation1, float rotation2) {
    v3 distance = hitbox2Pos - hitbox1Pos;
    bool hit = optimizedLineLength(v2(0, 0), distance.toV2()) <
              (hitbox1->maxDistance + hitbox2->maxDistance) *
                  (hitbox1->maxDistance + hitbox2->maxDistance);
    if (hit) {
      std::vector<v2> corners1 = hitbox1->getCorners();
      std::vector<v2> corners2 = hitbox2->getCorners();
      // formatting corners to reflect real position
      for (int i = 0; i < corners1.size(); i++) {
        corners1[i] = rotateAroundPointDegree(v2(0, 0), corners1[i], rotation1);
        corners1[i] += hitbox1Pos;
      }
      for (int i = 0; i < corners2.size(); i++) {
        corners2[i] = rotateAroundPointDegree(v2(0, 0), corners2[i], rotation2);
        corners2[i] += hitbox2Pos;
      }

      if (corners1.size() > 1 && corners2.size() > 1) {
        for (int p1 = 0; p1 < corners1.size(); p1++) {
          int p2 = (p1 + 1) % corners1.size();
          v2 axisProj = v2(-(corners1[p2].y - corners1[p1].y),
                          corners1[p2].x - corners1[p1].x);
          // axisProj /= sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
          float min_r1 = INFINITY, max_r1 = -INFINITY;
          for (int p = 0; p < corners1.size(); p++) {
            float q = (corners1[p].x * axisProj.x + corners1[p].y * axisProj.y);
            min_r1 = std::min(min_r1, q);
            max_r1 = std::max(max_r1, q);
          }
          float min_r2 = INFINITY, max_r2 = -INFINITY;
          for (int p = 0; p < corners2.size(); p++) {
            float q = (corners2[p].x * axisProj.x + corners2[p].y * axisProj.y);
            min_r2 = std::min(min_r2, q);
            max_r2 = std::max(max_r2, q);
          }
          if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) return false;
        }
        for (int p1 = 0; p1 < corners2.size(); p1++) {
          int p2 = (p1 + 1) % corners2.size();
          v2 axisProj = v2(-(corners2[p2].y - corners2[p1].y),
                          corners2[p2].x - corners2[p1].x);
          // axisProj /= sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
          float min_r1 = INFINITY, max_r1 = -INFINITY;
          for (int p = 0; p < corners2.size(); p++) {
            float q = (corners2[p].x * axisProj.x + corners2[p].y * axisProj.y);
            min_r1 = std::min(min_r1, q);
            max_r1 = std::max(max_r1, q);
          }
          float min_r2 = INFINITY, max_r2 = -INFINITY;
          for (int p = 0; p < corners1.size(); p++) {
            float q = (corners1[p].x * axisProj.x + corners1[p].y * axisProj.y);
            min_r2 = std::min(min_r2, q);
            max_r2 = std::max(max_r2, q);
          }
          if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) return false;
        }
        return true;
      } else {
        float angle = -radToDegree(fast_atan(distance.y / distance.x));
        if (distance.x < 0) angle -= 180;
        for (float a = 0; a < 135; a += 1) {
          hit = hitbox2->isInside(rotateAroundPointDegree(
              v2(0, 0),
              hitbox1->getPointAtAngleWithRotation(-angle + a, rotation1) +
                  hitbox1Pos - hitbox2Pos,
              rotation2));
          if (hit) {
            return true;
          }
          hit = hitbox2->isInside(rotateAroundPointDegree(
              v2(0, 0),
              hitbox1->getPointAtAngleWithRotation(-angle - a, rotation1) +
                  hitbox1Pos - hitbox2Pos,
              rotation2));
          if (hit) {
            return true;
          }
        }
      }
      return false;
    }
    return false;
  }

  bool isHit(PhysicsBodyComponent *bod1, PhysicsBodyComponent *bod2) {
    if (bod1 == bod2) return false;
    if (hitbox_inside(bod1->object->position.toV2(), bod1->body,
                      bod2->object->position.toV2(), bod2->body,
                      bod1->object->rotation.z, bod2->object->rotation.z)) {
      return true;
    }
    if ((bod1->object->position.x <= bod2->object->position.x !=
        bod1->lastPosition.x <= bod2->object->position.x) &&
        (bod1->object->position.y <= bod2->object->position.y !=
        bod1->lastPosition.y <= bod2->object->position.y)) {
      return true;
    }
    if ((bod2->object->position.x <= bod1->object->position.x !=
        bod2->lastPosition.x <= bod1->object->position.x) &&
        (bod2->object->position.y <= bod1->object->position.y !=
        bod2->lastPosition.y <= bod1->object->position.y)) {
      return true;
    }
    return false;
  }

  PhysicsEngine::PhysicsEngine() {
    physObjects = std::vector<PhysicsBodyComponent *>();
    this->flags.push_back(objectFlags::PhysicsEngine);
  }
  void PhysicsEngine::update(std::vector<msg> msgs) {
    float timeElapsed;
    for (msg mess : msgs) {
      if (mess.msgType == msg::TIME_ELAPSED) {
        timeElapsed = mess.msgBody.time;
        break;
      }
    }
    for (int i = 0; i < physObjects.size(); i++) {
      //useless code to make sure everything is setup ok
      physObjects[i]->object->position = physObjects[i]->lastPosition;
    }
    float stepSize = 0.01;
    for (int step = 0; step < 1.0 / stepSize; step++) {
      for (int i = 0; i < physObjects.size(); i++) {
        physObjects[i]->lastPosition = physObjects[i]->object->position;
        physObjects[i]->velocity +=
            physObjects[i]->acceleration * timeElapsed * stepSize;
        physObjects[i]->object->position +=
            physObjects[i]->velocity * timeElapsed * stepSize;
        physObjects[i]->rotationVel +=
            physObjects[i]->rotationAcc * timeElapsed * stepSize;
        physObjects[i]->object->rotation +=
            physObjects[i]->rotationVel * timeElapsed * stepSize;
        if (physObjects[i]->velocity.x != 0 && physObjects[i]->velocity.y != 0) {
          physObjects[i]->touchOtherBody = false;
        }
      }
      for (int i = 0; i < physObjects.size(); i++) {
        for (int j = i + 1; j < physObjects.size(); j++) {
          bool hit = isHit(physObjects[i], physObjects[j]);
          if (hit) {
            physObjects[i]->object->position -=
                physObjects[i]->velocity * timeElapsed * stepSize;
            physObjects[i]->object->rotation -=
                physObjects[i]->rotationVel * timeElapsed * stepSize;
            physObjects[j]->object->position -=
                physObjects[j]->velocity * timeElapsed * stepSize;
            physObjects[j]->object->rotation -=
                physObjects[j]->rotationVel * timeElapsed * stepSize;

            // set bouce vel
            v2 objectINewVel = physObjects[i]->velocity;
            v2 objectJNewVel = physObjects[j]->velocity;

            v2 rv = physObjects[j]->velocity -
                    physObjects[i]->velocity;  // vel of J relative to I
            // test after if it needs to be scalled so that length==1
            v2 normal=v2();
            { // fuck this messy code to find normal of collision
              // setup corners
              std::vector<v2> cornersI = physObjects[i]->body->getCorners();
              std::vector<v2> cornersJ = physObjects[j]->body->getCorners();
              for (int c = 0; c < cornersI.size(); c++) {
                cornersI[c] = rotateAroundPointDegree(
                    v2(0, 0), cornersI[c], physObjects[i]->object->rotation.z);
                cornersI[c] += physObjects[i]->object->position.toV2();
              }
              for (int c = 0; c < cornersJ.size(); c++) {
                cornersJ[c] = rotateAroundPointDegree(
                    v2(0, 0), cornersJ[c], physObjects[j]->object->rotation.z);
                cornersJ[c] += physObjects[j]->object->position.toV2();
              }
              if (cornersI.size() > 1 && cornersJ.size() > 1) {
                float min = INFINITY;
                for (int c1I = 0; c1I < cornersI.size(); c1I++) {
                  int c2I = (c1I + 1) % cornersI.size();
                  for (int pJ = 0; pJ < cornersJ.size(); pJ++) {
                    v2 lineTest = cornersJ[pJ] + physObjects[j]->velocity *
                                                    timeElapsed * stepSize;
                    if (lineCross(cornersI[c1I], cornersI[c2I], cornersJ[pJ],
                                  lineTest)) {
                      float crossPointLength =
                          optimizedLineLength(cornersJ[pJ],
                                    collisionLine(cornersI[c1I], cornersI[c2I],
                                                  cornersJ[pJ], lineTest));
                      if (min > crossPointLength) {
                        min = crossPointLength;
                        normal = v2(-(cornersI[c2I].y - cornersI[c1I].y),
                                    cornersI[c2I].x - cornersI[c1I].x);
                      }
                    }
                  }
                }
                for (int c1J = 0; c1J < cornersJ.size(); c1J++) {
                  int c2J = (c1J + 1) % cornersJ.size();
                  for (int pI = 0; pI < cornersI.size(); pI++) {
                    v2 lineTest = cornersJ[pI] + physObjects[j]->velocity *
                                                    timeElapsed * stepSize;
                    if (lineCross(cornersI[c1J], cornersI[c2J], cornersJ[pI],
                                  lineTest)) {
                      float crossPointLength =
                          optimizedLineLength(cornersJ[pI],
                                    collisionLine(cornersI[c1J], cornersI[c2J],
                                                  cornersJ[pI], lineTest));
                      if (min > crossPointLength) {
                        min = crossPointLength;
                        normal = v2(-(cornersI[c2J].y - cornersI[c1J].y),
                                    cornersI[c2J].x - cornersI[c1J].x);
                      }
                    }
                  }
                }
                if(min==INFINITY){
                  break;
                }
              } else {
                // dumb code for circle
                float min = INFINITY;
                int pointI,pointJ;
                for (int cI = 0; cI < cornersI.size(); cI++) {
                  for (int cJ = 0; cJ < cornersJ.size(); cJ++) {
                    float l = optimizedLineLength(cornersJ[cJ], cornersI[cI]);
                    if (l < min) {
                      min = l;
                      pointI=cI;
                      pointJ=cJ;
                    }
                  }
                }

                if(cornersI.size()>1){
                  v2 shapeLine=cornersI[(pointI+1)%cornersI.size()]-cornersI[pointI];
                  v2 pointLine=cornersJ[pointJ]-cornersI[pointI];
                  bool cornerBounce=true;
                  // if center hits face
                  if(shapeLine.x*pointLine.x+shapeLine.y*pointLine.y>0){
                    cornerBounce=false;
                    normal=v2(-shapeLine.y,shapeLine.x);
                  }
                  shapeLine = cornersI[(pointI - 1) % cornersI.size()] - cornersI[pointI];
                  if (shapeLine.x * pointLine.x + shapeLine.y * pointLine.y > 0) {
                    cornerBounce = false;
                    normal = v2(-shapeLine.y, shapeLine.x);
                  }
                  if(cornerBounce){
                    normal=pointLine;
                  }
                }else if(cornersJ.size()>1){
                  v2 shapeLine = cornersJ[(pointJ + 1) % cornersJ.size()] - cornersJ[pointJ];
                  v2 pointLine = cornersI[pointI] - cornersJ[pointJ];
                  bool cornerBounce = true;
                  // if center hits face
                  if (shapeLine.x * pointLine.x + shapeLine.y * pointLine.y > 0) {
                    cornerBounce = false;
                    normal = v2(-shapeLine.y, shapeLine.x);
                  }
                  shapeLine =
                      cornersJ[(pointJ - 1) % cornersJ.size()] - cornersJ[pointJ];
                  if (shapeLine.x * pointLine.x + shapeLine.y * pointLine.y > 0) {
                    cornerBounce = false;
                    normal = v2(-shapeLine.y, shapeLine.x);
                  }
                  if (cornerBounce) {
                    normal = pointLine;
                  }
                }else{
                  normal = physObjects[j]->object->position.toV2() -
                          physObjects[i]->object->position.toV2();
                }
              }
            }
            normal/=lineLength(v2(0,0),normal);
            float velAlongNormal=rv.x*normal.x+rv.y*normal.y;
            float elasticity=std::min(physObjects[i]->elasticity,physObjects[j]->elasticity);
            float impulseScalar=-(1+elasticity)*velAlongNormal;
            if (physObjects[i]->weight <= 0 && physObjects[j]->weight <= 0) {
              break;
            } else if (physObjects[i]->weight <= 0) {
              impulseScalar /= 1.0 / physObjects[j]->weight;
            } else if (physObjects[j]->weight <= 0) {
              impulseScalar /= 1.0 / physObjects[i]->weight;
            }else{
              impulseScalar /=
                  1.0 / physObjects[i]->weight + 1.0 / physObjects[j]->weight;
            }
            v2 impulse = normal * impulseScalar;
            if (physObjects[i]->weight <= 0 && physObjects[j]->weight <= 0) {
              break;
            } else if (physObjects[i]->weight <= 0) {
              physObjects[j]->velocity += impulse * 1 / physObjects[j]->weight;
            } else if (physObjects[j]->weight <= 0) {
              physObjects[i]->velocity -= impulse * 1 / physObjects[i]->weight;
            } else {
              physObjects[i]->velocity -= impulse * 1 / physObjects[i]->weight;
              physObjects[j]->velocity += impulse * 1 / physObjects[j]->weight;
            }
            // v2 objectINewVel = objects[i]->veloobjects[i]->velocity;city +
            //                    (objects[j]->velocity - objects[i]->velocity) *
            //                        objects[j]->weight / objects[i]->weight;
            // v2 objectJNewVel = objects[j]->velocity +
            //                    (objects[i]->velocity - objects[j]->velocity) *
            //                        objects[i]->weight / objects[j]->weight;
            // if (objects[i]->weight <= 0 && objects[j]->weight <= 0) {
            //   objectINewVel = objects[i]->velocity;
            //   objectJNewVel = objects[j]->velocity;
            // } else if (objects[i]->weight <= 0) {
            //   objectINewVel = objects[i]->velocity;
            //   objectJNewVel = (objects[i]->velocity - objects[j]->velocity);
            // } else if (objects[j]->weight <= 0) {
            //   objectINewVel = (objects[j]->velocity - objects[i]->velocity);
            //   objectJNewVel = objects[j]->velocity;
            // }
            // // insert bounce angle reflection here
            // objectINewVel *=
            // std::min(objects[i]->elasticity,objects[j]->elasticity);
            // objectJNewVel *=
            // std::min(objects[i]->elasticity,objects[j]->elasticity);
            // end of set new vel
            // if object is pushing on another
            // float newWeightI = objects[i]->weight;
            // float newWeightJ = objects[j]->weight;
            // if (objects[i]->touchOtherBody || objects[j]->touchOtherBody) {
            //   if (objects[i]->touchOtherBody) {
            //     objects[i]->object->position += objectINewVel * timeElapsed;
            //     for (int k = 0; k < objects.size(); k++) {
            //       if (k != i && k != j && isHit(objects[i], objects[k])) {
            //         if (objects[k]->weight <= 0) {
            //           newWeightI = 0;
            //           break;
            //         }
            //         newWeightI += objects[k]->weight;
            //       }
            //     }
            //     objects[i]->object->position -= objectINewVel * timeElapsed;
            //   }
            //   if (objects[j]->touchOtherBody) {
            //     objects[j]->object->position += objectJNewVel * timeElapsed;
            //     for (int k = 0; k < objects.size(); k++) {
            //       if (k != i && k != j && isHit(objects[j], objects[k])) {
            //         if (objects[k]->weight <= 0) {
            //           newWeightJ = 0;
            //           break;
            //         }
            //         newWeightJ += objects[k]->weight;
            //       }
            //     }
            //     objects[j]->object->position -= objectJNewVel * timeElapsed;
            //   }
            //   objectINewVel = objects[i]->velocity +
            //                   (objects[j]->velocity - objects[i]->velocity) *
            //                       objects[j]->weight / objects[i]->weight;
            //   objectJNewVel = objects[j]->velocity +
            //                   (objects[i]->velocity - objects[j]->velocity) *
            //                       objects[i]->weight / objects[j]->weight;
            //   if (newWeightI <= 0 && newWeightJ <= 0) {
            //     objectINewVel = objects[i]->velocity;
            //     objectJNewVel = objects[j]->velocity;
            //   } else if (newWeightI <= 0) {
            //     objectINewVel = objects[i]->velocity;
            //     objectJNewVel = (objects[i]->velocity - objects[j]->velocity);
            //   } else if (newWeightJ <= 0) {
            //     objectINewVel = (objects[j]->velocity - objects[i]->velocity);
            //     objectJNewVel = objects[j]->velocity;
            //   }
            //   objectINewVel *= objects[i]->elasticity * objects[j]->elasticity;
            //   objectJNewVel *= objects[i]->elasticity * objects[j]->elasticity;
            //   objects[i]->velocity = objectINewVel;
            //   objects[j]->velocity = objectJNewVel;
            // }
            physObjects[i]->touchOtherBody = true;
            physObjects[j]->touchOtherBody = true;
          }
        }
      }
    }

    // for (int i = 0; i < objects.size(); i++) {
    //   for (int j = i + 1; j < objects.size(); j++) {
    //     bool hit = isHit(objects[i], objects[j]);
    //     if (hit) {
    //       //uncollide, yes this is a word
    //       v2 reversePathI =
    //           (objects[i]->lastPosition - objects[i]->object->position).toV2();
    //       v2 reversePathJ =
    //           (objects[j]->lastPosition - objects[j]->object->position).toV2();
    //       v3 testPosI = objects[i]->object->position;
    //       v3 testPosJ = objects[j]->object->position;
    //       for (float step = 0; step <= 1; step += 0.01) {
    //         objects[i]->object->position = testPosI + reversePathI * step;
    //         objects[j]->object->position = testPosJ + reversePathJ * step;
    //         if (!isHit(objects[i], objects[j])) {
    //           break;
    //         }
    //       }

    //       //code to update vel

    //       // v2 objectINewVel = objects[i]->velocity +
    //       //                    (objects[j]->velocity - objects[i]->velocity) *
    //       //                        objects[j]->weight / objects[i]->weight;
    //       // v2 objectJNewVel = objects[j]->velocity +
    //       //                    (objects[i]->velocity - objects[j]->velocity) *
    //       //                        objects[i]->weight / objects[j]->weight;
    //       // if (objects[i]->weight <= 0 && objects[j]->weight <= 0) {
    //       //   objectINewVel = objects[i]->velocity;
    //       //   objectJNewVel = objects[j]->velocity;
    //       // } else if (objects[i]->weight <= 0) {
    //       //   objectINewVel = objects[i]->velocity;
    //       //   objectJNewVel = (objects[i]->velocity - objects[j]->velocity);
    //       // } else if (objects[j]->weight <= 0) {
    //       //   objectINewVel = (objects[j]->velocity - objects[i]->velocity);
    //       //   objectJNewVel = objects[j]->velocity;
    //       // }
    //       // objectINewVel *= objects[i]->elasticity * objects[j]->elasticity;
    //       // objectJNewVel *= objects[i]->elasticity * objects[j]->elasticity;
    //       // float newWeightI = objects[i]->weight;
    //       // float newWeightJ = objects[j]->weight;
    //       // if (objects[i]->touchOtherBody || objects[j]->touchOtherBody) {
    //       //   if (objects[i]->touchOtherBody) {
    //       //     objects[i]->object->position += objectINewVel * timeElapsed;
    //       //     for (int k = 0; k < objects.size(); k++) {
    //       //       if (k != i && k != j && isHit(objects[i], objects[k])) {
    //       //         if (objects[k]->weight <= 0) {
    //       //           newWeightI = 0;
    //       //           break;
    //       //         }
    //       //         newWeightI += objects[k]->weight;
    //       //       }
    //       //     }
    //       //     objects[i]->object->position -= objectINewVel * timeElapsed;
    //       //   }
    //       //   if (objects[j]->touchOtherBody) {
    //       //     objects[j]->object->position += objectJNewVel * timeElapsed;
    //       //     for (int k = 0; k < objects.size(); k++) {
    //       //       if (k != i && k != j && isHit(objects[j], objects[k])) {
    //       //         if (objects[k]->weight <= 0) {
    //       //           newWeightJ = 0;
    //       //           break;
    //       //         }
    //       //         newWeightJ += objects[k]->weight;
    //       //       }
    //       //     }
    //       //     objects[j]->object->position -= objectJNewVel * timeElapsed;
    //       //   }
    //       //   objectINewVel = objects[i]->velocity +
    //       //                   (objects[j]->velocity - objects[i]->velocity) *
    //       //                       objects[j]->weight / objects[i]->weight;
    //       //   objectJNewVel = objects[j]->velocity +
    //       //                   (objects[i]->velocity - objects[j]->velocity) *
    //       //                       objects[i]->weight / objects[j]->weight;
    //       //   if (newWeightI <= 0 && newWeightJ <= 0) {
    //       //     objectINewVel = objects[i]->velocity;
    //       //     objectJNewVel = objects[j]->velocity;
    //       //   } else if (newWeightI <= 0) {
    //       //     objectINewVel = objects[i]->velocity;
    //       //     objectJNewVel = (objects[i]->velocity - objects[j]->velocity);
    //       //   } else if (newWeightJ <= 0) {
    //       //     objectINewVel = (objects[j]->velocity - objects[i]->velocity);
    //       //     objectJNewVel = objects[j]->velocity;
    //       //   }
    //       //   objectINewVel *= objects[i]->elasticity *
    //       objects[j]->elasticity;
    //       //   objectJNewVel *= objects[i]->elasticity *
    //       objects[j]->elasticity;
    //       //   objects[i]->velocity = objectINewVel;
    //       //   objects[j]->velocity = objectJNewVel;
    //       // }
    //       // objects[i]->touchOtherBody = true;
    //       // objects[j]->touchOtherBody = true;
    //     }
    //   }
    // }
  }
  void PhysicsEngine::init(std::vector<EngineObject *> objects) {
    std::vector<ComponentGameObject *> physObjects =
        findObject<ComponentGameObject>(objects, objectFlags::Physics);
    for (ComponentGameObject *obj : physObjects) {
      PhysicsBodyComponent *com =
          obj->findComponent<PhysicsBodyComponent>("[physics_body]");
      if (com) {
        if (com->weight <= 0) {
          this->physObjects.push_back(com);
          com->engineIncluded = true;
        }
      }
    }
    for (ComponentGameObject *obj : physObjects) {
      PhysicsBodyComponent *com =
          obj->findComponent<PhysicsBodyComponent>("[physics_body]");
      if (com) {
        if (com->weight > 0) {
          this->physObjects.push_back(com);
          com->engineIncluded = true;
        }
      } else {
        this->physObjects.push_back(com);
        com->engineIncluded = true;
      }
    }
  }

  bool hitbox_inside(v2 hitbox1Pos, HitBox *hitbox1, v2 hitbox2Pos,
                    HitBox *hitbox2, float rotation1, float rotation2) {
    return collision_SAT(hitbox1Pos, hitbox1, hitbox2Pos, hitbox2, rotation1,
                        rotation2);
  }

  bool oldIsHit(v2 hitbox1Pos, HitBox *hitbox1, v2 hitbox2Pos, HitBox *hitbox2,
                float rotation1, float rotation2) {
    v3 distance = hitbox2Pos - hitbox1Pos;
    bool hit = optimizedLineLength(v2(0, 0), distance.toV2()) <
              (hitbox1->maxDistance + hitbox2->maxDistance) *
                  (hitbox1->maxDistance + hitbox2->maxDistance);
    if (hit) {
      float angle = -radToDegree(fast_atan(distance.y / distance.x));
      if (distance.x < 0) angle -= 180;
      for (float a = 0; a < 135; a += 1) {
        hit = hitbox2->isInside(rotateAroundPointDegree(
            v2(0, 0),
            hitbox1->getPointAtAngleWithRotation(-angle + a, rotation1) +
                hitbox1Pos - hitbox2Pos,
            rotation2));
        if (hit) {
          return true;
        }
        hit = hitbox2->isInside(rotateAroundPointDegree(
            v2(0, 0),
            hitbox1->getPointAtAngleWithRotation(-angle - a, rotation1) +
                hitbox1Pos - hitbox2Pos,
            rotation2));
        if (hit) {
          return true;
        }
      }
    }
    return false;
  }

  bool veryOldIsHit(PhysicsBodyComponent *bod1, PhysicsBodyComponent *bod2) {
    bool hit = false;
    v3 distance = bod2->object->position - bod1->object->position;
    float angle = -radToDegree(fast_atan(distance.y / distance.x));
    if (distance.x < 0) {
      angle -= 180;
    }
    if (bod1->body->useSquareDistance || bod2->body->useSquareDistance) {
      float squareDistI = bod1->body->squareDistanceAtAngle(angle);
      float squareDistJ = bod2->body->squareDistanceAtAngle(-angle);
      hit =
          optimizedLineLength(v2(0, 0), distance) <
          squareDistI + squareDistJ + 2.0 * std::sqrt(squareDistI * squareDistJ);
    } else {
      float distI = bod1->body->distanceAtAngle(angle);
      float distJ = bod2->body->distanceAtAngle(-angle);
      hit =
          optimizedLineLength(v2(0, 0), distance) < std::pow((distI + distJ), 2);
    }
    return hit;
  }
  }  // namespace physics
}