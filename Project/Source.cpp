#include "Angel.h"
#include "Mesh.h"
#include "glui.h"
#include <iostream>
using namespace std;

#define OBJECT1 101
#define OBJECT2 102
#define OBJECT3 103
#define CAMERA 104
#define LIGHT 105
#define ROTATEOBJECTAROUNDX 201
#define ROTATEOBJECTAROUNDY 202
#define ROTATEOBJECTAROUNDZ 203
#define STOPROTATION 204
#define ROTATEORBITBUNNY 205
#define ROTATEORBITDRAGON 206
#define FORTH 301
#define BACK 302
#define UP 303
#define	DOWN 304
#define RIGHT 305
#define LEFT 306
#define LIGHTFORTH 307
#define LIGHTBACK 308
#define LIGHTUP 309
#define	LIGHTDOWN 310
#define LIGHTRIGHT 311
#define LIGHTLEFT 312
#define ZOOMIN 313
#define ZOOMOUT 314
#define UPCAMERA 315
#define DOWNCAMERA 316
#define RIGHTCAMERA 317
#define LEFTCAMERA 318
#define INCREASEINTENSITY 401
#define DECREASEINTENSITY 402
#define SHADING 403
#define STARTANIMATION 404
#define STOPANIMATION 405

/*texture okunduðunda dosyanýn deðerlerini tutmak için oluþturulan struct*/
typedef struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
}Image;

typedef Angel::vec4  point4;

/*pencereler*/
GLUI *glui;
int main_window;

/*Cisimler için kullanýlan program*/
GLuint program;
/*texture için kullanýlan program*/
GLuint program_surface;

GLuint vao[2];

/*Viewing transformation parameters*/
GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0f * DegreesToRadians;

GLfloat  fovy = 50.0f;/*Field-of-view in Y direction angle (in degrees)*/
GLfloat  aspect;/*Viewport aspect ratio*/
GLfloat  zNear = 0.5f, zFar = 100.0f;

/*bunny*/				   /*Declare the Mesh objects and variable for vertices*/
Mesh object1;
std::vector<vec3> object1Vertices;
std::vector<vec3> object1Normals;

/*suzanne*/
Mesh object2;
std::vector<vec3> object2Vertices;
std::vector<vec3> object2Normals;

/*Dragon*/
Mesh object3;
std::vector<vec3> object3Vertices;
std::vector<vec3> object3Normals;

/*Coordinates of bunny*/
GLfloat object1X = -0.2f;
GLfloat object1Y = 0.2f;
GLfloat object1Z = -1.5f;

/*Coordinates of suzanne*/
GLfloat object2X = 0.25f;
GLfloat object2Y = 0.25f;
GLfloat object2Z = -1.5f;

/*Coordinates of dragon*/
GLfloat object3X = -0.75f;
GLfloat object3Y = 0.15f;
GLfloat object3Z = -1.5f;

/*Coordinates of camera*/
GLfloat cameraX = -3.0f;
GLfloat cameraY = -2.0f;
GLfloat cameraZ = 0.0f;

/*Coordinates of light*/
GLfloat lightX = 0.0f;
GLfloat lightY = 1.0f;
GLfloat lightZ = 0.0f;

/*Translation matrix of bunny*/
mat4 translationObject1;
/*Translation matrix of suzanne*/
mat4 translationObject2;
/*Translation matrix of dragon*/
mat4 translationObject3;

/*Translation of camera*/
vec3 translateCamera;

/*Scale rates of objects*/
GLfloat scaleObject1 = 0.08f;
GLfloat scaleObject2 = 0.1f;
GLfloat scaleObject3 = 0.03f;

/*cisimlerin scale animasyonunun baþlatan ve bitiren deðiþken*/
GLfloat scaleDirectionObject1 = -1.0f;
GLfloat scaleDirectionObject2 = -1.0f;
GLfloat scaleDirectionObject3 = -1.0f;

/*obje seçimii*/
GLfloat *selectedScale = &scaleObject1;
GLfloat *selectedScaleDirection = &scaleDirectionObject1;


