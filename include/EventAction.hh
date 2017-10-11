#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh" 
#include "SavedEvent.hh"
#include "HistoManager.hh"
#include <memory>


class EventAction : public G4UserEventAction
{
    public:
        EventAction(HistoManager* histo); 
        virtual ~EventAction();
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event* anEvent);

    private:
        std::unique_ptr<SavedEvent> fEvent {new SavedEvent()};
        HistoManager* fHisto;
};

#endif
