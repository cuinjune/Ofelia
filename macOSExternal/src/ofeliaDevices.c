/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#include "ofeliaDevices.h"
#include "m_pd.h"

#define MAXAUDIOINDEV 4
#define MAXAUDIOOUTDEV 4
#define MAXMIDIINDEV 16
#define MAXMIDIOUTDEV 16
#define MAXNDEV 20
#define DEVDESCSIZE 80

/* ________________________________________________________________________________
 * common methods
 */
int getDevClampIntValue(const int value, const int min, const int max) {
    
    return value < min ? min : value > max ? max : value;
}

/* ________________________________________________________________________________
 * ofGetAudioDevices object methods
 */
static t_class *ofeliaGetAudioDevices_class;

typedef struct _ofeliaGetAudioDevices
{
    t_object x_obj;
    t_outlet *inDevices_out, *outDevices_out;
    
} t_ofeliaGetAudioDevices;

void *ofeliaGetAudioDevices_new()
{
    t_ofeliaGetAudioDevices *x = (t_ofeliaGetAudioDevices *)pd_new(ofeliaGetAudioDevices_class);
    x->inDevices_out = outlet_new(&x->x_obj, &s_list);
    x->outDevices_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void sys_get_audio_devs(char *indevlist, int *nindevs,
                        char *outdevlist, int *noutdevs, int *canmulti,
                        int *cancallback,
                        int maxndev, int devdescsize);

void ofeliaGetAudioDevices_bang(t_ofeliaGetAudioDevices *x)
{
    char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
    int nindevs = 0, noutdevs = 0, canmulti, cancallback;
    sys_get_audio_devs(indevlist, &nindevs, outdevlist, &noutdevs,
                       &canmulti, &cancallback, MAXNDEV, DEVDESCSIZE);
    
    /* outlet audioOut devices */
    const int acout = noutdevs;
    t_atom *avout = (t_atom *)getbytes(acout * sizeof(t_atom));
    
    for (int i=0; i<acout; ++i) {
        
        avout[i].a_type = A_SYMBOL;
        avout[i].a_w.w_symbol = gensym(outdevlist + i * DEVDESCSIZE);
    }
    outlet_list(x->outDevices_out, &s_list, acout, avout);
    freebytes(avout, acout * sizeof(t_atom));
    
    /* outlet audioIn devices */
    const int acin = nindevs;
    t_atom *avin = (t_atom *)getbytes(acin * sizeof(t_atom));
    
    for (int i=0; i<acin; ++i) {
        
        avin[i].a_type = A_SYMBOL;
        avin[i].a_w.w_symbol = gensym(indevlist + i * DEVDESCSIZE);
    }
    outlet_list(x->inDevices_out, &s_list, acin, avin);
    freebytes(avin, acin * sizeof(t_atom));
}

void ofeliaGetAudioDevices_free(t_ofeliaGetAudioDevices *x)
{
    outlet_free(x->inDevices_out);
    outlet_free(x->outDevices_out);
}

void ofeliaGetAudioDevices_setup()
{
    ofeliaGetAudioDevices_class = class_new(gensym("ofGetAudioDevices"),
                                            (t_newmethod)ofeliaGetAudioDevices_new,
                                            (t_method)ofeliaGetAudioDevices_free,
                                            sizeof(t_ofeliaGetAudioDevices),
                                            CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetAudioDevices_class, (t_method)ofeliaGetAudioDevices_bang);
}

/* ________________________________________________________________________________
 * ofSetAudioDevices object methods
 */
static t_class *ofeliaSetAudioDevices_class;

typedef struct _ofeliaSetAudioDevices
{
    t_object x_obj;
    int inputDeviceID;
    int numInputChannels;
    int outputDeviceID;
    int numOutputChannels;
    int sampleRate;
    int advance;
    int callBack;
    int blockSize;
    
} t_ofeliaSetAudioDevices;

void sys_get_audio_params(int *pnaudioindev, int *paudioindev, int *chindev,
                          int *pnaudiooutdev, int *paudiooutdev, int *choutdev,
                          int *prate, int *padvance, int *pcallback, int *pblocksize);

void *ofeliaSetAudioDevices_new()
{
    t_ofeliaSetAudioDevices *x = (t_ofeliaSetAudioDevices *)pd_new(ofeliaSetAudioDevices_class);
    int naudioindev, audioindev[MAXAUDIOINDEV], chindev[MAXAUDIOINDEV];
    int naudiooutdev, audiooutdev[MAXAUDIOOUTDEV], choutdev[MAXAUDIOOUTDEV];
    int rate, advance, callback, blocksize;
    sys_get_audio_params(&naudioindev, audioindev, chindev,
                         &naudiooutdev, audiooutdev, choutdev, &rate, &advance,
                         &callback, &blocksize);
    
    if (naudioindev) {
        
        x->inputDeviceID = audioindev[0];
        x->numInputChannels = chindev[0];
    }
    else {
        
        x->inputDeviceID = 0;
        x->numInputChannels = 1;
    }
    if (naudiooutdev) {
        
        x->outputDeviceID = audiooutdev[0];
        x->numOutputChannels = choutdev[0];
    }
    else {
        
        x->outputDeviceID = 0;
        x->numOutputChannels = 2;
    }
    x->sampleRate = rate;
    x->advance = advance;
    x->callBack = callback;
    x->blockSize = blocksize;
    return (x);
}

void ofeliaSetAudioDevices_bang(t_ofeliaSetAudioDevices *x)
{
    /* audio-dialog message accepts total 20 arguments */
    /* input device 1-4, input channels 1-4, 
       output device 1-4, output channels 1-4, 
       sampleRate, delay, callback, blockSize */
    const t_symbol *pdSym = gensym("pd");
    t_atom av[20];
    
    for (int i=0; i<20; ++i) {
     
        av[i].a_type = A_FLOAT;
        av[i].a_w.w_float = 0.0f;
    }
    av[0].a_w.w_float = (t_float)x->inputDeviceID;
    av[4].a_w.w_float = (t_float)x->numInputChannels;
    av[8].a_w.w_float = (t_float)x->outputDeviceID;
    av[12].a_w.w_float = (t_float)x->numOutputChannels;
    av[16].a_w.w_float = (t_float)x->sampleRate;
    av[17].a_w.w_float = (t_float)x->advance;
    av[18].a_w.w_float = (t_float)x->callBack;
    av[19].a_w.w_float = (t_float)x->blockSize;

    if (pdSym->s_thing)
        pd_typedmess(pdSym->s_thing, gensym("audio-dialog"), 20, av);
}

int sys_audiodevnametonumber(int output, const char *name);
void sys_audiodevnumbertoname(int output, int devno, char *name,
                              int namesize);

void ofeliaSetAudioDevices_input(t_ofeliaSetAudioDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
            int nindevs = 0, noutdevs = 0, canmulti, cancallback;
            sys_get_audio_devs(indevlist, &nindevs, outdevlist, &noutdevs,
                               &canmulti, &cancallback, MAXNDEV, DEVDESCSIZE);
            x->inputDeviceID = getDevClampIntValue((int)argv[0].a_w.w_float, 0, nindevs-1);
            x->numInputChannels = getDevClampIntValue((int)argv[1].a_w.w_float, 0, 64);
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_FLOAT) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            const int inputDeviceID = sys_audiodevnametonumber(0, name);
            
            if (inputDeviceID < 0) {
                
                error("ofSetAudioDevices: couldn't find audio input device: %s", name);
                return;
            }
            x->inputDeviceID = inputDeviceID;
            x->numInputChannels = getDevClampIntValue((int)argv[1].a_w.w_float, 0, 64);
        }
        else {
            
            error("ofSetAudioDevices: wrong argument type");
        }
    }
    else {
        
        error("ofSetAudioDevices: invalid number of arguments");
    }
}

