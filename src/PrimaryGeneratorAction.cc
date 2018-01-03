#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh" 

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() 
:G4VUserPrimaryGeneratorAction()
{

}

PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) 
{

    GenerateTwoGamma(event);
}


void PrimaryGeneratorAction::GenerateTwoGamma(G4Event* event)
{
    // NO boost of progenies !
    
    G4double mass = 1022*keV;
    std::vector<G4double> mass_secondaries = {0., 0.};
       
    std::vector<G4LorentzVector> out;

    G4HadPhaseSpaceGenbod* phS = new G4HadPhaseSpaceGenbod();
    phS->Generate(mass,mass_secondaries,out);

    G4ParticleGun* fParticleGun  = new G4ParticleGun(1);
    G4ParticleGun* fParticleGun2  = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle= particleTable->FindParticle(particleName="gamma");

    G4double energy = 511*keV;
    G4ThreeVector position = G4ThreeVector(0.,0.,0.);
    G4ThreeVector momentumDirection = G4ThreeVector(out[0].px(),out[0].py(),out[0].pz());
    G4ThreeVector momentumDirection2 = G4ThreeVector(out[1].px(),out[1].py(),out[1].pz());


    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(momentumDirection);
    fParticleGun->GeneratePrimaryVertex(event);


    fParticleGun2->SetParticleDefinition(particle);
    fParticleGun2->SetParticleEnergy(energy);
    fParticleGun2->SetParticlePosition(position);
    fParticleGun2->SetParticleMomentumDirection(momentumDirection2);
    fParticleGun2->GeneratePrimaryVertex(event);

    delete fParticleGun;
    delete fParticleGun2; 
}

void PrimaryGeneratorAction::GenerateThreeGamma()
{
}
