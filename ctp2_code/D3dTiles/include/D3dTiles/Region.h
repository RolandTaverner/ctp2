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
    Region(Ptr parent, RegionID id,
      const Position &position, unsigned width, unsigned height, 
      bool isVisible = true, bool strictArea = true);
    Region(const Region &) = delete;
    Region &operator=(const Region &) = delete;
    virtual ~Region();

    RegionID ID() const;
    const Position &Pos() const;
    void SetPosition(const Position &pos);
    unsigned Width() const;
    void SetWidth(unsigned w);
    unsigned Height() const;
    void SetHeight(unsigned h);

    bool IsVisible() const;
    void SetVisible(bool isVisible);
    Rect GetRect() const;
    unsigned GetLevelsCount() const;
    Ptr CreateChild(const Position &position, unsigned width, unsigned height, 
      bool isVisible = true, bool strictArea = true);
    Ptr CreateLayer(unsigned level);
    Ptr CreateLayer();
    void Render(unsigned level, const Position &parentPosition, RendererBasePtr renderer);

    void MoveTo(int x, int y);
    void MoveBy(int dx, int dy);
    void Resize(unsigned width, unsigned height);

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
    bool m_isVisible;
    bool m_strictArea;
    RegionsMap m_children;
    LayersMap m_layers;
    GraphicElements m_graphics;
  };

} // namespace TileEngine
