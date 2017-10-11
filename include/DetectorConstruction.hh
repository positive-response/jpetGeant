#ifndef DetectorConstruction_h 
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "DetectorSD.hh"


class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField(); 

    G4LogicalVolume * scinLog;
};


#endif
