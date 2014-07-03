Cellule
=======

Cellule is a fast 2D cellular pattern ( Delauney-Voronoi ) engine written in C++ and is cross-platform. 

The Cellule foundations library ( CelluleF ) implement the data structure which is a 2D Delauney triangulation ( the corresponding Voronoi diagram is implicit ) of a set of circles ( weighted points which give the cells of the Voronoi diagram ). 
Cells can be added moved deleted dynamically and efficiently. 
The structure can quickly return the cell enclosing a given 2D point. 
Manipulating thousands of cells is ok. 

It can be used for 2D collision detections, 2D procedural cellular patterns generation, ... 
The Cellule world library ( CelluleW ) bring more features like datas association with cells, cell generators, ... 
Then the executable Cellule program is a GUI to create, modify, and visualize cellular patterns. 

Author: bEngo  foolscreen@gmail.com

Contributor: julianjon
