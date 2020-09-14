#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable
// tweaked by psyreco

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

vec3 hsv(float h, float s, float v){
    vec4 t = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(vec3(h+ time * 0.5) + t.xyz) * 6.0 - vec3(t.w));
    return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}
void main( void ) {
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x,resolution.y);
    p*=1.1;
     vec2 q;
     
    vec2 l=vec2(1.,3
	       
	       
	       
	       
	       
	       
	       
	       
	       ); 
    q.y = log2(length(sin(p)));
    q.x = dot(p,l)/(length(p)*length(l));
    float u =sin(log2(fract(atan(-sin(p.y), sin(p.x)) ) * 1.0) * 0.1);
   
    vec3 line = vec3(0.0);
        vec3  color = hsv(0.8, 1.0, 1.);
        float thickness=0.008;
         line += thickness / abs(0.1+u-mod(q.y+ time * 0.5,.4)) * color;
	line -= thickness / abs(0.2-mod(sin(q.y)- time * 0.25,.2)) * color;
         line += 1.-floor(q.y);
         line *= thickness*10. / abs(sin((atan(p.y, p.x)+time*0.5 ) * 20.)+q.y  )* color;
         

        gl_FragColor = vec4(line, 1.0);


}