JPetMC 
=============================================================================

# Software information
JPetMC is a simulation program written in a GEANT4 environment.
Details of the J-PET detector can be found on the project webpage [webpage](http://koza.if.uj.edu.pl/pet/).
Its main features:
- the J-PET detector geometry is loaded from the (CAD) technical drawings,
- user can load detector setups (identified by run numbers), which corresponds to 
one used in the experiment. Details can be found in section [How to run](#how-to-run),
- the mcGeant.root is created as an output file. It can be processed by the J-PET Framework
software

## Source code:
https://github.com/JPETTomography/J-PET-geant4

## Required packages:
- geant4.10.04
- root 6.0 
- cadmesh (https://github.com/christopherpoole/CADMesh)
- cmake 
- doxygen

## How to compile? 
mkdir build
cd build
cmake ..
make
- output file: (in build folder) bin/jpet_mc

## How to create documentation?
(in build folder) 
cmake .. && make doc
- open the doc/html/index.html in your favourite web browser 


# How to run 
The executable program is located in build/bin/jpet_mc. 
Simple execution of the program (./jpet_mc) should load the init_vis.mac script and show detector geometry.

User can adjust detector parameters and generated gamma-quanta by using macros. Exemplary  macro is given in bin folder along the executive file.

## Geometry
User can load one of the selected geometries:
 - /jpetmc/detector/loadGeomForRun runNr
   - runNr=3/5/6
    - only implemented since now 
    - loaded JPetBasicGeom
 - /jpetmc/detector/loadJPetBasicGeom 
   - 3 layers of scintillators (48,48,96)
   - each scintillator: 1.9x0.7x50 cm^3 wrapped in kapton foil
   - detector frame: loaded from CAD file
   - standard setup for runs 1-6
 - /jpetmc/detector/loadIdealGeom 
   - not implemented yet
   - should load n-layer fully packed with scintillators barrel
 - /jpetmc/detector/loadJPetExtendedGeom 
   - not implemented yet
   - should load JPET standard geometry with additional read-out modules
 - /jpetmc/detector/loadOnlyScintillators 
   - shows only scintillator wrapped in a kapton foil   

Following modifications for Runs can be introduced:
- simulate only oPs gamma: /jpetmc/material/xad/threeGammaOnly 
- simulate only 2g: /jpetmc/material/xad/twoGammaOnly
- simulate only pick-off process (2g & long lifetime): /jpetmc/material/xad/pickOffOnly
- define longest lifetime component: /jpetmc/material/xad/oPslifetime
- define fraction of oPs annihilations (0-1): /jpetmc/material/xad/oPsFraction
- define fraction of pick-off processes (0-1): /jpetmc/material/xad/pickOffFraction
- set center of the annihilation chamber: /jpetmc/run/setChamberCenter
- for run5: define range where we expect annihilations to occur: /jpetmc/run/setEffectivePositronRange  

General parameters:
- define time range, between hits in scintillator, which will be classified as single hit: /jpetmc/detector/hitMergingTime

## 


