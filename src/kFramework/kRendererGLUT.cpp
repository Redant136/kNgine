#include <math.h>
#define utils_StrManipulation
#include "utils.h"
#include "kRenderer.h"
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <ft2build.h>
#include FT_FREETYPE_H

// void (*partialDisplayFunction)();
// void (*partialDrawFunction)();
// double getWindowWidth()
// {
//   return glutGet(GLUT_WINDOW_WIDTH);
// }
// double getWindowHeight()
// {
//   return glutGet(GLUT_WINDOW_HEIGHT);
// }

// void init(int argc, char **argv)
// {
//   glutInit(&argc, argv);
//   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//   glutInitWindowPosition(0, 0);
// }
// void setWindowSize(int width, int height)
// {
//   glutInitWindowSize(1920, 1080);
// }
// void setWindowName(const char *windowName)
// {
//   glutCreateWindow(windowName);
// }
// void completeDisplayFunc()
// {
//   glOrtho(0, renderer::getWindowWidth(), renderer::getWindowHeight(), 0, 0, 1);
//   partialDisplayFunction();
//   glutSwapBuffers();
// }
// void setupWindow(void (*startDisplayFunc)())
// {
//   partialDisplayFunction = startDisplayFunc;
//   glutDisplayFunc(completeDisplayFunc);
// }
// void completeDrawFunc()
// {
//   partialDrawFunction();
//   glutSwapBuffers();
// }
// void drawFunction(void (*refreshFunction)())
// {
//   partialDrawFunction = refreshFunction;
//   glutIdleFunc(completeDrawFunc);
// }
// void launch()
// {
//   glutMainLoop();
// }

// void drawRect(v2 points[4])
// {
//   glBegin(GL_QUAD_STRIP);
//   glVertex2d(points[0].x, points[0].y);
//   glVertex2d(points[1].x, points[1].y);
//   glVertex2d(points[2].x, points[2].y);
//   glVertex2d(points[3].x, points[3].y);
//   glEnd();
// }
// void drawRect(v2 startPos, double width, double height)
// {
//   v2 points[4]{v2(startPos),
//                v2(startPos.x, startPos.y + height),
//                v2(startPos.x + width, startPos.y),
//                v2(startPos.x + width, startPos.y + height)};
//   renderer::drawRect(points);
// }
// void drawTriangle(v2 points[3])
// {
//   glBegin(GL_TRIANGLES);
//   glVertex2d(points[0].x, points[0].y);
//   glVertex2d(points[1].x, points[1].y);
//   glVertex2d(points[2].x, points[2].y);
//   glEnd();
// }
// void drawLine(v2 points[2])
// {
//   glBegin(GL_LINE);
//   glVertex2d(points[0].x, points[0].y);
//   glVertex2d(points[1].x, points[1].y);
//   glEnd();
// }
// void drawPoint(v2 point)
// {
//   glBegin(GL_POINTS);
//   glVertex2d(point.x, point.y);
//   glEnd();
// }
// void drawLineLoop(v2 *points, int numPoints)
// {
//   glBegin(GL_LINE_LOOP);
//   for (int i = 0; i < numPoints; i++)
//   {
//     glVertex2d(points[i].x, points[i].y);
//   }
//   glEnd();
// }
// void drawPolygon(v2 *points, double numPoints)
// {
//   glBegin(GL_POLYGON);
//   for (int i = 0; i < numPoints; i++)
//   {
//     glVertex2d(points[i].x, points[i].y);
//   }
//   glEnd();
// }
// void drawColorMap(color *colors, v2 position, int width, int height)
// {
//   for (int i = 0; i < height; i++)
//   {
//     for (int j = 0; j < width; j++)
//     {
//       color c = colors[i * width + j];
//       renderer::setDrawColor(c.r, c.g, c.b, c.a);
//       renderer::drawPoint(position + v2(j, i));
//     }
//   }
// }

// v2 convertToRelativePosition(v2 pos)
// {
//   v2 nPos = pos;
//   nPos.x /= getWindowWidth();
//   nPos.y /= renderer::getWindowHeight();
//   return nPos;
// }
// v2 convertToAbsolutePosition(v2 pos)
// {
//   v2 nPos = pos;
//   nPos.x *= renderer::getWindowWidth();
//   nPos.y *= renderer::getWindowHeight();
//   return nPos;
// }

#include <stdio.h>

static FT_Library ft;

static v3 rotation = {0, 0, 0};

static struct
{
  u32 currentFont;
  size_t length;
  struct
  {
    const char *font_name;
    struct
    {
      char character;
      u32 textureId;
      iv2 startPos;
      u8 width;
      u8 height;
      u8 advance;
    } character[256]; // (c+128)%256
  } fonts[kRenderer_maxFonts];
} fontMaps;

static struct
{
  size_t length;
  struct
  {
    i32 shaderPrograms[kRenderer_maxShaderPrograms];
    v3 min;
    v3 max;
    kRenderer_WindowContext *context;
    GLint window;
  } windows[kRenderer_maxWindows];
} kRenderer_WindowsContexts;
static u32 currentContext = 0;