/*Array of rotation angles (in degrees) for each coordinate axis*/
GLuint thetaLoc;
GLuint thetaLocOrbit;
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };

int AxisObject1 = Yaxis;/*Rotation axis of bunny*/
GLfloat  thetaObject1[NumAxes] = { 0.0, 0.0, 0.0 };/*Theta for bunny*/
float speedOfObject1 = 0.0;	/*Speed of the bunny*/

int AxisObject2 = Yaxis;/*Rotation axis of suzanne*/
GLfloat  thetaObject2[NumAxes] = { 0.0, 0.0, 0.0 };/*Theta for suzanne*/
float speedOfObject2 = 0.0;	/*Speed of the suzanne*/

int AxisObject3 = Yaxis;/*Rotation axis of dragon*/
GLfloat  thetaObject3[NumAxes] = { 0.0, 0.0, 0.0 };/*Theta for dragon*/
float speedOfObject3 = 0.0;	/*Speed of the dragon*/


int OrbitAxisObject1 = Yaxis;/*orbital üzerindeki döngüþ axisi*/
GLfloat  OrbitThetaObject1[NumAxes] = { 0.0, 0.0, 0.0 };/*orbital tethasý*/
float OrbitspeedOfObject1 = 0.0;/*Orbital Speed of the bunny*/

int OrbitAxisObject2 = Yaxis;/*orbital üzerindeki döngüþ axisi*/
GLfloat  OrbitThetaObject2[NumAxes] = { 0.0, 0.0, 0.0 };/*orbital tethasý*/
float OrbitspeedOfObject2 = 0.0;/*Orbital Speed of the suzeane*/

int OrbitAxisObject3 = Yaxis;/*orbital üzerindeki döngüþ axisi*/
GLfloat  OrbitThetaObject3[NumAxes] = { 0.0, 0.0, 0.0 };/*orbital tethasý*/
float OrbitspeedOfObject3 = 0.0;/*Orbital Speed of the dragon*/

								/*Coordinates of the initial (bunny) selected object*/
GLfloat *selectedX = &object1X;
GLfloat *selectedY = &object1Y;
GLfloat *selectedZ = &object1Z;

GLint *selectedAxis = &AxisObject1;
GLfloat *selectedSpeed = &speedOfObject1;

// Initialize shader lighting parameters
vec4 light_position;
vec4 light_ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
vec4 light_diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
vec4 light_specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Initialize shader material parameters
vec4 material_ambient = vec4(1.0f, 1.0f, 1.0f, 1.0f);
vec4 material_diffuse = vec4(1.0f, 0.8f, 0.0f, 1.0f);
vec4 material_specular = vec4(1.0f, 0.0f, 1.0f, 1.0f);
float  material_shininess = 5.0;

/*kamera için gerekli deðiþkneler*/
point4  eye(0.0f, 0.0f, 2.0f, 1.0f);
point4  at(-1.0f, -1.0f, 0.0f, 1.0f);
vec4    up(0.0f, 1.0f, 0.0f, 0.0f);

mat4  mv = LookAt(eye, at, up);

vec4 ambient_product;
vec4 diffuse_product;
vec4 specular_product;

/*zein kordinat ve texture koordinatlarý*/
int Index;
point4  points[6];
vec2	tex_coords[6];
vec3	surface_normals[6];

GLuint textures;
Image *imagetex;

int shading_option = 0;/*shading deðiþtiremede kullanýlan deðiþken*/

float orbitTransBunny = 0.5; /*dönüþ ekseni*/
float orbitTransDragon = 1.0;   /*dönüþ ekseni*/

GLuint OrbitTrans;

