//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : Activision User Interface general resource handling
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// WIN32
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Import structure changed to compile with Mingw
// - Variable scope corrected
// - Crash preventions
// - moved aui_UI::CalculateHash -> aui_Base::CalculateHash
// - fixed some filesystem portability issues
// - Initialized local variables. (Sep 9th 2005 Martin Gühmann)
//
//----------------------------------------------------------------------------

#ifndef __AUI_RESOURCE_H__
#define __AUI_RESOURCE_H__

//----------------------------------------------------------------------------
// Library imports
//----------------------------------------------------------------------------

#include <stdio.h>			// sprintf
#include <string.h>			// strcpy

#include <filesystem>
#include <string>

//----------------------------------------------------------------------------
// Exported names
//----------------------------------------------------------------------------

template <class T> class	aui_Resource;
template <class T> struct	aui_ResourceElement;

//----------------------------------------------------------------------------
// Project imports
//----------------------------------------------------------------------------

#include "ui/aui_common/aui_base.h"
#include "ui/aui_common/auitypes.h"		// AUI_...
#include "ctp/ctp2_utils/c3debug.h"		// Assert
#include "ctp/ctp2_utils/c3files.h"		// C3DIR...
#include "ctp/c3types.h"		// MBCHAR, sint32, uint32
#include "gs/fileio/CivPaths.h"		// g_civPaths
#include "ui/aui_common/tech_wllist.h"	// ListPos, tech_WLList

//----------------------------------------------------------------------------
// Class declarations
//----------------------------------------------------------------------------

template<class TT>
struct aui_ResourceElement
{
	aui_ResourceElement(
		const MBCHAR *newName,
		const MBCHAR *fullPath );
	virtual ~aui_ResourceElement();

	TT		*resource;
	MBCHAR	*name;
	uint32	hash;
	uint32	pathhash;
	sint32	refcount;
};


template<class T>
class aui_Resource
{
public:

	aui_Resource();
	virtual ~aui_Resource();

  T			*Load(const MBCHAR *name, C3DIR dir = C3DIR_DIRECT, uint32 size = 0);

  AUI_ERRCODE	Unload(T *resource);
  AUI_ERRCODE	Unload(const MBCHAR *name);

  AUI_ERRCODE	AddSearchPath(const MBCHAR *path);
  AUI_ERRCODE	RemoveSearchPath(const MBCHAR *path);

  bool FindFile(std::string &fullPath, const std::string &name);

  bool FindFile(MBCHAR *fullPath, const MBCHAR *name);

protected:
	tech_WLList<aui_ResourceElement<T> *>	*m_resourceList;


	static tech_WLList<MBCHAR *>			*m_pathList;

	static sint32							m_resourceRefCount;
};








template<class TT>
aui_ResourceElement<TT>::aui_ResourceElement(
	const MBCHAR *newName,
	const MBCHAR *fullPath )
:	resource(NULL),
	name((newName && fullPath) ? new MBCHAR[strlen(newName) + 1] : NULL),
	hash(aui_Base::CalculateHash(newName)),
	pathhash(aui_Base::CalculateHash(fullPath)),
	refcount(1)
{

	Assert( newName != NULL && fullPath != NULL );
	if ( !newName || !fullPath ) return;
	// Temporary patch: modern code would use std::string and initialiser
	strcpy(name, newName);

	AUI_ERRCODE errcode = AUI_ERRCODE_OK;
	resource = new TT( &errcode, fullPath );
	Assert( AUI_NEWOK(resource,errcode) );
	if ( !AUI_NEWOK(resource,errcode) ) return;

	errcode = resource->Load();
	if (!AUI_SUCCESS(errcode))
	{
		// Temporary patch to prevent access to invalid memory
		delete resource;
		resource = NULL;
	}
}

template<class TT>
aui_ResourceElement<TT>::~aui_ResourceElement()
{
    delete resource;
	delete [] name;
}









