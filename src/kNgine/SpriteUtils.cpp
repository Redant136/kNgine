#include <vector>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "../kFramework/kRenderer.h"
#include "SpriteUtils.hpp"
#include "../../extern/stb/stb_image.h"

namespace kNgine
{
  SpriteMap::SpriteMap() {}
  SpriteMap::~SpriteMap()
  {
    for (u32 i = 0; i < list.size(); i++)
    {
      freeSprite(list[i]);
    }
  }
  void SpriteMap::init(std::vector<EngineObject *> objects)
  {
  }
  void SpriteMap::end(std::vector<EngineObject *> objects)
  {
  }
  void SpriteMap::load(std::vector<EngineObject *> objects)
  {
    texIndex = std::vector<u32>();
    for (u32 i = 0; i < list.size(); i++)
    {
      u32 texture, VBO;
      kRenderer_bindTexture(&texture, list[i].buffer, list[i].width, list[i].height, list[i].numChannels);
      texIndex.push_back(texture);
    }
  }
  void SpriteMap::unload(std::vector<EngineObject *> objects)
  {
    for (u32 i = 0; i < list.size(); i++)
    {
      kRenderer_unbindTexture(texIndex[i]);
    }
  }
  void SpriteMap::offsetPixelsInSprites(cardinal8dir dir, u32 offset)
  {
    for(u32 i=0;i<list.size();i++){
      offsetPixelsInSprite(&list[i],dir,offset);
    }
  }

  SpriteMapAccessor::SpriteMapAccessor(ComponentGameObject *base) : SpriteAccessor(base)
  {
  }