//----------------------------------------------------------------------------
/*Zeminin vertex,texture koordinatlarýný yükleme fonksiyonu*/
void
draw_surface()
{
	points[Index] = point4(2.0f, -0.1f, -1.0f, 1.0f);
	tex_coords[Index] = vec2(0.0f, 0.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

	points[Index] = point4(-2.0f, -0.1f, -1.0f, 1.0f);
	tex_coords[Index] = vec2(1.0f, 0.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

	points[Index] = point4(-2.0f, -0.1f, -3.0f, 1.0f);
	tex_coords[Index] = vec2(1.0f, 1.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

	points[Index] = point4(2.0f, -0.1f, -1.0f, 1.0f);
	tex_coords[Index] = vec2(0.0f, 0.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

	points[Index] = point4(-2.0f, -0.1f, -3.0f, 1.0f);
	tex_coords[Index] = vec2(1.0f, 1.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

	points[Index] = point4(2.0f, -0.1f, -3.0f, 1.0f);
	tex_coords[Index] = vec2(0.0f, 1.0f);
	surface_normals[Index++] = vec3(0.0f, 0.0f, 1.0f);

}

/*Modellerin yüklendiði fonksiyon*/
static void Load3DModel()
{
	object1.LoadObjModel("bunny.smf");//load 3D object1 in smf formate
	object1Vertices = object1.returnVertices();//return the vertex data of mesh
	object1Normals = object1.returnNormals();//return the normal data of mesh

	object2.LoadObjModel("suzanne.smf");//load 3D object2 in smf formate
	object2Vertices = object2.returnVertices();//return the vertex data of mesh
	object2Normals = object2.returnNormals();//return the normal data of mesh

	object3.LoadObjModel("dragon.smf");//load 3D object3 in smf formate
	object3Vertices = object3.returnVertices();//return the vertex data of mesh
	object3Normals = object3.returnNormals();//return the normal data of mesh

	draw_surface();
}

//----------------------------------------------------------------------------
/*texture dosyasýnýn okunmasýnýn kontrolü*/
int ImageLoad(char *filename, Image *image) {

	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.
			   // make sure the file is there.

	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}

	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);

	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}

	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}

	//printf("Height of %s: %lu\n", filename, image->sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;

	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}

	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}

	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}

	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}

	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);

	// read the data.
	image->data = (char *)malloc(size);

	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}

	for (i = 0; i<size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}

	// we're done.
	return 1;
}
/*texture dosyasýnýn okunmasý*/
Image * loadTexture(char *filename) {

	Image *image1;
	// allocate space for texture
	image1 = (Image *)malloc(sizeof(Image));

	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}

	if (!ImageLoad(filename, image1)) {
		exit(1);
	}

	return image1;
}

//----------------------------------------------------------

