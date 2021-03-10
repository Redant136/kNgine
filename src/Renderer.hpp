#pragma once

// define if key updown is wanted
// #define KEY_UPDOWN_STATUS_INPUT

#include <cstdint>
#include <functional>
#include "utils.h"

namespace renderer {

// page creation methods
// call in order init>size>setup>draw>launch
void init(i32 argc, const char** argv);
void setWindowSize(i32 width, i32 height);
void setWindowName(const char* windowName);
void createWindow(i32 width, i32 height, const char* windowName);
void setupWindow(std::function<void(void)> startDisplayFunc);
void setDrawFunction(std::function<void(void)> refreshFunction);
// only call at the end of draw function
void launch();

// drawing methods
// set color of drawing tool
void setDrawColor(u8 r, u8 g, u8 b, u8 a=255);
void clear(u8 r, u8 g, u8 b, u8 a=255);
// clear the window, if no color specified, set to black with alpha = 255
inline void clear(u8 rgb=0){clear(rgb,rgb,rgb,255);}

void drawRect(v2 points[4]);
void drawRect(v2 startPos, double width, double height);
void drawTriangle(v2 points[3]);
void drawLine(v2 points[2]);
void drawPoint(v2 point);
void drawLineLoop(v2* points, i32 numPoints);
void drawCircle(v2 startPoint, double radius);
void drawBuffer(u8* colors, v2 position, i32 width, i32 height,
                  i32 realWidth, i32 realHeight, i32 numChannels,v3 rotation);
void bindTexture(u32 * textureIndex, unsigned char* colors, v2 position, i32 width,
                 i32 height, i32 realWidth, i32 realHeight, i32 numChannels);
void drawTexture(u32 textureIndex,v2 position,i32 width,i32 height,v3 rotation);
void unbindTexture(u32 textureIndex);

//key events methods
bool keyStatusPressed(Key e);
void onKeyPress(std::function<void(void)>func,Key e);
v2 cursorPosition();
bool mouseStatusPressed(Key e);

// utils methods
i32 getWindowWidth();
i32 getWindowHeight();
v2 getWindowSize();


}  // namespace renderer
