#pragma once

#include <memory>
#include <string>

class aui_FileFormat;

namespace ui::d3d {

typedef std::unique_ptr<aui_FileFormat> AUIFileFormat;

AUIFileFormat CreateFileFormat(const std::string &fileName);

} // namespace ui::d3d