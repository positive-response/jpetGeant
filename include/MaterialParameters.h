#ifndef MATERIALPARAMETERS_H
#define MATERIALPARAMETERS_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"

class MaterialParameters
{
  public:
    // Tau - greek letter; here use for denoted the mean lifetime
    static const G4double oPsTauVaccum;
    static const G4double oPsTauAl; 
    static const G4double oPsTauXAD;
    static const G4double oPsProbabilityAl;
    static const G4double oPsProbabilityXAD; // 4/3 I; taken only most intense component
    static const G4double fractionDirect3g;
    static const G4double tauKapton; 
    static const G4double tauBulk;
    // sodium parameters
    static const G4double sodiumGammaEnergy;
    static const G4double sodiumGammaTau;

    static G4double GetoPsFractionPALS(G4double, G4double);
    static void SetEventsTypesToSimulate(bool b_2g, bool b_3g, bool b_pickOff);


    static void SetXADoPsLifetime(G4double);
    static void SetXADoPsFraction(G4double);
    static void SetXADPickOffFraction(G4double);

    static G4double GetXADoPsTau();
    static G4double GetXADoPsFraction();
    static G4double GetXADPickOffFraction(); 

    static void SetXADoPsOnly();
    static void SetXAD2gOnly();
    static void SetXADPickOffOnly();

  private: 
    static G4double foPsTauXAD;
    static G4double foPsFractionXAD;
    static G4double fpickOffFractionXAD;

};

#endif
