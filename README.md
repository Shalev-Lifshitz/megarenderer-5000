# PSR Renderer
Rasterization rendering engine that uses physics and sound simulations to create a dynamic world.

# Guidelines
- Whenever we need to pass by reference, use & in the method signature (not *). This seems to be the easier way to achieve pass by reference.
- Whenever we need a function to return an object by reference, and not a copy, use the memory::unique_ptr class.
- Never use normal pointers and always use memory::unique_ptr (never use the *)?