/**
 *  @copyright Copyright 2019 The J-PET Monte Carlo Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetMC.cpp
 */

#include "MaterialParameters.h"
#include "MaterialExtension.h"

MaterialExtension::MaterialExtension( G4String materialType, const G4String& name, const G4Material* baseMaterial) :
  G4Material(
    name, baseMaterial->GetDensity(), baseMaterial, baseMaterial->GetState(),
    baseMaterial->GetTemperature(), baseMaterial->GetPressure()
  )
{
  fTarget = false ;
  //! depends on the material typ
  Material = new MaterialParameters();
  Material->SetMaterialByName(materialType);
  FillIntensities();
}

MaterialExtension::~MaterialExtension() {}

void MaterialExtension::Add_oPsComponent( G4double Lifetime, G4double Probability )
{
  if(Lifetime < 0. || Lifetime > 142.) 
    G4Exception("MaterialExtension", "ME01", JustWarning, "given oPs lifetime not in the range [0, 142] ns"); 
  else if( Probability < 0. || Probability > 1.)
    G4Exception("MaterialExtension", "ME01", JustWarning, "given oPs probability not in the range [0, 1]"); 
  else 
    Material->Add_oPsComponent(Lifetime, Probability);
}

void MaterialExtension::Add_directComponent( G4double Lifetime, G4double Probability )
{
  if(Lifetime < 0.1 || Lifetime > 0.6) 
    G4Exception("MaterialExtension", "ME01", JustWarning, "given direct lifetime not in the range [0.1, 0.6] ns"); 
  else if( Probability < 0. || Probability > 1.)
    G4Exception("MaterialExtension", "ME01", JustWarning, "given direct probability not in the range [0, 1]"); 
  else 
    Material->Add_DirectComponent(Lifetime, Probability);
}

void MaterialExtension::Set_pPsComponent(G4double Lifetime, G4double Fraction)
{
  if( Lifetime < 0. || Lifetime > 0.2 ) 
    G4Exception("MaterialExtension", "ME01", JustWarning, "given pPs lifetime not in the range [0, 0.2] ns"); 
  else if( Fraction < 0. || Fraction > 1.)
    G4Exception("MaterialExtension", "ME01", JustWarning, "given pPs fraction not in the range [0, 1]"); 
  else 
    Material->Set_pPsComponent(Lifetime, Fraction);
}

const G4double MaterialExtension::GetLifetime( double randNumber, G4String channel )
{
  if( channel == "ortho2G" )
    return Material->GetoPs2GLifetimeFromVector(randNumber);
  else if( channel == "ortho3G" )
    return Material->GetoPs3GLifetimeFromVector(randNumber);  
  else if( channel == "para2G" )
    return Material->GetpPsLifetime();
  else
    return Material->GetDirectLifetimeFromVector(randNumber);
}

void MaterialExtension::ChangeMaterialConstants()
{
  Material->SetMaterialByName("");
}

const std::vector<G4double> MaterialExtension::GetEventsFraction()
{
  std::vector<G4double> frac;
  if(MaterialParameters::AnnihlationMode != "")
  {
    if(MaterialParameters::AnnihlationMode == "oPs3G")
    {
      frac = {0.,0.,0.,0.,1.};
      return frac;
    }
    else if(MaterialParameters::AnnihlationMode == "oPs2G")
    {
      frac = {0.,0.,1.,0.,0.};
      return frac;        
    }
    else if(MaterialParameters::AnnihlationMode == "pPs2G")
    {
      frac = {1.,0.,0.,0.,0.};
      return frac;              
    }
  }
    
  G4double pPs = Material->GetpPs2GTotalIntensity();
  G4double direct2g = Material->GetDirect2GTotalIntensity();
  G4double oPs2G = Material->GetoPs2GTotalIntensity();
  G4double direct3g = Material->GetDirect3GTotalIntensity();
  G4double oPs3G = Material->GetoPs3GTotalIntensity();

  //! 2g direct // 2g pickoff // 3g direct // 3g oPs
  frac = {
  pPs,
  direct2g,
  oPs2G,
  direct3g,
  oPs3G
  };
  return frac;
}
