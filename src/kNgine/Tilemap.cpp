#include <vector>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "SpriteUtils.hpp"
#include "../../extern/box2d/include/box2d/box2d.h"
#include "PhysicsB2.hpp"
#include "Tilemap.hpp"

namespace kNgine
{
  std::vector<b2Shape*>shapesList;
  Tilemap::~Tilemap(){}
  void Tilemap::addTileDef(Sprite spr)
  {
    if(nocollider){
      TileDef def=TileDef(spriteList,spr);
      def.tileType=TileDef::nocollider;
      def.friction=this->friction;
      def.dimensions=this->spriteDimensions;
      this->addTileDef(def);
    }else{
      TileDef def = TileDef(spriteList, spr);
      def.tileType = TileDef::rect;
      def.friction = this->friction;
      def.dimensions = this->spriteDimensions;
      this->addTileDef(def);
    }
  }
  void Tilemap::addTile(v2 pos, TileDef tileDef){
    u32 index=this->tileDefs.size();
    this->tiles.push_back({pos,index});
  }
  void Tilemap::addTile(v2 pos, u32 tileDefIndex){
    this->tiles.push_back({pos,tileDefIndex});
  }
  void Tilemap::init(std::vector<EngineObject *> objects){
    std::vector<b2FixtureDef>fixtures;
    std::vector<SpriteAccessor *> spritesRefs;
    for(u32 i=0;i<tiles.size();i++){
      if(tileDefs[tiles[i].index].tileType==TileDef::rect){
        b2FixtureDef def;
        b2PolygonShape *shape = new b2PolygonShape();
        shape->SetAsBox(tileDefs[tiles[i].index].dimensions.x / 2, tileDefs[tiles[i].index].dimensions.y / 2,b2Vec2(tiles[i].pos.x,tiles[i].pos.y),0.0f);
        shapesList.push_back(shape);
        def.shape=shape;
        def.friction=tileDefs[tiles[i].index].friction;
        def.density=1.0f;
        fixtures.push_back(def);
      }
      else if (tileDefs[tiles[i].index].tileType == TileDef::circle)
      {

      }
      else if (tileDefs[tiles[i].index].tileType == TileDef::points)
      {

      }
      SpriteReferenceComponent*refComp=new SpriteReferenceComponent(this, spriteList, tileDefs[tiles[i].index].spriteMapIndex);
      refComp->offset = { tiles[i].pos.x, tiles[i].pos.y};
      spritesRefs.push_back(refComp);
    }
    addComponent(new SpriteList(this,spritesRefs,spriteList));
    addComponent(new physics::b2PhysicsBodyComponent(this,fixtures));
  }
  void Tilemap::end(std::vector<EngineObject *> objects){
    for(u32 i=0;i<shapesList.size();i++){
      delete shapesList[i];
    }
  }
}
