#include <cmath>
#include <functional>
#include <vector>
#include <cstdint>
#include "utils.h"
#include "oldRenderer.hpp"
#include <glad/glad.h>
#ifdef __unix__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static i32 KeyToGLFW(Key k){
  switch (k)
  {
  case Key::SPACE : return GLFW_KEY_SPACE;
  case Key::APOSTROPHE :return GLFW_KEY_APOSTROPHE /* ' */;
  case Key::COMMA :return GLFW_KEY_COMMA /* , */;
  case Key::MINUS :return GLFW_KEY_MINUS /* - */;
  case Key::PERIOD :return GLFW_KEY_PERIOD /* . */;
  case Key::SLASH :return GLFW_KEY_SLASH /* / */;
  case Key::KEY_0 :return GLFW_KEY_0;
  case Key::KEY_1 :return GLFW_KEY_1;
  case Key::KEY_2 :return GLFW_KEY_2;
  case Key::KEY_3 :return GLFW_KEY_3;
  case Key::KEY_4 :return GLFW_KEY_4;
  case Key::KEY_5 :return GLFW_KEY_5;
  case Key::KEY_6 :return GLFW_KEY_6;
  case Key::KEY_7 :return GLFW_KEY_7;
  case Key::KEY_8 :return GLFW_KEY_8;
  case Key::KEY_9 :return GLFW_KEY_9;
  case Key::SEMICOLON :return GLFW_KEY_SEMICOLON /* ; */;
  case Key::EQUAL :return GLFW_KEY_EQUAL /* = */;
  case Key::KEY_A :return GLFW_KEY_A;
  case Key::KEY_B :return GLFW_KEY_B;
  case Key::KEY_C :return GLFW_KEY_C;
  case Key::KEY_D :return GLFW_KEY_D;
  case Key::KEY_E :return GLFW_KEY_E;
  case Key::KEY_F :return GLFW_KEY_F;
  case Key::KEY_G :return GLFW_KEY_G;
  case Key::KEY_H :return GLFW_KEY_H;
  case Key::KEY_I :return GLFW_KEY_I;
  case Key::KEY_J :return GLFW_KEY_J;
  case Key::KEY_K :return GLFW_KEY_K;
  case Key::KEY_L :return GLFW_KEY_L;
  case Key::KEY_M :return GLFW_KEY_M;
  case Key::KEY_N :return GLFW_KEY_N;
  case Key::KEY_O :return GLFW_KEY_O;
  case Key::KEY_P :return GLFW_KEY_P;
  case Key::KEY_Q :return GLFW_KEY_Q;
  case Key::KEY_R :return GLFW_KEY_R;
  case Key::KEY_S :return GLFW_KEY_S;
  case Key::KEY_T :return GLFW_KEY_T;
  case Key::KEY_U :return GLFW_KEY_U;
  case Key::KEY_V :return GLFW_KEY_V;
  case Key::KEY_W :return GLFW_KEY_W;
  case Key::KEY_X :return GLFW_KEY_X;
  case Key::KEY_Y :return GLFW_KEY_Y;
  case Key::KEY_Z :return GLFW_KEY_Z;
  case Key::LEFT_BRACKET :return GLFW_KEY_LEFT_BRACKET /* [ */;
  case Key::BACKSLASH :return GLFW_KEY_BACKSLASH /* \ */;
  case Key::RIGHT_BRACKET :return GLFW_KEY_RIGHT_BRACKET /* ] */;
  case Key::GRAVE_ACCENT :return GLFW_KEY_GRAVE_ACCENT /* ` */;
  case Key::ESCAPE :return GLFW_KEY_ESCAPE;
  case Key::ENTER :return GLFW_KEY_ENTER;
  case Key::TAB :return GLFW_KEY_TAB;
  case Key::BACKSPACE :return GLFW_KEY_BACKSPACE;
  case Key::KEY_RIGHT :return GLFW_KEY_RIGHT;
  case Key::KEY_LEFT :return GLFW_KEY_LEFT;
  case Key::KEY_DOWN :return GLFW_KEY_DOWN;
  case Key::KEY_UP :return GLFW_KEY_UP;
  case Key::F1 :return GLFW_KEY_F1;
  case Key::F2 :return GLFW_KEY_F2;
  case Key::F3 :return GLFW_KEY_F3;
  case Key::F4 :return GLFW_KEY_F4;
  case Key::F5 :return GLFW_KEY_F5;
  case Key::F6 :return GLFW_KEY_F6;
  case Key::F7 :return GLFW_KEY_F7;
  case Key::F8 :return GLFW_KEY_F8;
  case Key::F9 :return GLFW_KEY_F9;
  case Key::F10 :return GLFW_KEY_F10;
  case Key::F11 :return GLFW_KEY_F11;
  case Key::F12 :return GLFW_KEY_F12;
  case Key::F13 :return GLFW_KEY_F13;
  case Key::F14 :return GLFW_KEY_F14;
  case Key::F15 :return GLFW_KEY_F15;
  case Key::F16 :return GLFW_KEY_F16;
  case Key::F17 :return GLFW_KEY_F17;
  case Key::F18 :return GLFW_KEY_F18;
  case Key::F19 :return GLFW_KEY_F19;
  case Key::F20 :return GLFW_KEY_F20;
  case Key::F21 :return GLFW_KEY_F21;
  case Key::F22 :return GLFW_KEY_F22;
  case Key::F23 :return GLFW_KEY_F23;
  case Key::F24 :return GLFW_KEY_F24;
  case Key::F25 :return GLFW_KEY_F25;
  case Key::LEFT_SHIFT :return GLFW_KEY_LEFT_SHIFT;
  case Key::LEFT_CONTROL :return GLFW_KEY_LEFT_CONTROL;
  case Key::LEFT_ALT :return GLFW_KEY_LEFT_ALT;
  case Key::LEFT_SUPER :return GLFW_KEY_LEFT_SUPER;
  case Key::RIGHT_SHIFT :return GLFW_KEY_RIGHT_SHIFT;
  case Key::RIGHT_CONTROL :return GLFW_KEY_RIGHT_CONTROL;
  case Key::RIGHT_ALT :return GLFW_KEY_RIGHT_ALT;
    default : return GLFW_KEY_UNKNOWN;
  }
}
static std::vector<std::function<void(void)>> boundFunctions = std::vector<std::function<void(void)>>(Key::KEY_LAST);

