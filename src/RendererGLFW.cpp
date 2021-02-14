#include <cmath>
#include <functional>
#include <vector>
#include <cstdint>
#include "utils.hpp"
#include "Renderer.hpp"
#ifdef __linux__
#include "../extern/glad/include/glad/glad.h"
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#elif __WIN32
#elif __APPLE__
#include <glad/glad.h>
// #define __gl3_h_
// #define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

const int GLFW_KEYS[] = {GLFW_KEY_SPACE,
                         GLFW_KEY_APOSTROPHE /* ' */,
                         GLFW_KEY_COMMA /* , */,
                         GLFW_KEY_MINUS /* - */,
                         GLFW_KEY_PERIOD /* . */,
                         GLFW_KEY_SLASH /* / */,
                         GLFW_KEY_0,
                         GLFW_KEY_1,
                         GLFW_KEY_2,
                         GLFW_KEY_3,
                         GLFW_KEY_4,
                         GLFW_KEY_5,
                         GLFW_KEY_6,
                         GLFW_KEY_7,
                         GLFW_KEY_8,
                         GLFW_KEY_9,
                         GLFW_KEY_SEMICOLON /* ; */,
                         GLFW_KEY_EQUAL /* = */,
                         GLFW_KEY_A,
                         GLFW_KEY_B,
                         GLFW_KEY_C,
                         GLFW_KEY_D,
                         GLFW_KEY_E,
                         GLFW_KEY_F,
                         GLFW_KEY_G,
                         GLFW_KEY_H,
                         GLFW_KEY_I,
                         GLFW_KEY_J,
                         GLFW_KEY_K,
                         GLFW_KEY_L,
                         GLFW_KEY_M,
                         GLFW_KEY_N,
                         GLFW_KEY_O,
                         GLFW_KEY_P,
                         GLFW_KEY_Q,
                         GLFW_KEY_R,
                         GLFW_KEY_S,
                         GLFW_KEY_T,
                         GLFW_KEY_U,
                         GLFW_KEY_V,
                         GLFW_KEY_W,
                         GLFW_KEY_X,
                         GLFW_KEY_Y,
                         GLFW_KEY_Z,
                         GLFW_KEY_LEFT_BRACKET /* [ */,
                         GLFW_KEY_BACKSLASH /* \ */,
                         GLFW_KEY_RIGHT_BRACKET /* ] */,
                         GLFW_KEY_GRAVE_ACCENT /* ` */,
                         GLFW_KEY_ESCAPE,
                         GLFW_KEY_ENTER,
                         GLFW_KEY_TAB,
                         GLFW_KEY_BACKSPACE,
                         GLFW_KEY_RIGHT,
                         GLFW_KEY_LEFT,
                         GLFW_KEY_DOWN,
                         GLFW_KEY_UP,
                         GLFW_KEY_F1,
                         GLFW_KEY_F2,
                         GLFW_KEY_F3,
                         GLFW_KEY_F4,
                         GLFW_KEY_F5,
                         GLFW_KEY_F6,
                         GLFW_KEY_F7,
                         GLFW_KEY_F8,
                         GLFW_KEY_F9,
                         GLFW_KEY_F10,
                         GLFW_KEY_F11,
                         GLFW_KEY_F12,
                         GLFW_KEY_F13,
                         GLFW_KEY_F14,
                         GLFW_KEY_F15,
                         GLFW_KEY_F16,
                         GLFW_KEY_F17,
                         GLFW_KEY_F18,
                         GLFW_KEY_F19,
                         GLFW_KEY_F20,
                         GLFW_KEY_F21,
                         GLFW_KEY_F22,
                         GLFW_KEY_F23,
                         GLFW_KEY_F24,
                         GLFW_KEY_F25,
                         GLFW_KEY_LEFT_SHIFT,
                         GLFW_KEY_LEFT_CONTROL,
                         GLFW_KEY_LEFT_ALT,
                         GLFW_KEY_LEFT_SUPER,
                         GLFW_KEY_RIGHT_SHIFT,
                         GLFW_KEY_RIGHT_CONTROL,
                         GLFW_KEY_RIGHT_ALT,
                         GLFW_KEY_UNKNOWN};
std::vector<std::function<void(void)>> boundFunctions = std::vector<std::function<void(void)>>(Key::KEY_LAST);

