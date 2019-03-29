#pragma once

#include <memory>

#include "D3dTiles/Region.h"

namespace TileEngine {

  class RendererBase;

  class Scene {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;
    typedef std::shared_ptr<Scene> ScenePtr;

    Scene() = delete;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    Scene(unsigned width, unsigned height);
    virtual ~Scene();

    unsigned Width() const;
    unsigned Height() const;
    unsigned GetLevelsCount() const;
    void Render(RendererBasePtr renderer);
    Region::RegionPtr Root();

  private:
    Region::RegionPtr m_root;
  };

} // namespace TileEngine
