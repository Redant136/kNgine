#include <math.h>
#include <glad/glad.h>
#define utils_StrManipulation
#include "utils.h"
#ifndef kRenderer_HeaderOnly
#include "kRenderer.h"
#endif
#ifdef __unix__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>

static v3 kRenderer_rotation = {0, 0, 0};

// fonts
static FT_Library ft;
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

// window info
static struct
{
  size_t length;
  struct
  {
    i32 shaderPrograms[kRenderer_maxShaderPrograms];
    v3 min;
    v3 max;
    kRenderer_WindowContext *context;
    GLFWwindow *window;
    struct
    {
      // number of bound objects
      size_t length;
      struct
      {
        kRenderer_RendererObject boundObjects;
        struct
        {
          u32 VBO;
          u32 VAO;
        } shaderData[kRenderer_maxShaderPrograms];
      } objectData[kRenderer_maxBoundObjects];
    } kRenderer_boundObjects;
  } windows[kRenderer_maxWindows];
} kRenderer_WindowsContexts;
static u32 currentContext = 0;

static i32 defaultShader;
static u32 defaultVAO = 0;

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

static void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    if (kRenderer_WindowsContexts.windows[i].window == window)
    {
      if (fCompare(kRenderer_WindowsContexts.windows[i].min.y, kRenderer_WindowsContexts.windows[i].context->height) &&
          fCompare(kRenderer_WindowsContexts.windows[i].max.x, kRenderer_WindowsContexts.windows[i].context->width))
      {
        kRenderer_WindowsContexts.windows[i].min.y = (f32)height;
        kRenderer_WindowsContexts.windows[i].max.x = (f32)width;
      }
      kRenderer_WindowsContexts.windows[i].context->windowSize = iv2(width, height);
    }
  }
  glViewport(0, 0, width, height);
}
static i32 KeyToGLFW(Key k)
{
  switch (k)
  {
  case KEY_SPACE:
    return GLFW_KEY_SPACE;
  case KEY_APOSTROPHE:
    return GLFW_KEY_APOSTROPHE /* ' */;
  case KEY_COMMA:
    return GLFW_KEY_COMMA /* , */;
  case KEY_MINUS:
    return GLFW_KEY_MINUS /* - */;
  case KEY_PERIOD:
    return GLFW_KEY_PERIOD /* . */;
  case KEY_SLASH:
    return GLFW_KEY_SLASH /* / */;
  case KEY_0:
    return GLFW_KEY_0;
  case KEY_1:
    return GLFW_KEY_1;
  case KEY_2:
    return GLFW_KEY_2;
  case KEY_3:
    return GLFW_KEY_3;
  case KEY_4:
    return GLFW_KEY_4;
  case KEY_5:
    return GLFW_KEY_5;
  case KEY_6:
    return GLFW_KEY_6;
  case KEY_7:
    return GLFW_KEY_7;
  case KEY_8:
    return GLFW_KEY_8;
  case KEY_9:
    return GLFW_KEY_9;
  case KEY_SEMICOLON:
    return GLFW_KEY_SEMICOLON /* ; */;
  case KEY_EQUAL:
    return GLFW_KEY_EQUAL /* = */;
  case KEY_A:
    return GLFW_KEY_A;
  case KEY_B:
    return GLFW_KEY_B;
  case KEY_C:
    return GLFW_KEY_C;
  case KEY_D:
    return GLFW_KEY_D;
  case KEY_E:
    return GLFW_KEY_E;
  case KEY_F:
    return GLFW_KEY_F;
  case KEY_G:
    return GLFW_KEY_G;
  case KEY_H:
    return GLFW_KEY_H;
  case KEY_I:
    return GLFW_KEY_I;
  case KEY_J:
    return GLFW_KEY_J;
  case KEY_K:
    return GLFW_KEY_K;
  case KEY_L:
    return GLFW_KEY_L;
  case KEY_M:
    return GLFW_KEY_M;
  case KEY_N:
    return GLFW_KEY_N;
  case KEY_O:
    return GLFW_KEY_O;
  case KEY_P:
    return GLFW_KEY_P;
  case KEY_Q:
    return GLFW_KEY_Q;
  case KEY_R:
    return GLFW_KEY_R;
  case KEY_S:
    return GLFW_KEY_S;
  case KEY_T:
    return GLFW_KEY_T;
  case KEY_U:
    return GLFW_KEY_U;
  case KEY_V:
    return GLFW_KEY_V;
  case KEY_W:
    return GLFW_KEY_W;
  case KEY_X:
    return GLFW_KEY_X;
  case KEY_Y:
    return GLFW_KEY_Y;
  case KEY_Z:
    return GLFW_KEY_Z;
  case KEY_LEFT_BRACKET:
    return GLFW_KEY_LEFT_BRACKET /* [ */;
  case KEY_BACKSLASH:
    return GLFW_KEY_BACKSLASH /* \ */;
  case KEY_RIGHT_BRACKET:
    return GLFW_KEY_RIGHT_BRACKET /* ] */;
  case KEY_GRAVE_ACCENT:
    return GLFW_KEY_GRAVE_ACCENT /* ` */;
  case KEY_ESCAPE:
    return GLFW_KEY_ESCAPE;
  case KEY_ENTER:
    return GLFW_KEY_ENTER;
  case KEY_TAB:
    return GLFW_KEY_TAB;
  case KEY_BACKSPACE:
    return GLFW_KEY_BACKSPACE;
  case KEY_INSERT:
    return GLFW_KEY_INSERT;
  case KEY_DELETE:
    return GLFW_KEY_DELETE;
  case KEY_RIGHT:
    return GLFW_KEY_RIGHT;
  case KEY_LEFT:
    return GLFW_KEY_LEFT;
  case KEY_DOWN:
    return GLFW_KEY_DOWN;
  case KEY_UP:
    return GLFW_KEY_UP;
  case KEY_F1:
    return GLFW_KEY_F1;
  case KEY_F2:
    return GLFW_KEY_F2;
  case KEY_F3:
    return GLFW_KEY_F3;
  case KEY_F4:
    return GLFW_KEY_F4;
  case KEY_F5:
    return GLFW_KEY_F5;
  case KEY_F6:
    return GLFW_KEY_F6;
  case KEY_F7:
    return GLFW_KEY_F7;
  case KEY_F8:
    return GLFW_KEY_F8;
  case KEY_F9:
    return GLFW_KEY_F9;
  case KEY_F10:
    return GLFW_KEY_F10;
  case KEY_F11:
    return GLFW_KEY_F11;
  case KEY_F12:
    return GLFW_KEY_F12;
  case KEY_F13:
    return GLFW_KEY_F13;
  case KEY_F14:
    return GLFW_KEY_F14;
  case KEY_F15:
    return GLFW_KEY_F15;
  case KEY_F16:
    return GLFW_KEY_F16;
  case KEY_F17:
    return GLFW_KEY_F17;
  case KEY_F18:
    return GLFW_KEY_F18;
  case KEY_F19:
    return GLFW_KEY_F19;
  case KEY_F20:
    return GLFW_KEY_F20;
  case KEY_F21:
    return GLFW_KEY_F21;
  case KEY_F22:
    return GLFW_KEY_F22;
  case KEY_F23:
    return GLFW_KEY_F23;
  case KEY_F24:
    return GLFW_KEY_F24;
  case KEY_F25:
    return GLFW_KEY_F25;
  case KEY_LEFT_SHIFT:
    return GLFW_KEY_LEFT_SHIFT;
  case KEY_LEFT_CONTROL:
    return GLFW_KEY_LEFT_CONTROL;
  case KEY_LEFT_ALT:
    return GLFW_KEY_LEFT_ALT;
  case KEY_LEFT_SUPER:
    return GLFW_KEY_LEFT_SUPER;
  case KEY_RIGHT_SHIFT:
    return GLFW_KEY_RIGHT_SHIFT;
  case KEY_RIGHT_CONTROL:
    return GLFW_KEY_RIGHT_CONTROL;
  case KEY_RIGHT_ALT:
    return GLFW_KEY_RIGHT_ALT;
  default:
    return GLFW_KEY_UNKNOWN;
  }
}
static i32 MouseToGLFW(Key k)
{
  switch (k)
  {
  case MOUSE_BUTTON1:
    return GLFW_MOUSE_BUTTON_1;
  case MOUSE_BUTTON2:
    return GLFW_MOUSE_BUTTON_2;
  case MOUSE_BUTTON3:
    return GLFW_MOUSE_BUTTON_3;
  case MOUSE_BUTTON4:
    return GLFW_MOUSE_BUTTON_4;
  case MOUSE_BUTTON5:
    return GLFW_MOUSE_BUTTON_5;
  case MOUSE_BUTTON6:
    return GLFW_MOUSE_BUTTON_6;
  case MOUSE_BUTTON7:
    return GLFW_MOUSE_BUTTON_7;
  case MOUSE_BUTTON8:
    return GLFW_MOUSE_BUTTON_8;
  default:
    return GLFW_MOUSE_BUTTON_1;
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
#ifdef __cplusplus
        switch (kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shaderType)
        {
        case kRenderer_shaderTypes::kRenderer_SHADER_Vertex:
          shaderType = GL_VERTEX_SHADER;
          break;
        case kRenderer_shaderTypes::kRenderer_SHADER_Fragment:
          shaderType = GL_FRAGMENT_SHADER;
          break;
        case kRenderer_shaderTypes::kRenderer_SHADER_Geometry:
          shaderType = GL_GEOMETRY_SHADER;
          break;
        default:
          shaderType = GL_VERTEX_SHADER;
          break;
        }
#else
        switch (kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shaderType)
        {
        case kRenderer_SHADER_Vertex:
          shaderType = GL_VERTEX_SHADER;
          break;
        case kRenderer_SHADER_Fragment:
          shaderType = GL_FRAGMENT_SHADER;
          break;
        case kRenderer_SHADER_Geometry:
          shaderType = GL_GEOMETRY_SHADER;
          break;
        default:
          shaderType = GL_VERTEX_SHADER;
          break;
        }
#endif
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
static void emptyFunction() {}

i32 kRenderer_init(i32 argc, const char **argv)
{
  if (!glfwInit())
  {
    assert(0 && "could not create context");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  return 0;
}
i32 kRenderer_createContext(kRenderer_WindowContext *context)
{
  kRenderer_WindowContext c = {
      {1,                                       // 1 shader
       {                                        // array of shaders
        {                                       // shader def
         {3,{3,1,4}},
         2,                                     // number of individual shaders
         {                                      // array of individual shaders
          {kRenderer_SHADER_Vertex, vertexShaderSource},
          {kRenderer_SHADER_Fragment, defaultFragmentShaderSource}}}}},
      "kRenderer",
      iv2(1920, 1080),
      v4(1, 1, 1, 1),
      true,
      emptyFunction,
      emptyFunction};
  *context = c;
  assert(kRenderer_WindowsContexts.length < kRenderer_maxWindows);
  currentContext = (u32)kRenderer_WindowsContexts.length;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].context = context;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].min = v3(0, 1080, 0);
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].max = v3(1920, 0, 1);
  // kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].kRenderer_boundObjects.length = 0;

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
  GLFWwindow *window = glfwCreateWindow(context->width, context->height, context->name, NULL, NULL);
  if (window == NULL)
  {
    glfwTerminate();
    assert(0 && "cannot create window");
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    assert(0 && "could not initialize GLAD");
    return -1;
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
  if (defaultVAO == 0)
  {
    glGenVertexArrays(1, &defaultVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(sizeof(f32) * 4));
    glEnableVertexAttribArray(2);
  }
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
  if (kRenderer_WindowsContexts.windows[currentContext].window != NULL)
  {
    glViewport(0, 0, width, height);
  }
}
void kRenderer_setWindowName(const char *windowName)
{
  kRenderer_WindowsContexts.windows[currentContext].context->name = windowName;
  if (kRenderer_WindowsContexts.windows[currentContext].window != NULL)
  {
    glfwSetWindowTitle(kRenderer_WindowsContexts.windows[currentContext].window, windowName);
  }
}
void kRenderer_setWindowVSync(u8 vSync)
{
  kRenderer_WindowsContexts.windows[currentContext].context->vSync = vSync;
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
  compileShaders();
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
    glfwMakeContextCurrent(kRenderer_WindowsContexts.windows[i].window);
    glClear(GL_COLOR_BUFFER_BIT);
    currentContext = i;
    kRenderer_WindowsContexts.windows[i].context->init();
    glfwSwapBuffers(kRenderer_WindowsContexts.windows[i].window);
    glfwPollEvents();
  }
  bool running = true;
  while (running)
  {
    running = false;
    double time = glfwGetTime();
    for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
    {
      glfwMakeContextCurrent(kRenderer_WindowsContexts.windows[i].window);
      glClear(GL_COLOR_BUFFER_BIT);
      currentContext = i;
      kRenderer_WindowsContexts.windows[i].context->draw();
      glfwSwapInterval(kRenderer_WindowsContexts.windows[i].context->vSync);
      glfwSwapBuffers(kRenderer_WindowsContexts.windows[i].window);
      glfwPollEvents();
      running |= !glfwWindowShouldClose(kRenderer_WindowsContexts.windows[i].window);
    }
    lastTime = time;
  }
  glfwTerminate();
}

