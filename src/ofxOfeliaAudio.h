#include "m_pd.h"

#define UNITBIT32 1572864.  /* 3*2^19; bit 32 has place value 1 */
#define TABNUM_TRIANGLE 20
#define TABNUM_SAW 40
#define TABNUM_SQUARE 20
#define TABSIZE 515

class ofxOfeliaOsc
{
public:
    ofxOfeliaOsc()
    :phase(0.0){};
    void setPhase(t_floatarg f);
    t_float getPhase();
    void sine(float *io1, int n1);
    void triangle(float *io1, int n1);
    void saw(float *io1, int n1);
    void square(float *io1, int n1);
    void pulse(float *io1, int n1, float *in2, int n2);
private:
    double phase;
    union tabfudge
    {
        double tf_d;
        int32_t tf_i[2];
    };
};

class ofxOfeliaBlOsc
{
public:
    ofxOfeliaBlOsc()
    :phase(0.0)
    ,phase2(0.0)
    ,isRawMode(true){};
    void setPhase(t_floatarg f);
    t_float getPhase();
    void blTriangle(float *io1, int n1);
    void blSaw(float *io1, int n1);
    void blSquare(float *io1, int n1);
    void blPulse(float *io1, int n1, float *in2, int n2);
private:
    double phase, phase2;
    bool isRawMode;
    union tabfudge
    {
        double tf_d;
        int32_t tf_i[2];
    };
    static t_float blTriangleTab[TABNUM_TRIANGLE][TABSIZE];
    static t_float blSawTab[TABNUM_SAW][TABSIZE];
    static t_float blSquareTab[TABNUM_SQUARE][TABSIZE];
};

class ofxOfeliaFilter
{
public:
    ofxOfeliaFilter()
    :x1(0)
    ,x2(0)
    ,y1(0)
    ,y2(0){};
    void clear(); /* clear internal state to zero */
    void lowPass(float *io1, int n1, float *in2, int n2, float *in3, int n3);
    void highPass(float *io1, int n1, float *in2, int n2, float *in3, int n3);
    void bandPass(float *io1, int n1, float *in2, int n2, float *in3, int n3);
    void notch(float *io1, int n1, float *in2, int n2, float *in3, int n3);
    void peaking(float *io1, int n1, float *in2, int n2,
                 float *in3, int n3, float *in4, int n4);
    void lowShelf(float *io1, int n1, float *in2, int n2,
                  float *in3, int n3, float *in4, int n4);
    void highShelf(float *io1, int n1, float *in2, int n2,
                   float *in3, int n3, float *in4, int n4);
    void allPass(float *io1, int n1, float *in2, int n2, float *in3, int n3);
private:
    t_float x1, x2, y1, y2;
};
