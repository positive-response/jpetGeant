#ifndef PrimaryGeneratorAction_h 
#define PrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleDefinition;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();
        virtual void GeneratePrimaries(G4Event*);
    private:
        G4ParticleGun* fParticleGun;

        void GenerateTwoGamma();
        void GenerateThreeGamma();

};
#endif