/*texture buffernýn doldurma fonksiyonu*/
void
loadTextureBuffer() {

	imagetex = loadTexture("grass2.bmp");
	glGenTextures(1, &textures);

	glBindTexture(GL_TEXTURE_2D, textures);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagetex->sizeX, imagetex->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, imagetex->data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

/*shaderlara gönderilen deðiþkenler*/
void
loadUniform(GLuint tempProgram, mat4 mv, mat4 p, mat4 s, mat4 trans) {

	glUniform4fv(glGetUniformLocation(tempProgram, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(tempProgram, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(tempProgram, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(tempProgram, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(tempProgram, "Shininess"), material_shininess);

	// Retrieve transformation uniform variable locations
	glUniformMatrix4fv(glGetUniformLocation(tempProgram, "ModelView"), 1, GL_TRUE, mv);
	glUniformMatrix4fv(glGetUniformLocation(tempProgram, "Projection"), 1, GL_TRUE, p);
	glUniformMatrix4fv(glGetUniformLocation(tempProgram, "translate"), 1, GL_FALSE, trans);
	glUniformMatrix4fv(glGetUniformLocation(tempProgram, "scale"), 1, GL_TRUE, s);
	glUniform3fv(glGetUniformLocation(tempProgram, "translateCamera"), 1, translateCamera);


}

/*shading için programýn deðiþtirilmesi*/
void changeShading()
{
	if (shading_option == 0)
	{
		//Load shaders and use the resulting shader program
		program = InitShader("vshader2.glsl", "fshader2.glsl");
		shading_option = 1;
		glClearColor(1.0f, 0.713725f, 0.756863f, 1.0f);
	}
	else {
		//Load shaders and use the resulting shader program
		program = InitShader("vshader.glsl", "fshader.glsl");
		shading_option = 0;
		glClearColor(0.678431f, 0.847059f, 0.901961f, 1.0f);
	}


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

}

//OpenGL initialization
void init()
{
	loadTextureBuffer();
	GLuint buffer;

	//Create a vertex array object
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);

	//Create and initialize a buffer object
	glGenBuffers(1, &buffer);//generate VBO names
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//bind a specific VBO

	glBufferData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3) + object1Normals.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3) + object2Normals.size() * sizeof(vec3) + object3Vertices.size() * sizeof(vec3) + object3Normals.size() * sizeof(vec3), NULL, GL_STATIC_DRAW);//load data into VBO
																																																																									/*Vertices*/
	glBufferSubData(GL_ARRAY_BUFFER, 0, object1Vertices.size() * sizeof(vec3), object1Vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3), object2Vertices.size() * sizeof(vec3), object2Vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3), object3Vertices.size() * sizeof(vec3), object3Vertices.data());

	/*Normals*/
	glBufferSubData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3) + object3Vertices.size() * sizeof(vec3), object1Normals.size() * sizeof(vec3), object1Normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3) + object3Vertices.size() * sizeof(vec3) + object1Normals.size() * sizeof(vec3), object2Normals.size() * sizeof(vec3), object2Normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, object1Vertices.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3) + object3Vertices.size() * sizeof(vec3) + object1Normals.size() * sizeof(vec3) + object2Normals.size() * sizeof(vec3), object3Normals.size() * sizeof(vec3), object3Normals.data());

	//Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	//set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	// connect the xyz to the "vPosition" attribute of the vertex shader
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// connect the xyz to the "vNormal" attribute of the vertex shader
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(object1Vertices.size() * sizeof(vec3) + object2Vertices.size() * sizeof(vec3) + object3Vertices.size() * sizeof(vec3)));

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	thetaLoc = glGetUniformLocation(program, "theta");


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures);

	program_surface = InitShader("vshader71.glsl", "fshader71.glsl");
	glUseProgram(program_surface);

	glBindVertexArray(vao[1]);
	//Create and initialize a buffer object
	glGenBuffers(1, &buffer);//generate VBO names
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//bind a specific VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(surface_normals) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);//load data into VBO
																													   /*Vertices*/
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(surface_normals), surface_normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(surface_normals), sizeof(tex_coords), tex_coords);

	GLuint vPosition1 = glGetAttribLocation(program_surface, "vPosition");
	// connect the xyz to the "vPosition" attribute of the vertex shader
	glEnableVertexAttribArray(vPosition1);
	glVertexAttribPointer(vPosition1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// connect the xyz to the "vNormal" attribute of the vertex shader
	GLuint vNormal1 = glGetAttribLocation(program_surface, "vNormal");
	glEnableVertexAttribArray(vNormal1);
	glVertexAttribPointer(vNormal1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint vTexCoord = glGetAttribLocation(program_surface, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points) + sizeof(surface_normals)));

	glUniform1i(glGetUniformLocation(program_surface, "texture"), 0);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.678431f, 0.847059f, 0.901961f, 1.0f);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (fovy < 0)
		fovy = 0;

	if (fovy > 90)
		fovy = 90;

	mat4  p = Perspective(fovy, aspect, zNear, zFar);
	light_position = vec4(lightX, lightY, lightZ, 0.0);

	glUseProgram(program);
	thetaLocOrbit = glGetUniformLocation(program, "thetaOrbit");
	OrbitTrans = glGetUniformLocation(program, "orbitTrans");
	glBindVertexArray(vao[0]);

	/*Object1*/
	glUniform3fv(thetaLoc, 1, thetaObject1);
	glUniform3fv(thetaLocOrbit, 1, OrbitThetaObject1);
	glUniform1f(OrbitTrans, orbitTransBunny);
	mat4 s = mat4(scaleObject1, 0.0, 0.0, 0.0, 0.0, scaleObject1, 0.0, 0.0, 0.0, 0.0, scaleObject1, 0.0, 0.0, 0.0, 0.0, 1.0);
	loadUniform(program, mv, p, s, translationObject1);
	glDrawArrays(GL_TRIANGLES, 0, object1Vertices.size());

	/*Object2*/
	glUniform3fv(thetaLoc, 1, thetaObject2);
	glUniform3fv(thetaLocOrbit, 1, OrbitThetaObject2);
	glUniform1f(OrbitTrans, 1.0);
	s = mat4(scaleObject2, 0.0, 0.0, 0.0, 0.0, scaleObject2, 0.0, 0.0, 0.0, 0.0, scaleObject2, 0.0, 0.0, 0.0, 0.0, 1.0);
	loadUniform(program, mv, p, s, translationObject2);
	glDrawArrays(GL_TRIANGLES, object1Vertices.size(), object2Vertices.size());

	/*Object3*/
	glUniform3fv(thetaLoc, 1, thetaObject3);
	glUniform3fv(thetaLocOrbit, 1, OrbitThetaObject3);
	glUniform1f(OrbitTrans, orbitTransDragon);
	s = mat4(scaleObject3, 0.0, 0.0, 0.0, 0.0, scaleObject3, 0.0, 0.0, 0.0, 0.0, scaleObject3, 0.0, 0.0, 0.0, 0.0, 1.0);
	loadUniform(program, mv, p, s, translationObject3);
	glDrawArrays(GL_TRIANGLES, object1Vertices.size() + object2Vertices.size(), object3Vertices.size());

	/*Surface*/
	glUseProgram(program_surface);
	glBindVertexArray(vao[1]);
	mat4 t = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	s = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	loadUniform(program_surface, mv, p, s, t);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	aspect = GLfloat(width) / height;

	glutPostRedisplay();

}

