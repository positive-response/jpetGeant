#ifndef MaterialExtension_h
#define MaterialExtension_h


#include "G4VMaterialExtension.hh"
#include "G4Material.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

/**
 * \class MaterialExtension
 * \brief extended material keeps information about 2g/3g creation fraction
 */

const G4double foPsVaccum = 142*ns;

//class MaterialExtension : public G4VMaterialExtension
class MaterialExtension : public G4Material
{
    public:
      MaterialExtension(const G4String& name, const G4Material* baseMaterial);

      ~MaterialExtension();
      G4Material* GetMaterial() {return fMaterial;};

      void SetoPsLifetime(G4double);
      void Set3gProbability(G4double);

      G4double Get3gFraction(); 


      G4bool IsTarget() {return fTarget;};
      void AllowsAnnihilations(G4bool tf){ fTarget = tf;};

      void Print() const {;};
      G4bool IsExtended() const { return true; }

    private:
      G4Material* fMaterial;
      G4bool fTarget;

      G4double f3gFraction; ///<  3g/2g events fraction 
      G4double foPsPobability; 
      G4double foPslifetime;

};

#endif
