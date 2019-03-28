#include "stdafx.h"

#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>

#include "Region.h"
#include "RendererBase.h"

namespace TileEngine {

  Region::Region() : Region(RegionWeakPtr(), 0, 0, 0) {
  }

  Region::Region(RegionWeakPtr parent, RegionID id, unsigned width, unsigned height) :
    m_parent(parent), m_ID(id), m_width(width), m_height(height) {
  }

  Region::~Region() {}

  Region::RegionID Region::ID() const {
    return m_ID;
  }

  unsigned Region::Width() const {
    return m_width;
  }

  unsigned Region::Height() const {
    return m_height;
  }

  Rect Region::GetRect(const Position &relative) const {
    const Position minPoint(relative);
    Position maxPoint(relative);
    boost::geometry::add_point(maxPoint, Position(Width(), Height()));
    return Rect(minPoint, maxPoint);
  }

  unsigned Region::GetLevelsCount() const {
    unsigned childLevels = 0;
    for (auto i : m_children) {
      childLevels = std::max<>(childLevels, i.second.region->GetLevelsCount());
    }

    unsigned layersLevels = 0;
    for (auto layer : m_layers) {
      layersLevels += layer.second->GetLevelsCount();
    }

    return childLevels + 1 + layersLevels;
  }

  Region::RegionPtr Region::AddChild(Position position, unsigned width, unsigned height) {
    const Position newMin(position);
    Position newMax(position);
    boost::geometry::add_point(newMax, Position(width, height));
    const Rect newRect(newMin, newMax);

    if (!boost::geometry::covered_by(newRect, GetRect(Position(0, 0)))) {
      throw std::invalid_argument("New child region outside parent's bounds");
    }

    for (auto i : m_children) {
      const Rect curRect(i.second.region->GetRect(i.second.position));
      if (boost::geometry::overlaps(curRect, newRect)) {
        throw std::invalid_argument("New child region overlaps with existing");
      }
    }

    const RegionID newID = m_children.empty() ? 0 : (m_children.rbegin()->first + 1);
    RegionPtr newRegion(std::make_shared<Region>(shared_from_this(), newID, width, height));
    m_children[newID] = ChildRegion{ position, newRegion };
    return newRegion;
  }

  Region::RegionPtr Region::AddLayer(unsigned level) {
    LayersMap::const_iterator i = m_layers.find(level);
    if (i != m_layers.end()) {
      throw std::invalid_argument("layer already exists");
    }

    RegionPtr newLevel(std::make_shared<Region>(shared_from_this(), level, Width(), Height()));
    m_layers[level] = newLevel;
    return newLevel;
  }

  void Region::Render(unsigned ownLevel, const Position &position, Region::RendererBasePtr renderer) {
    RenderSelf(ownLevel, position, renderer);

    unsigned level = ownLevel;

    for (auto child : m_children) {
      Position childPosition(position);
      boost::geometry::add_point(childPosition, child.second.position);
      child.second.region->Render(++level, childPosition, renderer);
    }

    for (auto layer : m_layers) {
      layer.second->Render(++level, position, renderer);
    }
  }

  class GraphicElementVisitor {
  public:
    GraphicElementVisitor(unsigned level, const Position &position, Region::RendererBasePtr renderer) :
      m_level(level), m_position(position), m_renderer(renderer){
    }

    void operator()(Bitmap::BitmapPtr &s) {
      Position minPoint(m_position);
      Position maxPoint(m_position);
      boost::geometry::add_point(maxPoint, Position(s->Width(), s->Height()));
      m_renderer->RenderBitmap(m_level, Rect(minPoint, maxPoint), s);
    }
  
  private:
    unsigned m_level;
    Position m_position;
    RendererBase::RendererBasePtr m_renderer;
  };

  void Region::RenderSelf(unsigned level, const Position &position, Region::RendererBasePtr renderer) {
    
    for (auto e : m_graphics) {
      Position pos(position);
      boost::geometry::add_point(pos, e.position);
      
      GraphicElementVisitor visitor(level, pos, renderer);
      std::visit<>(visitor, e.element);
    }
  }
  
  void Region::DrawPrimitive() {

  }

  void Region::DrawImage(const Position &position, Bitmap::BitmapPtr bitmap) {
    m_graphics.push_back(GraphicElementPosition{ position, GraphicElement(bitmap) });
  }

  void Region::Clear(bool children) {
    m_graphics.clear();
    if (children) {
      for (auto child : m_children) {
        child.second.region->Clear(children);
      }
    }
  }


} // namespace TileEngine
