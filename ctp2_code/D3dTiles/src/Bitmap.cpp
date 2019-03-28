#include "stdafx.h"

#include <string.h>

#include "Bitmap.h"

namespace TileEngine {

  Bitmap::Bitmap(unsigned width, unsigned height, const std::string &staticID, Bitmap::PixelData pixels) :
    m_width(width), m_height(height), m_staticID(staticID), m_pixels(pixels) {
  }

  Bitmap::Bitmap(unsigned width, unsigned height, Bitmap::PixelData pixels) :
    Bitmap(width, height, "", pixels) {
  }

  Bitmap::Bitmap(unsigned width, unsigned height) : Bitmap(width, height, Bitmap::PixelData()) {
    m_pixels.reset(new Pixel[width*height]);
    memset(m_pixels.get(), 0, sizeof(Pixel)*width*height);
  }

  Bitmap::Bitmap(const Bitmap &rhs) {
    *this = rhs;
  }

  Bitmap &Bitmap::operator=(const Bitmap &rhs) {
    if (this != &rhs) {
      m_staticID = rhs.m_staticID;
      m_width = rhs.m_width;
      m_height = rhs.m_height;
      if (IsStatic()) {
        m_pixels = rhs.m_pixels;
      }
      else {
        m_pixels.reset(new Pixel[m_width*m_height]);
        if (rhs.m_pixels) {
          memcpy(m_pixels.get(), rhs.m_pixels.get(), BufSize());
        }
      }
    }
    return *this;
  }

  Bitmap::Bitmap(Bitmap &&rhs) {
    m_staticID = std::move(rhs.m_staticID);
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_pixels = std::move(rhs.m_pixels);
    rhs.m_width = rhs.m_height = 0;
  }

  Bitmap &Bitmap::operator=(Bitmap &&rhs) {
    if (this != &rhs) {
      m_staticID = std::move(rhs.m_staticID);
      m_width = rhs.m_width;
      m_height = rhs.m_height;
      m_pixels = std::move(rhs.m_pixels);
      rhs.m_width = rhs.m_height = 0;
    }
    return *this;
  }

  Bitmap::~Bitmap() {
  }

  std::size_t Bitmap::BufSize() const {
    return sizeof(Pixel)*Width()*Height();
  }

  const std::string &Bitmap::StaticID() const {
    return m_staticID;
  }

  bool Bitmap::IsStatic() const {
    return !m_staticID.empty();
  }

  unsigned Bitmap::Width() const {
    return m_width;
  }

  unsigned Bitmap::Height() const {
    return m_height;
  }

  Bitmap::ConstPixelData Bitmap::GetPixels() const {
    return m_pixels;
  }

  Bitmap::PixelData Bitmap::GetPixelsForUpdate() {
    if (IsStatic()) {
      throw std::runtime_error("Can't update static data pixels");
    }

    return m_pixels;
  }

} // namespace TileEngine