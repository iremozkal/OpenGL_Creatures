# OpenGL_Creatures  

OpenGL object creation, translation, lighting&shading, camera and animation processes using GLUI interface with Visual C++.  

## Short Description  

There are three objects in the project: Rabbit, Monkey and Dragon.  
Lighting and toon shading are used on the objects.  
These objects are located on a ground created with grass texture and each object can be moved and rotated in the desired axis with the help of the GLUI interface.  
There is a freely movable light source and camera on the stage. Objects have heart beating and orbital rotation animations.

## Implementation

#### Creating 3D Objects  
The .obj files were converted to a .smf file, and objects were drawn by reading this file. Mesh class was created for reading operation. By using separate Mesh objects for each of the three objects, position and normal information were stored within these objects. After reading for each object, vertices and normals information is returned and loaded into buffers. To read the object files, [check this resource](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/).  

  ![creatures](https://github.com/iremozkal/OpenGL_Creatures/blob/main/Images/img1.PNG?raw=true)
  
#### Light Source Adjustments   
A light source was used on the stage and the user can adjust the position and light intensity of this light source with the interface, if desired. In OpenGL, to find the color under the light of a point, it is necessary to define 3 more light components other than the color given to that point: Ambient (from which point cannot be determined exactly), Diffuse (coming from a single point), Specular (coming from a certain point and with a bright surface reflecting in objects).  
Inside the vertex shader, defined normals for each vertex are available. The current normal vector is used to calculate the color of the vertex.  
Thanks to the prepared GLUI interface, the light moves in three axes and the illumination of the light can be controlled by the user by using the diffuse component of the light.

  ![creatures lighting](https://github.com/iremozkal/OpenGL_Creatures/blob/main/Images/img2.PNG?raw=true)

#### Texture Mapping  
The ground with the texture is initially created as a 'surface object'. To do texture mapping on this object, first a struct (Image) is created that contains the dimensions and information of the image. In the loadTextureBuffer () function, this texture is loaded into the buffer. By adding a picture with a .bmp extension to the loadTexture () function, a place for this image file is opened and the loading process is done.  
After the new shader files created for the surface are entered into the process and assigned to the program, the buffera texture information is discarded and the program is run. Grass picture was determined as the texture.

  ![creatures grass texture](https://github.com/iremozkal/OpenGL_Creatures/blob/main/Images/img3.PNG?raw=true)

#### Translation, Rotation and Sizing Operations
+ Translate  
Specific positions in x, y, and z coordinates for each object initially
are given. The translation matrix in which these positions are used is sent to the vertex shader uniformly. The translate information here is used to determine the position.  
Using the GLUI interface, the selected object can move freely in three dimensions with the help of the translate matrix. For this purpose, the selected coordinates of the selected objects are defined and the changes in the coordinates are reflected in the translation matrix to create new coordinates.
+ Rotation  
Rotation angle and axis are defined for each selected object.
The theta calculated to the Vertex shader is sent as uniform. Angle information is obtained by using this value and a rotation matrix is created.  
In the program, rotation is performed with the speedUp function using theta, speed and rotation axes of the objects. The object selected using the GLUI interface can rotate in three dimensions with the help of the rotation matrix.

#### Two Different Stage Appearances with Two Different Shading  
The user can change between 2 different shading options with the help of the interface. One is the shader (0) we use for the initial illumination of the objects, and its pillar is toon-shading (1).  
New vertex and fragment shaders have been created so that the user can choose between two different shading options. Toon shading effect was performed in the fragment shader. For this reason, normals were defined as varying variables and intensity calculation in fragment shader was performed over interpolated normals. In Intensity operations, not only normals but also light direction uniform variable were used. Intensity is equal to the dot product of these two values. The choice between shading options is made from the GLUI interface.

  ![creatures shadings](https://github.com/iremozkal/OpenGL_Creatures/blob/main/Images/img4.PNG?raw=true)

#### Perspective / Camera Adjustments
The camera can be adjusted to move in 3 axes and can rotate freely. GLUI Trackball is used for the camera. As stated, the camera can move up, down, left and right, as well as zoom in and out. Camera movements are free, but the up and down rotate movement is 180 degrees in the direction it is facing. It can go up to a certain distance from the screen size. It is made to prevent distortion of the image because the screen is reversed after a certain place. Its boundaries are also 90 degrees. Rotation movements are achieved by connecting the model view matrix to the rotation panel in the GLUI interface.

#### Animation  
As an animation, the dragon and the rabbit are rotated around the monkey. While doing the orbit rotate, the speed of the objects can be changed by + or - at the limit specified in the interface. For this, unlike the other rotate in the vertex shader, one angle is defined. With this angle, only the rotate matrix connected to the y axis is defined. It is moved away from the center by the translate matrix multiplication, and the orbit is multiplied by the rotate matrix. To move the object back to the center, it is multiplied by the inverse of the matrix we use to move it away from the origin. It is then translated with a second matrix to its original location.  
In addition, the beating heart effect can be applied to both objects with a scale matrix. The values of the sent scale matrix are created by using the scale function within the idle () function, increasing and decreasing them in certain sizes.

#### GLUI  

  ![creatures glui interface](https://github.com/iremozkal/OpenGL_Creatures/blob/main/Images/img5.PNG?raw=true)
