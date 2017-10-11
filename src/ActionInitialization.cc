#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization()
     : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    HistoManager*  histo = new HistoManager();
    SetUserAction(new RunAction(histo));

}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction);
    HistoManager*  histo = new HistoManager();
    SetUserAction(new RunAction(histo));
    SetUserAction(new EventAction(histo));

    SetUserAction(new TrackingAction);
}