void speedUp(GLfloat *theta, float speed, int axis)
{
	theta[axis] += speed;

	if (theta[axis] > 360.0) {
		theta[axis] -= 360.0;
	}

}

void scale(GLfloat *sSD, GLfloat *scale, double max, double min, double speed)
{
	/*Grow and shrink*/
	if (*sSD == 1.0)
	{
		*scale += speed;				/*increment the scale value*/

		if (*scale >= max)
		{
			*sSD = 0.0;					/*Start shrinking*/
		}
		else if (*scale <= min)
		{
			*sSD = 1.0;					/*Start growing*/
		}
	}
	else if (*sSD == 0.0)
	{
		*scale -= speed;				/*decrement the scale value*/

		if (*scale >= max)
		{
			*sSD = 0.0;					/*Start shrinking*/
		}
		else if (*scale <= min)
		{
			*sSD = 1.0;					/*Start growing*/
		}
	}
}

void idle(void)
{
	/*dönüþ hýzlarýnýn kontrolü*/
	speedUp(thetaObject1, speedOfObject1, AxisObject1);
	speedUp(thetaObject2, speedOfObject2, AxisObject2);
	speedUp(thetaObject3, speedOfObject3, AxisObject3);
	speedUp(OrbitThetaObject1, OrbitspeedOfObject1, OrbitAxisObject1);
	speedUp(OrbitThetaObject3, OrbitspeedOfObject3, OrbitAxisObject3);

	/*scale iþleminin kontrolü*/
	scale(&scaleDirectionObject1, &scaleObject1, 0.12, 0.04, 0.0020);
	scale(&scaleDirectionObject2, &scaleObject2, 0.15, 0.05, 0.0020);
	scale(&scaleDirectionObject3, &scaleObject3, 0.045, 0.015, 0.0005);

	/*obje tranferleri*/
	translationObject1 = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(object1X, object1Y, object1Z, 1.0));
	translationObject2 = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(object2X, object2Y, object2Z, 1.0));
	translationObject3 = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(object3X, object3Y, object3Z, 1.0));
	translateCamera = vec3(cameraX, cameraY, cameraZ);
	diffuse_product = light_diffuse * material_diffuse;

	glutSetWindow(main_window);
	glutPostRedisplay();
}

void myGlutReshape(int width, int height)
{
	aspect = GLfloat(width) / height;
	GLUI_Master.auto_set_viewport();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-aspect*.08, aspect*.08, -.08, .08, .1, 15.0);

	glutPostRedisplay();
}