  SpriteReferenceComponent::SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList,
                                                     i32 index)
      : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(1, 1);
    this->mapIndex = index;
    this->spriteList = spriteList;
  }
  SpriteReferenceComponent::SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList,
                                                     Sprite sprite)
      : SpriteMapAccessor(base)
  {
    this->spriteList = spriteList;
    this->mapIndex = spriteList->list.size();
    this->spriteList->list.push_back(sprite);
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(1, 1);
  }
  SpriteReferenceComponent::SpriteReferenceComponent(const SpriteComponent &base, SpriteMap *spriteList) : SpriteMapAccessor(base.object)
  {
    this->spriteList = spriteList;
    this->mapIndex = spriteList->list.size();
    this->spriteList->list.push_back(base.sprite);
    this->spriteLocation = base.spriteLocation;
    this->spriteDimension = base.spriteDimension;
  }
  SpriteReferenceComponent::SpriteReferenceComponent(const SpriteReferenceComponent &base)
      : SpriteMapAccessor(base.object)
  {
    this->spriteList = base.spriteList;
    this->mapIndex = base.mapIndex;
    this->spriteLocation = base.spriteLocation;
    this->spriteDimension = base.spriteDimension;
    this->label = base.label;
  }
  SpriteReferenceComponent::~SpriteReferenceComponent() {}
  void SpriteReferenceComponent::update(std::vector<msg> msgs) {}
  u32 SpriteReferenceComponent::getMapIndex() { return mapIndex; }
  Sprite *SpriteReferenceComponent::getSprite()
  {
    return &(spriteList->list[mapIndex]);
  }
  v2 SpriteReferenceComponent::getSpriteDimensions() { return spriteDimension; }

  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32> indexes,
                                   f32 frameLength, v2 spriteDimension) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spritesIndexes = indexes;
    this->spriteDimensions = std::vector<v2>(indexes.size());
    for (u32 i = 0; i < indexes.size(); i++)
    {
      this->spriteDimensions[i] = spriteDimension;
    }
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                                   f32 frameLength, v2 spriteDimension) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spritesIndexes = std::vector<u32>(sprites.size());
    this->spriteDimensions = std::vector<v2>(sprites.size());
    for (u32 i = 0; i < sprites.size(); i++)
    {
      this->spritesIndexes[i] = spriteList->list.size();
      this->spriteDimensions[i] = spriteDimension;
      this->spriteList->list.push_back(sprites[i]);
    }
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32> indexes,
                                   f32 frameLength, std::vector<v2> spriteDimensions) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spritesIndexes = indexes;
    this->spriteDimensions = spriteDimensions;
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                                   f32 frameLength, std::vector<v2> spriteDimensions) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spriteDimensions = spriteDimensions;
    for (u32 i = 0; i < sprites.size(); i++)
    {
      this->spritesIndexes[i] = this->spriteList->list.size();
      this->spriteList->list.push_back(sprites[i]);
    }
  }
  SpriteAnimation::SpriteAnimation(const SpriteAnimation &base) : SpriteMapAccessor(base)
  {
    this->spriteLocation = base.spriteLocation;
    this->spriteList = base.spriteList;
    this->frame = base.frame;
    this->frameLength = base.frameLength;
    this->spriteDimensions = base.spriteDimensions;
    this->spritesIndexes = base.spritesIndexes;
  }
  SpriteAnimation::~SpriteAnimation()
  {
  }
  void SpriteAnimation::update(std::vector<msg> msgs)
  {
    f32 timeElapsed = 0;
    for (u32 i = 0; i < msgs.size(); i++)
    {
      if (msgs[i].msgType == msg::TIME_ELAPSED)
      {
        timeElapsed = msgs[i].time;
        break;
      }
    }
    timeUntilNextFrame += timeElapsed;
    if (timeUntilNextFrame >= frameLength)
    {
      timeUntilNextFrame = 0;
      frame++;
      if (frame >= spritesIndexes.size())
      {
        frame = 0;
      }
    }
  }
  u32 SpriteAnimation::getMapIndex() { return spritesIndexes[frame]; }
  Sprite *SpriteAnimation::getSprite()
  {
    return &(spriteList->list[spritesIndexes[frame]]);
  }
  v2 SpriteAnimation::getSpriteDimensions()
  {
    return spriteDimensions[frame];
  }

  SpriteRendererObject_base::SpriteRendererObject_base(ComponentGameObject *base, SpriteMap *spriteList) : SpriteMapAccessor(base)
  {
    this->label="[RendererObject]";
    this->specialAccessor = true;
    this->spriteList = spriteList;
    this->spriteIndexes = std::vector<std::vector<u32>>();
  }
  SpriteRendererObject_base::SpriteRendererObject_base(ComponentGameObject *base,SpriteMap *spriteList, size_t numShaders,
    u32 *shaderIndex, size_t *numTriangles, void ***points[3]) : SpriteMapAccessor(base)
  {
    this->label = "[RendererObject]";
    this->specialAccessor = true;
    this->spriteList = spriteList;
    this->spriteIndexes = std::vector<std::vector<u32>>();
    kRenderer_RendererObject obj;
    obj.length=numShaders;
    for(u32 i=0;i<numShaders;i++){
      obj.shaderElements[i].shadersIndex=shaderIndex[i];
      obj.shaderElements[i].length=numTriangles[i];
      for(u32 j=0;j<numTriangles[i];j++){
        obj.shaderElements[i].triangles[j].arg[0] = points[i][j][0];
        obj.shaderElements[i].triangles[j].arg[1] = points[i][j][1];
        obj.shaderElements[i].triangles[j].arg[2] = points[i][j][2];
      }
    }
    kRenderer_bindObject(&rendererObjectIndex,obj);
    this->spriteIndexes = std::vector<std::vector<u32>>();
  }
  SpriteRendererObject_base::SpriteRendererObject_base(ComponentGameObject *base, SpriteMap *spriteList, size_t numShaders,
                                                       u32 *shaderIndex, size_t *numTriangles, void ***points[3], std::vector<std::vector<u32>> spriteIndexes)
      : SpriteRendererObject_base(base, spriteList, numShaders, shaderIndex, numTriangles, points)
  {
    this->spriteIndexes = spriteIndexes;
  }
  SpriteRendererObject_base::SpriteRendererObject_base(ComponentGameObject *base, SpriteMap *spriteList, size_t numShaders,
                                                       u32 *shaderIndex, size_t *numTriangles, void ***points[3], std::vector<std::vector<Sprite>> sprites)
      : SpriteRendererObject_base(base, spriteList, numShaders, shaderIndex, numTriangles, points)
  {
    this->spriteIndexes = std::vector<std::vector<u32>>(sprites.size());
    for(u32 i=0;i<sprites.size();i++){
      this->spriteIndexes[i] = std::vector<u32>(sprites[i].size());
      for(u32 j=0;j<sprites[i].size();j++){
        this->spriteIndexes[i][j] = this->spriteList->list.size();
        this->spriteList->list.push_back(sprites[i][j]);
      }
    }
  }

  SpriteRendererObject::SpriteRendererObject(ComponentGameObject *base, SpriteMap *spriteList, u32 spriteIndex):SpriteRendererObject_base(base,spriteList)
  {
    this->spriteIndexes=std::vector<std::vector<u32>>(1);
    spriteIndexes[0]=std::vector<u32>(1);
    spriteIndexes[0][0]=spriteIndex;

    kRenderer_RendererObject object =
        {1, // num shaders
         {
             0, // shader index
             2, // num triangles
             {  // triangles
              // triangle obj 1
              {
                  {&default_points[0], &default_points[1], &default_points[3]}, // corners
                  {false, false, false}                                         // value updated
              },
              // triangle obj 2
              {
                  {&default_points[0], &default_points[2], &default_points[3]}, // corners
                  {false, false, false}                                         // value updated
              }}}};

    default_points[0].pos = base->position;
    default_points[1].pos = base->position;
    default_points[2].pos = base->position;
    default_points[3].pos = base->position;

    default_points[0].isTex = 1;
    default_points[1].isTex = 1;
    default_points[2].isTex = 1;
    default_points[3].isTex = 1;

    default_points[0].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);;
    default_points[1].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);;
    default_points[2].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);;
    default_points[3].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);;

    kRenderer_bindObject(&rendererObjectIndex,object);
  }
  SpriteRendererObject::SpriteRendererObject(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite):SpriteRendererObject_base(base,spriteList)
  {
    this->spriteIndexes = std::vector<std::vector<u32>>(1);
    spriteIndexes[0] = std::vector<u32>(1);
    spriteIndexes[0][0] = spriteList->list.size();
    spriteList->list.push_back(sprite);

    kRenderer_RendererObject object =
        {1, // num shaders
         {
             0, // shader index
             2, // num triangles
             {  // triangles
              // triangle obj 1
              {
                  {&default_points[0], &default_points[1], &default_points[3]}, // corners
                  {false, false, false}                                         // value updated
              },
              // triangle obj 2
              {
                  {&default_points[0], &default_points[2], &default_points[3]}, // corners
                  {false, false, false}                                         // value updated
              }}}};

    default_points[0].pos = base->position;
    default_points[1].pos = base->position;
    default_points[2].pos = base->position;
    default_points[3].pos = base->position;

    default_points[0].isTex = 1;
    default_points[1].isTex = 1;
    default_points[2].isTex = 1;
    default_points[3].isTex = 1;

    default_points[0].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);;
    default_points[1].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);;
    default_points[2].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);;
    default_points[3].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);;

    kRenderer_bindObject(&rendererObjectIndex, object);
  }
  void SpriteRendererObject::updatePoints(m4 matrix){
    v2 dimensions = toV2(V4MultiplyM4(v4(object->position.x, object->position.y, object->position.z, 1)+ v4(getSpriteDimensions().x, getSpriteDimensions().y, 0, 1), matrix))- toV2(V4MultiplyM4(v4(object->position.x, object->position.y, object->position.z, 1), matrix));
    v2 spriteOffset = v2(getSpriteLocation().x*dimensions.x,getSpriteLocation().y*dimensions.y);
    kRenderer_RendererObject*obj= kRenderer_getBoundObject(rendererObjectIndex);
    obj->shaderElements[0].triangles[0].valueUpdated[0] = true;
    obj->shaderElements[0].triangles[1].valueUpdated[0] = true;

    default_points[0].pos = object->position+ v3(spriteOffset.x, spriteOffset.y, 0);
    default_points[1].pos = object->position+ v3(spriteOffset.x, spriteOffset.y, 0)+ v3(dimensions.x, 0, 0);
    default_points[2].pos = object->position+ v3(spriteOffset.x, spriteOffset.y, 0)+ v3(0, dimensions.y, 0);
    default_points[3].pos = object->position+ v3(spriteOffset.x, spriteOffset.y, 0)+ v3(dimensions.x, dimensions.y, 0);
    kRenderer_updateObject(rendererObjectIndex);
  }
  std::vector<Sprite> importSpriteSheet(const char *filename, i32 spriteWidth,
                                        i32 spriteHeight)
  {
    i32 width, height, numChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);

    i32 numSpritesHor = width / spriteWidth;
    i32 numSpritesVert = height / spriteHeight;
    std::vector<Sprite> sprites = std::vector<Sprite>();
    for (u32 i = 0; i < numSpritesVert; i++)
    {
      for (u32 j = 0; j < numSpritesHor; j++)
      {
        u8 *sprite = new u8[spriteWidth * spriteHeight * numChannels];
        for (i32 y = 0; y < spriteHeight; y++)
        {
          for (i32 x = 0; x < spriteWidth; x++)
          {
            for (i32 c = 0; c < numChannels; c++)
            {
              sprite[(x + y * spriteWidth) * numChannels + c] = data[((x + j * spriteWidth) + (y + i * spriteHeight) * width) * numChannels + c];
            }
          }
        }
        sprites.push_back(Sprite(spriteWidth, spriteHeight, numChannels, sprite));
      }
    }
    return sprites;
  }
} // namespace kNgine
