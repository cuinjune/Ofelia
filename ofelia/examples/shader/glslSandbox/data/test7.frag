// Rolling ball. By David Hoskins, April 2014.
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// https://www.shadertoy.com/view/lsfXz4


#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
float PI  = 4.0*atan(1.0);
vec3 sunLight  = normalize( vec3(  0.35, 0.2,  0.3 ) );
vec3 cameraPos;
vec3 camTar;
vec3 sunColour = vec3(1.0, .6, .4);
const mat2 rotate2D = mat2(1.932, 1.623, -1.623, 1.952);
float gTime = 0.0;
vec2 ballRoll;
float bounce;
//#define MOD2 vec2(3.07965, 7.4235)
#define MOD2 vec2(.16632,.17369)
#define f(p) length(fract(p/70.) - .5)

//--------------------------------------------------------------------------
// Noise functions...
float Hash( float p )
{
	vec2 p2 = fract(vec2(p) / MOD2);
    p2 += dot(p2.yx, p2.xy+19.19);
	return fract(p2.x * p2.y);
}

//--------------------------------------------------------------------------
float Hash(vec2 p)
{
	p  = fract(p / MOD2);
    p += dot(p.xy, p.yx+19.19);
    return fract(p.x * p.y);
}


//--------------------------------------------------------------------------
float Noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    float res = mix(mix( Hash(p), Hash(p+ vec2(1.0, 0.0)),f.x),
                    mix( Hash(p+ vec2(.0, 1.0)), Hash(p+ vec2(1.0, 1.0)),f.x),f.y);
    return res;
}

//--------------------------------------------------------------------------
vec2 Rotate2(vec2 p, float a)
{
	float si = sin(a);
	float co = cos(a);
	return mat2(co, si, -si, co) * p;
}



//--------------------------------------------------------------------------
vec2 Voronoi( in vec2 x )
{
	vec2 p = floor( x );
	vec2 f = fract( x );
	float res=100.0,id;
	for( int j=-1; j<=1; j++ )
	for( int i=-1; i<=1; i++ )
	{
		vec2 b = vec2( float(i), float(j) );
		vec2 r = b - f  + Hash( p + b );
		float d = dot(r,r);
		if( d < res )
		{
			res = d;
			id  = Hash(p+b);
		}			
    }
	return vec2(max(.4-sqrt(res), 0.0),id);
}


//--------------------------------------------------------------------------
vec3 BallTexture( in vec3 n)
{
	vec3 col = vec3(.5, 0.0, 0.0);
	col= mix(col, vec3(.0, .6, 0.9), smoothstep(-0.05, -.04, n.y) * smoothstep(0.05, .04, n.y));
	float at = cos(atan(n.x, n.z) * 5.0)*(n.y*n.y);
	col =mix(col, vec3(.7, .7, 0.0), smoothstep(0.3, .32, at));
	return col * .8;
}

//--------------------------------------------------------------------------
vec2 Terrain( in vec2 p)
{
	float type = 0.0;
	vec2 pos = p*0.003;
	float w = 50.0;
	float f = .0;
	for (int i = 0; i < 3; i++)
	{
		f += Noise(pos) * w;
		w = w * 0.62;
		pos *= 2.6;
	}

	return vec2(f, type);
}

//--------------------------------------------------------------------------
vec2 Map(in vec3 p)
{
	vec2 h = Terrain(p.xz);
    return vec2(p.y - h.x, h.y);
}

//--------------------------------------------------------------------------
float FractalNoise(in vec2 xy)
{
	float w = .7;
	float f = 0.0;

	for (int i = 0; i < 3; i++)
	{
		f += Noise(xy) * w;
		w = w*0.6;
		xy = 2.0 * xy;
	}
	return f;
}

//--------------------------------------------------------------------------
// Grab all sky information for a given ray from camera
vec3 GetSky(in vec3 rd, bool doClouds)
{
	float sunAmount = max( dot( rd, sunLight), 0.0 );
	float v = pow(1.0-max(rd.y,0.0),6.);
	vec3  sky = mix(vec3(.1, .2, .3), vec3(.32, .32, .32), v);
	sky = sky + sunColour * sunAmount * sunAmount * .25;
	sky = sky + sunColour * min(pow(sunAmount, 800.0)*1.5, .4);
	
	if (doClouds)
	{
		vec2 cl = rd.xz * (1.0/rd.y);
		v = FractalNoise(cl) * .3;
		sky = mix(sky, sunColour, v*v);
	}

	return clamp(sky, 0.0, 1.0);
}

