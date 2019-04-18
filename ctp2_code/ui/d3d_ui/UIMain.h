#pragma once

#include <memory>
#include <string>

#include "ui/d3d_ui/UI.h"

namespace ui::d3d {

class UIMain;
typedef std::shared_ptr<UIMain> UIMainPtr;

class UIMain : public UI {
public:
  UIMain();
  virtual ~UIMain();
  UIMain(const UIMain&) = delete;
  UIMain &operator=(const UIMain &) = delete;

  void Initialize(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string &ldlFile);
  void InitPaths(const std::string &pathsFile, const ExtraDataPaths &extraDataPaths);
  void SetCurScenarioPath(const std::string &path);
  const std::string &GetCurScenarioPath();
  void SetCurScenarioPackPath(const std::string &path);
  const std::string &GetCurScenarioPackPath();
};

} // namespace ui::d3d