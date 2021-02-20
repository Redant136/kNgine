#pragma once

#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.hpp"

namespace kNgine
{
  class TextBoxRenderer : public LayerRenderer
  {//not complete, only use as template code
  public:
    TextBoxRenderer();
    void displayTextBox(v2 displayRatioSize,v2 position);
    virtual void update(std::vector<msg>msgs);
    virtual void render();
  };
} // namespace kNgine