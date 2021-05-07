#version 150 
in vec4 vPosition; 
in vec4 vNormal;
out vec4 color; 
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;
uniform mat4 Projection;
uniform mat4 scale;
uniform mat4 translate;
uniform vec3 theta;
uniform vec3 translateCamera;
uniform vec3 thetaOrbit;
uniform float orbitTrans;


void main() 
	{
	 
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
		mat4 NormalMatrix = transpose(inverse(translate * inverseTranslate *  ryOrbit * originTranslate * rz * ry * rx * scale * ModelView));
		vec4 p =   translate * inverseTranslate *  ryOrbit * originTranslate * rz * ry * rx * scale * vPosition;
			
		// Transform vertex  position into eye coordinates 
		vec3 pos = (ModelView * p).xyz;
        vec3 L = normalize( (LightPosition).xyz - pos ); 
		vec3 V = normalize( -pos ); 
		vec3 H = normalize( L + V );
        // Transform vertex normal into eye coordinates 
	    vec3 N = normalize(NormalMatrix*vNormal).xyz;
	    // Compute terms in the illumination equation 
		vec4 ambient = AmbientProduct;
		// Compute terms in the illumination equation vec4 ambient = AmbientProduct;
        float Kd = max( dot(L, N), 0.0 ); 
		vec4  diffuse = Kd*DiffuseProduct;
        float Ks = pow( max(dot(N, H), 0.0), Shininess ); 
		vec4  specular = Ks * SpecularProduct; 
		// if the light is behind the object, again we correct the 
		// specular contribution 
		if( dot(L, N) < 0.0 ) 
			specular = vec4(0.0, 0.0, 0.0, 1.0);  

		
			
			gl_Position =  (Projection * ModelView * p) + vec4(translateCamera,0.0);			
			color = ambient + diffuse + specular;					
		
	} 