static i32 defaultShader;
static u32 defaultVAO = 0;

bool keysPressed[MOUSE_LAST];
iv2 mousePos;

static f64 lastTime = 0;

static const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in float aIsTex;\n"
    "layout (location = 2) in vec4 aColorTexCoord;\n"
    "out float isTex;\n"
    "out vec4 colorTexCoord;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "  isTex = aIsTex;\n"
    "  colorTexCoord = aColorTexCoord;\n"
    "}\0";
static const char *defaultFragmentShaderSource =
    "#version 330 core\n"
    "in vec4 colorTexCoord;\n"
    "in float isTex;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D textureMap;\n"
    "uniform vec4 colorScale = vec4(1.0f,1.0f,1.0f,1.0f);\n"
    "void main()\n"
    "{\n"
    "  if(isTex>0.5f){\n"
    "  \n"
    "    FragColor = texture(textureMap, vec2(colorTexCoord.x,colorTexCoord.y))*colorScale;\n"
    "  }else{\n"
    "    FragColor = colorTexCoord*colorScale;\n"
    "  }\n"
    "}\0";

static u32 KeyToGlutSpecial(Key k){
  switch (k)
  {
    case KEY_RIGHT:
      return GLUT_KEY_RIGHT;
    case KEY_LEFT:
      return GLUT_KEY_LEFT;
    case KEY_DOWN:
      return GLUT_KEY_DOWN;
    case KEY_UP:
      return GLUT_KEY_UP;
    case KEY_F1:
      return GLUT_KEY_F1;
    case KEY_F2:
      return GLUT_KEY_F2;
    case KEY_F3:
      return GLUT_KEY_F3;
    case KEY_F4:
      return GLUT_KEY_F4;
    case KEY_F5:
      return GLUT_KEY_F5;
    case KEY_F6:
      return GLUT_KEY_F6;
    case KEY_F7:
      return GLUT_KEY_F7;
    case KEY_F8:
      return GLUT_KEY_F8;
    case KEY_F9:
      return GLUT_KEY_F9;
    case KEY_F10:
      return GLUT_KEY_F10;
    case KEY_F11:
      return GLUT_KEY_F11;
    case KEY_F12:
      return GLUT_KEY_F12;
    default:
      return 0;
  }
}
static Key GlutKeyToKey(i32 k){
    switch (k)
  {
    case GLUT_KEY_RIGHT:
      return KEY_RIGHT;
    case GLUT_KEY_LEFT:
      return KEY_LEFT;
    case GLUT_KEY_DOWN:
      return KEY_DOWN;
    case GLUT_KEY_UP:
      return KEY_UP;
    case GLUT_KEY_F1:
      return KEY_F1;
    case GLUT_KEY_F2:
      return KEY_F2;
    case GLUT_KEY_F3:
      return KEY_F3;
    case GLUT_KEY_F4:
      return KEY_F4;
    case GLUT_KEY_F5:
      return KEY_F5;
    case GLUT_KEY_F6:
      return KEY_F6;
    case GLUT_KEY_F7:
      return KEY_F7;
    case GLUT_KEY_F8:
      return KEY_F8;
    case GLUT_KEY_F9:
      return KEY_F9;
    case GLUT_KEY_F10:
      return KEY_F10;
    case GLUT_KEY_F11:
      return KEY_F11;
    case GLUT_KEY_F12:
      return KEY_F12;
    default:
      return UNREGISTERED_KEY;
  }
}
static i32 KeyToGlutKeyModifier(Key k){
  switch(k){
    case KEY_LEFT_SHIFT:
      return GLUT_ACTIVE_SHIFT;
    case KEY_LEFT_CONTROL:
      return GLUT_ACTIVE_CTRL;
    case KEY_LEFT_ALT:
      return GLUT_ACTIVE_ALT;
    case KEY_LEFT_SUPER: // glut doesnt have a super key
      return 0;
    case KEY_RIGHT_SHIFT:
      return GLUT_ACTIVE_SHIFT;
    case KEY_RIGHT_CONTROL:
      return GLUT_ACTIVE_CTRL;
    case KEY_RIGHT_ALT:
      return GLUT_ACTIVE_ALT;
    default:
      return 0;
  }
}
static Key GlutModifierKeyToKey(i32 k){
  switch(k){
    case GLUT_ACTIVE_SHIFT:
      return KEY_LEFT_SHIFT;
    case GLUT_ACTIVE_CTRL:
      return KEY_LEFT_CONTROL;
    case GLUT_ACTIVE_ALT:
      return KEY_LEFT_ALT;
    default:
      return UNREGISTERED_KEY;
  }
}
static Key GlutMouseToKey(i32 k){
  switch (k)
  {
  case GLUT_LEFT_BUTTON:
    return MOUSE_LEFT;
  case GLUT_MIDDLE_BUTTON:
    return MOUSE_MIDDLE;
  case GLUT_RIGHT_BUTTON:
    return MOUSE_RIGHT;
  default:
    return UNREGISTERED_KEY;
  }
}

