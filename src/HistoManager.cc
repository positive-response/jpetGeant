#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <vector>



HistoManager::HistoManager()
{
    fEventPack = new JPetGeantEventPack();
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
    G4String fileName = "mcGeant.root";
    fRootFile = new TFile(fileName,"RECREATE");  
    if (! fRootFile) { 
        G4cout << " HistoManager::Book :"                    
            << " problem creating the ROOT TFile "  
            << G4endl;   
        return; 
    }

    Int_t bufsize=32000;
    Int_t splitlevel=2;


    fTree = new TTree("T", "Tree keeps output from Geant simulation",splitlevel);
    fTree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
    fBranchEventPack = fTree->Branch("eventPack", &fEventPack, bufsize, splitlevel);

}

//void HistoManager::AddGenInfo(EventInformation* info)
//{
//    JPetGeantEventInformation* geantInfo =  fEventPack->GetEventInformation();
//    geantInfo->SetThreeGammaGen(info->GetThreeGammaGen());
//    geantInfo->SetTwoGammaGen(info->GetTwoGammaGen());
//    geantInfo->SetPromptGammaGen(info->GetPromptGammaGen());
//
//    geantInfo->SetRunNr(info->GetRunNr());
//    geantInfo->SetVtxPosition(info->GetVtxPositionX(),info->GetVtxPositionY(),info->GetVtxPositionZ());
//
//    geantInfo->SetLifetime(info->GetLifetime());
//}

void HistoManager::AddNewHit(DetectorHit* hit)
{

  JPetGeantScinHits* geantHit =  fEventPack->ConstructNextHit();

   geantHit->Fill(
           fEventPack->GetEventNumber(),    //int evID, 
           hit->GetScinID(),    //int scinID, 
           hit->GetTrackID(),    //int trkID, 
           hit->GetTrackPDG(),    //int trkPDG, 
           hit->GetNumInteractions(),    //int nInter,
           hit->GetEdep(),    //float ene, 
           hit->GetTime()    //float time, 
               );

         // ugly way but there is no easy way to cast g4vector into root vector
    geantHit->SetHitPosition(
             hit->GetPosition().getX(),
             hit->GetPosition().getY(),
             hit->GetPosition().getZ());

    geantHit->SetPolarizationIn(
             hit->GetPolarizationIn().getX(),
             hit->GetPolarizationIn().getY(),
             hit->GetPolarizationIn().getZ());

    geantHit->SetPolarizationOut(
             hit->GetPolarizationOut().getX(),
             hit->GetPolarizationOut().getY(),
             hit->GetPolarizationOut().getZ());

    geantHit->SetMomentumIn(
             hit->GetMomentumIn().getX(),
             hit->GetMomentumIn().getY(),
             hit->GetMomentumIn().getZ());

    geantHit->SetMomentumOut(
             hit->GetMomentumOut().getX(),
             hit->GetMomentumOut().getY(),
             hit->GetMomentumOut().getZ());

}




void HistoManager::Save()
{
    if (! fRootFile) return;
     //fRootFile->Write(); 
     fTree->Write();
     fRootFile->Close(); 

    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

