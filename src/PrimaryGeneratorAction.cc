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
    // if setup for dedicated run is set then ignore its modifications made by user 
    if( DetectorConstruction::GetInstance()->GetRunNumber() != 0){
        if( GetSourceTypeInfo() != "run"){
          G4Exception("PrimaryGeneratorAction","PG03",JustWarning,
                 "User can not modify the predefined run geometry");
        }
        SetSourceTypeInfo("run");
    }


    if( GetSourceTypeInfo() == ("run")) {
    } else if (GetSourceTypeInfo() == ("beam")) {
    } else if (GetSourceTypeInfo() == ("isotope")) {
    } else {
          G4Exception("PrimaryGeneratorAction","PG04",FatalException,
                 "Required source type is not allowed");
    }


    fPrimaryGenerator->GeneratePrimaryVertex(event);


}

void PrimaryGeneratorAction::SetSourceTypeInfo(G4String newSourceType)
{
    if (std::find(std::begin(fAllowedSourceTypes), std::end(fAllowedSourceTypes), newSourceType) != std::end(fAllowedSourceTypes))
    {
        // setup found
        if( DetectorConstruction::GetInstance()->GetRunNumber() == 0){ 
            fGenerateSourceType = newSourceType;
        } else {
            G4Exception("PrimaryGeneratorAction","PG01",JustWarning,
                    "Chosen detector geometry corresponds to run number and it can not be changed");
        }
    } else {
        G4Exception("PrimaryGeneratorAction","PG02",JustWarning,
                "Please pick from avaliable setups: beam/isotope");
    }

}



