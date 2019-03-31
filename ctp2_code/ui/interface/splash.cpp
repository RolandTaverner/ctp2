#include "ctp/c3.h"

#include "ctp/civapp.h"
#include "ui/interface/splash.h"

#ifdef _DEBUG

extern CivApp               *g_civApp;

void SPLASH_STRING(const std::string &str) {
  if (g_civApp && g_civApp->UI()) { 
    g_civApp->UI()->GetSplash()->AddTextNL(str);
  }
}

void SPLASH_STRING_SIMPLE(const std::string &str) {
}


#else
void SPLASH_STRING(const std::string &str) {}
void SPLASH_STRING_SIMPLE(const std::string &str) {}
#endif
