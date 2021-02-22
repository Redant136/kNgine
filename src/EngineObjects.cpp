#include <vector>
#include <string>
#include <cmath>
#include "utils.hpp"
#include "EngineObjects.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

namespace kNgine{
  std::vector<EngineEvent> events;

  Sprite::Sprite() {
    width = 1;
    height = 1;
    numChannels = 4;
    colorMap = std::vector<unsigned char>(4);
    colorMap[0] = 0;
    colorMap[1] = 0;
    colorMap[2] = 0;
    colorMap[3] = 0;
  }
  Sprite::Sprite(int width, int height, color colorFill) {
    this->width = width;
    this->height = height;
    this->numChannels = 4;
    this->colorMap = std::vector<unsigned char>(width * height * numChannels);
    for (int i = 0; i < width * height * numChannels; i += numChannels) {
      colorMap[i] = colorFill.r;
      colorMap[i + 1] = colorFill.g;
      colorMap[i + 2] = colorFill.b;
      colorMap[i + 3] = colorFill.a;
    }
  }
  Sprite::Sprite(int width, int height, unsigned char *colorMap) {
    this->width = width;
    this->height = height;
    this->numChannels = 4;
    this->colorMap = std::vector<unsigned char>(width * height * numChannels);
    for (int i = 0; i < width * height * numChannels; i++) {
      this->colorMap[i] = colorMap[i];
    }
  }
  Sprite::Sprite(int width, int height, int numChannels,
                unsigned char *colorMap) {
    this->width = width;
    this->height = height;
    this->numChannels = numChannels;
    this->colorMap = std::vector<unsigned char>(width * height * numChannels);
    for (int i = 0; i < width * height * numChannels; i++) {
      this->colorMap[i] = colorMap[i];
    }
  }
  Sprite::Sprite(int width, int height, int numChannels,
                std::vector<unsigned char> colorMap) {
    this->width = width;
    this->height = height;
    this->numChannels = numChannels;
    this->colorMap = colorMap;
  }
  Sprite::Sprite(const Sprite &base) {
    this->width = base.width;
    this->height = base.height;
    this->numChannels = base.numChannels;
    this->colorMap = base.colorMap;
  }
  Sprite::~Sprite() {}
  void Sprite::resize(int newWidth, int newHeight) {
    // if image is same size with accuracy of 1 pixel, do nothing
    if (abs(width - newWidth) <= 2 && abs(height - newHeight) <= 2) return;
    std::vector<color> newColorMap = std::vector<color>();
    for (int i = 0; i < width * height * numChannels; i += numChannels) {
      if (numChannels == 3)
        newColorMap.push_back(color(this->colorMap[i], this->colorMap[i + 1],
                                    this->colorMap[i + 2], 255));
      else
        newColorMap.push_back(color(this->colorMap[i], this->colorMap[i + 1],
                                    this->colorMap[i + 2],
                                    this->colorMap[i + 3]));
    }

    std::vector<color> temp = std::vector<color>(newWidth * newHeight);
    float x_ratio = width / (float)newWidth;
    float y_ratio = height / (float)newHeight;
    float px, py;
    for (int i = 0; i < newHeight; i++) {
      for (int j = 0; j < newWidth; j++) {
        px = (int)(j * x_ratio);
        py = (int)(i * y_ratio);
        temp[(i * newWidth) + j] = newColorMap[(int)((py * width) + px)];
      }
    }
    this->width = newWidth;
    this->height = newHeight;
    this->numChannels = 4;
    this->colorMap = std::vector<unsigned char>();
    for (int i = 0; i < temp.size(); i++) {
      this->colorMap.push_back(temp[i].r);
      this->colorMap.push_back(temp[i].g);
      this->colorMap.push_back(temp[i].b);
      this->colorMap.push_back(temp[i].a);
    }
  }

  GameObject::GameObject() { flags.push_back(objectFlags::GAME_OBJECT); }
  GameObject::GameObject(const GameObject&base):EngineObject(base)
  {
    this->position=base.position;
    this->rotation=base.rotation;
  }

