#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <vector>


ClassImp(TrkBlock)
ClassImp(ScinBlock)

HistoManager::HistoManager()
{
    fTrk = new TrkBlock();
    fScin = new ScinBlock();
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
    G4String fileName = "ana.root";
    fRootFile = new TFile(fileName,"RECREATE");  
    if (! fRootFile) { 
        G4cout << " HistoManager::Book :"                    
            << " problem creating the ROOT TFile "  
            << G4endl;   
        return; 
    }

    //HISTO
    fHisto[0] = new TH1D("EAbs", "Edep in absorber (MeV)", 100, 0., 800*CLHEP::MeV);  

    fTree = new TTree("TTrk", "Tree keeps output from Geant simulation");
    fTree->SetAutoSave(1000000000); // autosave when 1 Gbyte written


    fBranchTrk = fTree->Branch("trk", &fTrk);

    fTree2 = new TTree("TScin", "Tree keeps output from Geant simulation");
    fTree2->SetAutoSave(1000000000); // autosave when 1 Gbyte written
    fBranchScin = fTree2->Branch("scin", &fScin);


}

void HistoManager::FillTrk()
{
    fTree->Fill();
}

void HistoManager::FillScin()
{
    fTree2->Fill();
}

void HistoManager::Save()
{
    if (! fRootFile) return;
    fRootFile->Write(); 
    fRootFile->Close(); 

    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

