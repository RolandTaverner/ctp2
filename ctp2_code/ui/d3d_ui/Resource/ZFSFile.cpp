#include "ctp/c3.h"

#include <string.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#include "ui/d3d_ui/Resource/ZFSFile.h"

namespace io = boost::iostreams;
namespace bi = boost::interprocess;

namespace ui::d3d {

//------------------------------------------------------------------------------
// ZFSFileBase
//------------------------------------------------------------------------------

ZFSFileBase::ZFSFileBase(const std::string &filePath) :
  m_filePath(filePath) {}

ZFSFileBase::~ZFSFileBase() {}

bool ZFSFileBase::Exists(const std::string &name) const {
  return m_entries.find(name) != m_entries.end();
}

const std::string ZFSFileBase::GetZFSFilePath() const {
  return m_filePath;
}

#define MAX_ENTRIES_PER_TABLE 100
#define MAX_RECORDNAME_LENGTH 16
#define ZFSFLAG_DELETED 0x0001

struct ZFS_RENTRY {
  char           rname[MAX_RECORDNAME_LENGTH];
  std::uint32_t  offset;
  std::uint32_t  rnum;
  std::uint32_t  size;
  std::uint32_t  time;
  std::uint32_t  flags;
};

struct ZFS_DTABLE {
  std::uint32_t next_dtable;
  ZFS_RENTRY    rentry[MAX_ENTRIES_PER_TABLE];
};

struct ZFS_FHEADER {
  char           filetag[4];
  std::uint32_t  version;
  std::uint32_t  max_recordname_length;
  std::uint32_t  max_entries_per_table;
  std::uint32_t  num_rentries;
  std::uint32_t  encrypt_key;
  std::uint32_t  dtable_head;
};

template<typename Device>
void ReadEntriesFromBuf(io::stream_buffer<Device> &sb, ZFSFileBase::Entries &entries) {
  ZFS_FHEADER header;
  memset(&header, 0, sizeof(header));

  if (io::read(sb, reinterpret_cast<char*>(&header), sizeof(header)) != sizeof(header)) {
    throw std::runtime_error("Can't read ZFS_FHEADER");
  }
  if ((header.filetag[0] != 'Z') ||
    (header.filetag[1] != 'F') ||
    (header.filetag[2] != 'S') ||
    (header.filetag[3] != '3') ||
    (header.version != 1) ||
    (header.max_recordname_length != MAX_RECORDNAME_LENGTH) ||
    (header.max_entries_per_table != MAX_ENTRIES_PER_TABLE)) {
    throw std::runtime_error("Invalid ZFS_FHEADER");
  }

  io::stream_offset dtablePos = header.dtable_head;
  ZFS_DTABLE dtable;

  do {
    if (io::seek(sb, dtablePos, std::ios_base::beg, std::ios_base::in) != dtablePos) {
      throw std::runtime_error("Can't seek ZFS buffer");
    }
    if (io::read(sb, reinterpret_cast<char*>(&dtable), sizeof(dtable)) != sizeof(dtable)) {
      throw std::runtime_error("Can't read ZFS_DTABLE. File is corrupt.");
    }

    for (unsigned i = 0; i < MAX_ENTRIES_PER_TABLE && dtable.rentry[i].rname[0]; ++i) {
      if (dtable.rentry[i].flags & ZFSFLAG_DELETED) {
        continue;
      }
      ZFSFileBase::Entry newEntry;
      newEntry.m_name = boost::algorithm::to_lower_copy(std::string(dtable.rentry[i].rname));
      newEntry.m_offset = dtable.rentry[i].offset;
      newEntry.m_size = dtable.rentry[i].size;
      
      entries[newEntry.m_name] = newEntry;
    }

  } while ((dtablePos = dtable.next_dtable) != 0);
}

//------------------------------------------------------------------------------
// ZFSFileMapped
//------------------------------------------------------------------------------
class ZFSFileMapped::Impl {
public:
  explicit Impl(const std::string &filePath) :
    m_fileMapping(filePath.c_str(), bi::read_only), m_region(m_fileMapping, bi::read_only) {}

  void ReadEntries(Entries &entries) {
    const char *begin = static_cast<char*>(m_region.get_address());
    io::stream_buffer<io::array_source> sb(io::array_source(begin, m_region.get_size()));
    ReadEntriesFromBuf<io::array_source>(sb, entries);
  }

  ~Impl() {
    m_region.swap(bi::mapped_region());
    m_fileMapping.swap(bi::file_mapping());
  }

  bi::file_mapping m_fileMapping;
  bi::mapped_region m_region;
};

class ZFSMappedData : public ZFSData {
public:
  ZFSMappedData(const void *begin, std::size_t size) :
    m_begin(begin), m_size(size) {
  }

  virtual ~ZFSMappedData() {}

  std::size_t GetSize() const override {
    return m_size;
  }
  
  const void *GetContent() override {
    return m_begin;
  }

private:
  const void *m_begin;
  std::size_t m_size;
};


ZFSFileMapped::ZFSFileMapped(const std::string &filePath) :
  ZFSFileBase(filePath), m_pImpl(std::make_unique<Impl>(filePath)) {
  m_pImpl->ReadEntries(m_entries);
}

ZFSFileMapped::~ZFSFileMapped() {
}

ZFSDataUPtr ZFSFileMapped::GetData(const std::string &name) {
  Entries::const_iterator it = m_entries.find(boost::algorithm::to_lower_copy(name));
  if (it == m_entries.end()) {
    return ZFSDataUPtr();
  }
  
  return std::make_unique<ZFSMappedData>((char*)m_pImpl->m_region.get_address() + it->second.m_offset, it->second.m_size);
}


ZFSData::~ZFSData() {}

} // namespace ui::d3d