  ObjectComponent::ObjectComponent(GameObject *base) {
    this->object = base;
    this->label = "none";
  }
  ObjectComponent::ObjectComponent(const ObjectComponent &base) {
    this->object = base.object;
    this->label = base.label;
  };
  ObjectComponent::~ObjectComponent() {}
  void ObjectComponent::update(std::vector<msg> msgs) {}
  ComponentGameObject::ComponentGameObject() {
    components = std::vector<ObjectComponent *>();
    flags.push_back(objectFlags::COMPONENT);
  }
  ComponentGameObject::ComponentGameObject(const ComponentGameObject &base) :GameObject(base){
    this->components=std::vector<ObjectComponent*>();
    for(int i=0;i<base.components.size();i++){
      ObjectComponent*comp=new ObjectComponent(this);
      *comp=*base.components[i];
      this->components.push_back(comp);
      this->components[this->components.size()-1]->object=this;
    }
  }
  ComponentGameObject::~ComponentGameObject() {
    for (int i = 0; i < components.size(); i++) {
      delete components[i];
    }
  }
  void ComponentGameObject::update(std::vector<msg> msgs) {
    for (ObjectComponent *mod : components) {
      mod->update(msgs);
    }
  }
  void ComponentGameObject::removeComponent(ObjectComponent*component){
    for(int i=0;i<components.size();i++){
      if(components[i]==component){
        components.erase(components.begin()+i);
        break;
      }
    }
  }

  SpriteAccessor::SpriteAccessor(GameObject *base) : ObjectComponent(base) {
    this->label = "[sprite]";
    this->object->flags.push_back(objectFlags::SPRITE);
    this->object->labels.push_back(label);
  }

  SpriteComponent::SpriteComponent(ComponentGameObject *base)
      : SpriteAccessor(base) {
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(0, 0);
    this->sprite = Sprite();
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
  SpriteComponent::~SpriteComponent() {}
  void SpriteComponent::update(std::vector<msg> msgs) {}
  Sprite *SpriteComponent::getSprite() { return &sprite; }
  v2 SpriteComponent::getSpriteDimensions() { return spriteDimension; }

  LayerRenderer::LayerRenderer(){
    flags.push_back(objectFlags::UI);
    labels.push_back("UI");
  }
  void LayerRenderer::render(){
  }

  ChildrenObject::ChildrenObject(GameObject *object, v3 &parentPosition)
      : parentPosition(parentPosition) {
    this->object = object;
    this->position = object->position-parentPosition;
    this->previousParentPosition = parentPosition;
    this->flags.push_back(objectFlags::CHILD);
    labels.push_back("_CHILDREN_");
  }
  void ChildrenObject::update(std::vector<msg> msgs) {
    object->position += parentPosition - previousParentPosition;
    this->position = object->position - parentPosition;
    this->previousParentPosition = parentPosition;
    object->update(msgs);
  }

  ParentObject::ParentObject() {
    children = std::vector<GameObject *>();
    flags.push_back(objectFlags::PARENT);
    labels.push_back("_PARENT_");
  }
  ParentObject::ParentObject(const ParentObject &base) : ComponentGameObject(base)
  {
    this->children=base.children;
  }

  Sprite importSprite(const char *filename) {
    int width, height, numChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);
    Sprite res;
    if (numChannels == 3) {
      unsigned char *data2 = new unsigned char[width * height * 4];
      for (int i = 0; i < width * height; i++) {
        data2[4 * i] = data[numChannels * i];
        data2[4 * i + 1] = data[numChannels * i + 1];
        data2[4 * i + 2] = data[numChannels * i + 2];
        data2[4 * i + 3] = 255;
      }
      res = Sprite(width, height, 4, data2);
      stbi_image_free(data);
      delete[] data2;
      return res;
    }
    res= Sprite(width, height, numChannels, data);
    stbi_image_free(data);
    return res;
  }

  void addEvent(EngineEvent event){
    events.push_back(event);
  }
  void callEvent(std::string name){
    for(int i=0;i<events.size();i++){
      if(events[i].name==name){
        events[i].event(*events[i].objects);
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
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
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
    for (int y = 0; y < height; y++) {
      row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    fclose(fp);

    std::vector<color> colorMap = std::vector<color>();
    unsigned char *newColors = new unsigned char[width * height * 4];
    for (int y = 0; y < height; y++) {
      png_bytep row = row_pointers[y];
      for (int x = 0; x < width; x++) {
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
