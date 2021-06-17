#version 330

uniform mat4 mvpMatrix;

uniform vec3 L[2];
uniform vec3 Lk[2];
uniform vec3 Lcolour[2];
uniform vec3 Ldirect[2];
uniform float Ldisp[2];

uniform int Ltype[2];


layout (location=0) in vec3 vertexPos;
layout (location=1) in vec4 vertexTexCoord;
layout (location=6) in vec3 mNormal;

out vec3 texCoord;
out vec3 brightness;


vec3 calcLightContributionForPointLight(vec3, VertexPos, vec3 mNormal, vec3 L, Vec3 Lcolour, vec3 Lk)
{

//distance vector
vec3 D = L - vertexPos;

//GLSL's Length function
float lengthD = length(D);


//normalise Distance Vector
vec3 D_  = normalize(D);


//calculate for Attentuation
float A = 1.0 / (Lk.x + Lk.y * lengthD + lenghtD + Lk.z * lengthD * lengthD);


//Lambertian
float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);
return lambertian * A * Lcolour;
}
//////////////////////////////////////////////////////////////

vec 3 calcLightContributionForDirectionalLight(vec3 vertexPos, vec3 L, vec3 Lcolour, vec3 Ldirect)
{
	
	//distance vector
	//Ldirect = L - vertexPos;

	//GLSL's length function
	//float lengthD = length(D);

	//normalise distance vector
	vec3 D_ = normalize(Ldirect);

	// calculate for attentuation
	//float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Lambertian
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	//float acos = dot(-D_, Ldirect);

	return lambertian * Lcolour;


}

vec3 calcLightContributionForSpotLight(vec3 vertexPos, vec3 mNormal vec3 L, vec3 Lcolour, vec3 Lk, vec3 Ldirect, float Ldisp)
{
	//distance vector
	vec3 D = L - VertexPos;

	//GLSL's length function
	float lengthD = length(D);

	//normalise distance vector
	vec3 D_ = normalize(D);

	// calculate for attentuation
	float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Lambertian
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	float acos = acos(dot(-D_, normalize(Ldirect)));
	if (acos > Ldisp) {
		
		A = 0.0;
	}

	return acos * lambertian * A * Lcolour;

}






void main(void) 
{


		//Here i add new lights and edit their vaules like drop off values
	//vec3 L[3];
	//L[0] = vec3(10.0, 15.0, 0.0); // Light source position 0		//x,y,z		x ->		y (up)			z (deep)
	//L[1] = vec3(0.0, 100.0, 0.0); // Light source position 1		//		<- -x		   -y(down)		   -z (toward you)
	//L[2] = vec3(0.0, -100.0, 0.0); // Light source position 1		//		<- -x		   -y(down)		   -z (toward you)


	//vec3 Lk[3];
	//Lk[0] = vec3(1.0f, 0.1, 0.0); // attenuation properties for light 0		//start brightness,  drop off value,   lowest value
	//Lk[1] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 1
	//Lk[2] = vec3(1.0f, 0.0, 0.0); // attenuation properties for light 1

	//vec3 Lcolour[3];
	//Lcolour[0] = vec3(1.0f, 0.0f, 0.0f);	//r,g,b
	//Lcolour[1] = vec3(1.0f, 1.0f, 1.0f);	//white
	//Lcolour[2] = vec3(1.0f, 1.0f, 1.0f);	//white
	/////////////////////////////////////////////////////////////////////////////////////////


	// Calculate brightness
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
