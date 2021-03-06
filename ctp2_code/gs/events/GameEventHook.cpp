//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Event handler object
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
// _DEBUG
// Generate extra debugging output.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Redesigned (replaced PointerList with std::list) to repair memory leaks.
// - Made the order of callback execution more logical: adding at the end of
//   the sublist with the same priority.
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"                 // Precompiled header with general items
#include "gs/events/GameEventHook.h"      // Own declarations (consistency check)

#include <algorithm>            // std::for_each
#include <functional>           // std::unary_function
#include "gs/events/GameEventManager.h"   // EVENTLOG

namespace
{

//----------------------------------------------------------------------------
//
// Name       : DumpToFile
//
// Description: Function object to dump callback data to a file.
//
// Parameters : f       : file to dump to
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : Assumption: file exists and is open for writing.
//
//----------------------------------------------------------------------------
class DumpToFile : public std::unary_function<GameEventHook::Node const &, void>
{
public:
    DumpToFile(FILE * f) : m_file(f) { ; };

    void operator () (GameEventHook::Node const & object)
    {
        char str[1024];
        object.m_cb->GetDescription(str, 1024);
        fprintf(m_file, "  %s\n", str);
    };

private:
    FILE * m_file;
};

} // namespace

char const GameEventHookCallback::DESCRIPTION_MISSING[] = "An undescribed callback";

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::GameEventHook
//
// Description: Constructor
//
// Parameters : type    : the game event that is handled by this handler list
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------
GameEventHook::GameEventHook(GAME_EVENT type)
:   m_type      (type),
    m_callbacks ()
{
}

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::GameEventHook
//
// Description: Destructor
//
// Parameters : -
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------
GameEventHook::~GameEventHook()
{
    std::list<Node>().swap(m_callbacks);
}

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::AddCallback
//
// Description: Add a <callback, priority> pair to the event handler list.
//
// Parameters : cb      : the callback to add
//              pri     : the priority
//
// Globals    : -
//
// Returns    : -       : may throw std::bad_alloc when no more memory
//
// Remark(s)  : * The new (Node) pair will be inserted before the first Node
//                - if any - with higher priority, so the list will stay
//                ordered when it was ordered when entering this function.
//              * NULL-callbacks will not be entered in the list, so we don't
//                have to check this later.
//
//----------------------------------------------------------------------------
void GameEventHook::AddCallback
(
    GameEventHookCallback * cb,
	GAME_EVENT_PRIORITY     pri
)
{
    Assert(cb);
    if (cb)
    {
#if defined(_DEBUG)
        char desc   [1024];
        cb->GetDescription(desc, 1024);
        Assert(strncmp(desc, GameEventHookCallback::DESCRIPTION_MISSING, 1024));
#endif

        std::list<Node>::iterator   walk = m_callbacks.begin();

        while ((walk != m_callbacks.end()) && (walk->m_priority <= pri))
        {
            ++walk;
        }

        m_callbacks.insert(walk, Node(cb, pri));
    }
}

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::RemoveCallback
//
// Description: Remove a callback from the event handler list.
//
// Parameters : cb      : the callback to remove
//
// Globals    : -
//
// Returns    : void    : should always succeed
//
// Remark(s)  : At the moment, this function is used only for cleaning up
//              (unique) SlicSegment pool objects that are being destroyed,
//              so we return immediately after deleting the first found.
//
//----------------------------------------------------------------------------
void GameEventHook::RemoveCallback(GameEventHookCallback * cb)
{
    std::list<Node>::iterator   walk = m_callbacks.begin();

    while (walk != m_callbacks.end())
    {
        if (walk->m_cb == cb)
        {
            walk = m_callbacks.erase(walk);
            return;
        }
        else
        {
            ++walk;
        }
    }
}

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::Activate
//
// Description: Resume executing the event handlers in the list in sequence.
//
// Parameters : args            : parameters to pass to the event handlers
//              startIndex      : the index of the first event handler to
//                                execute
//
// Globals    : -
//
// Returns    : resumeIndex     : the index of the first event handler that
//                                has not been executed yet.
//              GAME_EVENT_ERR  : result of execution
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------
GAME_EVENT_ERR GameEventHook::Activate
(
    GameEventArgList *  args,
    sint32              startIndex,
    sint32 &            resumeIndex
) const
{
    resumeIndex = 0;

	for
    (
        std::list<Node>::const_iterator walk = m_callbacks.begin();
        walk != m_callbacks.end();
        ++walk
    )
    {
        Node const &    runningNow  = *walk;
#if defined(_DEBUG)
        char desc[1024];
	    runningNow.m_cb->GetDescription(desc, 1024);
#endif //_DEBUG

        ++resumeIndex;  // updated before execution

        if (resumeIndex <= startIndex)
        {
            // No action yet
        }
        else
        {
#if defined(_DEBUG)
	        EVENTLOG(("  %s\n", desc));
#endif //_DEBUG

		    GAME_EVENT_HOOK_DISPOSITION const disp =
                runningNow.m_cb->GEVHookCallback(m_type, args);

		    switch (disp)
            {
		    default:
			    Assert(disp == GEV_HD_Continue);
                // Continue execution with the next handler in the list.
                break;

		    case GEV_HD_NeedUserInput:
			    return GEV_ERR_NeedUserInput;

		    case GEV_HD_Stop:
                Assert(runningNow.m_priority <= GEV_PRI_Primary);
			    return GEV_ERR_StopProcessing;

            } // switch
        }
    } // for

    return GEV_ERR_OK;
}

#if defined(_DEBUG)

//----------------------------------------------------------------------------
//
// Name       : GameEventHook::Dump
//
// Description: Log a description of all event handlers in the list to file.
//
// Parameters : f               : file to log to
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : Assumption: file exists and is open for writing.
//
//----------------------------------------------------------------------------
void GameEventHook::Dump(FILE * f) const
{
    if (m_callbacks.empty())
    {
		fprintf(f, "  <empty>\n");
	}
    else
    {
        std::for_each(m_callbacks.begin(), m_callbacks.end(), DumpToFile(f));
	}
}

#endif // _DEBUG
