#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGenerator.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGenerator* primGenerator)
    :fPrimGen(primGenerator)
{

    fDirectory = new G4UIdirectory("/jpetmc/source/"); 
    fDirectory->SetGuidance("Commands for controling the gamma quanta source (beam/target) and its parameters");

    fSourceType = new G4UIcmdWithAString("/jpetmc/source/setType",this);
    fSourceType->SetCandidates("beam target");
    fSourceType->SetDefaultValue("beam"); 

    fGammaBeamSetEnergy = new G4UIcmdWithADoubleAndUnit("/jpetmc/source/gammaBeam/setEnergy",this);
    fGammaBeamSetEnergy->SetGuidance("Set energy (value and unit) for beam of gamma quanta");
    fGammaBeamSetEnergy->SetDefaultValue(511);
    fGammaBeamSetEnergy->SetDefaultUnit("keV");
    fGammaBeamSetEnergy->SetUnitCandidates("eV keV MeV");


    fGammaBeamSetPosition = new G4UIcmdWith3VectorAndUnit("/jpetmc/source/gammaBeam/setPosition",this);
    fGammaBeamSetPosition->SetGuidance("Set vertex position of the gamma quanta beam");
    fGammaBeamSetPosition->SetDefaultValue(G4ThreeVector(0,0,0));
    fGammaBeamSetPosition->SetDefaultUnit("cm");
    fGammaBeamSetPosition->SetUnitCandidates("mm cm");
    fGammaBeamSetPosition->SetParameterName("Xvalue","Yvalue","Zvalue",false);

    fGammaBeamSetMomentum = new G4UIcmdWith3VectorAndUnit("/jpetmc/source/gammaBeam/setMomentum",this);
    fGammaBeamSetMomentum->SetGuidance("Set momentum of the gamma quanta beam");
    fGammaBeamSetMomentum->SetDefaultValue(G4ThreeVector(1,0,0));
    fGammaBeamSetMomentum->SetDefaultUnit("keV");
    fGammaBeamSetMomentum->SetUnitCandidates("eV keV MeV");
    fGammaBeamSetMomentum->SetParameterName("Xvalue","Yvalue","Zvalue",false);


}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete fSourceType;
    delete fGammaBeamSetEnergy;
    delete fGammaBeamSetPosition;
    delete fGammaBeamSetMomentum;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command==fSourceType){
  //      fPrimGen->SetSourceType(newValue);
    }


    if(command==fGammaBeamSetEnergy){
        // check if we really changing parameters corresponding to the beam
//        if(fPrimGen->GetSetupInfo() == "beam")
//        {
//
//        }
    }

    if(command==fGammaBeamSetPosition){
    }

    if(command==fGammaBeamSetMomentum){
    }
}

