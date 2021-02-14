#pragma once

// define if key updown is wanted
// #define KEY_UPDOWN_STATUS_INPUT

#include <cstdint>
#include <functional>
#include "utils.hpp"

namespace renderer {

// page creation methods
// call in order init>size>setup>draw>launch
void init(int argc, const char** argv);
void setWindowSize(int width, int height);
void setWindowName(const char* windowName);
void createWindow(int width, int height, const char* windowName);
void setupWindow(std::function<void(void)> startDisplayFunc);
void setDrawFunction(std::function<void(void)> refreshFunction);
// only call at the end of draw function
void launch();

// drawing methods
// set color of drawing tool
void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
// clear the window, if no color specified, set to black with alpha = 255
inline void clear(uint8_t rgb=0){clear(rgb,rgb,rgb,255);}

void drawRect(v2 points[4]);
void drawRect(v2 startPos, double width, double height);
void drawTriangle(v2 points[3]);
void drawLine(v2 points[2]);
void drawPoint(v2 point);
void drawLineLoop(v2* points, int numPoints);
void drawCircle(v2 startPoint, double radius);
void drawColorMap(unsigned char* colors, v2 position, int width, int height,
                  int realWidth, int realHeight, int numChannels,v3 rotation);
void bindTexture(unsigned int* textureIndex, unsigned char* colors, v2 position, int width,
                 int height, int realWidth, int realHeight, int numChannels);
void drawTexture(unsigned int textureIndex,v2 position,int width,int height,v3 rotation);
void unbindTexture(unsigned int textureIndex);

//key events methods
bool keyStatusPressed(Key e);
void onKeyPress(std::function<void(void)>func,Key e);
v2 cursorPosition();
bool mouseStatusPressed(Key e);

// utils methods
int getWindowWidth();
int getWindowHeight();
v2 getWindowSize();


}  // namespace renderer
