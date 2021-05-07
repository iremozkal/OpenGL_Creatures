#version 150 
uniform mat4 ModelView, Projection; 
uniform vec4 LightPosition; 
in vec4 vPosition, vNormal; 
out vec3 normal, lightDir;
uniform mat4 scale;
uniform mat4 translate;
uniform vec3 theta;
uniform vec3 translateCamera;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform vec3 thetaOrbit;
uniform float orbitTrans;

void main() {
 
 /*Compute the sines and cosines of theta for each of the three axes in one computation.*/ 
vec3 angles = radians(theta); 
vec3 c = cos( angles ); 
vec3 s = sin( angles ); 
mat4 rx = mat4( 1.0, 0.0, 0.0, 0.0, 0.0, c.x, s.x, 0.0, 0.0, -s.x, c.x, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
mat4 ry = mat4( c.y, 0.0, -s.y, 0.0, 0.0, 1.0, 0.0, 0.0, s.y, 0.0, c.y, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
ry[1][0] = 0.0; 
ry[1][1] = 1.0; 
mat4 rz = mat4( c.z, -s.z, 0.0, 0.0, s.z, c.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
rz[2][2] = 1.0;

		vec3 anglesOrbit = radians(thetaOrbit); 
	    vec3 cOrbit = cos( anglesOrbit ); 
	    vec3 sOrbit = sin( anglesOrbit ); 
		  mat4 rxOrbit = mat4( 1.0, 0.0, 0.0, 0.0, 0.0, cOrbit.x, sOrbit.x, 0.0, 0.0, -sOrbit.x, cOrbit.x, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
	    mat4 ryOrbit = mat4( cOrbit.y, 0.0, -sOrbit.y, 0.0, 0.0, 1.0, 0.0, 0.0, sOrbit.y, 0.0, cOrbit.y, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
	    ry[1][0] = 0.0; 
	    ry[1][1] = 1.0; 
	    mat4 rzOrbit = mat4( cOrbit.z, -sOrbit.z, 0.0, 0.0, sOrbit.z, cOrbit.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 ); 
	    rz[2][2] = 1.0;

		mat4 inverseTranslate =  mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(orbitTrans, 0.0, 0.0, 1.0)
    );

	   mat4 originTranslate =  mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(-orbitTrans, 0.0, 0.0, 1.0)
    ); 

mat4 NormalMatrix = transpose(inverse(translate * inverseTranslate *  ryOrbit * originTranslate   * rz * ry * rx * scale * ModelView));
vec4 p = translate * inverseTranslate *  ryOrbit * originTranslate  * rz * ry * rx * scale * vPosition;

lightDir = normalize(LightPosition.xyz); 
normal = normalize((NormalMatrix*vNormal).xyz);
gl_Position = (Projection*ModelView*p) +  vec4(translateCamera,0.0);
}
