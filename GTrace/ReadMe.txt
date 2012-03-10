Description
------------
GTrace is a simple ray tracer project with educational purposes.

.Current features:
* Phong lighting model
* Area light and soft shadow approximation
* Reflections and refractions
* Basic scene management and primitives
* Flexible material system with texture mapping
* Arbitrary camera orientation with pitch yaw and roll

.Planned features:
* More procedural textures with expression templates
* KD tree support
* Photon mapping
* Sub surface scattering
* Mesh importer and triangle support
* Texture filtering
* Depth of field
* Anti aliasing

.TODO list:

* Fixes
** Fix area lights: use fixed grid points for samples, randomize around these points for each sample

* Features
** Introduce BRDF. Freshnel equation for specular lighting, and also for reflection/refraction 
** Introduce Beer's law for absorption
** Introduce light types: directional, point, area. Adjustable sample size per light for area lights. Lights should be rendered too as objects.
** Redesign scene management classes: body, light, object?

* Optimizations
** Vectorization optimization: use Vector4f instead of 3f, better for vectorization!
** Trace rays in batch! 32x32
** Resolve whole material in one go, instead of doing each component separately.
** Vector pass by ref or value ?? Research

Bulding the project
-------------------
* Clone the project from git
* Download eigen3, unpack into 3rd/eigen directory next to GTrace
* Dowload CImg library, unpack into 3rd/CImg directory next to GTrace
* Build in visual studio 2010