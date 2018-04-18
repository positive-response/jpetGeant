#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh" 
#include "G4PrimaryVertex.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(HistoManager* histo) 
    :G4VUserPrimaryGeneratorAction(),
    fPrimaryGenerator(0), fHisto(histo)
{
    fPrimaryGenerator = new PrimaryGenerator();


    fMessenger = new PrimaryGeneratorActionMessenger(this);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{
    delete fPrimaryGenerator;
    delete fMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) 
{
    // if setup for dedicated run is set then ignore its modifications 
        if( DetectorConstruction::GetInstance()->GetRunNumber() != 0){
    //        SetSourceTypeInfo("beam");
        } else {
            SetSourceTypeInfo("run");
        }



    fPrimaryGenerator->GeneratePrimaryVertex(event);

    //    if( fGenerateSourceType == "run") // predefined; can not be modified by user 
    //    {
    //    } else if(fGenerateSourceType == "beam"){
    //    } else if(fGenerateSourceType == "target"){
    //         G4Exception("PrimaryGenerator","PG03",FatalException,
    //                "Option not implemented");
    //    } else {
    //         G4Exception("PrimaryGenerator","PG04",FatalException,
    //                "Required source type is not allowed");
    //    }
    //





    //   G4int numPrimVtx = event->GetNumberOfPrimaryVertex(); 
    //   for (int i=0; i<numPrimVtx; i++)
    //    {
    int i=0;    
    G4PrimaryVertex* vtx = event->GetPrimaryVertex(i);
    //        fHisto->GetEvtInfo()->FillEvtInfo( event->GetEventID(),
    //            vtx->GetX0(), vtx->GetY0(), vtx->GetZ0(), vtx->GetT0());    
    //        fHisto->FillEvtInfo();


    //    }
}

void PrimaryGeneratorAction::SetSourceTypeInfo(G4String newSetup)
{
    if (std::find(std::begin(fAllowedSourceTypes), std::end(fAllowedSourceTypes), newSetup) != std::end(fAllowedSourceTypes))
    {
        // setup found
        if( DetectorConstruction::GetInstance()->GetRunNumber() == 0){ 
            fGenerateSourceType = newSetup;
        } else {
            G4Exception("PrimaryGenerator","PG01",JustWarning,
                    "Chosen detector geometry corresponds to run number and it can not be changed");
        }
    } else {
        G4Exception("PrimaryGenerator","PG02",JustWarning,
                "Please pick from avaliable setups: beam/target");
    }

}