void control_cb(int control)
{
	switch (control) {
	case OBJECT1:selectedX = &object1X; selectedY = &object1Y; selectedZ = &object1Z; selectedAxis = &AxisObject1; selectedSpeed = &speedOfObject1; selectedScale = &scaleObject1; selectedScaleDirection = &scaleDirectionObject1; break;
	case OBJECT2:selectedX = &object2X; selectedY = &object2Y; selectedZ = &object2Z; selectedAxis = &AxisObject2; selectedSpeed = &speedOfObject2; selectedScale = &scaleObject2; selectedScaleDirection = &scaleDirectionObject2; break;
	case OBJECT3:selectedX = &object3X; selectedY = &object3Y; selectedZ = &object3Z; selectedAxis = &AxisObject3; selectedSpeed = &speedOfObject3; selectedScale = &scaleObject3; selectedScaleDirection = &scaleDirectionObject3; break;
	case LIGHT:selectedX = &lightX; selectedY = &lightY; selectedZ = &lightZ; break;
	case ROTATEOBJECTAROUNDX: *selectedAxis = Xaxis; *selectedSpeed = 2.0; break;
	case ROTATEOBJECTAROUNDY: *selectedAxis = Yaxis; *selectedSpeed = 2.0; break;
	case ROTATEOBJECTAROUNDZ: *selectedAxis = Zaxis; *selectedSpeed = 2.0; break;
	case ROTATEORBITBUNNY: OrbitspeedOfObject1 = 0.5; break;
	case ROTATEORBITDRAGON: OrbitspeedOfObject3 = 0.5; break;
	case STOPROTATION: *selectedSpeed = 0.0; break;
	case FORTH:*selectedZ += 0.05f; break;
	case BACK:*selectedZ -= 0.05f; break;
	case UP:*selectedY += 0.05f; break;
	case DOWN:*selectedY -= 0.05f; break;
	case RIGHT:*selectedX += 0.05f; break;
	case LEFT:*selectedX -= 0.05f; break;
	case UPCAMERA:cameraY -= 0.05f; break;
	case DOWNCAMERA:cameraY += 0.05f; break;
	case RIGHTCAMERA:cameraX -= 0.05f; break;
	case LEFTCAMERA:cameraX += 0.05f; break;
	case LIGHTFORTH:*selectedZ += 2.0f; break;
	case LIGHTBACK:*selectedZ -= 2.0f; break;
	case LIGHTUP:*selectedY += 2.0f; break;
	case LIGHTDOWN:*selectedY -= 2.0f; break;
	case LIGHTRIGHT:*selectedX += 2.0f; break;
	case LIGHTLEFT:*selectedX -= 2.0f; break;
	case ZOOMIN: fovy -= 1; break;
	case ZOOMOUT: fovy += 1; break;
	case INCREASEINTENSITY: light_diffuse.x += 0.25; light_diffuse.y += 0.25; light_diffuse.z += 0.25; break;
	case DECREASEINTENSITY: light_diffuse.x -= 0.25; light_diffuse.y -= 0.25; light_diffuse.z -= 0.25; break;
	case SHADING: changeShading(); break;
	case STARTANIMATION: *selectedScaleDirection = 1.0f; break;
	case STOPANIMATION: *selectedScaleDirection = -1.0f; break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//Specify window properties
	glutInitWindowSize(1080, 720);

	main_window = glutCreateWindow("result");//Create window with title "result"	
	glewExperimental = GL_TRUE;
	glewInit();
	Load3DModel();
	//Set OpenGL state and shader variables
	init();

	//Display callback 
	glutDisplayFunc(display);
	glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

	/*Obje rotota iþlemleri*/
	GLUI_Rollout *roll_object = new GLUI_Rollout(glui, "OBJECTS", false);
	glui->add_button_to_panel(roll_object, "SELECT BUNNY", OBJECT1, control_cb);
	glui->add_button_to_panel(roll_object, "SELECT SUZANNE", OBJECT2, control_cb);
	glui->add_button_to_panel(roll_object, "SELECT DRAGON", OBJECT3, control_cb);
	glui->add_button_to_panel(roll_object, "ROTATE AROUND X", ROTATEOBJECTAROUNDX, control_cb);
	glui->add_button_to_panel(roll_object, "ROTATE AROUND Y", ROTATEOBJECTAROUNDY, control_cb);
	glui->add_button_to_panel(roll_object, "ROTATE AROUND Z", ROTATEOBJECTAROUNDZ, control_cb);
	glui->add_button_to_panel(roll_object, "STOP ROTATION", STOPROTATION, control_cb);
	glui->add_button_to_panel(roll_object, "UP", UP, control_cb);
	glui->add_button_to_panel(roll_object, "DOWN", DOWN, control_cb);
	glui->add_button_to_panel(roll_object, "RIGHT", RIGHT, control_cb);
	glui->add_button_to_panel(roll_object, "LEFT", LEFT, control_cb);
	glui->add_button_to_panel(roll_object, "BACK", BACK, control_cb);
	glui->add_button_to_panel(roll_object, "FORTH", FORTH, control_cb);

	/*Kamera konutlarý*/
	GLUI_Rollout *roll_camera = new GLUI_Rollout(glui, "CAMERA", false);
	glui->add_button_to_panel(roll_camera, "CAMERA UP", UPCAMERA, control_cb);
	glui->add_button_to_panel(roll_camera, "CAMERA DOWN", DOWNCAMERA, control_cb);
	glui->add_button_to_panel(roll_camera, "CAMERA LEFT", LEFTCAMERA, control_cb);
	glui->add_button_to_panel(roll_camera, "CAMERA RIGHT", RIGHTCAMERA, control_cb);
	glui->add_button_to_panel(roll_camera, "ZOOM IN", ZOOMIN, control_cb);
	glui->add_button_to_panel(roll_camera, "ZOOM OUT", ZOOMOUT, control_cb);
	glui->add_rotation_to_panel(roll_camera, "ROTATE CAMERA", &mv[0][0]);

	/*Shading deðiþikliði*/
	GLUI_Rollout *roll_shading = new GLUI_Rollout(glui, "SHADING", false);
	glui->add_button_to_panel(roll_shading, "CHANGE SHADING", SHADING, control_cb);

	/*Iþýk komutlarý*/
	GLUI_Rollout *roll_light = new GLUI_Rollout(glui, "LIGHT", false);
	glui->add_button_to_panel(roll_light, "SELECT LIGHT", LIGHT, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT UP", LIGHTUP, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT DOWN", LIGHTDOWN, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT LEFT", LIGHTLEFT, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT RIGHT", LIGHTRIGHT, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT BACK", LIGHTBACK, control_cb);
	glui->add_button_to_panel(roll_light, "LIGHT FORTH", LIGHTFORTH, control_cb);
	glui->add_button_to_panel(roll_light, "INCREASE LIGHT INTENSITY", INCREASEINTENSITY, control_cb);
	glui->add_button_to_panel(roll_light, "DECREASE LIGHT INTENSITY", DECREASEINTENSITY, control_cb);

	/*animasyon komutlarý*/
	GLUI_Rollout *roll_animation = new GLUI_Rollout(glui, "ANIMATION", true);
	glui->add_button_to_panel(roll_animation, "START BEATING HEART ANIMATION", STARTANIMATION, control_cb);
	glui->add_button_to_panel(roll_animation, "STOP BEATING HEART ANIMATION", STOPANIMATION, control_cb);
	GLUI_Spinner *spinner1 = glui->add_spinner_to_panel(roll_animation, "Orbit Rotate Bunny:", GLUI_SPINNER_FLOAT, &OrbitspeedOfObject1);
	spinner1->set_int_limits(-5, 5);
	GLUI_Spinner *spinner2 = glui->add_spinner_to_panel(roll_animation, "Orbit Rotate Dragon:", GLUI_SPINNER_FLOAT, &OrbitspeedOfObject3);
	spinner2->set_int_limits(-5, 5);

	//glutReshapeFunc(reshape);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);

	glui->set_main_gfx_window(main_window);

	//Enter event loop
	glutMainLoop();
	return 0;
}

