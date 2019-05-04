#include "MaterialParameters.h"

namespace MaterialParameters
{

  namespace 
  {
    static G4double foPsTauXAD = oPsTauXAD;
    static G4double foPsFractionXAD = GetoPsFractionPALS(oPsProbabilityXAD,oPsTauXAD);
    static G4double fpickOffFractionXAD = 10*foPsFractionXAD;
  }

  void SetXADoPsLifetime(G4double x) {   foPsTauXAD = x;   }
  void SetXADoPsFraction(G4double x) { foPsFractionXAD = x; }
  void SetXADPickOffFraction(G4double x) { fpickOffFractionXAD =x;}

  G4double GetXADoPsTau(){return foPsTauXAD;}
  G4double GetXADoPsFraction(){ return foPsFractionXAD;}
  G4double GetXADPickOffFraction(){return fpickOffFractionXAD;} 
  
  void SetXADoPsOnly()
  {
    foPsFractionXAD = 1.0f;
    fpickOffFractionXAD = 0.0f;
  }

  void SetXAD2gOnly()
  {
    foPsFractionXAD = 0.0f;
    fpickOffFractionXAD = 0.0f;
  }

  void SetXADPickOffOnly()
  {
    foPsFractionXAD = 0.0f;
    fpickOffFractionXAD = 1.0f;
  }


  G4double GetoPsFractionPALS(G4double oPsPobability, G4double oPslifetime)
  {
    /// f3g = (1-4/3 Pi)/372 + tau_i/tau_vac \times Pi 
    //  Pi - propability of Ps formation
    //  f^{oPs}_{3g} = tau_i/tau_vac  
    //  foPsProbability = 4/3 I_3 measured in PALS
    //  3g = direct + oPs

    //return (1.-4/3*oPsPobability)/372 + (oPslifetime/oPsTauVaccum)*oPsPobability;
    return (oPslifetime/oPsTauVaccum)*oPsPobability/100.;

  }

}
