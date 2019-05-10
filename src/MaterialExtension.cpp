#include "MaterialExtension.h"
#include "MaterialParameters.h"


MaterialExtension::MaterialExtension(const G4String& name, const G4Material* baseMaterial)
: G4Material(name,baseMaterial->GetDensity(),baseMaterial,    
   baseMaterial->GetState(),baseMaterial->GetTemperature(),
   baseMaterial->GetPressure())
{
    foPslifetime = 0.;
    foPsFraction = 0.;
    fTarget = false ;
    fPickOffFraction = 0.;
}



MaterialExtension::~MaterialExtension()
{}


G4double MaterialExtension::Get3gFraction()
{
   return foPsFraction;
}

void MaterialExtension::SetoPsFraction(G4double x)
{
    if ( x <0 || x > 1)
    {
        G4Exception ("MaterialExtension", "ME01",JustWarning,
                "3gamma fraction can not extend 1; nothing happend");

    } else {
        foPsFraction = x;
    };
}


void MaterialExtension::SetoPsLifetime(G4double x)
{
    if ( x <0 || x > MaterialParameters::oPsTauVaccum )
    {
        G4Exception ("MaterialExtension", "ME02",JustWarning,
                "oPs lifetime can not excided 142*ns");

    } else {
        foPslifetime = x;
    };

}


std::vector<G4double> MaterialExtension::GetEventsFraction()
{
  G4double direct3g = MaterialParameters::fractionDirect3g;
  if( foPsFraction==1) direct3g = 0.0f;
  if( fPickOffFraction==1) direct3g = 0.0f;

   std::vector<G4double> frac = {1.-fPickOffFraction-direct3g-foPsFraction , fPickOffFraction, direct3g , foPsFraction};
  // 2g direct //  2g pickoff // 3g direct // 3g  oPs
  return frac;
}
