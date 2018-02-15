#ifndef PrimaryGenerator_h 
#define PrimaryGenerator_h 1

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4VPrimaryGenerator.hh"
#include "MaterialExtension.hh"
#include<TGenPhaseSpace.h> 
#include<TLorentzVector.h>
#include "PrimaryGeneratorMessenger.hh"


struct BeamParams
{
    G4double energy;
    G4ThreeVector position;
    G4ThreeVector momentum;
};

struct TargetParams
{
    G4String shape;
};


class PrimaryGenerator : public G4VPrimaryGenerator 
{
    public:
        PrimaryGenerator();
        ~PrimaryGenerator();

        G4String GetSetupInfo(){return fGenerateSourceType;};
        void SetSourceTypeInfo(G4String);
    public:
         virtual void GeneratePrimaryVertex(G4Event*);

     private:
        PrimaryGeneratorMessenger* fMessenger; 


        G4String fGenerateSourceType;///< "run", "beam", "target" 
        G4String fAllowedSourceTypes[3] = {"run", "beam", "target"};



         std::tuple<G4ThreeVector,int> GetVerticesDistribution(); 
        void GenerateTwoGammaVertex(G4PrimaryVertex*);
        void GenerateThreeGammaVertex(G4PrimaryVertex*);
        void GeneratePromptGammaSodium(G4PrimaryVertex*);

        G4ThreeVector VertexUniformInCylinder(G4double, G4double);

        Double_t calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3);
       
};


#endif 