static void compileShaders()
{
  if (1)
  {
    i32 success;
    char infoLog[512];
    defaultShader = glCreateProgram();
    i32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // error checks
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("%s\n", infoLog);
      assert(0 && "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }
    i32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // error checks
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      printf("%s\n", infoLog);
      assert(0 && "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    glAttachShader(defaultShader, vertexShader);
    glDeleteShader(vertexShader);
    glAttachShader(defaultShader, fragmentShader);
    glDeleteShader(fragmentShader);
    glLinkProgram(defaultShader);
    // check for linking errors
    glGetProgramiv(defaultShader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(defaultShader, 512, NULL, infoLog);
      printf("%s\n", infoLog);
      assert(0 && "ERROR::SHADER::COMPILATION_FAILED\n");
    }
  }
  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    for (u32 j = 0; j < kRenderer_WindowsContexts.windows[i].context->windowShaders.length; j++)
    {
      i32 success;
      char infoLog[512];
      kRenderer_WindowsContexts.windows[i].shaderPrograms[j] = glCreateProgram();
      for (u32 s = 0; s < kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].length; s++)
      {
        i32 shader;
        GLenum shaderType;
        switch (kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shaderType)
        {
        case kRenderer_WindowContext::shaderTypes::kRenderer_SHADER_Vertex:
          shaderType = GL_VERTEX_SHADER;
          break;
        case kRenderer_WindowContext::shaderTypes::kRenderer_SHADER_Fragment:
          shaderType = GL_FRAGMENT_SHADER;
          break;
        case kRenderer_WindowContext::shaderTypes::kRenderer_SHADER_Geometry:
          shaderType = GL_GEOMETRY_SHADER;
          break;
        default:
          shaderType = GL_VERTEX_SHADER;
          break;
        }
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shader, NULL);
        glCompileShader(shader);
        // error checks
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
          glGetShaderInfoLog(shader, 512, NULL, infoLog);
          printf("%s\n", infoLog);
          assert(0 && "ERROR::SHADER::COMPILATION_FAILED\n");
        }
        glAttachShader(kRenderer_WindowsContexts.windows[i].shaderPrograms[j], shader);
        glDeleteShader(shader);
      }
      glLinkProgram(kRenderer_WindowsContexts.windows[i].shaderPrograms[j]);
      // error check
      glGetProgramiv(kRenderer_WindowsContexts.windows[i].shaderPrograms[j], GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(kRenderer_WindowsContexts.windows[i].shaderPrograms[j], 512, NULL, infoLog);
        printf("%s\n", infoLog);
        assert(0 && "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
      }
    }
  }
}

static m4 kRenderer_getMapper()
{
  m4 mapper = M4InitDiagonal(1);
  mapper = M4MultiplyM4(mapper, M4Rotate(rotation.x, v3(1, 0, 0)));
  mapper = M4MultiplyM4(mapper, M4Rotate(rotation.y, v3(0, 1, 0)));
  mapper = M4MultiplyM4(mapper, M4Rotate(rotation.z, v3(0, 0, 1)));
  mapper = M4MultiplyM4(mapper, M4Mapper(kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max, v3(-1, -1, -1), v3(1, 1, 1)));
  return mapper;
}

