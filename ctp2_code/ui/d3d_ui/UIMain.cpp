#include "ctp/c3.h"

#include "ui/d3d_ui/UIMain.h"

namespace ui::d3d {

UIMain::UIMain() {}

UIMain::~UIMain() {}

void UIMain::Initialize(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string &ldlFile) {
  UI::InitializeUI(hinst, cmdshow, windowWidth, windowHeight, ldlFile);

}

} // namespace ui::d3d