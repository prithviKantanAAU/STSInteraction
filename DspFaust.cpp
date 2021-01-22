#define JUCE_DRIVER 1
#define SOUNDFILE 1
/* ------------------------------------------------------------
name: "STS_Synth"
Code generated with Faust 2.22.5 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

/************************************************************************
 ************************************************************************
 FAUST API Architecture File
 Copyright (C) 2016 GRAME, Romain Michon, CCRMA - Stanford University
 Copyright (C) 2014-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------

 This is sample code. This file is provided as an example of minimal
 FAUST architecture file. Redistribution and use in source and binary
 forms, with or without modification, in part or in full are permitted.
 In particular you can create a derived work of this FAUST architecture
 and distribute that work under terms of your choice.

 This sample code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 ************************************************************************
 ************************************************************************/

#include <cmath>
#include <cstring>

/************************** BEGIN misc.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <string>

/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) { int r = 0; while ((1<<r) < x) r++; return r; }

static long lopt(char* argv[], const char* name, long def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
    return def;
}

static long lopt1(int argc, char* argv[], const char* longname, const char* shortname, long def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return atoi(argv[i]);
        }
    }
    return def;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
    return def;
}

static const char* lopts1(int argc, char* argv[], const char* longname, const char* shortname, const char* def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return argv[i];
        }
    }
    return def;
}

static bool isopt(char* argv[], const char* name)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
    return false;
}

static std::string pathToContent(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ifstream::binary);
    
    file.seekg(0, file.end);
    int size = int(file.tellg());
    file.seekg(0, file.beg);
    
    // And allocate buffer to that a single line can be read...
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    
    // Terminate the string
    buffer[size] = 0;
    std::string result = buffer;
    file.close();
    delete [] buffer;
    return result;
}

#endif

/**************************  END  misc.h **************************/
/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN JSONUIDecoder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JSONUIDecoder__
#define __JSONUIDecoder__

#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <sstream>
#include <functional>

/************************** BEGIN CGlue.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef CGLUE_H
#define CGLUE_H

/************************** BEGIN CInterface.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef CINTERFACE_H
#define CINTERFACE_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
struct Soundfile;

/*******************************************************************************
 * UI, Meta and MemoryManager structures for C code.
 ******************************************************************************/

// -- widget's layouts

typedef void (* openTabBoxFun) (void* ui_interface, const char* label);
typedef void (* openHorizontalBoxFun) (void* ui_interface, const char* label);
typedef void (* openVerticalBoxFun) (void* ui_interface, const char* label);
typedef void (* closeBoxFun) (void* ui_interface);

// -- active widgets

typedef void (* addButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addCheckButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addVerticalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addHorizontalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addNumEntryFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

// -- passive widgets

typedef void (* addHorizontalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);
typedef void (* addVerticalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

// -- soundfiles
    
typedef void (* addSoundfileFun) (void* ui_interface, const char* label, const char* url, struct Soundfile** sf_zone);

typedef void (* declareFun) (void* ui_interface, FAUSTFLOAT* zone, const char* key, const char* value);

typedef struct {

    void* uiInterface;

    openTabBoxFun openTabBox;
    openHorizontalBoxFun openHorizontalBox;
    openVerticalBoxFun openVerticalBox;
    closeBoxFun closeBox;
    addButtonFun addButton;
    addCheckButtonFun addCheckButton;
    addVerticalSliderFun addVerticalSlider;
    addHorizontalSliderFun addHorizontalSlider;
    addNumEntryFun addNumEntry;
    addHorizontalBargraphFun addHorizontalBargraph;
    addVerticalBargraphFun addVerticalBargraph;
    addSoundfileFun addSoundfile;
    declareFun declare;

} UIGlue;

typedef void (* metaDeclareFun) (void* ui_interface, const char* key, const char* value);

typedef struct {

    void* metaInterface;
    
    metaDeclareFun declare;

} MetaGlue;

/***************************************
 *  Interface for the DSP object
 ***************************************/

typedef char dsp_imp;
    
typedef dsp_imp* (* newDspFun) ();
typedef void (* destroyDspFun) (dsp_imp* dsp);
typedef int (* getNumInputsFun) (dsp_imp* dsp);
typedef int (* getNumOutputsFun) (dsp_imp* dsp);
typedef void (* buildUserInterfaceFun) (dsp_imp* dsp, UIGlue* ui);
typedef int (* getSampleRateFun) (dsp_imp* dsp);
typedef void (* initFun) (dsp_imp* dsp, int sample_rate);
typedef void (* classInitFun) (int sample_rate);
typedef void (* instanceInitFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceConstantsFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceResetUserInterfaceFun) (dsp_imp* dsp);
typedef void (* instanceClearFun) (dsp_imp* dsp);
typedef void (* computeFun) (dsp_imp* dsp, int len, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs);
typedef void (* metadataFun) (MetaGlue* meta);
    
/***************************************
 * DSP memory manager functions
 ***************************************/

typedef void* (* allocateFun) (void* manager_interface, size_t size);
typedef void (* destroyFun) (void* manager_interface, void* ptr);

typedef struct {
    
    void* managerInterface;
    
    allocateFun allocate;
    destroyFun destroy;
    
} MemoryManagerGlue;

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CInterface.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * UI glue code
 ******************************************************************************/
 
class UIFloat
{

    public:

        UIFloat() {}

        virtual ~UIFloat() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, float* zone) = 0;
        virtual void addCheckButton(const char* label, float* zone) = 0;
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max) = 0;
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(float* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueFloat(void* cpp_interface)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueFloat(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueFloat(void* cpp_interface, float* zone, const char* key, const char* value)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

class UIDouble
{

    public:

        UIDouble() {}

        virtual ~UIDouble() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, double* zone) = 0;
        virtual void addCheckButton(const char* label, double* zone) = 0;
        virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max) = 0;
        virtual void addVerticalBargraph(const char* label, double* zone, double min, double max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(double* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueDouble(void* cpp_interface)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueDouble(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueDouble(void* cpp_interface, double* zone, const char* key, const char* value)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

static void buildUIGlue(UIGlue* glue, UI* ui_interface, bool is_double)
{
    glue->uiInterface = ui_interface;
    
    if (is_double) {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueDouble);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueDouble);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueDouble);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueDouble);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueDouble);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueDouble);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueDouble);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueDouble);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueDouble);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueDouble);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueDouble);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueDouble);
        glue->declare = reinterpret_cast<declareFun>(declareGlueDouble);
    } else {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueFloat);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueFloat);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueFloat);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueFloat);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueFloat);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueFloat);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueFloat);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueFloat);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueFloat);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueFloat);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueFloat);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueFloat);
        glue->declare = reinterpret_cast<declareFun>(declareGlueFloat);
    }
}
    
class UITemplate
{
    
    private:
        
        void* fCPPInterface;
        
    public:
        
        UITemplate(void* cpp_interface):fCPPInterface(cpp_interface)
        {}
        
        virtual ~UITemplate() {}
        
        // -- widget's layouts
        
        virtual void openTabBox(const char* label)
        {
            openTabBoxGlueFloat(fCPPInterface, label);
        }
        virtual void openHorizontalBox(const char* label)
        {
            openHorizontalBoxGlueFloat(fCPPInterface, label);
        }
        virtual void openVerticalBox(const char* label)
        {
            openVerticalBoxGlueFloat(fCPPInterface, label);
        }
        virtual void closeBox()
        {
            closeBoxGlueFloat(fCPPInterface);
        }
        
        // float version
        
        // -- active widgets
        
        virtual void addButton(const char* label, float* zone)
        {
            addButtonGlueFloat(fCPPInterface, label, zone);
        }
        virtual void addCheckButton(const char* label, float* zone)
        {
            addCheckButtonGlueFloat(fCPPInterface, label, zone);
        }
        
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
        {
            addVerticalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
        {
            addHorizontalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
        {
            addNumEntryGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max)
        {
            addHorizontalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
        }
        
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max)
        {
            addVerticalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
        }
    
        // -- metadata declarations
        
        virtual void declare(float* zone, const char* key, const char* val)
        {
            declareGlueFloat(fCPPInterface, zone, key, val);
        }
        
        // double version
        
        virtual void addButton(const char* label, double* zone)
        {
            addButtonGlueDouble(fCPPInterface, label, zone);
        }
        virtual void addCheckButton(const char* label, double* zone)
        {
            addCheckButtonGlueDouble(fCPPInterface, label, zone);
        }
        
        virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step)
        {
            addVerticalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step)
        {
            addHorizontalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step)
        {
            addNumEntryGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
    
        // -- soundfiles
        
        virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
        {
            addSoundfileGlueFloat(fCPPInterface, label, url, sf_zone);
        }
    
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max)
        {
            addHorizontalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
        }
        
        virtual void addVerticalBargraph(const char* label, double* zone, double min, double max)
        {
            addVerticalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
        }
    
        // -- metadata declarations
        
        virtual void declare(double* zone, const char* key, const char* val)
        {
            declareGlueDouble(fCPPInterface, zone, key, val);
        }

};

/*******************************************************************************
 * Meta glue code
 ******************************************************************************/

static void declareMetaGlue(void* cpp_interface, const char* key, const char* value)
{
    Meta* meta_interface = static_cast<Meta*>(cpp_interface);
    meta_interface->declare(key, value);
}

static void buildMetaGlue(MetaGlue* glue, Meta* meta)
{
    glue->metaInterface = meta;
    glue->declare = declareMetaGlue;
}
    
/*******************************************************************************
 * Memory manager glue code
 ******************************************************************************/

static void* allocateMemoryManagerGlue(void* cpp_interface, size_t size)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    return manager_interface->allocate(size);
}
    
static void destroyMemoryManagerGlue(void* cpp_interface, void* ptr)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    manager_interface->destroy(ptr);
}

static void buildManagerGlue(MemoryManagerGlue* glue, dsp_memory_manager* manager)
{
    glue->managerInterface = manager;
    glue->allocate = allocateMemoryManagerGlue;
    glue->destroy = destroyMemoryManagerGlue;
}

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CGlue.h **************************/
/************************** BEGIN SimpleParser.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

// ---------------------------------------------------------------------
//                          Simple Parser
// A parser returns true if it was able to parse what it is
// supposed to parse and advance the pointer. Otherwise it returns false
// and the pointer is not advanced so that another parser can be tried.
// ---------------------------------------------------------------------

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctype.h>

#ifndef _WIN32
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

struct itemInfo {
    std::string type;
    std::string label;
    std::string url;
    std::string address;
    int index;
    double init;
    double min;
    double max;
    double step;
    std::vector<std::pair<std::string, std::string> > meta;
    
    itemInfo():index(0), init(0.), min(0.), max(0.), step(0.)
    {}
};

// ---------------------------------------------------------------------
//                          Elementary parsers
// ---------------------------------------------------------------------

// Report a parsing error
static bool parseError(const char*& p, const char* errmsg)
{
    std::cerr << "Parse error : " << errmsg << " here : " << p << std::endl;
    return true;
}

/**
 * @brief skipBlank : advance pointer p to the first non blank character
 * @param p the string to parse, then the remaining string
 */
static void skipBlank(const char*& p)
{
    while (isspace(*p)) { p++; }
}

// Parse character x, but don't report error if fails
static bool tryChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseChar : parse a specific character x
 * @param p the string to parse, then the remaining string
 * @param x the character to recognize
 * @return true if x was found at the begin of p
 */
static bool parseChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseWord : parse a specific string w
 * @param p the string to parse, then the remaining string
 * @param w the string to recognize
 * @return true if string w was found at the begin of p
 */
static bool parseWord(const char*& p, const char* w)
{
    skipBlank(p);
    const char* saved = p;  // to restore position if we fail
    while ((*w == *p) && (*w)) {++w; ++p;}
    if (*w) {
        p = saved;
        return false;
    } else {
        return true;
    }
}

/**
 * @brief parseDouble : parse number [s]dddd[.dddd] and store the result in x
 * @param p the string to parse, then the remaining string
 * @param x the float number found if any
 * @return true if a float number was found at the begin of p
 */
static bool parseDouble(const char*& p, double& x)
{
    std::stringstream reader(p);
    std::streambuf* pbuf = reader.rdbuf();
    
    // Keep position before parsing
    std::streamsize size1 = pbuf->in_avail();
    
    // Parse the number
    reader >> x;
    
    // Keep position after parsing
    std::streamsize size2 = pbuf->in_avail();
    
    // Move from the actual size
    p += (size1 - size2);
    
    // True if the number contains at least one digit
    return (size1 > size2);
}

/**
 * @brief parseString, parse an arbitrary quoted string q...q and store the result in s
 * @param p the string to parse, then the remaining string
 * @param quote the character used to quote the string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseString(const char*& p, char quote, std::string& s)
{
    std::string str;
    skipBlank(p);
    
    const char* saved = p;  // to restore position if we fail
    if (*p++ == quote) {
        while ((*p != 0) && (*p != quote)) {
            str += *p++;
        }
        if (*p++ == quote) {
            s = str;
            return true;
        }
    }
    p = saved;
    return false;
}

/**
 * @brief parseSQString, parse a single quoted string '...' and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseSQString(const char*& p, std::string& s)
{
    return parseString(p, '\'', s);
}

/**
 * @brief parseDQString, parse a double quoted string "..." and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseDQString(const char*& p, std::string& s)
{
    return parseString(p, '"', s);
}

// ---------------------------------------------------------------------
//
//                          IMPLEMENTATION
// 
// ---------------------------------------------------------------------

/**
 * @brief parseMenuItem, parse a menu item ...'low':440.0...
 * @param p the string to parse, then the remaining string
 * @param name the name found
 * @param value the value found
 * @return true if a nemu item was found
 */
static bool parseMenuItem(const char*& p, std::string& name, double& value)
{
    const char* saved = p;  // to restore position if we fail
    if (parseSQString(p, name) && parseChar(p, ':') && parseDouble(p, value)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

static bool parseMenuItem2(const char*& p, std::string& name)
{
    const char* saved = p;  // to restore position if we fail
    // single quoted
    if (parseSQString(p, name)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

/**
 * @brief parseMenuList, parse a menu list {'low' : 440.0; 'mid' : 880.0; 'hi' : 1760.0}...
 * @param p the string to parse, then the remaining string
 * @param names the vector of names found
 * @param values the vector of values found
 * @return true if a menu list was found
 */
static bool parseMenuList(const char*& p, std::vector<std::string>& names, std::vector<double>& values)
{
    std::vector<std::string> tmpnames;
    std::vector<double> tmpvalues;
    const char* saved = p; // to restore position if we fail

    if (parseChar(p, '{')) {
        do {
            std::string n;
            double v;
            if (parseMenuItem(p, n, v)) {
                tmpnames.push_back(n);
                tmpvalues.push_back(v);
            } else {
                p = saved;
                return false;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            values = tmpvalues;
            return true;
        }
    }
    p = saved;
    return false;
}

static bool parseMenuList2(const char*& p, std::vector<std::string>& names, bool debug)
{
    std::vector<std::string> tmpnames;
    const char* saved = p;  // to restore position if we fail
    
    if (parseChar(p, '{')) {
        do {
            std::string n;
            if (parseMenuItem2(p, n)) {
                tmpnames.push_back(n);
            } else {
                goto error;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            return true;
        }
    }
    
error:
    if (debug) { std::cerr << "parseMenuList2 : (" << saved << ") is not a valid list !\n"; }
    p = saved;
    return false;
}

/// ---------------------------------------------------------------------
// Parse list of strings
/// ---------------------------------------------------------------------
static bool parseList(const char*& p, std::vector<std::string>& items)
{
    const char* saved = p;  // to restore position if we fail
    if (parseChar(p, '[')) {
        do {
            std::string item;
            if (!parseDQString(p, item)) {
                p = saved;
                return false;
            }
            items.push_back(item);
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseMetaData(const char*& p, std::map<std::string, std::string>& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas[metaKey] = metaValue;
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseItemMetaData(const char*& p, std::vector<std::pair<std::string, std::string> >& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas.push_back(std::make_pair(metaKey, metaValue));
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse metadatas of the interface:
// "name" : "...", "inputs" : "...", "outputs" : "...", ...
// and store the result as key/value
/// ---------------------------------------------------------------------
static bool parseGlobalMetaData(const char*& p, std::string& key, std::string& value, double& dbl, std::map<std::string, std::string>& metadatas, std::vector<std::string>& items)
{
    const char* saved = p; // to restore position if we fail
    if (parseDQString(p, key)) {
        if (key == "meta") {
            return parseMetaData(p, metadatas);
        } else {
            return parseChar(p, ':') && (parseDQString(p, value) || parseList(p, items) || parseDouble(p, dbl));
        }
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse gui:
// "type" : "...", "label" : "...", "address" : "...", ...
// and store the result in uiItems Vector
/// ---------------------------------------------------------------------
static bool parseUI(const char*& p, std::vector<itemInfo>& uiItems, int& numItems)
{
    const char* saved = p; // to restore position if we fail
    if (parseChar(p, '{')) {
   
        std::string label;
        std::string value;
        double dbl = 0;
        
        do {
            if (parseDQString(p, label)) {
                if (label == "type") {
                    if (uiItems.size() != 0) {
                        numItems++;
                    }
                    if (parseChar(p, ':') && parseDQString(p, value)) {   
                        itemInfo item;
                        item.type = value;
                        uiItems.push_back(item);
                    }
                }
                
                else if (label == "label") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].label = value;
                    }
                }
                
                else if (label == "url") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].url = value;
                    }
                }
                
                else if (label == "address") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].address = value;
                    }
                }
                
                else if (label == "index") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].index = int(dbl);
                    }
                }
                
                else if (label == "meta") {
                    if (!parseItemMetaData(p, uiItems[numItems].meta)) {
                        return false;
                    }
                }
                
                else if (label == "init") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].init = dbl;
                    }
                }
                
                else if (label == "min") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].min = dbl;
                    }
                }
                
                else if (label == "max") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].max = dbl;
                    }
                }
                
                else if (label == "step") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].step = dbl;
                    }
                }
                
                else if (label == "items") {
                    if (parseChar(p, ':') && parseChar(p, '[')) {
                        do {
                            if (!parseUI(p, uiItems, numItems)) {
                                p = saved;
                                return false;
                            }
                        } while (tryChar(p, ','));
                        if (parseChar(p, ']')) {
                            itemInfo item;
                            item.type = "close";
                            uiItems.push_back(item);
                            numItems++;
                        }
                    }
                }
            } else {
                p = saved;
                return false;
            }
            
        } while (tryChar(p, ','));
    
        return parseChar(p, '}');
    } else {
        return true; // "items": [] is valid
    }
}

// ---------------------------------------------------------------------
// Parse full JSON record describing a JSON/Faust interface :
// {"metadatas": "...", "ui": [{ "type": "...", "label": "...", "items": [...], "address": "...","init": "...", "min": "...", "max": "...","step": "..."}]}
//
// and store the result in map Metadatas and vector containing the items of the interface. Returns true if parsing was successfull.
/// ---------------------------------------------------------------------
static bool parseJson(const char*& p,
                      std::map<std::string, std::pair<std::string, double> >& metaDatas0,
                      std::map<std::string, std::string>& metaDatas1,
                      std::map<std::string, std::vector<std::string> >& metaDatas2,
                      std::vector<itemInfo>& uiItems)
{
    parseChar(p, '{');
    
    do {
        std::string key;
        std::string value;
        double dbl = 0;
        std::vector<std::string> items;
        if (parseGlobalMetaData(p, key, value, dbl, metaDatas1, items)) {
            if (key != "meta") {
                // keep "name", "inputs", "outputs" key/value pairs
                if (items.size() > 0) {
                    metaDatas2[key] = items;
                    items.clear();
                } else if (value != "") {
                    metaDatas0[key].first = value;
                } else {
                    metaDatas0[key].second = dbl;
                }
            }
        } else if (key == "ui") {
            int numItems = 0;
            parseChar(p, '[') && parseUI(p, uiItems, numItems);
        }
    } while (tryChar(p, ','));
    
    return parseChar(p, '}');
}

#endif // SIMPLEPARSER_H
/**************************  END  SimpleParser.h **************************/

#ifdef _WIN32
#include <windows.h>
#define snprintf _snprintf
#endif

//-------------------------------------------------------------------
//  Decode a dsp JSON description and implement 'buildUserInterface'
//-------------------------------------------------------------------

#define REAL_UI(ui_interface) reinterpret_cast<UIReal<REAL>*>(ui_interface)
#define REAL_ADR(offset)      reinterpret_cast<REAL*>(&memory_block[offset])
#define REAL_EXT_ADR(offset)  reinterpret_cast<FAUSTFLOAT*>(&memory_block[offset])
#define SOUNDFILE_ADR(offset) reinterpret_cast<Soundfile**>(&memory_block[offset])

typedef std::function<void(double)> ReflectFunction;
typedef std::function<double()> ModifyFunction;

struct ExtZoneParam {

    virtual void reflectZone() = 0;
    virtual void modifyZone() = 0;
    
    virtual void setReflectZoneFun(ReflectFunction reflect) = 0;
    virtual void setModifyZoneFun(ModifyFunction modify) = 0;
    
    virtual ~ExtZoneParam()
    {}
    
};

template <typename REAL>
struct JSONUIDecoderAux {
    
    struct ZoneParam : public ExtZoneParam {
        
        REAL fZone;
        int fIndex;
        ReflectFunction fReflect;
        ModifyFunction fModify;
        
    #if defined(TARGET_OS_IPHONE) || defined(WIN32)
        ZoneParam(int index, ReflectFunction reflect = nullptr, ModifyFunction modify = nullptr)
        :fIndex(index), fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { if (fReflect) fReflect(fZone); }
        void modifyZone() { if (fModify) fZone = fModify(); }
    #else
        ZoneParam(int index, ReflectFunction reflect = [](REAL value) {}, ModifyFunction modify = []() { return REAL(-1); })
        :fIndex(index), fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { fReflect(fZone); }
        void modifyZone() { fZone = fModify(); }
    #endif
        
        void setReflectZoneFun(ReflectFunction reflect) { fReflect = reflect; }
        void setModifyZoneFun(ModifyFunction modify) { fModify = modify; }
        
    };

    typedef std::vector<ExtZoneParam*> controlMap;
  
    std::string fName;
    std::string fFileName;
    std::string fJSON;
    std::string fVersion;
    std::string fCompileOptions;
    
    std::map<std::string, std::string> fMetadata;
    std::vector<itemInfo> fUiItems;
    
    std::vector<std::string> fLibraryList;
    std::vector<std::string> fIncludePathnames;
    
    Soundfile** fSoundfiles;
    
    int fNumInputs, fNumOutputs, fSRIndex;
    int fSoundfileItems;
    int fDSPSize;
    
    controlMap fPathInputTable;     // [path, ZoneParam]
    controlMap fPathOutputTable;    // [path, ZoneParam]

    bool isInput(const std::string& type)
    {
        return (type == "vslider" || type == "hslider" || type == "nentry" || type == "button" || type == "checkbox");
    }
    bool isOutput(const std::string& type) { return (type == "hbargraph" || type == "vbargraph"); }
    bool isSoundfile(const std::string& type) { return (type == "soundfile"); }
    
    std::string getString(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? map[key].first : "";
    }
    
    int getInt(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? int(map[key].second) : -1;
    }
    
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        fPathInputTable[index]->setReflectZoneFun(fun);
    }
    
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        fPathOutputTable[index]->setModifyZoneFun(fun);
    }

    JSONUIDecoderAux(const std::string& json)
    {
        fJSON = json;
        const char* p = fJSON.c_str();
        std::map<std::string, std::pair<std::string, double> > meta_data1;
        std::map<std::string, std::vector<std::string> > meta_data2;
        parseJson(p, meta_data1, fMetadata, meta_data2, fUiItems);
        
        // meta_data1 contains <name : val>, <inputs : val>, <ouputs : val> pairs etc...
        fName = getString(meta_data1, "name");
        fFileName = getString(meta_data1, "filename");
        fVersion = getString(meta_data1, "version");
        fCompileOptions = getString(meta_data1, "compile_options");
        
        if (meta_data2.find("library_list") != meta_data2.end()) {
            fLibraryList = meta_data2["library_list"];
        }
        if (meta_data2.find("include_pathnames") != meta_data2.end()) {
            fIncludePathnames = meta_data2["include_pathnames"];
        }
        
        fDSPSize = getInt(meta_data1, "size");
        fNumInputs = getInt(meta_data1, "inputs");
        fNumOutputs = getInt(meta_data1, "outputs");
        fSRIndex = getInt(meta_data1, "sr_index");
       
        fSoundfileItems = 0;
        for (auto& it : fUiItems) {
            std::string type = it.type;
            if (isSoundfile(type)) {
                fSoundfileItems++;
            }
        }
        
        fSoundfiles = new Soundfile*[fSoundfileItems];
        
        // Prepare the fPathTable and init zone
        for (auto& it : fUiItems) {
            std::string type = it.type;
            // Meta data declaration for input items
            if (isInput(type)) {
                ZoneParam* param = new ZoneParam(it.index);
                fPathInputTable.push_back(param);
                param->fZone = it.init;
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                ZoneParam* param = new ZoneParam(it.index);
                fPathOutputTable.push_back(param);
                param->fZone = REAL(0);
            }
        }
    }
    
    virtual ~JSONUIDecoderAux()
    {
        delete [] fSoundfiles;
    }
    
    void metadata(Meta* m)
    {
        for (auto& it : fMetadata) {
            m->declare(it.first.c_str(), it.second.c_str());
        }
    }
    
    void metadata(MetaGlue* m)
    {
        for (auto& it : fMetadata) {
            m->declare(m->metaInterface, it.first.c_str(), it.second.c_str());
        }
    }
    
    void resetUserInterface()
    {
        int item = 0;
        for (auto& it : fUiItems) {
            if (isInput(it.type)) {
                static_cast<ZoneParam*>(fPathInputTable[item++])->fZone = it.init;
            }
        }
    }
    
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        for (auto& it : fUiItems) {
            int offset = it.index;
            if (isInput(it.type)) {
                *REAL_ADR(offset) = it.init;
            } else if (isSoundfile(it.type)) {
                if (*SOUNDFILE_ADR(offset) == nullptr) {
                    *SOUNDFILE_ADR(offset) = defaultsound;
                }
            }
        }
    }
    
    int getSampleRate(char* memory_block)
    {
        return *reinterpret_cast<int*>(&memory_block[fSRIndex]);
    }
   
    void buildUserInterface(UI* ui_interface)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        int countIn = 0;
        int countOut = 0;
        int countSound = 0;
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(&static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(&static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                REAL_UI(ui_interface)->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") { 
                REAL_UI(ui_interface)->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                REAL_UI(ui_interface)->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                REAL_UI(ui_interface)->addVerticalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "hslider") {
                REAL_UI(ui_interface)->addHorizontalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "checkbox") {
                REAL_UI(ui_interface)->addCheckButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "soundfile") {
                REAL_UI(ui_interface)->addSoundfile(it.label.c_str(), it.url.c_str(), &fSoundfiles[countSound]);
            } else if (type == "hbargraph") {
                REAL_UI(ui_interface)->addHorizontalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "vbargraph") {
                REAL_UI(ui_interface)->addVerticalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "nentry") {
                REAL_UI(ui_interface)->addNumEntry(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "button") {
                REAL_UI(ui_interface)->addButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "close") {
                REAL_UI(ui_interface)->closeBox();
            }
            
            if (isInput(type)) {
                countIn++;
            } else if (isOutput(type)) {
                countOut++;
            } else if (isSoundfile(type)) {
                countSound++;
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            int offset = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                REAL_UI(ui_interface)->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") {
                REAL_UI(ui_interface)->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                REAL_UI(ui_interface)->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                REAL_UI(ui_interface)->addVerticalSlider(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "hslider") {
                REAL_UI(ui_interface)->addHorizontalSlider(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "checkbox") {
                REAL_UI(ui_interface)->addCheckButton(it.label.c_str(), REAL_ADR(offset));
            } else if (type == "soundfile") {
                REAL_UI(ui_interface)->addSoundfile(it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(offset));
            } else if (type == "hbargraph") {
                REAL_UI(ui_interface)->addHorizontalBargraph(it.label.c_str(), REAL_ADR(offset), min, max);
            } else if (type == "vbargraph") {
                REAL_UI(ui_interface)->addVerticalBargraph(it.label.c_str(), REAL_ADR(offset), min, max);
            } else if (type == "nentry") {
                REAL_UI(ui_interface)->addNumEntry(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "button") {
                REAL_UI(ui_interface)->addButton(it.label.c_str(), REAL_ADR(offset));
            } else if (type == "close") {
                REAL_UI(ui_interface)->closeBox();
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            int offset = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, 0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                ui_interface->openHorizontalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vgroup") {
                ui_interface->openVerticalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "tgroup") {
                ui_interface->openTabBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vslider") {
                ui_interface->addVerticalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "hslider") {
                ui_interface->addHorizontalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "checkbox") {
                ui_interface->addCheckButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset));
            } else if (type == "soundfile") {
                ui_interface->addSoundfile(ui_interface->uiInterface, it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(offset));
            } else if (type == "hbargraph") {
                ui_interface->addHorizontalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), min, max);
            } else if (type == "vbargraph") {
                ui_interface->addVerticalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), min, max);
            } else if (type == "nentry") {
                ui_interface->addNumEntry(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "button") {
                ui_interface->addButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset));
            } else if (type == "close") {
                ui_interface->closeBox(ui_interface->uiInterface);
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    bool hasCompileOption(const std::string& option)
    {
        std::istringstream iss(fCompileOptions);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            if (token == option) return true;
        }
        return false;
    }
    
};

// Templated decoder

struct JSONUITemplatedDecoder
{

    virtual ~JSONUITemplatedDecoder()
    {}
    
    virtual void metadata(Meta* m) = 0;
    virtual void metadata(MetaGlue* glue) = 0;
    virtual int getDSPSize() = 0;
    virtual std::string getName() = 0;
    virtual std::string getLibVersion() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;
    virtual int getNumInputs() = 0;
    virtual int getNumOutputs() = 0;
    virtual int getSampleRate(char* memory_block) = 0;
    virtual void setReflectZoneFun(int index, ReflectFunction fun) = 0;
    virtual void setModifyZoneFun(int index, ModifyFunction fun) = 0;
    virtual std::vector<ExtZoneParam*>& getInputControls() = 0;
    virtual std::vector<ExtZoneParam*>& getOutputControls() = 0;
    virtual void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr) = 0;
    virtual void buildUserInterface(UI* ui_interface) = 0;
    virtual void buildUserInterface(UI* ui_interface, char* memory_block) = 0;
    virtual void buildUserInterface(UIGlue* ui_interface, char* memory_block) = 0;
    virtual bool hasCompileOption(const std::string& option) = 0;
};

struct JSONUIFloatDecoder : public JSONUIDecoderAux<float>, public JSONUITemplatedDecoder
{
    JSONUIFloatDecoder(const std::string& json):JSONUIDecoderAux<float>(json)
    {}
    
    void metadata(Meta* m) { JSONUIDecoderAux<float>::metadata(m); }
    void metadata(MetaGlue* glue) { JSONUIDecoderAux<float>::metadata(glue); }
    int getDSPSize() { return fDSPSize; }
    std::string getName() { return fName; }
    std::string getLibVersion() { return fVersion; }
    std::string getCompileOptions() { return fCompileOptions; }
    std::vector<std::string> getLibraryList() { return fLibraryList; }
    std::vector<std::string> getIncludePathnames() { return fIncludePathnames; }
    int getNumInputs() { return fNumInputs; }
    int getNumOutputs() { return fNumOutputs; }
    int getSampleRate(char* memory_block)  { return JSONUIDecoderAux<float>::getSampleRate(memory_block); }
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        JSONUIDecoderAux<float>::setReflectZoneFun(index, fun);
    }
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        JSONUIDecoderAux<float>::setModifyZoneFun(index, fun);
    }
    std::vector<ExtZoneParam*>& getInputControls()
    {
        return fPathInputTable;
    }
    std::vector<ExtZoneParam*>& getOutputControls()
    {
        return fPathOutputTable;
    }
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        JSONUIDecoderAux<float>::resetUserInterface(memory_block, defaultsound);
    }
    void buildUserInterface(UI* ui_interface)
    {
        JSONUIDecoderAux<float>::buildUserInterface(ui_interface);
    }
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        JSONUIDecoderAux<float>::buildUserInterface(ui_interface, memory_block);
    }
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        JSONUIDecoderAux<float>::buildUserInterface(ui_interface, memory_block);
    }
    bool hasCompileOption(const std::string& option) { return JSONUIDecoderAux<float>::hasCompileOption(option); }
};

struct JSONUIDoubleDecoder : public JSONUIDecoderAux<double>, public JSONUITemplatedDecoder
{
    JSONUIDoubleDecoder(const std::string& json):JSONUIDecoderAux<double>(json)
    {}
    
    void metadata(Meta* m) { JSONUIDecoderAux<double>::metadata(m); }
    void metadata(MetaGlue* glue) { JSONUIDecoderAux<double>::metadata(glue); }
    int getDSPSize() { return fDSPSize; }
    std::string getName() { return fName; }
    std::string getLibVersion() { return fVersion; }
    std::string getCompileOptions() { return fCompileOptions; }
    std::vector<std::string> getLibraryList() { return fLibraryList; }
    std::vector<std::string> getIncludePathnames() { return fIncludePathnames; }
    int getNumInputs() { return fNumInputs; }
    int getNumOutputs() { return fNumOutputs; }
    int getSampleRate(char* memory_block) { return JSONUIDecoderAux<double>::getSampleRate(memory_block); }
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        JSONUIDecoderAux<double>::setReflectZoneFun(index, fun);
    }
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        JSONUIDecoderAux<double>::setModifyZoneFun(index, fun);
    }
    std::vector<ExtZoneParam*>& getInputControls()
    {
        return fPathInputTable;
    }
    std::vector<ExtZoneParam*>& getOutputControls()
    {
        return fPathOutputTable;
    }
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        JSONUIDecoderAux<double>::resetUserInterface(memory_block, defaultsound);
    }
    void buildUserInterface(UI* ui_interface)
    {
        JSONUIDecoderAux<double>::buildUserInterface(ui_interface);
    }
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        JSONUIDecoderAux<double>::buildUserInterface(ui_interface, memory_block);
    }
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        JSONUIDecoderAux<double>::buildUserInterface(ui_interface, memory_block);
    }
    bool hasCompileOption(const std::string& option) { return JSONUIDecoderAux<double>::hasCompileOption(option); }
};

// FAUSTFLOAT decoder

struct JSONUIDecoder : public JSONUIDecoderAux<FAUSTFLOAT>
{
    JSONUIDecoder(const std::string& json):JSONUIDecoderAux<FAUSTFLOAT>(json)
    {}
};

static JSONUITemplatedDecoder* createJSONUIDecoder(const std::string& json)
{
    JSONUIDecoder decoder(json);
    if (decoder.hasCompileOption("-double")) {
        return new JSONUIDoubleDecoder(json);
    } else {
        return new JSONUIFloatDecoder(json);
    }
}

#endif
/**************************  END  JSONUIDecoder.h **************************/
/************************** BEGIN dsp-adapter.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_adapter__
#define __dsp_adapter__

#include <string.h>
#include <iostream>
#include <cmath>


// Adapts a DSP for a different number of inputs/outputs
class dsp_adapter : public decorator_dsp {
    
    private:
        
        FAUSTFLOAT** fAdaptedInputs;
        FAUSTFLOAT** fAdaptedOutputs;
        int fHardwareInputs;
        int fHardwareOutputs;
        
        void adaptBuffers(FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int i = 0; i < fHardwareInputs; i++) {
                fAdaptedInputs[i] = inputs[i];
            }
            for (int i = 0; i < fHardwareOutputs; i++) {
                fAdaptedOutputs[i] = outputs[i];
            }
        }
        
    public:
        
        dsp_adapter(dsp* dsp, int hardware_inputs, int hardware_outputs, int buffer_size):decorator_dsp(dsp)
        {
            fHardwareInputs = hardware_inputs;
            fHardwareOutputs = hardware_outputs;
             
            fAdaptedInputs = new FAUSTFLOAT*[dsp->getNumInputs()];
            for (int i = 0; i < dsp->getNumInputs() - fHardwareInputs; i++) {
                fAdaptedInputs[i + fHardwareInputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedInputs[i + fHardwareInputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
            
            fAdaptedOutputs = new FAUSTFLOAT*[dsp->getNumOutputs()];
            for (int i = 0; i < dsp->getNumOutputs() - fHardwareOutputs; i++) {
                fAdaptedOutputs[i + fHardwareOutputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedOutputs[i + fHardwareOutputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
        }
        
        virtual ~dsp_adapter()
        {
            for (int i = 0; i < fDSP->getNumInputs() - fHardwareInputs; i++) {
                delete [] fAdaptedInputs[i + fHardwareInputs];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs() - fHardwareOutputs; i++) {
                delete [] fAdaptedOutputs[i + fHardwareOutputs];
            }
            delete [] fAdaptedOutputs;
        }
    
        virtual int getNumInputs() { return fHardwareInputs; }
        virtual int getNumOutputs() { return fHardwareOutputs; }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(date_usec, count, fAdaptedInputs, fAdaptedOutputs);
        }
        
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(count, fAdaptedInputs, fAdaptedOutputs);
        }
};

// Adapts a DSP for a different sample size
template <typename TYPE_INT, typename TYPE_EXT>
class dsp_sample_adapter : public decorator_dsp {
    
    protected:
    
        TYPE_INT** fAdaptedInputs;
        TYPE_INT** fAdaptedOutputs;
    
        void adaptInputBuffers(int count, FAUSTFLOAT** inputs)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    fAdaptedInputs[chan][frame] = TYPE_INT(reinterpret_cast<TYPE_EXT**>(inputs)[chan][frame]);
                }
            }
        }
    
        void adaptOutputsBuffers(int count, FAUSTFLOAT** outputs)
        {
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    reinterpret_cast<TYPE_EXT**>(outputs)[chan][frame] = TYPE_EXT(fAdaptedOutputs[chan][frame]);
                }
            }
        }
    
    public:
    
        dsp_sample_adapter(dsp* dsp):decorator_dsp(dsp)
        {
            fAdaptedInputs = new TYPE_INT*[dsp->getNumInputs()];
            for (int i = 0; i < dsp->getNumInputs(); i++) {
                fAdaptedInputs[i] = new TYPE_INT[4096];
            }
            
            fAdaptedOutputs = new TYPE_INT*[dsp->getNumOutputs()];
            for (int i = 0; i < dsp->getNumOutputs(); i++) {
                fAdaptedOutputs[i] = new TYPE_INT[4096];
            }
        }
    
        virtual ~dsp_sample_adapter()
        {
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                delete [] fAdaptedInputs[i];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                delete [] fAdaptedOutputs[i];
            }
            delete [] fAdaptedOutputs;
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses TYPE_INT
            fDSP->compute(count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
        }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses TYPE_INT
            fDSP->compute(date_usec, count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
       }
};

// Template used to specialize double parameters expressed as NUM/DENOM
template <int NUM, int DENOM>
struct Double {
    static constexpr double value() { return double(NUM)/double(DENOM); }
};

// Base class for filters
template <class fVslider0, int fVslider1>
struct Filter {
    inline int getFactor() { return fVslider1; }
};

// Generated with process = fi.lowpass(3, ma.SR*vslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/vslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3 : public Filter<fVslider0, fVslider1> {

    REAL fVec0[2];
    REAL fRec1[2];
    REAL fRec0[3];

    inline REAL mydsp_faustpower2_f(REAL value)
    {
        return (value * value);
    }

    LowPass3()
    {
        for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
            fVec0[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }

    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 1.0000000000000002) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (fSlow1 + 1.0));
        REAL fSlow4 = (1.0 - fSlow1);
        REAL fSlow5 = (((fSlow1 + -1.0000000000000002) / fSlow0) + 1.0);
        REAL fSlow6 = (2.0 * (1.0 - (1.0 / mydsp_faustpower2_f(fSlow0))));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            REAL fTemp0 = REAL(input0[i]);
            fVec0[0] = fTemp0;
            fRec1[0] = (0.0 - (fSlow3 * ((fSlow4 * fRec1[1]) - (fTemp0 + fVec0[1]))));
            fRec0[0] = (fRec1[0] - (fSlow2 * ((fSlow5 * fRec0[2]) + (fSlow6 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fVec0[1] = fVec0[0];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass(4, ma.SR*vslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/vslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass4 : public Filter<fVslider0, fVslider1> {
    
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL mydsp_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass4()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0f;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec0[l1] = 0.0f;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.76536686473017945) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (((fSlow1 + 1.8477590650225735) / fSlow0) + 1.0));
        REAL fSlow4 = (((fSlow1 + -1.8477590650225735) / fSlow0) + 1.0);
        REAL fSlow5 = (2.0 * (1.0 - (1.0 / mydsp_faustpower2_f(fSlow0))));
        REAL fSlow6 = (((fSlow1 + -0.76536686473017945) / fSlow0) + 1.0);
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow3 * ((fSlow4 * fRec1[2]) + (fSlow5 * fRec1[1]))));
            fRec0[0] = ((fSlow3 * (fRec1[2] + (fRec1[0] + (2.0 * fRec1[1])))) - (fSlow2 * ((fSlow6 * fRec0[2]) + (fSlow5 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass3e(ma.SR*vslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/vslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3e : public Filter<fVslider0, fVslider1> {

    REAL fRec1[3];
    REAL fVec0[2];
    REAL fRec0[2];
    
    inline REAL mydsp_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass3e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fVec0[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (fSlow1 + 0.82244590899881598));
        REAL fSlow3 = (0.82244590899881598 - fSlow1);
        REAL fSlow4 = (1.0 / (((fSlow1 + 0.80263676416103003) / fSlow0) + 1.4122708937742039));
        REAL fSlow5 = mydsp_faustpower2_f(fSlow0);
        REAL fSlow6 = (0.019809144837788999 / fSlow5);
        REAL fSlow7 = (fSlow6 + 1.1615164189826961);
        REAL fSlow8 = (((fSlow1 + -0.80263676416103003) / fSlow0) + 1.4122708937742039);
        REAL fSlow9 = (2.0 * (1.4122708937742039 - (1.0 / fSlow5)));
        REAL fSlow10 = (2.0 * (1.1615164189826961 - fSlow6));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow4 * ((fSlow8 * fRec1[2]) + (fSlow9 * fRec1[1]))));
            REAL fTemp0 = (fSlow4 * (((fSlow7 * fRec1[0]) + (fSlow10 * fRec1[1])) + (fSlow7 * fRec1[2])));
            fVec0[0] = fTemp0;
            fRec0[0] = (0.0 - (fSlow2 * ((fSlow3 * fRec0[1]) - (fTemp0 + fVec0[1]))));
            output0[i] = FAUSTFLOAT(fRec0[0]);
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fVec0[1] = fVec0[0];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass6e(ma.SR*vslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/vslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass6e : public Filter<fVslider0, fVslider1> {

    REAL fRec2[3];
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL mydsp_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass6e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec2[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.16840487111358901) / fSlow0) + 1.0693584077073119));
        REAL fSlow3 = mydsp_faustpower2_f(fSlow0);
        REAL fSlow4 = (1.0 / fSlow3);
        REAL fSlow5 = (fSlow4 + 53.536152954556727);
        REAL fSlow6 = (1.0 / (((fSlow1 + 0.51247864188914105) / fSlow0) + 0.68962136448467504));
        REAL fSlow7 = (fSlow4 + 7.6217312988706034);
        REAL fSlow8 = (1.0 / (((fSlow1 + 0.78241304682164503) / fSlow0) + 0.24529150870616001));
        REAL fSlow9 = (9.9999997054999994e-05 / fSlow3);
        REAL fSlow10 = (fSlow9 + 0.00043322720055500002);
        REAL fSlow11 = (((fSlow1 + -0.78241304682164503) / fSlow0) + 0.24529150870616001);
        REAL fSlow12 = (2.0 * (0.24529150870616001 - fSlow4));
        REAL fSlow13 = (2.0 * (0.00043322720055500002 - fSlow9));
        REAL fSlow14 = (((fSlow1 + -0.51247864188914105) / fSlow0) + 0.68962136448467504);
        REAL fSlow15 = (2.0 * (0.68962136448467504 - fSlow4));
        REAL fSlow16 = (2.0 * (7.6217312988706034 - fSlow4));
        REAL fSlow17 = (((fSlow1 + -0.16840487111358901) / fSlow0) + 1.0693584077073119);
        REAL fSlow18 = (2.0 * (1.0693584077073119 - fSlow4));
        REAL fSlow19 = (2.0 * (53.536152954556727 - fSlow4));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec2[0] = (REAL(input0[i]) - (fSlow8 * ((fSlow11 * fRec2[2]) + (fSlow12 * fRec2[1]))));
            fRec1[0] = ((fSlow8 * (((fSlow10 * fRec2[0]) + (fSlow13 * fRec2[1])) + (fSlow10 * fRec2[2]))) - (fSlow6 * ((fSlow14 * fRec1[2]) + (fSlow15 * fRec1[1]))));
            fRec0[0] = ((fSlow6 * (((fSlow7 * fRec1[0]) + (fSlow16 * fRec1[1])) + (fSlow7 * fRec1[2]))) - (fSlow2 * ((fSlow17 * fRec0[2]) + (fSlow18 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (((fSlow5 * fRec0[0]) + (fSlow19 * fRec0[1])) + (fSlow5 * fRec0[2]))));
            fRec2[2] = fRec2[1];
            fRec2[1] = fRec2[0];
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// A "si.bus(N)" like hard-coded class
struct dsp_bus : public dsp {
    
    int fChannels;
    int fSampleRate;
    
    dsp_bus(int channels):fChannels(channels), fSampleRate(-1)
    {}
    
    virtual int getNumInputs() { return fChannels; }
    virtual int getNumOutputs() { return fChannels; }
    
    virtual int getSampleRate() { return fSampleRate; }
    
    virtual void buildUserInterface(UI* ui_interface) {}
    virtual void init(int sample_rate)
    {
        //classInit(sample_rate);
        instanceInit(sample_rate);
    }
    
    virtual void instanceInit(int sample_rate)
    {
        fSampleRate = sample_rate;
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }
    
    virtual void instanceConstants(int sample_rate) {}
    virtual void instanceResetUserInterface() {}
    virtual void instanceClear() {}
    
    virtual dsp* clone() { return new dsp_bus(fChannels); }
    
    virtual void metadata(Meta* m) {}
    
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        for (int chan = 0; chan < fChannels; chan++) {
            memcpy(outputs[chan], inputs[chan], sizeof(FAUSTFLOAT) * count);
        }
    }
    
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        compute(count, inputs, outputs);
    }
    
};

// Base class for sample-rate adapter
template <typename FILTER>
class sr_sampler : public decorator_dsp {

    protected:
    
        std::vector<FILTER> fInputLowPass;
        std::vector<FILTER> fOutputLowPass;
    
        inline int getFactor() { return this->fOutputLowPass[0].getFactor(); }
    
    public:
    
        sr_sampler(dsp* dsp):decorator_dsp(dsp)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                fInputLowPass.push_back(FILTER());
            }
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                fOutputLowPass.push_back(FILTER());
            }
        }
 };

// Down sample-rate adapter
template <typename FILTER>
class dsp_down_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_down_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate / this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate / this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate / this->getFactor());
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count / this->getFactor();
           
            // Adapt inputs
            FAUSTFLOAT* fInputs[this->fDSP->getNumInputs()];
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Lowpass filtering in place on 'inputs'
                this->fInputLowPass[chan].compute(count, inputs[chan], inputs[chan]);
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Decimate
                for (int frame = 0; frame < real_count; frame++) {
                    fInputs[chan][frame] = inputs[chan][frame * this->getFactor()];
                }
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT* fOutputs[this->fDSP->getNumOutputs()];
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at lower rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Puts zeros
                memset(outputs[chan], 0, sizeof(FAUSTFLOAT) * count);
                for (int frame = 0; frame < real_count; frame++) {
                    // Copy one sample every 'DownFactor'
                    outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame] * this->getFactor();
                    //outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame];
                }
                // Lowpass filtering in place on 'outputs'
                this->fOutputLowPass[chan].compute(count, outputs[chan], outputs[chan]);
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

// Up sample-rate adapter
template <typename FILTER>
class dsp_up_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_up_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate * this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate * this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate * this->getFactor());
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count * this->getFactor();
            
            // Adapt inputs
            FAUSTFLOAT* fInputs[this->fDSP->getNumInputs()];
            
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Puts zeros
                memset(fInputs[chan], 0, sizeof(FAUSTFLOAT) * real_count);
                for (int frame = 0; frame < count; frame++) {
                    // Copy one sample every 'UpFactor'
                    fInputs[chan][frame * this->getFactor()] = inputs[chan][frame];
                }
                // Lowpass filtering in place on 'fInputs'
                this->fInputLowPass[chan].compute(real_count, fInputs[chan], fInputs[chan]);
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT* fOutputs[this->fDSP->getNumOutputs()];
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at upper rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Lowpass filtering in place on 'fOutputs'
                this->fOutputLowPass[chan].compute(real_count, fOutputs[chan], fOutputs[chan]);
                // Decimate
                for (int frame = 0; frame < count; frame++) {
                    outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()] * this->getFactor();
                    //outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()];
                }
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

#endif
/**************************  END  dsp-adapter.h **************************/

//**************************************************************
// Soundfile handling
//**************************************************************

// Must be done before <<includeclass>> otherwise the 'Soundfile' type is not known

#if SOUNDFILE
// So that the code uses JUCE audio file loading code
#if JUCE_DRIVER
#define JUCE_64BIT 1
#endif
/************************** BEGIN SoundUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __SoundUI_H__
#define __SoundUI_H__

#include <map>
#include <vector>
#include <string>
#include <iostream>

/************************** BEGIN DecoratorUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef Decorator_UI_H
#define Decorator_UI_H


//----------------------------------------------------------------
//  Generic UI empty implementation
//----------------------------------------------------------------

class GenericUI : public UI
{
    
    public:
        
        GenericUI() {}
        virtual ~GenericUI() {}
        
        // -- widget's layouts
        virtual void openTabBox(const char* label) {}
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* soundpath, Soundfile** sf_zone) {}
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
    
};

//----------------------------------------------------------------
//  Generic UI decorator
//----------------------------------------------------------------

class DecoratorUI : public UI
{
    
    protected:
        
        UI* fUI;
        
    public:
        
        DecoratorUI(UI* ui = 0):fUI(ui) {}
        virtual ~DecoratorUI() { delete fUI; }
        
        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addNumEntry(label, zone, init, min, max, step); }
        
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addVerticalBargraph(label, zone, min, max); }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) { fUI->addSoundfile(label, filename, sf_zone); }
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }
    
};

#endif
/**************************  END  DecoratorUI.h **************************/

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

// Always included otherwise -i mode later on will not always include it (with the conditional includes)
/************************** BEGIN Soundfile.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __Soundfile__
#define __Soundfile__

#include <iostream>
#include <string.h>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#define BUFFER_SIZE 16384
#define SAMPLE_RATE 44100
#define MAX_CHAN 64
#define MAX_SOUNDFILE_PARTS 256

#ifdef _MSC_VER
#define PRE_PACKED_STRUCTURE __pragma(pack(push, 1))
#define POST_PACKED_STRUCTURE \
    ;                         \
    __pragma(pack(pop))
#else
#define PRE_PACKED_STRUCTURE
#define POST_PACKED_STRUCTURE __attribute__((__packed__))
#endif

/*
 The soundfile structure to be used by the DSP code. Soundfile has a MAX_SOUNDFILE_PARTS parts 
 (even a single soundfile or an empty soundfile). 
 fLength, fOffset and fSR fields are filled accordingly by repeating
 the actual parts if needed.
 
 It has to be 'packed' to that the LLVM backend can correctly access it.

 Index computation:
    - p is the current part number [0..MAX_SOUNDFILE_PARTS-1] (must be proved by the type system)
    - i is the current position in the part. It will be constrained between [0..length]
    - idx(p,i) = fOffset[p] + max(0, min(i, fLength[p]));
*/

PRE_PACKED_STRUCTURE
struct Soundfile {
    FAUSTFLOAT** fBuffers;
    int* fLength;   // length of each part
    int* fSR;       // sample rate of each part
    int* fOffset;   // offset of each part in the global buffer
    int fChannels;  // max number of channels of all concatenated files

    Soundfile()
    {
        fBuffers  = NULL;
        fChannels = -1;
        fLength   = new int[MAX_SOUNDFILE_PARTS];
        fSR       = new int[MAX_SOUNDFILE_PARTS];
        fOffset   = new int[MAX_SOUNDFILE_PARTS];
    }

    ~Soundfile()
    {
        // Free the real channels only
        for (int chan = 0; chan < fChannels; chan++) {
            delete fBuffers[chan];
        }
        delete[] fBuffers;
        delete[] fLength;
        delete[] fSR;
        delete[] fOffset;
    }

} POST_PACKED_STRUCTURE;

/*
 The generic soundfile reader.
 */

class SoundfileReader {
    
   protected:
    
    int fDriverSR;
    
    void emptyFile(Soundfile* soundfile, int part, int& offset)
    {
        soundfile->fLength[part] = BUFFER_SIZE;
        soundfile->fSR[part] = SAMPLE_RATE;
        soundfile->fOffset[part] = offset;
        // Update offset
        offset += soundfile->fLength[part];
    }

    Soundfile* createSoundfile(int cur_chan, int length, int max_chan)
    {
        Soundfile* soundfile = new Soundfile();
        if (!soundfile) {
            throw std::bad_alloc();
        }
        
        soundfile->fBuffers = new FAUSTFLOAT*[max_chan];
        if (!soundfile->fBuffers) {
            throw std::bad_alloc();
        }
        
        for (int chan = 0; chan < cur_chan; chan++) {
            soundfile->fBuffers[chan] = new FAUSTFLOAT[length];
            if (!soundfile->fBuffers[chan]) {
                throw std::bad_alloc();
            }
            memset(soundfile->fBuffers[chan], 0, sizeof(FAUSTFLOAT) * length);
        }
        
        soundfile->fChannels = cur_chan;
        return soundfile;
    }

    void getBuffersOffset(Soundfile* soundfile, FAUSTFLOAT** buffers, int offset)
    {
        for (int chan = 0; chan < soundfile->fChannels; chan++) {
            buffers[chan] = &soundfile->fBuffers[chan][offset];
        }
    }
    
    // Check if a soundfile exists and return its real path_name
    std::string checkFile(const std::vector<std::string>& sound_directories, const std::string& file_name)
    {
        if (checkFile(file_name)) {
            return file_name;
        } else {
            for (size_t i = 0; i < sound_directories.size(); i++) {
                std::string path_name = sound_directories[i] + "/" + file_name;
                if (checkFile(path_name)) { return path_name; }
            }
            return "";
        }
    }
    
    bool isResampling(int sample_rate) { return (fDriverSR > 0 && fDriverSR != sample_rate); }
 
    // To be implemented by subclasses

    /**
     * Check the availability of a sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     *
     * @return true if the sound resource is available, false otherwise.
     */
    virtual bool checkFile(const std::string& path_name) = 0;
    
    /**
     * Check the availability of a sound resource.
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     *
     * @return true if the sound resource is available, false otherwise.
     */

    virtual bool checkFile(unsigned char* buffer, size_t length) { return true; }

    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(const std::string& path_name, int& channels, int& length) = 0;
    
    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(unsigned char* buffer, size_t size, int& channels, int& length) {}

    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan) = 0;
    
    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, unsigned char* buffer, size_t length, int part, int& offset, int max_chan) {}

  public:
    
    virtual ~SoundfileReader() {}
    
    void setSampleRate(int sample_rate) { fDriverSR = sample_rate; }
   
    Soundfile* createSoundfile(const std::vector<std::string>& path_name_list, int max_chan)
    {
        try {
            int cur_chan = 1; // At least one buffer
            int total_length = 0;
            
            // Compute total length and channels max of all files
            for (int i = 0; i < int(path_name_list.size()); i++) {
                int chan, length;
                if (path_name_list[i] == "__empty_sound__") {
                    length = BUFFER_SIZE;
                    chan = 1;
                } else {
                    getParamsFile(path_name_list[i], chan, length);
                }
                cur_chan = std::max<int>(cur_chan, chan);
                total_length += length;
            }
           
            // Complete with empty parts
            total_length += (MAX_SOUNDFILE_PARTS - path_name_list.size()) * BUFFER_SIZE;
            
            // Create the soundfile
            Soundfile* soundfile = createSoundfile(cur_chan, total_length, max_chan);
            
            // Init offset
            int offset = 0;
            
            // Read all files
            for (int i = 0; i < int(path_name_list.size()); i++) {
                if (path_name_list[i] == "__empty_sound__") {
                    emptyFile(soundfile, i, offset);
                } else {
                    readFile(soundfile, path_name_list[i], i, offset, max_chan);
                }
            }
            
            // Complete with empty parts
            for (int i = int(path_name_list.size()); i < MAX_SOUNDFILE_PARTS; i++) {
                emptyFile(soundfile, i, offset);
            }
            
            // Share the same buffers for all other channels so that we have max_chan channels available
            for (int chan = cur_chan; chan < max_chan; chan++) {
                soundfile->fBuffers[chan] = soundfile->fBuffers[chan % cur_chan];
            }
            
            return soundfile;
            
        } catch (...) {
            return NULL;
        }
    }

    // Check if all soundfiles exist and return their real path_name
    std::vector<std::string> checkFiles(const std::vector<std::string>& sound_directories,
                                        const std::vector<std::string>& file_name_list)
    {
        std::vector<std::string> path_name_list;
        for (size_t i = 0; i < file_name_list.size(); i++) {
            std::string path_name = checkFile(sound_directories, file_name_list[i]);
            // If 'path_name' is not found, it is replaced by an empty sound (= silence)
            path_name_list.push_back((path_name == "") ? "__empty_sound__" : path_name);
        }
        return path_name_list;
    }

};

#endif
/**************************  END  Soundfile.h **************************/

#if defined(JUCE_32BIT) || defined(JUCE_64BIT)
/************************** BEGIN JuceReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JuceReader__
#define __JuceReader__

#include <assert.h>

#include "../JuceLibraryCode/JuceHeader.h"


struct JuceReader : public SoundfileReader {
    
    AudioFormatManager fFormatManager;
    
    JuceReader() { fFormatManager.registerBasicFormats(); }
    virtual ~JuceReader()
    {}
    
    bool checkFile(const std::string& path_name)
    {
        File file(path_name);
        if (file.existsAsFile()) {
            return true;
        } else {
            std::cerr << "ERROR : cannot open '" << path_name << "'" << std::endl;
            return false;
        }
    }
    
    void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        std::unique_ptr<AudioFormatReader> formatReader (fFormatManager.createReaderFor (File (path_name)));
        channels = int(formatReader->numChannels);
        length = int(formatReader->lengthInSamples);
    }
    
    void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        std::unique_ptr<AudioFormatReader> formatReader (fFormatManager.createReaderFor (File (path_name)));
        
        soundfile->fLength[part] = int(formatReader->lengthInSamples);
        soundfile->fSR[part] = int(formatReader->sampleRate);
        soundfile->fOffset[part] = offset;
        
        FAUSTFLOAT** buffers = static_cast<FAUSTFLOAT**>(alloca(soundfile->fChannels * sizeof(FAUSTFLOAT*)));
                                                             
        getBuffersOffset(soundfile, buffers, offset);
        
        if (formatReader->read(reinterpret_cast<int *const *>(buffers), int(formatReader->numChannels), 0, int(formatReader->lengthInSamples), false)) {
            
            // Possibly concert samples
            if (!formatReader->usesFloatingPointData) {
                for (int chan = 0; chan < int(formatReader->numChannels); ++chan) {
                    FAUSTFLOAT* buffer = &soundfile->fBuffers[chan][soundfile->fOffset[part]];
                    FloatVectorOperations::convertFixedToFloat(buffer, reinterpret_cast<const int*>(buffer), 1.0f/0x7fffffff, int(formatReader->lengthInSamples));
                }
            }
            
        } else {
            std::cerr << "Error reading the file : " << path_name << std::endl;
        }
            
        // Update offset
        offset += soundfile->fLength[part];
    }
    
};

#endif
/**************************  END  JuceReader.h **************************/
JuceReader gReader;
#elif defined(MEMORY_READER)
/************************** BEGIN MemoryReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __MemoryReader__
#define __MemoryReader__


/*
 A 'MemoryReader' object can be used to prepare a set of sound resources in memory, to be used by SoundUI::addSoundfile.
 
 A Soundfile* object will have to be filled with a list of sound resources: the fLength, fOffset, fSampleRate and fBuffers fields 
 have to be completed with the appropriate values, and will be accessed in the DSP object while running.
 *
 */

// To adapt for a real case use

#define SOUND_CHAN      2
#define SOUND_LENGTH    4096
#define SOUND_SR        40100

struct MemoryReader : public SoundfileReader {
    
    MemoryReader()
    {}
    
    /**
     * Check the availability of a sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     *
     * @return true if the sound resource is available, false otherwise.
     */
    virtual bool checkFile(const std::string& path_name) { return true; }
    
    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        channels = SOUND_CHAN;
        length = SOUND_LENGTH;
    }
    
    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        soundfile->fLength[part] = SOUND_LENGTH;
        soundfile->fSR[part] = SOUND_SR;
        soundfile->fOffset[part] = offset;
        
        // Audio frames have to be written for each chan
        for (int sample = 0; sample < SOUND_LENGTH; sample++) {
            for (int chan = 0; chan < SOUND_CHAN; chan++) {
                soundfile->fBuffers[chan][offset + sample] = 0.f;
            }
        }
        
        // Update offset
        offset += SOUND_LENGTH;
    }
    
};

#endif
/**************************  END  MemoryReader.h **************************/
MemoryReader gReader;
#else
/************************** BEGIN LibsndfileReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __LibsndfileReader__
#define __LibsndfileReader__

#ifdef SAMPLERATE
#include <samplerate.h>
#endif
#include <sndfile.h>
#include <string.h>
#include <assert.h>
#include <iostream>


struct VFLibsndfile {
    
    #define SIGNED_SIZEOF(x) ((int)sizeof(x))
    
    unsigned char* fBuffer;
    size_t fLength;
    size_t fOffset;
    SF_VIRTUAL_IO fVIO;
    
    VFLibsndfile(unsigned char* buffer, size_t length):fBuffer(buffer), fLength(length), fOffset(0)
    {
        fVIO.get_filelen = vfget_filelen;
        fVIO.seek = vfseek;
        fVIO.read = vfread;
        fVIO.write = vfwrite;
        fVIO.tell = vftell;
    }
    
    static sf_count_t vfget_filelen(void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        return vf->fLength;
    }
  
    static sf_count_t vfseek(sf_count_t offset, int whence, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        switch (whence) {
            case SEEK_SET:
                vf->fOffset = offset;
                break;
                
            case SEEK_CUR:
                vf->fOffset = vf->fOffset + offset;
                break;
                
            case SEEK_END:
                vf->fOffset = vf->fLength + offset;
                break;
                
            default:
                break;
        };
        
        return vf->fOffset;
    }
    
    static sf_count_t vfread(void* ptr, sf_count_t count, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        
        /*
         **	This will break badly for files over 2Gig in length, but
         **	is sufficient for testing.
         */
        if (vf->fOffset + count > vf->fLength) {
            count = vf->fLength - vf->fOffset;
        }
        
        memcpy(ptr, vf->fBuffer + vf->fOffset, count);
        vf->fOffset += count;
        
        return count;
    }
    
    static sf_count_t vfwrite(const void* ptr, sf_count_t count, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        
        /*
         **	This will break badly for files over 2Gig in length, but
         **	is sufficient for testing.
         */
        if (vf->fOffset >= SIGNED_SIZEOF(vf->fBuffer)) {
            return 0;
        }
        
        if (vf->fOffset + count > SIGNED_SIZEOF(vf->fBuffer)) {
            count = sizeof (vf->fBuffer) - vf->fOffset;
        }
        
        memcpy(vf->fBuffer + vf->fOffset, ptr, (size_t)count);
        vf->fOffset += count;
        
        if (vf->fOffset > vf->fLength) {
            vf->fLength = vf->fOffset;
        }
        
        return count;
    }
    
    static sf_count_t vftell(void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        return vf->fOffset;
    }
 
};

struct LibsndfileReader : public SoundfileReader {
	
    LibsndfileReader() {}
	
    typedef sf_count_t (* sample_read)(SNDFILE* sndfile, FAUSTFLOAT* ptr, sf_count_t frames);
	
    // Check file
    bool checkFile(const std::string& path_name)
    {
        SF_INFO snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        return checkFileAux(snd_file, path_name);
    }
    
    bool checkFile(unsigned char* buffer, size_t length)
    {
        SF_INFO snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, length);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        return checkFileAux(snd_file, "virtual file");
    }
    
    bool checkFileAux(SNDFILE* snd_file, const std::string& path_name)
    {
        if (snd_file) {
            sf_close(snd_file);
            return true;
        } else {
            std::cerr << "ERROR : cannot open '" << path_name << "' (" << sf_strerror(NULL) << ")" << std::endl;
            return false;
        }
    }

    // Open the file and returns its length and channels
    void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        getParamsFileAux(snd_file, snd_info, channels, length);
    }
    
    void getParamsFile(unsigned char* buffer, size_t size, int& channels, int& length)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, size);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        getParamsFileAux(snd_file, snd_info, channels, length);
    }
    
    void getParamsFileAux(SNDFILE* snd_file, const SF_INFO& snd_info, int& channels, int& length)
    {
        assert(snd_file);
        channels = int(snd_info.channels);
    #ifdef SAMPLERATE
        length = (isResampling(snd_info.samplerate)) ? ((double(snd_info.frames) * double(fDriverSR) / double(snd_info.samplerate)) + BUFFER_SIZE) : int(snd_info.frames);
    #else
        length = int(snd_info.frames);
    #endif
        sf_close(snd_file);
    }
    
    // Read the file
    void copyToOut(Soundfile* soundfile, int size, int channels, int max_channels, int offset, FAUSTFLOAT* buffer)
    {
        for (int sample = 0; sample < size; sample++) {
            for (int chan = 0; chan < channels; chan++) {
                soundfile->fBuffers[chan][offset + sample] = buffer[sample * max_channels + chan];
            }
        }
    }
    
    void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
    }
    
    void readFile(Soundfile* soundfile, unsigned char* buffer, size_t length, int part, int& offset, int max_chan)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, length);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
    }
	
    // Will be called to fill all parts from 0 to MAX_SOUNDFILE_PARTS-1
    void readFileAux(Soundfile* soundfile, SNDFILE* snd_file, const SF_INFO& snd_info, int part, int& offset, int max_chan)
    {
        assert(snd_file);
        int channels = std::min<int>(max_chan, snd_info.channels);
    #ifdef SAMPLERATE
        if (isResampling(snd_info.samplerate)) {
            soundfile->fLength[part] = int(double(snd_info.frames) * double(fDriverSR) / double(snd_info.samplerate));
            soundfile->fSR[part] = fDriverSR;
        } else {
            soundfile->fLength[part] = int(snd_info.frames);
            soundfile->fSR[part] = snd_info.samplerate;
        }
    #else
        soundfile->fLength[part] = int(snd_info.frames);
        soundfile->fSR[part] = snd_info.samplerate;
    #endif
        soundfile->fOffset[part] = offset;
		
        // Read and fill snd_info.channels number of channels
        sf_count_t nbf;
        FAUSTFLOAT* buffer_in = (FAUSTFLOAT*)alloca(BUFFER_SIZE * sizeof(FAUSTFLOAT) * snd_info.channels);
        sample_read reader;
        
        if (sizeof(FAUSTFLOAT) == 4) {
            reader = reinterpret_cast<sample_read>(sf_readf_float);
        } else {
            reader = reinterpret_cast<sample_read>(sf_readf_double);
        }
        
    #ifdef SAMPLERATE
        // Resampling
        SRC_STATE* resampler = nullptr;
        FAUSTFLOAT* buffer_out = nullptr;
        if  (isResampling(snd_info.samplerate)) {
            int error;
            resampler = src_new(SRC_SINC_FASTEST, channels, &error);
            if (error != 0) {
                std::cerr << "ERROR : src_new " << src_strerror(error) << std::endl;
                throw -1;
            }
            buffer_out = (FAUSTFLOAT*)alloca(BUFFER_SIZE * sizeof(FAUSTFLOAT) * snd_info.channels);
        }
    #endif
        
        do {
            nbf = reader(snd_file, buffer_in, BUFFER_SIZE);
        #ifdef SAMPLERATE
            // Resampling
            if  (isResampling(snd_info.samplerate)) {
                int in_offset = 0;
                SRC_DATA src_data;
                src_data.src_ratio = double(fDriverSR)/double(snd_info.samplerate);
                do {
                    src_data.data_in = &buffer_in[in_offset * snd_info.channels];
                    src_data.data_out = buffer_out;
                    src_data.input_frames = nbf - in_offset;
                    src_data.output_frames = BUFFER_SIZE;
                    src_data.end_of_input = (nbf < BUFFER_SIZE);
                    int res = src_process(resampler, &src_data);
                    if (res != 0) {
                        std::cerr << "ERROR : src_process " << src_strerror(res) << std::endl;
                        throw -1;
                    }
                    copyToOut(soundfile, src_data.output_frames_gen, channels, snd_info.channels, offset, buffer_out);
                    in_offset += src_data.input_frames_used;
                    // Update offset
                    offset += src_data.output_frames_gen;
                } while (in_offset < nbf);
            } else {
                copyToOut(soundfile, nbf, channels, snd_info.channels, offset, buffer_in);
                // Update offset
                offset += nbf;
            }
        #else
            copyToOut(soundfile, nbf, channels, snd_info.channels, offset, buffer_in);
            // Update offset
            offset += nbf;
        #endif
        } while (nbf == BUFFER_SIZE);
		
        sf_close(snd_file);
    #ifdef SAMPLERATE
        if (resampler) src_delete(resampler);
    #endif
    }

};

#endif
/**************************  END  LibsndfileReader.h **************************/
LibsndfileReader gReader;
#endif

// To be used by DSP code if no SoundUI is used
std::vector<std::string> path_name_list;
Soundfile* defaultsound = gReader.createSoundfile(path_name_list, MAX_CHAN);

class SoundUI : public GenericUI
{
		
    private:
    
        std::vector<std::string> fSoundfileDir;             // The soundfile directories
        std::map<std::string, Soundfile*> fSoundfileMap;    // Map to share loaded soundfiles
        SoundfileReader* fSoundReader;

     public:
    
        SoundUI(const std::string& sound_directory = "", int sample_rate = -1, SoundfileReader* reader = nullptr)
        {
            fSoundfileDir.push_back(sound_directory);
            fSoundReader = (reader) ? reader : &gReader;
            fSoundReader->setSampleRate(sample_rate);
        }
    
        SoundUI(const std::vector<std::string>& sound_directories, int sample_rate = -1, SoundfileReader* reader = nullptr)
        :fSoundfileDir(sound_directories)
        {
            fSoundReader = (reader) ? reader : &gReader;
            fSoundReader->setSampleRate(sample_rate);
        }
    
        virtual ~SoundUI()
        {   
            // Delete all soundfiles
            std::map<std::string, Soundfile*>::iterator it;
            for (it = fSoundfileMap.begin(); it != fSoundfileMap.end(); it++) {
                delete (*it).second;
            }
        }

        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
        {
            const char* saved_url = url; // 'url' is consumed by parseMenuList2
            std::vector<std::string> file_name_list;
            
            bool menu = parseMenuList2(url, file_name_list, true);
            // If not a list, we have as single file
            if (!menu) { file_name_list.push_back(saved_url); }
            
            // Parse the possible list
            if (fSoundfileMap.find(saved_url) == fSoundfileMap.end()) {
                // Check all files and get their complete path
                std::vector<std::string> path_name_list = fSoundReader->checkFiles(fSoundfileDir, file_name_list);
                // Read them and create the Soundfile
                Soundfile* sound_file = fSoundReader->createSoundfile(path_name_list, MAX_CHAN);
                if (sound_file) {
                    fSoundfileMap[saved_url] = sound_file;
                } else {
                    // If failure, use 'defaultsound'
                    std::cerr << "addSoundfile : soundfile for " << saved_url << " cannot be created !" << std::endl;
                    *sf_zone = defaultsound;
                    return;
                }
            }
            
            // Get the soundfile
            *sf_zone = fSoundfileMap[saved_url];
        }
    
        static std::string getBinaryPath(std::string folder = "")
        {
            std::string bundle_path_str;
        #ifdef __APPLE__
            CFURLRef bundle_ref = CFBundleCopyBundleURL(CFBundleGetMainBundle());
            if (!bundle_ref) { std::cerr << "getBinaryPath CFBundleCopyBundleURL error '" << folder << "'" << std::endl; return ""; }
      
            UInt8 bundle_path[1024];
            if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
                bundle_path_str = std::string((char*)bundle_path) + folder;
            } else {
                std::cerr << "getBinaryPath CFURLGetFileSystemRepresentation error\n";
            }
        #endif
        #ifdef ANDROID_DRIVER
            bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
        #endif
            return bundle_path_str;
        }
        
        static std::string getBinaryPathFrom(const std::string& path)
        {
            std::string bundle_path_str;
        #ifdef __APPLE__
            CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFStringCreateWithCString(kCFAllocatorDefault, path.c_str(), CFStringGetSystemEncoding()));
            if (!bundle) { std::cerr << "getBinaryPathFrom CFBundleGetBundleWithIdentifier error '" << path << "'" << std::endl; return ""; }
         
            CFURLRef bundle_ref = CFBundleCopyBundleURL(bundle);
            if (!bundle_ref) { std::cerr << "getBinaryPathFrom CFBundleCopyBundleURL error\n"; return ""; }
            
            UInt8 bundle_path[1024];
            if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
                bundle_path_str = std::string((char*)bundle_path);
            } else {
                std::cerr << "getBinaryPathFrom CFURLGetFileSystemRepresentation error\n";
            }
        #endif
        #ifdef ANDROID_DRIVER
            bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
        #endif
            return bundle_path_str;
        }
};

#endif
/**************************  END  SoundUI.h **************************/
#endif

//**************************************************************
// OSC configuration (hardcoded for now...)
//**************************************************************

#define OSC_IP_ADDRESS  "192.168.1.112"
#define OSC_IN_PORT     "5510"
#define OSC_OUT_PORT    "5511"

//**************************************************************
// Intrinsic
//**************************************************************


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>

class mydspSIG0 {
	
  private:
	
	int iRec15[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	int getInputRatemydspSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatemydspSIG0(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			iRec15[l16] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec15[0] = (iRec15[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec15[0] + -1))));
			iRec15[1] = iRec15[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float mydsp_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower6_f(float value) {
	return (((((value * value) * value) * value) * value) * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider2;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	float fVec1[2];
	float fVec2[2];
	int iRec3[2];
	Soundfile* fSoundfile0;
	FAUSTFLOAT fHslider7;
	float fRec4[2];
	float fConst2;
	float fConst3;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst4;
	FAUSTFLOAT fHslider10;
	float fRec7[2];
	float fRec6[2];
	FAUSTFLOAT fHslider11;
	float fRec5[2];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec8[3];
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
	float fConst5;
	float fRec2[3];
	FAUSTFLOAT fHslider16;
	FAUSTFLOAT fHslider17;
	float fRec1[3];
	float fRec0[3];
	FAUSTFLOAT fHslider18;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider19;
	FAUSTFLOAT fHslider20;
	FAUSTFLOAT fHslider21;
	FAUSTFLOAT fHslider22;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fHslider24;
	float fVec3[2];
	float fVec4[2];
	int iVec5[2];
	FAUSTFLOAT fHslider25;
	float fRec13[2];
	FAUSTFLOAT fHslider26;
	float fConst10;
	float fRec16[2];
	float fRec14[2];
	float fRec18[2];
	float fConst11;
	float fConst12;
	FAUSTFLOAT fHslider27;
	float fRec20[2];
	float fRec17[3];
	float fRec21[3];
	float fRec22[3];
	float fRec23[3];
	float fRec24[3];
	float fConst13;
	float fConst14;
	int iRec25[2];
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fRec12[3];
	float fConst19;
	float fConst20;
	float fConst21;
	float fRec26[2];
	FAUSTFLOAT fHslider28;
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	float fRec29[2];
	float fRec28[2];
	FAUSTFLOAT fHslider31;
	float fRec27[2];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec30[3];
	FAUSTFLOAT fHslider34;
	FAUSTFLOAT fHslider35;
	float fRec11[3];
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fRec10[3];
	float fRec9[3];
	FAUSTFLOAT fHslider38;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider39;
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fHslider42;
	float fConst22;
	float fConst23;
	FAUSTFLOAT fHslider43;
	float fVec6[2];
	float fVec7[2];
	int iVec8[2];
	int iRec35[2];
	float fRec34[2];
	float fRec36[2];
	FAUSTFLOAT fHslider44;
	float fRec39[2];
	int iRec38[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec41[2];
	float fRec40[2];
	float fRec37[3];
	FAUSTFLOAT fHslider45;
	float fRec44[2];
	int iRec43[2];
	float fRec42[3];
	FAUSTFLOAT fHslider46;
	float fRec47[2];
	int iRec46[2];
	float fRec45[3];
	FAUSTFLOAT fHslider47;
	float fRec50[2];
	int iRec49[2];
	float fRec48[3];
	FAUSTFLOAT fHslider48;
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	float fRec53[2];
	float fRec52[2];
	FAUSTFLOAT fHslider51;
	float fRec51[2];
	FAUSTFLOAT fHslider52;
	FAUSTFLOAT fHslider53;
	float fRec54[3];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec33[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec32[3];
	float fRec31[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	FAUSTFLOAT fHslider63;
	float fVec9[2];
	float fVec10[2];
	int iRec58[2];
	Soundfile* fSoundfile1;
	float fRec59[2];
	FAUSTFLOAT fHslider64;
	FAUSTFLOAT fHslider65;
	FAUSTFLOAT fHslider66;
	float fRec62[2];
	float fRec61[2];
	FAUSTFLOAT fHslider67;
	float fRec60[2];
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	float fRec63[3];
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec57[3];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec56[3];
	float fRec55[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	float fConst27;
	float fRec67[2];
	float fRec69[2];
	float fConst28;
	float fRec73[2];
	float fRec72[2];
	float fRec71[2];
	float fRec70[2];
	float fRec68[2];
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	float fRec76[2];
	float fRec75[2];
	FAUSTFLOAT fHslider82;
	float fRec74[2];
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec77[3];
	FAUSTFLOAT fHslider85;
	FAUSTFLOAT fHslider86;
	float fRec66[3];
	FAUSTFLOAT fHslider87;
	FAUSTFLOAT fHslider88;
	float fRec65[3];
	float fRec64[3];
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fRec80[2];
	float fRec79[2];
	float fRec78[2];
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fRec83[2];
	float fRec82[2];
	float fRec81[2];
	float fRec86[2];
	float fRec85[2];
	float fRec84[2];
	int iRec91[2];
	float fRec90[3];
	int iRec93[2];
	float fRec92[3];
	int iRec95[2];
	float fRec94[3];
	int iRec97[2];
	float fRec96[3];
	float fRec100[2];
	float fRec99[2];
	float fRec98[2];
	float fRec101[3];
	float fRec89[3];
	float fRec88[3];
	float fRec87[3];
	float fRec104[2];
	float fRec103[2];
	float fRec102[2];
	float fRec107[2];
	float fRec106[2];
	float fRec105[2];
	float fRec110[2];
	float fRec109[2];
	float fRec108[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "STS_Synth.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/peak_eq_cq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_cq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_cq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonbp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonbp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonbp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonhp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonhp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonhp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonlp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonlp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonlp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.2");
		m->declare("misceffects.lib/name", "Faust Math Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "STS_Synth");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("synths.lib/name", "Faust Synthesizer Library");
		m->declare("synths.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (3.14159274f / fConst0);
		fConst2 = std::exp((0.0f - (20.0f / fConst0)));
		fConst3 = (1.0f - fConst2);
		fConst4 = (1.0f / fConst0);
		fConst5 = (6.28318548f / fConst0);
		fConst6 = std::tan((25132.7422f / fConst0));
		fConst7 = (1.0f / fConst6);
		fConst8 = (((fConst7 + 0.333333343f) / fConst6) + 1.0f);
		fConst9 = (0.25f / fConst8);
		fConst10 = (8.0f / fConst0);
		fConst11 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst12 = (1.0f - fConst11);
		fConst13 = std::max<float>(1.0f, (0.0199999996f * fConst0));
		fConst14 = (1.0f / fConst13);
		fConst15 = (1.0f / std::max<float>(1.0f, (2.0f * fConst0)));
		fConst16 = (1.0f / fConst8);
		fConst17 = (((fConst7 + -0.333333343f) / fConst6) + 1.0f);
		fConst18 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst6))));
		fConst19 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst20 = (1.0f / fConst19);
		fConst21 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst22 = std::exp((0.0f - (10000.0f / fConst0)));
		fConst23 = (1.0f - fConst22);
		fConst24 = (0.330000013f * fConst0);
		fConst25 = (0.100000001f * fConst0);
		fConst26 = (0.660000026f * fConst0);
		fConst27 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst28 = (81.0f / fConst0);
		fConst29 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst30 = (1.0f - fConst29);
		fConst31 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst32 = (1.0f - fConst31);
		fConst33 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst34 = (1.0f - fConst33);
		fConst35 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst36 = (1.0f - fConst35);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fVslider0 = FAUSTFLOAT(-6.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.69999999999999996f);
		fHslider3 = FAUSTFLOAT(20000.0f);
		fHslider4 = FAUSTFLOAT(2000.0f);
		fHslider5 = FAUSTFLOAT(500.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		if (uintptr_t(fSoundfile0) == 0) {
			fSoundfile0 = defaultsound;
		}
		fHslider7 = FAUSTFLOAT(7.0f);
		fHslider8 = FAUSTFLOAT(1.0f);
		fHslider9 = FAUSTFLOAT(0.10000000000000001f);
		fHslider10 = FAUSTFLOAT(0.10000000000000001f);
		fHslider11 = FAUSTFLOAT(0.0f);
		fHslider12 = FAUSTFLOAT(0.69999999999999996f);
		fHslider13 = FAUSTFLOAT(50.0f);
		fHslider14 = FAUSTFLOAT(0.0f);
		fHslider15 = FAUSTFLOAT(0.69999999999999996f);
		fHslider16 = FAUSTFLOAT(0.0f);
		fHslider17 = FAUSTFLOAT(0.69999999999999996f);
		fHslider18 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider19 = FAUSTFLOAT(0.69999999999999996f);
		fHslider20 = FAUSTFLOAT(20000.0f);
		fHslider21 = FAUSTFLOAT(2000.0f);
		fHslider22 = FAUSTFLOAT(500.0f);
		fHslider23 = FAUSTFLOAT(0.5f);
		fHslider24 = FAUSTFLOAT(0.0f);
		fHslider25 = FAUSTFLOAT(440.0f);
		fHslider26 = FAUSTFLOAT(0.0f);
		fHslider27 = FAUSTFLOAT(0.0f);
		fHslider28 = FAUSTFLOAT(1.0f);
		fHslider29 = FAUSTFLOAT(0.10000000000000001f);
		fHslider30 = FAUSTFLOAT(0.10000000000000001f);
		fHslider31 = FAUSTFLOAT(0.0f);
		fHslider32 = FAUSTFLOAT(0.69999999999999996f);
		fHslider33 = FAUSTFLOAT(50.0f);
		fHslider34 = FAUSTFLOAT(0.0f);
		fHslider35 = FAUSTFLOAT(0.69999999999999996f);
		fHslider36 = FAUSTFLOAT(0.0f);
		fHslider37 = FAUSTFLOAT(0.69999999999999996f);
		fHslider38 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider39 = FAUSTFLOAT(0.69999999999999996f);
		fHslider40 = FAUSTFLOAT(20000.0f);
		fHslider41 = FAUSTFLOAT(2000.0f);
		fHslider42 = FAUSTFLOAT(500.0f);
		fHslider43 = FAUSTFLOAT(0.0f);
		fHslider44 = FAUSTFLOAT(440.0f);
		fHslider45 = FAUSTFLOAT(440.0f);
		fHslider46 = FAUSTFLOAT(440.0f);
		fHslider47 = FAUSTFLOAT(440.0f);
		fHslider48 = FAUSTFLOAT(1.0f);
		fHslider49 = FAUSTFLOAT(0.10000000000000001f);
		fHslider50 = FAUSTFLOAT(0.10000000000000001f);
		fHslider51 = FAUSTFLOAT(0.0f);
		fHslider52 = FAUSTFLOAT(0.69999999999999996f);
		fHslider53 = FAUSTFLOAT(50.0f);
		fHslider54 = FAUSTFLOAT(0.0f);
		fHslider55 = FAUSTFLOAT(0.69999999999999996f);
		fHslider56 = FAUSTFLOAT(0.0f);
		fHslider57 = FAUSTFLOAT(0.69999999999999996f);
		fHslider58 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider59 = FAUSTFLOAT(0.69999999999999996f);
		fHslider60 = FAUSTFLOAT(20000.0f);
		fHslider61 = FAUSTFLOAT(2000.0f);
		fHslider62 = FAUSTFLOAT(500.0f);
		fHslider63 = FAUSTFLOAT(0.0f);
		if (uintptr_t(fSoundfile1) == 0) {
			fSoundfile1 = defaultsound;
		}
		fHslider64 = FAUSTFLOAT(1.0f);
		fHslider65 = FAUSTFLOAT(0.10000000000000001f);
		fHslider66 = FAUSTFLOAT(0.10000000000000001f);
		fHslider67 = FAUSTFLOAT(0.0f);
		fHslider68 = FAUSTFLOAT(0.69999999999999996f);
		fHslider69 = FAUSTFLOAT(50.0f);
		fHslider70 = FAUSTFLOAT(0.0f);
		fHslider71 = FAUSTFLOAT(0.69999999999999996f);
		fHslider72 = FAUSTFLOAT(0.0f);
		fHslider73 = FAUSTFLOAT(0.69999999999999996f);
		fHslider74 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider75 = FAUSTFLOAT(0.69999999999999996f);
		fHslider76 = FAUSTFLOAT(20000.0f);
		fHslider77 = FAUSTFLOAT(2000.0f);
		fHslider78 = FAUSTFLOAT(500.0f);
		fHslider79 = FAUSTFLOAT(1.0f);
		fHslider80 = FAUSTFLOAT(0.10000000000000001f);
		fHslider81 = FAUSTFLOAT(0.10000000000000001f);
		fHslider82 = FAUSTFLOAT(0.0f);
		fHslider83 = FAUSTFLOAT(0.69999999999999996f);
		fHslider84 = FAUSTFLOAT(50.0f);
		fHslider85 = FAUSTFLOAT(0.0f);
		fHslider86 = FAUSTFLOAT(0.69999999999999996f);
		fHslider87 = FAUSTFLOAT(0.0f);
		fHslider88 = FAUSTFLOAT(0.69999999999999996f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec2[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec3[l3] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec4[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec7[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec2[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec1[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec0[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fVec4[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			iVec5[l14] = 0;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec13[l15] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec17[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec21[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec22[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec23[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec24[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			iRec25[l26] = 0;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec12[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec29[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec28[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec27[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec30[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec11[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec10[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec9[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fVec6[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec7[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			iVec8[l38] = 0;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			iRec35[l39] = 0;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec34[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec36[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec39[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			iRec38[l43] = 0;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec41[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec40[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec37[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec44[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			iRec43[l48] = 0;
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec42[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec47[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			iRec46[l51] = 0;
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec45[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec50[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			iRec49[l54] = 0;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec48[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec53[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec52[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec51[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec54[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec33[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 3); l61 = (l61 + 1)) {
			fRec32[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 3); l62 = (l62 + 1)) {
			fRec31[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fVec9[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fVec10[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			iRec58[l65] = 0;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec59[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec62[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec61[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec60[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec63[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec57[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec56[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec55[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec67[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec69[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec73[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec72[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec71[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec70[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec68[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec76[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec75[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec74[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec77[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec66[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec65[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec64[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec80[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec79[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec78[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec83[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec82[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec81[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec86[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec85[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			iRec91[l97] = 0;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec90[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			iRec93[l99] = 0;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec92[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			iRec95[l101] = 0;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec94[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			iRec97[l103] = 0;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec96[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec100[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec99[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fRec98[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec101[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec89[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 3); l110 = (l110 + 1)) {
			fRec88[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 3); l111 = (l111 + 1)) {
			fRec87[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec104[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec103[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec102[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec107[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec106[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec105[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec110[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec109[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec108[l120] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("STS_Synth");
		ui_interface->openTabBox("Mapping_Tabs");
		ui_interface->openVerticalBox("Comp_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->declare(&fHslider9, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider9, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider8, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider8, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider10, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider10, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider11, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider11, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider29, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider29, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider28, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider28, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider30, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider30, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider31, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider31, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider49, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider49, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider48, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider48, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider50, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider51, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->declare(&fHslider65, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider65, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider64, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider64, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider66, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider66, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider67, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider67, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->declare(&fHslider80, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider80, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider79, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider79, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider81, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider81, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider82, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider82, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("EQ_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider13, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider12, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider12, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider5, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider14, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider15, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider16, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider17, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider3, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider2, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider33, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider33, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider32, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider22, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider22, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider34, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider35, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider21, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider36, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider37, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider20, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider20, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider19, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider53, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider53, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider52, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider42, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider42, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider54, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider55, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider41, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider41, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider56, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider56, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider57, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider40, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider40, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider39, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider39, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider69, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider69, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider68, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider68, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider62, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider62, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider70, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider70, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider71, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider71, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider61, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider61, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider72, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider72, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider73, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider73, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider60, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider60, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider59, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider59, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider84, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider84, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider83, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider83, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider78, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider78, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider85, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider85, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider86, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider86, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider77, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider77, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider87, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider87, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider88, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider88, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider76, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider76, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider75, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider75, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider1, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider18, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider38, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider58, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 5", &fHslider74, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox0);
		ui_interface->addCheckButton("2", &fCheckbox1);
		ui_interface->addCheckButton("3", &fCheckbox2);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->addCheckButton("5", &fCheckbox4);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider23, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider27, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider25, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider24, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider44, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider45, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider46, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider47, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider43, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider26, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 7");
		ui_interface->addHorizontalSlider("Control 0", &fHslider0, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider63, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 9");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 7.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->addSoundfile("PERC_MAIN_SMPL", "{'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_1.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_2.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_3.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_4.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_5.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_MAIN_6.wav'}", &fSoundfile0);
		ui_interface->addSoundfile("PERC_SEC_SMPL", "{'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_SEC_1.wav';'D:\\Audio Projects\\STSInteraction\\WAV SAMPLES\\PERC_SEC_2.wav'}", &fSoundfile1);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.41400003f * std::sqrt((1.0f - fSlow0)));
		float fSlow2 = (1.25892544f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow3 = (1.0f / float(fHslider2));
		float fSlow4 = std::tan((fConst1 * float(fHslider3)));
		float fSlow5 = (1.0f / fSlow4);
		float fSlow6 = (((fSlow3 + fSlow5) / fSlow4) + 1.0f);
		float fSlow7 = ((std::pow(10.0f, (0.0500000007f * float(fHslider1))) * (1.0f - float(fCheckbox0))) / fSlow6);
		float fSlow8 = float(fHslider4);
		float fSlow9 = std::tan((fConst1 * fSlow8));
		float fSlow10 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow9))));
		float fSlow11 = float(fHslider5);
		float fSlow12 = std::tan((fConst1 * fSlow11));
		float fSlow13 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow12))));
		float fSlow14 = float(fHslider6);
		int iSlow15 = (fSlow14 >= 3.0f);
		int iSlow16 = (fSlow14 >= 2.0f);
		int iSlow17 = (fSlow14 >= 1.0f);
		int iSlow18 = (fSlow14 > 0.0f);
		float fSlow19 = (0.0f - fSlow14);
		Soundfile* fSoundfile0ca = fSoundfile0;
		int* fSoundfile0ca_le0 = fSoundfile0ca->fLength;
		int iSlow20 = (fSoundfile0ca_le0[0] + -1);
		int* fSoundfile0ca_of0 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu0 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch0 = fSoundfile0ca_bu0[0];
		int* fSoundfile0ca_le1 = fSoundfile0ca->fLength;
		int iSlow21 = (fSoundfile0ca_le1[1] + -1);
		int* fSoundfile0ca_of1 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu1 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch1 = fSoundfile0ca_bu1[0];
		int* fSoundfile0ca_le2 = fSoundfile0ca->fLength;
		int iSlow22 = (fSoundfile0ca_le2[2] + -1);
		int* fSoundfile0ca_of2 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu2 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch2 = fSoundfile0ca_bu2[0];
		int iSlow23 = (fSlow14 >= 5.0f);
		int iSlow24 = (fSlow14 >= 4.0f);
		int* fSoundfile0ca_le3 = fSoundfile0ca->fLength;
		int iSlow25 = (fSoundfile0ca_le3[3] + -1);
		int* fSoundfile0ca_of3 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu3 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch3 = fSoundfile0ca_bu3[0];
		int* fSoundfile0ca_le4 = fSoundfile0ca->fLength;
		int iSlow26 = (fSoundfile0ca_le4[4] + -1);
		int* fSoundfile0ca_of4 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu4 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch4 = fSoundfile0ca_bu4[0];
		int* fSoundfile0ca_le5 = fSoundfile0ca->fLength;
		int iSlow27 = (fSoundfile0ca_le5[5] + -1);
		int* fSoundfile0ca_of5 = fSoundfile0ca->fOffset;
		FAUSTFLOAT** fSoundfile0ca_bu5 = fSoundfile0ca->fBuffers;
		FAUSTFLOAT* fSoundfile0ca_bu_ch5 = fSoundfile0ca_bu5[0];
		float fSlow28 = float(fHslider7);
		float fSlow29 = (fConst3 * ((1.0f / float(fHslider8)) + -1.0f));
		float fSlow30 = float(fHslider9);
		int iSlow31 = (std::fabs(fSlow30) < 1.1920929e-07f);
		float fSlow32 = (iSlow31 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow31 ? 1.0f : fSlow30)))));
		float fSlow33 = float(fHslider10);
		int iSlow34 = (std::fabs(fSlow33) < 1.1920929e-07f);
		float fSlow35 = (iSlow34 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow34 ? 1.0f : fSlow33)))));
		float fSlow36 = (1.0f - fSlow35);
		float fSlow37 = (1.0f - fSlow32);
		float fSlow38 = float(fHslider11);
		float fSlow39 = (1.0f / float(fHslider12));
		float fSlow40 = std::tan((fConst1 * float(fHslider13)));
		float fSlow41 = (1.0f / fSlow40);
		float fSlow42 = (1.0f / (((fSlow39 + fSlow41) / fSlow40) + 1.0f));
		float fSlow43 = (((fSlow41 - fSlow39) / fSlow40) + 1.0f);
		float fSlow44 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow40))));
		float fSlow45 = (1.0f / fSlow12);
		float fSlow46 = float(fHslider14);
		int iSlow47 = (fSlow46 > 0.0f);
		float fSlow48 = (float(fHslider15) * std::sin((fConst5 * fSlow11)));
		float fSlow49 = (fConst1 * ((fSlow11 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow46)))) / fSlow48));
		float fSlow50 = (fConst1 * (fSlow11 / fSlow48));
		float fSlow51 = (iSlow47 ? fSlow50 : fSlow49);
		float fSlow52 = ((fSlow45 * (fSlow45 - fSlow51)) + 1.0f);
		float fSlow53 = ((fSlow45 * (fSlow45 + fSlow51)) + 1.0f);
		float fSlow54 = (iSlow47 ? fSlow49 : fSlow50);
		float fSlow55 = ((fSlow45 * (fSlow45 + fSlow54)) + 1.0f);
		float fSlow56 = ((fSlow45 * (fSlow45 - fSlow54)) + 1.0f);
		float fSlow57 = (1.0f / fSlow9);
		float fSlow58 = float(fHslider16);
		int iSlow59 = (fSlow58 > 0.0f);
		float fSlow60 = (float(fHslider17) * std::sin((fConst5 * fSlow8)));
		float fSlow61 = (fConst1 * ((fSlow8 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow58)))) / fSlow60));
		float fSlow62 = (fConst1 * (fSlow8 / fSlow60));
		float fSlow63 = (iSlow59 ? fSlow62 : fSlow61);
		float fSlow64 = ((fSlow57 * (fSlow57 - fSlow63)) + 1.0f);
		float fSlow65 = ((fSlow57 * (fSlow57 + fSlow63)) + 1.0f);
		float fSlow66 = (iSlow59 ? fSlow61 : fSlow62);
		float fSlow67 = ((fSlow57 * (fSlow57 + fSlow66)) + 1.0f);
		float fSlow68 = ((fSlow57 * (fSlow57 - fSlow66)) + 1.0f);
		float fSlow69 = (1.0f / fSlow6);
		float fSlow70 = (((fSlow5 - fSlow3) / fSlow4) + 1.0f);
		float fSlow71 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow4))));
		float fSlow72 = (1.0f / float(fHslider19));
		float fSlow73 = std::tan((fConst1 * float(fHslider20)));
		float fSlow74 = (1.0f / fSlow73);
		float fSlow75 = (((fSlow72 + fSlow74) / fSlow73) + 1.0f);
		float fSlow76 = ((std::pow(10.0f, (0.0500000007f * float(fHslider18))) * (1.0f - float(fCheckbox1))) / fSlow75);
		float fSlow77 = float(fHslider21);
		float fSlow78 = std::tan((fConst1 * fSlow77));
		float fSlow79 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow78))));
		float fSlow80 = float(fHslider22);
		float fSlow81 = std::tan((fConst1 * fSlow80));
		float fSlow82 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow81))));
		float fSlow83 = std::pow(10.0f, (2.0f * (float(fHslider23) + -0.5f)));
		float fSlow84 = (2.0f * fSlow83);
		float fSlow85 = float(fHslider24);
		int iSlow86 = (fSlow85 > 0.0f);
		float fSlow87 = (0.0f - fSlow85);
		float fSlow88 = float(fHslider25);
		float fSlow89 = float(fHslider26);
		float fSlow90 = (0.5f * fSlow89);
		float fSlow91 = (fConst12 * float(fHslider27));
		float fSlow92 = (fConst3 * ((1.0f / float(fHslider28)) + -1.0f));
		float fSlow93 = float(fHslider29);
		int iSlow94 = (std::fabs(fSlow93) < 1.1920929e-07f);
		float fSlow95 = (iSlow94 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow94 ? 1.0f : fSlow93)))));
		float fSlow96 = float(fHslider30);
		int iSlow97 = (std::fabs(fSlow96) < 1.1920929e-07f);
		float fSlow98 = (iSlow97 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow97 ? 1.0f : fSlow96)))));
		float fSlow99 = (1.0f - fSlow98);
		float fSlow100 = (1.0f - fSlow95);
		float fSlow101 = float(fHslider31);
		float fSlow102 = (1.0f / float(fHslider32));
		float fSlow103 = std::tan((fConst1 * float(fHslider33)));
		float fSlow104 = (1.0f / fSlow103);
		float fSlow105 = (1.0f / (((fSlow102 + fSlow104) / fSlow103) + 1.0f));
		float fSlow106 = (((fSlow104 - fSlow102) / fSlow103) + 1.0f);
		float fSlow107 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow103))));
		float fSlow108 = (1.0f / fSlow81);
		float fSlow109 = float(fHslider34);
		int iSlow110 = (fSlow109 > 0.0f);
		float fSlow111 = (float(fHslider35) * std::sin((fConst5 * fSlow80)));
		float fSlow112 = (fConst1 * ((fSlow80 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow109)))) / fSlow111));
		float fSlow113 = (fConst1 * (fSlow80 / fSlow111));
		float fSlow114 = (iSlow110 ? fSlow113 : fSlow112);
		float fSlow115 = ((fSlow108 * (fSlow108 - fSlow114)) + 1.0f);
		float fSlow116 = ((fSlow108 * (fSlow108 + fSlow114)) + 1.0f);
		float fSlow117 = (iSlow110 ? fSlow112 : fSlow113);
		float fSlow118 = ((fSlow108 * (fSlow108 + fSlow117)) + 1.0f);
		float fSlow119 = ((fSlow108 * (fSlow108 - fSlow117)) + 1.0f);
		float fSlow120 = (1.0f / fSlow78);
		float fSlow121 = float(fHslider36);
		int iSlow122 = (fSlow121 > 0.0f);
		float fSlow123 = (float(fHslider37) * std::sin((fConst5 * fSlow77)));
		float fSlow124 = (fConst1 * ((fSlow77 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow121)))) / fSlow123));
		float fSlow125 = (fConst1 * (fSlow77 / fSlow123));
		float fSlow126 = (iSlow122 ? fSlow125 : fSlow124);
		float fSlow127 = ((fSlow120 * (fSlow120 - fSlow126)) + 1.0f);
		float fSlow128 = ((fSlow120 * (fSlow120 + fSlow126)) + 1.0f);
		float fSlow129 = (iSlow122 ? fSlow124 : fSlow125);
		float fSlow130 = ((fSlow120 * (fSlow120 + fSlow129)) + 1.0f);
		float fSlow131 = ((fSlow120 * (fSlow120 - fSlow129)) + 1.0f);
		float fSlow132 = (1.0f / fSlow75);
		float fSlow133 = (((fSlow74 - fSlow72) / fSlow73) + 1.0f);
		float fSlow134 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow73))));
		float fSlow135 = (1.0f / float(fHslider39));
		float fSlow136 = std::tan((fConst1 * float(fHslider40)));
		float fSlow137 = (1.0f / fSlow136);
		float fSlow138 = (((fSlow135 + fSlow137) / fSlow136) + 1.0f);
		float fSlow139 = ((std::pow(10.0f, (0.0500000007f * float(fHslider38))) * (1.0f - float(fCheckbox2))) / fSlow138);
		float fSlow140 = float(fHslider41);
		float fSlow141 = std::tan((fConst1 * fSlow140));
		float fSlow142 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow141))));
		float fSlow143 = float(fHslider42);
		float fSlow144 = std::tan((fConst1 * fSlow143));
		float fSlow145 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow144))));
		float fSlow146 = float(fHslider43);
		int iSlow147 = (fSlow146 > 0.0f);
		float fSlow148 = (0.0f - fSlow146);
		float fSlow149 = float(fHslider44);
		float fSlow150 = ((fSlow149 * fSlow83) * ((0.0500000007f * fSlow89) + 1.0f));
		float fSlow151 = (fConst23 * ((0.200000003f * ((0.5f * fSlow28) + -5.0f)) + 1.0f));
		float fSlow152 = ((float(fHslider45) * fSlow83) * (1.0f - (0.0299999993f * fSlow89)));
		float fSlow153 = ((float(fHslider46) * fSlow83) * ((0.310000002f * fSlow89) + 1.0f));
		float fSlow154 = ((float(fHslider47) * fSlow83) * (1.0f - (0.270000011f * fSlow89)));
		float fSlow155 = (fConst3 * ((1.0f / float(fHslider48)) + -1.0f));
		float fSlow156 = float(fHslider49);
		int iSlow157 = (std::fabs(fSlow156) < 1.1920929e-07f);
		float fSlow158 = (iSlow157 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow157 ? 1.0f : fSlow156)))));
		float fSlow159 = float(fHslider50);
		int iSlow160 = (std::fabs(fSlow159) < 1.1920929e-07f);
		float fSlow161 = (iSlow160 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow160 ? 1.0f : fSlow159)))));
		float fSlow162 = (1.0f - fSlow161);
		float fSlow163 = (1.0f - fSlow158);
		float fSlow164 = float(fHslider51);
		float fSlow165 = (1.0f / float(fHslider52));
		float fSlow166 = std::tan((fConst1 * float(fHslider53)));
		float fSlow167 = (1.0f / fSlow166);
		float fSlow168 = (1.0f / (((fSlow165 + fSlow167) / fSlow166) + 1.0f));
		float fSlow169 = (((fSlow167 - fSlow165) / fSlow166) + 1.0f);
		float fSlow170 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow166))));
		float fSlow171 = (1.0f / fSlow144);
		float fSlow172 = float(fHslider54);
		int iSlow173 = (fSlow172 > 0.0f);
		float fSlow174 = (float(fHslider55) * std::sin((fConst5 * fSlow143)));
		float fSlow175 = (fConst1 * ((fSlow143 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow172)))) / fSlow174));
		float fSlow176 = (fConst1 * (fSlow143 / fSlow174));
		float fSlow177 = (iSlow173 ? fSlow176 : fSlow175);
		float fSlow178 = ((fSlow171 * (fSlow171 - fSlow177)) + 1.0f);
		float fSlow179 = ((fSlow171 * (fSlow171 + fSlow177)) + 1.0f);
		float fSlow180 = (iSlow173 ? fSlow175 : fSlow176);
		float fSlow181 = ((fSlow171 * (fSlow171 + fSlow180)) + 1.0f);
		float fSlow182 = ((fSlow171 * (fSlow171 - fSlow180)) + 1.0f);
		float fSlow183 = (1.0f / fSlow141);
		float fSlow184 = float(fHslider56);
		int iSlow185 = (fSlow184 > 0.0f);
		float fSlow186 = (float(fHslider57) * std::sin((fConst5 * fSlow140)));
		float fSlow187 = (fConst1 * ((fSlow140 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow184)))) / fSlow186));
		float fSlow188 = (fConst1 * (fSlow140 / fSlow186));
		float fSlow189 = (iSlow185 ? fSlow188 : fSlow187);
		float fSlow190 = ((fSlow183 * (fSlow183 - fSlow189)) + 1.0f);
		float fSlow191 = ((fSlow183 * (fSlow183 + fSlow189)) + 1.0f);
		float fSlow192 = (iSlow185 ? fSlow187 : fSlow188);
		float fSlow193 = ((fSlow183 * (fSlow183 + fSlow192)) + 1.0f);
		float fSlow194 = ((fSlow183 * (fSlow183 - fSlow192)) + 1.0f);
		float fSlow195 = (1.0f / fSlow138);
		float fSlow196 = (((fSlow137 - fSlow135) / fSlow136) + 1.0f);
		float fSlow197 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow136))));
		float fSlow198 = (1.0f / float(fHslider59));
		float fSlow199 = std::tan((fConst1 * float(fHslider60)));
		float fSlow200 = (1.0f / fSlow199);
		float fSlow201 = (((fSlow198 + fSlow200) / fSlow199) + 1.0f);
		float fSlow202 = ((std::pow(10.0f, (0.0500000007f * float(fHslider58))) * (1.0f - float(fCheckbox3))) / fSlow201);
		float fSlow203 = (0.300000012f * fSlow202);
		float fSlow204 = float(fHslider61);
		float fSlow205 = std::tan((fConst1 * fSlow204));
		float fSlow206 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow205))));
		float fSlow207 = float(fHslider62);
		float fSlow208 = std::tan((fConst1 * fSlow207));
		float fSlow209 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow208))));
		int iSlow210 = ((fSlow28 > 8.5f) >= 1);
		float fSlow211 = float(fHslider63);
		int iSlow212 = (fSlow211 > 0.0f);
		float fSlow213 = (0.0f - fSlow211);
		Soundfile* fSoundfile1ca = fSoundfile1;
		int* fSoundfile1ca_le0 = fSoundfile1ca->fLength;
		int iSlow214 = (fSoundfile1ca_le0[0] + -1);
		int* fSoundfile1ca_of0 = fSoundfile1ca->fOffset;
		FAUSTFLOAT** fSoundfile1ca_bu0 = fSoundfile1ca->fBuffers;
		FAUSTFLOAT* fSoundfile1ca_bu_ch0 = fSoundfile1ca_bu0[0];
		int* fSoundfile1ca_le1 = fSoundfile1ca->fLength;
		int iSlow215 = (fSoundfile1ca_le1[1] + -1);
		int* fSoundfile1ca_of1 = fSoundfile1ca->fOffset;
		FAUSTFLOAT** fSoundfile1ca_bu1 = fSoundfile1ca->fBuffers;
		FAUSTFLOAT* fSoundfile1ca_bu_ch1 = fSoundfile1ca_bu1[0];
		float fSlow216 = (fConst3 * ((1.0f / float(fHslider64)) + -1.0f));
		float fSlow217 = float(fHslider65);
		int iSlow218 = (std::fabs(fSlow217) < 1.1920929e-07f);
		float fSlow219 = (iSlow218 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow218 ? 1.0f : fSlow217)))));
		float fSlow220 = float(fHslider66);
		int iSlow221 = (std::fabs(fSlow220) < 1.1920929e-07f);
		float fSlow222 = (iSlow221 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow221 ? 1.0f : fSlow220)))));
		float fSlow223 = (1.0f - fSlow222);
		float fSlow224 = (1.0f - fSlow219);
		float fSlow225 = float(fHslider67);
		float fSlow226 = (1.0f / float(fHslider68));
		float fSlow227 = std::tan((fConst1 * float(fHslider69)));
		float fSlow228 = (1.0f / fSlow227);
		float fSlow229 = (1.0f / (((fSlow226 + fSlow228) / fSlow227) + 1.0f));
		float fSlow230 = (((fSlow228 - fSlow226) / fSlow227) + 1.0f);
		float fSlow231 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow227))));
		float fSlow232 = (1.0f / fSlow208);
		float fSlow233 = float(fHslider70);
		int iSlow234 = (fSlow233 > 0.0f);
		float fSlow235 = (float(fHslider71) * std::sin((fConst5 * fSlow207)));
		float fSlow236 = (fConst1 * ((fSlow207 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow233)))) / fSlow235));
		float fSlow237 = (fConst1 * (fSlow207 / fSlow235));
		float fSlow238 = (iSlow234 ? fSlow237 : fSlow236);
		float fSlow239 = ((fSlow232 * (fSlow232 - fSlow238)) + 1.0f);
		float fSlow240 = ((fSlow232 * (fSlow232 + fSlow238)) + 1.0f);
		float fSlow241 = (iSlow234 ? fSlow236 : fSlow237);
		float fSlow242 = ((fSlow232 * (fSlow232 + fSlow241)) + 1.0f);
		float fSlow243 = ((fSlow232 * (fSlow232 - fSlow241)) + 1.0f);
		float fSlow244 = (1.0f / fSlow205);
		float fSlow245 = float(fHslider72);
		int iSlow246 = (fSlow245 > 0.0f);
		float fSlow247 = (float(fHslider73) * std::sin((fConst5 * fSlow204)));
		float fSlow248 = (fConst1 * ((fSlow204 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow245)))) / fSlow247));
		float fSlow249 = (fConst1 * (fSlow204 / fSlow247));
		float fSlow250 = (iSlow246 ? fSlow249 : fSlow248);
		float fSlow251 = ((fSlow244 * (fSlow244 - fSlow250)) + 1.0f);
		float fSlow252 = ((fSlow244 * (fSlow244 + fSlow250)) + 1.0f);
		float fSlow253 = (iSlow246 ? fSlow248 : fSlow249);
		float fSlow254 = ((fSlow244 * (fSlow244 + fSlow253)) + 1.0f);
		float fSlow255 = ((fSlow244 * (fSlow244 - fSlow253)) + 1.0f);
		float fSlow256 = (1.0f / fSlow201);
		float fSlow257 = (((fSlow200 - fSlow198) / fSlow199) + 1.0f);
		float fSlow258 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow199))));
		float fSlow259 = (1.0f / float(fHslider75));
		float fSlow260 = std::tan((fConst1 * float(fHslider76)));
		float fSlow261 = (1.0f / fSlow260);
		float fSlow262 = (((fSlow259 + fSlow261) / fSlow260) + 1.0f);
		float fSlow263 = ((std::pow(10.0f, (0.0500000007f * float(fHslider74))) * (1.0f - float(fCheckbox4))) / fSlow262);
		float fSlow264 = float(fHslider77);
		float fSlow265 = std::tan((fConst1 * fSlow264));
		float fSlow266 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow265))));
		float fSlow267 = float(fHslider78);
		float fSlow268 = std::tan((fConst1 * fSlow267));
		float fSlow269 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow268))));
		float fSlow270 = (0.5f * fSlow149);
		float fSlow271 = (fConst28 * fSlow83);
		float fSlow272 = (27.0f * fSlow83);
		float fSlow273 = (9.0f * fSlow83);
		float fSlow274 = (3.0f * fSlow83);
		float fSlow275 = (fConst3 * ((1.0f / float(fHslider79)) + -1.0f));
		float fSlow276 = float(fHslider80);
		int iSlow277 = (std::fabs(fSlow276) < 1.1920929e-07f);
		float fSlow278 = (iSlow277 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow277 ? 1.0f : fSlow276)))));
		float fSlow279 = float(fHslider81);
		int iSlow280 = (std::fabs(fSlow279) < 1.1920929e-07f);
		float fSlow281 = (iSlow280 ? 0.0f : std::exp((0.0f - (fConst4 / (iSlow280 ? 1.0f : fSlow279)))));
		float fSlow282 = (1.0f - fSlow281);
		float fSlow283 = (1.0f - fSlow278);
		float fSlow284 = float(fHslider82);
		float fSlow285 = (1.0f / float(fHslider83));
		float fSlow286 = std::tan((fConst1 * float(fHslider84)));
		float fSlow287 = (1.0f / fSlow286);
		float fSlow288 = (1.0f / (((fSlow285 + fSlow287) / fSlow286) + 1.0f));
		float fSlow289 = (((fSlow287 - fSlow285) / fSlow286) + 1.0f);
		float fSlow290 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow286))));
		float fSlow291 = (1.0f / fSlow268);
		float fSlow292 = float(fHslider85);
		int iSlow293 = (fSlow292 > 0.0f);
		float fSlow294 = (float(fHslider86) * std::sin((fConst5 * fSlow267)));
		float fSlow295 = (fConst1 * ((fSlow267 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow292)))) / fSlow294));
		float fSlow296 = (fConst1 * (fSlow267 / fSlow294));
		float fSlow297 = (iSlow293 ? fSlow296 : fSlow295);
		float fSlow298 = ((fSlow291 * (fSlow291 - fSlow297)) + 1.0f);
		float fSlow299 = ((fSlow291 * (fSlow291 + fSlow297)) + 1.0f);
		float fSlow300 = (iSlow293 ? fSlow295 : fSlow296);
		float fSlow301 = ((fSlow291 * (fSlow291 + fSlow300)) + 1.0f);
		float fSlow302 = ((fSlow291 * (fSlow291 - fSlow300)) + 1.0f);
		float fSlow303 = (1.0f / fSlow265);
		float fSlow304 = float(fHslider87);
		int iSlow305 = (fSlow304 > 0.0f);
		float fSlow306 = (float(fHslider88) * std::sin((fConst5 * fSlow264)));
		float fSlow307 = (fConst1 * ((fSlow264 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow304)))) / fSlow306));
		float fSlow308 = (fConst1 * (fSlow264 / fSlow306));
		float fSlow309 = (iSlow305 ? fSlow308 : fSlow307);
		float fSlow310 = ((fSlow303 * (fSlow303 - fSlow309)) + 1.0f);
		float fSlow311 = ((fSlow303 * (fSlow303 + fSlow309)) + 1.0f);
		float fSlow312 = (iSlow305 ? fSlow307 : fSlow308);
		float fSlow313 = ((fSlow303 * (fSlow303 + fSlow312)) + 1.0f);
		float fSlow314 = ((fSlow303 * (fSlow303 - fSlow312)) + 1.0f);
		float fSlow315 = (1.0f / fSlow262);
		float fSlow316 = (((fSlow261 - fSlow259) / fSlow260) + 1.0f);
		float fSlow317 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow260))));
		float fSlow318 = (1.41400003f * std::sqrt(fSlow0));
		float fSlow319 = (0.949999988f * fSlow202);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fVec1[0] = fSlow14;
			fVec2[0] = fSlow19;
			int iTemp0 = ((fSlow14 > fVec1[1]) + (iSlow18 * (fSlow19 > fVec2[1])));
			iRec3[0] = ((iTemp0 > 0) ? 0 : std::min<int>(96000, (iRec3[1] + 1)));
			fRec4[0] = (iTemp0 ? fSlow28 : fRec4[1]);
			float fTemp1 = ((iSlow15 ? (iSlow23 ? fSoundfile0ca_bu_ch5[(fSoundfile0ca_of5[5] + std::max<int>(0, std::min<int>(iRec3[0], iSlow27)))] : (iSlow24 ? fSoundfile0ca_bu_ch4[(fSoundfile0ca_of4[4] + std::max<int>(0, std::min<int>(iRec3[0], iSlow26)))] : fSoundfile0ca_bu_ch3[(fSoundfile0ca_of3[3] + std::max<int>(0, std::min<int>(iRec3[0], iSlow25)))])) : (iSlow16 ? fSoundfile0ca_bu_ch2[(fSoundfile0ca_of2[2] + std::max<int>(0, std::min<int>(iRec3[0], iSlow22)))] : (iSlow17 ? fSoundfile0ca_bu_ch1[(fSoundfile0ca_of1[1] + std::max<int>(0, std::min<int>(iRec3[0], iSlow21)))] : fSoundfile0ca_bu_ch0[(fSoundfile0ca_of0[0] + std::max<int>(0, std::min<int>(iRec3[0], iSlow20)))]))) * std::pow(10.0f, (0.144444451f * (fRec4[0] + -10.0f))));
			float fTemp2 = std::fabs((0.25f * fTemp1));
			fRec7[0] = std::max<float>(fTemp2, ((fRec7[1] * fSlow35) + (fTemp2 * fSlow36)));
			fRec6[0] = ((fRec6[1] * fSlow32) + (fRec7[0] * fSlow37));
			fRec5[0] = ((fConst2 * fRec5[1]) + (fSlow29 * std::max<float>(((20.0f * std::log10(fRec6[0])) - fSlow38), 0.0f)));
			float fTemp3 = (0.25f * (fTemp1 * std::pow(10.0f, (0.0500000007f * fRec5[0]))));
			fRec8[0] = (fTemp3 - (fSlow42 * ((fSlow43 * fRec8[2]) + (fSlow44 * fRec8[1]))));
			float fTemp4 = (fSlow13 * fRec2[1]);
			fRec2[0] = (fTemp3 - ((fSlow42 * (fRec8[2] + (fRec8[0] + (2.0f * fRec8[1])))) + (((fRec2[2] * fSlow52) + fTemp4) / fSlow53)));
			float fTemp5 = (fSlow10 * fRec1[1]);
			fRec1[0] = ((((fTemp4 + (fRec2[0] * fSlow55)) + (fRec2[2] * fSlow56)) / fSlow53) - (((fRec1[2] * fSlow64) + fTemp5) / fSlow65));
			fRec0[0] = ((((fTemp5 + (fRec1[0] * fSlow67)) + (fRec1[2] * fSlow68)) / fSlow65) - (fSlow69 * ((fSlow70 * fRec0[2]) + (fSlow71 * fRec0[1]))));
			fVec3[0] = fSlow85;
			fVec4[0] = fSlow87;
			int iTemp6 = ((fSlow85 > fVec3[1]) + (iSlow86 * (fSlow87 > fVec4[1])));
			iVec5[0] = iTemp6;
			fRec13[0] = (iTemp6 ? fSlow88 : fRec13[1]);
			fRec16[0] = (fConst10 + (fRec16[1] - std::floor((fConst10 + fRec16[1]))));
			fRec14[0] = ((0.999000013f * fRec14[1]) + (0.00100000005f * ((fSlow90 * ftbl0mydspSIG0[int((65536.0f * fRec16[0]))]) + 1.0f)));
			float fTemp7 = (0.5f * std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow84 * (fRec13[0] * fRec14[0])))));
			float fTemp8 = ((0.00366666657f * (400.0f - fTemp7)) + 3.0f);
			float fTemp9 = std::max<float>(1.00000001e-07f, std::fabs(fTemp7));
			float fTemp10 = (fRec18[1] + (fConst4 * fTemp9));
			float fTemp11 = (fTemp10 + -1.0f);
			int iTemp12 = (fTemp11 < 0.0f);
			fRec18[0] = (iTemp12 ? fTemp10 : fTemp11);
			float fRec19 = (iTemp12 ? fTemp10 : (fTemp10 + (fTemp11 * (1.0f - (fConst0 / fTemp9)))));
			float fTemp13 = (0.0399999991f * ((2.0f * fRec19) + -1.0f));
			fRec20[0] = (fSlow91 + (fConst11 * fRec20[1]));
			float fTemp14 = (fRec20[0] + 10.0f);
			int iTemp15 = (fTemp14 < 23.0f);
			int iTemp16 = (fTemp14 < 24.0f);
			float fTemp17 = (fRec20[0] + -13.0f);
			int iTemp18 = (fTemp14 < 22.0f);
			float fTemp19 = (fRec20[0] + -12.0f);
			int iTemp20 = (fTemp14 < 21.0f);
			float fTemp21 = (fRec20[0] + -11.0f);
			int iTemp22 = (fTemp14 < 20.0f);
			float fTemp23 = (fRec20[0] + -10.0f);
			int iTemp24 = (fTemp14 < 19.0f);
			float fTemp25 = (fRec20[0] + -9.0f);
			int iTemp26 = (fTemp14 < 18.0f);
			float fTemp27 = (fRec20[0] + -8.0f);
			int iTemp28 = (fTemp14 < 17.0f);
			float fTemp29 = (fRec20[0] + -7.0f);
			int iTemp30 = (fTemp14 < 16.0f);
			float fTemp31 = (fRec20[0] + -6.0f);
			float fTemp32 = (80.0f * fTemp31);
			int iTemp33 = (fTemp14 < 15.0f);
			float fTemp34 = (fRec20[0] + -5.0f);
			int iTemp35 = (fTemp14 < 14.0f);
			float fTemp36 = (fRec20[0] + -4.0f);
			int iTemp37 = (fTemp14 < 13.0f);
			float fTemp38 = (fRec20[0] + -3.0f);
			int iTemp39 = (fTemp14 < 12.0f);
			float fTemp40 = (fRec20[0] + -2.0f);
			int iTemp41 = (fTemp14 < 11.0f);
			float fTemp42 = (fRec20[0] + -1.0f);
			int iTemp43 = (fTemp14 < 10.0f);
			int iTemp44 = (fTemp14 < 9.0f);
			float fTemp45 = (fRec20[0] + 1.0f);
			int iTemp46 = (fTemp14 < 8.0f);
			float fTemp47 = (fRec20[0] + 2.0f);
			float fTemp48 = (50.0f * fTemp47);
			int iTemp49 = (fTemp14 < 7.0f);
			float fTemp50 = (fRec20[0] + 3.0f);
			int iTemp51 = (fTemp14 < 6.0f);
			float fTemp52 = (fRec20[0] + 4.0f);
			int iTemp53 = (fTemp14 < 5.0f);
			float fTemp54 = (fRec20[0] + 5.0f);
			int iTemp55 = (fTemp14 < 4.0f);
			float fTemp56 = (fRec20[0] + 6.0f);
			int iTemp57 = (fTemp14 < 3.0f);
			float fTemp58 = (fRec20[0] + 7.0f);
			int iTemp59 = (fTemp14 < 2.0f);
			float fTemp60 = (fRec20[0] + 8.0f);
			int iTemp61 = (fTemp14 < 1.0f);
			float fTemp62 = (fRec20[0] + 9.0f);
			int iTemp63 = (fTemp14 < 0.0f);
			float fTemp64 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 800.0f : (iTemp61 ? (800.0f - (400.0f * fTemp14)) : 400.0f)) : (iTemp59 ? (400.0f - (50.0f * fTemp62)) : 350.0f)) : (iTemp57 ? ((100.0f * fTemp60) + 350.0f) : 450.0f)) : (iTemp55 ? (450.0f - (125.0f * fTemp58)) : 325.0f)) : (iTemp53 ? ((275.0f * fTemp56) + 325.0f) : 600.0f)) : (iTemp51 ? (600.0f - (200.0f * fTemp54)) : 400.0f)) : (iTemp49 ? (400.0f - (150.0f * fTemp52)) : 250.0f)) : (iTemp46 ? ((150.0f * fTemp50) + 250.0f) : 400.0f)) : (iTemp44 ? (400.0f - fTemp48) : 350.0f)) : (iTemp43 ? ((310.0f * fTemp45) + 350.0f) : 660.0f)) : (iTemp41 ? (660.0f - (220.0f * fRec20[0])) : 440.0f)) : (iTemp39 ? (440.0f - (170.0f * fTemp42)) : 270.0f)) : (iTemp37 ? ((160.0f * fTemp40) + 270.0f) : 430.0f)) : (iTemp35 ? (430.0f - (60.0f * fTemp38)) : 370.0f)) : (iTemp33 ? ((430.0f * fTemp36) + 370.0f) : 800.0f)) : (iTemp30 ? (800.0f - (450.0f * fTemp34)) : 350.0f)) : (iTemp28 ? (350.0f - fTemp32) : 270.0f)) : (iTemp26 ? ((180.0f * fTemp29) + 270.0f) : 450.0f)) : (iTemp24 ? (450.0f - (125.0f * fTemp27)) : 325.0f)) : (iTemp22 ? (325.0f * (fTemp25 + 1.0f)) : 650.0f)) : (iTemp20 ? (650.0f - (250.0f * fTemp23)) : 400.0f)) : (iTemp18 ? (400.0f - (110.0f * fTemp21)) : 290.0f)) : (iTemp15 ? ((110.0f * fTemp19) + 290.0f) : 400.0f)) : (iTemp16 ? (400.0f - (50.0f * fTemp17)) : 350.0f));
			float fTemp65 = ((fTemp64 <= fTemp7) ? fTemp7 : fTemp64);
			float fTemp66 = std::tan((fConst1 * fTemp65));
			float fTemp67 = (1.0f / fTemp66);
			float fTemp68 = (20.0f * fTemp23);
			float fTemp69 = (20.0f * fTemp34);
			float fTemp70 = (10.0f * fRec20[0]);
			float fTemp71 = (10.0f * fTemp56);
			float fTemp72 = (20.0f * fTemp58);
			float fTemp73 = (20.0f * fTemp60);
			float fTemp74 = (20.0f * fTemp14);
			float fTemp75 = ((iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 80.0f : (iTemp61 ? (80.0f - fTemp74) : 60.0f)) : (iTemp59 ? (60.0f - (10.0f * fTemp62)) : 50.0f)) : (iTemp57 ? (fTemp73 + 50.0f) : 70.0f)) : (iTemp55 ? (70.0f - fTemp72) : 50.0f)) : (iTemp53 ? (fTemp71 + 50.0f) : 60.0f)) : (iTemp51 ? (60.0f - (20.0f * fTemp54)) : 40.0f)) : (iTemp49 ? ((20.0f * fTemp52) + 40.0f) : 60.0f)) : (iTemp46 ? (60.0f - (20.0f * fTemp50)) : 40.0f)) : 40.0f) : (iTemp43 ? (40.0f * (fTemp45 + 1.0f)) : 80.0f)) : (iTemp41 ? (80.0f - fTemp70) : 70.0f)) : (iTemp39 ? (70.0f - (30.0f * fTemp42)) : 40.0f)) : 40.0f) : 40.0f) : (iTemp33 ? (40.0f * (fTemp36 + 1.0f)) : 80.0f)) : (iTemp30 ? (80.0f - fTemp69) : 60.0f)) : 60.0f) : (iTemp26 ? (60.0f - (20.0f * fTemp29)) : 40.0f)) : (iTemp24 ? ((10.0f * fTemp27) + 40.0f) : 50.0f)) : 50.0f) : (iTemp20 ? (fTemp68 + 50.0f) : 70.0f)) : (iTemp18 ? (70.0f - (30.0f * fTemp21)) : 40.0f)) : (iTemp15 ? ((30.0f * fTemp19) + 40.0f) : 70.0f)) : (iTemp16 ? (70.0f - (30.0f * fTemp17)) : 40.0f)) / fTemp65);
			float fTemp76 = (((fTemp67 + fTemp75) / fTemp66) + 1.0f);
			fRec17[0] = (fTemp13 - (((fRec17[2] * (((fTemp67 - fTemp75) / fTemp66) + 1.0f)) + (2.0f * (fRec17[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp66)))))) / fTemp76));
			float fTemp77 = (fTemp66 * fTemp76);
			float fTemp78 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 1150.0f : (iTemp61 ? ((450.0f * fTemp14) + 1150.0f) : 1600.0f)) : (iTemp59 ? ((100.0f * fTemp62) + 1600.0f) : 1700.0f)) : (iTemp57 ? (1700.0f - (900.0f * fTemp60)) : 800.0f)) : (iTemp55 ? (800.0f - (100.0f * fTemp58)) : 700.0f)) : (iTemp53 ? ((340.0f * fTemp56) + 700.0f) : 1040.0f)) : (iTemp51 ? ((580.0f * fTemp54) + 1040.0f) : 1620.0f)) : (iTemp49 ? ((130.0f * fTemp52) + 1620.0f) : 1750.0f)) : (iTemp46 ? (1750.0f - (1000.0f * fTemp50)) : 750.0f)) : (iTemp44 ? (750.0f - (150.0f * fTemp47)) : 600.0f)) : (iTemp43 ? ((520.0f * fTemp45) + 600.0f) : 1120.0f)) : (iTemp41 ? ((680.0f * fRec20[0]) + 1120.0f) : 1800.0f)) : (iTemp39 ? ((50.0f * fTemp42) + 1800.0f) : 1850.0f)) : (iTemp37 ? (1850.0f - (1030.0f * fTemp40)) : 820.0f)) : (iTemp35 ? (820.0f - (190.0f * fTemp38)) : 630.0f)) : (iTemp33 ? ((520.0f * fTemp36) + 630.0f) : 1150.0f)) : (iTemp30 ? ((850.0f * fTemp34) + 1150.0f) : 2000.0f)) : (iTemp28 ? ((140.0f * fTemp31) + 2000.0f) : 2140.0f)) : (iTemp26 ? (2140.0f - (1340.0f * fTemp29)) : 800.0f)) : (iTemp24 ? (800.0f - (100.0f * fTemp27)) : 700.0f)) : (iTemp22 ? ((380.0f * fTemp25) + 700.0f) : 1080.0f)) : (iTemp20 ? ((620.0f * fTemp23) + 1080.0f) : 1700.0f)) : (iTemp18 ? ((170.0f * fTemp21) + 1700.0f) : 1870.0f)) : (iTemp15 ? (1870.0f - (1070.0f * fTemp19)) : 800.0f)) : (iTemp16 ? (800.0f - (200.0f * fTemp17)) : 600.0f));
			float fTemp79 = std::tan((fConst1 * fTemp78));
			float fTemp80 = (1.0f / fTemp79);
			float fTemp81 = (10.0f * fTemp19);
			float fTemp82 = (10.0f * fTemp23);
			float fTemp83 = (10.0f * fTemp45);
			float fTemp84 = (10.0f * fTemp54);
			float fTemp85 = ((iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 90.0f : (iTemp61 ? (90.0f - (10.0f * fTemp14)) : 80.0f)) : (iTemp59 ? ((20.0f * fTemp62) + 80.0f) : 100.0f)) : (iTemp57 ? (100.0f - fTemp73) : 80.0f)) : (iTemp55 ? (80.0f - fTemp72) : 60.0f)) : (iTemp53 ? (fTemp71 + 60.0f) : 70.0f)) : (iTemp51 ? (fTemp84 + 70.0f) : 80.0f)) : (iTemp49 ? ((10.0f * fTemp52) + 80.0f) : 90.0f)) : (iTemp46 ? (90.0f - (10.0f * fTemp50)) : 80.0f)) : 80.0f) : (iTemp43 ? (fTemp83 + 80.0f) : 90.0f)) : (iTemp41 ? (90.0f - fTemp70) : 80.0f)) : (iTemp39 ? ((10.0f * fTemp42) + 80.0f) : 90.0f)) : (iTemp37 ? (90.0f - (10.0f * fTemp40)) : 80.0f)) : (iTemp35 ? (80.0f - (20.0f * fTemp38)) : 60.0f)) : (iTemp33 ? ((30.0f * fTemp36) + 60.0f) : 90.0f)) : (iTemp30 ? ((10.0f * fTemp34) + 90.0f) : 100.0f)) : (iTemp28 ? (100.0f - (10.0f * fTemp31)) : 90.0f)) : (iTemp26 ? (90.0f - (10.0f * fTemp29)) : 80.0f)) : (iTemp24 ? (80.0f - (20.0f * fTemp27)) : 60.0f)) : (iTemp22 ? ((30.0f * fTemp25) + 60.0f) : 90.0f)) : (iTemp20 ? (90.0f - fTemp82) : 80.0f)) : (iTemp18 ? ((10.0f * fTemp21) + 80.0f) : 90.0f)) : (iTemp15 ? (90.0f - fTemp81) : 80.0f)) : (iTemp16 ? (80.0f - (20.0f * fTemp17)) : 60.0f)) / fTemp78);
			float fTemp86 = (((fTemp80 + fTemp85) / fTemp79) + 1.0f);
			fRec21[0] = (fTemp13 - (((fRec21[2] * (((fTemp80 - fTemp85) / fTemp79) + 1.0f)) + (2.0f * (fRec21[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp79)))))) / fTemp86));
			float fTemp87 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 0.630957007f : (iTemp61 ? (0.630957007f - (0.567861021f * fTemp14)) : 0.0630960017f)) : (iTemp59 ? ((0.0369039997f * fTemp62) + 0.0630960017f) : 0.100000001f)) : (iTemp57 ? ((0.254812986f * fTemp60) + 0.100000001f) : 0.35481301f)) : (iTemp55 ? (0.35481301f - (0.103624001f * fTemp58)) : 0.251188993f)) : (iTemp53 ? ((0.195494995f * fTemp56) + 0.251188993f) : 0.446684003f)) : (iTemp51 ? (0.446684003f - (0.195494995f * fTemp54)) : 0.251188993f)) : (iTemp49 ? (0.251188993f - (0.219566002f * fTemp52)) : 0.0316229984f)) : (iTemp46 ? ((0.250214994f * fTemp50) + 0.0316229984f) : 0.281838f)) : (iTemp44 ? (0.281838f - (0.181838006f * fTemp47)) : 0.100000001f)) : (iTemp43 ? ((0.401187003f * fTemp45) + 0.100000001f) : 0.501187027f)) : (iTemp41 ? (0.501187027f - (0.301661015f * fRec20[0])) : 0.199525997f)) : (iTemp39 ? (0.199525997f - (0.136429995f * fTemp42)) : 0.0630960017f)) : (iTemp37 ? ((0.253131986f * fTemp40) + 0.0630960017f) : 0.316228002f)) : (iTemp35 ? (0.316228002f - (0.216227993f * fTemp38)) : 0.100000001f)) : (iTemp33 ? ((0.401187003f * fTemp36) + 0.100000001f) : 0.501187027f)) : (iTemp30 ? (0.501187027f - (0.401187003f * fTemp34)) : 0.100000001f)) : (iTemp28 ? ((0.151188999f * fTemp31) + 0.100000001f) : 0.251188993f)) : (iTemp26 ? ((0.0306490008f * fTemp29) + 0.251188993f) : 0.281838f)) : (iTemp24 ? (0.281838f - (0.123349003f * fTemp27)) : 0.158489004f)) : (iTemp22 ? ((0.342698008f * fTemp25) + 0.158489004f) : 0.501187027f)) : (iTemp20 ? (0.501187027f - (0.301661015f * fTemp23)) : 0.199525997f)) : (iTemp18 ? (0.199525997f - (0.0216979999f * fTemp21)) : 0.177827999f)) : (iTemp15 ? ((0.138400003f * fTemp19) + 0.177827999f) : 0.316228002f)) : (iTemp16 ? (0.316228002f - (0.216227993f * fTemp17)) : 0.100000001f));
			float fTemp88 = (fTemp79 * fTemp86);
			float fTemp89 = (100.0f * fTemp17);
			float fTemp90 = (50.0f * fTemp25);
			float fTemp91 = (50.0f * fRec20[0]);
			float fTemp92 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 2800.0f : (iTemp61 ? (2800.0f - (100.0f * fTemp14)) : 2700.0f)) : 2700.0f) : (iTemp57 ? ((130.0f * fTemp60) + 2700.0f) : 2830.0f)) : (iTemp55 ? (2830.0f - (300.0f * fTemp58)) : 2530.0f)) : (iTemp53 ? (2530.0f - (280.0f * fTemp56)) : 2250.0f)) : (iTemp51 ? ((150.0f * fTemp54) + 2250.0f) : 2400.0f)) : (iTemp49 ? ((200.0f * fTemp52) + 2400.0f) : 2600.0f)) : (iTemp46 ? (2600.0f - (200.0f * fTemp50)) : 2400.0f)) : 2400.0f) : (iTemp43 ? ((350.0f * fTemp45) + 2400.0f) : 2750.0f)) : (iTemp41 ? (2750.0f - fTemp91) : 2700.0f)) : (iTemp39 ? ((200.0f * fTemp42) + 2700.0f) : 2900.0f)) : (iTemp37 ? (2900.0f - (200.0f * fTemp40)) : 2700.0f)) : (iTemp35 ? ((50.0f * fTemp38) + 2700.0f) : 2750.0f)) : (iTemp33 ? ((150.0f * fTemp36) + 2750.0f) : 2900.0f)) : (iTemp30 ? (2900.0f - (100.0f * fTemp34)) : 2800.0f)) : (iTemp28 ? ((150.0f * fTemp31) + 2800.0f) : 2950.0f)) : (iTemp26 ? (2950.0f - (120.0f * fTemp29)) : 2830.0f)) : (iTemp24 ? (2830.0f - (130.0f * fTemp27)) : 2700.0f)) : (iTemp22 ? (2700.0f - fTemp90) : 2650.0f)) : (iTemp20 ? (2650.0f - (50.0f * fTemp23)) : 2600.0f)) : (iTemp18 ? ((200.0f * fTemp21) + 2600.0f) : 2800.0f)) : (iTemp15 ? (2800.0f - (200.0f * fTemp19)) : 2600.0f)) : (iTemp16 ? (fTemp89 + 2600.0f) : 2700.0f));
			float fTemp93 = std::tan((fConst1 * fTemp92));
			float fTemp94 = (1.0f / fTemp93);
			float fTemp95 = (20.0f * fTemp36);
			float fTemp96 = (20.0f * fRec20[0]);
			float fTemp97 = (20.0f * fTemp45);
			float fTemp98 = (60.0f * fTemp56);
			float fTemp99 = ((iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? 120.0f : (iTemp57 ? (120.0f - fTemp73) : 100.0f)) : (iTemp55 ? ((70.0f * fTemp58) + 100.0f) : 170.0f)) : (iTemp53 ? (170.0f - fTemp98) : 110.0f)) : (iTemp51 ? (110.0f - fTemp84) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp43 ? (fTemp97 + 100.0f) : 120.0f)) : (iTemp41 ? (120.0f - fTemp96) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp33 ? (fTemp95 + 100.0f) : 120.0f)) : 120.0f) : (iTemp28 ? (120.0f - (20.0f * fTemp31)) : 100.0f)) : 100.0f) : (iTemp24 ? ((70.0f * fTemp27) + 100.0f) : 170.0f)) : (iTemp22 ? (170.0f - fTemp90) : 120.0f)) : (iTemp20 ? (120.0f - fTemp68) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fTemp92);
			float fTemp100 = (((fTemp94 + fTemp99) / fTemp93) + 1.0f);
			fRec22[0] = (fTemp13 - (((fRec22[2] * (((fTemp94 - fTemp99) / fTemp93) + 1.0f)) + (2.0f * (fRec22[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp93)))))) / fTemp100));
			float fTemp101 = (iTemp26 ? ((0.0293140002f * fTemp29) + 0.0501190014f) : 0.0794330016f);
			float fTemp102 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 0.100000001f : (iTemp61 ? (0.100000001f - (0.0683770031f * fTemp14)) : 0.0316229984f)) : 0.0316229984f) : (iTemp57 ? ((0.126865998f * fTemp60) + 0.0316229984f) : 0.158489004f)) : (iTemp55 ? (0.158489004f - (0.126865998f * fTemp58)) : 0.0316229984f)) : (iTemp53 ? ((0.323190004f * fTemp56) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iTemp49 ? (0.35481301f - (0.196324006f * fTemp52)) : 0.158489004f)) : (iTemp46 ? (0.158489004f - (0.0693639964f * fTemp50)) : 0.0891249999f)) : (iTemp44 ? (0.0891249999f - (0.0640060008f * fTemp47)) : 0.0251189992f)) : (iTemp43 ? ((0.0456760004f * fTemp45) + 0.0251189992f) : 0.0707949996f)) : (iTemp41 ? ((0.0550980009f * fRec20[0]) + 0.0707949996f) : 0.125892997f)) : (iTemp39 ? (0.125892997f - (0.0627970025f * fTemp42)) : 0.0630960017f)) : (iTemp37 ? (0.0630960017f - (0.0129770003f * fTemp40)) : 0.0501190014f)) : (iTemp35 ? ((0.020676f * fTemp38) + 0.0501190014f) : 0.0707949996f)) : (iTemp33 ? (0.0707949996f - (0.0456760004f * fTemp36)) : 0.0251189992f)) : (iTemp30 ? ((0.152709007f * fTemp34) + 0.0251189992f) : 0.177827999f)) : (iTemp28 ? (0.177827999f - (0.127709001f * fTemp31)) : 0.0501190014f)) : fTemp101) : (iTemp24 ? (0.0794330016f - (0.0616500005f * fTemp27)) : 0.0177829992f)) : (iTemp22 ? ((0.428900987f * fTemp25) + 0.0177829992f) : 0.446684003f)) : (iTemp20 ? (0.446684003f - (0.195494995f * fTemp23)) : 0.251188993f)) : (iTemp18 ? (0.251188993f - (0.125295997f * fTemp21)) : 0.125892997f)) : (iTemp15 ? ((0.125295997f * fTemp19) + 0.125892997f) : 0.251188993f)) : (iTemp16 ? (0.251188993f - (0.109935001f * fTemp17)) : 0.141253993f));
			float fTemp103 = (fTemp93 * fTemp100);
			float fTemp104 = (350.0f * fTemp54);
			float fTemp105 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 3500.0f : (iTemp61 ? (3500.0f - (200.0f * fTemp14)) : 3300.0f)) : (iTemp59 ? ((400.0f * fTemp62) + 3300.0f) : 3700.0f)) : (iTemp57 ? (3700.0f - (200.0f * fTemp60)) : 3500.0f)) : 3500.0f) : (iTemp53 ? (3500.0f - (1050.0f * fTemp56)) : 2450.0f)) : (iTemp51 ? (fTemp104 + 2450.0f) : 2800.0f)) : (iTemp49 ? ((250.0f * fTemp52) + 2800.0f) : 3050.0f)) : (iTemp46 ? (3050.0f - (450.0f * fTemp50)) : 2600.0f)) : (iTemp44 ? ((75.0f * fTemp47) + 2600.0f) : 2675.0f)) : (iTemp43 ? ((325.0f * fTemp45) + 2675.0f) : 3000.0f)) : 3000.0f) : (iTemp39 ? ((350.0f * fTemp42) + 3000.0f) : 3350.0f)) : (iTemp37 ? (3350.0f - (350.0f * fTemp40)) : 3000.0f)) : 3000.0f) : (iTemp33 ? ((900.0f * fTemp36) + 3000.0f) : 3900.0f)) : (iTemp30 ? (3900.0f - (300.0f * fTemp34)) : 3600.0f)) : (iTemp28 ? ((300.0f * fTemp31) + 3600.0f) : 3900.0f)) : (iTemp26 ? (3900.0f - (100.0f * fTemp29)) : 3800.0f)) : 3800.0f) : (iTemp22 ? (3800.0f - (900.0f * fTemp25)) : 2900.0f)) : (iTemp20 ? ((300.0f * fTemp23) + 2900.0f) : 3200.0f)) : (iTemp18 ? ((50.0f * fTemp21) + 3200.0f) : 3250.0f)) : (iTemp15 ? (3250.0f - (450.0f * fTemp19)) : 2800.0f)) : (iTemp16 ? (fTemp89 + 2800.0f) : 2900.0f));
			float fTemp106 = std::tan((fConst1 * fTemp105));
			float fTemp107 = (1.0f / fTemp106);
			float fTemp108 = ((iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 130.0f : (iTemp61 ? (fTemp74 + 130.0f) : 150.0f)) : 150.0f) : (iTemp57 ? (150.0f - fTemp73) : 130.0f)) : (iTemp55 ? ((50.0f * fTemp58) + 130.0f) : 180.0f)) : (iTemp53 ? (180.0f - fTemp98) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iTemp43 ? (fTemp83 + 120.0f) : 130.0f)) : (iTemp41 ? (130.0f - fTemp70) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp33 ? ((10.0f * fTemp36) + 120.0f) : 130.0f)) : (iTemp30 ? (fTemp69 + 130.0f) : 150.0f)) : (iTemp28 ? (150.0f - (30.0f * fTemp31)) : 120.0f)) : 120.0f) : (iTemp24 ? ((60.0f * fTemp27) + 120.0f) : 180.0f)) : (iTemp22 ? (180.0f - fTemp90) : 130.0f)) : (iTemp20 ? (130.0f - fTemp82) : 120.0f)) : 120.0f) : (iTemp15 ? (fTemp81 + 120.0f) : 130.0f)) : (iTemp16 ? (130.0f - (10.0f * fTemp17)) : 120.0f)) / fTemp105);
			float fTemp109 = (((fTemp107 + fTemp108) / fTemp106) + 1.0f);
			fRec23[0] = (fTemp13 - (((fRec23[2] * (((fTemp107 - fTemp108) / fTemp106) + 1.0f)) + (2.0f * (fRec23[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp106)))))) / fTemp109));
			float fTemp110 = (iTemp39 ? (0.100000001f - (0.0841509998f * fTemp42)) : 0.0158489998f);
			float fTemp111 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 0.0158489998f : (iTemp61 ? ((0.00193400006f * fTemp14) + 0.0158489998f) : 0.0177829992f)) : (iTemp59 ? (0.0177829992f - (0.00193400006f * fTemp62)) : 0.0158489998f)) : (iTemp57 ? ((0.0239620004f * fTemp60) + 0.0158489998f) : 0.0398110002f)) : (iTemp55 ? (0.0398110002f - (0.0298110005f * fTemp58)) : 0.00999999978f)) : (iTemp53 ? ((0.344812989f * fTemp56) + 0.00999999978f) : 0.35481301f)) : (iTemp51 ? (0.35481301f - (0.103624001f * fTemp54)) : 0.251188993f)) : (iTemp49 ? (0.251188993f - (0.171755999f * fTemp52)) : 0.0794330016f)) : (iTemp46 ? ((0.0205669999f * fTemp50) + 0.0794330016f) : 0.100000001f)) : (iTemp44 ? (0.100000001f - (0.0601890013f * fTemp47)) : 0.0398110002f)) : (iTemp43 ? ((0.0232849997f * fTemp45) + 0.0398110002f) : 0.0630960017f)) : (iTemp41 ? ((0.0369039997f * fRec20[0]) + 0.0630960017f) : 0.100000001f)) : fTemp110) : (iTemp37 ? ((0.0635839999f * fTemp40) + 0.0158489998f) : 0.0794330016f)) : (iTemp35 ? (0.0794330016f - (0.0478099994f * fTemp38)) : 0.0316229984f)) : (iTemp33 ? ((0.0683770031f * fTemp36) + 0.0316229984f) : 0.100000001f)) : (iTemp30 ? (0.100000001f - (0.0900000036f * fTemp34)) : 0.00999999978f)) : (iTemp28 ? ((0.0401189998f * fTemp31) + 0.00999999978f) : 0.0501190014f)) : fTemp101) : (iTemp24 ? (0.0794330016f - (0.0694330037f * fTemp27)) : 0.00999999978f)) : (iTemp22 ? ((0.388107002f * fTemp25) + 0.00999999978f) : 0.398106992f)) : (iTemp20 ? (0.398106992f - (0.198580995f * fTemp23)) : 0.199525997f)) : (iTemp18 ? (0.199525997f - (0.099526003f * fTemp21)) : 0.100000001f)) : (iTemp15 ? ((0.151188999f * fTemp19) + 0.100000001f) : 0.251188993f)) : (iTemp16 ? (0.251188993f - (0.0516630001f * fTemp17)) : 0.199525997f));
			float fTemp112 = (fTemp106 * fTemp109);
			float fTemp113 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? 4950.0f : (iTemp53 ? (4950.0f - (2200.0f * fTemp56)) : 2750.0f)) : (iTemp51 ? (fTemp104 + 2750.0f) : 3100.0f)) : (iTemp49 ? ((240.0f * fTemp52) + 3100.0f) : 3340.0f)) : (iTemp46 ? (3340.0f - (440.0f * fTemp50)) : 2900.0f)) : (iTemp44 ? (fTemp48 + 2900.0f) : 2950.0f)) : (iTemp43 ? ((400.0f * fTemp45) + 2950.0f) : 3350.0f)) : (iTemp41 ? (3350.0f - fTemp91) : 3300.0f)) : (iTemp39 ? ((290.0f * fTemp42) + 3300.0f) : 3590.0f)) : (iTemp37 ? (3590.0f - (290.0f * fTemp40)) : 3300.0f)) : (iTemp35 ? ((100.0f * fTemp38) + 3300.0f) : 3400.0f)) : (iTemp33 ? ((1550.0f * fTemp36) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iTemp22 ? (4950.0f - (1700.0f * fTemp25)) : 3250.0f)) : (iTemp20 ? ((330.0f * fTemp23) + 3250.0f) : 3580.0f)) : (iTemp18 ? (3580.0f - (40.0f * fTemp21)) : 3540.0f)) : (iTemp15 ? (3540.0f - (540.0f * fTemp19)) : 3000.0f)) : (iTemp16 ? ((300.0f * fTemp17) + 3000.0f) : 3300.0f));
			float fTemp114 = std::tan((fConst1 * fTemp113));
			float fTemp115 = (1.0f / fTemp114);
			float fTemp116 = ((iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? 140.0f : (iTemp61 ? ((60.0f * fTemp14) + 140.0f) : 200.0f)) : 200.0f) : (iTemp57 ? (200.0f - (65.0f * fTemp60)) : 135.0f)) : (iTemp55 ? ((65.0f * fTemp58) + 135.0f) : 200.0f)) : (iTemp53 ? (200.0f - (70.0f * fTemp56)) : 130.0f)) : (iTemp51 ? (130.0f - fTemp84) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp43 ? (fTemp97 + 120.0f) : 140.0f)) : (iTemp41 ? (140.0f - fTemp96) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp33 ? (fTemp95 + 120.0f) : 140.0f)) : (iTemp30 ? ((60.0f * fTemp34) + 140.0f) : 200.0f)) : (iTemp28 ? (200.0f - fTemp32) : 120.0f)) : 120.0f) : (iTemp24 ? ((80.0f * fTemp27) + 120.0f) : 200.0f)) : (iTemp22 ? (200.0f - (60.0f * fTemp25)) : 140.0f)) : (iTemp20 ? (140.0f - fTemp68) : 120.0f)) : 120.0f) : (iTemp15 ? ((15.0f * fTemp19) + 120.0f) : 135.0f)) : (iTemp16 ? (135.0f - (15.0f * fTemp17)) : 120.0f)) / fTemp113);
			float fTemp117 = (((fTemp115 + fTemp116) / fTemp114) + 1.0f);
			fRec24[0] = (fTemp13 - (((fRec24[2] * (((fTemp115 - fTemp116) / fTemp114) + 1.0f)) + (2.0f * (fRec24[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp114)))))) / fTemp117));
			float fTemp118 = (iTemp15 ? (iTemp18 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp33 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp44 ? (iTemp46 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? 0.00100000005f : (iTemp57 ? ((0.000777999987f * fTemp60) + 0.00100000005f) : 0.00177800003f)) : (iTemp55 ? (0.00177800003f - (0.001147f * fTemp58)) : 0.000630999974f)) : (iTemp53 ? ((0.0993689969f * fTemp56) + 0.000630999974f) : 0.100000001f)) : (iTemp51 ? ((0.0258930009f * fTemp54) + 0.100000001f) : 0.125892997f)) : (iTemp49 ? (0.125892997f - (0.0860819966f * fTemp52)) : 0.0398110002f)) : (iTemp46 ? (0.0398110002f - (0.0298110005f * fTemp50)) : 0.00999999978f)) : (iTemp44 ? ((0.00584900007f * fTemp47) + 0.00999999978f) : 0.0158489998f)) : (iTemp43 ? (0.0158489998f - (0.00325999991f * fTemp45)) : 0.0125890002f)) : (iTemp41 ? ((0.0874110013f * fRec20[0]) + 0.0125890002f) : 0.100000001f)) : fTemp110) : (iTemp37 ? ((0.00410400005f * fTemp40) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iTemp33 ? (0.0199529994f - (0.0167909991f * fTemp36)) : 0.0031620001f)) : (iTemp30 ? (0.0031620001f - (0.00157700002f * fTemp34)) : 0.00158499996f)) : (iTemp28 ? ((0.00472499989f * fTemp31) + 0.00158499996f) : 0.00631000008f)) : (iTemp26 ? (0.00631000008f - (0.00314799999f * fTemp29)) : 0.0031620001f)) : (iTemp24 ? (0.0031620001f - (0.00216199993f * fTemp27)) : 0.00100000005f)) : (iTemp22 ? ((0.0784329996f * fTemp25) + 0.00100000005f) : 0.0794330016f)) : (iTemp20 ? ((0.0205669999f * fTemp23) + 0.0794330016f) : 0.100000001f)) : (iTemp18 ? (0.100000001f - (0.0683770031f * fTemp21)) : 0.0316229984f)) : (iTemp15 ? ((0.0184959993f * fTemp19) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
			float fTemp119 = (fTemp114 * fTemp117);
			iRec25[0] = (((iRec25[1] + (iRec25[1] > 0)) * (iTemp6 <= iVec5[1])) + (iTemp6 > iVec5[1]));
			float fTemp120 = float(iRec25[0]);
			fRec12[0] = ((((fTemp8 * (((((fRec17[0] / fTemp77) + ((fRec21[0] * fTemp87) / fTemp88)) + ((fRec22[0] * fTemp102) / fTemp103)) + ((fRec23[0] * fTemp111) / fTemp112)) + ((fRec24[0] * fTemp118) / fTemp119))) + (((((fRec17[2] * (0.0f - (fTemp8 / fTemp77))) + (fRec21[2] * (0.0f - ((fTemp8 * fTemp87) / fTemp88)))) + (fRec22[2] * (0.0f - ((fTemp8 * fTemp102) / fTemp103)))) + (fRec23[2] * (0.0f - ((fTemp8 * fTemp111) / fTemp112)))) + (fRec24[2] * (0.0f - ((fTemp8 * fTemp118) / fTemp119))))) * std::max<float>(0.0f, std::min<float>((fConst14 * fTemp120), ((fConst15 * (fConst13 - fTemp120)) + 1.0f)))) - (fConst16 * ((fConst17 * fRec12[2]) + (fConst18 * fRec12[1]))));
			fRec26[0] = (iTemp6 ? fSlow28 : fRec26[1]);
			float fTemp121 = (((fRec12[2] + (fRec12[0] + (2.0f * fRec12[1]))) * std::max<float>(0.0f, std::min<float>((fConst20 * fTemp120), ((fConst21 * (fConst19 - fTemp120)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec26[0] + -10.0f))));
			float fTemp122 = std::fabs((fConst9 * fTemp121));
			fRec29[0] = std::max<float>(fTemp122, ((fRec29[1] * fSlow98) + (fTemp122 * fSlow99)));
			fRec28[0] = ((fRec28[1] * fSlow95) + (fRec29[0] * fSlow100));
			fRec27[0] = ((fConst2 * fRec27[1]) + (fSlow92 * std::max<float>(((20.0f * std::log10(fRec28[0])) - fSlow101), 0.0f)));
			float fTemp123 = (fConst9 * (fTemp121 * std::pow(10.0f, (0.0500000007f * fRec27[0]))));
			fRec30[0] = (fTemp123 - (fSlow105 * ((fSlow106 * fRec30[2]) + (fSlow107 * fRec30[1]))));
			float fTemp124 = (fSlow82 * fRec11[1]);
			fRec11[0] = (fTemp123 - ((fSlow105 * (fRec30[2] + (fRec30[0] + (2.0f * fRec30[1])))) + (((fRec11[2] * fSlow115) + fTemp124) / fSlow116)));
			float fTemp125 = (fSlow79 * fRec10[1]);
			fRec10[0] = ((((fTemp124 + (fRec11[0] * fSlow118)) + (fRec11[2] * fSlow119)) / fSlow116) - (((fRec10[2] * fSlow127) + fTemp125) / fSlow128));
			fRec9[0] = ((((fTemp125 + (fRec10[0] * fSlow130)) + (fRec10[2] * fSlow131)) / fSlow128) - (fSlow132 * ((fSlow133 * fRec9[2]) + (fSlow134 * fRec9[1]))));
			float fTemp126 = ((fSlow7 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + (fSlow76 * (fRec9[2] + (fRec9[0] + (2.0f * fRec9[1])))));
			fVec6[0] = fSlow146;
			fVec7[0] = fSlow148;
			int iTemp127 = ((fSlow146 > fVec6[1]) + (iSlow147 * (fSlow148 > fVec7[1])));
			iVec8[0] = iTemp127;
			iRec35[0] = (((iRec35[1] + (iRec35[1] > 0)) * (iTemp127 <= iVec8[1])) + (iTemp127 > iVec8[1]));
			float fTemp128 = float(iRec35[0]);
			float fTemp129 = (fConst20 * fTemp128);
			float fTemp130 = (fConst19 - fTemp128);
			fRec34[0] = ((fConst22 * fRec34[1]) + (fConst23 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp129, ((fConst21 * fTemp130) + 1.0f))))));
			fRec36[0] = (iTemp127 ? fSlow28 : fRec36[1]);
			float fTemp131 = std::pow(10.0f, (0.144444451f * (fRec36[0] + -10.0f)));
			float fTemp132 = (fRec34[0] * fTemp131);
			fRec39[0] = (iTemp127 ? fSlow150 : fRec39[1]);
			iRec38[0] = ((iVec0[1] + iRec38[1]) % int((fConst0 / fRec39[0])));
			float fTemp133 = float(iRec38[0]);
			int iTemp134 = (iTemp127 > 0);
			float fTemp135 = (0.144717798f * (iTemp134 ? 0.00100000005f : 1.60000002f));
			int iTemp136 = (std::fabs(fTemp135) < 1.1920929e-07f);
			float fTemp137 = (iTemp136 ? 0.0f : std::exp((0.0f - (fConst4 / (iTemp136 ? 1.0f : fTemp135)))));
			fRec41[0] = ((fRec41[1] * fTemp137) + ((iTemp134 ? float(iTemp127) : 0.400000006f) * (1.0f - fTemp137)));
			fRec40[0] = ((fConst22 * fRec40[1]) + (fSlow151 * fRec41[0]));
			float fTemp138 = (fRec40[0] + 0.00999999978f);
			float fTemp139 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec39[0] * fTemp138) * (1.0f - (0.000500000024f * std::min<float>(fRec39[0], 1000.0f)))))))));
			float fTemp140 = (1.0f / fTemp139);
			float fTemp141 = (((fTemp140 + 1.41421354f) / fTemp139) + 1.0f);
			fRec37[0] = ((2.0f * (((float((fTemp133 < (fConst24 / fRec39[0]))) + -0.5f) + (float((fTemp133 < (fConst25 / fRec39[0]))) + -0.5f)) + (float((fTemp133 < (fConst26 / fRec39[0]))) + -0.5f))) - (((fRec37[2] * (((fTemp140 + -1.41421354f) / fTemp139) + 1.0f)) + (2.0f * (fRec37[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp139)))))) / fTemp141));
			fRec44[0] = (iTemp127 ? fSlow152 : fRec44[1]);
			iRec43[0] = ((iVec0[1] + iRec43[1]) % int((fConst0 / fRec44[0])));
			float fTemp142 = float(iRec43[0]);
			float fTemp143 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fRec44[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec44[0], 1000.0f)))))))));
			float fTemp144 = (1.0f / fTemp143);
			float fTemp145 = (((fTemp144 + 1.41421354f) / fTemp143) + 1.0f);
			fRec42[0] = ((2.0f * (((float((fTemp142 < (fConst24 / fRec44[0]))) + -0.5f) + (float((fTemp142 < (fConst25 / fRec44[0]))) + -0.5f)) + (float((fTemp142 < (fConst26 / fRec44[0]))) + -0.5f))) - (((fRec42[2] * (((fTemp144 + -1.41421354f) / fTemp143) + 1.0f)) + (2.0f * (fRec42[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp143)))))) / fTemp145));
			fRec47[0] = (iTemp127 ? fSlow153 : fRec47[1]);
			iRec46[0] = ((iVec0[1] + iRec46[1]) % int((fConst0 / fRec47[0])));
			float fTemp146 = float(iRec46[0]);
			float fTemp147 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fRec47[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec47[0], 1000.0f)))))))));
			float fTemp148 = (1.0f / fTemp147);
			float fTemp149 = (((fTemp148 + 1.41421354f) / fTemp147) + 1.0f);
			fRec45[0] = ((2.0f * (((float((fTemp146 < (fConst24 / fRec47[0]))) + -0.5f) + (float((fTemp146 < (fConst25 / fRec47[0]))) + -0.5f)) + (float((fTemp146 < (fConst26 / fRec47[0]))) + -0.5f))) - (((fRec45[2] * (((fTemp148 + -1.41421354f) / fTemp147) + 1.0f)) + (2.0f * (fRec45[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp147)))))) / fTemp149));
			fRec50[0] = (iTemp127 ? fSlow154 : fRec50[1]);
			iRec49[0] = ((iVec0[1] + iRec49[1]) % int((fConst0 / fRec50[0])));
			float fTemp150 = float(iRec49[0]);
			float fTemp151 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fRec50[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec50[0], 1000.0f)))))))));
			float fTemp152 = (1.0f / fTemp151);
			float fTemp153 = (((fTemp152 + 1.41421354f) / fTemp151) + 1.0f);
			fRec48[0] = ((2.0f * (((float((fTemp150 < (fConst24 / fRec50[0]))) + -0.5f) + (float((fTemp150 < (fConst25 / fRec50[0]))) + -0.5f)) + (float((fTemp150 < (fConst26 / fRec50[0]))) + -0.5f))) - (((fRec48[2] * (((fTemp152 + -1.41421354f) / fTemp151) + 1.0f)) + (2.0f * (fRec48[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp151)))))) / fTemp153));
			float fTemp154 = (fTemp132 * (((((fRec37[2] + (fRec37[0] + (2.0f * fRec37[1]))) / fTemp141) + ((fRec42[2] + (fRec42[0] + (2.0f * fRec42[1]))) / fTemp145)) + ((fRec45[2] + (fRec45[0] + (2.0f * fRec45[1]))) / fTemp149)) + ((fRec48[2] + (fRec48[0] + (2.0f * fRec48[1]))) / fTemp153)));
			float fTemp155 = std::fabs((0.25f * fTemp154));
			fRec53[0] = std::max<float>(fTemp155, ((fRec53[1] * fSlow161) + (fTemp155 * fSlow162)));
			fRec52[0] = ((fRec52[1] * fSlow158) + (fRec53[0] * fSlow163));
			fRec51[0] = ((fConst2 * fRec51[1]) + (fSlow155 * std::max<float>(((20.0f * std::log10(fRec52[0])) - fSlow164), 0.0f)));
			float fTemp156 = (0.25f * (fTemp154 * std::pow(10.0f, (0.0500000007f * fRec51[0]))));
			fRec54[0] = (fTemp156 - (fSlow168 * ((fSlow169 * fRec54[2]) + (fSlow170 * fRec54[1]))));
			float fTemp157 = (fSlow145 * fRec33[1]);
			fRec33[0] = (fTemp156 - ((fSlow168 * (fRec54[2] + (fRec54[0] + (2.0f * fRec54[1])))) + (((fRec33[2] * fSlow178) + fTemp157) / fSlow179)));
			float fTemp158 = (fSlow142 * fRec32[1]);
			fRec32[0] = ((((fTemp157 + (fRec33[0] * fSlow181)) + (fRec33[2] * fSlow182)) / fSlow179) - (((fRec32[2] * fSlow190) + fTemp158) / fSlow191));
			fRec31[0] = ((((fTemp158 + (fRec32[0] * fSlow193)) + (fRec32[2] * fSlow194)) / fSlow191) - (fSlow195 * ((fSlow196 * fRec31[2]) + (fSlow197 * fRec31[1]))));
			fVec9[0] = fSlow211;
			fVec10[0] = fSlow213;
			int iTemp159 = ((fSlow211 > fVec9[1]) + (iSlow212 * (fSlow213 > fVec10[1])));
			iRec58[0] = ((iTemp159 > 0) ? 0 : std::min<int>(96000, (iRec58[1] + 1)));
			fRec59[0] = (iTemp159 ? fSlow28 : fRec59[1]);
			float fTemp160 = ((iSlow210 ? fSoundfile1ca_bu_ch1[(fSoundfile1ca_of1[1] + std::max<int>(0, std::min<int>(iRec58[0], iSlow215)))] : fSoundfile1ca_bu_ch0[(fSoundfile1ca_of0[0] + std::max<int>(0, std::min<int>(iRec58[0], iSlow214)))]) * std::pow(10.0f, (0.144444451f * (fRec59[0] + -10.0f))));
			float fTemp161 = std::fabs((0.25f * fTemp160));
			fRec62[0] = std::max<float>(fTemp161, ((fRec62[1] * fSlow222) + (fTemp161 * fSlow223)));
			fRec61[0] = ((fRec61[1] * fSlow219) + (fRec62[0] * fSlow224));
			fRec60[0] = ((fConst2 * fRec60[1]) + (fSlow216 * std::max<float>(((20.0f * std::log10(fRec61[0])) - fSlow225), 0.0f)));
			float fTemp162 = (0.25f * (fTemp160 * std::pow(10.0f, (0.0500000007f * fRec60[0]))));
			fRec63[0] = (fTemp162 - (fSlow229 * ((fSlow230 * fRec63[2]) + (fSlow231 * fRec63[1]))));
			float fTemp163 = (fSlow209 * fRec57[1]);
			fRec57[0] = (fTemp162 - ((fSlow229 * (fRec63[2] + (fRec63[0] + (2.0f * fRec63[1])))) + (((fRec57[2] * fSlow239) + fTemp163) / fSlow240)));
			float fTemp164 = (fSlow206 * fRec56[1]);
			fRec56[0] = ((((fTemp163 + (fRec57[0] * fSlow242)) + (fRec57[2] * fSlow243)) / fSlow240) - (((fRec56[2] * fSlow251) + fTemp164) / fSlow252));
			fRec55[0] = ((((fTemp164 + (fRec56[0] * fSlow254)) + (fRec56[2] * fSlow255)) / fSlow252) - (fSlow256 * ((fSlow257 * fRec55[2]) + (fSlow258 * fRec55[1]))));
			float fTemp165 = (fRec55[2] + (fRec55[0] + (2.0f * fRec55[1])));
			fRec67[0] = ((fConst11 * fRec67[1]) + (fConst12 * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp129, ((fConst27 * fTemp130) + 1.0f))))));
			fRec69[0] = (iTemp127 ? fSlow270 : fRec69[1]);
			float fTemp166 = (fRec73[1] + (fSlow271 * fRec69[0]));
			fRec73[0] = (fTemp166 - std::floor(fTemp166));
			float fTemp167 = (fRec72[1] + (fConst4 * ((315.0f * (fRec67[0] * ftbl0mydspSIG0[int((65536.0f * fRec73[0]))])) + (fSlow272 * fRec69[0]))));
			fRec72[0] = (fTemp167 - std::floor(fTemp167));
			float fTemp168 = (fRec71[1] + (fConst4 * ((315.0f * (fRec67[0] * ftbl0mydspSIG0[int((65536.0f * fRec72[0]))])) + (fSlow273 * fRec69[0]))));
			fRec71[0] = (fTemp168 - std::floor(fTemp168));
			float fTemp169 = (fRec70[1] + (fConst4 * ((315.0f * (fRec67[0] * ftbl0mydspSIG0[int((65536.0f * fRec71[0]))])) + (fSlow274 * fRec69[0]))));
			fRec70[0] = (fTemp169 - std::floor(fTemp169));
			float fTemp170 = (fRec68[1] + (fConst4 * ((fSlow83 * fRec69[0]) + (315.0f * (fRec67[0] * ftbl0mydspSIG0[int((65536.0f * fRec70[0]))])))));
			fRec68[0] = (fTemp170 - std::floor(fTemp170));
			float fTemp171 = ftbl0mydspSIG0[int((65536.0f * fRec68[0]))];
			float fTemp172 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp171)));
			float fTemp173 = ((fRec67[0] * fTemp131) * (fTemp171 + (0.0500000007f * (fTemp172 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp172)))))));
			float fTemp174 = std::fabs((0.25f * fTemp173));
			fRec76[0] = std::max<float>(fTemp174, ((fRec76[1] * fSlow281) + (fTemp174 * fSlow282)));
			fRec75[0] = ((fRec75[1] * fSlow278) + (fRec76[0] * fSlow283));
			fRec74[0] = ((fConst2 * fRec74[1]) + (fSlow275 * std::max<float>(((20.0f * std::log10(fRec75[0])) - fSlow284), 0.0f)));
			float fTemp175 = (0.25f * (fTemp173 * std::pow(10.0f, (0.0500000007f * fRec74[0]))));
			fRec77[0] = (fTemp175 - (fSlow288 * ((fSlow289 * fRec77[2]) + (fSlow290 * fRec77[1]))));
			float fTemp176 = (fSlow269 * fRec66[1]);
			fRec66[0] = (fTemp175 - ((fSlow288 * (fRec77[2] + (fRec77[0] + (2.0f * fRec77[1])))) + (((fRec66[2] * fSlow298) + fTemp176) / fSlow299)));
			float fTemp177 = (fSlow266 * fRec65[1]);
			fRec65[0] = ((((fTemp176 + (fRec66[0] * fSlow301)) + (fRec66[2] * fSlow302)) / fSlow299) - (((fRec65[2] * fSlow310) + fTemp177) / fSlow311));
			fRec64[0] = ((((fTemp177 + (fRec65[0] * fSlow313)) + (fRec65[2] * fSlow314)) / fSlow311) - (fSlow315 * ((fSlow316 * fRec64[2]) + (fSlow317 * fRec64[1]))));
			float fTemp178 = (fSlow263 * (fRec64[2] + (fRec64[0] + (2.0f * fRec64[1]))));
			float fTemp179 = (((fTemp126 + (fSlow139 * (fRec31[2] + (fRec31[0] + (2.0f * fRec31[1]))))) + (fSlow203 * fTemp165)) + fTemp178);
			float fTemp180 = std::fabs(fTemp179);
			fRec80[0] = std::max<float>(fTemp180, ((fConst31 * fRec80[1]) + (fConst32 * fTemp180)));
			fRec79[0] = ((fConst29 * fRec79[1]) + (fConst30 * fRec80[0]));
			fRec78[0] = ((fConst2 * fRec78[1]) + (fConst3 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec79[0]) + 1.0f)), 0.0f)))));
			float fTemp181 = (fTemp179 * std::pow(10.0f, (0.0500000007f * fRec78[0])));
			float fTemp182 = std::fabs(fTemp181);
			fRec83[0] = std::max<float>(fTemp182, ((fConst35 * fRec83[1]) + (fConst36 * fTemp182)));
			fRec82[0] = ((fConst33 * fRec82[1]) + (fConst34 * fRec83[0]));
			fRec81[0] = ((fConst2 * fRec81[1]) + (fConst3 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec82[0])) + 5.0f), 0.0f)))));
			float fTemp183 = (fTemp181 * std::pow(10.0f, (0.0500000007f * fRec81[0])));
			float fTemp184 = std::fabs((fSlow2 * fTemp183));
			fRec86[0] = std::max<float>(fTemp184, ((fConst2 * fRec86[1]) + (fConst3 * fTemp184)));
			fRec85[0] = ((fConst11 * fRec85[1]) + (fConst12 * fRec86[0]));
			fRec84[0] = ((fConst2 * fRec84[1]) + (fConst3 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec85[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT((fSlow1 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp183 * std::pow(10.0f, (0.0500000007f * fRec84[0]))))))));
			float fTemp185 = (fRec39[0] + 0.5f);
			iRec91[0] = ((iVec0[1] + iRec91[1]) % int((fConst0 / fTemp185)));
			float fTemp186 = float(iRec91[0]);
			float fTemp187 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fTemp185) * (1.0f - (0.000500000024f * std::min<float>(fTemp185, 1000.0f)))))))));
			float fTemp188 = (1.0f / fTemp187);
			float fTemp189 = (((fTemp188 + 1.41421354f) / fTemp187) + 1.0f);
			fRec90[0] = ((2.0f * (((float((fTemp186 < (fConst24 / fTemp185))) + -0.5f) + (float((fTemp186 < (fConst25 / fTemp185))) + -0.5f)) + (float((fTemp186 < (fConst26 / fTemp185))) + -0.5f))) - (((fRec90[2] * (((fTemp188 + -1.41421354f) / fTemp187) + 1.0f)) + (2.0f * (fRec90[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp187)))))) / fTemp189));
			float fTemp190 = (fRec44[0] + 0.5f);
			iRec93[0] = ((iVec0[1] + iRec93[1]) % int((fConst0 / fTemp190)));
			float fTemp191 = float(iRec93[0]);
			float fTemp192 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fTemp190) * (1.0f - (0.000500000024f * std::min<float>(fTemp190, 1000.0f)))))))));
			float fTemp193 = (1.0f / fTemp192);
			float fTemp194 = (((fTemp193 + 1.41421354f) / fTemp192) + 1.0f);
			fRec92[0] = ((2.0f * (((float((fTemp191 < (fConst24 / fTemp190))) + -0.5f) + (float((fTemp191 < (fConst25 / fTemp190))) + -0.5f)) + (float((fTemp191 < (fConst26 / fTemp190))) + -0.5f))) - (((fRec92[2] * (((fTemp193 + -1.41421354f) / fTemp192) + 1.0f)) + (2.0f * (fRec92[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp192)))))) / fTemp194));
			float fTemp195 = (fRec47[0] + 0.5f);
			iRec95[0] = ((iVec0[1] + iRec95[1]) % int((fConst0 / fTemp195)));
			float fTemp196 = float(iRec95[0]);
			float fTemp197 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fTemp195) * (1.0f - (0.000500000024f * std::min<float>(fTemp195, 1000.0f)))))))));
			float fTemp198 = (1.0f / fTemp197);
			float fTemp199 = (((fTemp198 + 1.41421354f) / fTemp197) + 1.0f);
			fRec94[0] = ((2.0f * (((float((fTemp196 < (fConst24 / fTemp195))) + -0.5f) + (float((fTemp196 < (fConst25 / fTemp195))) + -0.5f)) + (float((fTemp196 < (fConst26 / fTemp195))) + -0.5f))) - (((fRec94[2] * (((fTemp198 + -1.41421354f) / fTemp197) + 1.0f)) + (2.0f * (fRec94[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp197)))))) / fTemp199));
			float fTemp200 = (fRec50[0] + 0.5f);
			iRec97[0] = ((iVec0[1] + iRec97[1]) % int((fConst0 / fTemp200)));
			float fTemp201 = float(iRec97[0]);
			float fTemp202 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp138 * fTemp200) * (1.0f - (0.000500000024f * std::min<float>(fTemp200, 1000.0f)))))))));
			float fTemp203 = (1.0f / fTemp202);
			float fTemp204 = (((fTemp203 + 1.41421354f) / fTemp202) + 1.0f);
			fRec96[0] = ((2.0f * (((float((fTemp201 < (fConst24 / fTemp200))) + -0.5f) + (float((fTemp201 < (fConst25 / fTemp200))) + -0.5f)) + (float((fTemp201 < (fConst26 / fTemp200))) + -0.5f))) - (((fRec96[2] * (((fTemp203 + -1.41421354f) / fTemp202) + 1.0f)) + (2.0f * (fRec96[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp202)))))) / fTemp204));
			float fTemp205 = (fTemp132 * (((((fRec90[2] + (fRec90[0] + (2.0f * fRec90[1]))) / fTemp189) + ((fRec92[2] + (fRec92[0] + (2.0f * fRec92[1]))) / fTemp194)) + ((fRec94[2] + (fRec94[0] + (2.0f * fRec94[1]))) / fTemp199)) + ((fRec96[2] + (fRec96[0] + (2.0f * fRec96[1]))) / fTemp204)));
			float fTemp206 = std::fabs((0.25f * fTemp205));
			fRec100[0] = std::max<float>(fTemp206, ((fSlow161 * fRec100[1]) + (fSlow162 * fTemp206)));
			fRec99[0] = ((fSlow158 * fRec99[1]) + (fRec100[0] * fSlow163));
			fRec98[0] = ((fConst2 * fRec98[1]) + (fSlow155 * std::max<float>(((20.0f * std::log10(fRec99[0])) - fSlow164), 0.0f)));
			float fTemp207 = (0.25f * (fTemp205 * std::pow(10.0f, (0.0500000007f * fRec98[0]))));
			fRec101[0] = (fTemp207 - (fSlow168 * ((fSlow169 * fRec101[2]) + (fSlow170 * fRec101[1]))));
			float fTemp208 = (fSlow145 * fRec89[1]);
			fRec89[0] = (fTemp207 - ((fSlow168 * (fRec101[2] + (fRec101[0] + (2.0f * fRec101[1])))) + (((fSlow178 * fRec89[2]) + fTemp208) / fSlow179)));
			float fTemp209 = (fSlow142 * fRec88[1]);
			fRec88[0] = ((((fTemp208 + (fRec89[0] * fSlow181)) + (fSlow182 * fRec89[2])) / fSlow179) - (((fSlow190 * fRec88[2]) + fTemp209) / fSlow191));
			fRec87[0] = ((((fTemp209 + (fRec88[0] * fSlow193)) + (fSlow194 * fRec88[2])) / fSlow191) - (fSlow195 * ((fSlow196 * fRec87[2]) + (fSlow197 * fRec87[1]))));
			float fTemp210 = ((fSlow139 * (fRec87[2] + (fRec87[0] + (2.0f * fRec87[1])))) + (fTemp178 + (fTemp126 + (fSlow319 * fTemp165))));
			float fTemp211 = std::fabs(fTemp210);
			fRec104[0] = std::max<float>(fTemp211, ((fConst31 * fRec104[1]) + (fConst32 * fTemp211)));
			fRec103[0] = ((fConst29 * fRec103[1]) + (fConst30 * fRec104[0]));
			fRec102[0] = ((fConst2 * fRec102[1]) + (fConst3 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec103[0]) + 1.0f)), 0.0f)))));
			float fTemp212 = (fTemp210 * std::pow(10.0f, (0.0500000007f * fRec102[0])));
			float fTemp213 = std::fabs(fTemp212);
			fRec107[0] = std::max<float>(fTemp213, ((fConst35 * fRec107[1]) + (fConst36 * fTemp213)));
			fRec106[0] = ((fConst33 * fRec106[1]) + (fConst34 * fRec107[0]));
			fRec105[0] = ((fConst2 * fRec105[1]) + (fConst3 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec106[0])) + 5.0f), 0.0f)))));
			float fTemp214 = (fTemp212 * std::pow(10.0f, (0.0500000007f * fRec105[0])));
			float fTemp215 = std::fabs((fSlow2 * fTemp214));
			fRec110[0] = std::max<float>(fTemp215, ((fConst2 * fRec110[1]) + (fConst3 * fTemp215)));
			fRec109[0] = ((fConst11 * fRec109[1]) + (fConst12 * fRec110[0]));
			fRec108[0] = ((fConst2 * fRec108[1]) + (fConst3 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec109[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT((fSlow318 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp214 * std::pow(10.0f, (0.0500000007f * fRec108[0]))))))));
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			iRec3[1] = iRec3[0];
			fRec4[1] = fRec4[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fRec5[1] = fRec5[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			iVec5[1] = iVec5[0];
			fRec13[1] = fRec13[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec18[1] = fRec18[0];
			fRec20[1] = fRec20[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			iRec25[1] = iRec25[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec26[1] = fRec26[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec6[1] = fVec6[0];
			fVec7[1] = fVec7[0];
			iVec8[1] = iVec8[0];
			iRec35[1] = iRec35[0];
			fRec34[1] = fRec34[0];
			fRec36[1] = fRec36[0];
			fRec39[1] = fRec39[0];
			iRec38[1] = iRec38[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec44[1] = fRec44[0];
			iRec43[1] = iRec43[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec47[1] = fRec47[0];
			iRec46[1] = iRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec50[1] = fRec50[0];
			iRec49[1] = iRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fVec9[1] = fVec9[0];
			fVec10[1] = fVec10[0];
			iRec58[1] = iRec58[0];
			fRec59[1] = fRec59[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec67[1] = fRec67[0];
			fRec69[1] = fRec69[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec81[1] = fRec81[0];
			fRec86[1] = fRec86[0];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			iRec91[1] = iRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			iRec93[1] = iRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			iRec95[1] = iRec95[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			iRec97[1] = iRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec98[1] = fRec98[0];
			fRec101[2] = fRec101[1];
			fRec101[1] = fRec101[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			fRec102[1] = fRec102[0];
			fRec107[1] = fRec107[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fRec110[1] = fRec110[0];
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
		}
	}

};

//**************************************************************
// Polyphony
//**************************************************************

/************************** BEGIN faust-poly-engine.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2013 GRAME, Romain Michon, CCRMA - Stanford University
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __faust_poly_engine__
#define __faust_poly_engine__

#include <math.h>
#include <stdio.h>
#include <string.h>

/************************** BEGIN audio.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __audio__
#define __audio__

#include <set>
#include <utility>

class dsp;

typedef void (* shutdown_callback)(const char* message, void* arg);

typedef void (* compute_callback)(void* arg);

class audio {
    
    protected:
    
        shutdown_callback fShutdown;    // Shutdown callback
        void* fShutdownArg;             // Shutdown callback data
    
        std::set<std::pair<compute_callback, void*> > fComputeCallbackList;
    
    public:
    
        audio():fShutdown(nullptr), fShutdownArg(nullptr) {}
        virtual ~audio() {}

        virtual bool init(const char* name, dsp* dsp) = 0;
    
        virtual bool start() = 0;
        virtual void stop() = 0;
    
        void setShutdownCallback(shutdown_callback cb, void* arg)
        {
            fShutdown = cb;
            fShutdownArg = arg;
        }
    
        void addControlCallback(compute_callback cb, void* arg)
        {
            fComputeCallbackList.insert(std::make_pair(cb, arg));
        }
        bool removeControlCallback(compute_callback cb, void* arg)
        {
            return (fComputeCallbackList.erase(std::make_pair(cb, arg)) == 1);
        }
        void runControlCallbacks()
        {
            for (auto& it : fComputeCallbackList) {
                it.first(it.second);
            }
        }
    
        virtual int getBufferSize() = 0;
        virtual int getSampleRate() = 0;

        virtual int getNumInputs() = 0;
        virtual int getNumOutputs() = 0;
    
        // Returns the average proportion of available CPU being spent inside the audio callbacks (between 0 and 1.0).
        virtual float getCPULoad() { return 0.f; }
};
					
#endif
/**************************  END  audio.h **************************/
/************************** BEGIN JSONUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef FAUST_JSONUI_H
#define FAUST_JSONUI_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * JSONUI : Faust User Interface
 * This class produce a complete JSON decription of the DSP instance.
 ******************************************************************************/

template <typename REAL>
class JSONUIAux : public PathBuilder, public Meta, public UIReal<REAL>
{

    protected:
    
        std::stringstream fUI;
        std::stringstream fMeta;
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        std::string fVersion;           // Compiler version
        std::string fCompileOptions;    // Compilation options
        std::vector<std::string> fLibraryList;
        std::vector<std::string> fIncludePathnames;
        std::string fName;
        std::string fFileName;
        std::string fExpandedCode;
        std::string fSHAKey;
        int fDSPSize;                   // In bytes
        std::map<std::string, int> fPathTable;
        bool fExtended;
    
        char fCloseUIPar;
        char fCloseMetaPar;
        int fTab;
    
        int fInputs, fOutputs, fSRIndex;
         
        void tab(int n, std::ostream& fout)
        {
            fout << '\n';
            while (n-- > 0) {
                fout << '\t';
            }
        }
    
        std::string flatten(const std::string& src)
        {
            std::string dst;
            for (size_t i = 0; i < src.size(); i++) {
                switch (src[i]) {
                    case '\n':
                    case '\t':
                        break;
                    default:
                        dst += src[i];
                        break;
                }
            }
            return dst;
        }
    
        void addMeta(int tab_val, bool quote = true)
        {
            if (fMetaAux.size() > 0) {
                tab(tab_val, fUI); fUI << "\"meta\": [";
                std::string sep = "";
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    fUI << sep;
                    tab(tab_val + 1, fUI); fUI << "{ \"" << fMetaAux[i].first << "\": \"" << fMetaAux[i].second << "\" }";
                    sep = ",";
                }
                tab(tab_val, fUI); fUI << ((quote) ? "],": "]");
                fMetaAux.clear();
            }
        }
    
        int getAddressIndex(const std::string& path)
        {
            return (fPathTable.find(path) != fPathTable.end()) ? fPathTable[path] : -1;
        }
      
     public:
     
        JSONUIAux(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const std::map<std::string, int>& path_table)
        {
            init(name, filename, inputs, outputs, sr_index, sha_key, dsp_code, version, compile_options, library_list, include_pathnames, size, path_table);
        }

        JSONUIAux(const std::string& name, const std::string& filename, int inputs, int outputs)
        {
            init(name, filename, inputs, outputs, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }

        JSONUIAux(int inputs, int outputs)
        {
            init("", "", inputs, outputs, -1, "", "","", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }
        
        JSONUIAux()
        {
            init("", "", -1, -1, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }
 
        virtual ~JSONUIAux() {}
        
        void setInputs(int inputs) { fInputs = inputs; }
        void setOutputs(int outputs) { fOutputs = outputs; }
    
        void setSRIndex(int sr_index) { fSRIndex = sr_index; }
    
        // Init may be called multiple times so fMeta and fUI are reinitialized
        void init(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const std::map<std::string, int>& path_table,
                  bool extended = false)
        {
            fTab = 1;
            fExtended = extended;
            if (fExtended) {
                fUI << std::setprecision(std::numeric_limits<REAL>::max_digits10);
                fMeta << std::setprecision(std::numeric_limits<REAL>::max_digits10);
            }
            
            // Start Meta generation
            fMeta.str("");
            tab(fTab, fMeta); fMeta << "\"meta\": [";
            fCloseMetaPar = ' ';
            
            // Start UI generation
            fUI.str("");
            tab(fTab, fUI); fUI << "\"ui\": [";
            fCloseUIPar = ' ';
            fTab += 1;
            
            fName = name;
            fFileName = filename;
            fInputs = inputs;
            fOutputs = outputs;
            fSRIndex = sr_index;
            fExpandedCode = dsp_code;
            fSHAKey = sha_key;
            fDSPSize = size;
            fPathTable = path_table;
            fVersion = version;
            fCompileOptions = compile_options;
            fLibraryList = library_list;
            fIncludePathnames = include_pathnames;
        }
   
        // -- widget's layouts
    
        virtual void openGenericGroup(const char* label, const char* name)
        {
            pushLabel(label);
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"items\": [";
            fCloseUIPar = ' ';
            fTab += 1;
        }

        virtual void openTabBox(const char* label)
        {
            openGenericGroup(label, "tgroup");
        }
    
        virtual void openHorizontalBox(const char* label)
        {
            openGenericGroup(label, "hgroup");
        }
    
        virtual void openVerticalBox(const char* label)
        {
            openGenericGroup(label, "vgroup");
        }
    
        virtual void closeBox()
        {
            popLabel();
            fTab -= 1;
            tab(fTab, fUI); fUI << "]";
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        // -- active widgets
  
        virtual void addGenericButton(const char* label, const char* name)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ((fMetaAux.size() > 0) ? "," : "");
            } else {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fMetaAux.size() > 0) ? "," : "");
            }
            addMeta(fTab, false);
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "button");
        }
    
        virtual void addCheckButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "checkbox");
        }

        virtual void addGenericEntry(const char* label, const char* name, REAL init, REAL min, REAL max, REAL step)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"init\": " << init << ",";
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max << ",";
            tab(fTab, fUI); fUI << "\"step\": " << step;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "vslider", init, min, max, step);
        }
    
        virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "hslider", init, min, max, step);
        }
    
        virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "nentry", init, min, max, step);
        }

        // -- passive widgets
    
        virtual void addGenericBargraph(const char* label, const char* name, REAL min, REAL max) 
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) 
        {
            addGenericBargraph(label, "hbargraph", min, max);
        }
    
        virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max)
        {
            addGenericBargraph(label, "vbargraph", min, max);
        }
    
        virtual void addSoundfile(const char* label, const char* url, Soundfile** zone)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << "soundfile" << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\"" << ",";
            tab(fTab, fUI); fUI << "\"url\": \"" << url << "\"" << ",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fPathTable.size() > 0) ? "," : "");
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path);
            }
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        // -- metadata declarations

        virtual void declare(REAL* zone, const char* key, const char* val)
        {
            fMetaAux.push_back(std::make_pair(key, val));
        }
    
        // Meta interface
        virtual void declare(const char* key, const char* value)
        {
            fMeta << fCloseMetaPar;
            // fName found in metadata
            if ((strcmp(key, "name") == 0) && (fName == "")) fName = value;
            // fFileName found in metadata
            if ((strcmp(key, "filename") == 0) && (fFileName == "")) fFileName = value;
            tab(fTab, fMeta); fMeta << "{ " << "\"" << key << "\"" << ": " << "\"" << value << "\" }";
            fCloseMetaPar = ',';
        }
    
        std::string JSON(bool flat = false)
        {
            fTab = 0;
            std::stringstream JSON;
            if (fExtended) {
                JSON << std::setprecision(std::numeric_limits<REAL>::max_digits10);
            }
            JSON << "{";
            fTab += 1;
            tab(fTab, JSON); JSON << "\"name\": \"" << fName << "\",";
            tab(fTab, JSON); JSON << "\"filename\": \"" << fFileName << "\",";
            if (fVersion != "") { tab(fTab, JSON); JSON << "\"version\": \"" << fVersion << "\","; }
            if (fCompileOptions != "") { tab(fTab, JSON); JSON << "\"compile_options\": \"" <<  fCompileOptions << "\","; }
            if (fLibraryList.size() > 0) {
                tab(fTab, JSON);
                JSON << "\"library_list\": [";
                for (size_t i = 0; i < fLibraryList.size(); i++) {
                    JSON << "\"" << fLibraryList[i] << "\"";
                    if (i < (fLibraryList.size() - 1)) JSON << ",";
                }
                JSON << "],";
            }
            if (fIncludePathnames.size() > 0) {
                tab(fTab, JSON);
                JSON << "\"include_pathnames\": [";
                for (size_t i = 0; i < fIncludePathnames.size(); i++) {
                    JSON << "\"" << fIncludePathnames[i] << "\"";
                    if (i < (fIncludePathnames.size() - 1)) JSON << ",";
                }
                JSON << "],";
            }
            if (fDSPSize != -1) { tab(fTab, JSON); JSON << "\"size\": " << fDSPSize << ","; }
            if (fSHAKey != "") { tab(fTab, JSON); JSON << "\"sha_key\": \"" << fSHAKey << "\","; }
            if (fExpandedCode != "") { tab(fTab, JSON); JSON << "\"code\": \"" << fExpandedCode << "\","; }
            tab(fTab, JSON); JSON << "\"inputs\": " << fInputs << ",";
            tab(fTab, JSON); JSON << "\"outputs\": " << fOutputs << ",";
            if (fSRIndex != -1) { tab(fTab, JSON); JSON << "\"sr_index\": " << fSRIndex << ","; }
            tab(fTab, fMeta); fMeta << "],";
            tab(fTab, fUI); fUI << "]";
            fTab -= 1;
            if (fCloseMetaPar == ',') { // If "declare" has been called, fCloseMetaPar state is now ','
                JSON << fMeta.str() << fUI.str();
            } else {
                JSON << fUI.str();
            }
            tab(fTab, JSON); JSON << "}";
            return (flat) ? flatten(JSON.str()) : JSON.str();
        }
    
};

// Externally available class using FAUSTFLOAT

struct JSONUI : public JSONUIAux<FAUSTFLOAT>, public UI
{
    
    JSONUI(const std::string& name,
           const std::string& filename,
           int inputs,
           int outputs,
           int sr_index,
           const std::string& sha_key,
           const std::string& dsp_code,
           const std::string& version,
           const std::string& compile_options,
           const std::vector<std::string>& library_list,
           const std::vector<std::string>& include_pathnames,
           int size,
           const std::map<std::string, int>& path_table):
    JSONUIAux<FAUSTFLOAT>(name, filename,
                          inputs, outputs,
                          sr_index,
                          sha_key, dsp_code,
                          version, compile_options,
                          library_list, include_pathnames,
                          size, path_table)
    {}
    
    JSONUI(const std::string& name, const std::string& filename, int inputs, int outputs):
    JSONUIAux<FAUSTFLOAT>(name, filename, inputs, outputs)
    {}
    
    JSONUI(int inputs, int outputs):JSONUIAux<FAUSTFLOAT>(inputs, outputs)
    {}
    
    JSONUI():JSONUIAux<FAUSTFLOAT>()
    {}

    virtual void openTabBox(const char* label)
    {
        JSONUIAux<FAUSTFLOAT>::openTabBox(label);
    }
    virtual void openHorizontalBox(const char* label)
    {
        JSONUIAux<FAUSTFLOAT>::openHorizontalBox(label);
    }
    virtual void openVerticalBox(const char* label)
    {
        JSONUIAux<FAUSTFLOAT>::openVerticalBox(label);
    }
    virtual void closeBox()
    {
        JSONUIAux<FAUSTFLOAT>::closeBox();
    }
    
    // -- active widgets
    
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIAux<FAUSTFLOAT>::addButton(label, zone);
    }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIAux<FAUSTFLOAT>::addCheckButton(label, zone);
    }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIAux<FAUSTFLOAT>::addVerticalSlider(label, zone, init, min, max, step);
    }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIAux<FAUSTFLOAT>::addHorizontalSlider(label, zone, init, min, max, step);
    }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIAux<FAUSTFLOAT>::addNumEntry(label, zone, init, min, max, step);
    }
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIAux<FAUSTFLOAT>::addHorizontalBargraph(label, zone, min, max);
    }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIAux<FAUSTFLOAT>::addVerticalBargraph(label, zone, min, max);
    }
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone)
    {
        JSONUIAux<FAUSTFLOAT>::addSoundfile(label, filename, sf_zone);
    }
    
    // -- metadata declarations
    
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
    {
        JSONUIAux<FAUSTFLOAT>::declare(zone, key, val);
    }

    virtual void declare(const char* key, const char* val)
    {
        JSONUIAux<FAUSTFLOAT>::declare(key, val);
    }

    virtual ~JSONUI() {}
    
};

#endif // FAUST_JSONUI_H
/**************************  END  JSONUI.h **************************/
/************************** BEGIN APIUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef API_UI_H
#define API_UI_H

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>

/************************** BEGIN ValueConverter.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __ValueConverter__
#define __ValueConverter__

/***************************************************************************************
								ValueConverter.h
                            (GRAME, Copyright 2015-2019)

Set of conversion objects used to map user interface values (for example a gui slider
delivering values between 0 and 1) to faust values (for example a vslider between
20 and 20000) using a log scale.

-- Utilities

Range(lo,hi) : clip a value x between lo and hi
Interpolator(lo,hi,v1,v2) : Maps a value x between lo and hi to a value y between v1 and v2
Interpolator3pt(lo,mi,hi,v1,vm,v2) : Map values between lo mid hi to values between v1 vm v2

-- Value Converters

ValueConverter::ui2faust(x)
ValueConverter::faust2ui(x)

-- ValueConverters used for sliders depending of the scale

LinearValueConverter(umin, umax, fmin, fmax)
LinearValueConverter2(lo, mi, hi, v1, vm, v2) using 2 segments
LogValueConverter(umin, umax, fmin, fmax)
ExpValueConverter(umin, umax, fmin, fmax)

-- ValueConverters used for accelerometers based on 3 points

AccUpConverter(amin, amid, amax, fmin, fmid, fmax)		-- curve 0
AccDownConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 1
AccUpDownConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 2
AccDownUpConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 3

-- lists of ZoneControl are used to implement accelerometers metadata for each axes

ZoneControl(zone, valueConverter) : a zone with an accelerometer data converter

-- ZoneReader are used to implement screencolor metadata

ZoneReader(zone, valueConverter) : a zone with a data converter

****************************************************************************************/

#include <float.h>
#include <algorithm>    // std::max
#include <cmath>
#include <vector>
#include <assert.h>

//--------------------------------------------------------------------------------------
// Interpolator(lo,hi,v1,v2)
// Maps a value x between lo and hi to a value y between v1 and v2
// y = v1 + (x-lo)/(hi-lo)*(v2-v1)
// y = v1 + (x-lo) * coef   		with coef = (v2-v1)/(hi-lo)
// y = v1 + x*coef - lo*coef
// y = v1 - lo*coef + x*coef
// y = offset + x*coef				with offset = v1 - lo*coef
//--------------------------------------------------------------------------------------
class Interpolator
{
    private:

        //--------------------------------------------------------------------------------------
        // Range(lo,hi) clip a value between lo and hi
        //--------------------------------------------------------------------------------------
        struct Range
        {
            double fLo;
            double fHi;

            Range(double x, double y) : fLo(std::min<double>(x,y)), fHi(std::max<double>(x,y)) {}
            double operator()(double x) { return (x<fLo) ? fLo : (x>fHi) ? fHi : x; }
        };


        Range fRange;
        double fCoef;
        double fOffset;

    public:

        Interpolator(double lo, double hi, double v1, double v2) : fRange(lo,hi)
        {
            if (hi != lo) {
                // regular case
                fCoef = (v2-v1)/(hi-lo);
                fOffset = v1 - lo*fCoef;
            } else {
                // degenerate case, avoids division by zero
                fCoef = 0;
                fOffset = (v1+v2)/2;
            }
        }
        double operator()(double v)
        {
            double x = fRange(v);
            return  fOffset + x*fCoef;
        }

        void getLowHigh(double& amin, double& amax)
        {
            amin = fRange.fLo;
            amax = fRange.fHi;
        }
};

//--------------------------------------------------------------------------------------
// Interpolator3pt(lo,mi,hi,v1,vm,v2)
// Map values between lo mid hi to values between v1 vm v2
//--------------------------------------------------------------------------------------
class Interpolator3pt
{

    private:

        Interpolator fSegment1;
        Interpolator fSegment2;
        double fMid;

    public:

        Interpolator3pt(double lo, double mi, double hi, double v1, double vm, double v2) :
            fSegment1(lo, mi, v1, vm),
            fSegment2(mi, hi, vm, v2),
            fMid(mi) {}
        double operator()(double x) { return  (x < fMid) ? fSegment1(x) : fSegment2(x); }

        void getMappingValues(double& amin, double& amid, double& amax)
        {
            fSegment1.getLowHigh(amin, amid);
            fSegment2.getLowHigh(amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Abstract ValueConverter class. Converts values between UI and Faust representations
//--------------------------------------------------------------------------------------
class ValueConverter
{

    public:

        virtual ~ValueConverter() {}
        virtual double ui2faust(double x) = 0;
        virtual double faust2ui(double x) = 0;
};

//--------------------------------------------------------------------------------------
// A converter than can be updated
//--------------------------------------------------------------------------------------

class UpdatableValueConverter : public ValueConverter {
    
    protected:
        
        bool fActive;
        
    public:
        
        UpdatableValueConverter():fActive(true)
        {}
        virtual ~UpdatableValueConverter()
        {}
        
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max) = 0;
        virtual void getMappingValues(double& amin, double& amid, double& amax) = 0;
        
        void setActive(bool on_off) { fActive = on_off; }
        bool getActive() { return fActive; }
    
};


//--------------------------------------------------------------------------------------
// Linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LinearValueConverter : public ValueConverter
{
    
    private:
        
        Interpolator fUI2F;
        Interpolator fF2UI;
        
    public:
        
        LinearValueConverter(double umin, double umax, double fmin, double fmax) :
            fUI2F(umin,umax,fmin,fmax), fF2UI(fmin,fmax,umin,umax)
        {}
        
        LinearValueConverter() : fUI2F(0.,0.,0.,0.), fF2UI(0.,0.,0.,0.)
        {}
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x) { return fF2UI(x); }
    
};

//--------------------------------------------------------------------------------------
// Two segments linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LinearValueConverter2 : public UpdatableValueConverter
{
    
    private:
    
        Interpolator3pt fUI2F;
        Interpolator3pt fF2UI;
        
    public:
    
        LinearValueConverter2(double amin, double amid, double amax, double min, double init, double max) :
            fUI2F(amin, amid, amax, min, init, max), fF2UI(min, init, max, amin, amid, amax)
        {}
        
        LinearValueConverter2() : fUI2F(0.,0.,0.,0.,0.,0.), fF2UI(0.,0.,0.,0.,0.,0.)
        {}
    
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x) { return fF2UI(x); }
    
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max)
        {
            fUI2F = Interpolator3pt(amin, amid, amax, min, init, max);
            fF2UI = Interpolator3pt(min, init, max, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fUI2F.getMappingValues(amin, amid, amax);
        }
    
};

//--------------------------------------------------------------------------------------
// Logarithmic conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LogValueConverter : public LinearValueConverter
{

    public:

        LogValueConverter(double umin, double umax, double fmin, double fmax) :
        LinearValueConverter(umin, umax, log(std::max<double>(DBL_MIN, fmin)), std::log(std::max<double>(DBL_MIN, fmax)))
        {}

        virtual double ui2faust(double x) { return std::exp(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::log(std::max<double>(x, DBL_MIN))); }

};

//--------------------------------------------------------------------------------------
// Exponential conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class ExpValueConverter : public LinearValueConverter
{

    public:

        ExpValueConverter(double umin, double umax, double fmin, double fmax) :
            LinearValueConverter(umin, umax, exp(fmin), exp(fmax))
        {}

        virtual double ui2faust(double x) { return std::log(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::exp(x)); }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up curve (curve 0)
//--------------------------------------------------------------------------------------
class AccUpConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt fA2F;
        Interpolator3pt fF2A;

    public:

        AccUpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmid,fmax),
            fF2A(fmin,fmid,fmax,amin,amid,amax)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmid, fmax);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down curve (curve 1)
//--------------------------------------------------------------------------------------
class AccDownConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator3pt	fF2A;

    public:

        AccDownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmid,fmin),
            fF2A(fmin,fmid,fmax,amax,amid,amin)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
             //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmid, fmin);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amax, amid, amin);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up-Down curve (curve 2)
//--------------------------------------------------------------------------------------
class AccUpDownConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        AccUpDownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmax,fmin),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpDownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmax, fmin);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down-Up curve (curve 3)
//--------------------------------------------------------------------------------------
class AccDownUpConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        AccDownUpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmin,fmax),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownUpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmin, fmax);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Base class for ZoneControl
//--------------------------------------------------------------------------------------
class ZoneControl
{

    protected:

        FAUSTFLOAT*	fZone;

    public:

        ZoneControl(FAUSTFLOAT* zone) : fZone(zone) {}
        virtual ~ZoneControl() {}

        virtual void update(double v) {}

        virtual void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max) {}
        virtual void getMappingValues(double& amin, double& amid, double& amax) {}

        FAUSTFLOAT* getZone() { return fZone; }

        virtual void setActive(bool on_off) {}
        virtual bool getActive() { return false; }

        virtual int getCurve() { return -1; }

};

//--------------------------------------------------------------------------------------
//  Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class ConverterZoneControl : public ZoneControl
{

    private:

        ValueConverter* fValueConverter;

    public:

        ConverterZoneControl(FAUSTFLOAT* zone, ValueConverter* valueConverter) : ZoneControl(zone), fValueConverter(valueConverter) {}
        virtual ~ConverterZoneControl() { delete fValueConverter; } // Assuming fValueConverter is not kept elsewhere...

        void update(double v) { *fZone = fValueConverter->ui2faust(v); }

        ValueConverter* getConverter() { return fValueConverter; }

};

//--------------------------------------------------------------------------------------
// Association of a zone and a four value converter, each one for each possible curve.
// Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class CurveZoneControl : public ZoneControl
{

    private:

        std::vector<UpdatableValueConverter*> fValueConverters;
        int fCurve;

    public:

        CurveZoneControl(FAUSTFLOAT* zone, int curve, double amin, double amid, double amax, double min, double init, double max) : ZoneControl(zone), fCurve(0)
        {
            assert(curve >= 0 && curve <= 3);
            fValueConverters.push_back(new AccUpConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccDownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccUpDownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccDownUpConverter(amin, amid, amax, min, init, max));
            fCurve = curve;
        }
        virtual ~CurveZoneControl()
        {
            std::vector<UpdatableValueConverter*>::iterator it;
            for (it = fValueConverters.begin(); it != fValueConverters.end(); it++) {
                delete(*it);
            }
        }
        void update(double v) { if (fValueConverters[fCurve]->getActive()) *fZone = fValueConverters[fCurve]->ui2faust(v); }

        void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max)
        {
            fValueConverters[curve]->setMappingValues(amin, amid, amax, min, init, max);
            fCurve = curve;
        }

        void getMappingValues(double& amin, double& amid, double& amax)
        {
            fValueConverters[fCurve]->getMappingValues(amin, amid, amax);
        }

        void setActive(bool on_off)
        {
            std::vector<UpdatableValueConverter*>::iterator it;
            for (it = fValueConverters.begin(); it != fValueConverters.end(); it++) {
                (*it)->setActive(on_off);
            }
        }

        int getCurve() { return fCurve; }
};

class ZoneReader
{

    private:

        FAUSTFLOAT* fZone;
        Interpolator fInterpolator;

    public:

        ZoneReader(FAUSTFLOAT* zone, double lo, double hi) : fZone(zone), fInterpolator(lo, hi, 0, 255) {}

        virtual ~ZoneReader() {}

        int getValue()
        {
            return (fZone != nullptr) ? int(fInterpolator(*fZone)) : 127;
        }

};

#endif
/**************************  END  ValueConverter.h **************************/

class APIUI : public PathBuilder, public Meta, public UI
{
    public:
    
        enum ItemType { kButton = 0, kCheckButton, kVSlider, kHSlider, kNumEntry, kHBargraph, kVBargraph };
  
    protected:
    
        enum { kLin = 0, kLog = 1, kExp = 2 };
    
        int fNumParameters;
        std::vector<std::string> fPaths;
        std::vector<std::string> fLabels;
        std::map<std::string, int> fPathMap;
        std::map<std::string, int> fLabelMap;
        std::vector<ValueConverter*> fConversion;
        std::vector<FAUSTFLOAT*> fZone;
        std::vector<FAUSTFLOAT> fInit;
        std::vector<FAUSTFLOAT> fMin;
        std::vector<FAUSTFLOAT> fMax;
        std::vector<FAUSTFLOAT> fStep;
        std::vector<ItemType> fItemType;
        std::vector<std::map<std::string, std::string> > fMetaData;
        std::vector<ZoneControl*> fAcc[3];
        std::vector<ZoneControl*> fGyr[3];

        // Screen color control
        // "...[screencolor:red]..." etc.
        bool fHasScreenControl;      // true if control screen color metadata
        ZoneReader* fRedReader;
        ZoneReader* fGreenReader;
        ZoneReader* fBlueReader;

        // Current values controlled by metadata
        std::string fCurrentUnit;
        int fCurrentScale;
        std::string fCurrentAcc;
        std::string fCurrentGyr;
        std::string fCurrentColor;
        std::string fCurrentTooltip;
        std::map<std::string, std::string> fCurrentMetadata;
    
        // Add a generic parameter
        virtual void addParameter(const char* label,
                                FAUSTFLOAT* zone,
                                FAUSTFLOAT init,
                                FAUSTFLOAT min,
                                FAUSTFLOAT max,
                                FAUSTFLOAT step,
                                ItemType type)
        {
            std::string path = buildPath(label);
            fPathMap[path] = fLabelMap[label] = fNumParameters++;
            fPaths.push_back(path);
            fLabels.push_back(label);
            fZone.push_back(zone);
            fInit.push_back(init);
            fMin.push_back(min);
            fMax.push_back(max);
            fStep.push_back(step);
            fItemType.push_back(type);
            
            // handle scale metadata
            switch (fCurrentScale) {
                case kLin:
                    fConversion.push_back(new LinearValueConverter(0, 1, min, max));
                    break;
                case kLog:
                    fConversion.push_back(new LogValueConverter(0, 1, min, max));
                    break;
                case kExp: fConversion.push_back(new ExpValueConverter(0, 1, min, max));
                    break;
            }
            fCurrentScale = kLin;
            
            if (fCurrentAcc.size() > 0 && fCurrentGyr.size() > 0) {
                std::cerr << "warning : 'acc' and 'gyr' metadata used for the same " << label << " parameter !!\n";
            }

            // handle acc metadata "...[acc : <axe> <curve> <amin> <amid> <amax>]..."
            if (fCurrentAcc.size() > 0) {
                std::istringstream iss(fCurrentAcc);
                int axe, curve;
                double amin, amid, amax;
                iss >> axe >> curve >> amin >> amid >> amax;

                if ((0 <= axe) && (axe < 3) &&
                    (0 <= curve) && (curve < 4) &&
                    (amin < amax) && (amin <= amid) && (amid <= amax))
                {
                    fAcc[axe].push_back(new CurveZoneControl(zone, curve, amin, amid, amax, min, init, max));
                } else {
                    std::cerr << "incorrect acc metadata : " << fCurrentAcc << std::endl;
                }
                fCurrentAcc = "";
            }
       
            // handle gyr metadata "...[gyr : <axe> <curve> <amin> <amid> <amax>]..."
            if (fCurrentGyr.size() > 0) {
                std::istringstream iss(fCurrentGyr);
                int axe, curve;
                double amin, amid, amax;
                iss >> axe >> curve >> amin >> amid >> amax;

                if ((0 <= axe) && (axe < 3) &&
                    (0 <= curve) && (curve < 4) &&
                    (amin < amax) && (amin <= amid) && (amid <= amax))
                {
                    fGyr[axe].push_back(new CurveZoneControl(zone, curve, amin, amid, amax, min, init, max));
                } else {
                    std::cerr << "incorrect gyr metadata : " << fCurrentGyr << std::endl;
                }
                fCurrentGyr = "";
            }
        
            // handle screencolor metadata "...[screencolor:red|green|blue|white]..."
            if (fCurrentColor.size() > 0) {
                if ((fCurrentColor == "red") && (fRedReader == 0)) {
                    fRedReader = new ZoneReader(zone, min, max);
                    fHasScreenControl = true;
                } else if ((fCurrentColor == "green") && (fGreenReader == 0)) {
                    fGreenReader = new ZoneReader(zone, min, max);
                    fHasScreenControl = true;
                } else if ((fCurrentColor == "blue") && (fBlueReader == 0)) {
                    fBlueReader = new ZoneReader(zone, min, max);
                    fHasScreenControl = true;
                } else if ((fCurrentColor == "white") && (fRedReader == 0) && (fGreenReader == 0) && (fBlueReader == 0)) {
                    fRedReader = new ZoneReader(zone, min, max);
                    fGreenReader = new ZoneReader(zone, min, max);
                    fBlueReader = new ZoneReader(zone, min, max);
                    fHasScreenControl = true;
                } else {
                    std::cerr << "incorrect screencolor metadata : " << fCurrentColor << std::endl;
                }
            }
            fCurrentColor = "";
            
            fMetaData.push_back(fCurrentMetadata);
            fCurrentMetadata.clear();
        }

        int getZoneIndex(std::vector<ZoneControl*>* table, int p, int val)
        {
            FAUSTFLOAT* zone = fZone[p];
            for (size_t i = 0; i < table[val].size(); i++) {
                if (zone == table[val][i]->getZone()) return int(i);
            }
            return -1;
        }
    
        void setConverter(std::vector<ZoneControl*>* table, int p, int val, int curve, double amin, double amid, double amax)
        {
            int id1 = getZoneIndex(table, p, 0);
            int id2 = getZoneIndex(table, p, 1);
            int id3 = getZoneIndex(table, p, 2);
            
            // Deactivates everywhere..
            if (id1 != -1) table[0][id1]->setActive(false);
            if (id2 != -1) table[1][id2]->setActive(false);
            if (id3 != -1) table[2][id3]->setActive(false);
            
            if (val == -1) { // Means: no more mapping...
                // So stay all deactivated...
            } else {
                int id4 = getZoneIndex(table, p, val);
                if (id4 != -1) {
                    // Reactivate the one we edit...
                    table[val][id4]->setMappingValues(curve, amin, amid, amax, fMin[p], fInit[p], fMax[p]);
                    table[val][id4]->setActive(true);
                } else {
                    // Allocate a new CurveZoneControl which is 'active' by default
                    FAUSTFLOAT* zone = fZone[p];
                    table[val].push_back(new CurveZoneControl(zone, curve, amin, amid, amax, fMin[p], fInit[p], fMax[p]));
                }
            }
        }
    
        void getConverter(std::vector<ZoneControl*>* table, int p, int& val, int& curve, double& amin, double& amid, double& amax)
        {
            int id1 = getZoneIndex(table, p, 0);
            int id2 = getZoneIndex(table, p, 1);
            int id3 = getZoneIndex(table, p, 2);
            
            if (id1 != -1) {
                val = 0;
                curve = table[val][id1]->getCurve();
                table[val][id1]->getMappingValues(amin, amid, amax);
            } else if (id2 != -1) {
                val = 1;
                curve = table[val][id2]->getCurve();
                table[val][id2]->getMappingValues(amin, amid, amax);
            } else if (id3 != -1) {
                val = 2;
                curve = table[val][id3]->getCurve();
                table[val][id3]->getMappingValues(amin, amid, amax);
            } else {
                val = -1; // No mapping
                curve = 0;
                amin = -100.;
                amid = 0.;
                amax = 100.;
            }
        }

     public:
    
        enum Type { kAcc = 0, kGyr = 1, kNoType };
   
        APIUI() : fNumParameters(0), fHasScreenControl(false), fRedReader(0), fGreenReader(0), fBlueReader(0), fCurrentScale(kLin)
        {}

        virtual ~APIUI()
        {
            for (auto& it : fConversion) delete it;
            for (int i = 0; i < 3; i++) {
                for (auto& it : fAcc[i]) delete it;
                for (auto& it : fGyr[i]) delete it;
            }
            delete fRedReader;
            delete fGreenReader;
            delete fBlueReader;
        }
    
        // -- widget's layouts

        virtual void openTabBox(const char* label) { pushLabel(label); }
        virtual void openHorizontalBox(const char* label) { pushLabel(label); }
        virtual void openVerticalBox(const char* label) { pushLabel(label); }
        virtual void closeBox() { popLabel(); }

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addParameter(label, zone, 0, 0, 1, 1, kButton);
        }

        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addParameter(label, zone, 0, 0, 1, 1, kCheckButton);
        }

        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addParameter(label, zone, init, min, max, step, kVSlider);
        }

        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addParameter(label, zone, init, min, max, step, kHSlider);
        }

        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addParameter(label, zone, init, min, max, step, kNumEntry);
        }

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addParameter(label, zone, min, min, max, (max-min)/1000.0, kHBargraph);
        }

        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addParameter(label, zone, min, min, max, (max-min)/1000.0, kVBargraph);
        }
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {
            // Keep matadata
            fCurrentMetadata[key] = val;
            
            if (strcmp(key, "scale") == 0) {
                if (strcmp(val, "log") == 0) {
                    fCurrentScale = kLog;
                } else if (strcmp(val, "exp") == 0) {
                    fCurrentScale = kExp;
                } else {
                    fCurrentScale = kLin;
                }
            } else if (strcmp(key, "unit") == 0) {
                fCurrentUnit = val;
            } else if (strcmp(key, "acc") == 0) {
                fCurrentAcc = val;
            } else if (strcmp(key, "gyr") == 0) {
                fCurrentGyr = val;
            } else if (strcmp(key, "screencolor") == 0) {
                fCurrentColor = val; // val = "red", "green", "blue" or "white"
            } else if (strcmp(key, "tooltip") == 0) {
                fCurrentTooltip = val;
            }
        }

        virtual void declare(const char* key, const char* val)
        {}

		//-------------------------------------------------------------------------------
		// Simple API part
		//-------------------------------------------------------------------------------
		int getParamsCount() { return fNumParameters; }
        int getParamIndex(const char* path)
        {
            if (fPathMap.find(path) != fPathMap.end()) {
                return fPathMap[path];
            } else if (fLabelMap.find(path) != fLabelMap.end()) {
                return fLabelMap[path];
            } else {
                return -1;
            }
        }
        const char* getParamAddress(int p) { return fPaths[p].c_str(); }
        const char* getParamLabel(int p) { return fLabels[p].c_str(); }
        std::map<const char*, const char*> getMetadata(int p)
        {
            std::map<const char*, const char*> res;
            std::map<std::string, std::string> metadata = fMetaData[p];
            std::map<std::string, std::string>::iterator it;
            for (it = metadata.begin(); it != metadata.end(); ++it) {
                res[(*it).first.c_str()] = (*it).second.c_str();
            }
            return res;
        }

        const char* getMetadata(int p, const char* key)
        {
            return (fMetaData[p].find(key) != fMetaData[p].end()) ? fMetaData[p][key].c_str() : "";
        }
        FAUSTFLOAT getParamMin(int p) { return fMin[p]; }
        FAUSTFLOAT getParamMax(int p) { return fMax[p]; }
        FAUSTFLOAT getParamStep(int p) { return fStep[p]; }
        FAUSTFLOAT getParamInit(int p) { return fInit[p]; }

        FAUSTFLOAT* getParamZone(int p) { return fZone[p]; }
        FAUSTFLOAT getParamValue(int p) { return *fZone[p]; }
        void setParamValue(int p, FAUSTFLOAT v) { *fZone[p] = v; }

        double getParamRatio(int p) { return fConversion[p]->faust2ui(*fZone[p]); }
        void setParamRatio(int p, double r) { *fZone[p] = fConversion[p]->ui2faust(r); }

        double value2ratio(int p, double r)	{ return fConversion[p]->faust2ui(r); }
        double ratio2value(int p, double r)	{ return fConversion[p]->ui2faust(r); }
    
        /**
         * Return the control type (kAcc, kGyr, or -1) for a given parameter
         *
         * @param p - the UI parameter index
         *
         * @return the type
         */
        Type getParamType(int p)
        {
            if (p >= 0) {
                if (getZoneIndex(fAcc, p, 0) != -1
                    || getZoneIndex(fAcc, p, 1) != -1
                    || getZoneIndex(fAcc, p, 2) != -1) {
                    return kAcc;
                } else if (getZoneIndex(fGyr, p, 0) != -1
                           || getZoneIndex(fGyr, p, 1) != -1
                           || getZoneIndex(fGyr, p, 2) != -1) {
                    return kGyr;
                }
            }
            return kNoType;
        }
    
        /**
         * Return the Item type (kButton = 0, kCheckButton, kVSlider, kHSlider, kNumEntry, kHBargraph, kVBargraph) for a given parameter
         *
         * @param p - the UI parameter index
         *
         * @return the Item type
         */
        ItemType getParamItemType(int p)
        {
            return fItemType[p];
        }
   
        /**
         * Set a new value coming from an accelerometer, propagate it to all relevant FAUSTFLOAT* zones.
         *
         * @param acc - 0 for X accelerometer, 1 for Y accelerometer, 2 for Z accelerometer
         * @param value - the new value
         *
         */
        void propagateAcc(int acc, double value)
        {
            for (size_t i = 0; i < fAcc[acc].size(); i++) {
                fAcc[acc][i]->update(value);
            }
        }
    
        /**
         * Used to edit accelerometer curves and mapping. Set curve and related mapping for a given UI parameter.
         *
         * @param p - the UI parameter index
         * @param acc - 0 for X accelerometer, 1 for Y accelerometer, 2 for Z accelerometer (-1 means "no mapping")
         * @param curve - between 0 and 3
         * @param amin - mapping 'min' point
         * @param amid - mapping 'middle' point
         * @param amax - mapping 'max' point
         *
         */
        void setAccConverter(int p, int acc, int curve, double amin, double amid, double amax)
        {
            setConverter(fAcc, p, acc, curve, amin, amid, amax);
        }
    
        /**
         * Used to edit gyroscope curves and mapping. Set curve and related mapping for a given UI parameter.
         *
         * @param p - the UI parameter index
         * @param acc - 0 for X gyroscope, 1 for Y gyroscope, 2 for Z gyroscope (-1 means "no mapping")
         * @param curve - between 0 and 3
         * @param amin - mapping 'min' point
         * @param amid - mapping 'middle' point
         * @param amax - mapping 'max' point
         *
         */
        void setGyrConverter(int p, int gyr, int curve, double amin, double amid, double amax)
        {
             setConverter(fGyr, p, gyr, curve, amin, amid, amax);
        }
    
        /**
         * Used to edit accelerometer curves and mapping. Get curve and related mapping for a given UI parameter.
         *
         * @param p - the UI parameter index
         * @param acc - the acc value to be retrieved (-1 means "no mapping")
         * @param curve - the curve value to be retrieved
         * @param amin - the amin value to be retrieved
         * @param amid - the amid value to be retrieved
         * @param amax - the amax value to be retrieved
         *
         */
        void getAccConverter(int p, int& acc, int& curve, double& amin, double& amid, double& amax)
        {
            getConverter(fAcc, p, acc, curve, amin, amid, amax);
        }

        /**
         * Used to edit gyroscope curves and mapping. Get curve and related mapping for a given UI parameter.
         *
         * @param p - the UI parameter index
         * @param gyr - the gyr value to be retrieved (-1 means "no mapping")
         * @param curve - the curve value to be retrieved
         * @param amin - the amin value to be retrieved
         * @param amid - the amid value to be retrieved
         * @param amax - the amax value to be retrieved
         *
         */
        void getGyrConverter(int p, int& gyr, int& curve, double& amin, double& amid, double& amax)
        {
            getConverter(fGyr, p, gyr, curve, amin, amid, amax);
        }
    
        /**
         * Set a new value coming from an gyroscope, propagate it to all relevant FAUSTFLOAT* zones.
         *
         * @param gyr - 0 for X gyroscope, 1 for Y gyroscope, 2 for Z gyroscope
         * @param value - the new value
         *
         */
        void propagateGyr(int gyr, double value)
        {
            for (size_t i = 0; i < fGyr[gyr].size(); i++) {
                fGyr[gyr][i]->update(value);
            }
        }
    
        /**
         * Get the number of FAUSTFLOAT* zones controlled with the accelerometer
         *
         * @param acc - 0 for X accelerometer, 1 for Y accelerometer, 2 for Z accelerometer
         * @return the number of zones
         *
         */
        int getAccCount(int acc)
        {
            return (acc >= 0 && acc < 3) ? int(fAcc[acc].size()) : 0;
        }
    
        /**
         * Get the number of FAUSTFLOAT* zones controlled with the gyroscope
         *
         * @param gyr - 0 for X gyroscope, 1 for Y gyroscope, 2 for Z gyroscope
         * @param the number of zones
         *
         */
        int getGyrCount(int gyr)
        {
            return (gyr >= 0 && gyr < 3) ? int(fGyr[gyr].size()) : 0;
        }
   
        // getScreenColor() : -1 means no screen color control (no screencolor metadata found)
        // otherwise return 0x00RRGGBB a ready to use color
        int getScreenColor()
        {
            if (fHasScreenControl) {
                int r = (fRedReader) ? fRedReader->getValue() : 0;
                int g = (fGreenReader) ? fGreenReader->getValue() : 0;
                int b = (fBlueReader) ? fBlueReader->getValue() : 0;
                return (r<<16) | (g<<8) | b;
            } else {
                return -1;
            }
        }
 
};

#endif
/**************************  END  APIUI.h **************************/
/************************** BEGIN MidiUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MIDIUI_H
#define FAUST_MIDIUI_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <cmath>

/************************** BEGIN GUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __GUI_H__
#define __GUI_H__

#include <list>
#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

#ifdef _WIN32
# pragma warning (disable: 4100)
#else
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/************************** BEGIN ring-buffer.h **************************/
/*
  Copyright (C) 2000 Paul Davis
  Copyright (C) 2003 Rohan Drape
  Copyright (C) 2016 GRAME (renaming for internal use)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

  ISO/POSIX C version of Paul Davis's lock free ringbuffer C++ code.
  This is safe for the case of one read thread and one write thread.
*/

#ifndef __ring_buffer__
#define __ring_buffer__

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
# pragma warning (disable: 4334)
#else
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

typedef struct {
    char *buf;
    size_t len;
}
ringbuffer_data_t;

typedef struct {
    char *buf;
    volatile size_t write_ptr;
    volatile size_t read_ptr;
    size_t	size;
    size_t	size_mask;
    int	mlocked;
}
ringbuffer_t;

static ringbuffer_t *ringbuffer_create(size_t sz);
static void ringbuffer_free(ringbuffer_t *rb);
static void ringbuffer_get_read_vector(const ringbuffer_t *rb,
                                         ringbuffer_data_t *vec);
static void ringbuffer_get_write_vector(const ringbuffer_t *rb,
                                          ringbuffer_data_t *vec);
static size_t ringbuffer_read(ringbuffer_t *rb, char *dest, size_t cnt);
static size_t ringbuffer_peek(ringbuffer_t *rb, char *dest, size_t cnt);
static void ringbuffer_read_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_read_space(const ringbuffer_t *rb);
static int ringbuffer_mlock(ringbuffer_t *rb);
static void ringbuffer_reset(ringbuffer_t *rb);
static void ringbuffer_reset_size (ringbuffer_t * rb, size_t sz);
static size_t ringbuffer_write(ringbuffer_t *rb, const char *src,
                                 size_t cnt);
static void ringbuffer_write_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_write_space(const ringbuffer_t *rb);

/* Create a new ringbuffer to hold at least `sz' bytes of data. The
   actual buffer size is rounded up to the next power of two. */

static ringbuffer_t *
ringbuffer_create (size_t sz)
{
	size_t power_of_two;
	ringbuffer_t *rb;

	if ((rb = (ringbuffer_t *) malloc (sizeof (ringbuffer_t))) == NULL) {
		return NULL;
	}

	for (power_of_two = 1u; 1u << power_of_two < sz; power_of_two++);

	rb->size = 1u << power_of_two;
	rb->size_mask = rb->size;
	rb->size_mask -= 1;
	rb->write_ptr = 0;
	rb->read_ptr = 0;
	if ((rb->buf = (char *) malloc (rb->size)) == NULL) {
		free (rb);
		return NULL;
	}
	rb->mlocked = 0;

	return rb;
}

/* Free all data associated with the ringbuffer `rb'. */

static void
ringbuffer_free (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (rb->mlocked) {
		munlock (rb->buf, rb->size);
	}
#endif /* USE_MLOCK */
	free (rb->buf);
	free (rb);
}

/* Lock the data block of `rb' using the system call 'mlock'.  */

static int
ringbuffer_mlock (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (mlock (rb->buf, rb->size)) {
		return -1;
	}
#endif /* USE_MLOCK */
	rb->mlocked = 1;
	return 0;
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset (ringbuffer_t * rb)
{
	rb->read_ptr = 0;
	rb->write_ptr = 0;
    memset(rb->buf, 0, rb->size);
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset_size (ringbuffer_t * rb, size_t sz)
{
    rb->size = sz;
    rb->size_mask = rb->size;
    rb->size_mask -= 1;
    rb->read_ptr = 0;
    rb->write_ptr = 0;
}

/* Return the number of bytes available for reading. This is the
   number of bytes in front of the read pointer and behind the write
   pointer.  */

static size_t
ringbuffer_read_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return w - r;
	} else {
		return (w - r + rb->size) & rb->size_mask;
	}
}

/* Return the number of bytes available for writing. This is the
   number of bytes in front of the write pointer and behind the read
   pointer.  */

static size_t
ringbuffer_write_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		return (r - w) - 1;
	} else {
		return rb->size - 1;
	}
}

/* The copying data reader. Copy at most `cnt' bytes from `rb' to
   `dest'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_read (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[rb->read_ptr]), n1);
	rb->read_ptr = (rb->read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[rb->read_ptr]), n2);
		rb->read_ptr = (rb->read_ptr + n2) & rb->size_mask;
	}

	return to_read;
}

/* The copying data reader w/o read pointer advance. Copy at most
   `cnt' bytes from `rb' to `dest'.  Returns the actual number of bytes
   copied. */

static size_t
ringbuffer_peek (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;
	size_t tmp_read_ptr;

	tmp_read_ptr = rb->read_ptr;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = tmp_read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - tmp_read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[tmp_read_ptr]), n1);
	tmp_read_ptr = (tmp_read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[tmp_read_ptr]), n2);
	}

	return to_read;
}

/* The copying data writer. Copy at most `cnt' bytes to `rb' from
   `src'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_write (ringbuffer_t * rb, const char *src, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_write;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_write_space (rb)) == 0) {
		return 0;
	}

	to_write = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->write_ptr + to_write;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->write_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_write;
		n2 = 0;
	}

	memcpy (&(rb->buf[rb->write_ptr]), src, n1);
	rb->write_ptr = (rb->write_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (&(rb->buf[rb->write_ptr]), src + n1, n2);
		rb->write_ptr = (rb->write_ptr + n2) & rb->size_mask;
	}

	return to_write;
}

/* Advance the read pointer `cnt' places. */

static void
ringbuffer_read_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->read_ptr + cnt) & rb->size_mask;
	rb->read_ptr = tmp;
}

/* Advance the write pointer `cnt' places. */

static void
ringbuffer_write_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->write_ptr + cnt) & rb->size_mask;
	rb->write_ptr = tmp;
}

/* The non-copying data reader. `vec' is an array of two places. Set
   the values at `vec' to hold the current readable data at `rb'. If
   the readable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_read_vector (const ringbuffer_t * rb,
				 ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = w - r;
	} else {
		free_cnt = (w - r + rb->size) & rb->size_mask;
	}

	cnt2 = r + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = rb->size - r;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;

	} else {

		/* Single part vector: just the rest of the buffer */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

/* The non-copying data writer. `vec' is an array of two places. Set
   the values at `vec' to hold the current writeable data at `rb'. If
   the writeable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_write_vector (const ringbuffer_t * rb,
				  ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		free_cnt = (r - w) - 1;
	} else {
		free_cnt = rb->size - 1;
	}

	cnt2 = w + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[w]);
		vec[0].len = rb->size - w;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;
	} else {
		vec[0].buf = &(rb->buf[w]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

#endif // __ring_buffer__
/**************************  END  ring-buffer.h **************************/

/*******************************************************************************
 * GUI : Abstract Graphic User Interface
 * Provides additional mechanisms to synchronize widgets and zones. Widgets
 * should both reflect the value of a zone and allow to change this value.
 ******************************************************************************/

class uiItem;
class GUI;
struct clist;

typedef void (*uiCallback)(FAUSTFLOAT val, void* data);

struct uiItemBase
{
    
    uiItemBase(GUI* ui, FAUSTFLOAT* zone)
    {
        assert(ui);
        assert(zone);
    }
    
    virtual ~uiItemBase()
    {}
    
    virtual void modifyZone(FAUSTFLOAT v) = 0;
    virtual void modifyZone(double date, FAUSTFLOAT v) {}
    virtual double cache() = 0;
    virtual void reflectZone() = 0;
};


static void deleteClist(clist* cl);

struct clist : public std::list<uiItemBase*>
{
    
    virtual ~clist()
    {
        deleteClist(this);
    }
        
};

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data);

typedef std::map<FAUSTFLOAT*, clist*> zmap;

typedef std::map<FAUSTFLOAT*, ringbuffer_t*> ztimedmap;

class GUI : public UI
{
		
    private:
     
        static std::list<GUI*> fGuiList;
        zmap fZoneMap;
        bool fStopped;
        
     public:
            
        GUI():fStopped(false)
        {	
            fGuiList.push_back(this);
        }
        
        virtual ~GUI() 
        {   
            // delete all items
            for (auto& it : fZoneMap) {
                delete it.second;
            }
            // suppress 'this' in static fGuiList
            fGuiList.remove(this);
        }

        // -- registerZone(z,c) : zone management
        
        void registerZone(FAUSTFLOAT* z, uiItemBase* c)
        {
            if (fZoneMap.find(z) == fZoneMap.end()) fZoneMap[z] = new clist();
            fZoneMap[z]->push_back(c);
        }
 
        void updateZone(FAUSTFLOAT* z)
        {
            FAUSTFLOAT v = *z;
            clist* cl = fZoneMap[z];
            for (auto& c : *cl) {
                if (c->cache() != v) c->reflectZone();
            }
        }
    
        void updateAllZones()
        {
            for (auto& m : fZoneMap) {
                updateZone(m.first);
            }
        }
    
        static void updateAllGuis()
        {
            for (auto& g : fGuiList) {
                g->updateAllZones();
            }
        }
    
        static void runAllGuis()
        {
            for (auto& g : fGuiList) {
                g->run();
            }
        }
    
        void addCallback(FAUSTFLOAT* zone, uiCallback foo, void* data)
        {
            createUiCallbackItem(this, zone, foo, data);
        }

        virtual void show() {};	
        virtual bool run() { return false; };

        virtual void stop() { fStopped = true; }
        bool stopped() { return fStopped; }
    
        // -- widget's layouts
        
        virtual void openTabBox(const char* label) {};
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
    
        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
    
        // Static global for timed zones, shared between all UI that will set timed values
        static ztimedmap gTimedZoneMap;

};

/**
 * User Interface Item: abstract definition
 */

template <typename REAL>
class uiTypedItem : public uiItemBase
{
    protected:
        
        GUI* fGUI;
        REAL* fZone;
        REAL fCache;
        
        uiTypedItem(GUI* ui, REAL* zone):uiItemBase(ui, static_cast<FAUSTFLOAT*>(zone)),
        fGUI(ui), fZone(zone), fCache(REAL(-123456.654321))
        {
            ui->registerZone(zone, this);
        }
        
    public:
        
        virtual ~uiTypedItem()
        {}
    
        void modifyZone(REAL v)
        {
            fCache = v;
            if (*fZone != v) {
                *fZone = v;
                fGUI->updateZone(fZone);
            }
        }
    
        double cache() { return fCache; }
    
};

class uiItem : public uiTypedItem<FAUSTFLOAT> {
    
    protected:
    
        uiItem(GUI* ui, FAUSTFLOAT* zone):uiTypedItem<FAUSTFLOAT>(ui, zone)
        {}

    public:

        virtual ~uiItem() 
        {}

		void modifyZone(FAUSTFLOAT v)
		{
			fCache = v;
			if (*fZone != v) {
				*fZone = v;
				fGUI->updateZone(fZone);
			}
		}

};

/**
 * User Interface item owned (and so deleted) by external code
 */

class uiOwnedItem : public uiItem {
    
    protected:
    
        uiOwnedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
    
     public:
    
        virtual ~uiOwnedItem()
        {}
    
        virtual void reflectZone() {}
};

/**
 * Callback Item
 */

class uiCallbackItem : public uiItem {
    
    protected:
    
        uiCallback fCallback;
        void* fData;
    
    public:
    
        uiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
        : uiItem(ui, zone), fCallback(foo), fData(data) {}
        
        virtual void reflectZone() 
        {		
            FAUSTFLOAT v = *fZone;
            fCache = v; 
            fCallback(v, fData);	
        }
};

/**
 *  For timestamped control
 */

struct DatedControl {
    
    double fDate;
    FAUSTFLOAT fValue;
    
    DatedControl(double d = 0., FAUSTFLOAT v = FAUSTFLOAT(0)):fDate(d), fValue(v) {}
    
};

/**
 * Base class for timed items
 */

class uiTimedItem : public uiItem
{
    
    protected:
        
        bool fDelete;
        
    public:
    
        using uiItem::modifyZone;
        
        uiTimedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {
            if (GUI::gTimedZoneMap.find(fZone) == GUI::gTimedZoneMap.end()) {
                GUI::gTimedZoneMap[fZone] = ringbuffer_create(8192);
                fDelete = true;
            } else {
                fDelete = false;
            }
        }
        
        virtual ~uiTimedItem()
        {
            ztimedmap::iterator it;
            if (fDelete && ((it = GUI::gTimedZoneMap.find(fZone)) != GUI::gTimedZoneMap.end())) {
                ringbuffer_free((*it).second);
                GUI::gTimedZoneMap.erase(it);
            }
        }
        
        virtual void modifyZone(double date, FAUSTFLOAT v)
        {
            size_t res;
            DatedControl dated_val(date, v);
            if ((res = ringbuffer_write(GUI::gTimedZoneMap[fZone], (const char*)&dated_val, sizeof(DatedControl))) != sizeof(DatedControl)) {
                std::cerr << "ringbuffer_write error DatedControl" << std::endl;
            }
        }
    
};

/**
 * Allows to group a set of zones
 */

class uiGroupItem : public uiItem
{
    protected:
    
        std::vector<FAUSTFLOAT*> fZoneMap;

    public:
    
        uiGroupItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
        virtual ~uiGroupItem() 
        {}
        
        virtual void reflectZone() 
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            
            // Update all zones of the same group
            for (auto& it : fZoneMap) {
                *it = v;
            }
        }
        
        void addZone(FAUSTFLOAT* zone) { fZoneMap.push_back(zone); }

};

// Can not be defined as method in the classes

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
{
    new uiCallbackItem(ui, zone, foo, data);
}

static void deleteClist(clist* cl)
{
    for (auto& it : *cl) {
        uiOwnedItem* owned = dynamic_cast<uiOwnedItem*>(it);
        // owned items are deleted by external code
        if (!owned) {
            delete it;
        }
    }
}

#endif
/**************************  END  GUI.h **************************/
/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>


/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {};
        virtual ~MapUI() {};
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        { 
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end()) {}
            return (*it).first;
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            do {
                if ((*it).second == zone) return (*it).first;
            }
            while (it++ != fPathZoneMap.end());
            return "";
        }
    
        static bool endsWith(std::string const& str, std::string const& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN MetaDataUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef MetaData_UI_H
#define MetaData_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <map>
#include <set>
#include <string>
#include <assert.h>


static bool startWith(const std::string& str, const std::string& prefix)
{
    return (str.substr(0, prefix.size()) == prefix);
}

/**
 * Convert a dB value into a scale between 0 and 1 (following IEC standard ?)
 */
static FAUSTFLOAT dB2Scale(FAUSTFLOAT dB)
{
    FAUSTFLOAT scale = FAUSTFLOAT(1.0);
    
    /*if (dB < -70.0f)
     scale = 0.0f;
     else*/
    if (dB < FAUSTFLOAT(-60.0))
        scale = (dB + FAUSTFLOAT(70.0)) * FAUSTFLOAT(0.0025);
    else if (dB < FAUSTFLOAT(-50.0))
        scale = (dB + FAUSTFLOAT(60.0)) * FAUSTFLOAT(0.005) + FAUSTFLOAT(0.025);
    else if (dB < FAUSTFLOAT(-40.0))
        scale = (dB + FAUSTFLOAT(50.0)) * FAUSTFLOAT(0.0075) + FAUSTFLOAT(0.075);
    else if (dB < FAUSTFLOAT(-30.0))
        scale = (dB + FAUSTFLOAT(40.0)) * FAUSTFLOAT(0.015) + FAUSTFLOAT(0.15);
    else if (dB < FAUSTFLOAT(-20.0))
        scale = (dB + FAUSTFLOAT(30.0)) * FAUSTFLOAT(0.02) + FAUSTFLOAT(0.3);
    else if (dB < FAUSTFLOAT(-0.001) || dB > FAUSTFLOAT(0.001))  /* if (dB < 0.0) */
        scale = (dB + FAUSTFLOAT(20.0)) * FAUSTFLOAT(0.025) + FAUSTFLOAT(0.5);
    
    return scale;
}

/*******************************************************************************
 * MetaDataUI : Common class for MetaData handling
 ******************************************************************************/

//============================= BEGIN GROUP LABEL METADATA===========================
// Unlike widget's label, metadata inside group's label are not extracted directly by
// the Faust compiler. Therefore they must be extracted within the architecture file
//-----------------------------------------------------------------------------------

class MetaDataUI {
    
    protected:
        
        std::string                         fGroupTooltip;
        std::map<FAUSTFLOAT*, FAUSTFLOAT>   fGuiSize;            // map widget zone with widget size coef
        std::map<FAUSTFLOAT*, std::string>  fTooltip;            // map widget zone with tooltip strings
        std::map<FAUSTFLOAT*, std::string>  fUnit;               // map widget zone to unit string (i.e. "dB")
        std::map<FAUSTFLOAT*, std::string>  fRadioDescription;   // map zone to {'low':440; ...; 'hi':1000.0}
        std::map<FAUSTFLOAT*, std::string>  fMenuDescription;    // map zone to {'low':440; ...; 'hi':1000.0}
        std::set<FAUSTFLOAT*>               fKnobSet;            // set of widget zone to be knobs
        std::set<FAUSTFLOAT*>               fLedSet;             // set of widget zone to be LEDs
        std::set<FAUSTFLOAT*>               fNumSet;             // set of widget zone to be numerical bargraphs
        std::set<FAUSTFLOAT*>               fLogSet;             // set of widget zone having a log UI scale
        std::set<FAUSTFLOAT*>               fExpSet;             // set of widget zone having an exp UI scale
        std::set<FAUSTFLOAT*>               fHiddenSet;          // set of hidden widget zone
        
        void clearMetadata()
        {
            fGuiSize.clear();
            fTooltip.clear();
            fUnit.clear();
            fRadioDescription.clear();
            fMenuDescription.clear();
            fKnobSet.clear();
            fLedSet.clear();
            fNumSet.clear();
            fLogSet.clear();
            fExpSet.clear();
            fHiddenSet.clear();
        }
        
        /**
         * rmWhiteSpaces(): Remove the leading and trailing white spaces of a string
         * (but not those in the middle of the string)
         */
        static std::string rmWhiteSpaces(const std::string& s)
        {
            size_t i = s.find_first_not_of(" \t");
            size_t j = s.find_last_not_of(" \t");
            if ((i != std::string::npos) && (j != std::string::npos)) {
                return s.substr(i, 1+j-i);
            } else {
                return "";
            }
        }
        
        /**
         * Format tooltip string by replacing some white spaces by
         * return characters so that line width doesn't exceed n.
         * Limitation : long words exceeding n are not cut
         */
        std::string formatTooltip(int n, const std::string& tt)
        {
            std::string ss = tt;  // ss string we are going to format
            int lws = 0;          // last white space encountered
            int lri = 0;          // last return inserted
            for (int i = 0; i < (int)tt.size(); i++) {
                if (tt[i] == ' ') lws = i;
                if (((i-lri) >= n) && (lws > lri)) {
                    // insert return here
                    ss[lws] = '\n';
                    lri = lws;
                }
            }
            return ss;
        }
        
    public:
        
        virtual ~MetaDataUI()
        {}
        
        enum Scale {
            kLin,
            kLog,
            kExp
        };
        
        Scale getScale(FAUSTFLOAT* zone)
        {
            if (fLogSet.count(zone) > 0) return kLog;
            if (fExpSet.count(zone) > 0) return kExp;
            return kLin;
        }
        
        bool isKnob(FAUSTFLOAT* zone)
        {
            return fKnobSet.count(zone) > 0;
        }
        
        bool isRadio(FAUSTFLOAT* zone)
        {
            return fRadioDescription.count(zone) > 0;
        }
        
        bool isMenu(FAUSTFLOAT* zone)
        {
            return fMenuDescription.count(zone) > 0;
        }
        
        bool isLed(FAUSTFLOAT* zone)
        {
            return fLedSet.count(zone) > 0;
        }
        
        bool isNumerical(FAUSTFLOAT* zone)
        {
            return fNumSet.count(zone) > 0;
        }
        
        bool isHidden(FAUSTFLOAT* zone)
        {
            return fHiddenSet.count(zone) > 0;
        }
        
        /**
         * Extracts metadata from a label : 'vol [unit: dB]' -> 'vol' + metadata(unit=dB)
         */
        static void extractMetadata(const std::string& fulllabel, std::string& label, std::map<std::string, std::string>& metadata)
        {
            enum {kLabel, kEscape1, kEscape2, kEscape3, kKey, kValue};
            int state = kLabel; int deep = 0;
            std::string key, value;
            
            for (unsigned int i = 0; i < fulllabel.size(); i++) {
                char c = fulllabel[i];
                switch (state) {
                    case kLabel :
                        assert(deep == 0);
                        switch (c) {
                            case '\\' : state = kEscape1; break;
                            case '[' : state = kKey; deep++; break;
                            default : label += c;
                        }
                        break;
                        
                    case kEscape1:
                        label += c;
                        state = kLabel;
                        break;
                        
                    case kEscape2:
                        key += c;
                        state = kKey;
                        break;
                        
                    case kEscape3:
                        value += c;
                        state = kValue;
                        break;
                        
                    case kKey:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape2;
                                break;
                                
                            case '[':
                                deep++;
                                key += c;
                                break;
                                
                            case ':':
                                if (deep == 1) {
                                    state = kValue;
                                } else {
                                    key += c;
                                }
                                break;
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = "";
                                    state = kLabel;
                                    key="";
                                    value="";
                                } else {
                                    key += c;
                                }
                                break;
                            default : key += c;
                        }
                        break;
                        
                    case kValue:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape3;
                                break;
                                
                            case '[':
                                deep++;
                                value += c;
                                break;
                                
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = rmWhiteSpaces(value);
                                    state = kLabel;
                                    key = "";
                                    value = "";
                                } else {
                                    value += c;
                                }
                                break;
                            default : value += c;
                        }
                        break;
                        
                    default:
                        std::cerr << "ERROR unrecognized state " << state << std::endl;
                }
            }
            label = rmWhiteSpaces(label);
        }
        
        /**
         * Analyses the widget zone metadata declarations and takes appropriate actions
         */
        void declare(FAUSTFLOAT* zone, const char* key, const char* value)
        {
            if (zone == 0) {
                // special zone 0 means group metadata
                if (strcmp(key, "tooltip") == 0) {
                    // only group tooltip are currently implemented
                    fGroupTooltip = formatTooltip(30, value);
                } else if (strcmp(key, "hidden") == 0) {
                    fHiddenSet.insert(zone);
                }
            } else {
                if (strcmp(key, "size") == 0) {
                    fGuiSize[zone] = atof(value);
                }
                else if (strcmp(key, "tooltip") == 0) {
                    fTooltip[zone] = formatTooltip(30, value);
                }
                else if (strcmp(key, "unit") == 0) {
                    fUnit[zone] = value;
                }
                else if (strcmp(key, "hidden") == 0) {
                    fHiddenSet.insert(zone);
                }
                else if (strcmp(key, "scale") == 0) {
                    if (strcmp(value, "log") == 0) {
                        fLogSet.insert(zone);
                    } else if (strcmp(value, "exp") == 0) {
                        fExpSet.insert(zone);
                    }
                }
                else if (strcmp(key, "style") == 0) {
                    if (strcmp(value, "knob") == 0) {
                        fKnobSet.insert(zone);
                    } else if (strcmp(value, "led") == 0) {
                        fLedSet.insert(zone);
                    } else if (strcmp(value, "numerical") == 0) {
                        fNumSet.insert(zone);
                    } else {
                        const char* p = value;
                        if (parseWord(p, "radio")) {
                            fRadioDescription[zone] = std::string(p);
                        } else if (parseWord(p, "menu")) {
                            fMenuDescription[zone] = std::string(p);
                        }
                    }
                }
            }
        }
    
};

#endif
/**************************  END  MetaDataUI.h **************************/
/************************** BEGIN midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __midi__
#define __midi__

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

class MapUI;

/*************************************
 A time-stamped short MIDI message
**************************************/

// Force contiguous memory layout
#pragma pack (push, 1)
struct MIDIMessage
{
    uint32_t frameIndex;
    uint8_t byte0, byte1, byte2;
};
#pragma pack (pop)

/*******************************************************************************
 * MIDI processor definition.
 *
 * MIDI input or output handling classes will implement this interface,
 * so the same method names (keyOn, ctrlChange...) will be used either
 * when decoding MIDI input or encoding MIDI output events.
 *******************************************************************************/

class midi {

    public:

        midi() {}
        virtual ~midi() {}

        // Additional time-stamped API for MIDI input
        virtual MapUI* keyOn(double, int channel, int pitch, int velocity)
        {
            return keyOn(channel, pitch, velocity);
        }
        
        virtual void keyOff(double, int channel, int pitch, int velocity = 127)
        {
            keyOff(channel, pitch, velocity);
        }
    
        virtual void keyPress(double, int channel, int pitch, int press)
        {
            keyPress(channel, pitch, press);
        }
        
        virtual void chanPress(double date, int channel, int press)
        {
            chanPress(channel, press);
        }
    
        virtual void pitchWheel(double, int channel, int wheel)
        {
            pitchWheel(channel, wheel);
        }
           
        virtual void ctrlChange(double, int channel, int ctrl, int value)
        {
            ctrlChange(channel, ctrl, value);
        }
    
        virtual void ctrlChange14bits(double, int channel, int ctrl, int value)
        {
            ctrlChange14bits(channel, ctrl, value);
        }
    
        virtual void rpn(double, int channel, int ctrl, int value)
        {
            rpn(channel, ctrl, value);
        }

        virtual void progChange(double, int channel, int pgm)
        {
            progChange(channel, pgm);
        }
    
        virtual void sysEx(double, std::vector<unsigned char>& message)
        {
            sysEx(message);
        }

        // MIDI sync
        virtual void startSync(double date)  {}
        virtual void stopSync(double date)   {}
        virtual void clock(double date)  {}

        // Standard MIDI API
        virtual MapUI* keyOn(int channel, int pitch, int velocity)      { return nullptr; }
        virtual void keyOff(int channel, int pitch, int velocity)       {}
        virtual void keyPress(int channel, int pitch, int press)        {}
        virtual void chanPress(int channel, int press)                  {}
        virtual void ctrlChange(int channel, int ctrl, int value)       {}
        virtual void ctrlChange14bits(int channel, int ctrl, int value) {}
        virtual void rpn(int channel, int ctrl, int value)              {}
        virtual void pitchWheel(int channel, int wheel)                 {}
        virtual void progChange(int channel, int pgm)                   {}
        virtual void sysEx(std::vector<unsigned char>& message)         {}

        enum MidiStatus {
            // channel voice messages
            MIDI_NOTE_OFF = 0x80,
            MIDI_NOTE_ON = 0x90,
            MIDI_CONTROL_CHANGE = 0xB0,
            MIDI_PROGRAM_CHANGE = 0xC0,
            MIDI_PITCH_BEND = 0xE0,
            MIDI_AFTERTOUCH = 0xD0,         // aka channel pressure
            MIDI_POLY_AFTERTOUCH = 0xA0,    // aka key pressure
            MIDI_CLOCK = 0xF8,
            MIDI_START = 0xFA,
            MIDI_CONT = 0xFB,
            MIDI_STOP = 0xFC,
            MIDI_SYSEX_START = 0xF0,
            MIDI_SYSEX_STOP = 0xF7
        };

        enum MidiCtrl {
            ALL_NOTES_OFF = 123,
            ALL_SOUND_OFF = 120
        };
    
        enum MidiNPN {
            PITCH_BEND_RANGE = 0
        };

};

/*
 A class to decode NRPN and RPN messages, adapted from JUCE forum message: https://forum.juce.com/t/14bit-midi-controller-support/11517
*/

class MidiNRPN
{
    
    private:
    
        bool ctrlnew;
        int ctrlnum;
        int ctrlval;
        
        int nrpn_lsb, nrpn_msb;
        int data_lsb, data_msb;
        
        enum
        {
            midi_nrpn_lsb = 98,
            midi_nrpn_msb = 99,
            midi_rpn_lsb  = 100,
            midi_rpn_msb  = 101,
            midi_data_lsb = 38,
            midi_data_msb = 6
        };
    
    public:
        
        MidiNRPN(): ctrlnew(false), nrpn_lsb(-1), nrpn_msb(-1), data_lsb(-1), data_msb(-1)
        {}
        
        // return true if the message has been filtered
        bool process(int data1, int data2)
        {
            switch (data1)
            {
                case midi_nrpn_lsb: nrpn_lsb = data2; return true;
                case midi_nrpn_msb: nrpn_msb = data2; return true;
                case midi_rpn_lsb: {
                    if (data2 == 127) {
                        nrpn_lsb = data_lsb = -1;
                    } else {
                        nrpn_lsb = 0;
                        data_lsb = -1;
                    }
                    return true;
                }
                case midi_rpn_msb: {
                    if (data2 == 127) {
                        nrpn_msb = data_msb = -1;
                    } else {
                        nrpn_msb = 0;
                        data_msb = -1;
                    }
                    return true;
                }
                case midi_data_lsb:
                case midi_data_msb:
                {
                    if (data1 == midi_data_msb) {
                        if (nrpn_msb < 0) {
                            return false;
                        }
                        data_msb = data2;
                    } else { // midi_data_lsb
                        if (nrpn_lsb < 0) {
                            return false;
                        }
                        data_lsb = data2;
                    }
                    if (data_lsb >= 0 && data_msb >= 0) {
                        ctrlnum = (nrpn_msb << 7) | nrpn_lsb;
                        ctrlval = (data_msb << 7) | data_lsb;
                        data_lsb = data_msb = -1;
                        nrpn_msb = nrpn_lsb = -1;
                        ctrlnew = true;
                    }
                    return true;
                }
                default: return false;
            };
        }
        
        bool hasNewNRPN() { bool res = ctrlnew; ctrlnew = false; return res; }
        
        // results in [0, 16383]
        int getCtrl() const { return ctrlnum; }
        int getVal() const { return ctrlval; }
    
};


/****************************************************
 * Base class for MIDI input handling.
 *
 * Shared common code used for input handling:
 * - decoding Real-Time messages: handleSync
 * - decoding one data byte messages: handleData1
 * - decoding two data byte messages: handleData2
 * - getting ready messages in polling mode
 ****************************************************/

class midi_handler : public midi {

    protected:

        std::vector<midi*> fMidiInputs;
        std::string fName;
        MidiNRPN fNRPN;
    
        int range(int min, int max, int val) { return (val < min) ? min : ((val >= max) ? max : val); }
  
    public:

        midi_handler(const std::string& name = "MIDIHandler"):fName(name) {}
        virtual ~midi_handler() {}

        void addMidiIn(midi* midi_dsp) { if (midi_dsp) fMidiInputs.push_back(midi_dsp); }
        void removeMidiIn(midi* midi_dsp)
        {
            std::vector<midi*>::iterator it = std::find(fMidiInputs.begin(), fMidiInputs.end(), midi_dsp);
            if (it != fMidiInputs.end()) {
                fMidiInputs.erase(it);
            }
        }

        virtual bool startMidi() { return true; }
        virtual void stopMidi() {}
    
        void setName(const std::string& name) { fName = name; }
        std::string getName() { return fName; }
    
        // To be used in polling mode
        virtual int recvMessages(std::vector<MIDIMessage>* message) { return 0; }
        virtual void sendMessages(std::vector<MIDIMessage>* message, int count) {}
    
        // MIDI Real-Time
        void handleClock(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->clock(time);
            }
        }
        
        void handleStart(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->startSync(time);
            }
        }
        
        void handleStop(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->stopSync(time);
            }
        }
    
        void handleSync(double time, int type)
        {
            if (type == MIDI_CLOCK) {
                handleClock(time);
            // We can consider start and continue as identical messages
            } else if ((type == MIDI_START) || (type == MIDI_CONT)) {
                handleStart(time);
            } else if (type == MIDI_STOP) {
                handleStop(time);
            }
        }
    
        // MIDI 1 data
        void handleProgChange(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->progChange(time, channel, data1);
            }
        }
    
        void handleAfterTouch(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->chanPress(time, channel, data1);
            }
        }

        void handleData1(double time, int type, int channel, int data1)
        {
            if (type == MIDI_PROGRAM_CHANGE) {
                handleProgChange(time, channel, data1);
            } else if (type == MIDI_AFTERTOUCH) {
                handleAfterTouch(time, channel, data1);
            }
        }
    
        // MIDI 2 datas
        void handleKeyOff(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyOff(time, channel, data1, data2);
            }
        }
        
        void handleKeyOn(double time, int channel, int data1, int data2)
        {
            if (data2 == 0) {
                handleKeyOff(time, channel, data1, data2);
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->keyOn(time, channel, data1, data2);
                }
            }
        }
    
        void handleCtrlChange(double time, int channel, int data1, int data2)
        {
            // Special processing for NRPN and RPN
            if (fNRPN.process(data1, data2)) {
                if (fNRPN.hasNewNRPN()) {
                    for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                        fMidiInputs[i]->rpn(time, channel, fNRPN.getCtrl(), fNRPN.getVal());
                    }
                }
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->ctrlChange(time, channel, data1, data2);
                }
            }
        }
        
        void handlePitchWheel(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->pitchWheel(time, channel, (data2 << 7) + data1);
            }
        }
        
        void handlePolyAfterTouch(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyPress(time, channel, data1, data2);
            }
        }
  
        void handleData2(double time, int type, int channel, int data1, int data2)
        {
            if (type == MIDI_NOTE_OFF) {
                handleKeyOff(time, channel,  data1, data2);
            } else if (type == MIDI_NOTE_ON) {
                handleKeyOn(time, channel, data1, data2);
            } else if (type == MIDI_CONTROL_CHANGE) {
                handleCtrlChange(time, channel, data1, data2);
            } else if (type == MIDI_PITCH_BEND) {
                handlePitchWheel(time, channel, data1, data2);
            } else if (type == MIDI_POLY_AFTERTOUCH) {
                handlePolyAfterTouch(time, channel, data1, data2);
            }
        }
    
        // SysEx
        void handleSysex(double time, std::vector<unsigned char>& message)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->sysEx(time, message);
            }
        }
    
        void handleMessage(double time, int type, std::vector<unsigned char>& message)
        {
            if (type == MIDI_SYSEX_START) {
                handleSysex(time, message);
            }
        }
  
};

//-------------------------------
// For timestamped MIDI messages
//-------------------------------

struct DatedMessage {
    
    double fDate;
    unsigned char fBuffer[3];
    size_t fSize;
    
    DatedMessage(double date, unsigned char* buffer, size_t size)
    :fDate(date), fSize(size)
    {
        assert(size <= 3);
        memcpy(fBuffer, buffer, size);
    }
    
    DatedMessage():fDate(0.0), fSize(0)
    {}
    
};

#endif // __midi__
/**************************  END  midi.h **************************/

#ifdef _MSC_VER
#define gsscanf sscanf_s
#else
#define gsscanf sscanf
#endif

/*****************************************************************************
* Helper code for MIDI meta and polyphonic 'nvoices' parsing
******************************************************************************/

struct MidiMeta : public Meta, public std::map<std::string, std::string> {
    
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
    
    const std::string get(const char* key, const char* def)
    {
        return (this->find(key) != this->end()) ? (*this)[key] : def;
    }
    
    static void analyse(dsp* mono_dsp, bool& midi_sync, int& nvoices)
    {
        JSONUI jsonui;
        mono_dsp->buildUserInterface(&jsonui);
        std::string json = jsonui.JSON();
        midi_sync = ((json.find("midi") != std::string::npos) &&
                     ((json.find("start") != std::string::npos) ||
                      (json.find("stop") != std::string::npos) ||
                      (json.find("clock") != std::string::npos) ||
                      (json.find("timestamp") != std::string::npos)));
    
    #if defined(NVOICES) && NVOICES!=NUM_VOICES
        nvoices = NVOICES;
    #else
        MidiMeta meta;
        mono_dsp->metadata(&meta);
        bool found_voices = false;
        // If "options" metadata is used
        std::string options = meta.get("options", "");
        if (options != "") {
            std::map<std::string, std::string> metadata;
            std::string res;
            MetaDataUI::extractMetadata(options, res, metadata);
            if (metadata.find("nvoices") != metadata.end()) {
                nvoices = std::atoi(metadata["nvoices"].c_str());
                found_voices = true;
            }
        }
        // Otherwise test for "nvoices" metadata
        if (!found_voices) {
            std::string numVoices = meta.get("nvoices", "0");
            nvoices = std::atoi(numVoices.c_str());
        }
        nvoices = std::max<int>(0, nvoices);
    #endif
    }
    
    static bool checkPolyphony(dsp* mono_dsp)
    {
        MapUI map_ui;
        mono_dsp->buildUserInterface(&map_ui);
        bool has_freq = false;
        bool has_gate = false;
        bool has_gain = false;
        for (int i = 0; i < map_ui.getParamsCount(); i++) {
            std::string path = map_ui.getParamAddress(i);
            has_freq |= MapUI::endsWith(path, "/freq");
            has_gate |= MapUI::endsWith(path, "/gate");
            has_gain |= MapUI::endsWith(path, "/gain");
        }
        return (has_freq && has_gate && has_gain);
    }
    
};

/*******************************************************************************
 * MidiUI : Faust User Interface
 * This class decodes MIDI meta data and maps incoming MIDI messages to them.
 * Currently ctrl, keyon/keyoff, keypress, pgm, chanpress, pitchwheel/pitchbend
 * start/stop/clock meta data is handled.
 ******************************************************************************/

class uiMidi {
    
    friend class MidiUI;
    
    protected:
        
        midi* fMidiOut;
        bool fInputCtrl;
        int fChan;
    
        int rangeChan() { return (((fChan < 0) || (fChan > 15)) ? 0 : fChan); }
    
    public:
        
        uiMidi(midi* midi_out, bool input, int chan = -1):fMidiOut(midi_out), fInputCtrl(input), fChan(chan)
        {}
        virtual ~uiMidi()
        {}

};

/*****************************************************************************
 * Base class for MIDI aware UI items
 ******************************************************************************/

class uiMidiItem : public uiMidi, public uiItem {
    
    public:
        
        uiMidiItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidi(midi_out, input, chan), uiItem(ui, zone)
        {}
        virtual ~uiMidiItem()
        {}
    
        virtual void reflectZone() {}
    
};

/*****************************************************************************
 * Base class for MIDI aware UI items with timestamp support
 ******************************************************************************/

class uiMidiTimedItem : public uiMidi, public uiTimedItem {
    
    public:
        
        uiMidiTimedItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidi(midi_out, input, chan), uiTimedItem(ui, zone)
        {}
        virtual ~uiMidiTimedItem()
        {}
    
        virtual void reflectZone() {}
    
};

//-----------
// MIDI sync
//-----------

class uiMidiStart : public uiMidiTimedItem
{
  
    public:
    
        uiMidiStart(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStart()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->startSync(0);
            }
        }
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
        
};

class uiMidiStop : public uiMidiTimedItem {
  
    public:
    
        uiMidiStop(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStop()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(1)) {
                fMidiOut->stopSync(0);
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
};

class uiMidiClock : public uiMidiTimedItem {

    private:
        
        bool fState;
  
    public:
    
        uiMidiClock(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input), fState(false)
        {}
        virtual ~uiMidiClock()
        {}
    
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->clock(0);
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                fState = !fState;
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fState));
            }
        }

};

//----------------------
// Standard MIDI events
//----------------------

class uiMidiProgChange : public uiMidiTimedItem {
    
    private:
        
        int fPgm;
  
    public:
    
        uiMidiProgChange(midi* midi_out, int pgm, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fPgm(pgm)
        {}
        virtual ~uiMidiProgChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->progChange(rangeChan(), fPgm);
            }
        }
        
};

class uiMidiChanPress : public uiMidiTimedItem {
    
    private:
        
        int fPress;
  
    public:
    
        uiMidiChanPress(midi* midi_out, int press, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fPress(press)
        {}
        virtual ~uiMidiChanPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->chanPress(rangeChan(), fPress);
            }
        }
        
};

class uiMidiCtrlChange : public uiMidiTimedItem {
    
    private:
    
        int fCtrl;
        LinearValueConverter fConverter;
 
    public:

        uiMidiCtrlChange(midi* midi_out, int ctrl, GUI* ui, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fCtrl(ctrl), fConverter(0., 127., double(min), double(max))
        {}
        virtual ~uiMidiCtrlChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->ctrlChange(rangeChan(), fCtrl, fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
};

class uiMidiPitchWheel : public uiMidiTimedItem {

    private:
    
        LinearValueConverter2 fConverter;
    
    public:
    
        uiMidiPitchWheel(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true, int chan = -1)
        :uiMidiTimedItem(midi_out, ui, zone, input, chan)
        {
            if (min <= 0 && max >= 0) {
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min), 0., double(max));
            } else {
                // Degenerated case...
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min),double(min + (max - min)/FAUSTFLOAT(2)), double(max));
            }
        }
    
        virtual ~uiMidiPitchWheel()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->pitchWheel(rangeChan(), fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void setRange(int val)
        {
            double semi = (val / 128) + ((val % 128) / 100.);
            fConverter.setMappingValues(0., 8191., 16383., -semi, 0., semi);
        }
 
};

class uiMidiKeyOn : public uiMidiTimedItem {

    private:
        
        int fKeyOn;
        LinearValueConverter fConverter;
  
    public:
    
        uiMidiKeyOn(midi* midi_out, int key, GUI* ui, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fKeyOn(key), fConverter(0., 127., double(min), double(max))
        {}
        virtual ~uiMidiKeyOn()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyOn(rangeChan(), fKeyOn, fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
};

class uiMidiKeyOff : public uiMidiTimedItem {

    private:
        
        int fKeyOff;
        LinearValueConverter fConverter;
  
    public:
    
        uiMidiKeyOff(midi* midi_out, int key, GUI* ui, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fKeyOff(key), fConverter(0., 127., double(min), double(max))
        {}
        virtual ~uiMidiKeyOff()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyOff(rangeChan(), fKeyOff, fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
};

class uiMidiKeyPress : public uiMidiTimedItem {

    private:
    
        int fKey;
        LinearValueConverter fConverter;
  
    public:
    
        uiMidiKeyPress(midi* midi_out, int key, GUI* ui, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fKey(key), fConverter(0., 127., double(min), double(max))
        {}
        virtual ~uiMidiKeyPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyPress(rangeChan(), fKey, fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
};

/******************************************************************************************
 * MidiUI : Faust User Interface
 * This class decodes MIDI metadata and maps incoming MIDI messages to them.
 * Currently ctrl, keyon/keyoff, keypress, pgm, chanpress, pitchwheel/pitchbend
 * start/stop/clock meta data are handled.
 *
 * Maps associating MIDI event ID (like each ctrl number) with all MIDI aware UI items
 * are defined and progressively filled when decoding MIDI related metadata.
 * MIDI aware UI items are used in both directions:
 *  - modifying their internal state when receving MIDI input events
 *  - sending their internal state as MIDI output events
 *******************************************************************************************/

class MidiUI : public GUI, public midi {

    typedef std::map <int, std::vector<uiMidiCtrlChange*> > TCtrlChangeTable;
    typedef std::map <int, std::vector<uiMidiProgChange*> > TProgChangeTable;
    typedef std::map <int, std::vector<uiMidiChanPress*> >  TChanPressTable;
    typedef std::map <int, std::vector<uiMidiKeyOn*> >      TKeyOnTable;
    typedef std::map <int, std::vector<uiMidiKeyOff*> >     TKeyOffTable;
    typedef std::map <int, std::vector<uiMidiKeyPress*> >   TKeyPressTable;
    typedef std::vector<uiMidiPitchWheel*>                  TPitchWheelTable;
    
    protected:
    
        TCtrlChangeTable fCtrlChangeTable;
        TProgChangeTable fProgChangeTable;
        TChanPressTable  fChanPressTable;
        TKeyOnTable      fKeyOnTable;
        TKeyOffTable     fKeyOffTable;
        TKeyOnTable      fKeyTable;
        TKeyPressTable   fKeyPressTable;
        TPitchWheelTable fPitchWheelTable;
        
        std::vector<uiMidiStart*> fStartTable;
        std::vector<uiMidiStop*>  fStopTable;
        std::vector<uiMidiClock*> fClockTable;
        
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        
        midi_handler* fMidiHandler;
        bool fDelete;
        bool fTimeStamp;
    
        void addGenericZone(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true)
        {
            if (fMetaAux.size() > 0) {
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    unsigned num;
                    unsigned chan;
                    if (fMetaAux[i].first == "midi") {
                        if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u %u", &num, &chan) == 2) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u", &num) == 1) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u %u", &num, &chan) == 2) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u", &num) == 1) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u %u", &num, &chan) == 2) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u", &num) == 1) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u %u", &num, &chan) == 2) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u", &num) == 1) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u %u", &num, &chan) == 2) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u", &num) == 1) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "pgm %u %u", &num, &chan) == 2) {
                            fProgChangeTable[num].push_back(new uiMidiProgChange(fMidiHandler, num, this, zone, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "pgm %u", &num) == 1) {
                            fProgChangeTable[num].push_back(new uiMidiProgChange(fMidiHandler, num, this, zone, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "chanpress %u %u", &num, &chan) == 2) {
                            fChanPressTable[num].push_back(new uiMidiChanPress(fMidiHandler, num, this, zone, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "chanpress %u", &num) == 1) {
                            fChanPressTable[num].push_back(new uiMidiChanPress(fMidiHandler, num, this, zone, input));
                        } else if ((gsscanf(fMetaAux[i].second.c_str(), "pitchwheel %u", &chan) == 1) || (gsscanf(fMetaAux[i].second.c_str(), "pitchbend %u", &chan) == 1)) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input, chan));
                        } else if ((fMetaAux[i].second == "pitchwheel") || (fMetaAux[i].second == "pitchbend")) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input));
                        // MIDI sync
                        } else if (fMetaAux[i].second == "start") {
                            fStartTable.push_back(new uiMidiStart(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "stop") {
                            fStopTable.push_back(new uiMidiStop(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "clock") {
                            fClockTable.push_back(new uiMidiClock(fMidiHandler, this, zone, input));
                        // Explicit metadata to activate 'timestamp' mode
                        } else if (fMetaAux[i].second == "timestamp") {
                            fTimeStamp = true;
                        }
                    }
                }
            }
            fMetaAux.clear();
        }
    
        template <typename TABLE>
        void updateTable1(TABLE& table, double date, int channel, int val1)
        {
            for (size_t i = 0; i < table.size(); i++) {
                int channel_aux = table[i]->fChan;
                if (channel_aux == -1 || channel == channel_aux) {
                    if (fTimeStamp) {
                        table[i]->modifyZone(date, FAUSTFLOAT(val1));
                    } else {
                        table[i]->modifyZone(FAUSTFLOAT(val1));
                    }
                }
            }
        }
        
        template <typename TABLE>
        void updateTable2(TABLE& table, double date, int channel, int val1, int val2)
        {
            if (table.find(val1) != table.end()) {
                for (size_t i = 0; i < table[val1].size(); i++) {
                    int channel_aux = table[val1][i]->fChan;
                    if (channel_aux == -1 || channel == channel_aux) {
                        if (fTimeStamp) {
                            table[val1][i]->modifyZone(date, FAUSTFLOAT(val2));
                        } else {
                            table[val1][i]->modifyZone(FAUSTFLOAT(val2));
                        }
                    }
                }
            }
        }
    
    public:
    
        MidiUI(midi_handler* midi_handler, bool delete_handler = false)
        {
            fMidiHandler = midi_handler;
            fMidiHandler->addMidiIn(this);
            fDelete = delete_handler;
            fTimeStamp = false;
        }
 
        virtual ~MidiUI() 
        { 
            fMidiHandler->removeMidiIn(this);
            if (fDelete) delete fMidiHandler;
        }
        
        bool run() { return fMidiHandler->startMidi(); }
        void stop() { fMidiHandler->stopMidi(); }
        
        void addMidiIn(midi* midi_dsp) { fMidiHandler->addMidiIn(midi_dsp); }
        void removeMidiIn(midi* midi_dsp) { fMidiHandler->removeMidiIn(midi_dsp); }
      
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
        {
            addGenericZone(zone, min, max, false);
        }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addGenericZone(zone, min, max, false);
        }

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {
            fMetaAux.push_back(std::make_pair(key, val));
        }
        
        // -- MIDI API
    
        void key(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyTable, date, channel, note, velocity);
        }
    
        MapUI* keyOn(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyOnTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOn
            key(date, channel, note, velocity);
            return nullptr;
        }
        
        void keyOff(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOffTable>(fKeyOffTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOff with a 0 velocity
            key(date, channel, note, 0);
        }
        
        void ctrlChange(double date, int channel, int ctrl, int value)
        {
            updateTable2<TCtrlChangeTable>(fCtrlChangeTable, date, channel, ctrl, value);
        }
    
        void rpn(double date, int channel, int ctrl, int value)
        {
            if (ctrl == midi::PITCH_BEND_RANGE) {
                for (size_t i = 0; i < fPitchWheelTable.size(); i++) {
                    int channel_aux = fPitchWheelTable[i]->fChan;
                    if (channel_aux == -1 || channel == channel_aux) {
                        fPitchWheelTable[i]->setRange(value);
                    }
                }
            }
        }
    
        void progChange(double date, int channel, int pgm)
        {
            updateTable2<TProgChangeTable>(fProgChangeTable, date, channel, pgm, FAUSTFLOAT(1));
        }
        
        void pitchWheel(double date, int channel, int wheel) 
        {
            updateTable1<TPitchWheelTable>(fPitchWheelTable, date, channel, wheel);
        }
        
        void keyPress(double date, int channel, int pitch, int press) 
        {
            updateTable2<TKeyPressTable>(fKeyPressTable, date, channel, pitch, press);
        }
        
        void chanPress(double date, int channel, int press)
        {
            updateTable2<TChanPressTable>(fChanPressTable, date, channel, press, FAUSTFLOAT(1));
        }
        
        void ctrlChange14bits(double date, int channel, int ctrl, int value) {}
        
        // MIDI sync
        
        void startSync(double date)
        {
            for (size_t i = 0; i < fStartTable.size(); i++) {
                fStartTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
        
        void stopSync(double date)
        {
            for (size_t i = 0; i < fStopTable.size(); i++) {
                fStopTable[i]->modifyZone(date, FAUSTFLOAT(0));
            }
        }
        
        void clock(double date)
        {
            for (size_t i = 0; i < fClockTable.size(); i++) {
                fClockTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
};

#endif // FAUST_MIDIUI_H
/**************************  END  MidiUI.h **************************/
/************************** BEGIN poly-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __poly_dsp__
#define __poly_dsp__

#include <stdio.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <float.h>
#include <assert.h>

/************************** BEGIN dsp-combiner.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_combiner__
#define __dsp_combiner__

#include <string.h>
#include <string>
#include <assert.h>
#include <sstream>


// Base class and common code for binary combiners

class dsp_binary_combiner : public dsp {

    protected:

        dsp* fDSP1;
        dsp* fDSP2;

        void buildUserInterfaceAux(UI* ui_interface, const char* name)
        {
            ui_interface->openTabBox(name);
            ui_interface->openVerticalBox("DSP1");
            fDSP1->buildUserInterface(ui_interface);
            ui_interface->closeBox();
            ui_interface->openVerticalBox("DSP2");
            fDSP2->buildUserInterface(ui_interface);
            ui_interface->closeBox();
            ui_interface->closeBox();
        }

        FAUSTFLOAT** allocateChannels(int num, int buffer_size)
        {
            FAUSTFLOAT** channels = new FAUSTFLOAT*[num];
            for (int chan = 0; chan < num; chan++) {
                channels[chan] = new FAUSTFLOAT[buffer_size];
                memset(channels[chan], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
            return channels;
        }

        void deleteChannels(FAUSTFLOAT** channels, int num)
        {
            for (int chan = 0; chan < num; chan++) {
                delete [] channels[chan];
            }
            delete [] channels;
        }

     public:

        dsp_binary_combiner(dsp* dsp1, dsp* dsp2):fDSP1(dsp1), fDSP2(dsp2)
        {}

        virtual ~dsp_binary_combiner()
        {
            delete fDSP1;
            delete fDSP2;
        }

        virtual int getSampleRate()
        {
            return fDSP1->getSampleRate();
        }
        virtual void init(int sample_rate)
        {
            fDSP1->init(sample_rate);
            fDSP2->init(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            fDSP1->instanceInit(sample_rate);
            fDSP2->instanceInit(sample_rate);
        }
        virtual void instanceConstants(int sample_rate)
        {
            fDSP1->instanceConstants(sample_rate);
            fDSP2->instanceConstants(sample_rate);
        }

        virtual void instanceResetUserInterface()
        {
            fDSP1->instanceResetUserInterface();
            fDSP2->instanceResetUserInterface();
        }

        virtual void instanceClear()
        {
            fDSP1->instanceClear();
            fDSP2->instanceClear();
        }

        virtual void metadata(Meta* m)
        {
            fDSP1->metadata(m);
            fDSP2->metadata(m);
        }

};

// Combine two 'compatible' DSP in sequence

class dsp_sequencer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;

    public:

        dsp_sequencer(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
        }

        virtual ~dsp_sequencer()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Sequencer");
        }

        virtual dsp* clone()
        {
            return new dsp_sequencer(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);
            fDSP2->compute(count, fDSP1Outputs, outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

// Combine two DSP in parallel

class dsp_parallelizer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_parallelizer(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
            fDSP2Outputs = new FAUSTFLOAT*[fDSP2->getNumOutputs()];
        }

        virtual ~dsp_parallelizer()
        {
            delete [] fDSP2Inputs;
            delete [] fDSP2Outputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() + fDSP2->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs() + fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Parallelizer");
        }

        virtual dsp* clone()
        {
            return new dsp_parallelizer(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, outputs);

            // Shift inputs/outputs channels for fDSP2
            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                fDSP2Inputs[chan] = inputs[fDSP1->getNumInputs() + chan];
            }
            for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                fDSP2Outputs[chan] = outputs[fDSP1->getNumOutputs() + chan];
            }

            fDSP2->compute(count, fDSP2Inputs, fDSP2Outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

// Combine two 'compatible' DSP in splitter

class dsp_splitter : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

    public:

        dsp_splitter(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_splitter()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Splitter");
        }

        virtual dsp* clone()
        {
            return new dsp_splitter(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);

            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                 fDSP2Inputs[chan] = fDSP1Outputs[chan % fDSP1->getNumOutputs()];
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

// Combine two 'compatible' DSP in merger

class dsp_merger : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

        void mix(int count, FAUSTFLOAT* dst, FAUSTFLOAT* src)
        {
            for (int frame = 0; frame < count; frame++) {
                dst[frame] += src[frame];
            }
        }

    public:

        dsp_merger(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs(), buffer_size);
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_merger()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Merge");
        }

        virtual dsp* clone()
        {
            return new dsp_merger(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, fDSP1Inputs, fDSP1Outputs);

            memset(fDSP2Inputs, 0, sizeof(FAUSTFLOAT*) * fDSP2->getNumInputs());

            for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                int mchan = chan % fDSP2->getNumInputs();
                if (fDSP2Inputs[mchan]) {
                    mix(count, fDSP2Inputs[mchan], fDSP1Outputs[chan]);
                } else {
                    fDSP2Inputs[mchan] = fDSP1Outputs[chan];
                }
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

// Combine two 'compatible' DSP in a recursive way

class dsp_recursiver : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_recursiver(dsp* dsp1, dsp* dsp2):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs(), 1);
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), 1);
            fDSP2Inputs = allocateChannels(fDSP2->getNumInputs(), 1);
            fDSP2Outputs = allocateChannels(fDSP2->getNumOutputs(), 1);
        }

        virtual ~dsp_recursiver()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            deleteChannels(fDSP2Inputs, fDSP2->getNumInputs());
            deleteChannels(fDSP2Outputs, fDSP2->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() - fDSP2->getNumOutputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Recursiver");
        }

        virtual dsp* clone()
        {
            return new dsp_recursiver(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int frame = 0; (frame < count); frame++) {

                for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan][0] = fDSP2Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP1->getNumInputs() - fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan + fDSP2->getNumOutputs()][0] = inputs[chan][frame];
                }

                fDSP1->compute(1, fDSP1Inputs, fDSP1Outputs);

                for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                    outputs[chan][frame] = fDSP1Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                    fDSP2Inputs[chan][0] = fDSP1Outputs[chan][0];
                }

                fDSP2->compute(1, fDSP2Inputs, fDSP2Outputs);
            }
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

#ifndef __dsp_algebra_api__
#define __dsp_algebra_api__
// DSP algebra API
/*
 Each operation takes two DSP as parameters, returns the combined DSPs, or null if failure with an error message.
 */

static dsp* createDSPSequencer(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() != dsp2->getNumInputs()) {
        std::stringstream error_aux;
        error_aux << "Connection error int dsp_sequencer : the number of outputs ("
                  << dsp1->getNumOutputs() << ") of A "
                  << "must be equal to the number of inputs (" << dsp2->getNumInputs() << ") of B" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_sequencer(dsp1, dsp2);
    }
}

static dsp* createDSPParallelizer(dsp* dsp1, dsp* dsp2, std::string& error)
{
    return new dsp_parallelizer(dsp1, dsp2);
}

static dsp* createDSPSplitter(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_splitter : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_splitter : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() % dsp1->getNumOutputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_splitter : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a divisor of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2);
    } else {
        return new dsp_splitter(dsp1, dsp2);
    }
}

static dsp* createDSPMerger(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_merger : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_merger : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp1->getNumOutputs() % dsp2->getNumInputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_merger : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a multiple of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2);
    } else {
        return new dsp_merger(dsp1, dsp2);
    }
}

static dsp* createDSPRecursiver(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if ((dsp2->getNumInputs() > dsp1->getNumOutputs()) || (dsp2->getNumOutputs() > dsp1->getNumInputs())) {
        std::stringstream error_aux;
        error_aux << "Connection error in : dsp_recursiver" << std::endl;
        if (dsp2->getNumInputs() > dsp1->getNumOutputs()) {
            error_aux << "The number of outputs " << dsp1->getNumOutputs()
                      << " of the first expression should be greater or equal to the number of inputs ("
                      << dsp2->getNumInputs()
                      << ") of the second expression" << std::endl;
        }
        if (dsp2->getNumOutputs() > dsp1->getNumInputs()) {
            error_aux << "The number of inputs " << dsp1->getNumInputs()
                      << " of the first expression should be greater or equal to the number of outputs ("
                      << dsp2->getNumOutputs()
                      << ") of the second expression" << std::endl;
        }
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_recursiver(dsp1, dsp2);
    }
}
#endif

#endif
/**************************  END  dsp-combiner.h **************************/
/************************** BEGIN proxy-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __proxy_dsp__
#define __proxy_dsp__

#include <vector>
#include <map>


//----------------------------------------------------------------
//  Proxy dsp definition created from the DSP JSON description
//  This class allows a 'proxy' dsp to control a real dsp 
//  possibly running somewhere else.
//----------------------------------------------------------------

class proxy_dsp : public dsp {

    private:
    
        JSONUIDecoder* fDecoder;
        int fSampleRate;
        
    public:
    
        proxy_dsp():fDecoder(nullptr), fSampleRate(-1)
        {}
    
        proxy_dsp(const std::string& json)
        {
            init(json);
        }
    
        void init(const std::string& json)
        {
            fDecoder = new JSONUIDecoder(json);
            fSampleRate = -1;
        }
          
        proxy_dsp(dsp* dsp)
        {
            JSONUI builder(dsp->getNumInputs(), dsp->getNumOutputs());
            dsp->metadata(&builder);
            dsp->buildUserInterface(&builder);
            fSampleRate = dsp->getSampleRate();
            fDecoder = new JSONUIDecoder(builder.JSON());
        }
      
        virtual ~proxy_dsp()
        {
            delete fDecoder;
        }
       
        virtual int getNumInputs() { return fDecoder->fNumInputs; }
        virtual int getNumOutputs() { return fDecoder->fNumOutputs; }
        
        virtual void buildUserInterface(UI* ui) { fDecoder->buildUserInterface(ui); }
        
        // To possibly implement in a concrete proxy dsp 
        virtual void init(int sample_rate)
        {
            instanceInit(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            instanceConstants(sample_rate);
            instanceResetUserInterface();
            instanceClear();
        }
        virtual void instanceConstants(int sample_rate) { fSampleRate = sample_rate; }
        virtual void instanceResetUserInterface() { fDecoder->resetUserInterface(); }
        virtual void instanceClear() {}
    
        virtual int getSampleRate() { return fSampleRate; }
    
        virtual proxy_dsp* clone() { return new proxy_dsp(fDecoder->fJSON); }
        virtual void metadata(Meta* m) { fDecoder->metadata(m); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {}
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {} 
        
};

#endif
/**************************  END  proxy-dsp.h **************************/
/************************** BEGIN JSONControl.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JSON_CONTROL__
#define __JSON_CONTROL__

#include <string>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct JSONControl {
    
    virtual std::string getJSON() { return ""; }

    virtual void setParamValue(const std::string& path, FAUSTFLOAT value) {}

    virtual FAUSTFLOAT getParamValue(const std::string& path) { return 0; }
    
    virtual ~JSONControl()
    {}
    
};

#endif
/**************************  END  JSONControl.h **************************/

#define kActiveVoice      0
#define kFreeVoice        -1
#define kReleaseVoice     -2
#define kNoVoice          -3

#define VOICE_STOP_LEVEL  0.0005    // -70 db
#define MIX_BUFFER_SIZE   4096

// endsWith(<str>,<end>) : returns true if <str> ends with <end>

static double midiToFreq(double note)
{
    return 440.0 * std::pow(2.0, (note-69.0)/12.0);
}

/**
 * Allows to control zones in a grouped manner.
 */

class GroupUI : public GUI, public PathBuilder
{

    private:

        std::map<std::string, uiGroupItem*> fLabelZoneMap;

        void insertMap(std::string label, FAUSTFLOAT* zone)
        {
            if (!MapUI::endsWith(label, "/gate")
                && !MapUI::endsWith(label, "/freq")
                && !MapUI::endsWith(label, "/gain")) {

                // Groups all controller except 'freq', 'gate', and 'gain'
                if (fLabelZoneMap.find(label) != fLabelZoneMap.end()) {
                    fLabelZoneMap[label]->addZone(zone);
                } else {
                    fLabelZoneMap[label] = new uiGroupItem(this, zone);
                }
            }
        }

        uiCallbackItem* fPanic;

    public:

        GroupUI(FAUSTFLOAT* zone, uiCallback cb, void* arg)
        {
            fPanic = new uiCallbackItem(this, zone, cb, arg);
        }
    
        virtual ~GroupUI()
        {
            // 'fPanic' is kept and deleted in GUI, so do not delete here
        }

        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }

        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }

        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }

};

/**
 * One voice of polyphony.
 */

struct dsp_voice : public MapUI, public decorator_dsp {

    int fNote;                          // Playing note actual pitch
    int fDate;                          // KeyOn date
    int fRelease;                       // Current number of samples used in release mode to detect end of note
    int fMinRelease;                    // Max of samples used in release mode to detect end of note
    FAUSTFLOAT fLevel;                  // Last audio block level
    std::vector<std::string> fGatePath; // Paths of 'gate' control
    std::vector<std::string> fGainPath; // Paths of 'gain' control
    std::vector<std::string> fFreqPath; // Paths of 'freq' control
 
    dsp_voice(dsp* dsp):decorator_dsp(dsp)
    {
        dsp->buildUserInterface(this);
        fNote = kFreeVoice;
        fLevel = FAUSTFLOAT(0);
        fDate = 0;
        fMinRelease = dsp->getSampleRate()/2; // One 1/2 sec used in release mode to detect end of note
        extractPaths(fGatePath, fFreqPath, fGainPath);
    }
    virtual ~dsp_voice()
    {}

    void extractPaths(std::vector<std::string>& gate, std::vector<std::string>& freq, std::vector<std::string>& gain)
    {
        // Keep gain, freq and gate labels
        std::map<std::string, FAUSTFLOAT*>::iterator it;
        for (it = getMap().begin(); it != getMap().end(); it++) {
            std::string path = (*it).first;
            if (endsWith(path, "/gate")) {
                gate.push_back(path);
            } else if (endsWith(path, "/freq")) {
                freq.push_back(path);
            } else if (endsWith(path, "/gain")) {
                gain.push_back(path);
            }
        }
    }

    // MIDI velocity [0..127]
    void keyOn(int pitch, int velocity, bool trigger)
    {
        keyOn(pitch, float(velocity)/127.f, trigger);
    }

    // Normalized MIDI velocity [0..1]
    void keyOn(int pitch, float velocity, bool trigger)
    {
        // So that DSP state is always re-initialized
        fDSP->instanceClear();
        
        for (size_t i = 0; i < fFreqPath.size(); i++) {
            setParamValue(fFreqPath[i], midiToFreq(pitch));
        }
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(1));
        }
        for (size_t i = 0; i < fGainPath.size(); i++) {
            setParamValue(fGainPath[i], velocity);
        }
        
        fNote = pitch;
    }

    void keyOff(bool hard = false)
    {
        // No use of velocity for now...
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(0));
        }
        
        if (hard) {
            // Immediately stop voice
            fNote = kFreeVoice;
        } else {
            // Release voice
            fRelease = fMinRelease;
            fNote = kReleaseVoice;
        }
    }

};

/**
 * A group of voices.
 */

struct dsp_voice_group {

    GroupUI fGroups;

    std::vector<dsp_voice*> fVoiceTable; // Individual voices
    dsp* fVoiceGroup;                    // Voices group to be used for GUI grouped control

    FAUSTFLOAT fPanic;

    bool fVoiceControl;
    bool fGroupControl;

    dsp_voice_group(uiCallback cb, void* arg, bool control, bool group)
        :fGroups(&fPanic, cb, arg),
        fVoiceGroup(0), fPanic(FAUSTFLOAT(0)),
        fVoiceControl(control), fGroupControl(group)
    {}

    virtual ~dsp_voice_group()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            delete fVoiceTable[i];
        }
        delete fVoiceGroup;
    }

    void addVoice(dsp_voice* voice)
    {
        fVoiceTable.push_back(voice);
    }

    void clearVoices()
    {
        fVoiceTable.clear();
    }

    void init()
    {
        // Groups all uiItem for a given path
        fVoiceGroup = new proxy_dsp(fVoiceTable[0]);
        fVoiceGroup->buildUserInterface(&fGroups);
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->buildUserInterface(&fGroups);
        }
    }
    
    void instanceResetUserInterface()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->instanceResetUserInterface();
        }
    }

    void buildUserInterface(UI* ui_interface)
    {
        if (fVoiceTable.size() > 1) {
            ui_interface->openTabBox("Polyphonic");

            // Grouped voices UI
            ui_interface->openVerticalBox("Voices");
            ui_interface->addButton("Panic", &fPanic);
            fVoiceGroup->buildUserInterface(ui_interface);
            ui_interface->closeBox();

            // If not grouped, also add individual voices UI
            if (!fGroupControl) {
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    char buffer[32];
                    snprintf(buffer, 32, ((fVoiceTable.size() < 8) ? "Voice%ld" : "V%ld"), long(i+1));
                    ui_interface->openHorizontalBox(buffer);
                    fVoiceTable[i]->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                }
            }

            ui_interface->closeBox();
        } else {
            fVoiceTable[0]->buildUserInterface(ui_interface);
        }
    }

};

/**
 * Base class for MIDI controllable DSP.
 */

#ifdef EMCC
#endif

class dsp_poly : public decorator_dsp, public midi, public JSONControl {

    protected:
    
    #ifdef EMCC
        MapUI fMapUI;
        std::string fJSON;
        midi_handler fMIDIHandler;
        MidiUI fMIDIUI;
    #endif
    
    public:
    
    #ifdef EMCC
        dsp_poly(dsp* dsp):decorator_dsp(dsp), fMIDIUI(&fMIDIHandler)
        {
            JSONUI jsonui(getNumInputs(), getNumOutputs());
            buildUserInterface(&jsonui);
            fJSON = jsonui.JSON(true);
            buildUserInterface(&fMapUI);
            buildUserInterface(&fMIDIUI);
        }
    #else
        dsp_poly(dsp* dsp):decorator_dsp(dsp)
        {}
    #endif
    
        virtual ~dsp_poly() {}
    
        // Reimplemented for EMCC
    #ifdef EMCC
        virtual int getNumInputs() { return decorator_dsp::getNumInputs(); }
        virtual int getNumOutputs() { return decorator_dsp::getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { decorator_dsp::buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return decorator_dsp::getSampleRate(); }
        virtual void init(int sample_rate) { decorator_dsp::init(sample_rate); }
        virtual void instanceInit(int sample_rate) { decorator_dsp::instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { decorator_dsp::instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { decorator_dsp::instanceResetUserInterface(); }
        virtual void instanceClear() { decorator_dsp::instanceClear(); }
        virtual dsp_poly* clone() { return new dsp_poly(fDSP->clone()); }
        virtual void metadata(Meta* m) { decorator_dsp::metadata(m); }
    
        // Additional API
        std::string getJSON()
        {
            return fJSON;
        }
    
        virtual void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            fMapUI.setParamValue(path, value);
            GUI::updateAllGuis();
        }
        
        virtual FAUSTFLOAT getParamValue(const std::string& path) { return fMapUI.getParamValue(path); }

        virtual void computeJS(int count, uintptr_t inputs, uintptr_t outputs)
        {
            decorator_dsp::compute(count, reinterpret_cast<FAUSTFLOAT**>(inputs),reinterpret_cast<FAUSTFLOAT**>(outputs));
        }
    #endif
    
        virtual MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return midi::keyOn(channel, pitch, velocity);
        }
        virtual void keyOff(int channel, int pitch, int velocity)
        {
            midi::keyOff(channel, pitch, velocity);
        }
        virtual void keyPress(int channel, int pitch, int press)
        {
            midi::keyPress(channel, pitch, press);
        }
        virtual void chanPress(int channel, int press)
        {
            midi::chanPress(channel, press);
        }
        virtual void ctrlChange(int channel, int ctrl, int value)
        {
            midi::ctrlChange(channel, ctrl, value);
        }
        virtual void ctrlChange14bits(int channel, int ctrl, int value)
        {
            midi::ctrlChange14bits(channel, ctrl, value);
        }
        virtual void pitchWheel(int channel, int wheel)
        {
        #ifdef EMCC
            fMIDIUI.pitchWheel(0., channel, wheel);
            GUI::updateAllGuis();
        #else
            midi::pitchWheel(channel, wheel);
        #endif
        }
        virtual void progChange(int channel, int pgm)
        {
            midi::progChange(channel, pgm);
        }
    
        // Group API
        virtual void setGroup(bool group) {}
        virtual bool getGroup() { return false; }

};

/**
 * Polyphonic DSP: groups a set of DSP to be played together or triggered by MIDI.
 *
 * All voices are preallocated by cloning the single DSP voice given at creation time.
 * Dynamic voice allocation is done in 'getFreeVoice'
 */

class mydsp_poly : public dsp_voice_group, public dsp_poly {

    private:

        FAUSTFLOAT** fMixBuffer;
        int fDate;

        FAUSTFLOAT mixCheckVoice(int count, FAUSTFLOAT** outputBuffer, FAUSTFLOAT** mixBuffer)
        {
            FAUSTFLOAT level = 0;
            for (int i = 0; i < getNumOutputs(); i++) {
                FAUSTFLOAT* mixChannel = mixBuffer[i];
                FAUSTFLOAT* outChannel = outputBuffer[i];
                for (int j = 0; j < count; j++) {
                    level = std::max<FAUSTFLOAT>(level, (FAUSTFLOAT)fabs(outChannel[j]));
                    mixChannel[j] += outChannel[j];
                }
            }
            return level;
        }
    
        void mixVoice(int count, FAUSTFLOAT** outputBuffer, FAUSTFLOAT** mixBuffer)
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                FAUSTFLOAT* mixChannel = mixBuffer[i];
                FAUSTFLOAT* outChannel = outputBuffer[i];
                for (int j = 0; j < count; j++) {
                    mixChannel[j] += outChannel[j];
                }
            }
        }

        void clearOutput(int count, FAUSTFLOAT** mixBuffer)
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                memset(mixBuffer[i], 0, count * sizeof(FAUSTFLOAT));
            }
        }
    
        int getPlayingVoice(int pitch)
        {
            int voice_playing = kNoVoice;
            int oldest_date_playing = INT_MAX;
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fNote == pitch) {
                    // Keeps oldest playing voice
                    if (fVoiceTable[i]->fDate < oldest_date_playing) {
                        oldest_date_playing = fVoiceTable[i]->fDate;
                        voice_playing = int(i);
                    }
                }
            }
            
            return voice_playing;
        }
    
        // Always returns a voice
        int getFreeVoice()
        {
            int voice = kNoVoice;
            
            // Looks for the first available voice
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fNote == kFreeVoice) {
                    voice = int(i);
                    goto result;
                }
            }

            {
                // Otherwise steal one
                int voice_release = kNoVoice;
                int voice_playing = kNoVoice;
                
                int oldest_date_release = INT_MAX;
                int oldest_date_playing = INT_MAX;

                // Scan all voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    if (fVoiceTable[i]->fNote == kReleaseVoice) {
                        // Keeps oldest release voice
                        if (fVoiceTable[i]->fDate < oldest_date_release) {
                            oldest_date_release = fVoiceTable[i]->fDate;
                            voice_release = int(i);
                        }
                    } else {
                        // Otherwise keeps oldest playing voice
                        if (fVoiceTable[i]->fDate < oldest_date_playing) {
                            oldest_date_playing = fVoiceTable[i]->fDate;
                            voice_playing = int(i);
                        }
                    }
                }
            
                // Then decide which one to steal
                if (oldest_date_release != INT_MAX) {
                    std::cout << "Steal release voice : voice_date " << fVoiceTable[voice_release]->fDate << " cur_date = " << fDate << " voice = " << voice_release << std::endl;
                    voice = voice_release;
                    goto result;
                } else if (oldest_date_playing != INT_MAX) {
                    std::cout << "Steal playing voice : voice_date " << fVoiceTable[voice_playing]->fDate << " cur_date = " << fDate << " voice = " << voice_playing << std::endl;
                    voice = voice_playing;
                    goto result;
                } else {
                    assert(false);
                    return kNoVoice;
                }
            }
            
        result:
            fVoiceTable[voice]->fDate = fDate++;
            fVoiceTable[voice]->fNote = kActiveVoice;
            return voice;
        }

        static void panic(FAUSTFLOAT val, void* arg)
        {
            if (val == FAUSTFLOAT(1)) {
                static_cast<mydsp_poly*>(arg)->allNotesOff(true);
            }
        }

        bool checkPolyphony()
        {
            if (fVoiceTable.size() > 0) {
                return true;
            } else {
                std::cout << "DSP is not polyphonic...\n";
                return false;
            }
        }

    public:
    
        /**
         * Constructor.
         *
         * @param dsp - the dsp to be used for one voice. Beware: mydsp_poly will use and finally delete the pointer.
         * @param nvoices - number of polyphony voices, should be at least 1
         * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
         *                If false all voices are always running.
         * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
         *                a given control on all voices, assuming GUI::updateAllGuis() is called.
         *                If false, all voices can be individually controlled.
         *                setGroup/getGroup methods can be used to set/get the group state.
         *
         */
        mydsp_poly(dsp* dsp,
                   int nvoices,
                   bool control = false,
                   bool group = true)
        : dsp_voice_group(panic, this, control, group), dsp_poly(dsp) // dsp parameter is deallocated by ~dsp_poly
        {
            fDate = 0;

            // Create voices
            assert(nvoices > 0);
            for (int i = 0; i < nvoices; i++) {
                addVoice(new dsp_voice(dsp->clone()));
            }

            // Init audio output buffers
            fMixBuffer = new FAUSTFLOAT*[getNumOutputs()];
            for (int i = 0; i < getNumOutputs(); i++) {
                fMixBuffer[i] = new FAUSTFLOAT[MIX_BUFFER_SIZE];
            }

            dsp_voice_group::init();
        }

        virtual ~mydsp_poly()
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                delete[] fMixBuffer[i];
            }
            delete[] fMixBuffer;
        }

        // DSP API
    
        void buildUserInterface(UI* ui_interface)
        {
            dsp_voice_group::buildUserInterface(ui_interface);
        }

        void init(int sample_rate)
        {
            decorator_dsp::init(sample_rate);
            fVoiceGroup->init(sample_rate);
            fPanic = FAUSTFLOAT(0);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->init(sample_rate);
            }
        }
    
        void instanceInit(int samplingFreq)
        {
            instanceConstants(samplingFreq);
            instanceResetUserInterface();
            instanceClear();
        }

        void instanceConstants(int sample_rate)
        {
            decorator_dsp::instanceConstants(sample_rate);
            fVoiceGroup->instanceConstants(sample_rate);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceConstants(sample_rate);
            }
        }

        void instanceResetUserInterface()
        {
            decorator_dsp::instanceResetUserInterface();
            fVoiceGroup->instanceResetUserInterface();
            fPanic = FAUSTFLOAT(0);
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceResetUserInterface();
            }
        }

        void instanceClear()
        {
            decorator_dsp::instanceClear();
            fVoiceGroup->instanceClear();
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceClear();
            }
        }

        virtual mydsp_poly* clone()
        {
            return new mydsp_poly(fDSP->clone(), int(fVoiceTable.size()), fVoiceControl, fGroupControl);
        }

        void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            assert(count <= MIX_BUFFER_SIZE);

            // First clear the outputs
            clearOutput(count, outputs);

            if (fVoiceControl) {
                // Mix all playing voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    dsp_voice* voice = fVoiceTable[i];
                    if (voice->fNote != kFreeVoice) {
                        voice->compute(count, inputs, fMixBuffer);
                        // Mix it in result
                        voice->fLevel = mixCheckVoice(count, fMixBuffer, outputs);
                        // Check the level to possibly set the voice in kFreeVoice again
                        voice->fRelease -= count;
                        if ((voice->fNote == kReleaseVoice)
                            && (voice->fRelease < 0)
                            && (voice->fLevel < VOICE_STOP_LEVEL)) {
                            voice->fNote = kFreeVoice;
                        }
                    }
                }
            } else {
                // Mix all voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    fVoiceTable[i]->compute(count, inputs, fMixBuffer);
                    mixVoice(count, fMixBuffer, outputs);
                }
            }
        }

        void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
    
        // Terminate all active voices, gently or immediately (depending of 'hard' value)
        void allNotesOff(bool hard = false)
        {
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->keyOff(hard);
            }
        }

        // Additional polyphonic API
        MapUI* newVoice()
        {
            int voice = getFreeVoice();
            // So that DSP state is always re-initialized
            fVoiceTable[voice]->instanceClear();
            return fVoiceTable[voice];
        }

        void deleteVoice(MapUI* voice)
        {
            std::vector<dsp_voice*>::iterator it = find(fVoiceTable.begin(), fVoiceTable.end(), reinterpret_cast<dsp_voice*>(voice));
            if (it != fVoiceTable.end()) {
                (*it)->keyOff();
            } else {
                std::cout << "Voice not found\n";
            }
        }
    
        // Group API
        void setGroup(bool group) { fGroupControl = group; }
        bool getGroup() { return fGroupControl; }

        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            if (checkPolyphony()) {
                int voice = getFreeVoice();
                fVoiceTable[voice]->keyOn(pitch, velocity, true);
                return fVoiceTable[voice];
            } else {
                return 0;
            }
        }

        void keyOff(int channel, int pitch, int velocity = 127)
        {
            if (checkPolyphony()) {
                int voice = getPlayingVoice(pitch);
                if (voice != kNoVoice) {
                    fVoiceTable[voice]->keyOff();
                } else {
                    std::cout << "Playing pitch = " << pitch << " not found\n";
                }
            }
        }

        void ctrlChange(int channel, int ctrl, int value)
        {
            if (ctrl == ALL_NOTES_OFF || ctrl == ALL_SOUND_OFF) {
                allNotesOff();
            }
        }

};

/**
 * Polyphonic DSP with an integrated effect. fPolyDSP will respond to MIDI messages.
 */
class dsp_poly_effect : public dsp_poly {
    
    private:
        
        dsp_poly* fPolyDSP;
        
    public:
        
        dsp_poly_effect(dsp_poly* dsp1, dsp* dsp2)
        :dsp_poly(dsp2), fPolyDSP(dsp1)
        {}
        
        virtual ~dsp_poly_effect()
        {
            // dsp_poly_effect is also a decorator_dsp, which will free fPolyDSP
        }
        
        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return fPolyDSP->keyOn(channel, pitch, velocity);
        }
        void keyOff(int channel, int pitch, int velocity)
        {
            fPolyDSP->keyOff(channel, pitch, velocity);
        }
        void keyPress(int channel, int pitch, int press)
        {
            fPolyDSP->keyPress(channel, pitch, press);
        }
        void chanPress(int channel, int press)
        {
            fPolyDSP->chanPress(channel, press);
        }
        void ctrlChange(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange(channel, ctrl, value);
        }
        void ctrlChange14bits(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange14bits(channel, ctrl, value);
        }
        void pitchWheel(int channel, int wheel)
        {
            fPolyDSP->pitchWheel(channel, wheel);
        }
        void progChange(int channel, int pgm)
        {
            fPolyDSP->progChange(channel, pgm);
        }
        
        // Group API
        void setGroup(bool group)
        {
            fPolyDSP->setGroup(group);
        }
        bool getGroup()
        {
            return fPolyDSP->getGroup();
        }
};

/**
 * Polyphonic DSP factory class. Helper code to support polyphonic DSP source with an integrated effect.
 */

struct dsp_poly_factory : public dsp_factory {
    
    dsp_factory* fProcessFactory;
    dsp_factory* fEffectFactory;
    
    std::string getEffectCode(const std::string& dsp_content)
    {
        std::stringstream effect_code;
        effect_code << "adapt(1,1) = _; adapt(2,2) = _,_; adapt(1,2) = _ <: _,_; adapt(2,1) = _,_ :> _;";
        effect_code << "adaptor(F,G) = adapt(outputs(F),inputs(G)); dsp_code = environment{ " << dsp_content << " };";
        effect_code << "process = adaptor(dsp_code.process, dsp_code.effect) : dsp_code.effect;";
        return effect_code.str();
    }

    dsp_poly_factory(dsp_factory* process_factory = NULL,
                     dsp_factory* effect_factory = NULL):
    fProcessFactory(process_factory)
    ,fEffectFactory(effect_factory)
    {}
    
    virtual ~dsp_poly_factory()
    {}
    
    virtual std::string getName() { return fProcessFactory->getName(); }
    virtual std::string getSHAKey() { return fProcessFactory->getSHAKey(); }
    virtual std::string getDSPCode() { return fProcessFactory->getDSPCode(); }
    virtual std::string getCompileOptions() { return fProcessFactory->getCompileOptions(); }
    virtual std::vector<std::string> getLibraryList() { return fProcessFactory->getLibraryList(); }
    virtual std::vector<std::string> getIncludePathnames() { return fProcessFactory->getIncludePathnames(); }
    
    virtual void setMemoryManager(dsp_memory_manager* manager)
    {
        fProcessFactory->setMemoryManager(manager);
        if (fEffectFactory) {
            fEffectFactory->setMemoryManager(manager);
        }
    }
    virtual dsp_memory_manager* getMemoryManager() { return fProcessFactory->getMemoryManager(); }
    
    /* Create a new polyphonic DSP instance with global effect, to be deleted with C++ 'delete'
     *
     * @param nvoices - number of polyphony voices, should be at least 1
     * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
     *                If false all voices are always running.
     * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
     *                a given control on all voices, assuming GUI::updateAllGuis() is called.
     *                If false, all voices can be individually controlled.
     */
    dsp_poly* createPolyDSPInstance(int nvoices, bool control, bool group)
    {
        dsp_poly* dsp_poly = new mydsp_poly(fProcessFactory->createDSPInstance(), nvoices, control, group);
        if (fEffectFactory) {
            // the 'dsp_poly' object has to be controlled with MIDI, so kept separated from new dsp_sequencer(...) object
            return new dsp_poly_effect(dsp_poly, new dsp_sequencer(dsp_poly, fEffectFactory->createDSPInstance()));
        } else {
            return new dsp_poly_effect(dsp_poly, dsp_poly);
        }
    }
    
    /* Create a new DSP instance, to be deleted with C++ 'delete' */
    dsp* createDSPInstance()
    {
        return fProcessFactory->createDSPInstance();
    }
    
};

#endif // __poly_dsp__
/**************************  END  poly-dsp.h **************************/
/************************** BEGIN faust-engine.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2013 GRAME, Romain Michon, CCRMA - Stanford University
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __faust_engine__
#define __faust_engine__

#ifdef __cplusplus
extern "C" {
#endif

    void* create(int, int); // To be implemented
    void destroy(void*);

    bool start(void*);
    void stop(void*);
    
    bool isRunning(void*);

    uintptr_t keyOn(void*, int, int);
    int keyOff(void*, int);
    
    void propagateMidi(void*, int, double, int, int, int, int);

    const char* getJSONUI(void*);
    const char* getJSONMeta(void*);

    int getParamsCount(void*);

    void setParamValue(void*, const char*, float);
    float getParamValue(void*, const char*);
    
    void setParamIdValue(void*, int, float);
    float getParamIdValue(void*, int);

    void setVoiceParamValue(void*, const char*, uintptr_t, float);
    float getVoiceParamValue(void*, const char*, uintptr_t);

    const char* getParamAddress(void*, int);

    void propagateAcc(void*, int, float);
    void setAccConverter(void*, int, int, int, float, float, float);

    void propagateGyr(void*, int, float);
    void setGyrConverter(void*, int, int, int, float, float, float);

    float getCPULoad(void*);
    int getScreenColor(void*);

#ifdef __cplusplus
}
#endif

#endif // __faust_engine__
/**************************  END  faust-engine.h **************************/

//**************************************************************
// Mono or polyphonic audio DSP engine
//**************************************************************

using namespace std;

class FaustPolyEngine {
        
    protected:

        mydsp_poly* fPolyDSP;     // the polyphonic Faust object
        dsp* fFinalDSP;           // the "final" dsp object submitted to the audio driver
    
        APIUI fAPIUI;             // the UI description

        string fJSONUI;
        string fJSONMeta;
        bool fRunning;
        audio* fDriver;
    
        midi_handler fMidiHandler;
        MidiUI fMidiUI;
    
        void init(dsp* mono_dsp, audio* driver, midi_handler* midi)
        {
            bool midi_sync = false;
            int nvoices = 0;
            fRunning = false;
            
            MidiMeta::analyse(mono_dsp, midi_sync, nvoices);
            
            // Getting the UI JSON
            JSONUI jsonui1(mono_dsp->getNumInputs(), mono_dsp->getNumOutputs());
            mono_dsp->buildUserInterface(&jsonui1);
            fJSONUI = jsonui1.JSON();
            
            // Getting the metadata JSON
            JSONUI jsonui1M(mono_dsp->getNumInputs(), mono_dsp->getNumOutputs());
            mono_dsp->metadata(&jsonui1M);
            fJSONMeta = jsonui1M.JSON();
            
            if (nvoices > 0) {
                
                fPolyDSP = new mydsp_poly(mono_dsp, nvoices, true);
                fMidiHandler.addMidiIn(fPolyDSP);
                if (midi) midi->addMidiIn(fPolyDSP);
                
            #if POLY2
                fFinalDSP = new dsp_sequencer(fPolyDSP, new effect());
            #else
                fFinalDSP = fPolyDSP;
            #endif
                
                // Update JSONs with Poly version
                JSONUI jsonui2(mono_dsp->getNumInputs(), mono_dsp->getNumOutputs());
                fFinalDSP->buildUserInterface(&jsonui2);
                fJSONUI = jsonui2.JSON();
                
                JSONUI jsonui2M(mono_dsp->getNumInputs(), mono_dsp->getNumOutputs());
                fFinalDSP->metadata(&jsonui2M);
                fJSONMeta = jsonui2M.JSON();
                
            } else {
                fPolyDSP = NULL;
                fFinalDSP = mono_dsp;
            }
            
            fFinalDSP->buildUserInterface(&fMidiUI);
            fFinalDSP->buildUserInterface(&fAPIUI);
            
            // Retrieving DSP object name
            struct MyMeta : public Meta
            {
                string fName;
                void declare(const char* key, const char* value)
                {
                    if (strcmp(key, "name") == 0) fName = value;
                }
                MyMeta():fName("Dummy"){}
            };
      
            MyMeta meta;
            fFinalDSP->metadata(&meta);
            if (midi) midi->setName(meta.fName);
            
            if (driver) {
                // If driver cannot be initialized, start will fail later on...
                if (!driver->init(meta.fName.c_str(), fFinalDSP)) {
                    delete driver;
                    fDriver = NULL;
                } else {
                    fDriver = driver;
                }
            } else {
                fDriver = NULL;
            }
        }
    
    public:
    
        FaustPolyEngine(dsp* mono_dsp, audio* driver = NULL, midi_handler* midi = NULL):fMidiUI(&fMidiHandler)
        {
            init(((mono_dsp) ? mono_dsp : new mydsp()), driver, midi);
        }
    
        virtual ~FaustPolyEngine()
        {
            delete fDriver;
            delete fFinalDSP;
        }

        /*
         * start()
         * Begins the processing and return true if the connection
         * with the audio device was successful and false if not.
         */
        bool start()
        {
            if (!fRunning) {
                fRunning = (fDriver) ? fDriver->start() : false;
            }
            return fRunning;
        }
    
        /*
         * isRunning()
         * Returns true if the DSP frames are being computed and
         * false if not.
         */
        bool isRunning() 
        {
            return fRunning;
        }

        /*
         * stop()
         * Stops the processing, closes the audio engine.
         */
        void stop()
        {
            if (fRunning) {
                fRunning = false;
                if (fDriver) fDriver->stop();
            }
        }
    
        void setGroup(bool group) { if (fPolyDSP) fPolyDSP->setGroup(group); }
        bool getGroup() { return (fPolyDSP) ? fPolyDSP->getGroup() : false; }
    
        /*
         * keyOn(pitch, velocity)
         * Instantiates a new polyphonic voice where velocity
         * and pitch are MIDI numbers (0-127). keyOn can only
         * be used if the [style:poly] metadata is used in the
         * Faust code. keyOn will return 0 if the object is not
         * polyphonic and the allocated voice otherwise.
         */
        MapUI* keyOn(int pitch, int velocity)
        {
            if (fPolyDSP) {
                return fPolyDSP->keyOn(0, pitch, velocity); // MapUI* passed to Java as an integer
            } else {
                return 0;
            }
        }

        /*
         * keyOff(pitch)
         * De-instantiates a polyphonic voice where pitch is the
         * MIDI number of the note (0-127). keyOff can only be
         * used if the [style:poly] metadata is used in the Faust
         * code. keyOn will return 0 if the object is not polyphonic
         * and 1 otherwise.
         */
        int keyOff(int pitch, int velocity = 127)
        {
            if (fPolyDSP) {
                fPolyDSP->keyOff(0, pitch, velocity);
                return 1;
            } else {
                return 0;
            }
        }

        /*
         * newVoice()
         * Instantiate a new voice and returns the corresponding mapUI.
         */
        MapUI* newVoice()
        {
            if (fPolyDSP) {
                return fPolyDSP->newVoice();
            } else {
                return 0;
            }
        }

        /*
         * deleteVoice(MapUI* voice)
         * Delete a voice based on its MapUI*.
         */
        int deleteVoice(MapUI* voice)
        {
            if (fPolyDSP) {
                fPolyDSP->deleteVoice(voice);
                return 1;
            } else {
                return 0;
            }
        }

        /*
         * deleteVoice(uintptr_t voice)
         * Delete a voice based on its MapUI* casted as a uintptr_t.
         */
        int deleteVoice(uintptr_t voice)
        {
            return deleteVoice(reinterpret_cast<MapUI*>(voice));
        }
        
        /*
         * allNotesOff()
         * Terminates all the active voices, gently (with release when hard = false or immediately when hard = true)
         */
        void allNotesOff(bool hard = false)
        {
            if (fPolyDSP) {
                fPolyDSP->allNotesOff(hard);
            }
        }
    
        /*
         * Propagate MIDI data to the Faust object.
         */
        void propagateMidi(int count, double time, int type, int channel, int data1, int data2)
        {
            if (count == 3) fMidiHandler.handleData2(time, type, channel, data1, data2);
            else if (count == 2) fMidiHandler.handleData1(time, type, channel, data1);
            else if (count == 1) fMidiHandler.handleSync(time, type);
            GUI::updateAllGuis();
        }
    
        /*
         * getJSONUI()
         * Returns a string containing a JSON description of the
         * UI of the Faust object.
         */
        const char* getJSONUI()
        {
            return fJSONUI.c_str();
        }
        
        /*
         * getJSONMeta()
         * Returns a string containing a JSON description of the
         * metadata of the Faust object.
         */
        const char* getJSONMeta()
        {
            return fJSONMeta.c_str();
        }
    
        /*
         * buildUserInterface(ui)
         * Calls the polyphonic or monophonic buildUserInterface with the ui parameter.
         */
        void buildUserInterface(UI* ui_interface)
        {
            fFinalDSP->buildUserInterface(ui_interface);
        }
    
        void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fFinalDSP->compute(count, inputs, outputs);
        }

        /*
         * getParamsCount()
         * Returns the number of control parameters of the Faust object.
         */
        int getParamsCount()
        {
            return fAPIUI.getParamsCount();
        }
    
        /*
         * setParamValue(address, value)
         * Sets the value of the parameter associated with address.
         */
        void setParamValue(const char* address, float value)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            if (id >= 0) {
                fAPIUI.setParamValue(id, value);
                // In POLY mode, update all voices
                GUI::updateAllGuis();
            }
        }

        /*
         * getParamValue(address)
         * Takes the address of a parameter and returns its current
         * value.
         */
        float getParamValue(const char* address)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            return (id >= 0) ? fAPIUI.getParamValue(id) : 0.f;
        }
    
        /*
         * setParamValue(id, value)
         * Sets the value of the parameter associated with id.
         */
        void setParamValue(int id, float value)
        {
            fAPIUI.setParamValue(id, value);
            // In POLY mode, update all voices
            GUI::updateAllGuis();
        }
        
        /*
         * getParamValue(id)
         * Takes the id of a parameter and returns its current
         * value.
         */
        float getParamValue(int id)
        {
            return fAPIUI.getParamValue(id);
        }

        /*
         * setVoiceParamValue(address, voice, value)
         * Sets the value of the parameter associated with address for
         * the voice. setVoiceParamValue can only be
         * used if the [style:poly] metadata is used in the Faust code.
         */
        void setVoiceParamValue(const char* address, uintptr_t voice, float value)
        {
            reinterpret_cast<MapUI*>(voice)->setParamValue(address, value);
        }

        /*
         * setVoiceParamValue(id, voice, value)
         * Sets the value of the parameter associated with the id for
         * the voice. setVoiceParamValue can only be
         * used if the [style:poly] metadata is used in the Faust code.
         */
        void setVoiceParamValue(int id, uintptr_t voice, float value)
        {
            reinterpret_cast<MapUI*>(voice)->setParamValue(reinterpret_cast<MapUI*>(voice)->getParamAddress(id), value);
        }
    
        /*
         * getVoiceParamValue(address, voice)
         * Gets the parameter value associated with address for the voice.
         * getVoiceParamValue can only be used if the [style:poly] metadata
         * is used in the Faust code.
         */
        float getVoiceParamValue(const char* address, uintptr_t voice)
        {
            return reinterpret_cast<MapUI*>(voice)->getParamValue(address);
        }

        /*
         * getVoiceParamValue(id, voice)
         * Gets the parameter value associated with the id for the voice.
         * getVoiceParamValue can only be used if the [style:poly] metadata
         * is used in the Faust code.
         */
        float getVoiceParamValue(int id, uintptr_t voice)
        {
            return reinterpret_cast<MapUI*>(voice)->getParamValue(reinterpret_cast<MapUI*>(voice)->getParamAddress(id));
        }
    
        /*
         * getParamAddress(id)
         * Returns the address of a parameter in function of its "id".
         */
        const char* getParamAddress(int id)
        {
            return fAPIUI.getParamAddress(id);
        }

        /*
         * getVoiceParamAddress(id, voice)
         * Returns the address of a parameter for a specific voice 
         * in function of its "id".
         */
        const char* getVoiceParamAddress(int id, uintptr_t voice)
        {
            return reinterpret_cast<MapUI*>(voice)->getParamAddress(id).c_str();
        }
        
        /*
         * getParamMin(address)
         * Returns the minimum value of a parameter.
         */
        float getParamMin(const char* address)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            return (id >= 0) ? fAPIUI.getParamMin(id) : 0.f;
        }
    
        /*
         * getParamMin(id)
         * Returns the minimum value of a parameter.
         */
        float getParamMin(int id)
        {
            return fAPIUI.getParamMin(id);
        }
    
        /*
         * getParamMax(address)
         * Returns the maximum value of a parameter.
         */
        float getParamMax(const char* address)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            return (id >= 0) ? fAPIUI.getParamMax(id) : 0.f;
        }
    
        /*
         * getParamMax(id)
         * Returns the maximum value of a parameter.
         */
        float getParamMax(int id)
        {
            return fAPIUI.getParamMax(id);
        }
    
        /*
         * getParamInit(address)
         * Returns the default value of a parameter.
         */
        float getParamInit(const char* address)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            return (id >= 0) ? fAPIUI.getParamInit(id) : 0.f;
        }
    
        /*
         * getParamInit(id)
         * Returns the default value of a parameter.
         */
        float getParamInit(int id)
        {
            return fAPIUI.getParamInit(id);
        }
    
        /*
         * getMetadata(address, key)
         * Returns the metadata of a parameter.
         */
        const char* getMetadata(const char* address, const char* key)
        {
            int id = (address) ? fAPIUI.getParamIndex(address) : -1;
            return (id >= 0) ? fAPIUI.getMetadata(id, key) : "";
        }
    
        /*
         * getMetadata(id, key)
         * Returns the metadata of a parameter.
         */
        const char* getMetadata(int id, const char* key)
        {
            return fAPIUI.getMetadata(id, key);
        }

        /*
         * propagateAcc(int acc, float v)
         * Propage accelerometer value to the curve conversion layer.
         */
        void propagateAcc(int acc, float v)
        {
            fAPIUI.propagateAcc(acc, v);
            GUI::updateAllGuis();
        }

        /*
         * setAccConverter(int p, int acc, int curve, float amin, float amid, float amax)
         * Change accelerometer curve mapping.
         */
        void setAccConverter(int p, int acc, int curve, float amin, float amid, float amax)
        {
            fAPIUI.setAccConverter(p, acc, curve, amin, amid, amax);
        }

        /*
         * propagateGyr(int gyr, float v)
         * Propage gyroscope value to the curve conversion layer.
         */
        void propagateGyr(int gyr, float v)
        {
            fAPIUI.propagateGyr(gyr, v);
            GUI::updateAllGuis();
        }

        /*
         * setGyrConverter(int p, int acc, int curve, float amin, float amid, float amax)
         * Change gyroscope curve mapping.
         */
        void setGyrConverter(int p, int gyr, int curve, float amin, float amid, float amax)
        {
            fAPIUI.setGyrConverter(p, gyr, curve, amin, amid, amax);
        }
    
        /*
         * getCPULoad()
         * Return DSP CPU load.
         */
        float getCPULoad() { return (fDriver) ? fDriver->getCPULoad() : 0.f; }

        /*
         * getScreenColor() -> c:int
         * Get the requested screen color c :
         * c <  0 : no screen color requested (keep regular UI)
         * c >= 0 : requested color (no UI but a colored screen)
         */
        int getScreenColor()
        {
            return fAPIUI.getScreenColor();
        }

};

// Public C API

#ifdef __cplusplus
extern "C" {
#endif
    
    void destroy(void* dsp) { delete reinterpret_cast<FaustPolyEngine*>(dsp); }

    bool start(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->start(); }
    void stop(void* dsp) { reinterpret_cast<FaustPolyEngine*>(dsp)->stop(); }
    
    bool isRunning(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->isRunning(); }

    uintptr_t keyOn(void* dsp, int pitch, int velocity) { return (uintptr_t)reinterpret_cast<FaustPolyEngine*>(dsp)->keyOn(pitch, velocity); }
    int keyOff(void* dsp, int pitch) { return reinterpret_cast<FaustPolyEngine*>(dsp)->keyOff(pitch); }
    
    void propagateMidi(void* dsp, int count, double time, int type, int channel, int data1, int data2)
    {
        reinterpret_cast<FaustPolyEngine*>(dsp)->propagateMidi(count, time, type, channel, data1, data2);
    }

    const char* getJSONUI(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getJSONUI(); }
    const char* getJSONMeta(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getJSONMeta(); }

    int getParamsCount(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getParamsCount(); }
    
    void setParamValue(void* dsp, const char* address, float value) { reinterpret_cast<FaustPolyEngine*>(dsp)->setParamValue(address, value); }
    float getParamValue(void* dsp, const char* address) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getParamValue(address); }
   
    void setParamIdValue(void* dsp, int id, float value) { reinterpret_cast<FaustPolyEngine*>(dsp)->setParamValue(id, value); }
    float getParamIdValue(void* dsp, int id) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getParamValue(id); }
    
    void setVoiceParamValue(void* dsp, const char* address, uintptr_t voice, float value)
    {
        reinterpret_cast<FaustPolyEngine*>(dsp)->setVoiceParamValue(address, voice, value);
    }
    float getVoiceParamValue(void* dsp, const char* address, uintptr_t voice) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getVoiceParamValue(address, voice); }
    
    const char* getParamAddress(void* dsp, int id) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getParamAddress(id); }

    void propagateAcc(void* dsp, int acc, float v) { reinterpret_cast<FaustPolyEngine*>(dsp)->propagateAcc(acc, v); }
    void setAccConverter(void* dsp, int p, int acc, int curve, float amin, float amid, float amax)
    {
        reinterpret_cast<FaustPolyEngine*>(dsp)->setAccConverter(p, acc, curve, amin, amid, amax);
    }
    void propagateGyr(void* dsp, int acc, float v) { reinterpret_cast<FaustPolyEngine*>(dsp)->propagateGyr(acc, v); }
    void setGyrConverter(void* dsp, int p, int gyr, int curve, float amin, float amid, float amax)
    {
        reinterpret_cast<FaustPolyEngine*>(dsp)->setGyrConverter(p, gyr, curve, amin, amid, amax);
    }

    float getCPULoad(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getCPULoad(); }
    int getScreenColor(void* dsp) { return reinterpret_cast<FaustPolyEngine*>(dsp)->getScreenColor(); }
    
#ifdef __cplusplus
}
#endif

#endif // __faust_poly_engine__
/**************************  END  faust-poly-engine.h **************************/

//**************************************************************
// Audio driver
//**************************************************************

#if COREAUDIO_DRIVER
/************************** BEGIN coreaudio-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __coreaudio_dsp__
#define __coreaudio_dsp__

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sys/time.h>

#include <AudioToolbox/AudioConverter.h>
#include <CoreAudio/CoreAudio.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreServices/CoreServices.h>

#ifdef HAS_MATH_EXCEPTION
/************************** BEGIN fpe.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __fpe__
#define __fpe__

#include <cfenv>
#include <iostream>
#include <signal.h>

#ifndef HAVE_FEENABLEEXCEPT
#if defined(__APPLE__) && defined(__MACH__)

// Public domain polyfill for feenableexcept on OS X
// http://www-personal.umich.edu/~williams/archive/computation/fe-handling-example.c

inline int feenableexcept(unsigned int excepts)
{
    static fenv_t fenv;
    unsigned int new_excepts = excepts & FE_ALL_EXCEPT;
    // previous masks
    unsigned int old_excepts;
    
    if (fegetenv(&fenv)) {
        return -1;
    }
    old_excepts = fenv.__control & FE_ALL_EXCEPT;
    
    // unmask
    fenv.__control &= ~new_excepts;
    fenv.__mxcsr   &= ~(new_excepts << 7);
    
    return fesetenv(&fenv) ? -1 : old_excepts;
}

inline int fedisableexcept(unsigned int excepts)
{
    static fenv_t fenv;
    unsigned int new_excepts = excepts & FE_ALL_EXCEPT;
    // all previous masks
    unsigned int old_excepts;
    
    if (fegetenv(&fenv)) {
        return -1;
    }
    old_excepts = fenv.__control & FE_ALL_EXCEPT;
    
    // mask
    fenv.__control |= new_excepts;
    fenv.__mxcsr   |= new_excepts << 7;
    
    return fesetenv(&fenv) ? -1 : old_excepts;
}

#else
inline int feenableexcept(unsigned int excepts)
{
#pragma STDC FENV_ACCESS ON
    fexcept_t flags;
    /* Save current exception flags. */
    fegetexceptflag(&flags, FE_ALL_EXCEPT);
    
    feclearexcept(FE_ALL_EXCEPT);   /* clear all fp exception conditions */
    return fesetexceptflag(&flags, excepts) != 0 ? -1 : flags; /* set new flags */
    
}

inline int fedisableexcept(unsigned int excepts)
{
#pragma STDC FENV_ACCESS ON
    fexcept_t flags;
    /* Save current exception flags. */
    fegetexceptflag(&flags, FE_ALL_EXCEPT);
    
    feclearexcept(FE_ALL_EXCEPT);   /* clear all fp exception conditions */
    return fesetexceptflag(&flags, ~excepts) != 0 ? -1 : flags; /* set new flags */
}

#endif
#endif

// https://en.cppreference.com/w/c/numeric/fenv/FE_exceptions

static void fPEHandler(int sig, siginfo_t* sip, ucontext_t* scp)
{
    int fe_code = sip->si_code;
    
    switch (fe_code) {
    #ifdef FPE_NOOP  // occurs in OS X
        case FPE_NOOP: throw std::runtime_error("FE_NOOP"); break;
    #endif
        case FPE_FLTDIV: throw std::runtime_error("FE_DIVBYZERO"); break;   // divideByZero
        case FPE_FLTINV: throw std::runtime_error("FE_INVALID"); break;     // invalid
        case FPE_FLTOVF: throw std::runtime_error("FE_OVERFLOW"); break;    // overflow
        case FPE_FLTUND: throw std::runtime_error("FE_UNDERFLOW"); break;   // underflow
        case FPE_FLTRES: throw std::runtime_error("FE_INEXACT"); break;     // inexact
        case FPE_FLTSUB: throw std::runtime_error("FE_INVALID"); break;     // invalid
        case FPE_INTDIV: throw std::runtime_error("INT_DIVBYZERO"); break;  // divideByZero
        case FPE_INTOVF: throw std::runtime_error("INT_OVERFLOW"); break;   // overflow
        default: throw std::runtime_error("FE_NOOP"); break;
    }
}

static bool gSetFPEHandler = false;
static void setFPEHandler()
{
    feclearexcept(FE_ALL_EXCEPT);
    feenableexcept(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
    
    if (!gSetFPEHandler) {
        gSetFPEHandler = true;
        
        struct sigaction act;
        act.sa_sigaction = (void(*)(int, siginfo_t*, void*))fPEHandler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO;
        int res = sigaction(SIGFPE, &act, (struct sigaction*)0);
    }
}

/* 
 
TRY_FPE/CATCH_FPE to be used by wrapping code that needs for be protected
 
TRY_FPE
compute(...);
CATCH_FPE
 
*/

#define TRY_FPE     \
setFPEHandler();    \
try {               \

#define CATCH_FPE                   \
} catch (std::runtime_error e) {    \
    std::cerr << "Math exception : " << e.what() << std::endl;  \
    exit(-1);                       \
}                                   \

#endif

/**************************  END  fpe.h **************************/
#endif

/******************************************************************************
*******************************************************************************

							COREAUDIO INTERNAL INTERFACE

*******************************************************************************
*******************************************************************************/

#define OPEN_ERR -1
#define CLOSE_ERR -1
#define NO_ERR 0

#define WAIT_NOTIFICATION_COUNTER 60

typedef	UInt8	CAAudioHardwareDeviceSectionID;
#define	kAudioDeviceSectionInput	((CAAudioHardwareDeviceSectionID)0x01)
#define	kAudioDeviceSectionOutput	((CAAudioHardwareDeviceSectionID)0x00)
#define	kAudioDeviceSectionGlobal	((CAAudioHardwareDeviceSectionID)0x00)
#define	kAudioDeviceSectionWildcard	((CAAudioHardwareDeviceSectionID)0xFF)

class TCoreAudioRenderer;
typedef TCoreAudioRenderer* TCoreAudioRendererPtr;

static void PrintStreamDesc(AudioStreamBasicDescription *inDesc)
{
    printf("- - - - - - - - - - - - - - - - - - - -\n");
    printf("  Sample Rate:%f\n", inDesc->mSampleRate);
    printf("  Format ID:%.*s\n", (int)sizeof(inDesc->mFormatID), (char*)&inDesc->mFormatID);
    printf("  Format Flags:%lX\n", (unsigned long)inDesc->mFormatFlags);
    printf("  Bytes per Packet:%ld\n", (long)inDesc->mBytesPerPacket);
    printf("  Frames per Packet:%ld\n", (long)inDesc->mFramesPerPacket);
    printf("  Bytes per Frame:%ld\n", (long)inDesc->mBytesPerFrame);
    printf("  Channels per Frame:%ld\n", (long)inDesc->mChannelsPerFrame);
    printf("  Bits per Channel:%ld\n", (long)inDesc->mBitsPerChannel);
    printf("- - - - - - - - - - - - - - - - - - - -\n");
}

static void printError(OSStatus err)
{
    switch (err) {
        case kAudioHardwareNoError:
            printf("error code : kAudioHardwareNoError\n");
            break;
		case kAudioConverterErr_FormatNotSupported:
            printf("error code : kAudioConverterErr_FormatNotSupported\n");
            break;
        case kAudioConverterErr_OperationNotSupported:
            printf("error code : kAudioConverterErr_OperationNotSupported\n");
            break;
        case kAudioConverterErr_PropertyNotSupported:
            printf("error code : kAudioConverterErr_PropertyNotSupported\n");
            break;
        case kAudioConverterErr_InvalidInputSize:
            printf("error code : kAudioConverterErr_InvalidInputSize\n");
            break;
        case kAudioConverterErr_InvalidOutputSize:
            printf("error code : kAudioConverterErr_InvalidOutputSize\n");
            break;
        case kAudioConverterErr_UnspecifiedError:
            printf("error code : kAudioConverterErr_UnspecifiedError\n");
            break;
        case kAudioConverterErr_BadPropertySizeError:
            printf("error code : kAudioConverterErr_BadPropertySizeError\n");
            break;
        case kAudioConverterErr_RequiresPacketDescriptionsError:
            printf("error code : kAudioConverterErr_RequiresPacketDescriptionsError\n");
            break;
        case kAudioConverterErr_InputSampleRateOutOfRange:
            printf("error code : kAudioConverterErr_InputSampleRateOutOfRange\n");
            break;
        case kAudioConverterErr_OutputSampleRateOutOfRange:
            printf("error code : kAudioConverterErr_OutputSampleRateOutOfRange\n");
            break;
		case kAudioHardwareNotRunningError:
            printf("error code : kAudioHardwareNotRunningError\n");
            break;
        case kAudioHardwareUnknownPropertyError:
            printf("error code : kAudioHardwareUnknownPropertyError\n");
            break;
        case kAudioHardwareIllegalOperationError:
            printf("error code : kAudioHardwareIllegalOperationError\n");
            break;
        case kAudioHardwareBadDeviceError:
            printf("error code : kAudioHardwareBadDeviceError\n");
            break;
        case kAudioHardwareBadStreamError:
            printf("error code : kAudioHardwareBadStreamError\n");
            break;
        case kAudioDeviceUnsupportedFormatError:
            printf("error code : kAudioDeviceUnsupportedFormatError\n");
            break;
        case kAudioDevicePermissionsError:
            printf("error code : kAudioDevicePermissionsError\n");
            break;
        default:
            printf("error code : err = %d\n", err);
            break;
    }
}

static Float64 GetNominalSampleRate(AudioDeviceID inDevice) 
{
    Float64 sampleRate = 0;
    UInt32 outSize =  sizeof(Float64);
    OSStatus err = AudioDeviceGetProperty(inDevice, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyNominalSampleRate, &outSize, &sampleRate);
    if (err != noErr) {
        printf("Cannot get current sample rate\n");
        printError(err);
        return -1;
    } else {
        return sampleRate;
    }
}

static CFStringRef GetDeviceName(AudioDeviceID id)
{
    UInt32 size = sizeof(CFStringRef);
    CFStringRef UIname;
    OSStatus err = AudioDeviceGetProperty(id, 0, false, kAudioDevicePropertyDeviceUID, &size, &UIname);
    return (err == noErr) ? UIname : NULL;
}

static bool CheckAvailableDeviceName(const char* device_name, AudioDeviceID* device_id, int len = -1)
{
    UInt32 size;
    Boolean isWritable;
    int i, deviceNum;
    OSStatus err;

    err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &size, &isWritable);
    if (err != noErr) {
        return false;
    }

    deviceNum = size / sizeof(AudioDeviceID);
    AudioDeviceID devices[deviceNum];
 
    err = AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &size, devices);
    if (err != noErr) {
        return false;
    }

    for (i = 0; i < deviceNum; i++) {
        char device_name_aux[256];
    
        size = 256;
        err = AudioDeviceGetProperty(devices[i], 0, false, kAudioDevicePropertyDeviceName, &size, device_name_aux);
        if (err != noErr) {
            return false;
        }
 
        if (strncmp(device_name_aux, device_name, (len == -1) ? strlen(device_name) : len) == 0) {
            *device_id = devices[i];
            return true;
        }
    }

    return false;
}

class TCoreAudioRenderer
{
    
    protected:
    
        AudioDeviceID fAggregateDeviceID;
        AudioObjectID fAggregatePluginID;    // Used for aggregate device
     
        int fDevNumInChans;
        int fDevNumOutChans;
        
        int fPhysicalInputs;
        int fPhysicalOutputs;
        
        float** fInChannel;
        float** fOutChannel;

        int fBufferSize;
        int fSampleRate;
    
        bool fIsInJackDevice;
        bool fIsOutJackDevice;
        
        dsp* fDSP;
    
        audio* fAudio;

        AudioBufferList* fInputData;
        AudioDeviceID fDeviceID;
        AudioUnit fAUHAL;
        bool fState;

        OSStatus GetDefaultDeviceAndSampleRate(int inChan, int outChan, int& sample_rate, AudioDeviceID* device)
        {
            
            UInt32 theSize = sizeof(UInt32);
            AudioDeviceID inDefault;
            AudioDeviceID outDefault;
            OSStatus res;
            
            if ((res = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice,
                                                &theSize, &inDefault)) != noErr) {
                return res;
            }
            
            if ((res = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice,
                                                &theSize, &outDefault)) != noErr) {
                return res;
            }
            
            /*
            // TODO
            if (inDefault == 0) {
                printf("Error default input device is 0, will take 'Built-in'...\n");
                if (CheckAvailableDeviceName("Built-in Microphone", &inDefault) 
                    || CheckAvailableDeviceName("Built-in Line", &inDefault)) {
                    printf("GetDefaultInputDevice : output = %ld\n", inDefault);
                } else {
                    printf("Cannot find any input device to use...");
                    return -1;
                }
            }
            
            if (outDefault == 0) {
                printf("Error default ouput device is 0, will take 'Built-in'...\n");
                if (CheckAvailableDeviceName("Built-in Output", &outDefault)) {
                    printf("GetDefaultOutputDevice : output = %ld\n", outDefault);
                } else {
                    printf("Cannot find any output device to use...\n");
                    return -1;
                }
            }  
            */          
            
            //printf("GetDefaultDevice : input = %d output = %d\n", inDefault, outDefault);
            
            // Duplex mode
            if (inChan > 0 && outChan > 0) {
                // Get the device only if default input and output are the same
                if (inDefault == outDefault) {
                    *device = inDefault;
                    goto end;
                } else {
                    if (CreateAggregateDevice(inDefault, outDefault, sample_rate) != noErr) {
                        return kAudioHardwareBadDeviceError;
                    }
                    //printf("fAggregateDeviceID %d\n", fAggregateDeviceID);
                    *device = fAggregateDeviceID;
                    goto end;
                }
            } else if (inChan > 0) {
                *device = inDefault;
                goto end;
            } else if (outChan > 0) {
                *device = outDefault;
                goto end;
            } else {
                return kAudioHardwareBadDeviceError;
            }
            
        end:   
            
            if (sample_rate == -1) {
                // Possible take the current sample rate
                sample_rate = int(GetNominalSampleRate(*device));
            } else {
                // Otherwise force the one we want...
                SetupSampleRateAux(*device, sample_rate);
            }
            //printf("samplerate %d\n", sample_rate);
            fSampleRate = sample_rate;
            return noErr;
        }

        OSStatus CreateAggregateDevice(AudioDeviceID captureDeviceID, AudioDeviceID playbackDeviceID, int& sample_rate)
        {
            OSStatus err = noErr;
            AudioObjectID sub_device[32];
            UInt32 outSize = sizeof(sub_device);
            
            //printf("CreateAggregateDevice : input device %d\n", captureDeviceID);
            
            err = AudioDeviceGetProperty(captureDeviceID, 0, kAudioDeviceSectionGlobal, kAudioAggregateDevicePropertyActiveSubDeviceList, &outSize, sub_device);
            std::vector<AudioDeviceID> captureDeviceIDArray;
            
            if (err != noErr) {
                //printf("Input device does not have subdevices\n");
                captureDeviceIDArray.push_back(captureDeviceID);
            } else {
                int num_devices = outSize / sizeof(AudioObjectID);
                //printf("Input device has %d subdevices\n", num_devices);
                for (int i = 0; i < num_devices; i++) {
                    //printf("Input sub_device %d\n", sub_device[i]);
                    captureDeviceIDArray.push_back(sub_device[i]);
                }
            }
            
            outSize = sizeof(sub_device);
            err = AudioDeviceGetProperty(playbackDeviceID, 0, kAudioDeviceSectionGlobal, kAudioAggregateDevicePropertyActiveSubDeviceList, &outSize, sub_device);
            std::vector<AudioDeviceID> playbackDeviceIDArray;
            
            if (err != noErr) {
                //printf("Output device does not have subdevices\n");
                playbackDeviceIDArray.push_back(playbackDeviceID);
            } else {
                int num_devices = outSize / sizeof(AudioObjectID);
                //printf("Output device has %d subdevices\n", num_devices);
                for (int i = 0; i < num_devices; i++) {
                    //printf("Output sub_device %d\n", sub_device[i]);
                    playbackDeviceIDArray.push_back(sub_device[i]);
                }
            }
            
            return CreateAggregateDeviceAux(captureDeviceIDArray, playbackDeviceIDArray, sample_rate);
        }
        
        OSStatus CreateAggregateDeviceAux(std::vector<AudioDeviceID> captureDeviceID, std::vector<AudioDeviceID> playbackDeviceID, int& sample_rate)
        {
            OSStatus osErr = noErr;
            UInt32 outSize;
            Boolean outWritable;
            bool fClockDriftCompensate = true;
            
            // Prepare sub-devices for clock drift compensation
            // Workaround for bug in the HAL : until 10.6.2
            AudioObjectPropertyAddress theAddressOwned = { kAudioObjectPropertyOwnedObjects, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster };
            AudioObjectPropertyAddress theAddressDrift = { kAudioSubDevicePropertyDriftCompensation, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster };
            UInt32 theQualifierDataSize = sizeof(AudioObjectID);
            AudioClassID inClass = kAudioSubDeviceClassID;
            void* theQualifierData = &inClass;
            UInt32 subDevicesNum = 0;
            
            //---------------------------------------------------------------------------
            // Setup SR of both devices otherwise creating AD may fail...
            //---------------------------------------------------------------------------
            UInt32 keptclockdomain = 0;
            UInt32 clockdomain = 0;
            outSize = sizeof(UInt32);
            bool need_clock_drift_compensation = false;
            
            for (UInt32 i = 0; i < captureDeviceID.size(); i++) {
                if (SetupSampleRateAux(captureDeviceID[i], sample_rate) < 0) {
                    printf("TCoreAudioRenderer::CreateAggregateDeviceAux : cannot set SR of input device\n");
                } else  {
                    // Check clock domain
                    osErr = AudioDeviceGetProperty(captureDeviceID[i], 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyClockDomain, &outSize, &clockdomain);
                    if (osErr != 0) {
                        printf("TCoreAudioRenderer::CreateAggregateDeviceAux : kAudioDevicePropertyClockDomain error\n");
                        printError(osErr);
                    } else {
                        keptclockdomain = (keptclockdomain == 0) ? clockdomain : keptclockdomain;
                        //printf("TCoreAudioRenderer::CreateAggregateDevice : input clockdomain = %d\n", clockdomain);
                        if (clockdomain != 0 && clockdomain != keptclockdomain) {
                            //printf("TCoreAudioRenderer::CreateAggregateDevice : devices do not share the same clock!! clock drift compensation would be needed...\n");
                            need_clock_drift_compensation = true;
                        }
                    }
                }
            }
            
            for (UInt32 i = 0; i < playbackDeviceID.size(); i++) {
                if (SetupSampleRateAux(playbackDeviceID[i], sample_rate) < 0) {
                    printf("TCoreAudioRenderer::CreateAggregateDeviceAux : cannot set SR of output device\n");
                } else {
                    // Check clock domain
                    osErr = AudioDeviceGetProperty(playbackDeviceID[i], 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyClockDomain, &outSize, &clockdomain);
                    if (osErr != 0) {
                        printf("TCoreAudioRenderer::CreateAggregateDeviceAux : kAudioDevicePropertyClockDomain error\n");
                        printError(osErr);
                    } else {
                        keptclockdomain = (keptclockdomain == 0) ? clockdomain : keptclockdomain;
                        //printf("TCoreAudioRenderer::CreateAggregateDevice : output clockdomain = %d", clockdomain);
                        if (clockdomain != 0 && clockdomain != keptclockdomain) {
                            //printf("TCoreAudioRenderer::CreateAggregateDevice : devices do not share the same clock!! clock drift compensation would be needed...\n");
                            need_clock_drift_compensation = true;
                        }
                    }
                }
            }
            
            // If no valid clock domain was found, then assume we have to compensate...
            if (keptclockdomain == 0) {
                need_clock_drift_compensation = true;
            }
            
            //---------------------------------------------------------------------------
            // Start to create a new aggregate by getting the base audio hardware plugin
            //---------------------------------------------------------------------------
            
            char device_name[256];
            for (UInt32 i = 0; i < captureDeviceID.size(); i++) {
                GetDeviceNameFromID(captureDeviceID[i], device_name);
                //printf("Separated input = '%s' \n", device_name);
            }
            
            for (UInt32 i = 0; i < playbackDeviceID.size(); i++) {
                GetDeviceNameFromID(playbackDeviceID[i], device_name);
                //printf("Separated output = '%s' \n", device_name);
            }
            
            osErr = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyPlugInForBundleID, &outSize, &outWritable);
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioHardwareGetPropertyInfo kAudioHardwarePropertyPlugInForBundleID error\n");
                printError(osErr);
                return osErr;
            }
            
            AudioValueTranslation pluginAVT;
            CFStringRef inBundleRef = CFSTR("com.apple.audio.CoreAudio");
            
            pluginAVT.mInputData = &inBundleRef;
            pluginAVT.mInputDataSize = sizeof(inBundleRef);
            pluginAVT.mOutputData = &fAggregatePluginID;
            pluginAVT.mOutputDataSize = sizeof(fAggregatePluginID);
            
            osErr = AudioHardwareGetProperty(kAudioHardwarePropertyPlugInForBundleID, &outSize, &pluginAVT);
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioHardwareGetProperty kAudioHardwarePropertyPlugInForBundleID error\n");
                printError(osErr);
                return osErr;
            }
            
            //-------------------------------------------------
            // Create a CFDictionary for our aggregate device
            //-------------------------------------------------
            
            CFMutableDictionaryRef aggDeviceDict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            
            char buffer1[64];
            char buffer2[64];
            
            // generate "random" name
            struct timeval fTv1;
            struct timezone tz;
            gettimeofday(&fTv1, &tz);
            
            sprintf(buffer1, "com.grame.%d", fTv1.tv_sec + fTv1.tv_usec);
            sprintf(buffer2, "%d", fTv1.tv_sec + fTv1.tv_usec);
            
            CFStringRef AggregateDeviceNameRef = CFStringCreateWithCString(kCFAllocatorDefault, buffer1, CFStringGetSystemEncoding());
            CFStringRef AggregateDeviceUIDRef = CFStringCreateWithCString(kCFAllocatorDefault, buffer2, CFStringGetSystemEncoding());
            
            // add the name of the device to the dictionary
            CFDictionaryAddValue(aggDeviceDict, CFSTR(kAudioAggregateDeviceNameKey), AggregateDeviceNameRef);
            
            // add our choice of UID for the aggregate device to the dictionary
            CFDictionaryAddValue(aggDeviceDict, CFSTR(kAudioAggregateDeviceUIDKey), AggregateDeviceUIDRef);
            
            // add a "private aggregate key" to the dictionary
            int value = 1;
            CFNumberRef AggregateDeviceNumberRef = CFNumberCreate(NULL, kCFNumberIntType, &value);
            
            SInt32 system;
            Gestalt(gestaltSystemVersion, &system);
            
            //printf("TCoreAudioRenderer::CreateAggregateDevice : system version = %x limit = %x\n", system, 0x00001054);
            
            // Starting with 10.5.4 systems, the AD can be internal... (better)
            if (system < 0x00001054) {
                //printf("TCoreAudioRenderer::CreateAggregateDevice : public aggregate device....\n");
            } else {
                //printf("TCoreAudioRenderer::CreateAggregateDevice : private aggregate device....\n");
                CFDictionaryAddValue(aggDeviceDict, CFSTR(kAudioAggregateDeviceIsPrivateKey), AggregateDeviceNumberRef);
            }
            
            // Prepare sub-devices for clock drift compensation
            CFMutableArrayRef subDevicesArrayClock = NULL;
            
            /*
             if (fClockDriftCompensate) {
                 if (need_clock_drift_compensation) {
                     jack_info("Clock drift compensation activated...");
                     subDevicesArrayClock = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
                     
                     for (UInt32 i = 0; i < captureDeviceID.size(); i++) {
                         CFStringRef UID = GetDeviceName(captureDeviceID[i]);
                         if (UID) {
                         CFMutableDictionaryRef subdeviceAggDeviceDict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                         CFDictionaryAddValue(subdeviceAggDeviceDict, CFSTR(kAudioSubDeviceUIDKey), UID);
                         CFDictionaryAddValue(subdeviceAggDeviceDict, CFSTR(kAudioSubDeviceDriftCompensationKey), AggregateDeviceNumberRef);
                         //CFRelease(UID);
                         CFArrayAppendValue(subDevicesArrayClock, subdeviceAggDeviceDict);
                     }
                 }
                 
                 for (UInt32 i = 0; i < playbackDeviceID.size(); i++) {
                     CFStringRef UID = GetDeviceName(playbackDeviceID[i]);
                     if (UID) {
                         CFMutableDictionaryRef subdeviceAggDeviceDict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                         CFDictionaryAddValue(subdeviceAggDeviceDict, CFSTR(kAudioSubDeviceUIDKey), UID);
                         CFDictionaryAddValue(subdeviceAggDeviceDict, CFSTR(kAudioSubDeviceDriftCompensationKey), AggregateDeviceNumberRef);
                         //CFRelease(UID);
                         CFArrayAppendValue(subDevicesArrayClock, subdeviceAggDeviceDict);
                     }
                 }
                     
                     // add sub-device clock array for the aggregate device to the dictionary
                     CFDictionaryAddValue(aggDeviceDict, CFSTR(kAudioAggregateDeviceSubDeviceListKey), subDevicesArrayClock);
                     } else {
                     jack_info("Clock drift compensation was asked but is not needed (devices use the same clock domain)");
                 }
            }
            */
            
            //-------------------------------------------------
            // Create a CFMutableArray for our sub-device list
            //-------------------------------------------------
            
            // we need to append the UID for each device to a CFMutableArray, so create one here
            CFMutableArrayRef subDevicesArray = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
            
            std::vector<CFStringRef> captureDeviceUID;
            for (UInt32 i = 0; i < captureDeviceID.size(); i++) {
                CFStringRef ref = GetDeviceName(captureDeviceID[i]);
                if (ref == NULL) {
                    return -1;
                }
                captureDeviceUID.push_back(ref);
                // input sub-devices in this example, so append the sub-device's UID to the CFArray
                CFArrayAppendValue(subDevicesArray, ref);
            }
            
            std::vector<CFStringRef> playbackDeviceUID;
            for (UInt32 i = 0; i < playbackDeviceID.size(); i++) {
                CFStringRef ref = GetDeviceName(playbackDeviceID[i]);
                if (ref == NULL) {
                    return -1;
                }
                playbackDeviceUID.push_back(ref);
                // output sub-devices in this example, so append the sub-device's UID to the CFArray
                CFArrayAppendValue(subDevicesArray, ref);
            }
            
            //-----------------------------------------------------------------------
            // Feed the dictionary to the plugin, to create a blank aggregate device
            //-----------------------------------------------------------------------
            
            AudioObjectPropertyAddress pluginAOPA;
            pluginAOPA.mSelector = kAudioPlugInCreateAggregateDevice;
            pluginAOPA.mScope = kAudioObjectPropertyScopeGlobal;
            pluginAOPA.mElement = kAudioObjectPropertyElementMaster;
            UInt32 outDataSize;
            
            osErr = AudioObjectGetPropertyDataSize(fAggregatePluginID, &pluginAOPA, 0, NULL, &outDataSize);
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioObjectGetPropertyDataSize error\n");
                printError(osErr);
                goto error;
            }
            
            osErr = AudioObjectGetPropertyData(fAggregatePluginID, &pluginAOPA, sizeof(aggDeviceDict), &aggDeviceDict, &outDataSize, &fAggregateDeviceID);
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioObjectGetPropertyData error\n");
                printError(osErr);
                goto error;
            }
            
            // pause for a bit to make sure that everything completed correctly
            // this is to work around a bug in the HAL where a new aggregate device seems to disappear briefly after it is created
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
            
            //-------------------------
            // Set the sub-device list
            //-------------------------
            
            pluginAOPA.mSelector = kAudioAggregateDevicePropertyFullSubDeviceList;
            pluginAOPA.mScope = kAudioObjectPropertyScopeGlobal;
            pluginAOPA.mElement = kAudioObjectPropertyElementMaster;
            outDataSize = sizeof(CFMutableArrayRef);
            osErr = AudioObjectSetPropertyData(fAggregateDeviceID, &pluginAOPA, 0, NULL, outDataSize, &subDevicesArray);
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioObjectSetPropertyData for sub-device list error\n");
                printError(osErr);
                goto error;
            }
            
            // pause again to give the changes time to take effect
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
            
            //-----------------------
            // Set the master device
            //-----------------------
            
            // set the master device manually (this is the device which will act as the master clock for the aggregate device)
            // pass in the UID of the device you want to use
            pluginAOPA.mSelector = kAudioAggregateDevicePropertyMasterSubDevice;
            pluginAOPA.mScope = kAudioObjectPropertyScopeGlobal;
            pluginAOPA.mElement = kAudioObjectPropertyElementMaster;
            outDataSize = sizeof(CFStringRef);
            osErr = AudioObjectSetPropertyData(fAggregateDeviceID, &pluginAOPA, 0, NULL, outDataSize, &playbackDeviceUID[0]);  // First playback is master...
            if (osErr != noErr) {
                printf("TCoreAudioRenderer::CreateAggregateDeviceAux : AudioObjectSetPropertyData for master device error\n");
                printError(osErr);
                goto error;
            }
            
            // pause again to give the changes time to take effect
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
            
            // Prepare sub-devices for clock drift compensation
            // Workaround for bug in the HAL : until 10.6.2
            
            if (fClockDriftCompensate) {
                if (need_clock_drift_compensation) {
                    //printf("Clock drift compensation activated...\n");
                    
                    // Get the property data size
                    osErr = AudioObjectGetPropertyDataSize(fAggregateDeviceID, &theAddressOwned, theQualifierDataSize, theQualifierData, &outSize);
                    if (osErr != noErr) {
                        printf("TCoreAudioRenderer::CreateAggregateDeviceAux kAudioObjectPropertyOwnedObjects error\n");
                        printError(osErr);
                    }
                    
                    //	Calculate the number of object IDs
                    subDevicesNum = outSize / sizeof(AudioObjectID);
                    //printf("TCoreAudioRenderer::CreateAggregateDevice clock drift compensation, number of sub-devices = %d\n", subDevicesNum);
                    AudioObjectID subDevices[subDevicesNum];
                    outSize = sizeof(subDevices);
                    
                    osErr = AudioObjectGetPropertyData(fAggregateDeviceID, &theAddressOwned, theQualifierDataSize, theQualifierData, &outSize, subDevices);
                    if (osErr != noErr) {
                        printf("TCoreAudioRenderer::CreateAggregateDeviceAux kAudioObjectPropertyOwnedObjects error\n");
                        printError(osErr);
                    }
                    
                    // Set kAudioSubDevicePropertyDriftCompensation property...
                    for (UInt32 index = 0; index < subDevicesNum; ++index) {
                        UInt32 theDriftCompensationValue = 1;
                        osErr = AudioObjectSetPropertyData(subDevices[index], &theAddressDrift, 0, NULL, sizeof(UInt32), &theDriftCompensationValue);
                        if (osErr != noErr) {
                            printf("TCoreAudioRenderer::CreateAggregateDeviceAux kAudioSubDevicePropertyDriftCompensation error\n");
                            printError(osErr);
                        }
                    }
                } else {
                    //printf("Clock drift compensation was asked but is not needed (devices use the same clock domain)\n");
                }
            }
            
            // pause again to give the changes time to take effect
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
            
            //----------
            // Clean up
            //----------
            
            // release the private AD key
            CFRelease(AggregateDeviceNumberRef);
            
            // release the CF objects we have created - we don't need them any more
            CFRelease(aggDeviceDict);
            CFRelease(subDevicesArray);
            
            if (subDevicesArrayClock)
                CFRelease(subDevicesArrayClock);
            
            // release the device UID
            for (UInt32 i = 0; i < captureDeviceUID.size(); i++) {
                CFRelease(captureDeviceUID[i]);
            }
            
            for (UInt32 i = 0; i < playbackDeviceUID.size(); i++) {
                CFRelease(playbackDeviceUID[i]);
            }
            
            //printf("New aggregate device %d\n", fAggregateDeviceID);
            return noErr;
            
        error:
            DestroyAggregateDevice();
            return -1;
        }

        void DestroyAggregateDevice()
        {   
            if (fAggregateDeviceID > 0) {
                OSStatus osErr = noErr;
                AudioObjectPropertyAddress pluginAOPA;
                pluginAOPA.mSelector = kAudioPlugInDestroyAggregateDevice;
                pluginAOPA.mScope = kAudioObjectPropertyScopeGlobal;
                pluginAOPA.mElement = kAudioObjectPropertyElementMaster;
                UInt32 outDataSize;
                if (fAggregatePluginID > 0)   {
                    osErr = AudioObjectGetPropertyDataSize(fAggregatePluginID, &pluginAOPA, 0, NULL, &outDataSize);
                    if (osErr != noErr) {
                        //printf("TCoreAudioRenderer::DestroyAggregateDevice : AudioObjectGetPropertyDataSize error\n");
                        //printError(osErr);
                    }
                    osErr = AudioObjectGetPropertyData(fAggregatePluginID, &pluginAOPA, 0, NULL, &outDataSize, &fAggregateDeviceID);
                    if (osErr != noErr) {
                        //printf("TCoreAudioRenderer::DestroyAggregateDevice : AudioObjectGetPropertyData error\n");
                        //printError(osErr);
                    }
                }
            }
        }

        OSStatus GetDeviceNameFromID(AudioDeviceID id, char* name)
        {
            UInt32 size = 256;
            return AudioDeviceGetProperty(id, 0, false, kAudioDevicePropertyDeviceName, &size, name);
        }
        
        int SetupBufferSize(int buffer_size)
        {
            // Setting buffer size
            OSStatus err = noErr;
            UInt32 current_buffer_size = buffer_size;
            UInt32 outSize; 
            AudioValueRange buffer_size_range;
            
            outSize = sizeof(AudioValueRange);
            err = AudioDeviceGetProperty(fDeviceID, 0, true, kAudioDevicePropertyBufferFrameSizeRange, &outSize, &buffer_size_range);
            if (err != noErr) {
                printf("Cannot get buffer size range\n");
                printError(err);
                return -1;
            } else {
                //printf("SetupBufferSize : buffer size range min = %ld max = %ld\n", (int)buffer_size_range.mMinimum, (int)buffer_size_range.mMaximum);
            }
            
            outSize = sizeof(UInt32);
            err = AudioDeviceGetProperty(fDeviceID, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyBufferFrameSize, &outSize, &current_buffer_size);
            if (err != noErr) {
                printf("Cannot get buffer size\n");
                printError(err);
                return -1;
            } else {
                //printf("SetupBufferSize : current buffer size %ld\n", current_buffer_size);
            }

            // If needed, set new buffer size
            if (buffer_size != current_buffer_size && buffer_size >= (int)buffer_size_range.mMinimum && buffer_size <= (int)buffer_size_range.mMaximum) {
                current_buffer_size = buffer_size;

                // To get BS change notification
                err = AudioDeviceAddPropertyListener(fDeviceID, 0, true, kAudioDevicePropertyBufferFrameSize, BSNotificationCallback, this);
                if (err != noErr) {
                    printf("Error calling AudioDeviceAddPropertyListener with kAudioDevicePropertyBufferFrameSize\n");
                    printError(err);
                    return -1;
                }

                // Waiting for BS change notification
                int count = 0;
                fState = false;

                err = AudioDeviceSetProperty(fDeviceID, NULL, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyBufferFrameSize, outSize, &current_buffer_size);
                if (err != noErr) {
                    printf("SetupBufferSize : cannot set buffer size = %ld\n", current_buffer_size);
                    printError(err);
                    goto error;
                }

                while (!fState && count++ < WAIT_NOTIFICATION_COUNTER) {
                    usleep(100000);
                    //printf("SetupBufferSize : wait count = %d\n", count);
                }

                if (count >= WAIT_NOTIFICATION_COUNTER) {
                    printf("Did not get buffer size notification...\n");
                    goto error;
                }

                // Check new buffer size
                outSize = sizeof(UInt32);
                err = AudioDeviceGetProperty(fDeviceID, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyBufferFrameSize, &outSize, &current_buffer_size);
                if (err != noErr) {
                    printf("Cannot get current buffer size\n");
                    printError(err);
                } else {
                    //printf("SetupBufferSize : checked buffer size = %ld\n", current_buffer_size);
                }

                // Remove BS change notification
                AudioDeviceRemovePropertyListener(fDeviceID, 0, true, kAudioDevicePropertyBufferFrameSize, BSNotificationCallback);
            } else {
                //printf("Keep current buffer size = %ld\n", current_buffer_size);
            }
            
            fBufferSize = current_buffer_size;
            return 0;

        error:

            // Remove BS change notification
            AudioDeviceRemovePropertyListener(fDeviceID, 0, true, kAudioDevicePropertyBufferFrameSize, BSNotificationCallback);
            return -1;
        }
        
        static OSStatus BSNotificationCallback(AudioDeviceID inDevice,
                                                         UInt32 inChannel,
                                                         Boolean isInput,
                                                         AudioDevicePropertyID inPropertyID,
                                                         void* inClientData)
        {
            TCoreAudioRenderer* driver = (TCoreAudioRenderer*)inClientData;
        
            switch (inPropertyID) {

                case kAudioDevicePropertyBufferFrameSize: {
                    //printf("BSNotificationCallback kAudioDevicePropertyBufferFrameSize\n");
                    // Check new buffer size
                    UInt32 current_buffer_size;
                    UInt32 outSize = sizeof(UInt32);
                    OSStatus err = AudioDeviceGetProperty(inDevice, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyBufferFrameSize, &outSize, &current_buffer_size);
                    if (err != noErr) {
                        printf("Cannot get current buffer size\n");
                        printError(err);
                    } else {
                        //printf("BSNotificationCallback : checked current buffer size = %d\n", current_buffer_size);
                    }
                    driver->fState = true;
                    break;
                }
            }

            return noErr;
        }

        int SetupSampleRateAux(AudioDeviceID inDevice, int& sample_rate)
        {
            OSStatus err = noErr;
            UInt32 outSize = sizeof(Float64);
            Float64 sampleRate = GetNominalSampleRate(inDevice);
            
            if (sample_rate != -1 && sample_rate != (int)sampleRate) {
                sampleRate = (Float64)sample_rate;
                
                // To get SR change notification
                err = AudioDeviceAddPropertyListener(inDevice, 0, true, kAudioDevicePropertyNominalSampleRate, SRNotificationCallback, this);
                if (err != noErr) {
                    printf("Error calling AudioDeviceAddPropertyListener with kAudioDevicePropertyNominalSampleRate\n");
                    printError(err);
                    return -1;
                }
                err = AudioDeviceSetProperty(inDevice, NULL, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyNominalSampleRate, outSize, &sampleRate);
                if (err != noErr) {
                    printf("Cannot set sample rate = %d\n", sample_rate);
                    printError(err);
                    return -1;
                }
                
                // Waiting for SR change notification
                int count = 0;
                while (!fState && count++ < WAIT_NOTIFICATION_COUNTER) {
                    usleep(100000);
                    //printf("Wait count = %d\n", count);
                }
                
                // Check new sample rate
                outSize = sizeof(Float64);
                err = AudioDeviceGetProperty(inDevice, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyNominalSampleRate, &outSize, &sampleRate);
                if (err != noErr) {
                    printf("Cannot get current sample rate\n");
                    printError(err);
                } else {
                    //printf("Checked sample rate = %f\n", sampleRate);
                }
                
                // Remove SR change notification
                AudioDeviceRemovePropertyListener(inDevice, 0, true, kAudioDevicePropertyNominalSampleRate, SRNotificationCallback);
            }
            
            sample_rate = int(sampleRate);
            return 0;
        }   

        static OSStatus Render(void *inRefCon,
                           AudioUnitRenderActionFlags *ioActionFlags,
                           const AudioTimeStamp *inTimeStamp,
                           UInt32 inBusNumber,
                           UInt32 inNumberFrames,
                           AudioBufferList *ioData)
        {
            return static_cast<TCoreAudioRendererPtr>(inRefCon)->Render(ioActionFlags, inTimeStamp, inNumberFrames, ioData);
        }

        static OSStatus SRNotificationCallback(AudioDeviceID inDevice,
                                            UInt32 inChannel,
                                            Boolean	isInput,
                                            AudioDevicePropertyID inPropertyID,
                                               void* inClientData)
        {
            TCoreAudioRenderer* driver = (TCoreAudioRenderer*)inClientData;
            
            switch (inPropertyID) {
                    
                case kAudioDevicePropertyNominalSampleRate: {
                    //printf("SRNotificationCallback kAudioDevicePropertyNominalSampleRate\n");
                    driver->fState = true;
                    // Check new sample rate
                    Float64 sampleRate;
                    UInt32 outSize = sizeof(Float64);
                    OSStatus err = AudioDeviceGetProperty(inDevice, 0, kAudioDeviceSectionGlobal, kAudioDevicePropertyNominalSampleRate, &outSize, &sampleRate);
                    if (err != noErr) {
                        printf("Cannot get current sample rate\n");
                        printError(err);
                    } else {
                        //printf("SRNotificationCallback : checked sample rate = %f\n", sampleRate);
                    }
                    break;
                }
            }
            
            return noErr;
        }

        virtual OSStatus Render(AudioUnitRenderActionFlags *ioActionFlags,
                            const AudioTimeStamp *inTimeStamp,
                            UInt32 inNumberFrames,
                            AudioBufferList *ioData)
        {
            OSStatus err = noErr;
            if (fDevNumInChans > 0) {
                err = AudioUnitRender(fAUHAL, ioActionFlags, inTimeStamp, 1, inNumberFrames, fInputData);
            }
            if (err == noErr) {
                for (int i = 0; i < fDevNumInChans; i++) {
                    fInChannel[i] = (float*)fInputData->mBuffers[i].mData;
                }
                for (int i = 0; i < fDevNumOutChans; i++) {
                    fOutChannel[i] = (float*)ioData->mBuffers[i].mData;
                }
            #ifdef HAS_MATH_EXCEPTION
                TRY_FPE
            #endif
                fDSP->compute(double(AudioConvertHostTimeToNanos(inTimeStamp->mHostTime))/1000., inNumberFrames, fInChannel, fOutChannel);
            #ifdef HAS_MATH_EXCEPTION
                CATCH_FPE
            #endif
                fAudio->runControlCallbacks();
            } else {
                printf("AudioUnitRender error... %x\n", fInputData);
                printError(err);
            }
            return err;
        }
        
    public:
    
        TCoreAudioRenderer(audio* audio)
            :fAggregateDeviceID(-1),fAggregatePluginID(-1),
            fDevNumInChans(0),fDevNumOutChans(0),
            fPhysicalInputs(0), fPhysicalOutputs(0),
            fInChannel(0),fOutChannel(0),
            fBufferSize(0),fSampleRate(0),
            fIsInJackDevice(false),
            fIsOutJackDevice(false),
            fDSP(0),
            fAudio(audio),
            fInputData(0),
            fDeviceID(0),fAUHAL(0),
            fState(false)
        {}

        virtual ~TCoreAudioRenderer()
        {}
        
        int GetBufferSize() {return fBufferSize;}
        int GetSampleRate() {return fSampleRate;}
        
        static OSStatus RestartProc(AudioObjectID objectID, UInt32 numberAddresses,
                                   const AudioObjectPropertyAddress inAddresses[],
                                   void *clientData) 
        {
            /*
            TCoreAudioRenderer* renderer = (TCoreAudioRenderer*)clientData;
            AudioDeviceID defaultDevice;
            UInt32 theSize = sizeof(UInt32);
            OSStatus res;
            char device_name[256];
            
            // Test if new device is "JackRouter"
            if (inAddresses[0].mSelector == kAudioHardwarePropertyDefaultInputDevice) {
                
                if ((res = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice,
                                                    &theSize, &defaultDevice)) == noErr) {
                    renderer->GetDeviceNameFromID(defaultDevice, device_name);
                    renderer->fIsInJackDevice = strcmp(device_name, "JackRouter") == 0;
                }
                
            } else  if (inAddresses[0].mSelector == kAudioHardwarePropertyDefaultOutputDevice) {
                
                if ((res = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice,
                                                    &theSize, &defaultDevice)) == noErr) {
                    renderer->GetDeviceNameFromID(defaultDevice, device_name);
                    renderer->fIsOutJackDevice = strcmp(device_name, "JackRouter") == 0;
                }
                
            }
            
            // Switch only of input and output are "JackRouter"
            if (renderer->fIsInJackDevice && renderer->fIsOutJackDevice || !renderer->fIsInJackDevice && !renderer->fIsOutJackDevice) {
                renderer->Stop();
                renderer->Close();
                int sampleRate = -1; // Use the current sample rate
                int bufferSize = (renderer->fBufferSize > 0) ? renderer->fBufferSize : 512; // Use default if needed
                renderer->OpenDefault(renderer->fDSP, renderer->fDevNumInChans, renderer->fDevNumOutChans, bufferSize, sampleRate);
                renderer->Start();
            }
            */
            return 0;
        }
    
        int OpenDefault(dsp* DSP, int inChan, int outChan, int bufferSize, int& sampleRate)
        {
            fDevNumInChans = 0;
            fDevNumOutChans = 0;
            fInChannel = 0;
            fOutChannel = 0;
            fBufferSize = 0;
            fSampleRate = 0;
            fDSP = 0;
            fInputData = 0;
            fDeviceID = 0; 
            fAUHAL = 0;
            fState = false;
            return OpenDefault(inChan, outChan, bufferSize, sampleRate);
        }
    
        int OpenDefault(int inChan, int outChan, int buffer_size, int& sample_rate)
        {
            OSStatus err;
            UInt32 outSize;
            UInt32 enableIO;
            Boolean isWritable;
            AudioStreamBasicDescription srcFormat, dstFormat, sampleRate;
            
            fDevNumInChans = inChan;
            fDevNumOutChans = outChan;
            
            fInChannel = new float*[fDevNumInChans];
            fOutChannel = new float*[fDevNumOutChans];
            
            //printf("OpenDefault inChan = %ld outChan = %ld bufferSize = %ld sample_rate = %ld\n", inChan, outChan, bufferSize, sample_rate);
            
            SInt32 major;
            SInt32 minor;
            Gestalt(gestaltSystemVersionMajor, &major);
            Gestalt(gestaltSystemVersionMinor, &minor);
            
            // Starting with 10.6 systems, the HAL notification thread is created internally
            if (major == 10 && minor >= 6) {
                CFRunLoopRef theRunLoop = NULL;
                AudioObjectPropertyAddress theAddress = { kAudioHardwarePropertyRunLoop, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster };
                OSStatus osErr = AudioObjectSetPropertyData (kAudioObjectSystemObject, &theAddress, 0, NULL, sizeof(CFRunLoopRef), &theRunLoop);
                if (osErr != noErr) {
                    printf("TCoreAudioRenderer::Open kAudioHardwarePropertyRunLoop error\n");
                    printError(osErr);
                }
            }
             
            if (GetDefaultDeviceAndSampleRate(inChan, outChan, sample_rate, &fDeviceID) != noErr) {
                printf("Cannot open default device\n");
                return OPEN_ERR;
            }
             
            // Setting buffer size
            if (SetupBufferSize(buffer_size) < 0) {
                return OPEN_ERR;
            }
            
            // fBufferSize now has the real value, either 'bufferSize' (if could be changed) or driver current one
            
            // AUHAL
        
        #ifdef MAC_OS_X_VERSION_10_5
            ComponentDescription cd = {kAudioUnitType_Output, kAudioUnitSubType_HALOutput, kAudioUnitManufacturer_Apple, 0, 0};
            Component HALOutput = FindNextComponent(NULL, &cd);
            err = OpenAComponent(HALOutput, &fAUHAL);
            if (err != noErr) {
                printf("Error calling OpenAComponent\n");
                printError(err);
                goto error;
            }
        #else 
            AudioComponentDescription cd = {kAudioUnitType_Output, kAudioUnitSubType_HALOutput, kAudioUnitManufacturer_Apple, 0, 0};
            AudioComponent HALOutput = AudioComponentFindNext(NULL, &cd);
            err = AudioComponentInstanceNew(HALOutput, &fAUHAL);
            if (err != noErr) {
                printf("Error calling AudioComponentInstanceNew\n");
                printError(err);
                goto error;
            }
        #endif
            
            err = AudioUnitInitialize(fAUHAL);
            if (err != noErr) {
                printf("Cannot initialize AUHAL unit\n");
                printError(err);
                goto error;
            }
            
            if (inChan > 0) {
                enableIO = 1;
                //printf("OpenAUHAL : setup AUHAL input on\n");
            } else {
                enableIO = 0;
                //printf("OpenAUHAL : setup AUHAL input off\n");
            }
            
            err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &enableIO, sizeof(enableIO));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input\n");
                printError(err);
                goto error;
            }
                
            if (outChan > 0) {
                enableIO = 1;
                //printf("OpenAUHAL : setup AUHAL output on\n");
            } else {
                enableIO = 0;
                //printf("OpenAUHAL : setup AUHAL output off\n");
            }
            
            err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &enableIO, sizeof(enableIO));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output\n");
                printError(err);
                goto error;
            }
            
            AudioDeviceID currAudioDeviceID;
            outSize = sizeof(AudioDeviceID);
            err = AudioUnitGetProperty(fAUHAL, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &currAudioDeviceID, &outSize);
            if (err != noErr) {
                printf("Error calling AudioUnitGetProperty - kAudioOutputUnitProperty_CurrentDevice\n");
                printError(err);
                goto error;
            } else {
                //printf("AudioUnitGetPropertyCurrentDevice = %d\n", currAudioDeviceID);
            }
        
            // Setup up choosen device, in both input and output cases
            err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &fDeviceID, sizeof(AudioDeviceID));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_CurrentDevice\n");
                printError(err);
                goto error;
            }
            
            if (inChan > 0) {
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 1, (UInt32*)&fBufferSize, sizeof(UInt32));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_MaximumFramesPerSlice\n");
                    printError(err);
                    goto error;
                }
            }
            
            if (outChan > 0) {
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, (UInt32*)&fBufferSize, sizeof(UInt32));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_MaximumFramesPerSlice\n");
                    printError(err);
                    goto error;
                }
            }
            
            err = AudioUnitGetPropertyInfo(fAUHAL, kAudioOutputUnitProperty_ChannelMap, kAudioUnitScope_Input, 1, &outSize, &isWritable);
            if (err != noErr) {
                //printf("Error calling AudioUnitGetPropertyInfo - kAudioOutputUnitProperty_ChannelMap 1\n");
                //printError(err);
            } else {
                fPhysicalInputs = outSize / sizeof(SInt32);
                //printf("fPhysicalInputs = %ld\n", fPhysicalInputs);
            }
                    
            err = AudioUnitGetPropertyInfo(fAUHAL, kAudioOutputUnitProperty_ChannelMap, kAudioUnitScope_Output, 0, &outSize, &isWritable);
            if (err != noErr) {
                //printf("Error calling AudioUnitGetPropertyInfo - kAudioOutputUnitProperty_ChannelMap 0\n");
                //printError(err);
            } else {
                fPhysicalOutputs = outSize / sizeof(SInt32);
                //printf("fPhysicalOutputs = %ld\n", fPhysicalOutputs);
            }
            
            /*
             Just ignore this case : seems to work without any further change...
             
             if (outChan > fPhysicalOutputs) {
                printf("This device hasn't required output channels\n");
                goto error;
             }
             if (inChan > fPhysicalInputs) {
                printf("This device hasn't required input channels\n");
                goto error;
             }
             */
            
            if (inChan < fPhysicalInputs) {
                SInt32 chanArr[fPhysicalInputs];
                for (int i = 0; i < fPhysicalInputs; i++) {
                    chanArr[i] = -1;
                }
                for (int i = 0; i < inChan; i++) {
                    chanArr[i] = i;
                }
                AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_ChannelMap , kAudioUnitScope_Input, 1, chanArr, sizeof(SInt32) * fPhysicalInputs);
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_ChannelMap 1\n");
                    printError(err);
                }
            }
            
            if (outChan < fPhysicalOutputs) {
                SInt32 chanArr[fPhysicalOutputs];
                for (int i = 0;	i < fPhysicalOutputs; i++) {
                    chanArr[i] = -1;
                }
                for (int i = 0; i < outChan; i++) {
                    chanArr[i] = i;
                }
                err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_ChannelMap, kAudioUnitScope_Output, 0, chanArr, sizeof(SInt32) * fPhysicalOutputs);
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_ChannelMap 0\n");
                    printError(err);
                }
            }
            
            if (inChan > 0) {
                outSize = sizeof(AudioStreamBasicDescription);
                err = AudioUnitGetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &srcFormat, &outSize);
                if (err != noErr) {
                    printf("Error calling AudioUnitGetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
                //PrintStreamDesc(&srcFormat);
                
                srcFormat.mSampleRate = sample_rate;
                srcFormat.mFormatID = kAudioFormatLinearPCM;
                srcFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
                srcFormat.mBytesPerPacket = sizeof(float);
                srcFormat.mFramesPerPacket = 1;
                srcFormat.mBytesPerFrame = sizeof(float);
                srcFormat.mChannelsPerFrame = inChan;
                srcFormat.mBitsPerChannel = 32;
                
                //PrintStreamDesc(&srcFormat);
                
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &srcFormat, sizeof(AudioStreamBasicDescription));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
            }
            
            if (outChan > 0) {
                outSize = sizeof(AudioStreamBasicDescription);
                err = AudioUnitGetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &dstFormat, &outSize);
                if (err != noErr) {
                    printf("Error calling AudioUnitGetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
                //PrintStreamDesc(&dstFormat);
                
                dstFormat.mSampleRate = sample_rate;
                dstFormat.mFormatID = kAudioFormatLinearPCM;
                dstFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
                dstFormat.mBytesPerPacket = sizeof(float);
                dstFormat.mFramesPerPacket = 1;
                dstFormat.mBytesPerFrame = sizeof(float);
                dstFormat.mChannelsPerFrame = outChan;
                dstFormat.mBitsPerChannel = 32;
                
                //PrintStreamDesc(&dstFormat);
                
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &dstFormat, sizeof(AudioStreamBasicDescription));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
            }
            
            if (inChan > 0 && outChan == 0) {
                AURenderCallbackStruct output;
                output.inputProc = Render;
                output.inputProcRefCon = this;
                err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 0, &output, sizeof(output));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_SetRenderCallback 1\n");
                    printError(err);
                    goto error;
                }
            } else {
                AURenderCallbackStruct output;
                output.inputProc = Render;
                output.inputProcRefCon = this;
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &output, sizeof(output));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_SetRenderCallback 0\n");
                    printError(err);
                    goto error;
                }
            }
            
            if (inChan > 0) {
                fInputData = (AudioBufferList*)malloc(sizeof(UInt32) + inChan * sizeof(AudioBuffer));
                assert(fInputData);
                fInputData->mNumberBuffers = inChan;
                
                // Prepare buffers
                for (int i = 0; i < inChan; i++) {
                    fInputData->mBuffers[i].mNumberChannels = 1;
                    fInputData->mBuffers[i].mData = malloc(fBufferSize * sizeof(float));
                    assert(fInputData->mBuffers[i].mData),
                    fInputData->mBuffers[i].mDataByteSize = fBufferSize * sizeof(float);
                }
            }
            
            AudioObjectPropertyAddress property_address;
            property_address.mScope = kAudioObjectPropertyScopeGlobal;
            property_address.mElement = kAudioObjectPropertyElementMaster;
            
            property_address.mSelector = kAudioHardwarePropertyDefaultInputDevice;
            if (AudioObjectAddPropertyListener(kAudioObjectSystemObject, &property_address, RestartProc, this)) {
                printf("AudioObjectAddPropertyListener() failed\n");
                return OPEN_ERR;
            } else {
                //printf("AudioObjectAddPropertyListener() OK\n");
            }
            
            property_address.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
            if (AudioObjectAddPropertyListener(kAudioObjectSystemObject, &property_address, RestartProc, this)) {
                printf("AudioObjectAddPropertyListener() failed\n");
                return OPEN_ERR;
            } else {
                //printf("AudioObjectAddPropertyListener() OK\n");
            }
             
            return NO_ERR;
            
        error:
            AudioUnitUninitialize(fAUHAL);
            CloseComponent(fAUHAL);
            fAUHAL = 0;
            return OPEN_ERR;
        }
        
        int Close()
        {
            if (!fAUHAL) {
                return CLOSE_ERR;
            }
            
            for (int i = 0; i < fDevNumInChans; i++) {
                free(fInputData->mBuffers[i].mData);
            }
            if (fInputData) {
                free(fInputData);
            }
            AudioUnitUninitialize(fAUHAL);
            CloseComponent(fAUHAL);
            fAUHAL = NULL;
            
            DestroyAggregateDevice();
            
            delete[] fInChannel;
            delete[] fOutChannel;
            
            AudioObjectPropertyAddress property_address;
            property_address.mScope = kAudioObjectPropertyScopeGlobal;
            property_address.mElement = kAudioObjectPropertyElementMaster;
            
            property_address.mSelector = kAudioHardwarePropertyDefaultInputDevice;
            AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &property_address, RestartProc, this);
            
            property_address.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
            AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &property_address, RestartProc, this);
            
            return NO_ERR;
        }

        int Start()
        {
            if (!fAUHAL) {
                return OPEN_ERR;
            }
            
            OSStatus err = AudioOutputUnitStart(fAUHAL);
            
            if (err != noErr) {
                printf("Error while opening device : device open error \n");
                return OPEN_ERR;
            } else {
                return NO_ERR;
            }
        }
        
        int Stop()
        {
            if (!fAUHAL) {
                return OPEN_ERR;
            }
            
            OSStatus err = AudioOutputUnitStop(fAUHAL);
            
            if (err != noErr) {
                printf("Error while closing device : device close error \n");
                return OPEN_ERR;
            } else {
                return NO_ERR;
            }
        }
    
        void setDsp(dsp* DSP)
        {
            fDSP = DSP;
        }
        
        int GetNumInputs() { return fPhysicalInputs; }
        int GetNumOutputs() { return fPhysicalOutputs; }

};

/******************************************************************************
*******************************************************************************

							CORE AUDIO INTERFACE

*******************************************************************************
*******************************************************************************/
class coreaudio : public audio {

    protected:
        
        TCoreAudioRenderer fAudioDevice;
        int fSampleRate, fBufferSize;

    public:
      
        coreaudio(int srate, int bsize) : fAudioDevice(this), fSampleRate(srate), fBufferSize(bsize) {}
        coreaudio(int bsize) : fAudioDevice(this), fSampleRate(-1), fBufferSize(bsize) {}
        virtual ~coreaudio() { fAudioDevice.Close(); }

        virtual bool init(const char* /*name*/, dsp* DSP) 
        {
            if (fAudioDevice.OpenDefault(DSP, DSP->getNumInputs(), DSP->getNumOutputs(), fBufferSize, fSampleRate) < 0) {
                printf("Cannot open CoreAudio device\n");
                return false;
            }
            fAudioDevice.setDsp(DSP);
            // If -1 was given, fSampleRate will be changed by OpenDefault
            DSP->init(fSampleRate);
            return true;
        }

        virtual bool start() 
        {
            if (fAudioDevice.Start() < 0) {
                printf("Cannot start CoreAudio device\n");
                return false;
            }
            return true;
        }

        virtual void stop() 
        {
            fAudioDevice.Stop();
        }
        
        virtual int getBufferSize() { return fAudioDevice.GetBufferSize(); }
        virtual int getSampleRate() { return fAudioDevice.GetSampleRate(); }
        
        virtual int getNumInputs() { return fAudioDevice.GetNumInputs(); }
        virtual int getNumOutputs() { return fAudioDevice.GetNumOutputs(); }

};

#endif

/********************END ARCHITECTURE SECTION (part 2/2)****************/


/**************************  END  coreaudio-dsp.h **************************/
#elif IOS_DRIVER
/************************** BEGIN coreaudio-ios-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __coreaudio_ios_dsp__
#define __coreaudio_ios_dsp__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <time.h>


#include <AudioToolbox/AudioConverter.h>
#include <AudioToolbox/AudioServices.h>
#include <AudioUnit/AudioUnit.h>

using namespace std;

/******************************************************************************
*******************************************************************************

							COREAUDIO INTERFACE

*******************************************************************************
*******************************************************************************/

#define OPEN_ERR -1
#define NO_ERR 0

class TiPhoneCoreAudioRenderer
{

    protected:

        AudioUnit fAUHAL;

        int	fDevNumInChans;
        int	fDevNumOutChans;

        int fHWNumInChans;
        int fHWNumOutChans;

        dsp* fDSP;
    
        audio* fAudio;

        AudioBufferList* fCAInputData;

        static void PrintStreamDesc(AudioStreamBasicDescription *inDesc)
        {
            printf("- - - - - - - - - - - - - - - - - - - -\n");
            printf("  Sample Rate:%f\n", inDesc->mSampleRate);
            printf("  Format ID:%.*s\n", (int) sizeof(inDesc->mFormatID), (char*)&inDesc->mFormatID);
            printf("  Format Flags:%lX\n", inDesc->mFormatFlags);
            printf("  Bytes per Packet:%ld\n", inDesc->mBytesPerPacket);
            printf("  Frames per Packet:%ld\n", inDesc->mFramesPerPacket);
            printf("  Bytes per Frame:%ld\n", inDesc->mBytesPerFrame);
            printf("  Channels per Frame:%ld\n", inDesc->mChannelsPerFrame);
            printf("  Bits per Channel:%ld\n", inDesc->mBitsPerChannel);
            printf("- - - - - - - - - - - - - - - - - - - -\n");
        }

        static void printError(OSStatus err)
        {
            switch (err) {
                case kAudioConverterErr_FormatNotSupported:
                    printf("error code : kAudioConverterErr_FormatNotSupported\n");
                    break;
                case kAudioConverterErr_OperationNotSupported:
                    printf("error code : kAudioConverterErr_OperationNotSupported\n");
                    break;
                case kAudioConverterErr_PropertyNotSupported:
                    printf("error code : kAudioConverterErr_PropertyNotSupported\n");
                    break;
                case kAudioConverterErr_InvalidInputSize:
                    printf("error code : kAudioConverterErr_InvalidInputSize\n");
                    break;
                case kAudioConverterErr_InvalidOutputSize:
                    printf("error code : kAudioConverterErr_InvalidOutputSize\n");
                    break;
                case kAudioConverterErr_UnspecifiedError:
                    printf("error code : kAudioConverterErr_UnspecifiedError\n");
                    break;
                case kAudioConverterErr_BadPropertySizeError:
                    printf("error code : kAudioConverterErr_BadPropertySizeError\n");
                    break;
                case kAudioConverterErr_RequiresPacketDescriptionsError:
                    printf("error code : kAudioConverterErr_RequiresPacketDescriptionsError\n");
                    break;
                case kAudioConverterErr_InputSampleRateOutOfRange:
                    printf("error code : kAudioConverterErr_InputSampleRateOutOfRange\n");
                    break;
                case kAudioConverterErr_OutputSampleRateOutOfRange:
                    printf("error code : kAudioConverterErr_OutputSampleRateOutOfRange\n");
                    break;
                default:
                    printf("error code : unknown\n");
                    break;
            }
        }

        static OSStatus Render(void *inRefCon,AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData)
        {
            return static_cast<TiPhoneCoreAudioRenderer*>(inRefCon)->Render(ioActionFlags, inTimeStamp, inNumberFrames, ioData);
        }

        OSStatus Render(AudioUnitRenderActionFlags *ioActionFlags,
                        const AudioTimeStamp *inTimeStamp,
                        UInt32 inNumberFrames,
                        AudioBufferList *ioData)
        {
            OSStatus err = noErr;

            if (fDevNumInChans > 0) {
                err = AudioUnitRender(fAUHAL, ioActionFlags, inTimeStamp, 1, inNumberFrames, fCAInputData);
            }

            if (err == noErr) {
                float* fInChannel[fDevNumInChans];
                float* fOutChannel[fDevNumOutChans];

                for (int chan = 0; chan < fDevNumInChans; chan++) {
                    fInChannel[chan] = (float*)fCAInputData->mBuffers[chan].mData;
                }

                for (int chan = 0; chan < fDevNumOutChans; chan++) {
                    fOutChannel[chan] = (float*)ioData->mBuffers[chan].mData;
                }

                fDSP->compute((int)inNumberFrames, fInChannel, fOutChannel);
                
                fAudio->runControlCallbacks();
            }
            return err;
        }

        static void InterruptionListener(void *inClientData, UInt32 inInterruption)
        {
            TiPhoneCoreAudioRenderer *obj = (TiPhoneCoreAudioRenderer*)inClientData;
            printf("Session interrupted! --- %s ---", (inInterruption == kAudioSessionBeginInterruption) ? "Begin Interruption" : "End Interruption");

            if (inInterruption == kAudioSessionEndInterruption) {
                // Make sure we are again the active session
                AudioSessionSetActive(true);
                obj->SetupMixing();
                AudioOutputUnitStart(obj->fAUHAL);
            }

            if (inInterruption == kAudioSessionBeginInterruption) {
                AudioOutputUnitStop(obj->fAUHAL);
            }
        }
    
        int SetupMixing()
        {
            OSStatus err;

            /*
            01/07/2014 : cause iRig to fail, so deactivated for now...
            CFStringRef route;
            UInt32 routesize = sizeof(route);
            OSStatus err  = AudioSessionGetProperty(kAudioSessionProperty_AudioRoute, &routesize, &route);
            if (err == noErr) {
                if (CFStringCompare(route, CFSTR("ReceiverAndMicrophone"), 0) == kCFCompareEqualTo || CFStringCompare(route,CFSTR("Receiver"), 0) == kCFCompareEqualTo) {
                    // Re-route audio to the speaker (not the receiver, which no music app will ever want)
                    printf("Rerouting audio to speaker\n");
                    UInt32 newRoute = kAudioSessionOverrideAudioRoute_Speaker;
                    AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(newRoute), &newRoute);
                }
             }
            */

            UInt32 allowMixing = true;
            err = AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryMixWithOthers, sizeof(allowMixing), &allowMixing);
            if (err != noErr) {
                printf("Could not set audio session mixing\n");
                printError(err);
                return -1;
            } else {
                return 0;
            }
        }

        static void AudioSessionPropertyListener(void* inClientData, AudioSessionPropertyID inID, UInt32 inDataSize, const void* inData)
        {
            TiPhoneCoreAudioRenderer *obj = (TiPhoneCoreAudioRenderer*)inData;
            switch (inID) {
                case kAudioSessionProperty_ServerDied: {
                    printf("kAudioSessionProperty_ServerDied\n");
                    break;
                }
                case kAudioSessionProperty_AudioRouteChange: {
                    printf("kAudioSessionProperty_AudioRouteChange\n");
                    obj->SetupMixing();
                    break;
                }
                case kAudioSessionProperty_AudioInputAvailable: {
                    printf("kAudioSessionProperty_AudioInputAvailable\n");
                    obj->SetupMixing();
                    break;
                }
            }
        }

        static int SetAudioCategory(int input, int output)
        {
            // Set the audioCategory the way Faust DSP wants
            UInt32 audioCategory;
            if ((input > 0) && (output > 0)) {
                audioCategory = kAudioSessionCategory_PlayAndRecord;
                printf("AudioCategory kAudioSessionCategory_PlayAndRecord\n");
            } else if (input > 0) {
                audioCategory = kAudioSessionCategory_RecordAudio;
                printf("AudioCategory kAudioSessionCategory_RecordAudio\n");
            } else  if (output > 0) {
                audioCategory = kAudioSessionCategory_MediaPlayback;
                printf("AudioCategory kAudioSessionCategory_MediaPlayback\n");
            }

            OSStatus err = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);
            if (err != noErr) {
                printf("Couldn't set audio category\n");
                printError(err);
                return OPEN_ERR;
            }

            // 09/07/2015 : https://developer.apple.com/library/ios/qa/qa1754/_index.html
            if (audioCategory == kAudioSessionCategory_PlayAndRecord) {
                
                // 21/09/2017 Compatible with bluetooth devices. Deactivate bluetooth micro which run at 16khz, use internal micro instead.
                /*
                UInt32 overrideAudioRoute = 1;
                err = AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryDefaultToSpeaker, sizeof(UInt32), &overrideAudioRoute);
                if (err != noErr) {
                    printf("Error setting kAudioSessionProperty_OverrideCategoryDefaultToSpeaker\n");
                    printError(err);
                }

                UInt32 allowBluetoothInput = 1;
                err = AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryEnableBluetoothInput, sizeof(UInt32), &allowBluetoothInput);
                if (err != noErr) {
                    printf("Error setting kAudioSessionProperty_OverrideCategoryEnableBluetoothInput\n");
                    printError(err);
                }
                */
                
                // On devices with more than one built-in microphone, the microphone closest to the video camera is used.
                UInt32 allowInternalInput = kAudioSessionMode_VideoRecording;
                err = AudioSessionSetProperty(kAudioSessionProperty_Mode, sizeof(UInt32), &allowInternalInput);
                if (err != noErr) {
                    printf("Error setting kAudioSessionMode_VideoRecording\n");
                    printError(err);
                }

            }

        #if NOAGC
            // If input is used, disable AGC
            if (audioCategory == kAudioSessionCategory_RecordAudio || audioCategory == kAudioSessionCategory_PlayAndRecord) {

                UInt32 sessionMode = kAudioSessionMode_Measurement;
                err = AudioSessionSetProperty(kAudioSessionProperty_Mode, sizeof(sessionMode), &sessionMode);
                if (err != noErr) {
                    printf("Error setting kAudioSessionMode_Measurement\n");
                    printError(err);
                }

                UInt32 availableGain;
                UInt32 outSize = sizeof(availableGain);
                err = AudioSessionGetProperty(kAudioSessionProperty_InputGainAvailable, &outSize, &availableGain);
                if (err != noErr) {
                    printf("Error getting kAudioSessionProperty_InputGainAvailable\n");
                    printError(err);
                } else {
                    Float32 gain;
                    printf("Getting kAudioSessionProperty_InputGainAvailable OK\n");
                    outSize = sizeof(Float32);
                    AudioSessionGetProperty(kAudioSessionProperty_InputGainScalar, &outSize, &gain);
                    printf("Getting kAudioSessionProperty_InputGainScalar :  %f\n", gain);
                    gain = 1.0f;
                    err = AudioSessionSetProperty(kAudioSessionProperty_InputGainScalar, sizeof(Float32), &gain);
                    if (err != noErr) {
                        printf("Error setting kAudioSessionProperty_InputGainScalar\n");
                        printError(err);
                    } else {
                        printf("Setting kAudioSessionProperty_InputGainAvailable to 1.0 OK\n");
                    }
                }
            }
        #endif

            return NO_ERR;
        }

        int SetParameters(int bufferSize, int samplerate)
        {
            OSStatus err;
            UInt32 outSize;
            UInt32 enableIO;
            AudioStreamBasicDescription srcFormat, dstFormat;

            printf("SetParameters fDevNumInChans = %d fDevNumOutChans = %d bufferSize = %d samplerate = %d\n", fDevNumInChans, fDevNumOutChans, bufferSize, samplerate);

            err = AudioSessionSetActive(true);
            if (err != noErr) {
                printf("Couldn't set audio session active\n");
                printError(err);
                return OPEN_ERR;
            }

            AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, AudioSessionPropertyListener, this);
            AudioSessionAddPropertyListener(kAudioSessionProperty_AudioInputAvailable, AudioSessionPropertyListener, this);
            AudioSessionAddPropertyListener(kAudioSessionProperty_ServerDied, AudioSessionPropertyListener, this);

            if (SetAudioCategory(fDevNumInChans, fDevNumOutChans) != NO_ERR) {
                return OPEN_ERR;
            }

            // Scan Hardware
            outSize = sizeof(fHWNumInChans);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareInputNumberChannels, &outSize, &fHWNumInChans);
            if (err != noErr) {
                fHWNumInChans = 0;
                printf("Couldn't get hw input channels\n");
                printError(err);
            } else {
                printf("Get hw input channels %d\n", fHWNumInChans);
            }

            outSize = sizeof(fHWNumOutChans);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareOutputNumberChannels, &outSize, &fHWNumOutChans);
            if (err != noErr) {
                fHWNumOutChans = 0;
                printf("Couldn't get hw output channels\n");
                printError(err);
            } else {
                printf("Get hw output channels %d\n", fHWNumOutChans);
            }
            /*
            // Possibly reset the audioCategory the way hardware allows
            // 21/09/2017 cause problem when use bluetooth, deactive for now
            if (SetAudioCategory(fHWNumInChans, fHWNumOutChans) != NO_ERR) {
                return OPEN_ERR;
            }
            */
            if (SetupMixing() < 0) {
                return OPEN_ERR;
            }

            Float64 hwSampleRate;
            outSize = sizeof(hwSampleRate);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareSampleRate, &outSize, &hwSampleRate);
            if (err != noErr) {
                printf("Couldn't get hw sample rate\n");
                printError(err);
                return OPEN_ERR;
            } else {
                printf("Get hw sample rate %f\n", hwSampleRate);
            }

            Float32 hwBufferSize;
            outSize = sizeof(hwBufferSize);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, &outSize, &hwBufferSize);
            if (err != noErr) {
                printf("Couldn't get hw buffer duration\n");
                printError(err);
                return OPEN_ERR;
            } else {
                printf("Get hw buffer duration %f\n", hwBufferSize);
            }

            Float32 preferredPeriodDuration = float(bufferSize) / float(samplerate);
            printf("preferredPeriodDuration %f \n", preferredPeriodDuration);

            err = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(preferredPeriodDuration), &preferredPeriodDuration);
            if (err != noErr) {
                printf("Couldn't set i/o buffer duration\n");
                printError(err);
                return OPEN_ERR;
            }

            Float32 actualPeriodDuration;
            outSize = sizeof(actualPeriodDuration);
            err = AudioSessionGetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, &outSize, &actualPeriodDuration);
            if (err != noErr) {
                printf("Couldn't get hw buffer duration\n");
                printError(err);
                return OPEN_ERR;
            }

            printf("preferredPeriodDuration %f actualPeriodDuration %f\n", preferredPeriodDuration, actualPeriodDuration);
            if (preferredPeriodDuration != actualPeriodDuration) {
                printf("Couldn't set hw buffer duration\n");
                return OPEN_ERR;
            }

            Float64 preferredSamplerate = float(samplerate);
            err = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareSampleRate, sizeof(preferredSamplerate), &preferredSamplerate);
            if (err != noErr) {
                printf("Couldn't set i/o sample rate\n");
                printError(err);
                return OPEN_ERR;
            }

            Float32 inputLatency;
            outSize = sizeof(inputLatency);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareInputLatency, &outSize, &inputLatency);
            if (err != noErr) {
                printf("Couldn't get inputLatency\n");
                printError(err);
            } else {
                printf("inputLatency in sec : %f\n", inputLatency);
            }

            Float32 outputLatency;
            outSize = sizeof(outputLatency);
            err = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareOutputLatency, &outSize, &outputLatency);
            if (err != noErr) {
                printf("Couldn't get outputLatency\n");
                printError(err);
            } else {
                printf("outputLatency in sec : %f\n", outputLatency);
            }

            // AUHAL
            AudioComponentDescription cd = {kAudioUnitType_Output, kAudioUnitSubType_RemoteIO, kAudioUnitManufacturer_Apple, 0, 0};
            AudioComponent HALOutput = AudioComponentFindNext(NULL, &cd);

            err = AudioComponentInstanceNew(HALOutput, &fAUHAL);
            if (err != noErr) {
                printf("Error calling OpenAComponent\n");
                printError(err);
                goto error;
            }

            enableIO = 1;
            err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &enableIO, sizeof(enableIO));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output\n");
                printError(err);
                goto error;
            }

            enableIO = 1;
            err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &enableIO, sizeof(enableIO));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input\n");
                printError(err);
                goto error;
            }

            UInt32 maxFPS;
            outSize = sizeof(maxFPS);
            err = AudioUnitGetProperty(fAUHAL, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, &maxFPS, &outSize);
            if (err != noErr) {
                printf("Couldn't get kAudioUnitProperty_MaximumFramesPerSlice\n");
                printError(err);
                goto error;
            } else {
                printf("Get kAudioUnitProperty_MaximumFramesPerSlice %d\n", (unsigned int)maxFPS);
            }

            err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 1, (UInt32*)&bufferSize, sizeof(UInt32));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_MaximumFramesPerSlice\n");
                printError(err);
                goto error;
            }

            err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, (UInt32*)&bufferSize, sizeof(UInt32));
            if (err != noErr) {
                printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_MaximumFramesPerSlice\n");
                printError(err);
                goto error;
            }

            err = AudioUnitInitialize(fAUHAL);
            if (err != noErr) {
                printf("Cannot initialize AUHAL unit\n");
                printError(err);
                goto error;
            }

            // Setting format
            if (fDevNumInChans > 0) {
                outSize = sizeof(AudioStreamBasicDescription);
                err = AudioUnitGetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &srcFormat, &outSize);
                if (err != noErr) {
                    printf("Error calling AudioUnitGetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
                PrintStreamDesc(&srcFormat);

                srcFormat.mFormatID = kAudioFormatLinearPCM;
                srcFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
                srcFormat.mBytesPerPacket = sizeof(AudioUnitSampleType);
                srcFormat.mFramesPerPacket = 1;
                srcFormat.mBytesPerFrame = sizeof(AudioUnitSampleType);
                srcFormat.mChannelsPerFrame = fDevNumInChans;
                srcFormat.mBitsPerChannel = 32;

                PrintStreamDesc(&srcFormat);

                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &srcFormat, sizeof(AudioStreamBasicDescription));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Output\n");
                    printError(err);
                }
            }

            if (fDevNumOutChans > 0) {
                outSize = sizeof(AudioStreamBasicDescription);
                err = AudioUnitGetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &dstFormat, &outSize);
                if (err != noErr) {
                    printf("Error calling AudioUnitGetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Input\n");
                    printError(err);
                }
                PrintStreamDesc(&dstFormat);

                dstFormat.mFormatID = kAudioFormatLinearPCM;
                dstFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
                dstFormat.mBytesPerPacket = sizeof(AudioUnitSampleType);
                dstFormat.mFramesPerPacket = 1;
                dstFormat.mBytesPerFrame = sizeof(AudioUnitSampleType);
                dstFormat.mChannelsPerFrame = fDevNumOutChans;
                dstFormat.mBitsPerChannel = 32;

                PrintStreamDesc(&dstFormat);

                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &dstFormat, sizeof(AudioStreamBasicDescription));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_StreamFormat kAudioUnitScope_Input\n");
                    printError(err);
                }
            }

            if (fDevNumInChans > 0 && fDevNumOutChans == 0) {
                AURenderCallbackStruct output;
                output.inputProc = Render;
                output.inputProcRefCon = this;
                err = AudioUnitSetProperty(fAUHAL, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 0, &output, sizeof(output));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_SetRenderCallback 1\n");
                    printError(err);
                    goto error;
                }
            } else {
                AURenderCallbackStruct output;
                output.inputProc = Render;
                output.inputProcRefCon = this;
                err = AudioUnitSetProperty(fAUHAL, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &output, sizeof(output));
                if (err != noErr) {
                    printf("Error calling AudioUnitSetProperty - kAudioUnitProperty_SetRenderCallback 0\n");
                    printError(err);
                    goto error;
                }
            }

            // Possibly prepare input buffers
            if (fDevNumInChans > 0) {
                fCAInputData = (AudioBufferList*)malloc(sizeof(float) + fDevNumInChans * sizeof(AudioBuffer));
                fCAInputData->mNumberBuffers = fDevNumInChans;
                for (int i = 0; i < fDevNumInChans; i++) {
                    fCAInputData->mBuffers[i].mNumberChannels = 1;
                    fCAInputData->mBuffers[i].mDataByteSize = bufferSize * sizeof(float);
                    fCAInputData->mBuffers[i].mData = malloc(bufferSize * sizeof(float));
                }
            }

            return NO_ERR;

        error:
            AudioUnitUninitialize(fAUHAL);
            AudioComponentInstanceDispose(fAUHAL);
            return OPEN_ERR;
        }

    public:

        TiPhoneCoreAudioRenderer(audio* audio)
            :fAUHAL(0), fDevNumInChans(0), fDevNumOutChans(0),
            fHWNumInChans(0), fHWNumOutChans(0),
            fDSP(0), fAudio(audio), fCAInputData(NULL)
        {}

        virtual ~TiPhoneCoreAudioRenderer()
        {
            if (fCAInputData) {
                for (int i = 0; i < fDevNumInChans; i++) {
                    free(fCAInputData->mBuffers[i].mData);
                }
                free(fCAInputData);
            }
        }
    
        int Open(dsp* dsp, int inChan, int outChan, int buffersize, int samplerate)
        {
            fDSP = dsp;
            fDevNumInChans = inChan;
            fDevNumOutChans = outChan;
      
            // Initialize and configure the audio session
            OSStatus err = AudioSessionInitialize(NULL, NULL, InterruptionListener, this);
            if (err != noErr && err != kAudioSessionAlreadyInitialized) {
                printf("Couldn't initialize audio session\n");
                printError(err);
                return OPEN_ERR;
            }

            if (SetParameters(buffersize, samplerate) < 0) {
                printf("Cannot set parameters to CoreAudio device\n");
                return OPEN_ERR;
            }

            return NO_ERR;
        }

        int Close()
        {
            AudioUnitUninitialize(fAUHAL);
            AudioComponentInstanceDispose(fAUHAL);
            return NO_ERR;
        }

        int Start()
        {
            AudioSessionSetActive(true);

            if (AudioOutputUnitStart(fAUHAL) != noErr) {
                printf("Error while opening device : device open error\n");
                return OPEN_ERR;
            } else {
                return NO_ERR;
            }
        }

        int Stop()
        {
            AudioSessionSetActive(false);

            if (AudioOutputUnitStop(fAUHAL) != noErr) {
                printf("Error while closing device : device close error\n");
                return OPEN_ERR;
            } else {
                return NO_ERR;
            }
        }

        int GetNumInputs() { return fHWNumInChans; }
        int GetNumOutputs() { return fHWNumOutChans; }

};

/******************************************************************************
 *******************************************************************************

                                CORE AUDIO INTERFACE

 *******************************************************************************
 *******************************************************************************/

class iosaudio : public audio {
    
    protected:

        TiPhoneCoreAudioRenderer fAudioDevice;
        int fSampleRate, fBufferSize;
  
    public:

        iosaudio(int srate, int bsize)
        :fAudioDevice(this), fSampleRate(srate), fBufferSize(bsize)
        {}

        virtual ~iosaudio() { fAudioDevice.Close(); }
   
        virtual bool init(const char* /*name*/, dsp* DSP)
        {
            DSP->init(fSampleRate);
            if (fAudioDevice.Open(DSP, DSP->getNumInputs(), DSP->getNumOutputs(), fBufferSize, fSampleRate) < 0) {
                printf("Cannot open iOS audio device\n");
                return false;
            }
            return true;
        }

        virtual bool start()
        {
            if (fAudioDevice.Start() < 0) {
                printf("Cannot start iOS audio device\n");
                return false;
            }
            return true;
        }

        virtual void stop()
        {
            fAudioDevice.Stop();
        }

        virtual int getBufferSize() { return fBufferSize; }
        virtual int getSampleRate() { return fSampleRate; }

        virtual int getNumInputs() { return fAudioDevice.GetNumInputs(); }
        virtual int getNumOutputs() { return fAudioDevice.GetNumOutputs(); }

};

#endif

/********************END ARCHITECTURE SECTION (part 2/2)****************/
/**************************  END  coreaudio-ios-dsp.h **************************/
#elif ANDROID_DRIVER
/************************** BEGIN oboe-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __oboe_dsp__
#define __oboe_dsp__

#include <android/log.h>
#include <oboe/Oboe.h>
#include <time.h>
#include <assert.h>
#include <vector>

/************************** BEGIN dsp-tools.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_tools__
#define __dsp_tools__

#include <assert.h>
#include <string.h>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class Deinterleaver
{
    
    private:
    
        int fNumFrames;
        int fNumInputs;
        int fNumOutputs;
        
        FAUSTFLOAT* fInput;
        FAUSTFLOAT* fOutputs[256];
        
    public:
        
        Deinterleaver(int numFrames, int numInputs, int numOutputs)
        {
            fNumFrames = numFrames;
            fNumInputs = numInputs;
            fNumOutputs = std::max<int>(numInputs, numOutputs);
            
            // allocate interleaved input channel
            fInput = new FAUSTFLOAT[fNumFrames * fNumInputs];
            
            // allocate separate output channels
            for (int i = 0; i < fNumOutputs; i++) {
                fOutputs[i] = new FAUSTFLOAT[fNumFrames];
            }
        }
        
        ~Deinterleaver()
        {
            // free interleaved input channel
            delete [] fInput;
            
            // free separate output channels
            for (int i = 0; i < fNumOutputs; i++) {
                delete [] fOutputs[i];
            }
        }
        
        FAUSTFLOAT* input() { return fInput; }
        
        FAUSTFLOAT** outputs() { return fOutputs; }
        
        void deinterleave()
        {
            for (int s = 0; s < fNumFrames; s++) {
                for (int c = 0; c < fNumInputs; c++) {
                    fOutputs[c][s] = fInput[c + s * fNumInputs];
                }
            }
        }
};

class Interleaver
{
    
    private:
        
        int fNumFrames;
        int fNumInputs;
        int fNumOutputs;
    
        FAUSTFLOAT* fInputs[256];
        FAUSTFLOAT* fOutput;
        
    public:
        
        Interleaver(int numFrames, int numInputs, int numOutputs)
        {
            fNumFrames = numFrames;
            fNumInputs 	= std::max(numInputs, numOutputs);
            fNumOutputs = numOutputs;
            
            // allocate separate input channels
            for (int i = 0; i < fNumInputs; i++) {
                fInputs[i] = new FAUSTFLOAT[fNumFrames];
            }
            
            // allocate interleaved output channel
            fOutput = new FAUSTFLOAT[fNumFrames * fNumOutputs];
        }
        
        ~Interleaver()
        {
            // free separate input channels
            for (int i = 0; i < fNumInputs; i++) {
                delete [] fInputs[i];
            }
            
            // free interleaved output channel
            delete [] fOutput;
        }
        
        FAUSTFLOAT** inputs() { return fInputs; }
        
        FAUSTFLOAT* output() { return fOutput; }
        
        void interleave()
        {
            for (int s = 0; s < fNumFrames; s++) {
                for (int c = 0; c < fNumOutputs; c++) {
                    fOutput[c + s * fNumOutputs] = fInputs[c][s];
                }
            }
        }
};

//=============================================================================
// An AudioChannels is a group of non-interleaved buffers that knows how to read
// from or write to an interleaved buffer. The interleaved buffer may have a
// different number of channels than the AudioChannels internal channels.
//=============================================================================

class AudioChannels
{
    
    protected:
        
        const unsigned int fNumFrames;
        const unsigned int fNumChannels;
        FAUSTFLOAT** fChannels;
        
    public:
        
        AudioChannels(int nframes, int nchannels) : fNumFrames(nframes), fNumChannels(nchannels)
        {
            fChannels = new FAUSTFLOAT*[nchannels];
            
            // allocate audio channels
            for (unsigned int i = 0; i < fNumChannels; i++) {
                fChannels[i] = new FAUSTFLOAT[fNumFrames];
                memset(fChannels[i], 0, sizeof(FAUSTFLOAT) * fNumFrames);
            }
        }
        
        virtual ~AudioChannels()
        {
            // free separate input channels
            for (int i = 0; i < fNumChannels; i++) {
                delete[] fChannels[i];
            }
            delete[] fChannels;
        }
        
        //---------------------------------------------------------------------------------------
        // interleavedRead: read, from the interleaved buffer <inbuffer>, <length> frames on
        // <inchannels> channels. The samples are written to the <fNumChannels> internal
        // <fChannels>.
        void interleavedRead(float* inbuffer, unsigned int length, unsigned int inchannels)
        {
            assert(length <= fNumFrames);
            unsigned int C = std::min<unsigned int>(inchannels, fNumChannels);
            unsigned int F = std::min<unsigned int>(length, fNumFrames);
            
            for (unsigned int f = 0; f < F; f++) {
                unsigned int p = f * inchannels;
                for (unsigned int c = 0; c < C; c++) {
                    fChannels[c][f] = inbuffer[p++];
                }
                for (unsigned int c = C; c < fNumChannels; c++) {
                    fChannels[c][f] = 0;
                }
            }
        }
        
        //----------------------------------------------------------------------------------------
        // interleavedWrite: write to the interleaved buffer <inbuffer>, <length> frames on
        // <outchannels> channels. The samples are read from <fNumChannels> internal
        // <fChannels>.
        void interleavedWrite(float* outbuffer, unsigned int length, unsigned int outchannels)
        {
            assert(length <= fNumFrames);
            unsigned int C = std::min<unsigned int>(outchannels, fNumChannels);
            unsigned int F = std::min<unsigned int>(length, fNumFrames);
            
            for (unsigned int f = 0; f < F; f++) {
                unsigned int p = f * outchannels;
                for (unsigned int c = 0; c < C; c++) {
                    outbuffer[p++] = fChannels[c][f];
                }
                for (unsigned int c = C; c < outchannels; c++) {
                    outbuffer[p++] = 0;
                }
            }
        }
        
        //----------------------------------------------------------------------------------------
        // buffers: the internal buffers ready to use in the compute() method of a Faust dsp
        
        FAUSTFLOAT** buffers() { return fChannels; }
};

#endif
/**************************  END  dsp-tools.h **************************/

#define CPU_TABLE_SIZE 16

//http://stackoverflow.com/questions/17188761/how-to-obtain-computation-time-in-ndk

class oboeaudio : public audio, public oboe::AudioStreamCallback {
    
    protected:
    
        dsp* fDSP;
    
        int32_t fSampleRate;
        int32_t fBufferSize;
    
        oboe::ManagedStream fInputStream;
        oboe::ManagedStream fOutputStream;
    
        int64_t fCPUTable[CPU_TABLE_SIZE];
        int64_t fCPUTableIndex;
    
        AudioChannels* fInputs;
        AudioChannels* fOutputs;
    
        std::vector<int> fCpuIds;
        bool fIsThreadAffinitySet;
    
        void setThreadAffinity()
        {
            pid_t current_thread_id = gettid();
            cpu_set_t cpu_set;
            CPU_ZERO(&cpu_set);
            
            // If the callback cpu ids aren't specified then bind to the current cpu
            if (fCpuIds.empty()) {
                int current_cpu_id = sched_getcpu();
                __android_log_print(ANDROID_LOG_ERROR, "Faust", "Binding to current CPU ID %d", current_cpu_id);
                CPU_SET(current_cpu_id, &cpu_set);
            } else {
                __android_log_print(ANDROID_LOG_ERROR, "Faust", "Binding to %d CPU IDs", static_cast<int>(fCpuIds.size()));
                for (size_t i = 0; i < fCpuIds.size(); i++) {
                    int cpu_id = fCpuIds.at(i);
                    __android_log_print(ANDROID_LOG_ERROR, "Faust", "CPU ID %d added to cores set", cpu_id);
                    CPU_SET(cpu_id, &cpu_set);
                }
            }
            
            int res = sched_setaffinity(current_thread_id, sizeof(cpu_set_t), &cpu_set);
            if (res == 0) {
                __android_log_print(ANDROID_LOG_ERROR, "Faust", "Thread affinity set");
            } else {
                __android_log_print(ANDROID_LOG_ERROR, "Faust", "Error setting thread affinity error no: %d", res);
            }
            
            fIsThreadAffinitySet = true;
        }
  
        int64_t getTimeUsec() 
        {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            return ((int64_t) now.tv_sec * 1000000000LL + now.tv_nsec)/1000;
        }
    
        oboe::DataCallbackResult onAudioReady(oboe::AudioStream* audioStream, void* outbuffer, int32_t framesWrite)
        {
            fBufferSize = framesWrite;
            int64_t t1 = getTimeUsec();
            if (!fIsThreadAffinitySet) setThreadAffinity();
            
            // Duplex case
            if (fDSP->getNumInputs() > 0) {
                
                // Read inputs
                float* inbuffer = static_cast<float*>(alloca(fDSP->getNumInputs() * sizeof(float) * framesWrite));
                oboe::ResultWithValue<int32_t> res = fInputStream->read(inbuffer, framesWrite, 0);
                
                if (res) {
                    int32_t framesRead = res.value();
                    fInputs->interleavedRead(inbuffer, framesWrite, fDSP->getNumInputs());
                
                    // Call compute only when 'read frames' == 'frames to write'
                    if (framesRead == framesWrite) {
                        fDSP->compute(framesWrite, fInputs->buffers(), fOutputs->buffers());
                        fOutputs->interleavedWrite(static_cast<float*>(outbuffer), framesWrite, fDSP->getNumOutputs());
                    } else {
                        __android_log_print(ANDROID_LOG_ERROR, "Faust", "onAudioReady framesRead %d framesWrite %d", framesRead, framesWrite);
                    }
                    
                } else {
                    __android_log_print(ANDROID_LOG_ERROR, "Faust", "onAudioReady fInputStream->read fails");
                }
                
            } else {
                
                // Output only
                fDSP->compute(framesWrite, nullptr, fOutputs->buffers());
                fOutputs->interleavedWrite(static_cast<float*>(outbuffer), framesWrite, fDSP->getNumOutputs());
            }
            
            runControlCallbacks();
            
            int64_t t2 = getTimeUsec();
            fCPUTable[(fCPUTableIndex++)&(CPU_TABLE_SIZE-1)] = t2 - t1;
            return oboe::DataCallbackResult::Continue;
        }
 
    public:
    
        oboeaudio(long srate):
        fDSP(NULL),
        fSampleRate(srate),
        fBufferSize(512),
        fCPUTableIndex(0),
        fInputs(nullptr),
        fOutputs(nullptr),
        fIsThreadAffinitySet(false)
        {}

        virtual ~oboeaudio()
        {
            delete fInputs;
            delete fOutputs;
        }
    
        virtual bool init(const char* name, dsp* DSP)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Faust", "init");
            fDSP = DSP;
            oboe::AudioStreamBuilder builder;
            
            // Possibly set SR
            if (fSampleRate != -1) {
                builder.setSampleRate(fSampleRate);
            }
            
            if (fDSP->getNumInputs() > 0) {
                if (builder.setDirection(oboe::Direction::Input)
                    ->setFormat(oboe::AudioFormat::Float)
                    ->setChannelCount(fDSP->getNumInputs())
                    ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                    ->setSharingMode(oboe::SharingMode::Exclusive)
                    ->openManagedStream(fInputStream) != oboe::Result::OK) return false;
                // Allocate the stream adapter
                fInputs = new AudioChannels(4096, fDSP->getNumInputs());
                // Keep the input SR
                fSampleRate = fInputStream->getSampleRate();
            } else {
                fInputs = nullptr;
            }
            
            if (fDSP->getNumOutputs() > 0) {
                
                // Common setup
                builder.setDirection(oboe::Direction::Output)
                    ->setCallback(this)
                    ->setFormat(oboe::AudioFormat::Float)
                    ->setChannelCount(fDSP->getNumOutputs())
                    ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                    ->setSharingMode(oboe::SharingMode::Exclusive);
                
                // If fSampleRate was given at initialisation time, or has been set when opening the input
                if (fSampleRate != -1) {
                    // Force output to use the same SR as input, and possibly setup resampler
                    if (builder.setSampleRate(fSampleRate)
                        ->setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Fastest)
                        ->openManagedStream(fOutputStream) != oboe::Result::OK) return false;
                } else {
                    // Open with the native SR
                    if (builder.openManagedStream(fOutputStream) != oboe::Result::OK) return false;
                }
                
                // Allocate the stream adapter
                fOutputs = new AudioChannels(4096, fDSP->getNumOutputs());
                // Keep the output SR
                fSampleRate = fOutputStream->getSampleRate();
            } else {
                fOutputs = nullptr;
            }
            
            // Init with the stream SR
            fDSP->init(fSampleRate);
            __android_log_print(ANDROID_LOG_ERROR, "Faust", "init fSampleRate %d", fSampleRate);
            return true;
        }
    
        virtual bool start()
        {
            // Start output first
            if (fDSP->getNumOutputs() > 0) {
                if (fOutputStream->requestStart() != oboe::Result::OK) return false;
            }
            if (fDSP->getNumInputs() > 0) {
                if (fInputStream->requestStart() != oboe::Result::OK) return false;
            }
            return true;
        }
        
        virtual void stop()
        {
            // Stop output first (since the output stream calls the onAudioReady callback)
            if (fDSP->getNumOutputs() > 0) {
                fOutputStream->requestStop();
                fOutputStream->close();
            }
            if (fDSP->getNumInputs() > 0) {
                fInputStream->requestStop();
                fInputStream->close();
            }
        }
    
        virtual int getBufferSize()
        {
            return fBufferSize;
        }
        
        virtual int getSampleRate()
        {
            return fSampleRate;
        }
        
        virtual int getNumInputs()
        {
            return fDSP->getNumInputs();
        }
        
        virtual int getNumOutputs()
        {
            return fDSP->getNumOutputs();
        }
    
        // Returns the average proportion of available CPU being spent inside the audio callbacks (between 0 and 1.0).
        float getCPULoad()
        {
            float sum = 0.f;
            for (int i = 0; i < CPU_TABLE_SIZE; i++) {
                sum += fCPUTable[i];
            }
            return (sum/float(CPU_TABLE_SIZE))/(1000000.f*float(fBufferSize)/float(fSampleRate));
        }

};

#endif
/**************************  END  oboe-dsp.h **************************/
#elif ALSA_DRIVER
/************************** BEGIN alsa-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __alsa_dsp__
#define __alsa_dsp__

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>

#include <alsa/asoundlib.h>

/**
DEFAULT ALSA PARAMETERS CONTROLLED BY ENVIRONMENT VARIABLES

Some default parameters of Faust's ALSA applications are controlled by the following environment variables :

    FAUST2ALSA_DEVICE   = "hw:0"
    FAUST2ALSA_FREQUENCY= 44100
    FAUST2ALSA_BUFFER   = 512
    FAUST2ALSA_PERIODS  = 2
*/

// handle 32/64 bits int size issues

#ifdef __x86_64__

#define uint32	unsigned int
#define uint64	unsigned long int

#define int32	int
#define int64	long int

#else

#define uint32	unsigned int
#define uint64	unsigned long long int

#define int32	int
#define int64	long long int
#endif

// check 32/64 bits issues are correctly handled

#define check_error(err) if (err) { printf("%s:%d, alsa error %d : %s\n", __FILE__, __LINE__, err, snd_strerror(err)); exit(1); }
#define check_error_msg(err,msg) if (err) { fprintf(stderr, "%s:%d, %s : %s(%d)\n", __FILE__, __LINE__, msg, snd_strerror(err), err); exit(1); }
#define display_error_msg(err,msg) if (err) { fprintf(stderr, "%s:%d, %s : %s(%d)\n", __FILE__, __LINE__, msg, snd_strerror(err), err); }

/**
 * Used to set the priority and scheduling of the audi#include <sys/types.h>
       #include <pwd.h>
o thread
 */
static bool setRealtimePriority()
{
    struct passwd*          pw;
    int                     err;
    uid_t                   uid;
    struct sched_param      param;

    uid = getuid ();
    pw = getpwnam ("root");
    err = setuid (pw->pw_uid);
	if (err==0) {
    	param.sched_priority = 50; /* 0 to 99  */
    	err = sched_setscheduler(0, SCHED_RR, &param);
    	err = setuid (uid);
	}
    return (err != -1);
}

/******************************************************************************
*******************************************************************************

								AUDIO INTERFACE

*******************************************************************************
*******************************************************************************/

enum { kRead = 1, kWrite = 2, kReadWrite = 3 };

/**
 * A convenient class to pass parameters to AudioInterface
 */
struct AudioParam
{
 	const char*		fCardName;
	unsigned int	fFrequency;
	unsigned int	fBuffering;
	unsigned int	fPeriods;

	unsigned int	fSoftInputs;
	unsigned int	fSoftOutputs;

 	AudioParam() :
		fCardName("hw:0"),
		fFrequency(44100),
		fBuffering(512),
		fPeriods(2),
		fSoftInputs(2),
		fSoftOutputs(2)
	{}

	AudioParam&	cardName(const char* n)	{ fCardName = n; 		return *this; }
	AudioParam&	frequency(int f)		{ fFrequency = f; 		return *this; }
	AudioParam&	buffering(int fpb)		{ fBuffering = fpb; 	return *this; }
	AudioParam&	periods(int p)			{ fPeriods = p; 		return *this; }
	AudioParam&	inputs(int n)			{ fSoftInputs = n; 		return *this; }
	AudioParam&	outputs(int n)			{ fSoftOutputs = n; 	return *this; }
};

/**
 * An ALSA audio interface
 */
struct AudioInterface : public AudioParam
{

    snd_pcm_t*				fOutputDevice;
	snd_pcm_t*				fInputDevice;
	snd_pcm_hw_params_t* 	fInputParams;
	snd_pcm_hw_params_t* 	fOutputParams;

	snd_pcm_format_t 		fSampleFormat;
	snd_pcm_access_t 		fSampleAccess;

	unsigned int			fCardInputs;
	unsigned int			fCardOutputs;

	unsigned int			fChanInputs;
	unsigned int			fChanOutputs;

	bool					fDuplexMode;

	// interleaved mode audiocard buffers
	void*		fInputCardBuffer;
	void*		fOutputCardBuffer;

	// non interleaved mode audiocard buffers
	void*		fInputCardChannels[256];
	void*		fOutputCardChannels[256];

	// non interleaved mod, floating point software buffers
	float*		fInputSoftChannels[256];
	float*		fOutputSoftChannels[256];

	const char*	cardName()				{ return fCardName;  	}
 	int			frequency()				{ return fFrequency; 	}
	int			buffering()				{ return fBuffering;  	}
	int			periods()				{ return fPeriods;  	}

	float**		inputSoftChannels()		{ return fInputSoftChannels;	}
	float**		outputSoftChannels()	{ return fOutputSoftChannels;	}

	bool		duplexMode()			{ return fDuplexMode; }

	AudioInterface(const AudioParam& ap = AudioParam()) : AudioParam(ap)
	{

		fInputDevice 			= 0;
		fOutputDevice 			= 0;
		fInputParams			= 0;
		fOutputParams			= 0;
	}

	/**
	 * Open the audio interface
	 */
	void open()
	{
		int err;

		// try to open output device, quit if fail to open output device
		err = snd_pcm_open(&fOutputDevice, fCardName, SND_PCM_STREAM_PLAYBACK, 0); check_error(err)

		// setup output device parameters
		err = snd_pcm_hw_params_malloc(&fOutputParams); check_error(err)
		setAudioParams(fOutputDevice, fOutputParams);

		fCardOutputs = fSoftOutputs;
		snd_pcm_hw_params_set_channels_near(fOutputDevice, fOutputParams, &fCardOutputs);
		err = snd_pcm_hw_params(fOutputDevice, fOutputParams); check_error(err);

		// allocate alsa output buffers
		if (fSampleAccess == SND_PCM_ACCESS_RW_INTERLEAVED) {
			fOutputCardBuffer = calloc(interleavedBufferSize(fOutputParams), 1);
		} else {
			for (unsigned int i = 0; i < fCardOutputs; i++) {
				fOutputCardChannels[i] = calloc(noninterleavedBufferSize(fOutputParams), 1);
			}
        }

		// check for duplex mode (if we need and have an input device)
		if (fSoftInputs == 0) {
			fDuplexMode = false;
			fCardInputs = 0;
		} else {
			// try to open input device
			err = snd_pcm_open(&fInputDevice, fCardName,SND_PCM_STREAM_CAPTURE, 0);
			if (err == 0) {
				fDuplexMode = true;
			} else {
				printf("Warning : no input device");
				fDuplexMode = false;
				fCardInputs = 0;
			}
		}

		if (fDuplexMode) {

			// we have and need an input device
			// set the number of physical inputs close to what we need
			err = snd_pcm_hw_params_malloc(&fInputParams); check_error(err);
			setAudioParams(fInputDevice, fInputParams);
			fCardInputs = fSoftInputs;
			snd_pcm_hw_params_set_channels_near(fInputDevice, fInputParams, &fCardInputs);
            err = snd_pcm_hw_params(fInputDevice, fInputParams); check_error(err);

			// allocation of alsa buffers
			if (fSampleAccess == SND_PCM_ACCESS_RW_INTERLEAVED) {
				fInputCardBuffer = calloc(interleavedBufferSize(fInputParams), 1);
			} else {
				for (unsigned int i = 0; i < fCardInputs; i++) {
					fInputCardChannels[i] = calloc(noninterleavedBufferSize(fInputParams), 1);
				}
			}
        }

		printf("inputs : %u, outputs : %u\n", fCardInputs, fCardOutputs);

		// allocation of floating point buffers needed by the dsp code

		fChanInputs = max(fSoftInputs, fCardInputs);		assert (fChanInputs < 256);
		fChanOutputs = max(fSoftOutputs, fCardOutputs);		assert (fChanOutputs < 256);

		for (unsigned int i = 0; i < fChanInputs; i++) {
			fInputSoftChannels[i] = (float*)calloc(fBuffering, sizeof(float));
			for (unsigned int j = 0; j < fBuffering; j++) {
				fInputSoftChannels[i][j] = 0.0;
			}
		}

		for (unsigned int i = 0; i < fChanOutputs; i++) {
			fOutputSoftChannels[i] = (float*)calloc(fBuffering, sizeof(float));
			for (unsigned int j = 0; j < fBuffering; j++) {
				fOutputSoftChannels[i][j] = 0.0;
			}
		}
	}

	void setAudioParams(snd_pcm_t* stream, snd_pcm_hw_params_t* params)
	{
		int	err;

		// set params record with initial values
		err = snd_pcm_hw_params_any(stream, params);
		check_error_msg(err, "unable to init parameters")

		// set alsa access mode (and fSampleAccess field) either to non interleaved or interleaved

		err = snd_pcm_hw_params_set_access(stream, params, SND_PCM_ACCESS_RW_NONINTERLEAVED);
		if (err) {
			err = snd_pcm_hw_params_set_access(stream, params, SND_PCM_ACCESS_RW_INTERLEAVED);
			check_error_msg(err, "unable to set access mode neither to non-interleaved or to interleaved");
		}
		snd_pcm_hw_params_get_access(params, &fSampleAccess);

		// search for 32-bits or 16-bits format
		err = snd_pcm_hw_params_set_format(stream, params, SND_PCM_FORMAT_S32);
		if (err) {
			err = snd_pcm_hw_params_set_format(stream, params, SND_PCM_FORMAT_S16);
		 	check_error_msg(err, "unable to set format to either 32-bits or 16-bits");
		}
		snd_pcm_hw_params_get_format(params, &fSampleFormat);
		// set sample frequency
		snd_pcm_hw_params_set_rate_near(stream, params, &fFrequency, 0);

		// set period and period size (buffering)
		err = snd_pcm_hw_params_set_period_size(stream, params, fBuffering, 0);
		check_error_msg(err, "period size not available");

		err = snd_pcm_hw_params_set_periods(stream, params, fPeriods, 0);
		check_error_msg(err, "number of periods not available");
	}

	ssize_t interleavedBufferSize(snd_pcm_hw_params_t* params)
	{
		_snd_pcm_format 	format;  	snd_pcm_hw_params_get_format(params, &format);
		snd_pcm_uframes_t 	psize;		snd_pcm_hw_params_get_period_size(params, &psize, NULL);
		unsigned int 		channels; 	snd_pcm_hw_params_get_channels(params, &channels);
		ssize_t bsize = snd_pcm_format_size(format, psize * channels);
		return bsize;
	}

	ssize_t noninterleavedBufferSize(snd_pcm_hw_params_t* params)
	{
		_snd_pcm_format 	format;  	snd_pcm_hw_params_get_format(params, &format);
		snd_pcm_uframes_t 	psize;		snd_pcm_hw_params_get_period_size(params, &psize, NULL);
		ssize_t bsize = snd_pcm_format_size(format, psize);
		return bsize;
	}

	void close()
	{}

	/**
	 * Read audio samples from the audio card. Convert samples to floats and take
	 * care of interleaved buffers
	 */
	void read()
	{
        if (fSampleAccess == SND_PCM_ACCESS_RW_INTERLEAVED) {

			int count = snd_pcm_readi(fInputDevice, fInputCardBuffer, fBuffering);
			if (count < 0) {
				 //display_error_msg(count, "reading samples");
				 snd_pcm_prepare(fInputDevice);
				 //check_error_msg(err, "preparing input stream");
			}

			if (fSampleFormat == SND_PCM_FORMAT_S16) {
				short* buffer16b = (short*)fInputCardBuffer;
				for (unsigned int s = 0; s < fBuffering; s++) {
					for (unsigned int c = 0; c < fCardInputs; c++) {
						fInputSoftChannels[c][s] = float(buffer16b[c + s*fCardInputs])*(1.0/float(SHRT_MAX));
					}
				}
			} else if (fSampleFormat == SND_PCM_FORMAT_S32) {
				int32* buffer32b = (int32*)fInputCardBuffer;
				for (unsigned int s = 0; s < fBuffering; s++) {
					for (unsigned int c = 0; c < fCardInputs; c++) {
						fInputSoftChannels[c][s] = float(buffer32b[c + s*fCardInputs])*(1.0/float(INT_MAX));
					}
				}
			} else {
				printf("unrecognized input sample format : %u\n", fSampleFormat);
				exit(1);
			}

		} else if (fSampleAccess == SND_PCM_ACCESS_RW_NONINTERLEAVED) {

			int count = snd_pcm_readn(fInputDevice, fInputCardChannels, fBuffering);
			if (count < 0) {
				 //display_error_msg(count, "reading samples");
				 snd_pcm_prepare(fInputDevice);
				 //check_error_msg(err, "preparing input stream");
			}

			if (fSampleFormat == SND_PCM_FORMAT_S16) {
				for (unsigned int c = 0; c < fCardInputs; c++) {
					short* chan16b = (short*)fInputCardChannels[c];
					for (unsigned int s = 0; s < fBuffering; s++) {
						fInputSoftChannels[c][s] = float(chan16b[s])*(1.0/float(SHRT_MAX));
					}
				}
			} else if (fSampleFormat == SND_PCM_FORMAT_S32) {
				for (unsigned int c = 0; c < fCardInputs; c++) {
					int32* chan32b = (int32*)fInputCardChannels[c];
					for (unsigned int s = 0; s < fBuffering; s++) {
						fInputSoftChannels[c][s] = float(chan32b[s])*(1.0/float(INT_MAX));
					}
				}
			} else {
				printf("unrecognized input sample format : %u\n", fSampleFormat);
				exit(1);
			}

		} else {
			check_error_msg(-10000, "unknown access mode");
		}
    }

	/**
	 * write the output soft channels to the audio card. Convert sample
	 * format and interleaves buffers when needed
	 */
	void write()
	{
		recovery :

		if (fSampleAccess == SND_PCM_ACCESS_RW_INTERLEAVED) {

			if (fSampleFormat == SND_PCM_FORMAT_S16) {
				short* buffer16b = (short*)fOutputCardBuffer;
				for (unsigned int f = 0; f < fBuffering; f++) {
					for (unsigned int c = 0; c < fCardOutputs; c++) {
						float x = fOutputSoftChannels[c][f];
						buffer16b[c + f*fCardOutputs] = short(max(min(x,1.0f),-1.0f) * float(SHRT_MAX)) ;
					}
				}

			} else if (fSampleFormat == SND_PCM_FORMAT_S32)  {
				int32* buffer32b = (int32*)fOutputCardBuffer;
				for (unsigned int f = 0; f < fBuffering; f++) {
					for (unsigned int c = 0; c < fCardOutputs; c++) {
						float x = fOutputSoftChannels[c][f];
						buffer32b[c + f*fCardOutputs] = int(max(min(x,1.0f),-1.0f) * float(INT_MAX));
					}
				}
			} else {
				printf("unrecognized output sample format : %u\n", fSampleFormat);
				exit(1);
			}

			int count = snd_pcm_writei(fOutputDevice, fOutputCardBuffer, fBuffering);
			if (count<0) {
				//display_error_msg(count, "w3");
				snd_pcm_prepare(fOutputDevice);
				//check_error_msg(err, "preparing output stream");
				goto recovery;
			}


		} else if (fSampleAccess == SND_PCM_ACCESS_RW_NONINTERLEAVED) {

			if (fSampleFormat == SND_PCM_FORMAT_S16) {

				for (unsigned int c = 0; c < fCardOutputs; c++) {
					short* chan16b = (short*)fOutputCardChannels[c];
					for (unsigned int f = 0; f < fBuffering; f++) {
						float x = fOutputSoftChannels[c][f];
						chan16b[f] = short(max(min(x,1.0f),-1.0f) * float(SHRT_MAX));
					}
				}

			} else if (fSampleFormat == SND_PCM_FORMAT_S32) {

				for (unsigned int c = 0; c < fCardOutputs; c++) {
					int32* chan32b = (int32*)fOutputCardChannels[c];
					for (unsigned int f = 0; f < fBuffering; f++) {
						float x = fOutputSoftChannels[c][f];
						chan32b[f] = int(max(min(x,1.0f),-1.0f) * float(INT_MAX));
					}
				}

			} else {
				printf("unrecognized output sample format : %u\n", fSampleFormat);
				exit(1);
			}

			int count = snd_pcm_writen(fOutputDevice, fOutputCardChannels, fBuffering);
			if (count < 0) {
				//display_error_msg(count, "w3");
				snd_pcm_prepare(fOutputDevice);
				//check_error_msg(err, "preparing output stream");
				goto recovery;
			}

		} else {
			check_error_msg(-10000, "unknown access mode");
		}
	}

	/**
	 *  print short information on the audio device
	 */
	void shortinfo()
	{
		int						err;
		snd_ctl_card_info_t*	card_info;
    	snd_ctl_t*				ctl_handle;
		err = snd_ctl_open(&ctl_handle, fCardName, 0); check_error(err);
		snd_ctl_card_info_alloca(&card_info);
		err = snd_ctl_card_info(ctl_handle, card_info);	check_error(err);
		printf("%s|%d|%d|%d|%d|%s\n",
				snd_ctl_card_info_get_driver(card_info),
				fCardInputs, fCardOutputs,
				fFrequency, fBuffering,
				snd_pcm_format_name((_snd_pcm_format)fSampleFormat));
	}

	/**
	 *  print more detailled information on the audio device
	 */
	void longinfo()
	{
		int						err;
		snd_ctl_card_info_t*	card_info;
    	snd_ctl_t*				ctl_handle;

		printf("Audio Interface Description :\n");
		printf("Sampling Frequency : %d, Sample Format : %s, buffering : %d\n",
				fFrequency, snd_pcm_format_name((_snd_pcm_format)fSampleFormat), fBuffering);
		printf("Software inputs : %2d, Software outputs : %2d\n", fSoftInputs, fSoftOutputs);
		printf("Hardware inputs : %2d, Hardware outputs : %2d\n", fCardInputs, fCardOutputs);
		printf("Channel inputs  : %2d, Channel outputs  : %2d\n", fChanInputs, fChanOutputs);

		// affichage des infos de la carte
		err = snd_ctl_open(&ctl_handle, fCardName, 0); check_error(err);
		snd_ctl_card_info_alloca(&card_info);
		err = snd_ctl_card_info(ctl_handle, card_info); check_error(err);
		printCardInfo(card_info);

		// affichage des infos liees aux streams d'entree-sortie
		if (fSoftInputs > 0)	printHWParams(fInputParams);
		if (fSoftOutputs > 0)	printHWParams(fOutputParams);
	}

	void printCardInfo(snd_ctl_card_info_t*	ci)
	{
		printf("Card info (address : %p)\n", ci);
		printf("\tID         = %s\n", snd_ctl_card_info_get_id(ci));
		printf("\tDriver     = %s\n", snd_ctl_card_info_get_driver(ci));
		printf("\tName       = %s\n", snd_ctl_card_info_get_name(ci));
		printf("\tLongName   = %s\n", snd_ctl_card_info_get_longname(ci));
		printf("\tMixerName  = %s\n", snd_ctl_card_info_get_mixername(ci));
		printf("\tComponents = %s\n", snd_ctl_card_info_get_components(ci));
		printf("--------------\n");
	}

	void printHWParams(snd_pcm_hw_params_t* params)
	{
		printf("HW Params info (address : %p)\n", params);
#if 0
		printf("\tChannels    = %d\n", snd_pcm_hw_params_get_channels(params));
		printf("\tFormat      = %s\n", snd_pcm_format_name((_snd_pcm_format)snd_pcm_hw_params_get_format(params)));
		printf("\tAccess      = %s\n", snd_pcm_access_name((_snd_pcm_access)snd_pcm_hw_params_get_access(params)));
		printf("\tRate        = %d\n", snd_pcm_hw_params_get_rate(params, NULL));
		printf("\tPeriods     = %d\n", snd_pcm_hw_params_get_periods(params, NULL));
		printf("\tPeriod size = %d\n", (int)snd_pcm_hw_params_get_period_size(params, NULL));
		printf("\tPeriod time = %d\n", snd_pcm_hw_params_get_period_time(params, NULL));
		printf("\tBuffer size = %d\n", (int)snd_pcm_hw_params_get_buffer_size(params));
		printf("\tBuffer time = %d\n", snd_pcm_hw_params_get_buffer_time(params, NULL));
#endif
		printf("--------------\n");
	}
    
    int getNumInputs() { return fCardInputs; }
    int getNumOutputs() { return fCardOutputs; }

};

/**
 * Return the value of an environment variable or defval if undefined.
 */
static int getDefaultEnv(const char* name, int defval)
{
    const char* str = getenv(name);
    if (str) {
        return atoi(str);
    } else {
        return defval;
    }
}

/**
 * Return the value of an environment variable or defval if undefined.
 */
static const char* getDefaultEnv(const char* name, const char* defval)
{
    const char* str = getenv(name);
    if (str) {
        return str;
    } else {
        return defval;
    }
}

/******************************************************************************
*******************************************************************************

							   ALSA audio interface

*******************************************************************************
*******************************************************************************/
static void* __run(void* ptr);

class alsaaudio : public audio
{
	AudioInterface*	fAudio;
	dsp* 			fDSP;
	pthread_t 		fAudioThread;
	bool 			fRunning;

 public:

    alsaaudio(int argc, char* argv[], dsp* DSP) : fDSP(DSP), fRunning(false)
    {
        if (isopt(argv, "-help") || isopt(argv, "-h")) {
            std::cout << "prog [--device|-d <device> (default \"hw:0\")] [--frequency|-f <f> (default 44100)] [--buffer|-b <bs> (default 512)] [--periods|-p <n> (default 2)]\n";
            exit(1);
        }
        fAudio = new AudioInterface(AudioParam().cardName(lopts1(argc, argv, "--device", "-d", getDefaultEnv("FAUST2ALSA_DEVICE", "hw:0")))
            .frequency(lopt1(argc, argv, "--frequency", "-f", getDefaultEnv("FAUST2ALSA_FREQUENCY", 44100)))
            .buffering(lopt1(argc, argv, "--buffer", "-b", getDefaultEnv("FAUST2ALSA_BUFFER", 512)))
            .periods(lopt1(argc, argv, "--periods", "-p", getDefaultEnv("FAUST2ALSA_PERIODS", 2)))
            .inputs(DSP->getNumInputs())
            .outputs(DSP->getNumOutputs()));
    }
    
    alsaaudio(int srate, int bsize) : fDSP(0), fRunning(false)
    {
        fAudio = new AudioInterface(AudioParam().cardName("hw:0")
                                    .frequency(srate)
                                    .buffering(bsize)
                                    .periods(2));
    }

	virtual ~alsaaudio() { stop(); delete fAudio; }

	virtual bool init(const char* /*name*/, dsp* DSP)
    {
        fDSP = DSP;
        fAudio->inputs(DSP->getNumInputs());
        fAudio->outputs(DSP->getNumOutputs());
		fAudio->open();
	    DSP->init(fAudio->frequency());
 		return true;
	}

    virtual bool start()
    {
        fRunning = true;
        if (pthread_create(&fAudioThread, 0, __run, this)) {
            fRunning = false;
        }
        return fRunning;
    }

    virtual void stop()
    {
        if (fRunning) {
            fRunning = false;
            pthread_join(fAudioThread, 0);
        }
    }
    
    virtual int getBufferSize() { return fAudio->buffering(); }
    virtual int getSampleRate() { return fAudio->frequency(); }

	virtual void run()
    {
		bool rt = setRealtimePriority();
		printf(rt ? "RT : ":"NRT: "); fAudio->shortinfo();
        AVOIDDENORMALS;
		if (fAudio->duplexMode()) {
            fAudio->write();
			fAudio->write();
			while (fRunning) {
				fAudio->read();
				fDSP->compute(fAudio->buffering(), fAudio->inputSoftChannels(), fAudio->outputSoftChannels());
				fAudio->write();
			}
        } else {
            fAudio->write();
			while (fRunning) {
				fDSP->compute(fAudio->buffering(), fAudio->inputSoftChannels(), fAudio->outputSoftChannels());
				fAudio->write();
			}
		}
	}
    
    virtual int getNumInputs() { return fAudio->getNumInputs(); }
    virtual int getNumOutputs() { return fAudio->getNumOutputs(); }

};

void* __run (void* ptr)
{
	alsaaudio * alsa = (alsaaudio*)ptr;
	alsa->run();
	return 0;
}

#endif

/********************END ARCHITECTURE SECTION (part 2/2)****************/

/**************************  END  alsa-dsp.h **************************/
#elif JACK_DRIVER
/************************** BEGIN jack-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __jack_dsp__
#define __jack_dsp__

#include <stdio.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <string.h>
#include <jack/jack.h>

/************************** BEGIN jack-midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __jack_midi__
#define __jack_midi__

#include <iostream>
#include <cstdlib>

#include <jack/jack.h>
#include <jack/midiport.h>


class MapUI;

//-----------------------------------------------
// MIDI input/output handling using JACK library
//-----------------------------------------------

class jack_midi_handler : public midi_handler {
        
    protected:

        ringbuffer_t* fOutBuffer;
        jack_port_t* fInputMidiPort;    // JACK input MIDI port
        jack_port_t* fOutputMidiPort;   // JACK output MIDI port

        bool initPorts(jack_client_t* client)
        {
            fInputMidiPort = jack_port_register(client, "midi_in_1", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
            fOutputMidiPort = jack_port_register(client, "midi_out_1", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
            return fInputMidiPort && fOutputMidiPort;
        }
    
        void writeMessage(double date, unsigned char* buffer, size_t size)
        {
            size_t res;
            DatedMessage dated_message(date, buffer, size);
            if ((res = ringbuffer_write(fOutBuffer, (const char*)&dated_message, sizeof(DatedMessage))) != sizeof(DatedMessage)) {
                std::cerr << "ringbuffer_write error DatedMessage" << std::endl;
            }
        }

        void processMidiInBuffer(void* port_buf_in)
        {
            for (size_t i = 0; i < jack_midi_get_event_count(port_buf_in); ++i) {
                jack_midi_event_t event;
                if (jack_midi_event_get(&event, port_buf_in, i) == 0) {

                    size_t nBytes = event.size;
                    int type = (int)event.buffer[0] & 0xf0;
                    int channel = (int)event.buffer[0] & 0x0f;
                    double time = event.time; // Timestamp in frames

                    // MIDI sync
                    if (nBytes == 1) {
                        handleSync(time, (int)event.buffer[0]);
                    } else if (nBytes == 2) {
                        handleData1(time, type, channel, (int)event.buffer[1]);
                    } else if (nBytes == 3) {
                        handleData2(time, type, channel, (int)event.buffer[1], (int)event.buffer[2]);
                    } else {
                        std::vector<unsigned char> message(event.buffer, event.buffer + event.size);
                        handleMessage(time, type, message);
                    }
                }
            }
        }
    
        virtual void processMidiIn(jack_nframes_t nframes)
        {
            // MIDI input
            processMidiInBuffer(jack_port_get_buffer(fInputMidiPort, nframes));
        }

        void processMidiOutBuffer(void* port_buf_out_aux, bool reset = false)
        {
            // MIDI output
            unsigned char* port_buf_out = (unsigned char*)port_buf_out_aux;
            if (reset) {
                /*
                // 08/03/2019: "jack_midi_reset_buffer" is not official in JACK, so we deactivate the code.
                jack_midi_reset_buffer(port_buf_out);
                */
            } else {
                jack_midi_clear_buffer(port_buf_out);
            }
           
            // Write each message one by one
            DatedMessage dated_message;
            while (ringbuffer_read(fOutBuffer, (char*)&dated_message, sizeof(DatedMessage)) == sizeof(DatedMessage)) {
                jack_midi_data_t* data = jack_midi_event_reserve(port_buf_out, dated_message.fDate, dated_message.fSize);
                if (data) {
                    memcpy(data, dated_message.fBuffer, dated_message.fSize);
                } else {
                    std::cerr << "jack_midi_event_reserve error" << std::endl;
                }
            }
        }
    
        virtual void processMidiOut(jack_nframes_t nframes)
        {
            // MIDI output
            processMidiOutBuffer(jack_port_get_buffer(fOutputMidiPort, nframes));
        }

    public:

        jack_midi_handler(const std::string& name = "JACKHandler")
            :midi_handler(name), fInputMidiPort(nullptr), fOutputMidiPort(nullptr)
        {
            fOutBuffer = ringbuffer_create(8192);
        }
        virtual ~jack_midi_handler()
        {
            ringbuffer_free(fOutBuffer);
        }
    
        // To be used in polling mode
        int recvMessages(std::vector<MIDIMessage>* messages)
        {
            int count = 0;
            jack_nframes_t first_time_stamp = 0;
            void* port_buf_in = jack_port_get_buffer(fInputMidiPort, 1024);
            for (size_t i = 0; i < jack_midi_get_event_count(port_buf_in); ++i) {
                jack_midi_event_t event;
                if (jack_midi_event_get(&event, port_buf_in, i) == 0) {
                    // Small messages
                    if (event.size <= 3) {
                        if (count == 0) first_time_stamp = event.time;
                        MIDIMessage& mes = messages->at(count++);
                        mes.frameIndex = (uint32_t)(event.time - first_time_stamp);
                        mes.byte0 = event.buffer[0];
                        mes.byte1 = event.buffer[1];
                        mes.byte2 = event.buffer[2];
                    } else {
                        std::cerr << "recvMessages : long messages (" << event.size << ") are not supported yet\n";
                    }
                }
            }
            return count;
        }
    
        void sendMessages(std::vector<MIDIMessage>* messages, int count)
        {
            for (int i = 0; i < count; ++i) {
                MIDIMessage message = (*messages)[i];
                unsigned char buffer[3]
                    = { static_cast<unsigned char>(message.byte0),
                        static_cast<unsigned char>(message.byte1),
                        static_cast<unsigned char>(message.byte2) };
                writeMessage(0, buffer, 3);
            }
        }
    
        // MIDI output API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            unsigned char buffer[3]
                = { static_cast<unsigned char>(MIDI_NOTE_ON + channel),
                    static_cast<unsigned char>(pitch),
                    static_cast<unsigned char>(velocity) };
            writeMessage(0, buffer, 3);
            return 0;
        }

        void keyOff(int channel, int pitch, int velocity)
        {
            unsigned char buffer[3]
                = { static_cast<unsigned char>(MIDI_NOTE_OFF + channel),
                    static_cast<unsigned char>(pitch),
                    static_cast<unsigned char>(velocity) };
            writeMessage(0, buffer, 3);
        }

        void ctrlChange(int channel, int ctrl, int val)
        {
            unsigned char buffer[3]
                = { static_cast<unsigned char>(MIDI_CONTROL_CHANGE + channel),
                    static_cast<unsigned char>(ctrl),
                    static_cast<unsigned char>(val) };
            writeMessage(0, buffer, 3);
        }

        void chanPress(int channel, int press)
        {
            unsigned char buffer[2]
                = { static_cast<unsigned char>(MIDI_AFTERTOUCH + channel),
                    static_cast<unsigned char>(press) };
            writeMessage(0, buffer, 2);
        }

        void progChange(int channel, int pgm)
        {
            unsigned char buffer[2]
                = { static_cast<unsigned char>(MIDI_PROGRAM_CHANGE + channel),
                    static_cast<unsigned char>(pgm) };
            writeMessage(0, buffer, 2);
        }

        void keyPress(int channel, int pitch, int press)
        {
            unsigned char buffer[3]
                = { static_cast<unsigned char>(MIDI_POLY_AFTERTOUCH + channel),
                    static_cast<unsigned char>(pitch),
                    static_cast<unsigned char>(press) };
            writeMessage(0, buffer, 3);
        }

        void pitchWheel(int channel, int wheel)
        {
            unsigned char buffer[3]
                = { static_cast<unsigned char>(MIDI_PITCH_BEND + channel),
                    static_cast<unsigned char>(wheel & 0x7F),
                    static_cast<unsigned char>((wheel >> 7) & 0x7F) };
            writeMessage(0, buffer, 3);
        }

        void ctrlChange14bits(int channel, int ctrl, int value) {}

        void startSync(double date)
        {
            unsigned char buffer[1] = { MIDI_START };
            writeMessage(date, buffer, 1);
        }
  
        void stopSync(double date)
        {
            unsigned char buffer[1] = { MIDI_STOP };
            writeMessage(date, buffer, 1);
        }

        void clock(double date)
        {
            unsigned char buffer[1] = { MIDI_CLOCK };
            writeMessage(date, buffer, 1);
        }
    
        void sysEx(double date, std::vector<unsigned char>& message)
        {
            writeMessage(date, message.data(), (int)message.size());
        }

};

#endif
/**************************  END  jack-midi.h **************************/

#if defined(_WIN32) && !defined(__MINGW32__)
#define snprintf _snprintf_s
#endif

/******************************************************************************
 *******************************************************************************
 
 JACK AUDIO INTERFACE
 
 *******************************************************************************
 *******************************************************************************/

class jackaudio : public audio {
    
    protected:
        
        dsp* fDSP;              // FAUST DSP
        jack_client_t* fClient; // JACK client
        
        std::vector<jack_port_t*> fInputPorts;   // JACK input ports
        std::vector<jack_port_t*> fOutputPorts;  // JACK output ports
        
        std::vector<char*> fPhysicalInputs;
        std::vector<char*> fPhysicalOutputs;
        
        bool fAutoConnect;  // autoconnect with system in/out ports
        
        std::list<std::pair<std::string, std::string> > fConnections;   // Connections list
        
        static int _jack_srate(jack_nframes_t nframes, void* arg)
        {
            fprintf(stdout, "The sample rate is now %u/sec\n", nframes);
            return 0;
        }
        
        static void _jack_shutdown(void* arg)
        {}
        
        static void _jack_info_shutdown(jack_status_t code, const char* reason, void* arg)
        {
            fprintf(stderr, "%s\n", reason);
            static_cast<jackaudio*>(arg)->shutdown(reason);
        }
        
        static int _jack_process(jack_nframes_t nframes, void* arg)
        {
            return static_cast<jackaudio*>(arg)->process(nframes);
        }
        
        static int _jack_buffersize(jack_nframes_t nframes, void* arg)
        {
            fprintf(stdout, "The buffer size is now %u/sec\n", nframes);
            return 0;
        }
        
    #ifdef _OPENMP
        static void* _jack_thread(void* arg)
        {
            jackaudio* audio = (jackaudio*)arg;
            audio->process_thread();
            return 0;
        }
    #endif
        
        void shutdown(const char* message)
        {
            fClient = nullptr;
            
            if (fShutdown) {
                fShutdown(message, fShutdownArg);
            } else {
                exit(1); // By default
            }
        }
        
        // Save client connections
        virtual bool saveConnections()
        {
            if (fClient) {
                fConnections.clear();
                
                for (size_t i = 0; i < fInputPorts.size(); i++) {
                    const char** connected_port = jack_port_get_all_connections(fClient, fInputPorts[i]);
                    if (connected_port != nullptr) {
                        for (int port = 0; connected_port[port]; port++) {
                            fConnections.push_back(std::make_pair(connected_port[port], jack_port_name(fInputPorts[i])));
                            // printf("INPUT %s ==> %s\n", connected_port[port], jack_port_name(fInputPorts[i]));
                        }
                        jack_free(connected_port);
                    }
                }
                
                for (size_t i = 0; i < fOutputPorts.size(); i++) {
                    const char** connected_port = jack_port_get_all_connections(fClient, fOutputPorts[i]);
                    if (connected_port != nullptr) {
                        for (int port = 0; connected_port[port]; port++) {
                            fConnections.push_back(std::make_pair(jack_port_name(fOutputPorts[i]), connected_port[port]));
                            // printf("OUTPUT %s ==> %s\n", jack_port_name(fOutputPorts[i]), connected_port[port]);
                        }
                        jack_free(connected_port);
                    }
                }
                return true;
            } else {
                fprintf(stdout, "Client no more running...\n");
                return false;
            }
        }
        
        // Load client connections
        void loadConnections()
        {
            std::list<std::pair<std::string, std::string> >::const_iterator it;
            for (it = fConnections.begin(); it != fConnections.end(); it++) {
                std::pair<std::string, std::string> connection = *it;
                jack_connect(fClient, connection.first.c_str(), connection.second.c_str());
            }
        }
        
    #ifdef _OPENMP
        void process_thread()
        {
            jack_nframes_t nframes;
            while (1) {
                nframes = jack_cycle_wait(fClient);
                process(nframes);
                jack_cycle_signal(fClient, 0);
            }
        }
    #endif
        
        // JACK callbacks
        virtual int	process(jack_nframes_t nframes)
        {
            AVOIDDENORMALS;
            
            // Retrieve JACK inputs/output audio buffers
            float** fInChannel = (float**)alloca(fInputPorts.size() * sizeof(float*));
            for (size_t i = 0; i < fInputPorts.size(); i++) {
                fInChannel[i] = (float*)jack_port_get_buffer(fInputPorts[i], nframes);
            }
            
            float** fOutChannel = (float**)alloca(fOutputPorts.size() * sizeof(float*));
            for (size_t i = 0; i < fOutputPorts.size(); i++) {
                fOutChannel[i] = (float*)jack_port_get_buffer(fOutputPorts[i], nframes);
            }
            
            fDSP->compute(nframes, reinterpret_cast<FAUSTFLOAT**>(fInChannel), reinterpret_cast<FAUSTFLOAT**>(fOutChannel));
            
            runControlCallbacks();
            return 0;
        }
        
    public:
        
        jackaudio(bool auto_connect = true)
        : fDSP(0), fClient(0), fAutoConnect(auto_connect)
        {}
        
        virtual ~jackaudio()
        {
            if (fClient) {
                stop();
                
                for (size_t i = 0; i < fInputPorts.size(); i++) {
                    jack_port_unregister(fClient, fInputPorts[i]);
                }
                for (size_t i = 0; i < fOutputPorts.size(); i++) {
                    jack_port_unregister(fClient, fOutputPorts[i]);
                }
                jack_client_close(fClient);
            }
        }
        
        virtual bool init(const char* name, dsp* dsp)
        {
            if (initAux(name)) {
                if (dsp) { setDsp(dsp); }
                return true;
            } else {
                return false;
            }
        }
        
        bool initAux(const char* name)
        {
            if ((fClient = jack_client_open(name, JackNullOption, nullptr)) == 0) {
                fprintf(stderr, "JACK server not running ?\n");
                return false;
            }
            
    #ifdef _OPENMP
            jack_set_process_thread(fClient, _jack_thread, this);
    #else
            jack_set_process_callback(fClient, _jack_process, this);
    #endif
            
            jack_set_sample_rate_callback(fClient, _jack_srate, this);
            jack_set_buffer_size_callback(fClient, _jack_buffersize, this);
            jack_on_info_shutdown(fClient, _jack_info_shutdown, this);
            
            // Get Physical inputs
            int inputs = 0;
            char** physicalInPorts = (char**)jack_get_ports(fClient, nullptr, JACK_DEFAULT_AUDIO_TYPE, JackPortIsPhysical|JackPortIsOutput);
            if (physicalInPorts != nullptr) {
                while (physicalInPorts[inputs]) {
                    fPhysicalInputs.push_back(physicalInPorts[inputs]);
                    printf("physical input %s\n", physicalInPorts[inputs]);
                    inputs++;
                }
                jack_free(physicalInPorts);
            }
            
            // Get Physical outputs
            int outputs = 0;
            char** physicalOutPorts = (char**)jack_get_ports(fClient, nullptr, JACK_DEFAULT_AUDIO_TYPE, JackPortIsPhysical|JackPortIsInput);
            if (physicalOutPorts != nullptr) {
                while (physicalOutPorts[outputs]) {
                    fPhysicalOutputs.push_back(physicalOutPorts[outputs]);
                    printf("physical output %s\n", physicalOutPorts[outputs]);
                    outputs++;
                }
                jack_free(physicalOutPorts);
            }
            
            return true;
        }
        
        virtual bool start()
        {
            if (jack_activate(fClient)) {
                fprintf(stderr, "Cannot activate client\n");
                return false;
            }
            
            if (fConnections.size() > 0) {
                loadConnections();
            } else if (fAutoConnect) {
                defaultConnections();
            }
            
            return true;
        }
        
        virtual void stop()
        {
            if (fClient) {
                saveConnections();
                jack_deactivate(fClient);
            }
        }
        
        virtual int getBufferSize() { return jack_get_buffer_size(fClient); }
        virtual int getSampleRate() { return jack_get_sample_rate(fClient); }
        
        virtual int getNumInputs()
        {
            return fPhysicalInputs.size();
        }
        
        virtual int getNumOutputs()
        {
            return fPhysicalOutputs.size();
        }
        
        // Additional public API
        
        jack_client_t* getClient() { return fClient; }
        
        // Connect to physical inputs/outputs
        void defaultConnections()
        {
            // To avoid feedback at launch time, don't connect hardware inputs
            /*
             for (int i = 0; i < fInputPorts.size() && i < fPhysicalOutputs.size(); i++) {
                jack_connect(fClient, fPhysicalInputs[i], jack_port_name(fInputPorts[i]));
             }
             */
            for (size_t i = 0; i < fOutputPorts.size() && i < fPhysicalInputs.size(); i++) {
                jack_connect(fClient, jack_port_name(fOutputPorts[i]), fPhysicalOutputs[i]);
            }
        }
        
        virtual void setDsp(dsp* dsp)
        {
            fDSP = dsp;
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                char buf[256];
                snprintf(buf, 256, "in_%d", i);
                fInputPorts.push_back(jack_port_register(fClient, buf, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
            }
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                char buf[256];
                snprintf(buf, 256, "out_%d", i);
                fOutputPorts.push_back(jack_port_register(fClient, buf, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
            }
            fDSP->init(jack_get_sample_rate(fClient));
        }
        
        void connect(jackaudio* driver, int src, int dst, bool reverse)
        {
            if (driver) {
                // Connection between drivers
                jack_port_t* src_port = getOutputPort(src);
                jack_port_t* dst_port = driver->getInputPort(src);
                if (src_port && dst_port) {
                    jack_connect(fClient, jack_port_name(src_port), jack_port_name(dst_port));
                }
            } else if (reverse) {
                // Connection to physical input
                if ((size_t)src > fPhysicalInputs.size()) return;
                jack_port_t* dst_port = getInputPort(dst);
                if (dst_port) {
                    jack_connect(fClient, fPhysicalInputs[src], jack_port_name(dst_port));
                }
            } else {
                // Connection to physical output
                if ((size_t)dst > fPhysicalOutputs.size()) return;
                jack_port_t* src_port = getOutputPort(src);
                if (src_port) {
                    jack_connect(fClient, jack_port_name(src_port), fPhysicalOutputs[dst]);
                }
            }
        }
        
        void disconnect(jackaudio* driver, int src, int dst, bool reverse)
        {
            if (driver) {
                // Connection between drivers
                jack_port_t* src_port = getOutputPort(src);
                jack_port_t* dst_port = driver->getInputPort(src);
                if (src_port && dst_port) {
                    jack_disconnect(fClient, jack_port_name(src_port), jack_port_name(dst_port));
                }
            } else if (reverse) {
                // Connection to physical input
                if ((size_t)src > fPhysicalInputs.size()) return;
                jack_port_t* dst_port = getInputPort(dst);
                if (dst_port) {
                    jack_disconnect(fClient, fPhysicalInputs[src], jack_port_name(dst_port));
                }
            } else {
                // Connection to physical output
                if ((size_t)dst > fPhysicalOutputs.size()) return;
                jack_port_t* src_port = getOutputPort(src);
                if (src_port) {
                    jack_disconnect(fClient, jack_port_name(src_port), fPhysicalOutputs[dst]);
                }
            }
        }
        
        bool isConnected(jackaudio* driver, int src, int dst, bool reverse)
        {
            if (driver) {
                // Connection between drivers
                jack_port_t* src_port = getOutputPort(src);
                jack_port_t* dst_port = driver->getInputPort(src);
                if (src_port && dst_port) {
                    return jack_port_connected_to(src_port, jack_port_name(dst_port));
                } else {
                    return false;
                }
            } else if (reverse) {
                // Connection to physical input
                if ((size_t)src > fPhysicalInputs.size()) return false;
                jack_port_t* dst_port = getInputPort(dst);
                if (dst_port) {
                    return jack_port_connected_to(dst_port, fPhysicalInputs[src]);
                } else {
                    return false;
                }
            } else {
                // Connection to physical output
                if ((size_t)dst > fPhysicalOutputs.size()) return false;
                jack_port_t* src_port = getOutputPort(src);
                if (src_port) {
                    return jack_port_connected_to(src_port, fPhysicalOutputs[dst]);
                } else {
                    return false;
                }
            }
        }
        
        jack_port_t* getInputPort(int port)  { return (port >= 0 && port < (int)fInputPorts.size()) ? fInputPorts[port] : nullptr; }
        jack_port_t* getOutputPort(int port) { return (port >= 0 && port < (int)fOutputPorts.size()) ? fOutputPorts[port] : nullptr; }
        
};

// Add JACK MIDI

class jackaudio_midi : public jackaudio, public jack_midi_handler {
        
    protected:
    
        bool fPolling;
    
        virtual bool saveConnections()
        {
            if (jackaudio::saveConnections()) { // Audio connections can be saved, so try MIDI
                
                if (fInputMidiPort) {
                    const char** connected_port = jack_port_get_all_connections(fClient, fInputMidiPort);
                    if (connected_port != nullptr) {
                        for (int port = 0; connected_port[port]; port++) {
                            fConnections.push_back(std::make_pair(connected_port[port], jack_port_name(fInputMidiPort)));
                            // printf("INPUT %s ==> %s\n", connected_port[port], jack_port_name(fInputPorts[i]));
                        }
                        jack_free(connected_port);
                    }
                }
                
                if (fOutputMidiPort) {
                    const char** connected_port = jack_port_get_all_connections(fClient, fOutputMidiPort);
                    if (connected_port != nullptr) {
                        for (int port = 0; connected_port[port]; port++) {
                            fConnections.push_back(std::make_pair(jack_port_name(fOutputMidiPort), connected_port[port]));
                            // printf("OUTPUT %s ==> %s\n", jack_port_name(fOutputPorts[i]), connected_port[port]);
                        }
                        jack_free(connected_port);
                    }
                }
                return true;
                
            } else {
                return false;
            }
        }
    
        virtual void processAudio(jack_nframes_t nframes)
        {
            // Audio
            AVOIDDENORMALS;
            
            // Retrieve JACK inputs/output audio buffers
            float** fInChannel = (float**)alloca(fInputPorts.size() * sizeof(float*));
            for (size_t i = 0; i < fInputPorts.size(); i++) {
                fInChannel[i] = (float*)jack_port_get_buffer(fInputPorts[i], nframes);
            }
            
            float** fOutChannel = (float**)alloca(fOutputPorts.size() * sizeof(float*));
            for (size_t i = 0; i < fOutputPorts.size(); i++) {
                fOutChannel[i] = (float*)jack_port_get_buffer(fOutputPorts[i], nframes);
            }
            
            // By convention timestamp of -1 means 'no timestamp conversion' : events already have a timestamp espressed in frames
            fDSP->compute(-1, nframes, reinterpret_cast<FAUSTFLOAT**>(fInChannel), reinterpret_cast<FAUSTFLOAT**>(fOutChannel));
        }
    
        virtual int process(jack_nframes_t nframes)
        {
            // MIDI in
            if (!fPolling) processMidiIn(nframes);
            
            // Audio
            processAudio(nframes);
            
            // MIDI out
            processMidiOut(nframes);
            
            runControlCallbacks();
            return 0;
        }
        
    public:
        
        jackaudio_midi(bool auto_connect = true, bool is_polling = false)
        :jackaudio(auto_connect), jack_midi_handler("JACKMidi"), fPolling(is_polling)
        {}
        
        virtual ~jackaudio_midi()
        {}
        
        virtual bool init(const char* name, dsp* dsp)
        {
            if (jackaudio::initAux(name)) {
                if (dsp) { setDsp(dsp); }
                return initPorts(fClient);
            } else {
                return false;
            }
        }
        
        virtual bool start()
        {
            return jackaudio::start();
        }
        
        virtual void stop()
        {
            jackaudio::stop();
        }
    
};

#endif
/**************************  END  jack-dsp.h **************************/
#elif PORTAUDIO_DRIVER
/************************** BEGIN portaudio-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __portaudio_dsp__
#define __portaudio_dsp__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <portaudio.h>


static bool pa_error(int err)
{
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return true;
    } else {
        return false;
    }
}

/******************************************************************************
 *******************************************************************************
 
 PORT AUDIO INTERFACE
 
 *******************************************************************************
 *******************************************************************************/

class portaudio : public audio {
    
    protected:
        
        dsp* fDsp;
        PaStream* fAudioStream;
        long fSampleRate;
        long fBufferSize;
        PaStreamParameters fInputParameters;
        PaStreamParameters fOutputParameters;
        
        //----------------------------------------------------------------------------
        // 	Number of physical input and output channels of the PA device
        //----------------------------------------------------------------------------
        int	fDevNumInChans;
        int	fDevNumOutChans;
        
        static int audioCallback(const void* ibuf, void* obuf, unsigned long frames, const PaStreamCallbackTimeInfo* time, PaStreamCallbackFlags, void* drv)
        {
            return static_cast<portaudio*>(drv)->processAudio(time->currentTime, 
                                                            (float**)ibuf, 
                                                            static_cast<float**>(obuf), 
                                                            frames);
        }
        
        virtual int processAudio(PaTime current_time, float** ibuf, float** obuf, unsigned long frames) 
        {
            // Cleanup hardware outputs that are not used by DSP
            for (int i = fDsp->getNumOutputs(); i < fDevNumOutChans; i++) {
                memset(obuf[i], 0, sizeof(FAUSTFLOAT) * fBufferSize);
            }
            
            // Process samples
            fDsp->compute(current_time * 1000000., frames, ibuf, obuf);
            return paContinue;
        }
        
    public:
        
        portaudio(long srate, long bsize) : 
                fDsp(0), fAudioStream(0),
                fSampleRate(srate), fBufferSize(bsize), 
                fDevNumInChans(0), fDevNumOutChans(0) {}
                
        virtual ~portaudio() 
        {   
            if (fAudioStream) {
                pa_error(Pa_StopStream(fAudioStream));
                pa_error(Pa_CloseStream(fAudioStream));
                fAudioStream = 0;
            }
            // Note that Pa_Initialize handled multiple times calls and 
            // must be matched with a corresponding call to Pa_Terminate
            Pa_Terminate();
        }
        
        virtual bool init(const char* name, dsp* DSP)
        {
            if (init(name, DSP->getNumInputs(), DSP->getNumOutputs())) {
                setDsp(DSP);
                return true;
            } else {
                return false;
            }
        }
        
        bool init(const char* /*name*/, int numInputs, int numOutputs)
        {         
            // Note that Pa_Initialize handled multiple times calls and 
            // must be matched with a corresponding call to Pa_Terminate
            if (pa_error(Pa_Initialize())) {
                return false;
            }
            
            const PaDeviceInfo*	idev = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
            const PaDeviceInfo*	odev = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
            
            printf("DEVICE = %p || %p\n", idev, odev);
            
            //In case there is no audio device, the function fails
            
            if (idev == NULL) {
                fDevNumInChans = 0;
            } else {
                fDevNumInChans = idev->maxInputChannels;
                fInputParameters.device = Pa_GetDefaultInputDevice();
                fInputParameters.sampleFormat = paFloat32 | paNonInterleaved;
                fInputParameters.channelCount = fDevNumInChans;
                fInputParameters.hostApiSpecificStreamInfo = 0;
            }
            
            if (odev == NULL) {
                fDevNumOutChans = 0;
            } else{
                fDevNumOutChans = odev->maxOutputChannels;
                fOutputParameters.device = Pa_GetDefaultOutputDevice();
                fOutputParameters.sampleFormat = paFloat32 | paNonInterleaved;;
                fOutputParameters.channelCount = fDevNumOutChans;
                fOutputParameters.hostApiSpecificStreamInfo = 0;
            }
            
            // A DSP that has only outputs or only inputs forces the presence of an output or input device
            if (numInputs == 0 && numOutputs != 0 && fDevNumOutChans == 0) {
                printf("Devices not adapted to DSP\n");
                return false;
            }
            
            if (numInputs != 0 && numOutputs == 0 && fDevNumInChans == 0) {
                printf("Devices not adapted to DSP\n");
                return false;
            }
            
            // If no device exists : the function fails
            PaError err;
            if ((err = Pa_IsFormatSupported(((fDevNumInChans > 0) ? &fInputParameters : 0),
                                            ((fDevNumOutChans > 0) ? &fOutputParameters : 0), fSampleRate)) != 0) {
                printf("stream format is not supported err = %d\n", err);
                return false;
            }
            
            if (pa_error(Pa_OpenStream(&fAudioStream, ((fDevNumInChans > 0) ? &fInputParameters : 0),
                                       ((fDevNumOutChans > 0) ? &fOutputParameters : 0), 
                                       fSampleRate, fBufferSize, paNoFlag, audioCallback, this))) {
                return false;
            }    
            
            return true;
        }
        
        void setDsp(dsp* DSP)
        {
            fDsp = DSP;
            if (fDsp->getNumInputs() > fDevNumInChans || fDsp->getNumOutputs() > fDevNumOutChans) {
                printf("DSP has %d inputs and %d outputs, physical inputs = %d physical outputs = %d \n", 
                       fDsp->getNumInputs(), fDsp->getNumOutputs(), 
                       fDevNumInChans, fDevNumOutChans);
                fDsp = new dsp_adapter(fDsp, fDevNumInChans, fDevNumOutChans, fBufferSize);
            }
            
            fDsp->init(fSampleRate);
        }
        
        virtual bool start() 
        {
            if (pa_error(Pa_StartStream(fAudioStream))) {
                return false;
            } else {
                return true;
            }
        }
        
        virtual void stop() 
        {
            if (fAudioStream) {
                pa_error(Pa_StopStream(fAudioStream));
            }
        }
        
        virtual int getBufferSize() 
        { 
            return fBufferSize; 
        }
        
        virtual int getSampleRate()
        { 
            return fSampleRate; 
        }
        
        virtual int getNumInputs()
        {
            return fDevNumInChans;
        }
        
        virtual int getNumOutputs()
        {
            return fDevNumOutChans;
        }
};

#endif
/**************************  END  portaudio-dsp.h **************************/
#elif RTAUDIO_DRIVER
/************************** BEGIN rtaudio-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __rtaudio_dsp__
#define __rtaudio_dsp__

#include <stdio.h>
#include <assert.h>
#include <RtAudio.h>
#include <stdlib.h>


#define FORMAT RTAUDIO_FLOAT32

/******************************************************************************
 *******************************************************************************
 
 RTAUDIO INTERFACE
 
 *******************************************************************************
 *******************************************************************************/

class rtaudio : public audio {
    
    protected:
        
        dsp* fDsp;
        RtAudio fAudioDAC;
        unsigned int fSampleRate;
        unsigned int fBufferSize;
         
        //----------------------------------------------------------------------------
        // 	number of physical input and output channels of the PA device
        //----------------------------------------------------------------------------
        int	fDevNumInChans;
        int	fDevNumOutChans;
        
        virtual int processAudio(double streamTime, void* inbuf, void* outbuf, unsigned long frames) 
        {
            float* inputs[fDsp->getNumInputs()];
            float* outputs[fDsp->getNumOutputs()];
            
            for (int i = 0; i < fDsp->getNumInputs(); i++) {
                inputs[i] = &(static_cast<float*>(inbuf))[i * frames];
            }
            for (int i = 0; i < fDsp->getNumOutputs(); i++) {
                outputs[i] = &(static_cast<float*>(outbuf))[i * frames];
            }

            // process samples
            fDsp->compute(streamTime * 1000000., frames, inputs, outputs);
            return 0;
        }
    
        static int audioCallback(void* outputBuffer, void* inputBuffer, 
                                unsigned int nBufferFrames,
                                double streamTime, RtAudioStreamStatus status, 
                                void* drv)
        {
            return static_cast<rtaudio*>(drv)->processAudio(streamTime, inputBuffer, outputBuffer, nBufferFrames);
        }
      
    public:
        
        rtaudio(int srate, int bsize) : fDsp(0),
                fSampleRate(srate), fBufferSize(bsize), 
                fDevNumInChans(0), fDevNumOutChans(0) {}
            
        virtual ~rtaudio() 
        {   
            try {
                fAudioDAC.stopStream();
                fAudioDAC.closeStream();
            } catch (RtAudioError& e) {
                std::cout << '\n' << e.getMessage() << '\n' << std::endl;
            }
        }
        
        virtual bool init(const char* name, dsp* DSP)
        {
            if (init(name, DSP->getNumInputs(), DSP->getNumOutputs())) {
                setDsp(DSP);
                return true;
            } else {
                return false;
            }
        }
        
        bool init(const char* /*name*/, int numInputs, int numOutputs)
        {           
            if (fAudioDAC.getDeviceCount() < 1) {
                std::cout << "No audio devices found!\n";
                return false;
            }
            
            RtAudio::DeviceInfo info_in = fAudioDAC.getDeviceInfo(fAudioDAC.getDefaultInputDevice());
            RtAudio::DeviceInfo info_out = fAudioDAC.getDeviceInfo(fAudioDAC.getDefaultOutputDevice());
            RtAudio::StreamParameters iParams, oParams;
            
            iParams.deviceId = fAudioDAC.getDefaultInputDevice();
            fDevNumInChans = info_in.inputChannels;
            iParams.nChannels = fDevNumInChans;
            iParams.firstChannel = 0;
            
            oParams.deviceId = fAudioDAC.getDefaultOutputDevice();
            fDevNumOutChans = info_out.outputChannels;
            oParams.nChannels = fDevNumOutChans;
            oParams.firstChannel = 0;
            
            RtAudio::StreamOptions options;
            options.flags |= RTAUDIO_NONINTERLEAVED;
         
            try {
                fAudioDAC.openStream(((numOutputs > 0) ? &oParams : NULL), 
                    ((numInputs > 0) ? &iParams : NULL), FORMAT, 
                    fSampleRate, &fBufferSize, audioCallback, this, &options);
            } catch (RtAudioError& e) {
                std::cout << '\n' << e.getMessage() << '\n' << std::endl;
                return false;
            }
               
            return true;
        }
        
        void setDsp(dsp* DSP)
        {
            fDsp = DSP;
            
            if (fDsp->getNumInputs() > fDevNumInChans || fDsp->getNumOutputs() > fDevNumOutChans) {
                printf("DSP has %d inputs and %d outputs, physical inputs = %d physical outputs = %d \n", 
                       fDsp->getNumInputs(), fDsp->getNumOutputs(), 
                       fDevNumInChans, fDevNumOutChans);
                fDsp = new dsp_adapter(fDsp, fDevNumInChans, fDevNumOutChans, fBufferSize);
            }
            
            fDsp->init(fSampleRate);
        }
        
        virtual bool start() 
        {
            try {
                fAudioDAC.startStream();
            } catch (RtAudioError& e) {
                std::cout << '\n' << e.getMessage() << '\n' << std::endl;
                return false;
            }
            return true;
        }
        
        virtual void stop() 
        {
            try {
                fAudioDAC.stopStream();
            } catch (RtAudioError& e) {
                std::cout << '\n' << e.getMessage() << '\n' << std::endl;
            }
        }
        
        virtual int getBufferSize() 
        { 
            return fBufferSize; 
        }
        
        virtual int getSampleRate()
        { 
            return fSampleRate; 
        }
        
        virtual int getNumInputs()
        {
            return fDevNumInChans;
        }
        
        virtual int getNumOutputs()
        {
            return fDevNumOutChans;
        }
};

#endif
/**************************  END  rtaudio-dsp.h **************************/
#elif OPEN_FRAMEWORK_DRIVER
/************************** BEGIN ofaudio-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __openframework__
#define __openframework__

#include "ofSoundStream.h"
#include "ofBaseTypes.h"


class ofaudio : public audio, public ofBaseSoundInput, public ofBaseSoundOutput {
    
    private:
    
        ofSoundStream fStream;
        float* fInBuffer;
        float** fNIInputs;
        float** fNIOutputs;
        int fSampleRate;
        int fBufferSize;
        dsp* fDSP;
    
    public:
    
        ofaudio(int srate, int bsize):fInBuffer(nullptr), fSampleRate(srate), fBufferSize(bsize) {}
        virtual ~ofaudio()
        {
            fStream.stop();
            fStream.close();
            
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                delete [] fNIInputs[i];
            }
            delete [] fNIInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                delete [] fNIOutputs[i];
            }
            delete [] fNIOutputs;
        }
    
        void audioIn(float* input, int bufferSize, int nChannels)
        {
            // Keep the input buffer to be used in 'audioOut' for the same audio cycle
            fInBuffer = input;
        }
    
        void audioOut(float* output, int bufferSize, int nChannels)
        {
            // Deinterleave input (= fInBuffer)
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                for (int frame = 0; frame < bufferSize; frame++) {
                    fNIInputs[chan][frame] = fInBuffer[chan + frame * fDSP->getNumInputs()];
                }
            }
            
            fDSP->compute(bufferSize, fNIInputs, fNIOutputs);
            
            // Interleave output
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                for (int frame = 0; frame < bufferSize; frame++) {
                    output[chan + frame * fDSP->getNumOutputs()] = fNIOutputs[chan][frame];
                }
            }
        }

        bool init(const char* name, dsp* dsp)
        {
            fDSP = dsp;
            
            //fStream.printDeviceList();
            
            fNIInputs = new float*[fDSP->getNumInputs()];
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                fNIInputs[i] = new float[4046];
            }
            fNIOutputs = new float*[fDSP->getNumOutputs()];
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                fNIOutputs[i] = new float[4046];
            }
            
            if (fDSP->getNumInputs() > 0) {
                fStream.setInput(this);
            }
            if (fDSP->getNumOutputs() > 0) {
                fStream.setOutput(this);
            }
            
            fDSP->init(fSampleRate);
            return true;
        }
    
        bool start()
        {
            // 'setup' also starts the stream...
            return fStream.setup(fDSP->getNumOutputs(), fDSP->getNumInputs(), fSampleRate, fBufferSize, 1);
        }
    
        void stop()
        {
            fStream.stop();
        }
    
        int getBufferSize()
        {
            return fStream.getBufferSize();
        }
    
        int getSampleRate()
        {
            return fStream.getSampleRate();
        }

        int getNumInputs() { return fStream.getNumInputChannels(); }
        int getNumOutputs() { return fStream.getNumOutputChannels(); }

        float getCPULoad() { return 0.f; }
};
					
#endif
/**************************  END  ofaudio-dsp.h **************************/
#elif JUCE_DRIVER
/************************** BEGIN juce-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __juce_dsp__
#define __juce_dsp__

#include "../JuceLibraryCode/JuceHeader.h"


class juceaudio : public audio, private AudioAppComponent {
    
    private:
        
        dsp* fDSP;
    
        void prepareToPlay(int, double) override
        {
            const BigInteger activeInputChannels = deviceManager.getCurrentAudioDevice()->getActiveInputChannels();
            const BigInteger activeOutputChannels = deviceManager.getCurrentAudioDevice()->getActiveOutputChannels();
            const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
            const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;
            
            // Possibly adapt DSP...
            if (fDSP->getNumInputs() > maxInputChannels || fDSP->getNumOutputs() > maxOutputChannels) {
                fDSP = new dsp_adapter(fDSP, maxInputChannels, maxOutputChannels, 4096);
            }
            
            fDSP->init(int(deviceManager.getCurrentAudioDevice()->getCurrentSampleRate()));
        }
        
        void releaseResources() override
        {}
        
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
        {
            AVOIDDENORMALS;
            
            const float** inputs = (const float**)alloca(fDSP->getNumInputs() * sizeof(float));
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                inputs[i] = bufferToFill.buffer->getReadPointer(i, bufferToFill.startSample);
            }
            
            float** outputs = (float**)alloca(fDSP->getNumOutputs() * sizeof(float));
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                outputs[i] = bufferToFill.buffer->getWritePointer(i, bufferToFill.startSample);
            }
            
            // MIDI timestamp is expressed in frames
            fDSP->compute(-1, bufferToFill.numSamples, (float**)inputs, outputs);
        }
    
    public:
    
        juceaudio() {}
        virtual ~juceaudio()
        {
            shutdownAudio();
        }
        
        bool init(const char* name, dsp* dsp) override
        {
            fDSP = dsp;
            return true;
        }
        
        bool start() override
        {
            setAudioChannels (fDSP->getNumInputs(), fDSP->getNumOutputs());
            prepareToPlay(0, 0); // Unused samplerate and buffersize, taken from deviceManager
            return true;
        }
        
        void stop() override
        {
            shutdownAudio();
        }
        
        int getBufferSize() override
        {
            return int(deviceManager.getCurrentAudioDevice()->getCurrentBufferSizeSamples());
        }
        
        int getSampleRate() override
        {
            return int(deviceManager.getCurrentAudioDevice()->getCurrentSampleRate());
        }
        
        int getNumInputs() override { return deviceManager.getCurrentAudioDevice()->getActiveInputChannels().toInteger(); }
        int getNumOutputs() override { return deviceManager.getCurrentAudioDevice()->getActiveOutputChannels().toInteger(); }
        
        float getCPULoad() override { return float(deviceManager.getCpuUsage()); }
};

#endif
/**************************  END  juce-dsp.h **************************/
#elif DUMMY_DRIVER
/************************** BEGIN dummy-audio.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dummy_audio__
#define __dummy_audio__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <iomanip>

#ifdef USE_PTHREAD
#include <pthread.h>
#else
#include <thread>
#endif


#define BUFFER_TO_RENDER 10

class dummyaudio : public audio {
    
    private:
        
        dsp* fDSP;
        
        long fSampleRate;
        long fBufferSize;
        
        FAUSTFLOAT** fInChannel;
        FAUSTFLOAT** fOutChannel;
        
        int fNumInputs;
        int fNumOutputs;
        
        bool fRunning;
        
        int fRender;
        int fCount;
        int fSample;
        bool fManager;
        
    #ifdef USE_PTHREAD
        pthread_t fAudioThread;
        static void* run(void* ptr)
        {
            dummyaudio* audio = (dummyaudio*)ptr;
            try {
                audio->process();
            } catch (...) {}
            return 0;
        }
    #else
        static void run(dummyaudio* audio)
        {
            try {
                audio->process();
            } catch (...) {}
        }
        std::thread* fAudioThread = nullptr;
    #endif
        
        void process()
        {
            while (fRunning && (--fRender > 0)) {
                if (fSample > 0) { std::cout << "Render one buffer\n"; }
                render();
            }
            fRunning = false;
        }
        
    public:
        
        dummyaudio(int sr, int bs, int count = BUFFER_TO_RENDER, int sample = -1, bool manager = false)
        :fSampleRate(sr), fBufferSize(bs),
        fInChannel(nullptr), fOutChannel(nullptr),
        fNumInputs(-1), fNumOutputs(-1),
        fRender(0), fCount(count),
        fSample(sample), fManager(manager)
        {}
        
        dummyaudio(int count = BUFFER_TO_RENDER)
        :fSampleRate(48000), fBufferSize(512),
        fInChannel(nullptr), fOutChannel(nullptr),
        fNumInputs(-1), fNumOutputs(-1),
        fRender(0), fCount(count),
        fSample(512), fManager(false)
        {}
        
        virtual ~dummyaudio()
        {
            for (int i = 0; i < fNumInputs; i++) {
                delete[] fInChannel[i];
            }
            for (int i = 0; i < fNumOutputs; i++) {
                delete[] fOutChannel[i];
            }
            delete [] fInChannel;
            delete [] fOutChannel;
        }
        
        virtual bool init(const char* name, dsp* dsp)
        {
            fDSP = dsp;
            
            // To be used in destructor
            fNumInputs = fDSP->getNumInputs();
            fNumOutputs = fDSP->getNumOutputs();
            
            fInChannel = new FAUSTFLOAT*[fNumInputs];
            fOutChannel = new FAUSTFLOAT*[fNumOutputs];
            
            for (int i = 0; i < fNumInputs; i++) {
                fInChannel[i] = new FAUSTFLOAT[fBufferSize];
                memset(fInChannel[i], 0, sizeof(FAUSTFLOAT) * fBufferSize);
            }
            for (int i = 0; i < fNumOutputs; i++) {
                fOutChannel[i] = new FAUSTFLOAT[fBufferSize];
                memset(fOutChannel[i], 0, sizeof(FAUSTFLOAT) * fBufferSize);
            }
            
            if (fManager) {
                // classInit is called elsewhere with a custom memory manager
                fDSP->instanceInit(fSampleRate);
            } else {
                fDSP->init(fSampleRate);
            }
            
            return true;
        }
        
        virtual bool start()
        {
            fRender = fCount;
            fRunning = true;
            if (fCount == INT_MAX) {
            #ifdef USE_PTHREAD
                if (pthread_create(&fAudioThread, 0, run, this) != 0) {
                    fRunning = false;
                }
            #else
                fAudioThread = new std::thread (dummyaudio::run, this);
            #endif
                return fRunning;
            } else {
                process();
                return true;
            }
        }
        
        virtual void stop()
        {
            if (fRunning) {
                fRunning = false;
            #ifdef USE_PTHREAD
                pthread_join(fAudioThread, 0);
            #else
                fAudioThread->join();
                delete fAudioThread;
                fAudioThread = 0;
            #endif
            }
        }
        
        void render()
        {
            fDSP->compute(fBufferSize, fInChannel, fOutChannel);
            if (fNumInputs > 0) {
                for (int frame = 0; frame < fSample; frame++) {
                    std::cout << std::fixed << std::setprecision(6) << "sample in " << fInChannel[0][frame] << std::endl;
                }
            }
            if (fNumOutputs > 0) {
                for (int frame = 0; frame < fSample; frame++) {
                    std::cout << std::fixed << std::setprecision(6) << "sample out " << fOutChannel[0][frame] << std::endl;
                }
            }
        }
        
        virtual int getBufferSize() { return fBufferSize; }
        virtual int getSampleRate() { return fSampleRate; }
        
        virtual int getNumInputs() { return fNumInputs; }
        virtual int getNumOutputs() { return fNumOutputs; }
    
};

#endif
/**************************  END  dummy-audio.h **************************/
#endif

//**************************************************************
// Interface
//**************************************************************

#if MIDICTRL
#if JACK_DRIVER
    // Nothing to add since jack-dsp.h contains MIDI
#elif JUCE_DRIVER
/************************** BEGIN juce-midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __juce_midi__
#define __juce_midi__
 

class MapUI;

//-----------------------------------------------
// MIDI input/output handling using JUCE library
//-----------------------------------------------

class juce_midi_handler : public midi_handler {
    
    protected:
    
        MidiBuffer fOutputBuffer;
        CriticalSection fMutex;
    
        void decodeMessage(const MidiMessage& message)
        {
            const uint8* data = message.getRawData();
            int channel = message.getChannel() - 1; // which MIDI channel, 0-15
            double time = message.getTimeStamp();
            
            if (message.isNoteOff()) {
                handleKeyOff(time, channel, data[1], data[2]);
            } else if (message.isNoteOn()) {
                handleKeyOn(time, channel, data[1], data[2]);
            } else if (message.isAftertouch()) {
                handlePolyAfterTouch(time, channel, data[1], data[2]);
            } else if (message.isController()) {
                handleCtrlChange(time, channel, data[1], data[2]);
            } else if (message.isProgramChange()) {
                handleProgChange(time, channel, data[1]);
            } else if (message.isChannelPressure()) {
                handleAfterTouch(time, channel, data[1]);
            } else if (message.isPitchWheel()) {
                handlePitchWheel(time, channel, data[1], data[2]);
            } else if (message.isMidiClock()) {
                handleClock(time);
            // We can consider start and continue as identical messages.
            } else if (message.isMidiStart() || message.isMidiContinue()) {
                handleStart(time);
            } else if (message.isMidiStop()) {
                handleStop(time);
            } else if (message.isSysEx()) {
                std::vector<unsigned char> sysex(data, data + message.getRawDataSize());
                handleSysex(time, sysex);
            } else {
                std::cerr << "Unused MIDI message" << std::endl;
            }
        }
    
    public:
    
        juce_midi_handler():midi_handler("JUCE")
        {}
    
        virtual ~juce_midi_handler() {}
    
        // Used with MidiBuffer (containing several messages)
        void encodeBuffer(MidiBuffer& buffer)
        {
            const ScopedTryLock lock(fMutex);
            if (lock.isLocked()) {
                buffer.swapWith(fOutputBuffer);
                fOutputBuffer.clear();
            } else {
                std::cerr << "encodeBuffer fails..." << std::endl;
            }
        }
        
        void decodeBuffer(MidiBuffer& buffer)
        {
            MidiMessage msg;
            int ignore;
            for (MidiBuffer::Iterator it(buffer); it.getNextEvent(msg, ignore);) {
                decodeMessage(msg);
            }
            buffer.clear();
        }
    
        // MIDI output API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            fOutputBuffer.addEvent(MidiMessage::noteOn(channel + 1, pitch, uint8(velocity)), 0);
            return nullptr;
        }
        
        void keyOff(int channel, int pitch, int velocity)
        {
            fOutputBuffer.addEvent(MidiMessage::noteOff(channel + 1, pitch, uint8(velocity)), 0);
        }
        
        void ctrlChange(int channel, int ctrl, int val)
        {
            fOutputBuffer.addEvent(MidiMessage::controllerEvent(channel + 1, ctrl, uint8(val)), 0);
        }
        
        void chanPress(int channel, int press)
        {
            fOutputBuffer.addEvent(MidiMessage::channelPressureChange(channel + 1, press), 0);
        }
        
        void progChange(int channel, int pgm)
        {
            fOutputBuffer.addEvent(MidiMessage::programChange(channel + 1, pgm), 0);
        }
        
        void keyPress(int channel, int pitch, int press)
        {
            fOutputBuffer.addEvent(MidiMessage::aftertouchChange(channel + 1, pitch, press), 0);
        }
        
        void pitchWheel(int channel, int wheel)
        {
            fOutputBuffer.addEvent(MidiMessage::pitchWheel(channel + 1, range(0, 16383, wheel)), 0);
        }
        
        void ctrlChange14bits(int channel, int ctrl, int value)
        {
            // TODO
        }
        
        void startSync(double date)
        {
            fOutputBuffer.addEvent(MidiMessage::midiStart(), 0);
        }
        
        void stopSync(double date)
        {
            fOutputBuffer.addEvent(MidiMessage::midiStop(), 0);
        }
        
        void clock(double date)
        {
            fOutputBuffer.addEvent(MidiMessage::midiClock(), 0);
        }
    
        void sysEx(double date, std::vector<unsigned char>& message)
        {
            fOutputBuffer.addEvent(MidiMessage(message.data(), (int)message.size()), 0);
        }

};

class juce_midi : public juce_midi_handler, public MidiInputCallback {

    private:
    
        std::unique_ptr<MidiInput> fMidiIn;
        std::unique_ptr<MidiOutput> fMidiOut;
    
        void handleIncomingMidiMessage(MidiInput*, const MidiMessage& message)
        {
            decodeMessage(message);
        }
    
    public:
    
        virtual ~juce_midi()
        {
            stopMidi();
        }
        
        bool startMidi()
        {
            if ((fMidiIn = MidiInput::openDevice(MidiInput::getDefaultDeviceIndex(), this)) == nullptr) {
                return false;
            }
            if ((fMidiOut = MidiOutput::openDevice(MidiOutput::getDefaultDeviceIndex())) == nullptr) {
                return false;
            }
            fMidiIn->start();
            return true;
        }
        
        void stopMidi()
        { 
            fMidiIn->stop();
        }
    
        // MIDI output API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            fMidiOut->sendMessageNow(MidiMessage::noteOn(channel + 1, pitch, uint8(velocity)));
            return nullptr;
        }
        
        void keyOff(int channel, int pitch, int velocity) 
        {
            fMidiOut->sendMessageNow(MidiMessage::noteOff(channel + 1, pitch, uint8(velocity)));
        }
        
        void ctrlChange(int channel, int ctrl, int val) 
        {
            fMidiOut->sendMessageNow(MidiMessage::controllerEvent(channel + 1, ctrl, uint8(val)));
        }
        
        void chanPress(int channel, int press) 
        {
            fMidiOut->sendMessageNow(MidiMessage::channelPressureChange(channel + 1, press));
        }
        
        void progChange(int channel, int pgm) 
        {
            fMidiOut->sendMessageNow(MidiMessage::programChange(channel + 1, pgm));
        }
          
        void keyPress(int channel, int pitch, int press) 
        {
            fMidiOut->sendMessageNow(MidiMessage::aftertouchChange(channel + 1, pitch, press));
        }
   
        void pitchWheel(int channel, int wheel) 
        {
            fMidiOut->sendMessageNow(MidiMessage::pitchWheel(channel + 1, range(0, 16383, wheel)));
        }
        
        void ctrlChange14bits(int channel, int ctrl, int value)
        {
            // TODO
        }
    
        void startSync(double date) 
        {
            fMidiOut->sendMessageNow(MidiMessage::midiStart());
        }
       
        void stopSync(double date) 
        {
            fMidiOut->sendMessageNow(MidiMessage::midiStop());
        }
        
        void clock(double date) 
        {
            fMidiOut->sendMessageNow(MidiMessage::midiClock());
        }
    
        void sysEx(double date, std::vector<unsigned char>& message)
        {
            fMidiOut->sendMessageNow(MidiMessage(message.data(), (int)message.size()));
        }
    
};

#endif // __juce_midi__

/**************************  END  juce-midi.h **************************/
#else
/************************** BEGIN rt-midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __rt_midi__
#define __rt_midi__ 
 
#include <iostream>
#include <cstdlib>
/************************** BEGIN RtMidi.h **************************/
/*
 File: CAHostTimeBase.h
 Abstract: Part of CoreAudio Utility Classes
 Version: 1.0.3
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2013 Apple Inc. All Rights Reserved.
 */

#if !defined(__CAHostTimeBase_h__)
#define __CAHostTimeBase_h__

#ifdef __APPLE__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
#include <CoreAudio/CoreAudioTypes.h>
#else
#include <CoreAudioTypes.h>
#endif

#if TARGET_OS_MAC
#include <mach/mach_time.h>
#elif TARGET_OS_WIN32
#include <windows.h>
#else
#error	Unsupported operating system
#endif

//=============================================================================
//	CAHostTimeBase
//
//	This class provides platform independent access to the host's time base.
//=============================================================================

class CAHostTimeBase
{
    
public:
    static UInt64	ConvertToNanos(UInt64 inHostTime);
    static UInt64	ConvertFromNanos(UInt64 inNanos);
    
    static UInt64	GetTheCurrentTime();
#if TARGET_OS_MAC
    static UInt64	GetCurrentTime() { return GetTheCurrentTime(); }
#endif
    static UInt64	GetCurrentTimeInNanos();
    
    static Float64	GetFrequency() { if(!sIsInited) { Initialize(); } return sFrequency; }
    static Float64	GetInverseFrequency() { if(!sIsInited) { Initialize(); } return sInverseFrequency; }
    static UInt32	GetMinimumDelta() { if(!sIsInited) { Initialize(); } return sMinDelta; }
    
    static UInt64	AbsoluteHostDeltaToNanos(UInt64 inStartTime, UInt64 inEndTime);
    static SInt64	HostDeltaToNanos(UInt64 inStartTime, UInt64 inEndTime);
    
    static void     Initialize();
    
private:
    
    static bool sIsInited;
    
    static Float64 sFrequency;
    static Float64 sInverseFrequency;
    static UInt32 sMinDelta;
    static UInt32 sToNanosNumerator;
    static UInt32 sToNanosDenominator;
    static UInt32 sFromNanosNumerator;
    static UInt32 sFromNanosDenominator;
    static bool sUseMicroseconds;
#if Track_Host_TimeBase
    static UInt64	sLastTime;
#endif
};

inline UInt64 CAHostTimeBase::GetTheCurrentTime()
{
    UInt64 theTime = 0;
    
#if TARGET_OS_MAC
    theTime = mach_absolute_time();
    
#elif TARGET_OS_WIN32
    LARGE_INTEGER theValue;
    QueryPerformanceCounter(&theValue);
    theTime = *((UInt64*)&theValue);
#endif
    
#if	Track_Host_TimeBase
    if(sLastTime != 0)
    {
        if(theTime <= sLastTime)
        {
            DebugMessageN2("CAHostTimeBase::GetTheCurrentTime: the current time is earlier than the last time, now: %qd, then: %qd", theTime, sLastTime);
        }
        sLastTime = theTime;
    }
    else
    {
        sLastTime = theTime;
    }
#endif
    
    return theTime;
}

inline UInt64 CAHostTimeBase::ConvertToNanos(UInt64 inHostTime)
{
    if(!sIsInited)
    {
        Initialize();
    }
    
    Float64 theNumerator = static_cast<Float64>(sToNanosNumerator);
    Float64 theDenominator = static_cast<Float64>(sToNanosDenominator);
    Float64 theHostTime = static_cast<Float64>(inHostTime);
    
    Float64 thePartialAnswer = theHostTime / theDenominator;
    Float64 theFloatAnswer = thePartialAnswer * theNumerator;
    UInt64 theAnswer = static_cast<UInt64>(theFloatAnswer);
    
    return theAnswer;
}

inline UInt64 CAHostTimeBase::ConvertFromNanos(UInt64 inNanos)
{
    if(!sIsInited)
    {
        Initialize();
    }
    
    Float64 theNumerator = static_cast<Float64>(sToNanosNumerator);
    Float64 theDenominator = static_cast<Float64>(sToNanosDenominator);
    Float64 theNanos = static_cast<Float64>(inNanos);
    
    Float64 thePartialAnswer = theNanos / theNumerator;
    Float64 theFloatAnswer = thePartialAnswer * theDenominator;
    UInt64 theAnswer = static_cast<UInt64>(theFloatAnswer);
    
    return theAnswer;
}

inline UInt64 CAHostTimeBase::GetCurrentTimeInNanos()
{
    return ConvertToNanos(GetTheCurrentTime());
}

inline UInt64 CAHostTimeBase::AbsoluteHostDeltaToNanos(UInt64 inStartTime, UInt64 inEndTime)
{
    UInt64 theAnswer;
    
    if(inStartTime <= inEndTime)
    {
        theAnswer = inEndTime - inStartTime;
    }
    else
    {
        theAnswer = inStartTime - inEndTime;
    }
    
    return ConvertToNanos(theAnswer);
}

inline SInt64 CAHostTimeBase::HostDeltaToNanos(UInt64 inStartTime, UInt64 inEndTime)
{
    SInt64 theAnswer;
    SInt64 theSign = 1;
    
    if(inStartTime <= inEndTime)
    {
        theAnswer = inEndTime - inStartTime;
    }
    else
    {
        theAnswer = inStartTime - inEndTime;
        theSign = -1;
    }
    
    return theSign * ConvertToNanos(theAnswer);
}

#endif // __APPLE__

#endif

/**********************************************************************/
/*! \class RtMidi
    \brief An abstract base class for realtime MIDI input/output.

    This class implements some common functionality for the realtime
    MIDI input/output subclasses RtMidiIn and RtMidiOut.

    RtMidi WWW site: http://music.mcgill.ca/~gary/rtmidi/

    RtMidi: realtime MIDI i/o C++ classes
    Copyright (c) 2003-2017 Gary P. Scavone

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    Any person wishing to distribute modifications to the Software is
    asked to send the modifications to the original developer so that
    they can be incorporated into the canonical version.  This is,
    however, not a binding provision of this license.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/**********************************************************************/

/*!
  \file RtMidi.h
 */

#ifndef RTMIDI_H
#define RTMIDI_H

#define RTMIDI_VERSION "3.0.0"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#if __APPLE__
#define __MACOSX_CORE__ 1
//#define __UNIX_JACK__ 1
#endif

#if __linux__
#define __LINUX_ALSA__ 1
#endif

#if _WIN32
#define __WINDOWS_MM__ 1
#endif


/************************************************************************/
/*! \class RtMidiError
    \brief Exception handling class for RtMidi.

    The RtMidiError class is quite simple but it does allow errors to be
    "caught" by RtMidiError::Type. See the RtMidi documentation to know
    which methods can throw an RtMidiError.
*/
/************************************************************************/

class RtMidiError : public std::exception
{
 public:
  //! Defined RtMidiError types.
  enum Type {
    WARNING,           /*!< A non-critical error. */
    DEBUG_WARNING,     /*!< A non-critical error which might be useful for debugging. */
    UNSPECIFIED,       /*!< The default, unspecified error type. */
    NO_DEVICES_FOUND,  /*!< No devices found on system. */
    INVALID_DEVICE,    /*!< An invalid device ID was specified. */
    MEMORY_ERROR,      /*!< An error occured during memory allocation. */
    INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
    INVALID_USE,       /*!< The function was called incorrectly. */
    DRIVER_ERROR,      /*!< A system driver error occured. */
    SYSTEM_ERROR,      /*!< A system error occured. */
    THREAD_ERROR       /*!< A thread error occured. */
  };

  //! The constructor.
  RtMidiError( const std::string& message, Type type = RtMidiError::UNSPECIFIED ) throw() : message_(message), type_(type) {}
 
  //! The destructor.
  virtual ~RtMidiError( void ) throw() {}

  //! Prints thrown error message to stderr.
  virtual void printMessage( void ) const throw() { std::cerr << '\n' << message_ << "\n\n"; }

  //! Returns the thrown error message type.
  virtual const Type& getType(void) const throw() { return type_; }

  //! Returns the thrown error message string.
  virtual const std::string& getMessage(void) const throw() { return message_; }

  //! Returns the thrown error message as a c-style string.
  virtual const char* what( void ) const throw() { return message_.c_str(); }

 protected:
  std::string message_;
  Type type_;
};

//! RtMidi error callback function prototype.
/*!
    \param type Type of error.
    \param errorText Error description.

    Note that class behaviour is undefined after a critical error (not
    a warning) is reported.
 */
typedef void (*RtMidiErrorCallback)( RtMidiError::Type type, const std::string &errorText, void *userData );

class MidiApi;

class RtMidi
{
 public:

  //! MIDI API specifier arguments.
  enum Api {
    UNSPECIFIED,    /*!< Search for a working compiled API. */
    MACOSX_CORE,    /*!< Macintosh OS-X Core Midi API. */
    LINUX_ALSA,     /*!< The Advanced Linux Sound Architecture API. */
    UNIX_JACK,      /*!< The JACK Low-Latency MIDI Server API. */
    WINDOWS_MM,     /*!< The Microsoft Multimedia MIDI API. */
    RTMIDI_DUMMY    /*!< A compilable but non-functional API. */
  };

  //! A static function to determine the current RtMidi version.
  static std::string getVersion( void ) throw();

  //! A static function to determine the available compiled MIDI APIs.
  /*!
    The values returned in the std::vector can be compared against
    the enumerated list values.  Note that there can be more than one
    API compiled for certain operating systems.
  */
  static void getCompiledApi( std::vector<RtMidi::Api> &apis ) throw();

  //! Pure virtual openPort() function.
  virtual void openPort( unsigned int portNumber = 0, const std::string &portName = std::string( "RtMidi" ) ) = 0;

  //! Pure virtual openVirtualPort() function.
  virtual void openVirtualPort( const std::string &portName = std::string( "RtMidi" ) ) = 0;

  //! Pure virtual getPortCount() function.
  virtual unsigned int getPortCount() = 0;

  //! Pure virtual getPortName() function.
  virtual std::string getPortName( unsigned int portNumber = 0 ) = 0;

  //! Pure virtual closePort() function.
  virtual void closePort( void ) = 0;

  //! Returns true if a port is open and false if not.
  /*!
      Note that this only applies to connections made with the openPort()
      function, not to virtual ports.
  */
  virtual bool isPortOpen( void ) const = 0;

  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is best
    to set the error callback function before opening a port.
  */
  virtual void setErrorCallback( RtMidiErrorCallback errorCallback = NULL, void *userData = 0 ) = 0;

 protected:

  RtMidi();
  virtual ~RtMidi();

  MidiApi *rtapi_;
};

/**********************************************************************/
/*! \class RtMidiIn
    \brief A realtime MIDI input class.

    This class provides a common, platform-independent API for
    realtime MIDI input.  It allows access to a single MIDI input
    port.  Incoming MIDI messages are either saved to a queue for
    retrieval using the getMessage() function or immediately passed to
    a user-specified callback function.  Create multiple instances of
    this class to connect to more than one MIDI device at the same
    time.  With the OS-X, Linux ALSA, and JACK MIDI APIs, it is also
    possible to open a virtual input port to which other MIDI software
    clients can connect.

    by Gary P. Scavone, 2003-2017.
*/
/**********************************************************************/

// **************************************************************** //
//
// RtMidiIn and RtMidiOut class declarations.
//
// RtMidiIn / RtMidiOut are "controllers" used to select an available
// MIDI input or output interface.  They present common APIs for the
// user to call but all functionality is implemented by the classes
// MidiInApi, MidiOutApi and their subclasses.  RtMidiIn and RtMidiOut
// each create an instance of a MidiInApi or MidiOutApi subclass based
// on the user's API choice.  If no choice is made, they attempt to
// make a "logical" API selection.
//
// **************************************************************** //

class RtMidiIn : public RtMidi
{
 public:

  //! User callback function type definition.
  typedef void (*RtMidiCallback)( double timeStamp, std::vector<unsigned char> *message, void *userData);

  //! Default constructor that allows an optional api, client name and queue size.
  /*!
    An exception will be thrown if a MIDI system initialization
    error occurs.  The queue size defines the maximum number of
    messages that can be held in the MIDI queue (when not using a
    callback function).  If the queue size limit is reached,
    incoming messages will be ignored.

    If no API argument is specified and multiple API support has been
    compiled, the default order of use is ALSA, JACK (Linux) and CORE,
    JACK (OS-X).

    \param api        An optional API id can be specified.
    \param clientName An optional client name can be specified. This
                      will be used to group the ports that are created
                      by the application.
    \param queueSizeLimit An optional size of the MIDI input queue can be specified.
  */
  RtMidiIn( RtMidi::Api api=UNSPECIFIED,
            const std::string& clientName = "RtMidi Input Client",
            unsigned int queueSizeLimit = 100 );

  //! If a MIDI connection is still open, it will be closed by the destructor.
  ~RtMidiIn ( void ) throw();

  //! Returns the MIDI API specifier for the current instance of RtMidiIn.
  RtMidi::Api getCurrentApi( void ) throw();

  //! Open a MIDI input connection given by enumeration number.
  /*!
    \param portNumber An optional port number greater than 0 can be specified.
                      Otherwise, the default or first port found is opened.
    \param portName An optional name for the application port that is used to connect to portId can be specified.
  */
  void openPort( unsigned int portNumber = 0, const std::string &portName = std::string( "RtMidi Input" ) );

  //! Create a virtual input port, with optional name, to allow software connections (OS X, JACK and ALSA only).
  /*!
    This function creates a virtual MIDI input port to which other
    software applications can connect.  This type of functionality
    is currently only supported by the Macintosh OS-X, any JACK,
    and Linux ALSA APIs (the function returns an error for the other APIs).

    \param portName An optional name for the application port that is
                    used to connect to portId can be specified.
  */
  void openVirtualPort( const std::string &portName = std::string( "RtMidi Input" ) );

  //! Set a callback function to be invoked for incoming MIDI messages.
  /*!
    The callback function will be called whenever an incoming MIDI
    message is received.  While not absolutely necessary, it is best
    to set the callback function before opening a MIDI port to avoid
    leaving some messages in the queue.

    \param callback A callback function must be given.
    \param userData Optionally, a pointer to additional data can be
                    passed to the callback function whenever it is called.
  */
  void setCallback( RtMidiCallback callback, void *userData = 0 );

  //! Cancel use of the current callback function (if one exists).
  /*!
    Subsequent incoming MIDI messages will be written to the queue
    and can be retrieved with the \e getMessage function.
  */
  void cancelCallback();

  //! Close an open MIDI connection (if one exists).
  void closePort( void );

  //! Returns true if a port is open and false if not.
  /*!
      Note that this only applies to connections made with the openPort()
      function, not to virtual ports.
  */
  virtual bool isPortOpen() const;

  //! Return the number of available MIDI input ports.
  /*!
    \return This function returns the number of MIDI ports of the selected API.
  */
  unsigned int getPortCount();

  //! Return a string identifier for the specified MIDI input port number.
  /*!
    \return The name of the port with the given Id is returned.
    \retval An empty string is returned if an invalid port specifier
            is provided. User code should assume a UTF-8 encoding.
  */
  std::string getPortName( unsigned int portNumber = 0 );

  //! Specify whether certain MIDI message types should be queued or ignored during input.
  /*!
    By default, MIDI timing and active sensing messages are ignored
    during message input because of their relative high data rates.
    MIDI sysex messages are ignored by default as well.  Variable
    values of "true" imply that the respective message type will be
    ignored.
  */
  void ignoreTypes( bool midiSysex = true, bool midiTime = true, bool midiSense = true );

  //! Fill the user-provided vector with the data bytes for the next available MIDI message in the input queue and return the event delta-time in seconds.
  /*!
    This function returns immediately whether a new message is
    available or not.  A valid message is indicated by a non-zero
    vector size.  An exception is thrown if an error occurs during
    message retrieval or an input connection was not previously
    established.
  */
  double getMessage( std::vector<unsigned char> *message );

  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is best
    to set the error callback function before opening a port.
  */
  virtual void setErrorCallback( RtMidiErrorCallback errorCallback = NULL, void *userData = 0 );

 protected:
  void openMidiApi( RtMidi::Api api, const std::string &clientName, unsigned int queueSizeLimit );

};

/**********************************************************************/
/*! \class RtMidiOut
    \brief A realtime MIDI output class.

    This class provides a common, platform-independent API for MIDI
    output.  It allows one to probe available MIDI output ports, to
    connect to one such port, and to send MIDI bytes immediately over
    the connection.  Create multiple instances of this class to
    connect to more than one MIDI device at the same time.  With the
    OS-X, Linux ALSA and JACK MIDI APIs, it is also possible to open a
    virtual port to which other MIDI software clients can connect.

    by Gary P. Scavone, 2003-2017.
*/
/**********************************************************************/

class RtMidiOut : public RtMidi
{
 public:

  //! Default constructor that allows an optional client name.
  /*!
    An exception will be thrown if a MIDI system initialization error occurs.

    If no API argument is specified and multiple API support has been
    compiled, the default order of use is ALSA, JACK (Linux) and CORE,
    JACK (OS-X).
  */
  RtMidiOut( RtMidi::Api api=UNSPECIFIED,
             const std::string& clientName = "RtMidi Output Client" );

  //! The destructor closes any open MIDI connections.
  ~RtMidiOut( void ) throw();

  //! Returns the MIDI API specifier for the current instance of RtMidiOut.
  RtMidi::Api getCurrentApi( void ) throw();

  //! Open a MIDI output connection.
  /*!
      An optional port number greater than 0 can be specified.
      Otherwise, the default or first port found is opened.  An
      exception is thrown if an error occurs while attempting to make
      the port connection.
  */
  void openPort( unsigned int portNumber = 0, const std::string &portName = std::string( "RtMidi Output" ) );

  //! Close an open MIDI connection (if one exists).
  void closePort( void );

  //! Returns true if a port is open and false if not.
  /*!
      Note that this only applies to connections made with the openPort()
      function, not to virtual ports.
  */
  virtual bool isPortOpen() const;

  //! Create a virtual output port, with optional name, to allow software connections (OS X, JACK and ALSA only).
  /*!
      This function creates a virtual MIDI output port to which other
      software applications can connect.  This type of functionality
      is currently only supported by the Macintosh OS-X, Linux ALSA
      and JACK APIs (the function does nothing with the other APIs).
      An exception is thrown if an error occurs while attempting to
      create the virtual port.
  */
  void openVirtualPort( const std::string &portName = std::string( "RtMidi Output" ) );

  //! Return the number of available MIDI output ports.
  unsigned int getPortCount( void );

  //! Return a string identifier for the specified MIDI port type and number.
  /*!
    \return The name of the port with the given Id is returned.
    \retval An empty string is returned if an invalid port specifier
            is provided. User code should assume a UTF-8 encoding.
  */
  std::string getPortName( unsigned int portNumber = 0 );

  //! Immediately send a single message out an open MIDI output port.
  /*!
      An exception is thrown if an error occurs during output or an
      output connection was not previously established.
  */
  void sendMessage( const std::vector<unsigned char> *message );

  //! Immediately send a single message out an open MIDI output port.
  /*!
      An exception is thrown if an error occurs during output or an
      output connection was not previously established.

      \param message A pointer to the MIDI message as raw bytes
      \param size    Length of the MIDI message in bytes
  */
  void sendMessage( const unsigned char *message, size_t size );

  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is best
    to set the error callback function before opening a port.
  */
  virtual void setErrorCallback( RtMidiErrorCallback errorCallback = NULL, void *userData = 0 );

 protected:
  void openMidiApi( RtMidi::Api api, const std::string &clientName );
};


// **************************************************************** //
//
// MidiInApi / MidiOutApi class declarations.
//
// Subclasses of MidiInApi and MidiOutApi contain all API- and
// OS-specific code necessary to fully implement the RtMidi API.
//
// Note that MidiInApi and MidiOutApi are abstract base classes and
// cannot be explicitly instantiated.  RtMidiIn and RtMidiOut will
// create instances of a MidiInApi or MidiOutApi subclass.
//
// **************************************************************** //

class MidiApi
{
 public:

  MidiApi();
  virtual ~MidiApi();
  virtual RtMidi::Api getCurrentApi( void ) = 0;
  virtual void openPort( unsigned int portNumber, const std::string &portName ) = 0;
  virtual void openVirtualPort( const std::string &portName ) = 0;
  virtual void closePort( void ) = 0;

  virtual unsigned int getPortCount( void ) = 0;
  virtual std::string getPortName( unsigned int portNumber ) = 0;

  inline bool isPortOpen() const { return connected_; }
  void setErrorCallback( RtMidiErrorCallback errorCallback, void *userData );

  //! A basic error reporting function for RtMidi classes.
  void error( RtMidiError::Type type, std::string errorString );

protected:
  virtual void initialize( const std::string& clientName ) = 0;

  void *apiData_;
  bool connected_;
  std::string errorString_;
  RtMidiErrorCallback errorCallback_;
  bool firstErrorOccurred_;
  void *errorCallbackUserData_;
};

class MidiInApi : public MidiApi
{
 public:

  MidiInApi( unsigned int queueSizeLimit );
  virtual ~MidiInApi( void );
  void setCallback( RtMidiIn::RtMidiCallback callback, void *userData );
  void cancelCallback( void );
  virtual void ignoreTypes( bool midiSysex, bool midiTime, bool midiSense );
  double getMessage( std::vector<unsigned char> *message );

  // A MIDI structure used internally by the class to store incoming
  // messages.  Each message represents one and only one MIDI message.
  struct MidiMessage { 
    std::vector<unsigned char> bytes; 

    //! Time in seconds elapsed since the previous message
    double timeStamp;

    // Default constructor.
  MidiMessage()
  :bytes(0), timeStamp(0.0) {}
  };

  struct MidiQueue {
    unsigned int front;
    unsigned int back;
    unsigned int ringSize;
    MidiMessage *ring;

    // Default constructor.
  MidiQueue()
  :front(0), back(0), ringSize(0), ring(0) {}
    bool push(const MidiMessage&);
    bool pop(std::vector<unsigned char>*, double*);
    unsigned int size(unsigned int *back=0,
		      unsigned int *front=0);
  };

  // The RtMidiInData structure is used to pass private class data to
  // the MIDI input handling function or thread.
  struct RtMidiInData {
    MidiQueue queue;
    MidiMessage message;
    unsigned char ignoreFlags;
    bool doInput;
    bool firstMessage;
    void *apiData;
    bool usingCallback;
    RtMidiIn::RtMidiCallback userCallback;
    void *userData;
    bool continueSysex;

    // Default constructor.
  RtMidiInData()
  : ignoreFlags(7), doInput(false), firstMessage(true),
      apiData(0), usingCallback(false), userCallback(0), userData(0),
      continueSysex(false) {}
  };

 protected:
  RtMidiInData inputData_;
};

class MidiOutApi : public MidiApi
{
 public:

  MidiOutApi( void );
  virtual ~MidiOutApi( void );
  virtual void sendMessage( const unsigned char *message, size_t size ) = 0;
};

// **************************************************************** //
//
// Inline RtMidiIn and RtMidiOut definitions.
//
// **************************************************************** //

inline RtMidi::Api RtMidiIn :: getCurrentApi( void ) throw() { return rtapi_->getCurrentApi(); }
inline void RtMidiIn :: openPort( unsigned int portNumber, const std::string &portName ) { rtapi_->openPort( portNumber, portName ); }
inline void RtMidiIn :: openVirtualPort( const std::string &portName ) { rtapi_->openVirtualPort( portName ); }
inline void RtMidiIn :: closePort( void ) { rtapi_->closePort(); }
inline bool RtMidiIn :: isPortOpen() const { return rtapi_->isPortOpen(); }
inline void RtMidiIn :: setCallback( RtMidiCallback callback, void *userData ) { ((MidiInApi *)rtapi_)->setCallback( callback, userData ); }
inline void RtMidiIn :: cancelCallback( void ) { ((MidiInApi *)rtapi_)->cancelCallback(); }
inline unsigned int RtMidiIn :: getPortCount( void ) { return rtapi_->getPortCount(); }
inline std::string RtMidiIn :: getPortName( unsigned int portNumber ) { return rtapi_->getPortName( portNumber ); }
inline void RtMidiIn :: ignoreTypes( bool midiSysex, bool midiTime, bool midiSense ) { ((MidiInApi *)rtapi_)->ignoreTypes( midiSysex, midiTime, midiSense ); }
inline double RtMidiIn :: getMessage( std::vector<unsigned char> *message ) { return ((MidiInApi *)rtapi_)->getMessage( message ); }
inline void RtMidiIn :: setErrorCallback( RtMidiErrorCallback errorCallback, void *userData ) { rtapi_->setErrorCallback(errorCallback, userData); }

inline RtMidi::Api RtMidiOut :: getCurrentApi( void ) throw() { return rtapi_->getCurrentApi(); }
inline void RtMidiOut :: openPort( unsigned int portNumber, const std::string &portName ) { rtapi_->openPort( portNumber, portName ); }
inline void RtMidiOut :: openVirtualPort( const std::string &portName ) { rtapi_->openVirtualPort( portName ); }
inline void RtMidiOut :: closePort( void ) { rtapi_->closePort(); }
inline bool RtMidiOut :: isPortOpen() const { return rtapi_->isPortOpen(); }
inline unsigned int RtMidiOut :: getPortCount( void ) { return rtapi_->getPortCount(); }
inline std::string RtMidiOut :: getPortName( unsigned int portNumber ) { return rtapi_->getPortName( portNumber ); }
inline void RtMidiOut :: sendMessage( const std::vector<unsigned char> *message ) { ((MidiOutApi *)rtapi_)->sendMessage( &message->at(0), message->size() ); }
inline void RtMidiOut :: sendMessage( const unsigned char *message, size_t size ) { ((MidiOutApi *)rtapi_)->sendMessage( message, size ); }
inline void RtMidiOut :: setErrorCallback( RtMidiErrorCallback errorCallback, void *userData ) { rtapi_->setErrorCallback(errorCallback, userData); }

// **************************************************************** //
//
// MidiInApi and MidiOutApi subclass prototypes.
//
// **************************************************************** //

#if !defined(__LINUX_ALSA__) && !defined(__UNIX_JACK__) && !defined(__MACOSX_CORE__) && !defined(__WINDOWS_MM__)
  #define __RTMIDI_DUMMY__
#endif

#if defined(__MACOSX_CORE__)

class MidiInCore: public MidiInApi
{
 public:
  MidiInCore( const std::string &clientName, unsigned int queueSizeLimit );
  ~MidiInCore( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::MACOSX_CORE; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );

 protected:
  void initialize( const std::string& clientName );
};

class MidiOutCore: public MidiOutApi
{
 public:
  MidiOutCore( const std::string &clientName );
  ~MidiOutCore( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::MACOSX_CORE; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );
  void sendMessage( const unsigned char *message, size_t size );

 protected:
  void initialize( const std::string& clientName );
};

#endif

#if defined(__UNIX_JACK__)

class MidiInJack: public MidiInApi
{
 public:
  MidiInJack( const std::string &clientName, unsigned int queueSizeLimit );
  ~MidiInJack( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::UNIX_JACK; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );

 protected:
  std::string clientName;

  void connect( void );
  void initialize( const std::string& clientName );
};

class MidiOutJack: public MidiOutApi
{
 public:
  MidiOutJack( const std::string &clientName );
  ~MidiOutJack( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::UNIX_JACK; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );
  void sendMessage( const unsigned char *message, size_t size );

 protected:
  std::string clientName;

  void connect( void );
  void initialize( const std::string& clientName );
};

#endif

#if defined(__LINUX_ALSA__)

class MidiInAlsa: public MidiInApi
{
 public:
  MidiInAlsa( const std::string &clientName, unsigned int queueSizeLimit );
  ~MidiInAlsa( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::LINUX_ALSA; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );

 protected:
  void initialize( const std::string& clientName );
};

class MidiOutAlsa: public MidiOutApi
{
 public:
  MidiOutAlsa( const std::string &clientName );
  ~MidiOutAlsa( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::LINUX_ALSA; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );
  void sendMessage( const unsigned char *message, size_t size );

 protected:
  void initialize( const std::string& clientName );
};

#endif

#if defined(__WINDOWS_MM__)

class MidiInWinMM: public MidiInApi
{
 public:
  MidiInWinMM( const std::string &clientName, unsigned int queueSizeLimit );
  ~MidiInWinMM( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::WINDOWS_MM; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );

 protected:
  void initialize( const std::string& clientName );
};

class MidiOutWinMM: public MidiOutApi
{
 public:
  MidiOutWinMM( const std::string &clientName );
  ~MidiOutWinMM( void );
  RtMidi::Api getCurrentApi( void ) { return RtMidi::WINDOWS_MM; };
  void openPort( unsigned int portNumber, const std::string &portName );
  void openVirtualPort( const std::string &portName );
  void closePort( void );
  unsigned int getPortCount( void );
  std::string getPortName( unsigned int portNumber );
  void sendMessage( const unsigned char *message, size_t size );

 protected:
  void initialize( const std::string& clientName );
};

#endif

#if defined(__RTMIDI_DUMMY__)

class MidiInDummy: public MidiInApi
{
 public:
 MidiInDummy( const std::string &/*clientName*/, unsigned int queueSizeLimit ) : MidiInApi( queueSizeLimit ) { errorString_ = "MidiInDummy: This class provides no functionality."; error( RtMidiError::WARNING, errorString_ ); }
  RtMidi::Api getCurrentApi( void ) { return RtMidi::RTMIDI_DUMMY; }
  void openPort( unsigned int /*portNumber*/, const std::string &/*portName*/ ) {}
  void openVirtualPort( const std::string &/*portName*/ ) {}
  void closePort( void ) {}
  unsigned int getPortCount( void ) { return 0; }
  std::string getPortName( unsigned int /*portNumber*/ ) { return ""; }

 protected:
  void initialize( const std::string& /*clientName*/ ) {}
};

class MidiOutDummy: public MidiOutApi
{
 public:
  MidiOutDummy( const std::string &/*clientName*/ ) { errorString_ = "MidiOutDummy: This class provides no functionality."; error( RtMidiError::WARNING, errorString_ ); }
  RtMidi::Api getCurrentApi( void ) { return RtMidi::RTMIDI_DUMMY; }
  void openPort( unsigned int /*portNumber*/, const std::string &/*portName*/ ) {}
  void openVirtualPort( const std::string &/*portName*/ ) {}
  void closePort( void ) {}
  unsigned int getPortCount( void ) { return 0; }
  std::string getPortName( unsigned int /*portNumber*/ ) { return ""; }
  void sendMessage( const unsigned char * /*message*/, size_t /*size*/ ) {}

 protected:
  void initialize( const std::string& /*clientName*/ ) {}
};

#endif

#endif
/**************************  END  RtMidi.h **************************/

class MapUI;

//-------------------------------------------------
// MIDI input/output handling using RtMidi library
//-------------------------------------------------

class rt_midi : public midi_handler {

    private:
    
        std::vector<RtMidiIn*> fInput;
        std::vector<RtMidiOut*> fOutput;
        bool fIsVirtual;
        bool fPolling;
    
        static void midiCallback(double time, std::vector<unsigned char>* message, void* arg)
        {
            rt_midi* midi = static_cast<rt_midi*>(arg);
            size_t nBytes = message->size();
            int type = (int)message->at(0) & 0xf0;
            int channel = (int)message->at(0) & 0x0f;
            
            // MIDI sync
            if (nBytes == 1) {
                midi->handleSync(time, (int)message->at(0));
            // One data byte messages
            } else if (nBytes == 2) {
                midi->handleData1(time, type, channel, (int)message->at(1));
            // Two data bytes messages
            } else if (nBytes == 3) {
                midi->handleData2(time, type, channel, (int)message->at(1), (int)message->at(2));
            } else {
                midi->handleMessage(time, (int)message->at(0), *message);
            }
        }
        
        bool openMidiInputPorts()
        {
            // Get number of input ports
            RtMidiIn midi_in;
            unsigned nInPorts = midi_in.getPortCount();
            if (nInPorts == 0) {
                std::cout << "No input ports available!" << std::endl;
                return false;
            }
    
            // Then open all of them
            for (unsigned int i = 0; i < nInPorts; i++) {
                RtMidiIn* midi_in = new RtMidiIn();
                midi_in->ignoreTypes(true, false, true);
                fInput.push_back(midi_in);
                midi_in->openPort(i);
                if (!fPolling) {
                    midi_in->setCallback(&midiCallback, this);
                }
                //std::cout << "Input port #" << i << ": " << midi_in->getPortName(i) << '\n';
            }
            return true;
        }
        
        bool openMidiOutputPorts()
        {
            // Get number of output ports
            RtMidiOut midi_out;
            unsigned nOutPorts = midi_out.getPortCount();
            if (nOutPorts == 0) {
                std::cout << "No output ports available!" << std::endl;
                return false;
            }
    
            // Then open all of them
            for (unsigned int i = 0; i < nOutPorts; i++) {
                RtMidiOut* midi_out = new RtMidiOut();
                fOutput.push_back(midi_out);
                midi_out->openPort(i);
                //std::cout << "Output port #" << i << ": " << midi_out->getPortName(i) << '\n';
            }
            return true;
        }
        
        void chooseMidiInputPort(const std::string& name)
        {
            RtMidiIn* midi_in = new RtMidiIn();
            midi_in->ignoreTypes(true, false, true);
            fInput.push_back(midi_in);
            if (!fPolling) {
                midi_in->setCallback(&midiCallback, this);
            }
            midi_in->openVirtualPort(name);
        }
        
        void chooseMidiOutPort(const std::string& name)
        {
            RtMidiOut* midi_out = new RtMidiOut();
            fOutput.push_back(midi_out);
            midi_out->openVirtualPort(name);
        }
        
        void sendMessage(std::vector<unsigned char>& message)
        {
            for (auto& it : fOutput) {
                it->sendMessage(&message);
            }
        }
    
    public:
    
        rt_midi(const std::string& name = "RtMidi",
                bool is_virtual = false,
                bool is_polling = false)
        :midi_handler(name), fIsVirtual(is_virtual), fPolling(is_polling)
        {}
        
        virtual ~rt_midi()
        {
            stopMidi();
        }
        
        bool startMidi()
        {
            try {
            
            #if TARGET_OS_IPHONE
                if (!openMidiInputPorts())  { stopMidi(); return false; }
                if (!openMidiOutputPorts()) { stopMidi(); return false; }
            #else
                if (fIsVirtual) {
                    chooseMidiInputPort(fName);
                    chooseMidiOutPort(fName);
                } else {
                    if (!openMidiInputPorts())  { stopMidi(); return false; }
                    std::cerr << "Warning : MIDI outputs are not started in this mode !\n";
                }
            #endif
                return true;
                
            } catch (RtMidiError &error) {
                error.printMessage();
                stopMidi();
                return false;
            }
        }
        
        void stopMidi()
        {
            for (auto& it1 : fInput) {
                delete it1;
            }
            fInput.clear();
            for (auto& it2 : fOutput) {
                delete it2;
            }
            fOutput.clear();
        }
    
        // To be used in polling mode
        int recvMessages(std::vector<MIDIMessage>* messages)
        {
            int count = 0;
            double first_time_stamp = 0.;
            for (auto& it : fInput) {
                std::vector<unsigned char> message;
                double time_stamp = (uint32_t)it->getMessage(&message);
                // Small messages
                if (message.size() > 0) {
                    if (count == 0) first_time_stamp = time_stamp;
                    MIDIMessage& mes = messages->at(count++);
                    mes.frameIndex = (uint32_t)(time_stamp - first_time_stamp);
                    mes.byte0 = message[0];
                    mes.byte1 = message[1];
                    mes.byte2 = message[2];
                }
            }
            return count;
        }
    
        void sendMessages(std::vector<MIDIMessage>* messages, int count)
        {
            for (int i = 0; i < count; ++i) {
                MIDIMessage mes1 = (*messages)[i];
                std::vector<unsigned char> mes2;
                mes2.push_back(mes1.byte0);
                mes2.push_back(mes1.byte1);
                mes2.push_back(mes1.byte2);
                sendMessage(mes2);
            }
        }
    
        // MIDI output API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_NOTE_ON + channel);
            message.push_back(pitch);
            message.push_back(velocity);
            sendMessage(message);
            return 0;
        }
        
        void keyOff(int channel, int pitch, int velocity) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_NOTE_OFF + channel);
            message.push_back(pitch);
            message.push_back(velocity);
            sendMessage(message);
        }
        
        void ctrlChange(int channel, int ctrl, int val) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_CONTROL_CHANGE + channel);
            message.push_back(ctrl);
            message.push_back(val);
            sendMessage(message);
        }
        
        void chanPress(int channel, int press) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_AFTERTOUCH + channel);
            message.push_back(press);
            sendMessage(message);
        }
        
        void progChange(int channel, int pgm) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_PROGRAM_CHANGE + channel);
            message.push_back(pgm);
            sendMessage(message);
        }
          
        void keyPress(int channel, int pitch, int press) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_POLY_AFTERTOUCH + channel);
            message.push_back(pitch);
            message.push_back(press);
            sendMessage(message);
        }
   
        void pitchWheel(int channel, int wheel) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_PITCH_BEND + channel);
            message.push_back(wheel & 0x7F);		// lsb 7bit
            message.push_back((wheel >> 7) & 0x7F);	// msb 7bit
            sendMessage(message);
        }
        
        void ctrlChange14bits(int channel, int ctrl, int value) {}
         
        void startSync(double date)
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_START);
            sendMessage(message);
        }
       
        void stopSync(double date)
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_STOP);
            sendMessage(message);
        }
        
        void clock(double date) 
        {
            std::vector<unsigned char> message;
            message.push_back(MIDI_CLOCK);
            sendMessage(message);
        }
    
        void sysEx(double, std::vector<unsigned char>& message)
        {
            sendMessage(message);
        }
    
};


#endif // __rt_midi__
/**************************  END  rt-midi.h **************************/
/**********************************************************************/
/*! \class RtMidi
    \brief An abstract base class for realtime MIDI input/output.

    This class implements some common functionality for the realtime
    MIDI input/output subclasses RtMidiIn and RtMidiOut.

    RtMidi WWW site: http://music.mcgill.ca/~gary/rtmidi/

    RtMidi: realtime MIDI i/o C++ classes
    Copyright (c) 2003-2017 Gary P. Scavone

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    Any person wishing to distribute modifications to the Software is
    asked to send the modifications to the original developer so that
    they can be incorporated into the canonical version.  This is,
    however, not a binding provision of this license.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/**********************************************************************/

#include <sstream>

#ifdef __APPLE__

Float64 CAHostTimeBase::sFrequency = 0;
Float64 CAHostTimeBase::sInverseFrequency = 0;
UInt32 CAHostTimeBase::sMinDelta = 0;
UInt32 CAHostTimeBase::sToNanosNumerator = 0;
UInt32 CAHostTimeBase::sToNanosDenominator = 0;
UInt32 CAHostTimeBase::sFromNanosNumerator = 0;
UInt32 CAHostTimeBase::sFromNanosDenominator = 0;
bool CAHostTimeBase::sUseMicroseconds = false;
bool CAHostTimeBase::sIsInited = false;
#if Track_Host_TimeBase
UInt64 CAHostTimeBase::sLastTime = 0;
#endif

//=============================================================================
//	CAHostTimeBase
//
//	This class provides platform independent access to the host's time base.
//=============================================================================

void CAHostTimeBase::Initialize()
{
    if (!sIsInited) {
        
        //	get the info about Absolute time
#if TARGET_OS_MAC
        struct mach_timebase_info	theTimeBaseInfo;
        mach_timebase_info(&theTimeBaseInfo);
        sMinDelta = 1;
        sToNanosNumerator = theTimeBaseInfo.numer;
        sToNanosDenominator = theTimeBaseInfo.denom;
        sFromNanosNumerator = sToNanosDenominator;
        sFromNanosDenominator = sToNanosNumerator;
        
        //	the frequency of that clock is: (sToNanosDenominator / sToNanosNumerator) * 10^9
        sFrequency = static_cast<Float64>(sToNanosDenominator) / static_cast<Float64>(sToNanosNumerator);
        sFrequency *= 1000000000.0;
#elif TARGET_OS_WIN32
        LARGE_INTEGER theFrequency;
        QueryPerformanceFrequency(&theFrequency);
        sMinDelta = 1;
        sToNanosNumerator = 1000000000ULL;
        sToNanosDenominator = *((UInt64*)&theFrequency);
        sFromNanosNumerator = sToNanosDenominator;
        sFromNanosDenominator = sToNanosNumerator;
        sFrequency = static_cast<Float64>(*((UInt64*)&theFrequency));
#endif
        sInverseFrequency = 1.0 / sFrequency;
        
#if	Log_Host_Time_Base_Parameters
        DebugMessage(  "Host Time Base Parameters");
        DebugMessageN1(" Minimum Delta:          %lu", sMinDelta);
        DebugMessageN1(" Frequency:              %f", sFrequency);
        DebugMessageN1(" To Nanos Numerator:     %lu", sToNanosNumerator);
        DebugMessageN1(" To Nanos Denominator:   %lu", sToNanosDenominator);
        DebugMessageN1(" From Nanos Numerator:   %lu", sFromNanosNumerator);
        DebugMessageN1(" From Nanos Denominator: %lu", sFromNanosDenominator);
#endif
        
        sIsInited = true;
    }
}

#endif

#if defined(__MACOSX_CORE__)
  #if TARGET_OS_IPHONE
    #define AudioGetCurrentHostTime CAHostTimeBase::GetCurrentTime
    #define AudioConvertHostTimeToNanos CAHostTimeBase::ConvertToNanos
  #endif
#endif

// Default for Windows is to add an identifier to the port names; this
// flag can be undefined to disable this behaviour.
#define RTMIDI_ENSURE_UNIQUE_PORTNAMES

//*********************************************************************//
//  RtMidi Definitions
//*********************************************************************//

RtMidi :: RtMidi()
  : rtapi_(0)
{
#if defined(__MACOSX_CORE__)
    #if TARGET_OS_IPHONE
        CAHostTimeBase::Initialize();
    #endif
#endif
}

RtMidi :: ~RtMidi()
{
  delete rtapi_;
  rtapi_ = 0;
}

std::string RtMidi :: getVersion( void ) throw()
{
  return std::string( RTMIDI_VERSION );
}

void RtMidi :: getCompiledApi( std::vector<RtMidi::Api> &apis ) throw()
{
  apis.clear();

  // The order here will control the order of RtMidi's API search in
  // the constructor.
#if defined(__MACOSX_CORE__)
  apis.push_back( MACOSX_CORE );
#endif
#if defined(__LINUX_ALSA__)
  apis.push_back( LINUX_ALSA );
#endif
#if defined(__UNIX_JACK__)
  apis.push_back( UNIX_JACK );
#endif
#if defined(__WINDOWS_MM__)
  apis.push_back( WINDOWS_MM );
#endif
#if defined(__RTMIDI_DUMMY__)
  apis.push_back( RTMIDI_DUMMY );
#endif
}

//*********************************************************************//
//  RtMidiIn Definitions
//*********************************************************************//

void RtMidiIn :: openMidiApi( RtMidi::Api api, const std::string &clientName, unsigned int queueSizeLimit )
{
  delete rtapi_;
  rtapi_ = 0;

#if defined(__UNIX_JACK__)
  if ( api == UNIX_JACK )
    rtapi_ = new MidiInJack( clientName, queueSizeLimit );
#endif
#if defined(__LINUX_ALSA__)
  if ( api == LINUX_ALSA )
    rtapi_ = new MidiInAlsa( clientName, queueSizeLimit );
#endif
#if defined(__WINDOWS_MM__)
  if ( api == WINDOWS_MM )
    rtapi_ = new MidiInWinMM( clientName, queueSizeLimit );
#endif
#if defined(__MACOSX_CORE__)
  if ( api == MACOSX_CORE )
    rtapi_ = new MidiInCore( clientName, queueSizeLimit );
#endif
#if defined(__RTMIDI_DUMMY__)
  if ( api == RTMIDI_DUMMY )
    rtapi_ = new MidiInDummy( clientName, queueSizeLimit );
#endif
}

RtMidiIn :: RtMidiIn( RtMidi::Api api, const std::string &clientName, unsigned int queueSizeLimit )
  : RtMidi()
{
  if ( api != UNSPECIFIED ) {
    // Attempt to open the specified API.
    openMidiApi( api, clientName, queueSizeLimit );
    if ( rtapi_ ) return;

    // No compiled support for specified API value.  Issue a warning
    // and continue as if no API was specified.
    std::cerr << "\nRtMidiIn: no compiled support for specified API argument!\n\n" << std::endl;
  }

  // Iterate through the compiled APIs and return as soon as we find
  // one with at least one port or we reach the end of the list.
  std::vector< RtMidi::Api > apis;
  getCompiledApi( apis );
  for ( unsigned int i=0; i<apis.size(); i++ ) {
    openMidiApi( apis[i], clientName, queueSizeLimit );
    if ( rtapi_ && rtapi_->getPortCount() ) break;
  }

  if ( rtapi_ ) return;

  // It should not be possible to get here because the preprocessor
  // definition __RTMIDI_DUMMY__ is automatically defined if no
  // API-specific definitions are passed to the compiler. But just in
  // case something weird happens, we'll throw an error.
  std::string errorText = "RtMidiIn: no compiled API support found ... critical error!!";
  throw( RtMidiError( errorText, RtMidiError::UNSPECIFIED ) );
}

RtMidiIn :: ~RtMidiIn() throw()
{
}


//*********************************************************************//
//  RtMidiOut Definitions
//*********************************************************************//

void RtMidiOut :: openMidiApi( RtMidi::Api api, const std::string &clientName )
{
  delete rtapi_;
  rtapi_ = 0;

#if defined(__UNIX_JACK__)
  if ( api == UNIX_JACK )
    rtapi_ = new MidiOutJack( clientName );
#endif
#if defined(__LINUX_ALSA__)
  if ( api == LINUX_ALSA )
    rtapi_ = new MidiOutAlsa( clientName );
#endif
#if defined(__WINDOWS_MM__)
  if ( api == WINDOWS_MM )
    rtapi_ = new MidiOutWinMM( clientName );
#endif
#if defined(__MACOSX_CORE__)
  if ( api == MACOSX_CORE )
    rtapi_ = new MidiOutCore( clientName );
#endif
#if defined(__RTMIDI_DUMMY__)
  if ( api == RTMIDI_DUMMY )
    rtapi_ = new MidiOutDummy( clientName );
#endif
}

RtMidiOut :: RtMidiOut( RtMidi::Api api, const std::string &clientName)
{
  if ( api != UNSPECIFIED ) {
    // Attempt to open the specified API.
    openMidiApi( api, clientName );
    if ( rtapi_ ) return;

    // No compiled support for specified API value.  Issue a warning
    // and continue as if no API was specified.
    std::cerr << "\nRtMidiOut: no compiled support for specified API argument!\n\n" << std::endl;
  }

  // Iterate through the compiled APIs and return as soon as we find
  // one with at least one port or we reach the end of the list.
  std::vector< RtMidi::Api > apis;
  getCompiledApi( apis );
  for ( unsigned int i=0; i<apis.size(); i++ ) {
    openMidiApi( apis[i], clientName );
    if ( rtapi_ && rtapi_->getPortCount() ) break;
  }

  if ( rtapi_ ) return;

  // It should not be possible to get here because the preprocessor
  // definition __RTMIDI_DUMMY__ is automatically defined if no
  // API-specific definitions are passed to the compiler. But just in
  // case something weird happens, we'll thrown an error.
  std::string errorText = "RtMidiOut: no compiled API support found ... critical error!!";
  throw( RtMidiError( errorText, RtMidiError::UNSPECIFIED ) );
}

RtMidiOut :: ~RtMidiOut() throw()
{
}

//*********************************************************************//
//  Common MidiApi Definitions
//*********************************************************************//

MidiApi :: MidiApi( void )
  : apiData_( 0 ), connected_( false ), errorCallback_(0), firstErrorOccurred_(false), errorCallbackUserData_(0)
{
}

MidiApi :: ~MidiApi( void )
{
}

void MidiApi :: setErrorCallback( RtMidiErrorCallback errorCallback, void *userData = 0 )
{
    errorCallback_ = errorCallback;
    errorCallbackUserData_ = userData;
}

void MidiApi :: error( RtMidiError::Type type, std::string errorString )
{
  if ( errorCallback_ ) {

    if ( firstErrorOccurred_ )
      return;

    firstErrorOccurred_ = true;
    const std::string errorMessage = errorString;

    errorCallback_( type, errorMessage, errorCallbackUserData_);
    firstErrorOccurred_ = false;
    return;
  }

  if ( type == RtMidiError::WARNING ) {
    std::cerr << '\n' << errorString << "\n\n";
  }
  else if ( type == RtMidiError::DEBUG_WARNING ) {
#if defined(__RTMIDI_DEBUG__)
    std::cerr << '\n' << errorString << "\n\n";
#endif
  }
  else {
    std::cerr << '\n' << errorString << "\n\n";
    throw RtMidiError( errorString, type );
  }
}

//*********************************************************************//
//  Common MidiInApi Definitions
//*********************************************************************//

MidiInApi :: MidiInApi( unsigned int queueSizeLimit )
  : MidiApi()
{
  // Allocate the MIDI queue.
  inputData_.queue.ringSize = queueSizeLimit;
  if ( inputData_.queue.ringSize > 0 )
    inputData_.queue.ring = new MidiMessage[ inputData_.queue.ringSize ];
}

MidiInApi :: ~MidiInApi( void )
{
  // Delete the MIDI queue.
  if ( inputData_.queue.ringSize > 0 ) delete [] inputData_.queue.ring;
}

void MidiInApi :: setCallback( RtMidiIn::RtMidiCallback callback, void *userData )
{
  if ( inputData_.usingCallback ) {
    errorString_ = "MidiInApi::setCallback: a callback function is already set!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  if ( !callback ) {
    errorString_ = "RtMidiIn::setCallback: callback function value is invalid!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  inputData_.userCallback = callback;
  inputData_.userData = userData;
  inputData_.usingCallback = true;
}

void MidiInApi :: cancelCallback()
{
  if ( !inputData_.usingCallback ) {
    errorString_ = "RtMidiIn::cancelCallback: no callback function was set!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  inputData_.userCallback = 0;
  inputData_.userData = 0;
  inputData_.usingCallback = false;
}

void MidiInApi :: ignoreTypes( bool midiSysex, bool midiTime, bool midiSense )
{
  inputData_.ignoreFlags = 0;
  if ( midiSysex ) inputData_.ignoreFlags = 0x01;
  if ( midiTime ) inputData_.ignoreFlags |= 0x02;
  if ( midiSense ) inputData_.ignoreFlags |= 0x04;
}

double MidiInApi :: getMessage( std::vector<unsigned char> *message )
{
  message->clear();

  if ( inputData_.usingCallback ) {
    errorString_ = "RtMidiIn::getNextMessage: a user callback is currently set for this port.";
    error( RtMidiError::WARNING, errorString_ );
    return 0.0;
  }

  double timeStamp;
  if (!inputData_.queue.pop(message, &timeStamp))
    return 0.0;

  return timeStamp;
}

unsigned int MidiInApi::MidiQueue::size(unsigned int *__back,
					unsigned int *__front)
{
  // Access back/front members exactly once and make stack copies for
  // size calculation
  unsigned int _back = back, _front = front, _size;
  if (_back >= _front)
    _size = _back - _front;
  else
    _size = ringSize - _front + _back;

  // Return copies of back/front so no new and unsynchronized accesses
  // to member variables are needed.
  if (__back) *__back = _back;
  if (__front) *__front = _front;
  return _size;
}

// As long as we haven't reached our queue size limit, push the message.
bool MidiInApi::MidiQueue::push(const MidiInApi::MidiMessage& msg)
{
  // Local stack copies of front/back
  unsigned int _back, _front, _size;

  // Get back/front indexes exactly once and calculate current size
  _size = size(&_back, &_front);

  if ( _size < ringSize-1 )
  {
    ring[_back] = msg;
    back = (back+1)%ringSize;
    return true;
  }

  return false;
}

bool MidiInApi::MidiQueue::pop(std::vector<unsigned char> *msg, double* timeStamp)
{
  // Local stack copies of front/back
  unsigned int _back, _front, _size;

  // Get back/front indexes exactly once and calculate current size
  _size = size(&_back, &_front);

  if (_size == 0)
    return false;

  // Copy queued message to the vector pointer argument and then "pop" it.
  msg->assign( ring[_front].bytes.begin(), ring[_front].bytes.end() );
  *timeStamp = ring[_front].timeStamp;

  // Update front
  front = (front+1)%ringSize;
  return true;
}

//*********************************************************************//
//  Common MidiOutApi Definitions
//*********************************************************************//

MidiOutApi :: MidiOutApi( void )
  : MidiApi()
{
}

MidiOutApi :: ~MidiOutApi( void )
{
}

// *************************************************** //
//
// OS/API-specific methods.
//
// *************************************************** //

#if defined(__MACOSX_CORE__)

// The CoreMIDI API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// The CoreMIDI API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// OS-X CoreMIDI header files.
#include <CoreMIDI/CoreMIDI.h>

#if defined(__MACOSX_CORE__)
    #if TARGET_OS_IPHONE
    static inline UInt32 EndianS32_BtoN(UInt32 x)
        {
            return ((x << 24) & 0xFF000000) | ((x << 8) & 0x00FF0000)
            | ((x >> 8) & 0x0000FF00) | ((x >> 24) & 0x000000FF);
        }
    #else
        #include <CoreAudio/HostTime.h>
        #include <CoreServices/CoreServices.h>
    #endif
#endif

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct CoreMidiData {
  MIDIClientRef client;
  MIDIPortRef port;
  MIDIEndpointRef endpoint;
  MIDIEndpointRef destinationId;
  unsigned long long lastTime;
  MIDISysexSendRequest sysexreq;
};

//*********************************************************************//
//  API: OS-X
//  Class Definitions: MidiInCore
//*********************************************************************//

static void midiInputCallback( const MIDIPacketList *list, void *procRef, void */*srcRef*/ )
{
  MidiInApi::RtMidiInData *data = static_cast<MidiInApi::RtMidiInData *> (procRef);
  CoreMidiData *apiData = static_cast<CoreMidiData *> (data->apiData);

  unsigned char status;
  unsigned short nBytes, iByte, size;
  unsigned long long time;

  bool& continueSysex = data->continueSysex;
  MidiInApi::MidiMessage& message = data->message;

  const MIDIPacket *packet = &list->packet[0];
  for ( unsigned int i=0; i<list->numPackets; ++i ) {

    // My interpretation of the CoreMIDI documentation: all message
    // types, except sysex, are complete within a packet and there may
    // be several of them in a single packet.  Sysex messages can be
    // broken across multiple packets and PacketLists but are bundled
    // alone within each packet (these packets do not contain other
    // message types).  If sysex messages are split across multiple
    // MIDIPacketLists, they must be handled by multiple calls to this
    // function.

    nBytes = packet->length;
    if ( nBytes == 0 ) continue;

    // Calculate time stamp.

    /*
    if ( data->firstMessage ) {
      message.timeStamp = 0.0;
      data->firstMessage = false;
    }
    else {
      time = packet->timeStamp;
      if ( time == 0 ) { // this happens when receiving asynchronous sysex messages
        time = AudioGetCurrentHostTime();
      }
      time -= apiData->lastTime;
      time = AudioConvertHostTimeToNanos( time );
      if ( !continueSysex )
        message.timeStamp = time * 0.000000001;
    }
    apiData->lastTime = packet->timeStamp;
    if ( apiData->lastTime == 0 ) { // this happens when receiving asynchronous sysex messages
      apiData->lastTime = AudioGetCurrentHostTime();
    }
    //std::cout << "TimeStamp = " << packet->timeStamp << std::endl;
    */
      
    // 10/02/19: absolute usec based time stamp, possibly using current time if packet timestamp seems wrong...
    double packet_usec = AudioConvertHostTimeToNanos(packet->timeStamp) / 1000;
    double cur_date_usec = double(AudioConvertHostTimeToNanos(AudioGetCurrentHostTime())) / 1000.;
    double delta_usec = cur_date_usec - packet_usec;
    message.timeStamp = (delta_usec > 1000) ? cur_date_usec : packet_usec;
  
    iByte = 0;
    if ( continueSysex ) {
      // We have a continuing, segmented sysex message.
      if ( !( data->ignoreFlags & 0x01 ) ) {
        // If we're not ignoring sysex messages, copy the entire packet.
        for ( unsigned int j=0; j<nBytes; ++j )
          message.bytes.push_back( packet->data[j] );
      }
      continueSysex = packet->data[nBytes-1] != 0xF7;

      if ( !( data->ignoreFlags & 0x01 ) && !continueSysex ) {
        // If not a continuing sysex message, invoke the user callback function or queue the message.
        if ( data->usingCallback ) {
          RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
          callback( message.timeStamp, &message.bytes, data->userData );
        }
        else {
          // As long as we haven't reached our queue size limit, push the message.
          if (!data->queue.push(message))
            std::cerr << "\nMidiInCore: message queue limit reached!!\n\n";
        }
        message.bytes.clear();
      }
    }
    else {
      while ( iByte < nBytes ) {
        size = 0;
        // We are expecting that the next byte in the packet is a status byte.
        status = packet->data[iByte];
        if ( !(status & 0x80) ) break;
        // Determine the number of bytes in the MIDI message.
        if ( status < 0xC0 ) size = 3;
        else if ( status < 0xE0 ) size = 2;
        else if ( status < 0xF0 ) size = 3;
        else if ( status == 0xF0 ) {
          // A MIDI sysex
          if ( data->ignoreFlags & 0x01 ) {
            size = 0;
            iByte = nBytes;
          }
          else size = nBytes - iByte;
          continueSysex = packet->data[nBytes-1] != 0xF7;
        }
        else if ( status == 0xF1 ) {
            // A MIDI time code message
           if ( data->ignoreFlags & 0x02 ) {
            size = 0;
            iByte += 2;
           }
           else size = 2;
        }
        else if ( status == 0xF2 ) size = 3;
        else if ( status == 0xF3 ) size = 2;
        else if ( status == 0xF8 && ( data->ignoreFlags & 0x02 ) ) {
          // A MIDI timing tick message and we're ignoring it.
          size = 0;
          iByte += 1;
        }
        else if ( status == 0xFE && ( data->ignoreFlags & 0x04 ) ) {
          // A MIDI active sensing message and we're ignoring it.
          size = 0;
          iByte += 1;
        }
        else size = 1;

        // Copy the MIDI data to our vector.
        if ( size ) {
          message.bytes.assign( &packet->data[iByte], &packet->data[iByte+size] );
          if ( !continueSysex ) {
            // If not a continuing sysex message, invoke the user callback function or queue the message.
            if ( data->usingCallback ) {
              RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
              callback( message.timeStamp, &message.bytes, data->userData );
            }
            else {
              // As long as we haven't reached our queue size limit, push the message.
              if (!data->queue.push(message))
                std::cerr << "\nMidiInCore: message queue limit reached!!\n\n";
            }
            message.bytes.clear();
          }
          iByte += size;
        }
      }
    }
    packet = MIDIPacketNext(packet);
  }
}

MidiInCore :: MidiInCore( const std::string &clientName, unsigned int queueSizeLimit ) : MidiInApi( queueSizeLimit )
{
  initialize( clientName );
}

MidiInCore :: ~MidiInCore( void )
{
  // Close a connection if it exists.
  closePort();

  // Cleanup.
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
  MIDIClientDispose( data->client );
  if ( data->endpoint ) MIDIEndpointDispose( data->endpoint );
  delete data;
}

void MidiInCore :: initialize( const std::string& clientName )
{
  // Set up our client.
  MIDIClientRef client;
  CFStringRef name = CFStringCreateWithCString( NULL, clientName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDIClientCreate(name, NULL, NULL, &client );
  if ( result != noErr ) {
    std::ostringstream ost;
    ost << "MidiInCore::initialize: error creating OS-X MIDI client object (" << result << ").";
    errorString_ = ost.str();
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  CoreMidiData *data = (CoreMidiData *) new CoreMidiData;
  data->client = client;
  data->endpoint = 0;
  apiData_ = (void *) data;
  inputData_.apiData = (void *) data;
  CFRelease(name);
}

void MidiInCore :: openPort( unsigned int portNumber, const std::string &portName )
{
  if ( connected_ ) {
    errorString_ = "MidiInCore::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  unsigned int nSrc = MIDIGetNumberOfSources();
  if (nSrc < 1) {
    errorString_ = "MidiInCore::openPort: no MIDI input sources found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

  if ( portNumber >= nSrc ) {
    std::ostringstream ost;
    ost << "MidiInCore::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  MIDIPortRef port;
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
  OSStatus result = MIDIInputPortCreate( data->client, 
                                         CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII ),
                                         midiInputCallback, (void *)&inputData_, &port );
  if ( result != noErr ) {
    MIDIClientDispose( data->client );
    errorString_ = "MidiInCore::openPort: error creating OS-X MIDI input port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Get the desired input source identifier.
  MIDIEndpointRef endpoint = MIDIGetSource( portNumber );
  if ( endpoint == 0 ) {
    MIDIPortDispose( port );
    MIDIClientDispose( data->client );
    errorString_ = "MidiInCore::openPort: error getting MIDI input source reference.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Make the connection.
  result = MIDIPortConnectSource( port, endpoint, NULL );
  if ( result != noErr ) {
    MIDIPortDispose( port );
    MIDIClientDispose( data->client );
    errorString_ = "MidiInCore::openPort: error connecting OS-X MIDI input port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific port information.
  data->port = port;

  connected_ = true;
}

void MidiInCore :: openVirtualPort( const std::string &portName )
{
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

  // Create a virtual MIDI input destination.
  MIDIEndpointRef endpoint;
  OSStatus result = MIDIDestinationCreate( data->client,
                                           CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII ),
                                           midiInputCallback, (void *)&inputData_, &endpoint );
  if ( result != noErr ) {
    errorString_ = "MidiInCore::openVirtualPort: error creating virtual OS-X MIDI destination.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  data->endpoint = endpoint;
}

void MidiInCore :: closePort( void )
{
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

  if ( data->endpoint ) {
    MIDIEndpointDispose( data->endpoint );
    data->endpoint = 0;
  }

  if ( data->port ) {
    MIDIPortDispose( data->port );
    data->port = 0;
  }

  connected_ = false;
}

unsigned int MidiInCore :: getPortCount()
{
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  return MIDIGetNumberOfSources();
}

// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
CFStringRef EndpointName( MIDIEndpointRef endpoint, bool isExternal )
{
  CFMutableStringRef result = CFStringCreateMutable( NULL, 0 );
  CFStringRef str;

  // Begin with the endpoint's name.
  str = NULL;
  MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &str );
  if ( str != NULL ) {
    CFStringAppend( result, str );
    CFRelease( str );
  }

  MIDIEntityRef entity = 0;
  MIDIEndpointGetEntity( endpoint, &entity );
  if ( entity == 0 )
    // probably virtual
    return result;

  if ( CFStringGetLength( result ) == 0 ) {
    // endpoint name has zero length -- try the entity
    str = NULL;
    MIDIObjectGetStringProperty( entity, kMIDIPropertyName, &str );
    if ( str != NULL ) {
      CFStringAppend( result, str );
      CFRelease( str );
    }
  }
  // now consider the device's name
  MIDIDeviceRef device = 0;
  MIDIEntityGetDevice( entity, &device );
  if ( device == 0 )
    return result;

  str = NULL;
  MIDIObjectGetStringProperty( device, kMIDIPropertyName, &str );
  if ( CFStringGetLength( result ) == 0 ) {
      CFRelease( result );
      return str;
  }
  if ( str != NULL ) {
    // if an external device has only one entity, throw away
    // the endpoint name and just use the device name
    if ( isExternal && MIDIDeviceGetNumberOfEntities( device ) < 2 ) {
      CFRelease( result );
      return str;
    } else {
      if ( CFStringGetLength( str ) == 0 ) {
        CFRelease( str );
        return result;
      }
      // does the entity name already start with the device name?
      // (some drivers do this though they shouldn't)
      // if so, do not prepend
        if ( CFStringCompareWithOptions( result, /* endpoint name */
             str /* device name */,
             CFRangeMake(0, CFStringGetLength( str ) ), 0 ) != kCFCompareEqualTo ) {
        // prepend the device name to the entity name
        if ( CFStringGetLength( result ) > 0 )
          CFStringInsert( result, 0, CFSTR(" ") );
        CFStringInsert( result, 0, str );
      }
      CFRelease( str );
    }
  }
  return result;
}

// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
static CFStringRef ConnectedEndpointName( MIDIEndpointRef endpoint )
{
  CFMutableStringRef result = CFStringCreateMutable( NULL, 0 );
  CFStringRef str;
  OSStatus err;
  int i;

  // Does the endpoint have connections?
  CFDataRef connections = NULL;
  int nConnected = 0;
  bool anyStrings = false;
  err = MIDIObjectGetDataProperty( endpoint, kMIDIPropertyConnectionUniqueID, &connections );
  if ( connections != NULL ) {
    // It has connections, follow them
    // Concatenate the names of all connected devices
    nConnected = CFDataGetLength( connections ) / sizeof(MIDIUniqueID);
    if ( nConnected ) {
      const SInt32 *pid = (const SInt32 *)(CFDataGetBytePtr(connections));
      for ( i=0; i<nConnected; ++i, ++pid ) {
        MIDIUniqueID id = EndianS32_BtoN( *pid );
        MIDIObjectRef connObject;
        MIDIObjectType connObjectType;
        err = MIDIObjectFindByUniqueID( id, &connObject, &connObjectType );
        if ( err == noErr ) {
          if ( connObjectType == kMIDIObjectType_ExternalSource  ||
              connObjectType == kMIDIObjectType_ExternalDestination ) {
            // Connected to an external device's endpoint (10.3 and later).
            str = EndpointName( (MIDIEndpointRef)(connObject), true );
          } else {
            // Connected to an external device (10.2) (or something else, catch-
            str = NULL;
            MIDIObjectGetStringProperty( connObject, kMIDIPropertyName, &str );
          }
          if ( str != NULL ) {
            if ( anyStrings )
              CFStringAppend( result, CFSTR(", ") );
            else anyStrings = true;
            CFStringAppend( result, str );
            CFRelease( str );
          }
        }
      }
    }
    CFRelease( connections );
  }
  if ( anyStrings )
    return result;

  CFRelease( result );

  // Here, either the endpoint had no connections, or we failed to obtain names 
  return EndpointName( endpoint, false );
}

std::string MidiInCore :: getPortName( unsigned int portNumber )
{
  CFStringRef nameRef;
  MIDIEndpointRef portRef;
  char name[128];

  std::string stringName;
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  if ( portNumber >= MIDIGetNumberOfSources() ) {
    std::ostringstream ost;
    ost << "MidiInCore::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
    return stringName;
  }

  portRef = MIDIGetSource( portNumber );
  nameRef = ConnectedEndpointName(portRef);
  CFStringGetCString( nameRef, name, sizeof(name), kCFStringEncodingUTF8);
  CFRelease( nameRef );

  return stringName = name;
}

//*********************************************************************//
//  API: OS-X
//  Class Definitions: MidiOutCore
//*********************************************************************//

MidiOutCore :: MidiOutCore( const std::string &clientName ) : MidiOutApi()
{
  initialize( clientName );
}

MidiOutCore :: ~MidiOutCore( void )
{
  // Close a connection if it exists.
  closePort();

  // Cleanup.
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
  MIDIClientDispose( data->client );
  if ( data->endpoint ) MIDIEndpointDispose( data->endpoint );
  delete data;
}

void MidiOutCore :: initialize( const std::string& clientName )
{
  // Set up our client.
  MIDIClientRef client;
  CFStringRef name = CFStringCreateWithCString( NULL, clientName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDIClientCreate(name, NULL, NULL, &client );
  if ( result != noErr ) {
    std::ostringstream ost;
    ost << "MidiInCore::initialize: error creating OS-X MIDI client object (" << result << ").";
    errorString_ = ost.str();
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  CoreMidiData *data = (CoreMidiData *) new CoreMidiData;
  data->client = client;
  data->endpoint = 0;
  apiData_ = (void *) data;
  CFRelease( name );
}

unsigned int MidiOutCore :: getPortCount()
{
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  return MIDIGetNumberOfDestinations();
}

std::string MidiOutCore :: getPortName( unsigned int portNumber )
{
  CFStringRef nameRef;
  MIDIEndpointRef portRef;
  char name[128];

  std::string stringName;
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  if ( portNumber >= MIDIGetNumberOfDestinations() ) {
    std::ostringstream ost;
    ost << "MidiOutCore::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
    return stringName;
  }

  portRef = MIDIGetDestination( portNumber );
  nameRef = ConnectedEndpointName(portRef);
  CFStringGetCString( nameRef, name, sizeof(name), kCFStringEncodingUTF8 );
  CFRelease( nameRef );
  
  return stringName = name;
}

void MidiOutCore :: openPort( unsigned int portNumber, const std::string &portName )
{
  if ( connected_ ) {
    errorString_ = "MidiOutCore::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  unsigned int nDest = MIDIGetNumberOfDestinations();
  if (nDest < 1) {
    errorString_ = "MidiOutCore::openPort: no MIDI output destinations found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

  if ( portNumber >= nDest ) {
    std::ostringstream ost;
    ost << "MidiOutCore::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  MIDIPortRef port;
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
  CFStringRef portNameRef = CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDIOutputPortCreate( data->client, 
                                          portNameRef,
                                          &port );
  CFRelease( portNameRef );
  if ( result != noErr ) {
    MIDIClientDispose( data->client );
    errorString_ = "MidiOutCore::openPort: error creating OS-X MIDI output port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Get the desired output port identifier.
  MIDIEndpointRef destination = MIDIGetDestination( portNumber );
  if ( destination == 0 ) {
    MIDIPortDispose( port );
    MIDIClientDispose( data->client );
    errorString_ = "MidiOutCore::openPort: error getting MIDI output destination reference.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  data->port = port;
  data->destinationId = destination;
  connected_ = true;
}

void MidiOutCore :: closePort( void )
{
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

  if ( data->endpoint ) {
    MIDIEndpointDispose( data->endpoint );
    data->endpoint = 0;
  }

  if ( data->port ) {
    MIDIPortDispose( data->port );
    data->port = 0;
  }

  connected_ = false;
}

void MidiOutCore :: openVirtualPort( const std::string &portName )
{
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

  if ( data->endpoint ) {
    errorString_ = "MidiOutCore::openVirtualPort: a virtual output port already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  // Create a virtual MIDI output source.
  MIDIEndpointRef endpoint;
  OSStatus result = MIDISourceCreate( data->client,
                                      CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII ),
                                      &endpoint );
  if ( result != noErr ) {
    errorString_ = "MidiOutCore::initialize: error creating OS-X virtual MIDI source.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  data->endpoint = endpoint;
}

void MidiOutCore :: sendMessage( const unsigned char *message, size_t size )
{
  // We use the MIDISendSysex() function to asynchronously send sysex
  // messages.  Otherwise, we use a single CoreMidi MIDIPacket.
  unsigned int nBytes = static_cast<unsigned int> (size);
  if ( nBytes == 0 ) {
    errorString_ = "MidiOutCore::sendMessage: no data in message argument!";      
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  MIDITimeStamp timeStamp = AudioGetCurrentHostTime();
  CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
  OSStatus result;

  if ( message[0] != 0xF0 && nBytes > 3 ) {
    errorString_ = "MidiOutCore::sendMessage: message format problem ... not sysex but > 3 bytes?";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  Byte buffer[nBytes+(sizeof(MIDIPacketList))];
  ByteCount listSize = sizeof(buffer);
  MIDIPacketList *packetList = (MIDIPacketList*)buffer;
  MIDIPacket *packet = MIDIPacketListInit( packetList );

  ByteCount remainingBytes = nBytes;
  while (remainingBytes && packet) {
    ByteCount bytesForPacket = remainingBytes > 65535 ? 65535 : remainingBytes; // 65535 = maximum size of a MIDIPacket
    const Byte* dataStartPtr = (const Byte *) &message[nBytes - remainingBytes];
    packet = MIDIPacketListAdd( packetList, listSize, packet, timeStamp, bytesForPacket, dataStartPtr);
    remainingBytes -= bytesForPacket; 
  }

  if ( !packet ) {
    errorString_ = "MidiOutCore::sendMessage: could not allocate packet list";      
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Send to any destinations that may have connected to us.
  if ( data->endpoint ) {
    result = MIDIReceived( data->endpoint, packetList );
    if ( result != noErr ) {
      errorString_ = "MidiOutCore::sendMessage: error sending MIDI to virtual destinations.";
      error( RtMidiError::WARNING, errorString_ );
    }
  }

  // And send to an explicit destination port if we're connected.
  if ( connected_ ) {
    result = MIDISend( data->port, data->destinationId, packetList );
    if ( result != noErr ) {
      errorString_ = "MidiOutCore::sendMessage: error sending MIDI message to port.";
      error( RtMidiError::WARNING, errorString_ );
    }
  }
}

#endif  // __MACOSX_CORE__


//*********************************************************************//
//  API: LINUX ALSA SEQUENCER
//*********************************************************************//

// API information found at:
//   - http://www.alsa-project.org/documentation.php#Library

#if defined(__LINUX_ALSA__)

// The ALSA Sequencer API is based on the use of a callback function for
// MIDI input.
//
// Thanks to Pedro Lopez-Cabanillas for help with the ALSA sequencer
// time stamps and other assorted fixes!!!

// If you don't need timestamping for incoming MIDI events, define the
// preprocessor definition AVOID_TIMESTAMPING to save resources
// associated with the ALSA sequencer queues.

#include <pthread.h>
#include <sys/time.h>

// ALSA header file.
#include <alsa/asoundlib.h>

// A structure to hold variables related to the ALSA API
// implementation.
struct AlsaMidiData {
  snd_seq_t *seq;
  unsigned int portNum;
  int vport;
  snd_seq_port_subscribe_t *subscription;
  snd_midi_event_t *coder;
  unsigned int bufferSize;
  unsigned char *buffer;
  pthread_t thread;
  pthread_t dummy_thread_id;
  snd_seq_real_time_t lastTime;
  int queue_id; // an input queue is needed to get timestamped events
  int trigger_fds[2];
};

#define PORT_TYPE( pinfo, bits ) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))

//*********************************************************************//
//  API: LINUX ALSA
//  Class Definitions: MidiInAlsa
//*********************************************************************//

static void *alsaMidiHandler( void *ptr )
{
  MidiInApi::RtMidiInData *data = static_cast<MidiInApi::RtMidiInData *> (ptr);
  AlsaMidiData *apiData = static_cast<AlsaMidiData *> (data->apiData);

  long nBytes;
  double time;
  bool continueSysex = false;
  bool doDecode = false;
  MidiInApi::MidiMessage message;
  int poll_fd_count;
  struct pollfd *poll_fds;

  snd_seq_event_t *ev;
  int result;
  apiData->bufferSize = 32;
  result = snd_midi_event_new( 0, &apiData->coder );
  if ( result < 0 ) {
    data->doInput = false;
    std::cerr << "\nMidiInAlsa::alsaMidiHandler: error initializing MIDI event parser!\n\n";
    return 0;
  }
  unsigned char *buffer = (unsigned char *) malloc( apiData->bufferSize );
  if ( buffer == NULL ) {
    data->doInput = false;
    snd_midi_event_free( apiData->coder );
    apiData->coder = 0;
    std::cerr << "\nMidiInAlsa::alsaMidiHandler: error initializing buffer memory!\n\n";
    return 0;
  }
  snd_midi_event_init( apiData->coder );
  snd_midi_event_no_status( apiData->coder, 1 ); // suppress running status messages

  poll_fd_count = snd_seq_poll_descriptors_count( apiData->seq, POLLIN ) + 1;
  poll_fds = (struct pollfd*)alloca( poll_fd_count * sizeof( struct pollfd ));
  snd_seq_poll_descriptors( apiData->seq, poll_fds + 1, poll_fd_count - 1, POLLIN );
  poll_fds[0].fd = apiData->trigger_fds[0];
  poll_fds[0].events = POLLIN;

  while ( data->doInput ) {

    if ( snd_seq_event_input_pending( apiData->seq, 1 ) == 0 ) {
      // No data pending
      if ( poll( poll_fds, poll_fd_count, -1) >= 0 ) {
        if ( poll_fds[0].revents & POLLIN ) {
          bool dummy;
          int res = read( poll_fds[0].fd, &dummy, sizeof(dummy) );
          (void) res;
        }
      }
      continue;
    }

    // If here, there should be data.
    result = snd_seq_event_input( apiData->seq, &ev );
    if ( result == -ENOSPC ) {
      std::cerr << "\nMidiInAlsa::alsaMidiHandler: MIDI input buffer overrun!\n\n";
      continue;
    }
    else if ( result <= 0 ) {
      std::cerr << "\nMidiInAlsa::alsaMidiHandler: unknown MIDI input error!\n";
      perror("System reports");
      continue;
    }

    // This is a bit weird, but we now have to decode an ALSA MIDI
    // event (back) into MIDI bytes.  We'll ignore non-MIDI types.
    if ( !continueSysex ) message.bytes.clear();

    doDecode = false;
    switch ( ev->type ) {

    case SND_SEQ_EVENT_PORT_SUBSCRIBED:
#if defined(__RTMIDI_DEBUG__)
      std::cout << "MidiInAlsa::alsaMidiHandler: port connection made!\n";
#endif
      break;

    case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
#if defined(__RTMIDI_DEBUG__)
      std::cerr << "MidiInAlsa::alsaMidiHandler: port connection has closed!\n";
      std::cout << "sender = " << (int) ev->data.connect.sender.client << ":"
                << (int) ev->data.connect.sender.port
                << ", dest = " << (int) ev->data.connect.dest.client << ":"
                << (int) ev->data.connect.dest.port
                << std::endl;
#endif
      break;

    case SND_SEQ_EVENT_QFRAME: // MIDI time code
      if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
      break;

    case SND_SEQ_EVENT_TICK: // 0xF9 ... MIDI timing tick
      if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
      break;

    case SND_SEQ_EVENT_CLOCK: // 0xF8 ... MIDI timing (clock) tick
      if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
      break;

    case SND_SEQ_EVENT_SENSING: // Active sensing
      if ( !( data->ignoreFlags & 0x04 ) ) doDecode = true;
      break;

		case SND_SEQ_EVENT_SYSEX:
      if ( (data->ignoreFlags & 0x01) ) break;
      if ( ev->data.ext.len > apiData->bufferSize ) {
        apiData->bufferSize = ev->data.ext.len;
        free( buffer );
        buffer = (unsigned char *) malloc( apiData->bufferSize );
        if ( buffer == NULL ) {
          data->doInput = false;
          std::cerr << "\nMidiInAlsa::alsaMidiHandler: error resizing buffer memory!\n\n";
          break;
        }
      }

    default:
      doDecode = true;
    }

    if ( doDecode ) {

      nBytes = snd_midi_event_decode( apiData->coder, buffer, apiData->bufferSize, ev );
      if ( nBytes > 0 ) {
        // The ALSA sequencer has a maximum buffer size for MIDI sysex
        // events of 256 bytes.  If a device sends sysex messages larger
        // than this, they are segmented into 256 byte chunks.  So,
        // we'll watch for this and concatenate sysex chunks into a
        // single sysex message if necessary.
        if ( !continueSysex )
          message.bytes.assign( buffer, &buffer[nBytes] );
        else
          message.bytes.insert( message.bytes.end(), buffer, &buffer[nBytes] );

        continueSysex = ( ( ev->type == SND_SEQ_EVENT_SYSEX ) && ( message.bytes.back() != 0xF7 ) );
        if ( !continueSysex ) {

          // Calculate the time stamp:
          message.timeStamp = 0.0;

          // Method 1: Use the system time.
          //(void)gettimeofday(&tv, (struct timezone *)NULL);
          //time = (tv.tv_sec * 1000000) + tv.tv_usec;

          // Method 2: Use the ALSA sequencer event time data.
          // (thanks to Pedro Lopez-Cabanillas!).

          // Using method from:
          // https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html

          // Perform the carry for the later subtraction by updating y.
          snd_seq_real_time_t &x(ev->time.time);
          snd_seq_real_time_t &y(apiData->lastTime);
          if (x.tv_nsec < y.tv_nsec) {
              int nsec = (y.tv_nsec - x.tv_nsec) / 1000000000 + 1;
              y.tv_nsec -= 1000000000 * nsec;
              y.tv_sec += nsec;
          }
          if (x.tv_nsec - y.tv_nsec > 1000000000) {
              int nsec = (x.tv_nsec - y.tv_nsec) / 1000000000;
              y.tv_nsec += 1000000000 * nsec;
              y.tv_sec -= nsec;
          }

          // Compute the time difference.
          time = x.tv_sec - y.tv_sec + (x.tv_nsec - y.tv_nsec)*1e-9;

          apiData->lastTime = ev->time.time;

          if ( data->firstMessage == true )
            data->firstMessage = false;
          else
            message.timeStamp = time;
            
          // 10/02/19: absolute usec based time stamp
          message.timeStamp *= 1000000;
        }
        else {
#if defined(__RTMIDI_DEBUG__)
          std::cerr << "\nMidiInAlsa::alsaMidiHandler: event parsing error or not a MIDI event!\n\n";
#endif
        }
      }
    }

    snd_seq_free_event( ev );
    if ( message.bytes.size() == 0 || continueSysex ) continue;

    if ( data->usingCallback ) {
      RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
      callback( message.timeStamp, &message.bytes, data->userData );
    }
    else {
      // As long as we haven't reached our queue size limit, push the message.
      if (!data->queue.push(message))
        std::cerr << "\nMidiInAlsa: message queue limit reached!!\n\n";
    }
  }

  if ( buffer ) free( buffer );
  snd_midi_event_free( apiData->coder );
  apiData->coder = 0;
  apiData->thread = apiData->dummy_thread_id;
  return 0;
}

MidiInAlsa :: MidiInAlsa( const std::string &clientName, unsigned int queueSizeLimit ) : MidiInApi( queueSizeLimit )
{
  initialize( clientName );
}

MidiInAlsa :: ~MidiInAlsa()
{
  // Close a connection if it exists.
  closePort();

  // Shutdown the input thread.
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( inputData_.doInput ) {
    inputData_.doInput = false;
    int res = write( data->trigger_fds[1], &inputData_.doInput, sizeof(inputData_.doInput) );
    (void) res;
    if ( !pthread_equal(data->thread, data->dummy_thread_id) )
      pthread_join( data->thread, NULL );
  }

  // Cleanup.
  close ( data->trigger_fds[0] );
  close ( data->trigger_fds[1] );
  if ( data->vport >= 0 ) snd_seq_delete_port( data->seq, data->vport );
#ifndef AVOID_TIMESTAMPING
  snd_seq_free_queue( data->seq, data->queue_id );
#endif
  snd_seq_close( data->seq );
  delete data;
}

void MidiInAlsa :: initialize( const std::string& clientName )
{
  // Set up the ALSA sequencer client.
  snd_seq_t *seq;
  int result = snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
  if ( result < 0 ) {
    errorString_ = "MidiInAlsa::initialize: error creating ALSA sequencer client object.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Set client name.
  snd_seq_set_client_name( seq, clientName.c_str() );

  // Save our api-specific connection information.
  AlsaMidiData *data = (AlsaMidiData *) new AlsaMidiData;
  data->seq = seq;
  data->portNum = -1;
  data->vport = -1;
  data->subscription = 0;
  data->dummy_thread_id = pthread_self();
  data->thread = data->dummy_thread_id;
  data->trigger_fds[0] = -1;
  data->trigger_fds[1] = -1;
  apiData_ = (void *) data;
  inputData_.apiData = (void *) data;

   if ( pipe(data->trigger_fds) == -1 ) {
    errorString_ = "MidiInAlsa::initialize: error creating pipe objects.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Create the input queue
#ifndef AVOID_TIMESTAMPING
  data->queue_id = snd_seq_alloc_named_queue(seq, "RtMidi Queue");
  // Set arbitrary tempo (mm=100) and resolution (240)
  snd_seq_queue_tempo_t *qtempo;
  snd_seq_queue_tempo_alloca(&qtempo);
  snd_seq_queue_tempo_set_tempo(qtempo, 600000);
  snd_seq_queue_tempo_set_ppq(qtempo, 240);
  snd_seq_set_queue_tempo(data->seq, data->queue_id, qtempo);
  snd_seq_drain_output(data->seq);
#endif
}

// This function is used to count or get the pinfo structure for a given port number.
unsigned int portInfo( snd_seq_t *seq, snd_seq_port_info_t *pinfo, unsigned int type, int portNumber )
{
  snd_seq_client_info_t *cinfo;
  int client;
  int count = 0;
  snd_seq_client_info_alloca( &cinfo );

  snd_seq_client_info_set_client( cinfo, -1 );
  while ( snd_seq_query_next_client( seq, cinfo ) >= 0 ) {
    client = snd_seq_client_info_get_client( cinfo );
    if ( client == 0 ) continue;
    // Reset query info
    snd_seq_port_info_set_client( pinfo, client );
    snd_seq_port_info_set_port( pinfo, -1 );
    while ( snd_seq_query_next_port( seq, pinfo ) >= 0 ) {
      unsigned int atyp = snd_seq_port_info_get_type( pinfo );
      if ( ( ( atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC ) == 0 ) &&
           ( ( atyp & SND_SEQ_PORT_TYPE_SYNTH ) == 0 ) &&
           ( ( atyp & SND_SEQ_PORT_TYPE_APPLICATION ) == 0 ) ) continue;
	    
      unsigned int caps = snd_seq_port_info_get_capability( pinfo );
      if ( ( caps & type ) != type ) continue;
      if ( count == portNumber ) return 1;
      ++count;
    }
  }

  // If a negative portNumber was used, return the port count.
  if ( portNumber < 0 ) return count;
  return 0;
}

unsigned int MidiInAlsa :: getPortCount()
{
  snd_seq_port_info_t *pinfo;
  snd_seq_port_info_alloca( &pinfo );

  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  return portInfo( data->seq, pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, -1 );
}

std::string MidiInAlsa :: getPortName( unsigned int portNumber )
{
  snd_seq_client_info_t *cinfo;
  snd_seq_port_info_t *pinfo;
  snd_seq_client_info_alloca( &cinfo );
  snd_seq_port_info_alloca( &pinfo );

  std::string stringName;
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( portInfo( data->seq, pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, (int) portNumber ) ) {
    int cnum = snd_seq_port_info_get_client( pinfo );
    snd_seq_get_any_client_info( data->seq, cnum, cinfo );
    std::ostringstream os;
    os << snd_seq_client_info_get_name( cinfo );
    os << ":";
    os << snd_seq_port_info_get_name( pinfo );
    os << " ";                                    // These lines added to make sure devices are listed
    os << snd_seq_port_info_get_client( pinfo );  // with full portnames added to ensure individual device names
    os << ":";
    os << snd_seq_port_info_get_port( pinfo );
    stringName = os.str();
    return stringName;
  }

  // If we get here, we didn't find a match.
  errorString_ = "MidiInAlsa::getPortName: error looking for port name!";
  error( RtMidiError::WARNING, errorString_ );
  return stringName;
}

void MidiInAlsa :: openPort( unsigned int portNumber, const std::string &portName )
{
  if ( connected_ ) {
    errorString_ = "MidiInAlsa::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  unsigned int nSrc = this->getPortCount();
  if ( nSrc < 1 ) {
    errorString_ = "MidiInAlsa::openPort: no MIDI input sources found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

  snd_seq_port_info_t *src_pinfo;
  snd_seq_port_info_alloca( &src_pinfo );
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( portInfo( data->seq, src_pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, (int) portNumber ) == 0 ) {
    std::ostringstream ost;
    ost << "MidiInAlsa::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  snd_seq_addr_t sender, receiver;
  sender.client = snd_seq_port_info_get_client( src_pinfo );
  sender.port = snd_seq_port_info_get_port( src_pinfo );
  receiver.client = snd_seq_client_id( data->seq );

  snd_seq_port_info_t *pinfo;
  snd_seq_port_info_alloca( &pinfo );
  if ( data->vport < 0 ) {
    snd_seq_port_info_set_client( pinfo, 0 );
    snd_seq_port_info_set_port( pinfo, 0 );
    snd_seq_port_info_set_capability( pinfo,
                                      SND_SEQ_PORT_CAP_WRITE |
                                      SND_SEQ_PORT_CAP_SUBS_WRITE );
    snd_seq_port_info_set_type( pinfo,
                                SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                                SND_SEQ_PORT_TYPE_APPLICATION );
    snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef AVOID_TIMESTAMPING
    snd_seq_port_info_set_timestamping(pinfo, 1);
    snd_seq_port_info_set_timestamp_real(pinfo, 1);    
    snd_seq_port_info_set_timestamp_queue(pinfo, data->queue_id);
#endif
    snd_seq_port_info_set_name(pinfo,  portName.c_str() );
    data->vport = snd_seq_create_port(data->seq, pinfo);
  
    if ( data->vport < 0 ) {
      errorString_ = "MidiInAlsa::openPort: ALSA error creating input port.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
    data->vport = snd_seq_port_info_get_port(pinfo);
  }

  receiver.port = data->vport;

  if ( !data->subscription ) {
    // Make subscription
    if (snd_seq_port_subscribe_malloc( &data->subscription ) < 0) {
      errorString_ = "MidiInAlsa::openPort: ALSA error allocation port subscription.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
    snd_seq_port_subscribe_set_sender(data->subscription, &sender);
    snd_seq_port_subscribe_set_dest(data->subscription, &receiver);
    if ( snd_seq_subscribe_port(data->seq, data->subscription) ) {
      snd_seq_port_subscribe_free( data->subscription );
      data->subscription = 0;
      errorString_ = "MidiInAlsa::openPort: ALSA error making port connection.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
  }

  if ( inputData_.doInput == false ) {
    // Start the input queue
#ifndef AVOID_TIMESTAMPING
    snd_seq_start_queue( data->seq, data->queue_id, NULL );
    snd_seq_drain_output( data->seq );
#endif
    // Start our MIDI input thread.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

    inputData_.doInput = true;
    int err = pthread_create(&data->thread, &attr, alsaMidiHandler, &inputData_);
    pthread_attr_destroy(&attr);
    if ( err ) {
      snd_seq_unsubscribe_port( data->seq, data->subscription );
      snd_seq_port_subscribe_free( data->subscription );
      data->subscription = 0;
      inputData_.doInput = false;
      errorString_ = "MidiInAlsa::openPort: error starting MIDI input thread!";
      error( RtMidiError::THREAD_ERROR, errorString_ );
      return;
    }
  }

  connected_ = true;
}

void MidiInAlsa :: openVirtualPort( const std::string &portName )
{
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( data->vport < 0 ) {
    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );
    snd_seq_port_info_set_capability( pinfo,
				      SND_SEQ_PORT_CAP_WRITE |
				      SND_SEQ_PORT_CAP_SUBS_WRITE );
    snd_seq_port_info_set_type( pinfo,
				SND_SEQ_PORT_TYPE_MIDI_GENERIC |
				SND_SEQ_PORT_TYPE_APPLICATION );
    snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef AVOID_TIMESTAMPING
    snd_seq_port_info_set_timestamping(pinfo, 1);
    snd_seq_port_info_set_timestamp_real(pinfo, 1);    
    snd_seq_port_info_set_timestamp_queue(pinfo, data->queue_id);
#endif
    snd_seq_port_info_set_name(pinfo, portName.c_str());
    data->vport = snd_seq_create_port(data->seq, pinfo);

    if ( data->vport < 0 ) {
      errorString_ = "MidiInAlsa::openVirtualPort: ALSA error creating virtual port.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
    data->vport = snd_seq_port_info_get_port(pinfo);
  }

  if ( inputData_.doInput == false ) {
    // Wait for old thread to stop, if still running
    if ( !pthread_equal(data->thread, data->dummy_thread_id) )
      pthread_join( data->thread, NULL );

    // Start the input queue
#ifndef AVOID_TIMESTAMPING
    snd_seq_start_queue( data->seq, data->queue_id, NULL );
    snd_seq_drain_output( data->seq );
#endif
    // Start our MIDI input thread.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

    inputData_.doInput = true;
    int err = pthread_create(&data->thread, &attr, alsaMidiHandler, &inputData_);
    pthread_attr_destroy(&attr);
    if ( err ) {
      if ( data->subscription ) {
        snd_seq_unsubscribe_port( data->seq, data->subscription );
        snd_seq_port_subscribe_free( data->subscription );
        data->subscription = 0;
      }
      inputData_.doInput = false;
      errorString_ = "MidiInAlsa::openPort: error starting MIDI input thread!";
      error( RtMidiError::THREAD_ERROR, errorString_ );
      return;
    }
  }
}

void MidiInAlsa :: closePort( void )
{
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);

  if ( connected_ ) {
    if ( data->subscription ) {
      snd_seq_unsubscribe_port( data->seq, data->subscription );
      snd_seq_port_subscribe_free( data->subscription );
      data->subscription = 0;
    }
    // Stop the input queue
#ifndef AVOID_TIMESTAMPING
    snd_seq_stop_queue( data->seq, data->queue_id, NULL );
    snd_seq_drain_output( data->seq );
#endif
    connected_ = false;
  }

  // Stop thread to avoid triggering the callback, while the port is intended to be closed
  if ( inputData_.doInput ) {
    inputData_.doInput = false;
    int res = write( data->trigger_fds[1], &inputData_.doInput, sizeof(inputData_.doInput) );
    (void) res;
    if ( !pthread_equal(data->thread, data->dummy_thread_id) )
      pthread_join( data->thread, NULL );
  }
}

//*********************************************************************//
//  API: LINUX ALSA
//  Class Definitions: MidiOutAlsa
//*********************************************************************//

MidiOutAlsa :: MidiOutAlsa( const std::string &clientName ) : MidiOutApi()
{
  initialize( clientName );
}

MidiOutAlsa :: ~MidiOutAlsa()
{
  // Close a connection if it exists.
  closePort();

  // Cleanup.
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( data->vport >= 0 ) snd_seq_delete_port( data->seq, data->vport );
  if ( data->coder ) snd_midi_event_free( data->coder );
  if ( data->buffer ) free( data->buffer );
  snd_seq_close( data->seq );
  delete data;
}

void MidiOutAlsa :: initialize( const std::string& clientName )
{
  // Set up the ALSA sequencer client.
  snd_seq_t *seq;
  int result1 = snd_seq_open( &seq, "default", SND_SEQ_OPEN_OUTPUT, SND_SEQ_NONBLOCK );
  if ( result1 < 0 ) {
    errorString_ = "MidiOutAlsa::initialize: error creating ALSA sequencer client object.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
	}

  // Set client name.
  snd_seq_set_client_name( seq, clientName.c_str() );

  // Save our api-specific connection information.
  AlsaMidiData *data = (AlsaMidiData *) new AlsaMidiData;
  data->seq = seq;
  data->portNum = -1;
  data->vport = -1;
  data->bufferSize = 32;
  data->coder = 0;
  data->buffer = 0;
  int result = snd_midi_event_new( data->bufferSize, &data->coder );
  if ( result < 0 ) {
    delete data;
    errorString_ = "MidiOutAlsa::initialize: error initializing MIDI event parser!\n\n";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }
  data->buffer = (unsigned char *) malloc( data->bufferSize );
  if ( data->buffer == NULL ) {
    delete data;
    errorString_ = "MidiOutAlsa::initialize: error allocating buffer memory!\n\n";
    error( RtMidiError::MEMORY_ERROR, errorString_ );
    return;
  }
  snd_midi_event_init( data->coder );
  apiData_ = (void *) data;
}

unsigned int MidiOutAlsa :: getPortCount()
{
	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca( &pinfo );

  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  return portInfo( data->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, -1 );
}

std::string MidiOutAlsa :: getPortName( unsigned int portNumber )
{
  snd_seq_client_info_t *cinfo;
  snd_seq_port_info_t *pinfo;
  snd_seq_client_info_alloca( &cinfo );
  snd_seq_port_info_alloca( &pinfo );

  std::string stringName;
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( portInfo( data->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, (int) portNumber ) ) {
    int cnum = snd_seq_port_info_get_client(pinfo);
    snd_seq_get_any_client_info( data->seq, cnum, cinfo );
    std::ostringstream os;
    os << snd_seq_client_info_get_name(cinfo);
    os << ":";
    os << snd_seq_port_info_get_name( pinfo );
    os << " ";                                    // These lines added to make sure devices are listed
    os << snd_seq_port_info_get_client( pinfo );  // with full portnames added to ensure individual device names
    os << ":";
    os << snd_seq_port_info_get_port(pinfo);
    stringName = os.str();
    return stringName;
  }

  // If we get here, we didn't find a match.
  errorString_ = "MidiOutAlsa::getPortName: error looking for port name!";
  error( RtMidiError::WARNING, errorString_ );
  return stringName;
}

void MidiOutAlsa :: openPort( unsigned int portNumber, const std::string &portName )
{
  if ( connected_ ) {
    errorString_ = "MidiOutAlsa::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  unsigned int nSrc = this->getPortCount();
  if (nSrc < 1) {
    errorString_ = "MidiOutAlsa::openPort: no MIDI output sources found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca( &pinfo );
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( portInfo( data->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, (int) portNumber ) == 0 ) {
    std::ostringstream ost;
    ost << "MidiOutAlsa::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  snd_seq_addr_t sender, receiver;
  receiver.client = snd_seq_port_info_get_client( pinfo );
  receiver.port = snd_seq_port_info_get_port( pinfo );
  sender.client = snd_seq_client_id( data->seq );

  if ( data->vport < 0 ) {
    data->vport = snd_seq_create_simple_port( data->seq, portName.c_str(),
                                              SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                                              SND_SEQ_PORT_TYPE_MIDI_GENERIC|SND_SEQ_PORT_TYPE_APPLICATION );
    if ( data->vport < 0 ) {
      errorString_ = "MidiOutAlsa::openPort: ALSA error creating output port.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
  }

  sender.port = data->vport;

  // Make subscription
  if (snd_seq_port_subscribe_malloc( &data->subscription ) < 0) {
    snd_seq_port_subscribe_free( data->subscription );
    errorString_ = "MidiOutAlsa::openPort: error allocating port subscription.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }
  snd_seq_port_subscribe_set_sender(data->subscription, &sender);
  snd_seq_port_subscribe_set_dest(data->subscription, &receiver);
  snd_seq_port_subscribe_set_time_update(data->subscription, 1);
  snd_seq_port_subscribe_set_time_real(data->subscription, 1);
  if ( snd_seq_subscribe_port(data->seq, data->subscription) ) {
    snd_seq_port_subscribe_free( data->subscription );
    errorString_ = "MidiOutAlsa::openPort: ALSA error making port connection.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  connected_ = true;
}

void MidiOutAlsa :: closePort( void )
{
  if ( connected_ ) {
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    snd_seq_unsubscribe_port( data->seq, data->subscription );
    snd_seq_port_subscribe_free( data->subscription );
    data->subscription = 0;
    connected_ = false;
  }
}

void MidiOutAlsa :: openVirtualPort( const std::string &portName )
{
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  if ( data->vport < 0 ) {
    data->vport = snd_seq_create_simple_port( data->seq, portName.c_str(),
                                              SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                                              SND_SEQ_PORT_TYPE_MIDI_GENERIC|SND_SEQ_PORT_TYPE_APPLICATION );

    if ( data->vport < 0 ) {
      errorString_ = "MidiOutAlsa::openVirtualPort: ALSA error creating virtual port.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
    }
  }
}

void MidiOutAlsa :: sendMessage( const unsigned char *message, size_t size )
{
  int result;
  AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
  unsigned int nBytes = static_cast<unsigned int> (size);
  if ( nBytes > data->bufferSize ) {
    data->bufferSize = nBytes;
    result = snd_midi_event_resize_buffer ( data->coder, nBytes);
    if ( result != 0 ) {
      errorString_ = "MidiOutAlsa::sendMessage: ALSA error resizing MIDI event buffer.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
    free (data->buffer);
    data->buffer = (unsigned char *) malloc( data->bufferSize );
    if ( data->buffer == NULL ) {
    errorString_ = "MidiOutAlsa::initialize: error allocating buffer memory!\n\n";
    error( RtMidiError::MEMORY_ERROR, errorString_ );
    return;
    }
  }

  snd_seq_event_t ev;
  snd_seq_ev_clear(&ev);
  snd_seq_ev_set_source(&ev, data->vport);
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_set_direct(&ev);
  for ( unsigned int i=0; i<nBytes; ++i ) data->buffer[i] = message[i];
  result = snd_midi_event_encode( data->coder, data->buffer, (long)nBytes, &ev );
  if ( result < (int)nBytes ) {
    errorString_ = "MidiOutAlsa::sendMessage: event parsing error!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  // Send the event.
  result = snd_seq_event_output(data->seq, &ev);
  if ( result < 0 ) {
    errorString_ = "MidiOutAlsa::sendMessage: error sending MIDI message to port.";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }
  snd_seq_drain_output(data->seq);
}

#endif // __LINUX_ALSA__


//*********************************************************************//
//  API: Windows Multimedia Library (MM)
//*********************************************************************//

// API information deciphered from:
//  - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_midi_reference.asp

// Thanks to Jean-Baptiste Berruchon for the sysex code.

#if defined(__WINDOWS_MM__)

// The Windows MM API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// Windows MM MIDI header files.
#include <windows.h>
#include <mmsystem.h>

// Convert a null-terminated wide string or ANSI-encoded string to UTF-8.
static std::string ConvertToUTF8(const TCHAR *str)
{
  std::string u8str;
  const WCHAR *wstr = L"";
#if defined( UNICODE ) || defined( _UNICODE )
  wstr = str;
#else
  // Convert from ANSI encoding to wide string
  int wlength = MultiByteToWideChar( CP_ACP, 0, str, -1, NULL, 0 );
  std::wstring wstrtemp;
  if ( wlength )
  {
    wstrtemp.assign( wlength - 1, 0 );
    MultiByteToWideChar( CP_ACP, 0, str, -1, &wstrtemp[0], wlength );
    wstr = &wstrtemp[0];
  }
#endif
  // Convert from wide string to UTF-8
  int length = WideCharToMultiByte( CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL );
  if ( length )
  {
    u8str.assign( length - 1, 0 );
    length = WideCharToMultiByte( CP_UTF8, 0, wstr, -1, &u8str[0], length, NULL, NULL );
  }
  return u8str;
}

#define  RT_SYSEX_BUFFER_SIZE 1024
#define  RT_SYSEX_BUFFER_COUNT 4

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct WinMidiData {
  HMIDIIN inHandle;    // Handle to Midi Input Device
  HMIDIOUT outHandle;  // Handle to Midi Output Device
  DWORD lastTime;
  MidiInApi::MidiMessage message;
  LPMIDIHDR sysexBuffer[RT_SYSEX_BUFFER_COUNT];
  CRITICAL_SECTION _mutex; // [Patrice] see https://groups.google.com/forum/#!topic/mididev/6OUjHutMpEo
};

//*********************************************************************//
//  API: Windows MM
//  Class Definitions: MidiInWinMM
//*********************************************************************//

static void CALLBACK midiInputCallback( HMIDIIN /*hmin*/,
                                        UINT inputStatus, 
                                        DWORD_PTR instancePtr,
                                        DWORD_PTR midiMessage,
                                        DWORD timestamp )
{
  if ( inputStatus != MIM_DATA && inputStatus != MIM_LONGDATA && inputStatus != MIM_LONGERROR ) return;

  //MidiInApi::RtMidiInData *data = static_cast<MidiInApi::RtMidiInData *> (instancePtr);
  MidiInApi::RtMidiInData *data = (MidiInApi::RtMidiInData *)instancePtr;
  WinMidiData *apiData = static_cast<WinMidiData *> (data->apiData);

  /*
  // Calculate time stamp.
  if ( data->firstMessage == true ) {
    apiData->message.timeStamp = 0.0;
    data->firstMessage = false;
  }
  else apiData->message.timeStamp = (double) ( timestamp - apiData->lastTime ) * 0.001;
  apiData->lastTime = timestamp;
  */
    
  // 10/02/19: absolute usec based time stamp
  apiData->message.timeStamp = double(timestamp) * 1000;

  if ( inputStatus == MIM_DATA ) { // Channel or system message

    // Make sure the first byte is a status byte.
    unsigned char status = (unsigned char) (midiMessage & 0x000000FF);
    if ( !(status & 0x80) ) return;

    // Determine the number of bytes in the MIDI message.
    unsigned short nBytes = 1;
    if ( status < 0xC0 ) nBytes = 3;
    else if ( status < 0xE0 ) nBytes = 2;
    else if ( status < 0xF0 ) nBytes = 3;
    else if ( status == 0xF1 ) {
      if ( data->ignoreFlags & 0x02 ) return;
      else nBytes = 2;
    }
    else if ( status == 0xF2 ) nBytes = 3;
    else if ( status == 0xF3 ) nBytes = 2;
    else if ( status == 0xF8 && (data->ignoreFlags & 0x02) ) {
      // A MIDI timing tick message and we're ignoring it.
      return;
    }
    else if ( status == 0xFE && (data->ignoreFlags & 0x04) ) {
      // A MIDI active sensing message and we're ignoring it.
      return;
    }

    // Copy bytes to our MIDI message.
    unsigned char *ptr = (unsigned char *) &midiMessage;
    for ( int i=0; i<nBytes; ++i ) apiData->message.bytes.push_back( *ptr++ );
  }
  else { // Sysex message ( MIM_LONGDATA or MIM_LONGERROR )
    MIDIHDR *sysex = ( MIDIHDR *) midiMessage; 
    if ( !( data->ignoreFlags & 0x01 ) && inputStatus != MIM_LONGERROR ) {  
      // Sysex message and we're not ignoring it
      for ( int i=0; i<(int)sysex->dwBytesRecorded; ++i )
        apiData->message.bytes.push_back( sysex->lpData[i] );
    }

    // The WinMM API requires that the sysex buffer be requeued after
    // input of each sysex message.  Even if we are ignoring sysex
    // messages, we still need to requeue the buffer in case the user
    // decides to not ignore sysex messages in the future.  However,
    // it seems that WinMM calls this function with an empty sysex
    // buffer when an application closes and in this case, we should
    // avoid requeueing it, else the computer suddenly reboots after
    // one or two minutes.
    if ( apiData->sysexBuffer[sysex->dwUser]->dwBytesRecorded > 0 ) {
      //if ( sysex->dwBytesRecorded > 0 ) {
      EnterCriticalSection( &(apiData->_mutex) );
      MMRESULT result = midiInAddBuffer( apiData->inHandle, apiData->sysexBuffer[sysex->dwUser], sizeof(MIDIHDR) );
      LeaveCriticalSection( &(apiData->_mutex) );
      if ( result != MMSYSERR_NOERROR )
        std::cerr << "\nRtMidiIn::midiInputCallback: error sending sysex to Midi device!!\n\n";

      if ( data->ignoreFlags & 0x01 ) return;
    }
    else return;
  }

  if ( data->usingCallback ) {
    RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
    callback( apiData->message.timeStamp, &apiData->message.bytes, data->userData );
  }
  else {
    // As long as we haven't reached our queue size limit, push the message.
    if (!data->queue.push(apiData->message))
      std::cerr << "\nMidiInWinMM: message queue limit reached!!\n\n";
  }

  // Clear the vector for the next input message.
  apiData->message.bytes.clear();
}

MidiInWinMM :: MidiInWinMM( const std::string &clientName, unsigned int queueSizeLimit ) : MidiInApi( queueSizeLimit )
{
  initialize( clientName );
}

MidiInWinMM :: ~MidiInWinMM()
{
  // Close a connection if it exists.
  closePort();

  WinMidiData *data = static_cast<WinMidiData *> (apiData_);
  DeleteCriticalSection( &(data->_mutex) );

  // Cleanup.
  delete data;
}

void MidiInWinMM :: initialize( const std::string& /*clientName*/ )
{
  // We'll issue a warning here if no devices are available but not
  // throw an error since the user can plugin something later.
  unsigned int nDevices = midiInGetNumDevs();
  if ( nDevices == 0 ) {
    errorString_ = "MidiInWinMM::initialize: no MIDI input devices currently available.";
    error( RtMidiError::WARNING, errorString_ );
  }

  // Save our api-specific connection information.
  WinMidiData *data = (WinMidiData *) new WinMidiData;
  apiData_ = (void *) data;
  inputData_.apiData = (void *) data;
  data->message.bytes.clear();  // needs to be empty for first input message

  if ( !InitializeCriticalSectionAndSpinCount(&(data->_mutex), 0x00000400) ) {
    errorString_ = "MidiInWinMM::initialize: InitializeCriticalSectionAndSpinCount failed.";
    error( RtMidiError::WARNING, errorString_ );
  }
}

void MidiInWinMM :: openPort( unsigned int portNumber, const std::string &/*portName*/ )
{
  if ( connected_ ) {
    errorString_ = "MidiInWinMM::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  unsigned int nDevices = midiInGetNumDevs();
  if (nDevices == 0) {
    errorString_ = "MidiInWinMM::openPort: no MIDI input sources found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

  if ( portNumber >= nDevices ) {
    std::ostringstream ost;
    ost << "MidiInWinMM::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  WinMidiData *data = static_cast<WinMidiData *> (apiData_);
  MMRESULT result = midiInOpen( &data->inHandle,
                                portNumber,
                                (DWORD_PTR)&midiInputCallback,
                                (DWORD_PTR)&inputData_,
                                CALLBACK_FUNCTION );
  if ( result != MMSYSERR_NOERROR ) {
    errorString_ = "MidiInWinMM::openPort: error creating Windows MM MIDI input port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Allocate and init the sysex buffers.
  for ( int i=0; i<RT_SYSEX_BUFFER_COUNT; ++i ) {
    data->sysexBuffer[i] = (MIDIHDR*) new char[ sizeof(MIDIHDR) ];
    data->sysexBuffer[i]->lpData = new char[ RT_SYSEX_BUFFER_SIZE ];
    data->sysexBuffer[i]->dwBufferLength = RT_SYSEX_BUFFER_SIZE;
    data->sysexBuffer[i]->dwUser = i; // We use the dwUser parameter as buffer indicator
    data->sysexBuffer[i]->dwFlags = 0;

    result = midiInPrepareHeader( data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR) );
    if ( result != MMSYSERR_NOERROR ) {
      midiInClose( data->inHandle );
      data->inHandle = 0;
      errorString_ = "MidiInWinMM::openPort: error starting Windows MM MIDI input port (PrepareHeader).";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }

    // Register the buffer.
    result = midiInAddBuffer( data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR) );
    if ( result != MMSYSERR_NOERROR ) {
      midiInClose( data->inHandle );
      data->inHandle = 0;
      errorString_ = "MidiInWinMM::openPort: error starting Windows MM MIDI input port (AddBuffer).";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }
  }

  result = midiInStart( data->inHandle );
  if ( result != MMSYSERR_NOERROR ) {
    midiInClose( data->inHandle );
    data->inHandle = 0;
    errorString_ = "MidiInWinMM::openPort: error starting Windows MM MIDI input port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  connected_ = true;
}

void MidiInWinMM :: openVirtualPort( const std::string &/*portName*/ )
{
  // This function cannot be implemented for the Windows MM MIDI API.
  errorString_ = "MidiInWinMM::openVirtualPort: cannot be implemented in Windows MM MIDI API!";
  error( RtMidiError::WARNING, errorString_ );
}

void MidiInWinMM :: closePort( void )
{
  if ( connected_ ) {
    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    EnterCriticalSection( &(data->_mutex) );
    midiInReset( data->inHandle );
    midiInStop( data->inHandle );

    for ( int i=0; i<RT_SYSEX_BUFFER_COUNT; ++i ) {
      int result = midiInUnprepareHeader(data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR));
      delete [] data->sysexBuffer[i]->lpData;
      delete [] data->sysexBuffer[i];
      if ( result != MMSYSERR_NOERROR ) {
        midiInClose( data->inHandle );
        data->inHandle = 0;
        errorString_ = "MidiInWinMM::openPort: error closing Windows MM MIDI input port (midiInUnprepareHeader).";
        error( RtMidiError::DRIVER_ERROR, errorString_ );
        return;
      }
    }

    midiInClose( data->inHandle );
    data->inHandle = 0;
    connected_ = false;
    LeaveCriticalSection( &(data->_mutex) );
  }
}

unsigned int MidiInWinMM :: getPortCount()
{
  return midiInGetNumDevs();
}

std::string MidiInWinMM :: getPortName( unsigned int portNumber )
{
  std::string stringName;
  unsigned int nDevices = midiInGetNumDevs();
  if ( portNumber >= nDevices ) {
    std::ostringstream ost;
    ost << "MidiInWinMM::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
    return stringName;
  }

  MIDIINCAPS deviceCaps;
  midiInGetDevCaps( portNumber, &deviceCaps, sizeof(MIDIINCAPS));
  stringName = ConvertToUTF8( deviceCaps.szPname );

  // Next lines added to add the portNumber to the name so that 
  // the device's names are sure to be listed with individual names
  // even when they have the same brand name
#ifdef RTMIDI_ENSURE_UNIQUE_PORTNAMES
  std::ostringstream os;
  os << " ";
  os << portNumber;
  stringName += os.str();
#endif

  return stringName;
}

//*********************************************************************//
//  API: Windows MM
//  Class Definitions: MidiOutWinMM
//*********************************************************************//

MidiOutWinMM :: MidiOutWinMM( const std::string &clientName ) : MidiOutApi()
{
  initialize( clientName );
}

MidiOutWinMM :: ~MidiOutWinMM()
{
  // Close a connection if it exists.
  closePort();

  // Cleanup.
  WinMidiData *data = static_cast<WinMidiData *> (apiData_);
  delete data;
}

void MidiOutWinMM :: initialize( const std::string& /*clientName*/ )
{
  // We'll issue a warning here if no devices are available but not
  // throw an error since the user can plug something in later.
  unsigned int nDevices = midiOutGetNumDevs();
  if ( nDevices == 0 ) {
    errorString_ = "MidiOutWinMM::initialize: no MIDI output devices currently available.";
    error( RtMidiError::WARNING, errorString_ );
  }

  // Save our api-specific connection information.
  WinMidiData *data = (WinMidiData *) new WinMidiData;
  apiData_ = (void *) data;
}

unsigned int MidiOutWinMM :: getPortCount()
{
  return midiOutGetNumDevs();
}

std::string MidiOutWinMM :: getPortName( unsigned int portNumber )
{
  std::string stringName;
  unsigned int nDevices = midiOutGetNumDevs();
  if ( portNumber >= nDevices ) {
    std::ostringstream ost;
    ost << "MidiOutWinMM::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
    return stringName;
  }

  MIDIOUTCAPS deviceCaps;
  midiOutGetDevCaps( portNumber, &deviceCaps, sizeof(MIDIOUTCAPS));
  stringName = ConvertToUTF8( deviceCaps.szPname );

  // Next lines added to add the portNumber to the name so that 
  // the device's names are sure to be listed with individual names
  // even when they have the same brand name
  std::ostringstream os;
#ifdef RTMIDI_ENSURE_UNIQUE_PORTNAMES
  os << " ";
  os << portNumber;
  stringName += os.str();
#endif

  return stringName;
}

void MidiOutWinMM :: openPort( unsigned int portNumber, const std::string &/*portName*/ )
{
  if ( connected_ ) {
    errorString_ = "MidiOutWinMM::openPort: a valid connection already exists!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  unsigned int nDevices = midiOutGetNumDevs();
  if (nDevices < 1) {
    errorString_ = "MidiOutWinMM::openPort: no MIDI output destinations found!";
    error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }

  if ( portNumber >= nDevices ) {
    std::ostringstream ost;
    ost << "MidiOutWinMM::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }

  WinMidiData *data = static_cast<WinMidiData *> (apiData_);
  MMRESULT result = midiOutOpen( &data->outHandle,
                                 portNumber,
                                 (DWORD)NULL,
                                 (DWORD)NULL,
                                 CALLBACK_NULL );
  if ( result != MMSYSERR_NOERROR ) {
    errorString_ = "MidiOutWinMM::openPort: error creating Windows MM MIDI output port.";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  connected_ = true;
}

void MidiOutWinMM :: closePort( void )
{
  if ( connected_ ) {
    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    midiOutReset( data->outHandle );
    midiOutClose( data->outHandle );
    data->outHandle = 0;
    connected_ = false;
  }
}

void MidiOutWinMM :: openVirtualPort( const std::string &/*portName*/ )
{
  // This function cannot be implemented for the Windows MM MIDI API.
  errorString_ = "MidiOutWinMM::openVirtualPort: cannot be implemented in Windows MM MIDI API!";
  error( RtMidiError::WARNING, errorString_ );
}

void MidiOutWinMM :: sendMessage( const unsigned char *message, size_t size )
{
  if ( !connected_ ) return;

  unsigned int nBytes = static_cast<unsigned int>(size);
  if ( nBytes == 0 ) {
    errorString_ = "MidiOutWinMM::sendMessage: message argument is empty!";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  MMRESULT result;
  WinMidiData *data = static_cast<WinMidiData *> (apiData_);
  if ( message[0] == 0xF0 ) { // Sysex message

    // Allocate buffer for sysex data.
    char *buffer = (char *) malloc( nBytes );
    if ( buffer == NULL ) {
      errorString_ = "MidiOutWinMM::sendMessage: error allocating sysex message memory!";
      error( RtMidiError::MEMORY_ERROR, errorString_ );
      return;
    }

    // Copy data to buffer.
    for ( unsigned int i=0; i<nBytes; ++i ) buffer[i] = message[i];

    // Create and prepare MIDIHDR structure.
    MIDIHDR sysex;
    sysex.lpData = (LPSTR) buffer;
    sysex.dwBufferLength = nBytes;
    sysex.dwFlags = 0;
    result = midiOutPrepareHeader( data->outHandle,  &sysex, sizeof(MIDIHDR) ); 
    if ( result != MMSYSERR_NOERROR ) {
      free( buffer );
      errorString_ = "MidiOutWinMM::sendMessage: error preparing sysex header.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }

    // Send the message.
    result = midiOutLongMsg( data->outHandle, &sysex, sizeof(MIDIHDR) );
    if ( result != MMSYSERR_NOERROR ) {
      free( buffer );
      errorString_ = "MidiOutWinMM::sendMessage: error sending sysex message.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
      return;
    }

    // Unprepare the buffer and MIDIHDR.
    while ( MIDIERR_STILLPLAYING == midiOutUnprepareHeader( data->outHandle, &sysex, sizeof (MIDIHDR) ) ) Sleep( 1 );
    free( buffer );
  }
  else { // Channel or system message.

    // Make sure the message size isn't too big.
    if ( nBytes > 3 ) {
      errorString_ = "MidiOutWinMM::sendMessage: message size is greater than 3 bytes (and not sysex)!";
      error( RtMidiError::WARNING, errorString_ );
      return;
    }

    // Pack MIDI bytes into double word.
    DWORD packet;
    unsigned char *ptr = (unsigned char *) &packet;
    for ( unsigned int i=0; i<nBytes; ++i ) {
      *ptr = message[i];
      ++ptr;
    }

    // Send the message immediately.
    result = midiOutShortMsg( data->outHandle, packet );
    if ( result != MMSYSERR_NOERROR ) {
      errorString_ = "MidiOutWinMM::sendMessage: error sending MIDI message.";
      error( RtMidiError::DRIVER_ERROR, errorString_ );
    }
  }
}

#endif  // __WINDOWS_MM__


//*********************************************************************//
//  API: UNIX JACK
//
//  Written primarily by Alexander Svetalkin, with updates for delta
//  time by Gary Scavone, April 2011.
//
//  *********************************************************************//

#if defined(__UNIX_JACK__)

// JACK header files
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>
#ifdef HAVE_SEMAPHORE
  #include <semaphore.h>
#endif

#define JACK_RINGBUFFER_SIZE 16384 // Default size for ringbuffer

struct JackMidiData {
  jack_client_t *client;
  jack_port_t *port;
  jack_ringbuffer_t *buffSize;
  jack_ringbuffer_t *buffMessage;
  jack_time_t lastTime;
#ifdef HAVE_SEMAPHORE
  sem_t sem_cleanup;
  sem_t sem_needpost;
#endif
  MidiInApi :: RtMidiInData *rtMidiIn;
  };

//*********************************************************************//
//  API: JACK
//  Class Definitions: MidiInJack
//*********************************************************************//

static int jackProcessIn( jack_nframes_t nframes, void *arg )
{
  JackMidiData *jData = (JackMidiData *) arg;
  MidiInApi :: RtMidiInData *rtData = jData->rtMidiIn;
  jack_midi_event_t event;
  jack_time_t time;

  // Is port created?
  if ( jData->port == NULL ) return 0;
  void *buff = jack_port_get_buffer( jData->port, nframes );

  // We have midi events in buffer
  int evCount = jack_midi_get_event_count( buff );
  for (int j = 0; j < evCount; j++) {
    MidiInApi::MidiMessage message;
    message.bytes.clear();

    jack_midi_event_get( &event, buff, j );

    for ( unsigned int i = 0; i < event.size; i++ )
      message.bytes.push_back( event.buffer[i] );

    // Compute the delta time.
    time = jack_get_time();
    if ( rtData->firstMessage == true )
      rtData->firstMessage = false;
    else
      message.timeStamp = ( time - jData->lastTime ) * 0.000001;

    jData->lastTime = time;

    if ( !rtData->continueSysex ) {
      if ( rtData->usingCallback ) {
        RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) rtData->userCallback;
        callback( message.timeStamp, &message.bytes, rtData->userData );
      }
      else {
        // As long as we haven't reached our queue size limit, push the message.
        if (!rtData->queue.push(message))
          std::cerr << "\nMidiInJack: message queue limit reached!!\n\n";
      }
    }
  }

  return 0;
}

MidiInJack :: MidiInJack( const std::string &clientName, unsigned int queueSizeLimit ) : MidiInApi( queueSizeLimit )
{
  initialize( clientName );
}

void MidiInJack :: initialize( const std::string& clientName )
{
  JackMidiData *data = new JackMidiData;
  apiData_ = (void *) data;

  data->rtMidiIn = &inputData_;
  data->port = NULL;
  data->client = NULL;
  this->clientName = clientName;

  connect();
}

void MidiInJack :: connect()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  if ( data->client )
    return;

  // Initialize JACK client
  if (( data->client = jack_client_open( clientName.c_str(), JackNoStartServer, NULL )) == 0) {
    errorString_ = "MidiInJack::initialize: JACK server not running?";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  jack_set_process_callback( data->client, jackProcessIn, data );
  jack_activate( data->client );
}

MidiInJack :: ~MidiInJack()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  closePort();

  if ( data->client )
    jack_client_close( data->client );
  delete data;
}

void MidiInJack :: openPort( unsigned int portNumber, const std::string &portName )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  connect();

  // Creating new port
  if ( data->port == NULL)
    data->port = jack_port_register( data->client, portName.c_str(),
                                     JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );

  if ( data->port == NULL) {
    errorString_ = "MidiInJack::openPort: JACK error creating port";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Connecting to the output
  std::string name = getPortName( portNumber );
  jack_connect( data->client, name.c_str(), jack_port_name( data->port ) );
}

void MidiInJack :: openVirtualPort( const std::string &portName )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  connect();
  if ( data->port == NULL )
    data->port = jack_port_register( data->client, portName.c_str(),
                                     JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );

  if ( data->port == NULL ) {
    errorString_ = "MidiInJack::openVirtualPort: JACK error creating virtual port";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
  }
}

unsigned int MidiInJack :: getPortCount()
{
  int count = 0;
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  connect();
  if ( !data->client )
    return 0;

  // List of available ports
  const char **ports = jack_get_ports( data->client, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput );

  if ( ports == NULL ) return 0;
  while ( ports[count] != NULL )
    count++;

  free( ports );

  return count;
}

std::string MidiInJack :: getPortName( unsigned int portNumber )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  std::string retStr("");

  connect();

  // List of available ports
  const char **ports = jack_get_ports( data->client, NULL,
                                       JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput );

  // Check port validity
  if ( ports == NULL ) {
    errorString_ = "MidiInJack::getPortName: no ports available!";
    error( RtMidiError::WARNING, errorString_ );
    return retStr;
  }

  if ( ports[portNumber] == NULL ) {
    std::ostringstream ost;
    ost << "MidiInJack::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
  }
  else retStr.assign( ports[portNumber] );

  free( ports );
  return retStr;
}

void MidiInJack :: closePort()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  if ( data->port == NULL ) return;
  jack_port_unregister( data->client, data->port );
  data->port = NULL;
}

//*********************************************************************//
//  API: JACK
//  Class Definitions: MidiOutJack
//*********************************************************************//

// Jack process callback
static int jackProcessOut( jack_nframes_t nframes, void *arg )
{
  JackMidiData *data = (JackMidiData *) arg;
  jack_midi_data_t *midiData;
  int space;

  // Is port created?
  if ( data->port == NULL ) return 0;

  void *buff = jack_port_get_buffer( data->port, nframes );
  jack_midi_clear_buffer( buff );

  while ( jack_ringbuffer_read_space( data->buffSize ) > 0 ) {
    jack_ringbuffer_read( data->buffSize, (char *) &space, (size_t) sizeof(space) );
    midiData = jack_midi_event_reserve( buff, 0, space );

    jack_ringbuffer_read( data->buffMessage, (char *) midiData, (size_t) space );
  }

#ifdef HAVE_SEMAPHORE
  if (!sem_trywait(&data->sem_needpost))
    sem_post(&data->sem_cleanup);
#endif

  return 0;
}

MidiOutJack :: MidiOutJack( const std::string &clientName ) : MidiOutApi()
{
  initialize( clientName );
}

void MidiOutJack :: initialize( const std::string& clientName )
{
  JackMidiData *data = new JackMidiData;
  apiData_ = (void *) data;

  data->port = NULL;
  data->client = NULL;
#ifdef HAVE_SEMAPHORE
  sem_init(&data->sem_cleanup, 0, 0);
  sem_init(&data->sem_needpost, 0, 0);
#endif
  this->clientName = clientName;

  connect();
}

void MidiOutJack :: connect()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  if ( data->client )
    return;
  
  // Initialize output ringbuffers  
  data->buffSize = jack_ringbuffer_create( JACK_RINGBUFFER_SIZE );
  data->buffMessage = jack_ringbuffer_create( JACK_RINGBUFFER_SIZE );

  // Initialize JACK client
  if (( data->client = jack_client_open( clientName.c_str(), JackNoStartServer, NULL )) == 0) {
    errorString_ = "MidiOutJack::initialize: JACK server not running?";
    error( RtMidiError::WARNING, errorString_ );
    return;
  }

  jack_set_process_callback( data->client, jackProcessOut, data );
  jack_activate( data->client );
}

MidiOutJack :: ~MidiOutJack()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  closePort();
  
  // Cleanup
  jack_ringbuffer_free( data->buffSize );
  jack_ringbuffer_free( data->buffMessage );
  if ( data->client ) {
    jack_client_close( data->client );
  }

#ifdef HAVE_SEMAPHORE
  sem_destroy(&data->sem_cleanup);
  sem_destroy(&data->sem_needpost);
#endif

  delete data;
}

void MidiOutJack :: openPort( unsigned int portNumber, const std::string &portName )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  connect();

  // Creating new port
  if ( data->port == NULL )
    data->port = jack_port_register( data->client, portName.c_str(),
      JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0 );

  if ( data->port == NULL ) {
    errorString_ = "MidiOutJack::openPort: JACK error creating port";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Connecting to the output
  std::string name = getPortName( portNumber );
  jack_connect( data->client, jack_port_name( data->port ), name.c_str() );
}

void MidiOutJack :: openVirtualPort( const std::string &portName )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  connect();
  if ( data->port == NULL )
    data->port = jack_port_register( data->client, portName.c_str(),
      JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0 );

  if ( data->port == NULL ) {
    errorString_ = "MidiOutJack::openVirtualPort: JACK error creating virtual port";
    error( RtMidiError::DRIVER_ERROR, errorString_ );
  }
}

unsigned int MidiOutJack :: getPortCount()
{
  int count = 0;
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  connect();
  if ( !data->client )
    return 0;

  // List of available ports
  const char **ports = jack_get_ports( data->client, NULL,
    JACK_DEFAULT_MIDI_TYPE, JackPortIsInput );

  if ( ports == NULL ) return 0;
  while ( ports[count] != NULL )
    count++;

  free( ports );

  return count;
}

std::string MidiOutJack :: getPortName( unsigned int portNumber )
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);
  std::string retStr("");

  connect();

  // List of available ports
  const char **ports = jack_get_ports( data->client, NULL,
    JACK_DEFAULT_MIDI_TYPE, JackPortIsInput );

  // Check port validity
  if ( ports == NULL) {
    errorString_ = "MidiOutJack::getPortName: no ports available!";
    error( RtMidiError::WARNING, errorString_ );
    return retStr;
  }

  if ( ports[portNumber] == NULL) {
    std::ostringstream ost;
    ost << "MidiOutJack::getPortName: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    error( RtMidiError::WARNING, errorString_ );
  }
  else retStr.assign( ports[portNumber] );

  free( ports );
  return retStr;
}

void MidiOutJack :: closePort()
{
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  if ( data->port == NULL ) return;

#ifdef HAVE_SEMAPHORE
  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) != -1)
  {
    ts.tv_sec += 1; // wait max one second
    sem_post(&data->sem_needpost);
    sem_timedwait(&data->sem_cleanup, &ts);
  }
#endif

  jack_port_unregister( data->client, data->port );
  data->port = NULL;
}

void MidiOutJack :: sendMessage( const unsigned char *message, size_t size )
{
  int nBytes = static_cast<int>(size);
  JackMidiData *data = static_cast<JackMidiData *> (apiData_);

  // Write full message to buffer
  jack_ringbuffer_write( data->buffMessage, ( const char * ) message,
                         nBytes );
  jack_ringbuffer_write( data->buffSize, ( char * ) &nBytes, sizeof( nBytes ) );
}

#endif  // __UNIX_JACK__
#endif
#endif

#if OSCCTRL
#if JUCE_DRIVER
/************************** BEGIN JuceOSCUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __juce_osc__
#define __juce_osc__
 
#include "../JuceLibraryCode/JuceHeader.h"


class oscItem : public uiItem {
    
    protected:
        
        OSCSender* fSender;
        String fPath;
        
    public:
        
        oscItem(OSCSender* sender, GUI* ui, const String& path, FAUSTFLOAT* zone)
        :uiItem(ui, zone), fSender(sender), fPath(path) {}
        virtual ~oscItem()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fSender->send(fPath, float(v));
        }
    
};

class JuceOSCUI : private OSCReceiver, private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>, public GUI {
    
    private:
        
        OSCSender fSender;
        String fIP;
        int fInputPort, fOutputPort;
        APIUI fAPIUI;
        Array<oscItem*> fOSCItems;  // Pointers are kept and desallocated by the GUI class
        
    public:
        
        JuceOSCUI(const std::string& ip, int in_port, int out_port)
        :fIP(ip), fInputPort(in_port), fOutputPort(out_port)
        {}
        
        virtual ~JuceOSCUI()
        {}
        
        void oscMessageReceived(const OSCMessage& message) override
        {
            String address = message.getAddressPattern().toString();
            
            for (int i = 0; i < message.size(); ++i) {
                if (message[i].isFloat32()) {
                    fAPIUI.setParamValue(fAPIUI.getParamIndex(address.toStdString().c_str()), FAUSTFLOAT(message[i].getFloat32()));
                    // "get" message with correct address
                } else if (message[i].isString()
                           && message[i].getString().equalsIgnoreCase("get")
                           && String(fAPIUI.getParamAddress(0)).startsWith(address)) {
                    for (int p = 0; p < fAPIUI.getParamsCount(); ++p) {
                        fSender.send(fAPIUI.getParamAddress(p), float(fAPIUI.getParamValue(p)), float(fAPIUI.getParamMin(p)), float(fAPIUI.getParamMax(p)));
                    }
                    // "hello" message
                } else if (message[i].isString()
                           && address.equalsIgnoreCase("/*")
                           && message[i].getString().equalsIgnoreCase("hello")) {
                    String path = fAPIUI.getParamAddress(0);
                    int pos1 = path.indexOfChar('/');
                    int pos2 = path.indexOfChar(pos1 + 1, '/');
                    fSender.send(path.substring(pos1, pos2), fIP, fInputPort, fOutputPort);
                }
            }
        }
        
        bool run() override
        {
            // Keep all zones for update when OSC messages are received
            if (fOSCItems.size() == 0) {
                for (int p = 0; p < fAPIUI.getParamsCount(); ++p) {
                    fOSCItems.add(new oscItem(&fSender, this, fAPIUI.getParamAddress(p), fAPIUI.getParamZone(p)));
                }
            }
            
            if (!fSender.connect(fIP, fOutputPort)) {
                std::cerr << "Error: could not connect to UDP port " << fInputPort << std::endl;
                return false;
            }
            
            if (!connect(fInputPort)) {
                std::cerr << "Error: could not connect to UDP port " << fOutputPort << std::endl;
                return false;
            }
            addListener(this);
            return true;
        }
        
        void stop() override
        {
            fSender.disconnect();
            disconnect();
            removeListener(this);
        }
        
        // -- widget's layouts
        
        void openTabBox(const char* label) override { fAPIUI.openTabBox(label); }
        void openHorizontalBox(const char* label) override { fAPIUI.openHorizontalBox(label); }
        void openVerticalBox(const char* label) override { fAPIUI.openVerticalBox(label); }
        void closeBox() override { fAPIUI.closeBox(); }
        
        // -- active widgets
        
        void addButton(const char* label, FAUSTFLOAT* zone) override { fAPIUI.addButton(label, zone); }
        void addCheckButton(const char* label, FAUSTFLOAT* zone) override { fAPIUI.addCheckButton(label, zone); }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addVerticalSlider(label, zone, init, min, max, step); }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addHorizontalSlider(label, zone, init, min, max, step); }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addNumEntry(label, zone, init, min, max, step); }
        
        // -- passive widgets
        
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override
        { fAPIUI.addHorizontalBargraph(label, zone, min, max); }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override
        { fAPIUI.addVerticalBargraph(label, zone, min, max); }
        
        // -- metadata declarations
        
        void declare(FAUSTFLOAT* zone, const char* key, const char* val) override { fAPIUI.declare(zone, key, val); }
    
};

#endif // __juce_osc__

/**************************  END  JuceOSCUI.h **************************/
#else
/************************** BEGIN OSCUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __OSCUI__
#define __OSCUI__

#include <vector>
#include <string>

/*

  Faust Project

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __OSCControler__
#define __OSCControler__

#include <string>
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __FaustFactory__
#define __FaustFactory__

#include <stack>
#include <string>
#include <sstream>

/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __FaustNode__
#define __FaustNode__

#include <string>
#include <vector>

/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __MessageDriven__
#define __MessageDriven__

#include <string>
#include <vector>

/*

  Copyright (C) 2010  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __MessageProcessor__
#define __MessageProcessor__

namespace oscfaust
{

class Message;
//--------------------------------------------------------------------------
/*!
	\brief an abstract class for objects able to process OSC messages	
*/
class MessageProcessor
{
	public:
		virtual		~MessageProcessor() {}
		virtual void processMessage( const Message* msg ) = 0;
};

} // end namespoace

#endif
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __smartpointer__
#define __smartpointer__

#include <cassert>

namespace oscfaust
{

/*!
\brief the base class for smart pointers implementation

	Any object that want to support smart pointers should
	inherit from the smartable class which provides reference counting
	and automatic delete when the reference count drops to zero.
*/
class smartable {
	private:
		unsigned 	refCount;		
	public:
		//! gives the reference count of the object
		unsigned refs() const         { return refCount; }
		//! addReference increments the ref count and checks for refCount overflow
		void addReference()           { refCount++; assert(refCount != 0); }
		//! removeReference delete the object when refCount is zero		
		void removeReference()		  { if (--refCount == 0) delete this; }
		
	protected:
		smartable() : refCount(0) {}
		smartable(const smartable&): refCount(0) {}
		//! destructor checks for non-zero refCount
		virtual ~smartable()    
        { 
            /* 
                See "Static SFaustNode create (const char* name, C* zone, C init, C min, C max, const char* prefix, GUI* ui)" comment.
                assert (refCount == 0); 
            */
        }
		smartable& operator=(const smartable&) { return *this; }
};

/*!
\brief the smart pointer implementation

	A smart pointer is in charge of maintaining the objects reference count 
	by the way of pointers operators overloading. It supports class 
	inheritance and conversion whenever possible.
\n	Instances of the SMARTP class are supposed to use \e smartable types (or at least
	objects that implements the \e addReference and \e removeReference
	methods in a consistent way).
*/
template<class T> class SMARTP {
	private:
		//! the actual pointer to the class
		T* fSmartPtr;

	public:
		//! an empty constructor - points to null
		SMARTP()	: fSmartPtr(0) {}
		//! build a smart pointer from a class pointer
		SMARTP(T* rawptr) : fSmartPtr(rawptr)              { if (fSmartPtr) fSmartPtr->addReference(); }
		//! build a smart pointer from an convertible class reference
		template<class T2> 
		SMARTP(const SMARTP<T2>& ptr) : fSmartPtr((T*)ptr) { if (fSmartPtr) fSmartPtr->addReference(); }
		//! build a smart pointer from another smart pointer reference
		SMARTP(const SMARTP& ptr) : fSmartPtr((T*)ptr)     { if (fSmartPtr) fSmartPtr->addReference(); }

		//! the smart pointer destructor: simply removes one reference count
		~SMARTP()  { if (fSmartPtr) fSmartPtr->removeReference(); }
		
		//! cast operator to retrieve the actual class pointer
		operator T*() const  { return fSmartPtr;	}

		//! '*' operator to access the actual class pointer
		T& operator*() const {
			// checks for null dereference
			assert (fSmartPtr != 0);
			return *fSmartPtr;
		}

		//! operator -> overloading to access the actual class pointer
		T* operator->() const	{ 
			// checks for null dereference
			assert (fSmartPtr != 0);
			return fSmartPtr;
		}

		//! operator = that moves the actual class pointer
		template <class T2>
		SMARTP& operator=(T2 p1_)	{ *this=(T*)p1_; return *this; }

		//! operator = that moves the actual class pointer
		SMARTP& operator=(T* p_)	{
			// check first that pointers differ
			if (fSmartPtr != p_) {
				// increments the ref count of the new pointer if not null
				if (p_ != 0) p_->addReference();
				// decrements the ref count of the old pointer if not null
				if (fSmartPtr != 0) fSmartPtr->removeReference();
				// and finally stores the new actual pointer
				fSmartPtr = p_;
			}
			return *this;
		}
		//! operator < to support SMARTP map with Visual C++
		bool operator<(const SMARTP<T>& p_)	const			  { return fSmartPtr < ((T *) p_); }
		//! operator = to support inherited class reference
		SMARTP& operator=(const SMARTP<T>& p_)                { return operator=((T *) p_); }
		//! dynamic cast support
		template<class T2> SMARTP& cast(T2* p_)               { return operator=(dynamic_cast<T*>(p_)); }
		//! dynamic cast support
		template<class T2> SMARTP& cast(const SMARTP<T2>& p_) { return operator=(dynamic_cast<T*>(p_)); }
};

}

#endif

namespace oscfaust
{

class Message;
class OSCRegexp;
class MessageDriven;
typedef class SMARTP<MessageDriven>	SMessageDriven;

//--------------------------------------------------------------------------
/*!
	\brief a base class for objects accepting OSC messages
	
	Message driven objects are hierarchically organized in a tree.
	They provides the necessary to dispatch an OSC message to its destination
	node, according to the message OSC address. 
	
	The principle of the dispatch is the following:
	- first the processMessage() method should be called on the top level node
	- next processMessage call propose 
*/
class MessageDriven : public MessageProcessor, public smartable
{
	std::string						fName;			///< the node name
	std::string						fOSCPrefix;		///< the node OSC address prefix (OSCAddress = fOSCPrefix + '/' + fName)
	std::vector<SMessageDriven>		fSubNodes;		///< the subnodes of the current node

	protected:
				 MessageDriven(const char *name, const char *oscprefix) : fName (name), fOSCPrefix(oscprefix) {}
		virtual ~MessageDriven() {}

	public:
		static SMessageDriven create(const char* name, const char *oscprefix)	{ return new MessageDriven(name, oscprefix); }

		/*!
			\brief OSC message processing method.
			\param msg the osc message to be processed
			The method should be called on the top level node.
		*/
		virtual void	processMessage(const Message* msg);

		/*!
			\brief propose an OSc message at a given hierarchy level.
			\param msg the osc message currently processed
			\param regexp a regular expression based on the osc address head
			\param addrTail the osc address tail
			
			The method first tries to match the regular expression with the object name. 
			When it matches:
			- it calls \c accept when \c addrTail is empty 
			- or it \c propose the message to its subnodes when \c addrTail is not empty. 
			  In this case a new \c regexp is computed with the head of \c addrTail and a new \c addrTail as well.
		*/
		virtual void	propose(const Message* msg, const OSCRegexp* regexp, const std::string& addrTail);

		/*!
			\brief accept an OSC message. 
			\param msg the osc message currently processed
			\return true when the message is processed by the node
			
			The method is called only for the destination nodes. The real message acceptance is the node 
			responsability and may depend on the message content.
		*/
		virtual bool	accept(const Message* msg);

		/*!
			\brief handler for the \c 'get' message
			\param ipdest the output message destination IP
			
			The \c 'get' message is supported by every node:
			- it is propagated to the subnodes until it reaches terminal nodes
			- a terminal node send its state on \c 'get' request to the IP address given as parameter.
			The \c get method is basically called by the accept method.
		*/
		virtual void	get(unsigned long ipdest) const;

		/*!
			\brief handler for the \c 'get' 'attribute' message
			\param ipdest the output message destination IP
			\param what the requested attribute
			
			The \c 'get' message is supported by every node:
			- it is propagated to the subnodes until it reaches terminal nodes
			- a terminal node send its state on \c 'get' request to the IP address given as parameter.
			The \c get method is basically called by the accept method.
		*/
		virtual void	get(unsigned long ipdest, const std::string& what) const {}

		void			add(SMessageDriven node)	{ fSubNodes.push_back (node); }
		const char*		getName() const				{ return fName.c_str(); }
		std::string		getOSCAddress() const;
		int				size() const				{ return (int)fSubNodes.size (); }
		
		const std::string&	name() const			{ return fName; }
		SMessageDriven	subnode(int i)              { return fSubNodes[i]; }
};

} // end namespoace

#endif
/*

  Copyright (C) 2011  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#ifndef __Message__
#define __Message__

#include <string>
#include <vector>

namespace oscfaust
{

class OSCStream;
template <typename T> class MsgParam;
class baseparam;
typedef SMARTP<baseparam>	Sbaseparam;

//--------------------------------------------------------------------------
/*!
	\brief base class of a message parameters
*/
class baseparam : public smartable
{
	public:
		virtual ~baseparam() {}

		/*!
		 \brief utility for parameter type checking
		*/
		template<typename X> bool isType() const { return dynamic_cast<const MsgParam<X>*> (this) != 0; }
		/*!
		 \brief utility for parameter convertion
		 \param errvalue the returned value when no conversion applies
		 \return the parameter value when the type matches
		*/
		template<typename X> X	value(X errvalue) const 
			{ const MsgParam<X>* o = dynamic_cast<const MsgParam<X>*> (this); return o ? o->getValue() : errvalue; }
		/*!
		 \brief utility for parameter comparison
		*/
		template<typename X> bool	equal(const baseparam& p) const 
			{ 
				const MsgParam<X>* a = dynamic_cast<const MsgParam<X>*> (this); 
				const MsgParam<X>* b = dynamic_cast<const MsgParam<X>*> (&p);
				return a && b && (a->getValue() == b->getValue());
			}
		/*!
		 \brief utility for parameter comparison
		*/
		bool operator==(const baseparam& p) const 
			{ 
				return equal<float>(p) || equal<int>(p) || equal<std::string>(p);
			}
		bool operator!=(const baseparam& p) const
			{ 
				return !equal<float>(p) && !equal<int>(p) && !equal<std::string>(p);
			}
			
		virtual SMARTP<baseparam> copy() const = 0;
};

//--------------------------------------------------------------------------
/*!
	\brief template for a message parameter
*/
template <typename T> class MsgParam : public baseparam
{
	T fParam;
	public:
				 MsgParam(T val) : fParam(val)	{}
		virtual ~MsgParam() {}
		
		T getValue() const { return fParam; }
		
		virtual Sbaseparam copy() const { return new MsgParam<T>(fParam); }
};

//--------------------------------------------------------------------------
/*!
	\brief a message description
	
	A message is composed of an address (actually an OSC address),
	a message string that may be viewed as a method name
	and a list of message parameters.
*/
class Message
{
    public:
        typedef SMARTP<baseparam>		argPtr;		///< a message argument ptr type
        typedef std::vector<argPtr>		argslist;	///< args list type

    private:
        unsigned long	fSrcIP;			///< the message source IP number
        std::string	fAddress;			///< the message osc destination address
        std::string	fAlias;             ///< the message alias osc destination address
        argslist	fArguments;			///< the message arguments

    public:
            /*!
                \brief an empty message constructor
            */
             Message() {}
            /*!
                \brief a message constructor
                \param address the message destination address
            */
            Message(const std::string& address) : fAddress(address), fAlias("") {}
             
            Message(const std::string& address, const std::string& alias) : fAddress(address), fAlias(alias) {}
            /*!
                \brief a message constructor
                \param address the message destination address
                \param args the message parameters
            */
            Message(const std::string& address, const argslist& args) 
                : fAddress(address), fArguments(args) {}
            /*!
                \brief a message constructor
                \param msg a message
            */
             Message(const Message& msg);
    virtual ~Message() {} //{ freed++; std::cout << "running messages: " << (allocated - freed) << std::endl; }

    /*!
        \brief adds a parameter to the message
        \param val the parameter
    */
    template <typename T> void add(T val)	{ fArguments.push_back(new MsgParam<T>(val)); }
    /*!
        \brief adds a float parameter to the message
        \param val the parameter value
    */
    void	add(float val)					{ add<float>(val); }
    
    /*!
     \brief adds a double parameter to the message
     \param val the parameter value
     */
    void	add(double val)					{ add<double>(val); }
    
    /*!
        \brief adds an int parameter to the message
        \param val the parameter value
    */
    void	add(int val)					{ add<int>(val); }
    
    /*!
        \brief adds a string parameter to the message
        \param val the parameter value
    */
    void	add(const std::string& val)		{ add<std::string>(val); }

    /*!
        \brief adds a parameter to the message
        \param val the parameter
    */
    void	add(argPtr val)                 { fArguments.push_back( val ); }

    /*!
        \brief sets the message address
        \param addr the address
    */
    void				setSrcIP(unsigned long addr)		{ fSrcIP = addr; }

    /*!
        \brief sets the message address
        \param addr the address
    */
    void				setAddress(const std::string& addr)		{ fAddress = addr; }
    /*!
        \brief print the message
        \param out the output stream
    */
    void				print(std::ostream& out) const;
    /*!
        \brief send the message to OSC
        \param out the OSC output stream
    */
    void				print(OSCStream& out) const;
    /*!
        \brief print message arguments
        \param out the OSC output stream
    */
    void				printArgs(OSCStream& out) const;

    /// \brief gives the message address
    const std::string&	address() const		{ return fAddress; }
    /// \brief gives the message alias
    const std::string&	alias() const		{ return fAlias; }
    /// \brief gives the message parameters list
    const argslist&		params() const		{ return fArguments; }
    /// \brief gives the message parameters list
    argslist&			params()			{ return fArguments; }
    /// \brief gives the message source IP 
    unsigned long		src() const			{ return fSrcIP; }
    /// \brief gives the message parameters count
    int					size() const		{ return (int)fArguments.size(); }

    bool operator == (const Message& other) const;	

    /*!
        \brief gives a message float parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, float& val) const		{ val = params()[i]->value<float>(val); return params()[i]->isType<float>(); }
    
    /*!
     \brief gives a message double parameter
     \param i the parameter index (0 <= i < size())
     \param val on output: the parameter value when the parameter type matches
     \return false when types don't match
     */
    bool	param(int i, double& val) const		{ val = params()[i]->value<double>(val); return params()[i]->isType<double>(); }
    
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, int& val) const		{ val = params()[i]->value<int>(val); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, unsigned int& val) const		{ val = params()[i]->value<int>(val); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
        \note a boolean value is handled as integer
    */
    bool	param(int i, bool& val) const		{ int ival = 0; ival = params()[i]->value<int>(ival); val = ival!=0; return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, long int& val) const	{ val = long(params()[i]->value<int>(val)); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message string parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, std::string& val) const { val = params()[i]->value<std::string>(val); return params()[i]->isType<std::string>(); }
};


} // end namespoace

#endif
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __RootNode__
#define __RootNode__

#include <map>
#include <string>
#include <vector>


namespace oscfaust
{

class OSCIO;
class RootNode;
typedef class SMARTP<RootNode> SRootNode;

/**
 * an alias target includes a map to rescale input values to output values
 * and a target osc address. The input values can be given in reversed order
 * to reverse the control
 */
struct aliastarget
{
	double      fMinIn;
	double      fMaxIn;
	double      fMinOut;
	double      fMaxOut;
	std::string fTarget;	// the real osc address

	aliastarget(const char* address, double imin, double imax, double omin, double omax)
		: fMinIn(imin), fMaxIn(imax), fMinOut(omin), fMaxOut(omax), fTarget(address) {}

	aliastarget(const aliastarget& t)
		: fMinIn(t.fMinIn), fMaxIn(t.fMaxIn), fMinOut(t.fMinOut), fMaxOut(t.fMaxOut), fTarget(t.fTarget) {}

	double scale(double x) const 
    {
        if (fMinIn < fMaxIn) {
            // increasing control
            double z = (x < fMinIn) ? fMinIn : (x > fMaxIn) ? fMaxIn : x;
            return fMinOut + (z-fMinIn)*(fMaxOut-fMinOut)/(fMaxIn-fMinIn);
            
        } else if (fMinIn > fMaxIn) {
            // reversed control
            double z = (x < fMaxIn) ? fMaxIn : (x > fMinIn) ? fMinIn : x;
            return fMinOut + (fMinIn-z)*(fMaxOut-fMinOut)/(fMinIn-fMaxIn);
            
        } else {
            // no control !
            return (fMinOut+fMaxOut)/2.0f;
        }
    }
    
    double invscale(double x) const
    {
        if (fMinOut < fMaxOut) {
            // increasing control
            double z = (x < fMinOut) ? fMinOut : (x > fMaxOut) ? fMaxOut : x;
            return fMinIn + (z-fMinOut)*(fMaxIn-fMinIn)/(fMaxOut-fMinOut);
            
        } else if (fMinOut > fMaxOut) {
            // reversed control
            double z = (x < fMaxOut) ? fMaxOut : (x > fMinOut) ? fMinOut : x;
            return fMinIn + (fMinOut-z)*(fMaxIn-fMinIn)/(fMinOut-fMaxOut);
            
        } else {
            // no control !
            return (fMinIn+fMaxIn)/2.0f;
        }
    }
};

//--------------------------------------------------------------------------
/*!
	\brief a faust root node

	A Faust root node handles the \c 'hello' message and provides support
	for incoming osc signal data. 
*/
class RootNode : public MessageDriven
{

    private:
        int *fUPDIn, *fUDPOut, *fUDPErr;    // the osc port numbers (required by the hello method)
        OSCIO* fIO;                         // an OSC IO controler
        JSONUI* fJSON;

        typedef std::map<std::string, std::vector<aliastarget> > TAliasMap;
        TAliasMap fAliases;

        template <typename T>
        void processAliasAux(const std::string& address, T val);
        void processAlias(const std::string& address, float val);
        void processAlias(const std::string& address, double val);
    
        void eraseAliases(const std::string& target);
        void eraseAlias(const std::string& target, const std::string& alias);
        bool aliasError(const Message* msg);

    protected:
        RootNode(const char *name, JSONUI* json, OSCIO* io = NULL) : MessageDriven(name, ""), fUPDIn(0), fUDPOut(0), fUDPErr(0), fIO(io), fJSON(json) {}
        virtual ~RootNode() {}

    public:
        static SRootNode create(const char* name, JSONUI* json, OSCIO* io = NULL) { return new RootNode(name, json, io); }

        virtual void processMessage(const Message* msg);
        virtual bool accept(const Message* msg);
        virtual void get(unsigned long ipdest) const;
        virtual void get(unsigned long ipdest, const std::string& what) const;

        template <typename T>
        bool aliasMsgAux(const Message* msg, T omin, T omax);
        bool aliasMsg(const Message* msg, double omin, double omax);
        bool aliasMsg(const Message* msg, float omin, float omax);
    
        template <typename T>
        void addAliasAux(const char* alias, const char* address, T imin, T imax, T omin, T omax);
        void addAlias(const char* alias, const char* address, float imin, float imax, float omin, float omax);
        void addAlias(const char* alias, const char* address, double imin, double imax, double omin, double omax);
    
        bool acceptSignal(const Message* msg);      ///< handler for signal data
        void hello(unsigned long ipdest) const;     ///< handler for the 'hello' message
        void setPorts(int* in, int* out, int* err);

        std::vector<std::pair<std::string, double> > getAliases(const std::string& address, double value);
};

} // end namespoace

#endif

namespace oscfaust
{

/**
 * map (rescale) input values to output values
 */
template <typename C> struct mapping
{
	const C fMinOut;
	const C fMaxOut;
	mapping(C omin, C omax) : fMinOut(omin), fMaxOut(omax) {}
	C clip (C x) { return (x < fMinOut) ? fMinOut : (x > fMaxOut) ? fMaxOut : x; }
};

//--------------------------------------------------------------------------
/*!
	\brief a faust node is a terminal node and represents a faust parameter controler
*/
template <typename C> class FaustNode : public MessageDriven, public uiTypedItem<C>
{
	mapping<C>	fMapping;
    RootNode*   fRoot;
    bool        fInput;  // true for input nodes (slider, button...)
	
	//---------------------------------------------------------------------
	// Warning !!!
	// The cast (C*)fZone is necessary because the real size allocated is
	// only known at execution time. When the library is compiled, fZone is
	// uniquely defined by FAUSTFLOAT.
	//---------------------------------------------------------------------
	bool store(C val) { *(C*)this->fZone = fMapping.clip(val); return true; }
	void sendOSC() const;

	protected:
		FaustNode(RootNode* root, const char *name, C* zone, C init, C min, C max, const char* prefix, GUI* ui, bool initZone, bool input) 
			: MessageDriven(name, prefix), uiTypedItem<C>(ui, zone), fMapping(min, max), fRoot(root), fInput(input)
			{
                if (initZone) {
                    *zone = init; 
                }
            }
			
		virtual ~FaustNode() {}

	public:
		typedef SMARTP<FaustNode<C> > SFaustNode;
		static SFaustNode create(RootNode* root, const char* name, C* zone, C init, C min, C max, const char* prefix, GUI* ui, bool initZone, bool input)	
        { 
            SFaustNode node = new FaustNode(root, name, zone, init, min, max, prefix, ui, initZone, input); 
            /*
                Since FaustNode is a subclass of uiItem, the pointer will also be kept in the GUI class, and it's desallocation will be done there.
                So we don't want to have smartpointer logic desallocate it and we increment the refcount.
            */
            node->addReference();
            return node; 
        }
    
		bool accept(const Message* msg);
		void get(unsigned long ipdest) const;		///< handler for the 'get' message
		virtual void reflectZone() { sendOSC(); this->fCache = *this->fZone; }
};

} // end namespace

#endif

class GUI;
namespace oscfaust
{

class OSCIO;
class RootNode;
typedef class SMARTP<RootNode> SRootNode;
class MessageDriven;
typedef class SMARTP<MessageDriven>	SMessageDriven;

//--------------------------------------------------------------------------
/*!
	\brief a factory to build a OSC UI hierarchy
	
	Actually, makes use of a stack to build the UI hierarchy.
	It includes a pointer to a OSCIO controler, but just to give it to the root node.
*/
class FaustFactory
{
    std::stack<SMessageDriven>  fNodes;		///< maintains the current hierarchy level
    SRootNode  fRoot;   ///< keep track of the root node
    OSCIO*     fIO;     ///< hack to support audio IO via OSC, actually the field is given to the root node
    GUI*       fGUI;    ///< a GUI pointer to support updateAllGuis(), required for bi-directionnal OSC
    JSONUI*    fJSON;
    
    private:
        std::string addressFirst(const std::string& address) const;
        std::string addressTail(const std::string& address) const;
        
    public:
        FaustFactory(GUI* ui, JSONUI* json, OSCIO * io = NULL);
        virtual ~FaustFactory();
        
        template <typename C> void addnode(const char* label, C* zone, C init, C min, C max, bool initZone, bool input);
        template <typename C> void addAlias(const std::string& fullpath, C* zone, C imin, C imax, C init, C min, C max, const char* label);
        
        void addAlias(const char* alias, const char* address, float imin, float imax, float omin, float omax);
        void addAlias(const char* alias, const char* address, double imin, double imax, double omin, double omax);
        void opengroup(const char* label);
        void closegroup();
        
        SRootNode root() const; 
};

/**
 * Add a node to the OSC UI tree in the current group at the top of the stack 
 */
template <typename C> void FaustFactory::addnode(const char* label, C* zone, C init, C min, C max, bool initZone, bool input) 
{
	SMessageDriven top;
	if (fNodes.size()) top = fNodes.top();
	if (top) {
		std::string prefix = top->getOSCAddress();
		top->add(FaustNode<C>::create(root(), label, zone, init, min, max, prefix.c_str(), fGUI, initZone, input));
	}
}

/**
 * Add an alias (actually stored and handled at root node level
 */
template <typename C> void FaustFactory::addAlias(const std::string& fullpath, C* zone, C imin, C imax, C init, C min, C max, const char* label)
{
	std::istringstream 	ss(fullpath);
	std::string 		realpath; 
 
	ss >> realpath >> imin >> imax;
	SMessageDriven top = fNodes.top();
	if (top) {
		std::string target = top->getOSCAddress() + "/" + label;
		addAlias(realpath.c_str(), target.c_str(), C(imin), C(imax), C(min), C(max));
	}
}

} // end namespoace

#endif

class GUI;

typedef void (*ErrorCallback)(void*);  

namespace oscfaust
{

class OSCIO;
class OSCSetup;
class OSCRegexp;
    
//--------------------------------------------------------------------------
/*!
	\brief the main Faust OSC Lib API
	
	The OSCControler is essentially a glue between the memory representation (in charge of the FaustFactory),
	and the network services (in charge of OSCSetup).
*/
class OSCControler
{
	int fUDPPort,   fUDPOut, fUPDErr;	// the udp ports numbers
	std::string     fDestAddress;		// the osc messages destination address, used at initialization only
										// to collect the address from the command line
	std::string     fBindAddress;		// when non empty, the address used to bind the socket for listening
	OSCSetup*		fOsc;				// the network manager (handles the udp sockets)
	OSCIO*			fIO;				// hack for OSC IO support (actually only relayed to the factory)
	FaustFactory*	fFactory;			// a factory to build the memory representation

    bool            fInit;
    
	public:
		/*
			base udp port is chosen in an unassigned range from IANA PORT NUMBERS (last updated 2011-01-24)
			see at http://www.iana.org/assignments/port-numbers
			5507-5552  Unassigned
		*/
		enum { kUDPBasePort = 5510 };
            
        OSCControler(int argc, char* argv[], GUI* ui, JSONUI* json, OSCIO* io = NULL, ErrorCallback errCallback = NULL, void* arg = NULL, bool init = true);

        virtual ~OSCControler();
	
		//--------------------------------------------------------------------------
		// addnode, opengroup and closegroup are simply relayed to the factory
		//--------------------------------------------------------------------------
		// Add a node in the current group (top of the group stack)
		template <typename T> void addnode(const char* label, T* zone, T init, T min, T max, bool input = true)
							{ fFactory->addnode(label, zone, init, min, max, fInit, input); }
		
		//--------------------------------------------------------------------------
		// This method is used for alias messages. The arguments imin and imax allow
		// to map incomming values from the alias input range to the actual range 
		template <typename T> void addAlias(const std::string& fullpath, T* zone, T imin, T imax, T init, T min, T max, const char* label)
							{ fFactory->addAlias(fullpath, zone, imin, imax, init, min, max, label); }

		void opengroup(const char* label)		{ fFactory->opengroup(label); }
		void closegroup()						{ fFactory->closegroup(); }
	   
		//--------------------------------------------------------------------------
		void run();				// starts the network services
		void endBundle();		// when bundle mode is on, close and send the current bundle (if any)
		void stop();			// stop the network services
		std::string getInfos() const; // gives information about the current environment (version, port numbers,...)

		int	getUDPPort() const			{ return fUDPPort; }
		int	getUDPOut()	const			{ return fUDPOut; }
		int	getUDPErr()	const			{ return fUPDErr; }
		const char*	getDestAddress() const { return fDestAddress.c_str(); }
		const char*	getRootName() const;	// probably useless, introduced for UI extension experiments
    
        void setUDPPort(int port) { fUDPPort = port; }
        void setUDPOut(int port) { fUDPOut = port; }
        void setUDPErr(int port) { fUPDErr = port; }
        void setDestAddress(const char* address) { fDestAddress = address; }

        // By default, an osc interface emits all parameters. You can filter specific params dynamically.
        static std::vector<OSCRegexp*>     fFilteredPaths; // filtered paths will not be emitted
        static void addFilteredPath(std::string path);
        static bool isPathFiltered(std::string path);
        static void resetFilteredPaths();
    
		static float version();				// the Faust OSC library version number
		static const char* versionstr();	// the Faust OSC library version number as a string
		static int gXmit;                   // a static variable to control the transmission of values
                                            // i.e. the use of the interface as a controler
		static int gBundle;                 // a static variable to control the osc bundle mode
};

#define kNoXmit     0
#define kAll        1
#define kAlias      2

}

#endif

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

/******************************************************************************
 *******************************************************************************
 
 OSC (Open Sound Control) USER INTERFACE
 
 *******************************************************************************
 *******************************************************************************/
/*
 
 Note about the OSC addresses and the Faust UI names:
 ----------------------------------------------------
 There are potential conflicts between the Faust UI objects naming scheme and
 the OSC address space. An OSC symbolic names is an ASCII string consisting of
 printable characters other than the following:
	space
 #	number sign
 *	asterisk
 ,	comma
 /	forward
 ?	question mark
 [	open bracket
 ]	close bracket
 {	open curly brace
 }	close curly brace
 
 a simple solution to address the problem consists in replacing
 space or tabulation with '_' (underscore)
 all the other osc excluded characters with '-' (hyphen)
 
 This solution is implemented in the proposed OSC UI;
 */

class OSCUI : public GUI
{
    
    private:
        
        oscfaust::OSCControler*	fCtrl;
        std::vector<const char*> fAlias;
        JSONUI fJSON;
        
        const char* tr(const char* label) const
        {
            static char buffer[1024];
            char * ptr = buffer; int n=1;
            while (*label && (n++ < 1024)) {
                switch (*label) {
                    case ' ': case '	':
                        *ptr++ = '_';
                        break;
                    case '#': case '*': case ',': case '/': case '?':
                    case '[': case ']': case '{': case '}': case '(': case ')':
                        *ptr++ = '_';
                        break;
                    default:
                        *ptr++ = *label;
                }
                label++;
            }
            *ptr = 0;
            return buffer;
        }
        
        // add all accumulated alias
        void addalias(FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, const char* label)
        {
            for (unsigned int i = 0; i < fAlias.size(); i++) {
                fCtrl->addAlias(fAlias[i], zone, FAUSTFLOAT(0), FAUSTFLOAT(1), init, min, max, label);
            }
            fAlias.clear();
        }
        
    public:
        
        OSCUI(const char* /*applicationname*/, int argc, char* argv[], oscfaust::OSCIO* io = NULL, ErrorCallback errCallback = NULL, void* arg = NULL, bool init = true) : GUI()
        {
            fCtrl = new oscfaust::OSCControler(argc, argv, this, &fJSON, io, errCallback, arg, init);
            //		fCtrl->opengroup(applicationname);
        }
        
        virtual ~OSCUI() { delete fCtrl; }
        
        // -- widget's layouts
        
        virtual void openTabBox(const char* label)          { fCtrl->opengroup(tr(label)); fJSON.openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fCtrl->opengroup(tr(label)); fJSON.openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fCtrl->opengroup(tr(label)); fJSON.openVerticalBox(label); }
        virtual void closeBox()                             { fCtrl->closegroup(); fJSON.closeBox(); }
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            const char* l = tr(label);
            addalias(zone, 0, 0, 1, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), FAUSTFLOAT(0), FAUSTFLOAT(1));
            fJSON.addButton(label, zone);
        }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            const char* l = tr(label);
            addalias(zone, 0, 0, 1, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), FAUSTFLOAT(0), FAUSTFLOAT(1));
            fJSON.addCheckButton(label, zone);
        }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addVerticalSlider(label, zone, init, min, max, step);
        }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addHorizontalSlider(label, zone, init, min, max, step);
        }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addNumEntry(label, zone, init, min, max, step);
        }
        
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            const char* l = tr(label);
            addalias(zone, 0, min, max, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), min, max, false);
            fJSON.addHorizontalBargraph(label, zone, min, max);
        }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            const char* l = tr(label);
            addalias(zone, 0, min, max, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), min, max, false);
            fJSON.addVerticalBargraph(label, zone, min, max);
        }
            
        // -- metadata declarations
        
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* alias)
        {
            if (strcasecmp(key, "OSC") == 0) fAlias.push_back(alias);
            fJSON.declare(zone, key, alias);
        }
        
        virtual void show() {}
        
        bool run()
        {
            fCtrl->run();
            return true;
        }
        
        void stop()			{ fCtrl->stop(); }
        void endBundle() 	{ fCtrl->endBundle(); }
        
        std::string getInfos()          { return fCtrl->getInfos(); }
        
        const char* getRootName()		{ return fCtrl->getRootName(); }
        int getUDPPort()                { return fCtrl->getUDPPort(); }
        int getUDPOut()                 { return fCtrl->getUDPOut(); }
        int getUDPErr()                 { return fCtrl->getUDPErr(); }
        const char* getDestAddress()    { return fCtrl->getDestAddress(); }
        
        void setUDPPort(int port)       { fCtrl->setUDPPort(port); }
        void setUDPOut(int port)        { fCtrl->setUDPOut(port); }
        void setUDPErr(int port)        { fCtrl->setUDPErr(port); }
        void setDestAddress(const char* address)    { return fCtrl->setDestAddress(address); }
    
};

#endif // __OSCUI__
/**************************  END  OSCUI.h **************************/
#endif
static void osc_compute_callback(void* arg)
{
    static_cast<OSCUI*>(arg)->endBundle();
}
#endif

#if DYNAMIC_DSP
/************************** BEGIN llvm-dsp.h **************************/
/************************************************************************
 ************************************************************************
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 ************************************************************************
 ************************************************************************/

#ifndef LLVM_DSP_H
#define LLVM_DSP_H

#ifdef _WIN32
#define DEPRECATED(fun) __declspec(deprecated) fun
#else
#define DEPRECATED(fun) fun __attribute__ ((deprecated));
#endif

#include <string>
#include <vector>


/*!
 \addtogroup llvmcpp C++ interface for compiling Faust code. Note that the API is not thread safe : use 'startMTDSPFactories/stopMTDSPFactories' to use it in a multi-thread context.
 @{
 */
 
/**
 * Get the library version.
 * 
 * @return the library version as a static string.
 */
extern "C" const char* getCLibFaustVersion();

/**
 * DSP instance class with methods.
 */
class llvm_dsp : public dsp {
    
    private:
    
        // llvm_dsp objects are allocated using llvm_dsp_factory::createDSPInstance();
        llvm_dsp() {}
    
    public:
        
        int getNumInputs();
       
        int getNumOutputs();
        
        void buildUserInterface(UI* ui_interface);
       
        int getSampleRate();
        
        void init(int sample_rate);
       
        void instanceInit(int sample_rate);
    
        void instanceConstants(int sample_rate);
    
        void instanceResetUserInterface();
        
        void instanceClear();
        
        llvm_dsp* clone();
        
        void metadata(Meta* m);
        
        void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs);
    
};

/**
 * DSP factory class.
 */

class llvm_dsp_factory : public dsp_factory {

     public:
    
        virtual ~llvm_dsp_factory();
        
        /**
         *  Return factory name:
         *  either the name declared in DSP with [declare name "foo"] syntax
         *  or 'filename' (if createDSPFactoryFromFile is used)
         *  or 'name_app' (if createDSPFactoryFromString is used)
        */
        std::string getName();
    
        /* Return factory LLVM target (like 'i386-apple-macosx10.6.0:opteron')*/
        std::string getTarget();
        
        /* Return factory SHA key */
        std::string getSHAKey();
        
        /* Return factory expanded DSP code */
        std::string getDSPCode();
        
        /* Return factory compile options */
        std::string getCompileOptions();
        
        /* Get the Faust DSP factory list of library dependancies */
        std::vector<std::string> getLibraryList();
        
        /* Get the list of all used includes */
        std::vector<std::string> getIncludePathnames();
        
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        llvm_dsp* createDSPInstance();
        
        /* Set a custom memory manager to be used when creating instances */
        void setMemoryManager(dsp_memory_manager* manager);
        
        /* Return the currently set custom memory manager */
        dsp_memory_manager* getMemoryManager();

};

/**
 * Get the target (triple + CPU) of the machine.
 *
 * @return the target as a string.
 */
std::string getDSPMachineTarget();

/**
 * Get the Faust DSP factory associated with a given SHA key (created from the 'expanded' DSP source), 
 * if already allocated in the factories cache and increment it's reference counter. You will have to explicitly
 * use deleteDSPFactory to properly decrement reference counter when the factory is no more needed.
 *
 * @param sha_key - the SHA key for an already created factory, kept in the factory cache
 *
 * @return a DSP factory if one is associated with the SHA key, otherwise a null pointer.
 */
llvm_dsp_factory* getDSPFactoryFromSHAKey(const std::string& sha_key);

/**
 * Create a Faust DSP factory from a DSP source code as a file. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is same source code and 
 * same set of 'normalized' compilations options) will return the same (reference counted) factory pointer. You will have to explicitly
 * use deleteDSPFactory to properly decrement the reference counter when the factory is no more needed.
 * 
 * @param filename - the DSP filename
 * @param argc - the number of parameters in argv array 
 * @param argv - the array of parameters (Warning : aux files generation options will be filtered (-svg, ...) --> use generateAuxFiles)
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value' 
 * since the maximum value may change with new LLVM versions)
 *
 * @return a DSP factory on success, otherwise a null pointer.
 */ 
llvm_dsp_factory* createDSPFactoryFromFile(const std::string& filename, 
                                           int argc, const char* argv[], 
                                           const std::string& target, 
                                           std::string& error_msg,
                                           int opt_level = -1);

/**
 * Create a Faust DSP factory from a DSP source code as a string. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is same source code and 
 * same set of 'normalized' compilations options) will return the same (reference counted) factory pointer. You will have to explicitly
 * use deleteDSPFactory to properly decrement reference counter when the factory is no more needed.
 * 
 * @param name_app - the name of the Faust program
 * @param dsp_content - the Faust program as a string
 * @param argc - the number of parameters in argv array
 * @param argv - the array of parameters (Warning : aux files generation options will be filtered (-svg, ...) --> use generateAuxFiles)
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value' 
 * since the maximum value may change with new LLVM versions)
 *
 * @return a DSP factory on success, otherwise a null pointer.
 */ 
llvm_dsp_factory* createDSPFactoryFromString(const std::string& name_app,
                                             const std::string& dsp_content,
                                             int argc, const char* argv[], 
                                             const std::string& target, 
                                             std::string& error_msg,
                                             int opt_level = -1);

/**
 * Delete a Faust DSP factory, that is decrements it's reference counter, possibly really deleting the internal pointer. 
 * Possibly also delete DSP pointers associated with this factory, if they were not explicitly deleted with C++ delete.
 * Beware : all kept factories and DSP pointers (in local variables...) thus become invalid.
 * 
 * @param factory - the DSP factory
 *
 * @return true if the factory internal pointer was really deleted, and false if only 'decremented'.
 */                                 
bool deleteDSPFactory(llvm_dsp_factory* factory);

/**
 * Get the Faust DSP factory list of library dependancies.
 *
 * @deprecated : use factory getDSPFactoryLibraryList method.
 *
 * @param factory - the DSP factory
 * 
 * @return the list as a vector of strings.
 */
DEPRECATED(std::vector<std::string> getDSPFactoryLibraryList(llvm_dsp_factory* factory));

/**
 * Delete all Faust DSP factories kept in the library cache. Beware : all kept factory and DSP pointers (in local variables...) thus become invalid.
 * 
 */                                 
void deleteAllDSPFactories();

/**
 * Return Faust DSP factories of the library cache as a vector of their SHA keys.
 * 
 * @return the Faust DSP factories.
 */                                 
std::vector<std::string> getAllDSPFactories();

/**
 * Start multi-thread access mode (since by default the library is not 'multi-thread' safe).
 * 
 * @return true if 'multi-thread' safe access is started.
 */ 
extern "C" bool startMTDSPFactories();

/**
 * Stop multi-thread access mode.
 * 
 */ 
extern "C" void stopMTDSPFactories();

/**
 * Create a Faust DSP factory from a base64 encoded LLVM bitcode string. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same LLVM bitcode string) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param bit_code - the LLVM bitcode string
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value'
 * since the maximum value may change with new LLVM versions). A higher value than the one used when calling createDSPFactory can possibly be used.
 *
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromBitcode(const std::string& bit_code, const std::string& target, std::string& error_msg, int opt_level = -1);

/**
 * Write a Faust DSP factory into a base64 encoded LLVM bitcode string.
 * 
 * @param factory - the DSP factory
 *
 * @return the LLVM bitcode as a string.
 */
std::string writeDSPFactoryToBitcode(llvm_dsp_factory* factory);

/**
 * Create a Faust DSP factory from a LLVM bitcode file. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same LLVM bitcode file) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param bit_code_path - the LLVM bitcode file pathname
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value'
 * since the maximum value may change with new LLVM versions). A higher value than the one used when calling 
 * createDSPFactory can possibly be used.
 * 
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromBitcodeFile(const std::string& bit_code_path, const std::string& target, std::string& error_msg, int opt_level = -1);

/**
 * Write a Faust DSP factory into a LLVM bitcode file.
 * 
 * @param factory - the DSP factory
 * @param bit_code_path - the LLVM bitcode file pathname.
 *
 * @return true on success, false on failure.
 */
bool writeDSPFactoryToBitcodeFile(llvm_dsp_factory* factory, const std::string& bit_code_path);

/**
 * Create a Faust DSP factory from a LLVM IR (textual) string. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same LLVM IR string) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param ir_code - the LLVM IR (textual) string
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value'
 * since the maximum value may change with new LLVM versions). A higher value than the one used when calling 
 * createDSPFactory can possibly be used.
 *
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromIR(const std::string& ir_code, const std::string& target, std::string& error_msg, int opt_level = -1);

/**
 * Write a Faust DSP factory into a LLVM IR (textual) string.
 * 
 * @param factory - the DSP factory
 *
 * @return the LLVM IR (textual) as a string.
 */
std::string writeDSPFactoryToIR(llvm_dsp_factory* factory);

/**
 * Create a Faust DSP factory from a LLVM IR (textual) file. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same LLVM IR file) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param ir_code_path - the LLVM IR (textual) file pathname
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 * @param opt_level - LLVM IR to IR optimization level (from -1 to 4, -1 means 'maximum possible value'
 * since the maximum value may change with new LLVM versions), a higher value 
 * than the one used when calling createDSPFactory can possibly be used.
 *
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromIRFile(const std::string& ir_code_path, const std::string& target, std::string& error_msg, int opt_level = -1);

/**
 * Write a Faust DSP factory into a LLVM IR (textual) file.
 * 
 * @param factory - the DSP factory
 * @param ir_code_path - the LLVM bitcode file pathname.
 *
 * @return true on success, false on failure.
 */
bool writeDSPFactoryToIRFile(llvm_dsp_factory* factory, const std::string& ir_code_path);

/**
 * Create a Faust DSP factory from a base64 encoded machine code string. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same machine code string) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param machine_code - the machine code string
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 *
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromMachine(const std::string& machine_code, const std::string& target, std::string& error_msg);

/**
 * Write a Faust DSP factory into a base64 encoded machine code string.
 * 
 * @param factory - the DSP factory
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 *
 * @return the machine code as a string.
 */
std::string writeDSPFactoryToMachine(llvm_dsp_factory* factory, const std::string& target);

/**
 * Create a Faust DSP factory from a machine code file. Note that the library keeps an internal cache of all 
 * allocated factories so that the compilation of the same DSP code (that is the same machine code file) will return 
 * the same (reference counted) factory pointer. You will have to explicitly use deleteDSPFactory to properly 
 * decrement reference counter when the factory is no more needed.
 * 
 * @param machine_code_path - the machine code file pathname
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 * @param error_msg - the error string to be filled
 *
 * @return the DSP factory on success, otherwise a null pointer.
 */
llvm_dsp_factory* readDSPFactoryFromMachineFile(const std::string& machine_code_path, const std::string& target, std::string& error_msg);

/**
 * Write a Faust DSP factory into a machine code file.
 * 
 * @param factory - the DSP factory
 * @param machine_code_path - the machine code file pathname
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 *
 * @return true on success, false on failure.
 */
bool writeDSPFactoryToMachineFile(llvm_dsp_factory* factory, const std::string& machine_code_path, const std::string& target);

/**
 * Write a Faust DSP factory into a object code file.
 *
 * @param factory - the DSP factory
 * @param target - the LLVM machine target: like 'i386-apple-macosx10.6.0:opteron',
 *                 using an empty string takes the current machine settings,
 *                 and i386-apple-macosx10.6.0:generic kind of syntax for a generic processor
 *
 * @return true on success, false on failure.
 */
bool writeDSPFactoryToObjectcodeFile(llvm_dsp_factory* factory, const std::string& object_code_path, const std::string& target);

/**
 * Call global declarations with the given meta object.
 * 
 * @deprecated : use DSP instance metadata method.
 * @param factory - the DSP factory
 * @param meta - the meta object to be used.
 *
 */
DEPRECATED(void metadataDSPFactory(llvm_dsp_factory* factory, Meta* meta));

/**
 * Create a Faust DSP instance.
 * 
 * @deprecated : use factory createDSPInstance method.
 * @param factory - the DSP factory
 * 
 * @return the DSP instance on success, otherwise a null pointer.
 */
DEPRECATED(llvm_dsp* createDSPInstance(llvm_dsp_factory* factory));

/**
 * Delete a Faust DSP instance. You can also simply use C++ 'delete'. 
 * 
 * @deprecated : simply use C++ delete.
 * @param dsp - the DSP instance to be deleted.
 */ 
DEPRECATED(void deleteDSPInstance(llvm_dsp* dsp));

/*!
 @}
 */

#endif
/**************************  END  llvm-dsp.h **************************/
#endif

#include "DspFaust.h"

std::list<GUI*> GUI::fGuiList;
ztimedmap GUI::gTimedZoneMap;

DspFaust::DspFaust(bool auto_connect)
{
    audio* driver = NULL;
#if JACK_DRIVER
    // JACK has its own sample rate and buffer size
#if MIDICTRL
    driver = new jackaudio_midi(auto_connect);
#else
    driver = new jackaudio(auto_connect);
#endif
#elif JUCE_DRIVER
    // JUCE audio device has its own sample rate and buffer size
    driver = new juceaudio();
#else
    std::cerr << "You are not setting 'sample_rate' and 'buffer_size', but the audio driver needs it !\n";
    throw std::bad_alloc();
#endif
    init(NULL, driver);
}

DspFaust::DspFaust(int sample_rate, int buffer_size, bool auto_connect)
{
    init(NULL, createDriver(sample_rate, buffer_size, auto_connect));
}

#if DYNAMIC_DSP
DspFaust::DspFaust(const string& dsp_content, int sample_rate, int buffer_size, bool auto_connect)
{
    string error_msg;

    // Is dsp_content a filename ?
    fFactory = createDSPFactoryFromFile(dsp_content, 0, NULL, "", error_msg, -1);
    if (!fFactory) {
        std::cerr << error_msg;
        // Is dsp_content a string ?
        fFactory = createDSPFactoryFromString("FaustDSP", dsp_content, 0, NULL, "", error_msg);
        if (!fFactory) {
            std::cerr << error_msg;
            throw bad_alloc();
        }
    }

    dsp* dsp = fFactory->createDSPInstance();
    if (!dsp) {
        std::cerr << "Cannot allocate DSP instance\n";
        throw bad_alloc();
    }
    init(dsp, createDriver(sample_rate, buffer_size, auto_connect));
}
#endif

audio* DspFaust::createDriver(int sample_rate, int buffer_size, bool auto_connect)
{
#if COREAUDIO_DRIVER
    audio* driver = new coreaudio(sample_rate, buffer_size);
#elif IOS_DRIVER
    audio* driver = new iosaudio(sample_rate, buffer_size);
#elif ANDROID_DRIVER
    // OBOE has its own and buffer size
    std::cerr << "You are setting 'buffer_size' with a driver that does not need it !\n";
    audio* driver = new oboeaudio(-1);
#elif ALSA_DRIVER
    audio* driver = new alsaaudio(sample_rate, buffer_size);
#elif JACK_DRIVER
    // JACK has its own sample rate and buffer size
    std::cerr << "You are setting 'sample_rate' and 'buffer_size' with a driver that does not need it !\n";
#if MIDICTRL
    audio* driver = new jackaudio_midi(auto_connect);
#else
    audio* driver = new jackaudio(auto_connect);
#endif
#elif PORTAUDIO_DRIVER
    audio* driver = new portaudio(sample_rate, buffer_size);
#elif RTAUDIO_DRIVER
    audio* driver = new rtaudio(sample_rate, buffer_size);
#elif OPEN_FRAMEWORK_DRIVER
    audio* driver = new ofaudio(sample_rate, buffer_size);
#elif JUCE_DRIVER
    // JUCE audio device has its own sample rate and buffer size
    std::cerr << "You are setting 'sample_rate' and 'buffer_size' with a driver that does not need it !\n";
    audio* driver = new juceaudio();
#elif DUMMY_DRIVER
    audio* driver = new dummyaudio(sample_rate, buffer_size);
#endif
    return driver;
}

void DspFaust::init(dsp* mono_dsp, audio* driver)
{
#if MIDICTRL
    midi_handler* midi;
#if JACK_DRIVER
    midi = static_cast<jackaudio_midi*>(driver);
    fMidiInterface = new MidiUI(midi);
#elif JUCE_DRIVER
    midi = new juce_midi();
    fMidiInterface = new MidiUI(midi, true);
#else
    midi = new rt_midi();
    fMidiInterface = new MidiUI(midi, true);
#endif
    fPolyEngine = new FaustPolyEngine(mono_dsp, driver, midi);
    fPolyEngine->buildUserInterface(fMidiInterface);
#else
    fPolyEngine = new FaustPolyEngine(mono_dsp, driver);
#endif

#if OSCCTRL
#if JUCE_DRIVER
    fOSCInterface = new JuceOSCUI(OSC_IP_ADDRESS, atoi(OSC_IN_PORT), atoi(OSC_OUT_PORT));
#else
    const char* argv[9];
    argv[0] = "Faust";  // TODO may be should retrieve the actual name
    argv[1] = "-xmit";
    argv[2] = "1";      // TODO retrieve that from command line or somewhere
    argv[3] = "-desthost";
    argv[4] = OSC_IP_ADDRESS;   // TODO same
    argv[5] = "-port";
    argv[6] = OSC_IN_PORT;      // TODO same
    argv[7] = "-outport";
    argv[8] = OSC_OUT_PORT;     // TODO same
    /*
    // Deactivated for now (sometimes crashing)
    argv[9] = "-bundle";
    argv[10] = "1";             // TODO same
    */
    fOSCInterface = new OSCUI("Faust", 9, (char**)argv); // TODO fix name
    driver->addControlCallback(osc_compute_callback, fOSCInterface);
#endif
    fPolyEngine->buildUserInterface(fOSCInterface);
#endif

#if SOUNDFILE
#if JUCE_DRIVER
    auto file = File::getSpecialLocation(File::currentExecutableFile)
        .getParentDirectory().getParentDirectory().getChildFile("Resources");
    fSoundInterface = new SoundUI(file.getFullPathName().toStdString());
#else
    // Use bundle path
    fSoundInterface = new SoundUI(SoundUI::getBinaryPath());
#endif
    // SoundUI has to be dispatched on all internal voices
    fPolyEngine->setGroup(false);
    fPolyEngine->buildUserInterface(fSoundInterface);
    fPolyEngine->setGroup(true);
#endif
}

DspFaust::~DspFaust()
{
#if OSCCTRL
    delete fOSCInterface;
#endif
#if MIDICTRL
    delete fMidiInterface;
#endif
#if SOUNDFILE
    delete fSoundInterface;
#endif
    delete fPolyEngine;
#if DYNAMIC_DSP
    deleteDSPFactory(static_cast<llvm_dsp_factory*>(fFactory));
#endif
}

bool DspFaust::start()
{
#if OSCCTRL
    fOSCInterface->run();
#endif
#if MIDICTRL
    if (!fMidiInterface->run()) {
        std::cerr << "MIDI run error...\n";
    }
#endif
	return fPolyEngine->start();
}

void DspFaust::stop()
{
#if OSCCTRL
    fOSCInterface->stop();
#endif
#if MIDICTRL
    fMidiInterface->stop();
#endif
	fPolyEngine->stop();
}

bool DspFaust::configureOSC(int xmit, int inport, int outport, int errport, const char* address)
{
#if OSCCTRL
#if JUCE_DRIVER
    // Nothing for now
    return false;
#else
    if (isRunning()) {
        return false;
    } else {
        oscfaust::OSCControler::gXmit = xmit;
        fOSCInterface->setUDPPort(inport);
        fOSCInterface->setUDPOut(outport);
        fOSCInterface->setUDPErr(errport);
        fOSCInterface->setDestAddress(address);
        return true;
    }
#endif
#else
    return false;
#endif
}

bool DspFaust::isOSCOn()
{
#if OSCCTRL
	return true;
#else
  return false;
#endif
}

bool DspFaust::isRunning()
{
	return fPolyEngine->isRunning();
}

uintptr_t DspFaust::keyOn(int pitch, int velocity)
{
	return (uintptr_t)fPolyEngine->keyOn(pitch, velocity);
}

int DspFaust::keyOff(int pitch)
{
	return fPolyEngine->keyOff(pitch);
}

uintptr_t DspFaust::newVoice()
{
	return (uintptr_t)fPolyEngine->newVoice();
}

int DspFaust::deleteVoice(uintptr_t voice)
{
	return fPolyEngine->deleteVoice(voice);
}

void DspFaust::allNotesOff(bool hard)
{
    fPolyEngine->allNotesOff(hard);
}

void DspFaust::propagateMidi(int count, double time, int type, int channel, int data1, int data2)
{
    fPolyEngine->propagateMidi(count, time, type, channel, data1, data2);
}

const char* DspFaust::getJSONUI()
{
	return fPolyEngine->getJSONUI();
}

const char* DspFaust::getJSONMeta()
{
	return fPolyEngine->getJSONMeta();
}

int DspFaust::getParamsCount()
{
	return fPolyEngine->getParamsCount();
}

void DspFaust::setParamValue(const char* address, float value)
{
	fPolyEngine->setParamValue(address, value);
}

void DspFaust::setParamValue(int id, float value)
{
	fPolyEngine->setParamValue(id, value);
}

float DspFaust::getParamValue(const char* address)
{
	return fPolyEngine->getParamValue(address);
}

float DspFaust::getParamValue(int id)
{
	return fPolyEngine->getParamValue(id);
}

void DspFaust::setVoiceParamValue(const char* address, uintptr_t voice, float value)
{
	fPolyEngine->setVoiceParamValue(address, voice, value);
}

void DspFaust::setVoiceParamValue(int id, uintptr_t voice, float value)
{
	fPolyEngine->setVoiceParamValue(id, voice, value);
}

float DspFaust::getVoiceParamValue(const char* address, uintptr_t voice)
{
	return fPolyEngine->getVoiceParamValue(address, voice);
}

float DspFaust::getVoiceParamValue(int id, uintptr_t voice)
{
	return fPolyEngine->getVoiceParamValue(id, voice);
}

const char* DspFaust::getParamAddress(int id)
{
	return fPolyEngine->getParamAddress(id);
}

const char* DspFaust::getVoiceParamAddress(int id, uintptr_t voice)
{
	return fPolyEngine->getVoiceParamAddress(id, voice);
}

float DspFaust::getParamMin(const char* address)
{
    return fPolyEngine->getParamMin(address);
}

float DspFaust::getParamMin(int id)
{
    return fPolyEngine->getParamMin(id);
}

float DspFaust::getParamMax(const char* address)
{
    return fPolyEngine->getParamMax(address);
}

float DspFaust::getParamMax(int id)
{
    return fPolyEngine->getParamMax(id);
}

float DspFaust::getParamInit(const char* address)
{
    return fPolyEngine->getParamInit(address);
}

float DspFaust::getParamInit(int id)
{
    return fPolyEngine->getParamInit(id);
}

const char* DspFaust::getMetadata(const char* address, const char* key)
{
    return fPolyEngine->getMetadata(address, key);
}

const char* DspFaust::getMetadata(int id, const char* key)
{
    return fPolyEngine->getMetadata(id, key);
}

void DspFaust::propagateAcc(int acc, float v)
{
	fPolyEngine->propagateAcc(acc, v);
}

void DspFaust::setAccConverter(int p, int acc, int curve, float amin, float amid, float amax)
{
	fPolyEngine->setAccConverter(p, acc, curve, amin, amid, amax);
}

void DspFaust::propagateGyr(int acc, float v)
{
	fPolyEngine->propagateGyr(acc, v);
}

void DspFaust::setGyrConverter(int p, int gyr, int curve, float amin, float amid, float amax)
{
	fPolyEngine->setGyrConverter(p, gyr, curve, amin, amid, amax);
}

float DspFaust::getCPULoad()
{
	return fPolyEngine->getCPULoad();
}

int DspFaust::getScreenColor()
{
	return fPolyEngine->getScreenColor();
}

#ifdef BUILD
#include <unistd.h>

int main(int argc, char* argv[])
{
#ifdef DYNAMIC_DSP
    DspFaust* dsp = new DspFaust(argv[1], 44100, 512);
#else
    DspFaust* dsp = new DspFaust(44100, 512);
#endif
    dsp->start();
    std::cout << "Type 'q' to quit\n";
    char c;
    while ((c = getchar()) && (c != 'q')) { usleep(100000); }
    dsp->stop();
    delete dsp;
}

#endif

#endif