//--------------------------------------------------------------------------
// Merge grass into the sky background for correct fog colouring...
vec3 ApplyFog( in vec3  rgb, in float dis, in vec3 dir)
{
	float fogAmount = clamp(dis*dis* 0.0000011, 0.0, 1.0);
	return mix( rgb, GetSky(dir, false), fogAmount );
}

//--------------------------------------------------------------------------
vec3 DE(vec3 p)
{
	float base = Terrain(p.xz).x - 1.9;
	float height = Noise(p.xz*2.0)*.75 + Noise(p.xz)*.35 + Noise(p.xz*.5)*.2;
	//p.y += height;

	float y = p.y - base-height;
	y = y*y;
	
	// Move grass out of way of target (ball)...
	vec2 move = (p.xz-camTar.xz);
	float l = length(move);
	move = (move * y) * smoothstep(15.0, -6.0, l)/ (bounce+1.0);
	p.xz -= move;

	vec2 ret = Voronoi((p.xz*2.5+sin(y*4.0+p.zx*3.23)*0.12+vec2(sin(time*2.3+0.5*p.z),sin(time*3.6+.5*p.x))*y*.5));
	float f = ret.x * .6 + y * .58;
	return vec3( y - f*1.4, clamp(f * 1.5, 0.0, 1.0), ret.y);
}

//--------------------------------------------------------------------------
// eiffie's code for calculating the aperture size for a given distance...
float CircleOfConfusion(float t)
{
	return max(t * .04, (2.0 / resolution.y) * (1.0+t));
}

//--------------------------------------------------------------------------
float Linstep(float a, float b, float t)
{
	return clamp((t-a)/(b-a),0.,1.);
}

//--------------------------------------------------------------------------
float Sphere( in vec3 ro, in vec3 rd, in vec4 sph )
{
	vec3 oc = ro - sph.xyz;
	float b = dot( oc, rd );
	float c = dot( oc, oc ) - sph.w*sph.w;
	float h = b*b - c;
	if( h<0.0 ) return -1.0;
	return -b - sqrt( h );
}

//--------------------------------------------------------------------------
// Calculate sun light...
void DoLighting(inout vec3 mat, in vec3 normal, in float dis)
{
	float h = dot(sunLight,normal);
	mat = mat * sunColour*(max(h, 0.0)+max((normal.y+.3) * .2, 0.0)+.1);
}

//--------------------------------------------------------------------------
vec3 GrassOut(in vec3 rO, in vec3 rD, in vec3 mat, in vec3 normal, in float dist)
{
	float d = -2.0;
	
	// Only calculate cCoC once is enough here...
	float rCoC = CircleOfConfusion(dist*.3);
	float alpha = 0.0;
	
	vec4 col = vec4(mat, 0.0);

	for (int i = 0; i < 10; i++)
	{
		if (col.w > .99 || d > dist) break;
		vec3 p = rO + rD * d;
		
		vec3 ret = DE(p);
		ret.x += .5 * rCoC;

		if (ret.x < rCoC)
		{
			alpha = (1.0 - col.w) * Linstep(-rCoC, rCoC, -ret.x);//calculate the mix like cloud density
			
			// Mix material with white tips for grass...
			vec3 gra = mix(vec3(.0, .2, 0.0), vec3(.1, .4, min(pow(ret.z, 4.0)*35.0, .35)),
						   pow(ret.y, 9.0)*.7) * ret.y * .7;
			col += vec4(gra * alpha, alpha);
		}
		d += .02;
	}
	
	DoLighting(col.xyz, normal, dist);
	
	col.xyz = mix(mat, col.xyz, col.w);

	return col.xyz;
}


