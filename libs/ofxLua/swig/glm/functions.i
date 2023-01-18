// glm function bindings
// adapted from https://github.com/IndiumGames/swig-wrapper-glm
//
// The MIT License (MIT)
//
// Copyright (c) 2016 Indium Games (www.indiumgames.fi)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

%define FLOAT_VECTOR(function_name)
    vec2 function_name(const vec2 &);
    vec3 function_name(const vec3 &);
    vec4 function_name(const vec4 &);
%enddef

%define FLOAT_VECTOR_2_PARAMS(function_name)
    vec2 function_name(const vec2 &, const vec2 &);
    vec3 function_name(const vec3 &, const vec3 &);
    vec4 function_name(const vec4 &, const vec4 &);
%enddef

%define FLOAT_VECTOR_3_PARAMS(function_name)
    vec2 function_name(const vec2 &, const vec2 &, const vec2 &);
    vec3 function_name(const vec3 &, const vec3 &, const vec3 &);
    vec4 function_name(const vec4 &, const vec4 &, const vec4 &);
%enddef

%define FLOAT_VECTOR_RETURN_VALUE(function_name)
    float function_name(const vec2 &);
    float function_name(const vec3 &);
    float function_name(const vec4 &);
%enddef

%define FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(function_name)
    float function_name(const vec2 &, const vec2 &);
    float function_name(const vec3 &, const vec3 &);
    float function_name(const vec4 &, const vec4 &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR(function_name)
    float function_name(const float &);
    vec2  function_name(const vec2 &);
    vec3  function_name(const vec3 &);
    vec4  function_name(const vec4 &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_RETURN_VALUE(function_name)
    float function_name(const float &);
    float function_name(const vec2 &);
    float function_name(const vec3 &);
    float function_name(const vec4 &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_2_PARAMS(function_name)
    float function_name(const float &, const float &);
    vec2 function_name(const vec2 &, const vec2 &);
    vec3 function_name(const vec3 &, const vec3 &);
    vec4 function_name(const vec4 &, const vec4 &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_2_PARAMS_VECTOR_VALUE(function_name)
    vec2 function_name(const vec2 &, const float &);
    vec3 function_name(const vec3 &, const float &);
    vec4 function_name(const vec4 &, const float &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_3_PARAMS(function_name)
    float function_name(const float &, const float &, const float &);
    vec2 function_name(const vec2 &, const vec2 &, const vec2 &);
    vec3 function_name(const vec3 &, const vec3 &, const vec3 &);
    vec4 function_name(const vec4 &, const vec4 &, const vec4 &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_3_PARAMS_VECTOR_VALUE_VALUE(function_name)
    vec2 function_name(const vec2 &, const float &, const float &);
    vec3 function_name(const vec3 &, const float &, const float &);
    vec4 function_name(const vec4 &, const float &, const float &);
%enddef

%define FLOAT_SCALAR_OR_VECTOR_3_PARAMS_VECTOR_VECTOR_VALUE(function_name)
    vec2 function_name(const vec2 &, const vec2 &, const float &);
    vec3 function_name(const vec3 &, const vec3 &, const float &);
    vec4 function_name(const vec4 &, const vec4 &, const float &);
%enddef

// ----- glm/common.hpp -----

FLOAT_SCALAR_OR_VECTOR(abs);
FLOAT_SCALAR_OR_VECTOR(sign);
FLOAT_SCALAR_OR_VECTOR(floor);
FLOAT_SCALAR_OR_VECTOR(trunc);
FLOAT_SCALAR_OR_VECTOR(round);
FLOAT_SCALAR_OR_VECTOR(roundEven);
FLOAT_SCALAR_OR_VECTOR(ceil);
FLOAT_SCALAR_OR_VECTOR(fract);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS(mod);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS_VECTOR_VALUE(mod);
float modf(const float &, float &);
vec2 modf(const vec2 &, vec2 &);
vec3 modf(const vec3 &, vec3 &);
vec4 modf(const vec4 &, vec4 &);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS(min);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS_VECTOR_VALUE(min);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS(max);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS_VECTOR_VALUE(max);
FLOAT_SCALAR_OR_VECTOR_3_PARAMS(clamp);
FLOAT_SCALAR_OR_VECTOR_3_PARAMS_VECTOR_VALUE_VALUE(clamp);
float mix(const float &, const float &, const float &);
float mix(const float &, const float &, const bool  &);
vec2 mix(const vec2 &, const vec2 &, const vec2  &);
vec3 mix(const vec3 &, const vec3 &, const vec3  &);
vec4 mix(const vec4 &, const vec4 &, const vec4  &);
vec2 mix(const vec2 &, const vec2 &, const bool &);
vec3 mix(const vec3 &, const vec3 &, const bool &);
vec4 mix(const vec4 &, const vec4 &, const bool &);
vec2 step(const vec2 &, const vec2 &);
vec3 step(const vec3 &, const vec3 &);
vec4 step(const vec4 &, const vec4 &);
vec2 step(const float &, const vec2 &);
vec3 step(const float &, const vec3 &);
vec4 step(const float &, const vec4 &);
FLOAT_SCALAR_OR_VECTOR_3_PARAMS(smoothstep);
vec2 smoothstep(const float &, const float &, const vec2 &);
vec3 smoothstep(const float &, const float &, const vec3 &);
vec4 smoothstep(const float &, const float &, const vec4 &);
bool isnan(const float &);
bool isinf(const float &);

// fma appears to be scalar only now?
//FLOAT_SCALAR_OR_VECTOR_3_PARAMS(fma);
float fma(const float &, const float &, const float &);

// ----- glm/exponential.hpp -----

FLOAT_SCALAR_OR_VECTOR_2_PARAMS(pow);
FLOAT_SCALAR_OR_VECTOR(exp);
FLOAT_SCALAR_OR_VECTOR(log);
FLOAT_SCALAR_OR_VECTOR(exp2);
FLOAT_SCALAR_OR_VECTOR(log2);
FLOAT_VECTOR(sqrt);
FLOAT_SCALAR_OR_VECTOR(inversesqrt);

// ----- glm/geometric.hpp -----

FLOAT_VECTOR_RETURN_VALUE(length);
FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(distance);
FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(dot);
vec3 cross(const vec3 &, const vec3 &);
FLOAT_VECTOR(normalize);
FLOAT_VECTOR_3_PARAMS(faceforward);
FLOAT_VECTOR_2_PARAMS(reflect);
vec2 refract(const vec2 &, const vec2 &, const float &);
vec3 refract(const vec3 &, const vec3 &, const float &);
vec4 refract(const vec4 &, const vec4 &, const float &);

// ----- glm/matrix.hpp -----

mat3 matrixCompMult(const mat3 &, const mat3 &);
mat4 matrixCompMult(const mat4 &, const mat4 &);
mat3 outerProduct(const vec3 &, const vec3 &);
mat4 outerProduct(const vec4 &, const vec4 &);
mat3 transpose(const mat3 &);
mat4 transpose(const mat4 &);
float determinant(const mat3 &);
float determinant(const mat4 &);
mat3 inverse(const mat3 &);
mat4 inverse(const mat4 &);

// ----- glm/trigonometric.hpp -----

FLOAT_SCALAR_OR_VECTOR(radians);
FLOAT_SCALAR_OR_VECTOR(degrees);
FLOAT_SCALAR_OR_VECTOR(sin);
FLOAT_SCALAR_OR_VECTOR(cos);
FLOAT_SCALAR_OR_VECTOR(tan);
FLOAT_SCALAR_OR_VECTOR(asin);
FLOAT_SCALAR_OR_VECTOR(acos);
FLOAT_SCALAR_OR_VECTOR_2_PARAMS(atan);
FLOAT_SCALAR_OR_VECTOR(atan);
FLOAT_SCALAR_OR_VECTOR(sinh);
FLOAT_SCALAR_OR_VECTOR(cosh);
FLOAT_SCALAR_OR_VECTOR(tanh);
FLOAT_SCALAR_OR_VECTOR(asinh);
FLOAT_SCALAR_OR_VECTOR(acosh);
FLOAT_SCALAR_OR_VECTOR(atanh);

// ----- glm/gtc/epsilon.hpp -----

bool epsilonEqual(const float &, const float &, const float &);
vec2 epsilonEqual(const vec2 &, const vec2 &, const float &);
vec3 epsilonEqual(const vec3 &, const vec3 &, const float &);

bool epsilonNotEqual(const float &, const float &, const float &);
vec2 epsilonNotEqual(const vec2 &, const vec2 &, const float &);
vec3 epsilonNotEqual(const vec3 &, const vec3 &, const float &);

// ----- glm/gtc/matrix_access.hpp -----

vec3 row(const mat3 &,    const length_t &);
vec4 row(const mat4 &,    const length_t &);
mat3 row(const mat3 &,    const length_t &, const vec3 &);
mat4 row(const mat4 &,    const length_t &, const vec4 &);
vec3 column(const mat3 &, const length_t &);
vec4 column(const mat4 &, const length_t &);
mat3 column(const mat3 &, const length_t &, const vec3 &);
mat4 column(const mat4 &, const length_t &, const vec4 &);

// ----- glm/gtc/matrix_inverse.hpp -----

mat3 affineInverse(const mat3 &);
mat4 affineInverse(const mat4 &);
mat3 inverseTranspose(const mat3 &);
mat4 inverseTranspose(const mat4 &);

// ----- glm/ext/matrix_transform.hpp -----

//mat4 identity();
mat4 translate(const mat4 &, const vec3 &);
mat4 rotate(const mat4 &, const float &, const vec3 &);
mat4 scale(const mat4 &, const vec3 &);
mat4 lookAt(const vec3 &, const vec3 &, const vec3 &);

// ----- glm/ext/matrix_clip_space.hpp -----

mat4 ortho(const float &, const float &,
           const float &, const float &);
mat4 ortho(const float &, const float &,
           const float &, const float &,
           const float &, const float &);
mat4 frustum(const float &, const float &,
             const float &, const float &,
             const float &, const float &);
mat4 perspective(const float &, const float &,
                 const float &, const float &);
mat4 perspectiveFov(const float &,
                    const float &, const float &,
                    const float &, const float &);
mat4 infinitePerspective(const float &, const float &, const float &);
mat4 tweakedInfinitePerspective(const float &, const float &, const float &);

// ----- glm/ext/matrix_projection.hpp -----

vec3 project(const vec3 &, const mat4 &, const mat4 &, const vec4 &);
vec3 unProject(const vec3 &, const mat4 &, const mat4 &, const vec4 &);
mat4 pickMatrix(const vec2 &, const vec2 &, const vec4 &);

// ----- glm/gtx/compatibility.hpp -----

FLOAT_SCALAR_OR_VECTOR_3_PARAMS(lerp);
FLOAT_SCALAR_OR_VECTOR_3_PARAMS_VECTOR_VECTOR_VALUE(lerp);
FLOAT_VECTOR(saturate);
FLOAT_VECTOR_2_PARAMS(atan2);
bool isfinite(float &);
vec2 isfinite(vec2 &);
vec3 isfinite(vec3 &);
vec4 isfinite(vec4 &);

// scalar types seem to cause issues
//float atan2(float, float);
//float saturate(float);

// ----- glm/gtx/fast_square_root.hpp -----

FLOAT_VECTOR(fastSqrt);
FLOAT_SCALAR_OR_VECTOR(fastInverseSqrt);
FLOAT_VECTOR_RETURN_VALUE(fastLength);
FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(fastDistance);
FLOAT_VECTOR(fastNormalize);

// ----- glm/gtx/norm.hpp -----

FLOAT_VECTOR_RETURN_VALUE(length2);
FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(distance2);
float l1Norm(vec3 const &, vec3 const &);
float l1Norm(vec3 const &);
float l2Norm(vec3 const &, vec3 const &);
float l2Norm(vec3 const &);
float lxNorm(vec3 const &, vec3 const &, unsigned int);
float lxNorm(vec3 const &, unsigned int);

// ----- glm/gtx/perpendicular.hpp -----

vec2 perp(const vec2 &, const vec2 &);
vec3 perp(const vec3 &, const vec3 &);

// ----- glm/gtx/rotate_vector.hpp -----

vec3 slerp(const vec3 &, const vec3 &, const float &);
vec2 rotate(const vec2 &, const float &);
vec3 rotate(const vec3 &, const float &, const vec3 &);
vec4 rotate(const vec4 &, const float &, const vec3 &);
vec3 rotateX(const vec3 &, const float &);
vec4 rotateX(const vec4 &, const float &);
vec3 rotateY(const vec3 &, const float &);
vec4 rotateY(const vec4 &, const float &);
vec3 rotateZ(const vec3 &, const float &);
vec4 rotateZ(const vec4 &, const float &);
mat4 orientation(const vec3 &, const vec3 &);

// ----- glm/gtx/scalar_multiplication.hpp -----

// handled in the type interfaces (ie. vec2.i, vec3.i, etc)

// ----- glm/gtx/spline.hpp -----

vec2 catmullRom(const vec2 &, const vec2 &, const vec2 &, const vec2 &, const float &);
vec3 catmullRom(const vec3 &, const vec3 &, const vec3 &, const vec3 &, const float &);
vec2 hermite(const vec2 &, const vec2 &, const vec2 &, const vec2 &, const float &);
vec3 hermite(const vec3 &, const vec3 &, const vec3 &, const vec3 &, const float &);
vec2 cubic(const vec2 &, const vec2 &, const vec2 &, const vec2 &, const float &);
vec3 cubic(const vec3 &, const vec3 &, const vec3 &, const vec3 &, const float &);

// ----- glm/gtx/transform.hpp -----

mat4 translate(const vec3 &);
mat4 rotate(float angle, const vec3 &);
mat4 scale(const vec3 &);

// ----- glm/gtx/vector_angle.hpp -----

FLOAT_VECTOR_RETURN_VALUE_2_PARAMS(angle);
float orientedAngle(const vec2 &, const vec2 &);
float orientedAngle(const vec3 &, const vec3 &, const vec3 &);
