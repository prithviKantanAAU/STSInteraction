/* ------------------------------------------------------------
name: "STS_Synth"
Code generated with Faust 2.30.8 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

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
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
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
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
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
    
        std::string buildLabel(std::string label)
        {
            std::replace(label.begin(), label.end(), ' ', '_');
            return label;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

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
    
        std::string fNullStr = "";
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
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
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
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
        
        const std::string& getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return fNullStr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
    
        const std::string& getParamAddress(FAUSTFLOAT* zone)
        {
            for (auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return fNullStr;
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            }
            if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
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

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/

// BEGIN-FAUSTDSP

    // END-FAUSTDSP
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

class mydspSIG0 {
	
  private:
	
	int iRec12[2];
	
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
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			iRec12[l11] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec12[0] = (iRec12[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec12[0] + -1))));
			iRec12[1] = iRec12[0];
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
	FAUSTFLOAT fHslider7;
	float fRec3[3];
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst2;
	float fRec2[3];
	FAUSTFLOAT fHslider10;
	FAUSTFLOAT fHslider11;
	float fRec1[3];
	float fRec0[3];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider13;
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
	FAUSTFLOAT fHslider16;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	FAUSTFLOAT fHslider17;
	float fVec1[2];
	float fVec2[2];
	float fVec3[2];
	int iRec9[2];
	float fConst11;
	float fRec8[2];
	FAUSTFLOAT fHslider18;
	FAUSTFLOAT fHslider19;
	float fRec10[2];
	FAUSTFLOAT fHslider20;
	float fConst12;
	float fRec13[2];
	float fRec11[2];
	float fConst13;
	float fRec15[2];
	float fConst14;
	float fConst15;
	FAUSTFLOAT fHslider21;
	float fRec17[2];
	float fRec14[3];
	float fRec18[3];
	float fRec19[3];
	float fRec20[3];
	float fRec21[3];
	float fConst16;
	float fConst17;
	float fConst18;
	float fRec7[3];
	float fConst19;
	FAUSTFLOAT fHslider22;
	float fRec22[2];
	float fConst20;
	float fConst21;
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	float fRec25[2];
	float fRec24[2];
	FAUSTFLOAT fHslider26;
	float fRec23[2];
	FAUSTFLOAT fHslider27;
	FAUSTFLOAT fHslider28;
	float fRec26[3];
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	float fRec6[3];
	FAUSTFLOAT fHslider31;
	FAUSTFLOAT fHslider32;
	float fRec5[3];
	float fRec4[3];
	FAUSTFLOAT fHslider33;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider34;
	FAUSTFLOAT fHslider35;
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fConst22;
	float fConst23;
	FAUSTFLOAT fHslider38;
	float fVec4[2];
	float fVec5[2];
	float fVec6[2];
	int iRec31[2];
	float fRec30[2];
	float fRec32[2];
	FAUSTFLOAT fHslider39;
	float fRec35[2];
	int iRec34[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec37[2];
	float fRec36[2];
	float fRec33[3];
	FAUSTFLOAT fHslider40;
	float fRec40[2];
	int iRec39[2];
	float fRec38[3];
	FAUSTFLOAT fHslider41;
	float fRec43[2];
	int iRec42[2];
	float fRec41[3];
	FAUSTFLOAT fHslider42;
	float fRec46[2];
	int iRec45[2];
	float fRec44[3];
	FAUSTFLOAT fHslider43;
	FAUSTFLOAT fHslider44;
	FAUSTFLOAT fHslider45;
	float fRec49[2];
	float fRec48[2];
	FAUSTFLOAT fHslider46;
	float fRec47[2];
	FAUSTFLOAT fHslider47;
	FAUSTFLOAT fHslider48;
	float fRec50[3];
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	float fRec29[3];
	FAUSTFLOAT fHslider51;
	FAUSTFLOAT fHslider52;
	float fRec28[3];
	float fRec27[3];
	FAUSTFLOAT fHslider53;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fHslider59;
	float fRec54[3];
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	float fRec53[3];
	FAUSTFLOAT fHslider62;
	FAUSTFLOAT fHslider63;
	float fRec52[3];
	float fRec51[3];
	float fConst27;
	float fConst28;
	float fConst29;
	float fConst30;
	float fRec57[2];
	float fRec56[2];
	float fRec55[2];
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fRec60[2];
	float fRec59[2];
	float fRec58[2];
	float fConst35;
	float fConst36;
	float fRec63[2];
	float fRec62[2];
	float fRec61[2];
	int iRec68[2];
	float fRec67[3];
	int iRec70[2];
	float fRec69[3];
	int iRec72[2];
	float fRec71[3];
	int iRec74[2];
	float fRec73[3];
	float fRec77[2];
	float fRec76[2];
	float fRec75[2];
	float fRec78[3];
	float fRec66[3];
	float fRec65[3];
	float fRec64[3];
	float fRec81[2];
	float fRec80[2];
	float fRec79[2];
	float fRec84[2];
	float fRec83[2];
	float fRec82[2];
	float fRec87[2];
	float fRec86[2];
	float fRec85[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
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
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "STS_Synth");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
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
		fConst2 = (6.28318548f / fConst0);
		float fConst3 = std::tan((25132.7422f / fConst0));
		float fConst4 = (1.0f / fConst3);
		float fConst5 = (((fConst4 + 0.333333343f) / fConst3) + 1.0f);
		fConst6 = (0.25f / fConst5);
		fConst7 = std::exp((0.0f - (100.0f / fConst0)));
		fConst8 = (1.0f - fConst7);
		fConst9 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst10 = (1.0f / fConst9);
		fConst11 = (1.0f / std::max<float>(1.0f, (2.0f * fConst0)));
		fConst12 = (8.0f / fConst0);
		fConst13 = (1.0f / fConst0);
		fConst14 = std::exp((0.0f - (125.0f / fConst0)));
		fConst15 = (1.0f - fConst14);
		fConst16 = (1.0f / fConst5);
		fConst17 = (((fConst4 + -0.333333343f) / fConst3) + 1.0f);
		fConst18 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst3))));
		fConst19 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst20 = std::exp((0.0f - (20.0f / fConst0)));
		fConst21 = (1.0f - fConst20);
		fConst22 = std::exp((0.0f - (500.0f / fConst0)));
		fConst23 = (1.0f - fConst22);
		fConst24 = (0.330000013f * fConst0);
		fConst25 = (0.100000001f * fConst0);
		fConst26 = (0.660000026f * fConst0);
		fConst27 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst28 = (1.0f - fConst27);
		fConst29 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst30 = (1.0f - fConst29);
		fConst31 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst32 = (1.0f - fConst31);
		fConst33 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst34 = (1.0f - fConst33);
		fConst35 = std::exp((0.0f - (1000.0f / fConst0)));
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
		fHslider6 = FAUSTFLOAT(0.69999999999999996f);
		fHslider7 = FAUSTFLOAT(50.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.69999999999999996f);
		fHslider10 = FAUSTFLOAT(0.0f);
		fHslider11 = FAUSTFLOAT(0.69999999999999996f);
		fHslider12 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider13 = FAUSTFLOAT(0.69999999999999996f);
		fHslider14 = FAUSTFLOAT(20000.0f);
		fHslider15 = FAUSTFLOAT(2000.0f);
		fHslider16 = FAUSTFLOAT(500.0f);
		fHslider17 = FAUSTFLOAT(0.0f);
		fHslider18 = FAUSTFLOAT(0.5f);
		fHslider19 = FAUSTFLOAT(440.0f);
		fHslider20 = FAUSTFLOAT(0.0f);
		fHslider21 = FAUSTFLOAT(0.0f);
		fHslider22 = FAUSTFLOAT(7.0f);
		fHslider23 = FAUSTFLOAT(1.0f);
		fHslider24 = FAUSTFLOAT(0.10000000000000001f);
		fHslider25 = FAUSTFLOAT(0.10000000000000001f);
		fHslider26 = FAUSTFLOAT(0.0f);
		fHslider27 = FAUSTFLOAT(0.69999999999999996f);
		fHslider28 = FAUSTFLOAT(50.0f);
		fHslider29 = FAUSTFLOAT(0.0f);
		fHslider30 = FAUSTFLOAT(0.69999999999999996f);
		fHslider31 = FAUSTFLOAT(0.0f);
		fHslider32 = FAUSTFLOAT(0.69999999999999996f);
		fHslider33 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider34 = FAUSTFLOAT(0.69999999999999996f);
		fHslider35 = FAUSTFLOAT(20000.0f);
		fHslider36 = FAUSTFLOAT(2000.0f);
		fHslider37 = FAUSTFLOAT(500.0f);
		fHslider38 = FAUSTFLOAT(0.0f);
		fHslider39 = FAUSTFLOAT(440.0f);
		fHslider40 = FAUSTFLOAT(440.0f);
		fHslider41 = FAUSTFLOAT(440.0f);
		fHslider42 = FAUSTFLOAT(440.0f);
		fHslider43 = FAUSTFLOAT(1.0f);
		fHslider44 = FAUSTFLOAT(0.10000000000000001f);
		fHslider45 = FAUSTFLOAT(0.10000000000000001f);
		fHslider46 = FAUSTFLOAT(0.0f);
		fHslider47 = FAUSTFLOAT(0.69999999999999996f);
		fHslider48 = FAUSTFLOAT(50.0f);
		fHslider49 = FAUSTFLOAT(0.0f);
		fHslider50 = FAUSTFLOAT(0.69999999999999996f);
		fHslider51 = FAUSTFLOAT(0.0f);
		fHslider52 = FAUSTFLOAT(0.69999999999999996f);
		fHslider53 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider54 = FAUSTFLOAT(0.69999999999999996f);
		fHslider55 = FAUSTFLOAT(20000.0f);
		fHslider56 = FAUSTFLOAT(2000.0f);
		fHslider57 = FAUSTFLOAT(500.0f);
		fHslider58 = FAUSTFLOAT(0.69999999999999996f);
		fHslider59 = FAUSTFLOAT(50.0f);
		fHslider60 = FAUSTFLOAT(0.0f);
		fHslider61 = FAUSTFLOAT(0.69999999999999996f);
		fHslider62 = FAUSTFLOAT(0.0f);
		fHslider63 = FAUSTFLOAT(0.69999999999999996f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
			fRec3[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 3); l4 = (l4 + 1)) {
			fRec0[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			iRec9[l8] = 0;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec10[l10] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec13[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec15[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec18[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec19[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec20[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec21[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec7[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec22[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec25[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec24[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec26[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec6[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec5[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec4[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fVec4[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fVec5[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fVec6[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			iRec31[l33] = 0;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec30[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec32[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec35[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			iRec34[l37] = 0;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec37[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec36[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec33[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec40[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			iRec39[l42] = 0;
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec38[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			iRec42[l45] = 0;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec41[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec46[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			iRec45[l48] = 0;
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec44[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec49[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec48[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec47[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec50[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec29[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec28[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec27[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec54[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec53[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec52[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec51[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec57[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec56[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec55[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec60[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec59[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec58[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec63[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec62[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec61[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			iRec68[l70] = 0;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec67[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec70[l72] = 0;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec69[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			iRec72[l74] = 0;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec71[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			iRec74[l76] = 0;
		}
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec73[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec77[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec76[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec75[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec78[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec66[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec65[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec64[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec81[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec80[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec79[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec84[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec83[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec82[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec87[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec86[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec85[l93] = 0.0f;
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
		ui_interface->openTabBox("Mapping_Tabs");
		ui_interface->openVerticalBox("Comp_Section");
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider24, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider24, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider23, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider23, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider25, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider25, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider26, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider26, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider44, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider44, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider43, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider43, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider45, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider45, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider46, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider46, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("EQ_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider7, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider7, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider6, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider6, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider5, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider8, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider8, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider9, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider9, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider10, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider10, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider11, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider11, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
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
		ui_interface->declare(&fHslider28, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider28, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider27, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider27, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider16, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider29, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider29, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider30, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider30, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider15, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider31, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider31, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider32, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider14, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider13, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider48, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider48, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider47, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider47, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider37, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider49, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider49, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider50, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider36, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider51, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider52, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider35, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider34, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider59, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider59, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider58, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider58, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider57, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider60, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider60, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider61, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider61, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider56, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider56, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider62, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider62, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider63, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider63, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider55, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider54, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider1, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider12, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider33, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider53, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox0);
		ui_interface->addCheckButton("2", &fCheckbox1);
		ui_interface->addCheckButton("3", &fCheckbox2);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider18, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider21, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider19, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider17, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider39, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider40, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider41, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider42, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider38, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider20, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 7");
		ui_interface->addHorizontalSlider("Control 0", &fHslider0, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 9");
		ui_interface->addHorizontalSlider("Control 0", &fHslider22, 7.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
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
		float fSlow14 = (1.0f / float(fHslider6));
		float fSlow15 = std::tan((fConst1 * float(fHslider7)));
		float fSlow16 = (1.0f / fSlow15);
		float fSlow17 = (1.0f / (((fSlow14 + fSlow16) / fSlow15) + 1.0f));
		float fSlow18 = (((fSlow16 - fSlow14) / fSlow15) + 1.0f);
		float fSlow19 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow15))));
		float fSlow20 = (1.0f / fSlow12);
		float fSlow21 = float(fHslider8);
		int iSlow22 = (fSlow21 > 0.0f);
		float fSlow23 = (float(fHslider9) * std::sin((fConst2 * fSlow11)));
		float fSlow24 = (fConst1 * ((fSlow11 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow21)))) / fSlow23));
		float fSlow25 = (fConst1 * (fSlow11 / fSlow23));
		float fSlow26 = (iSlow22 ? fSlow25 : fSlow24);
		float fSlow27 = ((fSlow20 * (fSlow20 - fSlow26)) + 1.0f);
		float fSlow28 = ((fSlow20 * (fSlow20 + fSlow26)) + 1.0f);
		float fSlow29 = (iSlow22 ? fSlow24 : fSlow25);
		float fSlow30 = ((fSlow20 * (fSlow20 + fSlow29)) + 1.0f);
		float fSlow31 = ((fSlow20 * (fSlow20 - fSlow29)) + 1.0f);
		float fSlow32 = (1.0f / fSlow9);
		float fSlow33 = float(fHslider10);
		int iSlow34 = (fSlow33 > 0.0f);
		float fSlow35 = (float(fHslider11) * std::sin((fConst2 * fSlow8)));
		float fSlow36 = (fConst1 * ((fSlow8 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow33)))) / fSlow35));
		float fSlow37 = (fConst1 * (fSlow8 / fSlow35));
		float fSlow38 = (iSlow34 ? fSlow37 : fSlow36);
		float fSlow39 = ((fSlow32 * (fSlow32 - fSlow38)) + 1.0f);
		float fSlow40 = ((fSlow32 * (fSlow32 + fSlow38)) + 1.0f);
		float fSlow41 = (iSlow34 ? fSlow36 : fSlow37);
		float fSlow42 = ((fSlow32 * (fSlow32 + fSlow41)) + 1.0f);
		float fSlow43 = ((fSlow32 * (fSlow32 - fSlow41)) + 1.0f);
		float fSlow44 = (1.0f / fSlow6);
		float fSlow45 = (((fSlow5 - fSlow3) / fSlow4) + 1.0f);
		float fSlow46 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow4))));
		float fSlow47 = (1.0f / float(fHslider13));
		float fSlow48 = std::tan((fConst1 * float(fHslider14)));
		float fSlow49 = (1.0f / fSlow48);
		float fSlow50 = (((fSlow47 + fSlow49) / fSlow48) + 1.0f);
		float fSlow51 = ((std::pow(10.0f, (0.0500000007f * float(fHslider12))) * (1.0f - float(fCheckbox1))) / fSlow50);
		float fSlow52 = float(fHslider15);
		float fSlow53 = std::tan((fConst1 * fSlow52));
		float fSlow54 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow53))));
		float fSlow55 = float(fHslider16);
		float fSlow56 = std::tan((fConst1 * fSlow55));
		float fSlow57 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow56))));
		float fSlow58 = float(fHslider17);
		float fSlow59 = (0.0f - fSlow58);
		float fSlow60 = std::pow(10.0f, (2.0f * (float(fHslider18) + -0.5f)));
		float fSlow61 = (2.0f * fSlow60);
		float fSlow62 = float(fHslider19);
		float fSlow63 = float(fHslider20);
		float fSlow64 = (0.5f * fSlow63);
		float fSlow65 = (fConst15 * float(fHslider21));
		float fSlow66 = float(fHslider22);
		float fSlow67 = (fConst21 * ((1.0f / float(fHslider23)) + -1.0f));
		float fSlow68 = float(fHslider24);
		int iSlow69 = (std::fabs(fSlow68) < 1.1920929e-07f);
		float fSlow70 = (iSlow69 ? 0.0f : std::exp((0.0f - (fConst13 / (iSlow69 ? 1.0f : fSlow68)))));
		float fSlow71 = float(fHslider25);
		int iSlow72 = (std::fabs(fSlow71) < 1.1920929e-07f);
		float fSlow73 = (iSlow72 ? 0.0f : std::exp((0.0f - (fConst13 / (iSlow72 ? 1.0f : fSlow71)))));
		float fSlow74 = (1.0f - fSlow73);
		float fSlow75 = (1.0f - fSlow70);
		float fSlow76 = float(fHslider26);
		float fSlow77 = (1.0f / float(fHslider27));
		float fSlow78 = std::tan((fConst1 * float(fHslider28)));
		float fSlow79 = (1.0f / fSlow78);
		float fSlow80 = (1.0f / (((fSlow77 + fSlow79) / fSlow78) + 1.0f));
		float fSlow81 = (((fSlow79 - fSlow77) / fSlow78) + 1.0f);
		float fSlow82 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow78))));
		float fSlow83 = (1.0f / fSlow56);
		float fSlow84 = float(fHslider29);
		int iSlow85 = (fSlow84 > 0.0f);
		float fSlow86 = (float(fHslider30) * std::sin((fConst2 * fSlow55)));
		float fSlow87 = (fConst1 * ((fSlow55 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow84)))) / fSlow86));
		float fSlow88 = (fConst1 * (fSlow55 / fSlow86));
		float fSlow89 = (iSlow85 ? fSlow88 : fSlow87);
		float fSlow90 = ((fSlow83 * (fSlow83 - fSlow89)) + 1.0f);
		float fSlow91 = ((fSlow83 * (fSlow83 + fSlow89)) + 1.0f);
		float fSlow92 = (iSlow85 ? fSlow87 : fSlow88);
		float fSlow93 = ((fSlow83 * (fSlow83 + fSlow92)) + 1.0f);
		float fSlow94 = ((fSlow83 * (fSlow83 - fSlow92)) + 1.0f);
		float fSlow95 = (1.0f / fSlow53);
		float fSlow96 = float(fHslider31);
		int iSlow97 = (fSlow96 > 0.0f);
		float fSlow98 = (float(fHslider32) * std::sin((fConst2 * fSlow52)));
		float fSlow99 = (fConst1 * ((fSlow52 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow96)))) / fSlow98));
		float fSlow100 = (fConst1 * (fSlow52 / fSlow98));
		float fSlow101 = (iSlow97 ? fSlow100 : fSlow99);
		float fSlow102 = ((fSlow95 * (fSlow95 - fSlow101)) + 1.0f);
		float fSlow103 = ((fSlow95 * (fSlow95 + fSlow101)) + 1.0f);
		float fSlow104 = (iSlow97 ? fSlow99 : fSlow100);
		float fSlow105 = ((fSlow95 * (fSlow95 + fSlow104)) + 1.0f);
		float fSlow106 = ((fSlow95 * (fSlow95 - fSlow104)) + 1.0f);
		float fSlow107 = (1.0f / fSlow50);
		float fSlow108 = (((fSlow49 - fSlow47) / fSlow48) + 1.0f);
		float fSlow109 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow48))));
		float fSlow110 = (1.0f / float(fHslider34));
		float fSlow111 = std::tan((fConst1 * float(fHslider35)));
		float fSlow112 = (1.0f / fSlow111);
		float fSlow113 = (((fSlow110 + fSlow112) / fSlow111) + 1.0f);
		float fSlow114 = ((std::pow(10.0f, (0.0500000007f * float(fHslider33))) * (1.0f - float(fCheckbox2))) / fSlow113);
		float fSlow115 = float(fHslider36);
		float fSlow116 = std::tan((fConst1 * fSlow115));
		float fSlow117 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow116))));
		float fSlow118 = float(fHslider37);
		float fSlow119 = std::tan((fConst1 * fSlow118));
		float fSlow120 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow119))));
		float fSlow121 = float(fHslider38);
		float fSlow122 = (0.0f - fSlow121);
		float fSlow123 = ((float(fHslider39) * fSlow60) * ((0.0500000007f * fSlow63) + 1.0f));
		float fSlow124 = (fConst8 * ((0.200000003f * ((0.5f * fSlow66) + -5.0f)) + 1.0f));
		float fSlow125 = ((float(fHslider40) * fSlow60) * (1.0f - (0.0299999993f * fSlow63)));
		float fSlow126 = ((float(fHslider41) * fSlow60) * ((0.310000002f * fSlow63) + 1.0f));
		float fSlow127 = ((float(fHslider42) * fSlow60) * (1.0f - (0.270000011f * fSlow63)));
		float fSlow128 = (fConst21 * ((1.0f / float(fHslider43)) + -1.0f));
		float fSlow129 = float(fHslider44);
		int iSlow130 = (std::fabs(fSlow129) < 1.1920929e-07f);
		float fSlow131 = (iSlow130 ? 0.0f : std::exp((0.0f - (fConst13 / (iSlow130 ? 1.0f : fSlow129)))));
		float fSlow132 = float(fHslider45);
		int iSlow133 = (std::fabs(fSlow132) < 1.1920929e-07f);
		float fSlow134 = (iSlow133 ? 0.0f : std::exp((0.0f - (fConst13 / (iSlow133 ? 1.0f : fSlow132)))));
		float fSlow135 = (1.0f - fSlow134);
		float fSlow136 = (1.0f - fSlow131);
		float fSlow137 = float(fHslider46);
		float fSlow138 = (1.0f / float(fHslider47));
		float fSlow139 = std::tan((fConst1 * float(fHslider48)));
		float fSlow140 = (1.0f / fSlow139);
		float fSlow141 = (1.0f / (((fSlow138 + fSlow140) / fSlow139) + 1.0f));
		float fSlow142 = (((fSlow140 - fSlow138) / fSlow139) + 1.0f);
		float fSlow143 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow139))));
		float fSlow144 = (1.0f / fSlow119);
		float fSlow145 = float(fHslider49);
		int iSlow146 = (fSlow145 > 0.0f);
		float fSlow147 = (float(fHslider50) * std::sin((fConst2 * fSlow118)));
		float fSlow148 = (fConst1 * ((fSlow118 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow145)))) / fSlow147));
		float fSlow149 = (fConst1 * (fSlow118 / fSlow147));
		float fSlow150 = (iSlow146 ? fSlow149 : fSlow148);
		float fSlow151 = ((fSlow144 * (fSlow144 - fSlow150)) + 1.0f);
		float fSlow152 = ((fSlow144 * (fSlow144 + fSlow150)) + 1.0f);
		float fSlow153 = (iSlow146 ? fSlow148 : fSlow149);
		float fSlow154 = ((fSlow144 * (fSlow144 + fSlow153)) + 1.0f);
		float fSlow155 = ((fSlow144 * (fSlow144 - fSlow153)) + 1.0f);
		float fSlow156 = (1.0f / fSlow116);
		float fSlow157 = float(fHslider51);
		int iSlow158 = (fSlow157 > 0.0f);
		float fSlow159 = (float(fHslider52) * std::sin((fConst2 * fSlow115)));
		float fSlow160 = (fConst1 * ((fSlow115 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow157)))) / fSlow159));
		float fSlow161 = (fConst1 * (fSlow115 / fSlow159));
		float fSlow162 = (iSlow158 ? fSlow161 : fSlow160);
		float fSlow163 = ((fSlow156 * (fSlow156 - fSlow162)) + 1.0f);
		float fSlow164 = ((fSlow156 * (fSlow156 + fSlow162)) + 1.0f);
		float fSlow165 = (iSlow158 ? fSlow160 : fSlow161);
		float fSlow166 = ((fSlow156 * (fSlow156 + fSlow165)) + 1.0f);
		float fSlow167 = ((fSlow156 * (fSlow156 - fSlow165)) + 1.0f);
		float fSlow168 = (1.0f / fSlow113);
		float fSlow169 = (((fSlow112 - fSlow110) / fSlow111) + 1.0f);
		float fSlow170 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow111))));
		float fSlow171 = (1.0f / float(fHslider54));
		float fSlow172 = std::tan((fConst1 * float(fHslider55)));
		float fSlow173 = (1.0f / fSlow172);
		float fSlow174 = (((fSlow171 + fSlow173) / fSlow172) + 1.0f);
		float fSlow175 = ((std::pow(10.0f, (0.0500000007f * float(fHslider53))) * (1.0f - float(fCheckbox3))) / fSlow174);
		float fSlow176 = (0.300000012f * fSlow175);
		float fSlow177 = float(fHslider56);
		float fSlow178 = std::tan((fConst1 * fSlow177));
		float fSlow179 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow178))));
		float fSlow180 = float(fHslider57);
		float fSlow181 = std::tan((fConst1 * fSlow180));
		float fSlow182 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow181))));
		float fSlow183 = (1.0f / float(fHslider58));
		float fSlow184 = std::tan((fConst1 * float(fHslider59)));
		float fSlow185 = (1.0f / fSlow184);
		float fSlow186 = (1.0f / (((fSlow183 + fSlow185) / fSlow184) + 1.0f));
		float fSlow187 = (((fSlow185 - fSlow183) / fSlow184) + 1.0f);
		float fSlow188 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow184))));
		float fSlow189 = (1.0f / fSlow181);
		float fSlow190 = float(fHslider60);
		int iSlow191 = (fSlow190 > 0.0f);
		float fSlow192 = (float(fHslider61) * std::sin((fConst2 * fSlow180)));
		float fSlow193 = (fConst1 * ((fSlow180 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow190)))) / fSlow192));
		float fSlow194 = (fConst1 * (fSlow180 / fSlow192));
		float fSlow195 = (iSlow191 ? fSlow194 : fSlow193);
		float fSlow196 = ((fSlow189 * (fSlow189 - fSlow195)) + 1.0f);
		float fSlow197 = ((fSlow189 * (fSlow189 + fSlow195)) + 1.0f);
		float fSlow198 = (iSlow191 ? fSlow193 : fSlow194);
		float fSlow199 = ((fSlow189 * (fSlow189 + fSlow198)) + 1.0f);
		float fSlow200 = ((fSlow189 * (fSlow189 - fSlow198)) + 1.0f);
		float fSlow201 = (1.0f / fSlow178);
		float fSlow202 = float(fHslider62);
		int iSlow203 = (fSlow202 > 0.0f);
		float fSlow204 = (float(fHslider63) * std::sin((fConst2 * fSlow177)));
		float fSlow205 = (fConst1 * ((fSlow177 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow202)))) / fSlow204));
		float fSlow206 = (fConst1 * (fSlow177 / fSlow204));
		float fSlow207 = (iSlow203 ? fSlow206 : fSlow205);
		float fSlow208 = ((fSlow201 * (fSlow201 - fSlow207)) + 1.0f);
		float fSlow209 = ((fSlow201 * (fSlow201 + fSlow207)) + 1.0f);
		float fSlow210 = (iSlow203 ? fSlow205 : fSlow206);
		float fSlow211 = ((fSlow201 * (fSlow201 + fSlow210)) + 1.0f);
		float fSlow212 = ((fSlow201 * (fSlow201 - fSlow210)) + 1.0f);
		float fSlow213 = (1.0f / fSlow174);
		float fSlow214 = (((fSlow173 - fSlow171) / fSlow172) + 1.0f);
		float fSlow215 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow172))));
		float fSlow216 = (1.41400003f * std::sqrt(fSlow0));
		float fSlow217 = (0.949999988f * fSlow175);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec3[0] = (-1.0f * (fSlow17 * ((fSlow18 * fRec3[2]) + (fSlow19 * fRec3[1]))));
			float fTemp0 = (fSlow13 * fRec2[1]);
			fRec2[0] = (0.0f - ((fSlow17 * (fRec3[2] + (fRec3[0] + (2.0f * fRec3[1])))) + (((fRec2[2] * fSlow27) + fTemp0) / fSlow28)));
			float fTemp1 = (fSlow10 * fRec1[1]);
			fRec1[0] = ((((fTemp0 + (fRec2[0] * fSlow30)) + (fRec2[2] * fSlow31)) / fSlow28) - (((fRec1[2] * fSlow39) + fTemp1) / fSlow40));
			fRec0[0] = ((((fTemp1 + (fRec1[0] * fSlow42)) + (fRec1[2] * fSlow43)) / fSlow40) - (fSlow44 * ((fSlow45 * fRec0[2]) + (fSlow46 * fRec0[1]))));
			fVec1[0] = fSlow58;
			float fTemp2 = (fSlow58 - fVec1[1]);
			fVec2[0] = fSlow59;
			float fTemp3 = (0.0f - (fSlow58 + fVec2[1]));
			float fTemp4 = ((fTemp2 * float((fTemp2 > 0.0f))) + (fTemp3 * float((fTemp3 > 0.0f))));
			fVec3[0] = fTemp4;
			iRec9[0] = (((iRec9[1] + (iRec9[1] > 0)) * (fTemp4 <= fVec3[1])) + (fTemp4 > fVec3[1]));
			float fTemp5 = float(iRec9[0]);
			float fTemp6 = (fConst10 * fTemp5);
			float fTemp7 = (fConst9 - fTemp5);
			fRec8[0] = ((fConst7 * fRec8[1]) + (fConst8 * std::max<float>(0.0f, std::min<float>(fTemp6, ((fConst11 * fTemp7) + 1.0f)))));
			int iTemp8 = int(fTemp4);
			fRec10[0] = (iTemp8 ? fSlow62 : fRec10[1]);
			fRec13[0] = (fConst12 + (fRec13[1] - std::floor((fConst12 + fRec13[1]))));
			fRec11[0] = ((0.999000013f * fRec11[1]) + (0.00100000005f * ((fSlow64 * ftbl0mydspSIG0[int((65536.0f * fRec13[0]))]) + 1.0f)));
			float fTemp9 = std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow61 * (fRec10[0] * fRec11[0]))));
			float fTemp10 = ((0.00366666657f * (400.0f - fTemp9)) + 3.0f);
			float fTemp11 = std::max<float>(1.1920929e-07f, std::fabs(fTemp9));
			float fTemp12 = (fRec15[1] + (fConst13 * fTemp11));
			float fTemp13 = (fTemp12 + -1.0f);
			int iTemp14 = (fTemp13 < 0.0f);
			fRec15[0] = (iTemp14 ? fTemp12 : fTemp13);
			float fRec16 = (iTemp14 ? fTemp12 : (fTemp12 + (fTemp13 * (1.0f - (fConst0 / fTemp11)))));
			float fTemp15 = (0.400000006f * ((2.0f * fRec16) + -1.0f));
			fRec17[0] = (fSlow65 + (fConst14 * fRec17[1]));
			float fTemp16 = (fRec17[0] + 10.0f);
			int iTemp17 = (fTemp16 < 23.0f);
			int iTemp18 = (fTemp16 < 24.0f);
			float fTemp19 = (fRec17[0] + -13.0f);
			int iTemp20 = (fTemp16 < 22.0f);
			float fTemp21 = (fRec17[0] + -12.0f);
			int iTemp22 = (fTemp16 < 21.0f);
			float fTemp23 = (fRec17[0] + -11.0f);
			int iTemp24 = (fTemp16 < 20.0f);
			float fTemp25 = (fRec17[0] + -10.0f);
			int iTemp26 = (fTemp16 < 19.0f);
			float fTemp27 = (fRec17[0] + -9.0f);
			int iTemp28 = (fTemp16 < 18.0f);
			float fTemp29 = (fRec17[0] + -8.0f);
			int iTemp30 = (fTemp16 < 17.0f);
			float fTemp31 = (fRec17[0] + -7.0f);
			int iTemp32 = (fTemp16 < 16.0f);
			float fTemp33 = (fRec17[0] + -6.0f);
			float fTemp34 = (80.0f * fTemp33);
			int iTemp35 = (fTemp16 < 15.0f);
			float fTemp36 = (fRec17[0] + -5.0f);
			int iTemp37 = (fTemp16 < 14.0f);
			float fTemp38 = (fRec17[0] + -4.0f);
			int iTemp39 = (fTemp16 < 13.0f);
			float fTemp40 = (fRec17[0] + -3.0f);
			int iTemp41 = (fTemp16 < 12.0f);
			float fTemp42 = (fRec17[0] + -2.0f);
			int iTemp43 = (fTemp16 < 11.0f);
			float fTemp44 = (fRec17[0] + -1.0f);
			int iTemp45 = (fTemp16 < 10.0f);
			int iTemp46 = (fTemp16 < 9.0f);
			float fTemp47 = (fRec17[0] + 1.0f);
			int iTemp48 = (fTemp16 < 8.0f);
			float fTemp49 = (fRec17[0] + 2.0f);
			float fTemp50 = (50.0f * fTemp49);
			int iTemp51 = (fTemp16 < 7.0f);
			float fTemp52 = (fRec17[0] + 3.0f);
			int iTemp53 = (fTemp16 < 6.0f);
			float fTemp54 = (fRec17[0] + 4.0f);
			int iTemp55 = (fTemp16 < 5.0f);
			float fTemp56 = (fRec17[0] + 5.0f);
			int iTemp57 = (fTemp16 < 4.0f);
			float fTemp58 = (fRec17[0] + 6.0f);
			int iTemp59 = (fTemp16 < 3.0f);
			float fTemp60 = (fRec17[0] + 7.0f);
			int iTemp61 = (fTemp16 < 2.0f);
			float fTemp62 = (fRec17[0] + 8.0f);
			int iTemp63 = (fTemp16 < 1.0f);
			float fTemp64 = (fRec17[0] + 9.0f);
			int iTemp65 = (fTemp16 < 0.0f);
			float fTemp66 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 800.0f : (iTemp63 ? (800.0f - (400.0f * fTemp16)) : 400.0f)) : (iTemp61 ? (400.0f - (50.0f * fTemp64)) : 350.0f)) : (iTemp59 ? ((100.0f * fTemp62) + 350.0f) : 450.0f)) : (iTemp57 ? (450.0f - (125.0f * fTemp60)) : 325.0f)) : (iTemp55 ? ((275.0f * fTemp58) + 325.0f) : 600.0f)) : (iTemp53 ? (600.0f - (200.0f * fTemp56)) : 400.0f)) : (iTemp51 ? (400.0f - (150.0f * fTemp54)) : 250.0f)) : (iTemp48 ? ((150.0f * fTemp52) + 250.0f) : 400.0f)) : (iTemp46 ? (400.0f - fTemp50) : 350.0f)) : (iTemp45 ? ((310.0f * fTemp47) + 350.0f) : 660.0f)) : (iTemp43 ? (660.0f - (220.0f * fRec17[0])) : 440.0f)) : (iTemp41 ? (440.0f - (170.0f * fTemp44)) : 270.0f)) : (iTemp39 ? ((160.0f * fTemp42) + 270.0f) : 430.0f)) : (iTemp37 ? (430.0f - (60.0f * fTemp40)) : 370.0f)) : (iTemp35 ? ((430.0f * fTemp38) + 370.0f) : 800.0f)) : (iTemp32 ? (800.0f - (450.0f * fTemp36)) : 350.0f)) : (iTemp30 ? (350.0f - fTemp34) : 270.0f)) : (iTemp28 ? ((180.0f * fTemp31) + 270.0f) : 450.0f)) : (iTemp26 ? (450.0f - (125.0f * fTemp29)) : 325.0f)) : (iTemp24 ? (325.0f * (fTemp27 + 1.0f)) : 650.0f)) : (iTemp22 ? (650.0f - (250.0f * fTemp25)) : 400.0f)) : (iTemp20 ? (400.0f - (110.0f * fTemp23)) : 290.0f)) : (iTemp17 ? ((110.0f * fTemp21) + 290.0f) : 400.0f)) : (iTemp18 ? (400.0f - (50.0f * fTemp19)) : 350.0f));
			float fTemp67 = ((fTemp66 <= fTemp9) ? fTemp9 : fTemp66);
			float fTemp68 = std::tan((fConst1 * fTemp67));
			float fTemp69 = (1.0f / fTemp68);
			float fTemp70 = (20.0f * fTemp25);
			float fTemp71 = (20.0f * fTemp36);
			float fTemp72 = (10.0f * fRec17[0]);
			float fTemp73 = (10.0f * fTemp58);
			float fTemp74 = (20.0f * fTemp60);
			float fTemp75 = (20.0f * fTemp62);
			float fTemp76 = (20.0f * fTemp16);
			float fTemp77 = ((iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 80.0f : (iTemp63 ? (80.0f - fTemp76) : 60.0f)) : (iTemp61 ? (60.0f - (10.0f * fTemp64)) : 50.0f)) : (iTemp59 ? (fTemp75 + 50.0f) : 70.0f)) : (iTemp57 ? (70.0f - fTemp74) : 50.0f)) : (iTemp55 ? (fTemp73 + 50.0f) : 60.0f)) : (iTemp53 ? (60.0f - (20.0f * fTemp56)) : 40.0f)) : (iTemp51 ? ((20.0f * fTemp54) + 40.0f) : 60.0f)) : (iTemp48 ? (60.0f - (20.0f * fTemp52)) : 40.0f)) : 40.0f) : (iTemp45 ? (40.0f * (fTemp47 + 1.0f)) : 80.0f)) : (iTemp43 ? (80.0f - fTemp72) : 70.0f)) : (iTemp41 ? (70.0f - (30.0f * fTemp44)) : 40.0f)) : 40.0f) : 40.0f) : (iTemp35 ? (40.0f * (fTemp38 + 1.0f)) : 80.0f)) : (iTemp32 ? (80.0f - fTemp71) : 60.0f)) : 60.0f) : (iTemp28 ? (60.0f - (20.0f * fTemp31)) : 40.0f)) : (iTemp26 ? ((10.0f * fTemp29) + 40.0f) : 50.0f)) : 50.0f) : (iTemp22 ? (fTemp70 + 50.0f) : 70.0f)) : (iTemp20 ? (70.0f - (30.0f * fTemp23)) : 40.0f)) : (iTemp17 ? ((30.0f * fTemp21) + 40.0f) : 70.0f)) : (iTemp18 ? (70.0f - (30.0f * fTemp19)) : 40.0f)) / fTemp67);
			float fTemp78 = (((fTemp69 + fTemp77) / fTemp68) + 1.0f);
			fRec14[0] = (fTemp15 - (((fRec14[2] * (((fTemp69 - fTemp77) / fTemp68) + 1.0f)) + (2.0f * (fRec14[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp68)))))) / fTemp78));
			float fTemp79 = (fTemp68 * fTemp78);
			float fTemp80 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 1150.0f : (iTemp63 ? ((450.0f * fTemp16) + 1150.0f) : 1600.0f)) : (iTemp61 ? ((100.0f * fTemp64) + 1600.0f) : 1700.0f)) : (iTemp59 ? (1700.0f - (900.0f * fTemp62)) : 800.0f)) : (iTemp57 ? (800.0f - (100.0f * fTemp60)) : 700.0f)) : (iTemp55 ? ((340.0f * fTemp58) + 700.0f) : 1040.0f)) : (iTemp53 ? ((580.0f * fTemp56) + 1040.0f) : 1620.0f)) : (iTemp51 ? ((130.0f * fTemp54) + 1620.0f) : 1750.0f)) : (iTemp48 ? (1750.0f - (1000.0f * fTemp52)) : 750.0f)) : (iTemp46 ? (750.0f - (150.0f * fTemp49)) : 600.0f)) : (iTemp45 ? ((520.0f * fTemp47) + 600.0f) : 1120.0f)) : (iTemp43 ? ((680.0f * fRec17[0]) + 1120.0f) : 1800.0f)) : (iTemp41 ? ((50.0f * fTemp44) + 1800.0f) : 1850.0f)) : (iTemp39 ? (1850.0f - (1030.0f * fTemp42)) : 820.0f)) : (iTemp37 ? (820.0f - (190.0f * fTemp40)) : 630.0f)) : (iTemp35 ? ((520.0f * fTemp38) + 630.0f) : 1150.0f)) : (iTemp32 ? ((850.0f * fTemp36) + 1150.0f) : 2000.0f)) : (iTemp30 ? ((140.0f * fTemp33) + 2000.0f) : 2140.0f)) : (iTemp28 ? (2140.0f - (1340.0f * fTemp31)) : 800.0f)) : (iTemp26 ? (800.0f - (100.0f * fTemp29)) : 700.0f)) : (iTemp24 ? ((380.0f * fTemp27) + 700.0f) : 1080.0f)) : (iTemp22 ? ((620.0f * fTemp25) + 1080.0f) : 1700.0f)) : (iTemp20 ? ((170.0f * fTemp23) + 1700.0f) : 1870.0f)) : (iTemp17 ? (1870.0f - (1070.0f * fTemp21)) : 800.0f)) : (iTemp18 ? (800.0f - (200.0f * fTemp19)) : 600.0f));
			float fTemp81 = std::tan((fConst1 * fTemp80));
			float fTemp82 = (1.0f / fTemp81);
			float fTemp83 = (10.0f * fTemp21);
			float fTemp84 = (10.0f * fTemp25);
			float fTemp85 = (10.0f * fTemp47);
			float fTemp86 = (10.0f * fTemp56);
			float fTemp87 = ((iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 90.0f : (iTemp63 ? (90.0f - (10.0f * fTemp16)) : 80.0f)) : (iTemp61 ? ((20.0f * fTemp64) + 80.0f) : 100.0f)) : (iTemp59 ? (100.0f - fTemp75) : 80.0f)) : (iTemp57 ? (80.0f - fTemp74) : 60.0f)) : (iTemp55 ? (fTemp73 + 60.0f) : 70.0f)) : (iTemp53 ? (fTemp86 + 70.0f) : 80.0f)) : (iTemp51 ? ((10.0f * fTemp54) + 80.0f) : 90.0f)) : (iTemp48 ? (90.0f - (10.0f * fTemp52)) : 80.0f)) : 80.0f) : (iTemp45 ? (fTemp85 + 80.0f) : 90.0f)) : (iTemp43 ? (90.0f - fTemp72) : 80.0f)) : (iTemp41 ? ((10.0f * fTemp44) + 80.0f) : 90.0f)) : (iTemp39 ? (90.0f - (10.0f * fTemp42)) : 80.0f)) : (iTemp37 ? (80.0f - (20.0f * fTemp40)) : 60.0f)) : (iTemp35 ? ((30.0f * fTemp38) + 60.0f) : 90.0f)) : (iTemp32 ? ((10.0f * fTemp36) + 90.0f) : 100.0f)) : (iTemp30 ? (100.0f - (10.0f * fTemp33)) : 90.0f)) : (iTemp28 ? (90.0f - (10.0f * fTemp31)) : 80.0f)) : (iTemp26 ? (80.0f - (20.0f * fTemp29)) : 60.0f)) : (iTemp24 ? ((30.0f * fTemp27) + 60.0f) : 90.0f)) : (iTemp22 ? (90.0f - fTemp84) : 80.0f)) : (iTemp20 ? ((10.0f * fTemp23) + 80.0f) : 90.0f)) : (iTemp17 ? (90.0f - fTemp83) : 80.0f)) : (iTemp18 ? (80.0f - (20.0f * fTemp19)) : 60.0f)) / fTemp80);
			float fTemp88 = (((fTemp82 + fTemp87) / fTemp81) + 1.0f);
			fRec18[0] = (fTemp15 - (((fRec18[2] * (((fTemp82 - fTemp87) / fTemp81) + 1.0f)) + (2.0f * (fRec18[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp81)))))) / fTemp88));
			float fTemp89 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 0.630957007f : (iTemp63 ? (0.630957007f - (0.567861021f * fTemp16)) : 0.0630960017f)) : (iTemp61 ? ((0.0369039997f * fTemp64) + 0.0630960017f) : 0.100000001f)) : (iTemp59 ? ((0.254812986f * fTemp62) + 0.100000001f) : 0.35481301f)) : (iTemp57 ? (0.35481301f - (0.103624001f * fTemp60)) : 0.251188993f)) : (iTemp55 ? ((0.195494995f * fTemp58) + 0.251188993f) : 0.446684003f)) : (iTemp53 ? (0.446684003f - (0.195494995f * fTemp56)) : 0.251188993f)) : (iTemp51 ? (0.251188993f - (0.219566002f * fTemp54)) : 0.0316229984f)) : (iTemp48 ? ((0.250214994f * fTemp52) + 0.0316229984f) : 0.281838f)) : (iTemp46 ? (0.281838f - (0.181838006f * fTemp49)) : 0.100000001f)) : (iTemp45 ? ((0.401187003f * fTemp47) + 0.100000001f) : 0.501187027f)) : (iTemp43 ? (0.501187027f - (0.301661015f * fRec17[0])) : 0.199525997f)) : (iTemp41 ? (0.199525997f - (0.136429995f * fTemp44)) : 0.0630960017f)) : (iTemp39 ? ((0.253131986f * fTemp42) + 0.0630960017f) : 0.316228002f)) : (iTemp37 ? (0.316228002f - (0.216227993f * fTemp40)) : 0.100000001f)) : (iTemp35 ? ((0.401187003f * fTemp38) + 0.100000001f) : 0.501187027f)) : (iTemp32 ? (0.501187027f - (0.401187003f * fTemp36)) : 0.100000001f)) : (iTemp30 ? ((0.151188999f * fTemp33) + 0.100000001f) : 0.251188993f)) : (iTemp28 ? ((0.0306490008f * fTemp31) + 0.251188993f) : 0.281838f)) : (iTemp26 ? (0.281838f - (0.123349003f * fTemp29)) : 0.158489004f)) : (iTemp24 ? ((0.342698008f * fTemp27) + 0.158489004f) : 0.501187027f)) : (iTemp22 ? (0.501187027f - (0.301661015f * fTemp25)) : 0.199525997f)) : (iTemp20 ? (0.199525997f - (0.0216979999f * fTemp23)) : 0.177827999f)) : (iTemp17 ? ((0.138400003f * fTemp21) + 0.177827999f) : 0.316228002f)) : (iTemp18 ? (0.316228002f - (0.216227993f * fTemp19)) : 0.100000001f));
			float fTemp90 = (fTemp81 * fTemp88);
			float fTemp91 = (100.0f * fTemp19);
			float fTemp92 = (50.0f * fTemp27);
			float fTemp93 = (50.0f * fRec17[0]);
			float fTemp94 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 2800.0f : (iTemp63 ? (2800.0f - (100.0f * fTemp16)) : 2700.0f)) : 2700.0f) : (iTemp59 ? ((130.0f * fTemp62) + 2700.0f) : 2830.0f)) : (iTemp57 ? (2830.0f - (300.0f * fTemp60)) : 2530.0f)) : (iTemp55 ? (2530.0f - (280.0f * fTemp58)) : 2250.0f)) : (iTemp53 ? ((150.0f * fTemp56) + 2250.0f) : 2400.0f)) : (iTemp51 ? ((200.0f * fTemp54) + 2400.0f) : 2600.0f)) : (iTemp48 ? (2600.0f - (200.0f * fTemp52)) : 2400.0f)) : 2400.0f) : (iTemp45 ? ((350.0f * fTemp47) + 2400.0f) : 2750.0f)) : (iTemp43 ? (2750.0f - fTemp93) : 2700.0f)) : (iTemp41 ? ((200.0f * fTemp44) + 2700.0f) : 2900.0f)) : (iTemp39 ? (2900.0f - (200.0f * fTemp42)) : 2700.0f)) : (iTemp37 ? ((50.0f * fTemp40) + 2700.0f) : 2750.0f)) : (iTemp35 ? ((150.0f * fTemp38) + 2750.0f) : 2900.0f)) : (iTemp32 ? (2900.0f - (100.0f * fTemp36)) : 2800.0f)) : (iTemp30 ? ((150.0f * fTemp33) + 2800.0f) : 2950.0f)) : (iTemp28 ? (2950.0f - (120.0f * fTemp31)) : 2830.0f)) : (iTemp26 ? (2830.0f - (130.0f * fTemp29)) : 2700.0f)) : (iTemp24 ? (2700.0f - fTemp92) : 2650.0f)) : (iTemp22 ? (2650.0f - (50.0f * fTemp25)) : 2600.0f)) : (iTemp20 ? ((200.0f * fTemp23) + 2600.0f) : 2800.0f)) : (iTemp17 ? (2800.0f - (200.0f * fTemp21)) : 2600.0f)) : (iTemp18 ? (fTemp91 + 2600.0f) : 2700.0f));
			float fTemp95 = std::tan((fConst1 * fTemp94));
			float fTemp96 = (1.0f / fTemp95);
			float fTemp97 = (20.0f * fTemp38);
			float fTemp98 = (20.0f * fRec17[0]);
			float fTemp99 = (20.0f * fTemp47);
			float fTemp100 = (60.0f * fTemp58);
			float fTemp101 = ((iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? 120.0f : (iTemp59 ? (120.0f - fTemp75) : 100.0f)) : (iTemp57 ? ((70.0f * fTemp60) + 100.0f) : 170.0f)) : (iTemp55 ? (170.0f - fTemp100) : 110.0f)) : (iTemp53 ? (110.0f - fTemp86) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp45 ? (fTemp99 + 100.0f) : 120.0f)) : (iTemp43 ? (120.0f - fTemp98) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp35 ? (fTemp97 + 100.0f) : 120.0f)) : 120.0f) : (iTemp30 ? (120.0f - (20.0f * fTemp33)) : 100.0f)) : 100.0f) : (iTemp26 ? ((70.0f * fTemp29) + 100.0f) : 170.0f)) : (iTemp24 ? (170.0f - fTemp92) : 120.0f)) : (iTemp22 ? (120.0f - fTemp70) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fTemp94);
			float fTemp102 = (((fTemp96 + fTemp101) / fTemp95) + 1.0f);
			fRec19[0] = (fTemp15 - (((fRec19[2] * (((fTemp96 - fTemp101) / fTemp95) + 1.0f)) + (2.0f * (fRec19[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp95)))))) / fTemp102));
			float fTemp103 = (iTemp28 ? ((0.0293140002f * fTemp31) + 0.0501190014f) : 0.0794330016f);
			float fTemp104 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 0.100000001f : (iTemp63 ? (0.100000001f - (0.0683770031f * fTemp16)) : 0.0316229984f)) : 0.0316229984f) : (iTemp59 ? ((0.126865998f * fTemp62) + 0.0316229984f) : 0.158489004f)) : (iTemp57 ? (0.158489004f - (0.126865998f * fTemp60)) : 0.0316229984f)) : (iTemp55 ? ((0.323190004f * fTemp58) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iTemp51 ? (0.35481301f - (0.196324006f * fTemp54)) : 0.158489004f)) : (iTemp48 ? (0.158489004f - (0.0693639964f * fTemp52)) : 0.0891249999f)) : (iTemp46 ? (0.0891249999f - (0.0640060008f * fTemp49)) : 0.0251189992f)) : (iTemp45 ? ((0.0456760004f * fTemp47) + 0.0251189992f) : 0.0707949996f)) : (iTemp43 ? ((0.0550980009f * fRec17[0]) + 0.0707949996f) : 0.125892997f)) : (iTemp41 ? (0.125892997f - (0.0627970025f * fTemp44)) : 0.0630960017f)) : (iTemp39 ? (0.0630960017f - (0.0129770003f * fTemp42)) : 0.0501190014f)) : (iTemp37 ? ((0.020676f * fTemp40) + 0.0501190014f) : 0.0707949996f)) : (iTemp35 ? (0.0707949996f - (0.0456760004f * fTemp38)) : 0.0251189992f)) : (iTemp32 ? ((0.152709007f * fTemp36) + 0.0251189992f) : 0.177827999f)) : (iTemp30 ? (0.177827999f - (0.127709001f * fTemp33)) : 0.0501190014f)) : fTemp103) : (iTemp26 ? (0.0794330016f - (0.0616500005f * fTemp29)) : 0.0177829992f)) : (iTemp24 ? ((0.428900987f * fTemp27) + 0.0177829992f) : 0.446684003f)) : (iTemp22 ? (0.446684003f - (0.195494995f * fTemp25)) : 0.251188993f)) : (iTemp20 ? (0.251188993f - (0.125295997f * fTemp23)) : 0.125892997f)) : (iTemp17 ? ((0.125295997f * fTemp21) + 0.125892997f) : 0.251188993f)) : (iTemp18 ? (0.251188993f - (0.109935001f * fTemp19)) : 0.141253993f));
			float fTemp105 = (fTemp95 * fTemp102);
			float fTemp106 = (350.0f * fTemp56);
			float fTemp107 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 3500.0f : (iTemp63 ? (3500.0f - (200.0f * fTemp16)) : 3300.0f)) : (iTemp61 ? ((400.0f * fTemp64) + 3300.0f) : 3700.0f)) : (iTemp59 ? (3700.0f - (200.0f * fTemp62)) : 3500.0f)) : 3500.0f) : (iTemp55 ? (3500.0f - (1050.0f * fTemp58)) : 2450.0f)) : (iTemp53 ? (fTemp106 + 2450.0f) : 2800.0f)) : (iTemp51 ? ((250.0f * fTemp54) + 2800.0f) : 3050.0f)) : (iTemp48 ? (3050.0f - (450.0f * fTemp52)) : 2600.0f)) : (iTemp46 ? ((75.0f * fTemp49) + 2600.0f) : 2675.0f)) : (iTemp45 ? ((325.0f * fTemp47) + 2675.0f) : 3000.0f)) : 3000.0f) : (iTemp41 ? ((350.0f * fTemp44) + 3000.0f) : 3350.0f)) : (iTemp39 ? (3350.0f - (350.0f * fTemp42)) : 3000.0f)) : 3000.0f) : (iTemp35 ? ((900.0f * fTemp38) + 3000.0f) : 3900.0f)) : (iTemp32 ? (3900.0f - (300.0f * fTemp36)) : 3600.0f)) : (iTemp30 ? ((300.0f * fTemp33) + 3600.0f) : 3900.0f)) : (iTemp28 ? (3900.0f - (100.0f * fTemp31)) : 3800.0f)) : 3800.0f) : (iTemp24 ? (3800.0f - (900.0f * fTemp27)) : 2900.0f)) : (iTemp22 ? ((300.0f * fTemp25) + 2900.0f) : 3200.0f)) : (iTemp20 ? ((50.0f * fTemp23) + 3200.0f) : 3250.0f)) : (iTemp17 ? (3250.0f - (450.0f * fTemp21)) : 2800.0f)) : (iTemp18 ? (fTemp91 + 2800.0f) : 2900.0f));
			float fTemp108 = std::tan((fConst1 * fTemp107));
			float fTemp109 = (1.0f / fTemp108);
			float fTemp110 = ((iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 130.0f : (iTemp63 ? (fTemp76 + 130.0f) : 150.0f)) : 150.0f) : (iTemp59 ? (150.0f - fTemp75) : 130.0f)) : (iTemp57 ? ((50.0f * fTemp60) + 130.0f) : 180.0f)) : (iTemp55 ? (180.0f - fTemp100) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iTemp45 ? (fTemp85 + 120.0f) : 130.0f)) : (iTemp43 ? (130.0f - fTemp72) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp35 ? ((10.0f * fTemp38) + 120.0f) : 130.0f)) : (iTemp32 ? (fTemp71 + 130.0f) : 150.0f)) : (iTemp30 ? (150.0f - (30.0f * fTemp33)) : 120.0f)) : 120.0f) : (iTemp26 ? ((60.0f * fTemp29) + 120.0f) : 180.0f)) : (iTemp24 ? (180.0f - fTemp92) : 130.0f)) : (iTemp22 ? (130.0f - fTemp84) : 120.0f)) : 120.0f) : (iTemp17 ? (fTemp83 + 120.0f) : 130.0f)) : (iTemp18 ? (130.0f - (10.0f * fTemp19)) : 120.0f)) / fTemp107);
			float fTemp111 = (((fTemp109 + fTemp110) / fTemp108) + 1.0f);
			fRec20[0] = (fTemp15 - (((fRec20[2] * (((fTemp109 - fTemp110) / fTemp108) + 1.0f)) + (2.0f * (fRec20[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp108)))))) / fTemp111));
			float fTemp112 = (iTemp41 ? (0.100000001f - (0.0841509998f * fTemp44)) : 0.0158489998f);
			float fTemp113 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 0.0158489998f : (iTemp63 ? ((0.00193400006f * fTemp16) + 0.0158489998f) : 0.0177829992f)) : (iTemp61 ? (0.0177829992f - (0.00193400006f * fTemp64)) : 0.0158489998f)) : (iTemp59 ? ((0.0239620004f * fTemp62) + 0.0158489998f) : 0.0398110002f)) : (iTemp57 ? (0.0398110002f - (0.0298110005f * fTemp60)) : 0.00999999978f)) : (iTemp55 ? ((0.344812989f * fTemp58) + 0.00999999978f) : 0.35481301f)) : (iTemp53 ? (0.35481301f - (0.103624001f * fTemp56)) : 0.251188993f)) : (iTemp51 ? (0.251188993f - (0.171755999f * fTemp54)) : 0.0794330016f)) : (iTemp48 ? ((0.0205669999f * fTemp52) + 0.0794330016f) : 0.100000001f)) : (iTemp46 ? (0.100000001f - (0.0601890013f * fTemp49)) : 0.0398110002f)) : (iTemp45 ? ((0.0232849997f * fTemp47) + 0.0398110002f) : 0.0630960017f)) : (iTemp43 ? ((0.0369039997f * fRec17[0]) + 0.0630960017f) : 0.100000001f)) : fTemp112) : (iTemp39 ? ((0.0635839999f * fTemp42) + 0.0158489998f) : 0.0794330016f)) : (iTemp37 ? (0.0794330016f - (0.0478099994f * fTemp40)) : 0.0316229984f)) : (iTemp35 ? ((0.0683770031f * fTemp38) + 0.0316229984f) : 0.100000001f)) : (iTemp32 ? (0.100000001f - (0.0900000036f * fTemp36)) : 0.00999999978f)) : (iTemp30 ? ((0.0401189998f * fTemp33) + 0.00999999978f) : 0.0501190014f)) : fTemp103) : (iTemp26 ? (0.0794330016f - (0.0694330037f * fTemp29)) : 0.00999999978f)) : (iTemp24 ? ((0.388107002f * fTemp27) + 0.00999999978f) : 0.398106992f)) : (iTemp22 ? (0.398106992f - (0.198580995f * fTemp25)) : 0.199525997f)) : (iTemp20 ? (0.199525997f - (0.099526003f * fTemp23)) : 0.100000001f)) : (iTemp17 ? ((0.151188999f * fTemp21) + 0.100000001f) : 0.251188993f)) : (iTemp18 ? (0.251188993f - (0.0516630001f * fTemp19)) : 0.199525997f));
			float fTemp114 = (fTemp108 * fTemp111);
			float fTemp115 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? 4950.0f : (iTemp55 ? (4950.0f - (2200.0f * fTemp58)) : 2750.0f)) : (iTemp53 ? (fTemp106 + 2750.0f) : 3100.0f)) : (iTemp51 ? ((240.0f * fTemp54) + 3100.0f) : 3340.0f)) : (iTemp48 ? (3340.0f - (440.0f * fTemp52)) : 2900.0f)) : (iTemp46 ? (fTemp50 + 2900.0f) : 2950.0f)) : (iTemp45 ? ((400.0f * fTemp47) + 2950.0f) : 3350.0f)) : (iTemp43 ? (3350.0f - fTemp93) : 3300.0f)) : (iTemp41 ? ((290.0f * fTemp44) + 3300.0f) : 3590.0f)) : (iTemp39 ? (3590.0f - (290.0f * fTemp42)) : 3300.0f)) : (iTemp37 ? ((100.0f * fTemp40) + 3300.0f) : 3400.0f)) : (iTemp35 ? ((1550.0f * fTemp38) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iTemp24 ? (4950.0f - (1700.0f * fTemp27)) : 3250.0f)) : (iTemp22 ? ((330.0f * fTemp25) + 3250.0f) : 3580.0f)) : (iTemp20 ? (3580.0f - (40.0f * fTemp23)) : 3540.0f)) : (iTemp17 ? (3540.0f - (540.0f * fTemp21)) : 3000.0f)) : (iTemp18 ? ((300.0f * fTemp19) + 3000.0f) : 3300.0f));
			float fTemp116 = std::tan((fConst1 * fTemp115));
			float fTemp117 = (1.0f / fTemp116);
			float fTemp118 = ((iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? 140.0f : (iTemp63 ? ((60.0f * fTemp16) + 140.0f) : 200.0f)) : 200.0f) : (iTemp59 ? (200.0f - (65.0f * fTemp62)) : 135.0f)) : (iTemp57 ? ((65.0f * fTemp60) + 135.0f) : 200.0f)) : (iTemp55 ? (200.0f - (70.0f * fTemp58)) : 130.0f)) : (iTemp53 ? (130.0f - fTemp86) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp45 ? (fTemp99 + 120.0f) : 140.0f)) : (iTemp43 ? (140.0f - fTemp98) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp35 ? (fTemp97 + 120.0f) : 140.0f)) : (iTemp32 ? ((60.0f * fTemp36) + 140.0f) : 200.0f)) : (iTemp30 ? (200.0f - fTemp34) : 120.0f)) : 120.0f) : (iTemp26 ? ((80.0f * fTemp29) + 120.0f) : 200.0f)) : (iTemp24 ? (200.0f - (60.0f * fTemp27)) : 140.0f)) : (iTemp22 ? (140.0f - fTemp70) : 120.0f)) : 120.0f) : (iTemp17 ? ((15.0f * fTemp21) + 120.0f) : 135.0f)) : (iTemp18 ? (135.0f - (15.0f * fTemp19)) : 120.0f)) / fTemp115);
			float fTemp119 = (((fTemp117 + fTemp118) / fTemp116) + 1.0f);
			fRec21[0] = (fTemp15 - (((fRec21[2] * (((fTemp117 - fTemp118) / fTemp116) + 1.0f)) + (2.0f * (fRec21[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp116)))))) / fTemp119));
			float fTemp120 = (iTemp17 ? (iTemp20 ? (iTemp22 ? (iTemp24 ? (iTemp26 ? (iTemp28 ? (iTemp30 ? (iTemp32 ? (iTemp35 ? (iTemp37 ? (iTemp39 ? (iTemp41 ? (iTemp43 ? (iTemp45 ? (iTemp46 ? (iTemp48 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp57 ? (iTemp59 ? (iTemp61 ? 0.00100000005f : (iTemp59 ? ((0.000777999987f * fTemp62) + 0.00100000005f) : 0.00177800003f)) : (iTemp57 ? (0.00177800003f - (0.001147f * fTemp60)) : 0.000630999974f)) : (iTemp55 ? ((0.0993689969f * fTemp58) + 0.000630999974f) : 0.100000001f)) : (iTemp53 ? ((0.0258930009f * fTemp56) + 0.100000001f) : 0.125892997f)) : (iTemp51 ? (0.125892997f - (0.0860819966f * fTemp54)) : 0.0398110002f)) : (iTemp48 ? (0.0398110002f - (0.0298110005f * fTemp52)) : 0.00999999978f)) : (iTemp46 ? ((0.00584900007f * fTemp49) + 0.00999999978f) : 0.0158489998f)) : (iTemp45 ? (0.0158489998f - (0.00325999991f * fTemp47)) : 0.0125890002f)) : (iTemp43 ? ((0.0874110013f * fRec17[0]) + 0.0125890002f) : 0.100000001f)) : fTemp112) : (iTemp39 ? ((0.00410400005f * fTemp42) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iTemp35 ? (0.0199529994f - (0.0167909991f * fTemp38)) : 0.0031620001f)) : (iTemp32 ? (0.0031620001f - (0.00157700002f * fTemp36)) : 0.00158499996f)) : (iTemp30 ? ((0.00472499989f * fTemp33) + 0.00158499996f) : 0.00631000008f)) : (iTemp28 ? (0.00631000008f - (0.00314799999f * fTemp31)) : 0.0031620001f)) : (iTemp26 ? (0.0031620001f - (0.00216199993f * fTemp29)) : 0.00100000005f)) : (iTemp24 ? ((0.0784329996f * fTemp27) + 0.00100000005f) : 0.0794330016f)) : (iTemp22 ? ((0.0205669999f * fTemp25) + 0.0794330016f) : 0.100000001f)) : (iTemp20 ? (0.100000001f - (0.0683770031f * fTemp23)) : 0.0316229984f)) : (iTemp17 ? ((0.0184959993f * fTemp21) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
			float fTemp121 = (fTemp116 * fTemp119);
			fRec7[0] = ((fRec8[0] * ((fTemp10 * (((((fRec14[0] / fTemp79) + ((fRec18[0] * fTemp89) / fTemp90)) + ((fRec19[0] * fTemp104) / fTemp105)) + ((fRec20[0] * fTemp113) / fTemp114)) + ((fRec21[0] * fTemp120) / fTemp121))) + (((((fRec14[2] * (0.0f - (fTemp10 / fTemp79))) + (fRec18[2] * (0.0f - ((fTemp10 * fTemp89) / fTemp90)))) + (fRec19[2] * (0.0f - ((fTemp10 * fTemp104) / fTemp105)))) + (fRec20[2] * (0.0f - ((fTemp10 * fTemp113) / fTemp114)))) + (fRec21[2] * (0.0f - ((fTemp10 * fTemp120) / fTemp121)))))) - (fConst16 * ((fConst17 * fRec7[2]) + (fConst18 * fRec7[1]))));
			fRec22[0] = (iTemp8 ? fSlow66 : fRec22[1]);
			float fTemp122 = (((fRec7[2] + (fRec7[0] + (2.0f * fRec7[1]))) * std::max<float>(0.0f, std::min<float>(fTemp6, ((fConst19 * fTemp7) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec22[0] + -10.0f))));
			float fTemp123 = std::fabs((fConst6 * fTemp122));
			fRec25[0] = std::max<float>(fTemp123, ((fRec25[1] * fSlow73) + (fTemp123 * fSlow74)));
			fRec24[0] = ((fRec24[1] * fSlow70) + (fRec25[0] * fSlow75));
			fRec23[0] = ((fConst20 * fRec23[1]) + (fSlow67 * std::max<float>(((20.0f * std::log10(fRec24[0])) - fSlow76), 0.0f)));
			float fTemp124 = (fConst6 * (fTemp122 * std::pow(10.0f, (0.0500000007f * fRec23[0]))));
			fRec26[0] = (fTemp124 - (fSlow80 * ((fSlow81 * fRec26[2]) + (fSlow82 * fRec26[1]))));
			float fTemp125 = (fSlow57 * fRec6[1]);
			fRec6[0] = (fTemp124 - ((fSlow80 * (fRec26[2] + (fRec26[0] + (2.0f * fRec26[1])))) + (((fRec6[2] * fSlow90) + fTemp125) / fSlow91)));
			float fTemp126 = (fSlow54 * fRec5[1]);
			fRec5[0] = ((((fTemp125 + (fRec6[0] * fSlow93)) + (fRec6[2] * fSlow94)) / fSlow91) - (((fRec5[2] * fSlow102) + fTemp126) / fSlow103));
			fRec4[0] = ((((fTemp126 + (fRec5[0] * fSlow105)) + (fRec5[2] * fSlow106)) / fSlow103) - (fSlow107 * ((fSlow108 * fRec4[2]) + (fSlow109 * fRec4[1]))));
			float fTemp127 = ((fSlow7 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + (fSlow51 * (fRec4[2] + (fRec4[0] + (2.0f * fRec4[1])))));
			fVec4[0] = fSlow121;
			float fTemp128 = (fSlow121 - fVec4[1]);
			fVec5[0] = fSlow122;
			float fTemp129 = (0.0f - (fSlow121 + fVec5[1]));
			float fTemp130 = ((fTemp128 * float((fTemp128 > 0.0f))) + (fTemp129 * float((fTemp129 > 0.0f))));
			fVec6[0] = fTemp130;
			iRec31[0] = (((iRec31[1] + (iRec31[1] > 0)) * (fTemp130 <= fVec6[1])) + (fTemp130 > fVec6[1]));
			float fTemp131 = float(iRec31[0]);
			fRec30[0] = ((fConst22 * fRec30[1]) + (fConst23 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>((fConst10 * fTemp131), ((fConst19 * (fConst9 - fTemp131)) + 1.0f))))));
			int iTemp132 = int(fTemp130);
			fRec32[0] = (iTemp132 ? fSlow66 : fRec32[1]);
			float fTemp133 = (fRec30[0] * std::pow(10.0f, (0.144444451f * (fRec32[0] + -10.0f))));
			fRec35[0] = (iTemp132 ? fSlow123 : fRec35[1]);
			iRec34[0] = ((iVec0[1] + iRec34[1]) % int((fConst0 / fRec35[0])));
			float fTemp134 = float(iRec34[0]);
			int iTemp135 = (fTemp130 > 0.0f);
			float fTemp136 = (0.144717798f * (iTemp135 ? 0.00100000005f : 1.60000002f));
			int iTemp137 = (std::fabs(fTemp136) < 1.1920929e-07f);
			float fTemp138 = (iTemp137 ? 0.0f : std::exp((0.0f - (fConst13 / (iTemp137 ? 1.0f : fTemp136)))));
			fRec37[0] = ((fRec37[1] * fTemp138) + ((iTemp135 ? fTemp130 : 0.400000006f) * (1.0f - fTemp138)));
			fRec36[0] = ((fConst7 * fRec36[1]) + (fSlow124 * fRec37[0]));
			float fTemp139 = (fRec36[0] + 0.00999999978f);
			float fTemp140 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec35[0] * fTemp139) * (1.0f - (0.000500000024f * std::min<float>(fRec35[0], 1000.0f)))))))));
			float fTemp141 = (1.0f / fTemp140);
			float fTemp142 = (((fTemp141 + 1.41421354f) / fTemp140) + 1.0f);
			fRec33[0] = ((2.0f * (((float((fTemp134 < (fConst24 / fRec35[0]))) + -0.5f) + (float((fTemp134 < (fConst25 / fRec35[0]))) + -0.5f)) + (float((fTemp134 < (fConst26 / fRec35[0]))) + -0.5f))) - (((fRec33[2] * (((fTemp141 + -1.41421354f) / fTemp140) + 1.0f)) + (2.0f * (fRec33[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp140)))))) / fTemp142));
			fRec40[0] = (iTemp132 ? fSlow125 : fRec40[1]);
			iRec39[0] = ((iVec0[1] + iRec39[1]) % int((fConst0 / fRec40[0])));
			float fTemp143 = float(iRec39[0]);
			float fTemp144 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fRec40[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec40[0], 1000.0f)))))))));
			float fTemp145 = (1.0f / fTemp144);
			float fTemp146 = (((fTemp145 + 1.41421354f) / fTemp144) + 1.0f);
			fRec38[0] = ((2.0f * (((float((fTemp143 < (fConst24 / fRec40[0]))) + -0.5f) + (float((fTemp143 < (fConst25 / fRec40[0]))) + -0.5f)) + (float((fTemp143 < (fConst26 / fRec40[0]))) + -0.5f))) - (((fRec38[2] * (((fTemp145 + -1.41421354f) / fTemp144) + 1.0f)) + (2.0f * (fRec38[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp144)))))) / fTemp146));
			fRec43[0] = (iTemp132 ? fSlow126 : fRec43[1]);
			iRec42[0] = ((iVec0[1] + iRec42[1]) % int((fConst0 / fRec43[0])));
			float fTemp147 = float(iRec42[0]);
			float fTemp148 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fRec43[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec43[0], 1000.0f)))))))));
			float fTemp149 = (1.0f / fTemp148);
			float fTemp150 = (((fTemp149 + 1.41421354f) / fTemp148) + 1.0f);
			fRec41[0] = ((2.0f * (((float((fTemp147 < (fConst24 / fRec43[0]))) + -0.5f) + (float((fTemp147 < (fConst25 / fRec43[0]))) + -0.5f)) + (float((fTemp147 < (fConst26 / fRec43[0]))) + -0.5f))) - (((fRec41[2] * (((fTemp149 + -1.41421354f) / fTemp148) + 1.0f)) + (2.0f * (fRec41[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp148)))))) / fTemp150));
			fRec46[0] = (iTemp132 ? fSlow127 : fRec46[1]);
			iRec45[0] = ((iVec0[1] + iRec45[1]) % int((fConst0 / fRec46[0])));
			float fTemp151 = float(iRec45[0]);
			float fTemp152 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fRec46[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec46[0], 1000.0f)))))))));
			float fTemp153 = (1.0f / fTemp152);
			float fTemp154 = (((fTemp153 + 1.41421354f) / fTemp152) + 1.0f);
			fRec44[0] = ((2.0f * (((float((fTemp151 < (fConst24 / fRec46[0]))) + -0.5f) + (float((fTemp151 < (fConst25 / fRec46[0]))) + -0.5f)) + (float((fTemp151 < (fConst26 / fRec46[0]))) + -0.5f))) - (((fRec44[2] * (((fTemp153 + -1.41421354f) / fTemp152) + 1.0f)) + (2.0f * (fRec44[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp152)))))) / fTemp154));
			float fTemp155 = (fTemp133 * (((((fRec33[2] + (fRec33[0] + (2.0f * fRec33[1]))) / fTemp142) + ((fRec38[2] + (fRec38[0] + (2.0f * fRec38[1]))) / fTemp146)) + ((fRec41[2] + (fRec41[0] + (2.0f * fRec41[1]))) / fTemp150)) + ((fRec44[2] + (fRec44[0] + (2.0f * fRec44[1]))) / fTemp154)));
			float fTemp156 = std::fabs((0.25f * fTemp155));
			fRec49[0] = std::max<float>(fTemp156, ((fRec49[1] * fSlow134) + (fTemp156 * fSlow135)));
			fRec48[0] = ((fRec48[1] * fSlow131) + (fRec49[0] * fSlow136));
			fRec47[0] = ((fConst20 * fRec47[1]) + (fSlow128 * std::max<float>(((20.0f * std::log10(fRec48[0])) - fSlow137), 0.0f)));
			float fTemp157 = (0.25f * (fTemp155 * std::pow(10.0f, (0.0500000007f * fRec47[0]))));
			fRec50[0] = (fTemp157 - (fSlow141 * ((fSlow142 * fRec50[2]) + (fSlow143 * fRec50[1]))));
			float fTemp158 = (fSlow120 * fRec29[1]);
			fRec29[0] = (fTemp157 - ((fSlow141 * (fRec50[2] + (fRec50[0] + (2.0f * fRec50[1])))) + (((fRec29[2] * fSlow151) + fTemp158) / fSlow152)));
			float fTemp159 = (fSlow117 * fRec28[1]);
			fRec28[0] = ((((fTemp158 + (fRec29[0] * fSlow154)) + (fRec29[2] * fSlow155)) / fSlow152) - (((fRec28[2] * fSlow163) + fTemp159) / fSlow164));
			fRec27[0] = ((((fTemp159 + (fRec28[0] * fSlow166)) + (fRec28[2] * fSlow167)) / fSlow164) - (fSlow168 * ((fSlow169 * fRec27[2]) + (fSlow170 * fRec27[1]))));
			fRec54[0] = (-1.0f * (fSlow186 * ((fSlow187 * fRec54[2]) + (fSlow188 * fRec54[1]))));
			float fTemp160 = (fSlow182 * fRec53[1]);
			fRec53[0] = (0.0f - ((fSlow186 * (fRec54[2] + (fRec54[0] + (2.0f * fRec54[1])))) + (((fRec53[2] * fSlow196) + fTemp160) / fSlow197)));
			float fTemp161 = (fSlow179 * fRec52[1]);
			fRec52[0] = ((((fTemp160 + (fRec53[0] * fSlow199)) + (fRec53[2] * fSlow200)) / fSlow197) - (((fRec52[2] * fSlow208) + fTemp161) / fSlow209));
			fRec51[0] = ((((fTemp161 + (fRec52[0] * fSlow211)) + (fRec52[2] * fSlow212)) / fSlow209) - (fSlow213 * ((fSlow214 * fRec51[2]) + (fSlow215 * fRec51[1]))));
			float fTemp162 = (fRec51[2] + (fRec51[0] + (2.0f * fRec51[1])));
			float fTemp163 = ((fTemp127 + (fSlow114 * (fRec27[2] + (fRec27[0] + (2.0f * fRec27[1]))))) + (fSlow176 * fTemp162));
			float fTemp164 = std::fabs(fTemp163);
			fRec57[0] = std::max<float>(fTemp164, ((fConst29 * fRec57[1]) + (fConst30 * fTemp164)));
			fRec56[0] = ((fConst27 * fRec56[1]) + (fConst28 * fRec57[0]));
			fRec55[0] = ((fConst20 * fRec55[1]) + (fConst21 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec56[0]) + 1.0f)), 0.0f)))));
			float fTemp165 = (fTemp163 * std::pow(10.0f, (0.0500000007f * fRec55[0])));
			float fTemp166 = std::fabs(fTemp165);
			fRec60[0] = std::max<float>(fTemp166, ((fConst33 * fRec60[1]) + (fConst34 * fTemp166)));
			fRec59[0] = ((fConst31 * fRec59[1]) + (fConst32 * fRec60[0]));
			fRec58[0] = ((fConst20 * fRec58[1]) + (fConst21 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec59[0])) + 5.0f), 0.0f)))));
			float fTemp167 = (fTemp165 * std::pow(10.0f, (0.0500000007f * fRec58[0])));
			float fTemp168 = std::fabs((fSlow2 * fTemp167));
			fRec63[0] = std::max<float>(fTemp168, ((fConst20 * fRec63[1]) + (fConst21 * fTemp168)));
			fRec62[0] = ((fConst35 * fRec62[1]) + (fConst36 * fRec63[0]));
			fRec61[0] = ((fConst20 * fRec61[1]) + (fConst21 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec62[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT((fSlow1 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp167 * std::pow(10.0f, (0.0500000007f * fRec61[0]))))))));
			float fTemp169 = (fRec35[0] + 0.5f);
			iRec68[0] = ((iVec0[1] + iRec68[1]) % int((fConst0 / fTemp169)));
			float fTemp170 = float(iRec68[0]);
			float fTemp171 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fTemp169) * (1.0f - (0.000500000024f * std::min<float>(fTemp169, 1000.0f)))))))));
			float fTemp172 = (1.0f / fTemp171);
			float fTemp173 = (((fTemp172 + 1.41421354f) / fTemp171) + 1.0f);
			fRec67[0] = ((2.0f * (((float((fTemp170 < (fConst24 / fTemp169))) + -0.5f) + (float((fTemp170 < (fConst25 / fTemp169))) + -0.5f)) + (float((fTemp170 < (fConst26 / fTemp169))) + -0.5f))) - (((fRec67[2] * (((fTemp172 + -1.41421354f) / fTemp171) + 1.0f)) + (2.0f * (fRec67[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp171)))))) / fTemp173));
			float fTemp174 = (fRec40[0] + 0.5f);
			iRec70[0] = ((iVec0[1] + iRec70[1]) % int((fConst0 / fTemp174)));
			float fTemp175 = float(iRec70[0]);
			float fTemp176 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fTemp174) * (1.0f - (0.000500000024f * std::min<float>(fTemp174, 1000.0f)))))))));
			float fTemp177 = (1.0f / fTemp176);
			float fTemp178 = (((fTemp177 + 1.41421354f) / fTemp176) + 1.0f);
			fRec69[0] = ((2.0f * (((float((fTemp175 < (fConst24 / fTemp174))) + -0.5f) + (float((fTemp175 < (fConst25 / fTemp174))) + -0.5f)) + (float((fTemp175 < (fConst26 / fTemp174))) + -0.5f))) - (((fRec69[2] * (((fTemp177 + -1.41421354f) / fTemp176) + 1.0f)) + (2.0f * (fRec69[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp176)))))) / fTemp178));
			float fTemp179 = (fRec43[0] + 0.5f);
			iRec72[0] = ((iVec0[1] + iRec72[1]) % int((fConst0 / fTemp179)));
			float fTemp180 = float(iRec72[0]);
			float fTemp181 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fTemp179) * (1.0f - (0.000500000024f * std::min<float>(fTemp179, 1000.0f)))))))));
			float fTemp182 = (1.0f / fTemp181);
			float fTemp183 = (((fTemp182 + 1.41421354f) / fTemp181) + 1.0f);
			fRec71[0] = ((2.0f * (((float((fTemp180 < (fConst24 / fTemp179))) + -0.5f) + (float((fTemp180 < (fConst25 / fTemp179))) + -0.5f)) + (float((fTemp180 < (fConst26 / fTemp179))) + -0.5f))) - (((fRec71[2] * (((fTemp182 + -1.41421354f) / fTemp181) + 1.0f)) + (2.0f * (fRec71[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp181)))))) / fTemp183));
			float fTemp184 = (fRec46[0] + 0.5f);
			iRec74[0] = ((iVec0[1] + iRec74[1]) % int((fConst0 / fTemp184)));
			float fTemp185 = float(iRec74[0]);
			float fTemp186 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp139 * fTemp184) * (1.0f - (0.000500000024f * std::min<float>(fTemp184, 1000.0f)))))))));
			float fTemp187 = (1.0f / fTemp186);
			float fTemp188 = (((fTemp187 + 1.41421354f) / fTemp186) + 1.0f);
			fRec73[0] = ((2.0f * (((float((fTemp185 < (fConst24 / fTemp184))) + -0.5f) + (float((fTemp185 < (fConst25 / fTemp184))) + -0.5f)) + (float((fTemp185 < (fConst26 / fTemp184))) + -0.5f))) - (((fRec73[2] * (((fTemp187 + -1.41421354f) / fTemp186) + 1.0f)) + (2.0f * (fRec73[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp186)))))) / fTemp188));
			float fTemp189 = (fTemp133 * (((((fRec67[2] + (fRec67[0] + (2.0f * fRec67[1]))) / fTemp173) + ((fRec69[2] + (fRec69[0] + (2.0f * fRec69[1]))) / fTemp178)) + ((fRec71[2] + (fRec71[0] + (2.0f * fRec71[1]))) / fTemp183)) + ((fRec73[2] + (fRec73[0] + (2.0f * fRec73[1]))) / fTemp188)));
			float fTemp190 = std::fabs((0.25f * fTemp189));
			fRec77[0] = std::max<float>(fTemp190, ((fSlow134 * fRec77[1]) + (fSlow135 * fTemp190)));
			fRec76[0] = ((fSlow131 * fRec76[1]) + (fRec77[0] * fSlow136));
			fRec75[0] = ((fConst20 * fRec75[1]) + (fSlow128 * std::max<float>(((20.0f * std::log10(fRec76[0])) - fSlow137), 0.0f)));
			float fTemp191 = (0.25f * (fTemp189 * std::pow(10.0f, (0.0500000007f * fRec75[0]))));
			fRec78[0] = (fTemp191 - (fSlow141 * ((fSlow142 * fRec78[2]) + (fSlow143 * fRec78[1]))));
			float fTemp192 = (fSlow120 * fRec66[1]);
			fRec66[0] = (fTemp191 - ((fSlow141 * (fRec78[2] + (fRec78[0] + (2.0f * fRec78[1])))) + (((fSlow151 * fRec66[2]) + fTemp192) / fSlow152)));
			float fTemp193 = (fSlow117 * fRec65[1]);
			fRec65[0] = ((((fTemp192 + (fRec66[0] * fSlow154)) + (fSlow155 * fRec66[2])) / fSlow152) - (((fSlow163 * fRec65[2]) + fTemp193) / fSlow164));
			fRec64[0] = ((((fTemp193 + (fRec65[0] * fSlow166)) + (fSlow167 * fRec65[2])) / fSlow164) - (fSlow168 * ((fSlow169 * fRec64[2]) + (fSlow170 * fRec64[1]))));
			float fTemp194 = ((fSlow114 * (fRec64[2] + (fRec64[0] + (2.0f * fRec64[1])))) + (fTemp127 + (fSlow217 * fTemp162)));
			float fTemp195 = std::fabs(fTemp194);
			fRec81[0] = std::max<float>(fTemp195, ((fConst29 * fRec81[1]) + (fConst30 * fTemp195)));
			fRec80[0] = ((fConst27 * fRec80[1]) + (fConst28 * fRec81[0]));
			fRec79[0] = ((fConst20 * fRec79[1]) + (fConst21 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec80[0]) + 1.0f)), 0.0f)))));
			float fTemp196 = (fTemp194 * std::pow(10.0f, (0.0500000007f * fRec79[0])));
			float fTemp197 = std::fabs(fTemp196);
			fRec84[0] = std::max<float>(fTemp197, ((fConst33 * fRec84[1]) + (fConst34 * fTemp197)));
			fRec83[0] = ((fConst31 * fRec83[1]) + (fConst32 * fRec84[0]));
			fRec82[0] = ((fConst20 * fRec82[1]) + (fConst21 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec83[0])) + 5.0f), 0.0f)))));
			float fTemp198 = (fTemp196 * std::pow(10.0f, (0.0500000007f * fRec82[0])));
			float fTemp199 = std::fabs((fSlow2 * fTemp198));
			fRec87[0] = std::max<float>(fTemp199, ((fConst20 * fRec87[1]) + (fConst21 * fTemp199)));
			fRec86[0] = ((fConst35 * fRec86[1]) + (fConst36 * fRec87[0]));
			fRec85[0] = ((fConst20 * fRec85[1]) + (fConst21 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec86[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT((fSlow216 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp198 * std::pow(10.0f, (0.0500000007f * fRec85[0]))))))));
			iVec0[1] = iVec0[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			iRec9[1] = iRec9[0];
			fRec8[1] = fRec8[0];
			fRec10[1] = fRec10[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec15[1] = fRec15[0];
			fRec17[1] = fRec17[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec22[1] = fRec22[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			iRec31[1] = iRec31[0];
			fRec30[1] = fRec30[0];
			fRec32[1] = fRec32[0];
			fRec35[1] = fRec35[0];
			iRec34[1] = iRec34[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec40[1] = fRec40[0];
			iRec39[1] = iRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec43[1] = fRec43[0];
			iRec42[1] = iRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec46[1] = fRec46[0];
			iRec45[1] = iRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			iRec68[1] = iRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			iRec70[1] = iRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			iRec72[1] = iRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			iRec74[1] = iRec74[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec81[1] = fRec81[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec85[1] = fRec85[0];
		}
	}

};

#endif
