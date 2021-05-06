#include <kNgine/kNgineFramework.h>
#include <stdio.h>

kRenderer_WindowContext context;
u32 object;

struct
{
  v3 pos;
  f32 isTex;
  v4 color;
} points[3];

static void init()
{
  kRenderer_RendererObject obj;
  obj.length = 1;
  obj.shaderElements[0].shadersIndex = 0;
  obj.shaderElements[0].length = 1;
  obj.shaderElements[0].triangles=malloc(1*sizeof(struct triangle));
  obj.shaderElements[0].triangles[0].arg[0] = &points[0];
  obj.shaderElements[0].triangles[0].arg[1] = &points[1];
  obj.shaderElements[0].triangles[0].arg[2] = &points[2];

  points[0].pos = v3(-0.5, 0, 0);
  points[0].isTex = 0;
  points[0].color = v4(0, 0, 1, 1);

  points[1].pos = v3(0.5, 0, 0);
  points[1].isTex = 0;
  points[1].color = v4(0, 1, 0, 1);

  points[2].pos = v3(0, 1, 0);
  points[2].isTex = 0;
  points[2].color = v4(1, 0, 0, 1);

  kRenderer_bindObject(&object, obj);
}

static void draw()
{
  // kRenderer_drawRectV4(v4(100,0,100,100));
  points[0].pos.x += 0.001;

  points[1].pos.x -= 0.001;
  points[1].pos.y += 0.001;
  points[1].color.r += 0.01;

  points[2].pos.y -= 0.001;
  kRenderer_getBoundObject(object)->shaderElements[0].triangles[0].valueUpdated[0] = true;
  kRenderer_getBoundObject(object)->shaderElements[0].triangles[0].valueUpdated[1] = true;
  kRenderer_getBoundObject(object)->shaderElements[0].triangles[0].valueUpdated[2] = true;

  kRenderer_drawObject(object);
  kRenderer_updateObject(object);
}

int main()
{
  kRenderer_init(0, NULL);
  kRenderer_createContext(&context);
  context.vSync = 2;
  kRenderer_setWindowSize(600, 600);
  kRenderer_setWindowName("test");

  kRenderer_createWindow(&context);

  kRenderer_setStartFunction(init);
  kRenderer_setDrawFunction(draw);

  kRenderer_launch();
}