void ofeliaSetAudioDevices_output(t_ofeliaSetAudioDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
            int nindevs = 0, noutdevs = 0, canmulti, cancallback;
            sys_get_audio_devs(indevlist, &nindevs, outdevlist, &noutdevs,
                               &canmulti, &cancallback, MAXNDEV, DEVDESCSIZE);
            x->outputDeviceID = getDevClampIntValue((int)argv[0].a_w.w_float, 0, noutdevs-1);
            x->numOutputChannels = getDevClampIntValue((int)argv[1].a_w.w_float, 0, 64);
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_FLOAT) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            const int outputDeviceID = sys_audiodevnametonumber(1, name);
            
            if (outputDeviceID < 0) {
                
                error("ofSetAudioDevices: couldn't find audio output device: %s", name);
                return;
            }
            x->outputDeviceID = outputDeviceID;
            x->numOutputChannels = getDevClampIntValue((int)argv[1].a_w.w_float, 0, 64);
        }
        else {
            
            error("ofSetAudioDevices: wrong argument type");
        }
    }
    else {
        
        error("ofSetAudioDevices: invalid number of arguments");
    }
}

void ofeliaSetAudioDevices_sampleRate(t_ofeliaSetAudioDevices *x, t_floatarg f)
{
    const int sampleRate = (int)f;
    
    /* available sampleRates may differ depending on devices & platforms */
    if (sampleRate == 22050 ||
        sampleRate == 44100 ||
        sampleRate == 48000 ||
        sampleRate == 88200 ||
        sampleRate == 96000) {
        
        x->sampleRate = sampleRate;
    }
    else {
        
        error("ofSetAudioDevices: invalid sampleRate");
    }
}