i32 kRenderer_init(i32 argc, const char **argv)
{
  glutInit(&argc, (char**)argv);
  u64 glutOptions = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH;
#ifdef __APPLE__
  glutOptions|=GLUT_3_2_CORE_PROFILE;
#endif
  glutInitDisplayMode(glutOptions);
  glutInitWindowPosition(0, 0);
  return 0;
}
i32 kRenderer_createContext(kRenderer_WindowContext *context)
{
  kRenderer_WindowContext c = {"kRenderer", v4(1, 1, 1, 1), iv2(1920, 1080), true, {0, NULL}, NULL, NULL};
  *context = c;
  assert(kRenderer_WindowsContexts.length < kRenderer_maxWindows);
  currentContext = (u32)kRenderer_WindowsContexts.length;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].context = context;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].min = v3(0, 1080, 0);
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].max = v3(1920, 0, 1);

  kRenderer_WindowsContexts.length++;
  return 0;
}
i32 kRenderer_makeCurrentContext(kRenderer_WindowContext *context)
{
  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    if (kRenderer_WindowsContexts.windows[i].context == context)
    {
      currentContext = i;
      return 0;
    }
  }
  return -1;
}
i32 kRenderer_createWindow(kRenderer_WindowContext *context)
{
  glutInitWindowSize(context->width, context->height);
  GLint window = glutCreateWindow(context->name);
  if (!gladLoadGL())
  {
    assert(0 && "could not initialize GLAD");
    return -1;
  }

  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    if (kRenderer_WindowsContexts.windows[i].context == context)
    {
      kRenderer_WindowsContexts.windows[i].window = window;
      if (fCompare(kRenderer_WindowsContexts.windows[currentContext].min.y, kRenderer_WindowsContexts.windows[currentContext].context->height) &&
          fCompare(kRenderer_WindowsContexts.windows[currentContext].max.x, kRenderer_WindowsContexts.windows[currentContext].context->width))
      {
        kRenderer_WindowsContexts.windows[i].min = v3(0.f, (f32)kRenderer_getWindowHeight(), 0.f);
        kRenderer_WindowsContexts.windows[i].max = v3((f32)kRenderer_getWindowWidth(), 0.f, 1.f);
      }
      kRenderer_WindowsContexts.windows[currentContext].context->windowSize = kRenderer_getWindowSize();
    }
  }

  glutKeyboardFunc([](unsigned char c,i32 x,i32 y){
    keysPressed[CharToKey(c)]=true;
    for (u32 i = KEY_LEFT_SHIFT; i < KEY_LAST; i++)
    {
      keysPressed[i] = glutGetModifiers() & KeyToGlutKeyModifier((Key)i);
    }
  });
  glutKeyboardUpFunc([](unsigned char c,i32 x,i32 y){
    keysPressed[CharToKey(c)]=false;
  });
  glutSpecialFunc([](i32 c,i32 x,i32 y){
    keysPressed[GlutKeyToKey(c)]=true;
    for (u32 i = KEY_LEFT_SHIFT; i < KEY_LAST; i++)
    {
      keysPressed[i] = glutGetModifiers() & KeyToGlutKeyModifier((Key)i);
    }
  });
  glutSpecialUpFunc([](i32 c,i32 x,i32 y){
    keysPressed[GlutKeyToKey(c)]=false;
  });
  glutMouseFunc([](i32 button,i32 state,i32 x,i32 y){
    keysPressed[GlutMouseToKey(button)]=state==GLUT_DOWN;
  });
  glutPassiveMotionFunc([](i32 x,i32 y){
    mousePos=iv2(x,y);
  });

  // if (defaultVAO == 0)
  // {
  //   glGenVertexArrays(1, &defaultVAO);
  //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
  //   glEnableVertexAttribArray(0);
  //   glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
  //   glEnableVertexAttribArray(1);
  //   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
  //   glEnableVertexAttribArray(2);
  // }

  return 0;
}

void kRenderer_setWindowSize(i32 width, i32 height)
{
  if (fCompare(kRenderer_WindowsContexts.windows[currentContext].min.y, kRenderer_WindowsContexts.windows[currentContext].context->height) &&
      fCompare(kRenderer_WindowsContexts.windows[currentContext].max.x, kRenderer_WindowsContexts.windows[currentContext].context->width))
  {
    kRenderer_WindowsContexts.windows[currentContext].max.x = (f32)width;
    kRenderer_WindowsContexts.windows[currentContext].min.y = (f32)height;
  }
  kRenderer_WindowsContexts.windows[currentContext].context->windowSize = iv2(width, height);
  if (kRenderer_WindowsContexts.windows[currentContext].window != 0)
  {
    glViewport(0, 0, width, height);
  }
}
void kRenderer_setWindowName(const char *windowName)
{
  kRenderer_WindowsContexts.windows[currentContext].context->name = windowName;
  if (kRenderer_WindowsContexts.windows[currentContext].window != 0)
  {
    glutSetWindowTitle(windowName);
  }
}
void kRenderer_setStartFunction(void (*startDisplayFunc)())
{
  kRenderer_WindowsContexts.windows[currentContext].context->init = startDisplayFunc;
}
void kRenderer_setDrawFunction(void (*refreshFunction)())
{
  kRenderer_WindowsContexts.windows[currentContext].context->draw = refreshFunction;
}
void kRenderer_setWindowBoundsScale(v3 min, v3 max)
{
  kRenderer_WindowsContexts.windows[currentContext].min = min;
  kRenderer_WindowsContexts.windows[currentContext].max = max;
}

void kRenderer_launch()
{
  // All functions return a value different than 0 whenever an error occurred
  if (FT_Init_FreeType(&ft))
  {
    assert(0 && "ERROR::FREETYPE: Could not init FreeType Library");
  }
  fontMaps.currentFont = 0;
  kRenderer_loadFont("ressources/fonts/roboto/Roboto-Regular.ttf", "roboto");
  kRenderer_loadFont("ressources/fonts/rainyhearts/rainyhearts.ttf", "rainyhearts");
  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    glutSetWindow(kRenderer_WindowsContexts.windows[i].window);
    currentContext=i;
    glutDisplayFunc([]() {
      glClear(GL_COLOR_BUFFER_BIT);
      kRenderer_WindowsContexts.windows[currentContext].context->init();
      glutSwapBuffers();
    });
    glutIdleFunc([]() {
      glClear(GL_COLOR_BUFFER_BIT);
      kRenderer_WindowsContexts.windows[currentContext].context->draw();
      glutSwapBuffers();
    });
  }

  // assert(0 && "test stop, everything ok");
  glutMainLoop();
  // while (running)
  // {
  //   running = false;
  //   // double time = glfwGetTime();
  //   for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  //   {
  //     // glfwMakeContextCurrent(kRenderer_WindowsContexts.windows[i].window);
  //     glClear(GL_COLOR_BUFFER_BIT);
  //     currentContext = i;
  //     kRenderer_WindowsContexts.windows[i].context->draw();
  //     // glfwSwapInterval(kRenderer_WindowsContexts.windows[i].context->gSync ? 1 : 0);
  //     // glfwSwapBuffers(kRenderer_WindowsContexts.windows[i].window);
  //     // glfwPollEvents();
  //     // running |= !glfwWindowShouldClose(kRenderer_WindowsContexts.windows[i].window);
  //   }
  //   // lastTime = time;
  // }
  // glfwTerminate();
}

