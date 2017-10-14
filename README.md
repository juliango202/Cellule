# Cellule

**NOTE: this project has not been updated since 2008. See [Grasshopper 3D](https://en.wikipedia.org/wiki/Grasshopper_3D) for a more usable implementation of this idea.**

Cellule is a fast 2D cellular pattern ( Delauney-Voronoi ) engine written in C++ and cross-platform. 

The main goal was to generate interesting cellular procedural textures of infinite resolution.

## Gallery
[![Sample 1](https://juliango202.github.io/img/cellule/snap1_th.png)](https://juliango202.github.io/img/cellule/snap1.png)
[![Sample 2](https://juliango202.github.io/img/cellule/snap2_th.png)](https://juliango202.github.io/img/cellule/snap2.png)
[![Sample 3](https://juliango202.github.io/img/cellule/snap3_th.png)](https://juliango202.github.io/img/cellule/snap3.png)
[![Sample 4](https://juliango202.github.io/img/cellule/snap4_th.png)](https://juliango202.github.io/img/cellule/snap4.png)
[![Sample 5](https://juliango202.github.io/img/cellule/snap5_th.png)](https://juliango202.github.io/img/cellule/snap5.png)
[![Sample 6](https://juliango202.github.io/img/cellule/snap6_th.png)](https://juliango202.github.io/img/cellule/snap6.png)
[![Sample 7](https://juliango202.github.io/img/cellule/snap7_th.png)](https://juliango202.github.io/img/cellule/snap7.png)
[![Sample 8](https://juliango202.github.io/img/cellule/snap8_th.png)](https://juliango202.github.io/img/cellule/snap8.png)
[![Sample 9](https://juliango202.github.io/img/cellule/snap9_th.png)](https://juliango202.github.io/img/cellule/snap9.png)

## Implementation
The Cellule foundations library ( CelluleF ) implement the data structure which is a 2D Delauney triangulation ( the corresponding Voronoi diagram is implicit ) of a set of circles ( weighted points which give the cells of the Voronoi diagram ). 
Cells can be added moved deleted dynamically and efficiently. 
The structure can quickly return the cell enclosing a given 2D point. 
Manipulating thousands of cells is ok. 

The executable Cellule program is a GUI to create, modify, and visualize cellular patterns. 

### Contour smoothing
Each pixel of a cell is textured according to the shortest distance to a voronoi edge.
To be able to support arbitrary smoothness of cell contours, we build a bspline along the Voronoi edge before computing the distance.

![BSpline 1](https://juliango202.github.io/img/cellule/bspline01.png)
![BSpline 2](https://juliango202.github.io/img/cellule/bspline02.png)
![BSpline 3](https://juliango202.github.io/img/cellule/bspline03.png)
![BSpline 4](https://juliango202.github.io/img/cellule/bspline04.png)

## Credit
Author: bEngo  foolscreen@gmail.com

Key contributor: juliango

Licence: GNU GENERAL PUBLIC LICENSE Version 2
