# ParticleSystem
A small scale particle system that reads XML scripts to visualize a particle effect.

The system supports 4 types of emitters: Point(Default, particles spawn from a single point in space), Sphere(Spawns particles on the inside of a spherical shape), CircleEdge(Spawns particles on the circumference) , and Box (Spawns particles enclosed in a box shape)

The system supports, continouos and burst modes. it also supports random and consistent particle birth rates.

Comes with 3 basic particle affectors: velocity, fade, and scale which can affect the particles' velocity, fade, and scale over its life time respectively

The system also supports 3 blending modes: Additive, Fade, and no blending.

The system comes with a fire effect and a magic circle effect. 

Pressing the space bar will cycle between effects.

The system comes with a free move camera

# Install Instructions

## Windows

### Setting up Dev Environment

If dev environment not set up yet from other samples or TechLab 2050, follow these steps:

1) Install Visual Studio Community Edition 
2) Install Visual Studio Code
3) Install CMake
4) Ensure CMake is on your PATH
5) Install C/C++ VS Code Extension (0.28.3 or above)
6) Install CMake Tools VS Code extension (version 1.4.1 or above)
7) Install CMake VS Code extension (0.0.17 or above)
8) Install TortoiseGit (or another Git client, but I'll assume tortoise git)

### Building samples

1) Clone or download this repository.
2) Open the folder in VS Code
3) Ctrl+Shift+P then type/choose CMake: Configure
4) Choose one of the available options (e.g. Visual Studio Community 2017 Release - amd64)
5) Ctrl+Shift+P then type/choose CMake: Build
6) Hit F5 to run in debugger if all compiled well
7) You may need to choose "sample" from the dropdown the first time

## Linux

### Setting up Dev Environment

If dev environment not set up yet from other samples or TechLab 2050, follow these steps:

1) Install gcc or clang
2) Install Visual Studio Code
3) Install CMake
4) Ensure CMake is on your PATH
5) Install C/C++ VS Code Extension (0.28.3 or above)
6) Install CMake Tools VS Code extension (version 1.4.1 or above)
7) Install CMake VS Code extension (0.0.17 or above)
8) Install git

### Building samples

1) Clone or download this repository.
2) Open the folder in VS Code
3) Ctrl+Shift+P then type/choose CMake: Configure
4) Choose one of the available options (e.g. gcc)
5) Ctrl+Shift+P then type/choose CMake: Build
6) Hit F5 to run in debugger if all compiled well
7) You may need to choose "sample" from the dropdown the first time
