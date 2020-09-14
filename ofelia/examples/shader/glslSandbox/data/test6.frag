// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

const float PI = 3.1415926535897932384626433832795;

vec4 white  = vec4(1.0,1.0,1.0,1.0);
vec4 black  = vec4(0.0,0.0,0.0,1.0);
vec4 yello  = vec4(0.839, 0.917, 0.137, 1.0);

float thickness = 0.05;
vec2  mv0 = vec2(-.5);
vec2  c0  = vec2(0.0, 0.0);
float r0  = 0.05;
float bl  = 0.003;
vec2  v   = vec2(0.0, 0.0);
vec2  w   = v + vec2(0.5, -1.4);

float minimum_distance(vec2 v, vec2 w, vec2 p) {
  // Return minimum distance between line segment vw and point p
  float l2 = length(w - v);  // i.e. |w-v|^2 -  avoid a sqrt
  l2 *= l2;
	
  if (l2 == 0.0) return distance(p, v);   // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  // We clamp t from [0,1] to handle points outside the segment vw.
  float t = max(0., min(1., dot(p - v, w - v) / l2));
  vec2 projection = v + t * (w - v);  // Projection falls on the segment
  return distance(p, projection);
}

float segment(vec2 v, vec2 w, float thickness, vec2 p){
 float r = minimum_distance(v, w, p);
 return mix(0.0, 1.0, smoothstep(0.5 * thickness, 1.5 * thickness, r));
}

float circle(vec2 p, vec2 c, float r, float b, float bl){
 float 	t = abs(distance(p,c));
        t = smoothstep(r - b, r - b + bl, t) + smoothstep(r - bl, r, t);
 return t;
}

float disk(vec2 p, vec2 c, float r, float bl){
 float 	t = abs(distance(p,c));
        t = smoothstep(r, r + bl, t);
 return t;
}

const float accu = 1./256.;
const float pxbubble = 32.;
uniform sampler2D prevframe;
#define T2(S) texture2D(prevframe, S*gl_FragCoord.xy/resolution)

void main(){
    vec2 p  = gl_FragCoord.xy/resolution;
    vec2 p0 = p + mv0;
	
	
	vec2 m0 = mouse-.5;
	float t_value = 0.;
	
	t_value = atan(m0.x, m0.y)+.6;
	
	vec4 c_0 = T2(0.);
	vec4 c_1 = T2(1.);
	
	t_value += 0.5*time*c_0.z/accu;
	
	if(distance(vec2(0), gl_FragCoord.xy) <= pxbubble){
		float y_value = mod(t_value, 3.141*2.)/(2.*3.14159);
		float z_value = c_0.z;
		if(y_value > .5 + c_0.y){
			z_value -= accu;
		}
		if(y_value < -.5 + c_0.y){
			z_value += accu;
		}
		c_0 = vec4(1,y_value,z_value,1);
		gl_FragColor = c_0;
		return;
	}
	if(distance(resolution, gl_FragCoord.xy) <= pxbubble){
		c_1 = vec4(0,0,1,1);
		gl_FragColor = c_1;
		return;
	}
	
	
	
    float r = length(p0) * 2.0;
    float a = atan(p0.y,p0.x);
	
	a += t_value;

    float f = cos(a * 3.0);
    float t0 = smoothstep(f - 0.01, f + 0.01, r);
    float t1 = segment(v, w, 0.01, p0);
    float t2 = disk(p0,c0,r0,bl);
	
    vec3 back = mix(vec3(0.0, 0.0, 0.5), vec3(0.0, 0.5, 1.0), p.y * 0.5 + 0.5);
    vec4 color = vec4(mix(back, vec3(black), t0), 1.0);
	 color = vec4(mix(vec3(white), vec3(color), t1), 1.0);	
	 color = vec4(mix(vec3(yello), vec3(color), t2), 1.0); 
    gl_FragColor = color;
}