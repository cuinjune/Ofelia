#include "m_pd.h"

#define UNITBIT32 1572864.  /* 3*2^19; bit 32 has place value 1 */
#define TABNUM_TRIANGLE 20
#define TABNUM_SAW 40
#define TABNUM_SQUARE 20
#define TABSIZE 515

class ofxOfeliaAudio
{
public:
    ofxOfeliaAudio()
    :phase(0.0)
    ,phase2(0.0)
    ,isRawMode(true)
    ,x1(0)
    ,x2(0)
    ,y1(0)
    ,y2(0){};
    void setPhase(t_floatarg f);
    t_float getPhase();
    void sine(float *in1, int n1);
    void triangle(float *in1, int n1);
    void saw(float *in1, int n1);
    void square(float *in1, int n1);
    void pulse(float *in1, int n1, float *in2, int n2);
    void blTriangle(float *in1, int n1);
    void blSaw(float *in1, int n1);
    void blSquare(float *in1, int n1);
    void blPulse(float *in1, int n1, float *in2, int n2);
    void clear(); /* clear internal state to zero */
    void lowPass(float *in1, int n1, float *in2, int n2, float *in3, int n3);
    void highPass(float *in1, int n1, float *in2, int n2, float *in3, int n3);
    void bandPass(float *in1, int n1, float *in2, int n2, float *in3, int n3);
    void notch(float *in1, int n1, float *in2, int n2, float *in3, int n3);
    void peaking(float *in1, int n1, float *in2, int n2,
                 float *in3, int n3, float *in4, int n4);
    void lowShelf(float *in1, int n1, float *in2, int n2,
                  float *in3, int n3, float *in4, int n4);
    void highShelf(float *in1, int n1, float *in2, int n2,
                   float *in3, int n3, float *in4, int n4);
    void allPass(float *in1, int n1, float *in2, int n2, float *in3, int n3);
private:
    double phase, phase2;
    bool isRawMode;
    union tabfudge
    {
        double tf_d;
        int32_t tf_i[2];
    };
    t_float x1, x2, y1, y2;
    static t_float blTriangleTab[TABNUM_TRIANGLE][TABSIZE];
    static t_float blSawTab[TABNUM_SAW][TABSIZE];
    static t_float blSquareTab[TABNUM_SQUARE][TABSIZE];
};
