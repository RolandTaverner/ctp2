//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Slic segment list
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
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_uniqueid.h"
#include "ui/aui_ctp2/c3ui.h"
#include "ui/aui_common/aui_ldl.h"
#include "ui/aui_common/aui_uniqueid.h"
#include "ui/aui_common/aui_static.h"
#include "ui/aui_common/aui_hypertextbox.h"

#include "ui/aui_ctp2/c3_static.h"
#include "ui/aui_ctp2/thermometer.h"

#include "ui/aui_ctp2/textbutton.h"
#include "ui/aui_ctp2/c3_button.h"

#include "ui/aui_ctp2/c3textfield.h"

#include "ui/aui_ctp2/c3listbox.h"
#include "ui/aui_ctp2/c3_listbox.h"
#include "ui/aui_common/aui_listbox.h"

#include "ui/aui_ctp2/c3window.h"
#include "ui/aui_ctp2/c3windows.h"
#include "ui/aui_ctp2/c3_popupwindow.h"
#include "ui/aui_ctp2/c3_utilitydialogbox.h"

#include "ui/aui_ctp2/keypress.h"

#include "ui/slic_debug/segmentlist.h"
#include "ui/slic_debug/sourcelist.h"
#include "ui/slic_debug/watchlist.h"

#include "gs/slic/SlicEngine.h"
#include "gs/slic/SlicSegment.h"

extern C3UI			*g_c3ui;
SegmentList *g_segmentList = NULL;

void segmentlist_Callback(sint32 arg)
{
}

void segmentlist_Display()
{
	if(!g_segmentList) {
		g_segmentList = new SegmentList(segmentlist_Callback);
	}
	g_segmentList->DisplayWindow();
}

void segmentlist_Remove()
{
	if(g_segmentList) {
		g_segmentList->RemoveWindow();
	}
}

SegmentList::SegmentList(SegmentListCallback *callback, MBCHAR *ldlBlock)
:   KeyboardHandler     (),
    m_window            (NULL),
	m_list              (NULL),
	m_watchButton       (NULL),
	m_exitButton        (NULL),
    m_callback          (callback)
{
	AUI_ERRCODE errcode = AUI_ERRCODE_OK;
	MBCHAR		windowBlock[ k_AUI_LDL_MAXBLOCK + 1 ];

	if (ldlBlock) strcpy(windowBlock,ldlBlock);
	else strcpy(windowBlock,"SegmentListPopup");

	{
		m_window = new c3_PopupWindow( &errcode, aui_UniqueId(), windowBlock, 16, AUI_WINDOW_TYPE_FLOATING, false);
		Assert( AUI_NEWOK(m_window, errcode) );
		if ( !AUI_NEWOK(m_window, errcode) ) return;

		m_window->Resize(m_window->Width(),m_window->Height());
		m_window->GrabRegion()->Resize(m_window->Width(),m_window->Height());
		m_window->SetStronglyModal(FALSE);
		m_window->SetDraggable(TRUE);

	}

	Initialize( windowBlock );
}

SegmentList::~SegmentList(void)
{
    // m_callback not deleted: reference only
	delete m_exitButton;
	delete m_watchButton;

    if (m_list)
    {
        m_list->Clear();
    	delete m_list;
    }

    RemoveWindow();
	delete m_window;
}

void SegmentListActionCallback(aui_Control *control, uint32 action, uint32 data, void *cookie)
{
	if((action != (uint32)AUI_LISTBOX_ACTION_SELECT) &&
	   (action != (uint32)AUI_LISTBOX_ACTION_RMOUSESELECT) &&
	   (action != (uint32)AUI_LISTBOX_ACTION_DOUBLECLICKSELECT))
		return;

	SegmentList *list = (SegmentList *)cookie;

	SegmentListItem *item = (SegmentListItem *)list->GetList()->GetSelectedItem();

	if(!item)
		return;

	if(action == AUI_LISTBOX_ACTION_DOUBLECLICKSELECT) {
		item->Open();
	}

	if(action == AUI_LISTBOX_ACTION_RMOUSESELECT) {
		return;
	}

	return;
}

void SegmentListButtonCallback(aui_Control *control, uint32 action, uint32 data, void *cookie)
{
	if(action == AUI_BUTTON_ACTION_EXECUTE) {
		if(control == g_segmentList->m_watchButton) {
			watchlist_Display();
		}

		if(control == g_segmentList->m_exitButton) {
			segmentlist_Remove();
		}

	}
}

