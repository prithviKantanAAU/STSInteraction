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
	
	int iRec66[2];
	
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
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			iRec66[l30] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec66[0] = (iRec66[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec66[0] + -1))));
			iRec66[1] = iRec66[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

const static float fmydspSIG1Wave0[350] = {0.525285006f,0.814173996f,0.483260989f,0.296745002f,0.975055993f,0.472243994f,0.409500986f,0.425363988f,0.687559009f,0.28838101f,0.309285015f,0.123053998f,0.286332995f,0.576705992f,0.908321977f,0.626973987f,0.0801851973f,0.309834987f,0.451429993f,0.132844999f,0.470634997f,0.417008013f,0.265112013f,0.075280197f,0.463470012f,0.471810013f,0.275323987f,0.547026992f,0.512519002f,0.394077986f,0.595404029f,0.941305995f,0.392500997f,0.381435007f,0.391232014f,0.118923999f,0.339495003f,0.101420999f,0.241754994f,0.0873254985f,0.37894401f,0.637705028f,0.171946004f,0.149858996f,0.233290002f,0.541809976f,1.0f,0.115553997f,0.244172007f,0.574329019f,0.606171012f,0.93839699f,0.392554998f,0.277359009f,0.86857003f,0.432489008f,0.408856004f,0.407932013f,0.299814999f,0.256659001f,0.549571991f,0.406347007f,0.312330991f,0.62757802f,0.670167029f,0.524648011f,0.406926006f,0.637524009f,0.555836976f,1.0f,0.818979025f,0.705347002f,0.678140998f,0.427381992f,0.674404025f,0.636105001f,0.643634975f,0.699136019f,0.836201012f,0.613084972f,0.31901899f,0.725259006f,0.545518994f,0.479860991f,0.498360008f,0.488653988f,0.861671984f,0.314287007f,0.671051979f,0.531904995f,0.421781003f,0.81506598f,0.772032022f,0.488721997f,0.0896674022f,0.291285992f,0.65872997f,0.635631979f,0.679356992f,0.459497005f,0.360240012f,0.58228898f,0.650605023f,0.490949988f,0.381909996f,0.157260999f,0.479624003f,0.477490991f,0.174435005f,0.0130939996f,0.879113019f,0.608069003f,0.268877f,0.604479015f,0.245130002f,0.170506999f,0.292887986f,0.545849025f,0.476646006f,0.922316015f,0.669192016f,0.578094006f,0.578796983f,0.311396003f,0.601209998f,0.54995501f,1.0f,0.66573f,0.980114996f,0.537847996f,0.0740531012f,0.252472013f,0.25575f,0.223974004f,0.0865103006f,0.138209f,0.198623002f,0.0453034006f,0.432453007f,0.292407006f,0.394410014f,0.857658982f,0.271667987f,0.201545f,0.583993971f,0.0602377988f,0.190617993f,0.849505007f,0.975542009f,0.173140004f,0.206471995f,0.344792992f,0.761011004f,0.558125019f,0.117245004f,0.0338485017f,0.337597013f,0.336645991f,0.174253002f,0.230169997f,0.934872985f,0.593647003f,0.393225014f,0.683704019f,0.056609299f,0.0405011997f,0.148972005f,0.338721991f,0.283419013f,0.394006997f,0.237474993f,0.26996401f,0.428312987f,0.177498996f,0.462585002f,0.443962991f,0.981792986f,0.408239007f,0.676527023f,0.402864993f,0.0163302999f,0.0515113994f,0.341390014f,0.311134994f,0.613276005f,0.805884004f,0.953289986f,0.406091005f,0.578705013f,0.386785001f,0.434103012f,0.775259972f,1.0f,0.635909021f,0.78205198f,0.0137182996f,0.0387725011f,0.618964016f,0.857070982f,0.131522f,0.184988007f,0.299495012f,0.789211988f,0.603114009f,0.0704988986f,0.0129338996f,0.252481014f,0.254121006f,0.189206004f,0.357713014f,0.950308025f,0.552573025f,0.466453999f,0.777360022f,0.0307886004f,0.0251943003f,0.378886014f,0.740186989f,0.247637004f,0.235201001f,0.493045002f,0.517849982f,0.883953989f,0.429473013f,0.409433007f,0.415266007f,0.940198004f,0.282334f,0.437889993f,0.375384986f,0.0157366004f,0.0171763003f,0.485554993f,0.461014986f,0.858958006f,0.907990992f,0.935190976f,0.375510007f,1.0f,0.585493028f,0.269980997f,0.423052996f,0.666067004f,0.435090005f,0.79025197f,0.00889586005f,0.0208844002f,0.449734986f,0.790808022f,0.159856007f,0.0895989984f,0.161546007f,0.528168023f,0.380641997f,0.0206275992f,0.0072642602f,0.0315352008f,0.0315840989f,0.197649002f,0.475057006f,0.517232001f,0.360922009f,0.421204001f,0.631340027f,0.00952139031f,0.0161049999f,0.499615014f,0.922958016f,0.214983001f,0.0655141026f,0.503970027f,0.514847994f,1.0f,0.483619004f,0.254027009f,0.228371993f,0.436105013f,0.233125001f,0.152242005f,0.279513001f,0.00906739011f,0.0132331997f,0.45125699f,0.388565987f,0.737226009f,0.479378015f,0.233036995f,0.103767f,0.845609009f,0.644127011f,0.261359006f,0.371457011f,0.527229011f,0.381372988f,0.334491998f,0.00833749026f,0.00861981977f,0.255919009f,0.254197001f,0.0872332975f,0.0461511984f,0.113017999f,0.345986009f,0.236343995f,0.0107800001f,0.00816505961f,0.405180991f,0.387180001f,0.343681008f,0.816492975f,0.25908199f,0.211906001f,0.432455003f,0.696886003f,0.00576223992f,0.0131310001f,0.455969006f,0.81160903f,0.426544011f,0.128489003f,0.215937003f,0.233934f,0.723070025f,0.351622999f,0.394230992f,0.323765993f,0.168803006f,0.276932001f,0.264683992f,0.227703005f,0.0068093501f,0.0170703009f,0.603017986f,0.476460993f,0.585924983f,0.716960013f,1.0f,0.576526999f,0.475524008f,0.447322011f,0.356902003f,0.597572982f,0.697246015f,0.505333006f,0.285421014f,0.0147193f,0.0141618f,0.136188f,0.0336536989f,0.216436997f};
class mydspSIG1 {
	
  private:
	
	int fmydspSIG1Wave0_idx;
	
  public:
	
	int getNumInputsmydspSIG1() {
		return 0;
	}
	int getNumOutputsmydspSIG1() {
		return 1;
	}
	int getInputRatemydspSIG1(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatemydspSIG1(int channel) {
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
	
	void instanceInitmydspSIG1(int sample_rate) {
		fmydspSIG1Wave0_idx = 0;
	}
	
	void fillmydspSIG1(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			table[i] = fmydspSIG1Wave0[fmydspSIG1Wave0_idx];
			fmydspSIG1Wave0_idx = ((1 + fmydspSIG1Wave0_idx) % 350);
		}
	}

};

static mydspSIG1* newmydspSIG1() { return (mydspSIG1*)new mydspSIG1(); }
static void deletemydspSIG1(mydspSIG1* dsp) { delete dsp; }

static float mydsp_faustpower2_f(float value) {
	return (value * value);
}
static float mydsp_faustpower6_f(float value) {
	return (((((value * value) * value) * value) * value) * value);
}
static float ftbl0mydspSIG0[65536];
static float ftbl1mydspSIG1[350];

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	int iVec0[2];
	FAUSTFLOAT fVslider0;
	int fSampleRate;
	float fConst0;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst14;
	float fConst15;
	float fRec12[2];
	float fRec11[2];
	int IOTA;
	float fVec1[32768];
	int iConst17;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	float fConst18;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	FAUSTFLOAT fHslider6;
	float fVec2[2];
	float fVec3[2];
	float fVec4[2];
	int iRec17[2];
	float fConst23;
	float fRec16[2];
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fRec20[2];
	int iRec19[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec22[2];
	float fRec21[2];
	float fRec18[3];
	FAUSTFLOAT fHslider10;
	float fRec25[2];
	int iRec24[2];
	float fRec23[3];
	FAUSTFLOAT fHslider11;
	float fRec28[2];
	int iRec27[2];
	float fRec26[3];
	FAUSTFLOAT fHslider12;
	float fRec31[2];
	int iRec30[2];
	float fRec29[3];
	float fConst30;
	float fConst31;
	FAUSTFLOAT fHslider13;
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
	float fRec34[2];
	float fRec33[2];
	FAUSTFLOAT fHslider16;
	float fRec32[2];
	FAUSTFLOAT fHslider17;
	FAUSTFLOAT fHslider18;
	float fRec35[3];
	FAUSTFLOAT fHslider19;
	FAUSTFLOAT fHslider20;
	float fConst32;
	float fRec15[3];
	FAUSTFLOAT fHslider21;
	FAUSTFLOAT fHslider22;
	float fRec14[3];
	float fRec13[3];
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	FAUSTFLOAT fHslider26;
	FAUSTFLOAT fHslider27;
	float fConst33;
	float fConst34;
	float fConst35;
	float fRec67[2];
	float fRec65[2];
	FAUSTFLOAT fHslider28;
	float fVec5[2];
	float fVec6[2];
	FAUSTFLOAT fHslider29;
	float fRec68[2];
	float fRec61[2];
	FAUSTFLOAT fHslider30;
	float fRec69[2];
	float fRec71[4];
	float fRec72[2048];
	float fConst36;
	float fVec7[2];
	float fVec8[3];
	float fRec70[2048];
	float fRec57[2];
	float fRec53[2];
	float fRec49[2048];
	float fRec47[4];
	float fRec42[2048];
	float fRec40[2];
	FAUSTFLOAT fHslider31;
	float fRec73[2];
	float fRec39[3];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	FAUSTFLOAT fHslider34;
	float fRec76[2];
	float fRec75[2];
	FAUSTFLOAT fHslider35;
	float fRec74[2];
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fRec77[3];
	FAUSTFLOAT fHslider38;
	FAUSTFLOAT fHslider39;
	float fRec38[3];
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fHslider41;
	float fRec37[3];
	float fRec36[3];
	FAUSTFLOAT fHslider42;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider43;
	FAUSTFLOAT fHslider44;
	FAUSTFLOAT fHslider45;
	FAUSTFLOAT fHslider46;
	FAUSTFLOAT fHslider47;
	int iRec84[2];
	float fRec83[3];
	float fRec82[3];
	float fConst37;
	FAUSTFLOAT fHslider48;
	float fVec9[2];
	float fVec10[2];
	float fVec11[2];
	int iRec85[2];
	float fConst39;
	FAUSTFLOAT fHslider49;
	float fRec86[2];
	float fConst40;
	float fRec81[3];
	float fConst42;
	float fConst43;
	float fRec87[3];
	float fConst45;
	float fConst46;
	float fRec88[3];
	float fConst48;
	float fConst49;
	float fRec89[3];
	float fConst51;
	float fConst52;
	float fRec90[3];
	float fConst54;
	float fConst55;
	float fRec91[3];
	float fConst57;
	float fConst58;
	float fRec92[3];
	float fConst60;
	float fConst61;
	float fRec93[3];
	float fConst63;
	float fConst64;
	float fRec94[3];
	float fConst66;
	float fConst67;
	float fRec95[3];
	float fConst69;
	float fConst70;
	float fRec96[3];
	float fConst72;
	float fConst73;
	float fRec97[3];
	float fConst75;
	float fConst76;
	float fRec98[3];
	float fConst78;
	float fConst79;
	float fRec99[3];
	float fConst81;
	float fConst82;
	float fRec100[3];
	float fConst84;
	float fConst85;
	float fRec101[3];
	float fConst87;
	float fConst88;
	float fRec102[3];
	float fConst90;
	float fConst91;
	float fRec103[3];
	float fConst93;
	float fConst94;
	float fRec104[3];
	float fConst96;
	float fConst97;
	float fRec105[3];
	FAUSTFLOAT fHslider50;
	FAUSTFLOAT fHslider51;
	FAUSTFLOAT fHslider52;
	float fRec108[2];
	float fRec107[2];
	FAUSTFLOAT fHslider53;
	float fRec106[2];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec109[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec80[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fHslider59;
	float fRec79[3];
	float fRec78[3];
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	FAUSTFLOAT fHslider63;
	FAUSTFLOAT fHslider64;
	float fConst101;
	FAUSTFLOAT fHslider65;
	float fConst102;
	float fConst103;
	FAUSTFLOAT fHslider66;
	float fConst104;
	float fRec115[2];
	float fRec114[2];
	float fConst105;
	FAUSTFLOAT fHslider67;
	float fRec116[2];
	float fRec118[2];
	FAUSTFLOAT fHslider68;
	float fRec117[3];
	float fRec120[3];
	float fRec121[3];
	float fRec122[3];
	float fRec123[3];
	float fConst106;
	float fConst107;
	float fConst108;
	float fRec113[3];
	FAUSTFLOAT fHslider69;
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec126[2];
	float fRec125[2];
	FAUSTFLOAT fHslider72;
	float fRec124[2];
	FAUSTFLOAT fHslider73;
	FAUSTFLOAT fHslider74;
	float fRec127[3];
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	float fRec112[3];
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	float fRec111[3];
	float fRec110[3];
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	FAUSTFLOAT fHslider82;
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	int iRec146[2];
	float fRec152[2];
	FAUSTFLOAT fHslider85;
	float fConst109;
	float fRec153[2];
	float fConst110;
	float fConst111;
	FAUSTFLOAT fHslider86;
	float fVec12[2];
	float fVec13[2];
	FAUSTFLOAT fHslider87;
	float fRec157[2];
	float fRec156[2];
	float fRec155[2];
	float fRec154[2];
	float fConst115;
	float fConst116;
	float fConst117;
	float fConst118;
	float fRec158[3];
	float fRec159[2];
	float fRec160[2048];
	float fConst119;
	FAUSTFLOAT fHslider88;
	float fVec14[2];
	float fVec15[2048];
	float fVec16[2];
	float fRec151[2];
	float fRec142[2048];
	float fRec138[2];
	float fRec134[2048];
	float fRec132[2];
	float fRec133[2];
	float fRec131[2];
	FAUSTFLOAT fHslider89;
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	float fRec163[2];
	float fRec162[2];
	FAUSTFLOAT fHslider92;
	float fRec161[2];
	FAUSTFLOAT fHslider93;
	FAUSTFLOAT fHslider94;
	float fRec164[3];
	FAUSTFLOAT fHslider95;
	FAUSTFLOAT fHslider96;
	float fRec130[3];
	FAUSTFLOAT fHslider97;
	FAUSTFLOAT fHslider98;
	float fRec129[3];
	float fRec128[3];
	float fVec17[2048];
	int iConst120;
	float fVec18[2048];
	int iConst121;
	float fRec9[2];
	float fConst130;
	float fConst131;
	float fConst132;
	float fRec168[2];
	float fRec167[2];
	float fVec19[32768];
	int iConst134;
	float fVec20[4096];
	int iConst135;
	float fRec165[2];
	float fConst144;
	float fConst145;
	float fConst146;
	float fRec172[2];
	float fRec171[2];
	float fVec21[16384];
	int iConst148;
	float fVec22[4096];
	int iConst149;
	float fRec169[2];
	float fConst158;
	float fConst159;
	float fConst160;
	float fRec176[2];
	float fRec175[2];
	float fVec23[32768];
	int iConst162;
	float fVec24[4096];
	int iConst163;
	float fRec173[2];
	float fConst172;
	float fConst173;
	float fConst174;
	float fRec180[2];
	float fRec179[2];
	float fVec25[16384];
	int iConst176;
	int iRec185[2];
	float fRec184[3];
	int iRec187[2];
	float fRec186[3];
	int iRec189[2];
	float fRec188[3];
	int iRec191[2];
	float fRec190[3];
	float fRec194[2];
	float fRec193[2];
	float fRec192[2];
	float fRec195[3];
	float fRec183[3];
	float fRec182[3];
	float fRec181[3];
	float fVec26[2048];
	float fVec27[2048];
	int iConst177;
	float fRec177[2];
	float fConst186;
	float fConst187;
	float fConst188;
	float fRec199[2];
	float fRec198[2];
	float fVec28[16384];
	int iConst190;
	float fVec29[4096];
	int iConst191;
	float fRec196[2];
	float fConst200;
	float fConst201;
	float fConst202;
	float fRec203[2];
	float fRec202[2];
	float fVec30[16384];
	int iConst204;
	float fVec31[4096];
	int iConst205;
	float fRec200[2];
	float fConst214;
	float fConst215;
	float fConst216;
	float fRec207[2];
	float fRec206[2];
	float fVec32[16384];
	int iConst218;
	float fVec33[2048];
	int iConst219;
	float fRec204[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fConst220;
	float fConst221;
	float fConst222;
	float fConst223;
	float fRec208[2];
	float fConst224;
	float fConst225;
	float fConst226;
	float fRec0[3];
	FAUSTFLOAT fHslider99;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider100;
	FAUSTFLOAT fHslider101;
	FAUSTFLOAT fHslider102;
	FAUSTFLOAT fHslider103;
	float fConst227;
	float fRec212[2];
	float fRec214[2];
	float fConst228;
	float fRec218[2];
	float fRec217[2];
	float fRec216[2];
	float fRec215[2];
	float fRec213[2];
	FAUSTFLOAT fHslider104;
	FAUSTFLOAT fHslider105;
	FAUSTFLOAT fHslider106;
	float fRec221[2];
	float fRec220[2];
	FAUSTFLOAT fHslider107;
	float fRec219[2];
	FAUSTFLOAT fHslider108;
	FAUSTFLOAT fHslider109;
	float fRec222[3];
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fHslider111;
	float fRec211[3];
	FAUSTFLOAT fHslider112;
	FAUSTFLOAT fHslider113;
	float fRec210[3];
	float fRec209[3];
	FAUSTFLOAT fHslider114;
	FAUSTFLOAT fCheckbox7;
	FAUSTFLOAT fHslider115;
	FAUSTFLOAT fHslider116;
	FAUSTFLOAT fHslider117;
	FAUSTFLOAT fHslider118;
	float fConst229;
	float fConst232;
	float fConst235;
	float fConst237;
	float fConst238;
	float fConst239;
	float fRec228[3];
	float fConst240;
	float fConst241;
	float fConst242;
	float fRec227[3];
	float fConst243;
	FAUSTFLOAT fHslider119;
	float fVec34[2];
	float fVec35[2];
	float fVec36[2];
	int iRec229[2];
	float fConst245;
	float fConst246;
	float fRec226[3];
	float fConst247;
	float fConst249;
	float fConst250;
	float fRec230[3];
	float fConst251;
	float fConst253;
	float fConst254;
	float fRec231[3];
	float fConst255;
	float fConst257;
	float fConst258;
	float fRec232[3];
	float fConst259;
	float fConst261;
	float fConst262;
	float fRec233[3];
	float fConst263;
	float fConst265;
	float fConst266;
	float fRec234[3];
	float fConst267;
	float fConst269;
	float fConst270;
	float fRec235[3];
	float fConst271;
	float fConst273;
	float fConst274;
	float fRec236[3];
	float fConst275;
	float fConst277;
	float fConst278;
	float fRec237[3];
	float fConst279;
	float fConst281;
	float fConst282;
	float fRec238[3];
	float fConst283;
	float fConst285;
	float fConst286;
	float fRec239[3];
	float fConst287;
	float fConst289;
	float fConst290;
	float fRec240[3];
	float fConst291;
	float fConst293;
	float fConst294;
	float fRec241[3];
	float fConst295;
	float fConst297;
	float fConst298;
	float fRec242[3];
	float fConst299;
	float fConst301;
	float fConst302;
	float fRec243[3];
	float fConst303;
	float fConst305;
	float fConst306;
	float fRec244[3];
	float fConst307;
	float fConst309;
	float fConst310;
	float fRec245[3];
	float fConst311;
	float fConst313;
	float fConst314;
	float fRec246[3];
	float fConst315;
	float fConst317;
	float fConst318;
	float fRec247[3];
	float fConst319;
	float fConst321;
	float fConst322;
	float fRec248[3];
	float fConst323;
	float fConst325;
	float fConst326;
	float fRec249[3];
	float fConst327;
	float fConst329;
	float fConst330;
	float fRec250[3];
	float fConst331;
	float fConst333;
	float fConst334;
	float fRec251[3];
	float fConst335;
	float fConst337;
	float fConst338;
	float fRec252[3];
	float fConst339;
	float fConst341;
	float fConst342;
	float fRec253[3];
	float fConst343;
	float fConst345;
	float fConst346;
	float fRec254[3];
	float fConst347;
	float fConst349;
	float fConst350;
	float fRec255[3];
	float fConst351;
	float fConst353;
	float fConst354;
	float fRec256[3];
	float fConst355;
	float fConst357;
	float fConst358;
	float fRec257[3];
	float fConst359;
	float fConst361;
	float fConst362;
	float fRec258[3];
	float fConst363;
	float fConst365;
	float fConst366;
	float fRec259[3];
	float fConst367;
	float fConst369;
	float fConst370;
	float fRec260[3];
	float fConst371;
	float fConst373;
	float fConst374;
	float fRec261[3];
	float fConst375;
	float fConst377;
	float fConst378;
	float fRec262[3];
	float fConst379;
	float fConst381;
	float fConst382;
	float fRec263[3];
	float fConst383;
	float fConst385;
	float fConst386;
	float fRec264[3];
	float fConst387;
	float fConst389;
	float fConst390;
	float fRec265[3];
	float fConst391;
	float fConst393;
	float fConst394;
	float fRec266[3];
	float fConst395;
	float fConst397;
	float fConst398;
	float fRec267[3];
	float fConst399;
	float fConst401;
	float fConst402;
	float fRec268[3];
	float fConst403;
	float fConst405;
	float fConst406;
	float fRec269[3];
	float fConst407;
	float fConst409;
	float fConst410;
	float fRec270[3];
	float fConst411;
	float fConst413;
	float fConst414;
	float fRec271[3];
	float fConst415;
	float fConst417;
	float fConst418;
	float fRec272[3];
	float fConst419;
	float fConst421;
	float fConst422;
	float fRec273[3];
	float fConst423;
	float fConst425;
	float fConst426;
	float fRec274[3];
	float fConst427;
	float fConst429;
	float fConst430;
	float fRec275[3];
	float fConst431;
	float fConst433;
	float fConst434;
	float fRec276[3];
	float fConst435;
	float fConst437;
	float fConst438;
	float fRec277[3];
	float fConst439;
	float fConst441;
	float fConst442;
	float fRec278[3];
	float fConst443;
	FAUSTFLOAT fHslider120;
	float fRec279[2];
	float fRec280[2];
	FAUSTFLOAT fHslider121;
	float fRec281[2];
	float fRec282[2];
	FAUSTFLOAT fHslider122;
	float fRec283[2];
	float fRec284[2];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	FAUSTFLOAT fHslider125;
	float fRec287[2];
	float fRec286[2];
	FAUSTFLOAT fHslider126;
	float fRec285[2];
	FAUSTFLOAT fHslider127;
	FAUSTFLOAT fHslider128;
	float fRec288[3];
	FAUSTFLOAT fHslider129;
	FAUSTFLOAT fHslider130;
	float fRec225[3];
	FAUSTFLOAT fHslider131;
	FAUSTFLOAT fHslider132;
	float fRec224[3];
	float fRec223[3];
	float fConst444;
	float fConst445;
	float fConst446;
	float fConst447;
	float fRec291[2];
	float fRec290[2];
	float fRec289[2];
	float fConst448;
	float fConst449;
	float fConst450;
	float fConst451;
	float fRec294[2];
	float fRec293[2];
	float fRec292[2];
	float fRec297[2];
	float fRec296[2];
	float fRec295[2];
	float fRec298[3];
	float fRec301[2];
	float fRec300[2];
	float fRec299[2];
	float fRec304[2];
	float fRec303[2];
	float fRec302[2];
	float fRec307[2];
	float fRec306[2];
	float fRec305[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "STS_Synth.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
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
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonbp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonbp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonbp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonhp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonhp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonhp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonlp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonlp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonlp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
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
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
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
		mydspSIG1* sig1 = newmydspSIG1();
		sig1->instanceInitmydspSIG1(sample_rate);
		sig1->fillmydspSIG1(350, ftbl1mydspSIG1);
		deletemydspSIG0(sig0);
		deletemydspSIG1(sig1);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst2 = ((0.0f - (6.90775537f * fConst1)) / fConst0);
		float fConst3 = std::exp((0.25f * fConst2));
		float fConst4 = mydsp_faustpower2_f(fConst3);
		float fConst5 = std::cos((12566.3711f / fConst0));
		float fConst6 = (1.0f - (fConst4 * fConst5));
		float fConst7 = (1.0f - fConst4);
		float fConst8 = (fConst6 / fConst7);
		float fConst9 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst6) / mydsp_faustpower2_f(fConst7)) + -1.0f)));
		fConst10 = (fConst8 - fConst9);
		fConst11 = (fConst3 * (fConst9 + (1.0f - fConst8)));
		fConst12 = ((std::exp(fConst2) / fConst3) + -1.0f);
		float fConst13 = (1.0f / std::tan((314.159271f / fConst0)));
		fConst14 = (1.0f / (fConst13 + 1.0f));
		fConst15 = (1.0f - fConst13);
		float fConst16 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst17 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst1 - fConst16))));
		fConst18 = (3.14159274f / fConst0);
		fConst19 = std::exp((0.0f - (500.0f / fConst0)));
		fConst20 = (1.0f - fConst19);
		fConst21 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst22 = (1.0f / fConst21);
		fConst23 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst24 = (0.330000013f * fConst0);
		fConst25 = (0.100000001f * fConst0);
		fConst26 = (0.660000026f * fConst0);
		fConst27 = std::exp((0.0f - (100.0f / fConst0)));
		fConst28 = (0.699999988f * (1.0f - fConst27));
		fConst29 = (1.0f / fConst0);
		fConst30 = std::exp((0.0f - (20.0f / fConst0)));
		fConst31 = (1.0f - fConst30);
		fConst32 = (6.28318548f / fConst0);
		fConst33 = (0.00882352982f * fConst0);
		fConst34 = (0.00117647054f * fConst0);
		fConst35 = (8.0f / fConst0);
		fConst36 = (0.000294117635f * fConst0);
		fConst37 = (0.00200000009f * fConst0);
		float fConst38 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst39 = (0.0f - (2.0f * fConst38));
		fConst40 = mydsp_faustpower2_f(fConst38);
		float fConst41 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst42 = (0.0f - (2.0f * fConst41));
		fConst43 = mydsp_faustpower2_f(fConst41);
		float fConst44 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst45 = (0.0f - (2.0f * fConst44));
		fConst46 = mydsp_faustpower2_f(fConst44);
		float fConst47 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst48 = (0.0f - (2.0f * fConst47));
		fConst49 = mydsp_faustpower2_f(fConst47);
		float fConst50 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst51 = (0.0f - (2.0f * fConst50));
		fConst52 = mydsp_faustpower2_f(fConst50);
		float fConst53 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst54 = (0.0f - (2.0f * fConst53));
		fConst55 = mydsp_faustpower2_f(fConst53);
		float fConst56 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst57 = (0.0f - (2.0f * fConst56));
		fConst58 = mydsp_faustpower2_f(fConst56);
		float fConst59 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst60 = (0.0f - (2.0f * fConst59));
		fConst61 = mydsp_faustpower2_f(fConst59);
		float fConst62 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst63 = (0.0f - (2.0f * fConst62));
		fConst64 = mydsp_faustpower2_f(fConst62);
		float fConst65 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst66 = (0.0f - (2.0f * fConst65));
		fConst67 = mydsp_faustpower2_f(fConst65);
		float fConst68 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst69 = (0.0f - (2.0f * fConst68));
		fConst70 = mydsp_faustpower2_f(fConst68);
		float fConst71 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst72 = (0.0f - (2.0f * fConst71));
		fConst73 = mydsp_faustpower2_f(fConst71);
		float fConst74 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst75 = (0.0f - (2.0f * fConst74));
		fConst76 = mydsp_faustpower2_f(fConst74);
		float fConst77 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst78 = (0.0f - (2.0f * fConst77));
		fConst79 = mydsp_faustpower2_f(fConst77);
		float fConst80 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst81 = (0.0f - (2.0f * fConst80));
		fConst82 = mydsp_faustpower2_f(fConst80);
		float fConst83 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst84 = (0.0f - (2.0f * fConst83));
		fConst85 = mydsp_faustpower2_f(fConst83);
		float fConst86 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst87 = (0.0f - (2.0f * fConst86));
		fConst88 = mydsp_faustpower2_f(fConst86);
		float fConst89 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst90 = (0.0f - (2.0f * fConst89));
		fConst91 = mydsp_faustpower2_f(fConst89);
		float fConst92 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst93 = (0.0f - (2.0f * fConst92));
		fConst94 = mydsp_faustpower2_f(fConst92);
		float fConst95 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst96 = (0.0f - (2.0f * fConst95));
		fConst97 = mydsp_faustpower2_f(fConst95);
		float fConst98 = std::tan((25132.7422f / fConst0));
		float fConst99 = (1.0f / fConst98);
		float fConst100 = (((fConst99 + 0.333333343f) / fConst98) + 1.0f);
		fConst101 = (0.25f / fConst100);
		fConst102 = std::exp((0.0f - (50.0f / fConst0)));
		fConst103 = (1.0f - fConst102);
		fConst104 = (6.0f / fConst0);
		fConst105 = (2.0f * fConst103);
		fConst106 = (1.0f / fConst100);
		fConst107 = (((fConst99 + -0.333333343f) / fConst98) + 1.0f);
		fConst108 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst98))));
		fConst109 = (5.0f / fConst0);
		fConst110 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst111 = (1.0f - fConst110);
		float fConst112 = std::tan((6283.18555f / fConst0));
		float fConst113 = (1.0f / fConst112);
		float fConst114 = (((fConst113 + 1.41421354f) / fConst112) + 1.0f);
		fConst115 = (0.0500000007f / fConst114);
		fConst116 = (1.0f / fConst114);
		fConst117 = (((fConst113 + -1.41421354f) / fConst112) + 1.0f);
		fConst118 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst112))));
		fConst119 = (0.00147058826f * fConst0);
		iConst120 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst121 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst16 + -1.0f))));
		float fConst122 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst123 = ((0.0f - (6.90775537f * fConst122)) / fConst0);
		float fConst124 = std::exp((0.25f * fConst123));
		float fConst125 = mydsp_faustpower2_f(fConst124);
		float fConst126 = (1.0f - (fConst125 * fConst5));
		float fConst127 = (1.0f - fConst125);
		float fConst128 = (fConst126 / fConst127);
		float fConst129 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst126) / mydsp_faustpower2_f(fConst127)) + -1.0f)));
		fConst130 = (fConst128 - fConst129);
		fConst131 = (fConst124 * (fConst129 + (1.0f - fConst128)));
		fConst132 = ((std::exp(fConst123) / fConst124) + -1.0f);
		float fConst133 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst134 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst122 - fConst133))));
		iConst135 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst133 + -1.0f))));
		float fConst136 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst137 = ((0.0f - (6.90775537f * fConst136)) / fConst0);
		float fConst138 = std::exp((0.25f * fConst137));
		float fConst139 = mydsp_faustpower2_f(fConst138);
		float fConst140 = (1.0f - (fConst139 * fConst5));
		float fConst141 = (1.0f - fConst139);
		float fConst142 = (fConst140 / fConst141);
		float fConst143 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst140) / mydsp_faustpower2_f(fConst141)) + -1.0f)));
		fConst144 = (fConst142 - fConst143);
		fConst145 = (fConst138 * (fConst143 + (1.0f - fConst142)));
		fConst146 = ((std::exp(fConst137) / fConst138) + -1.0f);
		float fConst147 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst148 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst136 - fConst147))));
		iConst149 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst147 + -1.0f))));
		float fConst150 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst151 = ((0.0f - (6.90775537f * fConst150)) / fConst0);
		float fConst152 = std::exp((0.25f * fConst151));
		float fConst153 = mydsp_faustpower2_f(fConst152);
		float fConst154 = (1.0f - (fConst153 * fConst5));
		float fConst155 = (1.0f - fConst153);
		float fConst156 = (fConst154 / fConst155);
		float fConst157 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst154) / mydsp_faustpower2_f(fConst155)) + -1.0f)));
		fConst158 = (fConst156 - fConst157);
		fConst159 = (fConst152 * (fConst157 + (1.0f - fConst156)));
		fConst160 = ((std::exp(fConst151) / fConst152) + -1.0f);
		float fConst161 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst162 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst150 - fConst161))));
		iConst163 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst161 + -1.0f))));
		float fConst164 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst165 = ((0.0f - (6.90775537f * fConst164)) / fConst0);
		float fConst166 = std::exp((0.25f * fConst165));
		float fConst167 = mydsp_faustpower2_f(fConst166);
		float fConst168 = (1.0f - (fConst167 * fConst5));
		float fConst169 = (1.0f - fConst167);
		float fConst170 = (fConst168 / fConst169);
		float fConst171 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst168) / mydsp_faustpower2_f(fConst169)) + -1.0f)));
		fConst172 = (fConst170 - fConst171);
		fConst173 = (fConst166 * (fConst171 + (1.0f - fConst170)));
		fConst174 = ((std::exp(fConst165) / fConst166) + -1.0f);
		float fConst175 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst176 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst164 - fConst175))));
		iConst177 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst175 + -1.0f))));
		float fConst178 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst179 = ((0.0f - (6.90775537f * fConst178)) / fConst0);
		float fConst180 = std::exp((0.25f * fConst179));
		float fConst181 = mydsp_faustpower2_f(fConst180);
		float fConst182 = (1.0f - (fConst181 * fConst5));
		float fConst183 = (1.0f - fConst181);
		float fConst184 = (fConst182 / fConst183);
		float fConst185 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst182) / mydsp_faustpower2_f(fConst183)) + -1.0f)));
		fConst186 = (fConst184 - fConst185);
		fConst187 = (fConst180 * (fConst185 + (1.0f - fConst184)));
		fConst188 = ((std::exp(fConst179) / fConst180) + -1.0f);
		float fConst189 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst190 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst178 - fConst189))));
		iConst191 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst189 + -1.0f))));
		float fConst192 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst193 = ((0.0f - (6.90775537f * fConst192)) / fConst0);
		float fConst194 = std::exp((0.25f * fConst193));
		float fConst195 = mydsp_faustpower2_f(fConst194);
		float fConst196 = (1.0f - (fConst195 * fConst5));
		float fConst197 = (1.0f - fConst195);
		float fConst198 = (fConst196 / fConst197);
		float fConst199 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst196) / mydsp_faustpower2_f(fConst197)) + -1.0f)));
		fConst200 = (fConst198 - fConst199);
		fConst201 = (fConst194 * (fConst199 + (1.0f - fConst198)));
		fConst202 = ((std::exp(fConst193) / fConst194) + -1.0f);
		float fConst203 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst204 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst192 - fConst203))));
		iConst205 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst203 + -1.0f))));
		float fConst206 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst207 = ((0.0f - (6.90775537f * fConst206)) / fConst0);
		float fConst208 = std::exp((0.25f * fConst207));
		float fConst209 = mydsp_faustpower2_f(fConst208);
		float fConst210 = (1.0f - (fConst209 * fConst5));
		float fConst211 = (1.0f - fConst209);
		float fConst212 = (fConst210 / fConst211);
		float fConst213 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst210) / mydsp_faustpower2_f(fConst211)) + -1.0f)));
		fConst214 = (fConst212 - fConst213);
		fConst215 = (fConst208 * (fConst213 + (1.0f - fConst212)));
		fConst216 = ((std::exp(fConst207) / fConst208) + -1.0f);
		float fConst217 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst218 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst206 - fConst217))));
		iConst219 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst217 + -1.0f))));
		fConst220 = mydsp_faustpower2_f(fConst29);
		fConst221 = (2.0f * fConst220);
		fConst222 = (4.0f * (mydsp_faustpower2_f(fConst0) * std::tan((628.318542f / fConst0))));
		fConst223 = (3500.0f * fConst111);
		fConst224 = (2.0f / fConst0);
		fConst225 = (2.0f * fConst0);
		fConst226 = (0.5f / fConst0);
		fConst227 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst228 = (81.0f / fConst0);
		fConst229 = ftbl1mydspSIG1[50];
		float fConst230 = std::tan((31415.9258f / fConst0));
		float fConst231 = (1.0f / fConst230);
		fConst232 = (1.0f / (((fConst231 + 1.41421354f) / fConst230) + 1.0f));
		float fConst233 = std::tan((31.415926f / fConst0));
		float fConst234 = (1.0f / fConst233);
		fConst235 = (1.0f / (((fConst234 + 1.41421354f) / fConst233) + 1.0f));
		float fConst236 = mydsp_faustpower2_f(fConst233);
		fConst237 = (1.0f / fConst236);
		fConst238 = (((fConst234 + -1.41421354f) / fConst233) + 1.0f);
		fConst239 = (2.0f * (1.0f - fConst237));
		fConst240 = (0.0f - (2.0f / fConst236));
		fConst241 = (((fConst231 + -1.41421354f) / fConst230) + 1.0f);
		fConst242 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst230))));
		fConst243 = (1.0f / std::max<float>(1.0f, (0.00159999996f * fConst0)));
		float fConst244 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst245 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst244)));
		fConst246 = mydsp_faustpower2_f(fConst244);
		fConst247 = ftbl1mydspSIG1[51];
		float fConst248 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst249 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst248)));
		fConst250 = mydsp_faustpower2_f(fConst248);
		fConst251 = ftbl1mydspSIG1[52];
		float fConst252 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst253 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst252)));
		fConst254 = mydsp_faustpower2_f(fConst252);
		fConst255 = ftbl1mydspSIG1[53];
		float fConst256 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst257 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst256)));
		fConst258 = mydsp_faustpower2_f(fConst256);
		fConst259 = ftbl1mydspSIG1[54];
		float fConst260 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst261 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst260)));
		fConst262 = mydsp_faustpower2_f(fConst260);
		fConst263 = ftbl1mydspSIG1[55];
		float fConst264 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst265 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst264)));
		fConst266 = mydsp_faustpower2_f(fConst264);
		fConst267 = ftbl1mydspSIG1[56];
		float fConst268 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst269 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst268)));
		fConst270 = mydsp_faustpower2_f(fConst268);
		fConst271 = ftbl1mydspSIG1[57];
		float fConst272 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst273 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst272)));
		fConst274 = mydsp_faustpower2_f(fConst272);
		fConst275 = ftbl1mydspSIG1[58];
		float fConst276 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst277 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst276)));
		fConst278 = mydsp_faustpower2_f(fConst276);
		fConst279 = ftbl1mydspSIG1[59];
		float fConst280 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst281 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst280)));
		fConst282 = mydsp_faustpower2_f(fConst280);
		fConst283 = ftbl1mydspSIG1[60];
		float fConst284 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst285 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst284)));
		fConst286 = mydsp_faustpower2_f(fConst284);
		fConst287 = ftbl1mydspSIG1[61];
		float fConst288 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst289 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst288)));
		fConst290 = mydsp_faustpower2_f(fConst288);
		fConst291 = ftbl1mydspSIG1[62];
		float fConst292 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst293 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst292)));
		fConst294 = mydsp_faustpower2_f(fConst292);
		fConst295 = ftbl1mydspSIG1[63];
		float fConst296 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst297 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst296)));
		fConst298 = mydsp_faustpower2_f(fConst296);
		fConst299 = ftbl1mydspSIG1[64];
		float fConst300 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst301 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst300)));
		fConst302 = mydsp_faustpower2_f(fConst300);
		fConst303 = ftbl1mydspSIG1[65];
		float fConst304 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst305 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst304)));
		fConst306 = mydsp_faustpower2_f(fConst304);
		fConst307 = ftbl1mydspSIG1[66];
		float fConst308 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst309 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst308)));
		fConst310 = mydsp_faustpower2_f(fConst308);
		fConst311 = ftbl1mydspSIG1[67];
		float fConst312 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst313 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst312)));
		fConst314 = mydsp_faustpower2_f(fConst312);
		fConst315 = ftbl1mydspSIG1[68];
		float fConst316 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst317 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst316)));
		fConst318 = mydsp_faustpower2_f(fConst316);
		fConst319 = ftbl1mydspSIG1[69];
		float fConst320 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst321 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst320)));
		fConst322 = mydsp_faustpower2_f(fConst320);
		fConst323 = ftbl1mydspSIG1[70];
		float fConst324 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst325 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst324)));
		fConst326 = mydsp_faustpower2_f(fConst324);
		fConst327 = ftbl1mydspSIG1[71];
		float fConst328 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst329 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst328)));
		fConst330 = mydsp_faustpower2_f(fConst328);
		fConst331 = ftbl1mydspSIG1[72];
		float fConst332 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst333 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst332)));
		fConst334 = mydsp_faustpower2_f(fConst332);
		fConst335 = ftbl1mydspSIG1[73];
		float fConst336 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst337 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst336)));
		fConst338 = mydsp_faustpower2_f(fConst336);
		fConst339 = ftbl1mydspSIG1[74];
		float fConst340 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst341 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst340)));
		fConst342 = mydsp_faustpower2_f(fConst340);
		fConst343 = ftbl1mydspSIG1[75];
		float fConst344 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst345 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst344)));
		fConst346 = mydsp_faustpower2_f(fConst344);
		fConst347 = ftbl1mydspSIG1[76];
		float fConst348 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst349 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst348)));
		fConst350 = mydsp_faustpower2_f(fConst348);
		fConst351 = ftbl1mydspSIG1[77];
		float fConst352 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst353 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst352)));
		fConst354 = mydsp_faustpower2_f(fConst352);
		fConst355 = ftbl1mydspSIG1[78];
		float fConst356 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst357 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst356)));
		fConst358 = mydsp_faustpower2_f(fConst356);
		fConst359 = ftbl1mydspSIG1[79];
		float fConst360 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst361 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst360)));
		fConst362 = mydsp_faustpower2_f(fConst360);
		fConst363 = ftbl1mydspSIG1[80];
		float fConst364 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst365 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst364)));
		fConst366 = mydsp_faustpower2_f(fConst364);
		fConst367 = ftbl1mydspSIG1[81];
		float fConst368 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst369 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst368)));
		fConst370 = mydsp_faustpower2_f(fConst368);
		fConst371 = ftbl1mydspSIG1[82];
		float fConst372 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst373 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst372)));
		fConst374 = mydsp_faustpower2_f(fConst372);
		fConst375 = ftbl1mydspSIG1[83];
		float fConst376 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst377 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst376)));
		fConst378 = mydsp_faustpower2_f(fConst376);
		fConst379 = ftbl1mydspSIG1[84];
		float fConst380 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst381 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst380)));
		fConst382 = mydsp_faustpower2_f(fConst380);
		fConst383 = ftbl1mydspSIG1[85];
		float fConst384 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst385 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst384)));
		fConst386 = mydsp_faustpower2_f(fConst384);
		fConst387 = ftbl1mydspSIG1[86];
		float fConst388 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst389 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst388)));
		fConst390 = mydsp_faustpower2_f(fConst388);
		fConst391 = ftbl1mydspSIG1[87];
		float fConst392 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst393 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst392)));
		fConst394 = mydsp_faustpower2_f(fConst392);
		fConst395 = ftbl1mydspSIG1[88];
		float fConst396 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst397 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst396)));
		fConst398 = mydsp_faustpower2_f(fConst396);
		fConst399 = ftbl1mydspSIG1[89];
		float fConst400 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst401 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst400)));
		fConst402 = mydsp_faustpower2_f(fConst400);
		fConst403 = ftbl1mydspSIG1[90];
		float fConst404 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst405 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst404)));
		fConst406 = mydsp_faustpower2_f(fConst404);
		fConst407 = ftbl1mydspSIG1[91];
		float fConst408 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst409 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst408)));
		fConst410 = mydsp_faustpower2_f(fConst408);
		fConst411 = ftbl1mydspSIG1[92];
		float fConst412 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst413 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst412)));
		fConst414 = mydsp_faustpower2_f(fConst412);
		fConst415 = ftbl1mydspSIG1[93];
		float fConst416 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst417 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst416)));
		fConst418 = mydsp_faustpower2_f(fConst416);
		fConst419 = ftbl1mydspSIG1[94];
		float fConst420 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst421 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst420)));
		fConst422 = mydsp_faustpower2_f(fConst420);
		fConst423 = ftbl1mydspSIG1[95];
		float fConst424 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst425 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst424)));
		fConst426 = mydsp_faustpower2_f(fConst424);
		fConst427 = ftbl1mydspSIG1[96];
		float fConst428 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst429 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst428)));
		fConst430 = mydsp_faustpower2_f(fConst428);
		fConst431 = ftbl1mydspSIG1[97];
		float fConst432 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst433 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst432)));
		fConst434 = mydsp_faustpower2_f(fConst432);
		fConst435 = ftbl1mydspSIG1[98];
		float fConst436 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst437 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst436)));
		fConst438 = mydsp_faustpower2_f(fConst436);
		fConst439 = ftbl1mydspSIG1[99];
		float fConst440 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst441 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst440)));
		fConst442 = mydsp_faustpower2_f(fConst440);
		fConst443 = (1.0f / std::max<float>(1.0f, fConst225));
		fConst444 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst445 = (1.0f - fConst444);
		fConst446 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst447 = (1.0f - fConst446);
		fConst448 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst449 = (1.0f - fConst448);
		fConst450 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst451 = (1.0f - fConst450);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(-6.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.69999999999999996f);
		fHslider2 = FAUSTFLOAT(20000.0f);
		fHslider3 = FAUSTFLOAT(2000.0f);
		fHslider4 = FAUSTFLOAT(500.0f);
		fHslider5 = FAUSTFLOAT(1.0f);
		fHslider6 = FAUSTFLOAT(50.0f);
		fHslider7 = FAUSTFLOAT(440.0f);
		fHslider8 = FAUSTFLOAT(0.5f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(440.0f);
		fHslider11 = FAUSTFLOAT(440.0f);
		fHslider12 = FAUSTFLOAT(440.0f);
		fHslider13 = FAUSTFLOAT(1.0f);
		fHslider14 = FAUSTFLOAT(0.10000000000000001f);
		fHslider15 = FAUSTFLOAT(0.10000000000000001f);
		fHslider16 = FAUSTFLOAT(0.0f);
		fHslider17 = FAUSTFLOAT(0.69999999999999996f);
		fHslider18 = FAUSTFLOAT(50.0f);
		fHslider19 = FAUSTFLOAT(0.0f);
		fHslider20 = FAUSTFLOAT(0.69999999999999996f);
		fHslider21 = FAUSTFLOAT(0.0f);
		fHslider22 = FAUSTFLOAT(0.69999999999999996f);
		fHslider23 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider24 = FAUSTFLOAT(0.69999999999999996f);
		fHslider25 = FAUSTFLOAT(20000.0f);
		fHslider26 = FAUSTFLOAT(2000.0f);
		fHslider27 = FAUSTFLOAT(500.0f);
		fHslider28 = FAUSTFLOAT(50.0f);
		fHslider29 = FAUSTFLOAT(440.0f);
		fHslider30 = FAUSTFLOAT(0.01f);
		fHslider31 = FAUSTFLOAT(1500.0f);
		fHslider32 = FAUSTFLOAT(1.0f);
		fHslider33 = FAUSTFLOAT(0.10000000000000001f);
		fHslider34 = FAUSTFLOAT(0.10000000000000001f);
		fHslider35 = FAUSTFLOAT(0.0f);
		fHslider36 = FAUSTFLOAT(0.69999999999999996f);
		fHslider37 = FAUSTFLOAT(50.0f);
		fHslider38 = FAUSTFLOAT(0.0f);
		fHslider39 = FAUSTFLOAT(0.69999999999999996f);
		fHslider40 = FAUSTFLOAT(0.0f);
		fHslider41 = FAUSTFLOAT(0.69999999999999996f);
		fHslider42 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider43 = FAUSTFLOAT(0.69999999999999996f);
		fHslider44 = FAUSTFLOAT(20000.0f);
		fHslider45 = FAUSTFLOAT(2000.0f);
		fHslider46 = FAUSTFLOAT(500.0f);
		fHslider47 = FAUSTFLOAT(1.0f);
		fHslider48 = FAUSTFLOAT(50.0f);
		fHslider49 = FAUSTFLOAT(7.0f);
		fHslider50 = FAUSTFLOAT(1.0f);
		fHslider51 = FAUSTFLOAT(0.10000000000000001f);
		fHslider52 = FAUSTFLOAT(0.10000000000000001f);
		fHslider53 = FAUSTFLOAT(0.0f);
		fHslider54 = FAUSTFLOAT(0.69999999999999996f);
		fHslider55 = FAUSTFLOAT(50.0f);
		fHslider56 = FAUSTFLOAT(0.0f);
		fHslider57 = FAUSTFLOAT(0.69999999999999996f);
		fHslider58 = FAUSTFLOAT(0.0f);
		fHslider59 = FAUSTFLOAT(0.69999999999999996f);
		fHslider60 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider61 = FAUSTFLOAT(0.69999999999999996f);
		fHslider62 = FAUSTFLOAT(20000.0f);
		fHslider63 = FAUSTFLOAT(2000.0f);
		fHslider64 = FAUSTFLOAT(500.0f);
		fHslider65 = FAUSTFLOAT(0.0f);
		fHslider66 = FAUSTFLOAT(50.0f);
		fHslider67 = FAUSTFLOAT(440.0f);
		fHslider68 = FAUSTFLOAT(0.0f);
		fHslider69 = FAUSTFLOAT(1.0f);
		fHslider70 = FAUSTFLOAT(0.10000000000000001f);
		fHslider71 = FAUSTFLOAT(0.10000000000000001f);
		fHslider72 = FAUSTFLOAT(0.0f);
		fHslider73 = FAUSTFLOAT(0.69999999999999996f);
		fHslider74 = FAUSTFLOAT(50.0f);
		fHslider75 = FAUSTFLOAT(0.0f);
		fHslider76 = FAUSTFLOAT(0.69999999999999996f);
		fHslider77 = FAUSTFLOAT(0.0f);
		fHslider78 = FAUSTFLOAT(0.69999999999999996f);
		fHslider79 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider80 = FAUSTFLOAT(0.69999999999999996f);
		fHslider81 = FAUSTFLOAT(20000.0f);
		fHslider82 = FAUSTFLOAT(2000.0f);
		fHslider83 = FAUSTFLOAT(500.0f);
		fHslider84 = FAUSTFLOAT(0.90000000000000002f);
		fHslider85 = FAUSTFLOAT(0.035999999999999997f);
		fHslider86 = FAUSTFLOAT(50.0f);
		fHslider87 = FAUSTFLOAT(440.0f);
		fHslider88 = FAUSTFLOAT(0.5f);
		fHslider89 = FAUSTFLOAT(1.0f);
		fHslider90 = FAUSTFLOAT(0.10000000000000001f);
		fHslider91 = FAUSTFLOAT(0.10000000000000001f);
		fHslider92 = FAUSTFLOAT(0.0f);
		fHslider93 = FAUSTFLOAT(0.69999999999999996f);
		fHslider94 = FAUSTFLOAT(50.0f);
		fHslider95 = FAUSTFLOAT(0.0f);
		fHslider96 = FAUSTFLOAT(0.69999999999999996f);
		fHslider97 = FAUSTFLOAT(0.0f);
		fHslider98 = FAUSTFLOAT(0.69999999999999996f);
		fHslider99 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider100 = FAUSTFLOAT(0.69999999999999996f);
		fHslider101 = FAUSTFLOAT(20000.0f);
		fHslider102 = FAUSTFLOAT(2000.0f);
		fHslider103 = FAUSTFLOAT(500.0f);
		fHslider104 = FAUSTFLOAT(1.0f);
		fHslider105 = FAUSTFLOAT(0.10000000000000001f);
		fHslider106 = FAUSTFLOAT(0.10000000000000001f);
		fHslider107 = FAUSTFLOAT(0.0f);
		fHslider108 = FAUSTFLOAT(0.69999999999999996f);
		fHslider109 = FAUSTFLOAT(50.0f);
		fHslider110 = FAUSTFLOAT(0.0f);
		fHslider111 = FAUSTFLOAT(0.69999999999999996f);
		fHslider112 = FAUSTFLOAT(0.0f);
		fHslider113 = FAUSTFLOAT(0.69999999999999996f);
		fHslider114 = FAUSTFLOAT(0.0f);
		fCheckbox7 = FAUSTFLOAT(0.0f);
		fHslider115 = FAUSTFLOAT(0.69999999999999996f);
		fHslider116 = FAUSTFLOAT(20000.0f);
		fHslider117 = FAUSTFLOAT(2000.0f);
		fHslider118 = FAUSTFLOAT(500.0f);
		fHslider119 = FAUSTFLOAT(50.0f);
		fHslider120 = FAUSTFLOAT(10.0f);
		fHslider121 = FAUSTFLOAT(10.0f);
		fHslider122 = FAUSTFLOAT(10.0f);
		fHslider123 = FAUSTFLOAT(1.0f);
		fHslider124 = FAUSTFLOAT(0.10000000000000001f);
		fHslider125 = FAUSTFLOAT(0.10000000000000001f);
		fHslider126 = FAUSTFLOAT(0.0f);
		fHslider127 = FAUSTFLOAT(0.69999999999999996f);
		fHslider128 = FAUSTFLOAT(50.0f);
		fHslider129 = FAUSTFLOAT(0.0f);
		fHslider130 = FAUSTFLOAT(0.69999999999999996f);
		fHslider131 = FAUSTFLOAT(0.0f);
		fHslider132 = FAUSTFLOAT(0.69999999999999996f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec12[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec11[l2] = 0.0f;
		}
		IOTA = 0;
		for (int l3 = 0; (l3 < 32768); l3 = (l3 + 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec3[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec4[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec17[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec16[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec19[l10] = 0;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec22[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec21[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec18[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec25[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			iRec24[l15] = 0;
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec23[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec28[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			iRec27[l18] = 0;
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec26[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec31[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec30[l21] = 0;
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec29[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec34[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec33[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec32[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec35[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec15[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec14[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec13[l29] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec67[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec65[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fVec5[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fVec6[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec68[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec61[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec69[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 4); l38 = (l38 + 1)) {
			fRec71[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2048); l39 = (l39 + 1)) {
			fRec72[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fVec7[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 3); l41 = (l41 + 1)) {
			fVec8[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2048); l42 = (l42 + 1)) {
			fRec70[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec57[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec53[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2048); l45 = (l45 + 1)) {
			fRec49[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 4); l46 = (l46 + 1)) {
			fRec47[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2048); l47 = (l47 + 1)) {
			fRec42[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec40[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec73[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec39[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec76[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec75[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec74[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec77[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec38[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec37[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec36[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			iRec84[l58] = 0;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec83[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec82[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fVec9[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec10[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fVec11[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			iRec85[l64] = 0;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec86[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 3); l66 = (l66 + 1)) {
			fRec81[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec87[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec88[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec89[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec90[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec91[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec92[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec93[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec94[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec95[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec96[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec97[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 3); l78 = (l78 + 1)) {
			fRec98[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec99[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec100[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec101[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec102[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec103[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec104[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec105[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec108[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec107[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec106[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 3); l89 = (l89 + 1)) {
			fRec109[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec80[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec79[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec78[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec115[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec114[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec116[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec118[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec117[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec120[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec121[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec122[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec123[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec113[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec126[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec125[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec124[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec127[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec112[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec111[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec110[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			iRec146[l110] = 0;
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec152[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec153[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fVec12[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fVec13[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec157[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec156[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec155[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec154[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 3); l119 = (l119 + 1)) {
			fRec158[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec159[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 2048); l121 = (l121 + 1)) {
			fRec160[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fVec14[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 2048); l123 = (l123 + 1)) {
			fVec15[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fVec16[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec151[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 2048); l126 = (l126 + 1)) {
			fRec142[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fRec138[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 2048); l128 = (l128 + 1)) {
			fRec134[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec132[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			fRec133[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec131[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec163[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec162[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec161[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec164[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 3); l136 = (l136 + 1)) {
			fRec130[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 3); l137 = (l137 + 1)) {
			fRec129[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 3); l138 = (l138 + 1)) {
			fRec128[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 2048); l139 = (l139 + 1)) {
			fVec17[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 2048); l140 = (l140 + 1)) {
			fVec18[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fRec9[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			fRec168[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec167[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 32768); l144 = (l144 + 1)) {
			fVec19[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 4096); l145 = (l145 + 1)) {
			fVec20[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec165[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec172[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 2); l148 = (l148 + 1)) {
			fRec171[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 16384); l149 = (l149 + 1)) {
			fVec21[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 4096); l150 = (l150 + 1)) {
			fVec22[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fRec169[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec176[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec175[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 32768); l154 = (l154 + 1)) {
			fVec23[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 4096); l155 = (l155 + 1)) {
			fVec24[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec173[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec180[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec179[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 16384); l159 = (l159 + 1)) {
			fVec25[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			iRec185[l160] = 0;
		}
		for (int l161 = 0; (l161 < 3); l161 = (l161 + 1)) {
			fRec184[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			iRec187[l162] = 0;
		}
		for (int l163 = 0; (l163 < 3); l163 = (l163 + 1)) {
			fRec186[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			iRec189[l164] = 0;
		}
		for (int l165 = 0; (l165 < 3); l165 = (l165 + 1)) {
			fRec188[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			iRec191[l166] = 0;
		}
		for (int l167 = 0; (l167 < 3); l167 = (l167 + 1)) {
			fRec190[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec194[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fRec193[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fRec192[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 3); l171 = (l171 + 1)) {
			fRec195[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec183[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 3); l173 = (l173 + 1)) {
			fRec182[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 3); l174 = (l174 + 1)) {
			fRec181[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 2048); l175 = (l175 + 1)) {
			fVec26[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2048); l176 = (l176 + 1)) {
			fVec27[l176] = 0.0f;
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec177[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 2); l178 = (l178 + 1)) {
			fRec199[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec198[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 16384); l180 = (l180 + 1)) {
			fVec28[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 4096); l181 = (l181 + 1)) {
			fVec29[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 2); l182 = (l182 + 1)) {
			fRec196[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 2); l183 = (l183 + 1)) {
			fRec203[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 2); l184 = (l184 + 1)) {
			fRec202[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 16384); l185 = (l185 + 1)) {
			fVec30[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 4096); l186 = (l186 + 1)) {
			fVec31[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			fRec200[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 2); l188 = (l188 + 1)) {
			fRec207[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec206[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 16384); l190 = (l190 + 1)) {
			fVec32[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 2048); l191 = (l191 + 1)) {
			fVec33[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			fRec204[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 3); l193 = (l193 + 1)) {
			fRec1[l193] = 0.0f;
		}
		for (int l194 = 0; (l194 < 3); l194 = (l194 + 1)) {
			fRec2[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 3); l195 = (l195 + 1)) {
			fRec3[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec4[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec5[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec6[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec7[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 3); l200 = (l200 + 1)) {
			fRec8[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 2); l201 = (l201 + 1)) {
			fRec208[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 3); l202 = (l202 + 1)) {
			fRec0[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fRec212[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec214[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec218[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec217[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec216[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec215[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fRec213[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec221[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec220[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 2); l212 = (l212 + 1)) {
			fRec219[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 3); l213 = (l213 + 1)) {
			fRec222[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 3); l214 = (l214 + 1)) {
			fRec211[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 3); l215 = (l215 + 1)) {
			fRec210[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec209[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 3); l217 = (l217 + 1)) {
			fRec228[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec227[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fVec34[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 2); l220 = (l220 + 1)) {
			fVec35[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fVec36[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 2); l222 = (l222 + 1)) {
			iRec229[l222] = 0;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec226[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec230[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec231[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 3); l226 = (l226 + 1)) {
			fRec232[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 3); l227 = (l227 + 1)) {
			fRec233[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 3); l228 = (l228 + 1)) {
			fRec234[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 3); l229 = (l229 + 1)) {
			fRec235[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 3); l230 = (l230 + 1)) {
			fRec236[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 3); l231 = (l231 + 1)) {
			fRec237[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 3); l232 = (l232 + 1)) {
			fRec238[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 3); l233 = (l233 + 1)) {
			fRec239[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fRec240[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 3); l235 = (l235 + 1)) {
			fRec241[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 3); l236 = (l236 + 1)) {
			fRec242[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 3); l237 = (l237 + 1)) {
			fRec243[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 3); l238 = (l238 + 1)) {
			fRec244[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 3); l239 = (l239 + 1)) {
			fRec245[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 3); l240 = (l240 + 1)) {
			fRec246[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 3); l241 = (l241 + 1)) {
			fRec247[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 3); l242 = (l242 + 1)) {
			fRec248[l242] = 0.0f;
		}
		for (int l243 = 0; (l243 < 3); l243 = (l243 + 1)) {
			fRec249[l243] = 0.0f;
		}
		for (int l244 = 0; (l244 < 3); l244 = (l244 + 1)) {
			fRec250[l244] = 0.0f;
		}
		for (int l245 = 0; (l245 < 3); l245 = (l245 + 1)) {
			fRec251[l245] = 0.0f;
		}
		for (int l246 = 0; (l246 < 3); l246 = (l246 + 1)) {
			fRec252[l246] = 0.0f;
		}
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec253[l247] = 0.0f;
		}
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec254[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec255[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec256[l250] = 0.0f;
		}
		for (int l251 = 0; (l251 < 3); l251 = (l251 + 1)) {
			fRec257[l251] = 0.0f;
		}
		for (int l252 = 0; (l252 < 3); l252 = (l252 + 1)) {
			fRec258[l252] = 0.0f;
		}
		for (int l253 = 0; (l253 < 3); l253 = (l253 + 1)) {
			fRec259[l253] = 0.0f;
		}
		for (int l254 = 0; (l254 < 3); l254 = (l254 + 1)) {
			fRec260[l254] = 0.0f;
		}
		for (int l255 = 0; (l255 < 3); l255 = (l255 + 1)) {
			fRec261[l255] = 0.0f;
		}
		for (int l256 = 0; (l256 < 3); l256 = (l256 + 1)) {
			fRec262[l256] = 0.0f;
		}
		for (int l257 = 0; (l257 < 3); l257 = (l257 + 1)) {
			fRec263[l257] = 0.0f;
		}
		for (int l258 = 0; (l258 < 3); l258 = (l258 + 1)) {
			fRec264[l258] = 0.0f;
		}
		for (int l259 = 0; (l259 < 3); l259 = (l259 + 1)) {
			fRec265[l259] = 0.0f;
		}
		for (int l260 = 0; (l260 < 3); l260 = (l260 + 1)) {
			fRec266[l260] = 0.0f;
		}
		for (int l261 = 0; (l261 < 3); l261 = (l261 + 1)) {
			fRec267[l261] = 0.0f;
		}
		for (int l262 = 0; (l262 < 3); l262 = (l262 + 1)) {
			fRec268[l262] = 0.0f;
		}
		for (int l263 = 0; (l263 < 3); l263 = (l263 + 1)) {
			fRec269[l263] = 0.0f;
		}
		for (int l264 = 0; (l264 < 3); l264 = (l264 + 1)) {
			fRec270[l264] = 0.0f;
		}
		for (int l265 = 0; (l265 < 3); l265 = (l265 + 1)) {
			fRec271[l265] = 0.0f;
		}
		for (int l266 = 0; (l266 < 3); l266 = (l266 + 1)) {
			fRec272[l266] = 0.0f;
		}
		for (int l267 = 0; (l267 < 3); l267 = (l267 + 1)) {
			fRec273[l267] = 0.0f;
		}
		for (int l268 = 0; (l268 < 3); l268 = (l268 + 1)) {
			fRec274[l268] = 0.0f;
		}
		for (int l269 = 0; (l269 < 3); l269 = (l269 + 1)) {
			fRec275[l269] = 0.0f;
		}
		for (int l270 = 0; (l270 < 3); l270 = (l270 + 1)) {
			fRec276[l270] = 0.0f;
		}
		for (int l271 = 0; (l271 < 3); l271 = (l271 + 1)) {
			fRec277[l271] = 0.0f;
		}
		for (int l272 = 0; (l272 < 3); l272 = (l272 + 1)) {
			fRec278[l272] = 0.0f;
		}
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			fRec279[l273] = 0.0f;
		}
		for (int l274 = 0; (l274 < 2); l274 = (l274 + 1)) {
			fRec280[l274] = 0.0f;
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec281[l275] = 0.0f;
		}
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec282[l276] = 0.0f;
		}
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec283[l277] = 0.0f;
		}
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec284[l278] = 0.0f;
		}
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec287[l279] = 0.0f;
		}
		for (int l280 = 0; (l280 < 2); l280 = (l280 + 1)) {
			fRec286[l280] = 0.0f;
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec285[l281] = 0.0f;
		}
		for (int l282 = 0; (l282 < 3); l282 = (l282 + 1)) {
			fRec288[l282] = 0.0f;
		}
		for (int l283 = 0; (l283 < 3); l283 = (l283 + 1)) {
			fRec225[l283] = 0.0f;
		}
		for (int l284 = 0; (l284 < 3); l284 = (l284 + 1)) {
			fRec224[l284] = 0.0f;
		}
		for (int l285 = 0; (l285 < 3); l285 = (l285 + 1)) {
			fRec223[l285] = 0.0f;
		}
		for (int l286 = 0; (l286 < 2); l286 = (l286 + 1)) {
			fRec291[l286] = 0.0f;
		}
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec290[l287] = 0.0f;
		}
		for (int l288 = 0; (l288 < 2); l288 = (l288 + 1)) {
			fRec289[l288] = 0.0f;
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec294[l289] = 0.0f;
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec293[l290] = 0.0f;
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec292[l291] = 0.0f;
		}
		for (int l292 = 0; (l292 < 2); l292 = (l292 + 1)) {
			fRec297[l292] = 0.0f;
		}
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec296[l293] = 0.0f;
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec295[l294] = 0.0f;
		}
		for (int l295 = 0; (l295 < 3); l295 = (l295 + 1)) {
			fRec298[l295] = 0.0f;
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec301[l296] = 0.0f;
		}
		for (int l297 = 0; (l297 < 2); l297 = (l297 + 1)) {
			fRec300[l297] = 0.0f;
		}
		for (int l298 = 0; (l298 < 2); l298 = (l298 + 1)) {
			fRec299[l298] = 0.0f;
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec304[l299] = 0.0f;
		}
		for (int l300 = 0; (l300 < 2); l300 = (l300 + 1)) {
			fRec303[l300] = 0.0f;
		}
		for (int l301 = 0; (l301 < 2); l301 = (l301 + 1)) {
			fRec302[l301] = 0.0f;
		}
		for (int l302 = 0; (l302 < 2); l302 = (l302 + 1)) {
			fRec307[l302] = 0.0f;
		}
		for (int l303 = 0; (l303 < 2); l303 = (l303 + 1)) {
			fRec306[l303] = 0.0f;
		}
		for (int l304 = 0; (l304 < 2); l304 = (l304 + 1)) {
			fRec305[l304] = 0.0f;
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
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider51, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider50, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider52, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider53, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider53, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider70, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider70, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider69, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider69, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider71, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider71, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider72, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider72, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider14, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider13, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider15, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider16, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider90, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider89, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider89, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider91, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider92, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider92, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider105, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider104, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider106, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider106, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider107, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider107, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->declare(&fHslider33, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider33, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider32, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider34, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider35, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->declare(&fHslider124, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider124, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider123, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider123, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider125, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider125, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider126, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider126, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("EQ_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider55, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider54, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider46, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider46, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider56, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider56, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider57, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider45, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider45, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider58, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider58, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider59, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider59, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider44, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider44, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider43, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider43, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider74, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider74, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider73, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider73, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider64, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider64, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider75, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider75, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider76, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider76, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider63, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider63, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider77, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider77, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider78, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider78, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider62, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider62, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider61, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider61, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider18, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider18, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider17, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider19, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider20, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider20, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider3, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider21, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider22, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider22, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider2, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider1, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider94, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider94, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider93, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider83, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider83, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider95, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider95, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider96, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider96, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider82, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider82, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider97, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider97, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider98, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider98, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider81, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider81, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider80, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider80, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider109, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider109, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider108, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider108, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider103, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider110, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider110, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider111, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider111, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider102, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider112, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider112, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider113, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider113, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider101, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider100, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider37, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider36, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider27, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider27, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider38, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider38, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider39, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider39, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider26, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider26, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider40, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider40, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider41, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider41, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider25, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider25, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider24, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider24, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider128, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider128, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider127, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider127, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider118, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider118, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider129, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider129, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider130, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider130, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider117, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider117, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider131, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider131, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider132, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider132, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider116, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider116, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider115, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider115, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->addCheckButton("Reverb ON", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider42, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider60, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider0, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider79, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 5", &fHslider99, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 6", &fHslider23, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 7", &fHslider114, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox3);
		ui_interface->addCheckButton("2", &fCheckbox4);
		ui_interface->addCheckButton("3", &fCheckbox1);
		ui_interface->addCheckButton("4", &fCheckbox5);
		ui_interface->addCheckButton("5", &fCheckbox6);
		ui_interface->addCheckButton("6", &fCheckbox2);
		ui_interface->addCheckButton("7", &fCheckbox7);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider48, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider8, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider68, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider28, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider30, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 14");
		ui_interface->addHorizontalSlider("Control 0", &fHslider65, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 15");
		ui_interface->addHorizontalSlider("Control 0", &fHslider47, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 16");
		ui_interface->addHorizontalSlider("Control 0", &fHslider84, 0.899999976f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 17");
		ui_interface->addHorizontalSlider("Control 0", &fHslider119, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 18");
		ui_interface->addHorizontalSlider("Control 0", &fHslider31, 1500.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 19");
		ui_interface->addHorizontalSlider("Control 0", &fHslider29, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider67, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 20");
		ui_interface->addHorizontalSlider("Control 0", &fHslider120, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 21");
		ui_interface->addHorizontalSlider("Control 0", &fHslider121, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 22");
		ui_interface->addHorizontalSlider("Control 0", &fHslider122, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 23");
		ui_interface->addHorizontalSlider("Control 0", &fHslider88, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 24");
		ui_interface->addHorizontalSlider("Control 0", &fHslider86, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 25");
		ui_interface->addHorizontalSlider("Control 0", &fHslider85, 0.0359999985f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider66, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider10, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider11, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider12, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 50.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider9, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 7");
		ui_interface->addHorizontalSlider("Control 0", &fHslider5, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider87, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 9");
		ui_interface->addHorizontalSlider("Control 0", &fHslider49, 7.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (1.25892544f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = float(fCheckbox0);
		float fSlow2 = (1.0f / float(fHslider1));
		float fSlow3 = std::tan((fConst18 * float(fHslider2)));
		float fSlow4 = (1.0f / fSlow3);
		float fSlow5 = (((fSlow2 + fSlow4) / fSlow3) + 1.0f);
		float fSlow6 = ((std::pow(10.0f, (0.0500000007f * float(fHslider0))) * (1.0f - float(fCheckbox1))) / fSlow5);
		float fSlow7 = float(fHslider3);
		float fSlow8 = std::tan((fConst18 * fSlow7));
		float fSlow9 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow8))));
		float fSlow10 = float(fHslider4);
		float fSlow11 = std::tan((fConst18 * fSlow10));
		float fSlow12 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow11))));
		float fSlow13 = (0.25f * float(fHslider5));
		float fSlow14 = float(fHslider6);
		float fSlow15 = float((fSlow14 > 0.0f));
		float fSlow16 = (0.0f - fSlow14);
		float fSlow17 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider7)));
		float fSlow18 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider8))) + -0.5f)));
		float fSlow19 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider9)));
		float fSlow20 = ((fSlow17 * fSlow18) * ((0.0500000007f * fSlow19) + 1.0f));
		float fSlow21 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider10))) * fSlow18) * (1.0f - (0.0299999993f * fSlow19)));
		float fSlow22 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider11))) * fSlow18) * ((0.310000002f * fSlow19) + 1.0f));
		float fSlow23 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider12))) * fSlow18) * (1.0f - (0.270000011f * fSlow19)));
		float fSlow24 = (fConst31 * ((1.0f / float(fHslider13)) + -1.0f));
		float fSlow25 = float(fHslider14);
		int iSlow26 = (std::fabs(fSlow25) < 1.1920929e-07f);
		float fSlow27 = (iSlow26 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow26 ? 1.0f : fSlow25)))));
		float fSlow28 = float(fHslider15);
		int iSlow29 = (std::fabs(fSlow28) < 1.1920929e-07f);
		float fSlow30 = (iSlow29 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow29 ? 1.0f : fSlow28)))));
		float fSlow31 = (1.0f - fSlow30);
		float fSlow32 = (1.0f - fSlow27);
		float fSlow33 = float(fHslider16);
		float fSlow34 = (1.0f / float(fHslider17));
		float fSlow35 = std::tan((fConst18 * float(fHslider18)));
		float fSlow36 = (1.0f / fSlow35);
		float fSlow37 = (1.0f / (((fSlow34 + fSlow36) / fSlow35) + 1.0f));
		float fSlow38 = (((fSlow36 - fSlow34) / fSlow35) + 1.0f);
		float fSlow39 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow35))));
		float fSlow40 = (1.0f / fSlow11);
		float fSlow41 = float(fHslider19);
		int iSlow42 = (fSlow41 > 0.0f);
		float fSlow43 = (float(fHslider20) * std::sin((fConst32 * fSlow10)));
		float fSlow44 = (fConst18 * ((fSlow10 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow41)))) / fSlow43));
		float fSlow45 = (fConst18 * (fSlow10 / fSlow43));
		float fSlow46 = (iSlow42 ? fSlow45 : fSlow44);
		float fSlow47 = ((fSlow40 * (fSlow40 - fSlow46)) + 1.0f);
		float fSlow48 = ((fSlow40 * (fSlow40 + fSlow46)) + 1.0f);
		float fSlow49 = (iSlow42 ? fSlow44 : fSlow45);
		float fSlow50 = ((fSlow40 * (fSlow40 + fSlow49)) + 1.0f);
		float fSlow51 = ((fSlow40 * (fSlow40 - fSlow49)) + 1.0f);
		float fSlow52 = (1.0f / fSlow8);
		float fSlow53 = float(fHslider21);
		int iSlow54 = (fSlow53 > 0.0f);
		float fSlow55 = (float(fHslider22) * std::sin((fConst32 * fSlow7)));
		float fSlow56 = (fConst18 * ((fSlow7 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow53)))) / fSlow55));
		float fSlow57 = (fConst18 * (fSlow7 / fSlow55));
		float fSlow58 = (iSlow54 ? fSlow57 : fSlow56);
		float fSlow59 = ((fSlow52 * (fSlow52 - fSlow58)) + 1.0f);
		float fSlow60 = ((fSlow52 * (fSlow52 + fSlow58)) + 1.0f);
		float fSlow61 = (iSlow54 ? fSlow56 : fSlow57);
		float fSlow62 = ((fSlow52 * (fSlow52 + fSlow61)) + 1.0f);
		float fSlow63 = ((fSlow52 * (fSlow52 - fSlow61)) + 1.0f);
		float fSlow64 = (1.0f / fSlow5);
		float fSlow65 = (((fSlow4 - fSlow2) / fSlow3) + 1.0f);
		float fSlow66 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow3))));
		float fSlow67 = (1.0f / float(fHslider24));
		float fSlow68 = std::tan((fConst18 * float(fHslider25)));
		float fSlow69 = (1.0f / fSlow68);
		float fSlow70 = (((fSlow67 + fSlow69) / fSlow68) + 1.0f);
		float fSlow71 = ((std::pow(10.0f, (0.0500000007f * float(fHslider23))) * (1.0f - float(fCheckbox2))) / fSlow70);
		float fSlow72 = float(fHslider26);
		float fSlow73 = std::tan((fConst18 * fSlow72));
		float fSlow74 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow73))));
		float fSlow75 = float(fHslider27);
		float fSlow76 = std::tan((fConst18 * fSlow75));
		float fSlow77 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow76))));
		float fSlow78 = (340.0f / fSlow18);
		float fSlow79 = (0.5f * fSlow19);
		float fSlow80 = float(fHslider28);
		float fSlow81 = float((fSlow80 > 0.0f));
		float fSlow82 = (0.0f - fSlow80);
		float fSlow83 = (4.0f * float(fHslider29));
		float fSlow84 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider30)));
		float fSlow85 = (1.0f - fSlow84);
		float fSlow86 = (0.00999999978f * float(fHslider31));
		float fSlow87 = (fConst31 * ((1.0f / float(fHslider32)) + -1.0f));
		float fSlow88 = float(fHslider33);
		int iSlow89 = (std::fabs(fSlow88) < 1.1920929e-07f);
		float fSlow90 = (iSlow89 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow89 ? 1.0f : fSlow88)))));
		float fSlow91 = float(fHslider34);
		int iSlow92 = (std::fabs(fSlow91) < 1.1920929e-07f);
		float fSlow93 = (iSlow92 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow92 ? 1.0f : fSlow91)))));
		float fSlow94 = (1.0f - fSlow93);
		float fSlow95 = (1.0f - fSlow90);
		float fSlow96 = float(fHslider35);
		float fSlow97 = (1.0f / float(fHslider36));
		float fSlow98 = std::tan((fConst18 * float(fHslider37)));
		float fSlow99 = (1.0f / fSlow98);
		float fSlow100 = (1.0f / (((fSlow97 + fSlow99) / fSlow98) + 1.0f));
		float fSlow101 = (((fSlow99 - fSlow97) / fSlow98) + 1.0f);
		float fSlow102 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow98))));
		float fSlow103 = (1.0f / fSlow76);
		float fSlow104 = float(fHslider38);
		int iSlow105 = (fSlow104 > 0.0f);
		float fSlow106 = (float(fHslider39) * std::sin((fConst32 * fSlow75)));
		float fSlow107 = (fConst18 * ((fSlow75 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow104)))) / fSlow106));
		float fSlow108 = (fConst18 * (fSlow75 / fSlow106));
		float fSlow109 = (iSlow105 ? fSlow108 : fSlow107);
		float fSlow110 = ((fSlow103 * (fSlow103 - fSlow109)) + 1.0f);
		float fSlow111 = ((fSlow103 * (fSlow103 + fSlow109)) + 1.0f);
		float fSlow112 = (iSlow105 ? fSlow107 : fSlow108);
		float fSlow113 = ((fSlow103 * (fSlow103 + fSlow112)) + 1.0f);
		float fSlow114 = ((fSlow103 * (fSlow103 - fSlow112)) + 1.0f);
		float fSlow115 = (1.0f / fSlow73);
		float fSlow116 = float(fHslider40);
		int iSlow117 = (fSlow116 > 0.0f);
		float fSlow118 = (float(fHslider41) * std::sin((fConst32 * fSlow72)));
		float fSlow119 = (fConst18 * ((fSlow72 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow116)))) / fSlow118));
		float fSlow120 = (fConst18 * (fSlow72 / fSlow118));
		float fSlow121 = (iSlow117 ? fSlow120 : fSlow119);
		float fSlow122 = ((fSlow115 * (fSlow115 - fSlow121)) + 1.0f);
		float fSlow123 = ((fSlow115 * (fSlow115 + fSlow121)) + 1.0f);
		float fSlow124 = (iSlow117 ? fSlow119 : fSlow120);
		float fSlow125 = ((fSlow115 * (fSlow115 + fSlow124)) + 1.0f);
		float fSlow126 = ((fSlow115 * (fSlow115 - fSlow124)) + 1.0f);
		float fSlow127 = (1.0f / fSlow70);
		float fSlow128 = (((fSlow69 - fSlow67) / fSlow68) + 1.0f);
		float fSlow129 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow68))));
		float fSlow130 = (1.0f / float(fHslider43));
		float fSlow131 = std::tan((fConst18 * float(fHslider44)));
		float fSlow132 = (1.0f / fSlow131);
		float fSlow133 = (((fSlow130 + fSlow132) / fSlow131) + 1.0f);
		float fSlow134 = ((std::pow(10.0f, (0.0500000007f * float(fHslider42))) * (1.0f - float(fCheckbox3))) / fSlow133);
		float fSlow135 = float(fHslider45);
		float fSlow136 = std::tan((fConst18 * fSlow135));
		float fSlow137 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow136))));
		float fSlow138 = float(fHslider46);
		float fSlow139 = std::tan((fConst18 * fSlow138));
		float fSlow140 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow139))));
		float fSlow141 = std::min<float>(5.0f, std::max<float>(0.00999999978f, float(fHslider47)));
		float fSlow142 = (0.0125000002f * (std::pow(10.0f, (0.0500000007f * fSlow141)) + 1.0f));
		float fSlow143 = ((2.0f * fSlow141) + 1.0f);
		float fSlow144 = std::tan((fConst18 * ((1875.0f * fSlow143) + 500.0f)));
		float fSlow145 = (1.0f / fSlow144);
		float fSlow146 = (1.0f / (((fSlow145 + 1.41421354f) / fSlow144) + 1.0f));
		float fSlow147 = std::tan((fConst18 * ((62.5f * fSlow143) + 40.0f)));
		float fSlow148 = (1.0f / fSlow147);
		float fSlow149 = (1.0f / (((fSlow148 + 1.41421354f) / fSlow147) + 1.0f));
		float fSlow150 = mydsp_faustpower2_f(fSlow147);
		float fSlow151 = (1.0f / fSlow150);
		float fSlow152 = (((fSlow148 + -1.41421354f) / fSlow147) + 1.0f);
		float fSlow153 = (2.0f * (1.0f - fSlow151));
		float fSlow154 = (0.0f - (2.0f / fSlow150));
		float fSlow155 = (((fSlow145 + -1.41421354f) / fSlow144) + 1.0f);
		float fSlow156 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow144))));
		float fSlow157 = (1.0f / std::max<float>(1.0f, (fConst37 * fSlow143)));
		float fSlow158 = float(fHslider48);
		float fSlow159 = float((fSlow158 > 0.0f));
		float fSlow160 = (0.0f - fSlow158);
		float fSlow161 = float(fHslider49);
		float fSlow162 = (fConst31 * ((1.0f / float(fHslider50)) + -1.0f));
		float fSlow163 = float(fHslider51);
		int iSlow164 = (std::fabs(fSlow163) < 1.1920929e-07f);
		float fSlow165 = (iSlow164 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow164 ? 1.0f : fSlow163)))));
		float fSlow166 = float(fHslider52);
		int iSlow167 = (std::fabs(fSlow166) < 1.1920929e-07f);
		float fSlow168 = (iSlow167 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow167 ? 1.0f : fSlow166)))));
		float fSlow169 = (1.0f - fSlow168);
		float fSlow170 = (1.0f - fSlow165);
		float fSlow171 = float(fHslider53);
		float fSlow172 = (1.0f / float(fHslider54));
		float fSlow173 = std::tan((fConst18 * float(fHslider55)));
		float fSlow174 = (1.0f / fSlow173);
		float fSlow175 = (1.0f / (((fSlow172 + fSlow174) / fSlow173) + 1.0f));
		float fSlow176 = (((fSlow174 - fSlow172) / fSlow173) + 1.0f);
		float fSlow177 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow173))));
		float fSlow178 = (1.0f / fSlow139);
		float fSlow179 = float(fHslider56);
		int iSlow180 = (fSlow179 > 0.0f);
		float fSlow181 = (float(fHslider57) * std::sin((fConst32 * fSlow138)));
		float fSlow182 = (fConst18 * ((fSlow138 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow179)))) / fSlow181));
		float fSlow183 = (fConst18 * (fSlow138 / fSlow181));
		float fSlow184 = (iSlow180 ? fSlow183 : fSlow182);
		float fSlow185 = ((fSlow178 * (fSlow178 - fSlow184)) + 1.0f);
		float fSlow186 = ((fSlow178 * (fSlow178 + fSlow184)) + 1.0f);
		float fSlow187 = (iSlow180 ? fSlow182 : fSlow183);
		float fSlow188 = ((fSlow178 * (fSlow178 + fSlow187)) + 1.0f);
		float fSlow189 = ((fSlow178 * (fSlow178 - fSlow187)) + 1.0f);
		float fSlow190 = (1.0f / fSlow136);
		float fSlow191 = float(fHslider58);
		int iSlow192 = (fSlow191 > 0.0f);
		float fSlow193 = (float(fHslider59) * std::sin((fConst32 * fSlow135)));
		float fSlow194 = (fConst18 * ((fSlow135 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow191)))) / fSlow193));
		float fSlow195 = (fConst18 * (fSlow135 / fSlow193));
		float fSlow196 = (iSlow192 ? fSlow195 : fSlow194);
		float fSlow197 = ((fSlow190 * (fSlow190 - fSlow196)) + 1.0f);
		float fSlow198 = ((fSlow190 * (fSlow190 + fSlow196)) + 1.0f);
		float fSlow199 = (iSlow192 ? fSlow194 : fSlow195);
		float fSlow200 = ((fSlow190 * (fSlow190 + fSlow199)) + 1.0f);
		float fSlow201 = ((fSlow190 * (fSlow190 - fSlow199)) + 1.0f);
		float fSlow202 = (1.0f / fSlow133);
		float fSlow203 = (((fSlow132 - fSlow130) / fSlow131) + 1.0f);
		float fSlow204 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow131))));
		float fSlow205 = (1.0f / float(fHslider61));
		float fSlow206 = std::tan((fConst18 * float(fHslider62)));
		float fSlow207 = (1.0f / fSlow206);
		float fSlow208 = (((fSlow205 + fSlow207) / fSlow206) + 1.0f);
		float fSlow209 = ((std::pow(10.0f, (0.0500000007f * float(fHslider60))) * (1.0f - float(fCheckbox4))) / fSlow208);
		float fSlow210 = float(fHslider63);
		float fSlow211 = std::tan((fConst18 * fSlow210));
		float fSlow212 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow211))));
		float fSlow213 = float(fHslider64);
		float fSlow214 = std::tan((fConst18 * fSlow213));
		float fSlow215 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow214))));
		float fSlow216 = (fConst101 * float(fHslider65));
		float fSlow217 = (fConst103 * float((float(fHslider66) > 50.0f)));
		float fSlow218 = (fConst105 * (float(fHslider67) * fSlow18));
		float fSlow219 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider68)));
		float fSlow220 = (fSlow219 + 5.0f);
		int iSlow221 = (fSlow220 < 23.0f);
		int iSlow222 = (fSlow220 < 24.0f);
		float fSlow223 = (fSlow219 + -18.0f);
		int iSlow224 = (fSlow220 < 22.0f);
		float fSlow225 = (fSlow219 + -17.0f);
		int iSlow226 = (fSlow220 < 21.0f);
		float fSlow227 = (fSlow219 + -16.0f);
		int iSlow228 = (fSlow220 < 20.0f);
		float fSlow229 = (fSlow219 + -15.0f);
		int iSlow230 = (fSlow220 < 19.0f);
		float fSlow231 = (fSlow219 + -14.0f);
		int iSlow232 = (fSlow220 < 18.0f);
		float fSlow233 = (fSlow219 + -13.0f);
		int iSlow234 = (fSlow220 < 17.0f);
		float fSlow235 = (fSlow219 + -12.0f);
		int iSlow236 = (fSlow220 < 16.0f);
		float fSlow237 = (fSlow219 + -11.0f);
		float fSlow238 = (80.0f * fSlow237);
		int iSlow239 = (fSlow220 < 15.0f);
		float fSlow240 = (fSlow219 + -10.0f);
		int iSlow241 = (fSlow220 < 14.0f);
		float fSlow242 = (fSlow219 + -9.0f);
		int iSlow243 = (fSlow220 < 13.0f);
		float fSlow244 = (fSlow219 + -8.0f);
		int iSlow245 = (fSlow220 < 12.0f);
		float fSlow246 = (fSlow219 + -7.0f);
		int iSlow247 = (fSlow220 < 11.0f);
		float fSlow248 = (fSlow219 + -6.0f);
		int iSlow249 = (fSlow220 < 10.0f);
		float fSlow250 = (fSlow219 + -5.0f);
		int iSlow251 = (fSlow220 < 9.0f);
		float fSlow252 = (fSlow219 + -4.0f);
		int iSlow253 = (fSlow220 < 8.0f);
		float fSlow254 = (fSlow219 + -3.0f);
		float fSlow255 = (50.0f * fSlow254);
		int iSlow256 = (fSlow220 < 7.0f);
		float fSlow257 = (fSlow219 + -2.0f);
		int iSlow258 = (fSlow220 < 6.0f);
		float fSlow259 = (fSlow219 + -1.0f);
		int iSlow260 = (fSlow220 < 5.0f);
		int iSlow261 = (fSlow220 < 4.0f);
		float fSlow262 = (fSlow219 + 1.0f);
		int iSlow263 = (fSlow220 < 3.0f);
		float fSlow264 = (fSlow219 + 2.0f);
		int iSlow265 = (fSlow220 < 2.0f);
		float fSlow266 = (fSlow219 + 3.0f);
		int iSlow267 = (fSlow220 < 1.0f);
		float fSlow268 = (fSlow219 + 4.0f);
		int iSlow269 = (fSlow220 < 0.0f);
		float fSlow270 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 800.0f : (iSlow267 ? (800.0f - (400.0f * fSlow220)) : 400.0f)) : (iSlow265 ? (400.0f - (50.0f * fSlow268)) : 350.0f)) : (iSlow263 ? ((100.0f * fSlow266) + 350.0f) : 450.0f)) : (iSlow261 ? (450.0f - (125.0f * fSlow264)) : 325.0f)) : (iSlow260 ? ((275.0f * fSlow262) + 325.0f) : 600.0f)) : (iSlow258 ? (600.0f - (200.0f * fSlow219)) : 400.0f)) : (iSlow256 ? (400.0f - (150.0f * fSlow259)) : 250.0f)) : (iSlow253 ? ((150.0f * fSlow257) + 250.0f) : 400.0f)) : (iSlow251 ? (400.0f - fSlow255) : 350.0f)) : (iSlow249 ? ((310.0f * fSlow252) + 350.0f) : 660.0f)) : (iSlow247 ? (660.0f - (220.0f * fSlow250)) : 440.0f)) : (iSlow245 ? (440.0f - (170.0f * fSlow248)) : 270.0f)) : (iSlow243 ? ((160.0f * fSlow246) + 270.0f) : 430.0f)) : (iSlow241 ? (430.0f - (60.0f * fSlow244)) : 370.0f)) : (iSlow239 ? ((430.0f * fSlow242) + 370.0f) : 800.0f)) : (iSlow236 ? (800.0f - (450.0f * fSlow240)) : 350.0f)) : (iSlow234 ? (350.0f - fSlow238) : 270.0f)) : (iSlow232 ? ((180.0f * fSlow235) + 270.0f) : 450.0f)) : (iSlow230 ? (450.0f - (125.0f * fSlow233)) : 325.0f)) : (iSlow228 ? (325.0f * (fSlow231 + 1.0f)) : 650.0f)) : (iSlow226 ? (650.0f - (250.0f * fSlow229)) : 400.0f)) : (iSlow224 ? (400.0f - (110.0f * fSlow227)) : 290.0f)) : (iSlow221 ? ((110.0f * fSlow225) + 290.0f) : 400.0f)) : (iSlow222 ? (400.0f - (50.0f * fSlow223)) : 350.0f));
		float fSlow271 = (20.0f * fSlow229);
		float fSlow272 = (20.0f * fSlow240);
		float fSlow273 = (10.0f * fSlow250);
		float fSlow274 = (10.0f * fSlow262);
		float fSlow275 = (20.0f * fSlow264);
		float fSlow276 = (20.0f * fSlow266);
		float fSlow277 = (20.0f * fSlow220);
		float fSlow278 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 80.0f : (iSlow267 ? (80.0f - fSlow277) : 60.0f)) : (iSlow265 ? (60.0f - (10.0f * fSlow268)) : 50.0f)) : (iSlow263 ? (fSlow276 + 50.0f) : 70.0f)) : (iSlow261 ? (70.0f - fSlow275) : 50.0f)) : (iSlow260 ? (fSlow274 + 50.0f) : 60.0f)) : (iSlow258 ? (60.0f - (20.0f * fSlow219)) : 40.0f)) : (iSlow256 ? ((20.0f * fSlow259) + 40.0f) : 60.0f)) : (iSlow253 ? (60.0f - (20.0f * fSlow257)) : 40.0f)) : 40.0f) : (iSlow249 ? (40.0f * (fSlow252 + 1.0f)) : 80.0f)) : (iSlow247 ? (80.0f - fSlow273) : 70.0f)) : (iSlow245 ? (70.0f - (30.0f * fSlow248)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow239 ? (40.0f * (fSlow242 + 1.0f)) : 80.0f)) : (iSlow236 ? (80.0f - fSlow272) : 60.0f)) : 60.0f) : (iSlow232 ? (60.0f - (20.0f * fSlow235)) : 40.0f)) : (iSlow230 ? ((10.0f * fSlow233) + 40.0f) : 50.0f)) : 50.0f) : (iSlow226 ? (fSlow271 + 50.0f) : 70.0f)) : (iSlow224 ? (70.0f - (30.0f * fSlow227)) : 40.0f)) : (iSlow221 ? ((30.0f * fSlow225) + 40.0f) : 70.0f)) : (iSlow222 ? (70.0f - (30.0f * fSlow223)) : 40.0f));
		float fSlow279 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 1150.0f : (iSlow267 ? ((450.0f * fSlow220) + 1150.0f) : 1600.0f)) : (iSlow265 ? ((100.0f * fSlow268) + 1600.0f) : 1700.0f)) : (iSlow263 ? (1700.0f - (900.0f * fSlow266)) : 800.0f)) : (iSlow261 ? (800.0f - (100.0f * fSlow264)) : 700.0f)) : (iSlow260 ? ((340.0f * fSlow262) + 700.0f) : 1040.0f)) : (iSlow258 ? ((580.0f * fSlow219) + 1040.0f) : 1620.0f)) : (iSlow256 ? ((130.0f * fSlow259) + 1620.0f) : 1750.0f)) : (iSlow253 ? (1750.0f - (1000.0f * fSlow257)) : 750.0f)) : (iSlow251 ? (750.0f - (150.0f * fSlow254)) : 600.0f)) : (iSlow249 ? ((520.0f * fSlow252) + 600.0f) : 1120.0f)) : (iSlow247 ? ((680.0f * fSlow250) + 1120.0f) : 1800.0f)) : (iSlow245 ? ((50.0f * fSlow248) + 1800.0f) : 1850.0f)) : (iSlow243 ? (1850.0f - (1030.0f * fSlow246)) : 820.0f)) : (iSlow241 ? (820.0f - (190.0f * fSlow244)) : 630.0f)) : (iSlow239 ? ((520.0f * fSlow242) + 630.0f) : 1150.0f)) : (iSlow236 ? ((850.0f * fSlow240) + 1150.0f) : 2000.0f)) : (iSlow234 ? ((140.0f * fSlow237) + 2000.0f) : 2140.0f)) : (iSlow232 ? (2140.0f - (1340.0f * fSlow235)) : 800.0f)) : (iSlow230 ? (800.0f - (100.0f * fSlow233)) : 700.0f)) : (iSlow228 ? ((380.0f * fSlow231) + 700.0f) : 1080.0f)) : (iSlow226 ? ((620.0f * fSlow229) + 1080.0f) : 1700.0f)) : (iSlow224 ? ((170.0f * fSlow227) + 1700.0f) : 1870.0f)) : (iSlow221 ? (1870.0f - (1070.0f * fSlow225)) : 800.0f)) : (iSlow222 ? (800.0f - (200.0f * fSlow223)) : 600.0f));
		int iSlow280 = (fSlow279 >= 1300.0f);
		float fSlow281 = (fSlow279 + -1300.0f);
		float fSlow282 = (10.0f * fSlow225);
		float fSlow283 = (10.0f * fSlow229);
		float fSlow284 = (10.0f * fSlow252);
		float fSlow285 = (10.0f * fSlow219);
		float fSlow286 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 90.0f : (iSlow267 ? (90.0f - (10.0f * fSlow220)) : 80.0f)) : (iSlow265 ? ((20.0f * fSlow268) + 80.0f) : 100.0f)) : (iSlow263 ? (100.0f - fSlow276) : 80.0f)) : (iSlow261 ? (80.0f - fSlow275) : 60.0f)) : (iSlow260 ? (fSlow274 + 60.0f) : 70.0f)) : (iSlow258 ? (fSlow285 + 70.0f) : 80.0f)) : (iSlow256 ? ((10.0f * fSlow259) + 80.0f) : 90.0f)) : (iSlow253 ? (90.0f - (10.0f * fSlow257)) : 80.0f)) : 80.0f) : (iSlow249 ? (fSlow284 + 80.0f) : 90.0f)) : (iSlow247 ? (90.0f - fSlow273) : 80.0f)) : (iSlow245 ? ((10.0f * fSlow248) + 80.0f) : 90.0f)) : (iSlow243 ? (90.0f - (10.0f * fSlow246)) : 80.0f)) : (iSlow241 ? (80.0f - (20.0f * fSlow244)) : 60.0f)) : (iSlow239 ? ((30.0f * fSlow242) + 60.0f) : 90.0f)) : (iSlow236 ? ((10.0f * fSlow240) + 90.0f) : 100.0f)) : (iSlow234 ? (100.0f - (10.0f * fSlow237)) : 90.0f)) : (iSlow232 ? (90.0f - (10.0f * fSlow235)) : 80.0f)) : (iSlow230 ? (80.0f - (20.0f * fSlow233)) : 60.0f)) : (iSlow228 ? ((30.0f * fSlow231) + 60.0f) : 90.0f)) : (iSlow226 ? (90.0f - fSlow283) : 80.0f)) : (iSlow224 ? ((10.0f * fSlow227) + 80.0f) : 90.0f)) : (iSlow221 ? (90.0f - fSlow282) : 80.0f)) : (iSlow222 ? (80.0f - (20.0f * fSlow223)) : 60.0f));
		float fSlow287 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 0.630957007f : (iSlow267 ? (0.630957007f - (0.567861021f * fSlow220)) : 0.0630960017f)) : (iSlow265 ? ((0.0369039997f * fSlow268) + 0.0630960017f) : 0.100000001f)) : (iSlow263 ? ((0.254812986f * fSlow266) + 0.100000001f) : 0.35481301f)) : (iSlow261 ? (0.35481301f - (0.103624001f * fSlow264)) : 0.251188993f)) : (iSlow260 ? ((0.195494995f * fSlow262) + 0.251188993f) : 0.446684003f)) : (iSlow258 ? (0.446684003f - (0.195494995f * fSlow219)) : 0.251188993f)) : (iSlow256 ? (0.251188993f - (0.219566002f * fSlow259)) : 0.0316229984f)) : (iSlow253 ? ((0.250214994f * fSlow257) + 0.0316229984f) : 0.281838f)) : (iSlow251 ? (0.281838f - (0.181838006f * fSlow254)) : 0.100000001f)) : (iSlow249 ? ((0.401187003f * fSlow252) + 0.100000001f) : 0.501187027f)) : (iSlow247 ? (0.501187027f - (0.301661015f * fSlow250)) : 0.199525997f)) : (iSlow245 ? (0.199525997f - (0.136429995f * fSlow248)) : 0.0630960017f)) : (iSlow243 ? ((0.253131986f * fSlow246) + 0.0630960017f) : 0.316228002f)) : (iSlow241 ? (0.316228002f - (0.216227993f * fSlow244)) : 0.100000001f)) : (iSlow239 ? ((0.401187003f * fSlow242) + 0.100000001f) : 0.501187027f)) : (iSlow236 ? (0.501187027f - (0.401187003f * fSlow240)) : 0.100000001f)) : (iSlow234 ? ((0.151188999f * fSlow237) + 0.100000001f) : 0.251188993f)) : (iSlow232 ? ((0.0306490008f * fSlow235) + 0.251188993f) : 0.281838f)) : (iSlow230 ? (0.281838f - (0.123349003f * fSlow233)) : 0.158489004f)) : (iSlow228 ? ((0.342698008f * fSlow231) + 0.158489004f) : 0.501187027f)) : (iSlow226 ? (0.501187027f - (0.301661015f * fSlow229)) : 0.199525997f)) : (iSlow224 ? (0.199525997f - (0.0216979999f * fSlow227)) : 0.177827999f)) : (iSlow221 ? ((0.138400003f * fSlow225) + 0.177827999f) : 0.316228002f)) : (iSlow222 ? (0.316228002f - (0.216227993f * fSlow223)) : 0.100000001f));
		float fSlow288 = (100.0f * fSlow223);
		float fSlow289 = (50.0f * fSlow231);
		float fSlow290 = (50.0f * fSlow250);
		float fSlow291 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 2800.0f : (iSlow267 ? (2800.0f - (100.0f * fSlow220)) : 2700.0f)) : 2700.0f) : (iSlow263 ? ((130.0f * fSlow266) + 2700.0f) : 2830.0f)) : (iSlow261 ? (2830.0f - (300.0f * fSlow264)) : 2530.0f)) : (iSlow260 ? (2530.0f - (280.0f * fSlow262)) : 2250.0f)) : (iSlow258 ? ((150.0f * fSlow219) + 2250.0f) : 2400.0f)) : (iSlow256 ? ((200.0f * fSlow259) + 2400.0f) : 2600.0f)) : (iSlow253 ? (2600.0f - (200.0f * fSlow257)) : 2400.0f)) : 2400.0f) : (iSlow249 ? ((350.0f * fSlow252) + 2400.0f) : 2750.0f)) : (iSlow247 ? (2750.0f - fSlow290) : 2700.0f)) : (iSlow245 ? ((200.0f * fSlow248) + 2700.0f) : 2900.0f)) : (iSlow243 ? (2900.0f - (200.0f * fSlow246)) : 2700.0f)) : (iSlow241 ? ((50.0f * fSlow244) + 2700.0f) : 2750.0f)) : (iSlow239 ? ((150.0f * fSlow242) + 2750.0f) : 2900.0f)) : (iSlow236 ? (2900.0f - (100.0f * fSlow240)) : 2800.0f)) : (iSlow234 ? ((150.0f * fSlow237) + 2800.0f) : 2950.0f)) : (iSlow232 ? (2950.0f - (120.0f * fSlow235)) : 2830.0f)) : (iSlow230 ? (2830.0f - (130.0f * fSlow233)) : 2700.0f)) : (iSlow228 ? (2700.0f - fSlow289) : 2650.0f)) : (iSlow226 ? (2650.0f - (50.0f * fSlow229)) : 2600.0f)) : (iSlow224 ? ((200.0f * fSlow227) + 2600.0f) : 2800.0f)) : (iSlow221 ? (2800.0f - (200.0f * fSlow225)) : 2600.0f)) : (iSlow222 ? (fSlow288 + 2600.0f) : 2700.0f));
		float fSlow292 = std::tan((fConst18 * fSlow291));
		float fSlow293 = (1.0f / fSlow292);
		float fSlow294 = (20.0f * fSlow242);
		float fSlow295 = (20.0f * fSlow250);
		float fSlow296 = (20.0f * fSlow252);
		float fSlow297 = (60.0f * fSlow262);
		float fSlow298 = ((iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? 120.0f : (iSlow263 ? (120.0f - fSlow276) : 100.0f)) : (iSlow261 ? ((70.0f * fSlow264) + 100.0f) : 170.0f)) : (iSlow260 ? (170.0f - fSlow297) : 110.0f)) : (iSlow258 ? (110.0f - fSlow285) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow249 ? (fSlow296 + 100.0f) : 120.0f)) : (iSlow247 ? (120.0f - fSlow295) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow239 ? (fSlow294 + 100.0f) : 120.0f)) : 120.0f) : (iSlow234 ? (120.0f - (20.0f * fSlow237)) : 100.0f)) : 100.0f) : (iSlow230 ? ((70.0f * fSlow233) + 100.0f) : 170.0f)) : (iSlow228 ? (170.0f - fSlow289) : 120.0f)) : (iSlow226 ? (120.0f - fSlow271) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow291);
		float fSlow299 = (((fSlow293 - fSlow298) / fSlow292) + 1.0f);
		float fSlow300 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow292)));
		float fSlow301 = (((fSlow293 + fSlow298) / fSlow292) + 1.0f);
		float fSlow302 = (iSlow232 ? ((0.0293140002f * fSlow235) + 0.0501190014f) : 0.0794330016f);
		float fSlow303 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 0.100000001f : (iSlow267 ? (0.100000001f - (0.0683770031f * fSlow220)) : 0.0316229984f)) : 0.0316229984f) : (iSlow263 ? ((0.126865998f * fSlow266) + 0.0316229984f) : 0.158489004f)) : (iSlow261 ? (0.158489004f - (0.126865998f * fSlow264)) : 0.0316229984f)) : (iSlow260 ? ((0.323190004f * fSlow262) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow256 ? (0.35481301f - (0.196324006f * fSlow259)) : 0.158489004f)) : (iSlow253 ? (0.158489004f - (0.0693639964f * fSlow257)) : 0.0891249999f)) : (iSlow251 ? (0.0891249999f - (0.0640060008f * fSlow254)) : 0.0251189992f)) : (iSlow249 ? ((0.0456760004f * fSlow252) + 0.0251189992f) : 0.0707949996f)) : (iSlow247 ? ((0.0550980009f * fSlow250) + 0.0707949996f) : 0.125892997f)) : (iSlow245 ? (0.125892997f - (0.0627970025f * fSlow248)) : 0.0630960017f)) : (iSlow243 ? (0.0630960017f - (0.0129770003f * fSlow246)) : 0.0501190014f)) : (iSlow241 ? ((0.020676f * fSlow244) + 0.0501190014f) : 0.0707949996f)) : (iSlow239 ? (0.0707949996f - (0.0456760004f * fSlow242)) : 0.0251189992f)) : (iSlow236 ? ((0.152709007f * fSlow240) + 0.0251189992f) : 0.177827999f)) : (iSlow234 ? (0.177827999f - (0.127709001f * fSlow237)) : 0.0501190014f)) : fSlow302) : (iSlow230 ? (0.0794330016f - (0.0616500005f * fSlow233)) : 0.0177829992f)) : (iSlow228 ? ((0.428900987f * fSlow231) + 0.0177829992f) : 0.446684003f)) : (iSlow226 ? (0.446684003f - (0.195494995f * fSlow229)) : 0.251188993f)) : (iSlow224 ? (0.251188993f - (0.125295997f * fSlow227)) : 0.125892997f)) : (iSlow221 ? ((0.125295997f * fSlow225) + 0.125892997f) : 0.251188993f)) : (iSlow222 ? (0.251188993f - (0.109935001f * fSlow223)) : 0.141253993f));
		float fSlow304 = (fSlow292 * fSlow301);
		float fSlow305 = (350.0f * fSlow219);
		float fSlow306 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 3500.0f : (iSlow267 ? (3500.0f - (200.0f * fSlow220)) : 3300.0f)) : (iSlow265 ? ((400.0f * fSlow268) + 3300.0f) : 3700.0f)) : (iSlow263 ? (3700.0f - (200.0f * fSlow266)) : 3500.0f)) : 3500.0f) : (iSlow260 ? (3500.0f - (1050.0f * fSlow262)) : 2450.0f)) : (iSlow258 ? (fSlow305 + 2450.0f) : 2800.0f)) : (iSlow256 ? ((250.0f * fSlow259) + 2800.0f) : 3050.0f)) : (iSlow253 ? (3050.0f - (450.0f * fSlow257)) : 2600.0f)) : (iSlow251 ? ((75.0f * fSlow254) + 2600.0f) : 2675.0f)) : (iSlow249 ? ((325.0f * fSlow252) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow245 ? ((350.0f * fSlow248) + 3000.0f) : 3350.0f)) : (iSlow243 ? (3350.0f - (350.0f * fSlow246)) : 3000.0f)) : 3000.0f) : (iSlow239 ? ((900.0f * fSlow242) + 3000.0f) : 3900.0f)) : (iSlow236 ? (3900.0f - (300.0f * fSlow240)) : 3600.0f)) : (iSlow234 ? ((300.0f * fSlow237) + 3600.0f) : 3900.0f)) : (iSlow232 ? (3900.0f - (100.0f * fSlow235)) : 3800.0f)) : 3800.0f) : (iSlow228 ? (3800.0f - (900.0f * fSlow231)) : 2900.0f)) : (iSlow226 ? ((300.0f * fSlow229) + 2900.0f) : 3200.0f)) : (iSlow224 ? ((50.0f * fSlow227) + 3200.0f) : 3250.0f)) : (iSlow221 ? (3250.0f - (450.0f * fSlow225)) : 2800.0f)) : (iSlow222 ? (fSlow288 + 2800.0f) : 2900.0f));
		float fSlow307 = std::tan((fConst18 * fSlow306));
		float fSlow308 = (1.0f / fSlow307);
		float fSlow309 = ((iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 130.0f : (iSlow267 ? (fSlow277 + 130.0f) : 150.0f)) : 150.0f) : (iSlow263 ? (150.0f - fSlow276) : 130.0f)) : (iSlow261 ? ((50.0f * fSlow264) + 130.0f) : 180.0f)) : (iSlow260 ? (180.0f - fSlow297) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow249 ? (fSlow284 + 120.0f) : 130.0f)) : (iSlow247 ? (130.0f - fSlow273) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow239 ? ((10.0f * fSlow242) + 120.0f) : 130.0f)) : (iSlow236 ? (fSlow272 + 130.0f) : 150.0f)) : (iSlow234 ? (150.0f - (30.0f * fSlow237)) : 120.0f)) : 120.0f) : (iSlow230 ? ((60.0f * fSlow233) + 120.0f) : 180.0f)) : (iSlow228 ? (180.0f - fSlow289) : 130.0f)) : (iSlow226 ? (130.0f - fSlow283) : 120.0f)) : 120.0f) : (iSlow221 ? (fSlow282 + 120.0f) : 130.0f)) : (iSlow222 ? (130.0f - (10.0f * fSlow223)) : 120.0f)) / fSlow306);
		float fSlow310 = (((fSlow308 - fSlow309) / fSlow307) + 1.0f);
		float fSlow311 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow307)));
		float fSlow312 = (((fSlow308 + fSlow309) / fSlow307) + 1.0f);
		float fSlow313 = (iSlow245 ? (0.100000001f - (0.0841509998f * fSlow248)) : 0.0158489998f);
		float fSlow314 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 0.0158489998f : (iSlow267 ? ((0.00193400006f * fSlow220) + 0.0158489998f) : 0.0177829992f)) : (iSlow265 ? (0.0177829992f - (0.00193400006f * fSlow268)) : 0.0158489998f)) : (iSlow263 ? ((0.0239620004f * fSlow266) + 0.0158489998f) : 0.0398110002f)) : (iSlow261 ? (0.0398110002f - (0.0298110005f * fSlow264)) : 0.00999999978f)) : (iSlow260 ? ((0.344812989f * fSlow262) + 0.00999999978f) : 0.35481301f)) : (iSlow258 ? (0.35481301f - (0.103624001f * fSlow219)) : 0.251188993f)) : (iSlow256 ? (0.251188993f - (0.171755999f * fSlow259)) : 0.0794330016f)) : (iSlow253 ? ((0.0205669999f * fSlow257) + 0.0794330016f) : 0.100000001f)) : (iSlow251 ? (0.100000001f - (0.0601890013f * fSlow254)) : 0.0398110002f)) : (iSlow249 ? ((0.0232849997f * fSlow252) + 0.0398110002f) : 0.0630960017f)) : (iSlow247 ? ((0.0369039997f * fSlow250) + 0.0630960017f) : 0.100000001f)) : fSlow313) : (iSlow243 ? ((0.0635839999f * fSlow246) + 0.0158489998f) : 0.0794330016f)) : (iSlow241 ? (0.0794330016f - (0.0478099994f * fSlow244)) : 0.0316229984f)) : (iSlow239 ? ((0.0683770031f * fSlow242) + 0.0316229984f) : 0.100000001f)) : (iSlow236 ? (0.100000001f - (0.0900000036f * fSlow240)) : 0.00999999978f)) : (iSlow234 ? ((0.0401189998f * fSlow237) + 0.00999999978f) : 0.0501190014f)) : fSlow302) : (iSlow230 ? (0.0794330016f - (0.0694330037f * fSlow233)) : 0.00999999978f)) : (iSlow228 ? ((0.388107002f * fSlow231) + 0.00999999978f) : 0.398106992f)) : (iSlow226 ? (0.398106992f - (0.198580995f * fSlow229)) : 0.199525997f)) : (iSlow224 ? (0.199525997f - (0.099526003f * fSlow227)) : 0.100000001f)) : (iSlow221 ? ((0.151188999f * fSlow225) + 0.100000001f) : 0.251188993f)) : (iSlow222 ? (0.251188993f - (0.0516630001f * fSlow223)) : 0.199525997f));
		float fSlow315 = (fSlow307 * fSlow312);
		float fSlow316 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? 4950.0f : (iSlow260 ? (4950.0f - (2200.0f * fSlow262)) : 2750.0f)) : (iSlow258 ? (fSlow305 + 2750.0f) : 3100.0f)) : (iSlow256 ? ((240.0f * fSlow259) + 3100.0f) : 3340.0f)) : (iSlow253 ? (3340.0f - (440.0f * fSlow257)) : 2900.0f)) : (iSlow251 ? (fSlow255 + 2900.0f) : 2950.0f)) : (iSlow249 ? ((400.0f * fSlow252) + 2950.0f) : 3350.0f)) : (iSlow247 ? (3350.0f - fSlow290) : 3300.0f)) : (iSlow245 ? ((290.0f * fSlow248) + 3300.0f) : 3590.0f)) : (iSlow243 ? (3590.0f - (290.0f * fSlow246)) : 3300.0f)) : (iSlow241 ? ((100.0f * fSlow244) + 3300.0f) : 3400.0f)) : (iSlow239 ? ((1550.0f * fSlow242) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow228 ? (4950.0f - (1700.0f * fSlow231)) : 3250.0f)) : (iSlow226 ? ((330.0f * fSlow229) + 3250.0f) : 3580.0f)) : (iSlow224 ? (3580.0f - (40.0f * fSlow227)) : 3540.0f)) : (iSlow221 ? (3540.0f - (540.0f * fSlow225)) : 3000.0f)) : (iSlow222 ? ((300.0f * fSlow223) + 3000.0f) : 3300.0f));
		float fSlow317 = std::tan((fConst18 * fSlow316));
		float fSlow318 = (1.0f / fSlow317);
		float fSlow319 = ((iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? (iSlow267 ? (iSlow269 ? 140.0f : (iSlow267 ? ((60.0f * fSlow220) + 140.0f) : 200.0f)) : 200.0f) : (iSlow263 ? (200.0f - (65.0f * fSlow266)) : 135.0f)) : (iSlow261 ? ((65.0f * fSlow264) + 135.0f) : 200.0f)) : (iSlow260 ? (200.0f - (70.0f * fSlow262)) : 130.0f)) : (iSlow258 ? (130.0f - fSlow285) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow249 ? (fSlow296 + 120.0f) : 140.0f)) : (iSlow247 ? (140.0f - fSlow295) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow239 ? (fSlow294 + 120.0f) : 140.0f)) : (iSlow236 ? ((60.0f * fSlow240) + 140.0f) : 200.0f)) : (iSlow234 ? (200.0f - fSlow238) : 120.0f)) : 120.0f) : (iSlow230 ? ((80.0f * fSlow233) + 120.0f) : 200.0f)) : (iSlow228 ? (200.0f - (60.0f * fSlow231)) : 140.0f)) : (iSlow226 ? (140.0f - fSlow271) : 120.0f)) : 120.0f) : (iSlow221 ? ((15.0f * fSlow225) + 120.0f) : 135.0f)) : (iSlow222 ? (135.0f - (15.0f * fSlow223)) : 120.0f)) / fSlow316);
		float fSlow320 = (((fSlow318 - fSlow319) / fSlow317) + 1.0f);
		float fSlow321 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow317)));
		float fSlow322 = (((fSlow318 + fSlow319) / fSlow317) + 1.0f);
		float fSlow323 = (iSlow221 ? (iSlow224 ? (iSlow226 ? (iSlow228 ? (iSlow230 ? (iSlow232 ? (iSlow234 ? (iSlow236 ? (iSlow239 ? (iSlow241 ? (iSlow243 ? (iSlow245 ? (iSlow247 ? (iSlow249 ? (iSlow251 ? (iSlow253 ? (iSlow256 ? (iSlow258 ? (iSlow260 ? (iSlow261 ? (iSlow263 ? (iSlow265 ? 0.00100000005f : (iSlow263 ? ((0.000777999987f * fSlow266) + 0.00100000005f) : 0.00177800003f)) : (iSlow261 ? (0.00177800003f - (0.001147f * fSlow264)) : 0.000630999974f)) : (iSlow260 ? ((0.0993689969f * fSlow262) + 0.000630999974f) : 0.100000001f)) : (iSlow258 ? ((0.0258930009f * fSlow219) + 0.100000001f) : 0.125892997f)) : (iSlow256 ? (0.125892997f - (0.0860819966f * fSlow259)) : 0.0398110002f)) : (iSlow253 ? (0.0398110002f - (0.0298110005f * fSlow257)) : 0.00999999978f)) : (iSlow251 ? ((0.00584900007f * fSlow254) + 0.00999999978f) : 0.0158489998f)) : (iSlow249 ? (0.0158489998f - (0.00325999991f * fSlow252)) : 0.0125890002f)) : (iSlow247 ? ((0.0874110013f * fSlow250) + 0.0125890002f) : 0.100000001f)) : fSlow313) : (iSlow243 ? ((0.00410400005f * fSlow246) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow239 ? (0.0199529994f - (0.0167909991f * fSlow242)) : 0.0031620001f)) : (iSlow236 ? (0.0031620001f - (0.00157700002f * fSlow240)) : 0.00158499996f)) : (iSlow234 ? ((0.00472499989f * fSlow237) + 0.00158499996f) : 0.00631000008f)) : (iSlow232 ? (0.00631000008f - (0.00314799999f * fSlow235)) : 0.0031620001f)) : (iSlow230 ? (0.0031620001f - (0.00216199993f * fSlow233)) : 0.00100000005f)) : (iSlow228 ? ((0.0784329996f * fSlow231) + 0.00100000005f) : 0.0794330016f)) : (iSlow226 ? ((0.0205669999f * fSlow229) + 0.0794330016f) : 0.100000001f)) : (iSlow224 ? (0.100000001f - (0.0683770031f * fSlow227)) : 0.0316229984f)) : (iSlow221 ? ((0.0184959993f * fSlow225) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow324 = (fSlow317 * fSlow322);
		float fSlow325 = (fConst31 * ((1.0f / float(fHslider69)) + -1.0f));
		float fSlow326 = float(fHslider70);
		int iSlow327 = (std::fabs(fSlow326) < 1.1920929e-07f);
		float fSlow328 = (iSlow327 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow327 ? 1.0f : fSlow326)))));
		float fSlow329 = float(fHslider71);
		int iSlow330 = (std::fabs(fSlow329) < 1.1920929e-07f);
		float fSlow331 = (iSlow330 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow330 ? 1.0f : fSlow329)))));
		float fSlow332 = (1.0f - fSlow331);
		float fSlow333 = (1.0f - fSlow328);
		float fSlow334 = float(fHslider72);
		float fSlow335 = (1.0f / float(fHslider73));
		float fSlow336 = std::tan((fConst18 * float(fHslider74)));
		float fSlow337 = (1.0f / fSlow336);
		float fSlow338 = (1.0f / (((fSlow335 + fSlow337) / fSlow336) + 1.0f));
		float fSlow339 = (((fSlow337 - fSlow335) / fSlow336) + 1.0f);
		float fSlow340 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow336))));
		float fSlow341 = (1.0f / fSlow214);
		float fSlow342 = float(fHslider75);
		int iSlow343 = (fSlow342 > 0.0f);
		float fSlow344 = (float(fHslider76) * std::sin((fConst32 * fSlow213)));
		float fSlow345 = (fConst18 * ((fSlow213 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow342)))) / fSlow344));
		float fSlow346 = (fConst18 * (fSlow213 / fSlow344));
		float fSlow347 = (iSlow343 ? fSlow346 : fSlow345);
		float fSlow348 = ((fSlow341 * (fSlow341 - fSlow347)) + 1.0f);
		float fSlow349 = ((fSlow341 * (fSlow341 + fSlow347)) + 1.0f);
		float fSlow350 = (iSlow343 ? fSlow345 : fSlow346);
		float fSlow351 = ((fSlow341 * (fSlow341 + fSlow350)) + 1.0f);
		float fSlow352 = ((fSlow341 * (fSlow341 - fSlow350)) + 1.0f);
		float fSlow353 = (1.0f / fSlow211);
		float fSlow354 = float(fHslider77);
		int iSlow355 = (fSlow354 > 0.0f);
		float fSlow356 = (float(fHslider78) * std::sin((fConst32 * fSlow210)));
		float fSlow357 = (fConst18 * ((fSlow210 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow354)))) / fSlow356));
		float fSlow358 = (fConst18 * (fSlow210 / fSlow356));
		float fSlow359 = (iSlow355 ? fSlow358 : fSlow357);
		float fSlow360 = ((fSlow353 * (fSlow353 - fSlow359)) + 1.0f);
		float fSlow361 = ((fSlow353 * (fSlow353 + fSlow359)) + 1.0f);
		float fSlow362 = (iSlow355 ? fSlow357 : fSlow358);
		float fSlow363 = ((fSlow353 * (fSlow353 + fSlow362)) + 1.0f);
		float fSlow364 = ((fSlow353 * (fSlow353 - fSlow362)) + 1.0f);
		float fSlow365 = (1.0f / fSlow208);
		float fSlow366 = (((fSlow207 - fSlow205) / fSlow206) + 1.0f);
		float fSlow367 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow206))));
		float fSlow368 = (1.0f / float(fHslider80));
		float fSlow369 = std::tan((fConst18 * float(fHslider81)));
		float fSlow370 = (1.0f / fSlow369);
		float fSlow371 = (((fSlow368 + fSlow370) / fSlow369) + 1.0f);
		float fSlow372 = ((std::pow(10.0f, (0.0500000007f * float(fHslider79))) * (1.0f - float(fCheckbox5))) / fSlow371);
		float fSlow373 = float(fHslider82);
		float fSlow374 = std::tan((fConst18 * fSlow373));
		float fSlow375 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow374))));
		float fSlow376 = float(fHslider83);
		float fSlow377 = std::tan((fConst18 * fSlow376));
		float fSlow378 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow377))));
		float fSlow379 = float(fHslider84);
		float fSlow380 = (0.125f * fSlow379);
		float fSlow381 = float(fHslider85);
		float fSlow382 = (fConst109 * ((11.1111107f * (fSlow381 + -0.0359999985f)) + 1.0f));
		float fSlow383 = (fConst111 * fSlow379);
		float fSlow384 = float(fHslider86);
		float fSlow385 = float((fSlow384 > 0.0f));
		float fSlow386 = (0.0f - fSlow384);
		float fSlow387 = float(fHslider87);
		float fSlow388 = (0.400000006f * (float(fHslider88) + -0.5f));
		float fSlow389 = (fConst119 * (fSlow388 + 0.270000011f));
		float fSlow390 = (fConst119 * (0.730000019f - fSlow388));
		float fSlow391 = (fConst31 * ((1.0f / float(fHslider89)) + -1.0f));
		float fSlow392 = float(fHslider90);
		int iSlow393 = (std::fabs(fSlow392) < 1.1920929e-07f);
		float fSlow394 = (iSlow393 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow393 ? 1.0f : fSlow392)))));
		float fSlow395 = float(fHslider91);
		int iSlow396 = (std::fabs(fSlow395) < 1.1920929e-07f);
		float fSlow397 = (iSlow396 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow396 ? 1.0f : fSlow395)))));
		float fSlow398 = (1.0f - fSlow397);
		float fSlow399 = (1.0f - fSlow394);
		float fSlow400 = float(fHslider92);
		float fSlow401 = (1.0f / float(fHslider93));
		float fSlow402 = std::tan((fConst18 * float(fHslider94)));
		float fSlow403 = (1.0f / fSlow402);
		float fSlow404 = (1.0f / (((fSlow401 + fSlow403) / fSlow402) + 1.0f));
		float fSlow405 = (((fSlow403 - fSlow401) / fSlow402) + 1.0f);
		float fSlow406 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow402))));
		float fSlow407 = (1.0f / fSlow377);
		float fSlow408 = float(fHslider95);
		int iSlow409 = (fSlow408 > 0.0f);
		float fSlow410 = (float(fHslider96) * std::sin((fConst32 * fSlow376)));
		float fSlow411 = (fConst18 * ((fSlow376 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow408)))) / fSlow410));
		float fSlow412 = (fConst18 * (fSlow376 / fSlow410));
		float fSlow413 = (iSlow409 ? fSlow412 : fSlow411);
		float fSlow414 = ((fSlow407 * (fSlow407 - fSlow413)) + 1.0f);
		float fSlow415 = ((fSlow407 * (fSlow407 + fSlow413)) + 1.0f);
		float fSlow416 = (iSlow409 ? fSlow411 : fSlow412);
		float fSlow417 = ((fSlow407 * (fSlow407 + fSlow416)) + 1.0f);
		float fSlow418 = ((fSlow407 * (fSlow407 - fSlow416)) + 1.0f);
		float fSlow419 = (1.0f / fSlow374);
		float fSlow420 = float(fHslider97);
		int iSlow421 = (fSlow420 > 0.0f);
		float fSlow422 = (float(fHslider98) * std::sin((fConst32 * fSlow373)));
		float fSlow423 = (fConst18 * ((fSlow373 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow420)))) / fSlow422));
		float fSlow424 = (fConst18 * (fSlow373 / fSlow422));
		float fSlow425 = (iSlow421 ? fSlow424 : fSlow423);
		float fSlow426 = ((fSlow419 * (fSlow419 - fSlow425)) + 1.0f);
		float fSlow427 = ((fSlow419 * (fSlow419 + fSlow425)) + 1.0f);
		float fSlow428 = (iSlow421 ? fSlow423 : fSlow424);
		float fSlow429 = ((fSlow419 * (fSlow419 + fSlow428)) + 1.0f);
		float fSlow430 = ((fSlow419 * (fSlow419 - fSlow428)) + 1.0f);
		float fSlow431 = (1.0f / fSlow371);
		float fSlow432 = (((fSlow370 - fSlow368) / fSlow369) + 1.0f);
		float fSlow433 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow369))));
		float fSlow434 = (1.0f / float(fHslider100));
		float fSlow435 = std::tan((fConst18 * float(fHslider101)));
		float fSlow436 = (1.0f / fSlow435);
		float fSlow437 = (((fSlow434 + fSlow436) / fSlow435) + 1.0f);
		float fSlow438 = ((std::pow(10.0f, (0.0500000007f * float(fHslider99))) * (1.0f - float(fCheckbox6))) / fSlow437);
		float fSlow439 = float(fHslider102);
		float fSlow440 = std::tan((fConst18 * fSlow439));
		float fSlow441 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow440))));
		float fSlow442 = float(fHslider103);
		float fSlow443 = std::tan((fConst18 * fSlow442));
		float fSlow444 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow443))));
		float fSlow445 = (0.5f * fSlow17);
		float fSlow446 = (fConst228 * fSlow18);
		float fSlow447 = (27.0f * fSlow18);
		float fSlow448 = (9.0f * fSlow18);
		float fSlow449 = (3.0f * fSlow18);
		float fSlow450 = (fConst31 * ((1.0f / float(fHslider104)) + -1.0f));
		float fSlow451 = float(fHslider105);
		int iSlow452 = (std::fabs(fSlow451) < 1.1920929e-07f);
		float fSlow453 = (iSlow452 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow452 ? 1.0f : fSlow451)))));
		float fSlow454 = float(fHslider106);
		int iSlow455 = (std::fabs(fSlow454) < 1.1920929e-07f);
		float fSlow456 = (iSlow455 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow455 ? 1.0f : fSlow454)))));
		float fSlow457 = (1.0f - fSlow456);
		float fSlow458 = (1.0f - fSlow453);
		float fSlow459 = float(fHslider107);
		float fSlow460 = (1.0f / float(fHslider108));
		float fSlow461 = std::tan((fConst18 * float(fHslider109)));
		float fSlow462 = (1.0f / fSlow461);
		float fSlow463 = (1.0f / (((fSlow460 + fSlow462) / fSlow461) + 1.0f));
		float fSlow464 = (((fSlow462 - fSlow460) / fSlow461) + 1.0f);
		float fSlow465 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow461))));
		float fSlow466 = (1.0f / fSlow443);
		float fSlow467 = float(fHslider110);
		int iSlow468 = (fSlow467 > 0.0f);
		float fSlow469 = (float(fHslider111) * std::sin((fConst32 * fSlow442)));
		float fSlow470 = (fConst18 * ((fSlow442 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow467)))) / fSlow469));
		float fSlow471 = (fConst18 * (fSlow442 / fSlow469));
		float fSlow472 = (iSlow468 ? fSlow471 : fSlow470);
		float fSlow473 = ((fSlow466 * (fSlow466 - fSlow472)) + 1.0f);
		float fSlow474 = ((fSlow466 * (fSlow466 + fSlow472)) + 1.0f);
		float fSlow475 = (iSlow468 ? fSlow470 : fSlow471);
		float fSlow476 = ((fSlow466 * (fSlow466 + fSlow475)) + 1.0f);
		float fSlow477 = ((fSlow466 * (fSlow466 - fSlow475)) + 1.0f);
		float fSlow478 = (1.0f / fSlow440);
		float fSlow479 = float(fHslider112);
		int iSlow480 = (fSlow479 > 0.0f);
		float fSlow481 = (float(fHslider113) * std::sin((fConst32 * fSlow439)));
		float fSlow482 = (fConst18 * ((fSlow439 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow479)))) / fSlow481));
		float fSlow483 = (fConst18 * (fSlow439 / fSlow481));
		float fSlow484 = (iSlow480 ? fSlow483 : fSlow482);
		float fSlow485 = ((fSlow478 * (fSlow478 - fSlow484)) + 1.0f);
		float fSlow486 = ((fSlow478 * (fSlow478 + fSlow484)) + 1.0f);
		float fSlow487 = (iSlow480 ? fSlow482 : fSlow483);
		float fSlow488 = ((fSlow478 * (fSlow478 + fSlow487)) + 1.0f);
		float fSlow489 = ((fSlow478 * (fSlow478 - fSlow487)) + 1.0f);
		float fSlow490 = (1.0f / fSlow437);
		float fSlow491 = (((fSlow436 - fSlow434) / fSlow435) + 1.0f);
		float fSlow492 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow435))));
		float fSlow493 = (1.0f / float(fHslider115));
		float fSlow494 = std::tan((fConst18 * float(fHslider116)));
		float fSlow495 = (1.0f / fSlow494);
		float fSlow496 = (((fSlow493 + fSlow495) / fSlow494) + 1.0f);
		float fSlow497 = ((std::pow(10.0f, (0.0500000007f * float(fHslider114))) * (1.0f - float(fCheckbox7))) / fSlow496);
		float fSlow498 = float(fHslider117);
		float fSlow499 = std::tan((fConst18 * fSlow498));
		float fSlow500 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow499))));
		float fSlow501 = float(fHslider118);
		float fSlow502 = std::tan((fConst18 * fSlow501));
		float fSlow503 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow502))));
		float fSlow504 = float(fHslider119);
		float fSlow505 = float((fSlow504 > 0.0f));
		float fSlow506 = (0.0f - fSlow504);
		float fSlow507 = float(fHslider120);
		float fSlow508 = (0.00100000005f * float((fSlow507 > 310.0f)));
		float fSlow509 = (fConst29 * std::max<float>(20.0f, fSlow507));
		float fSlow510 = float(fHslider121);
		float fSlow511 = (0.00100000005f * float((fSlow510 > 610.0f)));
		float fSlow512 = (fConst29 * std::max<float>(20.0f, fSlow510));
		float fSlow513 = float(fHslider122);
		float fSlow514 = (0.00100000005f * float((fSlow513 > 1210.0f)));
		float fSlow515 = (fConst29 * std::max<float>(20.0f, fSlow513));
		float fSlow516 = (fConst31 * ((1.0f / float(fHslider123)) + -1.0f));
		float fSlow517 = float(fHslider124);
		int iSlow518 = (std::fabs(fSlow517) < 1.1920929e-07f);
		float fSlow519 = (iSlow518 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow518 ? 1.0f : fSlow517)))));
		float fSlow520 = float(fHslider125);
		int iSlow521 = (std::fabs(fSlow520) < 1.1920929e-07f);
		float fSlow522 = (iSlow521 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow521 ? 1.0f : fSlow520)))));
		float fSlow523 = (1.0f - fSlow522);
		float fSlow524 = (1.0f - fSlow519);
		float fSlow525 = float(fHslider126);
		float fSlow526 = (1.0f / float(fHslider127));
		float fSlow527 = std::tan((fConst18 * float(fHslider128)));
		float fSlow528 = (1.0f / fSlow527);
		float fSlow529 = (1.0f / (((fSlow526 + fSlow528) / fSlow527) + 1.0f));
		float fSlow530 = (((fSlow528 - fSlow526) / fSlow527) + 1.0f);
		float fSlow531 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow527))));
		float fSlow532 = (1.0f / fSlow502);
		float fSlow533 = float(fHslider129);
		int iSlow534 = (fSlow533 > 0.0f);
		float fSlow535 = (float(fHslider130) * std::sin((fConst32 * fSlow501)));
		float fSlow536 = (fConst18 * ((fSlow501 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow533)))) / fSlow535));
		float fSlow537 = (fConst18 * (fSlow501 / fSlow535));
		float fSlow538 = (iSlow534 ? fSlow537 : fSlow536);
		float fSlow539 = ((fSlow532 * (fSlow532 - fSlow538)) + 1.0f);
		float fSlow540 = ((fSlow532 * (fSlow532 + fSlow538)) + 1.0f);
		float fSlow541 = (iSlow534 ? fSlow536 : fSlow537);
		float fSlow542 = ((fSlow532 * (fSlow532 + fSlow541)) + 1.0f);
		float fSlow543 = ((fSlow532 * (fSlow532 - fSlow541)) + 1.0f);
		float fSlow544 = (1.0f / fSlow499);
		float fSlow545 = float(fHslider131);
		int iSlow546 = (fSlow545 > 0.0f);
		float fSlow547 = (float(fHslider132) * std::sin((fConst32 * fSlow498)));
		float fSlow548 = (fConst18 * ((fSlow498 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow545)))) / fSlow547));
		float fSlow549 = (fConst18 * (fSlow498 / fSlow547));
		float fSlow550 = (iSlow546 ? fSlow549 : fSlow548);
		float fSlow551 = ((fSlow544 * (fSlow544 - fSlow550)) + 1.0f);
		float fSlow552 = ((fSlow544 * (fSlow544 + fSlow550)) + 1.0f);
		float fSlow553 = (iSlow546 ? fSlow548 : fSlow549);
		float fSlow554 = ((fSlow544 * (fSlow544 + fSlow553)) + 1.0f);
		float fSlow555 = ((fSlow544 * (fSlow544 - fSlow553)) + 1.0f);
		float fSlow556 = (1.0f / fSlow496);
		float fSlow557 = (((fSlow495 - fSlow493) / fSlow494) + 1.0f);
		float fSlow558 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow494))));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec12[0] = (0.0f - (fConst14 * ((fConst15 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fConst10 * fRec11[1]) + (fConst11 * (fRec8[1] + (fConst12 * fRec12[0]))));
			fVec1[(IOTA & 32767)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fVec2[0] = fSlow14;
			float fTemp0 = (fSlow14 - fVec2[1]);
			fVec3[0] = fSlow16;
			float fTemp1 = (0.0f - (fSlow14 + fVec3[1]));
			float fTemp2 = std::min<float>(1.0f, ((fTemp0 * float((fTemp0 > 0.0f))) + (fSlow15 * (fTemp1 * float((fTemp1 > 0.0f))))));
			fVec4[0] = fTemp2;
			iRec17[0] = (((iRec17[1] + (iRec17[1] > 0)) * (fTemp2 <= fVec4[1])) + (fTemp2 > fVec4[1]));
			float fTemp3 = float(iRec17[0]);
			float fTemp4 = (fConst22 * fTemp3);
			float fTemp5 = (fConst21 - fTemp3);
			fRec16[0] = ((fConst19 * fRec16[1]) + (fConst20 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp4, ((fConst23 * fTemp5) + 1.0f))))));
			int iTemp6 = int(fTemp2);
			fRec20[0] = (iTemp6 ? fSlow20 : fRec20[1]);
			float fTemp7 = (fRec20[0] + 0.5f);
			iRec19[0] = ((iVec0[1] + iRec19[1]) % int((fConst0 / fTemp7)));
			float fTemp8 = float(iRec19[0]);
			int iTemp9 = (fTemp2 > 0.0f);
			float fTemp10 = (0.144717798f * (iTemp9 ? 0.00100000005f : 1.60000002f));
			int iTemp11 = (std::fabs(fTemp10) < 1.1920929e-07f);
			float fTemp12 = (iTemp11 ? 0.0f : std::exp((0.0f - (fConst29 / (iTemp11 ? 1.0f : fTemp10)))));
			fRec22[0] = ((fRec22[1] * fTemp12) + ((iTemp9 ? fTemp2 : 0.400000006f) * (1.0f - fTemp12)));
			fRec21[0] = ((fConst27 * fRec21[1]) + (fConst28 * fRec22[0]));
			float fTemp13 = (fRec21[0] + 0.00999999978f);
			float fTemp14 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp7) * (1.0f - (0.000500000024f * std::min<float>(fTemp7, 1000.0f)))))))));
			float fTemp15 = (1.0f / fTemp14);
			float fTemp16 = (((fTemp15 + 1.41421354f) / fTemp14) + 1.0f);
			fRec18[0] = ((2.0f * (((float((fTemp8 < (fConst24 / fTemp7))) + -0.5f) + (float((fTemp8 < (fConst25 / fTemp7))) + -0.5f)) + (float((fTemp8 < (fConst26 / fTemp7))) + -0.5f))) - (((fRec18[2] * (((fTemp15 + -1.41421354f) / fTemp14) + 1.0f)) + (2.0f * (fRec18[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp14)))))) / fTemp16));
			fRec25[0] = (iTemp6 ? fSlow21 : fRec25[1]);
			float fTemp17 = (fRec25[0] + 0.5f);
			iRec24[0] = ((iVec0[1] + iRec24[1]) % int((fConst0 / fTemp17)));
			float fTemp18 = float(iRec24[0]);
			float fTemp19 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp17) * (1.0f - (0.000500000024f * std::min<float>(fTemp17, 1000.0f)))))))));
			float fTemp20 = (1.0f / fTemp19);
			float fTemp21 = (((fTemp20 + 1.41421354f) / fTemp19) + 1.0f);
			fRec23[0] = ((2.0f * (((float((fTemp18 < (fConst24 / fTemp17))) + -0.5f) + (float((fTemp18 < (fConst25 / fTemp17))) + -0.5f)) + (float((fTemp18 < (fConst26 / fTemp17))) + -0.5f))) - (((fRec23[2] * (((fTemp20 + -1.41421354f) / fTemp19) + 1.0f)) + (2.0f * (fRec23[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp19)))))) / fTemp21));
			fRec28[0] = (iTemp6 ? fSlow22 : fRec28[1]);
			float fTemp22 = (fRec28[0] + 0.5f);
			iRec27[0] = ((iVec0[1] + iRec27[1]) % int((fConst0 / fTemp22)));
			float fTemp23 = float(iRec27[0]);
			float fTemp24 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp22) * (1.0f - (0.000500000024f * std::min<float>(fTemp22, 1000.0f)))))))));
			float fTemp25 = (1.0f / fTemp24);
			float fTemp26 = (((fTemp25 + 1.41421354f) / fTemp24) + 1.0f);
			fRec26[0] = ((2.0f * (((float((fTemp23 < (fConst24 / fTemp22))) + -0.5f) + (float((fTemp23 < (fConst25 / fTemp22))) + -0.5f)) + (float((fTemp23 < (fConst26 / fTemp22))) + -0.5f))) - (((fRec26[2] * (((fTemp25 + -1.41421354f) / fTemp24) + 1.0f)) + (2.0f * (fRec26[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp24)))))) / fTemp26));
			fRec31[0] = (iTemp6 ? fSlow23 : fRec31[1]);
			float fTemp27 = (fRec31[0] + 0.5f);
			iRec30[0] = ((iVec0[1] + iRec30[1]) % int((fConst0 / fTemp27)));
			float fTemp28 = float(iRec30[0]);
			float fTemp29 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp27) * (1.0f - (0.000500000024f * std::min<float>(fTemp27, 1000.0f)))))))));
			float fTemp30 = (1.0f / fTemp29);
			float fTemp31 = (((fTemp30 + 1.41421354f) / fTemp29) + 1.0f);
			fRec29[0] = ((2.0f * (((float((fTemp28 < (fConst24 / fTemp27))) + -0.5f) + (float((fTemp28 < (fConst25 / fTemp27))) + -0.5f)) + (float((fTemp28 < (fConst26 / fTemp27))) + -0.5f))) - (((fRec29[2] * (((fTemp30 + -1.41421354f) / fTemp29) + 1.0f)) + (2.0f * (fRec29[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp29)))))) / fTemp31));
			float fTemp32 = (fRec16[0] * (((((fRec18[2] + (fRec18[0] + (2.0f * fRec18[1]))) / fTemp16) + ((fRec23[2] + (fRec23[0] + (2.0f * fRec23[1]))) / fTemp21)) + ((fRec26[2] + (fRec26[0] + (2.0f * fRec26[1]))) / fTemp26)) + ((fRec29[2] + (fRec29[0] + (2.0f * fRec29[1]))) / fTemp31)));
			float fTemp33 = std::fabs((fSlow13 * fTemp32));
			fRec34[0] = std::max<float>(fTemp33, ((fSlow30 * fRec34[1]) + (fSlow31 * fTemp33)));
			fRec33[0] = ((fSlow27 * fRec33[1]) + (fRec34[0] * fSlow32));
			fRec32[0] = ((fConst30 * fRec32[1]) + (fSlow24 * std::max<float>(((20.0f * std::log10(fRec33[0])) - fSlow33), 0.0f)));
			float fTemp34 = (fSlow13 * (fTemp32 * std::pow(10.0f, (0.0500000007f * fRec32[0]))));
			fRec35[0] = (fTemp34 - (fSlow37 * ((fSlow38 * fRec35[2]) + (fSlow39 * fRec35[1]))));
			float fTemp35 = (fSlow12 * fRec15[1]);
			fRec15[0] = (fTemp34 - ((fSlow37 * (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])))) + (((fSlow47 * fRec15[2]) + fTemp35) / fSlow48)));
			float fTemp36 = (fSlow9 * fRec14[1]);
			fRec14[0] = ((((fTemp35 + (fRec15[0] * fSlow50)) + (fSlow51 * fRec15[2])) / fSlow48) - (((fSlow59 * fRec14[2]) + fTemp36) / fSlow60));
			fRec13[0] = ((((fTemp36 + (fRec14[0] * fSlow62)) + (fSlow63 * fRec14[2])) / fSlow60) - (fSlow64 * ((fSlow65 * fRec13[2]) + (fSlow66 * fRec13[1]))));
			float fTemp37 = (fSlow6 * (fRec13[2] + (fRec13[0] + (2.0f * fRec13[1]))));
			float fRec46 = (0.0f - (0.997305274f * ((0.899999976f * fRec47[2]) + (0.0500000007f * (fRec47[1] + fRec47[3])))));
			fRec67[0] = (fConst35 + (fRec67[1] - std::floor((fConst35 + fRec67[1]))));
			fRec65[0] = ((0.999000013f * fRec65[1]) + (0.00100000005f * ((fSlow79 * ftbl0mydspSIG0[int((65536.0f * fRec67[0]))]) + 1.0f)));
			fVec5[0] = fSlow80;
			float fTemp38 = (fSlow80 - fVec5[1]);
			fVec6[0] = fSlow82;
			float fTemp39 = (0.0f - (fSlow80 + fVec6[1]));
			float fTemp40 = std::min<float>(1.0f, ((fTemp38 * float((fTemp38 > 0.0f))) + (fSlow81 * (fTemp39 * float((fTemp39 > 0.0f))))));
			fRec68[0] = (int(fTemp40) ? fSlow83 : fRec68[1]);
			float fTemp41 = ((fSlow78 / (fRec65[0] * fRec68[0])) + -0.109999999f);
			float fTemp42 = (fConst34 * fTemp41);
			float fTemp43 = (fTemp42 + -1.49999499f);
			int iTemp44 = int(fTemp43);
			int iTemp45 = int(std::min<float>(fConst33, float(std::max<int>(0, iTemp44))));
			float fTemp46 = std::floor(fTemp43);
			float fTemp47 = (fTemp42 + (-1.0f - fTemp46));
			float fTemp48 = (0.0f - fTemp47);
			float fTemp49 = (fTemp42 + (-2.0f - fTemp46));
			float fTemp50 = (0.0f - (0.5f * fTemp49));
			float fTemp51 = (fTemp42 + (-3.0f - fTemp46));
			float fTemp52 = (0.0f - (0.333333343f * fTemp51));
			float fTemp53 = (fTemp42 + (-4.0f - fTemp46));
			float fTemp54 = (0.0f - (0.25f * fTemp53));
			float fTemp55 = (fTemp42 - fTemp46);
			int iTemp56 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp44 + 1)))));
			float fTemp57 = (0.0f - fTemp49);
			float fTemp58 = (0.0f - (0.5f * fTemp51));
			float fTemp59 = (0.0f - (0.333333343f * fTemp53));
			int iTemp60 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp44 + 2)))));
			float fTemp61 = (0.0f - fTemp51);
			float fTemp62 = (0.0f - (0.5f * fTemp53));
			float fTemp63 = (fTemp47 * fTemp49);
			int iTemp64 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp44 + 3)))));
			float fTemp65 = (0.0f - fTemp53);
			float fTemp66 = (fTemp63 * fTemp51);
			int iTemp67 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp44 + 4)))));
			fRec61[0] = (((((fRec42[((IOTA - (iTemp45 + 1)) & 2047)] * fTemp48) * fTemp50) * fTemp52) * fTemp54) + (fTemp55 * ((((((fRec42[((IOTA - (iTemp56 + 1)) & 2047)] * fTemp57) * fTemp58) * fTemp59) + (0.5f * (((fTemp47 * fRec42[((IOTA - (iTemp60 + 1)) & 2047)]) * fTemp61) * fTemp62))) + (0.166666672f * ((fTemp63 * fRec42[((IOTA - (iTemp64 + 1)) & 2047)]) * fTemp65))) + (0.0416666679f * (fTemp66 * fRec42[((IOTA - (iTemp67 + 1)) & 2047)])))));
			fRec69[0] = ((fSlow84 * fRec69[1]) + (fSlow85 * fRec61[1]));
			float fRec62 = fRec69[0];
			fRec71[0] = fRec40[1];
			fRec72[(IOTA & 2047)] = (0.0f - (0.997305274f * ((0.899999976f * fRec71[2]) + (0.0500000007f * (fRec71[1] + fRec71[3])))));
			float fTemp68 = (fConst36 * fTemp41);
			float fTemp69 = (fTemp68 + -1.49999499f);
			int iTemp70 = int(fTemp69);
			int iTemp71 = int(std::min<float>(fConst33, float(std::max<int>(0, iTemp70))));
			float fTemp72 = std::floor(fTemp69);
			float fTemp73 = (fTemp68 + (-1.0f - fTemp72));
			float fTemp74 = (0.0f - fTemp73);
			float fTemp75 = (fTemp68 + (-2.0f - fTemp72));
			float fTemp76 = (0.0f - (0.5f * fTemp75));
			float fTemp77 = (fTemp68 + (-3.0f - fTemp72));
			float fTemp78 = (0.0f - (0.333333343f * fTemp77));
			float fTemp79 = (fTemp68 + (-4.0f - fTemp72));
			float fTemp80 = (0.0f - (0.25f * fTemp79));
			float fTemp81 = (fTemp68 - fTemp72);
			int iTemp82 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp70 + 1)))));
			float fTemp83 = (0.0f - fTemp75);
			float fTemp84 = (0.0f - (0.5f * fTemp77));
			float fTemp85 = (0.0f - (0.333333343f * fTemp79));
			int iTemp86 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp70 + 2)))));
			float fTemp87 = (0.0f - fTemp77);
			float fTemp88 = (0.0f - (0.5f * fTemp79));
			float fTemp89 = (fTemp73 * fTemp75);
			int iTemp90 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp70 + 3)))));
			float fTemp91 = (0.0f - fTemp79);
			float fTemp92 = (fTemp89 * fTemp77);
			int iTemp93 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp70 + 4)))));
			fVec7[0] = (((((fRec72[((IOTA - (iTemp71 + 2)) & 2047)] * fTemp74) * fTemp76) * fTemp78) * fTemp80) + (fTemp81 * ((((((fRec72[((IOTA - (iTemp82 + 2)) & 2047)] * fTemp83) * fTemp84) * fTemp85) + (0.5f * (((fTemp73 * fRec72[((IOTA - (iTemp86 + 2)) & 2047)]) * fTemp87) * fTemp88))) + (0.166666672f * ((fTemp89 * fRec72[((IOTA - (iTemp90 + 2)) & 2047)]) * fTemp91))) + (0.0416666679f * (fTemp92 * fRec72[((IOTA - (iTemp93 + 2)) & 2047)])))));
			float fTemp94 = (fTemp40 + fVec7[1]);
			fVec8[0] = fTemp94;
			fRec70[(IOTA & 2047)] = ((fSlow84 * fRec70[((IOTA - 1) & 2047)]) + (fSlow85 * fVec8[2]));
			float fRec63 = (((((fTemp48 * fTemp50) * fTemp52) * fTemp54) * fRec70[((IOTA - iTemp45) & 2047)]) + (fTemp55 * ((((((fTemp57 * fTemp58) * fTemp59) * fRec70[((IOTA - iTemp56) & 2047)]) + (0.5f * (((fTemp47 * fTemp61) * fTemp62) * fRec70[((IOTA - iTemp60) & 2047)]))) + (0.166666672f * ((fTemp63 * fTemp65) * fRec70[((IOTA - iTemp64) & 2047)]))) + (0.0416666679f * (fTemp66 * fRec70[((IOTA - iTemp67) & 2047)])))));
			float fRec64 = (fVec8[1] + fRec57[1]);
			fRec57[0] = fRec62;
			float fRec58 = fRec57[1];
			float fRec59 = fRec63;
			float fRec60 = fRec64;
			fRec53[0] = fRec58;
			float fRec54 = (fTemp40 + fRec53[1]);
			float fRec55 = fRec59;
			float fRec56 = fRec60;
			fRec49[(IOTA & 2047)] = fRec54;
			float fRec50 = (((((fTemp74 * fTemp76) * fTemp78) * fTemp80) * fRec49[((IOTA - (iTemp71 + 1)) & 2047)]) + (fTemp81 * ((((((fTemp83 * fTemp84) * fTemp85) * fRec49[((IOTA - (iTemp82 + 1)) & 2047)]) + (0.5f * (((fTemp73 * fTemp87) * fTemp88) * fRec49[((IOTA - (iTemp86 + 1)) & 2047)]))) + (0.166666672f * ((fTemp89 * fTemp91) * fRec49[((IOTA - (iTemp90 + 1)) & 2047)]))) + (0.0416666679f * (fTemp92 * fRec49[((IOTA - (iTemp93 + 1)) & 2047)])))));
			float fRec51 = fRec55;
			float fRec52 = fRec56;
			fRec47[0] = fRec51;
			float fRec48 = fRec52;
			fRec42[(IOTA & 2047)] = fRec46;
			float fRec43 = fRec50;
			float fRec44 = fRec47[0];
			float fRec45 = fRec48;
			fRec40[0] = fRec43;
			float fRec41 = fRec45;
			fRec73[0] = (fSlow86 + (0.999000013f * fRec73[1]));
			float fTemp95 = std::min<float>(15000.0f, std::max<float>(500.0f, fRec73[0]));
			float fTemp96 = std::tan((fConst18 * fTemp95));
			float fTemp97 = (1.0f / fTemp96);
			float fTemp98 = (((fTemp97 + 1.42857146f) / fTemp96) + 1.0f);
			fRec39[0] = ((1.5f * fRec41) - (((fRec39[2] * (((fTemp97 + -1.42857146f) / fTemp96) + 1.0f)) + (2.0f * (fRec39[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp96)))))) / fTemp98));
			float fTemp99 = ((fRec39[2] + (fRec39[0] + (2.0f * fRec39[1]))) * ((6.66666674e-05f * (fTemp95 + -500.0f)) + 1.0f));
			float fTemp100 = std::fabs((0.25f * (fTemp99 / fTemp98)));
			fRec76[0] = std::max<float>(fTemp100, ((fRec76[1] * fSlow93) + (fTemp100 * fSlow94)));
			fRec75[0] = ((fRec75[1] * fSlow90) + (fRec76[0] * fSlow95));
			fRec74[0] = ((fConst30 * fRec74[1]) + (fSlow87 * std::max<float>(((20.0f * std::log10(fRec75[0])) - fSlow96), 0.0f)));
			float fTemp101 = (0.25f * ((fTemp99 * std::pow(10.0f, (0.0500000007f * fRec74[0]))) / fTemp98));
			fRec77[0] = (fTemp101 - (fSlow100 * ((fSlow101 * fRec77[2]) + (fSlow102 * fRec77[1]))));
			float fTemp102 = (fSlow77 * fRec38[1]);
			fRec38[0] = (fTemp101 - ((fSlow100 * (fRec77[2] + (fRec77[0] + (2.0f * fRec77[1])))) + (((fRec38[2] * fSlow110) + fTemp102) / fSlow111)));
			float fTemp103 = (fSlow74 * fRec37[1]);
			fRec37[0] = ((((fTemp102 + (fRec38[0] * fSlow113)) + (fRec38[2] * fSlow114)) / fSlow111) - (((fRec37[2] * fSlow122) + fTemp103) / fSlow123));
			fRec36[0] = ((((fTemp103 + (fRec37[0] * fSlow125)) + (fRec37[2] * fSlow126)) / fSlow123) - (fSlow127 * ((fSlow128 * fRec36[2]) + (fSlow129 * fRec36[1]))));
			float fTemp104 = (fSlow71 * (fRec36[2] + (fRec36[0] + (2.0f * fRec36[1]))));
			iRec84[0] = ((1103515245 * iRec84[1]) + 12345);
			float fTemp105 = (4.65661287e-10f * float(iRec84[0]));
			fRec83[0] = (fTemp105 - (fSlow149 * ((fSlow152 * fRec83[2]) + (fSlow153 * fRec83[1]))));
			fRec82[0] = ((fSlow149 * (((fSlow151 * fRec83[0]) + (fSlow154 * fRec83[1])) + (fSlow151 * fRec83[2]))) - (fSlow146 * ((fSlow155 * fRec82[2]) + (fSlow156 * fRec82[1]))));
			fVec9[0] = fSlow158;
			float fTemp106 = (fSlow158 - fVec9[1]);
			fVec10[0] = fSlow160;
			float fTemp107 = (0.0f - (fSlow158 + fVec10[1]));
			float fTemp108 = std::min<float>(1.0f, ((fTemp106 * float((fTemp106 > 0.0f))) + (fSlow159 * (fTemp107 * float((fTemp107 > 0.0f))))));
			fVec11[0] = fTemp108;
			iRec85[0] = (((iRec85[1] + (iRec85[1] > 0)) * (fTemp108 <= fVec11[1])) + (fTemp108 > fVec11[1]));
			float fTemp109 = (fSlow157 * float(iRec85[0]));
			float fTemp110 = (fSlow146 * ((fRec82[2] + (fRec82[0] + (2.0f * fRec82[1]))) * std::max<float>(0.0f, std::min<float>(fTemp109, (2.0f - fTemp109)))));
			fRec86[0] = (int(fTemp108) ? fSlow161 : fRec86[1]);
			fRec81[0] = (fTemp110 - ((fConst39 * (fRec81[1] * std::cos((fConst32 * (fRec86[0] + 50.0f))))) + (fConst40 * fRec81[2])));
			fRec87[0] = (fTemp110 - ((fConst42 * (fRec87[1] * std::cos((fConst32 * (fRec86[0] + 250.0f))))) + (fConst43 * fRec87[2])));
			fRec88[0] = (fTemp110 - ((fConst45 * (fRec88[1] * std::cos((fConst32 * (fRec86[0] + 450.0f))))) + (fConst46 * fRec88[2])));
			fRec89[0] = (fTemp110 - ((fConst48 * (fRec89[1] * std::cos((fConst32 * (fRec86[0] + 650.0f))))) + (fConst49 * fRec89[2])));
			fRec90[0] = (fTemp110 - ((fConst51 * (fRec90[1] * std::cos((fConst32 * (fRec86[0] + 850.0f))))) + (fConst52 * fRec90[2])));
			fRec91[0] = (fTemp110 - ((fConst54 * (fRec91[1] * std::cos((fConst32 * (fRec86[0] + 1050.0f))))) + (fConst55 * fRec91[2])));
			fRec92[0] = (fTemp110 - ((fConst57 * (fRec92[1] * std::cos((fConst32 * (fRec86[0] + 1250.0f))))) + (fConst58 * fRec92[2])));
			fRec93[0] = (fTemp110 - ((fConst60 * (fRec93[1] * std::cos((fConst32 * (fRec86[0] + 1450.0f))))) + (fConst61 * fRec93[2])));
			fRec94[0] = (fTemp110 - ((fConst63 * (fRec94[1] * std::cos((fConst32 * (fRec86[0] + 1650.0f))))) + (fConst64 * fRec94[2])));
			fRec95[0] = (fTemp110 - ((fConst66 * (fRec95[1] * std::cos((fConst32 * (fRec86[0] + 1850.0f))))) + (fConst67 * fRec95[2])));
			fRec96[0] = (fTemp110 - ((fConst69 * (fRec96[1] * std::cos((fConst32 * (fRec86[0] + 2050.0f))))) + (fConst70 * fRec96[2])));
			fRec97[0] = (fTemp110 - ((fConst72 * (fRec97[1] * std::cos((fConst32 * (fRec86[0] + 2250.0f))))) + (fConst73 * fRec97[2])));
			fRec98[0] = (fTemp110 - ((fConst75 * (fRec98[1] * std::cos((fConst32 * (fRec86[0] + 2450.0f))))) + (fConst76 * fRec98[2])));
			fRec99[0] = (fTemp110 - ((fConst78 * (fRec99[1] * std::cos((fConst32 * (fRec86[0] + 2650.0f))))) + (fConst79 * fRec99[2])));
			fRec100[0] = (fTemp110 - ((fConst81 * (fRec100[1] * std::cos((fConst32 * (fRec86[0] + 2850.0f))))) + (fConst82 * fRec100[2])));
			fRec101[0] = (fTemp110 - ((fConst84 * (fRec101[1] * std::cos((fConst32 * (fRec86[0] + 3050.0f))))) + (fConst85 * fRec101[2])));
			fRec102[0] = (fTemp110 - ((fConst87 * (fRec102[1] * std::cos((fConst32 * (fRec86[0] + 3250.0f))))) + (fConst88 * fRec102[2])));
			fRec103[0] = (fTemp110 - ((fConst90 * (fRec103[1] * std::cos((fConst32 * (fRec86[0] + 3450.0f))))) + (fConst91 * fRec103[2])));
			fRec104[0] = (fTemp110 - ((fConst93 * (fRec104[1] * std::cos((fConst32 * (fRec86[0] + 3650.0f))))) + (fConst94 * fRec104[2])));
			fRec105[0] = (fTemp110 - ((fConst96 * (fRec105[1] * std::cos((fConst32 * (fRec86[0] + 3850.0f))))) + (fConst97 * fRec105[2])));
			float fTemp111 = ((((((((((((((((((((fRec81[0] + (0.25f * (fRec87[0] - fRec87[2]))) + (0.111111112f * (fRec88[0] - fRec88[2]))) + (0.0625f * (fRec89[0] - fRec89[2]))) + (0.0399999991f * (fRec90[0] - fRec90[2]))) + (0.027777778f * (fRec91[0] - fRec91[2]))) + (0.0204081628f * (fRec92[0] - fRec92[2]))) + (0.015625f * (fRec93[0] - fRec93[2]))) + (0.0123456791f * (fRec94[0] - fRec94[2]))) + (0.00999999978f * (fRec95[0] - fRec95[2]))) + (0.00826446246f * (fRec96[0] - fRec96[2]))) + (0.0069444445f * (fRec97[0] - fRec97[2]))) + (0.00591715984f * (fRec98[0] - fRec98[2]))) + (0.00510204071f * (fRec99[0] - fRec99[2]))) + (0.00444444455f * (fRec100[0] - fRec100[2]))) + (0.00390625f * (fRec101[0] - fRec101[2]))) + (0.00346020772f * (fRec102[0] - fRec102[2]))) + (0.00308641978f * (fRec103[0] - fRec103[2]))) + (0.00277008303f * (fRec104[0] - fRec104[2]))) + (0.00249999994f * (fRec105[0] - fRec105[2]))) - fRec81[2]);
			float fTemp112 = std::fabs((fSlow142 * fTemp111));
			fRec108[0] = std::max<float>(fTemp112, ((fRec108[1] * fSlow168) + (fTemp112 * fSlow169)));
			fRec107[0] = ((fRec107[1] * fSlow165) + (fRec108[0] * fSlow170));
			fRec106[0] = ((fConst30 * fRec106[1]) + (fSlow162 * std::max<float>(((20.0f * std::log10(fRec107[0])) - fSlow171), 0.0f)));
			float fTemp113 = (fSlow142 * (fTemp111 * std::pow(10.0f, (0.0500000007f * fRec106[0]))));
			fRec109[0] = (fTemp113 - (fSlow175 * ((fSlow176 * fRec109[2]) + (fSlow177 * fRec109[1]))));
			float fTemp114 = (fSlow140 * fRec80[1]);
			fRec80[0] = (fTemp113 - ((fSlow175 * (fRec109[2] + (fRec109[0] + (2.0f * fRec109[1])))) + (((fRec80[2] * fSlow185) + fTemp114) / fSlow186)));
			float fTemp115 = (fSlow137 * fRec79[1]);
			fRec79[0] = ((((fTemp114 + (fRec80[0] * fSlow188)) + (fRec80[2] * fSlow189)) / fSlow186) - (((fRec79[2] * fSlow197) + fTemp115) / fSlow198));
			fRec78[0] = ((((fTemp115 + (fRec79[0] * fSlow200)) + (fRec79[2] * fSlow201)) / fSlow198) - (fSlow202 * ((fSlow203 * fRec78[2]) + (fSlow204 * fRec78[1]))));
			fRec115[0] = (fConst104 + (fRec115[1] - std::floor((fConst104 + fRec115[1]))));
			float fTemp116 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec115[0]))]) + 1.0f);
			fRec114[0] = ((fConst102 * fRec114[1]) + (fSlow217 * fTemp116));
			fRec116[0] = ((fConst102 * fRec116[1]) + (fSlow218 * (fRec65[0] * fTemp116)));
			float fTemp117 = ((0.00366666657f * (400.0f - fRec116[0])) + 3.0f);
			float fTemp118 = std::max<float>(1.1920929e-07f, std::fabs(fRec116[0]));
			float fTemp119 = (fRec118[1] + (fConst29 * fTemp118));
			float fTemp120 = (fTemp119 + -1.0f);
			int iTemp121 = (fTemp120 < 0.0f);
			fRec118[0] = (iTemp121 ? fTemp119 : fTemp120);
			float fRec119 = (iTemp121 ? fTemp119 : (fTemp119 + ((1.0f - (fConst0 / fTemp118)) * fTemp120)));
			float fTemp122 = (0.400000006f * ((2.0f * fRec119) + -1.0f));
			float fTemp123 = ((fSlow270 <= fRec116[0]) ? fRec116[0] : fSlow270);
			float fTemp124 = std::tan((fConst18 * fTemp123));
			float fTemp125 = (1.0f / fTemp124);
			float fTemp126 = (fSlow278 / fTemp123);
			float fTemp127 = (((fTemp125 + fTemp126) / fTemp124) + 1.0f);
			fRec117[0] = (fTemp122 - (((fRec117[2] * (((fTemp125 - fTemp126) / fTemp124) + 1.0f)) + (2.0f * (fRec117[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp124)))))) / fTemp127));
			float fTemp128 = (fTemp124 * fTemp127);
			float fTemp129 = ((2.0f * fRec116[0]) + 30.0f);
			float fTemp130 = ((iSlow280 & (fRec116[0] >= 200.0f)) ? (fSlow279 - (0.000952380942f * ((fRec116[0] + -200.0f) * fSlow281))) : ((fSlow279 <= fTemp129) ? fTemp129 : fSlow279));
			float fTemp131 = std::tan((fConst18 * fTemp130));
			float fTemp132 = (1.0f / fTemp131);
			float fTemp133 = (fSlow286 / fTemp130);
			float fTemp134 = (((fTemp132 + fTemp133) / fTemp131) + 1.0f);
			fRec120[0] = (fTemp122 - (((fRec120[2] * (((fTemp132 - fTemp133) / fTemp131) + 1.0f)) + (2.0f * (fRec120[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp131)))))) / fTemp134));
			float fTemp135 = (fTemp131 * fTemp134);
			fRec121[0] = (fTemp122 - (((fRec121[2] * fSlow299) + (2.0f * (fRec121[1] * fSlow300))) / fSlow301));
			fRec122[0] = (fTemp122 - (((fRec122[2] * fSlow310) + (2.0f * (fRec122[1] * fSlow311))) / fSlow312));
			fRec123[0] = (fTemp122 - (((fRec123[2] * fSlow320) + (2.0f * (fRec123[1] * fSlow321))) / fSlow322));
			fRec113[0] = ((fRec114[0] * ((fTemp117 * (((((fRec117[0] / fTemp128) + ((fRec120[0] * fSlow287) / fTemp135)) + ((fRec121[0] * fSlow303) / fSlow304)) + ((fRec122[0] * fSlow314) / fSlow315)) + ((fRec123[0] * fSlow323) / fSlow324))) + (((((fRec117[2] * (0.0f - (fTemp117 / fTemp128))) + (fRec120[2] * (0.0f - ((fTemp117 * fSlow287) / fTemp135)))) + (fRec121[2] * (0.0f - ((fTemp117 * fSlow303) / fSlow304)))) + (fRec122[2] * (0.0f - ((fTemp117 * fSlow314) / fSlow315)))) + (fRec123[2] * (0.0f - ((fTemp117 * fSlow323) / fSlow324)))))) - (fConst106 * ((fConst107 * fRec113[2]) + (fConst108 * fRec113[1]))));
			float fTemp136 = (fRec113[2] + (fRec113[0] + (2.0f * fRec113[1])));
			float fTemp137 = std::fabs((fSlow216 * fTemp136));
			fRec126[0] = std::max<float>(fTemp137, ((fRec126[1] * fSlow331) + (fTemp137 * fSlow332)));
			fRec125[0] = ((fRec125[1] * fSlow328) + (fRec126[0] * fSlow333));
			fRec124[0] = ((fConst30 * fRec124[1]) + (fSlow325 * std::max<float>(((20.0f * std::log10(fRec125[0])) - fSlow334), 0.0f)));
			float fTemp138 = (fSlow216 * (fTemp136 * std::pow(10.0f, (0.0500000007f * fRec124[0]))));
			fRec127[0] = (fTemp138 - (fSlow338 * ((fSlow339 * fRec127[2]) + (fSlow340 * fRec127[1]))));
			float fTemp139 = (fSlow215 * fRec112[1]);
			fRec112[0] = (fTemp138 - ((fSlow338 * (fRec127[2] + (fRec127[0] + (2.0f * fRec127[1])))) + (((fRec112[2] * fSlow348) + fTemp139) / fSlow349)));
			float fTemp140 = (fSlow212 * fRec111[1]);
			fRec111[0] = ((((fTemp139 + (fRec112[0] * fSlow351)) + (fRec112[2] * fSlow352)) / fSlow349) - (((fRec111[2] * fSlow360) + fTemp140) / fSlow361));
			fRec110[0] = ((((fTemp140 + (fRec111[0] * fSlow363)) + (fRec111[2] * fSlow364)) / fSlow361) - (fSlow365 * ((fSlow366 * fRec110[2]) + (fSlow367 * fRec110[1]))));
			float fTemp141 = ((fSlow134 * (fRec78[2] + (fRec78[0] + (2.0f * fRec78[1])))) + (fSlow209 * (fRec110[2] + (fRec110[0] + (2.0f * fRec110[1])))));
			iRec146[0] = 0;
			fRec152[0] = ((0.699999988f * fRec152[1]) + (0.284999996f * fRec151[1]));
			float fRec150 = (fRec152[0] + float(iRec146[1]));
			fRec153[0] = (fSlow382 + (fRec153[1] - std::floor((fSlow382 + fRec153[1]))));
			fVec12[0] = fSlow384;
			float fTemp142 = (fSlow384 - fVec12[1]);
			fVec13[0] = fSlow386;
			float fTemp143 = (0.0f - (fSlow384 + fVec13[1]));
			fRec157[0] = (int(std::min<float>(1.0f, ((fTemp142 * float((fTemp142 > 0.0f))) + (fSlow385 * (fTemp143 * float((fTemp143 > 0.0f))))))) ? fSlow387 : fRec157[1]);
			fRec156[0] = ((fConst110 * fRec156[1]) + (fConst111 * fRec157[0]));
			float fTemp144 = (fSlow18 * (fRec65[0] * fRec156[0]));
			fRec155[0] = ((fConst102 * fRec155[1]) + (fConst103 * float((fTemp144 > 430.0f))));
			fRec154[0] = ((fConst110 * fRec154[1]) + (fSlow383 * fRec155[0]));
			fRec158[0] = (fTemp105 - (fConst116 * ((fConst117 * fRec158[2]) + (fConst118 * fRec158[1]))));
			fRec159[0] = fRec132[1];
			fRec160[(IOTA & 2047)] = (0.949999988f * fRec159[1]);
			float fTemp145 = (fTemp144 + -440.0f);
			float fTemp146 = ((340.0f / ((fTemp145 * ((fTemp145 * ((fTemp145 * ((4.09700007e-08f * fTemp145) + -2.77700001e-05f)) + 0.00809509959f)) + 1.0f)) + 440.0f)) + 0.270000011f);
			int iTemp147 = int(((fSlow389 * fTemp146) + -1.49999499f));
			int iTemp148 = (int(std::min<float>(fConst33, float(std::max<int>(0, iTemp147)))) + 1);
			float fTemp149 = ((340.0f / ((fTemp145 * ((fTemp145 * ((0.00809509959f - (2.77700001e-05f * fTemp145)) + (4.09700007e-08f * mydsp_faustpower2_f(fTemp145)))) + 1.0f)) + 440.0f)) + 0.270000011f);
			float fTemp150 = (fSlow389 * fTemp149);
			float fTemp151 = std::floor((fTemp150 + -1.49999499f));
			float fTemp152 = (fTemp150 + (-1.0f - fTemp151));
			float fTemp153 = (0.0f - fTemp152);
			float fTemp154 = (fTemp150 + (-2.0f - fTemp151));
			float fTemp155 = (0.0f - (0.5f * fTemp154));
			float fTemp156 = (fTemp150 + (-3.0f - fTemp151));
			float fTemp157 = (0.0f - (0.333333343f * fTemp156));
			float fTemp158 = (fTemp150 + (-4.0f - fTemp151));
			float fTemp159 = (0.0f - (0.25f * fTemp158));
			float fTemp160 = (fTemp150 - fTemp151);
			int iTemp161 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp147 + 1))))) + 1);
			float fTemp162 = (0.0f - fTemp154);
			float fTemp163 = (0.0f - (0.5f * fTemp156));
			float fTemp164 = (0.0f - (0.333333343f * fTemp158));
			int iTemp165 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp147 + 2))))) + 1);
			float fTemp166 = (0.0f - fTemp156);
			float fTemp167 = (0.0f - (0.5f * fTemp158));
			float fTemp168 = (fTemp152 * fTemp154);
			int iTemp169 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp147 + 3))))) + 1);
			float fTemp170 = (0.0f - fTemp158);
			float fTemp171 = (fTemp168 * fTemp156);
			int iTemp172 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp147 + 4))))) + 1);
			fVec14[0] = (((((fRec160[((IOTA - iTemp148) & 2047)] * fTemp153) * fTemp155) * fTemp157) * fTemp159) + (fTemp160 * ((((((fRec160[((IOTA - iTemp161) & 2047)] * fTemp162) * fTemp163) * fTemp164) + (0.5f * (((fTemp152 * fRec160[((IOTA - iTemp165) & 2047)]) * fTemp166) * fTemp167))) + (0.166666672f * ((fTemp168 * fRec160[((IOTA - iTemp169) & 2047)]) * fTemp170))) + (0.0416666679f * (fTemp171 * fRec160[((IOTA - iTemp172) & 2047)])))));
			float fTemp173 = (((fSlow381 * ftbl0mydspSIG0[int((65536.0f * fRec153[0]))]) + (fRec154[0] * ((fConst115 * (fRec158[2] + (fRec158[0] + (2.0f * fRec158[1])))) + 1.0f))) - (0.5f * fVec14[1]));
			float fTemp174 = ((0.5f * fRec138[1]) + std::max<float>(-1.0f, std::min<float>(1.0f, (fTemp173 * (mydsp_faustpower2_f(fTemp173) + -1.0f)))));
			fVec15[(IOTA & 2047)] = fTemp174;
			int iTemp175 = int(((fSlow390 * fTemp146) + -1.49999499f));
			int iTemp176 = (int(std::min<float>(fConst33, float(std::max<int>(0, iTemp175)))) + 1);
			float fTemp177 = (fSlow390 * fTemp149);
			float fTemp178 = std::floor((fTemp177 + -1.49999499f));
			float fTemp179 = (fTemp177 + (-1.0f - fTemp178));
			float fTemp180 = (0.0f - fTemp179);
			float fTemp181 = (fTemp177 + (-2.0f - fTemp178));
			float fTemp182 = (0.0f - (0.5f * fTemp181));
			float fTemp183 = (fTemp177 + (-3.0f - fTemp178));
			float fTemp184 = (0.0f - (0.333333343f * fTemp183));
			float fTemp185 = (fTemp177 + (-4.0f - fTemp178));
			float fTemp186 = (0.0f - (0.25f * fTemp185));
			float fTemp187 = (fTemp177 - fTemp178);
			int iTemp188 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp175 + 1))))) + 1);
			float fTemp189 = (0.0f - fTemp181);
			float fTemp190 = (0.0f - (0.5f * fTemp183));
			float fTemp191 = (0.0f - (0.333333343f * fTemp185));
			int iTemp192 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp175 + 2))))) + 1);
			float fTemp193 = (0.0f - fTemp183);
			float fTemp194 = (0.0f - (0.5f * fTemp185));
			float fTemp195 = (fTemp179 * fTemp181);
			int iTemp196 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp175 + 3))))) + 1);
			float fTemp197 = (0.0f - fTemp185);
			float fTemp198 = (fTemp195 * fTemp183);
			int iTemp199 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp175 + 4))))) + 1);
			fVec16[0] = (((((fVec15[((IOTA - iTemp176) & 2047)] * fTemp180) * fTemp182) * fTemp184) * fTemp186) + (fTemp187 * ((((((fVec15[((IOTA - iTemp188) & 2047)] * fTemp189) * fTemp190) * fTemp191) + (0.5f * (((fTemp179 * fVec15[((IOTA - iTemp192) & 2047)]) * fTemp193) * fTemp194))) + (0.166666672f * ((fTemp195 * fVec15[((IOTA - iTemp196) & 2047)]) * fTemp197))) + (0.0416666679f * (fTemp198 * fVec15[((IOTA - iTemp199) & 2047)])))));
			fRec151[0] = fVec16[1];
			float fRec147 = fRec150;
			float fRec148 = fRec151[0];
			float fRec149 = fRec151[0];
			fRec142[(IOTA & 2047)] = fRec147;
			float fRec143 = (((((fTemp180 * fTemp182) * fTemp184) * fTemp186) * fRec142[((IOTA - iTemp176) & 2047)]) + (fTemp187 * ((((((fTemp189 * fTemp190) * fTemp191) * fRec142[((IOTA - iTemp188) & 2047)]) + (0.5f * (((fTemp179 * fTemp193) * fTemp194) * fRec142[((IOTA - iTemp192) & 2047)]))) + (0.166666672f * ((fTemp195 * fTemp197) * fRec142[((IOTA - iTemp196) & 2047)]))) + (0.0416666679f * (fTemp198 * fRec142[((IOTA - iTemp199) & 2047)])))));
			float fRec144 = fRec148;
			float fRec145 = fRec149;
			fRec138[0] = fRec143;
			float fRec139 = fRec138[1];
			float fRec140 = fRec144;
			float fRec141 = fRec145;
			fRec134[(IOTA & 2047)] = fRec139;
			float fRec135 = (((((fTemp153 * fTemp155) * fTemp157) * fTemp159) * fRec134[((IOTA - iTemp148) & 2047)]) + (fTemp160 * ((((((fTemp162 * fTemp163) * fTemp164) * fRec134[((IOTA - iTemp161) & 2047)]) + (0.5f * (((fTemp152 * fTemp166) * fTemp167) * fRec134[((IOTA - iTemp165) & 2047)]))) + (0.166666672f * ((fTemp168 * fTemp170) * fRec134[((IOTA - iTemp169) & 2047)]))) + (0.0416666679f * (fTemp171 * fRec134[((IOTA - iTemp172) & 2047)])))));
			float fRec136 = fRec140;
			float fRec137 = fRec141;
			fRec132[0] = fRec135;
			fRec133[0] = fRec137;
			fRec131[0] = ((fRec133[0] + (0.995000005f * fRec131[1])) - fRec133[1]);
			float fTemp200 = std::fabs((fSlow380 * fRec131[0]));
			fRec163[0] = std::max<float>(fTemp200, ((fRec163[1] * fSlow397) + (fTemp200 * fSlow398)));
			fRec162[0] = ((fRec162[1] * fSlow394) + (fRec163[0] * fSlow399));
			fRec161[0] = ((fConst30 * fRec161[1]) + (fSlow391 * std::max<float>(((20.0f * std::log10(fRec162[0])) - fSlow400), 0.0f)));
			float fTemp201 = (fSlow380 * (fRec131[0] * std::pow(10.0f, (0.0500000007f * fRec161[0]))));
			fRec164[0] = (fTemp201 - (fSlow404 * ((fSlow405 * fRec164[2]) + (fSlow406 * fRec164[1]))));
			float fTemp202 = (fSlow378 * fRec130[1]);
			fRec130[0] = (fTemp201 - ((fSlow404 * (fRec164[2] + (fRec164[0] + (2.0f * fRec164[1])))) + (((fRec130[2] * fSlow414) + fTemp202) / fSlow415)));
			float fTemp203 = (fSlow375 * fRec129[1]);
			fRec129[0] = ((((fTemp202 + (fRec130[0] * fSlow417)) + (fRec130[2] * fSlow418)) / fSlow415) - (((fRec129[2] * fSlow426) + fTemp203) / fSlow427));
			fRec128[0] = ((((fTemp203 + (fRec129[0] * fSlow429)) + (fRec129[2] * fSlow430)) / fSlow427) - (fSlow431 * ((fSlow432 * fRec128[2]) + (fSlow433 * fRec128[1]))));
			float fTemp204 = (fSlow372 * (fRec128[2] + (fRec128[0] + (2.0f * fRec128[1]))));
			float fTemp205 = (fTemp141 + fTemp204);
			fVec17[(IOTA & 2047)] = (fSlow1 * (fTemp37 + (fTemp104 + fTemp205)));
			float fTemp206 = (0.300000012f * fVec17[((IOTA - iConst120) & 2047)]);
			float fTemp207 = (((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst17) & 32767)]) - fTemp206);
			fVec18[(IOTA & 2047)] = fTemp207;
			fRec9[0] = fVec18[((IOTA - iConst121) & 2047)];
			float fRec10 = (0.0f - (0.600000024f * fTemp207));
			fRec168[0] = (0.0f - (fConst14 * ((fConst15 * fRec168[1]) - (fRec4[1] + fRec4[2]))));
			fRec167[0] = ((fConst130 * fRec167[1]) + (fConst131 * (fRec4[1] + (fConst132 * fRec168[0]))));
			fVec19[(IOTA & 32767)] = ((0.353553385f * fRec167[0]) + 9.99999968e-21f);
			float fTemp208 = (((0.600000024f * fRec165[1]) + fVec19[((IOTA - iConst134) & 32767)]) - fTemp206);
			fVec20[(IOTA & 4095)] = fTemp208;
			fRec165[0] = fVec20[((IOTA - iConst135) & 4095)];
			float fRec166 = (0.0f - (0.600000024f * fTemp208));
			fRec172[0] = (0.0f - (fConst14 * ((fConst15 * fRec172[1]) - (fRec6[1] + fRec6[2]))));
			fRec171[0] = ((fConst144 * fRec171[1]) + (fConst145 * (fRec6[1] + (fConst146 * fRec172[0]))));
			fVec21[(IOTA & 16383)] = ((0.353553385f * fRec171[0]) + 9.99999968e-21f);
			float fTemp209 = (fVec21[((IOTA - iConst148) & 16383)] + (fTemp206 + (0.600000024f * fRec169[1])));
			fVec22[(IOTA & 4095)] = fTemp209;
			fRec169[0] = fVec22[((IOTA - iConst149) & 4095)];
			float fRec170 = (0.0f - (0.600000024f * fTemp209));
			fRec176[0] = (0.0f - (fConst14 * ((fConst15 * fRec176[1]) - (fRec2[1] + fRec2[2]))));
			fRec175[0] = ((fConst158 * fRec175[1]) + (fConst159 * (fRec2[1] + (fConst160 * fRec176[0]))));
			fVec23[(IOTA & 32767)] = ((0.353553385f * fRec175[0]) + 9.99999968e-21f);
			float fTemp210 = (fTemp206 + ((0.600000024f * fRec173[1]) + fVec23[((IOTA - iConst162) & 32767)]));
			fVec24[(IOTA & 4095)] = fTemp210;
			fRec173[0] = fVec24[((IOTA - iConst163) & 4095)];
			float fRec174 = (0.0f - (0.600000024f * fTemp210));
			fRec180[0] = (0.0f - (fConst14 * ((fConst15 * fRec180[1]) - (fRec7[1] + fRec7[2]))));
			fRec179[0] = ((fConst172 * fRec179[1]) + (fConst173 * (fRec7[1] + (fConst174 * fRec180[0]))));
			fVec25[(IOTA & 16383)] = ((0.353553385f * fRec179[0]) + 9.99999968e-21f);
			iRec185[0] = ((iVec0[1] + iRec185[1]) % int((fConst0 / fRec20[0])));
			float fTemp211 = float(iRec185[0]);
			float fTemp212 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fRec20[0] * fTemp13) * (1.0f - (0.000500000024f * std::min<float>(fRec20[0], 1000.0f)))))))));
			float fTemp213 = (1.0f / fTemp212);
			float fTemp214 = (((fTemp213 + 1.41421354f) / fTemp212) + 1.0f);
			fRec184[0] = ((2.0f * (((float((fTemp211 < (fConst24 / fRec20[0]))) + -0.5f) + (float((fTemp211 < (fConst25 / fRec20[0]))) + -0.5f)) + (float((fTemp211 < (fConst26 / fRec20[0]))) + -0.5f))) - (((fRec184[2] * (((fTemp213 + -1.41421354f) / fTemp212) + 1.0f)) + (2.0f * (fRec184[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp212)))))) / fTemp214));
			iRec187[0] = ((iVec0[1] + iRec187[1]) % int((fConst0 / fRec25[0])));
			float fTemp215 = float(iRec187[0]);
			float fTemp216 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec25[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec25[0], 1000.0f)))))))));
			float fTemp217 = (1.0f / fTemp216);
			float fTemp218 = (((fTemp217 + 1.41421354f) / fTemp216) + 1.0f);
			fRec186[0] = ((2.0f * (((float((fTemp215 < (fConst24 / fRec25[0]))) + -0.5f) + (float((fTemp215 < (fConst25 / fRec25[0]))) + -0.5f)) + (float((fTemp215 < (fConst26 / fRec25[0]))) + -0.5f))) - (((fRec186[2] * (((fTemp217 + -1.41421354f) / fTemp216) + 1.0f)) + (2.0f * (fRec186[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp216)))))) / fTemp218));
			iRec189[0] = ((iVec0[1] + iRec189[1]) % int((fConst0 / fRec28[0])));
			float fTemp219 = float(iRec189[0]);
			float fTemp220 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec28[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec28[0], 1000.0f)))))))));
			float fTemp221 = (1.0f / fTemp220);
			float fTemp222 = (((fTemp221 + 1.41421354f) / fTemp220) + 1.0f);
			fRec188[0] = ((2.0f * (((float((fTemp219 < (fConst24 / fRec28[0]))) + -0.5f) + (float((fTemp219 < (fConst25 / fRec28[0]))) + -0.5f)) + (float((fTemp219 < (fConst26 / fRec28[0]))) + -0.5f))) - (((fRec188[2] * (((fTemp221 + -1.41421354f) / fTemp220) + 1.0f)) + (2.0f * (fRec188[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp220)))))) / fTemp222));
			iRec191[0] = ((iVec0[1] + iRec191[1]) % int((fConst0 / fRec31[0])));
			float fTemp223 = float(iRec191[0]);
			float fTemp224 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec31[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec31[0], 1000.0f)))))))));
			float fTemp225 = (1.0f / fTemp224);
			float fTemp226 = (((fTemp225 + 1.41421354f) / fTemp224) + 1.0f);
			fRec190[0] = ((2.0f * (((float((fTemp223 < (fConst24 / fRec31[0]))) + -0.5f) + (float((fTemp223 < (fConst25 / fRec31[0]))) + -0.5f)) + (float((fTemp223 < (fConst26 / fRec31[0]))) + -0.5f))) - (((fRec190[2] * (((fTemp225 + -1.41421354f) / fTemp224) + 1.0f)) + (2.0f * (fRec190[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp224)))))) / fTemp226));
			float fTemp227 = (fRec16[0] * (((((fRec184[2] + (fRec184[0] + (2.0f * fRec184[1]))) / fTemp214) + ((fRec186[2] + (fRec186[0] + (2.0f * fRec186[1]))) / fTemp218)) + ((fRec188[2] + (fRec188[0] + (2.0f * fRec188[1]))) / fTemp222)) + ((fRec190[2] + (fRec190[0] + (2.0f * fRec190[1]))) / fTemp226)));
			float fTemp228 = std::fabs((fSlow13 * fTemp227));
			fRec194[0] = std::max<float>(fTemp228, ((fRec194[1] * fSlow30) + (fTemp228 * fSlow31)));
			fRec193[0] = ((fRec193[1] * fSlow27) + (fRec194[0] * fSlow32));
			fRec192[0] = ((fConst30 * fRec192[1]) + (fSlow24 * std::max<float>(((20.0f * std::log10(fRec193[0])) - fSlow33), 0.0f)));
			float fTemp229 = (fSlow13 * (fTemp227 * std::pow(10.0f, (0.0500000007f * fRec192[0]))));
			fRec195[0] = (fTemp229 - (fSlow37 * ((fSlow38 * fRec195[2]) + (fSlow39 * fRec195[1]))));
			float fTemp230 = (fSlow12 * fRec183[1]);
			fRec183[0] = (fTemp229 - ((fSlow37 * (fRec195[2] + (fRec195[0] + (2.0f * fRec195[1])))) + (((fRec183[2] * fSlow47) + fTemp230) / fSlow48)));
			float fTemp231 = (fSlow9 * fRec182[1]);
			fRec182[0] = ((((fTemp230 + (fRec183[0] * fSlow50)) + (fRec183[2] * fSlow51)) / fSlow48) - (((fRec182[2] * fSlow59) + fTemp231) / fSlow60));
			fRec181[0] = ((((fTemp231 + (fRec182[0] * fSlow62)) + (fRec182[2] * fSlow63)) / fSlow60) - (fSlow64 * ((fSlow65 * fRec181[2]) + (fSlow66 * fRec181[1]))));
			float fTemp232 = ((fTemp141 + (fSlow6 * (fRec181[2] + (fRec181[0] + (2.0f * fRec181[1]))))) + fTemp204);
			fVec26[(IOTA & 2047)] = (fSlow1 * (fTemp232 + fTemp104));
			float fTemp233 = (0.300000012f * fVec26[((IOTA - iConst120) & 2047)]);
			float fTemp234 = (fVec25[((IOTA - iConst176) & 16383)] - (fTemp233 + (0.600000024f * fRec177[1])));
			fVec27[(IOTA & 2047)] = fTemp234;
			fRec177[0] = fVec27[((IOTA - iConst177) & 2047)];
			float fRec178 = (0.600000024f * fTemp234);
			fRec199[0] = (0.0f - (fConst14 * ((fConst15 * fRec199[1]) - (fRec3[1] + fRec3[2]))));
			fRec198[0] = ((fConst186 * fRec198[1]) + (fConst187 * (fRec3[1] + (fConst188 * fRec199[0]))));
			fVec28[(IOTA & 16383)] = ((0.353553385f * fRec198[0]) + 9.99999968e-21f);
			float fTemp235 = (fVec28[((IOTA - iConst190) & 16383)] - (fTemp233 + (0.600000024f * fRec196[1])));
			fVec29[(IOTA & 4095)] = fTemp235;
			fRec196[0] = fVec29[((IOTA - iConst191) & 4095)];
			float fRec197 = (0.600000024f * fTemp235);
			fRec203[0] = (0.0f - (fConst14 * ((fConst15 * fRec203[1]) - (fRec5[1] + fRec5[2]))));
			fRec202[0] = ((fConst200 * fRec202[1]) + (fConst201 * (fRec5[1] + (fConst202 * fRec203[0]))));
			fVec30[(IOTA & 16383)] = ((0.353553385f * fRec202[0]) + 9.99999968e-21f);
			float fTemp236 = ((fTemp233 + fVec30[((IOTA - iConst204) & 16383)]) - (0.600000024f * fRec200[1]));
			fVec31[(IOTA & 4095)] = fTemp236;
			fRec200[0] = fVec31[((IOTA - iConst205) & 4095)];
			float fRec201 = (0.600000024f * fTemp236);
			fRec207[0] = (0.0f - (fConst14 * ((fConst15 * fRec207[1]) - (fRec1[1] + fRec1[2]))));
			fRec206[0] = ((fConst214 * fRec206[1]) + (fConst215 * (fRec1[1] + (fConst216 * fRec207[0]))));
			fVec32[(IOTA & 16383)] = ((0.353553385f * fRec206[0]) + 9.99999968e-21f);
			float fTemp237 = ((fVec32[((IOTA - iConst218) & 16383)] + fTemp233) - (0.600000024f * fRec204[1]));
			fVec33[(IOTA & 2047)] = fTemp237;
			fRec204[0] = fVec33[((IOTA - iConst219) & 2047)];
			float fRec205 = (0.600000024f * fTemp237);
			float fTemp238 = (fRec205 + fRec201);
			float fTemp239 = (fRec178 + (fRec197 + fTemp238));
			fRec1[0] = (fRec9[1] + (fRec165[1] + (fRec169[1] + (fRec173[1] + (fRec177[1] + (fRec196[1] + (fRec200[1] + (fRec204[1] + (fRec10 + (fRec166 + (fRec170 + (fRec174 + fTemp239))))))))))));
			fRec2[0] = ((fRec177[1] + (fRec196[1] + (fRec200[1] + (fRec204[1] + fTemp239)))) - (fRec9[1] + (fRec165[1] + (fRec169[1] + (fRec173[1] + (fRec10 + (fRec166 + (fRec174 + fRec170))))))));
			float fTemp240 = (fRec197 + fRec178);
			fRec3[0] = ((fRec169[1] + (fRec173[1] + (fRec200[1] + (fRec204[1] + (fRec170 + (fRec174 + fTemp238)))))) - (fRec9[1] + (fRec165[1] + (fRec177[1] + (fRec196[1] + (fRec10 + (fRec166 + fTemp240)))))));
			fRec4[0] = ((fRec9[1] + (fRec165[1] + (fRec200[1] + (fRec204[1] + (fRec10 + (fRec166 + fTemp238)))))) - (fRec169[1] + (fRec173[1] + (fRec177[1] + (fRec196[1] + (fRec170 + (fRec174 + fTemp240)))))));
			float fTemp241 = (fRec205 + fRec197);
			float fTemp242 = (fRec201 + fRec178);
			fRec5[0] = ((fRec165[1] + (fRec173[1] + (fRec196[1] + (fRec204[1] + (fRec166 + (fRec174 + fTemp241)))))) - (fRec9[1] + (fRec169[1] + (fRec177[1] + (fRec200[1] + (fRec10 + (fRec170 + fTemp242)))))));
			fRec6[0] = ((fRec9[1] + (fRec169[1] + (fRec196[1] + (fRec204[1] + (fRec10 + (fRec170 + fTemp241)))))) - (fRec165[1] + (fRec173[1] + (fRec177[1] + (fRec200[1] + (fRec166 + (fRec174 + fTemp242)))))));
			float fTemp243 = (fRec205 + fRec178);
			float fTemp244 = (fRec201 + fRec197);
			fRec7[0] = ((fRec9[1] + (fRec173[1] + (fRec177[1] + (fRec204[1] + (fRec10 + (fRec174 + fTemp243)))))) - (fRec165[1] + (fRec169[1] + (fRec196[1] + (fRec200[1] + (fRec166 + (fRec170 + fTemp244)))))));
			fRec8[0] = ((fRec165[1] + (fRec169[1] + (fRec177[1] + (fRec204[1] + (fRec166 + (fRec170 + fTemp243)))))) - (fRec9[1] + (fRec173[1] + (fRec196[1] + (fRec200[1] + (fRec10 + (fRec174 + fTemp244)))))));
			fRec208[0] = (fConst223 + (fConst110 * fRec208[1]));
			float fTemp245 = std::tan((fConst18 * fRec208[0]));
			float fTemp246 = mydsp_faustpower2_f(std::sqrt((fConst222 * fTemp245)));
			float fTemp247 = ((fConst221 * fTemp246) + -8.0f);
			float fTemp248 = (fConst220 * fTemp246);
			float fTemp249 = ((fConst225 * fTemp245) - (fConst226 * (fTemp246 / fTemp245)));
			float fTemp250 = (fConst224 * fTemp249);
			float fTemp251 = (fTemp248 + (4.0f - fTemp250));
			float fTemp252 = ((fTemp248 + fTemp250) + 4.0f);
			fRec0[0] = ((0.370000005f * (fRec2[0] + fRec3[0])) - (((fRec0[1] * fTemp247) + (fRec0[2] * fTemp251)) / fTemp252));
			float fTemp253 = (0.0f - (fConst224 * (fTemp249 / fTemp252)));
			fRec212[0] = ((0.999000013f * fRec212[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp4, ((fConst227 * fTemp5) + 1.0f))))));
			fRec214[0] = (iTemp6 ? fSlow445 : fRec214[1]);
			float fTemp254 = (fRec218[1] + (fSlow446 * fRec214[0]));
			fRec218[0] = (fTemp254 - std::floor(fTemp254));
			float fTemp255 = (fRec217[1] + (fConst29 * ((315.0f * (fRec212[0] * ftbl0mydspSIG0[int((65536.0f * fRec218[0]))])) + (fSlow447 * fRec214[0]))));
			fRec217[0] = (fTemp255 - std::floor(fTemp255));
			float fTemp256 = (fRec216[1] + (fConst29 * ((315.0f * (fRec212[0] * ftbl0mydspSIG0[int((65536.0f * fRec217[0]))])) + (fSlow448 * fRec214[0]))));
			fRec216[0] = (fTemp256 - std::floor(fTemp256));
			float fTemp257 = (fRec215[1] + (fConst29 * ((315.0f * (fRec212[0] * ftbl0mydspSIG0[int((65536.0f * fRec216[0]))])) + (fSlow449 * fRec214[0]))));
			fRec215[0] = (fTemp257 - std::floor(fTemp257));
			float fTemp258 = (fRec213[1] + (fConst29 * ((fSlow18 * fRec214[0]) + (315.0f * (fRec212[0] * ftbl0mydspSIG0[int((65536.0f * fRec215[0]))])))));
			fRec213[0] = (fTemp258 - std::floor(fTemp258));
			float fTemp259 = ftbl0mydspSIG0[int((65536.0f * fRec213[0]))];
			float fTemp260 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp259)));
			float fTemp261 = (fRec212[0] * (fTemp259 + (0.0500000007f * (fTemp260 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp260)))))));
			float fTemp262 = std::fabs((0.25f * fTemp261));
			fRec221[0] = std::max<float>(fTemp262, ((fRec221[1] * fSlow456) + (fTemp262 * fSlow457)));
			fRec220[0] = ((fRec220[1] * fSlow453) + (fRec221[0] * fSlow458));
			fRec219[0] = ((fConst30 * fRec219[1]) + (fSlow450 * std::max<float>(((20.0f * std::log10(fRec220[0])) - fSlow459), 0.0f)));
			float fTemp263 = (0.25f * (fTemp261 * std::pow(10.0f, (0.0500000007f * fRec219[0]))));
			fRec222[0] = (fTemp263 - (fSlow463 * ((fSlow464 * fRec222[2]) + (fSlow465 * fRec222[1]))));
			float fTemp264 = (fSlow444 * fRec211[1]);
			fRec211[0] = (fTemp263 - ((fSlow463 * (fRec222[2] + (fRec222[0] + (2.0f * fRec222[1])))) + (((fRec211[2] * fSlow473) + fTemp264) / fSlow474)));
			float fTemp265 = (fSlow441 * fRec210[1]);
			fRec210[0] = ((((fTemp264 + (fRec211[0] * fSlow476)) + (fRec211[2] * fSlow477)) / fSlow474) - (((fRec210[2] * fSlow485) + fTemp265) / fSlow486));
			fRec209[0] = ((((fTemp265 + (fRec210[0] * fSlow488)) + (fRec210[2] * fSlow489)) / fSlow486) - (fSlow490 * ((fSlow491 * fRec209[2]) + (fSlow492 * fRec209[1]))));
			float fTemp266 = (fSlow438 * (fRec209[2] + (fRec209[0] + (2.0f * fRec209[1]))));
			fRec228[0] = (fTemp105 - (fConst235 * ((fConst238 * fRec228[2]) + (fConst239 * fRec228[1]))));
			fRec227[0] = ((fConst235 * (((fConst237 * fRec228[0]) + (fConst240 * fRec228[1])) + (fConst237 * fRec228[2]))) - (fConst232 * ((fConst241 * fRec227[2]) + (fConst242 * fRec227[1]))));
			fVec34[0] = fSlow504;
			float fTemp267 = (fSlow504 - fVec34[1]);
			fVec35[0] = fSlow506;
			float fTemp268 = (0.0f - (fSlow504 + fVec35[1]));
			float fTemp269 = std::min<float>(1.0f, ((fTemp267 * float((fTemp267 > 0.0f))) + (fSlow505 * (fTemp268 * float((fTemp268 > 0.0f))))));
			fVec36[0] = fTemp269;
			iRec229[0] = (((iRec229[1] + (iRec229[1] > 0)) * (fTemp269 <= fVec36[1])) + (fTemp269 > fVec36[1]));
			float fTemp270 = float(iRec229[0]);
			float fTemp271 = (fConst243 * fTemp270);
			float fTemp272 = (fConst232 * ((fRec227[2] + (fRec227[0] + (2.0f * fRec227[1]))) * std::max<float>(0.0f, std::min<float>(fTemp271, (2.0f - fTemp271)))));
			fRec226[0] = (fTemp272 - ((fConst245 * fRec226[1]) + (fConst246 * fRec226[2])));
			fRec230[0] = (fTemp272 - ((fConst249 * fRec230[1]) + (fConst250 * fRec230[2])));
			fRec231[0] = (fTemp272 - ((fConst253 * fRec231[1]) + (fConst254 * fRec231[2])));
			fRec232[0] = (fTemp272 - ((fConst257 * fRec232[1]) + (fConst258 * fRec232[2])));
			fRec233[0] = (fTemp272 - ((fConst261 * fRec233[1]) + (fConst262 * fRec233[2])));
			fRec234[0] = (fTemp272 - ((fConst265 * fRec234[1]) + (fConst266 * fRec234[2])));
			fRec235[0] = (fTemp272 - ((fConst269 * fRec235[1]) + (fConst270 * fRec235[2])));
			fRec236[0] = (fTemp272 - ((fConst273 * fRec236[1]) + (fConst274 * fRec236[2])));
			fRec237[0] = (fTemp272 - ((fConst277 * fRec237[1]) + (fConst278 * fRec237[2])));
			fRec238[0] = (fTemp272 - ((fConst281 * fRec238[1]) + (fConst282 * fRec238[2])));
			fRec239[0] = (fTemp272 - ((fConst285 * fRec239[1]) + (fConst286 * fRec239[2])));
			fRec240[0] = (fTemp272 - ((fConst289 * fRec240[1]) + (fConst290 * fRec240[2])));
			fRec241[0] = (fTemp272 - ((fConst293 * fRec241[1]) + (fConst294 * fRec241[2])));
			fRec242[0] = (fTemp272 - ((fConst297 * fRec242[1]) + (fConst298 * fRec242[2])));
			fRec243[0] = (fTemp272 - ((fConst301 * fRec243[1]) + (fConst302 * fRec243[2])));
			fRec244[0] = (fTemp272 - ((fConst305 * fRec244[1]) + (fConst306 * fRec244[2])));
			fRec245[0] = (fTemp272 - ((fConst309 * fRec245[1]) + (fConst310 * fRec245[2])));
			fRec246[0] = (fTemp272 - ((fConst313 * fRec246[1]) + (fConst314 * fRec246[2])));
			fRec247[0] = (fTemp272 - ((fConst317 * fRec247[1]) + (fConst318 * fRec247[2])));
			fRec248[0] = (fTemp272 - ((fConst321 * fRec248[1]) + (fConst322 * fRec248[2])));
			fRec249[0] = (fTemp272 - ((fConst325 * fRec249[1]) + (fConst326 * fRec249[2])));
			fRec250[0] = (fTemp272 - ((fConst329 * fRec250[1]) + (fConst330 * fRec250[2])));
			fRec251[0] = (fTemp272 - ((fConst333 * fRec251[1]) + (fConst334 * fRec251[2])));
			fRec252[0] = (fTemp272 - ((fConst337 * fRec252[1]) + (fConst338 * fRec252[2])));
			fRec253[0] = (fTemp272 - ((fConst341 * fRec253[1]) + (fConst342 * fRec253[2])));
			fRec254[0] = (fTemp272 - ((fConst345 * fRec254[1]) + (fConst346 * fRec254[2])));
			fRec255[0] = (fTemp272 - ((fConst349 * fRec255[1]) + (fConst350 * fRec255[2])));
			fRec256[0] = (fTemp272 - ((fConst353 * fRec256[1]) + (fConst354 * fRec256[2])));
			fRec257[0] = (fTemp272 - ((fConst357 * fRec257[1]) + (fConst358 * fRec257[2])));
			fRec258[0] = (fTemp272 - ((fConst361 * fRec258[1]) + (fConst362 * fRec258[2])));
			fRec259[0] = (fTemp272 - ((fConst365 * fRec259[1]) + (fConst366 * fRec259[2])));
			fRec260[0] = (fTemp272 - ((fConst369 * fRec260[1]) + (fConst370 * fRec260[2])));
			fRec261[0] = (fTemp272 - ((fConst373 * fRec261[1]) + (fConst374 * fRec261[2])));
			fRec262[0] = (fTemp272 - ((fConst377 * fRec262[1]) + (fConst378 * fRec262[2])));
			fRec263[0] = (fTemp272 - ((fConst381 * fRec263[1]) + (fConst382 * fRec263[2])));
			fRec264[0] = (fTemp272 - ((fConst385 * fRec264[1]) + (fConst386 * fRec264[2])));
			fRec265[0] = (fTemp272 - ((fConst389 * fRec265[1]) + (fConst390 * fRec265[2])));
			fRec266[0] = (fTemp272 - ((fConst393 * fRec266[1]) + (fConst394 * fRec266[2])));
			fRec267[0] = (fTemp272 - ((fConst397 * fRec267[1]) + (fConst398 * fRec267[2])));
			fRec268[0] = (fTemp272 - ((fConst401 * fRec268[1]) + (fConst402 * fRec268[2])));
			fRec269[0] = (fTemp272 - ((fConst405 * fRec269[1]) + (fConst406 * fRec269[2])));
			fRec270[0] = (fTemp272 - ((fConst409 * fRec270[1]) + (fConst410 * fRec270[2])));
			fRec271[0] = (fTemp272 - ((fConst413 * fRec271[1]) + (fConst414 * fRec271[2])));
			fRec272[0] = (fTemp272 - ((fConst417 * fRec272[1]) + (fConst418 * fRec272[2])));
			fRec273[0] = (fTemp272 - ((fConst421 * fRec273[1]) + (fConst422 * fRec273[2])));
			fRec274[0] = (fTemp272 - ((fConst425 * fRec274[1]) + (fConst426 * fRec274[2])));
			fRec275[0] = (fTemp272 - ((fConst429 * fRec275[1]) + (fConst430 * fRec275[2])));
			fRec276[0] = (fTemp272 - ((fConst433 * fRec276[1]) + (fConst434 * fRec276[2])));
			fRec277[0] = (fTemp272 - ((fConst437 * fRec277[1]) + (fConst438 * fRec277[2])));
			fRec278[0] = (fTemp272 - ((fConst441 * fRec278[1]) + (fConst442 * fRec278[2])));
			fRec279[0] = (fSlow508 + (0.999000013f * fRec279[1]));
			fRec280[0] = (fSlow509 + (fRec280[1] - std::floor((fSlow509 + fRec280[1]))));
			fRec281[0] = (fSlow511 + (0.999000013f * fRec281[1]));
			fRec282[0] = (fSlow512 + (fRec282[1] - std::floor((fSlow512 + fRec282[1]))));
			fRec283[0] = (fSlow514 + (0.999000013f * fRec283[1]));
			fRec284[0] = (fSlow515 + (fRec284[1] - std::floor((fSlow515 + fRec284[1]))));
			float fTemp273 = ((0.0199999996f * (((((((((((((((((((((((((((((((((((((((((((((((((((fConst229 * (fRec226[0] - fRec226[2])) + (fConst247 * (fRec230[0] - fRec230[2]))) + (fConst251 * (fRec231[0] - fRec231[2]))) + (fConst255 * (fRec232[0] - fRec232[2]))) + (fConst259 * (fRec233[0] - fRec233[2]))) + (fConst263 * (fRec234[0] - fRec234[2]))) + (fConst267 * (fRec235[0] - fRec235[2]))) + (fConst271 * (fRec236[0] - fRec236[2]))) + (fConst275 * (fRec237[0] - fRec237[2]))) + (fConst279 * (fRec238[0] - fRec238[2]))) + (fConst283 * (fRec239[0] - fRec239[2]))) + (fConst287 * (fRec240[0] - fRec240[2]))) + (fConst291 * (fRec241[0] - fRec241[2]))) + (fConst295 * (fRec242[0] - fRec242[2]))) + (fConst299 * (fRec243[0] - fRec243[2]))) + (fConst303 * (fRec244[0] - fRec244[2]))) + (fConst307 * (fRec245[0] - fRec245[2]))) + (fConst311 * (fRec246[0] - fRec246[2]))) + (fConst315 * (fRec247[0] - fRec247[2]))) + (fConst319 * (fRec248[0] - fRec248[2]))) + (fConst323 * (fRec249[0] - fRec249[2]))) + (fConst327 * (fRec250[0] - fRec250[2]))) + (fConst331 * (fRec251[0] - fRec251[2]))) + (fConst335 * (fRec252[0] - fRec252[2]))) + (fConst339 * (fRec253[0] - fRec253[2]))) + (fConst343 * (fRec254[0] - fRec254[2]))) + (fConst347 * (fRec255[0] - fRec255[2]))) + (fConst351 * (fRec256[0] - fRec256[2]))) + (fConst355 * (fRec257[0] - fRec257[2]))) + (fConst359 * (fRec258[0] - fRec258[2]))) + (fConst363 * (fRec259[0] - fRec259[2]))) + (fConst367 * (fRec260[0] - fRec260[2]))) + (fConst371 * (fRec261[0] - fRec261[2]))) + (fConst375 * (fRec262[0] - fRec262[2]))) + (fConst379 * (fRec263[0] - fRec263[2]))) + (fConst383 * (fRec264[0] - fRec264[2]))) + (fConst387 * (fRec265[0] - fRec265[2]))) + (fConst391 * (fRec266[0] - fRec266[2]))) + (fConst395 * (fRec267[0] - fRec267[2]))) + (fConst399 * (fRec268[0] - fRec268[2]))) + (fConst403 * (fRec269[0] - fRec269[2]))) + (fConst407 * (fRec270[0] - fRec270[2]))) + (fConst411 * (fRec271[0] - fRec271[2]))) + (fConst415 * (fRec272[0] - fRec272[2]))) + (fConst419 * (fRec273[0] - fRec273[2]))) + (fConst423 * (fRec274[0] - fRec274[2]))) + (fConst427 * (fRec275[0] - fRec275[2]))) + (fConst431 * (fRec276[0] - fRec276[2]))) + (fConst435 * (fRec277[0] - fRec277[2]))) + (fConst439 * (fRec278[0] - fRec278[2]))) * std::max<float>(0.0f, std::min<float>((fConst22 * fTemp270), ((fConst443 * (fConst21 - fTemp270)) + 1.0f))))) + (2.0f * (((fRec279[0] * ftbl0mydspSIG0[int((65536.0f * fRec280[0]))]) + (fRec281[0] * ftbl0mydspSIG0[int((65536.0f * fRec282[0]))])) + (fRec283[0] * ftbl0mydspSIG0[int((65536.0f * fRec284[0]))]))));
			float fTemp274 = std::fabs((0.25f * fTemp273));
			fRec287[0] = std::max<float>(fTemp274, ((fRec287[1] * fSlow522) + (fTemp274 * fSlow523)));
			fRec286[0] = ((fRec286[1] * fSlow519) + (fRec287[0] * fSlow524));
			fRec285[0] = ((fConst30 * fRec285[1]) + (fSlow516 * std::max<float>(((20.0f * std::log10(fRec286[0])) - fSlow525), 0.0f)));
			float fTemp275 = (0.25f * (fTemp273 * std::pow(10.0f, (0.0500000007f * fRec285[0]))));
			fRec288[0] = (fTemp275 - (fSlow529 * ((fSlow530 * fRec288[2]) + (fSlow531 * fRec288[1]))));
			float fTemp276 = (fSlow503 * fRec225[1]);
			fRec225[0] = (fTemp275 - ((fSlow529 * (fRec288[2] + (fRec288[0] + (2.0f * fRec288[1])))) + (((fRec225[2] * fSlow539) + fTemp276) / fSlow540)));
			float fTemp277 = (fSlow500 * fRec224[1]);
			fRec224[0] = ((((fTemp276 + (fRec225[0] * fSlow542)) + (fRec225[2] * fSlow543)) / fSlow540) - (((fRec224[2] * fSlow551) + fTemp277) / fSlow552));
			fRec223[0] = ((((fTemp277 + (fRec224[0] * fSlow554)) + (fRec224[2] * fSlow555)) / fSlow552) - (fSlow556 * ((fSlow557 * fRec223[2]) + (fSlow558 * fRec223[1]))));
			float fTemp278 = (fSlow497 * (fRec223[2] + (fRec223[0] + (2.0f * fRec223[1]))));
			float fTemp279 = ((fRec0[2] * fTemp253) + ((((fTemp232 + fTemp266) + fTemp104) + fTemp278) + (fConst224 * ((fRec0[0] * fTemp249) / fTemp252))));
			float fTemp280 = std::fabs(fTemp279);
			fRec291[0] = std::max<float>(fTemp280, ((fConst446 * fRec291[1]) + (fConst447 * fTemp280)));
			fRec290[0] = ((fConst444 * fRec290[1]) + (fConst445 * fRec291[0]));
			fRec289[0] = ((fConst30 * fRec289[1]) + (fConst31 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec290[0]) + 1.0f)), 0.0f)))));
			float fTemp281 = (fTemp279 * std::pow(10.0f, (0.0500000007f * fRec289[0])));
			float fTemp282 = std::fabs(fTemp281);
			fRec294[0] = std::max<float>(fTemp282, ((fConst450 * fRec294[1]) + (fConst451 * fTemp282)));
			fRec293[0] = ((fConst448 * fRec293[1]) + (fConst449 * fRec294[0]));
			fRec292[0] = ((fConst30 * fRec292[1]) + (fConst31 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec293[0])) + 5.0f), 0.0f)))));
			float fTemp283 = (fTemp281 * std::pow(10.0f, (0.0500000007f * fRec292[0])));
			float fTemp284 = std::fabs((fSlow0 * fTemp283));
			fRec297[0] = std::max<float>(fTemp284, ((fConst30 * fRec297[1]) + (fConst31 * fTemp284)));
			fRec296[0] = ((fConst110 * fRec296[1]) + (fConst111 * fRec297[0]));
			fRec295[0] = ((fConst30 * fRec295[1]) + (fConst31 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec296[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp283 * std::pow(10.0f, (0.0500000007f * fRec295[0])))))));
			fRec298[0] = ((0.370000005f * (fRec2[0] - fRec3[0])) - (((fTemp247 * fRec298[1]) + (fTemp251 * fRec298[2])) / fTemp252));
			float fTemp285 = ((fTemp253 * fRec298[2]) + ((fTemp37 + (fTemp278 + (fTemp104 + (fTemp266 + fTemp205)))) + (fConst224 * ((fRec298[0] * fTemp249) / fTemp252))));
			float fTemp286 = std::fabs(fTemp285);
			fRec301[0] = std::max<float>(fTemp286, ((fConst446 * fRec301[1]) + (fConst447 * fTemp286)));
			fRec300[0] = ((fConst444 * fRec300[1]) + (fConst445 * fRec301[0]));
			fRec299[0] = ((fConst30 * fRec299[1]) + (fConst31 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec300[0]) + 1.0f)), 0.0f)))));
			float fTemp287 = (fTemp285 * std::pow(10.0f, (0.0500000007f * fRec299[0])));
			float fTemp288 = std::fabs(fTemp287);
			fRec304[0] = std::max<float>(fTemp288, ((fConst450 * fRec304[1]) + (fConst451 * fTemp288)));
			fRec303[0] = ((fConst448 * fRec303[1]) + (fConst449 * fRec304[0]));
			fRec302[0] = ((fConst30 * fRec302[1]) + (fConst31 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec303[0])) + 5.0f), 0.0f)))));
			float fTemp289 = (fTemp287 * std::pow(10.0f, (0.0500000007f * fRec302[0])));
			float fTemp290 = std::fabs((fSlow0 * fTemp289));
			fRec307[0] = std::max<float>(fTemp290, ((fConst30 * fRec307[1]) + (fConst31 * fTemp290)));
			fRec306[0] = ((fConst110 * fRec306[1]) + (fConst111 * fRec307[0]));
			fRec305[0] = ((fConst30 * fRec305[1]) + (fConst31 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec306[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp289 * std::pow(10.0f, (0.0500000007f * fRec305[0])))))));
			iVec0[1] = iVec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			iRec17[1] = iRec17[0];
			fRec16[1] = fRec16[0];
			fRec20[1] = fRec20[0];
			iRec19[1] = iRec19[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec25[1] = fRec25[0];
			iRec24[1] = iRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec28[1] = fRec28[0];
			iRec27[1] = iRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec31[1] = fRec31[0];
			iRec30[1] = iRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec67[1] = fRec67[0];
			fRec65[1] = fRec65[0];
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			fRec68[1] = fRec68[0];
			fRec61[1] = fRec61[0];
			fRec69[1] = fRec69[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec71[j0] = fRec71[(j0 - 1)];
			}
			fVec7[1] = fVec7[0];
			fVec8[2] = fVec8[1];
			fVec8[1] = fVec8[0];
			fRec57[1] = fRec57[0];
			fRec53[1] = fRec53[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec47[j1] = fRec47[(j1 - 1)];
			}
			fRec40[1] = fRec40[0];
			fRec73[1] = fRec73[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			iRec84[1] = iRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fVec9[1] = fVec9[0];
			fVec10[1] = fVec10[0];
			fVec11[1] = fVec11[0];
			iRec85[1] = iRec85[0];
			fRec86[1] = fRec86[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec99[2] = fRec99[1];
			fRec99[1] = fRec99[0];
			fRec100[2] = fRec100[1];
			fRec100[1] = fRec100[0];
			fRec101[2] = fRec101[1];
			fRec101[1] = fRec101[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fRec103[2] = fRec103[1];
			fRec103[1] = fRec103[0];
			fRec104[2] = fRec104[1];
			fRec104[1] = fRec104[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fRec108[1] = fRec108[0];
			fRec107[1] = fRec107[0];
			fRec106[1] = fRec106[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec115[1] = fRec115[0];
			fRec114[1] = fRec114[0];
			fRec116[1] = fRec116[0];
			fRec118[1] = fRec118[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec126[1] = fRec126[0];
			fRec125[1] = fRec125[0];
			fRec124[1] = fRec124[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fRec112[2] = fRec112[1];
			fRec112[1] = fRec112[0];
			fRec111[2] = fRec111[1];
			fRec111[1] = fRec111[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			iRec146[1] = iRec146[0];
			fRec152[1] = fRec152[0];
			fRec153[1] = fRec153[0];
			fVec12[1] = fVec12[0];
			fVec13[1] = fVec13[0];
			fRec157[1] = fRec157[0];
			fRec156[1] = fRec156[0];
			fRec155[1] = fRec155[0];
			fRec154[1] = fRec154[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec159[1] = fRec159[0];
			fVec14[1] = fVec14[0];
			fVec16[1] = fVec16[0];
			fRec151[1] = fRec151[0];
			fRec138[1] = fRec138[0];
			fRec132[1] = fRec132[0];
			fRec133[1] = fRec133[0];
			fRec131[1] = fRec131[0];
			fRec163[1] = fRec163[0];
			fRec162[1] = fRec162[0];
			fRec161[1] = fRec161[0];
			fRec164[2] = fRec164[1];
			fRec164[1] = fRec164[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec128[2] = fRec128[1];
			fRec128[1] = fRec128[0];
			fRec9[1] = fRec9[0];
			fRec168[1] = fRec168[0];
			fRec167[1] = fRec167[0];
			fRec165[1] = fRec165[0];
			fRec172[1] = fRec172[0];
			fRec171[1] = fRec171[0];
			fRec169[1] = fRec169[0];
			fRec176[1] = fRec176[0];
			fRec175[1] = fRec175[0];
			fRec173[1] = fRec173[0];
			fRec180[1] = fRec180[0];
			fRec179[1] = fRec179[0];
			iRec185[1] = iRec185[0];
			fRec184[2] = fRec184[1];
			fRec184[1] = fRec184[0];
			iRec187[1] = iRec187[0];
			fRec186[2] = fRec186[1];
			fRec186[1] = fRec186[0];
			iRec189[1] = iRec189[0];
			fRec188[2] = fRec188[1];
			fRec188[1] = fRec188[0];
			iRec191[1] = iRec191[0];
			fRec190[2] = fRec190[1];
			fRec190[1] = fRec190[0];
			fRec194[1] = fRec194[0];
			fRec193[1] = fRec193[0];
			fRec192[1] = fRec192[0];
			fRec195[2] = fRec195[1];
			fRec195[1] = fRec195[0];
			fRec183[2] = fRec183[1];
			fRec183[1] = fRec183[0];
			fRec182[2] = fRec182[1];
			fRec182[1] = fRec182[0];
			fRec181[2] = fRec181[1];
			fRec181[1] = fRec181[0];
			fRec177[1] = fRec177[0];
			fRec199[1] = fRec199[0];
			fRec198[1] = fRec198[0];
			fRec196[1] = fRec196[0];
			fRec203[1] = fRec203[0];
			fRec202[1] = fRec202[0];
			fRec200[1] = fRec200[0];
			fRec207[1] = fRec207[0];
			fRec206[1] = fRec206[0];
			fRec204[1] = fRec204[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec208[1] = fRec208[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec212[1] = fRec212[0];
			fRec214[1] = fRec214[0];
			fRec218[1] = fRec218[0];
			fRec217[1] = fRec217[0];
			fRec216[1] = fRec216[0];
			fRec215[1] = fRec215[0];
			fRec213[1] = fRec213[0];
			fRec221[1] = fRec221[0];
			fRec220[1] = fRec220[0];
			fRec219[1] = fRec219[0];
			fRec222[2] = fRec222[1];
			fRec222[1] = fRec222[0];
			fRec211[2] = fRec211[1];
			fRec211[1] = fRec211[0];
			fRec210[2] = fRec210[1];
			fRec210[1] = fRec210[0];
			fRec209[2] = fRec209[1];
			fRec209[1] = fRec209[0];
			fRec228[2] = fRec228[1];
			fRec228[1] = fRec228[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
			fVec34[1] = fVec34[0];
			fVec35[1] = fVec35[0];
			fVec36[1] = fVec36[0];
			iRec229[1] = iRec229[0];
			fRec226[2] = fRec226[1];
			fRec226[1] = fRec226[0];
			fRec230[2] = fRec230[1];
			fRec230[1] = fRec230[0];
			fRec231[2] = fRec231[1];
			fRec231[1] = fRec231[0];
			fRec232[2] = fRec232[1];
			fRec232[1] = fRec232[0];
			fRec233[2] = fRec233[1];
			fRec233[1] = fRec233[0];
			fRec234[2] = fRec234[1];
			fRec234[1] = fRec234[0];
			fRec235[2] = fRec235[1];
			fRec235[1] = fRec235[0];
			fRec236[2] = fRec236[1];
			fRec236[1] = fRec236[0];
			fRec237[2] = fRec237[1];
			fRec237[1] = fRec237[0];
			fRec238[2] = fRec238[1];
			fRec238[1] = fRec238[0];
			fRec239[2] = fRec239[1];
			fRec239[1] = fRec239[0];
			fRec240[2] = fRec240[1];
			fRec240[1] = fRec240[0];
			fRec241[2] = fRec241[1];
			fRec241[1] = fRec241[0];
			fRec242[2] = fRec242[1];
			fRec242[1] = fRec242[0];
			fRec243[2] = fRec243[1];
			fRec243[1] = fRec243[0];
			fRec244[2] = fRec244[1];
			fRec244[1] = fRec244[0];
			fRec245[2] = fRec245[1];
			fRec245[1] = fRec245[0];
			fRec246[2] = fRec246[1];
			fRec246[1] = fRec246[0];
			fRec247[2] = fRec247[1];
			fRec247[1] = fRec247[0];
			fRec248[2] = fRec248[1];
			fRec248[1] = fRec248[0];
			fRec249[2] = fRec249[1];
			fRec249[1] = fRec249[0];
			fRec250[2] = fRec250[1];
			fRec250[1] = fRec250[0];
			fRec251[2] = fRec251[1];
			fRec251[1] = fRec251[0];
			fRec252[2] = fRec252[1];
			fRec252[1] = fRec252[0];
			fRec253[2] = fRec253[1];
			fRec253[1] = fRec253[0];
			fRec254[2] = fRec254[1];
			fRec254[1] = fRec254[0];
			fRec255[2] = fRec255[1];
			fRec255[1] = fRec255[0];
			fRec256[2] = fRec256[1];
			fRec256[1] = fRec256[0];
			fRec257[2] = fRec257[1];
			fRec257[1] = fRec257[0];
			fRec258[2] = fRec258[1];
			fRec258[1] = fRec258[0];
			fRec259[2] = fRec259[1];
			fRec259[1] = fRec259[0];
			fRec260[2] = fRec260[1];
			fRec260[1] = fRec260[0];
			fRec261[2] = fRec261[1];
			fRec261[1] = fRec261[0];
			fRec262[2] = fRec262[1];
			fRec262[1] = fRec262[0];
			fRec263[2] = fRec263[1];
			fRec263[1] = fRec263[0];
			fRec264[2] = fRec264[1];
			fRec264[1] = fRec264[0];
			fRec265[2] = fRec265[1];
			fRec265[1] = fRec265[0];
			fRec266[2] = fRec266[1];
			fRec266[1] = fRec266[0];
			fRec267[2] = fRec267[1];
			fRec267[1] = fRec267[0];
			fRec268[2] = fRec268[1];
			fRec268[1] = fRec268[0];
			fRec269[2] = fRec269[1];
			fRec269[1] = fRec269[0];
			fRec270[2] = fRec270[1];
			fRec270[1] = fRec270[0];
			fRec271[2] = fRec271[1];
			fRec271[1] = fRec271[0];
			fRec272[2] = fRec272[1];
			fRec272[1] = fRec272[0];
			fRec273[2] = fRec273[1];
			fRec273[1] = fRec273[0];
			fRec274[2] = fRec274[1];
			fRec274[1] = fRec274[0];
			fRec275[2] = fRec275[1];
			fRec275[1] = fRec275[0];
			fRec276[2] = fRec276[1];
			fRec276[1] = fRec276[0];
			fRec277[2] = fRec277[1];
			fRec277[1] = fRec277[0];
			fRec278[2] = fRec278[1];
			fRec278[1] = fRec278[0];
			fRec279[1] = fRec279[0];
			fRec280[1] = fRec280[0];
			fRec281[1] = fRec281[0];
			fRec282[1] = fRec282[0];
			fRec283[1] = fRec283[0];
			fRec284[1] = fRec284[0];
			fRec287[1] = fRec287[0];
			fRec286[1] = fRec286[0];
			fRec285[1] = fRec285[0];
			fRec288[2] = fRec288[1];
			fRec288[1] = fRec288[0];
			fRec225[2] = fRec225[1];
			fRec225[1] = fRec225[0];
			fRec224[2] = fRec224[1];
			fRec224[1] = fRec224[0];
			fRec223[2] = fRec223[1];
			fRec223[1] = fRec223[0];
			fRec291[1] = fRec291[0];
			fRec290[1] = fRec290[0];
			fRec289[1] = fRec289[0];
			fRec294[1] = fRec294[0];
			fRec293[1] = fRec293[0];
			fRec292[1] = fRec292[0];
			fRec297[1] = fRec297[0];
			fRec296[1] = fRec296[0];
			fRec295[1] = fRec295[0];
			fRec298[2] = fRec298[1];
			fRec298[1] = fRec298[0];
			fRec301[1] = fRec301[0];
			fRec300[1] = fRec300[0];
			fRec299[1] = fRec299[0];
			fRec304[1] = fRec304[0];
			fRec303[1] = fRec303[0];
			fRec302[1] = fRec302[0];
			fRec307[1] = fRec307[0];
			fRec306[1] = fRec306[0];
			fRec305[1] = fRec305[0];
		}
	}

};

#endif