namespace renderer {
GLFWwindow* window;
std::function<void()> displayFunction;
std::function<void()> drawFunction;
unsigned char* currentColor=new unsigned char[4];
unsigned int storedVBO = 0;
unsigned int storedVAO = 0;
unsigned int storedEBO = 0;
int shaderProgram;
// can only draw triangle
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "uniform mat4 transform;\n"
    "out vec2 TexCoord;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   color = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 color;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D textureMap;"
    "void main()\n"
    "{\n"
    "   "
    "    if(TexCoord.x>1.0f||TexCoord.y>1.0f||TexCoord.x<-1.0f||TexCoord.y<-1.0f){"
    "\n"
    "       FragColor = color;\n"
    "   }else{\n"
    "       FragColor = texture(textureMap, TexCoord)*color;\n"
    "   }"
    "}\0";
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

int getWindowWidth() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  return width;
}
int getWindowHeight() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  return height;
}
v2 getWindowSize() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  return v2(width, height);
}

// key input
bool keyStatusPressed(Key e) {
  return glfwGetKey(window, GLFW_KEYS[e]) == GLFW_PRESS;
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
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    throw "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
  }
  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void init(int argc, const char** argv) {
  currentColor = new unsigned char[4];
  for(int i=0;i<boundFunctions.size();i++){
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
void setWindowSize(int width, int height) { glViewport(0, 0, width, height); }
void setWindowName(const char* windowName) {
  window = glfwCreateWindow(1920, 1080, windowName, NULL, NULL);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
void createWindow(int width, int height, const char* windowName) {
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  for(int i=0;i<boundFunctions.size()-1;i++){
    if(key==GLFW_KEYS[i] && action==GLFW_PRESS){
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

void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  // glColor4ub(r, g, b, a);
  currentColor[0] = r;
  currentColor[1] = g;
  currentColor[2] = b;
  currentColor[3] = a;
}
void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  glClearColor(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void drawRect(v2 points[4]) {
  float vertices[] = {
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
  unsigned int indices[] = {0, 1, 3, 0, 2, 3};
  unsigned int VBO, VAO, EBO;
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
  v2 points[4]{v2(startPos), v2(startPos.x, startPos.y + height),
               v2(startPos.x + width, startPos.y),
               v2(startPos.x + width, startPos.y + height)};
  renderer::drawRect(points);
}
void drawTriangle(v2 points[3]) {
  float vertices[] = {
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
  unsigned int VBO, VAO;
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}
void drawLine(v2 points[2]) {
  float vertices[] = {
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
  unsigned int VBO, VAO;
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
void drawLineLoop(v2* points, int numPoints) {
  for (int i = 0; i < numPoints; i++) {
    v2 lines[2] = {points[i], points[(i + 1) % 4]};
    renderer::drawLine(lines);
  }
}
void drawPoint(v2 point)  // very slow
{
  renderer::drawRect(point, 1, 1);
}
void drawCircle(v2 startPoint, double radius) {
  for (int a = 0; a < 360; a++) {
    double angle = a * M_PI / 180;
    v2 line[2] = {startPoint, startPoint + v2(std::cos(angle) * radius,
                                              std::sin(angle) * radius)};
    renderer::drawLine(line);
  }
}
void drawColorMap(unsigned char* colors, v2 position, int width, int height,
                  int realWidth, int realHeight, int numChannels, v3 rotation) {
  float vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                      0.5,   -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                      -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  unsigned int indices[] = {0, 1, 3, 0, 2, 3};
  unsigned int VBO, VAO, EBO;
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

  unsigned int texture;
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
  v2 windowSize = getWindowSize();
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
  unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
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
void bindTexture(unsigned int* textureIndex, unsigned char* colors, v2 position,
                 int width, int height, int realWidth, int realHeight,
                 int numChannels) {
  if (storedVBO == 0) {
    float vertices[] = { 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                         0.5, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                        -0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    unsigned int indices[] = {0, 1, 3, 0, 2, 3};
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
void drawTexture(unsigned int textureIndex, v2 position, int width, int height,
                 v3 rotation) {
  glBindVertexArray(storedVAO);  // setup variables in gpu memory
  glBindBuffer(GL_ARRAY_BUFFER, storedVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);

  // draw------
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  v2 windowSize = getWindowSize();
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
  unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storedEBO);
  glBindVertexArray(storedVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  transform = glm::mat4(1.0f);
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}
void unbindTexture(unsigned int textureIndex) {
  glDeleteTextures(1, &textureIndex);
}

}  // namespace renderer