sint32 SegmentList::Initialize(MBCHAR *windowBlock)
{
	AUI_ERRCODE errcode = AUI_ERRCODE_OK;
	MBCHAR		controlBlock[ k_AUI_LDL_MAXBLOCK + 1 ];




	sprintf( controlBlock, "%s.%s", windowBlock, "SegmentList" );
	m_list = new c3_ListBox(&errcode, aui_UniqueId(), controlBlock, SegmentListActionCallback, this);

	Assert( AUI_NEWOK(m_list, errcode) );
	if ( !AUI_NEWOK(m_list, errcode) ) return -1;

	sprintf(controlBlock, "%s.%s", windowBlock, "WatchButton");
	m_watchButton = new c3_Button(&errcode, aui_UniqueId(), controlBlock, SegmentListButtonCallback, this);
	Assert(AUI_NEWOK(m_watchButton, errcode));
	if(!AUI_NEWOK(m_watchButton, errcode)) return -1;

	sprintf(controlBlock, "%s.%s", windowBlock, "ExitButton");
	m_exitButton = new c3_Button(&errcode, aui_UniqueId(), controlBlock, SegmentListButtonCallback, this);
	Assert(AUI_NEWOK(m_exitButton, errcode));
	if(!AUI_NEWOK(m_exitButton, errcode)) return -1;

	errcode = aui_Ldl::SetupHeirarchyFromRoot( windowBlock );
	Assert( AUI_SUCCESS(errcode) );

	return 0;
}

void SegmentList::DisplayWindow(void)
{
	UpdateData();
	AUI_ERRCODE const auiErr = g_c3ui->AddWindow(m_window);
	Assert(auiErr == AUI_ERRCODE_OK);
	keypress_RegisterHandler(this);
}

void SegmentList::RemoveWindow(void)
{
    if (g_c3ui && m_window)
    {
	    g_c3ui->RemoveWindow(m_window->Id());
    }
	keypress_RemoveHandler(this);
}

void SegmentList::kh_Close()
{
	RemoveWindow();
}

sint32 SegmentList::UpdateData(void)
{
	MBCHAR ldlBlock[k_AUI_LDL_MAXBLOCK + 1];
	strcpy(ldlBlock, "SegmentListItem");

	m_list->BuildListStart();
    {
	m_list->Clear();

	SlicSegmentHash *hash = g_slicEngine->GetSegmentHash();
	    for (sint32 i = 0; i < hash->m_numSegments; ++i)
        {
            AUI_ERRCODE         retval;
		    SegmentListItem *   item    =
                new SegmentListItem(&retval, i, hash->m_segments[i], ldlBlock);
		m_list->AddItem((c3_ListItem *)item);
	}
    }
	m_list->BuildListEnd();

	return 0;
}

SegmentListItem::SegmentListItem(AUI_ERRCODE *retval, sint32 index,
								 SlicSegment *segment, MBCHAR *ldlBlock) :
	aui_ImageBase(ldlBlock),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	c3_ListItem(retval, ldlBlock)
{
	m_index = index;
	m_segment = segment;

	Assert(AUI_SUCCESS(*retval));
	if(!AUI_SUCCESS(*retval)) return;

	*retval = InitCommonLdl(segment, ldlBlock);
	Assert(AUI_SUCCESS(*retval));
	if(!AUI_SUCCESS(*retval)) return;
}

AUI_ERRCODE SegmentListItem::InitCommonLdl(SlicSegment *segment,
										   MBCHAR *ldlBlock)
{
	MBCHAR			block[ k_AUI_LDL_MAXBLOCK + 1 ];
	AUI_ERRCODE		retval;

	sprintf(block, "%s.%s", ldlBlock, "Segment");
	c3_Static * subItem = new c3_Static(&retval, aui_UniqueId(), block);
	AddChild(subItem);

	Update();
	return AUI_ERRCODE_OK;
}

void SegmentListItem::Update(void)
{
	c3_Static * name = static_cast<c3_Static *>(GetChildByIndex(0));
	name->SetText(m_segment->GetName());
}

sint32 SegmentListItem::Compare(c3_ListItem *item2, uint32 column)
{
	return 0;
}

void SegmentListItem::Open()
{
	sourcelist_Display(m_segment);
}
