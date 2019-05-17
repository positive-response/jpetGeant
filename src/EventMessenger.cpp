#include "EventMessenger.h"
#include "RunManager.h"

EventMessenger*  EventMessenger::fInstance = nullptr;

EventMessenger* EventMessenger::GetEventMessenger()
{
  if ( fInstance == nullptr ){
   fInstance = new EventMessenger();
  }

  return fInstance;
}


EventMessenger::EventMessenger()
{

  fDirectory = new G4UIdirectory("/jpetmc/event/"); 
  fDirectory->SetGuidance("Define events to save");

  fSave2g = new G4UIcmdWithoutParameter("/jpetmc/event/save2g", this);
  fSave2g->SetGuidance("Events with registered 2g will be saved");
  fSave3g = new G4UIcmdWithoutParameter("/jpetmc/event/save3g",this);
  fSave3g->SetGuidance("Events with registered 3g will be saved");

}

EventMessenger::~EventMessenger()
{
  delete fSave2g;
  delete fSave3g;
}


void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command==fSave2g)
  {
    fSave2gEvts = true;
  }

  if(command==fSave3g)
  {
    fSave3gEvts = true;
  }
 
  
}

