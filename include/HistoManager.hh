#ifndef HistoManager_h 
#define HistoManager_h 1 

#include "globals.hh"
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include "DecayTree.hh"
#include "ScinBlock.hh"
#include "EvtInfo.hh"

class TFile;
class TTree;
const G4int MaxHisto = 4;

/**
 * \class HistoManager
 * \brief class reach for informations stored during sumulations
 * and saves them into ROOT tree structures
 */
class HistoManager
{
    public:
        HistoManager();
        ~HistoManager();

        void Book(); //< call once; book all trees and histograms
        void Save(); //< call once; save all trees and histograms
        void FillTrk(); //< translate DecayTree.cc into tree
        void FillScin(); //< translate ScinBlock.cc into tree
        void FillEvtInfo(); 
        
       DecayTree* GetDecayTree() const {return fDecayTree;}
       ScinBlock* GetScinBlock() const {return fScin;}
       EvtInfo*   GetEvtInfo() const {return fEvtInfo;}


    private:
        TFile*   fRootFile;
        TH1D*    fHisto[MaxHisto];
        TTree*   fTree; 
        TTree*   fTree2; 
        TTree*   fTree3; 
        TBranch* fBranchTrk;
        TBranch* fBranchScin;
        TBranch* fBranchEventInfo;

        DecayTree* fDecayTree;
        ScinBlock* fScin;
        EvtInfo*   fEvtInfo;
//        std::unique_ptr<Event> fEvent {new Event()};

};

#endif
