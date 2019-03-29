#include "stdafx.h"

#include "D3dTiles/RendererBase.h"
#include "D3dTiles/Scene.h"

namespace TileEngine {

  Scene::Scene(unsigned width, unsigned height) :
    m_root(std::make_shared<Region>(Region::RegionWeakPtr(), 0, Position(0, 0), width, height)) {
  }

  Scene::~Scene() {
  }

  unsigned Scene::Width() const {
    return m_root->Width();
  }

  unsigned Scene::Height() const {
    return m_root->Height();
  }

  unsigned Scene::GetLevelsCount() const {
    return m_root->GetLevelsCount();
  }

  void Scene::Render(Scene::RendererBasePtr renderer) {
    m_root->Render(0, Position(0, 0), renderer);
  }

  Region::RegionPtr Scene::Root() {
    return m_root;
  }

} // namespace TileEngine
