#include <vector>
#include <string>
#include <cmath>
#include "kutils.h"
#include "EngineObjects.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace kNgine{
  std::vector<EngineEvent> events;

  GameObject::GameObject() { flags |= ObjectFlags::GAME_OBJECT; }
  GameObject::GameObject(const GameObject&base):EngineObject(base)
  {
    this->position=base.position;
    this->rotation=base.rotation;
  }

  ObjectComponent::ObjectComponent(ComponentGameObject *base)
  {
    this->object = base;
    this->label = "none";
  }
  ObjectComponent::ObjectComponent(const ObjectComponent &base) {
    this->object = base.object;
    this->label = base.label;
  };
  
  ComponentGameObject::ComponentGameObject() {
    components = std::vector<ObjectComponent *>();
    flags |= ObjectFlags::COMPONENT;
  }
  ComponentGameObject::ComponentGameObject(const ComponentGameObject &base) :GameObject(base){
    this->components=std::vector<ObjectComponent*>();
    for(u32 i=0;i<base.components.size();i++){
      ObjectComponent*comp=new ObjectComponent(this);
      *comp=*base.components[i];
      this->components.push_back(comp);
      this->components[this->components.size()-1]->object=this;
    }
  }
  ComponentGameObject::~ComponentGameObject() {
    for (u32 i = 0; i < components.size(); i++) {
      delete components[i];
    }
  }
  void ComponentGameObject::addComponent(ObjectComponent *component)
  {
    components.push_back(component);
    this->labels.push_back(component->label);
    this->flags |= component->flags;
  }

  void ComponentGameObject::init(std::vector<EngineObject *> objects){
    for (u32 i = 0; i < components.size(); i++)
    {
      components[i]->init(objects);
    }
  }
  void ComponentGameObject::load()
  {
    for(u32 i=0;i<components.size();i++){
      components[i]->load();
    }
  }
  void ComponentGameObject::update(std::vector<msg> msgs) {
    for (ObjectComponent *mod : components) {
      mod->update(msgs);
    }
  }
  void ComponentGameObject::unload()
  {
    for (u32 i = 0; i < components.size(); i++)
    {
      components[i]->unload();
    }
  }
  void ComponentGameObject::end(std::vector<EngineObject *> objects)
  {
    for (u32 i = 0; i < components.size(); i++)
    {
      components[i]->end(objects);
    }
  }
  void ComponentGameObject::removeComponent(ObjectComponent*component){
    for(u32 i=0;i<components.size();i++){
      if(components[i]==component){
        components[i]->unload();
        delete components[i];
        components.erase(components.begin()+i);
        break;
      }
    }
  }

  SpriteAccessor::SpriteAccessor(ComponentGameObject *base) : Renderable(base)
  {
    this->label = "[sprite]";
    this->flags|=ObjectFlags::RENDERABLE;
  }

  SpriteComponent::SpriteComponent(ComponentGameObject *base)
      : SpriteAccessor(base) {
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(0, 0);
    this->sprite = fillSprite(1,1,{0,0,0,0});
  }
  SpriteComponent::SpriteComponent(ComponentGameObject *base, Sprite sprite)
      : SpriteAccessor(base) {
    this->sprite = sprite;
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(1, 1);
  }
  SpriteComponent::SpriteComponent(const SpriteComponent &base)
      : SpriteAccessor(base.object) {
    this->sprite = base.sprite;
    this->spriteLocation = base.spriteLocation;
    this->spriteDimension = base.spriteDimension;
    this->label = base.label;
  }
  SpriteComponent::~SpriteComponent() {freeSprite(this->sprite);}
  void SpriteComponent::update(std::vector<msg> msgs) {}
  Sprite *SpriteComponent::getSprite() { return &sprite; }
  v2 SpriteComponent::getSpriteDimensions() { return spriteDimension; }

  NodeObjectComponent::NodeObjectComponent(ComponentGameObject *parent, GameObject *child) : ObjectComponent(parent)
  {
    this->previousParentPos=parent->position;
    this->child=child;
    this->label = "[child]";
    this->flags |= ObjectFlags::PARENT;
  }
  void NodeObjectComponent::update(std::vector<msg> msgs) {
    child->position = (this->object->position - previousParentPos) + child->position;
    this->previousParentPos = this->object->position;
  }

  Sprite importSprite(const char *filename) {
    i32 width, height, numChannels;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);
    Sprite res;
    if (numChannels == 3) {
      u8 *data2 = new u8[width * height * 4];
      for (u32 i = 0; i < (u64) width * height; i++) {
        data2[4 * i] = data[numChannels * i];
        data2[4 * i + 1] = data[numChannels * i + 1];
        data2[4 * i + 2] = data[numChannels * i + 2];
        data2[4 * i + 3] = 255;
      }
      res = Sprite(width, height, 4, data2);
      stbi_image_free(data);
      // delete[] data2;
      return res;
    }
    res= Sprite(width, height, numChannels, data);
    // stbi_image_free(data);
    return res;
  }

  void addEvent(EngineEvent event){
    events.push_back(event);
  }
  void* callEvent(std::string name,void*arg){
    for(u32 i=0;i<events.size();i++){
      if(events[i].name==name){
        return events[i].event(arg);
      }
    }
    return NULL;
  }
  EngineEvent &getEvent(std::string name){
    for (u32 i = 0; i < events.size(); i++)
    {
      if (events[i].name == name)
      {
        return events[i];
      }
    }
    assert(0 && "no such event");
    return events[0];
  }
  void removeEvent(std::string name){
    for (u32 i = 0; i < events.size(); i++)
    {
      if (events[i].name == name)
      {
        events.erase(events.begin()+i);
      }
    }
  }

  #ifdef PNG_H
  Sprite [[deprecated]] importPNG(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) fp = fopen(("~/" + std::string(filename)).c_str(), "rb");
    if (!fp)
      throw "[read_png_file] File " + std::string(filename) +
          " could not be opened for reading";

    png_structp png =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
      throw "[read_png_file] png_create_read_struct failed";
    }
    png_infop info = png_create_info_struct(png);
    if (!info) {
      throw "[read_png_file] png_create_info_struct failed";
    }
    if (setjmp(png_jmpbuf(png))) throw "[read_png_file] Error during init_io";

    png_init_io(png, fp);
    png_read_info(png, info);
    i32 width = png_get_image_width(png, info);
    i32 height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);
    if (bit_depth == 16) png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
      png_set_expand_gray_1_2_4_to_8(png);
    }
    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
      png_set_tRNS_to_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
      png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
      png_set_gray_to_rgb(png);
    }
    png_read_update_info(png, info);
    png_bytep *row_pointers = new png_bytep[height];
    for (i32 y = 0; y < height; y++) {
      row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    fclose(fp);

    std::vector<color> colorMap = std::vector<color>();
    unsigned char *newColors = new unsigned char[width * height * 4];
    for (i32 y = 0; y < height; y++) {
      png_bytep row = row_pointers[y];
      for (i32 x = 0; x < width; x++) {
        png_bytep px = &(row[x * 4]);
        colorMap.push_back(
            color(px[0] / 255.0, px[1] / 255.0, px[2] / 255.0, px[3] / 255.0));
        newColors[x + y * height] = px[0];
        newColors[x + y * height + 1] = px[1];
        newColors[x + y * height + 2] = px[2];
        newColors[x + y * height + 3] = 255;
      }
    }
    png_destroy_read_struct(&png, &info, NULL);
    return Sprite(width, height, 4, newColors);
  }
  #endif
}
