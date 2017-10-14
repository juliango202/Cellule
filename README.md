Cellule
=======

Cellule is a fast 2D cellular pattern ( Delauney-Voronoi ) engine written in C++ and is cross-platform. 

It can be used to generate interesting cellular procedural textures.

The Cellule foundations library ( CelluleF ) implement the data structure which is a 2D Delauney triangulation ( the corresponding Voronoi diagram is implicit ) of a set of circles ( weighted points which give the cells of the Voronoi diagram ). 
Cells can be added moved deleted dynamically and efficiently. 
The structure can quickly return the cell enclosing a given 2D point. 
Manipulating thousands of cells is ok. 

Then the executable Cellule program is a GUI to create, modify, and visualize cellular patterns. 

[Sample 1](https://juliango202.github.io/img/cellule/snap1.png | width=80)
[Sample 2](https://juliango202.github.io/img/cellule/snap2.png | width=80)

Author: bEngo  foolscreen@gmail.com

Contributor: juliango
