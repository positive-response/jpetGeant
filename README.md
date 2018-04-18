JPetMC 
=============================================================================

# Software information
JPetMC is a simulation program written in a GEANT4 environment.
Details of the J-PET detector can be found on the project webpage [webpage](http://koza.if.uj.edu.pl/pet/).
Its main features:
- the J-PET detector geometry is loaded from the (CAD) technical drawings,
- user can load detector setups (identified by run numbers), which corresponds to 
one used in the experiment. Details can be found in section [Simulated setups](#simulated-setups),
- the mcGeant.root is created as an output file. It can be processed by the J-PET Framework
software

## Source code:
https://github.com/daria137/jpetmc.git

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


# Simulated setups
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

