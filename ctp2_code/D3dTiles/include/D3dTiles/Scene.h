#pragma once

#include <memory>

#include "D3dTiles/Region.h"

namespace TileEngine {

  class RendererBase;

  class Scene {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;
    typedef std::shared_ptr<Scene> Ptr;

    Scene() = delete;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    Scene(unsigned width, unsigned height);
    virtual ~Scene();

    unsigned Width() const;
    unsigned Height() const;
    unsigned GetLevelsCount() const;
    void Render(RendererBasePtr renderer);
    Region::Ptr Root();

  private:
    Region::Ptr m_root;
  };

} // namespace TileEngine