//---------------------------------

void kRenderer_rotateViewport(v3 angle)
{
  rotation = angle;
}

void kRenderer_loadFont(const char *fontPath, const char *fontName)
{
  assert(fontMaps.length < kRenderer_maxFonts);
  fontMaps.fonts[fontMaps.length].font_name = fontName;
  FT_Face face;
  if (FT_New_Face(ft, fontPath, 0, &face))
  {
    assert(0 && "ERROR::FREETYPE: Failed to load font");
  }
  else
  {
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // modify for extended ascii
    for (u8 c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        assert(0 && "ERROR::FREETYTPE: Failed to load Glyph");
        return;
      }
      // generate texture
      u8 *grayScaleBuffer = new u8[face->glyph->bitmap.width * face->glyph->bitmap.rows * 4];
      for (u32 y = 0; y < face->glyph->bitmap.rows; y++)
      {
        for (u32 x = 0; x < face->glyph->bitmap.width; x++)
        {
          grayScaleBuffer[(y * face->glyph->bitmap.width + x) * 4] = 255;
          grayScaleBuffer[(y * face->glyph->bitmap.width + x) * 4 + 1] = 255;
          grayScaleBuffer[(y * face->glyph->bitmap.width + x) * 4 + 2] = 255;
          grayScaleBuffer[(y * face->glyph->bitmap.width + x) * 4 + 3] = face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x];
        }
      }
      u32 texture;
      kRenderer_bindTexture(&texture, grayScaleBuffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 4);
      delete[]grayScaleBuffer;
      fontMaps.fonts[fontMaps.length].character[c + 128].character = c;
      fontMaps.fonts[fontMaps.length].character[c + 128].textureId = texture;
      fontMaps.fonts[fontMaps.length].character[c + 128].startPos = iv2(face->glyph->bitmap_left, 48 - face->glyph->bitmap_top); // will have to test this
      fontMaps.fonts[fontMaps.length].character[c + 128].width = face->glyph->bitmap.width;
      fontMaps.fonts[fontMaps.length].character[c + 128].height = face->glyph->bitmap.rows;
      fontMaps.fonts[fontMaps.length].character[c + 128].advance = (u8)face->glyph->advance.x >> 6;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  fontMaps.length++;
  FT_Done_Face(face);
}

