#define kNgine_DEBUG
#include <kNgine/kNgine.hpp>
#include <iostream>


class MainCharacter : public kNgine::ComponentGameObject{
  public:
  bool right=true;
  MainCharacter():ComponentGameObject(){
  }
  void update(std::vector<kNgine::msg>msgs){
    ComponentGameObject::update(msgs);
    kNgine::physics::b2PhysicsBodyComponent* phys=findComponent<kNgine::physics::b2PhysicsBodyComponent>("[b2_physics_body]");
    f32 time;
    v2 newVel = {0, 0};
    v2 mousePos;
    for (kNgine::msg m : msgs)
    {
      if (m.msgType == kNgine::msg::ASCII_KEY)
      {
        if (m.key & KeyBitmap(KEY_D))
        {
          newVel.x += 1;
        }
        if (m.key & KeyBitmap(KEY_A))
        {
          newVel.x -= 1;
        }
        if (m.key & KeyBitmap(KEY_W))
        {
          newVel.y += 1;
        }
        if (m.key & KeyBitmap(KEY_S))
        {
          newVel.y -= 1;
        }
        if (m.key & KeyBitmap(KEY_SPACE))
        {
          newVel.y+=5;
        }
        if (m.key & KeyBitmap(KEY_P))
        {
          std::cout<<"position"<<std::endl;
          std::cout << position.x << " " << position.y << std::endl;
        }
        if (m.key & KeyBitmap(KEY_C))
        {
          findComponent<kNgine::SpriteAnimationSystem>("[sprite]")->setActive("chungus");
        }
        if (m.key & KeyBitmap(KEY_Q))
        {
          findComponent<kNgine::SpriteAnimationSystem>("[sprite]")->setActive("walk");
        }
      }
      else if(m.msgType==kNgine::msg::NONASCII_KEY)
      {
        if (m.key == KeyBitmap(MOUSE_BUTTON1))
        {
          std::cout << "left mouse" << std::endl;
          std::cout << mousePos.x << " " << mousePos.y << std::endl;
        }
      }
      else if(m.msgType==kNgine::msg::CURSOR){
        mousePos = m.cursorPos;
      }else if(m.msgType==kNgine::msg::TIME_ELAPSED){
        time=m.time;
      }
    }
    newVel=V2MultiplyF32(newVel,time);
    newVel = V2MultiplyF32(newVel, 5);
    phys->addVelocity(newVel);
    if(phys->getVelocity().x<-0.1&&right){
      findComponent<kNgine::SpriteAnimationSystem>("[sprite]")->spriteDimension.x*=-1;
      right=false;
    }
    else if (phys->getVelocity().x > 0.1 && !right){
      findComponent<kNgine::SpriteAnimationSystem>("[sprite]")->spriteDimension.x *= -1;
      right = true;
    }
  }
};

int main()
{
  // kNgine::engine engine = kNgine::engine();
  kNgine::window_name="test";
  kNgine::DEBUG=true;
  kNgine::SpriteMap *sprMap = new kNgine::SpriteMap();
  kNgine::physics::b2PhysicsEngine *physEngine = new kNgine::physics::b2PhysicsEngine({0, -5});

  { //main character
    MainCharacter *mc = new MainCharacter();

    mc->addComponent(new kNgine::physics::b2Rect(mc, 1, 1, 1, 0.5f));

    kNgine::SpriteMap*mcSprites=new kNgine::SpriteMap();
    kNgine::SpriteAnimationSystem *animations = new kNgine::SpriteAnimationSystem(mc, mcSprites);

    kNgine::SpriteAnimation *adventurer = new kNgine::SpriteAnimation(NULL, mcSprites,
                                                                      kNgine::importSpriteSheet("games_sprites/Adventurer/Adventurer-1.5/adventurer-v1.5-Sheet.png", 50, 37), 1.0 / 15);
    mcSprites->offsetPixelsInSprites(NORTH,1);
    kNgine::SpriteAnimation* walk_animation=new kNgine::SpriteAnimation(NULL,mcSprites,kNgine::importSpriteSheet("walking_animation.png",64,64),1.0/15);

    animations->addSprite(adventurer,"adventurer");
    animations->addSprite(walk_animation,"walk");
    mc->addComponent(animations);


    kNgine::SoundListenerComponent *sound = new kNgine::SoundListenerComponent(mc);
    mc->addComponent(sound);
  
    kNgine::Camera* cam = new kNgine::Camera(20, 1920, 1080);
    cam->position.z = 10;
    mc->addComponent(new kNgine::NodeObjectComponent(mc,cam));
    kNgine::addObject(mcSprites);
    kNgine::addObject(mc);
  }
  
  // kNgine::Camera *cam = new kNgine::Camera(20, 1920, 1080);
  // cam->position.z = 10;
  // kNgine::addObject(cam);

  { //floor
    kNgine::ComponentGameObject *floor = new kNgine::ComponentGameObject();
    floor->position = v3(0, -5,0);

    kNgine::physics::b2PhysicsBodyComponent *floorPhys = new kNgine::physics::b2Rect(floor, 20, 1, 50, 0.5f);
    floorPhys->setStatic(false);
    floor->addComponent(floorPhys);

    kNgine::SpriteComponent *floorSpr = new kNgine::SpriteComponent(floor, kNgine::fillSprite(200, 200, {0, 255, 0, 255}));
    floorSpr->spriteDimension = v2(20, 1);
    floor->addComponent(floorSpr);

    kNgine::addObject(floor);
  }
  
  kNgine::SpriteMap*tilemapSpr=new kNgine::SpriteMap();
  kNgine::Tilemap *tilemap = new kNgine::Tilemap(tilemapSpr);
  tilemap->addTileDef(kNgine::importSpriteSheet("brick_tiles.png", 32, 32));
  for(int i=0;i<20;i++){
    tilemap->addTile({i-9.5f, -5}, 13);
  }
  kNgine::addObject(tilemap);
  kNgine::addObject(tilemapSpr);

  // kNgine::addEvent({"test",&engine.workingObjects,[](std::vector<kNgine::EngineObject *>objects){
  //   std::cout<<"yes"<<std::endl;
  //   std::cout<<objects.size()<<std::endl;
  // }});

  kNgine::AudioEngine*audio=new kNgine::AudioEngine();
  kNgine::addObject(audio);
  audio->play("test.wav", kNgine::audiofiletype::wav);

  kNgine::addObject(sprMap);
  kNgine::addObject(physEngine);

  kNgine::start();
  kNgine::cleanup();
  return 0;
}