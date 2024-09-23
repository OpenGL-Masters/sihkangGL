# Object Loader
Making 3D obj by pointing each vertex is hard.
-> Objects are designed in 3D modeling tools.

## 3D modeling tool
create 3D model and modify.
-> modeling, sculpting, UV unwrapping, rigging, animation ...
e.g.) 3D studio Max, maya, Blender 3D.

## AASIMP
open asset import library.

address: github.com/assimp/assimp

support multiple language and 3D model files.
cross-platform. c/c++ interface.
assimp load the obj files to our program.

## Refactoring
### buffer class modifying
replace dataSize to stride and count.

`dataSize = stride * count`

### Adding mesh.h, mesh.cpp
we will load the mesh data from obj file.
mesh consist of the information of vertex buffer and vertex Layout.
So, mesh class should have these information(vertices, indices).

in mesh object, set the vertex, index buffer and bind to vertexArray.

### why is VertexLayout unique pointer, but Buffer is shared pointer?
VBO, EBO can be used for another VAO. (reusable)
VAO is only for certain mesh.

## Scene Tree(graph)
Method that manage the 3D scene as a tree structure.

child's transform information(position/rotation/scale) is described by parent's local coordinate.


model.h를 만ㄷ르어보자.
