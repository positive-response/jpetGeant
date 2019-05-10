#ifndef DetectorConstants_h 
#define DetectorConstants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"


class DetectorConstants 
{
  public:
      static const G4double world_size[3]; ///< max world size

      static const G4int layers = 3; ///< number of simulated layers in detector
      static const G4double scinDim[3]; ///<  dimensions of simulated strip

      static const G4double radius[layers]; ///< layer radius up to center of the strip
      static const int nSegments[layers]; ///< number of segments in each layer

      static const int extraLayers = 2;
      static const int nSegmentsExtraLayers[extraLayers];
      static const G4double radiusExtraLayers[extraLayers];

      static const G4double wrappingThickness; ///<  total width of used wrapping 
      static const G4double wrappingShift; ///<  free space between wrapping and scinitlator 


      // parameters for modular layer (known as 4th layer)
      static const G4double scinDim_inModule[3]; ///<  dimensions of simulated strip in modular layer

      static const int modulesInModularLayer = 24;



      static G4double GetMergingTimeValueForScin(); 
      static void SetMergingTimeValueForScin(G4double );

      static void SetChamberCenter(const G4ThreeVector);
      static const G4ThreeVector GetChamberCenter();
  private:
     static G4double fScinHitMergingTime; ///ns
     static G4double fChamberCenter[3]; 


};


#endif
