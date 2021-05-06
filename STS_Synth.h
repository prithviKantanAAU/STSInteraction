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
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider1;
	float fConst18;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	FAUSTFLOAT fHslider5;
	float fVec2[2];
	float fVec3[2];
	float fVec4[2];
	int iRec17[2];
	float fConst23;
	float fRec16[2];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
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
	FAUSTFLOAT fHslider9;
	float fRec25[2];
	int iRec24[2];
	float fRec23[3];
	FAUSTFLOAT fHslider10;
	float fRec28[2];
	int iRec27[2];
	float fRec26[3];
	FAUSTFLOAT fHslider11;
	float fRec31[2];
	int iRec30[2];
	float fRec29[3];
	float fConst30;
	float fConst31;
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	FAUSTFLOAT fHslider14;
	float fRec34[2];
	float fRec33[2];
	FAUSTFLOAT fHslider15;
	float fRec32[2];
	FAUSTFLOAT fHslider16;
	FAUSTFLOAT fHslider17;
	float fRec35[3];
	FAUSTFLOAT fHslider18;
	FAUSTFLOAT fHslider19;
	float fConst32;
	float fRec15[3];
	FAUSTFLOAT fHslider20;
	FAUSTFLOAT fHslider21;
	float fRec14[3];
	float fRec13[3];
	FAUSTFLOAT fHslider22;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	FAUSTFLOAT fHslider26;
	float fConst33;
	float fConst34;
	float fConst35;
	float fRec67[2];
	float fRec65[2];
	FAUSTFLOAT fHslider27;
	float fVec5[2];
	float fVec6[2];
	float fVec7[2];
	FAUSTFLOAT fHslider28;
	float fRec68[2];
	float fRec61[2];
	FAUSTFLOAT fHslider29;
	float fRec69[2];
	float fRec71[4];
	float fRec72[1024];
	float fConst36;
	float fVec8[2];
	float fVec9[3];
	float fRec70[2048];
	float fRec57[2];
	float fRec53[2];
	float fRec49[1024];
	float fRec47[4];
	float fRec42[2048];
	float fRec40[2];
	FAUSTFLOAT fHslider30;
	float fRec73[2];
	float fRec39[3];
	int iRec74[2];
	int iRec75[2];
	FAUSTFLOAT fHslider31;
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec78[2];
	float fRec77[2];
	FAUSTFLOAT fHslider34;
	float fRec76[2];
	FAUSTFLOAT fHslider35;
	FAUSTFLOAT fHslider36;
	float fRec79[3];
	FAUSTFLOAT fHslider37;
	FAUSTFLOAT fHslider38;
	float fRec38[3];
	FAUSTFLOAT fHslider39;
	FAUSTFLOAT fHslider40;
	float fRec37[3];
	float fRec36[3];
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider42;
	FAUSTFLOAT fHslider43;
	FAUSTFLOAT fHslider44;
	FAUSTFLOAT fHslider45;
	float fConst40;
	float fConst41;
	float fConst42;
	FAUSTFLOAT fHslider46;
	float fConst43;
	float fRec85[2];
	float fRec84[2];
	float fConst44;
	FAUSTFLOAT fHslider47;
	float fRec86[2];
	float fRec88[2];
	FAUSTFLOAT fHslider48;
	float fRec87[3];
	float fRec90[3];
	float fRec91[3];
	float fRec92[3];
	float fRec93[3];
	float fConst45;
	float fConst46;
	float fConst47;
	float fRec83[3];
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	FAUSTFLOAT fHslider51;
	float fRec96[2];
	float fRec95[2];
	FAUSTFLOAT fHslider52;
	float fRec94[2];
	FAUSTFLOAT fHslider53;
	FAUSTFLOAT fHslider54;
	float fRec97[3];
	FAUSTFLOAT fHslider55;
	FAUSTFLOAT fHslider56;
	float fRec82[3];
	FAUSTFLOAT fHslider57;
	FAUSTFLOAT fHslider58;
	float fRec81[3];
	float fRec80[3];
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	FAUSTFLOAT fHslider63;
	int iRec116[2];
	float fRec122[2];
	float fConst48;
	float fRec123[2];
	float fConst49;
	float fConst50;
	FAUSTFLOAT fHslider64;
	FAUSTFLOAT fHslider65;
	float fVec10[2];
	float fVec11[2];
	FAUSTFLOAT fHslider66;
	float fRec126[2];
	float fRec125[2];
	float fRec124[2];
	float fConst54;
	int iRec128[2];
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec127[3];
	float fRec129[2];
	float fRec130[2048];
	float fConst58;
	FAUSTFLOAT fHslider67;
	float fVec12[2];
	float fVec13[2048];
	float fVec14[2];
	float fRec121[2];
	float fRec112[2048];
	float fRec108[2];
	float fRec104[2048];
	float fRec102[2];
	float fRec103[2];
	float fRec101[2];
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	FAUSTFLOAT fHslider70;
	float fRec133[2];
	float fRec132[2];
	FAUSTFLOAT fHslider71;
	float fRec131[2];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec134[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fHslider75;
	float fRec100[3];
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fHslider77;
	float fRec99[3];
	float fRec98[3];
	float fVec15[2048];
	int iConst59;
	float fVec16[2048];
	int iConst60;
	float fRec9[2];
	float fConst69;
	float fConst70;
	float fConst71;
	float fRec138[2];
	float fRec137[2];
	float fVec17[32768];
	int iConst73;
	float fVec18[4096];
	int iConst74;
	float fRec135[2];
	float fConst83;
	float fConst84;
	float fConst85;
	float fRec142[2];
	float fRec141[2];
	float fVec19[16384];
	int iConst87;
	float fVec20[4096];
	int iConst88;
	float fRec139[2];
	float fConst97;
	float fConst98;
	float fConst99;
	float fRec146[2];
	float fRec145[2];
	float fVec21[32768];
	int iConst101;
	float fVec22[4096];
	int iConst102;
	float fRec143[2];
	float fConst111;
	float fConst112;
	float fConst113;
	float fRec150[2];
	float fRec149[2];
	float fVec23[16384];
	int iConst115;
	int iRec155[2];
	float fRec154[3];
	int iRec157[2];
	float fRec156[3];
	int iRec159[2];
	float fRec158[3];
	int iRec161[2];
	float fRec160[3];
	float fRec164[2];
	float fRec163[2];
	float fRec162[2];
	float fRec165[3];
	float fRec153[3];
	float fRec152[3];
	float fRec151[3];
	float fVec24[2048];
	float fVec25[2048];
	int iConst116;
	float fRec147[2];
	float fConst125;
	float fConst126;
	float fConst127;
	float fRec169[2];
	float fRec168[2];
	float fVec26[16384];
	int iConst129;
	float fVec27[4096];
	int iConst130;
	float fRec166[2];
	float fConst139;
	float fConst140;
	float fConst141;
	float fRec173[2];
	float fRec172[2];
	float fVec28[16384];
	int iConst143;
	float fVec29[4096];
	int iConst144;
	float fRec170[2];
	float fConst153;
	float fConst154;
	float fConst155;
	float fRec177[2];
	float fRec176[2];
	float fVec30[16384];
	int iConst157;
	float fVec31[2048];
	int iConst158;
	float fRec174[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fConst159;
	float fConst160;
	float fConst161;
	float fConst162;
	float fRec178[2];
	float fConst163;
	float fConst164;
	float fConst165;
	float fRec0[3];
	FAUSTFLOAT fHslider78;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	FAUSTFLOAT fHslider82;
	FAUSTFLOAT fHslider83;
	float fRec184[3];
	float fRec183[3];
	float fConst166;
	FAUSTFLOAT fHslider84;
	float fVec32[2];
	float fVec33[2];
	float fVec34[2];
	int iRec185[2];
	float fConst168;
	float fRec186[2];
	float fConst169;
	float fRec182[3];
	float fConst171;
	float fConst172;
	float fRec187[3];
	float fConst174;
	float fConst175;
	float fRec188[3];
	float fConst177;
	float fConst178;
	float fRec189[3];
	float fConst180;
	float fConst181;
	float fRec190[3];
	float fConst183;
	float fConst184;
	float fRec191[3];
	float fConst186;
	float fConst187;
	float fRec192[3];
	float fConst189;
	float fConst190;
	float fRec193[3];
	float fConst192;
	float fConst193;
	float fRec194[3];
	float fConst195;
	float fConst196;
	float fRec195[3];
	float fConst198;
	float fConst199;
	float fRec196[3];
	float fConst201;
	float fConst202;
	float fRec197[3];
	float fConst204;
	float fConst205;
	float fRec198[3];
	float fConst207;
	float fConst208;
	float fRec199[3];
	float fConst210;
	float fConst211;
	float fRec200[3];
	float fConst213;
	float fConst214;
	float fRec201[3];
	float fConst216;
	float fConst217;
	float fRec202[3];
	float fConst219;
	float fConst220;
	float fRec203[3];
	float fConst222;
	float fConst223;
	float fRec204[3];
	float fConst225;
	float fConst226;
	float fRec205[3];
	FAUSTFLOAT fHslider85;
	FAUSTFLOAT fHslider86;
	FAUSTFLOAT fHslider87;
	float fRec208[2];
	float fRec207[2];
	FAUSTFLOAT fHslider88;
	float fRec206[2];
	FAUSTFLOAT fHslider89;
	FAUSTFLOAT fHslider90;
	float fRec209[3];
	FAUSTFLOAT fHslider91;
	FAUSTFLOAT fHslider92;
	float fRec181[3];
	FAUSTFLOAT fHslider93;
	FAUSTFLOAT fHslider94;
	float fRec180[3];
	float fRec179[3];
	FAUSTFLOAT fHslider95;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider96;
	FAUSTFLOAT fHslider97;
	FAUSTFLOAT fHslider98;
	FAUSTFLOAT fHslider99;
	float fConst227;
	float fRec213[2];
	float fRec215[2];
	float fConst228;
	float fRec219[2];
	float fRec218[2];
	float fRec217[2];
	float fRec216[2];
	float fRec214[2];
	FAUSTFLOAT fHslider100;
	FAUSTFLOAT fHslider101;
	FAUSTFLOAT fHslider102;
	float fRec222[2];
	float fRec221[2];
	FAUSTFLOAT fHslider103;
	float fRec220[2];
	FAUSTFLOAT fHslider104;
	FAUSTFLOAT fHslider105;
	float fRec223[3];
	FAUSTFLOAT fHslider106;
	FAUSTFLOAT fHslider107;
	float fRec212[3];
	FAUSTFLOAT fHslider108;
	FAUSTFLOAT fHslider109;
	float fRec211[3];
	float fRec210[3];
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider111;
	FAUSTFLOAT fHslider112;
	FAUSTFLOAT fHslider113;
	FAUSTFLOAT fHslider114;
	float fConst229;
	float fConst232;
	float fConst235;
	float fConst237;
	float fConst238;
	float fConst239;
	float fRec229[3];
	float fConst240;
	float fConst241;
	float fConst242;
	float fRec228[3];
	float fConst243;
	FAUSTFLOAT fHslider115;
	float fVec35[2];
	float fVec36[2];
	float fVec37[2];
	int iRec230[2];
	float fConst245;
	float fConst246;
	float fRec227[3];
	float fConst247;
	float fConst249;
	float fConst250;
	float fRec231[3];
	float fConst251;
	float fConst253;
	float fConst254;
	float fRec232[3];
	float fConst255;
	float fConst257;
	float fConst258;
	float fRec233[3];
	float fConst259;
	float fConst261;
	float fConst262;
	float fRec234[3];
	float fConst263;
	float fConst265;
	float fConst266;
	float fRec235[3];
	float fConst267;
	float fConst269;
	float fConst270;
	float fRec236[3];
	float fConst271;
	float fConst273;
	float fConst274;
	float fRec237[3];
	float fConst275;
	float fConst277;
	float fConst278;
	float fRec238[3];
	float fConst279;
	float fConst281;
	float fConst282;
	float fRec239[3];
	float fConst283;
	float fConst285;
	float fConst286;
	float fRec240[3];
	float fConst287;
	float fConst289;
	float fConst290;
	float fRec241[3];
	float fConst291;
	float fConst293;
	float fConst294;
	float fRec242[3];
	float fConst295;
	float fConst297;
	float fConst298;
	float fRec243[3];
	float fConst299;
	float fConst301;
	float fConst302;
	float fRec244[3];
	float fConst303;
	float fConst305;
	float fConst306;
	float fRec245[3];
	float fConst307;
	float fConst309;
	float fConst310;
	float fRec246[3];
	float fConst311;
	float fConst313;
	float fConst314;
	float fRec247[3];
	float fConst315;
	float fConst317;
	float fConst318;
	float fRec248[3];
	float fConst319;
	float fConst321;
	float fConst322;
	float fRec249[3];
	float fConst323;
	float fConst325;
	float fConst326;
	float fRec250[3];
	float fConst327;
	float fConst329;
	float fConst330;
	float fRec251[3];
	float fConst331;
	float fConst333;
	float fConst334;
	float fRec252[3];
	float fConst335;
	float fConst337;
	float fConst338;
	float fRec253[3];
	float fConst339;
	float fConst341;
	float fConst342;
	float fRec254[3];
	float fConst343;
	float fConst345;
	float fConst346;
	float fRec255[3];
	float fConst347;
	float fConst349;
	float fConst350;
	float fRec256[3];
	float fConst351;
	float fConst353;
	float fConst354;
	float fRec257[3];
	float fConst355;
	float fConst357;
	float fConst358;
	float fRec258[3];
	float fConst359;
	float fConst361;
	float fConst362;
	float fRec259[3];
	float fConst363;
	float fConst365;
	float fConst366;
	float fRec260[3];
	float fConst367;
	float fConst369;
	float fConst370;
	float fRec261[3];
	float fConst371;
	float fConst373;
	float fConst374;
	float fRec262[3];
	float fConst375;
	float fConst377;
	float fConst378;
	float fRec263[3];
	float fConst379;
	float fConst381;
	float fConst382;
	float fRec264[3];
	float fConst383;
	float fConst385;
	float fConst386;
	float fRec265[3];
	float fConst387;
	float fConst389;
	float fConst390;
	float fRec266[3];
	float fConst391;
	float fConst393;
	float fConst394;
	float fRec267[3];
	float fConst395;
	float fConst397;
	float fConst398;
	float fRec268[3];
	float fConst399;
	float fConst401;
	float fConst402;
	float fRec269[3];
	float fConst403;
	float fConst405;
	float fConst406;
	float fRec270[3];
	float fConst407;
	float fConst409;
	float fConst410;
	float fRec271[3];
	float fConst411;
	float fConst413;
	float fConst414;
	float fRec272[3];
	float fConst415;
	float fConst417;
	float fConst418;
	float fRec273[3];
	float fConst419;
	float fConst421;
	float fConst422;
	float fRec274[3];
	float fConst423;
	float fConst425;
	float fConst426;
	float fRec275[3];
	float fConst427;
	float fConst429;
	float fConst430;
	float fRec276[3];
	float fConst431;
	float fConst433;
	float fConst434;
	float fRec277[3];
	float fConst435;
	float fConst437;
	float fConst438;
	float fRec278[3];
	float fConst439;
	float fConst441;
	float fConst442;
	float fRec279[3];
	float fConst443;
	FAUSTFLOAT fHslider116;
	float fRec280[2];
	float fRec281[2];
	FAUSTFLOAT fHslider117;
	float fRec282[2];
	float fRec283[2];
	FAUSTFLOAT fHslider118;
	float fRec284[2];
	float fRec285[2];
	FAUSTFLOAT fHslider119;
	FAUSTFLOAT fHslider120;
	FAUSTFLOAT fHslider121;
	float fRec288[2];
	float fRec287[2];
	FAUSTFLOAT fHslider122;
	float fRec286[2];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	float fRec289[3];
	FAUSTFLOAT fHslider125;
	FAUSTFLOAT fHslider126;
	float fRec226[3];
	FAUSTFLOAT fHslider127;
	FAUSTFLOAT fHslider128;
	float fRec225[3];
	float fRec224[3];
	float fConst444;
	float fConst445;
	float fConst446;
	float fConst447;
	float fRec292[2];
	float fRec291[2];
	float fRec290[2];
	float fConst448;
	float fConst449;
	float fConst450;
	float fConst451;
	float fRec295[2];
	float fRec294[2];
	float fRec293[2];
	float fRec298[2];
	float fRec297[2];
	float fRec296[2];
	float fRec299[3];
	float fRec302[2];
	float fRec301[2];
	float fRec300[2];
	float fRec305[2];
	float fRec304[2];
	float fRec303[2];
	float fRec308[2];
	float fRec307[2];
	float fRec306[2];
	
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
		float fConst37 = std::tan((25132.7422f / fConst0));
		float fConst38 = (1.0f / fConst37);
		float fConst39 = (((fConst38 + 0.333333343f) / fConst37) + 1.0f);
		fConst40 = (0.25f / fConst39);
		fConst41 = std::exp((0.0f - (50.0f / fConst0)));
		fConst42 = (1.0f - fConst41);
		fConst43 = (6.0f / fConst0);
		fConst44 = (2.0f * fConst42);
		fConst45 = (1.0f / fConst39);
		fConst46 = (((fConst38 + -0.333333343f) / fConst37) + 1.0f);
		fConst47 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst37))));
		fConst48 = (5.0f / fConst0);
		fConst49 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst50 = (1.0f - fConst49);
		float fConst51 = std::tan((6283.18555f / fConst0));
		float fConst52 = (1.0f / fConst51);
		float fConst53 = (((fConst52 + 1.41421354f) / fConst51) + 1.0f);
		fConst54 = (0.0500000007f / fConst53);
		fConst55 = (1.0f / fConst53);
		fConst56 = (((fConst52 + -1.41421354f) / fConst51) + 1.0f);
		fConst57 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst51))));
		fConst58 = (0.00147058826f * fConst0);
		iConst59 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst60 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst16 + -1.0f))));
		float fConst61 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst62 = ((0.0f - (6.90775537f * fConst61)) / fConst0);
		float fConst63 = std::exp((0.25f * fConst62));
		float fConst64 = mydsp_faustpower2_f(fConst63);
		float fConst65 = (1.0f - (fConst64 * fConst5));
		float fConst66 = (1.0f - fConst64);
		float fConst67 = (fConst65 / fConst66);
		float fConst68 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst65) / mydsp_faustpower2_f(fConst66)) + -1.0f)));
		fConst69 = (fConst67 - fConst68);
		fConst70 = (fConst63 * (fConst68 + (1.0f - fConst67)));
		fConst71 = ((std::exp(fConst62) / fConst63) + -1.0f);
		float fConst72 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst73 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst61 - fConst72))));
		iConst74 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst72 + -1.0f))));
		float fConst75 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst76 = ((0.0f - (6.90775537f * fConst75)) / fConst0);
		float fConst77 = std::exp((0.25f * fConst76));
		float fConst78 = mydsp_faustpower2_f(fConst77);
		float fConst79 = (1.0f - (fConst78 * fConst5));
		float fConst80 = (1.0f - fConst78);
		float fConst81 = (fConst79 / fConst80);
		float fConst82 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst79) / mydsp_faustpower2_f(fConst80)) + -1.0f)));
		fConst83 = (fConst81 - fConst82);
		fConst84 = (fConst77 * (fConst82 + (1.0f - fConst81)));
		fConst85 = ((std::exp(fConst76) / fConst77) + -1.0f);
		float fConst86 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst87 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst75 - fConst86))));
		iConst88 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst86 + -1.0f))));
		float fConst89 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst90 = ((0.0f - (6.90775537f * fConst89)) / fConst0);
		float fConst91 = std::exp((0.25f * fConst90));
		float fConst92 = mydsp_faustpower2_f(fConst91);
		float fConst93 = (1.0f - (fConst92 * fConst5));
		float fConst94 = (1.0f - fConst92);
		float fConst95 = (fConst93 / fConst94);
		float fConst96 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst93) / mydsp_faustpower2_f(fConst94)) + -1.0f)));
		fConst97 = (fConst95 - fConst96);
		fConst98 = (fConst91 * (fConst96 + (1.0f - fConst95)));
		fConst99 = ((std::exp(fConst90) / fConst91) + -1.0f);
		float fConst100 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst101 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst89 - fConst100))));
		iConst102 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst100 + -1.0f))));
		float fConst103 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst104 = ((0.0f - (6.90775537f * fConst103)) / fConst0);
		float fConst105 = std::exp((0.25f * fConst104));
		float fConst106 = mydsp_faustpower2_f(fConst105);
		float fConst107 = (1.0f - (fConst106 * fConst5));
		float fConst108 = (1.0f - fConst106);
		float fConst109 = (fConst107 / fConst108);
		float fConst110 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst107) / mydsp_faustpower2_f(fConst108)) + -1.0f)));
		fConst111 = (fConst109 - fConst110);
		fConst112 = (fConst105 * (fConst110 + (1.0f - fConst109)));
		fConst113 = ((std::exp(fConst104) / fConst105) + -1.0f);
		float fConst114 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst115 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst103 - fConst114))));
		iConst116 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst114 + -1.0f))));
		float fConst117 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst118 = ((0.0f - (6.90775537f * fConst117)) / fConst0);
		float fConst119 = std::exp((0.25f * fConst118));
		float fConst120 = mydsp_faustpower2_f(fConst119);
		float fConst121 = (1.0f - (fConst120 * fConst5));
		float fConst122 = (1.0f - fConst120);
		float fConst123 = (fConst121 / fConst122);
		float fConst124 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst121) / mydsp_faustpower2_f(fConst122)) + -1.0f)));
		fConst125 = (fConst123 - fConst124);
		fConst126 = (fConst119 * (fConst124 + (1.0f - fConst123)));
		fConst127 = ((std::exp(fConst118) / fConst119) + -1.0f);
		float fConst128 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst129 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst117 - fConst128))));
		iConst130 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst128 + -1.0f))));
		float fConst131 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst132 = ((0.0f - (6.90775537f * fConst131)) / fConst0);
		float fConst133 = std::exp((0.25f * fConst132));
		float fConst134 = mydsp_faustpower2_f(fConst133);
		float fConst135 = (1.0f - (fConst134 * fConst5));
		float fConst136 = (1.0f - fConst134);
		float fConst137 = (fConst135 / fConst136);
		float fConst138 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst135) / mydsp_faustpower2_f(fConst136)) + -1.0f)));
		fConst139 = (fConst137 - fConst138);
		fConst140 = (fConst133 * (fConst138 + (1.0f - fConst137)));
		fConst141 = ((std::exp(fConst132) / fConst133) + -1.0f);
		float fConst142 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst143 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst131 - fConst142))));
		iConst144 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst142 + -1.0f))));
		float fConst145 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst146 = ((0.0f - (6.90775537f * fConst145)) / fConst0);
		float fConst147 = std::exp((0.25f * fConst146));
		float fConst148 = mydsp_faustpower2_f(fConst147);
		float fConst149 = (1.0f - (fConst148 * fConst5));
		float fConst150 = (1.0f - fConst148);
		float fConst151 = (fConst149 / fConst150);
		float fConst152 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst149) / mydsp_faustpower2_f(fConst150)) + -1.0f)));
		fConst153 = (fConst151 - fConst152);
		fConst154 = (fConst147 * (fConst152 + (1.0f - fConst151)));
		fConst155 = ((std::exp(fConst146) / fConst147) + -1.0f);
		float fConst156 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst157 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst145 - fConst156))));
		iConst158 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst156 + -1.0f))));
		fConst159 = mydsp_faustpower2_f(fConst29);
		fConst160 = (2.0f * fConst159);
		fConst161 = (4.0f * (mydsp_faustpower2_f(fConst0) * std::tan((628.318542f / fConst0))));
		fConst162 = (3500.0f * fConst50);
		fConst163 = (2.0f / fConst0);
		fConst164 = (2.0f * fConst0);
		fConst165 = (0.5f / fConst0);
		fConst166 = (0.00200000009f * fConst0);
		float fConst167 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst168 = (0.0f - (2.0f * fConst167));
		fConst169 = mydsp_faustpower2_f(fConst167);
		float fConst170 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst171 = (0.0f - (2.0f * fConst170));
		fConst172 = mydsp_faustpower2_f(fConst170);
		float fConst173 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst174 = (0.0f - (2.0f * fConst173));
		fConst175 = mydsp_faustpower2_f(fConst173);
		float fConst176 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst177 = (0.0f - (2.0f * fConst176));
		fConst178 = mydsp_faustpower2_f(fConst176);
		float fConst179 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst180 = (0.0f - (2.0f * fConst179));
		fConst181 = mydsp_faustpower2_f(fConst179);
		float fConst182 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst183 = (0.0f - (2.0f * fConst182));
		fConst184 = mydsp_faustpower2_f(fConst182);
		float fConst185 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst186 = (0.0f - (2.0f * fConst185));
		fConst187 = mydsp_faustpower2_f(fConst185);
		float fConst188 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst189 = (0.0f - (2.0f * fConst188));
		fConst190 = mydsp_faustpower2_f(fConst188);
		float fConst191 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst192 = (0.0f - (2.0f * fConst191));
		fConst193 = mydsp_faustpower2_f(fConst191);
		float fConst194 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst195 = (0.0f - (2.0f * fConst194));
		fConst196 = mydsp_faustpower2_f(fConst194);
		float fConst197 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst198 = (0.0f - (2.0f * fConst197));
		fConst199 = mydsp_faustpower2_f(fConst197);
		float fConst200 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst201 = (0.0f - (2.0f * fConst200));
		fConst202 = mydsp_faustpower2_f(fConst200);
		float fConst203 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst204 = (0.0f - (2.0f * fConst203));
		fConst205 = mydsp_faustpower2_f(fConst203);
		float fConst206 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst207 = (0.0f - (2.0f * fConst206));
		fConst208 = mydsp_faustpower2_f(fConst206);
		float fConst209 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst210 = (0.0f - (2.0f * fConst209));
		fConst211 = mydsp_faustpower2_f(fConst209);
		float fConst212 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst213 = (0.0f - (2.0f * fConst212));
		fConst214 = mydsp_faustpower2_f(fConst212);
		float fConst215 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst216 = (0.0f - (2.0f * fConst215));
		fConst217 = mydsp_faustpower2_f(fConst215);
		float fConst218 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst219 = (0.0f - (2.0f * fConst218));
		fConst220 = mydsp_faustpower2_f(fConst218);
		float fConst221 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst222 = (0.0f - (2.0f * fConst221));
		fConst223 = mydsp_faustpower2_f(fConst221);
		float fConst224 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst225 = (0.0f - (2.0f * fConst224));
		fConst226 = mydsp_faustpower2_f(fConst224);
		fConst227 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst228 = (81.0f / fConst0);
		fConst229 = ftbl1mydspSIG1[98];
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
		float fConst244 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst245 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst244)));
		fConst246 = mydsp_faustpower2_f(fConst244);
		fConst247 = ftbl1mydspSIG1[50];
		float fConst248 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst249 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst248)));
		fConst250 = mydsp_faustpower2_f(fConst248);
		fConst251 = ftbl1mydspSIG1[51];
		float fConst252 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst253 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst252)));
		fConst254 = mydsp_faustpower2_f(fConst252);
		fConst255 = ftbl1mydspSIG1[52];
		float fConst256 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst257 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst256)));
		fConst258 = mydsp_faustpower2_f(fConst256);
		fConst259 = ftbl1mydspSIG1[53];
		float fConst260 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst261 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst260)));
		fConst262 = mydsp_faustpower2_f(fConst260);
		fConst263 = ftbl1mydspSIG1[54];
		float fConst264 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst265 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst264)));
		fConst266 = mydsp_faustpower2_f(fConst264);
		fConst267 = ftbl1mydspSIG1[55];
		float fConst268 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst269 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst268)));
		fConst270 = mydsp_faustpower2_f(fConst268);
		fConst271 = ftbl1mydspSIG1[56];
		float fConst272 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst273 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst272)));
		fConst274 = mydsp_faustpower2_f(fConst272);
		fConst275 = ftbl1mydspSIG1[57];
		float fConst276 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst277 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst276)));
		fConst278 = mydsp_faustpower2_f(fConst276);
		fConst279 = ftbl1mydspSIG1[58];
		float fConst280 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst281 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst280)));
		fConst282 = mydsp_faustpower2_f(fConst280);
		fConst283 = ftbl1mydspSIG1[59];
		float fConst284 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst285 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst284)));
		fConst286 = mydsp_faustpower2_f(fConst284);
		fConst287 = ftbl1mydspSIG1[60];
		float fConst288 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst289 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst288)));
		fConst290 = mydsp_faustpower2_f(fConst288);
		fConst291 = ftbl1mydspSIG1[61];
		float fConst292 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst293 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst292)));
		fConst294 = mydsp_faustpower2_f(fConst292);
		fConst295 = ftbl1mydspSIG1[62];
		float fConst296 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst297 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst296)));
		fConst298 = mydsp_faustpower2_f(fConst296);
		fConst299 = ftbl1mydspSIG1[63];
		float fConst300 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst301 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst300)));
		fConst302 = mydsp_faustpower2_f(fConst300);
		fConst303 = ftbl1mydspSIG1[64];
		float fConst304 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst305 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst304)));
		fConst306 = mydsp_faustpower2_f(fConst304);
		fConst307 = ftbl1mydspSIG1[65];
		float fConst308 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst309 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst308)));
		fConst310 = mydsp_faustpower2_f(fConst308);
		fConst311 = ftbl1mydspSIG1[66];
		float fConst312 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst313 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst312)));
		fConst314 = mydsp_faustpower2_f(fConst312);
		fConst315 = ftbl1mydspSIG1[67];
		float fConst316 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst317 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst316)));
		fConst318 = mydsp_faustpower2_f(fConst316);
		fConst319 = ftbl1mydspSIG1[68];
		float fConst320 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst321 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst320)));
		fConst322 = mydsp_faustpower2_f(fConst320);
		fConst323 = ftbl1mydspSIG1[69];
		float fConst324 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst325 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst324)));
		fConst326 = mydsp_faustpower2_f(fConst324);
		fConst327 = ftbl1mydspSIG1[70];
		float fConst328 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst329 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst328)));
		fConst330 = mydsp_faustpower2_f(fConst328);
		fConst331 = ftbl1mydspSIG1[71];
		float fConst332 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst333 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst332)));
		fConst334 = mydsp_faustpower2_f(fConst332);
		fConst335 = ftbl1mydspSIG1[72];
		float fConst336 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst337 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst336)));
		fConst338 = mydsp_faustpower2_f(fConst336);
		fConst339 = ftbl1mydspSIG1[73];
		float fConst340 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst341 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst340)));
		fConst342 = mydsp_faustpower2_f(fConst340);
		fConst343 = ftbl1mydspSIG1[74];
		float fConst344 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst345 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst344)));
		fConst346 = mydsp_faustpower2_f(fConst344);
		fConst347 = ftbl1mydspSIG1[75];
		float fConst348 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst349 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst348)));
		fConst350 = mydsp_faustpower2_f(fConst348);
		fConst351 = ftbl1mydspSIG1[76];
		float fConst352 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst353 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst352)));
		fConst354 = mydsp_faustpower2_f(fConst352);
		fConst355 = ftbl1mydspSIG1[77];
		float fConst356 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst357 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst356)));
		fConst358 = mydsp_faustpower2_f(fConst356);
		fConst359 = ftbl1mydspSIG1[78];
		float fConst360 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst361 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst360)));
		fConst362 = mydsp_faustpower2_f(fConst360);
		fConst363 = ftbl1mydspSIG1[79];
		float fConst364 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst365 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst364)));
		fConst366 = mydsp_faustpower2_f(fConst364);
		fConst367 = ftbl1mydspSIG1[80];
		float fConst368 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst369 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst368)));
		fConst370 = mydsp_faustpower2_f(fConst368);
		fConst371 = ftbl1mydspSIG1[81];
		float fConst372 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst373 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst372)));
		fConst374 = mydsp_faustpower2_f(fConst372);
		fConst375 = ftbl1mydspSIG1[82];
		float fConst376 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst377 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst376)));
		fConst378 = mydsp_faustpower2_f(fConst376);
		fConst379 = ftbl1mydspSIG1[83];
		float fConst380 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst381 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst380)));
		fConst382 = mydsp_faustpower2_f(fConst380);
		fConst383 = ftbl1mydspSIG1[84];
		float fConst384 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst385 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst384)));
		fConst386 = mydsp_faustpower2_f(fConst384);
		fConst387 = ftbl1mydspSIG1[85];
		float fConst388 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst389 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst388)));
		fConst390 = mydsp_faustpower2_f(fConst388);
		fConst391 = ftbl1mydspSIG1[86];
		float fConst392 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst393 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst392)));
		fConst394 = mydsp_faustpower2_f(fConst392);
		fConst395 = ftbl1mydspSIG1[87];
		float fConst396 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst397 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst396)));
		fConst398 = mydsp_faustpower2_f(fConst396);
		fConst399 = ftbl1mydspSIG1[88];
		float fConst400 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst401 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst400)));
		fConst402 = mydsp_faustpower2_f(fConst400);
		fConst403 = ftbl1mydspSIG1[89];
		float fConst404 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst405 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst404)));
		fConst406 = mydsp_faustpower2_f(fConst404);
		fConst407 = ftbl1mydspSIG1[90];
		float fConst408 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst409 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst408)));
		fConst410 = mydsp_faustpower2_f(fConst408);
		fConst411 = ftbl1mydspSIG1[91];
		float fConst412 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst413 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst412)));
		fConst414 = mydsp_faustpower2_f(fConst412);
		fConst415 = ftbl1mydspSIG1[92];
		float fConst416 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst417 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst416)));
		fConst418 = mydsp_faustpower2_f(fConst416);
		fConst419 = ftbl1mydspSIG1[93];
		float fConst420 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst421 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst420)));
		fConst422 = mydsp_faustpower2_f(fConst420);
		fConst423 = ftbl1mydspSIG1[94];
		float fConst424 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst425 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst424)));
		fConst426 = mydsp_faustpower2_f(fConst424);
		fConst427 = ftbl1mydspSIG1[95];
		float fConst428 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst429 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst428)));
		fConst430 = mydsp_faustpower2_f(fConst428);
		fConst431 = ftbl1mydspSIG1[96];
		float fConst432 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst433 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst432)));
		fConst434 = mydsp_faustpower2_f(fConst432);
		fConst435 = ftbl1mydspSIG1[97];
		float fConst436 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst437 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst436)));
		fConst438 = mydsp_faustpower2_f(fConst436);
		fConst439 = ftbl1mydspSIG1[99];
		float fConst440 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst441 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst440)));
		fConst442 = mydsp_faustpower2_f(fConst440);
		fConst443 = (1.0f / std::max<float>(1.0f, fConst164));
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
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.69999999999999996f);
		fHslider2 = FAUSTFLOAT(20000.0f);
		fHslider3 = FAUSTFLOAT(2000.0f);
		fHslider4 = FAUSTFLOAT(500.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(440.0f);
		fHslider7 = FAUSTFLOAT(0.5f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(440.0f);
		fHslider10 = FAUSTFLOAT(440.0f);
		fHslider11 = FAUSTFLOAT(440.0f);
		fHslider12 = FAUSTFLOAT(1.0f);
		fHslider13 = FAUSTFLOAT(0.10000000000000001f);
		fHslider14 = FAUSTFLOAT(0.10000000000000001f);
		fHslider15 = FAUSTFLOAT(0.0f);
		fHslider16 = FAUSTFLOAT(0.69999999999999996f);
		fHslider17 = FAUSTFLOAT(50.0f);
		fHslider18 = FAUSTFLOAT(0.0f);
		fHslider19 = FAUSTFLOAT(0.69999999999999996f);
		fHslider20 = FAUSTFLOAT(0.0f);
		fHslider21 = FAUSTFLOAT(0.69999999999999996f);
		fHslider22 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider23 = FAUSTFLOAT(0.69999999999999996f);
		fHslider24 = FAUSTFLOAT(20000.0f);
		fHslider25 = FAUSTFLOAT(2000.0f);
		fHslider26 = FAUSTFLOAT(500.0f);
		fHslider27 = FAUSTFLOAT(0.0f);
		fHslider28 = FAUSTFLOAT(440.0f);
		fHslider29 = FAUSTFLOAT(0.01f);
		fHslider30 = FAUSTFLOAT(1500.0f);
		fHslider31 = FAUSTFLOAT(1.0f);
		fHslider32 = FAUSTFLOAT(0.10000000000000001f);
		fHslider33 = FAUSTFLOAT(0.10000000000000001f);
		fHslider34 = FAUSTFLOAT(0.0f);
		fHslider35 = FAUSTFLOAT(0.69999999999999996f);
		fHslider36 = FAUSTFLOAT(50.0f);
		fHslider37 = FAUSTFLOAT(0.0f);
		fHslider38 = FAUSTFLOAT(0.69999999999999996f);
		fHslider39 = FAUSTFLOAT(0.0f);
		fHslider40 = FAUSTFLOAT(0.69999999999999996f);
		fHslider41 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider42 = FAUSTFLOAT(0.69999999999999996f);
		fHslider43 = FAUSTFLOAT(20000.0f);
		fHslider44 = FAUSTFLOAT(2000.0f);
		fHslider45 = FAUSTFLOAT(500.0f);
		fHslider46 = FAUSTFLOAT(0.0f);
		fHslider47 = FAUSTFLOAT(440.0f);
		fHslider48 = FAUSTFLOAT(0.0f);
		fHslider49 = FAUSTFLOAT(1.0f);
		fHslider50 = FAUSTFLOAT(0.10000000000000001f);
		fHslider51 = FAUSTFLOAT(0.10000000000000001f);
		fHslider52 = FAUSTFLOAT(0.0f);
		fHslider53 = FAUSTFLOAT(0.69999999999999996f);
		fHslider54 = FAUSTFLOAT(50.0f);
		fHslider55 = FAUSTFLOAT(0.0f);
		fHslider56 = FAUSTFLOAT(0.69999999999999996f);
		fHslider57 = FAUSTFLOAT(0.0f);
		fHslider58 = FAUSTFLOAT(0.69999999999999996f);
		fHslider59 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider60 = FAUSTFLOAT(0.69999999999999996f);
		fHslider61 = FAUSTFLOAT(20000.0f);
		fHslider62 = FAUSTFLOAT(2000.0f);
		fHslider63 = FAUSTFLOAT(500.0f);
		fHslider64 = FAUSTFLOAT(0.90000000000000002f);
		fHslider65 = FAUSTFLOAT(0.0f);
		fHslider66 = FAUSTFLOAT(420.0f);
		fHslider67 = FAUSTFLOAT(0.5f);
		fHslider68 = FAUSTFLOAT(1.0f);
		fHslider69 = FAUSTFLOAT(0.10000000000000001f);
		fHslider70 = FAUSTFLOAT(0.10000000000000001f);
		fHslider71 = FAUSTFLOAT(0.0f);
		fHslider72 = FAUSTFLOAT(0.69999999999999996f);
		fHslider73 = FAUSTFLOAT(50.0f);
		fHslider74 = FAUSTFLOAT(0.0f);
		fHslider75 = FAUSTFLOAT(0.69999999999999996f);
		fHslider76 = FAUSTFLOAT(0.0f);
		fHslider77 = FAUSTFLOAT(0.69999999999999996f);
		fHslider78 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider79 = FAUSTFLOAT(0.69999999999999996f);
		fHslider80 = FAUSTFLOAT(20000.0f);
		fHslider81 = FAUSTFLOAT(2000.0f);
		fHslider82 = FAUSTFLOAT(500.0f);
		fHslider83 = FAUSTFLOAT(1.0f);
		fHslider84 = FAUSTFLOAT(0.0f);
		fHslider85 = FAUSTFLOAT(1.0f);
		fHslider86 = FAUSTFLOAT(0.10000000000000001f);
		fHslider87 = FAUSTFLOAT(0.10000000000000001f);
		fHslider88 = FAUSTFLOAT(0.0f);
		fHslider89 = FAUSTFLOAT(0.69999999999999996f);
		fHslider90 = FAUSTFLOAT(50.0f);
		fHslider91 = FAUSTFLOAT(0.0f);
		fHslider92 = FAUSTFLOAT(0.69999999999999996f);
		fHslider93 = FAUSTFLOAT(0.0f);
		fHslider94 = FAUSTFLOAT(0.69999999999999996f);
		fHslider95 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider96 = FAUSTFLOAT(0.69999999999999996f);
		fHslider97 = FAUSTFLOAT(20000.0f);
		fHslider98 = FAUSTFLOAT(2000.0f);
		fHslider99 = FAUSTFLOAT(500.0f);
		fHslider100 = FAUSTFLOAT(1.0f);
		fHslider101 = FAUSTFLOAT(0.10000000000000001f);
		fHslider102 = FAUSTFLOAT(0.10000000000000001f);
		fHslider103 = FAUSTFLOAT(0.0f);
		fHslider104 = FAUSTFLOAT(0.69999999999999996f);
		fHslider105 = FAUSTFLOAT(50.0f);
		fHslider106 = FAUSTFLOAT(0.0f);
		fHslider107 = FAUSTFLOAT(0.69999999999999996f);
		fHslider108 = FAUSTFLOAT(0.0f);
		fHslider109 = FAUSTFLOAT(0.69999999999999996f);
		fHslider110 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider111 = FAUSTFLOAT(0.69999999999999996f);
		fHslider112 = FAUSTFLOAT(20000.0f);
		fHslider113 = FAUSTFLOAT(2000.0f);
		fHslider114 = FAUSTFLOAT(500.0f);
		fHslider115 = FAUSTFLOAT(0.0f);
		fHslider116 = FAUSTFLOAT(10.0f);
		fHslider117 = FAUSTFLOAT(10.0f);
		fHslider118 = FAUSTFLOAT(10.0f);
		fHslider119 = FAUSTFLOAT(1.0f);
		fHslider120 = FAUSTFLOAT(0.10000000000000001f);
		fHslider121 = FAUSTFLOAT(0.10000000000000001f);
		fHslider122 = FAUSTFLOAT(0.0f);
		fHslider123 = FAUSTFLOAT(0.69999999999999996f);
		fHslider124 = FAUSTFLOAT(50.0f);
		fHslider125 = FAUSTFLOAT(0.0f);
		fHslider126 = FAUSTFLOAT(0.69999999999999996f);
		fHslider127 = FAUSTFLOAT(0.0f);
		fHslider128 = FAUSTFLOAT(0.69999999999999996f);
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
			fVec7[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec68[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec61[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec69[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 4); l39 = (l39 + 1)) {
			fRec71[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 1024); l40 = (l40 + 1)) {
			fRec72[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fVec8[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fVec9[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2048); l43 = (l43 + 1)) {
			fRec70[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec57[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec53[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 1024); l46 = (l46 + 1)) {
			fRec49[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 4); l47 = (l47 + 1)) {
			fRec47[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2048); l48 = (l48 + 1)) {
			fRec42[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec40[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec73[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec39[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			iRec74[l52] = 0;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			iRec75[l53] = 0;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec78[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec77[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec76[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec79[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec38[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec37[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec36[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec85[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec84[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec86[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec88[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 3); l65 = (l65 + 1)) {
			fRec87[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 3); l66 = (l66 + 1)) {
			fRec90[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec91[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec92[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec93[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec83[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec96[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec95[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec94[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec97[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec82[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec81[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec80[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			iRec116[l78] = 0;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec122[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec123[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fVec10[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fVec11[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec126[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec125[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec124[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			iRec128[l86] = 0;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec127[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec129[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2048); l89 = (l89 + 1)) {
			fRec130[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fVec12[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2048); l91 = (l91 + 1)) {
			fVec13[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec14[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec121[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2048); l94 = (l94 + 1)) {
			fRec112[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec108[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2048); l96 = (l96 + 1)) {
			fRec104[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec102[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec103[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			fRec101[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec133[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec132[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec131[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec134[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec100[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec99[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec98[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 2048); l107 = (l107 + 1)) {
			fVec15[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 2048); l108 = (l108 + 1)) {
			fVec16[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec9[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec138[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec137[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 32768); l112 = (l112 + 1)) {
			fVec17[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 4096); l113 = (l113 + 1)) {
			fVec18[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec135[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec142[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec141[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 16384); l117 = (l117 + 1)) {
			fVec19[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 4096); l118 = (l118 + 1)) {
			fVec20[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec139[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec146[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec145[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 32768); l122 = (l122 + 1)) {
			fVec21[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 4096); l123 = (l123 + 1)) {
			fVec22[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec143[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec150[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec149[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 16384); l127 = (l127 + 1)) {
			fVec23[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			iRec155[l128] = 0;
		}
		for (int l129 = 0; (l129 < 3); l129 = (l129 + 1)) {
			fRec154[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			iRec157[l130] = 0;
		}
		for (int l131 = 0; (l131 < 3); l131 = (l131 + 1)) {
			fRec156[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			iRec159[l132] = 0;
		}
		for (int l133 = 0; (l133 < 3); l133 = (l133 + 1)) {
			fRec158[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			iRec161[l134] = 0;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec160[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 2); l136 = (l136 + 1)) {
			fRec164[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec163[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 2); l138 = (l138 + 1)) {
			fRec162[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 3); l139 = (l139 + 1)) {
			fRec165[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 3); l140 = (l140 + 1)) {
			fRec153[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 3); l141 = (l141 + 1)) {
			fRec152[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 3); l142 = (l142 + 1)) {
			fRec151[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 2048); l143 = (l143 + 1)) {
			fVec24[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 2048); l144 = (l144 + 1)) {
			fVec25[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			fRec147[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec169[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec168[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 16384); l148 = (l148 + 1)) {
			fVec26[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 4096); l149 = (l149 + 1)) {
			fVec27[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 2); l150 = (l150 + 1)) {
			fRec166[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fRec173[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec172[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 16384); l153 = (l153 + 1)) {
			fVec28[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 4096); l154 = (l154 + 1)) {
			fVec29[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec170[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec177[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec176[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 16384); l158 = (l158 + 1)) {
			fVec30[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 2048); l159 = (l159 + 1)) {
			fVec31[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec174[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 3); l161 = (l161 + 1)) {
			fRec1[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 3); l162 = (l162 + 1)) {
			fRec2[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 3); l163 = (l163 + 1)) {
			fRec3[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 3); l164 = (l164 + 1)) {
			fRec4[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 3); l165 = (l165 + 1)) {
			fRec5[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 3); l166 = (l166 + 1)) {
			fRec6[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 3); l167 = (l167 + 1)) {
			fRec7[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 3); l168 = (l168 + 1)) {
			fRec8[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fRec178[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 3); l170 = (l170 + 1)) {
			fRec0[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 3); l171 = (l171 + 1)) {
			fRec184[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec183[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fVec32[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 2); l174 = (l174 + 1)) {
			fVec33[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 2); l175 = (l175 + 1)) {
			fVec34[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			iRec185[l176] = 0;
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec186[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 3); l178 = (l178 + 1)) {
			fRec182[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 3); l179 = (l179 + 1)) {
			fRec187[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 3); l180 = (l180 + 1)) {
			fRec188[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 3); l181 = (l181 + 1)) {
			fRec189[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 3); l182 = (l182 + 1)) {
			fRec190[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 3); l183 = (l183 + 1)) {
			fRec191[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 3); l184 = (l184 + 1)) {
			fRec192[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 3); l185 = (l185 + 1)) {
			fRec193[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 3); l186 = (l186 + 1)) {
			fRec194[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 3); l187 = (l187 + 1)) {
			fRec195[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 3); l188 = (l188 + 1)) {
			fRec196[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 3); l189 = (l189 + 1)) {
			fRec197[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 3); l190 = (l190 + 1)) {
			fRec198[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 3); l191 = (l191 + 1)) {
			fRec199[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 3); l192 = (l192 + 1)) {
			fRec200[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 3); l193 = (l193 + 1)) {
			fRec201[l193] = 0.0f;
		}
		for (int l194 = 0; (l194 < 3); l194 = (l194 + 1)) {
			fRec202[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 3); l195 = (l195 + 1)) {
			fRec203[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec204[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec205[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 2); l198 = (l198 + 1)) {
			fRec208[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 2); l199 = (l199 + 1)) {
			fRec207[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 2); l200 = (l200 + 1)) {
			fRec206[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec209[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 3); l202 = (l202 + 1)) {
			fRec181[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 3); l203 = (l203 + 1)) {
			fRec180[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 3); l204 = (l204 + 1)) {
			fRec179[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec213[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec215[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec219[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec218[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fRec217[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec216[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec214[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 2); l212 = (l212 + 1)) {
			fRec222[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec221[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec220[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 3); l215 = (l215 + 1)) {
			fRec223[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec212[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 3); l217 = (l217 + 1)) {
			fRec211[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec210[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 3); l219 = (l219 + 1)) {
			fRec229[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 3); l220 = (l220 + 1)) {
			fRec228[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fVec35[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 2); l222 = (l222 + 1)) {
			fVec36[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fVec37[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 2); l224 = (l224 + 1)) {
			iRec230[l224] = 0;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec227[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 3); l226 = (l226 + 1)) {
			fRec231[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 3); l227 = (l227 + 1)) {
			fRec232[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 3); l228 = (l228 + 1)) {
			fRec233[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 3); l229 = (l229 + 1)) {
			fRec234[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 3); l230 = (l230 + 1)) {
			fRec235[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 3); l231 = (l231 + 1)) {
			fRec236[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 3); l232 = (l232 + 1)) {
			fRec237[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 3); l233 = (l233 + 1)) {
			fRec238[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fRec239[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 3); l235 = (l235 + 1)) {
			fRec240[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 3); l236 = (l236 + 1)) {
			fRec241[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 3); l237 = (l237 + 1)) {
			fRec242[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 3); l238 = (l238 + 1)) {
			fRec243[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 3); l239 = (l239 + 1)) {
			fRec244[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 3); l240 = (l240 + 1)) {
			fRec245[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 3); l241 = (l241 + 1)) {
			fRec246[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 3); l242 = (l242 + 1)) {
			fRec247[l242] = 0.0f;
		}
		for (int l243 = 0; (l243 < 3); l243 = (l243 + 1)) {
			fRec248[l243] = 0.0f;
		}
		for (int l244 = 0; (l244 < 3); l244 = (l244 + 1)) {
			fRec249[l244] = 0.0f;
		}
		for (int l245 = 0; (l245 < 3); l245 = (l245 + 1)) {
			fRec250[l245] = 0.0f;
		}
		for (int l246 = 0; (l246 < 3); l246 = (l246 + 1)) {
			fRec251[l246] = 0.0f;
		}
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec252[l247] = 0.0f;
		}
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec253[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec254[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec255[l250] = 0.0f;
		}
		for (int l251 = 0; (l251 < 3); l251 = (l251 + 1)) {
			fRec256[l251] = 0.0f;
		}
		for (int l252 = 0; (l252 < 3); l252 = (l252 + 1)) {
			fRec257[l252] = 0.0f;
		}
		for (int l253 = 0; (l253 < 3); l253 = (l253 + 1)) {
			fRec258[l253] = 0.0f;
		}
		for (int l254 = 0; (l254 < 3); l254 = (l254 + 1)) {
			fRec259[l254] = 0.0f;
		}
		for (int l255 = 0; (l255 < 3); l255 = (l255 + 1)) {
			fRec260[l255] = 0.0f;
		}
		for (int l256 = 0; (l256 < 3); l256 = (l256 + 1)) {
			fRec261[l256] = 0.0f;
		}
		for (int l257 = 0; (l257 < 3); l257 = (l257 + 1)) {
			fRec262[l257] = 0.0f;
		}
		for (int l258 = 0; (l258 < 3); l258 = (l258 + 1)) {
			fRec263[l258] = 0.0f;
		}
		for (int l259 = 0; (l259 < 3); l259 = (l259 + 1)) {
			fRec264[l259] = 0.0f;
		}
		for (int l260 = 0; (l260 < 3); l260 = (l260 + 1)) {
			fRec265[l260] = 0.0f;
		}
		for (int l261 = 0; (l261 < 3); l261 = (l261 + 1)) {
			fRec266[l261] = 0.0f;
		}
		for (int l262 = 0; (l262 < 3); l262 = (l262 + 1)) {
			fRec267[l262] = 0.0f;
		}
		for (int l263 = 0; (l263 < 3); l263 = (l263 + 1)) {
			fRec268[l263] = 0.0f;
		}
		for (int l264 = 0; (l264 < 3); l264 = (l264 + 1)) {
			fRec269[l264] = 0.0f;
		}
		for (int l265 = 0; (l265 < 3); l265 = (l265 + 1)) {
			fRec270[l265] = 0.0f;
		}
		for (int l266 = 0; (l266 < 3); l266 = (l266 + 1)) {
			fRec271[l266] = 0.0f;
		}
		for (int l267 = 0; (l267 < 3); l267 = (l267 + 1)) {
			fRec272[l267] = 0.0f;
		}
		for (int l268 = 0; (l268 < 3); l268 = (l268 + 1)) {
			fRec273[l268] = 0.0f;
		}
		for (int l269 = 0; (l269 < 3); l269 = (l269 + 1)) {
			fRec274[l269] = 0.0f;
		}
		for (int l270 = 0; (l270 < 3); l270 = (l270 + 1)) {
			fRec275[l270] = 0.0f;
		}
		for (int l271 = 0; (l271 < 3); l271 = (l271 + 1)) {
			fRec276[l271] = 0.0f;
		}
		for (int l272 = 0; (l272 < 3); l272 = (l272 + 1)) {
			fRec277[l272] = 0.0f;
		}
		for (int l273 = 0; (l273 < 3); l273 = (l273 + 1)) {
			fRec278[l273] = 0.0f;
		}
		for (int l274 = 0; (l274 < 3); l274 = (l274 + 1)) {
			fRec279[l274] = 0.0f;
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec280[l275] = 0.0f;
		}
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec281[l276] = 0.0f;
		}
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec282[l277] = 0.0f;
		}
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec283[l278] = 0.0f;
		}
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec284[l279] = 0.0f;
		}
		for (int l280 = 0; (l280 < 2); l280 = (l280 + 1)) {
			fRec285[l280] = 0.0f;
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec288[l281] = 0.0f;
		}
		for (int l282 = 0; (l282 < 2); l282 = (l282 + 1)) {
			fRec287[l282] = 0.0f;
		}
		for (int l283 = 0; (l283 < 2); l283 = (l283 + 1)) {
			fRec286[l283] = 0.0f;
		}
		for (int l284 = 0; (l284 < 3); l284 = (l284 + 1)) {
			fRec289[l284] = 0.0f;
		}
		for (int l285 = 0; (l285 < 3); l285 = (l285 + 1)) {
			fRec226[l285] = 0.0f;
		}
		for (int l286 = 0; (l286 < 3); l286 = (l286 + 1)) {
			fRec225[l286] = 0.0f;
		}
		for (int l287 = 0; (l287 < 3); l287 = (l287 + 1)) {
			fRec224[l287] = 0.0f;
		}
		for (int l288 = 0; (l288 < 2); l288 = (l288 + 1)) {
			fRec292[l288] = 0.0f;
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec291[l289] = 0.0f;
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec290[l290] = 0.0f;
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec295[l291] = 0.0f;
		}
		for (int l292 = 0; (l292 < 2); l292 = (l292 + 1)) {
			fRec294[l292] = 0.0f;
		}
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec293[l293] = 0.0f;
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec298[l294] = 0.0f;
		}
		for (int l295 = 0; (l295 < 2); l295 = (l295 + 1)) {
			fRec297[l295] = 0.0f;
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec296[l296] = 0.0f;
		}
		for (int l297 = 0; (l297 < 3); l297 = (l297 + 1)) {
			fRec299[l297] = 0.0f;
		}
		for (int l298 = 0; (l298 < 2); l298 = (l298 + 1)) {
			fRec302[l298] = 0.0f;
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec301[l299] = 0.0f;
		}
		for (int l300 = 0; (l300 < 2); l300 = (l300 + 1)) {
			fRec300[l300] = 0.0f;
		}
		for (int l301 = 0; (l301 < 2); l301 = (l301 + 1)) {
			fRec305[l301] = 0.0f;
		}
		for (int l302 = 0; (l302 < 2); l302 = (l302 + 1)) {
			fRec304[l302] = 0.0f;
		}
		for (int l303 = 0; (l303 < 2); l303 = (l303 + 1)) {
			fRec303[l303] = 0.0f;
		}
		for (int l304 = 0; (l304 < 2); l304 = (l304 + 1)) {
			fRec308[l304] = 0.0f;
		}
		for (int l305 = 0; (l305 < 2); l305 = (l305 + 1)) {
			fRec307[l305] = 0.0f;
		}
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec306[l306] = 0.0f;
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
		ui_interface->declare(&fHslider86, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider86, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider85, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider85, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider87, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider87, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider88, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider88, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider50, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider49, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider49, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider51, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider52, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider13, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider12, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider12, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider14, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider15, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->declare(&fHslider69, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider69, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider68, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider68, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider70, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider70, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider71, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider71, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider101, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider100, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider102, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider103, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider32, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider31, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider31, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider33, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider33, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider34, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->declare(&fHslider120, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider120, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider119, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider119, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider121, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider121, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider122, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider122, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("EQ_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider90, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider89, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider89, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider82, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider82, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider91, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider92, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider92, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider81, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider81, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider93, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider94, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider94, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider80, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider80, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider79, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider79, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider54, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider53, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider53, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider45, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider45, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider55, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider56, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider56, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider44, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider44, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider57, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider58, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider58, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider43, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider43, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider42, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider42, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider17, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider16, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider18, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider18, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider19, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider3, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider20, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider20, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider21, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
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
		ui_interface->declare(&fHslider73, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider73, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider72, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider72, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider63, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider63, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider74, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider74, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider75, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider75, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider62, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider62, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider76, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider76, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider77, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider77, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider61, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider61, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider60, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider60, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider105, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider104, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider99, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider99, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider106, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider106, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider107, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider107, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider98, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider98, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider108, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider108, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider109, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider109, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider97, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider97, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider96, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider96, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider36, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider35, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider26, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider26, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider37, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider38, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider38, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider25, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider25, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider39, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider39, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider40, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider40, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider24, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider24, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider23, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider23, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider124, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider124, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider123, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider123, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider114, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider114, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider125, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider125, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider126, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider126, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider113, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider113, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider127, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider127, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider128, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider128, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider112, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider112, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider111, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider111, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider78, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider41, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider0, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider59, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 5", &fHslider95, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 6", &fHslider22, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 7", &fHslider110, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox4);
		ui_interface->addCheckButton("2", &fCheckbox2);
		ui_interface->addCheckButton("3", &fCheckbox0);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->addCheckButton("5", &fCheckbox5);
		ui_interface->addCheckButton("6", &fCheckbox1);
		ui_interface->addCheckButton("7", &fCheckbox6);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider84, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider48, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider27, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider29, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 15");
		ui_interface->addHorizontalSlider("Control 0", &fHslider83, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 16");
		ui_interface->addHorizontalSlider("Control 0", &fHslider64, 0.899999976f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 17");
		ui_interface->addHorizontalSlider("Control 0", &fHslider115, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 18");
		ui_interface->addHorizontalSlider("Control 0", &fHslider30, 1500.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 19");
		ui_interface->addHorizontalSlider("Control 0", &fHslider28, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider47, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 20");
		ui_interface->addHorizontalSlider("Control 0", &fHslider116, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 21");
		ui_interface->addHorizontalSlider("Control 0", &fHslider117, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 22");
		ui_interface->addHorizontalSlider("Control 0", &fHslider118, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 23");
		ui_interface->addHorizontalSlider("Control 0", &fHslider67, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 24");
		ui_interface->addHorizontalSlider("Control 0", &fHslider65, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider46, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider9, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider10, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider11, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider5, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider8, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider66, 420.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (1.25892544f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = (1.0f / float(fHslider1));
		float fSlow2 = std::tan((fConst18 * float(fHslider2)));
		float fSlow3 = (1.0f / fSlow2);
		float fSlow4 = (((fSlow1 + fSlow3) / fSlow2) + 1.0f);
		float fSlow5 = ((std::pow(10.0f, (0.0500000007f * float(fHslider0))) * (1.0f - float(fCheckbox0))) / fSlow4);
		float fSlow6 = float(fHslider3);
		float fSlow7 = std::tan((fConst18 * fSlow6));
		float fSlow8 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow7))));
		float fSlow9 = float(fHslider4);
		float fSlow10 = std::tan((fConst18 * fSlow9));
		float fSlow11 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow10))));
		float fSlow12 = float(fHslider5);
		float fSlow13 = float((fSlow12 > 0.0f));
		float fSlow14 = (0.0f - fSlow12);
		float fSlow15 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider6)));
		float fSlow16 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider7))) + -0.5f)));
		float fSlow17 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider8)));
		float fSlow18 = ((fSlow15 * fSlow16) * ((0.0500000007f * fSlow17) + 1.0f));
		float fSlow19 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider9))) * fSlow16) * (1.0f - (0.0299999993f * fSlow17)));
		float fSlow20 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider10))) * fSlow16) * ((0.310000002f * fSlow17) + 1.0f));
		float fSlow21 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider11))) * fSlow16) * (1.0f - (0.270000011f * fSlow17)));
		float fSlow22 = (fConst31 * ((1.0f / float(fHslider12)) + -1.0f));
		float fSlow23 = float(fHslider13);
		int iSlow24 = (std::fabs(fSlow23) < 1.1920929e-07f);
		float fSlow25 = (iSlow24 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow24 ? 1.0f : fSlow23)))));
		float fSlow26 = float(fHslider14);
		int iSlow27 = (std::fabs(fSlow26) < 1.1920929e-07f);
		float fSlow28 = (iSlow27 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow27 ? 1.0f : fSlow26)))));
		float fSlow29 = (1.0f - fSlow28);
		float fSlow30 = (1.0f - fSlow25);
		float fSlow31 = float(fHslider15);
		float fSlow32 = (1.0f / float(fHslider16));
		float fSlow33 = std::tan((fConst18 * float(fHslider17)));
		float fSlow34 = (1.0f / fSlow33);
		float fSlow35 = (1.0f / (((fSlow32 + fSlow34) / fSlow33) + 1.0f));
		float fSlow36 = (((fSlow34 - fSlow32) / fSlow33) + 1.0f);
		float fSlow37 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow33))));
		float fSlow38 = (1.0f / fSlow10);
		float fSlow39 = float(fHslider18);
		int iSlow40 = (fSlow39 > 0.0f);
		float fSlow41 = (float(fHslider19) * std::sin((fConst32 * fSlow9)));
		float fSlow42 = (fConst18 * ((fSlow9 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow39)))) / fSlow41));
		float fSlow43 = (fConst18 * (fSlow9 / fSlow41));
		float fSlow44 = (iSlow40 ? fSlow43 : fSlow42);
		float fSlow45 = ((fSlow38 * (fSlow38 - fSlow44)) + 1.0f);
		float fSlow46 = ((fSlow38 * (fSlow38 + fSlow44)) + 1.0f);
		float fSlow47 = (iSlow40 ? fSlow42 : fSlow43);
		float fSlow48 = ((fSlow38 * (fSlow38 + fSlow47)) + 1.0f);
		float fSlow49 = ((fSlow38 * (fSlow38 - fSlow47)) + 1.0f);
		float fSlow50 = (1.0f / fSlow7);
		float fSlow51 = float(fHslider20);
		int iSlow52 = (fSlow51 > 0.0f);
		float fSlow53 = (float(fHslider21) * std::sin((fConst32 * fSlow6)));
		float fSlow54 = (fConst18 * ((fSlow6 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow51)))) / fSlow53));
		float fSlow55 = (fConst18 * (fSlow6 / fSlow53));
		float fSlow56 = (iSlow52 ? fSlow55 : fSlow54);
		float fSlow57 = ((fSlow50 * (fSlow50 - fSlow56)) + 1.0f);
		float fSlow58 = ((fSlow50 * (fSlow50 + fSlow56)) + 1.0f);
		float fSlow59 = (iSlow52 ? fSlow54 : fSlow55);
		float fSlow60 = ((fSlow50 * (fSlow50 + fSlow59)) + 1.0f);
		float fSlow61 = ((fSlow50 * (fSlow50 - fSlow59)) + 1.0f);
		float fSlow62 = (1.0f / fSlow4);
		float fSlow63 = (((fSlow3 - fSlow1) / fSlow2) + 1.0f);
		float fSlow64 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow2))));
		float fSlow65 = (1.0f / float(fHslider23));
		float fSlow66 = std::tan((fConst18 * float(fHslider24)));
		float fSlow67 = (1.0f / fSlow66);
		float fSlow68 = (((fSlow65 + fSlow67) / fSlow66) + 1.0f);
		float fSlow69 = ((std::pow(10.0f, (0.0500000007f * float(fHslider22))) * (1.0f - float(fCheckbox1))) / fSlow68);
		float fSlow70 = float(fHslider25);
		float fSlow71 = std::tan((fConst18 * fSlow70));
		float fSlow72 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow71))));
		float fSlow73 = float(fHslider26);
		float fSlow74 = std::tan((fConst18 * fSlow73));
		float fSlow75 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow74))));
		float fSlow76 = (2.0f * fSlow16);
		float fSlow77 = (0.5f * fSlow17);
		float fSlow78 = float(fHslider27);
		float fSlow79 = float((fSlow78 > 0.0f));
		float fSlow80 = (0.0f - fSlow78);
		float fSlow81 = (2.0f * float(fHslider28));
		float fSlow82 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider29)));
		float fSlow83 = (1.0f - fSlow82);
		float fSlow84 = (0.00999999978f * float(fHslider30));
		float fSlow85 = (fConst31 * ((1.0f / float(fHslider31)) + -1.0f));
		float fSlow86 = float(fHslider32);
		int iSlow87 = (std::fabs(fSlow86) < 1.1920929e-07f);
		float fSlow88 = (iSlow87 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow87 ? 1.0f : fSlow86)))));
		float fSlow89 = float(fHslider33);
		int iSlow90 = (std::fabs(fSlow89) < 1.1920929e-07f);
		float fSlow91 = (iSlow90 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow90 ? 1.0f : fSlow89)))));
		float fSlow92 = (1.0f - fSlow91);
		float fSlow93 = (1.0f - fSlow88);
		float fSlow94 = float(fHslider34);
		float fSlow95 = (1.0f / float(fHslider35));
		float fSlow96 = std::tan((fConst18 * float(fHslider36)));
		float fSlow97 = (1.0f / fSlow96);
		float fSlow98 = (1.0f / (((fSlow95 + fSlow97) / fSlow96) + 1.0f));
		float fSlow99 = (((fSlow97 - fSlow95) / fSlow96) + 1.0f);
		float fSlow100 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow96))));
		float fSlow101 = (1.0f / fSlow74);
		float fSlow102 = float(fHslider37);
		int iSlow103 = (fSlow102 > 0.0f);
		float fSlow104 = (float(fHslider38) * std::sin((fConst32 * fSlow73)));
		float fSlow105 = (fConst18 * ((fSlow73 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow102)))) / fSlow104));
		float fSlow106 = (fConst18 * (fSlow73 / fSlow104));
		float fSlow107 = (iSlow103 ? fSlow106 : fSlow105);
		float fSlow108 = ((fSlow101 * (fSlow101 - fSlow107)) + 1.0f);
		float fSlow109 = ((fSlow101 * (fSlow101 + fSlow107)) + 1.0f);
		float fSlow110 = (iSlow103 ? fSlow105 : fSlow106);
		float fSlow111 = ((fSlow101 * (fSlow101 + fSlow110)) + 1.0f);
		float fSlow112 = ((fSlow101 * (fSlow101 - fSlow110)) + 1.0f);
		float fSlow113 = (1.0f / fSlow71);
		float fSlow114 = float(fHslider39);
		int iSlow115 = (fSlow114 > 0.0f);
		float fSlow116 = (float(fHslider40) * std::sin((fConst32 * fSlow70)));
		float fSlow117 = (fConst18 * ((fSlow70 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow114)))) / fSlow116));
		float fSlow118 = (fConst18 * (fSlow70 / fSlow116));
		float fSlow119 = (iSlow115 ? fSlow118 : fSlow117);
		float fSlow120 = ((fSlow113 * (fSlow113 - fSlow119)) + 1.0f);
		float fSlow121 = ((fSlow113 * (fSlow113 + fSlow119)) + 1.0f);
		float fSlow122 = (iSlow115 ? fSlow117 : fSlow118);
		float fSlow123 = ((fSlow113 * (fSlow113 + fSlow122)) + 1.0f);
		float fSlow124 = ((fSlow113 * (fSlow113 - fSlow122)) + 1.0f);
		float fSlow125 = (1.0f / fSlow68);
		float fSlow126 = (((fSlow67 - fSlow65) / fSlow66) + 1.0f);
		float fSlow127 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow66))));
		float fSlow128 = (1.0f / float(fHslider42));
		float fSlow129 = std::tan((fConst18 * float(fHslider43)));
		float fSlow130 = (1.0f / fSlow129);
		float fSlow131 = (((fSlow128 + fSlow130) / fSlow129) + 1.0f);
		float fSlow132 = ((std::pow(10.0f, (0.0500000007f * float(fHslider41))) * (1.0f - float(fCheckbox2))) / fSlow131);
		float fSlow133 = float(fHslider44);
		float fSlow134 = std::tan((fConst18 * fSlow133));
		float fSlow135 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow134))));
		float fSlow136 = float(fHslider45);
		float fSlow137 = std::tan((fConst18 * fSlow136));
		float fSlow138 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow137))));
		float fSlow139 = (fConst42 * float((float(fHslider46) > 0.0f)));
		float fSlow140 = (fConst44 * (float(fHslider47) * fSlow16));
		float fSlow141 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider48)));
		float fSlow142 = (fSlow141 + 5.0f);
		int iSlow143 = (fSlow142 < 23.0f);
		int iSlow144 = (fSlow142 < 24.0f);
		float fSlow145 = (fSlow141 + -18.0f);
		int iSlow146 = (fSlow142 < 22.0f);
		float fSlow147 = (fSlow141 + -17.0f);
		int iSlow148 = (fSlow142 < 21.0f);
		float fSlow149 = (fSlow141 + -16.0f);
		int iSlow150 = (fSlow142 < 20.0f);
		float fSlow151 = (fSlow141 + -15.0f);
		int iSlow152 = (fSlow142 < 19.0f);
		float fSlow153 = (fSlow141 + -14.0f);
		int iSlow154 = (fSlow142 < 18.0f);
		float fSlow155 = (fSlow141 + -13.0f);
		int iSlow156 = (fSlow142 < 17.0f);
		float fSlow157 = (fSlow141 + -12.0f);
		int iSlow158 = (fSlow142 < 16.0f);
		float fSlow159 = (fSlow141 + -11.0f);
		float fSlow160 = (80.0f * fSlow159);
		int iSlow161 = (fSlow142 < 15.0f);
		float fSlow162 = (fSlow141 + -10.0f);
		int iSlow163 = (fSlow142 < 14.0f);
		float fSlow164 = (fSlow141 + -9.0f);
		int iSlow165 = (fSlow142 < 13.0f);
		float fSlow166 = (fSlow141 + -8.0f);
		int iSlow167 = (fSlow142 < 12.0f);
		float fSlow168 = (fSlow141 + -7.0f);
		int iSlow169 = (fSlow142 < 11.0f);
		float fSlow170 = (fSlow141 + -6.0f);
		int iSlow171 = (fSlow142 < 10.0f);
		float fSlow172 = (fSlow141 + -5.0f);
		int iSlow173 = (fSlow142 < 9.0f);
		float fSlow174 = (fSlow141 + -4.0f);
		int iSlow175 = (fSlow142 < 8.0f);
		float fSlow176 = (fSlow141 + -3.0f);
		float fSlow177 = (50.0f * fSlow176);
		int iSlow178 = (fSlow142 < 7.0f);
		float fSlow179 = (fSlow141 + -2.0f);
		int iSlow180 = (fSlow142 < 6.0f);
		float fSlow181 = (fSlow141 + -1.0f);
		int iSlow182 = (fSlow142 < 5.0f);
		int iSlow183 = (fSlow142 < 4.0f);
		float fSlow184 = (fSlow141 + 1.0f);
		int iSlow185 = (fSlow142 < 3.0f);
		float fSlow186 = (fSlow141 + 2.0f);
		int iSlow187 = (fSlow142 < 2.0f);
		float fSlow188 = (fSlow141 + 3.0f);
		int iSlow189 = (fSlow142 < 1.0f);
		float fSlow190 = (fSlow141 + 4.0f);
		int iSlow191 = (fSlow142 < 0.0f);
		float fSlow192 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 800.0f : (iSlow189 ? (800.0f - (400.0f * fSlow142)) : 400.0f)) : (iSlow187 ? (400.0f - (50.0f * fSlow190)) : 350.0f)) : (iSlow185 ? ((100.0f * fSlow188) + 350.0f) : 450.0f)) : (iSlow183 ? (450.0f - (125.0f * fSlow186)) : 325.0f)) : (iSlow182 ? ((275.0f * fSlow184) + 325.0f) : 600.0f)) : (iSlow180 ? (600.0f - (200.0f * fSlow141)) : 400.0f)) : (iSlow178 ? (400.0f - (150.0f * fSlow181)) : 250.0f)) : (iSlow175 ? ((150.0f * fSlow179) + 250.0f) : 400.0f)) : (iSlow173 ? (400.0f - fSlow177) : 350.0f)) : (iSlow171 ? ((310.0f * fSlow174) + 350.0f) : 660.0f)) : (iSlow169 ? (660.0f - (220.0f * fSlow172)) : 440.0f)) : (iSlow167 ? (440.0f - (170.0f * fSlow170)) : 270.0f)) : (iSlow165 ? ((160.0f * fSlow168) + 270.0f) : 430.0f)) : (iSlow163 ? (430.0f - (60.0f * fSlow166)) : 370.0f)) : (iSlow161 ? ((430.0f * fSlow164) + 370.0f) : 800.0f)) : (iSlow158 ? (800.0f - (450.0f * fSlow162)) : 350.0f)) : (iSlow156 ? (350.0f - fSlow160) : 270.0f)) : (iSlow154 ? ((180.0f * fSlow157) + 270.0f) : 450.0f)) : (iSlow152 ? (450.0f - (125.0f * fSlow155)) : 325.0f)) : (iSlow150 ? (325.0f * (fSlow153 + 1.0f)) : 650.0f)) : (iSlow148 ? (650.0f - (250.0f * fSlow151)) : 400.0f)) : (iSlow146 ? (400.0f - (110.0f * fSlow149)) : 290.0f)) : (iSlow143 ? ((110.0f * fSlow147) + 290.0f) : 400.0f)) : (iSlow144 ? (400.0f - (50.0f * fSlow145)) : 350.0f));
		float fSlow193 = (20.0f * fSlow151);
		float fSlow194 = (20.0f * fSlow162);
		float fSlow195 = (10.0f * fSlow172);
		float fSlow196 = (10.0f * fSlow184);
		float fSlow197 = (20.0f * fSlow186);
		float fSlow198 = (20.0f * fSlow188);
		float fSlow199 = (20.0f * fSlow142);
		float fSlow200 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 80.0f : (iSlow189 ? (80.0f - fSlow199) : 60.0f)) : (iSlow187 ? (60.0f - (10.0f * fSlow190)) : 50.0f)) : (iSlow185 ? (fSlow198 + 50.0f) : 70.0f)) : (iSlow183 ? (70.0f - fSlow197) : 50.0f)) : (iSlow182 ? (fSlow196 + 50.0f) : 60.0f)) : (iSlow180 ? (60.0f - (20.0f * fSlow141)) : 40.0f)) : (iSlow178 ? ((20.0f * fSlow181) + 40.0f) : 60.0f)) : (iSlow175 ? (60.0f - (20.0f * fSlow179)) : 40.0f)) : 40.0f) : (iSlow171 ? (40.0f * (fSlow174 + 1.0f)) : 80.0f)) : (iSlow169 ? (80.0f - fSlow195) : 70.0f)) : (iSlow167 ? (70.0f - (30.0f * fSlow170)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow161 ? (40.0f * (fSlow164 + 1.0f)) : 80.0f)) : (iSlow158 ? (80.0f - fSlow194) : 60.0f)) : 60.0f) : (iSlow154 ? (60.0f - (20.0f * fSlow157)) : 40.0f)) : (iSlow152 ? ((10.0f * fSlow155) + 40.0f) : 50.0f)) : 50.0f) : (iSlow148 ? (fSlow193 + 50.0f) : 70.0f)) : (iSlow146 ? (70.0f - (30.0f * fSlow149)) : 40.0f)) : (iSlow143 ? ((30.0f * fSlow147) + 40.0f) : 70.0f)) : (iSlow144 ? (70.0f - (30.0f * fSlow145)) : 40.0f));
		float fSlow201 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 1150.0f : (iSlow189 ? ((450.0f * fSlow142) + 1150.0f) : 1600.0f)) : (iSlow187 ? ((100.0f * fSlow190) + 1600.0f) : 1700.0f)) : (iSlow185 ? (1700.0f - (900.0f * fSlow188)) : 800.0f)) : (iSlow183 ? (800.0f - (100.0f * fSlow186)) : 700.0f)) : (iSlow182 ? ((340.0f * fSlow184) + 700.0f) : 1040.0f)) : (iSlow180 ? ((580.0f * fSlow141) + 1040.0f) : 1620.0f)) : (iSlow178 ? ((130.0f * fSlow181) + 1620.0f) : 1750.0f)) : (iSlow175 ? (1750.0f - (1000.0f * fSlow179)) : 750.0f)) : (iSlow173 ? (750.0f - (150.0f * fSlow176)) : 600.0f)) : (iSlow171 ? ((520.0f * fSlow174) + 600.0f) : 1120.0f)) : (iSlow169 ? ((680.0f * fSlow172) + 1120.0f) : 1800.0f)) : (iSlow167 ? ((50.0f * fSlow170) + 1800.0f) : 1850.0f)) : (iSlow165 ? (1850.0f - (1030.0f * fSlow168)) : 820.0f)) : (iSlow163 ? (820.0f - (190.0f * fSlow166)) : 630.0f)) : (iSlow161 ? ((520.0f * fSlow164) + 630.0f) : 1150.0f)) : (iSlow158 ? ((850.0f * fSlow162) + 1150.0f) : 2000.0f)) : (iSlow156 ? ((140.0f * fSlow159) + 2000.0f) : 2140.0f)) : (iSlow154 ? (2140.0f - (1340.0f * fSlow157)) : 800.0f)) : (iSlow152 ? (800.0f - (100.0f * fSlow155)) : 700.0f)) : (iSlow150 ? ((380.0f * fSlow153) + 700.0f) : 1080.0f)) : (iSlow148 ? ((620.0f * fSlow151) + 1080.0f) : 1700.0f)) : (iSlow146 ? ((170.0f * fSlow149) + 1700.0f) : 1870.0f)) : (iSlow143 ? (1870.0f - (1070.0f * fSlow147)) : 800.0f)) : (iSlow144 ? (800.0f - (200.0f * fSlow145)) : 600.0f));
		int iSlow202 = (fSlow201 >= 1300.0f);
		float fSlow203 = (fSlow201 + -1300.0f);
		float fSlow204 = (10.0f * fSlow147);
		float fSlow205 = (10.0f * fSlow151);
		float fSlow206 = (10.0f * fSlow174);
		float fSlow207 = (10.0f * fSlow141);
		float fSlow208 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 90.0f : (iSlow189 ? (90.0f - (10.0f * fSlow142)) : 80.0f)) : (iSlow187 ? ((20.0f * fSlow190) + 80.0f) : 100.0f)) : (iSlow185 ? (100.0f - fSlow198) : 80.0f)) : (iSlow183 ? (80.0f - fSlow197) : 60.0f)) : (iSlow182 ? (fSlow196 + 60.0f) : 70.0f)) : (iSlow180 ? (fSlow207 + 70.0f) : 80.0f)) : (iSlow178 ? ((10.0f * fSlow181) + 80.0f) : 90.0f)) : (iSlow175 ? (90.0f - (10.0f * fSlow179)) : 80.0f)) : 80.0f) : (iSlow171 ? (fSlow206 + 80.0f) : 90.0f)) : (iSlow169 ? (90.0f - fSlow195) : 80.0f)) : (iSlow167 ? ((10.0f * fSlow170) + 80.0f) : 90.0f)) : (iSlow165 ? (90.0f - (10.0f * fSlow168)) : 80.0f)) : (iSlow163 ? (80.0f - (20.0f * fSlow166)) : 60.0f)) : (iSlow161 ? ((30.0f * fSlow164) + 60.0f) : 90.0f)) : (iSlow158 ? ((10.0f * fSlow162) + 90.0f) : 100.0f)) : (iSlow156 ? (100.0f - (10.0f * fSlow159)) : 90.0f)) : (iSlow154 ? (90.0f - (10.0f * fSlow157)) : 80.0f)) : (iSlow152 ? (80.0f - (20.0f * fSlow155)) : 60.0f)) : (iSlow150 ? ((30.0f * fSlow153) + 60.0f) : 90.0f)) : (iSlow148 ? (90.0f - fSlow205) : 80.0f)) : (iSlow146 ? ((10.0f * fSlow149) + 80.0f) : 90.0f)) : (iSlow143 ? (90.0f - fSlow204) : 80.0f)) : (iSlow144 ? (80.0f - (20.0f * fSlow145)) : 60.0f));
		float fSlow209 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 0.630957007f : (iSlow189 ? (0.630957007f - (0.567861021f * fSlow142)) : 0.0630960017f)) : (iSlow187 ? ((0.0369039997f * fSlow190) + 0.0630960017f) : 0.100000001f)) : (iSlow185 ? ((0.254812986f * fSlow188) + 0.100000001f) : 0.35481301f)) : (iSlow183 ? (0.35481301f - (0.103624001f * fSlow186)) : 0.251188993f)) : (iSlow182 ? ((0.195494995f * fSlow184) + 0.251188993f) : 0.446684003f)) : (iSlow180 ? (0.446684003f - (0.195494995f * fSlow141)) : 0.251188993f)) : (iSlow178 ? (0.251188993f - (0.219566002f * fSlow181)) : 0.0316229984f)) : (iSlow175 ? ((0.250214994f * fSlow179) + 0.0316229984f) : 0.281838f)) : (iSlow173 ? (0.281838f - (0.181838006f * fSlow176)) : 0.100000001f)) : (iSlow171 ? ((0.401187003f * fSlow174) + 0.100000001f) : 0.501187027f)) : (iSlow169 ? (0.501187027f - (0.301661015f * fSlow172)) : 0.199525997f)) : (iSlow167 ? (0.199525997f - (0.136429995f * fSlow170)) : 0.0630960017f)) : (iSlow165 ? ((0.253131986f * fSlow168) + 0.0630960017f) : 0.316228002f)) : (iSlow163 ? (0.316228002f - (0.216227993f * fSlow166)) : 0.100000001f)) : (iSlow161 ? ((0.401187003f * fSlow164) + 0.100000001f) : 0.501187027f)) : (iSlow158 ? (0.501187027f - (0.401187003f * fSlow162)) : 0.100000001f)) : (iSlow156 ? ((0.151188999f * fSlow159) + 0.100000001f) : 0.251188993f)) : (iSlow154 ? ((0.0306490008f * fSlow157) + 0.251188993f) : 0.281838f)) : (iSlow152 ? (0.281838f - (0.123349003f * fSlow155)) : 0.158489004f)) : (iSlow150 ? ((0.342698008f * fSlow153) + 0.158489004f) : 0.501187027f)) : (iSlow148 ? (0.501187027f - (0.301661015f * fSlow151)) : 0.199525997f)) : (iSlow146 ? (0.199525997f - (0.0216979999f * fSlow149)) : 0.177827999f)) : (iSlow143 ? ((0.138400003f * fSlow147) + 0.177827999f) : 0.316228002f)) : (iSlow144 ? (0.316228002f - (0.216227993f * fSlow145)) : 0.100000001f));
		float fSlow210 = (100.0f * fSlow145);
		float fSlow211 = (50.0f * fSlow153);
		float fSlow212 = (50.0f * fSlow172);
		float fSlow213 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 2800.0f : (iSlow189 ? (2800.0f - (100.0f * fSlow142)) : 2700.0f)) : 2700.0f) : (iSlow185 ? ((130.0f * fSlow188) + 2700.0f) : 2830.0f)) : (iSlow183 ? (2830.0f - (300.0f * fSlow186)) : 2530.0f)) : (iSlow182 ? (2530.0f - (280.0f * fSlow184)) : 2250.0f)) : (iSlow180 ? ((150.0f * fSlow141) + 2250.0f) : 2400.0f)) : (iSlow178 ? ((200.0f * fSlow181) + 2400.0f) : 2600.0f)) : (iSlow175 ? (2600.0f - (200.0f * fSlow179)) : 2400.0f)) : 2400.0f) : (iSlow171 ? ((350.0f * fSlow174) + 2400.0f) : 2750.0f)) : (iSlow169 ? (2750.0f - fSlow212) : 2700.0f)) : (iSlow167 ? ((200.0f * fSlow170) + 2700.0f) : 2900.0f)) : (iSlow165 ? (2900.0f - (200.0f * fSlow168)) : 2700.0f)) : (iSlow163 ? ((50.0f * fSlow166) + 2700.0f) : 2750.0f)) : (iSlow161 ? ((150.0f * fSlow164) + 2750.0f) : 2900.0f)) : (iSlow158 ? (2900.0f - (100.0f * fSlow162)) : 2800.0f)) : (iSlow156 ? ((150.0f * fSlow159) + 2800.0f) : 2950.0f)) : (iSlow154 ? (2950.0f - (120.0f * fSlow157)) : 2830.0f)) : (iSlow152 ? (2830.0f - (130.0f * fSlow155)) : 2700.0f)) : (iSlow150 ? (2700.0f - fSlow211) : 2650.0f)) : (iSlow148 ? (2650.0f - (50.0f * fSlow151)) : 2600.0f)) : (iSlow146 ? ((200.0f * fSlow149) + 2600.0f) : 2800.0f)) : (iSlow143 ? (2800.0f - (200.0f * fSlow147)) : 2600.0f)) : (iSlow144 ? (fSlow210 + 2600.0f) : 2700.0f));
		float fSlow214 = std::tan((fConst18 * fSlow213));
		float fSlow215 = (1.0f / fSlow214);
		float fSlow216 = (20.0f * fSlow164);
		float fSlow217 = (20.0f * fSlow172);
		float fSlow218 = (20.0f * fSlow174);
		float fSlow219 = (60.0f * fSlow184);
		float fSlow220 = ((iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? 120.0f : (iSlow185 ? (120.0f - fSlow198) : 100.0f)) : (iSlow183 ? ((70.0f * fSlow186) + 100.0f) : 170.0f)) : (iSlow182 ? (170.0f - fSlow219) : 110.0f)) : (iSlow180 ? (110.0f - fSlow207) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow171 ? (fSlow218 + 100.0f) : 120.0f)) : (iSlow169 ? (120.0f - fSlow217) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow161 ? (fSlow216 + 100.0f) : 120.0f)) : 120.0f) : (iSlow156 ? (120.0f - (20.0f * fSlow159)) : 100.0f)) : 100.0f) : (iSlow152 ? ((70.0f * fSlow155) + 100.0f) : 170.0f)) : (iSlow150 ? (170.0f - fSlow211) : 120.0f)) : (iSlow148 ? (120.0f - fSlow193) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow213);
		float fSlow221 = (((fSlow215 - fSlow220) / fSlow214) + 1.0f);
		float fSlow222 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow214)));
		float fSlow223 = (((fSlow215 + fSlow220) / fSlow214) + 1.0f);
		float fSlow224 = (iSlow154 ? ((0.0293140002f * fSlow157) + 0.0501190014f) : 0.0794330016f);
		float fSlow225 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 0.100000001f : (iSlow189 ? (0.100000001f - (0.0683770031f * fSlow142)) : 0.0316229984f)) : 0.0316229984f) : (iSlow185 ? ((0.126865998f * fSlow188) + 0.0316229984f) : 0.158489004f)) : (iSlow183 ? (0.158489004f - (0.126865998f * fSlow186)) : 0.0316229984f)) : (iSlow182 ? ((0.323190004f * fSlow184) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow178 ? (0.35481301f - (0.196324006f * fSlow181)) : 0.158489004f)) : (iSlow175 ? (0.158489004f - (0.0693639964f * fSlow179)) : 0.0891249999f)) : (iSlow173 ? (0.0891249999f - (0.0640060008f * fSlow176)) : 0.0251189992f)) : (iSlow171 ? ((0.0456760004f * fSlow174) + 0.0251189992f) : 0.0707949996f)) : (iSlow169 ? ((0.0550980009f * fSlow172) + 0.0707949996f) : 0.125892997f)) : (iSlow167 ? (0.125892997f - (0.0627970025f * fSlow170)) : 0.0630960017f)) : (iSlow165 ? (0.0630960017f - (0.0129770003f * fSlow168)) : 0.0501190014f)) : (iSlow163 ? ((0.020676f * fSlow166) + 0.0501190014f) : 0.0707949996f)) : (iSlow161 ? (0.0707949996f - (0.0456760004f * fSlow164)) : 0.0251189992f)) : (iSlow158 ? ((0.152709007f * fSlow162) + 0.0251189992f) : 0.177827999f)) : (iSlow156 ? (0.177827999f - (0.127709001f * fSlow159)) : 0.0501190014f)) : fSlow224) : (iSlow152 ? (0.0794330016f - (0.0616500005f * fSlow155)) : 0.0177829992f)) : (iSlow150 ? ((0.428900987f * fSlow153) + 0.0177829992f) : 0.446684003f)) : (iSlow148 ? (0.446684003f - (0.195494995f * fSlow151)) : 0.251188993f)) : (iSlow146 ? (0.251188993f - (0.125295997f * fSlow149)) : 0.125892997f)) : (iSlow143 ? ((0.125295997f * fSlow147) + 0.125892997f) : 0.251188993f)) : (iSlow144 ? (0.251188993f - (0.109935001f * fSlow145)) : 0.141253993f));
		float fSlow226 = (fSlow214 * fSlow223);
		float fSlow227 = (350.0f * fSlow141);
		float fSlow228 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 3500.0f : (iSlow189 ? (3500.0f - (200.0f * fSlow142)) : 3300.0f)) : (iSlow187 ? ((400.0f * fSlow190) + 3300.0f) : 3700.0f)) : (iSlow185 ? (3700.0f - (200.0f * fSlow188)) : 3500.0f)) : 3500.0f) : (iSlow182 ? (3500.0f - (1050.0f * fSlow184)) : 2450.0f)) : (iSlow180 ? (fSlow227 + 2450.0f) : 2800.0f)) : (iSlow178 ? ((250.0f * fSlow181) + 2800.0f) : 3050.0f)) : (iSlow175 ? (3050.0f - (450.0f * fSlow179)) : 2600.0f)) : (iSlow173 ? ((75.0f * fSlow176) + 2600.0f) : 2675.0f)) : (iSlow171 ? ((325.0f * fSlow174) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow167 ? ((350.0f * fSlow170) + 3000.0f) : 3350.0f)) : (iSlow165 ? (3350.0f - (350.0f * fSlow168)) : 3000.0f)) : 3000.0f) : (iSlow161 ? ((900.0f * fSlow164) + 3000.0f) : 3900.0f)) : (iSlow158 ? (3900.0f - (300.0f * fSlow162)) : 3600.0f)) : (iSlow156 ? ((300.0f * fSlow159) + 3600.0f) : 3900.0f)) : (iSlow154 ? (3900.0f - (100.0f * fSlow157)) : 3800.0f)) : 3800.0f) : (iSlow150 ? (3800.0f - (900.0f * fSlow153)) : 2900.0f)) : (iSlow148 ? ((300.0f * fSlow151) + 2900.0f) : 3200.0f)) : (iSlow146 ? ((50.0f * fSlow149) + 3200.0f) : 3250.0f)) : (iSlow143 ? (3250.0f - (450.0f * fSlow147)) : 2800.0f)) : (iSlow144 ? (fSlow210 + 2800.0f) : 2900.0f));
		float fSlow229 = std::tan((fConst18 * fSlow228));
		float fSlow230 = (1.0f / fSlow229);
		float fSlow231 = ((iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 130.0f : (iSlow189 ? (fSlow199 + 130.0f) : 150.0f)) : 150.0f) : (iSlow185 ? (150.0f - fSlow198) : 130.0f)) : (iSlow183 ? ((50.0f * fSlow186) + 130.0f) : 180.0f)) : (iSlow182 ? (180.0f - fSlow219) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow171 ? (fSlow206 + 120.0f) : 130.0f)) : (iSlow169 ? (130.0f - fSlow195) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow161 ? ((10.0f * fSlow164) + 120.0f) : 130.0f)) : (iSlow158 ? (fSlow194 + 130.0f) : 150.0f)) : (iSlow156 ? (150.0f - (30.0f * fSlow159)) : 120.0f)) : 120.0f) : (iSlow152 ? ((60.0f * fSlow155) + 120.0f) : 180.0f)) : (iSlow150 ? (180.0f - fSlow211) : 130.0f)) : (iSlow148 ? (130.0f - fSlow205) : 120.0f)) : 120.0f) : (iSlow143 ? (fSlow204 + 120.0f) : 130.0f)) : (iSlow144 ? (130.0f - (10.0f * fSlow145)) : 120.0f)) / fSlow228);
		float fSlow232 = (((fSlow230 - fSlow231) / fSlow229) + 1.0f);
		float fSlow233 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow229)));
		float fSlow234 = (((fSlow230 + fSlow231) / fSlow229) + 1.0f);
		float fSlow235 = (iSlow167 ? (0.100000001f - (0.0841509998f * fSlow170)) : 0.0158489998f);
		float fSlow236 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 0.0158489998f : (iSlow189 ? ((0.00193400006f * fSlow142) + 0.0158489998f) : 0.0177829992f)) : (iSlow187 ? (0.0177829992f - (0.00193400006f * fSlow190)) : 0.0158489998f)) : (iSlow185 ? ((0.0239620004f * fSlow188) + 0.0158489998f) : 0.0398110002f)) : (iSlow183 ? (0.0398110002f - (0.0298110005f * fSlow186)) : 0.00999999978f)) : (iSlow182 ? ((0.344812989f * fSlow184) + 0.00999999978f) : 0.35481301f)) : (iSlow180 ? (0.35481301f - (0.103624001f * fSlow141)) : 0.251188993f)) : (iSlow178 ? (0.251188993f - (0.171755999f * fSlow181)) : 0.0794330016f)) : (iSlow175 ? ((0.0205669999f * fSlow179) + 0.0794330016f) : 0.100000001f)) : (iSlow173 ? (0.100000001f - (0.0601890013f * fSlow176)) : 0.0398110002f)) : (iSlow171 ? ((0.0232849997f * fSlow174) + 0.0398110002f) : 0.0630960017f)) : (iSlow169 ? ((0.0369039997f * fSlow172) + 0.0630960017f) : 0.100000001f)) : fSlow235) : (iSlow165 ? ((0.0635839999f * fSlow168) + 0.0158489998f) : 0.0794330016f)) : (iSlow163 ? (0.0794330016f - (0.0478099994f * fSlow166)) : 0.0316229984f)) : (iSlow161 ? ((0.0683770031f * fSlow164) + 0.0316229984f) : 0.100000001f)) : (iSlow158 ? (0.100000001f - (0.0900000036f * fSlow162)) : 0.00999999978f)) : (iSlow156 ? ((0.0401189998f * fSlow159) + 0.00999999978f) : 0.0501190014f)) : fSlow224) : (iSlow152 ? (0.0794330016f - (0.0694330037f * fSlow155)) : 0.00999999978f)) : (iSlow150 ? ((0.388107002f * fSlow153) + 0.00999999978f) : 0.398106992f)) : (iSlow148 ? (0.398106992f - (0.198580995f * fSlow151)) : 0.199525997f)) : (iSlow146 ? (0.199525997f - (0.099526003f * fSlow149)) : 0.100000001f)) : (iSlow143 ? ((0.151188999f * fSlow147) + 0.100000001f) : 0.251188993f)) : (iSlow144 ? (0.251188993f - (0.0516630001f * fSlow145)) : 0.199525997f));
		float fSlow237 = (fSlow229 * fSlow234);
		float fSlow238 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? 4950.0f : (iSlow182 ? (4950.0f - (2200.0f * fSlow184)) : 2750.0f)) : (iSlow180 ? (fSlow227 + 2750.0f) : 3100.0f)) : (iSlow178 ? ((240.0f * fSlow181) + 3100.0f) : 3340.0f)) : (iSlow175 ? (3340.0f - (440.0f * fSlow179)) : 2900.0f)) : (iSlow173 ? (fSlow177 + 2900.0f) : 2950.0f)) : (iSlow171 ? ((400.0f * fSlow174) + 2950.0f) : 3350.0f)) : (iSlow169 ? (3350.0f - fSlow212) : 3300.0f)) : (iSlow167 ? ((290.0f * fSlow170) + 3300.0f) : 3590.0f)) : (iSlow165 ? (3590.0f - (290.0f * fSlow168)) : 3300.0f)) : (iSlow163 ? ((100.0f * fSlow166) + 3300.0f) : 3400.0f)) : (iSlow161 ? ((1550.0f * fSlow164) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow150 ? (4950.0f - (1700.0f * fSlow153)) : 3250.0f)) : (iSlow148 ? ((330.0f * fSlow151) + 3250.0f) : 3580.0f)) : (iSlow146 ? (3580.0f - (40.0f * fSlow149)) : 3540.0f)) : (iSlow143 ? (3540.0f - (540.0f * fSlow147)) : 3000.0f)) : (iSlow144 ? ((300.0f * fSlow145) + 3000.0f) : 3300.0f));
		float fSlow239 = std::tan((fConst18 * fSlow238));
		float fSlow240 = (1.0f / fSlow239);
		float fSlow241 = ((iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? (iSlow189 ? (iSlow191 ? 140.0f : (iSlow189 ? ((60.0f * fSlow142) + 140.0f) : 200.0f)) : 200.0f) : (iSlow185 ? (200.0f - (65.0f * fSlow188)) : 135.0f)) : (iSlow183 ? ((65.0f * fSlow186) + 135.0f) : 200.0f)) : (iSlow182 ? (200.0f - (70.0f * fSlow184)) : 130.0f)) : (iSlow180 ? (130.0f - fSlow207) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow171 ? (fSlow218 + 120.0f) : 140.0f)) : (iSlow169 ? (140.0f - fSlow217) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow161 ? (fSlow216 + 120.0f) : 140.0f)) : (iSlow158 ? ((60.0f * fSlow162) + 140.0f) : 200.0f)) : (iSlow156 ? (200.0f - fSlow160) : 120.0f)) : 120.0f) : (iSlow152 ? ((80.0f * fSlow155) + 120.0f) : 200.0f)) : (iSlow150 ? (200.0f - (60.0f * fSlow153)) : 140.0f)) : (iSlow148 ? (140.0f - fSlow193) : 120.0f)) : 120.0f) : (iSlow143 ? ((15.0f * fSlow147) + 120.0f) : 135.0f)) : (iSlow144 ? (135.0f - (15.0f * fSlow145)) : 120.0f)) / fSlow238);
		float fSlow242 = (((fSlow240 - fSlow241) / fSlow239) + 1.0f);
		float fSlow243 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow239)));
		float fSlow244 = (((fSlow240 + fSlow241) / fSlow239) + 1.0f);
		float fSlow245 = (iSlow143 ? (iSlow146 ? (iSlow148 ? (iSlow150 ? (iSlow152 ? (iSlow154 ? (iSlow156 ? (iSlow158 ? (iSlow161 ? (iSlow163 ? (iSlow165 ? (iSlow167 ? (iSlow169 ? (iSlow171 ? (iSlow173 ? (iSlow175 ? (iSlow178 ? (iSlow180 ? (iSlow182 ? (iSlow183 ? (iSlow185 ? (iSlow187 ? 0.00100000005f : (iSlow185 ? ((0.000777999987f * fSlow188) + 0.00100000005f) : 0.00177800003f)) : (iSlow183 ? (0.00177800003f - (0.001147f * fSlow186)) : 0.000630999974f)) : (iSlow182 ? ((0.0993689969f * fSlow184) + 0.000630999974f) : 0.100000001f)) : (iSlow180 ? ((0.0258930009f * fSlow141) + 0.100000001f) : 0.125892997f)) : (iSlow178 ? (0.125892997f - (0.0860819966f * fSlow181)) : 0.0398110002f)) : (iSlow175 ? (0.0398110002f - (0.0298110005f * fSlow179)) : 0.00999999978f)) : (iSlow173 ? ((0.00584900007f * fSlow176) + 0.00999999978f) : 0.0158489998f)) : (iSlow171 ? (0.0158489998f - (0.00325999991f * fSlow174)) : 0.0125890002f)) : (iSlow169 ? ((0.0874110013f * fSlow172) + 0.0125890002f) : 0.100000001f)) : fSlow235) : (iSlow165 ? ((0.00410400005f * fSlow168) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow161 ? (0.0199529994f - (0.0167909991f * fSlow164)) : 0.0031620001f)) : (iSlow158 ? (0.0031620001f - (0.00157700002f * fSlow162)) : 0.00158499996f)) : (iSlow156 ? ((0.00472499989f * fSlow159) + 0.00158499996f) : 0.00631000008f)) : (iSlow154 ? (0.00631000008f - (0.00314799999f * fSlow157)) : 0.0031620001f)) : (iSlow152 ? (0.0031620001f - (0.00216199993f * fSlow155)) : 0.00100000005f)) : (iSlow150 ? ((0.0784329996f * fSlow153) + 0.00100000005f) : 0.0794330016f)) : (iSlow148 ? ((0.0205669999f * fSlow151) + 0.0794330016f) : 0.100000001f)) : (iSlow146 ? (0.100000001f - (0.0683770031f * fSlow149)) : 0.0316229984f)) : (iSlow143 ? ((0.0184959993f * fSlow147) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow246 = (fSlow239 * fSlow244);
		float fSlow247 = (fConst31 * ((1.0f / float(fHslider49)) + -1.0f));
		float fSlow248 = float(fHslider50);
		int iSlow249 = (std::fabs(fSlow248) < 1.1920929e-07f);
		float fSlow250 = (iSlow249 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow249 ? 1.0f : fSlow248)))));
		float fSlow251 = float(fHslider51);
		int iSlow252 = (std::fabs(fSlow251) < 1.1920929e-07f);
		float fSlow253 = (iSlow252 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow252 ? 1.0f : fSlow251)))));
		float fSlow254 = (1.0f - fSlow253);
		float fSlow255 = (1.0f - fSlow250);
		float fSlow256 = float(fHslider52);
		float fSlow257 = (1.0f / float(fHslider53));
		float fSlow258 = std::tan((fConst18 * float(fHslider54)));
		float fSlow259 = (1.0f / fSlow258);
		float fSlow260 = (1.0f / (((fSlow257 + fSlow259) / fSlow258) + 1.0f));
		float fSlow261 = (((fSlow259 - fSlow257) / fSlow258) + 1.0f);
		float fSlow262 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow258))));
		float fSlow263 = (1.0f / fSlow137);
		float fSlow264 = float(fHslider55);
		int iSlow265 = (fSlow264 > 0.0f);
		float fSlow266 = (float(fHslider56) * std::sin((fConst32 * fSlow136)));
		float fSlow267 = (fConst18 * ((fSlow136 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow264)))) / fSlow266));
		float fSlow268 = (fConst18 * (fSlow136 / fSlow266));
		float fSlow269 = (iSlow265 ? fSlow268 : fSlow267);
		float fSlow270 = ((fSlow263 * (fSlow263 - fSlow269)) + 1.0f);
		float fSlow271 = ((fSlow263 * (fSlow263 + fSlow269)) + 1.0f);
		float fSlow272 = (iSlow265 ? fSlow267 : fSlow268);
		float fSlow273 = ((fSlow263 * (fSlow263 + fSlow272)) + 1.0f);
		float fSlow274 = ((fSlow263 * (fSlow263 - fSlow272)) + 1.0f);
		float fSlow275 = (1.0f / fSlow134);
		float fSlow276 = float(fHslider57);
		int iSlow277 = (fSlow276 > 0.0f);
		float fSlow278 = (float(fHslider58) * std::sin((fConst32 * fSlow133)));
		float fSlow279 = (fConst18 * ((fSlow133 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow276)))) / fSlow278));
		float fSlow280 = (fConst18 * (fSlow133 / fSlow278));
		float fSlow281 = (iSlow277 ? fSlow280 : fSlow279);
		float fSlow282 = ((fSlow275 * (fSlow275 - fSlow281)) + 1.0f);
		float fSlow283 = ((fSlow275 * (fSlow275 + fSlow281)) + 1.0f);
		float fSlow284 = (iSlow277 ? fSlow279 : fSlow280);
		float fSlow285 = ((fSlow275 * (fSlow275 + fSlow284)) + 1.0f);
		float fSlow286 = ((fSlow275 * (fSlow275 - fSlow284)) + 1.0f);
		float fSlow287 = (1.0f / fSlow131);
		float fSlow288 = (((fSlow130 - fSlow128) / fSlow129) + 1.0f);
		float fSlow289 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow129))));
		float fSlow290 = (1.0f / float(fHslider60));
		float fSlow291 = std::tan((fConst18 * float(fHslider61)));
		float fSlow292 = (1.0f / fSlow291);
		float fSlow293 = (((fSlow290 + fSlow292) / fSlow291) + 1.0f);
		float fSlow294 = ((std::pow(10.0f, (0.0500000007f * float(fHslider59))) * (1.0f - float(fCheckbox3))) / fSlow293);
		float fSlow295 = float(fHslider62);
		float fSlow296 = std::tan((fConst18 * fSlow295));
		float fSlow297 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow296))));
		float fSlow298 = float(fHslider63);
		float fSlow299 = std::tan((fConst18 * fSlow298));
		float fSlow300 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow299))));
		float fSlow301 = (fConst50 * float(fHslider64));
		float fSlow302 = float(fHslider65);
		float fSlow303 = float((fSlow302 > 0.0f));
		float fSlow304 = (0.0f - fSlow302);
		float fSlow305 = float(fHslider66);
		float fSlow306 = (0.400000006f * (float(fHslider67) + -0.5f));
		float fSlow307 = (fConst58 * (fSlow306 + 0.270000011f));
		float fSlow308 = (fConst58 * (0.730000019f - fSlow306));
		float fSlow309 = (fConst31 * ((1.0f / float(fHslider68)) + -1.0f));
		float fSlow310 = float(fHslider69);
		int iSlow311 = (std::fabs(fSlow310) < 1.1920929e-07f);
		float fSlow312 = (iSlow311 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow311 ? 1.0f : fSlow310)))));
		float fSlow313 = float(fHslider70);
		int iSlow314 = (std::fabs(fSlow313) < 1.1920929e-07f);
		float fSlow315 = (iSlow314 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow314 ? 1.0f : fSlow313)))));
		float fSlow316 = (1.0f - fSlow315);
		float fSlow317 = (1.0f - fSlow312);
		float fSlow318 = float(fHslider71);
		float fSlow319 = (1.0f / float(fHslider72));
		float fSlow320 = std::tan((fConst18 * float(fHslider73)));
		float fSlow321 = (1.0f / fSlow320);
		float fSlow322 = (1.0f / (((fSlow319 + fSlow321) / fSlow320) + 1.0f));
		float fSlow323 = (((fSlow321 - fSlow319) / fSlow320) + 1.0f);
		float fSlow324 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow320))));
		float fSlow325 = (1.0f / fSlow299);
		float fSlow326 = float(fHslider74);
		int iSlow327 = (fSlow326 > 0.0f);
		float fSlow328 = (float(fHslider75) * std::sin((fConst32 * fSlow298)));
		float fSlow329 = (fConst18 * ((fSlow298 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow326)))) / fSlow328));
		float fSlow330 = (fConst18 * (fSlow298 / fSlow328));
		float fSlow331 = (iSlow327 ? fSlow330 : fSlow329);
		float fSlow332 = ((fSlow325 * (fSlow325 - fSlow331)) + 1.0f);
		float fSlow333 = ((fSlow325 * (fSlow325 + fSlow331)) + 1.0f);
		float fSlow334 = (iSlow327 ? fSlow329 : fSlow330);
		float fSlow335 = ((fSlow325 * (fSlow325 + fSlow334)) + 1.0f);
		float fSlow336 = ((fSlow325 * (fSlow325 - fSlow334)) + 1.0f);
		float fSlow337 = (1.0f / fSlow296);
		float fSlow338 = float(fHslider76);
		int iSlow339 = (fSlow338 > 0.0f);
		float fSlow340 = (float(fHslider77) * std::sin((fConst32 * fSlow295)));
		float fSlow341 = (fConst18 * ((fSlow295 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow338)))) / fSlow340));
		float fSlow342 = (fConst18 * (fSlow295 / fSlow340));
		float fSlow343 = (iSlow339 ? fSlow342 : fSlow341);
		float fSlow344 = ((fSlow337 * (fSlow337 - fSlow343)) + 1.0f);
		float fSlow345 = ((fSlow337 * (fSlow337 + fSlow343)) + 1.0f);
		float fSlow346 = (iSlow339 ? fSlow341 : fSlow342);
		float fSlow347 = ((fSlow337 * (fSlow337 + fSlow346)) + 1.0f);
		float fSlow348 = ((fSlow337 * (fSlow337 - fSlow346)) + 1.0f);
		float fSlow349 = (1.0f / fSlow293);
		float fSlow350 = (((fSlow292 - fSlow290) / fSlow291) + 1.0f);
		float fSlow351 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow291))));
		float fSlow352 = (1.0f / float(fHslider79));
		float fSlow353 = std::tan((fConst18 * float(fHslider80)));
		float fSlow354 = (1.0f / fSlow353);
		float fSlow355 = (((fSlow352 + fSlow354) / fSlow353) + 1.0f);
		float fSlow356 = ((std::pow(10.0f, (0.0500000007f * float(fHslider78))) * (1.0f - float(fCheckbox4))) / fSlow355);
		float fSlow357 = float(fHslider81);
		float fSlow358 = std::tan((fConst18 * fSlow357));
		float fSlow359 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow358))));
		float fSlow360 = float(fHslider82);
		float fSlow361 = std::tan((fConst18 * fSlow360));
		float fSlow362 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow361))));
		float fSlow363 = std::min<float>(5.0f, std::max<float>(0.00999999978f, float(fHslider83)));
		float fSlow364 = (0.0125000002f * (std::pow(10.0f, (0.0500000007f * fSlow363)) + 1.0f));
		float fSlow365 = ((2.0f * fSlow363) + 1.0f);
		float fSlow366 = std::tan((fConst18 * ((1875.0f * fSlow365) + 500.0f)));
		float fSlow367 = (1.0f / fSlow366);
		float fSlow368 = (1.0f / (((fSlow367 + 1.41421354f) / fSlow366) + 1.0f));
		float fSlow369 = std::tan((fConst18 * ((62.5f * fSlow365) + 40.0f)));
		float fSlow370 = (1.0f / fSlow369);
		float fSlow371 = (1.0f / (((fSlow370 + 1.41421354f) / fSlow369) + 1.0f));
		float fSlow372 = mydsp_faustpower2_f(fSlow369);
		float fSlow373 = (1.0f / fSlow372);
		float fSlow374 = (((fSlow370 + -1.41421354f) / fSlow369) + 1.0f);
		float fSlow375 = (2.0f * (1.0f - fSlow373));
		float fSlow376 = (0.0f - (2.0f / fSlow372));
		float fSlow377 = (((fSlow367 + -1.41421354f) / fSlow366) + 1.0f);
		float fSlow378 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow366))));
		float fSlow379 = (1.0f / std::max<float>(1.0f, (fConst166 * fSlow365)));
		float fSlow380 = float(fHslider84);
		float fSlow381 = float((fSlow380 > 0.0f));
		float fSlow382 = (0.0f - fSlow380);
		float fSlow383 = (fConst31 * ((1.0f / float(fHslider85)) + -1.0f));
		float fSlow384 = float(fHslider86);
		int iSlow385 = (std::fabs(fSlow384) < 1.1920929e-07f);
		float fSlow386 = (iSlow385 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow385 ? 1.0f : fSlow384)))));
		float fSlow387 = float(fHslider87);
		int iSlow388 = (std::fabs(fSlow387) < 1.1920929e-07f);
		float fSlow389 = (iSlow388 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow388 ? 1.0f : fSlow387)))));
		float fSlow390 = (1.0f - fSlow389);
		float fSlow391 = (1.0f - fSlow386);
		float fSlow392 = float(fHslider88);
		float fSlow393 = (1.0f / float(fHslider89));
		float fSlow394 = std::tan((fConst18 * float(fHslider90)));
		float fSlow395 = (1.0f / fSlow394);
		float fSlow396 = (1.0f / (((fSlow393 + fSlow395) / fSlow394) + 1.0f));
		float fSlow397 = (((fSlow395 - fSlow393) / fSlow394) + 1.0f);
		float fSlow398 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow394))));
		float fSlow399 = (1.0f / fSlow361);
		float fSlow400 = float(fHslider91);
		int iSlow401 = (fSlow400 > 0.0f);
		float fSlow402 = (float(fHslider92) * std::sin((fConst32 * fSlow360)));
		float fSlow403 = (fConst18 * ((fSlow360 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow400)))) / fSlow402));
		float fSlow404 = (fConst18 * (fSlow360 / fSlow402));
		float fSlow405 = (iSlow401 ? fSlow404 : fSlow403);
		float fSlow406 = ((fSlow399 * (fSlow399 - fSlow405)) + 1.0f);
		float fSlow407 = ((fSlow399 * (fSlow399 + fSlow405)) + 1.0f);
		float fSlow408 = (iSlow401 ? fSlow403 : fSlow404);
		float fSlow409 = ((fSlow399 * (fSlow399 + fSlow408)) + 1.0f);
		float fSlow410 = ((fSlow399 * (fSlow399 - fSlow408)) + 1.0f);
		float fSlow411 = (1.0f / fSlow358);
		float fSlow412 = float(fHslider93);
		int iSlow413 = (fSlow412 > 0.0f);
		float fSlow414 = (float(fHslider94) * std::sin((fConst32 * fSlow357)));
		float fSlow415 = (fConst18 * ((fSlow357 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow412)))) / fSlow414));
		float fSlow416 = (fConst18 * (fSlow357 / fSlow414));
		float fSlow417 = (iSlow413 ? fSlow416 : fSlow415);
		float fSlow418 = ((fSlow411 * (fSlow411 - fSlow417)) + 1.0f);
		float fSlow419 = ((fSlow411 * (fSlow411 + fSlow417)) + 1.0f);
		float fSlow420 = (iSlow413 ? fSlow415 : fSlow416);
		float fSlow421 = ((fSlow411 * (fSlow411 + fSlow420)) + 1.0f);
		float fSlow422 = ((fSlow411 * (fSlow411 - fSlow420)) + 1.0f);
		float fSlow423 = (1.0f / fSlow355);
		float fSlow424 = (((fSlow354 - fSlow352) / fSlow353) + 1.0f);
		float fSlow425 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow353))));
		float fSlow426 = (1.0f / float(fHslider96));
		float fSlow427 = std::tan((fConst18 * float(fHslider97)));
		float fSlow428 = (1.0f / fSlow427);
		float fSlow429 = (((fSlow426 + fSlow428) / fSlow427) + 1.0f);
		float fSlow430 = ((std::pow(10.0f, (0.0500000007f * float(fHslider95))) * (1.0f - float(fCheckbox5))) / fSlow429);
		float fSlow431 = float(fHslider98);
		float fSlow432 = std::tan((fConst18 * fSlow431));
		float fSlow433 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow432))));
		float fSlow434 = float(fHslider99);
		float fSlow435 = std::tan((fConst18 * fSlow434));
		float fSlow436 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow435))));
		float fSlow437 = (0.5f * fSlow15);
		float fSlow438 = (fConst228 * fSlow16);
		float fSlow439 = (27.0f * fSlow16);
		float fSlow440 = (9.0f * fSlow16);
		float fSlow441 = (3.0f * fSlow16);
		float fSlow442 = (fConst31 * ((1.0f / float(fHslider100)) + -1.0f));
		float fSlow443 = float(fHslider101);
		int iSlow444 = (std::fabs(fSlow443) < 1.1920929e-07f);
		float fSlow445 = (iSlow444 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow444 ? 1.0f : fSlow443)))));
		float fSlow446 = float(fHslider102);
		int iSlow447 = (std::fabs(fSlow446) < 1.1920929e-07f);
		float fSlow448 = (iSlow447 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow447 ? 1.0f : fSlow446)))));
		float fSlow449 = (1.0f - fSlow448);
		float fSlow450 = (1.0f - fSlow445);
		float fSlow451 = float(fHslider103);
		float fSlow452 = (1.0f / float(fHslider104));
		float fSlow453 = std::tan((fConst18 * float(fHslider105)));
		float fSlow454 = (1.0f / fSlow453);
		float fSlow455 = (1.0f / (((fSlow452 + fSlow454) / fSlow453) + 1.0f));
		float fSlow456 = (((fSlow454 - fSlow452) / fSlow453) + 1.0f);
		float fSlow457 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow453))));
		float fSlow458 = (1.0f / fSlow435);
		float fSlow459 = float(fHslider106);
		int iSlow460 = (fSlow459 > 0.0f);
		float fSlow461 = (float(fHslider107) * std::sin((fConst32 * fSlow434)));
		float fSlow462 = (fConst18 * ((fSlow434 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow459)))) / fSlow461));
		float fSlow463 = (fConst18 * (fSlow434 / fSlow461));
		float fSlow464 = (iSlow460 ? fSlow463 : fSlow462);
		float fSlow465 = ((fSlow458 * (fSlow458 - fSlow464)) + 1.0f);
		float fSlow466 = ((fSlow458 * (fSlow458 + fSlow464)) + 1.0f);
		float fSlow467 = (iSlow460 ? fSlow462 : fSlow463);
		float fSlow468 = ((fSlow458 * (fSlow458 + fSlow467)) + 1.0f);
		float fSlow469 = ((fSlow458 * (fSlow458 - fSlow467)) + 1.0f);
		float fSlow470 = (1.0f / fSlow432);
		float fSlow471 = float(fHslider108);
		int iSlow472 = (fSlow471 > 0.0f);
		float fSlow473 = (float(fHslider109) * std::sin((fConst32 * fSlow431)));
		float fSlow474 = (fConst18 * ((fSlow431 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow471)))) / fSlow473));
		float fSlow475 = (fConst18 * (fSlow431 / fSlow473));
		float fSlow476 = (iSlow472 ? fSlow475 : fSlow474);
		float fSlow477 = ((fSlow470 * (fSlow470 - fSlow476)) + 1.0f);
		float fSlow478 = ((fSlow470 * (fSlow470 + fSlow476)) + 1.0f);
		float fSlow479 = (iSlow472 ? fSlow474 : fSlow475);
		float fSlow480 = ((fSlow470 * (fSlow470 + fSlow479)) + 1.0f);
		float fSlow481 = ((fSlow470 * (fSlow470 - fSlow479)) + 1.0f);
		float fSlow482 = (1.0f / fSlow429);
		float fSlow483 = (((fSlow428 - fSlow426) / fSlow427) + 1.0f);
		float fSlow484 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow427))));
		float fSlow485 = (1.0f / float(fHslider111));
		float fSlow486 = std::tan((fConst18 * float(fHslider112)));
		float fSlow487 = (1.0f / fSlow486);
		float fSlow488 = (((fSlow485 + fSlow487) / fSlow486) + 1.0f);
		float fSlow489 = ((std::pow(10.0f, (0.0500000007f * float(fHslider110))) * (1.0f - float(fCheckbox6))) / fSlow488);
		float fSlow490 = float(fHslider113);
		float fSlow491 = std::tan((fConst18 * fSlow490));
		float fSlow492 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow491))));
		float fSlow493 = float(fHslider114);
		float fSlow494 = std::tan((fConst18 * fSlow493));
		float fSlow495 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow494))));
		float fSlow496 = float(fHslider115);
		float fSlow497 = float((fSlow496 > 0.0f));
		float fSlow498 = (0.0f - fSlow496);
		float fSlow499 = float(fHslider116);
		float fSlow500 = (0.00100000005f * float((fSlow499 > 310.0f)));
		float fSlow501 = (fConst29 * std::max<float>(20.0f, fSlow499));
		float fSlow502 = float(fHslider117);
		float fSlow503 = (0.00100000005f * float((fSlow502 > 610.0f)));
		float fSlow504 = (fConst29 * std::max<float>(20.0f, fSlow502));
		float fSlow505 = float(fHslider118);
		float fSlow506 = (0.00100000005f * float((fSlow505 > 1210.0f)));
		float fSlow507 = (fConst29 * std::max<float>(20.0f, fSlow505));
		float fSlow508 = (fConst31 * ((1.0f / float(fHslider119)) + -1.0f));
		float fSlow509 = float(fHslider120);
		int iSlow510 = (std::fabs(fSlow509) < 1.1920929e-07f);
		float fSlow511 = (iSlow510 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow510 ? 1.0f : fSlow509)))));
		float fSlow512 = float(fHslider121);
		int iSlow513 = (std::fabs(fSlow512) < 1.1920929e-07f);
		float fSlow514 = (iSlow513 ? 0.0f : std::exp((0.0f - (fConst29 / (iSlow513 ? 1.0f : fSlow512)))));
		float fSlow515 = (1.0f - fSlow514);
		float fSlow516 = (1.0f - fSlow511);
		float fSlow517 = float(fHslider122);
		float fSlow518 = (1.0f / float(fHslider123));
		float fSlow519 = std::tan((fConst18 * float(fHslider124)));
		float fSlow520 = (1.0f / fSlow519);
		float fSlow521 = (1.0f / (((fSlow518 + fSlow520) / fSlow519) + 1.0f));
		float fSlow522 = (((fSlow520 - fSlow518) / fSlow519) + 1.0f);
		float fSlow523 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow519))));
		float fSlow524 = (1.0f / fSlow494);
		float fSlow525 = float(fHslider125);
		int iSlow526 = (fSlow525 > 0.0f);
		float fSlow527 = (float(fHslider126) * std::sin((fConst32 * fSlow493)));
		float fSlow528 = (fConst18 * ((fSlow493 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow525)))) / fSlow527));
		float fSlow529 = (fConst18 * (fSlow493 / fSlow527));
		float fSlow530 = (iSlow526 ? fSlow529 : fSlow528);
		float fSlow531 = ((fSlow524 * (fSlow524 - fSlow530)) + 1.0f);
		float fSlow532 = ((fSlow524 * (fSlow524 + fSlow530)) + 1.0f);
		float fSlow533 = (iSlow526 ? fSlow528 : fSlow529);
		float fSlow534 = ((fSlow524 * (fSlow524 + fSlow533)) + 1.0f);
		float fSlow535 = ((fSlow524 * (fSlow524 - fSlow533)) + 1.0f);
		float fSlow536 = (1.0f / fSlow491);
		float fSlow537 = float(fHslider127);
		int iSlow538 = (fSlow537 > 0.0f);
		float fSlow539 = (float(fHslider128) * std::sin((fConst32 * fSlow490)));
		float fSlow540 = (fConst18 * ((fSlow490 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow537)))) / fSlow539));
		float fSlow541 = (fConst18 * (fSlow490 / fSlow539));
		float fSlow542 = (iSlow538 ? fSlow541 : fSlow540);
		float fSlow543 = ((fSlow536 * (fSlow536 - fSlow542)) + 1.0f);
		float fSlow544 = ((fSlow536 * (fSlow536 + fSlow542)) + 1.0f);
		float fSlow545 = (iSlow538 ? fSlow540 : fSlow541);
		float fSlow546 = ((fSlow536 * (fSlow536 + fSlow545)) + 1.0f);
		float fSlow547 = ((fSlow536 * (fSlow536 - fSlow545)) + 1.0f);
		float fSlow548 = (1.0f / fSlow488);
		float fSlow549 = (((fSlow487 - fSlow485) / fSlow486) + 1.0f);
		float fSlow550 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow486))));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec12[0] = (0.0f - (fConst14 * ((fConst15 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fConst10 * fRec11[1]) + (fConst11 * (fRec8[1] + (fConst12 * fRec12[0]))));
			fVec1[(IOTA & 32767)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fVec2[0] = fSlow12;
			float fTemp0 = (fSlow12 - fVec2[1]);
			fVec3[0] = fSlow14;
			float fTemp1 = (0.0f - (fSlow12 + fVec3[1]));
			float fTemp2 = std::min<float>(1.0f, ((fTemp0 * float((fTemp0 > 0.0f))) + (fSlow13 * (fTemp1 * float((fTemp1 > 0.0f))))));
			fVec4[0] = fTemp2;
			iRec17[0] = (((iRec17[1] + (iRec17[1] > 0)) * (fTemp2 <= fVec4[1])) + (fTemp2 > fVec4[1]));
			float fTemp3 = float(iRec17[0]);
			float fTemp4 = (fConst22 * fTemp3);
			float fTemp5 = (fConst21 - fTemp3);
			fRec16[0] = ((fConst19 * fRec16[1]) + (fConst20 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp4, ((fConst23 * fTemp5) + 1.0f))))));
			int iTemp6 = int(fTemp2);
			fRec20[0] = (iTemp6 ? fSlow18 : fRec20[1]);
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
			fRec25[0] = (iTemp6 ? fSlow19 : fRec25[1]);
			float fTemp17 = (fRec25[0] + 0.5f);
			iRec24[0] = ((iVec0[1] + iRec24[1]) % int((fConst0 / fTemp17)));
			float fTemp18 = float(iRec24[0]);
			float fTemp19 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp17) * (1.0f - (0.000500000024f * std::min<float>(fTemp17, 1000.0f)))))))));
			float fTemp20 = (1.0f / fTemp19);
			float fTemp21 = (((fTemp20 + 1.41421354f) / fTemp19) + 1.0f);
			fRec23[0] = ((2.0f * (((float((fTemp18 < (fConst24 / fTemp17))) + -0.5f) + (float((fTemp18 < (fConst25 / fTemp17))) + -0.5f)) + (float((fTemp18 < (fConst26 / fTemp17))) + -0.5f))) - (((fRec23[2] * (((fTemp20 + -1.41421354f) / fTemp19) + 1.0f)) + (2.0f * (fRec23[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp19)))))) / fTemp21));
			fRec28[0] = (iTemp6 ? fSlow20 : fRec28[1]);
			float fTemp22 = (fRec28[0] + 0.5f);
			iRec27[0] = ((iVec0[1] + iRec27[1]) % int((fConst0 / fTemp22)));
			float fTemp23 = float(iRec27[0]);
			float fTemp24 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp22) * (1.0f - (0.000500000024f * std::min<float>(fTemp22, 1000.0f)))))))));
			float fTemp25 = (1.0f / fTemp24);
			float fTemp26 = (((fTemp25 + 1.41421354f) / fTemp24) + 1.0f);
			fRec26[0] = ((2.0f * (((float((fTemp23 < (fConst24 / fTemp22))) + -0.5f) + (float((fTemp23 < (fConst25 / fTemp22))) + -0.5f)) + (float((fTemp23 < (fConst26 / fTemp22))) + -0.5f))) - (((fRec26[2] * (((fTemp25 + -1.41421354f) / fTemp24) + 1.0f)) + (2.0f * (fRec26[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp24)))))) / fTemp26));
			fRec31[0] = (iTemp6 ? fSlow21 : fRec31[1]);
			float fTemp27 = (fRec31[0] + 0.5f);
			iRec30[0] = ((iVec0[1] + iRec30[1]) % int((fConst0 / fTemp27)));
			float fTemp28 = float(iRec30[0]);
			float fTemp29 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fTemp27) * (1.0f - (0.000500000024f * std::min<float>(fTemp27, 1000.0f)))))))));
			float fTemp30 = (1.0f / fTemp29);
			float fTemp31 = (((fTemp30 + 1.41421354f) / fTemp29) + 1.0f);
			fRec29[0] = ((2.0f * (((float((fTemp28 < (fConst24 / fTemp27))) + -0.5f) + (float((fTemp28 < (fConst25 / fTemp27))) + -0.5f)) + (float((fTemp28 < (fConst26 / fTemp27))) + -0.5f))) - (((fRec29[2] * (((fTemp30 + -1.41421354f) / fTemp29) + 1.0f)) + (2.0f * (fRec29[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp29)))))) / fTemp31));
			float fTemp32 = (fRec16[0] * (((((fRec18[2] + (fRec18[0] + (2.0f * fRec18[1]))) / fTemp16) + ((fRec23[2] + (fRec23[0] + (2.0f * fRec23[1]))) / fTemp21)) + ((fRec26[2] + (fRec26[0] + (2.0f * fRec26[1]))) / fTemp26)) + ((fRec29[2] + (fRec29[0] + (2.0f * fRec29[1]))) / fTemp31)));
			float fTemp33 = std::fabs((0.25f * fTemp32));
			fRec34[0] = std::max<float>(fTemp33, ((fSlow28 * fRec34[1]) + (fSlow29 * fTemp33)));
			fRec33[0] = ((fSlow25 * fRec33[1]) + (fRec34[0] * fSlow30));
			fRec32[0] = ((fConst30 * fRec32[1]) + (fSlow22 * std::max<float>(((20.0f * std::log10(fRec33[0])) - fSlow31), 0.0f)));
			float fTemp34 = (0.25f * (fTemp32 * std::pow(10.0f, (0.0500000007f * fRec32[0]))));
			fRec35[0] = (fTemp34 - (fSlow35 * ((fSlow36 * fRec35[2]) + (fSlow37 * fRec35[1]))));
			float fTemp35 = (fSlow11 * fRec15[1]);
			fRec15[0] = (fTemp34 - ((fSlow35 * (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])))) + (((fSlow45 * fRec15[2]) + fTemp35) / fSlow46)));
			float fTemp36 = (fSlow8 * fRec14[1]);
			fRec14[0] = ((((fTemp35 + (fRec15[0] * fSlow48)) + (fSlow49 * fRec15[2])) / fSlow46) - (((fSlow57 * fRec14[2]) + fTemp36) / fSlow58));
			fRec13[0] = ((((fTemp36 + (fRec14[0] * fSlow60)) + (fSlow61 * fRec14[2])) / fSlow58) - (fSlow62 * ((fSlow63 * fRec13[2]) + (fSlow64 * fRec13[1]))));
			float fTemp37 = (fSlow5 * (fRec13[2] + (fRec13[0] + (2.0f * fRec13[1]))));
			float fRec46 = (0.0f - (0.997305274f * ((0.899999976f * fRec47[2]) + (0.0500000007f * (fRec47[1] + fRec47[3])))));
			fRec67[0] = (fConst35 + (fRec67[1] - std::floor((fConst35 + fRec67[1]))));
			fRec65[0] = ((0.999000013f * fRec65[1]) + (0.00100000005f * ((fSlow77 * ftbl0mydspSIG0[int((65536.0f * fRec67[0]))]) + 1.0f)));
			fVec5[0] = fSlow78;
			float fTemp38 = (fSlow78 - fVec5[1]);
			fVec6[0] = fSlow80;
			float fTemp39 = (0.0f - (fSlow78 + fVec6[1]));
			float fTemp40 = std::min<float>(1.0f, ((fTemp38 * float((fTemp38 > 0.0f))) + (fSlow79 * (fTemp39 * float((fTemp39 > 0.0f))))));
			fVec7[0] = fTemp40;
			int iTemp41 = int(fTemp40);
			fRec68[0] = (iTemp41 ? fSlow81 : fRec68[1]);
			float fTemp42 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow76 * (fRec65[0] * fRec68[0]))))) + -0.109999999f);
			float fTemp43 = (fConst34 * fTemp42);
			float fTemp44 = (fTemp43 + -1.49999499f);
			int iTemp45 = int(fTemp44);
			int iTemp46 = int(std::min<float>(fConst33, float(std::max<int>(0, iTemp45))));
			float fTemp47 = std::floor(fTemp44);
			float fTemp48 = (fTemp43 + (-1.0f - fTemp47));
			float fTemp49 = (0.0f - fTemp48);
			float fTemp50 = (fTemp43 + (-2.0f - fTemp47));
			float fTemp51 = (0.0f - (0.5f * fTemp50));
			float fTemp52 = (fTemp43 + (-3.0f - fTemp47));
			float fTemp53 = (0.0f - (0.333333343f * fTemp52));
			float fTemp54 = (fTemp43 + (-4.0f - fTemp47));
			float fTemp55 = (0.0f - (0.25f * fTemp54));
			float fTemp56 = (fTemp43 - fTemp47);
			int iTemp57 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp45 + 1)))));
			float fTemp58 = (0.0f - fTemp50);
			float fTemp59 = (0.0f - (0.5f * fTemp52));
			float fTemp60 = (0.0f - (0.333333343f * fTemp54));
			int iTemp61 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp45 + 2)))));
			float fTemp62 = (0.0f - fTemp52);
			float fTemp63 = (0.0f - (0.5f * fTemp54));
			float fTemp64 = (fTemp48 * fTemp50);
			int iTemp65 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp45 + 3)))));
			float fTemp66 = (0.0f - fTemp54);
			float fTemp67 = (fTemp64 * fTemp52);
			int iTemp68 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp45 + 4)))));
			fRec61[0] = (((((fRec42[((IOTA - (iTemp46 + 1)) & 2047)] * fTemp49) * fTemp51) * fTemp53) * fTemp55) + (fTemp56 * ((((((fRec42[((IOTA - (iTemp57 + 1)) & 2047)] * fTemp58) * fTemp59) * fTemp60) + (0.5f * (((fTemp48 * fRec42[((IOTA - (iTemp61 + 1)) & 2047)]) * fTemp62) * fTemp63))) + (0.166666672f * ((fTemp64 * fRec42[((IOTA - (iTemp65 + 1)) & 2047)]) * fTemp66))) + (0.0416666679f * (fTemp67 * fRec42[((IOTA - (iTemp68 + 1)) & 2047)])))));
			fRec69[0] = ((fSlow82 * fRec69[1]) + (fSlow83 * fRec61[1]));
			float fRec62 = fRec69[0];
			fRec71[0] = fRec40[1];
			fRec72[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec71[2]) + (0.0500000007f * (fRec71[1] + fRec71[3])))));
			float fTemp69 = (fConst36 * fTemp42);
			float fTemp70 = (fTemp69 + -1.49999499f);
			int iTemp71 = int(fTemp70);
			int iTemp72 = int(std::min<float>(fConst33, float(std::max<int>(0, iTemp71))));
			float fTemp73 = std::floor(fTemp70);
			float fTemp74 = (fTemp69 + (-1.0f - fTemp73));
			float fTemp75 = (0.0f - fTemp74);
			float fTemp76 = (fTemp69 + (-2.0f - fTemp73));
			float fTemp77 = (0.0f - (0.5f * fTemp76));
			float fTemp78 = (fTemp69 + (-3.0f - fTemp73));
			float fTemp79 = (0.0f - (0.333333343f * fTemp78));
			float fTemp80 = (fTemp69 + (-4.0f - fTemp73));
			float fTemp81 = (0.0f - (0.25f * fTemp80));
			float fTemp82 = (fTemp69 - fTemp73);
			int iTemp83 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp71 + 1)))));
			float fTemp84 = (0.0f - fTemp76);
			float fTemp85 = (0.0f - (0.5f * fTemp78));
			float fTemp86 = (0.0f - (0.333333343f * fTemp80));
			int iTemp87 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp71 + 2)))));
			float fTemp88 = (0.0f - fTemp78);
			float fTemp89 = (0.0f - (0.5f * fTemp80));
			float fTemp90 = (fTemp74 * fTemp76);
			int iTemp91 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp71 + 3)))));
			float fTemp92 = (0.0f - fTemp80);
			float fTemp93 = (fTemp90 * fTemp78);
			int iTemp94 = int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp71 + 4)))));
			fVec8[0] = (((((fRec72[((IOTA - (iTemp72 + 2)) & 1023)] * fTemp75) * fTemp77) * fTemp79) * fTemp81) + (fTemp82 * ((((((fRec72[((IOTA - (iTemp83 + 2)) & 1023)] * fTemp84) * fTemp85) * fTemp86) + (0.5f * (((fTemp74 * fRec72[((IOTA - (iTemp87 + 2)) & 1023)]) * fTemp88) * fTemp89))) + (0.166666672f * ((fTemp90 * fRec72[((IOTA - (iTemp91 + 2)) & 1023)]) * fTemp92))) + (0.0416666679f * (fTemp93 * fRec72[((IOTA - (iTemp94 + 2)) & 1023)])))));
			float fTemp95 = (fTemp40 + fVec8[1]);
			fVec9[0] = fTemp95;
			fRec70[(IOTA & 2047)] = ((fSlow82 * fRec70[((IOTA - 1) & 2047)]) + (fSlow83 * fVec9[2]));
			float fRec63 = (((((fTemp49 * fTemp51) * fTemp53) * fTemp55) * fRec70[((IOTA - iTemp46) & 2047)]) + (fTemp56 * ((((((fTemp58 * fTemp59) * fTemp60) * fRec70[((IOTA - iTemp57) & 2047)]) + (0.5f * (((fTemp48 * fTemp62) * fTemp63) * fRec70[((IOTA - iTemp61) & 2047)]))) + (0.166666672f * ((fTemp64 * fTemp66) * fRec70[((IOTA - iTemp65) & 2047)]))) + (0.0416666679f * (fTemp67 * fRec70[((IOTA - iTemp68) & 2047)])))));
			float fRec64 = (fVec9[1] + fRec57[1]);
			fRec57[0] = fRec62;
			float fRec58 = fRec57[1];
			float fRec59 = fRec63;
			float fRec60 = fRec64;
			fRec53[0] = fRec58;
			float fRec54 = (fTemp40 + fRec53[1]);
			float fRec55 = fRec59;
			float fRec56 = fRec60;
			fRec49[(IOTA & 1023)] = fRec54;
			float fRec50 = (((((fTemp75 * fTemp77) * fTemp79) * fTemp81) * fRec49[((IOTA - (iTemp72 + 1)) & 1023)]) + (fTemp82 * ((((((fTemp84 * fTemp85) * fTemp86) * fRec49[((IOTA - (iTemp83 + 1)) & 1023)]) + (0.5f * (((fTemp74 * fTemp88) * fTemp89) * fRec49[((IOTA - (iTemp87 + 1)) & 1023)]))) + (0.166666672f * ((fTemp90 * fTemp92) * fRec49[((IOTA - (iTemp91 + 1)) & 1023)]))) + (0.0416666679f * (fTemp93 * fRec49[((IOTA - (iTemp94 + 1)) & 1023)])))));
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
			fRec73[0] = (fSlow84 + (0.999000013f * fRec73[1]));
			float fTemp96 = std::min<float>(15000.0f, std::max<float>(500.0f, fRec73[0]));
			float fTemp97 = std::tan((fConst18 * fTemp96));
			float fTemp98 = (1.0f / fTemp97);
			float fTemp99 = (((fTemp98 + 1.42857146f) / fTemp97) + 1.0f);
			fRec39[0] = ((1.5f * fRec41) - (((fRec39[2] * (((fTemp98 + -1.42857146f) / fTemp97) + 1.0f)) + (2.0f * (fRec39[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp97)))))) / fTemp99));
			iRec74[0] = (((iRec74[1] + (iRec74[1] > 0)) * (fTemp40 <= fVec7[1])) + (fTemp40 > fVec7[1]));
			float fTemp100 = float(iRec74[0]);
			iRec75[0] = (iTemp41 ? 7 : iRec75[1]);
			float fTemp101 = ((((fRec39[2] + (fRec39[0] + (2.0f * fRec39[1]))) * ((6.66666674e-05f * (fTemp96 + -500.0f)) + 1.0f)) * std::max<float>(0.0f, std::min<float>((fConst22 * fTemp100), ((fConst23 * (fConst21 - fTemp100)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec75[0] + -10)))));
			float fTemp102 = std::fabs((0.25f * (fTemp101 / fTemp99)));
			fRec78[0] = std::max<float>(fTemp102, ((fRec78[1] * fSlow91) + (fTemp102 * fSlow92)));
			fRec77[0] = ((fRec77[1] * fSlow88) + (fRec78[0] * fSlow93));
			fRec76[0] = ((fConst30 * fRec76[1]) + (fSlow85 * std::max<float>(((20.0f * std::log10(fRec77[0])) - fSlow94), 0.0f)));
			float fTemp103 = (0.25f * ((fTemp101 * std::pow(10.0f, (0.0500000007f * fRec76[0]))) / fTemp99));
			fRec79[0] = (fTemp103 - (fSlow98 * ((fSlow99 * fRec79[2]) + (fSlow100 * fRec79[1]))));
			float fTemp104 = (fSlow75 * fRec38[1]);
			fRec38[0] = (fTemp103 - ((fSlow98 * (fRec79[2] + (fRec79[0] + (2.0f * fRec79[1])))) + (((fRec38[2] * fSlow108) + fTemp104) / fSlow109)));
			float fTemp105 = (fSlow72 * fRec37[1]);
			fRec37[0] = ((((fTemp104 + (fRec38[0] * fSlow111)) + (fRec38[2] * fSlow112)) / fSlow109) - (((fRec37[2] * fSlow120) + fTemp105) / fSlow121));
			fRec36[0] = ((((fTemp105 + (fRec37[0] * fSlow123)) + (fRec37[2] * fSlow124)) / fSlow121) - (fSlow125 * ((fSlow126 * fRec36[2]) + (fSlow127 * fRec36[1]))));
			float fTemp106 = (fSlow69 * (fRec36[2] + (fRec36[0] + (2.0f * fRec36[1]))));
			fRec85[0] = (fConst43 + (fRec85[1] - std::floor((fConst43 + fRec85[1]))));
			float fTemp107 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec85[0]))]) + 1.0f);
			fRec84[0] = ((fConst41 * fRec84[1]) + (fSlow139 * fTemp107));
			fRec86[0] = ((fConst41 * fRec86[1]) + (fSlow140 * (fRec65[0] * fTemp107)));
			float fTemp108 = ((0.00366666657f * (400.0f - fRec86[0])) + 3.0f);
			float fTemp109 = std::max<float>(1.1920929e-07f, std::fabs(fRec86[0]));
			float fTemp110 = (fRec88[1] + (fConst29 * fTemp109));
			float fTemp111 = (fTemp110 + -1.0f);
			int iTemp112 = (fTemp111 < 0.0f);
			fRec88[0] = (iTemp112 ? fTemp110 : fTemp111);
			float fRec89 = (iTemp112 ? fTemp110 : (fTemp110 + ((1.0f - (fConst0 / fTemp109)) * fTemp111)));
			float fTemp113 = (0.400000006f * ((2.0f * fRec89) + -1.0f));
			float fTemp114 = ((fSlow192 <= fRec86[0]) ? fRec86[0] : fSlow192);
			float fTemp115 = std::tan((fConst18 * fTemp114));
			float fTemp116 = (1.0f / fTemp115);
			float fTemp117 = (fSlow200 / fTemp114);
			float fTemp118 = (((fTemp116 + fTemp117) / fTemp115) + 1.0f);
			fRec87[0] = (fTemp113 - (((fRec87[2] * (((fTemp116 - fTemp117) / fTemp115) + 1.0f)) + (2.0f * (fRec87[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp115)))))) / fTemp118));
			float fTemp119 = (fTemp115 * fTemp118);
			float fTemp120 = ((2.0f * fRec86[0]) + 30.0f);
			float fTemp121 = ((iSlow202 & (fRec86[0] >= 200.0f)) ? (fSlow201 - (0.000952380942f * ((fRec86[0] + -200.0f) * fSlow203))) : ((fSlow201 <= fTemp120) ? fTemp120 : fSlow201));
			float fTemp122 = std::tan((fConst18 * fTemp121));
			float fTemp123 = (1.0f / fTemp122);
			float fTemp124 = (fSlow208 / fTemp121);
			float fTemp125 = (((fTemp123 + fTemp124) / fTemp122) + 1.0f);
			fRec90[0] = (fTemp113 - (((fRec90[2] * (((fTemp123 - fTemp124) / fTemp122) + 1.0f)) + (2.0f * (fRec90[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp122)))))) / fTemp125));
			float fTemp126 = (fTemp122 * fTemp125);
			fRec91[0] = (fTemp113 - (((fRec91[2] * fSlow221) + (2.0f * (fRec91[1] * fSlow222))) / fSlow223));
			fRec92[0] = (fTemp113 - (((fRec92[2] * fSlow232) + (2.0f * (fRec92[1] * fSlow233))) / fSlow234));
			fRec93[0] = (fTemp113 - (((fRec93[2] * fSlow242) + (2.0f * (fRec93[1] * fSlow243))) / fSlow244));
			fRec83[0] = ((fRec84[0] * ((fTemp108 * (((((fRec87[0] / fTemp119) + ((fRec90[0] * fSlow209) / fTemp126)) + ((fRec91[0] * fSlow225) / fSlow226)) + ((fRec92[0] * fSlow236) / fSlow237)) + ((fRec93[0] * fSlow245) / fSlow246))) + (((((fRec87[2] * (0.0f - (fTemp108 / fTemp119))) + (fRec90[2] * (0.0f - ((fTemp108 * fSlow209) / fTemp126)))) + (fRec91[2] * (0.0f - ((fTemp108 * fSlow225) / fSlow226)))) + (fRec92[2] * (0.0f - ((fTemp108 * fSlow236) / fSlow237)))) + (fRec93[2] * (0.0f - ((fTemp108 * fSlow245) / fSlow246)))))) - (fConst45 * ((fConst46 * fRec83[2]) + (fConst47 * fRec83[1]))));
			float fTemp127 = (fRec83[2] + (fRec83[0] + (2.0f * fRec83[1])));
			float fTemp128 = std::fabs((fConst40 * fTemp127));
			fRec96[0] = std::max<float>(fTemp128, ((fRec96[1] * fSlow253) + (fTemp128 * fSlow254)));
			fRec95[0] = ((fRec95[1] * fSlow250) + (fRec96[0] * fSlow255));
			fRec94[0] = ((fConst30 * fRec94[1]) + (fSlow247 * std::max<float>(((20.0f * std::log10(fRec95[0])) - fSlow256), 0.0f)));
			float fTemp129 = (fConst40 * (fTemp127 * std::pow(10.0f, (0.0500000007f * fRec94[0]))));
			fRec97[0] = (fTemp129 - (fSlow260 * ((fSlow261 * fRec97[2]) + (fSlow262 * fRec97[1]))));
			float fTemp130 = (fSlow138 * fRec82[1]);
			fRec82[0] = (fTemp129 - ((fSlow260 * (fRec97[2] + (fRec97[0] + (2.0f * fRec97[1])))) + (((fRec82[2] * fSlow270) + fTemp130) / fSlow271)));
			float fTemp131 = (fSlow135 * fRec81[1]);
			fRec81[0] = ((((fTemp130 + (fRec82[0] * fSlow273)) + (fRec82[2] * fSlow274)) / fSlow271) - (((fRec81[2] * fSlow282) + fTemp131) / fSlow283));
			fRec80[0] = ((((fTemp131 + (fRec81[0] * fSlow285)) + (fRec81[2] * fSlow286)) / fSlow283) - (fSlow287 * ((fSlow288 * fRec80[2]) + (fSlow289 * fRec80[1]))));
			float fTemp132 = (fSlow132 * (fRec80[2] + (fRec80[0] + (2.0f * fRec80[1]))));
			iRec116[0] = 0;
			fRec122[0] = ((0.699999988f * fRec122[1]) + (0.284999996f * fRec121[1]));
			float fRec120 = (fRec122[0] + float(iRec116[1]));
			fRec123[0] = (fConst48 + (fRec123[1] - std::floor((fConst48 + fRec123[1]))));
			fVec10[0] = fSlow302;
			float fTemp133 = (fSlow302 - fVec10[1]);
			fVec11[0] = fSlow304;
			float fTemp134 = (0.0f - (fSlow302 + fVec11[1]));
			fRec126[0] = (int(std::min<float>(1.0f, ((fTemp133 * float((fTemp133 > 0.0f))) + (fSlow303 * (fTemp134 * float((fTemp134 > 0.0f))))))) ? fSlow305 : fRec126[1]);
			fRec125[0] = ((fConst49 * fRec125[1]) + (fConst50 * fRec126[0]));
			float fTemp135 = (fSlow16 * fRec125[0]);
			fRec124[0] = ((fConst49 * fRec124[1]) + (fSlow301 * float((fTemp135 > 430.0f))));
			iRec128[0] = ((1103515245 * iRec128[1]) + 12345);
			float fTemp136 = (4.65661287e-10f * float(iRec128[0]));
			fRec127[0] = (fTemp136 - (fConst55 * ((fConst56 * fRec127[2]) + (fConst57 * fRec127[1]))));
			fRec129[0] = fRec102[1];
			fRec130[(IOTA & 2047)] = (0.949999988f * fRec129[1]);
			float fTemp137 = (fTemp135 + -440.0f);
			float fTemp138 = ((340.0f / ((fTemp137 * ((fTemp137 * ((fTemp137 * ((4.09700007e-08f * fTemp137) + -2.77700001e-05f)) + 0.00809509959f)) + 1.0f)) + 440.0f)) + 0.270000011f);
			int iTemp139 = int(((fSlow307 * fTemp138) + -1.49999499f));
			int iTemp140 = (int(std::min<float>(fConst33, float(std::max<int>(0, iTemp139)))) + 1);
			float fTemp141 = ((340.0f / ((fTemp137 * ((fTemp137 * ((0.00809509959f - (2.77700001e-05f * fTemp137)) + (4.09700007e-08f * mydsp_faustpower2_f(fTemp137)))) + 1.0f)) + 440.0f)) + 0.270000011f);
			float fTemp142 = (fSlow307 * fTemp141);
			float fTemp143 = std::floor((fTemp142 + -1.49999499f));
			float fTemp144 = (fTemp142 + (-1.0f - fTemp143));
			float fTemp145 = (0.0f - fTemp144);
			float fTemp146 = (fTemp142 + (-2.0f - fTemp143));
			float fTemp147 = (0.0f - (0.5f * fTemp146));
			float fTemp148 = (fTemp142 + (-3.0f - fTemp143));
			float fTemp149 = (0.0f - (0.333333343f * fTemp148));
			float fTemp150 = (fTemp142 + (-4.0f - fTemp143));
			float fTemp151 = (0.0f - (0.25f * fTemp150));
			float fTemp152 = (fTemp142 - fTemp143);
			int iTemp153 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp139 + 1))))) + 1);
			float fTemp154 = (0.0f - fTemp146);
			float fTemp155 = (0.0f - (0.5f * fTemp148));
			float fTemp156 = (0.0f - (0.333333343f * fTemp150));
			int iTemp157 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp139 + 2))))) + 1);
			float fTemp158 = (0.0f - fTemp148);
			float fTemp159 = (0.0f - (0.5f * fTemp150));
			float fTemp160 = (fTemp144 * fTemp146);
			int iTemp161 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp139 + 3))))) + 1);
			float fTemp162 = (0.0f - fTemp150);
			float fTemp163 = (fTemp160 * fTemp148);
			int iTemp164 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp139 + 4))))) + 1);
			fVec12[0] = (((((fRec130[((IOTA - iTemp140) & 2047)] * fTemp145) * fTemp147) * fTemp149) * fTemp151) + (fTemp152 * ((((((fRec130[((IOTA - iTemp153) & 2047)] * fTemp154) * fTemp155) * fTemp156) + (0.5f * (((fTemp144 * fRec130[((IOTA - iTemp157) & 2047)]) * fTemp158) * fTemp159))) + (0.166666672f * ((fTemp160 * fRec130[((IOTA - iTemp161) & 2047)]) * fTemp162))) + (0.0416666679f * (fTemp163 * fRec130[((IOTA - iTemp164) & 2047)])))));
			float fTemp165 = (((0.0359999985f * ftbl0mydspSIG0[int((65536.0f * fRec123[0]))]) + (fRec124[0] * ((fConst54 * (fRec127[2] + (fRec127[0] + (2.0f * fRec127[1])))) + 1.0f))) - (0.5f * fVec12[1]));
			float fTemp166 = ((0.5f * fRec108[1]) + std::max<float>(-1.0f, std::min<float>(1.0f, (fTemp165 * (mydsp_faustpower2_f(fTemp165) + -1.0f)))));
			fVec13[(IOTA & 2047)] = fTemp166;
			int iTemp167 = int(((fSlow308 * fTemp138) + -1.49999499f));
			int iTemp168 = (int(std::min<float>(fConst33, float(std::max<int>(0, iTemp167)))) + 1);
			float fTemp169 = (fSlow308 * fTemp141);
			float fTemp170 = std::floor((fTemp169 + -1.49999499f));
			float fTemp171 = (fTemp169 + (-1.0f - fTemp170));
			float fTemp172 = (0.0f - fTemp171);
			float fTemp173 = (fTemp169 + (-2.0f - fTemp170));
			float fTemp174 = (0.0f - (0.5f * fTemp173));
			float fTemp175 = (fTemp169 + (-3.0f - fTemp170));
			float fTemp176 = (0.0f - (0.333333343f * fTemp175));
			float fTemp177 = (fTemp169 + (-4.0f - fTemp170));
			float fTemp178 = (0.0f - (0.25f * fTemp177));
			float fTemp179 = (fTemp169 - fTemp170);
			int iTemp180 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp167 + 1))))) + 1);
			float fTemp181 = (0.0f - fTemp173);
			float fTemp182 = (0.0f - (0.5f * fTemp175));
			float fTemp183 = (0.0f - (0.333333343f * fTemp177));
			int iTemp184 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp167 + 2))))) + 1);
			float fTemp185 = (0.0f - fTemp175);
			float fTemp186 = (0.0f - (0.5f * fTemp177));
			float fTemp187 = (fTemp171 * fTemp173);
			int iTemp188 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp167 + 3))))) + 1);
			float fTemp189 = (0.0f - fTemp177);
			float fTemp190 = (fTemp187 * fTemp175);
			int iTemp191 = (int(std::min<float>(fConst33, float(std::max<int>(0, (iTemp167 + 4))))) + 1);
			fVec14[0] = (((((fVec13[((IOTA - iTemp168) & 2047)] * fTemp172) * fTemp174) * fTemp176) * fTemp178) + (fTemp179 * ((((((fVec13[((IOTA - iTemp180) & 2047)] * fTemp181) * fTemp182) * fTemp183) + (0.5f * (((fTemp171 * fVec13[((IOTA - iTemp184) & 2047)]) * fTemp185) * fTemp186))) + (0.166666672f * ((fTemp187 * fVec13[((IOTA - iTemp188) & 2047)]) * fTemp189))) + (0.0416666679f * (fTemp190 * fVec13[((IOTA - iTemp191) & 2047)])))));
			fRec121[0] = fVec14[1];
			float fRec117 = fRec120;
			float fRec118 = fRec121[0];
			float fRec119 = fRec121[0];
			fRec112[(IOTA & 2047)] = fRec117;
			float fRec113 = (((((fTemp172 * fTemp174) * fTemp176) * fTemp178) * fRec112[((IOTA - iTemp168) & 2047)]) + (fTemp179 * ((((((fTemp181 * fTemp182) * fTemp183) * fRec112[((IOTA - iTemp180) & 2047)]) + (0.5f * (((fTemp171 * fTemp185) * fTemp186) * fRec112[((IOTA - iTemp184) & 2047)]))) + (0.166666672f * ((fTemp187 * fTemp189) * fRec112[((IOTA - iTemp188) & 2047)]))) + (0.0416666679f * (fTemp190 * fRec112[((IOTA - iTemp191) & 2047)])))));
			float fRec114 = fRec118;
			float fRec115 = fRec119;
			fRec108[0] = fRec113;
			float fRec109 = fRec108[1];
			float fRec110 = fRec114;
			float fRec111 = fRec115;
			fRec104[(IOTA & 2047)] = fRec109;
			float fRec105 = (((((fTemp145 * fTemp147) * fTemp149) * fTemp151) * fRec104[((IOTA - iTemp140) & 2047)]) + (fTemp152 * ((((((fTemp154 * fTemp155) * fTemp156) * fRec104[((IOTA - iTemp153) & 2047)]) + (0.5f * (((fTemp144 * fTemp158) * fTemp159) * fRec104[((IOTA - iTemp157) & 2047)]))) + (0.166666672f * ((fTemp160 * fTemp162) * fRec104[((IOTA - iTemp161) & 2047)]))) + (0.0416666679f * (fTemp163 * fRec104[((IOTA - iTemp164) & 2047)])))));
			float fRec106 = fRec110;
			float fRec107 = fRec111;
			fRec102[0] = fRec105;
			fRec103[0] = fRec107;
			fRec101[0] = ((fRec103[0] + (0.995000005f * fRec101[1])) - fRec103[1]);
			float fTemp192 = std::fabs((0.125f * fRec101[0]));
			fRec133[0] = std::max<float>(fTemp192, ((fRec133[1] * fSlow315) + (fTemp192 * fSlow316)));
			fRec132[0] = ((fRec132[1] * fSlow312) + (fRec133[0] * fSlow317));
			fRec131[0] = ((fConst30 * fRec131[1]) + (fSlow309 * std::max<float>(((20.0f * std::log10(fRec132[0])) - fSlow318), 0.0f)));
			float fTemp193 = (0.125f * (fRec101[0] * std::pow(10.0f, (0.0500000007f * fRec131[0]))));
			fRec134[0] = (fTemp193 - (fSlow322 * ((fSlow323 * fRec134[2]) + (fSlow324 * fRec134[1]))));
			float fTemp194 = (fSlow300 * fRec100[1]);
			fRec100[0] = (fTemp193 - ((fSlow322 * (fRec134[2] + (fRec134[0] + (2.0f * fRec134[1])))) + (((fRec100[2] * fSlow332) + fTemp194) / fSlow333)));
			float fTemp195 = (fSlow297 * fRec99[1]);
			fRec99[0] = ((((fTemp194 + (fRec100[0] * fSlow335)) + (fRec100[2] * fSlow336)) / fSlow333) - (((fRec99[2] * fSlow344) + fTemp195) / fSlow345));
			fRec98[0] = ((((fTemp195 + (fRec99[0] * fSlow347)) + (fRec99[2] * fSlow348)) / fSlow345) - (fSlow349 * ((fSlow350 * fRec98[2]) + (fSlow351 * fRec98[1]))));
			float fTemp196 = (fSlow294 * (fRec98[2] + (fRec98[0] + (2.0f * fRec98[1]))));
			fVec15[(IOTA & 2047)] = (fTemp37 + (fTemp106 + (fTemp132 + fTemp196)));
			float fTemp197 = (0.300000012f * fVec15[((IOTA - iConst59) & 2047)]);
			float fTemp198 = (((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst17) & 32767)]) - fTemp197);
			fVec16[(IOTA & 2047)] = fTemp198;
			fRec9[0] = fVec16[((IOTA - iConst60) & 2047)];
			float fRec10 = (0.0f - (0.600000024f * fTemp198));
			fRec138[0] = (0.0f - (fConst14 * ((fConst15 * fRec138[1]) - (fRec4[1] + fRec4[2]))));
			fRec137[0] = ((fConst69 * fRec137[1]) + (fConst70 * (fRec4[1] + (fConst71 * fRec138[0]))));
			fVec17[(IOTA & 32767)] = ((0.353553385f * fRec137[0]) + 9.99999968e-21f);
			float fTemp199 = (((0.600000024f * fRec135[1]) + fVec17[((IOTA - iConst73) & 32767)]) - fTemp197);
			fVec18[(IOTA & 4095)] = fTemp199;
			fRec135[0] = fVec18[((IOTA - iConst74) & 4095)];
			float fRec136 = (0.0f - (0.600000024f * fTemp199));
			fRec142[0] = (0.0f - (fConst14 * ((fConst15 * fRec142[1]) - (fRec6[1] + fRec6[2]))));
			fRec141[0] = ((fConst83 * fRec141[1]) + (fConst84 * (fRec6[1] + (fConst85 * fRec142[0]))));
			fVec19[(IOTA & 16383)] = ((0.353553385f * fRec141[0]) + 9.99999968e-21f);
			float fTemp200 = (fVec19[((IOTA - iConst87) & 16383)] + (fTemp197 + (0.600000024f * fRec139[1])));
			fVec20[(IOTA & 4095)] = fTemp200;
			fRec139[0] = fVec20[((IOTA - iConst88) & 4095)];
			float fRec140 = (0.0f - (0.600000024f * fTemp200));
			fRec146[0] = (0.0f - (fConst14 * ((fConst15 * fRec146[1]) - (fRec2[1] + fRec2[2]))));
			fRec145[0] = ((fConst97 * fRec145[1]) + (fConst98 * (fRec2[1] + (fConst99 * fRec146[0]))));
			fVec21[(IOTA & 32767)] = ((0.353553385f * fRec145[0]) + 9.99999968e-21f);
			float fTemp201 = (fTemp197 + ((0.600000024f * fRec143[1]) + fVec21[((IOTA - iConst101) & 32767)]));
			fVec22[(IOTA & 4095)] = fTemp201;
			fRec143[0] = fVec22[((IOTA - iConst102) & 4095)];
			float fRec144 = (0.0f - (0.600000024f * fTemp201));
			fRec150[0] = (0.0f - (fConst14 * ((fConst15 * fRec150[1]) - (fRec7[1] + fRec7[2]))));
			fRec149[0] = ((fConst111 * fRec149[1]) + (fConst112 * (fRec7[1] + (fConst113 * fRec150[0]))));
			fVec23[(IOTA & 16383)] = ((0.353553385f * fRec149[0]) + 9.99999968e-21f);
			iRec155[0] = ((iVec0[1] + iRec155[1]) % int((fConst0 / fRec20[0])));
			float fTemp202 = float(iRec155[0]);
			float fTemp203 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fRec20[0] * fTemp13) * (1.0f - (0.000500000024f * std::min<float>(fRec20[0], 1000.0f)))))))));
			float fTemp204 = (1.0f / fTemp203);
			float fTemp205 = (((fTemp204 + 1.41421354f) / fTemp203) + 1.0f);
			fRec154[0] = ((2.0f * (((float((fTemp202 < (fConst24 / fRec20[0]))) + -0.5f) + (float((fTemp202 < (fConst25 / fRec20[0]))) + -0.5f)) + (float((fTemp202 < (fConst26 / fRec20[0]))) + -0.5f))) - (((fRec154[2] * (((fTemp204 + -1.41421354f) / fTemp203) + 1.0f)) + (2.0f * (fRec154[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp203)))))) / fTemp205));
			iRec157[0] = ((iVec0[1] + iRec157[1]) % int((fConst0 / fRec25[0])));
			float fTemp206 = float(iRec157[0]);
			float fTemp207 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec25[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec25[0], 1000.0f)))))))));
			float fTemp208 = (1.0f / fTemp207);
			float fTemp209 = (((fTemp208 + 1.41421354f) / fTemp207) + 1.0f);
			fRec156[0] = ((2.0f * (((float((fTemp206 < (fConst24 / fRec25[0]))) + -0.5f) + (float((fTemp206 < (fConst25 / fRec25[0]))) + -0.5f)) + (float((fTemp206 < (fConst26 / fRec25[0]))) + -0.5f))) - (((fRec156[2] * (((fTemp208 + -1.41421354f) / fTemp207) + 1.0f)) + (2.0f * (fRec156[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp207)))))) / fTemp209));
			iRec159[0] = ((iVec0[1] + iRec159[1]) % int((fConst0 / fRec28[0])));
			float fTemp210 = float(iRec159[0]);
			float fTemp211 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec28[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec28[0], 1000.0f)))))))));
			float fTemp212 = (1.0f / fTemp211);
			float fTemp213 = (((fTemp212 + 1.41421354f) / fTemp211) + 1.0f);
			fRec158[0] = ((2.0f * (((float((fTemp210 < (fConst24 / fRec28[0]))) + -0.5f) + (float((fTemp210 < (fConst25 / fRec28[0]))) + -0.5f)) + (float((fTemp210 < (fConst26 / fRec28[0]))) + -0.5f))) - (((fRec158[2] * (((fTemp212 + -1.41421354f) / fTemp211) + 1.0f)) + (2.0f * (fRec158[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp211)))))) / fTemp213));
			iRec161[0] = ((iVec0[1] + iRec161[1]) % int((fConst0 / fRec31[0])));
			float fTemp214 = float(iRec161[0]);
			float fTemp215 = std::tan((fConst18 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp13 * fRec31[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec31[0], 1000.0f)))))))));
			float fTemp216 = (1.0f / fTemp215);
			float fTemp217 = (((fTemp216 + 1.41421354f) / fTemp215) + 1.0f);
			fRec160[0] = ((2.0f * (((float((fTemp214 < (fConst24 / fRec31[0]))) + -0.5f) + (float((fTemp214 < (fConst25 / fRec31[0]))) + -0.5f)) + (float((fTemp214 < (fConst26 / fRec31[0]))) + -0.5f))) - (((fRec160[2] * (((fTemp216 + -1.41421354f) / fTemp215) + 1.0f)) + (2.0f * (fRec160[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp215)))))) / fTemp217));
			float fTemp218 = (fRec16[0] * (((((fRec154[2] + (fRec154[0] + (2.0f * fRec154[1]))) / fTemp205) + ((fRec156[2] + (fRec156[0] + (2.0f * fRec156[1]))) / fTemp209)) + ((fRec158[2] + (fRec158[0] + (2.0f * fRec158[1]))) / fTemp213)) + ((fRec160[2] + (fRec160[0] + (2.0f * fRec160[1]))) / fTemp217)));
			float fTemp219 = std::fabs((0.25f * fTemp218));
			fRec164[0] = std::max<float>(fTemp219, ((fRec164[1] * fSlow28) + (fTemp219 * fSlow29)));
			fRec163[0] = ((fRec163[1] * fSlow25) + (fRec164[0] * fSlow30));
			fRec162[0] = ((fConst30 * fRec162[1]) + (fSlow22 * std::max<float>(((20.0f * std::log10(fRec163[0])) - fSlow31), 0.0f)));
			float fTemp220 = (0.25f * (fTemp218 * std::pow(10.0f, (0.0500000007f * fRec162[0]))));
			fRec165[0] = (fTemp220 - (fSlow35 * ((fSlow36 * fRec165[2]) + (fSlow37 * fRec165[1]))));
			float fTemp221 = (fSlow11 * fRec153[1]);
			fRec153[0] = (fTemp220 - ((fSlow35 * (fRec165[2] + (fRec165[0] + (2.0f * fRec165[1])))) + (((fRec153[2] * fSlow45) + fTemp221) / fSlow46)));
			float fTemp222 = (fSlow8 * fRec152[1]);
			fRec152[0] = ((((fTemp221 + (fRec153[0] * fSlow48)) + (fRec153[2] * fSlow49)) / fSlow46) - (((fRec152[2] * fSlow57) + fTemp222) / fSlow58));
			fRec151[0] = ((((fTemp222 + (fRec152[0] * fSlow60)) + (fRec152[2] * fSlow61)) / fSlow58) - (fSlow62 * ((fSlow63 * fRec151[2]) + (fSlow64 * fRec151[1]))));
			float fTemp223 = (fSlow5 * (fRec151[2] + (fRec151[0] + (2.0f * fRec151[1]))));
			fVec24[(IOTA & 2047)] = (fTemp106 + (fTemp196 + (fTemp132 + fTemp223)));
			float fTemp224 = (0.300000012f * fVec24[((IOTA - iConst59) & 2047)]);
			float fTemp225 = (fVec23[((IOTA - iConst115) & 16383)] - (fTemp224 + (0.600000024f * fRec147[1])));
			fVec25[(IOTA & 2047)] = fTemp225;
			fRec147[0] = fVec25[((IOTA - iConst116) & 2047)];
			float fRec148 = (0.600000024f * fTemp225);
			fRec169[0] = (0.0f - (fConst14 * ((fConst15 * fRec169[1]) - (fRec3[1] + fRec3[2]))));
			fRec168[0] = ((fConst125 * fRec168[1]) + (fConst126 * (fRec3[1] + (fConst127 * fRec169[0]))));
			fVec26[(IOTA & 16383)] = ((0.353553385f * fRec168[0]) + 9.99999968e-21f);
			float fTemp226 = (fVec26[((IOTA - iConst129) & 16383)] - (fTemp224 + (0.600000024f * fRec166[1])));
			fVec27[(IOTA & 4095)] = fTemp226;
			fRec166[0] = fVec27[((IOTA - iConst130) & 4095)];
			float fRec167 = (0.600000024f * fTemp226);
			fRec173[0] = (0.0f - (fConst14 * ((fConst15 * fRec173[1]) - (fRec5[1] + fRec5[2]))));
			fRec172[0] = ((fConst139 * fRec172[1]) + (fConst140 * (fRec5[1] + (fConst141 * fRec173[0]))));
			fVec28[(IOTA & 16383)] = ((0.353553385f * fRec172[0]) + 9.99999968e-21f);
			float fTemp227 = ((fTemp224 + fVec28[((IOTA - iConst143) & 16383)]) - (0.600000024f * fRec170[1]));
			fVec29[(IOTA & 4095)] = fTemp227;
			fRec170[0] = fVec29[((IOTA - iConst144) & 4095)];
			float fRec171 = (0.600000024f * fTemp227);
			fRec177[0] = (0.0f - (fConst14 * ((fConst15 * fRec177[1]) - (fRec1[1] + fRec1[2]))));
			fRec176[0] = ((fConst153 * fRec176[1]) + (fConst154 * (fRec1[1] + (fConst155 * fRec177[0]))));
			fVec30[(IOTA & 16383)] = ((0.353553385f * fRec176[0]) + 9.99999968e-21f);
			float fTemp228 = ((fVec30[((IOTA - iConst157) & 16383)] + fTemp224) - (0.600000024f * fRec174[1]));
			fVec31[(IOTA & 2047)] = fTemp228;
			fRec174[0] = fVec31[((IOTA - iConst158) & 2047)];
			float fRec175 = (0.600000024f * fTemp228);
			float fTemp229 = (fRec175 + fRec171);
			float fTemp230 = (fRec148 + (fRec167 + fTemp229));
			fRec1[0] = (fRec9[1] + (fRec135[1] + (fRec139[1] + (fRec143[1] + (fRec147[1] + (fRec166[1] + (fRec170[1] + (fRec174[1] + (fRec10 + (fRec136 + (fRec140 + (fRec144 + fTemp230))))))))))));
			fRec2[0] = ((fRec147[1] + (fRec166[1] + (fRec170[1] + (fRec174[1] + fTemp230)))) - (fRec9[1] + (fRec135[1] + (fRec139[1] + (fRec143[1] + (fRec10 + (fRec136 + (fRec144 + fRec140))))))));
			float fTemp231 = (fRec167 + fRec148);
			fRec3[0] = ((fRec139[1] + (fRec143[1] + (fRec170[1] + (fRec174[1] + (fRec140 + (fRec144 + fTemp229)))))) - (fRec9[1] + (fRec135[1] + (fRec147[1] + (fRec166[1] + (fRec10 + (fRec136 + fTemp231)))))));
			fRec4[0] = ((fRec9[1] + (fRec135[1] + (fRec170[1] + (fRec174[1] + (fRec10 + (fRec136 + fTemp229)))))) - (fRec139[1] + (fRec143[1] + (fRec147[1] + (fRec166[1] + (fRec140 + (fRec144 + fTemp231)))))));
			float fTemp232 = (fRec175 + fRec167);
			float fTemp233 = (fRec171 + fRec148);
			fRec5[0] = ((fRec135[1] + (fRec143[1] + (fRec166[1] + (fRec174[1] + (fRec136 + (fRec144 + fTemp232)))))) - (fRec9[1] + (fRec139[1] + (fRec147[1] + (fRec170[1] + (fRec10 + (fRec140 + fTemp233)))))));
			fRec6[0] = ((fRec9[1] + (fRec139[1] + (fRec166[1] + (fRec174[1] + (fRec10 + (fRec140 + fTemp232)))))) - (fRec135[1] + (fRec143[1] + (fRec147[1] + (fRec170[1] + (fRec136 + (fRec144 + fTemp233)))))));
			float fTemp234 = (fRec175 + fRec148);
			float fTemp235 = (fRec171 + fRec167);
			fRec7[0] = ((fRec9[1] + (fRec143[1] + (fRec147[1] + (fRec174[1] + (fRec10 + (fRec144 + fTemp234)))))) - (fRec135[1] + (fRec139[1] + (fRec166[1] + (fRec170[1] + (fRec136 + (fRec140 + fTemp235)))))));
			fRec8[0] = ((fRec135[1] + (fRec139[1] + (fRec147[1] + (fRec174[1] + (fRec136 + (fRec140 + fTemp234)))))) - (fRec9[1] + (fRec143[1] + (fRec166[1] + (fRec170[1] + (fRec10 + (fRec144 + fTemp235)))))));
			fRec178[0] = (fConst162 + (fConst49 * fRec178[1]));
			float fTemp236 = std::tan((fConst18 * fRec178[0]));
			float fTemp237 = mydsp_faustpower2_f(std::sqrt((fConst161 * fTemp236)));
			float fTemp238 = ((fConst160 * fTemp237) + -8.0f);
			float fTemp239 = (fConst159 * fTemp237);
			float fTemp240 = ((fConst164 * fTemp236) - (fConst165 * (fTemp237 / fTemp236)));
			float fTemp241 = (fConst163 * fTemp240);
			float fTemp242 = (fTemp239 + (4.0f - fTemp241));
			float fTemp243 = ((fTemp239 + fTemp241) + 4.0f);
			fRec0[0] = ((0.370000005f * (fRec2[0] + fRec3[0])) - (((fRec0[1] * fTemp238) + (fRec0[2] * fTemp242)) / fTemp243));
			float fTemp244 = (0.0f - (fConst163 * (fTemp240 / fTemp243)));
			fRec184[0] = (fTemp136 - (fSlow371 * ((fSlow374 * fRec184[2]) + (fSlow375 * fRec184[1]))));
			fRec183[0] = ((fSlow371 * (((fSlow373 * fRec184[0]) + (fSlow376 * fRec184[1])) + (fSlow373 * fRec184[2]))) - (fSlow368 * ((fSlow377 * fRec183[2]) + (fSlow378 * fRec183[1]))));
			fVec32[0] = fSlow380;
			float fTemp245 = (fSlow380 - fVec32[1]);
			fVec33[0] = fSlow382;
			float fTemp246 = (0.0f - (fSlow380 + fVec33[1]));
			float fTemp247 = std::min<float>(1.0f, ((fTemp245 * float((fTemp245 > 0.0f))) + (fSlow381 * (fTemp246 * float((fTemp246 > 0.0f))))));
			fVec34[0] = fTemp247;
			iRec185[0] = (((iRec185[1] + (iRec185[1] > 0)) * (fTemp247 <= fVec34[1])) + (fTemp247 > fVec34[1]));
			float fTemp248 = (fSlow379 * float(iRec185[0]));
			float fTemp249 = (fSlow368 * ((fRec183[2] + (fRec183[0] + (2.0f * fRec183[1]))) * std::max<float>(0.0f, std::min<float>(fTemp248, (2.0f - fTemp248)))));
			fRec186[0] = (int(fTemp247) ? fSlow380 : fRec186[1]);
			fRec182[0] = (fTemp249 - ((fConst168 * (fRec182[1] * std::cos((fConst32 * (fRec186[0] + 50.0f))))) + (fConst169 * fRec182[2])));
			fRec187[0] = (fTemp249 - ((fConst171 * (fRec187[1] * std::cos((fConst32 * (fRec186[0] + 250.0f))))) + (fConst172 * fRec187[2])));
			fRec188[0] = (fTemp249 - ((fConst174 * (fRec188[1] * std::cos((fConst32 * (fRec186[0] + 450.0f))))) + (fConst175 * fRec188[2])));
			fRec189[0] = (fTemp249 - ((fConst177 * (fRec189[1] * std::cos((fConst32 * (fRec186[0] + 650.0f))))) + (fConst178 * fRec189[2])));
			fRec190[0] = (fTemp249 - ((fConst180 * (fRec190[1] * std::cos((fConst32 * (fRec186[0] + 850.0f))))) + (fConst181 * fRec190[2])));
			fRec191[0] = (fTemp249 - ((fConst183 * (fRec191[1] * std::cos((fConst32 * (fRec186[0] + 1050.0f))))) + (fConst184 * fRec191[2])));
			fRec192[0] = (fTemp249 - ((fConst186 * (fRec192[1] * std::cos((fConst32 * (fRec186[0] + 1250.0f))))) + (fConst187 * fRec192[2])));
			fRec193[0] = (fTemp249 - ((fConst189 * (fRec193[1] * std::cos((fConst32 * (fRec186[0] + 1450.0f))))) + (fConst190 * fRec193[2])));
			fRec194[0] = (fTemp249 - ((fConst192 * (fRec194[1] * std::cos((fConst32 * (fRec186[0] + 1650.0f))))) + (fConst193 * fRec194[2])));
			fRec195[0] = (fTemp249 - ((fConst195 * (fRec195[1] * std::cos((fConst32 * (fRec186[0] + 1850.0f))))) + (fConst196 * fRec195[2])));
			fRec196[0] = (fTemp249 - ((fConst198 * (fRec196[1] * std::cos((fConst32 * (fRec186[0] + 2050.0f))))) + (fConst199 * fRec196[2])));
			fRec197[0] = (fTemp249 - ((fConst201 * (fRec197[1] * std::cos((fConst32 * (fRec186[0] + 2250.0f))))) + (fConst202 * fRec197[2])));
			fRec198[0] = (fTemp249 - ((fConst204 * (fRec198[1] * std::cos((fConst32 * (fRec186[0] + 2450.0f))))) + (fConst205 * fRec198[2])));
			fRec199[0] = (fTemp249 - ((fConst207 * (fRec199[1] * std::cos((fConst32 * (fRec186[0] + 2650.0f))))) + (fConst208 * fRec199[2])));
			fRec200[0] = (fTemp249 - ((fConst210 * (fRec200[1] * std::cos((fConst32 * (fRec186[0] + 2850.0f))))) + (fConst211 * fRec200[2])));
			fRec201[0] = (fTemp249 - ((fConst213 * (fRec201[1] * std::cos((fConst32 * (fRec186[0] + 3050.0f))))) + (fConst214 * fRec201[2])));
			fRec202[0] = (fTemp249 - ((fConst216 * (fRec202[1] * std::cos((fConst32 * (fRec186[0] + 3250.0f))))) + (fConst217 * fRec202[2])));
			fRec203[0] = (fTemp249 - ((fConst219 * (fRec203[1] * std::cos((fConst32 * (fRec186[0] + 3450.0f))))) + (fConst220 * fRec203[2])));
			fRec204[0] = (fTemp249 - ((fConst222 * (fRec204[1] * std::cos((fConst32 * (fRec186[0] + 3650.0f))))) + (fConst223 * fRec204[2])));
			fRec205[0] = (fTemp249 - ((fConst225 * (fRec205[1] * std::cos((fConst32 * (fRec186[0] + 3850.0f))))) + (fConst226 * fRec205[2])));
			float fTemp250 = ((((((((((((((((((((fRec182[0] + (0.25f * (fRec187[0] - fRec187[2]))) + (0.111111112f * (fRec188[0] - fRec188[2]))) + (0.0625f * (fRec189[0] - fRec189[2]))) + (0.0399999991f * (fRec190[0] - fRec190[2]))) + (0.027777778f * (fRec191[0] - fRec191[2]))) + (0.0204081628f * (fRec192[0] - fRec192[2]))) + (0.015625f * (fRec193[0] - fRec193[2]))) + (0.0123456791f * (fRec194[0] - fRec194[2]))) + (0.00999999978f * (fRec195[0] - fRec195[2]))) + (0.00826446246f * (fRec196[0] - fRec196[2]))) + (0.0069444445f * (fRec197[0] - fRec197[2]))) + (0.00591715984f * (fRec198[0] - fRec198[2]))) + (0.00510204071f * (fRec199[0] - fRec199[2]))) + (0.00444444455f * (fRec200[0] - fRec200[2]))) + (0.00390625f * (fRec201[0] - fRec201[2]))) + (0.00346020772f * (fRec202[0] - fRec202[2]))) + (0.00308641978f * (fRec203[0] - fRec203[2]))) + (0.00277008303f * (fRec204[0] - fRec204[2]))) + (0.00249999994f * (fRec205[0] - fRec205[2]))) - fRec182[2]);
			float fTemp251 = std::fabs((fSlow364 * fTemp250));
			fRec208[0] = std::max<float>(fTemp251, ((fRec208[1] * fSlow389) + (fTemp251 * fSlow390)));
			fRec207[0] = ((fRec207[1] * fSlow386) + (fRec208[0] * fSlow391));
			fRec206[0] = ((fConst30 * fRec206[1]) + (fSlow383 * std::max<float>(((20.0f * std::log10(fRec207[0])) - fSlow392), 0.0f)));
			float fTemp252 = (fSlow364 * (fTemp250 * std::pow(10.0f, (0.0500000007f * fRec206[0]))));
			fRec209[0] = (fTemp252 - (fSlow396 * ((fSlow397 * fRec209[2]) + (fSlow398 * fRec209[1]))));
			float fTemp253 = (fSlow362 * fRec181[1]);
			fRec181[0] = (fTemp252 - ((fSlow396 * (fRec209[2] + (fRec209[0] + (2.0f * fRec209[1])))) + (((fRec181[2] * fSlow406) + fTemp253) / fSlow407)));
			float fTemp254 = (fSlow359 * fRec180[1]);
			fRec180[0] = ((((fTemp253 + (fRec181[0] * fSlow409)) + (fRec181[2] * fSlow410)) / fSlow407) - (((fRec180[2] * fSlow418) + fTemp254) / fSlow419));
			fRec179[0] = ((((fTemp254 + (fRec180[0] * fSlow421)) + (fRec180[2] * fSlow422)) / fSlow419) - (fSlow423 * ((fSlow424 * fRec179[2]) + (fSlow425 * fRec179[1]))));
			float fTemp255 = ((fSlow356 * (fRec179[2] + (fRec179[0] + (2.0f * fRec179[1])))) + fTemp132);
			fRec213[0] = ((0.999000013f * fRec213[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp4, ((fConst227 * fTemp5) + 1.0f))))));
			fRec215[0] = (iTemp6 ? fSlow437 : fRec215[1]);
			float fTemp256 = (fRec219[1] + (fSlow438 * fRec215[0]));
			fRec219[0] = (fTemp256 - std::floor(fTemp256));
			float fTemp257 = (fRec218[1] + (fConst29 * ((315.0f * (fRec213[0] * ftbl0mydspSIG0[int((65536.0f * fRec219[0]))])) + (fSlow439 * fRec215[0]))));
			fRec218[0] = (fTemp257 - std::floor(fTemp257));
			float fTemp258 = (fRec217[1] + (fConst29 * ((315.0f * (fRec213[0] * ftbl0mydspSIG0[int((65536.0f * fRec218[0]))])) + (fSlow440 * fRec215[0]))));
			fRec217[0] = (fTemp258 - std::floor(fTemp258));
			float fTemp259 = (fRec216[1] + (fConst29 * ((315.0f * (fRec213[0] * ftbl0mydspSIG0[int((65536.0f * fRec217[0]))])) + (fSlow441 * fRec215[0]))));
			fRec216[0] = (fTemp259 - std::floor(fTemp259));
			float fTemp260 = (fRec214[1] + (fConst29 * ((fSlow16 * fRec215[0]) + (315.0f * (fRec213[0] * ftbl0mydspSIG0[int((65536.0f * fRec216[0]))])))));
			fRec214[0] = (fTemp260 - std::floor(fTemp260));
			float fTemp261 = ftbl0mydspSIG0[int((65536.0f * fRec214[0]))];
			float fTemp262 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp261)));
			float fTemp263 = (fRec213[0] * (fTemp261 + (0.0500000007f * (fTemp262 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp262)))))));
			float fTemp264 = std::fabs((0.25f * fTemp263));
			fRec222[0] = std::max<float>(fTemp264, ((fRec222[1] * fSlow448) + (fTemp264 * fSlow449)));
			fRec221[0] = ((fRec221[1] * fSlow445) + (fRec222[0] * fSlow450));
			fRec220[0] = ((fConst30 * fRec220[1]) + (fSlow442 * std::max<float>(((20.0f * std::log10(fRec221[0])) - fSlow451), 0.0f)));
			float fTemp265 = (0.25f * (fTemp263 * std::pow(10.0f, (0.0500000007f * fRec220[0]))));
			fRec223[0] = (fTemp265 - (fSlow455 * ((fSlow456 * fRec223[2]) + (fSlow457 * fRec223[1]))));
			float fTemp266 = (fSlow436 * fRec212[1]);
			fRec212[0] = (fTemp265 - ((fSlow455 * (fRec223[2] + (fRec223[0] + (2.0f * fRec223[1])))) + (((fRec212[2] * fSlow465) + fTemp266) / fSlow466)));
			float fTemp267 = (fSlow433 * fRec211[1]);
			fRec211[0] = ((((fTemp266 + (fRec212[0] * fSlow468)) + (fRec212[2] * fSlow469)) / fSlow466) - (((fRec211[2] * fSlow477) + fTemp267) / fSlow478));
			fRec210[0] = ((((fTemp267 + (fRec211[0] * fSlow480)) + (fRec211[2] * fSlow481)) / fSlow478) - (fSlow482 * ((fSlow483 * fRec210[2]) + (fSlow484 * fRec210[1]))));
			float fTemp268 = (fSlow430 * (fRec210[2] + (fRec210[0] + (2.0f * fRec210[1]))));
			fRec229[0] = (fTemp136 - (fConst235 * ((fConst238 * fRec229[2]) + (fConst239 * fRec229[1]))));
			fRec228[0] = ((fConst235 * (((fConst237 * fRec229[0]) + (fConst240 * fRec229[1])) + (fConst237 * fRec229[2]))) - (fConst232 * ((fConst241 * fRec228[2]) + (fConst242 * fRec228[1]))));
			fVec35[0] = fSlow496;
			float fTemp269 = (fSlow496 - fVec35[1]);
			fVec36[0] = fSlow498;
			float fTemp270 = (0.0f - (fSlow496 + fVec36[1]));
			float fTemp271 = std::min<float>(1.0f, ((fTemp269 * float((fTemp269 > 0.0f))) + (fSlow497 * (fTemp270 * float((fTemp270 > 0.0f))))));
			fVec37[0] = fTemp271;
			iRec230[0] = (((iRec230[1] + (iRec230[1] > 0)) * (fTemp271 <= fVec37[1])) + (fTemp271 > fVec37[1]));
			float fTemp272 = float(iRec230[0]);
			float fTemp273 = (fConst243 * fTemp272);
			float fTemp274 = (fConst232 * ((fRec228[2] + (fRec228[0] + (2.0f * fRec228[1]))) * std::max<float>(0.0f, std::min<float>(fTemp273, (2.0f - fTemp273)))));
			fRec227[0] = (fTemp274 - ((fConst245 * fRec227[1]) + (fConst246 * fRec227[2])));
			fRec231[0] = (fTemp274 - ((fConst249 * fRec231[1]) + (fConst250 * fRec231[2])));
			fRec232[0] = (fTemp274 - ((fConst253 * fRec232[1]) + (fConst254 * fRec232[2])));
			fRec233[0] = (fTemp274 - ((fConst257 * fRec233[1]) + (fConst258 * fRec233[2])));
			fRec234[0] = (fTemp274 - ((fConst261 * fRec234[1]) + (fConst262 * fRec234[2])));
			fRec235[0] = (fTemp274 - ((fConst265 * fRec235[1]) + (fConst266 * fRec235[2])));
			fRec236[0] = (fTemp274 - ((fConst269 * fRec236[1]) + (fConst270 * fRec236[2])));
			fRec237[0] = (fTemp274 - ((fConst273 * fRec237[1]) + (fConst274 * fRec237[2])));
			fRec238[0] = (fTemp274 - ((fConst277 * fRec238[1]) + (fConst278 * fRec238[2])));
			fRec239[0] = (fTemp274 - ((fConst281 * fRec239[1]) + (fConst282 * fRec239[2])));
			fRec240[0] = (fTemp274 - ((fConst285 * fRec240[1]) + (fConst286 * fRec240[2])));
			fRec241[0] = (fTemp274 - ((fConst289 * fRec241[1]) + (fConst290 * fRec241[2])));
			fRec242[0] = (fTemp274 - ((fConst293 * fRec242[1]) + (fConst294 * fRec242[2])));
			fRec243[0] = (fTemp274 - ((fConst297 * fRec243[1]) + (fConst298 * fRec243[2])));
			fRec244[0] = (fTemp274 - ((fConst301 * fRec244[1]) + (fConst302 * fRec244[2])));
			fRec245[0] = (fTemp274 - ((fConst305 * fRec245[1]) + (fConst306 * fRec245[2])));
			fRec246[0] = (fTemp274 - ((fConst309 * fRec246[1]) + (fConst310 * fRec246[2])));
			fRec247[0] = (fTemp274 - ((fConst313 * fRec247[1]) + (fConst314 * fRec247[2])));
			fRec248[0] = (fTemp274 - ((fConst317 * fRec248[1]) + (fConst318 * fRec248[2])));
			fRec249[0] = (fTemp274 - ((fConst321 * fRec249[1]) + (fConst322 * fRec249[2])));
			fRec250[0] = (fTemp274 - ((fConst325 * fRec250[1]) + (fConst326 * fRec250[2])));
			fRec251[0] = (fTemp274 - ((fConst329 * fRec251[1]) + (fConst330 * fRec251[2])));
			fRec252[0] = (fTemp274 - ((fConst333 * fRec252[1]) + (fConst334 * fRec252[2])));
			fRec253[0] = (fTemp274 - ((fConst337 * fRec253[1]) + (fConst338 * fRec253[2])));
			fRec254[0] = (fTemp274 - ((fConst341 * fRec254[1]) + (fConst342 * fRec254[2])));
			fRec255[0] = (fTemp274 - ((fConst345 * fRec255[1]) + (fConst346 * fRec255[2])));
			fRec256[0] = (fTemp274 - ((fConst349 * fRec256[1]) + (fConst350 * fRec256[2])));
			fRec257[0] = (fTemp274 - ((fConst353 * fRec257[1]) + (fConst354 * fRec257[2])));
			fRec258[0] = (fTemp274 - ((fConst357 * fRec258[1]) + (fConst358 * fRec258[2])));
			fRec259[0] = (fTemp274 - ((fConst361 * fRec259[1]) + (fConst362 * fRec259[2])));
			fRec260[0] = (fTemp274 - ((fConst365 * fRec260[1]) + (fConst366 * fRec260[2])));
			fRec261[0] = (fTemp274 - ((fConst369 * fRec261[1]) + (fConst370 * fRec261[2])));
			fRec262[0] = (fTemp274 - ((fConst373 * fRec262[1]) + (fConst374 * fRec262[2])));
			fRec263[0] = (fTemp274 - ((fConst377 * fRec263[1]) + (fConst378 * fRec263[2])));
			fRec264[0] = (fTemp274 - ((fConst381 * fRec264[1]) + (fConst382 * fRec264[2])));
			fRec265[0] = (fTemp274 - ((fConst385 * fRec265[1]) + (fConst386 * fRec265[2])));
			fRec266[0] = (fTemp274 - ((fConst389 * fRec266[1]) + (fConst390 * fRec266[2])));
			fRec267[0] = (fTemp274 - ((fConst393 * fRec267[1]) + (fConst394 * fRec267[2])));
			fRec268[0] = (fTemp274 - ((fConst397 * fRec268[1]) + (fConst398 * fRec268[2])));
			fRec269[0] = (fTemp274 - ((fConst401 * fRec269[1]) + (fConst402 * fRec269[2])));
			fRec270[0] = (fTemp274 - ((fConst405 * fRec270[1]) + (fConst406 * fRec270[2])));
			fRec271[0] = (fTemp274 - ((fConst409 * fRec271[1]) + (fConst410 * fRec271[2])));
			fRec272[0] = (fTemp274 - ((fConst413 * fRec272[1]) + (fConst414 * fRec272[2])));
			fRec273[0] = (fTemp274 - ((fConst417 * fRec273[1]) + (fConst418 * fRec273[2])));
			fRec274[0] = (fTemp274 - ((fConst421 * fRec274[1]) + (fConst422 * fRec274[2])));
			fRec275[0] = (fTemp274 - ((fConst425 * fRec275[1]) + (fConst426 * fRec275[2])));
			fRec276[0] = (fTemp274 - ((fConst429 * fRec276[1]) + (fConst430 * fRec276[2])));
			fRec277[0] = (fTemp274 - ((fConst433 * fRec277[1]) + (fConst434 * fRec277[2])));
			fRec278[0] = (fTemp274 - ((fConst437 * fRec278[1]) + (fConst438 * fRec278[2])));
			fRec279[0] = (fTemp274 - ((fConst441 * fRec279[1]) + (fConst442 * fRec279[2])));
			fRec280[0] = (fSlow500 + (0.999000013f * fRec280[1]));
			fRec281[0] = (fSlow501 + (fRec281[1] - std::floor((fSlow501 + fRec281[1]))));
			fRec282[0] = (fSlow503 + (0.999000013f * fRec282[1]));
			fRec283[0] = (fSlow504 + (fRec283[1] - std::floor((fSlow504 + fRec283[1]))));
			fRec284[0] = (fSlow506 + (0.999000013f * fRec284[1]));
			fRec285[0] = (fSlow507 + (fRec285[1] - std::floor((fSlow507 + fRec285[1]))));
			float fTemp275 = ((0.0199999996f * (((fConst229 * (fRec227[0] - fRec227[2])) + (((((((((((((((((((((((((((((((((((((((((((((((((fConst247 * (fRec231[0] - fRec231[2])) + (fConst251 * (fRec232[0] - fRec232[2]))) + (fConst255 * (fRec233[0] - fRec233[2]))) + (fConst259 * (fRec234[0] - fRec234[2]))) + (fConst263 * (fRec235[0] - fRec235[2]))) + (fConst267 * (fRec236[0] - fRec236[2]))) + (fConst271 * (fRec237[0] - fRec237[2]))) + (fConst275 * (fRec238[0] - fRec238[2]))) + (fConst279 * (fRec239[0] - fRec239[2]))) + (fConst283 * (fRec240[0] - fRec240[2]))) + (fConst287 * (fRec241[0] - fRec241[2]))) + (fConst291 * (fRec242[0] - fRec242[2]))) + (fConst295 * (fRec243[0] - fRec243[2]))) + (fConst299 * (fRec244[0] - fRec244[2]))) + (fConst303 * (fRec245[0] - fRec245[2]))) + (fConst307 * (fRec246[0] - fRec246[2]))) + (fConst311 * (fRec247[0] - fRec247[2]))) + (fConst315 * (fRec248[0] - fRec248[2]))) + (fConst319 * (fRec249[0] - fRec249[2]))) + (fConst323 * (fRec250[0] - fRec250[2]))) + (fConst327 * (fRec251[0] - fRec251[2]))) + (fConst331 * (fRec252[0] - fRec252[2]))) + (fConst335 * (fRec253[0] - fRec253[2]))) + (fConst339 * (fRec254[0] - fRec254[2]))) + (fConst343 * (fRec255[0] - fRec255[2]))) + (fConst347 * (fRec256[0] - fRec256[2]))) + (fConst351 * (fRec257[0] - fRec257[2]))) + (fConst355 * (fRec258[0] - fRec258[2]))) + (fConst359 * (fRec259[0] - fRec259[2]))) + (fConst363 * (fRec260[0] - fRec260[2]))) + (fConst367 * (fRec261[0] - fRec261[2]))) + (fConst371 * (fRec262[0] - fRec262[2]))) + (fConst375 * (fRec263[0] - fRec263[2]))) + (fConst379 * (fRec264[0] - fRec264[2]))) + (fConst383 * (fRec265[0] - fRec265[2]))) + (fConst387 * (fRec266[0] - fRec266[2]))) + (fConst391 * (fRec267[0] - fRec267[2]))) + (fConst395 * (fRec268[0] - fRec268[2]))) + (fConst399 * (fRec269[0] - fRec269[2]))) + (fConst403 * (fRec270[0] - fRec270[2]))) + (fConst407 * (fRec271[0] - fRec271[2]))) + (fConst411 * (fRec272[0] - fRec272[2]))) + (fConst415 * (fRec273[0] - fRec273[2]))) + (fConst419 * (fRec274[0] - fRec274[2]))) + (fConst423 * (fRec275[0] - fRec275[2]))) + (fConst427 * (fRec276[0] - fRec276[2]))) + (fConst431 * (fRec277[0] - fRec277[2]))) + (fConst435 * (fRec278[0] - fRec278[2]))) + (fConst439 * (fRec279[0] - fRec279[2])))) * std::max<float>(0.0f, std::min<float>((fConst22 * fTemp272), ((fConst443 * (fConst21 - fTemp272)) + 1.0f))))) + (2.0f * (((fRec280[0] * ftbl0mydspSIG0[int((65536.0f * fRec281[0]))]) + (fRec282[0] * ftbl0mydspSIG0[int((65536.0f * fRec283[0]))])) + (fRec284[0] * ftbl0mydspSIG0[int((65536.0f * fRec285[0]))]))));
			float fTemp276 = std::fabs((0.25f * fTemp275));
			fRec288[0] = std::max<float>(fTemp276, ((fRec288[1] * fSlow514) + (fTemp276 * fSlow515)));
			fRec287[0] = ((fRec287[1] * fSlow511) + (fRec288[0] * fSlow516));
			fRec286[0] = ((fConst30 * fRec286[1]) + (fSlow508 * std::max<float>(((20.0f * std::log10(fRec287[0])) - fSlow517), 0.0f)));
			float fTemp277 = (0.25f * (fTemp275 * std::pow(10.0f, (0.0500000007f * fRec286[0]))));
			fRec289[0] = (fTemp277 - (fSlow521 * ((fSlow522 * fRec289[2]) + (fSlow523 * fRec289[1]))));
			float fTemp278 = (fSlow495 * fRec226[1]);
			fRec226[0] = (fTemp277 - ((fSlow521 * (fRec289[2] + (fRec289[0] + (2.0f * fRec289[1])))) + (((fRec226[2] * fSlow531) + fTemp278) / fSlow532)));
			float fTemp279 = (fSlow492 * fRec225[1]);
			fRec225[0] = ((((fTemp278 + (fRec226[0] * fSlow534)) + (fRec226[2] * fSlow535)) / fSlow532) - (((fRec225[2] * fSlow543) + fTemp279) / fSlow544));
			fRec224[0] = ((((fTemp279 + (fRec225[0] * fSlow546)) + (fRec225[2] * fSlow547)) / fSlow544) - (fSlow548 * ((fSlow549 * fRec224[2]) + (fSlow550 * fRec224[1]))));
			float fTemp280 = (fSlow489 * (fRec224[2] + (fRec224[0] + (2.0f * fRec224[1]))));
			float fTemp281 = ((fRec0[2] * fTemp244) + ((((((fTemp255 + fTemp223) + fTemp196) + fTemp268) + fTemp106) + fTemp280) + (fConst163 * ((fRec0[0] * fTemp240) / fTemp243))));
			float fTemp282 = std::fabs(fTemp281);
			fRec292[0] = std::max<float>(fTemp282, ((fConst446 * fRec292[1]) + (fConst447 * fTemp282)));
			fRec291[0] = ((fConst444 * fRec291[1]) + (fConst445 * fRec292[0]));
			fRec290[0] = ((fConst30 * fRec290[1]) + (fConst31 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec291[0]) + 1.0f)), 0.0f)))));
			float fTemp283 = (fTemp281 * std::pow(10.0f, (0.0500000007f * fRec290[0])));
			float fTemp284 = std::fabs(fTemp283);
			fRec295[0] = std::max<float>(fTemp284, ((fConst450 * fRec295[1]) + (fConst451 * fTemp284)));
			fRec294[0] = ((fConst448 * fRec294[1]) + (fConst449 * fRec295[0]));
			fRec293[0] = ((fConst30 * fRec293[1]) + (fConst31 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec294[0])) + 5.0f), 0.0f)))));
			float fTemp285 = (fTemp283 * std::pow(10.0f, (0.0500000007f * fRec293[0])));
			float fTemp286 = std::fabs((fSlow0 * fTemp285));
			fRec298[0] = std::max<float>(fTemp286, ((fConst30 * fRec298[1]) + (fConst31 * fTemp286)));
			fRec297[0] = ((fConst49 * fRec297[1]) + (fConst50 * fRec298[0]));
			fRec296[0] = ((fConst30 * fRec296[1]) + (fConst31 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec297[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp285 * std::pow(10.0f, (0.0500000007f * fRec296[0])))))));
			fRec299[0] = ((0.370000005f * (fRec2[0] - fRec3[0])) - (((fTemp238 * fRec299[1]) + (fTemp242 * fRec299[2])) / fTemp243));
			float fTemp287 = ((fTemp244 * fRec299[2]) + ((fTemp37 + (fTemp280 + (fTemp106 + (fTemp268 + (fTemp255 + fTemp196))))) + (fConst163 * ((fRec299[0] * fTemp240) / fTemp243))));
			float fTemp288 = std::fabs(fTemp287);
			fRec302[0] = std::max<float>(fTemp288, ((fConst446 * fRec302[1]) + (fConst447 * fTemp288)));
			fRec301[0] = ((fConst444 * fRec301[1]) + (fConst445 * fRec302[0]));
			fRec300[0] = ((fConst30 * fRec300[1]) + (fConst31 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec301[0]) + 1.0f)), 0.0f)))));
			float fTemp289 = (fTemp287 * std::pow(10.0f, (0.0500000007f * fRec300[0])));
			float fTemp290 = std::fabs(fTemp289);
			fRec305[0] = std::max<float>(fTemp290, ((fConst450 * fRec305[1]) + (fConst451 * fTemp290)));
			fRec304[0] = ((fConst448 * fRec304[1]) + (fConst449 * fRec305[0]));
			fRec303[0] = ((fConst30 * fRec303[1]) + (fConst31 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec304[0])) + 5.0f), 0.0f)))));
			float fTemp291 = (fTemp289 * std::pow(10.0f, (0.0500000007f * fRec303[0])));
			float fTemp292 = std::fabs((fSlow0 * fTemp291));
			fRec308[0] = std::max<float>(fTemp292, ((fConst30 * fRec308[1]) + (fConst31 * fTemp292)));
			fRec307[0] = ((fConst49 * fRec307[1]) + (fConst50 * fRec308[0]));
			fRec306[0] = ((fConst30 * fRec306[1]) + (fConst31 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec307[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp291 * std::pow(10.0f, (0.0500000007f * fRec306[0])))))));
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
			fVec7[1] = fVec7[0];
			fRec68[1] = fRec68[0];
			fRec61[1] = fRec61[0];
			fRec69[1] = fRec69[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec71[j0] = fRec71[(j0 - 1)];
			}
			fVec8[1] = fVec8[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			fRec57[1] = fRec57[0];
			fRec53[1] = fRec53[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec47[j1] = fRec47[(j1 - 1)];
			}
			fRec40[1] = fRec40[0];
			fRec73[1] = fRec73[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			iRec74[1] = iRec74[0];
			iRec75[1] = iRec75[0];
			fRec78[1] = fRec78[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec86[1] = fRec86[0];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec96[1] = fRec96[0];
			fRec95[1] = fRec95[0];
			fRec94[1] = fRec94[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			iRec116[1] = iRec116[0];
			fRec122[1] = fRec122[0];
			fRec123[1] = fRec123[0];
			fVec10[1] = fVec10[0];
			fVec11[1] = fVec11[0];
			fRec126[1] = fRec126[0];
			fRec125[1] = fRec125[0];
			fRec124[1] = fRec124[0];
			iRec128[1] = iRec128[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fRec129[1] = fRec129[0];
			fVec12[1] = fVec12[0];
			fVec14[1] = fVec14[0];
			fRec121[1] = fRec121[0];
			fRec108[1] = fRec108[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec101[1] = fRec101[0];
			fRec133[1] = fRec133[0];
			fRec132[1] = fRec132[0];
			fRec131[1] = fRec131[0];
			fRec134[2] = fRec134[1];
			fRec134[1] = fRec134[0];
			fRec100[2] = fRec100[1];
			fRec100[1] = fRec100[0];
			fRec99[2] = fRec99[1];
			fRec99[1] = fRec99[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec9[1] = fRec9[0];
			fRec138[1] = fRec138[0];
			fRec137[1] = fRec137[0];
			fRec135[1] = fRec135[0];
			fRec142[1] = fRec142[0];
			fRec141[1] = fRec141[0];
			fRec139[1] = fRec139[0];
			fRec146[1] = fRec146[0];
			fRec145[1] = fRec145[0];
			fRec143[1] = fRec143[0];
			fRec150[1] = fRec150[0];
			fRec149[1] = fRec149[0];
			iRec155[1] = iRec155[0];
			fRec154[2] = fRec154[1];
			fRec154[1] = fRec154[0];
			iRec157[1] = iRec157[0];
			fRec156[2] = fRec156[1];
			fRec156[1] = fRec156[0];
			iRec159[1] = iRec159[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			iRec161[1] = iRec161[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			fRec164[1] = fRec164[0];
			fRec163[1] = fRec163[0];
			fRec162[1] = fRec162[0];
			fRec165[2] = fRec165[1];
			fRec165[1] = fRec165[0];
			fRec153[2] = fRec153[1];
			fRec153[1] = fRec153[0];
			fRec152[2] = fRec152[1];
			fRec152[1] = fRec152[0];
			fRec151[2] = fRec151[1];
			fRec151[1] = fRec151[0];
			fRec147[1] = fRec147[0];
			fRec169[1] = fRec169[0];
			fRec168[1] = fRec168[0];
			fRec166[1] = fRec166[0];
			fRec173[1] = fRec173[0];
			fRec172[1] = fRec172[0];
			fRec170[1] = fRec170[0];
			fRec177[1] = fRec177[0];
			fRec176[1] = fRec176[0];
			fRec174[1] = fRec174[0];
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
			fRec178[1] = fRec178[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec184[2] = fRec184[1];
			fRec184[1] = fRec184[0];
			fRec183[2] = fRec183[1];
			fRec183[1] = fRec183[0];
			fVec32[1] = fVec32[0];
			fVec33[1] = fVec33[0];
			fVec34[1] = fVec34[0];
			iRec185[1] = iRec185[0];
			fRec186[1] = fRec186[0];
			fRec182[2] = fRec182[1];
			fRec182[1] = fRec182[0];
			fRec187[2] = fRec187[1];
			fRec187[1] = fRec187[0];
			fRec188[2] = fRec188[1];
			fRec188[1] = fRec188[0];
			fRec189[2] = fRec189[1];
			fRec189[1] = fRec189[0];
			fRec190[2] = fRec190[1];
			fRec190[1] = fRec190[0];
			fRec191[2] = fRec191[1];
			fRec191[1] = fRec191[0];
			fRec192[2] = fRec192[1];
			fRec192[1] = fRec192[0];
			fRec193[2] = fRec193[1];
			fRec193[1] = fRec193[0];
			fRec194[2] = fRec194[1];
			fRec194[1] = fRec194[0];
			fRec195[2] = fRec195[1];
			fRec195[1] = fRec195[0];
			fRec196[2] = fRec196[1];
			fRec196[1] = fRec196[0];
			fRec197[2] = fRec197[1];
			fRec197[1] = fRec197[0];
			fRec198[2] = fRec198[1];
			fRec198[1] = fRec198[0];
			fRec199[2] = fRec199[1];
			fRec199[1] = fRec199[0];
			fRec200[2] = fRec200[1];
			fRec200[1] = fRec200[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec202[2] = fRec202[1];
			fRec202[1] = fRec202[0];
			fRec203[2] = fRec203[1];
			fRec203[1] = fRec203[0];
			fRec204[2] = fRec204[1];
			fRec204[1] = fRec204[0];
			fRec205[2] = fRec205[1];
			fRec205[1] = fRec205[0];
			fRec208[1] = fRec208[0];
			fRec207[1] = fRec207[0];
			fRec206[1] = fRec206[0];
			fRec209[2] = fRec209[1];
			fRec209[1] = fRec209[0];
			fRec181[2] = fRec181[1];
			fRec181[1] = fRec181[0];
			fRec180[2] = fRec180[1];
			fRec180[1] = fRec180[0];
			fRec179[2] = fRec179[1];
			fRec179[1] = fRec179[0];
			fRec213[1] = fRec213[0];
			fRec215[1] = fRec215[0];
			fRec219[1] = fRec219[0];
			fRec218[1] = fRec218[0];
			fRec217[1] = fRec217[0];
			fRec216[1] = fRec216[0];
			fRec214[1] = fRec214[0];
			fRec222[1] = fRec222[0];
			fRec221[1] = fRec221[0];
			fRec220[1] = fRec220[0];
			fRec223[2] = fRec223[1];
			fRec223[1] = fRec223[0];
			fRec212[2] = fRec212[1];
			fRec212[1] = fRec212[0];
			fRec211[2] = fRec211[1];
			fRec211[1] = fRec211[0];
			fRec210[2] = fRec210[1];
			fRec210[1] = fRec210[0];
			fRec229[2] = fRec229[1];
			fRec229[1] = fRec229[0];
			fRec228[2] = fRec228[1];
			fRec228[1] = fRec228[0];
			fVec35[1] = fVec35[0];
			fVec36[1] = fVec36[0];
			fVec37[1] = fVec37[0];
			iRec230[1] = iRec230[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
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
			fRec279[2] = fRec279[1];
			fRec279[1] = fRec279[0];
			fRec280[1] = fRec280[0];
			fRec281[1] = fRec281[0];
			fRec282[1] = fRec282[0];
			fRec283[1] = fRec283[0];
			fRec284[1] = fRec284[0];
			fRec285[1] = fRec285[0];
			fRec288[1] = fRec288[0];
			fRec287[1] = fRec287[0];
			fRec286[1] = fRec286[0];
			fRec289[2] = fRec289[1];
			fRec289[1] = fRec289[0];
			fRec226[2] = fRec226[1];
			fRec226[1] = fRec226[0];
			fRec225[2] = fRec225[1];
			fRec225[1] = fRec225[0];
			fRec224[2] = fRec224[1];
			fRec224[1] = fRec224[0];
			fRec292[1] = fRec292[0];
			fRec291[1] = fRec291[0];
			fRec290[1] = fRec290[0];
			fRec295[1] = fRec295[0];
			fRec294[1] = fRec294[0];
			fRec293[1] = fRec293[0];
			fRec298[1] = fRec298[0];
			fRec297[1] = fRec297[0];
			fRec296[1] = fRec296[0];
			fRec299[2] = fRec299[1];
			fRec299[1] = fRec299[0];
			fRec302[1] = fRec302[0];
			fRec301[1] = fRec301[0];
			fRec300[1] = fRec300[0];
			fRec305[1] = fRec305[0];
			fRec304[1] = fRec304[0];
			fRec303[1] = fRec303[0];
			fRec308[1] = fRec308[0];
			fRec307[1] = fRec307[0];
			fRec306[1] = fRec306[0];
		}
	}

};

#endif
