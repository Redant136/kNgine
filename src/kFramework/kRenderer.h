#pragma once

#include "utils.h"
#include "kStruct_def.h"

#define kRenderer_maxWindows 8
#define kRenderer_maxFonts 32
#define kRenderer_maxShaderPrograms 16
#define kRenderer_maxShadersPerProgram 16

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct kRenderer_WindowContext
  {
    const char *name;
    v4 currentColor;
    union
    {
      iv2 windowSize;
      struct
      {
        u32 width;
        u32 height;
      };
    };
    bool gSync;
    enum shaderTypes
    {
      kRenderer_SHADER_Vertex,
      kRenderer_SHADER_Fragment,
      kRenderer_SHADER_Geometry
    };
    // the first argument of the shader must be v3 position
    struct
    {
      size_t length;
      struct
      {
        kStruct_StructDef argsDef;
        size_t length;
        struct
        {
          enum shaderTypes shaderType;
          const char *shader;
        } shaders[kRenderer_maxShadersPerProgram];
      } programs[kRenderer_maxShaderPrograms];
    } windowShaders;
    void (*init)();
    void (*draw)();
  } kRenderer_WindowContext;

  // page creation methods
  // call in order init>createContext>size|name>createwindow>draw>launch
  i32 kRenderer_init(i32 argc, const char **argv);
  i32 kRenderer_createContext(kRenderer_WindowContext *context);
  i32 kRenderer_makeCurrentContext(kRenderer_WindowContext *context);
  i32 kRenderer_createWindow(kRenderer_WindowContext *context);

  void kRenderer_setWindowSize(i32 width, i32 height);
  void kRenderer_setWindowName(const char *windowName);
  void kRenderer_setStartFunction(void (*startDisplayFunc)());
  void kRenderer_setDrawFunction(void (*refreshFunction)());
  void kRenderer_setWindowBoundsScale(v3 mins, v3 maxs); // sets the scale on which the elements are draw

  void kRenderer_launch();

  // runtime methods
  void kRenderer_rotateViewport(v3 angle);

  void kRenderer_loadFont(const char*fontPath,const char*fontName);

  void kRenderer_setDrawColor(v4 color);
  void kRenderer_clear(v4 color);
  void kRenderer_clearGray(f32 gray);

  void kRenderer_drawRect(v2 points[4]);
  void kRenderer_drawRectV4(v4 rect);
  void kRenderer_drawTriangle(v2 points[3]);
  void kRenderer_drawLine(v2 points[2]);
  void kRenderer_drawPoint(v2 point);
  void kRenderer_drawLineLoop(v2 *points, i32 numPoints);
  void kRenderer_drawCircle(v2 startPoint, f32 radius);

  void kRenderer_drawBuffer_defaultShader(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                                          v3 position, i32 width, i32 height, v3 rotation);
  void kRenderer_drawBuffer(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                            v3 position, i32 width, i32 height, v3 rotation, void *args[kRenderer_maxShaderPrograms]);
  void kRenderer_bindTexture(u32 *textureIndex, u8 *buffer, i32 bufferWidth, i32 bufferHeight, i32 numChannels);
  void kRenderer_drawStoredTexture_defaultShader(u32 textureIndex, v3 position, i32 width, i32 height, v3 rotation);
  void kRenderer_drawStoredTexture(u32 textureIndex, v3 position, i32 width, i32 height, v3 rotation, void *args[kRenderer_maxShaderPrograms]);
  void kRenderer_unbindTexture(u32 textureIndex);

  void kRenderer_setFont(const char*fontName);
  void kRenderer_displayText(v3 position,v3 rotation, const char *text,f32 scale);

  //key events methods
  bool kRenderer_keyStatusPressed(Key e);
  v2 kRenderer_cursorPosition();
  bool kRenderer_mouseStatusPressed(Key e);

  // utils methods
  i32 kRenderer_getWindowWidth();
  i32 kRenderer_getWindowHeight();
  iv2 kRenderer_getWindowSize();
  f64 kRenderer_getTimeSinceLastFrame();

#ifdef __cplusplus
}
#endif
