# Marching Cubes Algorithm Implementation on GPU 
## Description 
The main focus of this project is to implement the Marching Cubes algorithm on the GPU. The algorithm has been implemented through the compute shader, utilizing lookup tables and grid points sent from the CPU. The goal is to efficiently generate three-dimensional surfaces based on the input data, enhancing performance by leveraging the parallel processing capabilities of the graphics processing unit.


## How to use
### Windows
1. Clone the project 
2. Create a build directory in project folder and run cmake
   ```
   mkdir build
   cd build
   cmake ..
   ```
3. Build the project
   ```
   cmake --build .
   ``` 

You can experiment with different resolutions using the 'gridResolution_3D'(make sure you have enough ram for high resolutions like 512(24.4 gb)) variable in the 'src/MarchingCubes.cpp' file.
The 'resources/shaders/marchingCubes.comp' file contains the algorithm implementation. You can define and visualize your own functions using the 'densityFunction' there.

## Functionalities
- Function based 3d surface generation
- Smooth shading
- High run-time performance 


## TO DO 
- Add examples of different visualizations. 
- Make further optimization. Memory is highly occupied with this setup.



