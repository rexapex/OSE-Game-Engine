### Origami Sheep Engine V.2

A modular game engine project

### Dependencies

- rapidxml (included in project)

### Installation

###### On Windows 10, with Visual Studio 2017

- Clone the git repository
- Open the project file in VS2017 and build in release mode

###### Alternatively, on Linux with g++

- Clone the git repository
- To build the precompiled header, run the following command: g++ -c stdafx.h -o ../../build/stdafx.h.gch
- Then, run the following command: g++ -std=c++17 *.cpp -o ../../build/ose -pthread