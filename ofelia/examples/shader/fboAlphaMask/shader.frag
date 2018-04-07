#version 120

uniform sampler2D tex0;
uniform sampler2D maskTex;

varying vec2 texCoordVarying;

void main()
{
    // Get color value from
    vec3 src = texture2D(tex0, texCoordVarying).rgb;

    // Get alpha value
    float mask = texture2D(maskTex, texCoordVarying).r;

    // Set
    gl_FragColor = vec4(src , mask);
}
