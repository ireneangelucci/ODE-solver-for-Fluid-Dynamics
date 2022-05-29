## Worksheet 2

### Plain Shear Flow

### Karman Vortex Simulation

### Flow Over a Step

### Natural Convection

### Fluid Trap Simulation

### Rayleigh Benard Convection


---

## Worksheet 1
### Pressure Visualization

The image below shows the pressure contour for the lid driven cavity simulation, after some thousands time steps. As the picture shows, the highest pressure is in the upper right corner of the lid driven cavity while the lowest pressure is observed at the left upper corner. If one also takes a look at the glyphs in the velocity profile (next section), then it's obvious that the fluid moves from high pressure to low pressure. The only exception is the top of the cavity. Here the cells used for the discretization are "attached" to the lid and hence move with it (from low pressure to high pressure). 

<p> 
<img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray.png" width="500"> | <img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray_scaled.png" width="500">
</p>
<em>Pressure Contour default [left] and scaled [right]</em>

### Velocity Visualization

A expected, velocity is zero at left, right and bottom boundaries, while it's equal to the lid's velocity (one) at the top boundary, where cells are attached to the lid (as explained in the previous section).

<p> 
<img src="example_cases/LidDrivenCavity/Plots/contour_u_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/contour_v_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/Glyph_velocity_gray.png" width="500">
</p>
<em>Velocity Contour in x-direction [left], Velocity Contour in y-direction [middle] and Velocity Glyph [right]</em>


### Examination of SOR solvers behavior depending on ω

To examine the SOR solver's behaviour depending on the relaxation factor ω, we conducted the simulation with different values of ω and for each one of these and for each time step, we searched for the maximum number of iterations necessary for the solver to converge (res < eps). Among the ω values we used, the one that required less iterations to converge was ω=1.9. For very low values of ω a very large number of iterations is necessary to obtain convergence. That's consistent with the SOR solver formulation since p(n+1)->p(n) for ω->0. In the table below we report also the time step at which the max iteration is reached, i.e. the time step that required the maximum number of iterations to converge.

ω | max-iter | timestep |
--- | --- | --- |
0.5 | 3499 | 592 |
1.0 | 1132 | 1992 |
1.3 | 655 | 1992 |
1.6 | 346 | 1992 |
1.7 | 260 | 1992 |
1.8 | 176 | 1992 |
1.9 | 154 | 1992 |
1.95 | 295 | - |
1.99 | 1350 | - |


### The algorithm’s behavior depending on Timestep (δt)

In our code we provided an implementation for adapting the time step size δt in accordance to the stability condition in Equation (13). However, in order to analyse the algorithm's behaviour depending on δt, we kept it constant and equal to the values reported in the table below throughout the whole simulation. We observed that the solution diverged for the timesteps 0.01, 0.03 and 0.05 and converged for timesteps 0.005, 0.007 and 0.009.

dt | timestep | stabilitiy |
--- | --- | --- |
0.05 | 5 | Diverged |
0.03 | 7 | Diverged |
0.01 | 71 | Diverged |
0.009 | - | Converged |
0.007 | - | Converged |
0.005 | - | Converged |

### The algorithm’s behavior depending on i_max = j_max

We also studied the algorithm's behaviour when using different values of i_max an j_max. The bigger i_max=j_max gets, the finer the grid gets (smaller dx and dy). To ensure the stability condition (Equation 12), δt also has to smaller. We conducted our simulations for varying values of i_max=j_max, but always keeping δt constant to 0.05. We then looked at u(i_max/2, 7*j_max/8) at the end of the simulation for each one of these time steps. The only converged result we obtained was for i_max=j_max=16 for kinematic viscosity, nu=0.01, in all the other cases, solution diverged. However, for nu = 0.001, we obtained convergence upto i_max=j_max=32. The solution diverged for cases with further refinement. In order to obtain physical results also for bigger values of i_max=j_max we would need to decrease the time step size δt. 

imax/jmax | Cell(i,j) | u(i,j) for nu=0.01 | u(i,j) for nu=0.001 |
--- | --- | --- | --- |
16 | (8,14) | 0.183833 | 0.139114 |
32 | (16,28) | -nan | 0.208146 |
64 | (32,56) | -nan | -nan |
128 | (64,112) | +nan | -nan |
256 | (128,224) | +nan | +nan |



### Effect of kinematic viscosity

We investigated the effect of kinematic viscosity on our simulation. We looked at the velocity profile for different values of kinematic viscosity (nu). For lower values of viscosity the effect of the moving lid can propagate deeper into the fluid, however the velocity magnitude is very high only for regions of fluid that are very close to the lid. This can be understood by considering the Reynolds number (Re), which is inversely proportional to the kinematic viscosity. At large Re, the boundary layer attached to the lid is similar to a turbulent boundary layer (zero relative velocity close the wall which decreases sharply as you move away from it). Also, large Re corresponds to greater mixing in the flow causing the effect of moving lid to be felt at distant locations in the cavity.

For large viscosity (low Re), the region of "high" (close to 1) velocity magnitude extends more (as in a laminar flow boundary layer) but fluid away from the lid remains largely unaffected (the rest of the cavity is at values very close to zero).

nu = 0.01 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu01.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu01.png" width="500"> 

nu = 0.002 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu002.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu002.png" width="500"> 

nu = 0.0005 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0005.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0005.png" width="500"> 

nu = 0.0001 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0001.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0001.png" width="500"> 
