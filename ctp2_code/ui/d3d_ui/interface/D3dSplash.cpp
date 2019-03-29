#include "ctp/c3.h"

#include "D3dTiles/Primitives/ColoredRectangle.h"

#include "ui/d3d_ui/interface/D3dSplash.h"

static const unsigned s_splashMargin = 10;

namespace te = TileEngine;

namespace ui::d3d {

Splash::Splash(te::Region::Ptr region) : m_layer(region) {
  m_textArea = m_layer->AddChild(
    te::Position(s_splashMargin, s_splashMargin),
    m_layer->Width() - 2*s_splashMargin, m_layer->Height() - 2*s_splashMargin);
}

void Splash::AddText(MBCHAR const *text) {
}

void Splash::AddTextNL(MBCHAR const *text) {
}

void Splash::AddHilitedTextNL(MBCHAR const *text) {
}

} // namespace ui::d3d 