//---------------------------------

void kRenderer_rotateViewport(v3 angle)
{
  kRenderer_rotation = angle;
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
      u8 *grayScaleBuffer = (u8 *)malloc(sizeof(u8) * face->glyph->bitmap.width * face->glyph->bitmap.rows * 4);
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
      free(grayScaleBuffer);
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
  v2 points[4] = {toV2(rect),
                  v2(rect.x, rect.y + rect.height),
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
                                        v3 position, f32 width, f32 height, v3 rotation)
{
  u32 texture;
  kRenderer_bindTexture(&texture, buffer, bufferWidth, bufferHeight, numChannels);
  kRenderer_drawStoredTexture_defaultShader(texture, position, width, height, rotation);
  kRenderer_unbindTexture(texture);
}
// void kRenderer_drawBuffer(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
//                           void *args[kRenderer_maxShaderPrograms][4]) // TODO
// {
//   u32 texture;
//   kRenderer_bindTexture(&texture, buffer, bufferWidth, bufferHeight, numChannels);
//   kRenderer_drawStoredTexture(texture, args);
//   kRenderer_unbindTexture(texture);
// }

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
void kRenderer_drawStoredTexture_defaultShader(u32 textureIndex, v3 position, f32 width, f32 height, v3 rotation)
{
  struct corner
  {
    v3 pos;
    f32 b;
    v4 texCoord;
  };
  v3 points[4]={position,v3(position.x+width,position.y,position.z),v3(position.x,position.y+height,position.z),v3(position.x+width,position.y+height,position.z)};
  struct corner corners[4];

  v2 windowSize=V2MultiplyF32(toV2(kRenderer_getWindowSize()),0.5);

  m4 mat = M4InitDiagonal(1.0f);
  mat = M4MultiplyM4(mat, kRenderer_getMapper());
  mat = M4MultiplyM4(mat,M4TranslateV3(v3(width/2.f,height/2,0)));
  mat = M4MultiplyM4(mat,M4TranslateV3(position));
  mat = M4MultiplyM4(mat, M4Mapper(v3(-windowSize.x, -windowSize.y, 0),
                                   v3(windowSize.x, windowSize.y, 1),
                                   kRenderer_WindowsContexts.windows[currentContext].min,
                                   kRenderer_WindowsContexts.windows[currentContext].max));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.x, v3(1.0f, 0.0f, 0.0f)));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.y, v3(0.0f, 1.0f, 0.0f)));
  mat = M4MultiplyM4(mat, M4Rotate(rotation.z, v3(0.0f, 0.0f, 1.0f)));
  mat = M4MultiplyM4(mat, M4Mapper(kRenderer_WindowsContexts.windows[currentContext].min,
                                   kRenderer_WindowsContexts.windows[currentContext].max,
                                   v3(-windowSize.x, -windowSize.y, 0),
                                   v3(windowSize.x, windowSize.y, 1)));
  mat = M4MultiplyM4(mat,M4TranslateV3(V3MultiplyF32(position,-1)));
  mat = M4MultiplyM4(mat,M4TranslateV3(v3(-width/2.f,-height/2,0)));

  for (u32 i = 0; i < 4; i++)
  {
    v4 rP = {points[i].x, points[i].y, points[i].z, 1.0f};
    rP = V4MultiplyM4(rP, mat);
    struct corner a = {toV3(rP),
                       1.0f,
                       v4(1, 1, 1, 1)};
    corners[i] = a;
  }

  corners[0].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);
  corners[1].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);
  corners[2].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);
  corners[3].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);

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
void kRenderer_drawStoredTexture(u32 textureIndex, size_t numShaders, u32 *shaders, f32 **args) // TODO
{
  // there will be a problem when converting to shaderElement, prob 2 i for loop

//   kRenderer_WindowContext *current = kRenderer_WindowsContexts.windows[currentContext].context;
//   for (u32 i = 0; i < numShaders; i++)
//   {
//     u32 VBO, VAO;
//     glBindTexture(GL_TEXTURE_2D, textureIndex);
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     struct shaderElement
//     {
//       u32 numElements;
//       f32 elements[kStruct_arg_def_max_length * 4]; // times 4 because v4
//     };
//     struct shaderElement formatedArgs[4];
//     for (u32 e = 0; e < 4; e++)
//     {
//       formatedArgs[e].numElements = 0;
//       for (u32 j = 0; j < current->windowShaders.programs[shaders[i]].argsDef.length; j++)
//       {
// #ifdef __cplusplus
//         if (current->windowShaders.programs[shaders[i]].argsDef.args[j] >= kTypes::kTYPE_v2)
// #else
//         if (current->windowShaders.programs[shaders[i]].argsDef.args[j] >= kTYPE_v2)
// #endif
//         {
//           if (kType_sizeOf(current->windowShaders.programs[shaders[i]].argsDef.args[j]) == 8)
//           {
//             v2 v;
//             kCastType(v2, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[shaders[i]].argsDef, args), current->windowShaders.programs[shaders[i]].argsDef.args[j]);
//             formatedArgs[e].elements[formatedArgs[e].numElements] = v.x;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 1] = v.y;
//             formatedArgs[e].numElements += 2;
//           }
//           else if (kType_sizeOf(current->windowShaders.programs[shaders[i]].argsDef.args[j]) == 12)
//           {
//             v3 v;
//             kCastType(v3, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[i].argsDef, args), current->windowShaders.programs[shaders[i]].argsDef.args[j]);
//             formatedArgs[e].elements[formatedArgs[e].numElements] = v.x;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 1] = v.y;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 2] = v.z;
//             formatedArgs[e].numElements += 3;
//           }
//           else if (kType_sizeOf(current->windowShaders.programs[shaders[i]].argsDef.args[j]) == 16)
//           {
//             v4 v;
//             kCastType(v4, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[shaders[i]].argsDef, args), current->windowShaders.programs[shaders[i]].argsDef.args[j]);
//             formatedArgs[e].elements[formatedArgs[e].numElements] = v.x;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 1] = v.y;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 2] = v.z;
//             formatedArgs[e].elements[formatedArgs[e].numElements + 3] = v.w;
//             formatedArgs[e].numElements += 4;
//           }
//         }
//         else
//         {
//           kCastType(f32, formatedArgs[e].elements[formatedArgs[e].numElements], kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[shaders[i]].argsDef, args), current->windowShaders.programs[shaders[i]].argsDef.args[j]);
//           formatedArgs[e].numElements++;
//         }
//       }
//     }
//     struct shaderElement corners[6] = {formatedArgs[0], formatedArgs[1], formatedArgs[3], formatedArgs[0], formatedArgs[2], formatedArgs[3]};
//     glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * formatedArgs[0].numElements, corners, GL_STATIC_DRAW);
//     u32 currentElementsIndex;
//     for (u32 j = 0; j < current->windowShaders.programs[shaders[i]].argsDef.length; j++)
//     {
// #ifdef __cplusplus
//       if (current->windowShaders.programs[shaders[i]].argsDef.args[j] >= kTypes::kTYPE_v2)
// #else
//       if (current->windowShaders.programs[shaders[i]].argsDef.args[j] >= kTYPE_v2)
// #endif
//       {
//         u8 offset = kType_sizeOf(current->windowShaders.programs[shaders[i]].argsDef.args[j]) / 4;
//         glVertexAttribPointer(j, offset, GL_FLOAT, GL_FALSE, sizeof(f32) * formatedArgs[0].numElements, (void *)(sizeof(f32) * currentElementsIndex));
//         currentElementsIndex += offset;
//       }
//       else
//       {
//         glVertexAttribPointer(j, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * formatedArgs[0].numElements, (void *)(sizeof(f32) * currentElementsIndex));
//         currentElementsIndex++;
//       }
//       glEnableVertexAttribArray(j);
//     }
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     glBindVertexArray(VAO);
//     glUseProgram(kRenderer_WindowsContexts.windows[currentContext].shaderPrograms[i]);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
//     glBindVertexArray(0);
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//   }

  //   for (u32 i = 0; i < current->windowShaders.length; i++)
  //   {
  //     u32 VBO, VAO;
  //     glBindTexture(GL_TEXTURE_2D, textureIndex);
  //     glGenVertexArrays(1, &VAO);
  //     glGenBuffers(1, &VBO);
  //     glBindVertexArray(VAO);
  //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //     struct shaderElement
  //     {
  //       u32 numElements;
  //       f32 elements[kStruct_arg_def_max_length * 4]; // times 4 because v4
  //     };
  //     struct shaderElement formatedArgs[4];
  //     for (u32 e = 0; e < 4; e++)
  //     {
  //       formatedArgs[e].numElements = 0;
  //       for (u32 j = 0; j < current->windowShaders.programs[i].argsDef.length; j++)
  //       {
  // #ifdef __cplusplus
  //         if (current->windowShaders.programs[i].argsDef.args[j] >= kTypes::kTYPE_v2)
  // #else
  //         if (current->windowShaders.programs[i].argsDef.args[j] >= kTYPE_v2)
  // #endif
  //         {
  //           if(kType_sizeOf(current->windowShaders.programs[i].argsDef.args[j])==8){
  //             v2 v;
  //             kCastType(v2, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[i].argsDef, args), current->windowShaders.programs[i].argsDef.args[j]);
  //             formatedArgs[e].elements[formatedArgs[e].numElements]=v.x;
  //             formatedArgs[e].elements[formatedArgs[e].numElements+1] = v.y;
  //             formatedArgs[e].numElements+=2;
  //           }
  //           else if (kType_sizeOf(current->windowShaders.programs[i].argsDef.args[j]) == 12)
  //           {
  //             v3 v;
  //             kCastType(v3, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[i].argsDef, args), current->windowShaders.programs[i].argsDef.args[j]);
  //             formatedArgs[e].elements[formatedArgs[e].numElements]=v.x;
  //             formatedArgs[e].elements[formatedArgs[e].numElements+1] = v.y;
  //             formatedArgs[e].elements[formatedArgs[e].numElements + 2] = v.z;
  //             formatedArgs[e].numElements+=3;
  //           }
  //           else if (kType_sizeOf(current->windowShaders.programs[i].argsDef.args[j]) == 16)
  //           {
  //             v4 v;
  //             kCastType(v4, v, kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[i].argsDef, args), current->windowShaders.programs[i].argsDef.args[j]);
  //             formatedArgs[e].elements[formatedArgs[e].numElements] = v.x;
  //             formatedArgs[e].elements[formatedArgs[e].numElements + 1] = v.y;
  //             formatedArgs[e].elements[formatedArgs[e].numElements + 2] = v.z;
  //             formatedArgs[e].elements[formatedArgs[e].numElements + 3] = v.w;
  //             formatedArgs[e].numElements += 4;
  //           }
  //         }
  //         else
  //         {
  //           kCastType(f32, formatedArgs[e].elements[formatedArgs[e].numElements], kStruct_getElementOfObjectAtIndex(j, current->windowShaders.programs[i].argsDef, args), current->windowShaders.programs[i].argsDef.args[j]);
  //           formatedArgs[e].numElements++;
  //         }
  //       }
  //     }
  //     struct shaderElement corners[6] = {formatedArgs[0], formatedArgs[1], formatedArgs[3], formatedArgs[0], formatedArgs[2], formatedArgs[3]};
  //     glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * formatedArgs[0].numElements, corners, GL_STATIC_DRAW);
  //     u32 currentElementsIndex;
  //     for (u32 j = 0; j < current->windowShaders.programs[i].argsDef.length; j++)
  //     {
  // #ifdef __cplusplus
  //       if (current->windowShaders.programs[i].argsDef.args[j] >= kTypes::kTYPE_v2)
  // #else
  //       if (current->windowShaders.programs[i].argsDef.args[j] >= kTYPE_v2)
  // #endif
  //       {
  //         u8 offset=kType_sizeOf(current->windowShaders.programs[i].argsDef.args[j])/4;
  //         glVertexAttribPointer(j, offset, GL_FLOAT, GL_FALSE, sizeof(f32) * formatedArgs[0].numElements, (void *)(sizeof(f32) * currentElementsIndex));
  //         currentElementsIndex += offset;
  //       }
  //       else
  //       {
  //         glVertexAttribPointer(j, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * formatedArgs[0].numElements, (void *)(sizeof(f32) * currentElementsIndex));
  //         currentElementsIndex++;
  //       }
  //       glEnableVertexAttribArray(j);
  //     }
  //     glEnable(GL_BLEND);
  //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //     glBindVertexArray(VAO);
  //     glUseProgram(kRenderer_WindowsContexts.windows[currentContext].shaderPrograms[i]);
  //     glDrawArrays(GL_TRIANGLES, 0, 6);
  //     glBindVertexArray(0);
  //     glDeleteVertexArrays(1, &VAO);
  //     glDeleteBuffers(1, &VBO);
  //   }
}
void kRenderer_unbindTexture(u32 textureIndex)
{
  glDeleteTextures(1, &textureIndex);
}

