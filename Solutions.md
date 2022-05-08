### Pressure Visualization

<img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray.png" width="500">


### Velocity Visualization

![](example_cases/LidDrivenCavity/Plots/contour_u_gray.png)
![](example_cases/LidDrivenCavity/Plots/contour_v_gray.png)
![](example_cases/LidDrivenCavity/Plots/Glyph_velocity_gray.png)




### Examination of SOR solvers behavior depending on ω

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

nu = 0.01 :
![](/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu01.png) | ![](/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu01.png)

nu = 0.002 :
![](/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu002.png) | ![](/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu002.png)

nu = 0.0005 :
![](/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0005.png) | ![](/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0005.png)

nu = 0.0001 :
![](/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0001.png) | ![](/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0001.png)
