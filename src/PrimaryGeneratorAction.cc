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
    G4int nofParticles = 1;
    //G4double *energy = new G4double[nofParticles] ;
    G4double energy = 511*keV;
    G4ThreeVector position = G4ThreeVector(.0,.0,.0);
    G4ThreeVector momentumDirection = G4ThreeVector(1.,.0,.0);


    fParticleGun  = new G4ParticleGun(nofParticles);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;

    G4ParticleDefinition* particle= particleTable->FindParticle(particleName="gamma");
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(momentumDirection);


}

PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) 
{

    fParticleGun->GeneratePrimaryVertex(event);

}


void PrimaryGeneratorAction::GenerateTwoGamma()
{
}

void PrimaryGeneratorAction::GenerateThreeGamma()
{
}
