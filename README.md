# J-PET Monte Carlo with Geant4
=============================================================================

## Software information
**JPetMC** is a simulation program written in a **GEANT4** environment.  
Details of the J-PET detector can be found on the project [webpage](http://koza.if.uj.edu.pl/pet/).  
Its main features:  
- the J-PET detector geometry is loaded from the (CAD) technical drawings  
- user can load detector setups (identified by run numbers), which corresponds to
one used in the experiment. Details can be found in section [How to run](#how-to-run)  
- the mcGeant.root is created as an output file. It can be processed by the [J-PET Framework](https://github.com/JPETTomography/j-pet-framework-examples) software  

## Required packages:  
* `cmake`  
* `root 6.0`  
* [`geant4.10.04`](https://github.com/Geant4/geant4)  
 Built with cmake flags:  
 `-DGEANT4_USE_QT=ON -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_GDML=ON`  
* [cadmesh](https://github.com/christopherpoole/CADMesh)
* doxygen

## How to compile?
`mkdir build`  
`cd build`  
`cmake ..`  
`make`  
output file: (in build folder)  
`bin/jpet_mc`  

## How to create documentation?
(in build folder)  
`cmake .. && make doc`  
- open the `doc/html/index.html` in your favorite web browser  

## How to run
The executable program is located in `build/bin/jpet_mc`.  
Simple execution of the program (`./jpet_mc`) should load the `init_vis.mac` script and show detector geometry.  
User can adjust detector parameters and generated gamma-quanta by using macros. Exemplary macro is given in `bin` folder along the executive file.  

## Available parameters
Please refer to [PARAMETERS](PARAMETERS.md) file for program run options, such as geometries, source parameters, saving files.
