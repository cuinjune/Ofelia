// glm::mat4 bindings
// 2018 Dan Wilcox <danomatika@gmail.com>

// ----- glm/detail/type_mat4hpp -----

struct mat4 {

    static length_t length();

    mat4();
    mat4(mat4 const & v);
    mat4(float scalar);
    mat4(float x0, float y0, float z0, float w0,
         float x1, float y1, float z1, float w1,
         float x2, float y2, float z2, float w2,
         float x3, float y3, float z3, float w3);
    mat4(vec4 const & v1, vec4 const & v2, vec4 const & v3, vec4 const & v4);
    mat4(mat3 const & m);

    mat4 & operator=(mat4 const & m);
};

mat4 operator+(mat4 const & m, float scalar);
mat4 operator+(float scalar, mat4 const & m);
mat4 operator+(mat4 const & m1, mat4 const & m2);
mat4 operator-(mat4 const & m, float scalar);
mat4 operator-(float scalar, mat4 const & m);
mat4 operator-(mat4 const & m1, mat4 const & m2);
mat4 operator*(mat4 const & m, float scalar);
mat4 operator*(float scalar, mat4 const & v);
mat4 operator*(mat4 const & m1, mat4 const & m2);
vec4 operator*(mat4 const & m, vec4 const & v);
vec4 operator*(vec4 const & v, mat4 const & m);
mat4 operator/(mat4 const & m, float scalar);
mat4 operator/(float scalar, mat4 const & m);
mat4 operator/(mat4 const & m1, mat4 const & m2);
vec4 operator/(mat4 const & m, vec4 const & v);
vec4 operator/(vec4 const & v, mat4 const & m);
bool operator==(mat4 const & m1, mat4 const & m2);
bool operator!=(mat4 const & m1, mat4 const & m2);

%extend mat4 {

    // [] getter
    // out of bounds throws a string, which causes a Lua error
    vec4 __getitem__(int i) throw (std::out_of_range) {
        #ifdef SWIGLUA
            if(i < 1 || i > $self->length()) {
                throw std::out_of_range("in glm::mat4::__getitem__()");
            }
            return (*$self)[i-1];
        #else
            if(i < 0 || i >= $self->length()) {
                throw std::out_of_range("in glm::mat4::__getitem__()");
            }
            return (*$self)[i];
        #endif
    }

    // [] setter
    // out of bounds throws a string, which causes a Lua error
    void __setitem__(int i, vec4 v) throw (std::out_of_range) {
        #ifdef SWIGLUA
            if(i < 1 || i > $self->length()) {
                throw std::out_of_range("in glm::mat4::__setitem__()");
            }
            (*$self)[i-1] = v;
        #else
            if(i < 0 || i >= $self->length()) {
                throw std::out_of_range("in glm::mat4::__setitem__()");
            }
            (*$self)[i] = v;
        #endif
    }

    // tostring operator
    std::string __tostring() {
        std::stringstream str;
        const glm::length_t width = $self->length();
        const glm::length_t height = (*$self)[0].length();
        for(glm::length_t row = 0; row < height; ++row) {
            for(glm::length_t col = 0; col < width; ++col) {
                str << (*$self)[col][row];
                if(col + 1 != width) {
                    str << "\t";
                }
            }
            if(row + 1 != height) {
                str << "\n";
            }
        }
        return str.str();
    }

    // extend operators, otherwise some languages (lua)
    // won't be able to act on objects directly (ie. v1 + v2)
    mat4 operator+(float scalar) {return (*$self) + scalar;}
    mat4 operator+(mat4 const & m) {return (*$self) + m;}
    mat4 operator-(float scalar) {return (*$self) - scalar;}
    mat4 operator-(mat4 const & m) {return (*$self) - m;}
    mat4 operator*(float scalar) {return (*$self) * scalar;}
    mat4 operator*(mat4 const & m) {return (*$self) * m;}
    vec4 operator*(vec4 const & v) {return (*$self) * v;}
    mat4 operator/(float scalar) {return (*$self) / scalar;}
    mat4 operator/(mat4 const & m) {return (*$self) / m;}
    vec4 operator/(vec4 const & v) {return (*$self) / v;}
    bool operator==(mat4 const & m) {return (*$self) == m;}
    bool operator!=(mat4 const & m1) {return (*$self) != m;}
};
