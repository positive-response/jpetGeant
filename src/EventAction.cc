#include "EventAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"

EventAction::EventAction(HistoManager* histo )
{

    fHisto = histo;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEvent->Clean();
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
     const G4int primNum  = anEvent->GetNumberOfPrimaryVertex();
//     G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);

     G4double x = anEvent->GetPrimaryVertex(0)->GetX0();
     fHisto->GetEvent()->GetVtxBlock()->SetMotherId(4);
     fHisto->GetEvent()->GetVtxBlock()->SetTime(30.2);


     fHisto->FillTree();

}