//--------------------------------------------------------------------------
vec3 GrassBlades(in vec3 rO, in vec3 rD, in vec3 mat, in float dist)
{
	float d = 0.0;
	float f;
	// Only calculate cCoC once is enough here...
	float rCoC = CircleOfConfusion(dist*.3);
	float alpha = 0.0;
	
	vec4 col = vec4(mat*0.15, 0.0);

	for (int i = 0; i < 15; i++)
	{
		if (col.w > .99) break;
		vec3 p = rO + rD * d;
		
		vec3 ret = DE(p);
		ret.x += .5 * rCoC;

		if (ret.x < rCoC)
		{
			alpha = (1.0 - col.y) * Linstep(-rCoC, rCoC, -ret.x) * 2.0;//calculate the mix like cloud density
			f = clamp(ret.y, 0.0, 1.0);
			// Mix material with white tips for grass...
			vec3 gra = mix(mat, vec3(.2, .3, min(pow(ret.z, 14.0)*3.0, .3)), pow(ret.y,100.0)*.6 ) * ret.y;
			col += vec4(gra * alpha, alpha);
		}
		d += max(ret.x * .7, .02);
	}
	if(col.w < .2)col.xyz = vec3(0.1, .15, 0.05);
	return col.xyz;
}

//--------------------------------------------------------------------------
vec3 TerrainColour(vec3 pos, vec3 dir,  vec3 normal, float dis, float type)
{
	vec3 mat;
	if (type == 0.0)
	{
		// Random colour...
		mat = mix(vec3(.0,.2,.0), vec3(.1,.3,.0), Noise(pos.xz*.025));
		// Random shadows...
		float t = FractalNoise(pos.xz * .1)+.5;
		// Do grass blade tracing...
		mat = GrassBlades(pos, dir, mat, dis) * t;
		DoLighting(mat, normal, dis);
		float f = Sphere( pos, sunLight, vec4(camTar, 10.0));
		if (f > 0.0)
		{
			mat *= clamp(f*.05, 0.4, 1.0);
		}
	}else
	{
		// Ball...
		vec3 nor = normalize(pos-camTar);
		vec3 spin = nor;
		
		spin.xz = Rotate2(spin.xz, ballRoll.y);
		spin.zy = Rotate2(spin.zy, ballRoll.x);
		spin.xy = Rotate2(spin.xy, .4);
		
		mat = BallTexture(spin);
		DoLighting(mat, nor, dis);
		vec3 ref = reflect(dir, nor);
		mat += sunColour * pow(max(dot(ref, sunLight), 0.0), 6.0) * .3;
		
		if (pos.y < Terrain(pos.xz).x+1.5)
		{
			mat = GrassOut(pos, dir, mat, normal, dis);
		}
	}
	mat = ApplyFog(mat, dis, dir);
	return mat;
}

//--------------------------------------------------------------------------
// Home in on the surface by dividing by two and split...
float BinarySubdivision(in vec3 rO, in vec3 rD, float t, float oldT)
{
	float halfwayT = 0.0;
	for (int n = 0; n < 5; n++)
	{
		halfwayT = (oldT + t ) * .5;
		if (Map(rO + halfwayT*rD).x < .05)
		{
			t = halfwayT;
		}else
		{
			oldT = halfwayT;
		}
	}
	return t;
}


//--------------------------------------------------------------------------
vec3 CameraPath( float t )
{
	//t = time + t;
    vec2 p = vec2(200.0 * sin(3.54*t), 200.0 * cos(3.54*t) );
	return vec3(p.x+5.0,  0.0, -94.0+p.y);
} 

//--------------------------------------------------------------------------
bool Scene(in vec3 rO, in vec3 rD, inout float resT, inout float type )
{
    float t = 5.;
	float oldT = 0.0;
	float delta = 0.;
	vec2 h = vec2(1.0, 1.0);
	bool hit = false;
	for( int j=0; j < 70; j++ )
	{
	    vec3 p = rO + t*rD;
        if (p.y > 90.0) break;

		h = Map(p); // ...Get this position's height mapping.

		// Are we inside, and close enough to fudge a hit?...
		if( h.x < 0.05)
		{
			hit = true;
            		break;
		}
	        
		delta = h.x + (t*0.03);
		oldT = t;
		t += delta;
	}
    type = h.y;
    resT = BinarySubdivision(rO, rD, t, oldT);
	float f = Sphere( rO, rD, vec4(camTar, 10.0));
	
	if (f > 0.0 && f < resT+4.5)
	{
		hit = true;
		type = 1.0;
		resT = f;
		
		
	}
	return hit;
}

