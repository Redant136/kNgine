#include <vector>
#include "utils.hpp"
#include "EngineObjects.hpp"
#include "SpriteUtils.hpp"
#include "../extern/box2d/include/box2d/box2d.h"
#include "PhysicsB2.hpp"
#include "Tilemap.hpp"

namespace kNgine
{
  std::vector<b2Shape*>shapesList;
  void Tileset::addTileDef(Sprite spr)
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
  void Tileset::addTile(v2 pos, TileDef tileDef){
    unsigned int index=this->tileDefs.size();
    this->tiles.push_back({pos,index});
  }
  void Tileset::addTile(v2 pos, unsigned int tileDefIndex){
    this->tiles.push_back({pos,tileDefIndex});
  }
  void Tileset::init(std::vector<EngineObject *> objects){
    std::vector<b2FixtureDef>fixtures;
    std::vector<SpriteAccessor *> spritesRefs;
    for(int i=0;i<tiles.size();i++){
      if(tileDefs[tiles[i].index].tileType==TileDef::rect){
        b2FixtureDef def;
        b2PolygonShape *shape = new b2PolygonShape();
        spriteList->list[tileDefs[tiles[i].index].spriteMapIndex].width;
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
      spritesRefs.push_back(new SpriteReferenceComponent(this, spriteList, tileDefs[tiles[i].index].spriteMapIndex));
    }
    addComponent(new SpriteList(this,spritesRefs,spriteList));
    addComponent(new physics::b2PhysicsBodyComponent(this,fixtures));
  }
  void Tileset::end(std::vector<EngineObject *> objects){
    for(int i=0;i<shapesList.size();i++){
      delete shapesList[i];
    }
  }
}
