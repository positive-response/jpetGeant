#ifndef RunManager_h
#define RunManager_h 1
#include "G4RunManager.hh"
#include "EventMessenger.h"

class RunManager : public G4RunManager
{
  public:
    void DoEventLoop(G4int n_event,const char* macroFile=0,G4int n_select=-1) override;

  private:
    EventMessenger* fEvtMessenger = EventMessenger::GetEventMessenger();
    
};

#endif