namespace renderer {
static GLFWwindow* window;
static std::function<void()> displayFunction;
static std::function<void()> drawFunction;
static u8* currentColor=new u8[4];
static u32 storedVBO = 0;
static u32 storedVAO = 0;
static u32 storedEBO = 0;
static i32 shaderProgram;
// can only draw triangle
static const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "uniform mat4 transform;\n"
    "out vec4 color;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   color = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

static const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 color;\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D textureMap;\n"
    "void main()\n"
    "{\n"
    "   if(TexCoord.x>1.0f||TexCoord.y>1.0f||TexCoord.x<-1.0f||TexCoord.y<-1.0f){\n"
    "       FragColor = color;\n"
    "   }else{\n"
    "       FragColor = texture(textureMap, TexCoord)*color;\n"
    "   }"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

i32 getWindowWidth() {
  i32 width, height;
  glfwGetWindowSize(window, &width, &height);
  return width;
}
i32 getWindowHeight() {
  i32 width, height;
  glfwGetWindowSize(window, &width, &height);
  return height;
}
iv2 getWindowSize() {
  i32 width, height;
  glfwGetWindowSize(window, &width, &height);
  return iv2(width, height);
}

// key input
bool keyStatusPressed(Key e) {
  return glfwGetKey(window, KeyToGLFW(e)) == GLFW_PRESS;
}
void onKeyPress(std::function<void(void)>func,Key e){
  boundFunctions[e]=func;
}
v2 cursorPosition(){
  double xpos,ypos;
  glfwGetCursorPos(window,&xpos,&ypos);
  return v2(xpos,ypos);
}
bool mouseStatusPressed(Key e){
  switch(e){
    case Key::MOUSE1:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)==GLFW_PRESS;
    case Key::MOUSE2:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)==GLFW_PRESS;
    case Key::MOUSE3:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3)==GLFW_PRESS;
    case Key::MOUSE4:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4)==GLFW_PRESS;
    case Key::MOUSE5:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5)==GLFW_PRESS;
    case Key::MOUSE6:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6)==GLFW_PRESS;
    case Key::MOUSE7:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7)==GLFW_PRESS;
    case Key::MOUSE8:return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_8)==GLFW_PRESS;
    default:return false;
  }
}
// private
void compileShaders() {
  i32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  i32 success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    assert(0);
    throw "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
  }
  // fragment shader
  i32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    throw "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
  }
  // link shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glDeleteShader(vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glDeleteShader(fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
  }
}

