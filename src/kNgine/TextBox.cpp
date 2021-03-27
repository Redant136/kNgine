#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "../kFramework/utils.h"
#include "Renderer.hpp"
#include "TextBox.hpp"

namespace kNgine{
  TextBoxRenderer::TextBoxRenderer(){

  }
  void TextBoxRenderer::displayTextBox(v2 positionRatio, v2 displayRatioSize)
  {
    renderer::setDrawColor(255,255,255);
    iv2 displaySize=renderer::getWindowSize();
    renderer::drawRect({positionRatio.x*displaySize.x,positionRatio.y*displaySize.y},displaySize.x*displayRatioSize.x,displaySize.y*displayRatioSize.y);
  }
  void TextBoxRenderer::update(std::vector<msg> msgs){
  }
  void TextBoxRenderer::render()
  {
    displayTextBox({0.05,0.7},{0.9,0.25});
  }
}