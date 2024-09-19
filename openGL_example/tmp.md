# lighting
defining the color on the surface of the objects.
- light source, material, complicated physical phenomena
We chose Phong model to descript lighting.

## ILLUMINATION MODEL
- There are two cases in illumination model depending on reflection light.
reflection light: the light made by the collision between the other obj and the light.

local / global illumination model
local : no considering reflecting light;
global : considering relfecting light. -> high costs, high performance.

## Phong's Illumination model
This model expresses the color between the light and objects by 3 terms.
- ambient / diffuse / specular
Phong model determines the color by adding above three terms of light.

### Ambient light
The basic light that reached to every objects. It is independent to normal vector and light direction.
passed by constant value.

it is composed of [light color, ambient strength] with object color.
These factors will be added in fragment shader to calculate for each pixels.

### Diffuse light
When the light collide to obj surface, the light spreaded every directions.
The amount of the diffuse light is defined by light source's direction and normal vector.
If the light source is orthogonal to objects, the diffuse light is maximum. (dot product of light and normal vector)

It is composed of light direction, strength and normal direction.
for normal vector, we need to add the normal value of the vertices.

gl_Position : the coordinatev value of canonical space.
If we want to calculate diffuse light, it could be done in World space. So, we transform the position and normal from local to world by model transformation.

the position calculated in vertex shader will be passed to fragment shader.

But the normal should be transformed by **inverse transpose of modelTransform**. because it is a vector, not a point.

the normal of pixels size might not be 1. so, the normal is re-normalized in fragment shader.

### SPECULAR LIGHT

the light reflected by the object surface.
when the view direction and reflected direction is same, the light is the strongest. (dot product of reflection and view)

the calculation of lighting will be done on each pixels. so, GPU should calculate the lighting for efficiency.
-> Making vertex, fragment shader.

as we said, the Phong model add three terms of light.

reflect(light, normal) function returns the reflected vector.
Dot product of reflectVector, viewVector is how much reflected light come in.

specularStrength is the strength of the light.
specularShininess is control the area of the reflected light.

---

final color is determined by light color and material(object) color.
ambient + diffuse + specular -> color.

we can make the variables to one structure.
-> struct Light, struct Material.


### Lighting maps
ambient, diffuse, specular terms are compose of material.
these could be replaced to texture map.

Download the texture image for lighting.
the color in texture map will replace the material's diffuse and specular term. the fragment shader will pick up the texture point and use the point's color for lighting.