template<class T> tech_WLList<MBCHAR *> *aui_Resource<T>::m_pathList = NULL;
template<class T> sint32 aui_Resource<T>::m_resourceRefCount = 0;

template<class T>
aui_Resource<T>::aui_Resource()
{
	m_resourceList = new tech_WLList<aui_ResourceElement<T> *>;
	Assert( m_resourceList != NULL );

	if ( !m_resourceRefCount++ )
	{
		m_pathList = new tech_WLList<MBCHAR *>;
		Assert( m_pathList != NULL );
	}
}

template<class T>
aui_Resource<T>::~aui_Resource()
{
	Assert(!m_resourceList || (m_resourceList->L() == 0));
	delete m_resourceList;
    m_resourceList = NULL;

	if (!--m_resourceRefCount)
	{
		ListPos position = m_pathList->GetHeadPosition();
		for ( sint32 i = m_pathList->L(); i; i-- )
		{
			MBCHAR *path = m_pathList->GetNext( position );
			delete [] path;
		}
		delete m_pathList;
		m_pathList = NULL;
	}
}








template<class T>
AUI_ERRCODE aui_Resource<T>::AddSearchPath( const MBCHAR *path )
{
	Assert(path);
	if (!path) return AUI_ERRCODE_INVALIDPARAM;

	size_t len = strlen(path);
	Assert((len > 0) && (len <= MAX_PATH));
	if ((len <= 0) || (len > MAX_PATH)) return AUI_ERRCODE_INVALIDPARAM;

	const MBCHAR *last = path + len - 1;
	Assert( *last != FILE_SEPC );
	while ( len && ( *last == FILE_SEPC )) {
		if (--len)
			last--;
	}
	if (0 == len) {
		len++;
	}

	MBCHAR *newPath = new MBCHAR[ len + 1 ];
	Assert( newPath != NULL );
	if ( !newPath ) return AUI_ERRCODE_MEMALLOCFAILED;

	strncpy( newPath, path, len);
	newPath[len] = '\0';

	m_pathList->AddTail( newPath );

	return AUI_ERRCODE_OK;
}

template<class T>
AUI_ERRCODE aui_Resource<T>::RemoveSearchPath( const MBCHAR *path )
{

	ListPos position = m_pathList->GetHeadPosition();
	for ( sint32 i = m_pathList->L(); i; i-- )
	{
		ListPos prevPosition = position;

		MBCHAR *thisPath = m_pathList->GetNext( position );
		if ( strcmp( path, thisPath ) == 0 )
		{
			m_pathList->DeleteAt( prevPosition );
			delete [] thisPath;
			return AUI_ERRCODE_OK;
		}
	}

	return AUI_ERRCODE_OK;
}

template<class T>
T *aui_Resource<T>::Load( const MBCHAR *resName, C3DIR dir, uint32 size) {
	Assert(resName);
	if (!resName || strlen(resName) == 0) 
    return NULL;

	std::string name;
	MBCHAR tempName[MAX_PATH + 1];

	if (size) {
		sprintf(tempName, "%s%d", resName, size);
		name = tempName;
	} else {
		name = resName;
	}

	uint32 hash = aui_Base::CalculateHash( name.c_str() );

	ListPos position = m_resourceList->GetHeadPosition();
	for ( sint32 i = m_resourceList->L(); i; i-- )
	{
		aui_ResourceElement<T> *re = m_resourceList->GetNext( position );
		if (((hash == re->hash) && (strcmp(name.c_str(), re->name) == 0))   ||
		    ((hash == re->pathhash) &&
             (!re->resource || (strcmp(name.c_str(), re->resource->GetFilename()) == 0))
            )
           )
		{
			re->refcount++;
			return re->resource;
		}
	}

	if (size)
		name = resName;

  std::string fullPath = name;

	if (dir != C3DIR_DIRECT) {
		std::string path = g_civPaths->FindFile(dir, name, true);
		if (!path.empty()) {
			fullPath = path;
		} else {
			if (dir == C3DIR_PICTURES) {
        path = g_civPaths->FindFile(C3DIR_PATTERNS, name, true);
				if (!path.empty()) {
					fullPath = path;
				} else {
          path = g_civPaths->FindFile(C3DIR_ICONS, name, true);
					if (!path.empty()) {
						fullPath = path;
					} else {
						FindFile( fullPath, name );
					}
				}
			} else {
				FindFile( fullPath, name );
			}
		}
	} else {
		FindFile( fullPath, name );
	}

	aui_ResourceElement<T> *re = new aui_ResourceElement<T>( name.c_str(), fullPath.c_str());
	Assert( re != NULL );
	if ( re )
	{
		m_resourceList->AddTail( re );
		return re->resource;
	}

	return NULL;
}

