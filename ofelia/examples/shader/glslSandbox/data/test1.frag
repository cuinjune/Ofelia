#ifdef GL_ES
precision mediump float;
#endif

//kanatacrude - bit less seasickness from camera movement
	
uniform float time;
uniform vec2 resolution;

float t = 0.0;
float t2 = 0.0;

float map(vec3 p)
{
    const int MAX_ITER = 10;
    const float BAILOUT=4.0;
    float Power=6.0;

    vec3 v = p;
    vec3 c = v;

    float r=0.0;
    float d=1.0;
    for(int n=0; n<=MAX_ITER; ++n)
    {
        r = length(v);
        if(r>BAILOUT) break;

        float theta = acos(v.z/r) + sin(t);
        float phi = atan(v.y, v.x);
        d = pow(r,Power-1.2)*Power*d+1.1 + sin(t);

        float zr = pow(r,Power);
        theta = theta*Power;
        phi = phi*Power;
        v = (vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta))*zr)+c;
    }
    return 0.5*log(r)*r/d;
}


void main( void )
{
    t = time * 0.1;//1.1;
    t2 = time * 0.3;

    vec2 pos = (gl_FragCoord.xy*2.0 - resolution.xy) / resolution.y;
    vec3 camPos = vec3(cos(t2*0.3+t2), sin(t2*0.3-t2), 2.-sin(t2*.2)/2.);
    vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    vec3 camUp  = normalize(vec3(0.0, 1.0, 0.8));
    vec3 camSide = cross(camDir, camUp);
    float focus = 1.8;

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir*focus);
    vec3 ray = camPos;
    float m = 0.0;
    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 3000;
    const float MAX_DISTANCE = 20009.0;
    for(int i=0; i<MAX_MARCH; ++i) {
        d = map(ray);
        total_d += d;
        ray += rayDir * d;
        m += 1.0;
        if(d<0.001) { break; }
        if(total_d>MAX_DISTANCE) { total_d=MAX_DISTANCE; break; }
    }

    float c = (total_d)*0.0001;
    vec4 result = vec4( 1.0-vec3(c, c, c) - vec3(0.1024576534223, 0.02506, 0.2532)*m*0.8, 1.0 );
    gl_FragColor = result;
}
