# PSR Renderer
Rasterization rendering engine that uses physics and sound simulations to create a dynamic world.

# Guidelines
- Whenever we need to pass by reference, use & in the method signature (not *). This seems to be the easier way to achieve pass by reference.
- Whenever we need a function to return an object by reference (using pointers), and not a copy, use the std::unique_ptr class.
- Never use normal pointers and always use std::unique_ptr (never use the plain * to define a pointer)

# Rendering workflow
1. For each mesh, do the following: