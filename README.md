# Boids

The project simulates the behavior of flocks of objects called "boids" (abbreviation of bird-oid objects) in a two-dimensional snippet of a toroidal space (so called "pac-man" effect). The simulation creates randomly positioned and initialized boids that can move in the space following certain rules for boid-boid interactions. 
Additionally in the simulation there can be added simple obstacles with which the boids may interact in various ways. 

## Prerequisites

The project makes use of: 
- SFML 2.6.2
It is necessary to have any kind of SFML 2.6.x for the correct execution of these executables.

The SFML package used was retrieved with macOS system by:  
`brew install sfml@2`  
The CMakeLists.txt looks instead for any kind of SFML 2.x packages; it is advised to use the same version as the one said to be used in the files (2.6.x) to not encounter any compiling issue. 

It is discouraged for macOS users to run:  
`brew update sfml`   
as this may lead to the rewriting of the package as SFML 3.x, which has a completely new redefined set of functions. 
To further prevent this it is suggested to lock sfml@2 out of updates via:  
`brew pin sfml@2`  
It is advised, if SFML 3.x was already present to lock away the SFML 3.x package if installed via brew through the sequence of commands:  
`brew unlink sfml`  
`brew link sfml@2`  
this will lead sfml@2 (SFML 2.6.x) to be called over sfml (SFML 3.x).

## Builds

To build the actual executable and the corresponding tests we recommend creating two folders for the debug mode and release mode respectively.
First of all one must compile through:  
`cmake -S . -B build -G "Ninja Multi-Config" -DBUILD_TESTING=ON`  
- for **Debug Mode**:   
`cmake --build build --config Debug > build_debug.log`  
- for **Release Mode**:   
`cmake --build build --config Release > build_release.log`  
The release mode will generate a warning given by an unused variable that was kept for a nearby assert test. 

To run the executables within the chosen mode one can either use the command:  
`./build/Debug/BoidSimulation`   
or:  
`./build/Release/BoidSimulation`  

One is free to set   
`cmake -S . -B build -G "Ninja Multi-Config" -DBUILD_TESTING=OFF`  
in case he prefers not to build the tests.

## Tests Running

In the project a file called *test_boid.cpp* contains all the DOCTESTs.  
To generate these tests from the root directory to the Debug or Release subfolders one must use:  
- to have the tests executables in the debug mode folder:  
`cmake --build build --config Debug --target test_boid`  
- to have the tests executables in the release mode folder:  
`cmake --build build --config Release --target test_boid`  
To run the tests through CTest one obtains a single test called *boid_tests* and must run either:  
- `ctest --test-dir build --build-config Debug --output-on-failure`  
or:  
- `ctest --test-dir build --build-config Release --output-on-failure`  
To run the tests individually and in details one must give the commands:  
- `./build/Debug/test_boid`  
- `./build/Release/test_boid`  

## UI Guide and Other Functionalities

The executable when launched presents to the user the first menu called "mainMenu" in the files. Other menus and the simulation are reachable through the present buttons. In all the sub-menus the *Back* button leads back to the main menu with saved changes. 

### Main Menu

From the main menu the user can see four objects:
- *spawned boids* slider: this can be used to set how many boids there will be at the start of the simulation, from a minimum of 20 to a maximum of 300. The limit of 300 is the general arbitrary set limit for the boids present in the simulation. 
- **Boids Radii** this brings the user to the radii settings menu.
- **Modes** this brings the user to the modalities settings menu.
- **Start** this brings the user to the simulation.

### Boids Radii Menu

From the *Boids Radii* menu one can modify the four main radii that define the behaviour of the boids:
- *Boid Size* relates to the boid own size. The value is centered to a default value and the slider let the user choose between one tenth of the default size or an increase up to 90% of the default value.
- *Separation Radius* relates to the distance at which the boids feel the separation forces. The value is centered to a default value and the slider let the user choose between a decrease or increase of 40% of the default value.
- *Cohesion Radius* relates to the distance at which the boids feel the cohesion forces. The value is centered to a default value and the slider let the user choose between a decrease or increase of 40% of the default value.
- *Alignment Radius* relates to the distance at which the boids feel the alignment forces. The value is centered to a default value and the slider let the user choose between a decrease or increase of 40% of the default value.

### Modes Menu

From the *Modes* menu one can check in what way the boids will behave:
- the **Gradual Damage** check leads the boid to receive only partial damages when hitting obstacles, so that they pass through different stages of damage signaled by various colours before "dying". 
- the **Complete Evasion** check leads the boids to completely avoid the obstacles through an additional repulsion force.
- the **Arrow Following** check leads the boids to follow the user's arrow pointer. 
All of these modes when checked are applied since the very beginning of the simulation. 

### Simulation

The simulation starts with various randomly generated boids. These will behave accordingly to the previously written rules. 
During the simulation there are various keys that can be used to update some of the modes while in the simulation:
- "**E**" allows to activate or deactivate the "complete evasion" mode.
- "**F**" allows to activate or deactivate the "arrow following" mode.
- "**O**" allows to interchange between obstacles or boids generation through left click.
- "**A**" allows to exit the game returning to the main menu. This leads to the reset of the simulation: any "progress" made like the generated obstacles or added boids will be deleted. All the modes will go back to the settings dictated by the Modes menu.  
