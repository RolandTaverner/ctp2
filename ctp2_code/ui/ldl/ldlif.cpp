#include "ctp/c3.h"

#include <map>
#include <list>
#include <stack>
#include <unordered_map>

#include <inttypes.h>

#include "ui/ldl/ldlif.h"
#include "ui/ldl/ldl_data.hpp"
#include "ui/ldl/ldl_attr.hpp"

#include "gs/fileio/CivPaths.h"
#include "ctp/ctp2_utils/c3errors.h"

#include "gs/slic/StringHash.h"
#include "ctp/ctp2_utils/pointerlist.h"
#include "ctp/ctp2_utils/AvlTree.h"

#include "ui/aui_common/aui_ui.h"

#include "ctp/ctp2_utils/c3errors.h"

class LDLString {
  char *m_name;
public:
  LDLString() : m_name(nullptr) {
  }

  explicit LDLString(const char *text) {
    m_name = new char[strlen(text) + 1];
    strcpy(m_name, text);
  }

  LDLString(LDLString &&rhs) {
    m_name = rhs.m_name;
    rhs.m_name = nullptr;
  }

  LDLString(const LDLString &rhs) : 
    m_name(nullptr) {
    if (rhs.m_name) {
      m_name = new char[strlen(rhs.m_name) + 1];
      strcpy(m_name, rhs.m_name);
    }
  }

  LDLString &operator=(LDLString &&rhs) {
    if (this != &rhs) {
      m_name = rhs.m_name;
      rhs.m_name = nullptr;
    }
    return *this;
  }
  
  LDLString &operator=(const LDLString &rhs) {
    if (this != &rhs) {
      if (m_name) {
        delete[] m_name;
        m_name = nullptr;
      }
      if (rhs.m_name) {
        m_name = new char[strlen(rhs.m_name) + 1];
        strcpy(m_name, rhs.m_name);
      }
    }
    return *this;
  }

  ~LDLString() {
    delete[] m_name;
  }

  char * GetName() const { return m_name; }
};

std::unordered_map<std::string, LDLString> s_ldlStringHash;

std::stack<LDLBlockPtr> s_blockStack;
std::map<std::string, LDLBlockPtr> s_blockTree;

extern "C" { void ldlif_report_error(const char *text); }

void ldlif_report_error(const char *text) {
  c3errors_ErrorDialog("LDL", text);
}

LDLBlockPtr ldlif_find_block(char const * name) {
  auto it = s_blockTree.find(std::string(name));
  if (it != s_blockTree.end()) {
    return it->second;
  }
  return LDLBlockPtr();
}

int ldlif_find_file(const char *filename, char *fullpath) {
  const std::string fp = g_civPaths->FindFile(C3DIR_LAYOUT, filename, fullpath);
  if (fp.empty())
    return 0;
  strcpy(fullpath, fp.c_str());
  return 1;
}

char *ldlif_getnameptr(const char *name) {
  auto itExisting = s_ldlStringHash.find(name);
  if (itExisting != s_ldlStringHash.end()) {
    return itExisting->second.GetName();
  }
  auto [itNew, ok] = s_ldlStringHash.emplace(std::make_pair<>(std::string(name), LDLString(name)));
  return itNew->second.GetName();
}

char *ldlif_getstringptr(const char *text) {
  return ldlif_getnameptr(text);
}

void ldlif_add_name(void **newnames, char *name, void *oldnames) {
  PointerList<char> *namelist = (PointerList<char> *)oldnames;
  if (!namelist) {
    namelist = new PointerList<char>;
  }
  namelist->AddHead(name);
  *newnames = (void *)namelist;
}

void ldlif_init_log() {
#ifdef _DEBUG
  FILE *f = fopen("ldlparselog.txt", "w");
  if (f) {
    fprintf(f, "%" PRId64 "\n", time(0));
    fclose(f);
  }
#endif
}
void ldlif_log(char *format, ...) {
#ifdef _DEBUG
  va_list list;
  va_start(list, format);

  FILE *f = fopen("ldlparselog.txt", "a");
  vfprintf(f, format, list);
  fclose(f);
  va_end(list);
#endif
}

void ldlif_indent_log(int indent) {
#ifdef _DEBUG
  int i;
  for (i = 0; i < indent; i++) {
    ldlif_log("    ");
  }
#endif
}

void ldlif_start_block(void *names) {
  PointerList<char> *namelist = (PointerList<char> *)names;
  Assert(namelist);

  LDLBlockPtr block = std::make_shared<ldl_datablock>(namelist);

  if (!s_blockStack.empty()) {
    s_blockStack.top()->AddChild(block);
  }
  s_blockStack.push(block);

  ldlif_add_block_to_tree(block);
}

void ldlif_add_block_to_tree(LDLBlockPtr block) {
  const std::string fullName = block->GetFullName();
  auto[it, ok] = s_blockTree.emplace(fullName, block);
  ldlif_log("Added: %s\n", fullName.c_str());

  if (!ok) {
    const std::string msg = "Duplicate block " + fullName + "\n";
    ldlif_report_error(msg.c_str());
  }

  for (LDLBlockPtr &c : block->GetChildList()) {
    ldlif_add_block_to_tree(c);
  }
}

void ldlif_remove_block_from_tree(LDLBlockPtr block) {
  s_blockTree.erase(block->GetFullName());
}

void *ldlif_end_block(void *names) {
  PointerList<char> *namelist = (PointerList<char> *)names;
  Assert(namelist);

  LDLBlockPtr block = s_blockStack.top();
  s_blockStack.pop();

  block->AddTemplateChildren();

  delete namelist;
  return block.get(); // TODO: remove
}

void *ldlif_add_empty_block(void *names) {
  ldlif_start_block(names);
  return ldlif_end_block(names);
}

void ldlif_add_bool_attribute(char *name, int val) {
  s_blockStack.top()->AddAttribute(ldl_attribute(name, ATTRIBUTE_TYPE_BOOL, val != 0));
}

void ldlif_add_int_attribute(char *name, int val) {
  s_blockStack.top()->AddAttribute(ldl_attribute(name, ATTRIBUTE_TYPE_INT, val));
}

void ldlif_add_float_attribute(char *name, double val) {
  s_blockStack.top()->AddAttribute(ldl_attribute(name, ATTRIBUTE_TYPE_DOUBLE, val));
}

void ldlif_add_string_attribute(char *name, char *val) {
  s_blockStack.top()->AddAttribute(ldl_attribute(name, ATTRIBUTE_TYPE_STRING, std::string(val)));
}

void ldlif_allocate_stuff() {
}

void ldlif_deallocate_stuff() {
  s_blockStack.swap(std::stack<LDLBlockPtr>());

  s_ldlStringHash.clear();

  s_blockTree.clear();
}
