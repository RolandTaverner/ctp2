#include "stdafx.h"

#include <boost/geometry/algorithms/within.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>

#include "D3dTiles/Region.h"
#include "D3dTiles/RendererBase.h"

namespace TileEngine {

  Region::Region() : Region(Region::Ptr(), 0, Position(0, 0), 0, 0, true, true) {
  }

  Region::Region(Region::Ptr parent, RegionID id,
    const Position &position, unsigned width, unsigned height, 
    bool isVisible, bool strictArea) :
    m_parent(parent), m_ID(id), 
    m_position(position), m_width(width), m_height(height), 
    m_isVisible(isVisible), m_strictArea(strictArea) {
  }

  Region::~Region() {}

  Region::RegionID Region::ID() const {
    return m_ID;
  }

  const Position &Region::Pos() const {
    return m_position;
  }

  void Region::SetPosition(const Position &pos) {
    m_position = pos;
  }

  unsigned Region::Width() const {
    return m_width;
  }

  void Region::SetWidth(unsigned w) {
    m_width = w;
  }

  unsigned Region::Height() const {
    return m_height;
  }

  void Region::SetHeight(unsigned h) {
    m_height = h;
  }
  
  bool Region::IsVisible() const {
    return m_isVisible;
  }

  void Region::SetVisible(bool isVisible) {
    m_isVisible = isVisible;
  }

  void Region::MoveTo(int x, int y) {
    // TODO: check bounds?
    m_position = Position(x, y);
  }
  
  void Region::MoveBy(int dx, int dy) {
    // TODO: check bounds?
    boost::geometry::add_point(m_position, Position(dx, dy));
  }
  
  void Region::Resize(unsigned width, unsigned height) {
    // TODO: check bounds
    m_width = width;
    m_height = height;
  }

  Rect Region::GetRect() const {
    const Position &minPoint = Pos();
    Position maxPoint(minPoint);
    boost::geometry::add_point(maxPoint, Position(Width(), Height()));
    return Rect(minPoint, maxPoint);
  }

  unsigned Region::GetLevelsCount() const {
    unsigned childLevels = 0;
    for (auto i : m_children) {
      childLevels = std::max<>(childLevels, i.second->GetLevelsCount());
    }

    unsigned layersLevels = 0;
    for (auto layer : m_layers) {
      layersLevels += layer.second->GetLevelsCount();
    }

    return childLevels + 1 + layersLevels;
  }

  Region::Ptr Region::CreateChild(const Position &position, unsigned width, unsigned height, 
    bool isVisible, bool strictArea) {
    const Rect thisRect(Position(0, 0), Position(Width(), Height()));
    
    if (m_strictArea) {
      const Position newMin(position);
      Position newMax(position);
      boost::geometry::add_point(newMax, Position(width, height));
      const Rect newRect(newMin, newMax);

      if (!boost::geometry::within(newRect, thisRect)) {
        throw std::invalid_argument("New child region outside parent's bounds");
      }

      for (auto i : m_children) {
        const Rect curRect(i.second->GetRect());
        if (boost::geometry::overlaps(curRect, newRect)) {
          throw std::invalid_argument("New child region overlaps with existing");
        }
      }
    }

    const RegionID newID = m_children.empty() ? 0 : (m_children.rbegin()->first + 1);
    Ptr newRegion(std::make_shared<Region>(shared_from_this(), newID, position, width, height, isVisible, strictArea));
    m_children[newID] = newRegion;
    return newRegion;
  }

  Region::Ptr Region::CreateLayer(unsigned level) {
    LayersMap::const_iterator i = m_layers.find(level);
    if (i != m_layers.end()) {
      throw std::invalid_argument("layer already exists");
    }

    Ptr newLevel(std::make_shared<Region>(shared_from_this(), level, Position(0, 0), Width(), Height()));
    m_layers[level] = newLevel;
    return newLevel;
  }

  Region::Ptr Region::CreateLayer() {
    if (m_layers.empty()) {
      return CreateLayer(0);
    }
    const unsigned maxLayer = m_layers.rbegin()->first;
    return CreateLayer(maxLayer + 1);
  }

  void Region::Render(unsigned ownLevel, const Position &parentPosition, Region::RendererBasePtr renderer) {
    Position absPosition(parentPosition);
    boost::geometry::add_point(absPosition, Pos());

    RenderSelf(ownLevel, absPosition, renderer);

    unsigned level = ownLevel;

    for (auto child : m_children) {
      child.second->Render(++level, absPosition, renderer);
    }

    for (auto layer : m_layers) {
      layer.second->Render(++level, absPosition, renderer);
    }
  }

  class GraphicElementVisitor {
  public:
    GraphicElementVisitor(unsigned level, const Position &position, Region::RendererBasePtr renderer) :
      m_level(level), m_position(position), m_renderer(renderer){
    }

    void operator()(Bitmap::Ptr &s) {
      Position minPoint(m_position);
      Position maxPoint(m_position);
      boost::geometry::add_point(maxPoint, Position(s->Width(), s->Height()));
      m_renderer->RenderBitmap(m_level, Rect(minPoint, maxPoint), s);
    }

    void operator()(ColoredRectangle::Ptr &s) {
      m_renderer->RenderColoredRectangle(m_level, m_position, s);
    }

    void operator()(TexturedRectangle::Ptr &s) {
      m_renderer->RenderTexturedRectangle(m_level, m_position, s);
    }

    void operator()(Text::Ptr &t) {
      m_renderer->RenderText(m_level, m_position, t);
    }

  private:
    unsigned m_level;
    Position m_position;
    RendererBase::Ptr m_renderer;
  };

  void Region::RenderSelf(unsigned level, const Position &position, Region::RendererBasePtr renderer) {
    
    for (auto e : m_graphics) {
      Position pos(position);
      boost::geometry::add_point(pos, e.position);
      
      GraphicElementVisitor visitor(level, pos, renderer);
      std::visit<>(visitor, e.element);
    }
  }
  
  void Region::DrawPrimitive(const Position &position, ColoredRectangle::Ptr p) {
    m_graphics.push_back(GraphicElementPosition{ position, GraphicElement(p) });
  }
  
  void Region::DrawPrimitive(const Position &position, TexturedRectangle::Ptr p) {
    m_graphics.push_back(GraphicElementPosition{ position, GraphicElement(p) });
  }

  void Region::DrawImage(const Position &position, Bitmap::Ptr bitmap) {
    m_graphics.push_back(GraphicElementPosition{ position, GraphicElement(bitmap) });
  }

  void Region::DrawPrimitive(const Position &position, Text::Ptr p) {
    m_graphics.push_back(GraphicElementPosition{ position, GraphicElement(p) });
  }

  void Region::ClearPrimitives() {
    m_graphics.clear();
  }

  void Region::Clear(bool children) {
    m_graphics.clear();
    if (children) {
      for (auto child : m_children) {
        child.second->Clear(children);
      }
    }
  }


} // namespace TileEngine