void ofeliaSetAudioDevices_blockSize(t_ofeliaSetAudioDevices *x, t_floatarg f)
{
    const int blockSize = (int)f;
    
    if (blockSize == 64 ||
        blockSize == 128 ||
        blockSize == 256 ||
        blockSize == 512 ||
        blockSize == 1024 ||
        blockSize == 2048) { //OFELIANOTE : pd freezes if blockSize is >= 1024 on MacOS
        
        x->blockSize = blockSize;
    }
    else {
        
        error("ofSetAudioDevices: invalid blockSize");
    }
}

void ofeliaSetAudioDevices_print(t_ofeliaSetAudioDevices *x)
{
    post("\n[ofSetAudioDevices]");
    char audio_indevnames[MAXAUDIOINDEV * DEVDESCSIZE];
    sys_audiodevnumbertoname(0, x->inputDeviceID, &audio_indevnames[x->inputDeviceID * DEVDESCSIZE], DEVDESCSIZE);
    post("input : %s %d", audio_indevnames + x->inputDeviceID * DEVDESCSIZE, x->numInputChannels);
    char audio_outdevnames[MAXAUDIOINDEV * DEVDESCSIZE];
    sys_audiodevnumbertoname(1, x->outputDeviceID, &audio_outdevnames[x->outputDeviceID * DEVDESCSIZE], DEVDESCSIZE);
    post("output : %s %d", audio_outdevnames + x->outputDeviceID * DEVDESCSIZE, x->numOutputChannels);
    post("sampleRate : %d", x->sampleRate);
    post("blockSize : %d", x->blockSize);
}

