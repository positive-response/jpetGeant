Two options are provided for applying the Energy-Range cuts.
First option:
* To applying direct cut to stop the simulation and kill the produced secondary particles using:  
`/jpetmc/event/GetEnergyCut [value with unit(keV)]`  

for example if one want to apply cut on 511 keV photon if 
it deposits at lease 60 keV during first interaction, Cut value is chosesn
as 511 - 60 = 451 keV 

Second option: 
* The cut value on range(in mm) in material represents the minimum energy required to travel that range 
  by the the secondary particle ( in our case e- ). Such a cut can be invoked using the following command in macro:  
 `/jpetmc/event/GetRangeCut [value with unit(mm)]`
