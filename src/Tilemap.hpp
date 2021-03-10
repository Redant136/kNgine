#pragma once

#include <vector>
#include "utils.h"
#include "EngineObjects.hpp"
#include "SpriteUtils.hpp"

namespace kNgine{
  // must be added before phys engine
  class Tilemap:public ComponentGameObject{
  private:
    struct Tile{
      v2 pos;
      u32 index;
    };
  public:
    // bool preJoinSprites=false;// instead of rendering multiple sprites, should joins tiles into one sprite and should (to test) improves performance at the cost of ram.
    class TileDef{
    public:
      u32 spriteMapIndex;
      enum
      {
        nocollider,
        rect,
        circle,
        points
      } tileType=nocollider;
      f32 rectPoints[4][2];// from 0-1
      f32 friction = 1.0f;
      v2 dimensions={1.0f,1.0f};
      TileDef(SpriteMap*spriteList,Sprite sprite)
      {
        this->spriteMapIndex=spriteList->list.size();
        spriteList->list.push_back(sprite);
      }
      TileDef(u32 spriteMapIndex)
      {
        this->spriteMapIndex=spriteMapIndex;
      }
    };
    std::vector<TileDef> tileDefs;
    std::vector<Tile>tiles;
    bool nocollider=true;
    v2 spriteDimensions={1.0f,1.0f};
    f32 friction=1.0f;
    SpriteMap*spriteList;
    Tilemap(SpriteMap *spriteList)
    {
      this->spriteList=spriteList;
    }
    virtual ~Tilemap();
    void addTileDef(TileDef tileDef){tileDefs.push_back(tileDef);}
    void addTileDef(Sprite spr);
    void addTileDef(std::vector<Sprite>sprites){for(i32 i=0;i<sprites.size();i++){addTileDef(sprites[i]);}}
    void addTile(v2 pos, TileDef tileDef);
    void addTile(v2 pos,u32 tileDefIndex);
    virtual void init(std::vector<EngineObject*>objects);
    virtual void end(std::vector<EngineObject *> objects);
  };
}
