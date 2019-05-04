#ifndef MATERIALPARAMETERS_H
#define MATERIALPARAMETERS_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"

namespace MaterialParameters
{
  // Tau - greek letter; here use for denoted the mean lifetime
  const G4double oPsTauVaccum = 142*ns;
  const G4double oPsTauAl = 0.32*ns;  // data for AL  https://doi.org/10.1007/s00339-015-9214-0
  const G4double oPsTauXAD = 2.45*ns;

  // oPs probability formation in different materials
  const G4double oPsProbabilityAl = 0.12;
  const G4double oPsProbabilityXAD = 89.6; // 4/3 I; taken only most intense component


  const G4double fractionDirect3g = 0.002688;
  const G4double tauKapton = 0.380; // doi 10.2478/umcschem-2013-0012 
  const G4double tauBulk = 0.2*ns;

  // sodium parameters
  const G4double sodiumGammaEnergy = 1.2770*MeV;
  const G4double sodiumGammaTau = 3.7*ps;

  G4double GetoPsFractionPALS(G4double, G4double);
  void SetEventsTypesToSimulate(bool b_2g, bool b_3g, bool b_pickOff);
  

  static bool useParamsProvidedByUser = false;
  void SetXADoPsLifetime(G4double);
  void SetXADoPsFraction(G4double);
  void SetXADPickOffFraction(G4double);

  G4double GetXADoPsTau();
  G4double GetXADoPsFraction();
  G4double GetXADPickOffFraction(); 

  void SetXADoPsOnly();
  void SetXAD2gOnly();
  void SetXADPickOffOnly();
}

#endif
