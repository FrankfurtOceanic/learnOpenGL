# learnOpenGL

This is a repo for documenting my learning progress in OpenGL!

## First milestone
After completing the first to sections of learnOpenGL (up to chapter 18), I have a scene with phong lighting that supports multiple light sources (directional lighting, point lights, and spotlights). Additionally, I have written 
vertex and fragment shaders in glsl. The vertex shader passes vertex information like the position, normal direction and the texure coordinates. It also handles the mode, view, and  projection matrices. The fragment shader takes in 
the albedo(diffuse) map and the specular map as well as multiple lights to calculate the fragment color based on the Phong lighting method and specular workflow (Results look similar to skyrim). The point and spot lights use attenuation to calculate the intensity of the light based of the distance.
In addition, the spot light has parameters to create a smooth gradient rather than a sharp lit section. Here's a gif that shows this off!

<img src="openglMultipleLights.gif" width = "400">  
