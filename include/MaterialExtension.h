#ifndef MaterialExtension_h
#define MaterialExtension_h


#include "G4VMaterialExtension.hh"
#include "G4Material.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "MaterialExtensionMessenger.h"

/**
 * \class MaterialExtension
 * \brief extended material keeps information about 2g/3g creation fraction
 */


class MaterialExtension : public G4Material
{
    public:
      MaterialExtension(const G4String& name, const G4Material* baseMaterial);

      ~MaterialExtension();
      G4Material* GetMaterial() {return fMaterial;};

      void SetoPsLifetime(G4double);
      void SetoPsFraction(G4double);
      void SetPickOffFraction(G4double sf){fPickOffFraction=sf;}

      G4double Get3gFraction(); 
      G4double GetoPsLifetime(){return foPslifetime;};
      std::vector<G4double> GetEventsFraction(); //< 2g direct // 2g pickoff (lifetime 3g) // 3g direct // 3g oPs (lifetime 3g)


      G4bool IsTarget() {return fTarget;};
      void AllowsAnnihilations(G4bool tf){ fTarget = tf;};

      void Print() const {;};
      G4bool IsExtended() const { return true; }

    private:
      G4Material* fMaterial;
      MaterialExtensionMessenger* fMaterialExtensionMessenger =  MaterialExtensionMessenger::GetMaterialExtensionMessenger();
      G4bool fTarget;

      G4double foPsFraction; ///<  3g/2g events fraction 
      G4double foPslifetime;
      G4double fPickOffFraction;

};

#endif
