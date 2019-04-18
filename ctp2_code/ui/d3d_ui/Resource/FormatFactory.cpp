#include "ctp/c3.h"

#include <filesystem>

#include <boost/algorithm/string/case_conv.hpp>

#include "ui/aui_common/aui_image.h"
#include "ui/aui_common/aui_sound.h"
#include "ui/aui_common/aui_movie.h"
#include "ui/aui_ctp2/c3imageformats.h" // TargaImageFormat, TiffImageFormat

#include "FormatFactory.h"

namespace ui::d3d {
AUIFileFormat CreateFileFormat(const std::string &fileName) {
  const std::string ext = boost::algorithm::to_lower_copy(std::filesystem::path(fileName).extension().string());
  /*
  if (ext == "tga") {
    return std::make_unique<TargaImageFormat>();
  } else if (ext == "tif") {
    return std::make_unique<TiffImageFormat>();
  } else if (ext == "bmp") {
    return std::make_unique<aui_BmpImageFormat>();
  } else if (ext == "wav") {
    return std::make_unique<aui_WavSoundFormat>();
  } else if (ext == "avi") {
    return std::make_unique<aui_AviMovieFormat>();
  }
  */

  return AUIFileFormat();
}
} // namespace ui::d3d