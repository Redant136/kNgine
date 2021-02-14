#pragma once

#include <vector>
#include "utils.hpp"
#include "EngineObjects.hpp"

namespace kNgine{

  namespace physics {
  #ifdef engine_2D
  class HitBox  // centered around 0,0. Extend to create specific HitBox
  {
  public:
    bool useSquareDistance = false;
    float maxDistance = 0;
    virtual ~HitBox();
    virtual void setMaxDistance();
    virtual bool isInside(v2 point);
    virtual float squareDistanceAtAngle(float angle);
    // take angle in degrees
    virtual float distanceAtAngle(float angle);
    virtual v2 getPointAtAngle(float angle);
    virtual v2 getPointAtAngleWithRotation(float angle,float rotation);
    virtual std::vector<v2> getCorners();
  };

  class rectHitBox final : public HitBox {
  public:
    float width, height;
    rectHitBox();
    rectHitBox(float width, float height);
    ~rectHitBox();
    void setMaxDistance();
    bool isInside(v2 point);
    float squareDistanceAtAngle(float angle);
    float distanceAtAngle(float angle);
    std::vector<v2> getCorners();
  };

  class circularHitBox final : public HitBox {
  public:
    float radius;
    circularHitBox();
    circularHitBox(float radius);
    ~circularHitBox();
    bool isInside(v2 point);
    float squareDistanceAtAngle(float angle);
    float distanceAtAngle(float angle);
    // std::vector<v2> getCorners();
  };

  class HitBoxComponent : public ObjectComponent {
    bool actived;
    HitBox *hitbox;
    v2 positionOffset;
    // TODO called when hitbox entered
    void (*onEntered)();
    HitBoxComponent(ComponentGameObject *base);
    void update(std::vector<msg> msgs);
  };

  //[physics_body]
  class PhysicsBodyComponent : public ObjectComponent {
  public:
    v2 velocity=v2(), acceleration=v2();
    //will cause physics problems, didn't implement rotation speed transfer and will not
    v3 rotationVel=v3(),rotationAcc=v3();
    v3 lastPosition;
    HitBox *body;
    float weight;
    float elasticity;
    bool touchOtherBody = false;
    bool engineIncluded = false;
    PhysicsBodyComponent(ComponentGameObject *base);
    PhysicsBodyComponent(HitBox *body, ComponentGameObject *base);
    PhysicsBodyComponent(HitBox *body, float weight, ComponentGameObject *base);
    PhysicsBodyComponent(HitBox *body, float weight, float elasticity,
                        ComponentGameObject *base);
    void update(std::vector<msg> msgs);
  };

  // does not support concave shapes
  class PhysicsEngine : public EngineObject {
  public:
    std::vector<PhysicsBodyComponent *> physObjects;
    PhysicsEngine();
    virtual void init(std::vector<EngineObject *> objects);
    virtual void update(std::vector<msg> msgs);
  };

  bool hitbox_inside(v2 hitbox1Pos, HitBox *hitbox1, v2 hitbox2Pos,
                    HitBox *hitbox2,float angle1,float angle2);
  #endif

  }  // namespace physics
}