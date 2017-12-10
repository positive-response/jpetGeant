#ifndef HistoManager_h 
#define HistoManager_h 1 

#include "globals.hh"
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include "TrkBlock.hh"
#include "ScinBlock.hh"

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
        void FillTrk(); //< translate TrkBlock.cc into tree
        void FillScin(); //< translate ScinBlock.cc into tree
        
       TrkBlock* GetTrkBlock() const {return fTrk;}
       ScinBlock* GetScinBlock() const {return fScin;}


    private:
        TFile*   fRootFile;
        TH1D*    fHisto[MaxHisto];
        TTree*   fTree; 
        TTree*   fTree2; 
        TBranch* fBranchTrk;
        TBranch* fBranchScin;
        TrkBlock* fTrk;
        ScinBlock* fScin;
//        std::unique_ptr<Event> fEvent {new Event()};

};

#endif
