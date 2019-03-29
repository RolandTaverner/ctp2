#pragma once

#include <memory>

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Scene.h"

#include <memory>

namespace TileEngine {

  class RendererBase : public std::enable_shared_from_this<RendererBase>
  {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;

    virtual ~RendererBase() {}

    virtual void SetScene(Scene::ScenePtr scene) {
      m_scene = scene;
    }

    virtual void RenderBitmap(unsigned level, const Rect &absRect, Bitmap::BitmapPtr s) = 0;
    virtual void RenderPrimitive(unsigned level) = 0;

  protected:
    Scene::ScenePtr Scene() {
      return m_scene;
    }

  private:
    Scene::ScenePtr m_scene;
  };

} // namespace TileEngine