//--------------------------------------------------------------------------
vec3 PostEffects(vec3 rgb, vec2 xy)
{
	// Gamma first...
	rgb = pow(rgb, vec3(0.45));
	
	// Then...
	#define CONTRAST 1.1
	#define SATURATION 1.3
	#define BRIGHTNESS 1.3
	rgb = mix(vec3(.5), mix(vec3(dot(vec3(.2125, .7154, .0721), rgb*BRIGHTNESS)), rgb*BRIGHTNESS, SATURATION), CONTRAST);
	// Vignette...
	rgb *= .4+0.6*pow(40.0*xy.x*xy.y*(1.0-xy.x)*(1.0-xy.y), 0.2 );	
	return rgb;
}

//--------------------------------------------------------------------------
void main(void)
{
	float m = (mouse.x/resolution.x)*300.0;
	float gTime = (time*5.0+m+468.0)*.006;
    	vec2 xy = gl_FragCoord.xy / resolution.xy;
	vec2 uv = (-1.0 + 2.0 * xy) * vec2(resolution.x/resolution.y,1.0);
	
	if (xy.y < .13 || xy.y >= .87)
	{
		// Top and bottom cine-crop - what a waste! :)
		gl_FragColor=vec4(vec4(0.0));
		return;
	}

	cameraPos = CameraPath(gTime + 0.0);
    cameraPos.x-= 20.0;
	camTar	  = CameraPath(gTime + .06);
	
	vec2 diff = (camTar.xz - CameraPath(gTime + .13).xz);
	ballRoll.x = -gTime * 72.0;
	ballRoll.y = atan(diff.x, diff.y);
	
	cameraPos.y = Terrain(cameraPos.xz).x + 7.0;
	camTar.y    = Terrain(camTar.xz).x+7.0;
	bounce = abs(sin(gTime*130.0))* 40.0 * max(abs(sin(gTime*15.0))-.85, 0.0);
	camTar.y += bounce;
	
	float roll = .3*sin(gTime*3.0+.5);
	vec3 cw = normalize(vec3(camTar.x, cameraPos.y, camTar.z)-cameraPos);
	vec3 cp = vec3(sin(roll), cos(roll),0.0);
	vec3 cu = cross(cw,cp);
	vec3 cv = cross(cu,cw);
	vec3 dir = normalize(uv.x*cu + uv.y*cv + 1.3*cw);
	mat3 camMat = mat3(cu, cv, cw);

	vec3 col;
	float distance = 1e20;
	float type = 0.0;
	if( !Scene(cameraPos, dir, distance, type) )
	{
		// Missed scene, now just get the sky...
		col = GetSky(dir, true);
	}
	else
	{
		// Get world coordinate of landscape...
		vec3 pos = cameraPos + distance * dir;
		// Get normal from sampling the high definition height map
		vec2 p = vec2(0.1, 0.0);
		vec3 nor  	= vec3(0.0,		Terrain(pos.xz).x, 0.0);
		vec3 v2		= nor-vec3(p.x,	Terrain(pos.xz+p).x, 0.0);
		vec3 v3		= nor-vec3(0.0,	Terrain(pos.xz-p.yx).x, -p.x);
		nor = cross(v2, v3);
		nor = normalize(nor);

		// Get the colour using all available data...
		col = TerrainColour(pos, dir, nor, distance, type);
	}
	
	// bri is the brightness of sun at the centre of the camera direction.
	float bri = dot(cw, sunLight)*.75;
	if (bri > 0.0)
	{
		vec2 sunPos = vec2( dot( sunLight, cu ), dot( sunLight, cv ) );
		vec2 uvT = uv-sunPos;
		uvT = uvT*(length(uvT));
		bri = pow(bri, 6.0)*.8;

		// glare = the red shifted blob...
		float glare1 = max(dot(normalize(vec3(dir.x, dir.y+.3, dir.z)),sunLight),0.0)*1.4;
		// glare2 is the yellow ring...
		float glare2 = max(1.0-length(uvT+sunPos*.5)*4.0, 0.0);
		uvT = mix (uvT, uv, -2.3);
		// glare3 is a purple splodge...
		float glare3 = max(1.0-length(uvT+sunPos*5.0)*1.2, 0.0);

		col += bri * vec3(1.0, .0, .0)  * pow(glare1, 12.5)*.05;
		col += bri * vec3(1.0, 1.0, 0.2) * pow(glare2, 2.0)*2.5;
		col += bri * sunColour * pow(glare3, 2.0)*3.0;
	}
	col = PostEffects(col, xy);	
	
	gl_FragColor=vec4(col,1.0);
}

//--------------------------------------------------------------------------