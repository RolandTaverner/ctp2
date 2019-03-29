#pragma once

#include <list>
#include <map>
#include <memory>
#include <variant>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/Geometry.h"

namespace TileEngine {

  class RendererBase;

  class Region : public std::enable_shared_from_this<Region>
  {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;
    typedef std::shared_ptr<Region> RegionPtr;
    typedef std::weak_ptr<Region> RegionWeakPtr;
    typedef unsigned long RegionID;

  private:
    typedef std::map<RegionID, RegionPtr> RegionsMap;

    typedef std::map<unsigned, RegionPtr> LayersMap;

    typedef std::variant<Bitmap::BitmapPtr> GraphicElement;
    typedef struct {  
      Position position;
      GraphicElement element;
    } GraphicElementPosition;
    typedef std::list<GraphicElementPosition> GraphicElements;

  public:
    Region();
    Region(RegionWeakPtr parent, RegionID id, const Position &position, unsigned width, unsigned height);
    virtual ~Region();

    RegionID ID() const;
    const Position &Pos() const;
    unsigned Width() const;
    unsigned Height() const;
    Rect GetRect() const;
    unsigned GetLevelsCount() const;
    RegionPtr AddChild(const Position &position, unsigned width, unsigned height);
    RegionPtr AddLayer(unsigned level);
    RegionPtr AddLayer();
    void Render(unsigned level, const Position &parentPosition, RendererBasePtr renderer);

    void DrawPrimitive();
    void DrawImage(const Position &position, Bitmap::BitmapPtr bitmap);

    void Clear(bool children);

  private:
    void RenderSelf(unsigned level, const Position &position, RendererBasePtr renderer);

  private:
    RegionWeakPtr m_parent;
    RegionID m_ID;
    Position m_position;
    unsigned m_width, m_height;
    RegionsMap m_children;
    LayersMap m_layers;
    GraphicElements m_graphics;
  };

} // namespace TileEngine
