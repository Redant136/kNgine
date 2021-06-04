#pragma once

#include "SpriteUtils.hpp"

namespace kNgine{
  // must be added before phys engine
  class Tilemap:public ComponentGameObject{
  private:
  public:
    SpriteMap *spriteMap;
    const bool privateSpriteMap;
    
    struct Tile
    {
      v3 pos;
      u32 index;
    };
    struct TileDef
    {
      enum TileDefType
      {
        // data for physics component generation
        nocollider,
        rect,
        circle,
        polygon,
        multi // if def is composed of multiple def
      } tileType=nocollider;
      v2 size = v2(1.f,1.f);
      union{
        struct
        {
          u32 spriteMapIndex;
          size_t points_length;
          v2* polyPoints = NULL;// from 0-1
          f32 friction = 1.0f;
        } single;

        std::vector<Tile>tiles;
      };
      TileDef(u32 spriteMapIndex, TileDefType type = nocollider)
        {
          assert(type != TileDefType::multi);
          tileType = type;
          single.spriteMapIndex = spriteMapIndex;
          if (type == rect)
          {
            single.points_length = 4;
            single.polyPoints = new v2[4];
            v2 temp[4] = {v2(0, 0), v2(1, 0), v2(0, 1), v2(1, 1)};
            memcpy(single.polyPoints, temp, sizeof(v2) * 4);
          }
      }
      TileDef(u32 spriteMapIndex,size_t points_length,v2*polyPoints,f32 friction=1)
      {
        tileType=polygon;
        this->single.points_length=points_length;
        this->single.polyPoints=new v2[points_length];
        memcpy(this->single.polyPoints,polyPoints,sizeof(v2)*points_length);
      }
      TileDef(std::vector<Tile>tiles)
      {
        this->tileType=multi;
        this->tiles=tiles;
      }
      TileDef(const TileDef&base)
      {
        this->tileType=base.tileType;
        if(this->tileType!=TileDefType::multi){
          this->single = base.single;
        }
        else
        {
          this->tiles = base.tiles;
        }
      }
      ~TileDef(){}
      void free()
      {
        if (this->tileType != TileDefType::multi)
        {
          delete[] single.polyPoints;
        }
      }
    };

  private:
    ComponentGameObject *getTileFromDef(TileDef def, v3 pos);
  public:
    std::vector<TileDef> tileDefs;
    std::vector<Tile>tiles;
    bool nocollider=true;
    Tilemap() : privateSpriteMap(true), spriteMap(new SpriteMap())
    {
    }
    Tilemap(SpriteMap *spriteList):privateSpriteMap(false),spriteMap(spriteList)
    {
    }
    virtual ~Tilemap()
    {
      if(privateSpriteMap)
      {
        delete spriteMap;
      }
    }
    void addTileDef(TileDef tileDef){tileDefs.push_back(tileDef);}
    void addTileDef(Sprite spr);
    void addTileDef(std::vector<Sprite>sprites){for(u32 i=0;i<sprites.size();i++){addTileDef(sprites[i]);}}
    void addTile(v3 pos, TileDef tileDef);
    void addTile(v3 pos, u32 tileDefIndex);
    void addTile(Tile tile);
    virtual void init(std::vector<EngineObject*>objects);
    virtual void load();
    virtual void unload();
    virtual void end(std::vector<EngineObject *> objects);
  };
}