u32 kRenderer_addObject(u32*indexIn,kRenderer_RendererObject obj){
  *indexIn = kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.length;
  u32 index = *indexIn;
  assert(index + 1 < kRenderer_maxBoundObjects);
  kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.length++;
  kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects = obj;
  return index;
}
u32 kRenderer_bindObject(u32 *indexIn, kRenderer_RendererObject obj)
{
  kRenderer_addObject(indexIn,obj);
  kRenderer_loadObject(*indexIn);
  return *indexIn;
}
kRenderer_RendererObject *kRenderer_getBoundObject(u32 index)
{
  return &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects;
}
void kRenderer_loadObject(u32 index){
  const kRenderer_RendererObject* obj = &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects;

  for (u32 s = 0; s < obj->length; s++)
  {
    u32 *VBO = &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[s].VBO;
    u32 *VAO = &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[s].VAO;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    kRenderer_WindowContext *current = kRenderer_WindowsContexts.windows[currentContext].context;
    u32 structSize=0;
    for(u32 i=0;i<current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.argLength;i++){
      structSize += current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i];
    }

    u32 dataSize = structSize * 3 * kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.shaderElements[s].length;
    f32 *data = malloc(dataSize*sizeof(f32));
    for(u32 t=0;t<kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.shaderElements[s].length;t++)
    {
      for(u32 i=0;i<3;i++)
      {
        memcpy(data + t * 3 * structSize + i * structSize, kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.shaderElements[s].triangles[t].arg[i], structSize*sizeof(f32));
      }
    }

    glBufferData(GL_ARRAY_BUFFER,dataSize*sizeof(f32),data,GL_DYNAMIC_DRAW);
    free(data);

    u32 offset=0;
    for (u32 i = 0; i < current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.argLength; i++)
    {
      glVertexAttribPointer(i, current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i], GL_FLOAT, GL_FALSE, sizeof(f32) * structSize, (void *)(sizeof(f32) * offset));
      glEnableVertexAttribArray(i);
      offset += current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i];
    }
  }
}
void kRenderer_updateObjects()
{
  for (u32 i = 0; i < kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.length; i++)
  {
    kRenderer_updateObject(i);
  }
}
void kRenderer_updateObject(u32 index)
{
  const kRenderer_WindowContext *current = kRenderer_WindowsContexts.windows[currentContext].context;
  kRenderer_RendererObject *obj = &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects;
  for(u32 s=0;s<obj->length;s++){
    glBindBuffer(GL_ARRAY_BUFFER, kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[s].VBO);
    for(u32 t=0;t<obj->shaderElements[s].length;t++){
      u32 structSize=0;
      for (u32 i = 0; i < current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.argLength;i++){
        structSize += current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i];
      }
      u32 offset=0;
      for (u32 i = 0; i < current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.argLength; i++)
      {
        if (obj->shaderElements[s].triangles[t].valueUpdated[i]){
          for(u32 j=0;j<3;j++){
            glBufferSubData(GL_ARRAY_BUFFER, (j * structSize + t * structSize * 3 + offset) * sizeof(f32), current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i] * sizeof(f32), obj->shaderElements[s].triangles[t].arg[j]+offset);
            obj->shaderElements[s].triangles[t].valueUpdated[i]=false;
          }
        }
        offset += current->windowShaders.programs[obj->shaderElements[s].shadersIndex].structDef.vecSize[i];
      }
    }
  }
}
void kRenderer_drawObject(u32 index)
{
  for (u32 i = 0; i < kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.length; i++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[i].VBO);
    glBindVertexArray(kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[i].VAO);
    glUseProgram(kRenderer_WindowsContexts.windows[currentContext].shaderPrograms[kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.shaderElements[i].shadersIndex]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects.shaderElements[i].length);
  }
  glBindVertexArray(0);
}
void kRenderer_drawObjectWithTexture(u32 objectIndex, u32 **textureIndex)
{
  for (u32 i = 0; i < kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[objectIndex].boundObjects.length; i++)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindBuffer(GL_ARRAY_BUFFER, kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[objectIndex].shaderData[i].VBO);
    glBindVertexArray(kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[objectIndex].shaderData[i].VAO);
    for (u32 t = 0; t < kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[objectIndex].boundObjects.shaderElements[i].length; t++)
    {
      glBindTexture(GL_TEXTURE_2D, textureIndex[i][t]);
      glUseProgram(kRenderer_WindowsContexts.windows[currentContext].shaderPrograms[kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[objectIndex].boundObjects.shaderElements[i].shadersIndex]);
      glDrawArrays(GL_TRIANGLES, 3 * t, 3 * (t + 1));
    }
  }
  glBindVertexArray(0);
}
void kRenderer_unloadObject(u32 index)
{
  const kRenderer_RendererObject *obj = &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].boundObjects;
  for (u32 s = 0; s < obj->length; s++)
  {
    for (u32 t = 0; t < obj->shaderElements[s].length; t++)
    {
      glDeleteVertexArrays(1, &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[s].VAO);
      glDeleteBuffers(1, &kRenderer_WindowsContexts.windows[currentContext].kRenderer_boundObjects.objectData[index].shaderData[s].VBO);
    }
  }
  // remove vbos and vaos
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
  mat = M4MultiplyM4(mat, M4TranslateV3(V3MultiplyF32(toV3(pixelPos), -1.f)));

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
      struct corner a = {toV3(rP), 1.0f, v4(0, 0, 0, 1)};
      corners[j] = a;
    }
    corners[0].texCoord = v4(0.0f, 0.0f, 0.0f, 0.0f);
    corners[1].texCoord = v4(1.0f, 0.0f, 1.0f, 0.0f);
    corners[2].texCoord = v4(0.0f, 1.0f, 0.0f, 1.0f);
    corners[3].texCoord = v4(1.0f, 1.0f, 1.0f, 1.0f);
    if (1) // drawing of character
    {
      u32 VBO;
      struct corner corners1[6] = {corners[0], corners[1], corners[3], corners[0], corners[2], corners[3]};

      glBindTexture(GL_TEXTURE_2D, fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].textureId);

      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBindVertexArray(defaultVAO);                                                      // setup variables in gpu memory
      glBufferData(GL_ARRAY_BUFFER, sizeof(struct corner) * 6, corners1, GL_STATIC_DRAW); // initialise value VBO

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

    widthOffset += fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].advance + fontMaps.fonts[fontMaps.currentFont].character[charactersIndexs[i]].width;
  }
}

