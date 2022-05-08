### Pressure Visualization

When we look at the pressure contour, it can be observed that the highest pressure is seen in the upper right corner of the lid driven cavity and the lowest pressure can be observed at the left upper corner. The fkuid moves from high pressure to low pressure. For the top, fluid is moving from left to right and left part of the top is the place where the pressure is lowest and the right is the place where the pressure is highest.  

<img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray.png" width="500">


### Velocity Visualization

At boundaires, velocity is observed as zero. 

<img src="example_cases/LidDrivenCavity/Plots/contour_u_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/contour_v_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/Glyph_velocity_gray.png" width="500">




### Examination of SOR solvers behavior depending on ω

When maximum iteration number is observed with respect to ω which is relaxation factor a decrease is increased up to an ω value and after that maximum iteration number will again start to increase.

ω | max-iter | timestep |
--- | --- | --- |
0.5 | 3499 | 592 |
--- | --- | --- |
1.0 | 1132 | 1992 |
--- | --- | --- |
1.3 | 655 | 1992 |
--- | --- | --- |
1.6 | 346 | 1992 |
--- | --- | --- |
1.7 | 260 | 1992 |
--- | --- | --- |
1.8 | 176 | 1992 |
--- | --- | --- |
1.9 | 154 | 1992 |
--- | --- | --- |
1.95 | 295 | - |
--- | --- | --- |
1.99 | 1350 | - |


### The algorithm’s behavior depending on δt

Stability condition for δt is provided in Equation (13). We observed divergence for the time steps 0.01, 0.03 and 0.05. And convergence is observed for time steps 0.005, 0.007 and 0.009.

dt | timestep | stabilitiy condition |
--- | --- | --- |
0.05 | 5 | div |
--- | --- | --- |
0.03 | 7 | div |
--- | --- | --- |
0.01 | 71 | div |
--- | --- | --- |
0.005 | - | conv |
--- | --- | --- |
0.007 | - | conv |
--- | --- | --- |
0.009 | - | conv |


According to Equation (13) when δx and δt increased, δt will increase. And increase in δt leads to a smaller imax and jmax for stability. In the following case we observed u value for changing imax and jmax. For the increasing values of imax and jmax, we observed divergence.

imax/jmax | (i,j) | u(i,j) |
--- | --- | --- |
16 | (8,14) | 0.183833 |
--- | --- | --- |
32 | (16,28) | -nan |
--- | --- | --- |
64 | (32,56) | -nan |
--- | --- | --- |
128 | (64,112) | +nan |
--- | --- | --- |
256 | (128,224) | +nan |



### Effect of kinematic viscosity

When the viscosity change is investigated, for lower values of viscosity the motion can be propagate to deeper points of flow but for the higher values of viscosity the motion can propagate less deep into the flow. 

nu = 0.01 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu01.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu01.png" width="500"> 

nu = 0.002 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu002.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu002.png" width="500"> 

nu = 0.0005 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0005.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0005.png" width="500"> 

nu = 0.0001 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0001.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0001.png" width="500"> 