void init(i32 argc, const char** argv) {
  currentColor = new u8[4];
  for(i32 i=0;i<boundFunctions.size();i++){
    boundFunctions[i]=[](){};
  }
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}
void setWindowSize(i32 width, i32 height) { glViewport(0, 0, width, height); }
void setWindowName(const char* windowName) {
  window = glfwCreateWindow(1920, 1080, windowName, NULL, NULL);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
void createWindow(i32 width, i32 height, const char* windowName) {
  window = glfwCreateWindow(width, height, windowName, NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    throw "cannot create window";
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw "could not initialize GLAD";
  }
}
void setupWindow(std::function<void(void)> startDisplayFunc) {
  displayFunction = startDisplayFunc;
}
void setDrawFunction(std::function<void(void)> refreshFunction) {
  drawFunction = refreshFunction;
}
void key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
  for(i32 i=0;i<boundFunctions.size()-1;i++){
    if(key==KeyToGLFW((Key)i) && action==GLFW_PRESS){
      boundFunctions[i]();
    }
  }
}
void launch() {
  compileShaders();
  glfwSetKeyCallback(window, key_callback);
  displayFunction();
  glfwSwapBuffers(window);
  glfwPollEvents();
  while (!glfwWindowShouldClose(window)) {
    drawFunction();
    // set to 0 to get better fps, sets vsync
    glfwSwapInterval(1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}

void setDrawColor(u8 r, u8 g, u8 b, u8 a) {
  // glColor4ub(r, g, b, a);
  currentColor[0] = r;
  currentColor[1] = g;
  currentColor[2] = b;
  currentColor[3] = a;
}
void clear(u8 r, u8 g, u8 b, u8 a) {
  glClearColor(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void drawRect(v2 points[4]) {
  f32 vertices[] = {
      (float)(points[0].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[0].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[1].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[1].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[2].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[2].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[3].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[3].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f};
  u32 indices[] = {0, 1, 3, 0, 2, 3};
  u32 VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);  // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)0);  // 3 values xyz,size for one point
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(3 * sizeof(float)));  // 4 values rgba,size for one point
  glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // draw------
  glm::mat4 transform = glm::mat4(1.0f);
  u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shaderProgram);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
void drawRect(v2 startPos, double width, double height) {
  v2 points[4]{startPos, v2(startPos.x, startPos.y + height),
               v2(startPos.x + width, startPos.y),
               v2(startPos.x + width, startPos.y + height)};
  renderer::drawRect(points);
}
void drawTriangle(v2 points[3]) {
  f32 vertices[] = {
      (float)(points[0].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[0].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[1].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[1].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[2].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[2].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f};
  u32 VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);  // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)0);  // 3 values xyz,size for one point
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(3 * sizeof(float)));  // 4 values rgba,size for one point
  glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // draw------
  glm::mat4 transform = glm::mat4(1.0f);
  u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}
void drawLine(v2 points[2]) {
  f32 vertices[] = {
      (float)(points[0].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[0].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f,
      (float)(points[1].x / renderer::getWindowWidth()) * 2.0f - 1.0f,
      (float)-(points[1].y / renderer::getWindowHeight()) * 2.0f + 1.0f,
      0.0f,
      currentColor[0] / 255.0f,
      currentColor[1] / 255.0f,
      currentColor[2] / 255.0f,
      currentColor[3] / 255.0f,
      1.1f,
      1.1f};
  u32 VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);  // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)0);  // 3 values xyz,size for one point
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(3 * sizeof(float)));  // 4 values rgba,size for one point
  glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // draw------
  glm::mat4 transform = glm::mat4(1.0f);
  u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, 2);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawLineLoop(v2* points, i32 numPoints) {
  for (i32 i = 0; i < numPoints; i++) {
    v2 lines[2] = {points[i], points[(i + 1) % 4]};
    renderer::drawLine(lines);
  }
}
void drawPoint(v2 point)  // very slow
{
  renderer::drawRect(point, 1, 1);
}
void drawCircle(v2 startPoint, double radius) {
  for (i32 a = 0; a < 360; a++) {
    double angle = a * M_PI / 180;
    v2 line[2] = {startPoint, V2AddV2(startPoint, v2(std::cos(angle) * radius, std::sin(angle) * radius))};
    renderer::drawLine(line);
  }
}
void drawBuffer(u8* colors, v2 position, i32 width, i32 height,
                  i32 realWidth, i32 realHeight, i32 numChannels, v3 rotation) {
  f32 vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                      0.5,   -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                      -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  u32 indices[] = {0, 1, 3, 0, 2, 3};
  u32 VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);  // initialise value VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)0);  // 3 values xyz,size for one point
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(3 * sizeof(float)));  // 4 values rgba,size for one point
  glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  u32 texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  if(numChannels==3){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, realWidth, realHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, colors);
  }else{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, colors);
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  // draw------
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  iv2 windowSize = getWindowSize();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(
      transform,
      glm::vec3(
          (float)((position.x + width / 2) / renderer::getWindowWidth()) *
                  2.0f -
              1.0f,
          (float)-((position.y + height / 2) / renderer::getWindowHeight()) *
                  2.0f +
              1.0f,
          0.0f));
  transform = glm::scale(
      transform,
      glm::vec3((float)(((float)width) / windowSize.x) * 2.0f,
                (float)-(((float)height) / windowSize.y) * 2.0f, 1.0f));
  transform = glm::rotate(transform, -rotation.x,
                          glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, -rotation.y,
                          glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, -rotation.z,
                          glm::vec3(0.0f, 0.0f, 1.0f));
  glUseProgram(shaderProgram);
  u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  transform = glm::mat4(1.0f);
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glDeleteTextures(1, &texture);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
void bindTexture(u32 * textureIndex, u8* colors, v2 position,
                 i32 width, i32 height, i32 realWidth, i32 realHeight,
                 i32 numChannels) {
  if (storedVBO == 0) {
    f32 vertices[] = { 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                         0.5, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                        -0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    u32 indices[] = {0, 1, 3, 0, 2, 3};
    glGenBuffers(1, &storedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, storedVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);  // initialise value VBO
    glGenVertexArrays(1, &storedVAO);
    glBindVertexArray(storedVAO);  // setup variables in gpu memory
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void*)0);  // 3 values xyz,size for one point
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)(3 * sizeof(float)));  // 4 values rgba,size for one point
    glEnableVertexAttribArray(1);     // set values of VAO to pass to shader
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glGenBuffers(1, &storedEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
  }

  glGenTextures(1, textureIndex);
  glBindTexture(GL_TEXTURE_2D, *textureIndex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realWidth, realHeight, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, colors);
  glGenerateMipmap(GL_TEXTURE_2D);
}
void drawTexture(u32 textureIndex, v2 position, i32 width, i32 height,
                 v3 rotation) {
  glBindVertexArray(storedVAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, storedVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);

  // draw------
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  iv2 windowSize = getWindowSize();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureIndex);
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(
      transform,
      glm::vec3(
          (float)((position.x + width / 2) / renderer::getWindowWidth()) *
                  2.0f -
              1.0f,
          (float)-((position.y + height / 2) / renderer::getWindowHeight()) *
                  2.0f +
              1.0f,
          0.0f));
  transform = glm::scale(
      transform,
      glm::vec3((float)(((float)width) / windowSize.x) * 2.0f,
                (float)-(((float)height) / windowSize.y) * 2.0f, 1.0f));
  transform = glm::rotate(transform, -rotation.x,
                          glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, -rotation.y,
                          glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, -rotation.z,
                          glm::vec3(0.0f, 0.0f, 1.0f));
  glUseProgram(shaderProgram);
  u32 transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);
  glBindVertexArray(storedVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  transform = glm::mat4(1.0f);
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}
void unbindTexture(u32 textureIndex) {
  glDeleteTextures(1, &textureIndex);
}

}  // namespace renderer