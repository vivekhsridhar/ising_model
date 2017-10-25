About
------

The Ising model is a mathematical model of ferromagnetism from statistical physics. The model consists of discrete
variables that represent dipole moments of atomic spins. Each spin is allowed to exist in one of two states 
(+1 or -1). Usually, the spins are arranged in a square lattice where they interact with other spins within a defined
neighbourhood. In this implementation, spins interact with their four closest neighbours.

Use
---

The above code is written using C++11 and tested on MacOS Sierra 10.12.5. The visualisation is executed using 
OpenCV 3.2.0. In case of troubles installing OpenCV / linking it to the C++ project, visualisation can be turned 
off by silencing OpenCV headers and the graphics function in main.cpp. The rest of the code should still work on 
naked C++.

