JPetMC is a simulation of the JPET detector conducted in GEANT4 enviroment.  
=============================================================================

## Source code:
https://github.com/daria137/jpetmc.git

## Required packages:
- geant4.10.04
- root 6.06.02 (not working with 6.12.04)
- cadmesh (https://github.com/christopherpoole/CADMesh)
- cmake 
- doxygen

## How to compile? 
cmake . &&  make
- output file: bin/jpet_mc

## How to create documentation?
make doc
- open the doc/index.html in your favourite web browser 

## Basics information
- geometry
    - 3 layers of scintillators (48,48,96)
    - each scintillator: 1.9x0.7x50 cm^3 wrapped in kapton foil
    - detector frame: constructed based on simple volumes or loaded by CAD files
- materials can keep information about oPs creation and lifetime
- desired solution: for each run type a dedicated target should be loaded  

## TODO list:
1. check geometry overlapping (done)
2. user friendly interface
3. fill all information in root trees
4. include big annihilation chamber (2g /3g direct annihilations)
5. integrate with framework output  (separate program - cosmics from data runs; input should check if number of generated events 
corresponds to the source activity)

