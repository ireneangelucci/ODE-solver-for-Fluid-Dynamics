## Simulating Non-Newtonian Shear Thinning Fluids
*Group B*

*Manish Mishra, Irene Angelucci, Basak Kapusuzoglu*

### Theory
Non-newtonian fluids are - types - plot of different types

expected velocity for shear thinning and shear thickening fluids

Egs. of shear thinning fluids

Blood and importance, Wall Shear Stress

### Objective
1. Simulate blood and observe the effect of non newtonian model
2. To understand scenarios where non newtonian models are significant
3. 

### Modified Algorithm
Scheme - where is the viscosity, averaging for flux computation
Just add the algorithm `plot here` 
highlight key changes wrt fluidchen

### Pipe Flow with Inflow Boundary
We selected flow in a pipe as the base case for our study because of its simplicity and resemblance to the actual problem of blood flow in arteries. We began with simulating newtonian flow to establish ground results to compare non-newtonian simulations with later on. However, we observed that the velocity profile along the y-direction remains unchanged (same parabolic profile) for different viscosity values at the steady state. As we looked closely, we realized that this can be attributed to the physics of the problem at the steady state and uniform velocity inflow boundary condition. Derivation is shown below for reference. However, in this case, we get a straight forward relationship between WSS and viscosity. As the velocity profile remains unchanged, WSS is directly proportional to the viscosity (or inversely proportional to the Re).

<p> 
<img src="docs/ProjectPlots/Derivation_PipeFlow.png" width="800">
</p>
<em>Derivation showing cross-section velocity profile in a pipe flow with inflow bounday condition is independent of the viscosity </em>

Re for blood flow in arteries is ~1000. We carried out both Newtonian & Non-Newtonian simulations at Re 3000 & Re 300 and compared the WSS for both cases. Velocity profile along cross section was same for the reasons explained above, however the viscosity for non Newtonian case was slightly different. In both the cases, we obtained a lower WSS for non-Newtonian case. For Re 3000, the difference was about ~0.4% while for Re 300, the difference was as high as ~4%. 

<p> 
<img src="docs/ProjectPlots/ViscosityField_nupt2_inflowBC.png" width="500">
<img src="docs/ProjectPlots/VelocityField_nupt2_inflowBC.png" width="500">
</p>
<em> Viscosity field (left) and Velocity field (right) for Non-Newtonian simulation of a pipe flow with limiting viscosity 0.2  </em>

<p> 
<img src="docs/ProjectPlots/PressureDropComparison_inflowBC_nupt2.png" width="500">
<img src="docs/ProjectPlots/VelocityProfileComparison_inflowBC_nupt2.png" width="500">
</p>
<em> Viscosity field (left) and Velocity field (right) for Non-Newtonian simulation of a pipe flow with limiting viscosity 0.2  </em>

### Pressure Driven Pipe Flow
To circumvent the problem, we moved towards pressure-driven flow simulation. In this case, the pressure (and hence the pressure gradient) is constratint causing the velocity variation to be dependent on the viscosity (this can be seen in the last expression in the derivation above). This case is also much closer to the human body where the heart pumps the blood with a certain pressure (although the pressure is not constant but cyclic). 

We carried out Newtonian and Non-Newtonian simulation with viscosity 0.2 (for non-Newtonian case, this value represents the viscosity when the power law coefficient goes to zero, i.e. limiting Newtonian viscosity). With a fixed pressure drop of 1 unit over the entire length, we obtained ~40% higher WSS in Non-Newtonian case. We observed that the velocity gradient at wall was smaller but viscosity was much higher, leading to the higher WSS. However, this difference can be simply attributed to the difference in Re. Re for non-Newtonian case is ~45% of Re for the Newtonian simulation. Note that Re is calculated with peak velocity and viscosity near the wall (which is also the minimum value in the domain).

In the scaled velocity profile, we observe the expected trend. The velocity profile is steeper near the wall while flatter at the top when compared to the velocity profile for Newtonian case.

<p> 
<img src="docs/ProjectPlots/VelocityComparison_PD.png" width="570">
<img src="docs/ProjectPlots/VelocityComparison_PD_scaled.png" width="500">
</p>
<em>Velocity profiles actual (left) and scaled(right) for Newtonian and Non-Newtonian simulation for pressure-driven flow with viscosity 0.2 </em>

compare velocity profile for two viscosities (newtonian) + 
compare vel profile for newtonian and non newtonian 

For higher viscosity, pressure driven flow simulation becomes unstable for the specified pressure drop of 1 unit.

### Simulating Blood Flow
just add observations

talk about cases when effect can be more severe, like curved cases, pressure fluctuation at source (heart), elastic walls, etc. 


### Channel with step

### Channel with Obstacle

### Key conclusions
1. WSS shear stress varies with Reynolds number.
2. Inflow boundary condition leads to same steady state velocity profile in pipe flow.
3. Pressure driven flow yields different velocity profile for same applied pressure drop across it - 
4. Non Newtonian effects are dominant in unsteady cases at low Re (high viscosity). 