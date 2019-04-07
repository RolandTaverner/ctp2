/*******************************************************************************

	$Workfile:: ldl_file.hpp                                                   $
	  $Author$
	    $Date$
	$Rev$
	 $Archive:: /LDL/test/ldl_file.hpp                                         $

*******************************************************************************/

#ifndef LDL_FILE_HPP
#define LDL_FILE_HPP

#include <memory>

class ldl_datablock;
typedef std::shared_ptr<ldl_datablock> LDLDataBlockPtr;

class ldl {
public:
  ldl();
	ldl(const char *szName, const char *szOutputDir = NULL, BOOL bUseInternalMemmap = TRUE );
	~ldl();

	// read an ldl file and new the data associated with it
	BOOL ReadData(const char *fname = NULL);

	ldl_datablock *FindDataBlock(const char *szName, ldl_datablock *dbParent = NULL );
  LDLDataBlockPtr FindDataBlockPtr(const char *szName) const;
	BOOL WriteData();
};

#endif