void kRenderer_setDrawColor(v4 color)
{
  kRenderer_WindowsContexts.windows[currentContext].context->currentColor = color;
}
void kRenderer_clear(v4 color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
void kRenderer_clearGray(f32 gray)
{
  kRenderer_clear(v4(gray, gray, gray, 1));
}

void kRenderer_drawRect(v2 points[4])
{
  v2 p[3] = {points[0], points[1], points[3]};
  kRenderer_drawTriangle(p);
  p[1] = points[2];
  kRenderer_drawTriangle(p);
}
void kRenderer_drawRectV4(v4 rect)
{
  v2 points[4] = {toV2(rect), v2(rect.x, rect.y + rect.height),
                  v2(rect.x + rect.width, rect.y),
                  v2(rect.x + rect.width, rect.y + rect.height)};
  kRenderer_drawRect(points);
}
void kRenderer_drawTriangle(v2 points[3])
{
  struct corner
  {
    v3 pos;
    f32 b;
    v4 color;
  };
  struct corner corners[3];
  for (u32 i = 0; i < 3; i++)
  {
    v4 rP = {points[i].x, points[i].y, 0.0f, 1.0f};
    rP = V4MultiplyM4(rP, kRenderer_getMapper());
    struct corner a = {v3(rP.x, rP.y, rP.z),
                       0.0f,
                       kRenderer_WindowsContexts.windows[currentContext].context->currentColor};
    corners[i] = a;
  }

  u32 VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(defaultVAO);                                               // setup variables in gpu memory
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8 * 3, corners, GL_STATIC_DRAW); // initialise value VBO

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
  glEnableVertexAttribArray(2);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(defaultVAO);
  glUseProgram(defaultShader);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDeleteBuffers(1, &VBO);
}
void kRenderer_drawLine(v2 points[2])
{
  struct corner
  {
    v3 pos;
    f32 b;
    v4 color;
  };
  struct corner corners[2];
  for (u32 i = 0; i < 2; i++)
  {
    v4 rP = {points[i].x, points[i].y, 0.0f, 1.0f};
    rP = V4MultiplyM4(rP, kRenderer_getMapper());
    struct corner a = {v3(rP.x, rP.y, 0.0f),
                       false,
                       kRenderer_WindowsContexts.windows[currentContext].context->currentColor};
    corners[i] = a;
  }

  u32 VBO;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenBuffers(1, &VBO);
  glBindVertexArray(defaultVAO); // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8 * 2, corners, GL_STATIC_DRAW); // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
  glEnableVertexAttribArray(2);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(defaultVAO);
  glUseProgram(defaultShader);
  glDrawArrays(GL_LINES, 0, 2);
  glDeleteBuffers(1, &VBO);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void kRenderer_drawLineLoop(v2 *points, i32 numPoints)
{
  for (u32 i = 0; i < numPoints; i++)
  {
    v2 lines[2] = {points[i], points[(i + 1) % 4]};
    kRenderer_drawLine(lines);
  }
}
void kRenderer_drawPoint(v2 point) // very slow
{
  kRenderer_drawRectV4(v4(point.x, point.y, 1, 1));
}
void kRenderer_drawCircle(v2 startPoint, f32 radius)
{
  for (i32 a = 0; a < 360; a++)
  {
    float angle = a * PIf / 180;
    v2 line[2] = {startPoint, V2AddV2(startPoint, v2(fast_cos(angle) * radius, fast_sin(angle) * radius))};
    kRenderer_drawLine(line);
  }
}

void kRenderer_drawBuffer_defaultShader(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                                        v3 position, i32 width, i32 height, v3 rotation)
{
  u32 texture;
  kRenderer_bindTexture(&texture, buffer, bufferWidth, bufferHeight, numChannels);
  kRenderer_drawStoredTexture_defaultShader(texture, position, width, height, rotation);
  kRenderer_unbindTexture(texture);
}
void kRenderer_drawBuffer(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                          void *args[kRenderer_maxShaderPrograms]) // TODO
{
  u32 texture;
  kRenderer_bindTexture(&texture, buffer, bufferWidth, bufferHeight, numChannels);
  kRenderer_drawStoredTexture(texture, args);
  kRenderer_unbindTexture(texture);
}

void kRenderer_bindTexture(u32 *textureIndex, u8 *buffer, i32 realWidth, i32 realHeight, i32 numChannels)
{
  glGenTextures(1, textureIndex);
  glBindTexture(GL_TEXTURE_2D, *textureIndex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (numChannels == 1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RED,
                 GL_UNSIGNED_BYTE, buffer);
  }
  else if (numChannels == 3)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, realWidth, realHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, buffer);
  }
  else if (numChannels == 4)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, buffer);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
}
void kRenderer_drawStoredTexture_defaultShader(u32 textureIndex, v3 position, i32 width, i32 height, v3 rotation)
{
  struct corner
  {
    v3 pos;
    f32 b;
    v4 texCoord;
  };
  v3 points[4] = {v3(-width / 2.0f, -height / 2.0f, 0), v3(width / 2.0f, -height / 2.0f, 0), v3(-width / 2.0f, height / 2.0f, 0), v3(width / 2.0f, height / 2.0f, 0)};
  struct corner corners[4];

  m4 mat = M4InitDiagonal(1.0f);
  mat = M4MultiplyM4(mat, kRenderer_getMapper());

  mat = M4MultiplyM4(mat, M4TranslateV3(position));
  mat = M4MultiplyM4(mat, M4TranslateV3(v3(width / 2.f, height / 2.f, 0.f)));

  mat = M4MultiplyM4(mat, M4Rotate(-rotation.x, v3(1.0f, 0.0f, 0.0f)));
  mat = M4MultiplyM4(mat, M4Rotate(-rotation.y, v3(0.0f, 1.0f, 0.0f)));
  mat = M4MultiplyM4(mat, M4Rotate(-rotation.z, v3(0.0f, 0.0f, 1.0f)));

  mat = M4MultiplyM4(mat, M4ScaleV3(v3(1.0f, -1.0f, 1.0f)));

  for (u32 i = 0; i < 4; i++)
  {
    v4 rP = {points[i].x, points[i].y, points[i].z, 1.0f};
    rP = V4MultiplyM4(rP, mat);
    struct corner a = {toV3(rP),
                       1.0f,
                       v4(1, 1, 1, 1)};
    corners[i] = a;
  }

  corners[0].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);
  corners[1].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);
  corners[2].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);
  corners[3].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);

  u32 VBO;
  struct corner corners1[6] = {corners[0], corners[1], corners[3], corners[0], corners[2], corners[3]};

  glBindTexture(GL_TEXTURE_2D, textureIndex);

  glGenBuffers(1, &VBO);
  glBindVertexArray(defaultVAO); // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8 * 6, corners1, GL_STATIC_DRAW); // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
  glEnableVertexAttribArray(2);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(defaultVAO);
  glUseProgram(defaultShader);
  glUniform4f(glGetUniformLocation(defaultShader, "colorScale"), kRenderer_WindowsContexts.windows[currentContext].context->currentColor.r, kRenderer_WindowsContexts.windows[currentContext].context->currentColor.g, kRenderer_WindowsContexts.windows[currentContext].context->currentColor.b, kRenderer_WindowsContexts.windows[currentContext].context->currentColor.a);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glUniform4f(glGetUniformLocation(defaultShader, "colorScale"), 1.0, 1.0, 1.0, 1.0);
  glDeleteBuffers(1, &VBO);
}
void kRenderer_drawStoredTexture(u32 textureIndex, void *args[kRenderer_maxShaderPrograms][4]) // TODO
{

  // struct corner {v3 pos;f32 b;v4 color;};

  // f32 vertices[] = {0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  //                   0.5, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //                   -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //                   -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  // u32 indices[] = {0, 1, 3, 0, 2, 3};
  // u32 VBO, VAO, EBO;
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glBindVertexArray(VAO); // setup variables in gpu memory
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
  //              GL_STATIC_DRAW); // initialise value VBO
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //                       (void *)0); // 3 values xyz,size for one point
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(
  //     1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //     (void *)(3 * sizeof(float))); // 4 values rgba,size for one point
  // glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //                       (void *)(7 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // u32 texture;
  // glGenTextures(1, &texture);
  // glBindTexture(GL_TEXTURE_2D, texture);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, texture);
  // if (numChannels == 3)
  // {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, realWidth, realHeight, 0, GL_RGB,
  //                GL_UNSIGNED_BYTE, colors);
  // }
  // else
  // {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RGBA,
  //                GL_UNSIGNED_BYTE, colors);
  // }
  // glGenerateMipmap(GL_TEXTURE_2D);

  // // draw------
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // iv2 windowSize = kRenderer_getWindowSize();
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, texture);

  // glm::mat4 transform = glm::mat4(1.0f);
  // transform = glm::translate(
  //     transform,
  //     glm::vec3(
  //         (float)((position.x + width / 2) / kRenderer_getWindowWidth()) *
  //                 2.0f -
  //             1.0f,
  //         (float)-((position.y + height / 2) / kRenderer_getWindowHeight()) *
  //                 2.0f +
  //             1.0f,
  //         0.0f));
  // transform = glm::scale(
  //     transform,
  //     glm::vec3((float)(((float)width) / windowSize.x) * 2.0f,
  //               (float)-(((float)height) / windowSize.y) * 2.0f, 1.0f));
  // transform = glm::rotate(transform, -rotation.x,
  //                         glm::vec3(1.0f, 0.0f, 0.0f));
  // transform = glm::rotate(transform, -rotation.y,
  //                         glm::vec3(0.0f, 1.0f, 0.0f));
  // transform = glm::rotate(transform, -rotation.z,
  //                         glm::vec3(0.0f, 0.0f, 1.0f));

  // glUseProgram(shaderProgram);
  // u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  // // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBindVertexArray(VAO);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // // transform = glm::mat4(1.0f);
  // // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  // glDeleteTextures(1, &texture);
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
}
void kRenderer_unbindTexture(u32 textureIndex)
{
  glDeleteTextures(1, &textureIndex);
}

