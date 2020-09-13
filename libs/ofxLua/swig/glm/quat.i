// glm::quat bindings
// 2018 Dan Wilcox <danomatika@gmail.com>

// ----- glm/gtc/quaternion.hpp -----

struct quat {

	float x, y, z, w;

	static length_t length();

	quat();
    quat(quat const & q);
    quat(float s, vec3 const & v);
    quat(float w, float x, float y, float z);
    quat(vec3 const & u, vec3 const & v);
    quat(vec3 const & eulerAngles);
    quat(mat3 const & m);
    quat(mat4 const & m);
};

quat operator+(quat const & q, quat const & p);
quat operator*(quat const & q, quat const & p);
vec3 operator*(quat const & q, vec3 const & v);
vec3 operator*(vec3 const & v, quat const & q);
vec4 operator*(quat const & q, vec4 const & v);
vec4 operator*(vec4 const & v, quat const & q);
quat operator*(quat const & q, float const & s);
quat operator*(float const & s, quat const & q);
quat operator/(quat const & q, float const & s);
bool operator==(quat const & q1, quat const & q2);
bool operator!=(quat const & q1, quat const & q2);

float length(quat const & q);

quat normalize(quat const & q);
float dot(quat const & x, quat const & y);
quat mix(quat const & x, quat const & y, float a);
quat lerp(quat const & x, quat const & y, float a);
quat slerp(quat const & x, quat const & y, float a);
quat conjugate(quat const & q);
quat inverse(quat const & q);
quat rotate(quat const & q, float const & angle, vec3 const & axis);
vec3 eulerAngles(quat const & x);
float roll(quat const & x);
float pitch(quat const & x);
float yaw(quat const & x);
mat3 mat3_cast(quat const & x);
mat4 mat4_cast(quat const & x);
quat quat_cast(glm::mat3 const & x);
quat quat_cast(glm::mat4 const & x);
float angle(quat const & x);
vec3 axis(quat const & x);
quat angleAxis(float const & angle, vec3 const & axis);

// not sure what to do with these yet
// vec4<bool, P> lessThan(quat const & x, quat const & y);
// vec4<bool, P> lessThanEqual(quat const & x, quat const & y);
// vec4<bool, P> greaterThan(quat const & x, quat const & y);
// vec4<bool, P> greaterThanEqual(quat const & x, quat const & y);
// vec4<bool, P> equal(quat const & x, quat const & y);
// vec4<bool, P> notEqual(quat const & x, quat const & y);
// vec4<bool, P> isnan(quat const & x);
// vec4<bool, P> isinf(quat const & x);

%extend quat {

    // [] getter
    // out of bounds throws a string, which causes a Lua error
    float __getitem__(int i) throw (std::out_of_range) {
        #ifdef SWIGLUA
            if(i < 1 || i > $self->length()) {
                throw std::out_of_range("in glm::quat::__getitem__()");
            }
            return (*$self)[i-1];
        #else
            if(i < 0 || i >= $self->length()) {
                throw std::out_of_range("in glm::quat::__getitem__()");
            }
            return (*$self)[i];
        #endif
    }

    // [] setter
    // out of bounds throws a string, which causes a Lua error
    void __setitem__(int i, float f) throw (std::out_of_range) {
        #ifdef SWIGLUA
            if(i < 1 || i > $self->length()) {
                throw std::out_of_range("in glm::quat::__setitem__()");
            }
            (*$self)[i-1] = f;
        #else
            if(i < 0 || i >= $self->length()) {
                throw std::out_of_range("in glm::quat::__setitem__()");
            }
            (*$self)[i] = f;
        #endif
    }

	// to string operator
    std::string __tostring() {
        std::stringstream str;
        for(glm::length_t i = 0; i < $self->length(); ++i) {
            str << (*$self)[i];   
            if(i + 1 != $self->length()) {
                str << " ";
            }
        }
        return str.str();
    }

    // extend operators, otherwise some languages (lua)
    // won't be able to act on objects directly (ie. v1 + v2)
    quat operator+(quat const & q) {return (*$self) + q;}
    quat operator*(quat const & q) {return (*$self) * q;}
    vec3 operator*(vec3 const & v) {return (*$self) * v;}
    vec4 operator*(vec4 const & v) {return (*$self) * v;}
    quat operator*(float const & s) {return (*$self) * s;}
    quat operator/(float const & s) {return (*$self) / s;}
    bool operator==(quat const & q) {return (*$self) == q;}
    bool operator!=(quat const & q) {return (*$self) != q;}
};
