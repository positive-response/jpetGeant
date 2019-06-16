#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh" 
#include "HistoManager.h"
#include "JPetGeantDecayTree.h"
#include "JPetGeantScinHits.h"
#include <memory>
#include "EventMessenger.h"

/**
 * \class EventAction
 * \brief class managing events 
 *
 * Here also one can reach for information
 * after propagation of all tracks in detector.
 */
class EventAction : public G4UserEventAction
{
    public:
        EventAction(HistoManager* histo); 
        virtual ~EventAction();
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event* anEvent);

        static bool Is2gRegistered();
        static bool Is3gRegistered();

    private:


        HistoManager* fHisto = nullptr;
        G4int fScinCollID;
        EventMessenger* fEvtMessenger = EventMessenger::GetEventMessenger();

        void CheckIf2gIsRegistered(const G4Event* anEvent);
        void CheckIf3gIsRegistered(const G4Event* anEvent);
        void WriteToFile(const G4Event* anEvent);
        static bool is2gRec;
        static bool is3gRec;
};

#endif
