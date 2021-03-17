#include <math.h>
#include "utils.h"
#include "kRenderer.h"
#include <glad/glad.h>
#ifdef __unix__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <stdio.h>

static struct
{
  size_t length;
  struct{
    i32 shaderPrograms[kRenderer_maxShaderPrograms];
    v3 min;
    v3 max;
    kRenderer_WindowContext *context;
    GLFWwindow *window;
  } windows[kRenderer_maxWindows];
} kRenderer_WindowsContexts;
static u32 currentContext=0;

static i32 defaultShader;
static u32 defaultVAO = 0;
static u32 defaultVBO = 0;
static bool defaultVBOIsTex = false;
static v4 defaultVBOColor = {0,0,0,0};

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
static const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 colorTexCoord;\n"
    "in float isTex;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D textureMap;\n"
    "void main()\n"
    "{\n"
    "  if(isTex>0.5f){\n"
    "  \n"
    "    FragColor = texture(textureMap, vec2(colorTexCoord.x,colorTexCoord.y));\n"
    "  }else{\n"
    "    FragColor = colorTexCoord;\n"
    "  }\n"
    "}\0";

static void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
  for(u32 i=0;i<kRenderer_WindowsContexts.length;i++){
    if(kRenderer_WindowsContexts.windows[i].window==window){
      if (fCompare(kRenderer_WindowsContexts.windows[i].min.y, kRenderer_WindowsContexts.windows[i].context->height) &&
          fCompare(kRenderer_WindowsContexts.windows[i].max.x, kRenderer_WindowsContexts.windows[i].context->width))
      {
        printf("a\n");
        kRenderer_WindowsContexts.windows[i].min.y = height;
        kRenderer_WindowsContexts.windows[i].max.x = width;
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
  case SPACE:
    return GLFW_KEY_SPACE;
  case APOSTROPHE:
    return GLFW_KEY_APOSTROPHE /* ' */;
  case COMMA:
    return GLFW_KEY_COMMA /* , */;
  case MINUS:
    return GLFW_KEY_MINUS /* - */;
  case PERIOD:
    return GLFW_KEY_PERIOD /* . */;
  case SLASH:
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
  case SEMICOLON:
    return GLFW_KEY_SEMICOLON /* ; */;
  case EQUAL:
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
  case LEFT_BRACKET:
    return GLFW_KEY_LEFT_BRACKET /* [ */;
  case BACKSLASH:
    return GLFW_KEY_BACKSLASH /* \ */;
  case RIGHT_BRACKET:
    return GLFW_KEY_RIGHT_BRACKET /* ] */;
  case GRAVE_ACCENT:
    return GLFW_KEY_GRAVE_ACCENT /* ` */;
  case ESCAPE:
    return GLFW_KEY_ESCAPE;
  case ENTER:
    return GLFW_KEY_ENTER;
  case TAB:
    return GLFW_KEY_TAB;
  case BACKSPACE:
    return GLFW_KEY_BACKSPACE;
  case KEY_RIGHT:
    return GLFW_KEY_RIGHT;
  case KEY_LEFT:
    return GLFW_KEY_LEFT;
  case KEY_DOWN:
    return GLFW_KEY_DOWN;
  case KEY_UP:
    return GLFW_KEY_UP;
  case F1:
    return GLFW_KEY_F1;
  case F2:
    return GLFW_KEY_F2;
  case F3:
    return GLFW_KEY_F3;
  case F4:
    return GLFW_KEY_F4;
  case F5:
    return GLFW_KEY_F5;
  case F6:
    return GLFW_KEY_F6;
  case F7:
    return GLFW_KEY_F7;
  case F8:
    return GLFW_KEY_F8;
  case F9:
    return GLFW_KEY_F9;
  case F10:
    return GLFW_KEY_F10;
  case F11:
    return GLFW_KEY_F11;
  case F12:
    return GLFW_KEY_F12;
  case F13:
    return GLFW_KEY_F13;
  case F14:
    return GLFW_KEY_F14;
  case F15:
    return GLFW_KEY_F15;
  case F16:
    return GLFW_KEY_F16;
  case F17:
    return GLFW_KEY_F17;
  case F18:
    return GLFW_KEY_F18;
  case F19:
    return GLFW_KEY_F19;
  case F20:
    return GLFW_KEY_F20;
  case F21:
    return GLFW_KEY_F21;
  case F22:
    return GLFW_KEY_F22;
  case F23:
    return GLFW_KEY_F23;
  case F24:
    return GLFW_KEY_F24;
  case F25:
    return GLFW_KEY_F25;
  case LEFT_SHIFT:
    return GLFW_KEY_LEFT_SHIFT;
  case LEFT_CONTROL:
    return GLFW_KEY_LEFT_CONTROL;
  case LEFT_ALT:
    return GLFW_KEY_LEFT_ALT;
  case LEFT_SUPER:
    return GLFW_KEY_LEFT_SUPER;
  case RIGHT_SHIFT:
    return GLFW_KEY_RIGHT_SHIFT;
  case RIGHT_CONTROL:
    return GLFW_KEY_RIGHT_CONTROL;
  case RIGHT_ALT:
    return GLFW_KEY_RIGHT_ALT;
  default:
    return GLFW_KEY_UNKNOWN;
  }
}
static i32 MouseToGLFW(Key k)
{
  switch (k)
  {
  case MOUSE1:
    return GLFW_MOUSE_BUTTON_1;
  case MOUSE2:
    return GLFW_MOUSE_BUTTON_2;
  case MOUSE3:
    return GLFW_MOUSE_BUTTON_3;
  case MOUSE4:
    return GLFW_MOUSE_BUTTON_4;
  case MOUSE5:
    return GLFW_MOUSE_BUTTON_5;
  case MOUSE6:
    return GLFW_MOUSE_BUTTON_6;
  case MOUSE7:
    return GLFW_MOUSE_BUTTON_7;
  case MOUSE8:
    return GLFW_MOUSE_BUTTON_8;
  default:
    return GLFW_MOUSE_BUTTON_1;
  }
}
static void compileShaders()
{
  if(1)
  {
    i32 success;
    char infoLog[512];
    defaultShader=glCreateProgram();
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
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
  for(u32 i=0;i<kRenderer_WindowsContexts.length;i++){
    for(u32 j=0;j<kRenderer_WindowsContexts.windows[i].context->windowShaders.length;j++){
      i32 success;
      char infoLog[512];
      kRenderer_WindowsContexts.windows[i].shaderPrograms[j] = glCreateProgram();
      for(u32 s=0;s<kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].length;s++){
        i32 shader;
        GLenum shaderType;
        switch (kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shaderType)
        {
        case kRenderer_SHADER_Vertex:
          shaderType=GL_VERTEX_SHADER;
          break;
        case kRenderer_SHADER_Fragment:
          shaderType=GL_FRAGMENT_SHADER;
          break;
        case kRenderer_SHADER_Geometry:
          shaderType=GL_GEOMETRY_SHADER;
          break;
        default:
          shaderType = GL_VERTEX_SHADER;
          break;
        }
        shader=glCreateShader(shaderType);
        glShaderSource(shader, 1, &kRenderer_WindowsContexts.windows[i].context->windowShaders.programs[j].shaders[s].shader,NULL);
        glCompileShader(shader);
        // error checks
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
          glGetShaderInfoLog(shader, 512, NULL, infoLog);
          printf("%s\n",infoLog);
          assert(0 && "ERROR::SHADER::COMPILATION_FAILED\n");
        }
        glAttachShader(kRenderer_WindowsContexts.windows[i].shaderPrograms[j],shader);
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
static m4 getMapper(){
  return M4Mapper(kRenderer_WindowsContexts.windows[currentContext].min, kRenderer_WindowsContexts.windows[currentContext].max, v3(-1, -1, -1), v3(1, 1, 1)); // TODO fix this
}
static void updateDefaultVBO(v3 newPos,bool isTexture,v4 colorTexCoord){

}

i32 kRenderer_init(i32 argc, const char **argv, kRenderer_WindowContext *context)
{
  if(!glfwInit()){
    assert(0 && "could not create context");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  kRenderer_WindowContext c = {"kRenderer", v4(1, 1, 1, 1), iv2(1920, 1080), true,
  {1,
    {
      {
        {3,{kTYPE_v3,kTYPE_BOOL,kTYPE_v4}},
        2,
        {
          {kRenderer_SHADER_Vertex,vertexShaderSource},
          {kRenderer_SHADER_Fragment,fragmentShaderSource}
        }
      }
    }
  }, NULL, NULL};
  *context=c;
  assert(kRenderer_WindowsContexts.length < kRenderer_maxWindows);
  currentContext = kRenderer_WindowsContexts.length;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].context = context;
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].min=v3(0,1080,0);
  kRenderer_WindowsContexts.windows[kRenderer_WindowsContexts.length].max = v3(1920, 0, 1);

  kRenderer_WindowsContexts.length++;
  return 0;
}
i32 kRenderer_makeCurrentContext(kRenderer_WindowContext *context){
  for (u32 i = 0; i < kRenderer_WindowsContexts.length; i++)
  {
    if (kRenderer_WindowsContexts.windows[i].context == context)
    {
      currentContext=i;
      return 0;
    }
  }
  return -1;
}
i32 kRenderer_createWindow(kRenderer_WindowContext *context)
{
  GLFWwindow*window = glfwCreateWindow(context->width, context->height, context->name, NULL, NULL);
  if (window == NULL)
  {
    glfwTerminate();
    assert(0 && "cannot create window");
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    assert(0 && "could not initialize GLAD");
    return -1;
  }
  for(u32 i=0;i<kRenderer_WindowsContexts.length;i++)
  {
    if(kRenderer_WindowsContexts.windows[i].context==context){
      kRenderer_WindowsContexts.windows[i].window=window;
      kRenderer_WindowsContexts.windows[i].min = v3(0, kRenderer_getWindowHeight(), 0);
      kRenderer_WindowsContexts.windows[i].max = v3(kRenderer_getWindowWidth(),0,1);
    }
  }
  if (defaultVAO == 0)
  {
    glGenVertexArrays(1, &defaultVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(f32) * 8,(void *)(sizeof(f32) * 3));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8,(void *)(sizeof(f32) * 4));
  }
  return 0;
}

void kRenderer_setWindowSize(i32 width, i32 height) 
{
  if (fCompare(kRenderer_WindowsContexts.windows[currentContext].min.y, kRenderer_WindowsContexts.windows[currentContext].context->height) &&
      fCompare(kRenderer_WindowsContexts.windows[currentContext].max.x, kRenderer_WindowsContexts.windows[currentContext].context->width)){
    kRenderer_WindowsContexts.windows[currentContext].min.y=height;
    kRenderer_WindowsContexts.windows[currentContext].max.x=width;
  }
  kRenderer_WindowsContexts.windows[currentContext].context->windowSize = iv2(width, height);
  if(kRenderer_WindowsContexts.windows[currentContext].window!=NULL){
    glViewport(0, 0, width, height);
  }
}
void kRenderer_setWindowName(const char *windowName){
  kRenderer_WindowsContexts.windows[currentContext].context->name=windowName;
  if (kRenderer_WindowsContexts.windows[currentContext].window!=NULL)
  {
    glfwSetWindowTitle(kRenderer_WindowsContexts.windows[currentContext].window, windowName);
  }
}
void kRenderer_setStartFunction(void (*startDisplayFunc)())
{
  kRenderer_WindowsContexts.windows[currentContext].context->init = startDisplayFunc;
}
void kRenderer_setDrawFunction(void(*refreshFunction)())
{
  kRenderer_WindowsContexts.windows[currentContext].context->draw = refreshFunction;
}
void kRenderer_setWindowBoundsScale(v3 min, v3 max)
{
  kRenderer_WindowsContexts.windows[currentContext].min=min;
  kRenderer_WindowsContexts.windows[currentContext].max=max;
}

void kRenderer_launch()
{
  compileShaders();
  for(u32 i=0;i<kRenderer_WindowsContexts.length;i++){
    glfwMakeContextCurrent(kRenderer_WindowsContexts.windows[i].window);
    glClear(GL_COLOR_BUFFER_BIT);
    currentContext=i;
    kRenderer_WindowsContexts.windows[i].context->init();
    glfwSwapBuffers(kRenderer_WindowsContexts.windows[i].window);
    glfwPollEvents();
  }
  bool running=true;
  while(running){
    running=false;
    double time=glfwGetTime();
    for(u32 i=0;i<kRenderer_WindowsContexts.length;i++){
      glfwMakeContextCurrent(kRenderer_WindowsContexts.windows[i].window);
      glClear(GL_COLOR_BUFFER_BIT);
      currentContext = i;
      kRenderer_WindowsContexts.windows[i].context->draw();
      glfwSwapInterval(kRenderer_WindowsContexts.windows[i].context->gSync?1:0);
      glfwSwapBuffers(kRenderer_WindowsContexts.windows[i].window);
      glfwPollEvents();
      running|=!glfwWindowShouldClose(kRenderer_WindowsContexts.windows[i].window);
    }
    lastTime=time;
  }
  glfwTerminate();
}

void kRenderer_setDrawColor(v4 color)
{
  kRenderer_WindowsContexts.windows[currentContext].context->currentColor = color;
}
void kRenderer_clear(v4 color)
{
  glClearColor(color.r,color.g,color.b,color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
void kRenderer_clearGray(f32 gray) 
{
  kRenderer_clear(v4(gray, gray, gray, 1));
}

void kRenderer_drawRect(v2 points[4])
{
  struct corner {v3 pos;f32 b;v4 color;};
  struct corner corners[4];
  m4 mapper=getMapper();
  for(u32 i=0;i<4;i++){
    v4 rP={points[i].x,points[i].y,0.0f,1.0f};

    rP=V4MultiplyM4(rP, mapper);
    struct corner a = {v3(rP.x, rP.y, rP.z),
                0.0f,
                kRenderer_WindowsContexts.windows[currentContext].context->currentColor};
    corners[i] = a;
  }

  u32 VBO1,VBO2;
  struct corner corners1[3] = {corners[0], corners[1], corners[3]};
  struct corner corners2[3] = {corners[0], corners[2], corners[3]};

  glGenBuffers(1, &VBO1);
  glBindVertexArray(defaultVAO); // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32)*8*3, corners1, GL_STATIC_DRAW); // initialise value VBO
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32)*8*3, corners2, GL_STATIC_DRAW); // initialise value VBO
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
void kRenderer_drawRectV4(v4 rect)
{
  v2 points[4]={toV2(rect), v2(rect.x, rect.y + rect.height),
               v2(rect.x + rect.width, rect.y),
               v2(rect.x + rect.width, rect.y + rect.height)};
  kRenderer_drawRect(points);
}
void kRenderer_drawTriangle(v2 points[3])
{
  struct corner{v3 pos;f32 b;v4 color;};
  struct corner corners[3];
  for (u32 i = 0; i < 3; i++)
  {
    v4 rP = {points[i].x, points[i].y, 0.0f, 1.0f};
    rP=V4MultiplyM4(rP, getMapper());
    struct corner a = {v3(rP.x, rP.y, rP.z),
                       0.0f,
                       kRenderer_WindowsContexts.windows[currentContext].context->currentColor};
    corners[i] = a;
  }

  u32 VBO;
  glGenBuffers(1, &VBO);
  glBindVertexArray(defaultVAO); // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
  struct corner{v3 pos;f32 b;v4 color;};
  struct corner corners[2];
  for (u32 i = 0; i < 2; i++)
  {
    v4 rP = {points[i].x, points[i].y, 0.0f, 1.0f};
    rP=V4MultiplyM4(rP, getMapper());
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
  for (i32 i = 0; i < numPoints; i++)
  {
    v2 lines[2] = {points[i], points[(i + 1) % 4]};
    kRenderer_drawLine(lines);
  }
}
void kRenderer_drawPoint(v2 point) // very slow
{
  kRenderer_drawRectV4(v4(point.x,point.y,1,1));
}
void kRenderer_drawCircle(v2 startPoint, f32 radius)
{
  for (i32 a = 0; a < 360; a++)
  {
    double angle = a * M_PI / 180;
    v2 line[2] = {startPoint, V2AddV2(startPoint, v2(fast_cos(angle) * radius, fast_sin(angle) * radius))};
    kRenderer_drawLine(line);
  }
}

void kRenderer_drawBuffer_defaultShader(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
                                        v3 position, i32 width, i32 height,v3 rotation)
{
  struct corner {v3 pos;f32 b;v4 texCoord;};
  v2 points[4]={v2(position.x,position.y),v2(position.x+width,position.y),v2(position.x,position.y+height),v2(position.x+width,position.y+height)};
  struct corner corners[4];
  for(u32 i=0;i<4;i++){
    v4 rP={points[i].x,points[i].y,0.0f,1.0f};
    m4 mat = getMapper();
    mat = M4MultiplyM4(mat, M4Rotate(rotation.x, v3(1.0f, 0.0f, 0.0f)));
    mat = M4MultiplyM4(mat, M4Rotate(rotation.y, v3(0.0f, 1.0f, 0.0f)));
    mat = M4MultiplyM4(mat, M4Rotate(rotation.z, v3(0.0f, 0.0f, 1.0f)));
    rP = V4MultiplyM4(rP, mat);
    struct corner a = {v3(rP.x, rP.y, rP.z),
                1.0f,
                v4(1,1,1,1)};
     corners[i] = a;
  }

  corners[0].texCoord=v4(0.0f,0.0f,0.0f,0.0f);
  corners[1].texCoord=v4(1.0f,0.0f,1.0f,0.0f);
  corners[2].texCoord=v4(0.0f,1.0f,0.0f,1.0f);
  corners[3].texCoord=v4(1.0f,1.0f,1.0f,1.0f);

  u32 VBO1,VBO2;
  struct corner corners1[3] = {corners[0], corners[1], corners[3]};
  struct corner corners2[3] = {corners[0], corners[2], corners[3]};
  u32 texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (numChannels == 3)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, buffer);
  }
  else
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, buffer);
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  glGenBuffers(1, &VBO1);
  glBindVertexArray(defaultVAO); // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32)*8*3, corners1, GL_STATIC_DRAW); // initialise value VBO
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32)*8*3, corners2, GL_STATIC_DRAW); // initialise value VBO
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

  glDeleteTextures(1, &texture);
}
void kRenderer_drawBuffer(u8 *buffer, u32 bufferWidth, u32 bufferHeight, u32 numChannels,
       i32 width, i32 height, void *args[kRenderer_maxShaderPrograms])
{
  struct corner {v3 pos;f32 b;v4 color;};

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

void kRenderer_bindTexture(u32 *textureIndex, u8 *colors, v2 position, i32 width,
                           i32 height, i32 realWidth, i32 realHeight, i32 numChannels){}
void kRenderer_drawTexture(u32 textureIndex, v2 position, i32 width, i32 height, v3 rotation){}
void kRenderer_unbindTexture(u32 textureIndex){}

// void kRenderer_bindTexture(u32 *textureIndex, u8 *colors, v2 position,
//                            i32 width, i32 height, i32 realWidth, i32 realHeight,
//                            i32 numChannels)
// {
  // if (storedVBO == 0)
  // {
  //   f32 vertices[] = {0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //                     0.5, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //                     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //                     -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
  //   u32 indices[] = {0, 1, 3, 0, 2, 3};
  //   glGenBuffers(1, &storedVBO);
  //   glBindBuffer(GL_ARRAY_BUFFER, storedVBO);
  //   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
  //                GL_STATIC_DRAW); // initialise value VBO
  //   glGenVertexArrays(1, &storedVAO);
  //   glBindVertexArray(storedVAO); // setup variables in gpu memory
  //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //                         (void *)0); // 3 values xyz,size for one point
  //   glEnableVertexAttribArray(0);
  //   glVertexAttribPointer(
  //       1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //       (void *)(3 * sizeof(float))); // 4 values rgba,size for one point
  //   glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  //   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
  //                         (void *)(7 * sizeof(float)));
  //   glEnableVertexAttribArray(2);
  //   glGenBuffers(1, &storedEBO);
  //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);
  //   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //                GL_STATIC_DRAW);
  // }

  // glGenTextures(1, textureIndex);
  // glBindTexture(GL_TEXTURE_2D, *textureIndex);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RGBA,
  //              GL_UNSIGNED_BYTE, colors);
  // glGenerateMipmap(GL_TEXTURE_2D);
// }
// void kRenderer_drawTexture(u32 textureIndex, v2 position, i32 width, i32 height,
//                            v3 rotation)
// {
//   glBindVertexArray(storedVAO); // setup variables in gpu memory
//   glBindBuffer(GL_ARRAY_BUFFER, storedVBO);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);

//   // draw------
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   iv2 windowSize = kRenderer_getWindowSize();
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, textureIndex);
//   // glm::mat4 transform = glm::mat4(1.0f);
//   // transform = glm::translate(
//   //     transform,
//   //     glm::vec3(
//   //         (float)((position.x + width / 2) / kRenderer_getWindowWidth()) *
//   //                 2.0f -
//   //             1.0f,
//   //         (float)-((position.y + height / 2) / kRenderer_getWindowHeight()) *
//   //                 2.0f +
//   //             1.0f,
//   //         0.0f));
//   // transform = glm::scale(
//   //     transform,
//   //     glm::vec3((float)(((float)width) / windowSize.x) * 2.0f,
//   //               (float)-(((float)height) / windowSize.y) * 2.0f, 1.0f));
//   // transform = glm::rotate(transform, -rotation.x,
//   //                         glm::vec3(1.0f, 0.0f, 0.0f));
//   // transform = glm::rotate(transform, -rotation.y,
//   //                         glm::vec3(0.0f, 1.0f, 0.0f));
//   // transform = glm::rotate(transform, -rotation.z,
//   //                         glm::vec3(0.0f, 0.0f, 1.0f));
//   glUseProgram(shaderProgram);
//   u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
//   // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);
//   glBindVertexArray(storedVAO);
//   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//   // transform = glm::mat4(1.0f);
//   // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
// }
// void kRenderer_unbindTexture(u32 textureIndex)
// {
//   glDeleteTextures(1, &textureIndex);
// }

// key input
bool kRenderer_keyStatusPressed(Key e)
{
  return glfwGetKey(kRenderer_WindowsContexts.windows[currentContext].window, KeyToGLFW(e)) == GLFW_PRESS;
}
v2 kRenderer_cursorPosition()
{
  f64 xpos, ypos;
  glfwGetCursorPos(kRenderer_WindowsContexts.windows[currentContext].window, &xpos, &ypos);
  return v2(xpos, ypos);
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
  return glfwGetTime()-lastTime;
}