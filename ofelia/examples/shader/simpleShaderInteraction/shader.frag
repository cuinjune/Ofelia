#version 120

uniform vec4 mouseColor[1];

void main()
{
    gl_FragColor = mouseColor[0] / 255.0f;
}
