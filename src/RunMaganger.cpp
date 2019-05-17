#include "RunManager.h"
#include "EventAction.h"

void RunManager::DoEventLoop(G4int n_event,const char* macroFile,G4int n_select)
{
  InitializeEventLoop(n_event,macroFile,n_select);

// Event loop
  for(G4int i_event=0; i_event<n_event; i_event++ )
  {

    if ( i_event % 1000 == 0)
    { 
      printf (" === Processed %i events \n", i_event);
    }

    if(fEvtMessenger->SaveOnlySelectedEvents())
    {
      bool requirement = false;
      while (!requirement)
      {
        ProcessOneEvent(i_event);
        if(fEvtMessenger->Save2gEvents())
        {
          requirement = EventAction::Is2gRegistered();
        } else if (fEvtMessenger->Save3gEvents())
        {
          requirement = EventAction::Is3gRegistered();
        } else {
          G4Exception ("RunManager","RM01", FatalException, 
             " This can not happened ");    
        }
      }
    } else {
      ProcessOneEvent(i_event);
    }
    TerminateOneEvent(); //updating counters
    if(runAborted) break;
  }

  // For G4MTRunManager, TerminateEventLoop() is invoked after all threads are finished.
  if(runManagerType==sequentialRM) TerminateEventLoop();

}



