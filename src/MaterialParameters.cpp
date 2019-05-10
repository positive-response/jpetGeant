#include "MaterialParameters.h"


const G4double MaterialParameters::oPsTauVaccum = 142*ns;
const G4double MaterialParameters::oPsTauAl = 0.32*ns;  // data for AL  https://doi.org/10.1007/s00339-015-9214-0
const G4double MaterialParameters::oPsTauXAD = 2.45*ns;

const G4double MaterialParameters::oPsProbabilityAl = 0.12;
const G4double MaterialParameters::oPsProbabilityXAD = 89.6; // 4/3 I; taken only most intense component


const G4double MaterialParameters::fractionDirect3g = 0.002688;
const G4double MaterialParameters::tauKapton = 0.380; // doi 10.2478/umcschem-2013-0012 
const G4double MaterialParameters::tauBulk = 0.2*ns;

const G4double MaterialParameters::sodiumGammaEnergy = 1.2770*MeV;
const G4double MaterialParameters::sodiumGammaTau = 3.7*ps;

G4double MaterialParameters::foPsTauXAD = oPsTauXAD;
G4double MaterialParameters::foPsFractionXAD = GetoPsFractionPALS(oPsProbabilityXAD,oPsTauXAD);
G4double MaterialParameters::fpickOffFractionXAD = 10*foPsFractionXAD;



void MaterialParameters::SetXADoPsLifetime(G4double x) {   foPsTauXAD = x;   }
void MaterialParameters::SetXADoPsFraction(G4double x) { foPsFractionXAD = x; }
void MaterialParameters::SetXADPickOffFraction(G4double x) { fpickOffFractionXAD =x;}

G4double MaterialParameters::GetXADoPsTau(){return foPsTauXAD;}
G4double MaterialParameters::GetXADoPsFraction(){ return foPsFractionXAD;}
G4double MaterialParameters::GetXADPickOffFraction(){return fpickOffFractionXAD;} 

void MaterialParameters::SetXADoPsOnly()
{
  foPsFractionXAD = 1.0f;
  fpickOffFractionXAD = 0.0f;
}

void MaterialParameters::SetXAD2gOnly()
{
  foPsFractionXAD = 0.0f;
  fpickOffFractionXAD = 0.0f;
}

void MaterialParameters::SetXADPickOffOnly()
{
  foPsFractionXAD = 0.0f;
  fpickOffFractionXAD = 1.0f;
}


G4double MaterialParameters::GetoPsFractionPALS(G4double oPsPobability, G4double oPslifetime)
{
  /// f3g = (1-4/3 Pi)/372 + tau_i/tau_vac \times Pi 
  //  Pi - propability of Ps formation
  //  f^{oPs}_{3g} = tau_i/tau_vac  
  //  foPsProbability = 4/3 I_3 measured in PALS
  //  3g = direct + oPs

  //return (1.-4/3*oPsPobability)/372 + (oPslifetime/oPsTauVaccum)*oPsPobability;
  return (oPslifetime/oPsTauVaccum)*oPsPobability/100.;
}

