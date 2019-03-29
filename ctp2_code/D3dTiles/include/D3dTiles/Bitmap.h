#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <boost/shared_array.hpp>

namespace TileEngine {

class Bitmap {
public:
  typedef std::uint32_t Pixel;
  typedef boost::shared_array<Pixel> PixelData;
  typedef boost::shared_array<const Pixel> ConstPixelData;
  typedef std::shared_ptr<Bitmap> Ptr;
  typedef std::shared_ptr<const Bitmap> ConstPtr;

  Bitmap(unsigned width, unsigned height, const std::string &staticID, PixelData pixels);
  Bitmap(unsigned width, unsigned height, PixelData pixels);
  Bitmap(unsigned width, unsigned height);
  Bitmap(const Bitmap &rhs);
  Bitmap &operator=(const Bitmap &rhs);
  Bitmap(Bitmap &&rhs);
  Bitmap &operator=(Bitmap &&rhs);
  virtual ~Bitmap();

  unsigned Width() const;
  unsigned Height() const;
  const std::string &StaticID() const;
  bool IsStatic() const;
  std::size_t BufSize() const;
  ConstPixelData GetPixels() const;
  PixelData GetPixelsForUpdate();

private:
  unsigned m_width;
  unsigned m_height;
  std::string m_staticID;
  PixelData m_pixels;
};

} // namespace TileEngine
