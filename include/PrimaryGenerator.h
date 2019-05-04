#ifndef PrimaryGenerator_h 
#define PrimaryGenerator_h 1

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4VPrimaryGenerator.hh"
#include "MaterialExtension.h"
#include<TGenPhaseSpace.h> 
#include<TLorentzVector.h>
#include "BeamParams.h"
#include "SourceParams.h"
#include"G4Navigator.hh"
#include "G4TransportationManager.hh" 


class PrimaryGenerator : public G4VPrimaryGenerator 
{
    public:
        PrimaryGenerator();
        ~PrimaryGenerator();
        void GenerateBeam(BeamParams*, G4Event* );
        void GenerateIsotope(SourceParams*, G4Event* );
        void GenerateNema(G4int, G4Event* );
        void GenerateEvtSmallChamber(G4Event* event, const G4double);
        void GenerateEvtLargeChamber(G4Event* event);

        virtual void GeneratePrimaryVertex(G4Event*){};

     private:

         std::tuple<G4ThreeVector,G4double,G4double> GetVerticesDistribution(G4double,G4double,G4double);  ///< return: vtx position, 2/3g ratio, meanlifetime; as input the maximal dimension(/2) of annihilation chamber are taken (to speed up simulatons)

        std::tuple<G4ThreeVector,MaterialExtension*> GetVerticesDistributionInFilledSphere(const G4ThreeVector center, G4double radius);

        std::tuple<G4ThreeVector,MaterialExtension*> GetVerticesDistributionAlongStepVector(const G4ThreeVector center, const G4ThreeVector direction);

        G4PrimaryVertex* GenerateTwoGammaVertex(const G4ThreeVector vtxPosition, const G4double T0,  const G4double lifetime2g);
        G4PrimaryVertex* GenerateThreeGammaVertex(const G4ThreeVector vtxPosition, const G4double T0,  const G4double lifetime3g);
        G4PrimaryVertex* GeneratePromptGammaVertex(const G4ThreeVector vtxPosition, const G4double T0,  const G4double lifetimePrompt, const G4double energy);

        G4ThreeVector VertexUniformInCylinder(G4double, G4double);

        G4double calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3);
       
        const G4ThreeVector  GetRandomPointInFilledSphere(G4double radius);
        const G4ThreeVector  GetRandomPointOnSphere(G4double radius);

        G4Navigator* theNavigator =  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
};


#endif 
