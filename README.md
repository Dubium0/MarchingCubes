# Marching Cubes Algorithm Implementation on GPU 
## Description 
The main focus of this project is to implement the Marching Cubes algorithm on the GPU. The algorithm has been implemented through the geometry shader, utilizing lookup tables and grid points sent from the CPU. The goal is to efficiently generate three-dimensional surfaces based on the input data, enhancing performance by leveraging the parallel processing capabilities of the graphics processing unit.


## How to use
1. Clone the project 
2. Build the project using CMake

You can experiment with different resolutions using the 'gridResolution_3D' variable in the 'src/MarchingCubes.cpp' file.
The 'resources/shaders/marchingCubes.geom' file contains the algorithm implementation. You can define and visualize your own functions using the 'densityFunction' there.

## TO DO 
- Dynamically change the grid resolution.
- Add examples of different visualizations. 
- Clean up the project, and remove unnecessary stuff.
- Make further optimization. It is slower than I expected right now.

