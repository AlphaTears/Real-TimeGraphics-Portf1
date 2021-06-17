#version 330

uniform sampler2D texture;

in vec2 texCoord;

layout (location=0) out vec4 fragColour;

void main(void) {

	vec4 texColor = texture2D(texture, texCoord);
	fragColour = texColor;

    //fragColour = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    
}
//This class was included within the example project and nothing was changed inside of it.