template<class T>
bool aui_Resource<T>::FindFile(std::string &fullPath, const std::string &name) {
  if (name.find_first_of(":") == std::string::npos && name.compare(0, 2, FILE_SEP FILE_SEP)) {
    ListPos position = m_pathList->GetHeadPosition();
    if (position) {
      for (sint32 i = m_pathList->L(); i; --i) {
        const std::filesystem::path p = std::filesystem::path(m_pathList->GetNext(position)) / name;
        fullPath = p.string();
        if (std::filesystem::exists(p) && std::filesystem::is_regular_file(p)) {
          return true;
        }
      }

      fullPath = name;
    }
  }

  return false;
}

template<class T>
bool aui_Resource<T>::FindFile( MBCHAR *fullPath, const MBCHAR *name )
{
	if ( !strchr( name, ':' ) && strncmp( name, FILE_SEP FILE_SEP, 2 ) ) {
		ListPos position = m_pathList->GetHeadPosition();
		if ( position ) {
			for ( sint32 i = m_pathList->L(); i; i-- ) {
				MBCHAR *path = m_pathList->GetNext( position );
				sprintf( fullPath, "%s%s%s", path, FILE_SEP, name );

#if defined(WIN32)
				if ( GetFileAttributes( fullPath ) != 0xffffffff )
#else
            struct stat st;
            if (0 == stat(fullPath, &st))
#endif
				{
                    return true;
				}
			}

				strncpy( fullPath, name, MAX_PATH );
		}
	}

    return false;
}

template<class T>
AUI_ERRCODE aui_Resource<T>::Unload( T *resource )
{
	ListPos position = m_resourceList->GetHeadPosition();
	for ( sint32 i = m_resourceList->L(); i; i-- )
	{
		ListPos prevPosition = position;

		aui_ResourceElement<T> *re = m_resourceList->GetNext( position );
		if (resource == re->resource)
        {
            if (!--re->refcount)
            {
				m_resourceList->DeleteAt(prevPosition);
			    delete re;
            }
			return AUI_ERRCODE_OK;
		}
	}

	return AUI_ERRCODE_OK;
}

template<class T>
AUI_ERRCODE aui_Resource<T>::Unload( const MBCHAR *name )
{
	uint32 hash = aui_Base::CalculateHash( name );

	ListPos position = m_resourceList->GetHeadPosition();
	for ( sint32 i = m_resourceList->L(); i; i-- )
	{
		ListPos prevPosition = position;

		aui_ResourceElement<T> *re = m_resourceList->GetNext( position );
		if (((hash == re->hash) && (strcmp(name, re->name) == 0))   ||
		    ((hash == re->pathhash) &&
             (!re->resource || (strcmp(name, re->resource->GetFilename()) == 0))
            )
           )
		{
			if (!--re->refcount)
			{
				m_resourceList->DeleteAt(prevPosition);
			    delete re;
            }
			return AUI_ERRCODE_OK;
		}
	}

	return AUI_ERRCODE_OK;
}

#endif
