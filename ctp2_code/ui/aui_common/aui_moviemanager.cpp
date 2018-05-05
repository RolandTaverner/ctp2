#include "ctp/c3.h"
#include "ui/aui_common/aui_moviemanager.h"


aui_MovieManager::aui_MovieManager(bool init)
:
    aui_Base		(),
    m_movieResource     (init ? new aui_Resource<aui_Movie>() : NULL)
{
}

aui_MovieManager::~aui_MovieManager()
{
    delete m_movieResource;
}
