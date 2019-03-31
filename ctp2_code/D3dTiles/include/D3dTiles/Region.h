#pragma once

#include <list>
#include <map>
#include <memory>
#include <variant>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Primitives/ColoredRectangle.h"
#include "D3dTiles/Primitives/TexturedRectangle.h"
#include "D3dTiles/Primitives/Text.h"

namespace TileEngine {

  class RendererBase;

  class Region : public std::enable_shared_from_this<Region>
  {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;
    typedef std::shared_ptr<Region> Ptr;
    typedef std::weak_ptr<Region> WeakPtr;
    typedef unsigned long RegionID;

  private:
    typedef std::map<RegionID, Ptr> RegionsMap;

    typedef std::map<unsigned, Ptr> LayersMap;

    typedef std::variant<Bitmap::Ptr, ColoredRectangle::Ptr, TexturedRectangle::Ptr, Text::Ptr> GraphicElement;
    typedef struct {  
      Position position;
      GraphicElement element;
    } GraphicElementPosition;
    typedef std::list<GraphicElementPosition> GraphicElements;

  public:
    Region();
    Region(WeakPtr parent, RegionID id, const Position &position, unsigned width, unsigned height);
    virtual ~Region();

    RegionID ID() const;
    const Position &Pos() const;
    unsigned Width() const;
    unsigned Height() const;
    Rect GetRect() const;
    unsigned GetLevelsCount() const;
    Ptr AddChild(const Position &position, unsigned width, unsigned height);
    Ptr AddLayer(unsigned level);
    Ptr AddLayer();
    void Render(unsigned level, const Position &parentPosition, RendererBasePtr renderer);

    void DrawImage(const Position &position, Bitmap::Ptr bitmap);
    void DrawPrimitive(const Position &position, ColoredRectangle::Ptr p);
    void DrawPrimitive(const Position &position, TexturedRectangle::Ptr p);
    void DrawPrimitive(const Position &position, Text::Ptr p);
    void ClearPrimitives();

    void Clear(bool children);

  private:
    void RenderSelf(unsigned level, const Position &position, RendererBasePtr renderer);

  private:
    WeakPtr m_parent;
    RegionID m_ID;
    Position m_position;
    unsigned m_width, m_height;
    RegionsMap m_children;
    LayersMap m_layers;
    GraphicElements m_graphics;
  };

} // namespace TileEngine
