#include "SpriteUtils.hpp"
#include "kRenderer.h"
#include <stb_image.h>

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
  void SpriteMap::load()
  {
    texIndex = std::vector<u32>();
    for (u32 i = 0; i < list.size(); i++)
    {
      u32 texture;
      kRenderer_bindTexture(&texture, list[i].buffer, list[i].width, list[i].height, list[i].numChannels);
      texIndex.push_back(texture);
    }
  }
  void SpriteMap::unload()
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
    this->spriteList = NULL;
  }

  SpriteReferenceComponent::SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList,
                                                     u32 index)
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

  RendererObject_base::RendererObject_base(ComponentGameObject *base) : Renderable(base)
  {
    this->label="[RendererObject]";
    this->specialAccessor = true;
  }
  RendererObject_base::RendererObject_base(ComponentGameObject *base, size_t numShaders,
    u32 *shaderIndex, size_t *numTriangles, f32 ***points[3]) : Renderable(base)
  {
    this->label = "[RendererObject]";
    this->specialAccessor = true;
    kRenderer_RendererObject obj;
    obj.length=numShaders;
    for(u32 i=0;i<numShaders;i++){
      obj.shaderElements[i].shadersIndex=shaderIndex[i];
      obj.shaderElements[i].length=numTriangles[i];
      obj.shaderElements[i].triangles=new kRenderer_triangleInfo[numTriangles[i]];
      for(u32 j=0;j<numTriangles[i];j++){
        obj.shaderElements[i].triangles[j].arg[0] = points[i][j][0];
        obj.shaderElements[i].triangles[j].arg[1] = points[i][j][1];
        obj.shaderElements[i].triangles[j].arg[2] = points[i][j][2];
      }
    }
    kRenderer_addObject(&rendererObjectIndex,obj);
  }
  void RendererObject_base::load(){
    kRenderer_loadObject(rendererObjectIndex);
  }
  void RendererObject_base::unload(){
    kRenderer_unloadObject(rendererObjectIndex);
  }

  RendererObject::RendererObject(ComponentGameObject *base, SpriteMapAccessor *renderable) : RendererObject_base(base)
  {
    this->label = "[RendererObject]";
    this->specialAccessor = true;
    this->renderable = renderable;

    kRenderer_RendererObject object;
    object.length = 1;
    object.shaderElements[0].shadersIndex = 0;
    object.shaderElements[0].length = 2;
    object.shaderElements[0].triangles = new kRenderer_triangleInfo[2];
    object.shaderElements[0].triangles[0].arg[0] = (f32 *)&default_points[0];
    object.shaderElements[0].triangles[0].arg[1] = (f32 *)&default_points[1];
    object.shaderElements[0].triangles[0].arg[2] = (f32 *)&default_points[3];
    object.shaderElements[0].triangles[1].arg[0] = (f32 *)&default_points[0];
    object.shaderElements[0].triangles[1].arg[1] = (f32 *)&default_points[2];
    object.shaderElements[0].triangles[1].arg[2] = (f32 *)&default_points[3];

    default_points[0].pos = v3(-0.5, 0.5, 0);
    default_points[1].pos = v3(0.5, 0.5, 0);
    default_points[2].pos = v3(-0.5, -0.5, 0);
    default_points[3].pos = v3(0.5, -0.5, 0);

    default_points[0].isTex = 1;
    default_points[1].isTex = 1;
    default_points[2].isTex = 1;
    default_points[3].isTex = 1;

    default_points[0].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);
    default_points[1].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);
    default_points[2].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);
    default_points[3].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);

    kRenderer_addObject(&rendererObjectIndex, object);
  }
  RendererObject::RendererObject(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite) : RendererObject(base, new SpriteReferenceComponent(base,spriteList,sprite))
  {
  }
  void RendererObject::updatePoints(m4 matrix)  
  {
    v2 spriteDimensions=renderable->getSpriteDimensions();
    v2 dimensions = toV2(V4MultiplyM4(v4(object->position.x, object->position.y, object->position.z, 1) + v4(spriteDimensions.x, spriteDimensions.y, 0, 0), matrix)) - toV2(V4MultiplyM4(v4(object->position.x, object->position.y, object->position.z, 1), matrix));
    v3 spriteOffset = v3(-0.5f*dimensions.x,-0.5f*dimensions.y,0);
    kRenderer_RendererObject*obj= kRenderer_getBoundObject(rendererObjectIndex);
    obj->shaderElements[0].triangles[0].valueUpdated[0] = true;
    obj->shaderElements[0].triangles[1].valueUpdated[0] = true;

    default_points[0].pos = toV3(V4MultiplyM4(v4xyz(renderable->offset,0) + v4(object->position.x,object->position.y,object->position.z,1),matrix)) + spriteOffset;
    default_points[1].pos = toV3(V4MultiplyM4(v4xyz(renderable->offset,0) + v4(object->position.x+spriteDimensions.x,object->position.y,object->position.z,1),matrix)) + spriteOffset;
    default_points[2].pos = toV3(V4MultiplyM4(v4xyz(renderable->offset,0) + v4(object->position.x,object->position.y+spriteDimensions.y,object->position.z,1),matrix)) + spriteOffset;
    default_points[3].pos = toV3(V4MultiplyM4(v4xyz(renderable->offset,0) + v4(object->position.x+spriteDimensions.x,object->position.y+spriteDimensions.y,object->position.z,1),matrix)) + spriteOffset;

    m4 rotation=M4InitDiagonal(1);
    iv2 windowSize=kRenderer_getWindowSize();
    rotation = M4MultiplyM4(rotation, M4TranslateV3(default_points[0].pos + v3(dimensions.x, dimensions.y, 0)));
    rotation = M4MultiplyM4(rotation, M4Mapper(v3(-windowSize.x, -windowSize.y, 0),
                                     v3(windowSize.x, windowSize.y, 1),
                                     v3(-1,-1,-1),
                                     v3(1,1,1)));
    rotation = M4MultiplyM4(rotation, M4Rotate(object->rotation.x, v3(1.0f, 0.0f, 0.0f)));
    rotation = M4MultiplyM4(rotation, M4Rotate(object->rotation.y, v3(0.0f, 1.0f, 0.0f)));
    rotation = M4MultiplyM4(rotation, M4Rotate(object->rotation.z, v3(0.0f, 0.0f, 1.0f)));
    rotation = M4MultiplyM4(rotation, M4Mapper(v3(-1,-1,-1),
                                     v3(1,1,1),
                                     v3(-windowSize.x, -windowSize.y, 0),
                                     v3(windowSize.x, windowSize.y, 1)));
    rotation = M4MultiplyM4(rotation, M4TranslateV3(default_points[0].pos * -1 - v3(dimensions.x,dimensions.y,0)));
    
    default_points[0].pos = toV3(V4MultiplyM4(v4xyz(default_points[0].pos,1),rotation));
    default_points[1].pos = toV3(V4MultiplyM4(v4xyz(default_points[1].pos,1),rotation));
    default_points[2].pos = toV3(V4MultiplyM4(v4xyz(default_points[2].pos,1),rotation));
    default_points[3].pos = toV3(V4MultiplyM4(v4xyz(default_points[3].pos,1),rotation));

    kRenderer_updateObject(rendererObjectIndex);
  }
  std::vector<std::vector<u32>> RendererObject::getSpriteIndexes()
  {
    std::vector<std::vector<u32>> res = std::vector<std::vector<u32>>(1);
    res[0]=std::vector<u32>(2);
    res[0][0] = renderable->spriteList->texIndex[renderable->getMapIndex()];
    res[0][1] = renderable->spriteList->texIndex[renderable->getMapIndex()];
    return res;
  }

  std::vector<Sprite> importSpriteSheet(const char *filename, i32 spriteWidth,
                                        i32 spriteHeight)
  {
    i32 width, height, numChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);

    u32 numSpritesHor = width / spriteWidth;
    u32 numSpritesVert = height / spriteHeight;
    std::vector<Sprite> sprites = std::vector<Sprite>();
    for (u32 i = 0; i < numSpritesVert; i++)
    {
      for (u32 j = 0; j < numSpritesHor; j++)
      {
        u8 *sprite = new u8[(u64)spriteWidth * spriteHeight * numChannels];
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
