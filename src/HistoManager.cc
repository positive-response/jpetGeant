#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <vector>


ClassImp(Event)
ClassImp(VtxBlock)
ClassImp(ParticleBlock)

HistoManager::HistoManager()
{}

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

    fTree = new TTree("TreeMC", "Tree keeps output from Geant simulation");
    fTree->SetAutoSave(1000000000); // autosave when 1 Gbyte written

     fEvent = new  Event();

    fBranch = fTree->Branch("event", &fEvent);
//    fBranch = fTree->Branch("event", &fEvent,64000,0);
//    fBranch->SetAutoDelete(kFALSE);
//

//    fNtuple1->Branch("Eabs", &fEabs, "Eabs/D");
//    fNtuple1->Branch("Egap", &fEgap, "Egap/D");
//
//     fNtuple2 = new TTree("Ntuple2", "TrackL");  
//     fNtuple2->Branch("Labs", &fLabs, "Labs/D");


}

void HistoManager::FillTree()
{
    fTree->Fill();
}

void HistoManager::Save()
{
    if (! fRootFile) return;
    fRootFile->Write(); 
    fRootFile->Close(); 

    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

