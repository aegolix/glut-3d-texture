# glut-3d-texture

### Program Description
The key idea is to practice constructing 3D models based on primitive geometry (triangles and quads) with OpenGL draw modes such as GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN and GL_QUAD_STRIP; practice texture’s UV mapping on each vertex; and practice simple animation such as translation and rotation around object’s local axis. 
Each shape’s render() function takes parameters including that shape’s dimensions (height, big radius, small radius), position (mouse-click x, y, depth), texture ID and rotation per time unit. Where only mouse-click x, y are user’s input, the rest are randomized every time new shape is created.
User can toggle between wireframe mode and fill mode, paused rotation, reset rotation and resume rotation.
#### Usage instruction:
* Implemented shapes: Circular Plane, Cube, Pyramid, Sphere, Cylinder, Cone, Torus, Paraboloid and Hyperboloid.
*	Program draws Cube by default.
*	To draw other shapes:
    * Right-click menu, choose an item.
    * Left-click anywhere on the windows to draw shape at such x, y position (windows’ x, y are remapped to GL coordinates); or
    * Press key n or N to render such new shape at (0, 0, z).
* To change between WIREFRAME mode and FILL mode:
    * Right-click menu to choose; or
    * Press key w or W to toggle.
*	To reset objects to its initial rotation
    *	Press key r or R (stands for reset rotation)
* 	To pause objects' rotation
    *	Press key space bar to toggle
* To add more textures
    *	Put .jpg files in Textures/ folder with file name convention: tex<next-id>.jpg, e.g: tex3.jpg, tex22.jpg
    *	Update file total-number-of-textures.txt with new number of textures
#### Key implementation steps of each object:
##### 1.	Constructor ()
* Get random value of dimension, position, rotation and ID number of the texture (texture ID)
* Check if images have been loaded into texture array, if not, load the array. Then retrieve the texture with such texture ID
* Calculate vertices (x,y,z,u,v) and store into std::vector (I do this in one shape in the hope that it should render faster). 
* Output shape’s information to console.
##### 2.	Render ()
*	Reset model-view matrix.
*	Translate object to random depth.
*	Rotate object with current value of rotation angle. If reset-rotation is on, then don’t do rotation.
*	Calculate and draw vertices position (x, y, z) and texture mapping (u, v); or
*	Draw vertices position (x, y, z) and texture mapping (u, v) based on the data already calculated in constructor.
*	Update rotation angle for the next frame. If pause-rotation is on, then increase rotation angle by zero.
#### Load images into texture array:
*	Read from a text file one integer which is the total number of textures in the Textures/ folder. 
*	First iteration: load images into texture array using SOIL library with flag SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y to preserve texture colors and invert V mapping.
*	Second iteration: check if any texture in the array is null. If there is one, replace that slot with a texture that works (fallback to a texture).
*	Texture generation using data from the bitmap (glTexParameteri())