// key input
bool kRenderer_keyStatusPressed(Key e)
{
  return glfwGetKey(kRenderer_WindowsContexts.windows[currentContext].window, KeyToGLFW(e)) == GLFW_PRESS;
}
v2 kRenderer_cursorPosition()
{
  f64 xpos, ypos;
  glfwGetCursorPos(kRenderer_WindowsContexts.windows[currentContext].window, &xpos, &ypos);
  v4 v = v4(xpos, ypos, 0, 1);
  v = V4MultiplyM4(v, M4Mapper(v3(0, kRenderer_getWindowHeight(), 0), v3(kRenderer_getWindowWidth(), 0, 1), kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max));
  return v2(v.x, v.y);
}
bool kRenderer_mouseStatusPressed(Key e)
{
  return glfwGetMouseButton(kRenderer_WindowsContexts.windows[currentContext].window, MouseToGLFW(e)) == GLFW_PRESS;
}

i32 kRenderer_getWindowWidth()
{
  i32 width, height;
  glfwGetWindowSize(kRenderer_WindowsContexts.windows[currentContext].window, &width, &height);
  return width;
}
i32 kRenderer_getWindowHeight()
{
  i32 width, height;
  glfwGetWindowSize(kRenderer_WindowsContexts.windows[currentContext].window, &width, &height);
  return height;
}
iv2 kRenderer_getWindowSize()
{
  i32 width, height;
  glfwGetWindowSize(kRenderer_WindowsContexts.windows[currentContext].window, &width, &height);
  return iv2(width, height);
}
f64 kRenderer_getTimeSinceLastFrame()
{
  return glfwGetTime() - lastTime;
}
m4 kRenderer_getMapper()
{
  m4 mapper = M4InitDiagonal(1);
  mapper = M4MultiplyM4(mapper, M4Rotate(kRenderer_rotation.x, v3(1, 0, 0)));
  mapper = M4MultiplyM4(mapper, M4Rotate(kRenderer_rotation.y, v3(0, 1, 0)));
  mapper = M4MultiplyM4(mapper, M4Rotate(kRenderer_rotation.z, v3(0, 0, 1)));
  mapper = M4MultiplyM4(mapper, M4Mapper(kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max, v3(-1, -1, -1), v3(1, 1, 1)));
  return mapper;
}
