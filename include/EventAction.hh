#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh" 
#include "HistoManager.hh"
#include "TrkBlock.hh"
#include "ScinBlock.hh"
#include <memory>

/**
 * \class EventAction
 * \brief class managing events 
 *
 * Here also one can reach for information
 * after propagation of all tracks in detector.
 * One ned to be shue to store all trajectories while propagating (hidden in standard usage)
 */
class EventAction : public G4UserEventAction
{
    public:
        EventAction(HistoManager* histo); 
        virtual ~EventAction();
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event* anEvent);

    private:
        //std::unique_ptr<SavedEvent> fEvent {new SavedEvent()};
        HistoManager* fHisto;
        G4int fScinCollID;
};

#endif
