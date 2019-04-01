#pragma once

#include <memory>

namespace ui::d3d {

class UIRegion;

typedef std::shared_ptr<UIRegion> UIRegionPtr;
typedef std::weak_ptr<UIRegion> UIRegionWPtr;

} // namespace ui::d3d