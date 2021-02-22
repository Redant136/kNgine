#pragma once

#include <vector>
#include "utils.hpp"
#include "EngineObjects.hpp"
#include "SpriteUtils.hpp"

#include <iostream>

namespace kNgine{
  /* either object that simply serves as container for multiple pieces of ground and 
    just shortens code writen to create multiple similar objects
    or
    tilemap that only holds a single copy of everything and improves performance
  */

  // must be added before phys engine
  class Tilemap:public ComponentGameObject{
  private:
    struct Tile{
      v2 pos;
      unsigned int index;
    };
  public:
    // bool preJoinSprites=false;// instead of rendering multiple sprites, should joins tiles into one sprite and should (to test) improves performance at the cost of ram.
    class TileDef{
    public:
      unsigned int spriteMapIndex;
      enum
      {
        nocollider,
        rect,
        circle,
        points
      } tileType=nocollider;
      float rectPoints[4][2];// from 0-1
      float friction = 1.0f;
      v2 dimensions={1.0f,1.0f};
      TileDef(SpriteMap*spriteList,Sprite sprite)
      {
        this->spriteMapIndex=spriteList->list.size();
        spriteList->list.push_back(sprite);
      }
      TileDef(unsigned int spriteMapIndex)
      {
        this->spriteMapIndex=spriteMapIndex;
      }
    };
    std::vector<TileDef> tileDefs;
    std::vector<Tile>tiles;
    bool nocollider=true;
    v2 spriteDimensions={1.0f,1.0f};
    float friction=1.0f;
    SpriteMap*spriteList;
    Tilemap(SpriteMap *spriteList)
    {
      this->spriteList=spriteList;
    }
    virtual ~Tilemap();
    void addTileDef(TileDef tileDef){tileDefs.push_back(tileDef);}
    void addTileDef(Sprite spr);
    void addTileDef(std::vector<Sprite>sprites){for(int i=0;i<sprites.size();i++){addTileDef(sprites[i]);}}
    void addTile(v2 pos, TileDef tileDef);
    void addTile(v2 pos,unsigned int tileDefIndex);
    virtual void init(std::vector<EngineObject*>objects);
    virtual void end(std::vector<EngineObject *> objects);
  };
}
