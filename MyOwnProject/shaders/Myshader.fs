#version 330

uniform sampler2D texturel


in vec3 texCoord;
in vec3 brightness;

layout (location=0) out vec4 fragColour;

void main(void) 
{


    fragColour = texture2D(texture,texCoord.xy) * vec4(brightness, 1.0);
}
