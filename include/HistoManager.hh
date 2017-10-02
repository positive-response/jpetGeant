#ifndef HistoManager_h 
#define HistoManager_h 1 

#include "globals.hh"
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>

class TFile;
class TTree;
const G4int MaxHisto = 4;

class HistoManager
{
    public:
        HistoManager();
        ~HistoManager();

        void Book();
        void Save();

    private:
        TFile*   fRootFile;
        TH1D*    fHisto[MaxHisto];
        TTree*   fTree; 
        TBranch* fBranch;

};

#endif
