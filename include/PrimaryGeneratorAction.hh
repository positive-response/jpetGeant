#ifndef PrimaryGeneratorAction_h 
#define PrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4HadPhaseSpaceGenbod.hh"

class G4ParticleDefinition;
class G4ParticleGun;
class G4Event;

/**
 * \class PrimaryGeneratorAction
 * \brief heart of simulated physics
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();
        virtual void GeneratePrimaries(G4Event*);
    private:

        void GenerateTwoGamma(G4Event* event);
        void GenerateThreeGamma();

};
#endif
