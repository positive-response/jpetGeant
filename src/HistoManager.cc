#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <vector>


ClassImp(DecayTree)
ClassImp(ScinBlock)
ClassImp(EvtInfo)


HistoManager::HistoManager()
{
    fDecayTree = new DecayTree();
    fScin = new ScinBlock();
    fEvtInfo = new EvtInfo();
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

    Int_t bufsize=32000;
    Int_t splitlevel=2;

    //HISTO
    //fHisto[0] = new TH1D("EAbs", "Edep in absorber (MeV)", 100, 0., 800*CLHEP::MeV);  

//    fTree = new TTree("TTrk", "Tree keeps output from Geant simulation",splitlevel);
//    fTree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
//    fBranchTrk = fTree->Branch("decayTree", &fDecayTree, bufsize, splitlevel);
//
//    fTree2 = new TTree("TScin", "Tree keeps output from Geant simulation");
//    fTree2->SetAutoSave(1000000000); // autosave when 1 Gbyte written
//    fBranchScin = fTree2->Branch("scin", &fScin, bufsize, splitlevel);

    fTree3 = new TTree("EvtInfo", "Tree keeps output from Geant simulation");
    fTree3->SetAutoSave(1000000000); // autosave when 1 Gbyte written
    fBranchEventInfo = fTree3->Branch("evtInfo", &fEvtInfo, bufsize, splitlevel);


}

void HistoManager::FillTrk()
{
  //  fTree->Fill();
}

void HistoManager::FillScin()
{
  //  fTree2->Fill();
}

void HistoManager::FillEvtInfo()
{
    fTree3->Fill();
}


void HistoManager::Save()
{
    if (! fRootFile) return;
     //fRootFile->Write(); 
     fTree3->Write();
    fRootFile->Close(); 

    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

