#pragma once

#include <memory>

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/FontManager.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Scene.h"
#include "D3dTiles/Primitives/ColoredRectangle.h"
#include "D3dTiles/Primitives/TexturedRectangle.h"
#include "D3dTiles/Primitives/Text.h"

#include <memory>

namespace TileEngine {

  class RendererBase : public std::enable_shared_from_this<RendererBase>
  {
  public:
    typedef std::shared_ptr<RendererBase> Ptr;

    virtual ~RendererBase() {}

    virtual void SetScene(Scene::Ptr scene) {
      m_scene = scene;
    }
    
    virtual FontManager::Ptr GetFontManager() = 0;

    virtual void RenderBitmap(unsigned level, const Rect &absRect, Bitmap::Ptr s) = 0;
    virtual void RenderColoredRectangle(unsigned level, const Position &position, ColoredRectangle::Ptr p) = 0;
    virtual void RenderTexturedRectangle(unsigned level, const Position &position, TexturedRectangle::Ptr p) = 0;
    virtual void RenderText(unsigned level, const Position &position, Text::Ptr p) = 0;

  protected:
    Scene::Ptr Scene() {
      return m_scene;
    }

  private:
    Scene::Ptr m_scene;
  };

} // namespace TileEngine