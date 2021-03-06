#ifndef __AUI_DIRECTINPUT_H__
#define __AUI_DIRECTINPUT_H__

#ifdef __AUI_USE_DIRECTX__

#include "ui/aui_common/aui_input.h"
#include "ui/aui_directx/aui_directx.h"


class aui_DirectInput : public virtual aui_Input, public aui_DirectX
{
public:

	aui_DirectInput(
		AUI_ERRCODE *retval,
		BOOL useExclusiveMode = FALSE );
	virtual ~aui_DirectInput();

protected:
	aui_DirectInput() {}
	AUI_ERRCODE InitCommon( BOOL useExclusiveMode );

public:

	virtual AUI_ERRCODE Acquire( void );
	virtual AUI_ERRCODE Unacquire( void );

protected:
	LPDIRECTINPUTDEVICE8	m_lpdid;
	HANDLE				m_inputEvent;
};

typedef aui_DirectInput aui_NativeInput;

#endif

#endif
