#include <vector>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "SpriteUtils.hpp"
// #include "../../extern/box2d/include/box2d/box2d.h"
#include "PhysicsChipmunk.hpp"
#include "Tilemap.hpp"

namespace kNgine
{
  // std::vector<b2Shape*>shapesList;

  ComponentGameObject *Tilemap::getTileFromDef(TileDef def, v3 pos)
  {
    ComponentGameObject *tile = new ComponentGameObject();
    if (def.tileType != Tilemap::TileDef::multi)
    {
      if (def.tileType == Tilemap::TileDef::nocollider)
      {
        SpriteReferenceComponent *accessor = new SpriteReferenceComponent(tile, spriteMap, def.single.spriteMapIndex);
        accessor->spriteDimension=def.size;
        tile->addComponent(new RendererObject(tile, accessor));
      }
    }
    else
    {
    }
    tile->position=pos;
    return tile;
  }

  void Tilemap::addTileDef(Sprite spr)
  {
    if(nocollider){
      TileDef def=TileDef(spriteMap->list.size(),TileDef::nocollider);
      this->addTileDef(def);
    }else{
      TileDef def = TileDef(spriteMap->list.size(),TileDef::rect);
      this->addTileDef(def);
    }
    spriteMap->list.push_back(spr);
  }
  void Tilemap::addTile(v3 pos, TileDef tileDef)
  {
    u32 index=this->tileDefs.size();
    addTileDef(tileDef);
    addTile(pos, index);
  }
  void Tilemap::addTile(v3 pos, u32 tileDefIndex){
    this->tiles.push_back({pos,tileDefIndex});
    addTile({pos, tileDefIndex});
  }
  void Tilemap::addTile(Tile tile)
  {
    ComponentGameObject*obj=getTileFromDef(tileDefs[tile.index],tile.pos);
    addComponent(new NodeObjectComponent(this,obj));
  }
  void Tilemap::init(std::vector<EngineObject *> objects)
  {
    if(privateSpriteMap){
      spriteMap->init(objects);
    }
    for(u32 i=0;i<tiles.size();i++){
      TileDef def=tileDefs[tiles[i].index];
      ComponentGameObject *tile=new ComponentGameObject();
      // tile->addComponent(new SpriteReferenceComponent(tile, spriteList, def.spriteMapIndex));
      addComponent(new NodeObjectComponent(this,tile));
    }
    // std::vector<b2FixtureDef>fixtures;
    // std::vector<SpriteAccessor *> spritesRefs;
    // for(u32 i=0;i<tiles.size();i++){
    //   if(tileDefs[tiles[i].index].tileType==TileDef::rect){
        // b2FixtureDef def;
        // b2PolygonShape *shape = new b2PolygonShape();
        // shape->SetAsBox(tileDefs[tiles[i].index].dimensions.x / 2, tileDefs[tiles[i].index].dimensions.y / 2,b2Vec2(tiles[i].pos.x,tiles[i].pos.y),0.0f);
        // shapesList.push_back(shape);
        // def.shape=shape;
        // def.friction=tileDefs[tiles[i].index].friction;
        // def.density=1.0f;
        // fixtures.push_back(def);
    //   }
    //   else if (tileDefs[tiles[i].index].tileType == TileDef::circle)
    //   {

    //   }
    //   else if (tileDefs[tiles[i].index].tileType == TileDef::points)
    //   {

    //   }
    //   SpriteReferenceComponent*refComp=new SpriteReferenceComponent(this, spriteList, tileDefs[tiles[i].index].spriteMapIndex);
    //   refComp->offset = { tiles[i].pos.x, tiles[i].pos.y};
    //   spritesRefs.push_back(refComp);
    // }
    // addComponent(new SpriteList(this,spritesRefs,spriteList));
    // addComponent(new physics::b2PhysicsBodyComponent(this,fixtures));
  }
  void Tilemap::load()
  {
    if (privateSpriteMap)
    {
      spriteMap->load();
    }
  }
  void Tilemap::unload()
  {
    if (privateSpriteMap)
    {
      spriteMap->unload();
    }
  }
  void Tilemap::end(std::vector<EngineObject *> objects){
    if (privateSpriteMap)
    {
      spriteMap->end(objects);
    }

    ComponentGameObject::end(objects);
    // for(u32 i=0;i<shapesList.size();i++){
    //   delete shapesList[i];
    // }
  }
}
