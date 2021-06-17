#version 330

uniform mat4 mvpMatrix;
// creating the different veriables for the light such as colour, directions, dispolay.
//Lights need to be vect
uniform vec3 L[3];
uniform vec3 Lk[3];
uniform vec3 Lcolour[3];
uniform vec3 Ldirect[2];
uniform float Ldisp[2];

uniform int Ltype[4];
// vec3s used for mesh rendering which are used in the VBOMaker;
layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexTexCoord;
layout (location=6) in vec3 mNormal;

//used to calculate how the light interiactes with the world/ how bright it is
out vec3 texCoord;
out vec3 brightness;


//Making the calculations for a point light in the scene when used, which in my case i chose not to use it
vec3 calcLightContributionForPointLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Lk)
{

	//Calculation for the distance vector
	vec3 D = L - vertexPos;

	//Getting the distance using the created vector D
	float lengthD = length(D);

	//Calculations for a normalised Vector D
	vec3 D_ = normalize(D);

	// Calculations for attenuation
	float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//calculating the lambertian using the DOT function
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	return lambertian * A * Lcolour;
}


//This is the same however its calculations for a directional light.
vec3 calcLightContributionForDirectionalLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Ldirect )
{

	
	//normalise distance vector
	vec3 D_ = normalize(Ldirect);

	

	//again calculation of the lambertian using the DOT function
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	

	return lambertian * Lcolour;
}


//Calculations for a spotlight
vec3 calcLightContributionForSpotLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Lk, vec3 Ldirect, float Ldisp ){

	//Creating the D vector or Distance vector
	vec3 D = L - vertexPos;

	//Calculating the length of the vector D
	float lengthD = length(D);

	//normalising the vector D
	vec3 D_ = normalize(D);

	//calculation for attenuation 
	float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Calculation of Lambertian using the DOT function for a spotlight
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	float acos = acos(dot(-D_, normalize(Ldirect)));
	if (acos > Ldisp) {
		
		A = 0.0;
	}

	return acos * lambertian * A * Lcolour;
}



//////////////////////////////////////
void main(void) {

 	//Create Simple lights in our scene 
	vec3 L[3];
	L[0] = vec3(0.0, 100.0, 0.0); // Light source position 0		//x,y,z		x ->		y (up)			z (deep)
	L[1] = vec3(0.0, 0.0, 100.0); // Light source position 1		//		<- -x		   -y(down)		   -z (toward you)
	L[2] = vec3(100.0, 0.0, 0.0); // Light source position 2		//		<- -x		   -y(down)		   -z (toward you)
	//L[3] = vec3(-200.0, 0.0, 0.0); // Light source position 3		//		<- -x		   -y(down)		   -z (toward you)

	//in here we are giving properties to our lights
	vec3 Lk[3];
	Lk[0] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 0		//start brightness,  drop off value,   lowest value
	Lk[1] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 1
	Lk[2] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 2
	//Lk[3] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 3

	//setting the colour values for each on of our lights which come from RGB colours. 
	vec3 Lcolour[3];
	Lcolour[0] = vec3(1.0f, 1.0f, 1.0f);	//white
	Lcolour[1] = vec3(1.0f, 1.0f, 1.0f);	//white
	Lcolour[2] = vec3(1.0f, 1.0f, 1.0f);	//white
	//Lcolour[3] = vec3(1.0f, 1.0f, 1.0f);	//white
	/////////////////////////////////////////////////////////////////////////////////////////


	// Calculating brightness for each type of light in our scene, not used.
	brightness = vec3(0.0f, 0.0f, 0.0f);		

	for (int i=0; i < 2; ++i) {

		if (Ltype[i]==0) {
			brightness += calcLightContributionForPointLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i]);
			//brightness += vec3(0.0, 0.0, 1.0);//calcLightContributionForPointLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i]);
		}
		else if (Ltype[i]==1) {
			brightness += calcLightContributionForDirectionalLight(vertexPos, mNormal, L[i], Lcolour[i], Ldirect[i]);
			//brightness += vec3(0.0, 1.0, 0.0);//calcLightContributionForDirectionalLight(vertexPos, mNormal, L[i], Lcolour[i], Ldirect[i]);
		}
		else if (Ltype[i]==2) {
			brightness += calcLightContributionForSpotLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i], Ldirect[i], Ldisp[i]);
		}
	}
		
		gl_Position = mvpMatrix * vec4(vertexPos, 1.0);
		
		texCoord = vertexTexCoord;
	
}
