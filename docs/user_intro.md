## How to run
The executable program is located in `build/bin/jpet_mc`.  
Simple execution of the program (`./jpet_mc`) should load the `init_vis.mac` script and show detector geometry.  
User can adjust detector parameters and generated gamma-quanta by using macros. Exemplary macro is given in `bin` folder along the executive file.  

## Examples 
Each example is prepared in a macro form and can be executed from the command line:  
`./jpet_mc macro.mac`

### Ps decays in the chamber
For selected J-PET run N a whole setup, scintillator configuration and annihilation chamber, can be called by single command:  
`/jpetmc/detector/loadGeomForRun N`  
Examples are given in macros: run5.mac, run6.mac, run7.mac.

### Beam of the photons
In order to simulate beam of photons user needs to use following command:  
`/jpetmc/source/setType beam`  
Example is given in macro singleBeam.mac.

### Cylindrical isotope
An extended cylindrical source can be defined in following way:  
`/jpetmc/source/setType isotope`   
Example is given in macro extendedSource.mac.

### NEMA sources
In the plane comprising the central detector axis the 0.1-mm cylindrical 22 Na sources are simulated.
 Their positions were chosen according to the NEMA NU 2-2012 norm and coordinates
are adjusted to the laboratory measurement preformed in laboratory. Simulation is executed
by command:   
`/jpetmc/source/setType nema`   
`/jpetmc/source/nema P`   
where P denotes source position (1-6).


## Interaction with the simulation
Available parameters are stored in  [PARAMETERS](PARAMETERS.md) file.  
User can modify the run options, such as geometries, source parameters, saving events.