void ofeliaSetAudioDevices_setup()
{
    ofeliaSetAudioDevices_class = class_new(gensym("ofSetAudioDevices"),
                                            (t_newmethod)ofeliaSetAudioDevices_new,
                                            0, sizeof(t_ofeliaSetAudioDevices),
                                            CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_bang);
    class_addmethod(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_input, gensym("input"), A_GIMME, 0);
    class_addmethod(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_output, gensym("output"), A_GIMME, 0);
    class_addmethod(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_sampleRate, gensym("sampleRate"), A_FLOAT, 0);
    class_addmethod(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_blockSize, gensym("blockSize"), A_FLOAT, 0);
    class_addmethod(ofeliaSetAudioDevices_class, (t_method)ofeliaSetAudioDevices_print, gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMidiDevices object methods
 */
static t_class *ofeliaGetMidiDevices_class;

typedef struct _ofeliaGetMidiDevices
{
    t_object x_obj;
    t_outlet *inDevices_out, *outDevices_out;
    
} t_ofeliaGetMidiDevices;

void *ofeliaGetMidiDevices_new()
{
    t_ofeliaGetMidiDevices *x = (t_ofeliaGetMidiDevices *)pd_new(ofeliaGetMidiDevices_class);
    x->inDevices_out = outlet_new(&x->x_obj, &s_list);
    x->outDevices_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void sys_get_midi_devs(char *indevlist, int *nindevs,
                       char *outdevlist, int *noutdevs,
                       int maxndev, int devdescsize);

void ofeliaGetMidiDevices_bang(t_ofeliaGetMidiDevices *x)
{
    char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
    int nindevs = 0, noutdevs = 0;
    sys_get_midi_devs(indevlist, &nindevs,
                      outdevlist, &noutdevs,
                      MAXNDEV, DEVDESCSIZE);
    
    /* outlet midiOut devices */
    const int acout = noutdevs;
    t_atom *avout = (t_atom *)getbytes(acout * sizeof(t_atom));
    
    for (int i=0; i<acout; ++i) {
        
        avout[i].a_type = A_SYMBOL;
        avout[i].a_w.w_symbol = gensym(outdevlist + i * DEVDESCSIZE);
    }
    outlet_list(x->outDevices_out, &s_list, acout, avout);
    freebytes(avout, acout * sizeof(t_atom));
    
    /* outlet midiIn devices */
    const int acin = nindevs;
    t_atom *avin = (t_atom *)getbytes(acin * sizeof(t_atom));
    
    for (int i=0; i<acin; ++i) {
        
        avin[i].a_type = A_SYMBOL;
        avin[i].a_w.w_symbol = gensym(indevlist + i * DEVDESCSIZE);
    }
    outlet_list(x->inDevices_out, &s_list, acin, avin);
    freebytes(avin, acin * sizeof(t_atom));
}

void ofeliaGetMidiDevices_free(t_ofeliaGetMidiDevices *x)
{
    outlet_free(x->inDevices_out);
    outlet_free(x->outDevices_out);
}

void ofeliaGetMidiDevices_setup()
{
    ofeliaGetMidiDevices_class = class_new(gensym("ofGetMidiDevices"),
                                           (t_newmethod)ofeliaGetMidiDevices_new,
                                           (t_method)ofeliaGetMidiDevices_free,
                                           sizeof(t_ofeliaGetMidiDevices),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetMidiDevices_class, (t_method)ofeliaGetMidiDevices_bang);
}

/* ________________________________________________________________________________
 * ofSetMidiDevices object methods
 */
static t_class *ofeliaSetMidiDevices_class;

typedef struct _ofeliaSetMidiDevices
{
    t_object x_obj;
    t_atom midiDevices[20];
    
} t_ofeliaSetMidiDevices;

void sys_get_midi_params(int *pnmidiindev, int *pmidiindev,
                         int *pnmidioutdev, int *pmidioutdev);

void *ofeliaSetMidiDevices_new()
{
    t_ofeliaSetMidiDevices *x = (t_ofeliaSetMidiDevices *)pd_new(ofeliaSetMidiDevices_class);
    int nmidiindev, midiindev[MAXMIDIINDEV];
    int nmidioutdev, midioutdev[MAXMIDIOUTDEV];
    sys_get_midi_params(&nmidiindev, midiindev, &nmidioutdev, midioutdev);
    
    for (int i=0; i<20; ++i) {
        
        x->midiDevices[i].a_type = A_FLOAT;
        x->midiDevices[i].a_w.w_float = 0.0f;
    }
    for (int i=0; i<nmidiindev; ++i)
        x->midiDevices[i].a_w.w_float = (t_float)midiindev[i];
    
    for (int i=0; i<nmidioutdev; ++i)
        x->midiDevices[i+9].a_w.w_float = (t_float)midioutdev[i];
    x->midiDevices[18].a_w.w_float = 16.0f;
    x->midiDevices[19].a_w.w_float = 16.0f;
    return (x);
}

void ofeliaSetMidiDevices_bang(t_ofeliaSetMidiDevices *x)
{
    /* midi-dialog message accepts total 20 arguments */
    /* input device 1-9, output device 1-9, numInputChannels, numOutputChannels */
    const t_symbol *pdSym = gensym("pd");
    
    if (pdSym->s_thing)
        pd_typedmess(pdSym->s_thing, gensym("midi-dialog"), 20, x->midiDevices);
}

int sys_mididevnametonumber(int output, const char *name);
void sys_mididevnumbertoname(int output, int devno, char *name, int namesize);

void ofeliaSetMidiDevices_input(t_ofeliaSetMidiDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 0 && argc <= 9) {
        
        int inDevices[9] = {0,0,0,0,0,0,0,0,0};
        char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
        int nindevs = 0, noutdevs = 0;
        sys_get_midi_devs(indevlist, &nindevs,
                          outdevlist, &noutdevs,
                          MAXNDEV, DEVDESCSIZE);
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                inDevices[i] = getDevClampIntValue((int)argv[i].a_w.w_float, 0, nindevs);
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                const char *name = argv[i].a_w.w_symbol->s_name;
                inDevices[i] = sys_mididevnametonumber(0, name) + 1;
                
                if (inDevices[i] < 1) {
                    
                    error("ofSetMidiDevices: couldn't find midi input device: %s", name);
                    return;
                }
            }
            else {
                
                error("ofSetMidiDevices: wrong argument type");
                return;
            }
        }
        for (int i=0; i<9; ++i)
            x->midiDevices[i].a_w.w_float = (t_float)inDevices[i];
    }
    else {
        
        error("ofSetMidiDevices: invalid number of arguments");
    }
}

