#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <boost/noncopyable.hpp>

namespace ui::d3d {

class ZFSData {
public:
  virtual ~ZFSData();
  virtual std::size_t GetSize() const = 0;
  virtual const void *GetContent() = 0;
};

typedef std::unique_ptr<ZFSData> ZFSDataUPtr;

class ZFSFileBase : boost::noncopyable {
public:
  class Entry {
  public:
    std::string m_name;
    std::size_t m_offset;
    std::size_t m_size;
  };
  typedef std::unordered_map<std::string, Entry> Entries;

public:
  explicit ZFSFileBase(const std::string &filePath);
  virtual ~ZFSFileBase();

  bool Exists(const std::string &name) const;
  const std::string GetZFSFilePath() const;
  virtual ZFSDataUPtr GetData(const std::string &name) = 0;

protected:
  std::string m_filePath;
  Entries m_entries;
};

typedef std::unique_ptr<ZFSFileBase> ZFSFileBaseUPtr;

class ZFSFileMapped : public ZFSFileBase {
public:
  explicit ZFSFileMapped(const std::string &filePath);
  virtual ~ZFSFileMapped();

  ZFSDataUPtr GetData(const std::string &name) override;

private:
  class Impl;
  std::unique_ptr<Impl> m_pImpl;
};


} // namespace ui::d3d
