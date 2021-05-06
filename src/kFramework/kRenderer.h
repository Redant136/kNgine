#pragma once

#define utils_StrManipulation
#include "utils.h"

#define kRenderer_maxWindows 8
#define kRenderer_maxFonts 32
#define kRenderer_maxShaderPrograms 16
#define kRenderer_maxShadersPerProgram 8

#define kRenderer_maxObjectTriangles 128
#define kRenderer_maxBoundObjects 64
#define kRenderer_maxObjectElements 16

#ifdef __cplusplus
extern "C"
{
#endif

  enum kRenderer_shaderTypes
  {
    kRenderer_SHADER_Vertex,
    kRenderer_SHADER_Fragment,
    kRenderer_SHADER_Geometry
  };

  typedef struct kRenderer_WindowContext
  {
    // cannot be changed after window creation
    // ----------------------------------------------------------
    // cannot be changed after launch
    struct
    {
      // the number of shader programs
      size_t length;
      struct
      {
        struct {
          size_t argLength;
          u32 vecSize[kRenderer_maxObjectElements];
        } structDef;

        size_t length;
        struct
        {
          enum kRenderer_shaderTypes shaderType;
          const char *shader;
        } shaders[kRenderer_maxShadersPerProgram];
      } programs[kRenderer_maxShaderPrograms];
    } windowShaders;
    // ----------------------------------------------------------
    // should be changed through kRenderer after window creation
    
    const char *name;
    union
    {
      iv2 windowSize;
      struct
      {
        u32 width;
        u32 height;
      };
    };
    // ----------------------------------------------------------
    // can be changed through context
    v4 currentColor;
    u8 vSync;
    void (*init)();
    void (*draw)();
  } kRenderer_WindowContext;

  typedef struct kRenderer_RendererObject
  {
    struct triangle
    {
      // the data of each triangle point
      f32 *arg[3];
      // if the values at that index were updated
      bool valueUpdated[kRenderer_maxObjectElements];
    };
    // the number of shader programs it uses
    size_t length;
    struct
    {
      u32 shadersIndex;
      // the number of triangles
      size_t length;
      struct triangle*triangles;
    } shaderElements[kRenderer_maxShaderPrograms];
  } kRenderer_RendererObject;

  // page creation methods
  // call in order init>createContext>size|name>createwindow>draw>launch
  i32 kRenderer_init(i32 argc, const char **argv);
  i32 kRenderer_createContext(kRenderer_WindowContext *context);
  i32 kRenderer_makeCurrentContext(kRenderer_WindowContext *context);
  i32 kRenderer_createWindow(kRenderer_WindowContext *context);

  void kRenderer_setWindowSize(i32 width, i32 height);
  void kRenderer_setWindowName(const char *windowName);
  void kRenderer_setWindowVSync(u8 vSync);
  void kRenderer_setStartFunction(void (*startDisplayFunc)());
  void kRenderer_setDrawFunction(void (*refreshFunction)());
  void kRenderer_setWindowBoundsScale(v3 mins, v3 maxs); // sets the scale on which the elements are draw

  void kRenderer_launch();

  // runtime methods
  void kRenderer_rotateViewport(v3 angle);

  void kRenderer_loadFont(const char *fontPath, const char *fontName);

  void kRenderer_setDrawColor(v4 color);
  void kRenderer_clear(v4 color);
  void kRenderer_clearGray(f32 gray);

  void kRenderer_drawRect(v2 points[4]);
  void kRenderer_drawRectV4(v4 rect);
  void kRenderer_drawTriangle(v2 points[3]);
  void kRenderer_drawLine(v2 points[2]);
  void kRenderer_drawPoint(v2 point);
  void kRenderer_drawLineLoop(v2 *points, u32 numPoints);
  void kRenderer_drawCircle(v2 startPoint, f32 radius);

  void kRenderer_drawBuffer_defaultShader(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                                          v3 position, f32 width, f32 height, v3 rotation);
  // void kRenderer_drawBuffer(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
  //                           void *args[kRenderer_maxShaderPrograms][4]);
  void kRenderer_bindTexture(u32 *textureIndex, u8 *buffer, i32 bufferWidth, i32 bufferHeight, i32 numChannels);
  void kRenderer_drawStoredTexture_defaultShader(u32 textureIndex, v3 position, f32 width, f32 height, v3 rotation);
  void kRenderer_drawStoredTexture(u32 textureIndex, size_t numShaders, u32 *shaders, f32 **args);
  void kRenderer_unbindTexture(u32 textureIndex);

  u32 kRenderer_addObject(u32*index,kRenderer_RendererObject obj);
  u32 kRenderer_bindObject(u32*index,kRenderer_RendererObject obj);
  kRenderer_RendererObject *kRenderer_getBoundObject(u32 index);
  void kRenderer_loadObject(u32 index);
  void kRenderer_updateObjects();         // updates the values of all objects
  void kRenderer_updateObject(u32 index); // updates the values of a single object
  void kRenderer_drawObject(u32 index);
  void kRenderer_drawObjectWithTexture(u32 objectIndex, u32 **textureIndex);
  void kRenderer_unloadObject(u32 index);

  void kRenderer_setFont(const char *fontName);
  void kRenderer_displayText(v3 position, v3 rotation, const char *text, f32 scale);

  //key events methods
  bool kRenderer_keyStatusPressed(Key e);
  v2 kRenderer_cursorPosition();
  bool kRenderer_mouseStatusPressed(Key e);

  // utils methods
  i32 kRenderer_getWindowWidth();
  i32 kRenderer_getWindowHeight();
  iv2 kRenderer_getWindowSize();
  f64 kRenderer_getTimeSinceLastFrame();
  m4 kRenderer_getMapper();

// this is not precompiled, as such the user will have to provide their own glad, glfw and freetype lib
#ifdef kRenderer_HeaderOnly
#include "kRendererGLFW.c"
#endif

#ifdef __cplusplus
}
#endif