void kRenderer_setFont(const char *fontName)
{
  for (u32 i = 0; i < fontMaps.length; i++)
  {
    if (CStringMatchCaseInsensitive(fontMaps.fonts[i].font_name, fontName))
    {
      fontMaps.currentFont = i;
    }
  }
}
void kRenderer_displayText(v3 position, v3 rotation, const char *text, f32 scale)
{
  u32 charactersIndexs[1024];
  size_t textLength = 0;
  u32 pixWidth = 0;
  u32 pixHeight = 0;

  for (const char *c = text; *c != '\0'; c++, textLength++)
  {
    u8 cIndex = (*c + 128) % 256;
    if (fontMaps.fonts[fontMaps.currentFont].character[cIndex].character != *c)
    {
      for (u32 i = 0; i < 256; i++)
      {
        if (*c == fontMaps.fonts[fontMaps.currentFont].character[i].character)
        {
          pixWidth += fontMaps.fonts[fontMaps.currentFont].character[i].advance;
          charactersIndexs[textLength] = i;
          if ((i32)pixHeight < fontMaps.fonts[fontMaps.currentFont].character[i].height - fontMaps.fonts[fontMaps.currentFont].character[i].startPos.y)
            pixHeight = fontMaps.fonts[fontMaps.currentFont].character[i].height - fontMaps.fonts[fontMaps.currentFont].character[i].startPos.y;
        }
      }
    }
    else
    {
      pixWidth += fontMaps.fonts[fontMaps.currentFont].character[cIndex].advance;
      charactersIndexs[textLength] = cIndex;
      if ((i32)pixHeight < fontMaps.fonts[fontMaps.currentFont].character[cIndex].height - fontMaps.fonts[fontMaps.currentFont].character[cIndex].startPos.y)
        pixHeight = fontMaps.fonts[fontMaps.currentFont].character[cIndex].height - fontMaps.fonts[fontMaps.currentFont].character[cIndex].startPos.y;
    }
  }

  v4 pixelPos = V4MultiplyM4(v4(position.x, position.y, position.z, 1),
                             M4Mapper(kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max,
                                      v3(0.f, (f32)kRenderer_getWindowHeight(), 0.f), v3((f32)kRenderer_getWindowWidth(), 0.f, 1.f)));

  m4 mat = M4InitDiagonal(1);
  mat = M4MultiplyM4(mat, M4Mapper(v3(0.f, (f32)kRenderer_getWindowHeight(), 0.f), v3((f32)kRenderer_getWindowWidth(), 0.f, 1.f), v3(-1.f, -1.f, -1.f), v3(1.f, 1.f, 1.f)));
  mat = M4MultiplyM4(mat, M4TranslateV3(toV3(pixelPos)));
  mat = M4MultiplyM4(mat, M4ScaleV3(v3(scale, scale, 1)));
  mat = M4MultiplyM4(mat, M4TranslateV3(v3(pixWidth / 2.f, pixHeight / 2.f, 0.f)));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.x, v3(1.f, 0.f, 0.f)));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.y, v3(0.f, 1.f, 0.f)));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.z, v3(0.f, 0.f, 1.f)));
  mat = M4MultiplyM4(mat, M4TranslateV3(v3(-(pixWidth / 2.f), -(pixHeight / 2.f), 0)));

  u32 widthOffset = 0;
  struct corner
  {
    v3 pos;
    f32 b;
    v4 texCoord;
  };
  for (u32 i = 0; i < textLength; i++)
  {
    v3 characterPos = toV3(pixelPos);
    characterPos.x += widthOffset;
    characterPos = V3AddV3(characterPos, v3((f32)fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].startPos.x,
                                            (f32)fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].startPos.y, 0.f));
    v3 texturePoints[4] = {
        characterPos,
        V3AddV3(characterPos, v3(fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].width, 0, 0)),
        V3AddV3(characterPos, v3(0, fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].height, 0)),
        V3AddV3(characterPos, v3(fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].width,
                                 fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].height, 0))};
    struct corner corners[4];
    for (u32 j = 0; j < 4; j++)
    {
      v4 rP = v4(texturePoints[j].x, texturePoints[j].y, texturePoints[j].z, 1);
      rP = V4MultiplyM4(rP, mat);
      struct corner a = {toV3(rP), 1.0f, v4(0, 0, 0, 0)};
      corners[j] = a;
    }
    corners[0].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);
    corners[1].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);
    corners[2].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);
    corners[3].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);
    if (1) // drawing of character
    {
      u32 VBO1, VBO2;
      struct corner corners1[3] = {corners[0], corners[1], corners[3]};
      struct corner corners2[3] = {corners[0], corners[2], corners[3]};

      glBindTexture(GL_TEXTURE_2D, fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].textureId);

      glGenBuffers(1, &VBO1);
      glBindVertexArray(defaultVAO); // setup variables in gpu memory
      glBindBuffer(GL_ARRAY_BUFFER, VBO1);
      glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8 * 3, corners1, GL_STATIC_DRAW); // initialise value VBO
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
      glEnableVertexAttribArray(2);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBindVertexArray(defaultVAO);
      glUseProgram(defaultShader);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDeleteBuffers(1, &VBO1);

      glGenBuffers(1, &VBO2);
      glBindVertexArray(defaultVAO); // setup variables in gpu memory
      glBindBuffer(GL_ARRAY_BUFFER, VBO2);
      glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 8 * 3, corners2, GL_STATIC_DRAW); // initialise value VBO
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
      glEnableVertexAttribArray(2);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBindVertexArray(defaultVAO);
      glUseProgram(defaultShader);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDeleteBuffers(1, &VBO2);
    }

    widthOffset += fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].advance;
  }
}

// key input
bool kRenderer_keyStatusPressed(Key e)
{
  return keysPressed[e];
}
v2 kRenderer_cursorPosition()
{
  v4 v = v4(mousePos.x, mousePos.y, 0, 1);
  v = V4MultiplyM4(v, M4Mapper(v3(0, kRenderer_getWindowHeight(), 0), v3(kRenderer_getWindowWidth(), 0, 1), kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max));
  return v2(v.x, v.y);
}
bool kRenderer_mouseStatusPressed(Key e)
{
  return keysPressed[e];
}

i32 kRenderer_getWindowWidth()
{
  return glutGet(GLUT_WINDOW_WIDTH);
}
i32 kRenderer_getWindowHeight()
{
  return glutGet(GLUT_WINDOW_HEIGHT);
}
iv2 kRenderer_getWindowSize()
{
  return iv2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
f64 kRenderer_getTimeSinceLastFrame()
{
  // return glfwGetTime() - lastTime;
  return 0.16;
}