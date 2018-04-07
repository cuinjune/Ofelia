#version 120

uniform vec2 windowDimen;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
	float r = gl_FragCoord.x / windowDimen.x;
	float g = gl_FragCoord.y / windowDimen.y;
	float b = 1.0;
	float a = 1.0;
	gl_FragColor = vec4(r, g, b, a);
}