void ofeliaSetMidiDevices_output(t_ofeliaSetMidiDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 0 && argc <= 9) {
        
        int outDevices[9] = {0,0,0,0,0,0,0,0,0};
        char indevlist[MAXNDEV*DEVDESCSIZE], outdevlist[MAXNDEV*DEVDESCSIZE];
        int nindevs = 0, noutdevs = 0;
        sys_get_midi_devs(indevlist, &nindevs,
                          outdevlist, &noutdevs,
                          MAXNDEV, DEVDESCSIZE);
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                outDevices[i] = getDevClampIntValue((int)argv[i].a_w.w_float, 0, noutdevs);
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                const char *name = argv[i].a_w.w_symbol->s_name;
                outDevices[i] = sys_mididevnametonumber(1, name) + 1;
                
                if (outDevices[i] < 1) {
                    
                    error("ofSetMidiDevices: couldn't find midi output device: %s", name);
                    return;
                }
            }
            else {
                
                error("ofSetMidiDevices: wrong argument type");
                return;
            }
        }
        for (int i=0; i<9; ++i)
            x->midiDevices[i+9].a_w.w_float = (t_float)outDevices[i];
    }
    else {
        
        error("ofSetMidiDevices: invalid number of arguments");
    }
}

void ofeliaSetMidiDevices_print(t_ofeliaSetMidiDevices *x)
{
    post("\n[ofSetMidiDevices]");
    post("input : ");
    char midi_indevnames[MAXMIDIINDEV * DEVDESCSIZE];
    
    for (int i=0; i<9; ++i) {
        
        const int devno = (int)x->midiDevices[i].a_w.w_float - 1;
        
        if (devno >= 0) {
            
            sys_mididevnumbertoname(0, devno, &midi_indevnames[devno * DEVDESCSIZE], DEVDESCSIZE);
            post(" %s", midi_indevnames + devno * DEVDESCSIZE);
        }
    }
    post("output : ");
    char midi_outdevnames[MAXMIDIOUTDEV * DEVDESCSIZE];
    
    for (int i=0; i<9; ++i) {
        
        const int devno = (int)x->midiDevices[i+9].a_w.w_float - 1;
        
        if (devno >= 0) {
            
            sys_mididevnumbertoname(1, devno, &midi_outdevnames[devno * DEVDESCSIZE], DEVDESCSIZE);
            post(" %s", midi_outdevnames + devno * DEVDESCSIZE);
        }
    }
}

void ofeliaSetMidiDevices_setup()
{
    ofeliaSetMidiDevices_class = class_new(gensym("ofSetMidiDevices"),
                                           (t_newmethod)ofeliaSetMidiDevices_new,
                                           0, sizeof(t_ofeliaSetMidiDevices),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSetMidiDevices_class, (t_method)ofeliaSetMidiDevices_bang);
    class_addmethod(ofeliaSetMidiDevices_class, (t_method)ofeliaSetMidiDevices_input, gensym("input"), A_GIMME, 0);
    class_addmethod(ofeliaSetMidiDevices_class, (t_method)ofeliaSetMidiDevices_output, gensym("output"), A_GIMME, 0);
    class_addmethod(ofeliaSetMidiDevices_class, (t_method)ofeliaSetMidiDevices_print, gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaDevices_setup()
{
    ofeliaGetAudioDevices_setup();
    ofeliaSetAudioDevices_setup();
    ofeliaGetMidiDevices_setup();
    ofeliaSetMidiDevices_setup();
}

