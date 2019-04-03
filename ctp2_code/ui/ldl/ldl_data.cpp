#include "ctp/c3.h"

#include "ui/ldl/ldl_file.hpp"
#include "ui/ldl/ldl_data.hpp"

#include "ui/ldl/ldlif.h"

ldl_datablock::ldl_datablock(PointerList<char> *templateNames) :
  m_attributes() {
  PointerList<char>::Walker walk(templateNames);

  Assert(walk.IsValid());
  m_name = walk.GetObj();
  walk.Next();

  while (walk.IsValid()) {
    LDLBlockPtr temp = ldlif_find_block(walk.GetObj());
    if (temp) {
      m_templates.push_back(temp);
    } else {
      ldlif_log("Unknown template %s\n", walk.GetObj());
    }
    walk.Next();
  }
}

ldl_datablock::ldl_datablock(const ldl_datablock &rhs) :
  m_name(rhs.m_name) {
  for (const ldl_attribute &attr : rhs.GetAttributes()) {
    m_attributes.push_back(attr);
  }
  for (const LDLBlockPtr &c : rhs.m_children) {
    AddChild(std::make_shared<ldl_datablock>(*c));
  }
  for (const LDLBlockPtr &t : rhs.m_templates) {
    m_templates.push_back(std::make_shared<ldl_datablock>(*t));
  }
}

ldl_datablock::ldl_datablock(sint32 hash) :
  m_attributes() {}

ldl_datablock::ldl_datablock(ldl *theLdl, char const * name) :
  m_attributes(),
  m_name(ldlif_getnameptr(name)) {
  ldlif_add_block_to_tree(shared_from_this());
}

ldl_datablock::~ldl_datablock() {
  m_children.clear();
  m_templates.clear();

  if (!GetName().empty())
    ldlif_remove_block_from_tree(shared_from_this());
}

std::string ldl_datablock::GetFullName() const {
  std::string output;
  if (!m_parent.expired()) {
    auto parent = m_parent.lock();
    output = parent->GetFullName();
    output += ".";
    output += m_name;
  } else {
    output = m_name;
  }

  return output;
}

const ldl_attribute ldl_datablock::GetAttribute(const char *szName) const {
  return LDLAttrListFindAttribute(m_attributes, szName).first;
}

int ldl_datablock::GetAttributeType(const char *szName) const {
  auto[attr, ok] = LDLAttrListFindAttribute(m_attributes, szName);
  if (ok) {
    return attr.GetType();
  }

  return ATTRIBUTE_TYPE_UNKNOWN;
}

void ldl_datablock::Dump(sint32 indent) {

  PointerList<ldl_datablock>::Walker bwalk;
  ldlif_indent_log(indent);

  ldlif_log("%s", m_name.c_str());

  for (const LDLBlockPtr &b : m_templates) {
    ldlif_log(":%s", b->GetName().c_str());
  }
  ldlif_log(" {\n");

  for (const ldl_attribute &attr : m_attributes) {
    ldlif_indent_log(indent);
    ldlif_log("    %s %s %s\n", attr.GetTypeName().c_str(),
      attr.GetName().c_str(),
      attr.GetValueText().c_str());
  }

  for (LDLBlockPtr &b : m_templates) {
    b->Dump(indent + 1);
  }

  ldlif_indent_log(indent);
  ldlif_log("}\n");
}

bool ldl_datablock::AttributeNameTaken(char *szName) {
  return !GetAttribute(szName).IsEmpty();
}

void ldl_datablock::SetValue(char *name, int value) {
  ldl_attributelist::iterator it = std::find_if(std::begin(m_attributes),
    std::end(m_attributes),
    [&name](const ldl_attribute &v) { return v.GetName() == name; });

  if (it != m_attributes.end()) {
    Assert(it->GetType() == ATTRIBUTE_TYPE_INT);
    it->SetValueInt(value);
  }
}

void ldl_datablock::AddTemplateChildren() {
  for (const LDLBlockPtr &b : m_templates) {
    CopyAttributesFrom(b);
    b->AddTemplateChildrenTo(shared_from_this());
  }
}

void ldl_datablock::AddTemplateChildrenTo(LDLBlockPtr block) {
  for (const LDLBlockPtr &child : m_children) {
    bool alreadyHaveIt = false;
    for (LDLBlockPtr &childArg : block->m_children) {
      if (childArg->GetName() == child->GetName()) {
        childArg->CopyAttributesFrom(child);
        for (const LDLBlockPtr &templ : child->m_templates) {
          childArg->m_templates.push_back(templ);
        }
        childArg->AddTemplateChildren();
        alreadyHaveIt = true;
        break;
      }
    }

    if (!alreadyHaveIt) {
      LDLBlockPtr newblock = std::make_shared<ldl_datablock>(*child);
      block->AddChild(newblock);
      newblock->AddTemplateChildren();
      ldlif_add_block_to_tree(newblock);
    }
  }
}

void ldl_datablock::CopyAttributesFrom(LDLBlockPtr templ) {
  for (const ldl_attribute &attr : templ->GetAttributes()) {
    auto[existingAttr, ok] = LDLAttrListFindAttribute(m_attributes, attr.GetName());
    if (!ok) {
      m_attributes.push_back(attr);
    }
  }

  for (LDLBlockPtr &templChild : templ->m_children) {
    bool foundIt = false;
    for (LDLBlockPtr &child : m_children) {
      if (child->GetName() == templChild->GetName()) {
        child->CopyAttributesFrom(templChild);
        foundIt = true;
        break;
      }
    }
    if (!foundIt) {
      LDLBlockPtr newblock = std::make_shared<ldl_datablock>(*templChild);
      newblock->CopyAttributesFrom(templChild);
      AddChild(newblock);
      ldlif_add_block_to_tree(newblock);
    }
  }
}
