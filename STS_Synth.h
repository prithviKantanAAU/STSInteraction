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
	
	int iRec37[2];
	
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
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			iRec37[l36] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec37[0] = (iRec37[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec37[0] + -1))));
			iRec37[1] = iRec37[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

const static float fmydspSIG1Wave0[250] = {1.0f,0.776724994f,0.625723004f,0.855223f,0.760159016f,0.69837302f,0.768010974f,0.64112699f,0.244034007f,0.707754016f,0.634012997f,0.247527003f,0.660848975f,0.450396001f,0.567782998f,0.106361002f,0.716813982f,0.663919985f,0.291207999f,0.310598999f,0.801495016f,0.635291994f,0.307435006f,0.874123991f,0.497667998f,0.487087995f,0.459114999f,0.733455002f,0.541818023f,0.441318005f,0.313919991f,0.40309f,0.685352981f,0.603139997f,0.400552005f,0.453511f,0.634386003f,0.291547f,0.131604999f,0.368506998f,0.839906991f,0.602159977f,0.288296014f,0.579670012f,0.0242493004f,0.262746006f,0.368588001f,0.890284002f,0.408962995f,0.556071997f,0.884427011f,0.832109988f,0.612015009f,0.757175982f,0.919476986f,1.0f,0.827962995f,0.89240998f,0.0357408002f,0.480789006f,0.75287199f,0.0546301007f,0.235936999f,0.362937987f,0.444471985f,0.101751f,0.703418016f,0.453135997f,0.316628993f,0.490393996f,0.982508004f,0.551621974f,0.602008998f,0.666957021f,0.776830018f,0.905662f,0.0987197012f,0.402967989f,0.829451978f,0.307644993f,0.640479982f,0.983971f,0.584204972f,0.650364995f,0.334446996f,0.583570004f,0.540190995f,0.672533989f,0.245711997f,0.687298f,0.883058012f,0.792949975f,0.600619018f,0.572682023f,0.122612f,0.388247997f,0.290657997f,0.380255014f,0.290966988f,0.567818999f,0.0737721026f,0.42098999f,0.0786577985f,0.393994987f,0.268983006f,0.260614008f,0.494085997f,0.238025993f,0.0987823978f,0.277879f,0.440562993f,0.0770211965f,0.450590998f,0.128137007f,0.0368275009f,0.128699005f,0.329605013f,0.374511987f,0.363590002f,0.272594005f,0.379052013f,0.305240989f,0.0741128996f,0.34572801f,0.299349993f,0.221284002f,0.0261390992f,0.293202013f,0.361885011f,0.114330001f,0.239004999f,0.434156001f,0.329582989f,0.219459996f,0.284175009f,0.198554993f,0.431975991f,0.302985013f,1.0f,0.146220997f,0.140700996f,0.264243007f,0.185996994f,0.426322013f,0.304780006f,0.343989998f,0.195429996f,0.386954993f,0.187600002f,0.172812f,0.0434115008f,0.303761005f,0.0694539994f,0.453943014f,0.832450986f,0.317817003f,0.940600991f,1.0f,0.180657998f,0.737921f,0.832297027f,0.402352005f,0.126785994f,0.594398022f,0.485455006f,0.324470013f,0.365101993f,0.777921975f,0.588271976f,0.401353002f,0.610734999f,0.158693001f,0.0746072009f,0.825098991f,0.925459027f,0.65377003f,0.260791987f,0.719384015f,0.559907973f,0.372590005f,0.360035002f,0.622938991f,0.210271001f,0.444595009f,0.311286002f,0.464309007f,0.557231009f,0.524079978f,0.0701055974f,0.320749015f,0.194460005f,0.727608979f,0.522062004f,0.394003987f,0.235035002f,0.395646006f,0.494796008f,0.51731801f,0.109751999f,0.69284898f,0.00632009003f,0.0207582992f,0.00306107011f,0.0637191013f,0.0816610008f,0.0351100005f,0.127813995f,0.202294007f,0.0764145032f,0.263126999f,0.400198996f,0.267277986f,0.633385003f,1.0f,0.73990202f,0.413762987f,0.418110013f,0.612715006f,0.67237401f,0.339673996f,0.211720005f,0.459645003f,0.102499999f,0.325890005f,0.148154005f,0.265442014f,0.0974304974f,0.286437988f,0.275213003f,0.109111004f,0.575088978f,0.370283008f,0.29411f,0.259826005f,0.0648718998f,0.583418012f,0.282662988f,0.182004005f,0.117421001f,0.417726994f,0.169650003f,0.24853f,0.122818999f,0.185486004f,0.0433618017f,0.373849005f,0.25276801f,0.195103005f,0.0927835032f,0.166543007f};
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
			fmydspSIG1Wave0_idx = ((1 + fmydspSIG1Wave0_idx) % 250);
		}
	}

};

static mydspSIG1* newmydspSIG1() { return (mydspSIG1*)new mydspSIG1(); }
static void deletemydspSIG1(mydspSIG1* dsp) { delete dsp; }

const static float fmydspSIG2Wave0[350] = {0.525285006f,0.814173996f,0.483260989f,0.296745002f,0.975055993f,0.472243994f,0.409500986f,0.425363988f,0.687559009f,0.28838101f,0.309285015f,0.123053998f,0.286332995f,0.576705992f,0.908321977f,0.626973987f,0.0801851973f,0.309834987f,0.451429993f,0.132844999f,0.470634997f,0.417008013f,0.265112013f,0.075280197f,0.463470012f,0.471810013f,0.275323987f,0.547026992f,0.512519002f,0.394077986f,0.595404029f,0.941305995f,0.392500997f,0.381435007f,0.391232014f,0.118923999f,0.339495003f,0.101420999f,0.241754994f,0.0873254985f,0.37894401f,0.637705028f,0.171946004f,0.149858996f,0.233290002f,0.541809976f,1.0f,0.115553997f,0.244172007f,0.574329019f,0.606171012f,0.93839699f,0.392554998f,0.277359009f,0.86857003f,0.432489008f,0.408856004f,0.407932013f,0.299814999f,0.256659001f,0.549571991f,0.406347007f,0.312330991f,0.62757802f,0.670167029f,0.524648011f,0.406926006f,0.637524009f,0.555836976f,1.0f,0.818979025f,0.705347002f,0.678140998f,0.427381992f,0.674404025f,0.636105001f,0.643634975f,0.699136019f,0.836201012f,0.613084972f,0.31901899f,0.725259006f,0.545518994f,0.479860991f,0.498360008f,0.488653988f,0.861671984f,0.314287007f,0.671051979f,0.531904995f,0.421781003f,0.81506598f,0.772032022f,0.488721997f,0.0896674022f,0.291285992f,0.65872997f,0.635631979f,0.679356992f,0.459497005f,0.360240012f,0.58228898f,0.650605023f,0.490949988f,0.381909996f,0.157260999f,0.479624003f,0.477490991f,0.174435005f,0.0130939996f,0.879113019f,0.608069003f,0.268877f,0.604479015f,0.245130002f,0.170506999f,0.292887986f,0.545849025f,0.476646006f,0.922316015f,0.669192016f,0.578094006f,0.578796983f,0.311396003f,0.601209998f,0.54995501f,1.0f,0.66573f,0.980114996f,0.537847996f,0.0740531012f,0.252472013f,0.25575f,0.223974004f,0.0865103006f,0.138209f,0.198623002f,0.0453034006f,0.432453007f,0.292407006f,0.394410014f,0.857658982f,0.271667987f,0.201545f,0.583993971f,0.0602377988f,0.190617993f,0.849505007f,0.975542009f,0.173140004f,0.206471995f,0.344792992f,0.761011004f,0.558125019f,0.117245004f,0.0338485017f,0.337597013f,0.336645991f,0.174253002f,0.230169997f,0.934872985f,0.593647003f,0.393225014f,0.683704019f,0.056609299f,0.0405011997f,0.148972005f,0.338721991f,0.283419013f,0.394006997f,0.237474993f,0.26996401f,0.428312987f,0.177498996f,0.462585002f,0.443962991f,0.981792986f,0.408239007f,0.676527023f,0.402864993f,0.0163302999f,0.0515113994f,0.341390014f,0.311134994f,0.613276005f,0.805884004f,0.953289986f,0.406091005f,0.578705013f,0.386785001f,0.434103012f,0.775259972f,1.0f,0.635909021f,0.78205198f,0.0137182996f,0.0387725011f,0.618964016f,0.857070982f,0.131522f,0.184988007f,0.299495012f,0.789211988f,0.603114009f,0.0704988986f,0.0129338996f,0.252481014f,0.254121006f,0.189206004f,0.357713014f,0.950308025f,0.552573025f,0.466453999f,0.777360022f,0.0307886004f,0.0251943003f,0.378886014f,0.740186989f,0.247637004f,0.235201001f,0.493045002f,0.517849982f,0.883953989f,0.429473013f,0.409433007f,0.415266007f,0.940198004f,0.282334f,0.437889993f,0.375384986f,0.0157366004f,0.0171763003f,0.485554993f,0.461014986f,0.858958006f,0.907990992f,0.935190976f,0.375510007f,1.0f,0.585493028f,0.269980997f,0.423052996f,0.666067004f,0.435090005f,0.79025197f,0.00889586005f,0.0208844002f,0.449734986f,0.790808022f,0.159856007f,0.0895989984f,0.161546007f,0.528168023f,0.380641997f,0.0206275992f,0.0072642602f,0.0315352008f,0.0315840989f,0.197649002f,0.475057006f,0.517232001f,0.360922009f,0.421204001f,0.631340027f,0.00952139031f,0.0161049999f,0.499615014f,0.922958016f,0.214983001f,0.0655141026f,0.503970027f,0.514847994f,1.0f,0.483619004f,0.254027009f,0.228371993f,0.436105013f,0.233125001f,0.152242005f,0.279513001f,0.00906739011f,0.0132331997f,0.45125699f,0.388565987f,0.737226009f,0.479378015f,0.233036995f,0.103767f,0.845609009f,0.644127011f,0.261359006f,0.371457011f,0.527229011f,0.381372988f,0.334491998f,0.00833749026f,0.00861981977f,0.255919009f,0.254197001f,0.0872332975f,0.0461511984f,0.113017999f,0.345986009f,0.236343995f,0.0107800001f,0.00816505961f,0.405180991f,0.387180001f,0.343681008f,0.816492975f,0.25908199f,0.211906001f,0.432455003f,0.696886003f,0.00576223992f,0.0131310001f,0.455969006f,0.81160903f,0.426544011f,0.128489003f,0.215937003f,0.233934f,0.723070025f,0.351622999f,0.394230992f,0.323765993f,0.168803006f,0.276932001f,0.264683992f,0.227703005f,0.0068093501f,0.0170703009f,0.603017986f,0.476460993f,0.585924983f,0.716960013f,1.0f,0.576526999f,0.475524008f,0.447322011f,0.356902003f,0.597572982f,0.697246015f,0.505333006f,0.285421014f,0.0147193f,0.0141618f,0.136188f,0.0336536989f,0.216436997f};
class mydspSIG2 {
	
  private:
	
	int fmydspSIG2Wave0_idx;
	
  public:
	
	int getNumInputsmydspSIG2() {
		return 0;
	}
	int getNumOutputsmydspSIG2() {
		return 1;
	}
	int getInputRatemydspSIG2(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatemydspSIG2(int channel) {
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
	
	void instanceInitmydspSIG2(int sample_rate) {
		fmydspSIG2Wave0_idx = 0;
	}
	
	void fillmydspSIG2(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			table[i] = fmydspSIG2Wave0[fmydspSIG2Wave0_idx];
			fmydspSIG2Wave0_idx = ((1 + fmydspSIG2Wave0_idx) % 350);
		}
	}

};

static mydspSIG2* newmydspSIG2() { return (mydspSIG2*)new mydspSIG2(); }
static void deletemydspSIG2(mydspSIG2* dsp) { delete dsp; }

static float mydsp_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower6_f(float value) {
	return (((((value * value) * value) * value) * value) * value);
}
static float ftbl1mydspSIG1[250];
static float ftbl2mydspSIG2[350];

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
	FAUSTFLOAT fHslider0;
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
	int iRec6[2];
	float fRec5[3];
	float fRec4[3];
	float fConst2;
	FAUSTFLOAT fHslider7;
	float fVec1[2];
	float fVec2[2];
	float fVec3[2];
	int iRec7[2];
	float fConst4;
	float fConst5;
	float fRec8[2];
	float fConst6;
	float fRec3[3];
	float fConst8;
	float fConst9;
	float fRec9[3];
	float fConst11;
	float fConst12;
	float fRec10[3];
	float fConst14;
	float fConst15;
	float fRec11[3];
	float fConst17;
	float fConst18;
	float fRec12[3];
	float fConst20;
	float fConst21;
	float fRec13[3];
	float fConst23;
	float fConst24;
	float fRec14[3];
	float fConst26;
	float fConst27;
	float fRec15[3];
	float fConst29;
	float fConst30;
	float fRec16[3];
	float fConst32;
	float fConst33;
	float fRec17[3];
	float fConst35;
	float fConst36;
	float fRec18[3];
	float fConst38;
	float fConst39;
	float fRec19[3];
	float fConst41;
	float fConst42;
	float fRec20[3];
	float fConst44;
	float fConst45;
	float fRec21[3];
	float fConst47;
	float fConst48;
	float fRec22[3];
	float fConst50;
	float fConst51;
	float fRec23[3];
	float fConst53;
	float fConst54;
	float fRec24[3];
	float fConst56;
	float fConst57;
	float fRec25[3];
	float fConst59;
	float fConst60;
	float fRec26[3];
	float fConst62;
	float fConst63;
	float fRec27[3];
	float fConst64;
	float fConst65;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst66;
	FAUSTFLOAT fHslider10;
	float fRec30[2];
	float fRec29[2];
	FAUSTFLOAT fHslider11;
	float fRec28[2];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec31[3];
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
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
	float fConst70;
	float fConst71;
	float fConst72;
	FAUSTFLOAT fHslider23;
	float fConst73;
	float fRec38[2];
	float fRec36[2];
	float fConst74;
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	FAUSTFLOAT fHslider26;
	float fConst75;
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec43[2];
	FAUSTFLOAT fHslider27;
	float fRec42[3];
	float fRec45[3];
	float fRec46[3];
	float fRec47[3];
	float fRec48[3];
	float fConst76;
	float fConst77;
	float fConst78;
	float fRec35[3];
	FAUSTFLOAT fHslider28;
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	float fRec51[2];
	float fRec50[2];
	FAUSTFLOAT fHslider31;
	float fRec49[2];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec52[3];
	FAUSTFLOAT fHslider34;
	FAUSTFLOAT fHslider35;
	float fRec34[3];
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fRec33[3];
	float fRec32[3];
	FAUSTFLOAT fHslider38;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider39;
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fHslider42;
	float fConst79;
	float fConst80;
	float fConst81;
	float fConst82;
	FAUSTFLOAT fHslider43;
	float fVec4[2];
	float fVec5[2];
	float fVec6[2];
	int iRec57[2];
	float fConst83;
	float fRec56[2];
	FAUSTFLOAT fHslider44;
	float fRec60[2];
	int iRec59[2];
	float fConst84;
	float fConst85;
	float fConst86;
	float fConst87;
	float fConst88;
	float fRec62[2];
	float fRec61[2];
	float fRec58[3];
	FAUSTFLOAT fHslider45;
	float fRec65[2];
	int iRec64[2];
	float fRec63[3];
	FAUSTFLOAT fHslider46;
	float fRec68[2];
	int iRec67[2];
	float fRec66[3];
	FAUSTFLOAT fHslider47;
	float fRec71[2];
	int iRec70[2];
	float fRec69[3];
	FAUSTFLOAT fHslider48;
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	float fRec74[2];
	float fRec73[2];
	FAUSTFLOAT fHslider51;
	float fRec72[2];
	FAUSTFLOAT fHslider52;
	FAUSTFLOAT fHslider53;
	float fRec75[3];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec55[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec54[3];
	float fRec53[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	int iRec79[2];
	float fRec83[2];
	int IOTA;
	float fVec7[2048];
	float fConst89;
	float fConst90;
	FAUSTFLOAT fHslider63;
	float fVec8[2];
	float fVec9[2];
	float fVec10[2];
	float fRec84[2];
	float fRec81[2];
	float fRec85[2];
	float fConst93;
	float fConst96;
	float fConst98;
	float fConst99;
	float fConst100;
	float fRec88[3];
	float fConst101;
	float fConst102;
	float fConst103;
	float fRec87[3];
	float fConst104;
	int iRec89[2];
	float fConst106;
	float fConst107;
	float fRec86[3];
	float fConst108;
	float fConst109;
	float fConst111;
	float fConst112;
	float fConst113;
	float fRec90[3];
	float fConst114;
	float fConst116;
	float fConst117;
	float fConst118;
	float fRec91[3];
	float fConst119;
	float fConst121;
	float fConst122;
	float fConst123;
	float fRec92[3];
	float fConst124;
	float fConst126;
	float fConst127;
	float fConst128;
	float fRec93[3];
	float fConst129;
	float fConst131;
	float fConst132;
	float fConst133;
	float fRec94[3];
	float fConst134;
	float fConst136;
	float fConst137;
	float fConst138;
	float fRec95[3];
	float fConst139;
	float fConst141;
	float fConst142;
	float fConst143;
	float fRec96[3];
	float fConst144;
	float fConst146;
	float fConst147;
	float fConst148;
	float fRec97[3];
	float fConst149;
	float fConst151;
	float fConst152;
	float fConst153;
	float fRec98[3];
	float fConst154;
	float fConst156;
	float fConst157;
	float fConst158;
	float fRec99[3];
	float fConst159;
	float fConst161;
	float fConst162;
	float fConst163;
	float fRec100[3];
	float fConst164;
	float fConst166;
	float fConst167;
	float fConst168;
	float fRec101[3];
	float fConst169;
	float fConst171;
	float fConst172;
	float fConst173;
	float fRec102[3];
	float fConst174;
	float fConst176;
	float fConst177;
	float fConst178;
	float fRec103[3];
	float fConst179;
	float fConst181;
	float fConst182;
	float fConst183;
	float fRec104[3];
	float fConst184;
	float fConst186;
	float fConst187;
	float fConst188;
	float fRec105[3];
	float fConst189;
	float fConst191;
	float fConst192;
	float fConst193;
	float fRec106[3];
	float fConst194;
	float fConst196;
	float fConst197;
	float fConst198;
	float fRec107[3];
	float fConst199;
	float fConst201;
	float fConst202;
	float fConst203;
	float fRec108[3];
	float fConst204;
	float fConst206;
	float fConst207;
	float fConst208;
	float fRec109[3];
	float fConst209;
	float fConst211;
	float fConst212;
	float fConst213;
	float fRec110[3];
	float fConst214;
	float fConst216;
	float fConst217;
	float fConst218;
	float fRec111[3];
	float fConst219;
	float fConst221;
	float fConst222;
	float fConst223;
	float fRec112[3];
	float fConst224;
	float fConst226;
	float fConst227;
	float fConst228;
	float fRec113[3];
	float fConst229;
	float fConst231;
	float fConst232;
	float fConst233;
	float fRec114[3];
	float fConst234;
	float fConst236;
	float fConst237;
	float fConst238;
	float fRec115[3];
	float fConst239;
	float fConst241;
	float fConst242;
	float fConst243;
	float fRec116[3];
	float fConst244;
	float fConst246;
	float fConst247;
	float fConst248;
	float fRec117[3];
	float fConst249;
	float fConst251;
	float fConst252;
	float fConst253;
	float fRec118[3];
	float fConst254;
	float fConst256;
	float fConst257;
	float fConst258;
	float fRec119[3];
	float fConst259;
	float fConst261;
	float fConst262;
	float fConst263;
	float fRec120[3];
	float fConst264;
	float fConst266;
	float fConst267;
	float fConst268;
	float fRec121[3];
	float fConst269;
	float fConst271;
	float fConst272;
	float fConst273;
	float fRec122[3];
	float fConst274;
	float fConst276;
	float fConst277;
	float fConst278;
	float fRec123[3];
	float fConst279;
	float fConst281;
	float fConst282;
	float fConst283;
	float fRec124[3];
	float fConst284;
	float fConst286;
	float fConst287;
	float fConst288;
	float fRec125[3];
	float fConst289;
	float fConst291;
	float fConst292;
	float fConst293;
	float fRec126[3];
	float fConst294;
	float fConst296;
	float fConst297;
	float fConst298;
	float fRec127[3];
	float fConst299;
	float fConst301;
	float fConst302;
	float fConst303;
	float fRec128[3];
	float fConst304;
	float fConst306;
	float fConst307;
	float fConst308;
	float fRec129[3];
	float fConst309;
	float fConst311;
	float fConst312;
	float fConst313;
	float fRec130[3];
	float fConst314;
	float fConst316;
	float fConst317;
	float fConst318;
	float fRec131[3];
	float fConst319;
	float fConst321;
	float fConst322;
	float fConst323;
	float fRec132[3];
	float fConst324;
	float fConst326;
	float fConst327;
	float fConst328;
	float fRec133[3];
	float fConst329;
	float fConst331;
	float fConst332;
	float fConst333;
	float fRec134[3];
	float fConst334;
	float fConst336;
	float fConst337;
	float fConst338;
	float fRec135[3];
	float fConst339;
	float fConst341;
	float fConst342;
	float fConst343;
	float fRec136[3];
	float fConst344;
	float fConst346;
	float fConst347;
	float fConst348;
	float fRec137[3];
	float fConst349;
	float fConst351;
	float fConst352;
	float fConst353;
	float fRec138[3];
	float fConst354;
	float fVec11[2];
	float fVec12[2048];
	float fRec82[2];
	FAUSTFLOAT fHslider64;
	FAUSTFLOAT fHslider65;
	FAUSTFLOAT fHslider66;
	float fRec141[2];
	float fRec140[2];
	FAUSTFLOAT fHslider67;
	float fRec139[2];
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	float fRec142[3];
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec78[3];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec77[3];
	float fRec76[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	float fConst355;
	float fRec146[2];
	float fRec148[2];
	float fConst356;
	float fRec152[2];
	float fRec151[2];
	float fRec150[2];
	float fRec149[2];
	float fRec147[2];
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	float fRec155[2];
	float fRec154[2];
	FAUSTFLOAT fHslider82;
	float fRec153[2];
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec156[3];
	FAUSTFLOAT fHslider85;
	FAUSTFLOAT fHslider86;
	float fRec145[3];
	FAUSTFLOAT fHslider87;
	FAUSTFLOAT fHslider88;
	float fRec144[3];
	float fRec143[3];
	FAUSTFLOAT fHslider89;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	FAUSTFLOAT fHslider92;
	FAUSTFLOAT fHslider93;
	float fConst357;
	FAUSTFLOAT fHslider94;
	float fVec13[2];
	float fVec14[2];
	float fVec15[2];
	FAUSTFLOAT fHslider95;
	float fRec186[2];
	float fRec182[2];
	FAUSTFLOAT fHslider96;
	float fRec187[2];
	float fRec189[4];
	float fRec190[1024];
	float fConst358;
	float fVec16[2];
	float fVec17[3];
	float fRec188[2048];
	float fRec178[2];
	float fRec174[2];
	float fRec170[1024];
	float fRec168[4];
	float fRec163[2048];
	float fRec161[2];
	FAUSTFLOAT fHslider97;
	float fRec191[2];
	float fRec160[3];
	int iRec192[2];
	int iRec193[2];
	FAUSTFLOAT fHslider98;
	FAUSTFLOAT fHslider99;
	FAUSTFLOAT fHslider100;
	float fRec196[2];
	float fRec195[2];
	FAUSTFLOAT fHslider101;
	float fRec194[2];
	FAUSTFLOAT fHslider102;
	FAUSTFLOAT fHslider103;
	float fRec197[3];
	FAUSTFLOAT fHslider104;
	FAUSTFLOAT fHslider105;
	float fRec159[3];
	FAUSTFLOAT fHslider106;
	FAUSTFLOAT fHslider107;
	float fRec158[3];
	float fRec157[3];
	FAUSTFLOAT fHslider108;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider109;
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fHslider111;
	FAUSTFLOAT fHslider112;
	float fConst359;
	float fConst362;
	float fConst363;
	float fConst364;
	float fRec202[3];
	float fConst365;
	FAUSTFLOAT fHslider113;
	float fVec18[2];
	float fVec19[2];
	float fVec20[2];
	int iRec203[2];
	float fConst367;
	float fConst368;
	float fRec201[3];
	float fConst369;
	float fConst371;
	float fConst372;
	float fRec204[3];
	float fConst373;
	float fConst375;
	float fConst376;
	float fRec205[3];
	float fConst377;
	float fConst379;
	float fConst380;
	float fRec206[3];
	float fConst381;
	float fConst383;
	float fConst384;
	float fRec207[3];
	float fConst385;
	float fConst387;
	float fConst388;
	float fRec208[3];
	float fConst389;
	float fConst391;
	float fConst392;
	float fRec209[3];
	float fConst393;
	float fConst395;
	float fConst396;
	float fRec210[3];
	float fConst397;
	float fConst399;
	float fConst400;
	float fRec211[3];
	float fConst401;
	float fConst403;
	float fConst404;
	float fRec212[3];
	float fConst405;
	float fConst407;
	float fConst408;
	float fRec213[3];
	float fConst409;
	float fConst411;
	float fConst412;
	float fRec214[3];
	float fConst413;
	float fConst415;
	float fConst416;
	float fRec215[3];
	float fConst417;
	float fConst419;
	float fConst420;
	float fRec216[3];
	float fConst421;
	float fConst423;
	float fConst424;
	float fRec217[3];
	float fConst425;
	float fConst427;
	float fConst428;
	float fRec218[3];
	float fConst429;
	float fConst431;
	float fConst432;
	float fRec219[3];
	float fConst433;
	float fConst435;
	float fConst436;
	float fRec220[3];
	float fConst437;
	float fConst439;
	float fConst440;
	float fRec221[3];
	float fConst441;
	float fConst443;
	float fConst444;
	float fRec222[3];
	float fConst445;
	float fConst447;
	float fConst448;
	float fRec223[3];
	float fConst449;
	float fConst451;
	float fConst452;
	float fRec224[3];
	float fConst453;
	float fConst455;
	float fConst456;
	float fRec225[3];
	float fConst457;
	float fConst459;
	float fConst460;
	float fRec226[3];
	float fConst461;
	float fConst463;
	float fConst464;
	float fRec227[3];
	float fConst465;
	float fConst467;
	float fConst468;
	float fRec228[3];
	float fConst469;
	float fConst471;
	float fConst472;
	float fRec229[3];
	float fConst473;
	float fConst475;
	float fConst476;
	float fRec230[3];
	float fConst477;
	float fConst479;
	float fConst480;
	float fRec231[3];
	float fConst481;
	float fConst483;
	float fConst484;
	float fRec232[3];
	float fConst485;
	float fConst487;
	float fConst488;
	float fRec233[3];
	float fConst489;
	float fConst491;
	float fConst492;
	float fRec234[3];
	float fConst493;
	float fConst495;
	float fConst496;
	float fRec235[3];
	float fConst497;
	float fConst499;
	float fConst500;
	float fRec236[3];
	float fConst501;
	float fConst503;
	float fConst504;
	float fRec237[3];
	float fConst505;
	float fConst507;
	float fConst508;
	float fRec238[3];
	float fConst509;
	float fConst511;
	float fConst512;
	float fRec239[3];
	float fConst513;
	float fConst515;
	float fConst516;
	float fRec240[3];
	float fConst517;
	float fConst519;
	float fConst520;
	float fRec241[3];
	float fConst521;
	float fConst523;
	float fConst524;
	float fRec242[3];
	float fConst525;
	float fConst527;
	float fConst528;
	float fRec243[3];
	float fConst529;
	float fConst531;
	float fConst532;
	float fRec244[3];
	float fConst533;
	float fConst535;
	float fConst536;
	float fRec245[3];
	float fConst537;
	float fConst539;
	float fConst540;
	float fRec246[3];
	float fConst541;
	float fConst543;
	float fConst544;
	float fRec247[3];
	float fConst545;
	float fConst547;
	float fConst548;
	float fRec248[3];
	float fConst549;
	float fConst551;
	float fConst552;
	float fRec249[3];
	float fConst553;
	float fConst555;
	float fConst556;
	float fRec250[3];
	float fConst557;
	float fConst559;
	float fConst560;
	float fRec251[3];
	float fConst561;
	float fConst563;
	float fConst564;
	float fRec252[3];
	float fConst565;
	float fConst566;
	FAUSTFLOAT fHslider114;
	float fRec253[2];
	float fRec254[2];
	FAUSTFLOAT fHslider115;
	float fRec255[2];
	float fRec256[2];
	FAUSTFLOAT fHslider116;
	float fRec257[2];
	float fRec258[2];
	FAUSTFLOAT fHslider117;
	FAUSTFLOAT fHslider118;
	FAUSTFLOAT fHslider119;
	float fRec261[2];
	float fRec260[2];
	FAUSTFLOAT fHslider120;
	float fRec259[2];
	FAUSTFLOAT fHslider121;
	FAUSTFLOAT fHslider122;
	float fRec262[3];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	float fRec200[3];
	FAUSTFLOAT fHslider125;
	FAUSTFLOAT fHslider126;
	float fRec199[3];
	float fRec198[3];
	float fConst567;
	float fConst577;
	float fConst578;
	float fConst579;
	float fConst581;
	float fConst582;
	float fRec275[2];
	float fRec274[2];
	float fVec21[32768];
	int iConst584;
	int iRec280[2];
	float fRec279[3];
	int iRec282[2];
	float fRec281[3];
	int iRec284[2];
	float fRec283[3];
	int iRec286[2];
	float fRec285[3];
	float fRec289[2];
	float fRec288[2];
	float fRec287[2];
	float fRec290[3];
	float fRec278[3];
	float fRec277[3];
	float fRec276[3];
	float fVec22[2048];
	int iConst585;
	float fVec23[2048];
	int iConst586;
	float fRec272[2];
	float fConst595;
	float fConst596;
	float fConst597;
	float fRec294[2];
	float fRec293[2];
	float fVec24[32768];
	int iConst599;
	float fVec25[4096];
	int iConst600;
	float fRec291[2];
	float fConst609;
	float fConst610;
	float fConst611;
	float fRec298[2];
	float fRec297[2];
	float fVec26[16384];
	int iConst613;
	float fVec27[4096];
	int iConst614;
	float fRec295[2];
	float fConst623;
	float fConst624;
	float fConst625;
	float fRec302[2];
	float fRec301[2];
	float fVec28[32768];
	int iConst627;
	float fVec29[4096];
	int iConst628;
	float fRec299[2];
	float fConst637;
	float fConst638;
	float fConst639;
	float fRec306[2];
	float fRec305[2];
	float fVec30[16384];
	int iConst641;
	float fVec31[2048];
	float fVec32[2048];
	int iConst642;
	float fRec303[2];
	float fConst651;
	float fConst652;
	float fConst653;
	float fRec310[2];
	float fRec309[2];
	float fVec33[16384];
	int iConst655;
	float fVec34[4096];
	int iConst656;
	float fRec307[2];
	float fConst665;
	float fConst666;
	float fConst667;
	float fRec314[2];
	float fRec313[2];
	float fVec35[16384];
	int iConst669;
	float fVec36[4096];
	int iConst670;
	float fRec311[2];
	float fConst679;
	float fConst680;
	float fConst681;
	float fRec318[2];
	float fRec317[2];
	float fVec37[16384];
	int iConst683;
	float fVec38[2048];
	int iConst684;
	float fRec315[2];
	float fRec264[3];
	float fRec265[3];
	float fRec266[3];
	float fRec267[3];
	float fRec268[3];
	float fRec269[3];
	float fRec270[3];
	float fRec271[3];
	float fConst685;
	float fConst686;
	float fConst687;
	float fConst688;
	float fConst689;
	float fConst690;
	float fRec319[2];
	float fConst691;
	float fRec263[3];
	float fConst692;
	float fConst693;
	float fConst694;
	float fConst695;
	float fRec322[2];
	float fRec321[2];
	float fRec320[2];
	float fConst696;
	float fConst697;
	float fConst698;
	float fConst699;
	float fRec325[2];
	float fRec324[2];
	float fRec323[2];
	float fRec328[2];
	float fRec327[2];
	float fRec326[2];
	float fRec329[3];
	float fRec332[2];
	float fRec331[2];
	float fRec330[2];
	float fRec335[2];
	float fRec334[2];
	float fRec333[2];
	float fRec338[2];
	float fRec337[2];
	float fRec336[2];
	
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
		sig1->fillmydspSIG1(250, ftbl1mydspSIG1);
		mydspSIG2* sig2 = newmydspSIG2();
		sig2->instanceInitmydspSIG2(sample_rate);
		sig2->fillmydspSIG2(350, ftbl2mydspSIG2);
		deletemydspSIG0(sig0);
		deletemydspSIG1(sig1);
		deletemydspSIG2(sig2);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (3.14159274f / fConst0);
		fConst2 = (0.00200000009f * fConst0);
		float fConst3 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst4 = (0.0f - (2.0f * fConst3));
		fConst5 = (6.28318548f / fConst0);
		fConst6 = mydsp_faustpower2_f(fConst3);
		float fConst7 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst8 = (0.0f - (2.0f * fConst7));
		fConst9 = mydsp_faustpower2_f(fConst7);
		float fConst10 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst11 = (0.0f - (2.0f * fConst10));
		fConst12 = mydsp_faustpower2_f(fConst10);
		float fConst13 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst14 = (0.0f - (2.0f * fConst13));
		fConst15 = mydsp_faustpower2_f(fConst13);
		float fConst16 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst17 = (0.0f - (2.0f * fConst16));
		fConst18 = mydsp_faustpower2_f(fConst16);
		float fConst19 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst20 = (0.0f - (2.0f * fConst19));
		fConst21 = mydsp_faustpower2_f(fConst19);
		float fConst22 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst23 = (0.0f - (2.0f * fConst22));
		fConst24 = mydsp_faustpower2_f(fConst22);
		float fConst25 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst26 = (0.0f - (2.0f * fConst25));
		fConst27 = mydsp_faustpower2_f(fConst25);
		float fConst28 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst29 = (0.0f - (2.0f * fConst28));
		fConst30 = mydsp_faustpower2_f(fConst28);
		float fConst31 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst32 = (0.0f - (2.0f * fConst31));
		fConst33 = mydsp_faustpower2_f(fConst31);
		float fConst34 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst35 = (0.0f - (2.0f * fConst34));
		fConst36 = mydsp_faustpower2_f(fConst34);
		float fConst37 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst38 = (0.0f - (2.0f * fConst37));
		fConst39 = mydsp_faustpower2_f(fConst37);
		float fConst40 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst41 = (0.0f - (2.0f * fConst40));
		fConst42 = mydsp_faustpower2_f(fConst40);
		float fConst43 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst44 = (0.0f - (2.0f * fConst43));
		fConst45 = mydsp_faustpower2_f(fConst43);
		float fConst46 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst47 = (0.0f - (2.0f * fConst46));
		fConst48 = mydsp_faustpower2_f(fConst46);
		float fConst49 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst50 = (0.0f - (2.0f * fConst49));
		fConst51 = mydsp_faustpower2_f(fConst49);
		float fConst52 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst53 = (0.0f - (2.0f * fConst52));
		fConst54 = mydsp_faustpower2_f(fConst52);
		float fConst55 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst56 = (0.0f - (2.0f * fConst55));
		fConst57 = mydsp_faustpower2_f(fConst55);
		float fConst58 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst59 = (0.0f - (2.0f * fConst58));
		fConst60 = mydsp_faustpower2_f(fConst58);
		float fConst61 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst62 = (0.0f - (2.0f * fConst61));
		fConst63 = mydsp_faustpower2_f(fConst61);
		fConst64 = std::exp((0.0f - (20.0f / fConst0)));
		fConst65 = (1.0f - fConst64);
		fConst66 = (1.0f / fConst0);
		float fConst67 = std::tan((25132.7422f / fConst0));
		float fConst68 = (1.0f / fConst67);
		float fConst69 = (((fConst68 + 0.333333343f) / fConst67) + 1.0f);
		fConst70 = (0.25f / fConst69);
		fConst71 = std::exp((0.0f - (50.0f / fConst0)));
		fConst72 = (1.0f - fConst71);
		fConst73 = (6.0f / fConst0);
		fConst74 = (2.0f * fConst72);
		fConst75 = (8.0f / fConst0);
		fConst76 = (1.0f / fConst69);
		fConst77 = (((fConst68 + -0.333333343f) / fConst67) + 1.0f);
		fConst78 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst67))));
		fConst79 = std::exp((0.0f - (500.0f / fConst0)));
		fConst80 = (1.0f - fConst79);
		fConst81 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst82 = (1.0f / fConst81);
		fConst83 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst84 = (0.330000013f * fConst0);
		fConst85 = (0.100000001f * fConst0);
		fConst86 = (0.660000026f * fConst0);
		fConst87 = std::exp((0.0f - (100.0f / fConst0)));
		fConst88 = (0.699999988f * (1.0f - fConst87));
		fConst89 = (0.00882352982f * fConst0);
		fConst90 = (0.00147058826f * fConst0);
		float fConst91 = std::tan((37699.1133f / fConst0));
		float fConst92 = (1.0f / fConst91);
		fConst93 = (1.0f / (((fConst92 + 1.41421354f) / fConst91) + 1.0f));
		float fConst94 = std::tan((31.415926f / fConst0));
		float fConst95 = (1.0f / fConst94);
		fConst96 = (1.0f / (((fConst95 + 1.41421354f) / fConst94) + 1.0f));
		float fConst97 = mydsp_faustpower2_f(fConst94);
		fConst98 = (1.0f / fConst97);
		fConst99 = (((fConst95 + -1.41421354f) / fConst94) + 1.0f);
		fConst100 = (2.0f * (1.0f - fConst98));
		fConst101 = (0.0f - (2.0f / fConst97));
		fConst102 = (((fConst92 + -1.41421354f) / fConst91) + 1.0f);
		fConst103 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst91))));
		fConst104 = (1.0f / std::max<float>(1.0f, (0.00400000019f * fConst0)));
		float fConst105 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst106 = (0.0f - (2.0f * fConst105));
		fConst107 = mydsp_faustpower2_f(fConst105);
		fConst108 = ((0.5f * fConst0) + -1.0f);
		fConst109 = ftbl1mydspSIG1[0];
		float fConst110 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst111 = (0.0f - (2.0f * fConst110));
		fConst112 = (20.8197117f / fConst0);
		fConst113 = mydsp_faustpower2_f(fConst110);
		fConst114 = ftbl1mydspSIG1[1];
		float fConst115 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst116 = (0.0f - (2.0f * fConst115));
		fConst117 = (24.0940685f / fConst0);
		fConst118 = mydsp_faustpower2_f(fConst115);
		fConst119 = ftbl1mydspSIG1[2];
		float fConst120 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst121 = (0.0f - (2.0f * fConst120));
		fConst122 = (50.6621399f / fConst0);
		fConst123 = mydsp_faustpower2_f(fConst120);
		fConst124 = ftbl1mydspSIG1[3];
		float fConst125 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst126 = (0.0f - (2.0f * fConst125));
		fConst127 = (59.3621521f / fConst0);
		fConst128 = mydsp_faustpower2_f(fConst125);
		fConst129 = ftbl1mydspSIG1[4];
		float fConst130 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst131 = (0.0f - (2.0f * fConst130));
		fConst132 = (88.6990967f / fConst0);
		fConst133 = mydsp_faustpower2_f(fConst130);
		fConst134 = ftbl1mydspSIG1[5];
		float fConst135 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst136 = (0.0f - (2.0f * fConst135));
		fConst137 = (115.510078f / fConst0);
		fConst138 = mydsp_faustpower2_f(fConst135);
		fConst139 = ftbl1mydspSIG1[6];
		float fConst140 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst141 = (0.0f - (2.0f * fConst140));
		fConst142 = (132.010986f / fConst0);
		fConst143 = mydsp_faustpower2_f(fConst140);
		fConst144 = ftbl1mydspSIG1[7];
		float fConst145 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst146 = (0.0f - (2.0f * fConst145));
		fConst147 = (164.478088f / fConst0);
		fConst148 = mydsp_faustpower2_f(fConst145);
		fConst149 = ftbl1mydspSIG1[8];
		float fConst150 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst151 = (0.0f - (2.0f * fConst150));
		fConst152 = (182.177185f / fConst0);
		fConst153 = mydsp_faustpower2_f(fConst150);
		fConst154 = ftbl1mydspSIG1[9];
		float fConst155 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst156 = (0.0f - (2.0f * fConst155));
		fConst157 = (232.935272f / fConst0);
		fConst158 = mydsp_faustpower2_f(fConst155);
		fConst159 = ftbl1mydspSIG1[10];
		float fConst160 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst161 = (0.0f - (2.0f * fConst160));
		fConst162 = (237.946106f / fConst0);
		fConst163 = mydsp_faustpower2_f(fConst160);
		fConst164 = ftbl1mydspSIG1[11];
		float fConst165 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst166 = (0.0f - (2.0f * fConst165));
		fConst167 = (251.725769f / fConst0);
		fConst168 = mydsp_faustpower2_f(fConst165);
		fConst169 = ftbl1mydspSIG1[12];
		float fConst170 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst171 = (0.0f - (2.0f * fConst170));
		fConst172 = (299.355438f / fConst0);
		fConst173 = mydsp_faustpower2_f(fConst170);
		fConst174 = ftbl1mydspSIG1[13];
		float fConst175 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst176 = (0.0f - (2.0f * fConst175));
		fConst177 = (320.561829f / fConst0);
		fConst178 = mydsp_faustpower2_f(fConst175);
		fConst179 = ftbl1mydspSIG1[14];
		float fConst180 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst181 = (0.0f - (2.0f * fConst180));
		fConst182 = (329.427399f / fConst0);
		fConst183 = mydsp_faustpower2_f(fConst180);
		fConst184 = ftbl1mydspSIG1[15];
		float fConst185 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst186 = (0.0f - (2.0f * fConst185));
		fConst187 = (366.221741f / fConst0);
		fConst188 = mydsp_faustpower2_f(fConst185);
		fConst189 = ftbl1mydspSIG1[16];
		float fConst190 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst191 = (0.0f - (2.0f * fConst190));
		fConst192 = (399.287628f / fConst0);
		fConst193 = mydsp_faustpower2_f(fConst190);
		fConst194 = ftbl1mydspSIG1[17];
		float fConst195 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst196 = (0.0f - (2.0f * fConst195));
		fConst197 = (410.686584f / fConst0);
		fConst198 = mydsp_faustpower2_f(fConst195);
		fConst199 = ftbl1mydspSIG1[18];
		float fConst200 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst201 = (0.0f - (2.0f * fConst200));
		fConst202 = (420.713928f / fConst0);
		fConst203 = mydsp_faustpower2_f(fConst200);
		fConst204 = ftbl1mydspSIG1[19];
		float fConst205 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst206 = (0.0f - (2.0f * fConst205));
		fConst207 = (468.286438f / fConst0);
		fConst208 = mydsp_faustpower2_f(fConst205);
		fConst209 = ftbl1mydspSIG1[20];
		float fConst210 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst211 = (0.0f - (2.0f * fConst210));
		fConst212 = (494.436432f / fConst0);
		fConst213 = mydsp_faustpower2_f(fConst210);
		fConst214 = ftbl1mydspSIG1[21];
		float fConst215 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst216 = (0.0f - (2.0f * fConst215));
		fConst217 = (507.916992f / fConst0);
		fConst218 = mydsp_faustpower2_f(fConst215);
		fConst219 = ftbl1mydspSIG1[22];
		float fConst220 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst221 = (0.0f - (2.0f * fConst220));
		fConst222 = (565.34845f / fConst0);
		fConst223 = mydsp_faustpower2_f(fConst220);
		fConst224 = ftbl1mydspSIG1[23];
		float fConst225 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst226 = (0.0f - (2.0f * fConst225));
		fConst227 = (584.12323f / fConst0);
		fConst228 = mydsp_faustpower2_f(fConst225);
		fConst229 = ftbl1mydspSIG1[24];
		float fConst230 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst231 = (0.0f - (2.0f * fConst230));
		fConst232 = (598.105225f / fConst0);
		fConst233 = mydsp_faustpower2_f(fConst230);
		fConst234 = ftbl1mydspSIG1[25];
		float fConst235 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst236 = (0.0f - (2.0f * fConst235));
		fConst237 = (612.489319f / fConst0);
		fConst238 = mydsp_faustpower2_f(fConst235);
		fConst239 = ftbl1mydspSIG1[26];
		float fConst240 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst241 = (0.0f - (2.0f * fConst240));
		fConst242 = (695.045959f / fConst0);
		fConst243 = mydsp_faustpower2_f(fConst240);
		fConst244 = ftbl1mydspSIG1[27];
		float fConst245 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst246 = (0.0f - (2.0f * fConst245));
		fConst247 = (704.150269f / fConst0);
		fConst248 = mydsp_faustpower2_f(fConst245);
		fConst249 = ftbl1mydspSIG1[28];
		float fConst250 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst251 = (0.0f - (2.0f * fConst250));
		fConst252 = (715.18988f / fConst0);
		fConst253 = mydsp_faustpower2_f(fConst250);
		fConst254 = ftbl1mydspSIG1[29];
		float fConst255 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst256 = (0.0f - (2.0f * fConst255));
		fConst257 = (749.935852f / fConst0);
		fConst258 = mydsp_faustpower2_f(fConst255);
		fConst259 = ftbl1mydspSIG1[30];
		float fConst260 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst261 = (0.0f - (2.0f * fConst260));
		fConst262 = (798.247253f / fConst0);
		fConst263 = mydsp_faustpower2_f(fConst260);
		fConst264 = ftbl1mydspSIG1[31];
		float fConst265 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst266 = (0.0f - (2.0f * fConst265));
		fConst267 = (816.700989f / fConst0);
		fConst268 = mydsp_faustpower2_f(fConst265);
		fConst269 = ftbl1mydspSIG1[32];
		float fConst270 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst271 = (0.0f - (2.0f * fConst270));
		fConst272 = (831.007812f / fConst0);
		fConst273 = mydsp_faustpower2_f(fConst270);
		fConst274 = ftbl1mydspSIG1[33];
		float fConst275 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst276 = (0.0f - (2.0f * fConst275));
		fConst277 = (838.660706f / fConst0);
		fConst278 = mydsp_faustpower2_f(fConst275);
		fConst279 = ftbl1mydspSIG1[34];
		float fConst280 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst281 = (0.0f - (2.0f * fConst280));
		fConst282 = (908.228149f / fConst0);
		fConst283 = mydsp_faustpower2_f(fConst280);
		fConst284 = ftbl1mydspSIG1[35];
		float fConst285 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst286 = (0.0f - (2.0f * fConst285));
		fConst287 = (938.946655f / fConst0);
		fConst288 = mydsp_faustpower2_f(fConst285);
		fConst289 = ftbl1mydspSIG1[36];
		float fConst290 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst291 = (0.0f - (2.0f * fConst290));
		fConst292 = (955.251526f / fConst0);
		fConst293 = mydsp_faustpower2_f(fConst290);
		fConst294 = ftbl1mydspSIG1[37];
		float fConst295 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst296 = (0.0f - (2.0f * fConst295));
		fConst297 = (962.370361f / fConst0);
		fConst298 = mydsp_faustpower2_f(fConst295);
		fConst299 = ftbl1mydspSIG1[38];
		float fConst300 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst301 = (0.0f - (2.0f * fConst300));
		fConst302 = (977.644775f / fConst0);
		fConst303 = mydsp_faustpower2_f(fConst300);
		fConst304 = ftbl1mydspSIG1[39];
		float fConst305 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst306 = (0.0f - (2.0f * fConst305));
		fConst307 = (993.893127f / fConst0);
		fConst308 = mydsp_faustpower2_f(fConst305);
		fConst309 = ftbl1mydspSIG1[40];
		float fConst310 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst311 = (0.0f - (2.0f * fConst310));
		fConst312 = (1056.23486f / fConst0);
		fConst313 = mydsp_faustpower2_f(fConst310);
		fConst314 = ftbl1mydspSIG1[41];
		float fConst315 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst316 = (0.0f - (2.0f * fConst315));
		fConst317 = (1079.84705f / fConst0);
		fConst318 = mydsp_faustpower2_f(fConst315);
		fConst319 = ftbl1mydspSIG1[42];
		float fConst320 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst321 = (0.0f - (2.0f * fConst320));
		fConst322 = (1096.1897f / fConst0);
		fConst323 = mydsp_faustpower2_f(fConst320);
		fConst324 = ftbl1mydspSIG1[43];
		float fConst325 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst326 = (0.0f - (2.0f * fConst325));
		fConst327 = (1124.29431f / fConst0);
		fConst328 = mydsp_faustpower2_f(fConst325);
		fConst329 = ftbl1mydspSIG1[44];
		float fConst330 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst331 = (0.0f - (2.0f * fConst330));
		fConst332 = (1140.28503f / fConst0);
		fConst333 = mydsp_faustpower2_f(fConst330);
		fConst334 = ftbl1mydspSIG1[45];
		float fConst335 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst336 = (0.0f - (2.0f * fConst335));
		fConst337 = (1164.89001f / fConst0);
		fConst338 = mydsp_faustpower2_f(fConst335);
		fConst339 = ftbl1mydspSIG1[46];
		float fConst340 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst341 = (0.0f - (2.0f * fConst340));
		fConst342 = (1196.12366f / fConst0);
		fConst343 = mydsp_faustpower2_f(fConst340);
		fConst344 = ftbl1mydspSIG1[47];
		float fConst345 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst346 = (0.0f - (2.0f * fConst345));
		fConst347 = (1207.56543f / fConst0);
		fConst348 = mydsp_faustpower2_f(fConst345);
		fConst349 = ftbl1mydspSIG1[48];
		float fConst350 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst351 = (0.0f - (2.0f * fConst350));
		fConst352 = (1228.39417f / fConst0);
		fConst353 = mydsp_faustpower2_f(fConst350);
		fConst354 = ftbl1mydspSIG1[49];
		fConst355 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst356 = (81.0f / fConst0);
		fConst357 = (0.00117647054f * fConst0);
		fConst358 = (0.000294117635f * fConst0);
		fConst359 = ftbl2mydspSIG2[50];
		float fConst360 = std::tan((31415.9258f / fConst0));
		float fConst361 = (1.0f / fConst360);
		fConst362 = (1.0f / (((fConst361 + 1.41421354f) / fConst360) + 1.0f));
		fConst363 = (((fConst361 + -1.41421354f) / fConst360) + 1.0f);
		fConst364 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst360))));
		fConst365 = (1.0f / std::max<float>(1.0f, (0.00159999996f * fConst0)));
		float fConst366 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst367 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst366)));
		fConst368 = mydsp_faustpower2_f(fConst366);
		fConst369 = ftbl2mydspSIG2[51];
		float fConst370 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst371 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst370)));
		fConst372 = mydsp_faustpower2_f(fConst370);
		fConst373 = ftbl2mydspSIG2[52];
		float fConst374 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst375 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst374)));
		fConst376 = mydsp_faustpower2_f(fConst374);
		fConst377 = ftbl2mydspSIG2[53];
		float fConst378 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst379 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst378)));
		fConst380 = mydsp_faustpower2_f(fConst378);
		fConst381 = ftbl2mydspSIG2[54];
		float fConst382 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst383 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst382)));
		fConst384 = mydsp_faustpower2_f(fConst382);
		fConst385 = ftbl2mydspSIG2[55];
		float fConst386 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst387 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst386)));
		fConst388 = mydsp_faustpower2_f(fConst386);
		fConst389 = ftbl2mydspSIG2[56];
		float fConst390 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst391 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst390)));
		fConst392 = mydsp_faustpower2_f(fConst390);
		fConst393 = ftbl2mydspSIG2[57];
		float fConst394 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst395 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst394)));
		fConst396 = mydsp_faustpower2_f(fConst394);
		fConst397 = ftbl2mydspSIG2[58];
		float fConst398 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst399 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst398)));
		fConst400 = mydsp_faustpower2_f(fConst398);
		fConst401 = ftbl2mydspSIG2[59];
		float fConst402 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst403 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst402)));
		fConst404 = mydsp_faustpower2_f(fConst402);
		fConst405 = ftbl2mydspSIG2[60];
		float fConst406 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst407 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst406)));
		fConst408 = mydsp_faustpower2_f(fConst406);
		fConst409 = ftbl2mydspSIG2[61];
		float fConst410 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst411 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst410)));
		fConst412 = mydsp_faustpower2_f(fConst410);
		fConst413 = ftbl2mydspSIG2[62];
		float fConst414 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst415 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst414)));
		fConst416 = mydsp_faustpower2_f(fConst414);
		fConst417 = ftbl2mydspSIG2[63];
		float fConst418 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst419 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst418)));
		fConst420 = mydsp_faustpower2_f(fConst418);
		fConst421 = ftbl2mydspSIG2[64];
		float fConst422 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst423 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst422)));
		fConst424 = mydsp_faustpower2_f(fConst422);
		fConst425 = ftbl2mydspSIG2[65];
		float fConst426 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst427 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst426)));
		fConst428 = mydsp_faustpower2_f(fConst426);
		fConst429 = ftbl2mydspSIG2[66];
		float fConst430 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst431 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst430)));
		fConst432 = mydsp_faustpower2_f(fConst430);
		fConst433 = ftbl2mydspSIG2[67];
		float fConst434 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst435 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst434)));
		fConst436 = mydsp_faustpower2_f(fConst434);
		fConst437 = ftbl2mydspSIG2[68];
		float fConst438 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst439 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst438)));
		fConst440 = mydsp_faustpower2_f(fConst438);
		fConst441 = ftbl2mydspSIG2[69];
		float fConst442 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst443 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst442)));
		fConst444 = mydsp_faustpower2_f(fConst442);
		fConst445 = ftbl2mydspSIG2[70];
		float fConst446 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst447 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst446)));
		fConst448 = mydsp_faustpower2_f(fConst446);
		fConst449 = ftbl2mydspSIG2[71];
		float fConst450 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst451 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst450)));
		fConst452 = mydsp_faustpower2_f(fConst450);
		fConst453 = ftbl2mydspSIG2[72];
		float fConst454 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst455 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst454)));
		fConst456 = mydsp_faustpower2_f(fConst454);
		fConst457 = ftbl2mydspSIG2[73];
		float fConst458 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst459 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst458)));
		fConst460 = mydsp_faustpower2_f(fConst458);
		fConst461 = ftbl2mydspSIG2[74];
		float fConst462 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst463 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst462)));
		fConst464 = mydsp_faustpower2_f(fConst462);
		fConst465 = ftbl2mydspSIG2[75];
		float fConst466 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst467 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst466)));
		fConst468 = mydsp_faustpower2_f(fConst466);
		fConst469 = ftbl2mydspSIG2[76];
		float fConst470 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst471 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst470)));
		fConst472 = mydsp_faustpower2_f(fConst470);
		fConst473 = ftbl2mydspSIG2[77];
		float fConst474 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst475 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst474)));
		fConst476 = mydsp_faustpower2_f(fConst474);
		fConst477 = ftbl2mydspSIG2[78];
		float fConst478 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst479 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst478)));
		fConst480 = mydsp_faustpower2_f(fConst478);
		fConst481 = ftbl2mydspSIG2[79];
		float fConst482 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst483 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst482)));
		fConst484 = mydsp_faustpower2_f(fConst482);
		fConst485 = ftbl2mydspSIG2[80];
		float fConst486 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst487 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst486)));
		fConst488 = mydsp_faustpower2_f(fConst486);
		fConst489 = ftbl2mydspSIG2[81];
		float fConst490 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst491 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst490)));
		fConst492 = mydsp_faustpower2_f(fConst490);
		fConst493 = ftbl2mydspSIG2[82];
		float fConst494 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst495 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst494)));
		fConst496 = mydsp_faustpower2_f(fConst494);
		fConst497 = ftbl2mydspSIG2[83];
		float fConst498 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst499 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst498)));
		fConst500 = mydsp_faustpower2_f(fConst498);
		fConst501 = ftbl2mydspSIG2[84];
		float fConst502 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst503 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst502)));
		fConst504 = mydsp_faustpower2_f(fConst502);
		fConst505 = ftbl2mydspSIG2[85];
		float fConst506 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst507 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst506)));
		fConst508 = mydsp_faustpower2_f(fConst506);
		fConst509 = ftbl2mydspSIG2[86];
		float fConst510 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst511 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst510)));
		fConst512 = mydsp_faustpower2_f(fConst510);
		fConst513 = ftbl2mydspSIG2[87];
		float fConst514 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst515 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst514)));
		fConst516 = mydsp_faustpower2_f(fConst514);
		fConst517 = ftbl2mydspSIG2[88];
		float fConst518 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst519 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst518)));
		fConst520 = mydsp_faustpower2_f(fConst518);
		fConst521 = ftbl2mydspSIG2[89];
		float fConst522 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst523 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst522)));
		fConst524 = mydsp_faustpower2_f(fConst522);
		fConst525 = ftbl2mydspSIG2[90];
		float fConst526 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst527 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst526)));
		fConst528 = mydsp_faustpower2_f(fConst526);
		fConst529 = ftbl2mydspSIG2[91];
		float fConst530 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst531 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst530)));
		fConst532 = mydsp_faustpower2_f(fConst530);
		fConst533 = ftbl2mydspSIG2[92];
		float fConst534 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst535 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst534)));
		fConst536 = mydsp_faustpower2_f(fConst534);
		fConst537 = ftbl2mydspSIG2[93];
		float fConst538 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst539 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst538)));
		fConst540 = mydsp_faustpower2_f(fConst538);
		fConst541 = ftbl2mydspSIG2[94];
		float fConst542 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst543 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst542)));
		fConst544 = mydsp_faustpower2_f(fConst542);
		fConst545 = ftbl2mydspSIG2[95];
		float fConst546 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst547 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst546)));
		fConst548 = mydsp_faustpower2_f(fConst546);
		fConst549 = ftbl2mydspSIG2[96];
		float fConst550 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst551 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst550)));
		fConst552 = mydsp_faustpower2_f(fConst550);
		fConst553 = ftbl2mydspSIG2[97];
		float fConst554 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst555 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst554)));
		fConst556 = mydsp_faustpower2_f(fConst554);
		fConst557 = ftbl2mydspSIG2[98];
		float fConst558 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst559 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst558)));
		fConst560 = mydsp_faustpower2_f(fConst558);
		fConst561 = ftbl2mydspSIG2[99];
		float fConst562 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst563 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst562)));
		fConst564 = mydsp_faustpower2_f(fConst562);
		fConst565 = (2.0f * fConst0);
		fConst566 = (1.0f / std::max<float>(1.0f, fConst565));
		fConst567 = (2.0f / fConst0);
		float fConst568 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst569 = ((0.0f - (6.90775537f * fConst568)) / fConst0);
		float fConst570 = std::exp((0.25f * fConst569));
		float fConst571 = mydsp_faustpower2_f(fConst570);
		float fConst572 = std::cos((12566.3711f / fConst0));
		float fConst573 = (1.0f - (fConst571 * fConst572));
		float fConst574 = (1.0f - fConst571);
		float fConst575 = (fConst573 / fConst574);
		float fConst576 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst573) / mydsp_faustpower2_f(fConst574)) + -1.0f)));
		fConst577 = (fConst575 - fConst576);
		fConst578 = (fConst570 * (fConst576 + (1.0f - fConst575)));
		fConst579 = ((std::exp(fConst569) / fConst570) + -1.0f);
		float fConst580 = (1.0f / std::tan((314.159271f / fConst0)));
		fConst581 = (1.0f / (fConst580 + 1.0f));
		fConst582 = (1.0f - fConst580);
		float fConst583 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst584 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst568 - fConst583))));
		iConst585 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst586 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst583 + -1.0f))));
		float fConst587 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst588 = ((0.0f - (6.90775537f * fConst587)) / fConst0);
		float fConst589 = std::exp((0.25f * fConst588));
		float fConst590 = mydsp_faustpower2_f(fConst589);
		float fConst591 = (1.0f - (fConst590 * fConst572));
		float fConst592 = (1.0f - fConst590);
		float fConst593 = (fConst591 / fConst592);
		float fConst594 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst591) / mydsp_faustpower2_f(fConst592)) + -1.0f)));
		fConst595 = (fConst593 - fConst594);
		fConst596 = (fConst589 * (fConst594 + (1.0f - fConst593)));
		fConst597 = ((std::exp(fConst588) / fConst589) + -1.0f);
		float fConst598 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst599 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst587 - fConst598))));
		iConst600 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst598 + -1.0f))));
		float fConst601 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst602 = ((0.0f - (6.90775537f * fConst601)) / fConst0);
		float fConst603 = std::exp((0.25f * fConst602));
		float fConst604 = mydsp_faustpower2_f(fConst603);
		float fConst605 = (1.0f - (fConst604 * fConst572));
		float fConst606 = (1.0f - fConst604);
		float fConst607 = (fConst605 / fConst606);
		float fConst608 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst605) / mydsp_faustpower2_f(fConst606)) + -1.0f)));
		fConst609 = (fConst607 - fConst608);
		fConst610 = (fConst603 * (fConst608 + (1.0f - fConst607)));
		fConst611 = ((std::exp(fConst602) / fConst603) + -1.0f);
		float fConst612 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst613 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst601 - fConst612))));
		iConst614 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst612 + -1.0f))));
		float fConst615 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst616 = ((0.0f - (6.90775537f * fConst615)) / fConst0);
		float fConst617 = std::exp((0.25f * fConst616));
		float fConst618 = mydsp_faustpower2_f(fConst617);
		float fConst619 = (1.0f - (fConst618 * fConst572));
		float fConst620 = (1.0f - fConst618);
		float fConst621 = (fConst619 / fConst620);
		float fConst622 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst619) / mydsp_faustpower2_f(fConst620)) + -1.0f)));
		fConst623 = (fConst621 - fConst622);
		fConst624 = (fConst617 * (fConst622 + (1.0f - fConst621)));
		fConst625 = ((std::exp(fConst616) / fConst617) + -1.0f);
		float fConst626 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst627 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst615 - fConst626))));
		iConst628 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst626 + -1.0f))));
		float fConst629 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst630 = ((0.0f - (6.90775537f * fConst629)) / fConst0);
		float fConst631 = std::exp((0.25f * fConst630));
		float fConst632 = mydsp_faustpower2_f(fConst631);
		float fConst633 = (1.0f - (fConst632 * fConst572));
		float fConst634 = (1.0f - fConst632);
		float fConst635 = (fConst633 / fConst634);
		float fConst636 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst633) / mydsp_faustpower2_f(fConst634)) + -1.0f)));
		fConst637 = (fConst635 - fConst636);
		fConst638 = (fConst631 * (fConst636 + (1.0f - fConst635)));
		fConst639 = ((std::exp(fConst630) / fConst631) + -1.0f);
		float fConst640 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst641 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst629 - fConst640))));
		iConst642 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst640 + -1.0f))));
		float fConst643 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst644 = ((0.0f - (6.90775537f * fConst643)) / fConst0);
		float fConst645 = std::exp((0.25f * fConst644));
		float fConst646 = mydsp_faustpower2_f(fConst645);
		float fConst647 = (1.0f - (fConst646 * fConst572));
		float fConst648 = (1.0f - fConst646);
		float fConst649 = (fConst647 / fConst648);
		float fConst650 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst647) / mydsp_faustpower2_f(fConst648)) + -1.0f)));
		fConst651 = (fConst649 - fConst650);
		fConst652 = (fConst645 * (fConst650 + (1.0f - fConst649)));
		fConst653 = ((std::exp(fConst644) / fConst645) + -1.0f);
		float fConst654 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst655 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst643 - fConst654))));
		iConst656 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst654 + -1.0f))));
		float fConst657 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst658 = ((0.0f - (6.90775537f * fConst657)) / fConst0);
		float fConst659 = std::exp((0.25f * fConst658));
		float fConst660 = mydsp_faustpower2_f(fConst659);
		float fConst661 = (1.0f - (fConst660 * fConst572));
		float fConst662 = (1.0f - fConst660);
		float fConst663 = (fConst661 / fConst662);
		float fConst664 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst661) / mydsp_faustpower2_f(fConst662)) + -1.0f)));
		fConst665 = (fConst663 - fConst664);
		fConst666 = (fConst659 * (fConst664 + (1.0f - fConst663)));
		fConst667 = ((std::exp(fConst658) / fConst659) + -1.0f);
		float fConst668 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst669 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst657 - fConst668))));
		iConst670 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst668 + -1.0f))));
		float fConst671 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst672 = ((0.0f - (6.90775537f * fConst671)) / fConst0);
		float fConst673 = std::exp((0.25f * fConst672));
		float fConst674 = mydsp_faustpower2_f(fConst673);
		float fConst675 = (1.0f - (fConst674 * fConst572));
		float fConst676 = (1.0f - fConst674);
		float fConst677 = (fConst675 / fConst676);
		float fConst678 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst675) / mydsp_faustpower2_f(fConst676)) + -1.0f)));
		fConst679 = (fConst677 - fConst678);
		fConst680 = (fConst673 * (fConst678 + (1.0f - fConst677)));
		fConst681 = ((std::exp(fConst672) / fConst673) + -1.0f);
		float fConst682 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst683 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst671 - fConst682))));
		iConst684 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst682 + -1.0f))));
		fConst685 = mydsp_faustpower2_f(fConst66);
		fConst686 = (2.0f * fConst685);
		fConst687 = (4.0f * (mydsp_faustpower2_f(fConst0) * std::tan((628.318542f / fConst0))));
		fConst688 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst689 = (1.0f - fConst688);
		fConst690 = (3500.0f * fConst689);
		fConst691 = (0.5f / fConst0);
		fConst692 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst693 = (1.0f - fConst692);
		fConst694 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst695 = (1.0f - fConst694);
		fConst696 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst697 = (1.0f - fConst696);
		fConst698 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst699 = (1.0f - fConst698);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(-6.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.69999999999999996f);
		fHslider3 = FAUSTFLOAT(20000.0f);
		fHslider4 = FAUSTFLOAT(2000.0f);
		fHslider5 = FAUSTFLOAT(500.0f);
		fHslider6 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
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
		fHslider23 = FAUSTFLOAT(0.0f);
		fHslider24 = FAUSTFLOAT(440.0f);
		fHslider25 = FAUSTFLOAT(0.5f);
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
		fHslider89 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider90 = FAUSTFLOAT(0.69999999999999996f);
		fHslider91 = FAUSTFLOAT(20000.0f);
		fHslider92 = FAUSTFLOAT(2000.0f);
		fHslider93 = FAUSTFLOAT(500.0f);
		fHslider94 = FAUSTFLOAT(0.0f);
		fHslider95 = FAUSTFLOAT(440.0f);
		fHslider96 = FAUSTFLOAT(0.01f);
		fHslider97 = FAUSTFLOAT(1500.0f);
		fHslider98 = FAUSTFLOAT(1.0f);
		fHslider99 = FAUSTFLOAT(0.10000000000000001f);
		fHslider100 = FAUSTFLOAT(0.10000000000000001f);
		fHslider101 = FAUSTFLOAT(0.0f);
		fHslider102 = FAUSTFLOAT(0.69999999999999996f);
		fHslider103 = FAUSTFLOAT(50.0f);
		fHslider104 = FAUSTFLOAT(0.0f);
		fHslider105 = FAUSTFLOAT(0.69999999999999996f);
		fHslider106 = FAUSTFLOAT(0.0f);
		fHslider107 = FAUSTFLOAT(0.69999999999999996f);
		fHslider108 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider109 = FAUSTFLOAT(0.69999999999999996f);
		fHslider110 = FAUSTFLOAT(20000.0f);
		fHslider111 = FAUSTFLOAT(2000.0f);
		fHslider112 = FAUSTFLOAT(500.0f);
		fHslider113 = FAUSTFLOAT(0.0f);
		fHslider114 = FAUSTFLOAT(10.0f);
		fHslider115 = FAUSTFLOAT(10.0f);
		fHslider116 = FAUSTFLOAT(10.0f);
		fHslider117 = FAUSTFLOAT(1.0f);
		fHslider118 = FAUSTFLOAT(0.10000000000000001f);
		fHslider119 = FAUSTFLOAT(0.10000000000000001f);
		fHslider120 = FAUSTFLOAT(0.0f);
		fHslider121 = FAUSTFLOAT(0.69999999999999996f);
		fHslider122 = FAUSTFLOAT(50.0f);
		fHslider123 = FAUSTFLOAT(0.0f);
		fHslider124 = FAUSTFLOAT(0.69999999999999996f);
		fHslider125 = FAUSTFLOAT(0.0f);
		fHslider126 = FAUSTFLOAT(0.69999999999999996f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec6[l1] = 0;
		}
		for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
			fRec5[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec7[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec3[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec14[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec19[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec20[l21] = 0.0f;
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
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec25[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec26[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec27[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec30[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec29[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec28[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec31[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec2[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec1[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec0[l35] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec38[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec36[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec41[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec43[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec42[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec45[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec46[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec47[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec48[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec35[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec51[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec50[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec49[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec52[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec34[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec33[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec32[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fVec4[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fVec5[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fVec6[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			iRec57[l59] = 0;
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec56[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec60[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			iRec59[l62] = 0;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec62[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec61[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 3); l65 = (l65 + 1)) {
			fRec58[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec65[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			iRec64[l67] = 0;
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec63[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec68[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			iRec67[l70] = 0;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec66[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec71[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			iRec70[l73] = 0;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec69[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec74[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec73[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec72[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 3); l78 = (l78 + 1)) {
			fRec75[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec55[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec54[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec53[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			iRec79[l82] = 0;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec83[l83] = 0.0f;
		}
		IOTA = 0;
		for (int l84 = 0; (l84 < 2048); l84 = (l84 + 1)) {
			fVec7[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fVec8[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fVec9[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fVec10[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec84[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec81[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec85[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec88[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec87[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			iRec89[l93] = 0;
		}
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec86[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec90[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec91[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec92[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec93[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec94[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec95[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec96[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec97[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec98[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec99[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec100[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec101[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec102[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec103[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec104[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 3); l110 = (l110 + 1)) {
			fRec105[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 3); l111 = (l111 + 1)) {
			fRec106[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 3); l112 = (l112 + 1)) {
			fRec107[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 3); l113 = (l113 + 1)) {
			fRec108[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 3); l114 = (l114 + 1)) {
			fRec109[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 3); l115 = (l115 + 1)) {
			fRec110[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 3); l116 = (l116 + 1)) {
			fRec111[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 3); l117 = (l117 + 1)) {
			fRec112[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 3); l118 = (l118 + 1)) {
			fRec113[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 3); l119 = (l119 + 1)) {
			fRec114[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 3); l120 = (l120 + 1)) {
			fRec115[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 3); l121 = (l121 + 1)) {
			fRec116[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 3); l122 = (l122 + 1)) {
			fRec117[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 3); l123 = (l123 + 1)) {
			fRec118[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 3); l124 = (l124 + 1)) {
			fRec119[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 3); l125 = (l125 + 1)) {
			fRec120[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 3); l126 = (l126 + 1)) {
			fRec121[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 3); l127 = (l127 + 1)) {
			fRec122[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 3); l128 = (l128 + 1)) {
			fRec123[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 3); l129 = (l129 + 1)) {
			fRec124[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 3); l130 = (l130 + 1)) {
			fRec125[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 3); l131 = (l131 + 1)) {
			fRec126[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 3); l132 = (l132 + 1)) {
			fRec127[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 3); l133 = (l133 + 1)) {
			fRec128[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 3); l134 = (l134 + 1)) {
			fRec129[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec130[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 3); l136 = (l136 + 1)) {
			fRec131[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 3); l137 = (l137 + 1)) {
			fRec132[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 3); l138 = (l138 + 1)) {
			fRec133[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 3); l139 = (l139 + 1)) {
			fRec134[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 3); l140 = (l140 + 1)) {
			fRec135[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 3); l141 = (l141 + 1)) {
			fRec136[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 3); l142 = (l142 + 1)) {
			fRec137[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 3); l143 = (l143 + 1)) {
			fRec138[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fVec11[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 2048); l145 = (l145 + 1)) {
			fVec12[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec82[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec141[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 2); l148 = (l148 + 1)) {
			fRec140[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec139[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 3); l150 = (l150 + 1)) {
			fRec142[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 3); l151 = (l151 + 1)) {
			fRec78[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 3); l152 = (l152 + 1)) {
			fRec77[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 3); l153 = (l153 + 1)) {
			fRec76[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 2); l154 = (l154 + 1)) {
			fRec146[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec148[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec152[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec151[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec150[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec149[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec147[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec155[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec154[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 2); l163 = (l163 + 1)) {
			fRec153[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 3); l164 = (l164 + 1)) {
			fRec156[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 3); l165 = (l165 + 1)) {
			fRec145[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 3); l166 = (l166 + 1)) {
			fRec144[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 3); l167 = (l167 + 1)) {
			fRec143[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fVec13[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fVec14[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fVec15[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec186[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 2); l172 = (l172 + 1)) {
			fRec182[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fRec187[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 4); l174 = (l174 + 1)) {
			fRec189[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 1024); l175 = (l175 + 1)) {
			fRec190[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fVec16[l176] = 0.0f;
		}
		for (int l177 = 0; (l177 < 3); l177 = (l177 + 1)) {
			fVec17[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 2048); l178 = (l178 + 1)) {
			fRec188[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec178[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 2); l180 = (l180 + 1)) {
			fRec174[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 1024); l181 = (l181 + 1)) {
			fRec170[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 4); l182 = (l182 + 1)) {
			fRec168[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 2048); l183 = (l183 + 1)) {
			fRec163[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 2); l184 = (l184 + 1)) {
			fRec161[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fRec191[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 3); l186 = (l186 + 1)) {
			fRec160[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			iRec192[l187] = 0;
		}
		for (int l188 = 0; (l188 < 2); l188 = (l188 + 1)) {
			iRec193[l188] = 0;
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec196[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 2); l190 = (l190 + 1)) {
			fRec195[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 2); l191 = (l191 + 1)) {
			fRec194[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 3); l192 = (l192 + 1)) {
			fRec197[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 3); l193 = (l193 + 1)) {
			fRec159[l193] = 0.0f;
		}
		for (int l194 = 0; (l194 < 3); l194 = (l194 + 1)) {
			fRec158[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 3); l195 = (l195 + 1)) {
			fRec157[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec202[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 2); l197 = (l197 + 1)) {
			fVec18[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 2); l198 = (l198 + 1)) {
			fVec19[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 2); l199 = (l199 + 1)) {
			fVec20[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 2); l200 = (l200 + 1)) {
			iRec203[l200] = 0;
		}
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec201[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 3); l202 = (l202 + 1)) {
			fRec204[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 3); l203 = (l203 + 1)) {
			fRec205[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 3); l204 = (l204 + 1)) {
			fRec206[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 3); l205 = (l205 + 1)) {
			fRec207[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 3); l206 = (l206 + 1)) {
			fRec208[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 3); l207 = (l207 + 1)) {
			fRec209[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 3); l208 = (l208 + 1)) {
			fRec210[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 3); l209 = (l209 + 1)) {
			fRec211[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 3); l210 = (l210 + 1)) {
			fRec212[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 3); l211 = (l211 + 1)) {
			fRec213[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 3); l212 = (l212 + 1)) {
			fRec214[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 3); l213 = (l213 + 1)) {
			fRec215[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 3); l214 = (l214 + 1)) {
			fRec216[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 3); l215 = (l215 + 1)) {
			fRec217[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec218[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 3); l217 = (l217 + 1)) {
			fRec219[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec220[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 3); l219 = (l219 + 1)) {
			fRec221[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 3); l220 = (l220 + 1)) {
			fRec222[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 3); l221 = (l221 + 1)) {
			fRec223[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 3); l222 = (l222 + 1)) {
			fRec224[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec225[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec226[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec227[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 3); l226 = (l226 + 1)) {
			fRec228[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 3); l227 = (l227 + 1)) {
			fRec229[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 3); l228 = (l228 + 1)) {
			fRec230[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 3); l229 = (l229 + 1)) {
			fRec231[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 3); l230 = (l230 + 1)) {
			fRec232[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 3); l231 = (l231 + 1)) {
			fRec233[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 3); l232 = (l232 + 1)) {
			fRec234[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 3); l233 = (l233 + 1)) {
			fRec235[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fRec236[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 3); l235 = (l235 + 1)) {
			fRec237[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 3); l236 = (l236 + 1)) {
			fRec238[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 3); l237 = (l237 + 1)) {
			fRec239[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 3); l238 = (l238 + 1)) {
			fRec240[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 3); l239 = (l239 + 1)) {
			fRec241[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 3); l240 = (l240 + 1)) {
			fRec242[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 3); l241 = (l241 + 1)) {
			fRec243[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 3); l242 = (l242 + 1)) {
			fRec244[l242] = 0.0f;
		}
		for (int l243 = 0; (l243 < 3); l243 = (l243 + 1)) {
			fRec245[l243] = 0.0f;
		}
		for (int l244 = 0; (l244 < 3); l244 = (l244 + 1)) {
			fRec246[l244] = 0.0f;
		}
		for (int l245 = 0; (l245 < 3); l245 = (l245 + 1)) {
			fRec247[l245] = 0.0f;
		}
		for (int l246 = 0; (l246 < 3); l246 = (l246 + 1)) {
			fRec248[l246] = 0.0f;
		}
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec249[l247] = 0.0f;
		}
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec250[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec251[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec252[l250] = 0.0f;
		}
		for (int l251 = 0; (l251 < 2); l251 = (l251 + 1)) {
			fRec253[l251] = 0.0f;
		}
		for (int l252 = 0; (l252 < 2); l252 = (l252 + 1)) {
			fRec254[l252] = 0.0f;
		}
		for (int l253 = 0; (l253 < 2); l253 = (l253 + 1)) {
			fRec255[l253] = 0.0f;
		}
		for (int l254 = 0; (l254 < 2); l254 = (l254 + 1)) {
			fRec256[l254] = 0.0f;
		}
		for (int l255 = 0; (l255 < 2); l255 = (l255 + 1)) {
			fRec257[l255] = 0.0f;
		}
		for (int l256 = 0; (l256 < 2); l256 = (l256 + 1)) {
			fRec258[l256] = 0.0f;
		}
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fRec261[l257] = 0.0f;
		}
		for (int l258 = 0; (l258 < 2); l258 = (l258 + 1)) {
			fRec260[l258] = 0.0f;
		}
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec259[l259] = 0.0f;
		}
		for (int l260 = 0; (l260 < 3); l260 = (l260 + 1)) {
			fRec262[l260] = 0.0f;
		}
		for (int l261 = 0; (l261 < 3); l261 = (l261 + 1)) {
			fRec200[l261] = 0.0f;
		}
		for (int l262 = 0; (l262 < 3); l262 = (l262 + 1)) {
			fRec199[l262] = 0.0f;
		}
		for (int l263 = 0; (l263 < 3); l263 = (l263 + 1)) {
			fRec198[l263] = 0.0f;
		}
		for (int l264 = 0; (l264 < 2); l264 = (l264 + 1)) {
			fRec275[l264] = 0.0f;
		}
		for (int l265 = 0; (l265 < 2); l265 = (l265 + 1)) {
			fRec274[l265] = 0.0f;
		}
		for (int l266 = 0; (l266 < 32768); l266 = (l266 + 1)) {
			fVec21[l266] = 0.0f;
		}
		for (int l267 = 0; (l267 < 2); l267 = (l267 + 1)) {
			iRec280[l267] = 0;
		}
		for (int l268 = 0; (l268 < 3); l268 = (l268 + 1)) {
			fRec279[l268] = 0.0f;
		}
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			iRec282[l269] = 0;
		}
		for (int l270 = 0; (l270 < 3); l270 = (l270 + 1)) {
			fRec281[l270] = 0.0f;
		}
		for (int l271 = 0; (l271 < 2); l271 = (l271 + 1)) {
			iRec284[l271] = 0;
		}
		for (int l272 = 0; (l272 < 3); l272 = (l272 + 1)) {
			fRec283[l272] = 0.0f;
		}
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			iRec286[l273] = 0;
		}
		for (int l274 = 0; (l274 < 3); l274 = (l274 + 1)) {
			fRec285[l274] = 0.0f;
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec289[l275] = 0.0f;
		}
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec288[l276] = 0.0f;
		}
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec287[l277] = 0.0f;
		}
		for (int l278 = 0; (l278 < 3); l278 = (l278 + 1)) {
			fRec290[l278] = 0.0f;
		}
		for (int l279 = 0; (l279 < 3); l279 = (l279 + 1)) {
			fRec278[l279] = 0.0f;
		}
		for (int l280 = 0; (l280 < 3); l280 = (l280 + 1)) {
			fRec277[l280] = 0.0f;
		}
		for (int l281 = 0; (l281 < 3); l281 = (l281 + 1)) {
			fRec276[l281] = 0.0f;
		}
		for (int l282 = 0; (l282 < 2048); l282 = (l282 + 1)) {
			fVec22[l282] = 0.0f;
		}
		for (int l283 = 0; (l283 < 2048); l283 = (l283 + 1)) {
			fVec23[l283] = 0.0f;
		}
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec272[l284] = 0.0f;
		}
		for (int l285 = 0; (l285 < 2); l285 = (l285 + 1)) {
			fRec294[l285] = 0.0f;
		}
		for (int l286 = 0; (l286 < 2); l286 = (l286 + 1)) {
			fRec293[l286] = 0.0f;
		}
		for (int l287 = 0; (l287 < 32768); l287 = (l287 + 1)) {
			fVec24[l287] = 0.0f;
		}
		for (int l288 = 0; (l288 < 4096); l288 = (l288 + 1)) {
			fVec25[l288] = 0.0f;
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec291[l289] = 0.0f;
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec298[l290] = 0.0f;
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec297[l291] = 0.0f;
		}
		for (int l292 = 0; (l292 < 16384); l292 = (l292 + 1)) {
			fVec26[l292] = 0.0f;
		}
		for (int l293 = 0; (l293 < 4096); l293 = (l293 + 1)) {
			fVec27[l293] = 0.0f;
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec295[l294] = 0.0f;
		}
		for (int l295 = 0; (l295 < 2); l295 = (l295 + 1)) {
			fRec302[l295] = 0.0f;
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec301[l296] = 0.0f;
		}
		for (int l297 = 0; (l297 < 32768); l297 = (l297 + 1)) {
			fVec28[l297] = 0.0f;
		}
		for (int l298 = 0; (l298 < 4096); l298 = (l298 + 1)) {
			fVec29[l298] = 0.0f;
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec299[l299] = 0.0f;
		}
		for (int l300 = 0; (l300 < 2); l300 = (l300 + 1)) {
			fRec306[l300] = 0.0f;
		}
		for (int l301 = 0; (l301 < 2); l301 = (l301 + 1)) {
			fRec305[l301] = 0.0f;
		}
		for (int l302 = 0; (l302 < 16384); l302 = (l302 + 1)) {
			fVec30[l302] = 0.0f;
		}
		for (int l303 = 0; (l303 < 2048); l303 = (l303 + 1)) {
			fVec31[l303] = 0.0f;
		}
		for (int l304 = 0; (l304 < 2048); l304 = (l304 + 1)) {
			fVec32[l304] = 0.0f;
		}
		for (int l305 = 0; (l305 < 2); l305 = (l305 + 1)) {
			fRec303[l305] = 0.0f;
		}
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec310[l306] = 0.0f;
		}
		for (int l307 = 0; (l307 < 2); l307 = (l307 + 1)) {
			fRec309[l307] = 0.0f;
		}
		for (int l308 = 0; (l308 < 16384); l308 = (l308 + 1)) {
			fVec33[l308] = 0.0f;
		}
		for (int l309 = 0; (l309 < 4096); l309 = (l309 + 1)) {
			fVec34[l309] = 0.0f;
		}
		for (int l310 = 0; (l310 < 2); l310 = (l310 + 1)) {
			fRec307[l310] = 0.0f;
		}
		for (int l311 = 0; (l311 < 2); l311 = (l311 + 1)) {
			fRec314[l311] = 0.0f;
		}
		for (int l312 = 0; (l312 < 2); l312 = (l312 + 1)) {
			fRec313[l312] = 0.0f;
		}
		for (int l313 = 0; (l313 < 16384); l313 = (l313 + 1)) {
			fVec35[l313] = 0.0f;
		}
		for (int l314 = 0; (l314 < 4096); l314 = (l314 + 1)) {
			fVec36[l314] = 0.0f;
		}
		for (int l315 = 0; (l315 < 2); l315 = (l315 + 1)) {
			fRec311[l315] = 0.0f;
		}
		for (int l316 = 0; (l316 < 2); l316 = (l316 + 1)) {
			fRec318[l316] = 0.0f;
		}
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec317[l317] = 0.0f;
		}
		for (int l318 = 0; (l318 < 16384); l318 = (l318 + 1)) {
			fVec37[l318] = 0.0f;
		}
		for (int l319 = 0; (l319 < 2048); l319 = (l319 + 1)) {
			fVec38[l319] = 0.0f;
		}
		for (int l320 = 0; (l320 < 2); l320 = (l320 + 1)) {
			fRec315[l320] = 0.0f;
		}
		for (int l321 = 0; (l321 < 3); l321 = (l321 + 1)) {
			fRec264[l321] = 0.0f;
		}
		for (int l322 = 0; (l322 < 3); l322 = (l322 + 1)) {
			fRec265[l322] = 0.0f;
		}
		for (int l323 = 0; (l323 < 3); l323 = (l323 + 1)) {
			fRec266[l323] = 0.0f;
		}
		for (int l324 = 0; (l324 < 3); l324 = (l324 + 1)) {
			fRec267[l324] = 0.0f;
		}
		for (int l325 = 0; (l325 < 3); l325 = (l325 + 1)) {
			fRec268[l325] = 0.0f;
		}
		for (int l326 = 0; (l326 < 3); l326 = (l326 + 1)) {
			fRec269[l326] = 0.0f;
		}
		for (int l327 = 0; (l327 < 3); l327 = (l327 + 1)) {
			fRec270[l327] = 0.0f;
		}
		for (int l328 = 0; (l328 < 3); l328 = (l328 + 1)) {
			fRec271[l328] = 0.0f;
		}
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			fRec319[l329] = 0.0f;
		}
		for (int l330 = 0; (l330 < 3); l330 = (l330 + 1)) {
			fRec263[l330] = 0.0f;
		}
		for (int l331 = 0; (l331 < 2); l331 = (l331 + 1)) {
			fRec322[l331] = 0.0f;
		}
		for (int l332 = 0; (l332 < 2); l332 = (l332 + 1)) {
			fRec321[l332] = 0.0f;
		}
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			fRec320[l333] = 0.0f;
		}
		for (int l334 = 0; (l334 < 2); l334 = (l334 + 1)) {
			fRec325[l334] = 0.0f;
		}
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			fRec324[l335] = 0.0f;
		}
		for (int l336 = 0; (l336 < 2); l336 = (l336 + 1)) {
			fRec323[l336] = 0.0f;
		}
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec328[l337] = 0.0f;
		}
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec327[l338] = 0.0f;
		}
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fRec326[l339] = 0.0f;
		}
		for (int l340 = 0; (l340 < 3); l340 = (l340 + 1)) {
			fRec329[l340] = 0.0f;
		}
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fRec332[l341] = 0.0f;
		}
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec331[l342] = 0.0f;
		}
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec330[l343] = 0.0f;
		}
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec335[l344] = 0.0f;
		}
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fRec334[l345] = 0.0f;
		}
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec333[l346] = 0.0f;
		}
		for (int l347 = 0; (l347 < 2); l347 = (l347 + 1)) {
			fRec338[l347] = 0.0f;
		}
		for (int l348 = 0; (l348 < 2); l348 = (l348 + 1)) {
			fRec337[l348] = 0.0f;
		}
		for (int l349 = 0; (l349 < 2); l349 = (l349 + 1)) {
			fRec336[l349] = 0.0f;
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
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->declare(&fHslider99, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider99, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider98, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider98, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider100, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider101, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->declare(&fHslider118, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider118, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider117, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider117, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider119, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider119, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider120, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider120, 0.0f, -96.0f, 0.0f, 0.100000001f);
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
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider103, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider102, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider93, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider104, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider105, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider92, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider92, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider106, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider106, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider107, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider107, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider91, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider90, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider122, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider122, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider121, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider121, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider112, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider112, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider123, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider123, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider124, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider124, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider111, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider111, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider125, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider125, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider126, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider126, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider110, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider110, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider109, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider109, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
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
		ui_interface->addHorizontalSlider("Track 6", &fHslider89, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 7", &fHslider108, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox0);
		ui_interface->addCheckButton("2", &fCheckbox1);
		ui_interface->addCheckButton("3", &fCheckbox2);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->addCheckButton("5", &fCheckbox4);
		ui_interface->addCheckButton("6", &fCheckbox5);
		ui_interface->addCheckButton("7", &fCheckbox6);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider25, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider27, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider94, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider96, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 15");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 17");
		ui_interface->addHorizontalSlider("Control 0", &fHslider113, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 18");
		ui_interface->addHorizontalSlider("Control 0", &fHslider97, 1500.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 19");
		ui_interface->addHorizontalSlider("Control 0", &fHslider95, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider24, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 20");
		ui_interface->addHorizontalSlider("Control 0", &fHslider114, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 21");
		ui_interface->addHorizontalSlider("Control 0", &fHslider115, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 22");
		ui_interface->addHorizontalSlider("Control 0", &fHslider116, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 23");
		ui_interface->addHorizontalSlider("Control 0", &fHslider0, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider23, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
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
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider63, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (1.25892544f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = float(fHslider0);
		float fSlow2 = std::sqrt((1.0f - fSlow1));
		float fSlow3 = std::pow(10.0f, (0.0f - (1.20000005f * fSlow2)));
		float fSlow4 = (1.0f / float(fHslider2));
		float fSlow5 = std::tan((fConst1 * float(fHslider3)));
		float fSlow6 = (1.0f / fSlow5);
		float fSlow7 = (((fSlow4 + fSlow6) / fSlow5) + 1.0f);
		float fSlow8 = ((std::pow(10.0f, (0.0500000007f * float(fHslider1))) * (1.0f - float(fCheckbox0))) / fSlow7);
		float fSlow9 = float(fHslider4);
		float fSlow10 = std::tan((fConst1 * fSlow9));
		float fSlow11 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow10))));
		float fSlow12 = float(fHslider5);
		float fSlow13 = std::tan((fConst1 * fSlow12));
		float fSlow14 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow13))));
		float fSlow15 = std::min<float>(5.0f, std::max<float>(0.00999999978f, float(fHslider6)));
		float fSlow16 = (0.0125000002f * (std::pow(10.0f, (0.0500000007f * fSlow15)) + 1.0f));
		float fSlow17 = ((2.0f * fSlow15) + 1.0f);
		float fSlow18 = std::tan((fConst1 * ((1875.0f * fSlow17) + 500.0f)));
		float fSlow19 = (1.0f / fSlow18);
		float fSlow20 = (1.0f / (((fSlow19 + 1.41421354f) / fSlow18) + 1.0f));
		float fSlow21 = std::tan((fConst1 * ((62.5f * fSlow17) + 40.0f)));
		float fSlow22 = (1.0f / fSlow21);
		float fSlow23 = (1.0f / (((fSlow22 + 1.41421354f) / fSlow21) + 1.0f));
		float fSlow24 = mydsp_faustpower2_f(fSlow21);
		float fSlow25 = (1.0f / fSlow24);
		float fSlow26 = (((fSlow22 + -1.41421354f) / fSlow21) + 1.0f);
		float fSlow27 = (2.0f * (1.0f - fSlow25));
		float fSlow28 = (0.0f - (2.0f / fSlow24));
		float fSlow29 = (((fSlow19 + -1.41421354f) / fSlow18) + 1.0f);
		float fSlow30 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow18))));
		float fSlow31 = (1.0f / std::max<float>(1.0f, (fConst2 * fSlow17)));
		float fSlow32 = float(fHslider7);
		float fSlow33 = float((fSlow32 > 0.0f));
		float fSlow34 = (0.0f - fSlow32);
		float fSlow35 = (fConst65 * ((1.0f / float(fHslider8)) + -1.0f));
		float fSlow36 = float(fHslider9);
		int iSlow37 = (std::fabs(fSlow36) < 1.1920929e-07f);
		float fSlow38 = (iSlow37 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow37 ? 1.0f : fSlow36)))));
		float fSlow39 = float(fHslider10);
		int iSlow40 = (std::fabs(fSlow39) < 1.1920929e-07f);
		float fSlow41 = (iSlow40 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow40 ? 1.0f : fSlow39)))));
		float fSlow42 = (1.0f - fSlow41);
		float fSlow43 = (1.0f - fSlow38);
		float fSlow44 = float(fHslider11);
		float fSlow45 = (1.0f / float(fHslider12));
		float fSlow46 = std::tan((fConst1 * float(fHslider13)));
		float fSlow47 = (1.0f / fSlow46);
		float fSlow48 = (1.0f / (((fSlow45 + fSlow47) / fSlow46) + 1.0f));
		float fSlow49 = (((fSlow47 - fSlow45) / fSlow46) + 1.0f);
		float fSlow50 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow46))));
		float fSlow51 = (1.0f / fSlow13);
		float fSlow52 = float(fHslider14);
		int iSlow53 = (fSlow52 > 0.0f);
		float fSlow54 = (float(fHslider15) * std::sin((fConst5 * fSlow12)));
		float fSlow55 = (fConst1 * ((fSlow12 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow52)))) / fSlow54));
		float fSlow56 = (fConst1 * (fSlow12 / fSlow54));
		float fSlow57 = (iSlow53 ? fSlow56 : fSlow55);
		float fSlow58 = ((fSlow51 * (fSlow51 - fSlow57)) + 1.0f);
		float fSlow59 = ((fSlow51 * (fSlow51 + fSlow57)) + 1.0f);
		float fSlow60 = (iSlow53 ? fSlow55 : fSlow56);
		float fSlow61 = ((fSlow51 * (fSlow51 + fSlow60)) + 1.0f);
		float fSlow62 = ((fSlow51 * (fSlow51 - fSlow60)) + 1.0f);
		float fSlow63 = (1.0f / fSlow10);
		float fSlow64 = float(fHslider16);
		int iSlow65 = (fSlow64 > 0.0f);
		float fSlow66 = (float(fHslider17) * std::sin((fConst5 * fSlow9)));
		float fSlow67 = (fConst1 * ((fSlow9 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow64)))) / fSlow66));
		float fSlow68 = (fConst1 * (fSlow9 / fSlow66));
		float fSlow69 = (iSlow65 ? fSlow68 : fSlow67);
		float fSlow70 = ((fSlow63 * (fSlow63 - fSlow69)) + 1.0f);
		float fSlow71 = ((fSlow63 * (fSlow63 + fSlow69)) + 1.0f);
		float fSlow72 = (iSlow65 ? fSlow67 : fSlow68);
		float fSlow73 = ((fSlow63 * (fSlow63 + fSlow72)) + 1.0f);
		float fSlow74 = ((fSlow63 * (fSlow63 - fSlow72)) + 1.0f);
		float fSlow75 = (1.0f / fSlow7);
		float fSlow76 = (((fSlow6 - fSlow4) / fSlow5) + 1.0f);
		float fSlow77 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow5))));
		float fSlow78 = (1.0f / float(fHslider19));
		float fSlow79 = std::tan((fConst1 * float(fHslider20)));
		float fSlow80 = (1.0f / fSlow79);
		float fSlow81 = (((fSlow78 + fSlow80) / fSlow79) + 1.0f);
		float fSlow82 = ((std::pow(10.0f, (0.0500000007f * float(fHslider18))) * (1.0f - float(fCheckbox1))) / fSlow81);
		float fSlow83 = float(fHslider21);
		float fSlow84 = std::tan((fConst1 * fSlow83));
		float fSlow85 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow84))));
		float fSlow86 = float(fHslider22);
		float fSlow87 = std::tan((fConst1 * fSlow86));
		float fSlow88 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow87))));
		float fSlow89 = (fConst72 * float((float(fHslider23) > 0.0f)));
		float fSlow90 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider25))) + -0.5f)));
		float fSlow91 = (fConst74 * (float(fHslider24) * fSlow90));
		float fSlow92 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider26)));
		float fSlow93 = (0.5f * fSlow92);
		float fSlow94 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider27)));
		float fSlow95 = (fSlow94 + 5.0f);
		int iSlow96 = (fSlow95 < 23.0f);
		int iSlow97 = (fSlow95 < 24.0f);
		float fSlow98 = (fSlow94 + -18.0f);
		int iSlow99 = (fSlow95 < 22.0f);
		float fSlow100 = (fSlow94 + -17.0f);
		int iSlow101 = (fSlow95 < 21.0f);
		float fSlow102 = (fSlow94 + -16.0f);
		int iSlow103 = (fSlow95 < 20.0f);
		float fSlow104 = (fSlow94 + -15.0f);
		int iSlow105 = (fSlow95 < 19.0f);
		float fSlow106 = (fSlow94 + -14.0f);
		int iSlow107 = (fSlow95 < 18.0f);
		float fSlow108 = (fSlow94 + -13.0f);
		int iSlow109 = (fSlow95 < 17.0f);
		float fSlow110 = (fSlow94 + -12.0f);
		int iSlow111 = (fSlow95 < 16.0f);
		float fSlow112 = (fSlow94 + -11.0f);
		float fSlow113 = (80.0f * fSlow112);
		int iSlow114 = (fSlow95 < 15.0f);
		float fSlow115 = (fSlow94 + -10.0f);
		int iSlow116 = (fSlow95 < 14.0f);
		float fSlow117 = (fSlow94 + -9.0f);
		int iSlow118 = (fSlow95 < 13.0f);
		float fSlow119 = (fSlow94 + -8.0f);
		int iSlow120 = (fSlow95 < 12.0f);
		float fSlow121 = (fSlow94 + -7.0f);
		int iSlow122 = (fSlow95 < 11.0f);
		float fSlow123 = (fSlow94 + -6.0f);
		int iSlow124 = (fSlow95 < 10.0f);
		float fSlow125 = (fSlow94 + -5.0f);
		int iSlow126 = (fSlow95 < 9.0f);
		float fSlow127 = (fSlow94 + -4.0f);
		int iSlow128 = (fSlow95 < 8.0f);
		float fSlow129 = (fSlow94 + -3.0f);
		float fSlow130 = (50.0f * fSlow129);
		int iSlow131 = (fSlow95 < 7.0f);
		float fSlow132 = (fSlow94 + -2.0f);
		int iSlow133 = (fSlow95 < 6.0f);
		float fSlow134 = (fSlow94 + -1.0f);
		int iSlow135 = (fSlow95 < 5.0f);
		int iSlow136 = (fSlow95 < 4.0f);
		float fSlow137 = (fSlow94 + 1.0f);
		int iSlow138 = (fSlow95 < 3.0f);
		float fSlow139 = (fSlow94 + 2.0f);
		int iSlow140 = (fSlow95 < 2.0f);
		float fSlow141 = (fSlow94 + 3.0f);
		int iSlow142 = (fSlow95 < 1.0f);
		float fSlow143 = (fSlow94 + 4.0f);
		int iSlow144 = (fSlow95 < 0.0f);
		float fSlow145 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 800.0f : (iSlow142 ? (800.0f - (400.0f * fSlow95)) : 400.0f)) : (iSlow140 ? (400.0f - (50.0f * fSlow143)) : 350.0f)) : (iSlow138 ? ((100.0f * fSlow141) + 350.0f) : 450.0f)) : (iSlow136 ? (450.0f - (125.0f * fSlow139)) : 325.0f)) : (iSlow135 ? ((275.0f * fSlow137) + 325.0f) : 600.0f)) : (iSlow133 ? (600.0f - (200.0f * fSlow94)) : 400.0f)) : (iSlow131 ? (400.0f - (150.0f * fSlow134)) : 250.0f)) : (iSlow128 ? ((150.0f * fSlow132) + 250.0f) : 400.0f)) : (iSlow126 ? (400.0f - fSlow130) : 350.0f)) : (iSlow124 ? ((310.0f * fSlow127) + 350.0f) : 660.0f)) : (iSlow122 ? (660.0f - (220.0f * fSlow125)) : 440.0f)) : (iSlow120 ? (440.0f - (170.0f * fSlow123)) : 270.0f)) : (iSlow118 ? ((160.0f * fSlow121) + 270.0f) : 430.0f)) : (iSlow116 ? (430.0f - (60.0f * fSlow119)) : 370.0f)) : (iSlow114 ? ((430.0f * fSlow117) + 370.0f) : 800.0f)) : (iSlow111 ? (800.0f - (450.0f * fSlow115)) : 350.0f)) : (iSlow109 ? (350.0f - fSlow113) : 270.0f)) : (iSlow107 ? ((180.0f * fSlow110) + 270.0f) : 450.0f)) : (iSlow105 ? (450.0f - (125.0f * fSlow108)) : 325.0f)) : (iSlow103 ? (325.0f * (fSlow106 + 1.0f)) : 650.0f)) : (iSlow101 ? (650.0f - (250.0f * fSlow104)) : 400.0f)) : (iSlow99 ? (400.0f - (110.0f * fSlow102)) : 290.0f)) : (iSlow96 ? ((110.0f * fSlow100) + 290.0f) : 400.0f)) : (iSlow97 ? (400.0f - (50.0f * fSlow98)) : 350.0f));
		float fSlow146 = (20.0f * fSlow104);
		float fSlow147 = (20.0f * fSlow115);
		float fSlow148 = (10.0f * fSlow125);
		float fSlow149 = (10.0f * fSlow137);
		float fSlow150 = (20.0f * fSlow139);
		float fSlow151 = (20.0f * fSlow141);
		float fSlow152 = (20.0f * fSlow95);
		float fSlow153 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 80.0f : (iSlow142 ? (80.0f - fSlow152) : 60.0f)) : (iSlow140 ? (60.0f - (10.0f * fSlow143)) : 50.0f)) : (iSlow138 ? (fSlow151 + 50.0f) : 70.0f)) : (iSlow136 ? (70.0f - fSlow150) : 50.0f)) : (iSlow135 ? (fSlow149 + 50.0f) : 60.0f)) : (iSlow133 ? (60.0f - (20.0f * fSlow94)) : 40.0f)) : (iSlow131 ? ((20.0f * fSlow134) + 40.0f) : 60.0f)) : (iSlow128 ? (60.0f - (20.0f * fSlow132)) : 40.0f)) : 40.0f) : (iSlow124 ? (40.0f * (fSlow127 + 1.0f)) : 80.0f)) : (iSlow122 ? (80.0f - fSlow148) : 70.0f)) : (iSlow120 ? (70.0f - (30.0f * fSlow123)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow114 ? (40.0f * (fSlow117 + 1.0f)) : 80.0f)) : (iSlow111 ? (80.0f - fSlow147) : 60.0f)) : 60.0f) : (iSlow107 ? (60.0f - (20.0f * fSlow110)) : 40.0f)) : (iSlow105 ? ((10.0f * fSlow108) + 40.0f) : 50.0f)) : 50.0f) : (iSlow101 ? (fSlow146 + 50.0f) : 70.0f)) : (iSlow99 ? (70.0f - (30.0f * fSlow102)) : 40.0f)) : (iSlow96 ? ((30.0f * fSlow100) + 40.0f) : 70.0f)) : (iSlow97 ? (70.0f - (30.0f * fSlow98)) : 40.0f));
		float fSlow154 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 1150.0f : (iSlow142 ? ((450.0f * fSlow95) + 1150.0f) : 1600.0f)) : (iSlow140 ? ((100.0f * fSlow143) + 1600.0f) : 1700.0f)) : (iSlow138 ? (1700.0f - (900.0f * fSlow141)) : 800.0f)) : (iSlow136 ? (800.0f - (100.0f * fSlow139)) : 700.0f)) : (iSlow135 ? ((340.0f * fSlow137) + 700.0f) : 1040.0f)) : (iSlow133 ? ((580.0f * fSlow94) + 1040.0f) : 1620.0f)) : (iSlow131 ? ((130.0f * fSlow134) + 1620.0f) : 1750.0f)) : (iSlow128 ? (1750.0f - (1000.0f * fSlow132)) : 750.0f)) : (iSlow126 ? (750.0f - (150.0f * fSlow129)) : 600.0f)) : (iSlow124 ? ((520.0f * fSlow127) + 600.0f) : 1120.0f)) : (iSlow122 ? ((680.0f * fSlow125) + 1120.0f) : 1800.0f)) : (iSlow120 ? ((50.0f * fSlow123) + 1800.0f) : 1850.0f)) : (iSlow118 ? (1850.0f - (1030.0f * fSlow121)) : 820.0f)) : (iSlow116 ? (820.0f - (190.0f * fSlow119)) : 630.0f)) : (iSlow114 ? ((520.0f * fSlow117) + 630.0f) : 1150.0f)) : (iSlow111 ? ((850.0f * fSlow115) + 1150.0f) : 2000.0f)) : (iSlow109 ? ((140.0f * fSlow112) + 2000.0f) : 2140.0f)) : (iSlow107 ? (2140.0f - (1340.0f * fSlow110)) : 800.0f)) : (iSlow105 ? (800.0f - (100.0f * fSlow108)) : 700.0f)) : (iSlow103 ? ((380.0f * fSlow106) + 700.0f) : 1080.0f)) : (iSlow101 ? ((620.0f * fSlow104) + 1080.0f) : 1700.0f)) : (iSlow99 ? ((170.0f * fSlow102) + 1700.0f) : 1870.0f)) : (iSlow96 ? (1870.0f - (1070.0f * fSlow100)) : 800.0f)) : (iSlow97 ? (800.0f - (200.0f * fSlow98)) : 600.0f));
		int iSlow155 = (fSlow154 >= 1300.0f);
		float fSlow156 = (fSlow154 + -1300.0f);
		float fSlow157 = (10.0f * fSlow100);
		float fSlow158 = (10.0f * fSlow104);
		float fSlow159 = (10.0f * fSlow127);
		float fSlow160 = (10.0f * fSlow94);
		float fSlow161 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 90.0f : (iSlow142 ? (90.0f - (10.0f * fSlow95)) : 80.0f)) : (iSlow140 ? ((20.0f * fSlow143) + 80.0f) : 100.0f)) : (iSlow138 ? (100.0f - fSlow151) : 80.0f)) : (iSlow136 ? (80.0f - fSlow150) : 60.0f)) : (iSlow135 ? (fSlow149 + 60.0f) : 70.0f)) : (iSlow133 ? (fSlow160 + 70.0f) : 80.0f)) : (iSlow131 ? ((10.0f * fSlow134) + 80.0f) : 90.0f)) : (iSlow128 ? (90.0f - (10.0f * fSlow132)) : 80.0f)) : 80.0f) : (iSlow124 ? (fSlow159 + 80.0f) : 90.0f)) : (iSlow122 ? (90.0f - fSlow148) : 80.0f)) : (iSlow120 ? ((10.0f * fSlow123) + 80.0f) : 90.0f)) : (iSlow118 ? (90.0f - (10.0f * fSlow121)) : 80.0f)) : (iSlow116 ? (80.0f - (20.0f * fSlow119)) : 60.0f)) : (iSlow114 ? ((30.0f * fSlow117) + 60.0f) : 90.0f)) : (iSlow111 ? ((10.0f * fSlow115) + 90.0f) : 100.0f)) : (iSlow109 ? (100.0f - (10.0f * fSlow112)) : 90.0f)) : (iSlow107 ? (90.0f - (10.0f * fSlow110)) : 80.0f)) : (iSlow105 ? (80.0f - (20.0f * fSlow108)) : 60.0f)) : (iSlow103 ? ((30.0f * fSlow106) + 60.0f) : 90.0f)) : (iSlow101 ? (90.0f - fSlow158) : 80.0f)) : (iSlow99 ? ((10.0f * fSlow102) + 80.0f) : 90.0f)) : (iSlow96 ? (90.0f - fSlow157) : 80.0f)) : (iSlow97 ? (80.0f - (20.0f * fSlow98)) : 60.0f));
		float fSlow162 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 0.630957007f : (iSlow142 ? (0.630957007f - (0.567861021f * fSlow95)) : 0.0630960017f)) : (iSlow140 ? ((0.0369039997f * fSlow143) + 0.0630960017f) : 0.100000001f)) : (iSlow138 ? ((0.254812986f * fSlow141) + 0.100000001f) : 0.35481301f)) : (iSlow136 ? (0.35481301f - (0.103624001f * fSlow139)) : 0.251188993f)) : (iSlow135 ? ((0.195494995f * fSlow137) + 0.251188993f) : 0.446684003f)) : (iSlow133 ? (0.446684003f - (0.195494995f * fSlow94)) : 0.251188993f)) : (iSlow131 ? (0.251188993f - (0.219566002f * fSlow134)) : 0.0316229984f)) : (iSlow128 ? ((0.250214994f * fSlow132) + 0.0316229984f) : 0.281838f)) : (iSlow126 ? (0.281838f - (0.181838006f * fSlow129)) : 0.100000001f)) : (iSlow124 ? ((0.401187003f * fSlow127) + 0.100000001f) : 0.501187027f)) : (iSlow122 ? (0.501187027f - (0.301661015f * fSlow125)) : 0.199525997f)) : (iSlow120 ? (0.199525997f - (0.136429995f * fSlow123)) : 0.0630960017f)) : (iSlow118 ? ((0.253131986f * fSlow121) + 0.0630960017f) : 0.316228002f)) : (iSlow116 ? (0.316228002f - (0.216227993f * fSlow119)) : 0.100000001f)) : (iSlow114 ? ((0.401187003f * fSlow117) + 0.100000001f) : 0.501187027f)) : (iSlow111 ? (0.501187027f - (0.401187003f * fSlow115)) : 0.100000001f)) : (iSlow109 ? ((0.151188999f * fSlow112) + 0.100000001f) : 0.251188993f)) : (iSlow107 ? ((0.0306490008f * fSlow110) + 0.251188993f) : 0.281838f)) : (iSlow105 ? (0.281838f - (0.123349003f * fSlow108)) : 0.158489004f)) : (iSlow103 ? ((0.342698008f * fSlow106) + 0.158489004f) : 0.501187027f)) : (iSlow101 ? (0.501187027f - (0.301661015f * fSlow104)) : 0.199525997f)) : (iSlow99 ? (0.199525997f - (0.0216979999f * fSlow102)) : 0.177827999f)) : (iSlow96 ? ((0.138400003f * fSlow100) + 0.177827999f) : 0.316228002f)) : (iSlow97 ? (0.316228002f - (0.216227993f * fSlow98)) : 0.100000001f));
		float fSlow163 = (100.0f * fSlow98);
		float fSlow164 = (50.0f * fSlow106);
		float fSlow165 = (50.0f * fSlow125);
		float fSlow166 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 2800.0f : (iSlow142 ? (2800.0f - (100.0f * fSlow95)) : 2700.0f)) : 2700.0f) : (iSlow138 ? ((130.0f * fSlow141) + 2700.0f) : 2830.0f)) : (iSlow136 ? (2830.0f - (300.0f * fSlow139)) : 2530.0f)) : (iSlow135 ? (2530.0f - (280.0f * fSlow137)) : 2250.0f)) : (iSlow133 ? ((150.0f * fSlow94) + 2250.0f) : 2400.0f)) : (iSlow131 ? ((200.0f * fSlow134) + 2400.0f) : 2600.0f)) : (iSlow128 ? (2600.0f - (200.0f * fSlow132)) : 2400.0f)) : 2400.0f) : (iSlow124 ? ((350.0f * fSlow127) + 2400.0f) : 2750.0f)) : (iSlow122 ? (2750.0f - fSlow165) : 2700.0f)) : (iSlow120 ? ((200.0f * fSlow123) + 2700.0f) : 2900.0f)) : (iSlow118 ? (2900.0f - (200.0f * fSlow121)) : 2700.0f)) : (iSlow116 ? ((50.0f * fSlow119) + 2700.0f) : 2750.0f)) : (iSlow114 ? ((150.0f * fSlow117) + 2750.0f) : 2900.0f)) : (iSlow111 ? (2900.0f - (100.0f * fSlow115)) : 2800.0f)) : (iSlow109 ? ((150.0f * fSlow112) + 2800.0f) : 2950.0f)) : (iSlow107 ? (2950.0f - (120.0f * fSlow110)) : 2830.0f)) : (iSlow105 ? (2830.0f - (130.0f * fSlow108)) : 2700.0f)) : (iSlow103 ? (2700.0f - fSlow164) : 2650.0f)) : (iSlow101 ? (2650.0f - (50.0f * fSlow104)) : 2600.0f)) : (iSlow99 ? ((200.0f * fSlow102) + 2600.0f) : 2800.0f)) : (iSlow96 ? (2800.0f - (200.0f * fSlow100)) : 2600.0f)) : (iSlow97 ? (fSlow163 + 2600.0f) : 2700.0f));
		float fSlow167 = std::tan((fConst1 * fSlow166));
		float fSlow168 = (1.0f / fSlow167);
		float fSlow169 = (20.0f * fSlow117);
		float fSlow170 = (20.0f * fSlow125);
		float fSlow171 = (20.0f * fSlow127);
		float fSlow172 = (60.0f * fSlow137);
		float fSlow173 = ((iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? 120.0f : (iSlow138 ? (120.0f - fSlow151) : 100.0f)) : (iSlow136 ? ((70.0f * fSlow139) + 100.0f) : 170.0f)) : (iSlow135 ? (170.0f - fSlow172) : 110.0f)) : (iSlow133 ? (110.0f - fSlow160) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow124 ? (fSlow171 + 100.0f) : 120.0f)) : (iSlow122 ? (120.0f - fSlow170) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow114 ? (fSlow169 + 100.0f) : 120.0f)) : 120.0f) : (iSlow109 ? (120.0f - (20.0f * fSlow112)) : 100.0f)) : 100.0f) : (iSlow105 ? ((70.0f * fSlow108) + 100.0f) : 170.0f)) : (iSlow103 ? (170.0f - fSlow164) : 120.0f)) : (iSlow101 ? (120.0f - fSlow146) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow166);
		float fSlow174 = (((fSlow168 - fSlow173) / fSlow167) + 1.0f);
		float fSlow175 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow167)));
		float fSlow176 = (((fSlow168 + fSlow173) / fSlow167) + 1.0f);
		float fSlow177 = (iSlow107 ? ((0.0293140002f * fSlow110) + 0.0501190014f) : 0.0794330016f);
		float fSlow178 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 0.100000001f : (iSlow142 ? (0.100000001f - (0.0683770031f * fSlow95)) : 0.0316229984f)) : 0.0316229984f) : (iSlow138 ? ((0.126865998f * fSlow141) + 0.0316229984f) : 0.158489004f)) : (iSlow136 ? (0.158489004f - (0.126865998f * fSlow139)) : 0.0316229984f)) : (iSlow135 ? ((0.323190004f * fSlow137) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow131 ? (0.35481301f - (0.196324006f * fSlow134)) : 0.158489004f)) : (iSlow128 ? (0.158489004f - (0.0693639964f * fSlow132)) : 0.0891249999f)) : (iSlow126 ? (0.0891249999f - (0.0640060008f * fSlow129)) : 0.0251189992f)) : (iSlow124 ? ((0.0456760004f * fSlow127) + 0.0251189992f) : 0.0707949996f)) : (iSlow122 ? ((0.0550980009f * fSlow125) + 0.0707949996f) : 0.125892997f)) : (iSlow120 ? (0.125892997f - (0.0627970025f * fSlow123)) : 0.0630960017f)) : (iSlow118 ? (0.0630960017f - (0.0129770003f * fSlow121)) : 0.0501190014f)) : (iSlow116 ? ((0.020676f * fSlow119) + 0.0501190014f) : 0.0707949996f)) : (iSlow114 ? (0.0707949996f - (0.0456760004f * fSlow117)) : 0.0251189992f)) : (iSlow111 ? ((0.152709007f * fSlow115) + 0.0251189992f) : 0.177827999f)) : (iSlow109 ? (0.177827999f - (0.127709001f * fSlow112)) : 0.0501190014f)) : fSlow177) : (iSlow105 ? (0.0794330016f - (0.0616500005f * fSlow108)) : 0.0177829992f)) : (iSlow103 ? ((0.428900987f * fSlow106) + 0.0177829992f) : 0.446684003f)) : (iSlow101 ? (0.446684003f - (0.195494995f * fSlow104)) : 0.251188993f)) : (iSlow99 ? (0.251188993f - (0.125295997f * fSlow102)) : 0.125892997f)) : (iSlow96 ? ((0.125295997f * fSlow100) + 0.125892997f) : 0.251188993f)) : (iSlow97 ? (0.251188993f - (0.109935001f * fSlow98)) : 0.141253993f));
		float fSlow179 = (fSlow167 * fSlow176);
		float fSlow180 = (350.0f * fSlow94);
		float fSlow181 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 3500.0f : (iSlow142 ? (3500.0f - (200.0f * fSlow95)) : 3300.0f)) : (iSlow140 ? ((400.0f * fSlow143) + 3300.0f) : 3700.0f)) : (iSlow138 ? (3700.0f - (200.0f * fSlow141)) : 3500.0f)) : 3500.0f) : (iSlow135 ? (3500.0f - (1050.0f * fSlow137)) : 2450.0f)) : (iSlow133 ? (fSlow180 + 2450.0f) : 2800.0f)) : (iSlow131 ? ((250.0f * fSlow134) + 2800.0f) : 3050.0f)) : (iSlow128 ? (3050.0f - (450.0f * fSlow132)) : 2600.0f)) : (iSlow126 ? ((75.0f * fSlow129) + 2600.0f) : 2675.0f)) : (iSlow124 ? ((325.0f * fSlow127) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow120 ? ((350.0f * fSlow123) + 3000.0f) : 3350.0f)) : (iSlow118 ? (3350.0f - (350.0f * fSlow121)) : 3000.0f)) : 3000.0f) : (iSlow114 ? ((900.0f * fSlow117) + 3000.0f) : 3900.0f)) : (iSlow111 ? (3900.0f - (300.0f * fSlow115)) : 3600.0f)) : (iSlow109 ? ((300.0f * fSlow112) + 3600.0f) : 3900.0f)) : (iSlow107 ? (3900.0f - (100.0f * fSlow110)) : 3800.0f)) : 3800.0f) : (iSlow103 ? (3800.0f - (900.0f * fSlow106)) : 2900.0f)) : (iSlow101 ? ((300.0f * fSlow104) + 2900.0f) : 3200.0f)) : (iSlow99 ? ((50.0f * fSlow102) + 3200.0f) : 3250.0f)) : (iSlow96 ? (3250.0f - (450.0f * fSlow100)) : 2800.0f)) : (iSlow97 ? (fSlow163 + 2800.0f) : 2900.0f));
		float fSlow182 = std::tan((fConst1 * fSlow181));
		float fSlow183 = (1.0f / fSlow182);
		float fSlow184 = ((iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 130.0f : (iSlow142 ? (fSlow152 + 130.0f) : 150.0f)) : 150.0f) : (iSlow138 ? (150.0f - fSlow151) : 130.0f)) : (iSlow136 ? ((50.0f * fSlow139) + 130.0f) : 180.0f)) : (iSlow135 ? (180.0f - fSlow172) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow124 ? (fSlow159 + 120.0f) : 130.0f)) : (iSlow122 ? (130.0f - fSlow148) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow114 ? ((10.0f * fSlow117) + 120.0f) : 130.0f)) : (iSlow111 ? (fSlow147 + 130.0f) : 150.0f)) : (iSlow109 ? (150.0f - (30.0f * fSlow112)) : 120.0f)) : 120.0f) : (iSlow105 ? ((60.0f * fSlow108) + 120.0f) : 180.0f)) : (iSlow103 ? (180.0f - fSlow164) : 130.0f)) : (iSlow101 ? (130.0f - fSlow158) : 120.0f)) : 120.0f) : (iSlow96 ? (fSlow157 + 120.0f) : 130.0f)) : (iSlow97 ? (130.0f - (10.0f * fSlow98)) : 120.0f)) / fSlow181);
		float fSlow185 = (((fSlow183 - fSlow184) / fSlow182) + 1.0f);
		float fSlow186 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow182)));
		float fSlow187 = (((fSlow183 + fSlow184) / fSlow182) + 1.0f);
		float fSlow188 = (iSlow120 ? (0.100000001f - (0.0841509998f * fSlow123)) : 0.0158489998f);
		float fSlow189 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 0.0158489998f : (iSlow142 ? ((0.00193400006f * fSlow95) + 0.0158489998f) : 0.0177829992f)) : (iSlow140 ? (0.0177829992f - (0.00193400006f * fSlow143)) : 0.0158489998f)) : (iSlow138 ? ((0.0239620004f * fSlow141) + 0.0158489998f) : 0.0398110002f)) : (iSlow136 ? (0.0398110002f - (0.0298110005f * fSlow139)) : 0.00999999978f)) : (iSlow135 ? ((0.344812989f * fSlow137) + 0.00999999978f) : 0.35481301f)) : (iSlow133 ? (0.35481301f - (0.103624001f * fSlow94)) : 0.251188993f)) : (iSlow131 ? (0.251188993f - (0.171755999f * fSlow134)) : 0.0794330016f)) : (iSlow128 ? ((0.0205669999f * fSlow132) + 0.0794330016f) : 0.100000001f)) : (iSlow126 ? (0.100000001f - (0.0601890013f * fSlow129)) : 0.0398110002f)) : (iSlow124 ? ((0.0232849997f * fSlow127) + 0.0398110002f) : 0.0630960017f)) : (iSlow122 ? ((0.0369039997f * fSlow125) + 0.0630960017f) : 0.100000001f)) : fSlow188) : (iSlow118 ? ((0.0635839999f * fSlow121) + 0.0158489998f) : 0.0794330016f)) : (iSlow116 ? (0.0794330016f - (0.0478099994f * fSlow119)) : 0.0316229984f)) : (iSlow114 ? ((0.0683770031f * fSlow117) + 0.0316229984f) : 0.100000001f)) : (iSlow111 ? (0.100000001f - (0.0900000036f * fSlow115)) : 0.00999999978f)) : (iSlow109 ? ((0.0401189998f * fSlow112) + 0.00999999978f) : 0.0501190014f)) : fSlow177) : (iSlow105 ? (0.0794330016f - (0.0694330037f * fSlow108)) : 0.00999999978f)) : (iSlow103 ? ((0.388107002f * fSlow106) + 0.00999999978f) : 0.398106992f)) : (iSlow101 ? (0.398106992f - (0.198580995f * fSlow104)) : 0.199525997f)) : (iSlow99 ? (0.199525997f - (0.099526003f * fSlow102)) : 0.100000001f)) : (iSlow96 ? ((0.151188999f * fSlow100) + 0.100000001f) : 0.251188993f)) : (iSlow97 ? (0.251188993f - (0.0516630001f * fSlow98)) : 0.199525997f));
		float fSlow190 = (fSlow182 * fSlow187);
		float fSlow191 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? 4950.0f : (iSlow135 ? (4950.0f - (2200.0f * fSlow137)) : 2750.0f)) : (iSlow133 ? (fSlow180 + 2750.0f) : 3100.0f)) : (iSlow131 ? ((240.0f * fSlow134) + 3100.0f) : 3340.0f)) : (iSlow128 ? (3340.0f - (440.0f * fSlow132)) : 2900.0f)) : (iSlow126 ? (fSlow130 + 2900.0f) : 2950.0f)) : (iSlow124 ? ((400.0f * fSlow127) + 2950.0f) : 3350.0f)) : (iSlow122 ? (3350.0f - fSlow165) : 3300.0f)) : (iSlow120 ? ((290.0f * fSlow123) + 3300.0f) : 3590.0f)) : (iSlow118 ? (3590.0f - (290.0f * fSlow121)) : 3300.0f)) : (iSlow116 ? ((100.0f * fSlow119) + 3300.0f) : 3400.0f)) : (iSlow114 ? ((1550.0f * fSlow117) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow103 ? (4950.0f - (1700.0f * fSlow106)) : 3250.0f)) : (iSlow101 ? ((330.0f * fSlow104) + 3250.0f) : 3580.0f)) : (iSlow99 ? (3580.0f - (40.0f * fSlow102)) : 3540.0f)) : (iSlow96 ? (3540.0f - (540.0f * fSlow100)) : 3000.0f)) : (iSlow97 ? ((300.0f * fSlow98) + 3000.0f) : 3300.0f));
		float fSlow192 = std::tan((fConst1 * fSlow191));
		float fSlow193 = (1.0f / fSlow192);
		float fSlow194 = ((iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? (iSlow142 ? (iSlow144 ? 140.0f : (iSlow142 ? ((60.0f * fSlow95) + 140.0f) : 200.0f)) : 200.0f) : (iSlow138 ? (200.0f - (65.0f * fSlow141)) : 135.0f)) : (iSlow136 ? ((65.0f * fSlow139) + 135.0f) : 200.0f)) : (iSlow135 ? (200.0f - (70.0f * fSlow137)) : 130.0f)) : (iSlow133 ? (130.0f - fSlow160) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow124 ? (fSlow171 + 120.0f) : 140.0f)) : (iSlow122 ? (140.0f - fSlow170) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow114 ? (fSlow169 + 120.0f) : 140.0f)) : (iSlow111 ? ((60.0f * fSlow115) + 140.0f) : 200.0f)) : (iSlow109 ? (200.0f - fSlow113) : 120.0f)) : 120.0f) : (iSlow105 ? ((80.0f * fSlow108) + 120.0f) : 200.0f)) : (iSlow103 ? (200.0f - (60.0f * fSlow106)) : 140.0f)) : (iSlow101 ? (140.0f - fSlow146) : 120.0f)) : 120.0f) : (iSlow96 ? ((15.0f * fSlow100) + 120.0f) : 135.0f)) : (iSlow97 ? (135.0f - (15.0f * fSlow98)) : 120.0f)) / fSlow191);
		float fSlow195 = (((fSlow193 - fSlow194) / fSlow192) + 1.0f);
		float fSlow196 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow192)));
		float fSlow197 = (((fSlow193 + fSlow194) / fSlow192) + 1.0f);
		float fSlow198 = (iSlow96 ? (iSlow99 ? (iSlow101 ? (iSlow103 ? (iSlow105 ? (iSlow107 ? (iSlow109 ? (iSlow111 ? (iSlow114 ? (iSlow116 ? (iSlow118 ? (iSlow120 ? (iSlow122 ? (iSlow124 ? (iSlow126 ? (iSlow128 ? (iSlow131 ? (iSlow133 ? (iSlow135 ? (iSlow136 ? (iSlow138 ? (iSlow140 ? 0.00100000005f : (iSlow138 ? ((0.000777999987f * fSlow141) + 0.00100000005f) : 0.00177800003f)) : (iSlow136 ? (0.00177800003f - (0.001147f * fSlow139)) : 0.000630999974f)) : (iSlow135 ? ((0.0993689969f * fSlow137) + 0.000630999974f) : 0.100000001f)) : (iSlow133 ? ((0.0258930009f * fSlow94) + 0.100000001f) : 0.125892997f)) : (iSlow131 ? (0.125892997f - (0.0860819966f * fSlow134)) : 0.0398110002f)) : (iSlow128 ? (0.0398110002f - (0.0298110005f * fSlow132)) : 0.00999999978f)) : (iSlow126 ? ((0.00584900007f * fSlow129) + 0.00999999978f) : 0.0158489998f)) : (iSlow124 ? (0.0158489998f - (0.00325999991f * fSlow127)) : 0.0125890002f)) : (iSlow122 ? ((0.0874110013f * fSlow125) + 0.0125890002f) : 0.100000001f)) : fSlow188) : (iSlow118 ? ((0.00410400005f * fSlow121) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow114 ? (0.0199529994f - (0.0167909991f * fSlow117)) : 0.0031620001f)) : (iSlow111 ? (0.0031620001f - (0.00157700002f * fSlow115)) : 0.00158499996f)) : (iSlow109 ? ((0.00472499989f * fSlow112) + 0.00158499996f) : 0.00631000008f)) : (iSlow107 ? (0.00631000008f - (0.00314799999f * fSlow110)) : 0.0031620001f)) : (iSlow105 ? (0.0031620001f - (0.00216199993f * fSlow108)) : 0.00100000005f)) : (iSlow103 ? ((0.0784329996f * fSlow106) + 0.00100000005f) : 0.0794330016f)) : (iSlow101 ? ((0.0205669999f * fSlow104) + 0.0794330016f) : 0.100000001f)) : (iSlow99 ? (0.100000001f - (0.0683770031f * fSlow102)) : 0.0316229984f)) : (iSlow96 ? ((0.0184959993f * fSlow100) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow199 = (fSlow192 * fSlow197);
		float fSlow200 = (fConst65 * ((1.0f / float(fHslider28)) + -1.0f));
		float fSlow201 = float(fHslider29);
		int iSlow202 = (std::fabs(fSlow201) < 1.1920929e-07f);
		float fSlow203 = (iSlow202 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow202 ? 1.0f : fSlow201)))));
		float fSlow204 = float(fHslider30);
		int iSlow205 = (std::fabs(fSlow204) < 1.1920929e-07f);
		float fSlow206 = (iSlow205 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow205 ? 1.0f : fSlow204)))));
		float fSlow207 = (1.0f - fSlow206);
		float fSlow208 = (1.0f - fSlow203);
		float fSlow209 = float(fHslider31);
		float fSlow210 = (1.0f / float(fHslider32));
		float fSlow211 = std::tan((fConst1 * float(fHslider33)));
		float fSlow212 = (1.0f / fSlow211);
		float fSlow213 = (1.0f / (((fSlow210 + fSlow212) / fSlow211) + 1.0f));
		float fSlow214 = (((fSlow212 - fSlow210) / fSlow211) + 1.0f);
		float fSlow215 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow211))));
		float fSlow216 = (1.0f / fSlow87);
		float fSlow217 = float(fHslider34);
		int iSlow218 = (fSlow217 > 0.0f);
		float fSlow219 = (float(fHslider35) * std::sin((fConst5 * fSlow86)));
		float fSlow220 = (fConst1 * ((fSlow86 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow217)))) / fSlow219));
		float fSlow221 = (fConst1 * (fSlow86 / fSlow219));
		float fSlow222 = (iSlow218 ? fSlow221 : fSlow220);
		float fSlow223 = ((fSlow216 * (fSlow216 - fSlow222)) + 1.0f);
		float fSlow224 = ((fSlow216 * (fSlow216 + fSlow222)) + 1.0f);
		float fSlow225 = (iSlow218 ? fSlow220 : fSlow221);
		float fSlow226 = ((fSlow216 * (fSlow216 + fSlow225)) + 1.0f);
		float fSlow227 = ((fSlow216 * (fSlow216 - fSlow225)) + 1.0f);
		float fSlow228 = (1.0f / fSlow84);
		float fSlow229 = float(fHslider36);
		int iSlow230 = (fSlow229 > 0.0f);
		float fSlow231 = (float(fHslider37) * std::sin((fConst5 * fSlow83)));
		float fSlow232 = (fConst1 * ((fSlow83 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow229)))) / fSlow231));
		float fSlow233 = (fConst1 * (fSlow83 / fSlow231));
		float fSlow234 = (iSlow230 ? fSlow233 : fSlow232);
		float fSlow235 = ((fSlow228 * (fSlow228 - fSlow234)) + 1.0f);
		float fSlow236 = ((fSlow228 * (fSlow228 + fSlow234)) + 1.0f);
		float fSlow237 = (iSlow230 ? fSlow232 : fSlow233);
		float fSlow238 = ((fSlow228 * (fSlow228 + fSlow237)) + 1.0f);
		float fSlow239 = ((fSlow228 * (fSlow228 - fSlow237)) + 1.0f);
		float fSlow240 = (1.0f / fSlow81);
		float fSlow241 = (((fSlow80 - fSlow78) / fSlow79) + 1.0f);
		float fSlow242 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow79))));
		float fSlow243 = (1.0f / float(fHslider39));
		float fSlow244 = std::tan((fConst1 * float(fHslider40)));
		float fSlow245 = (1.0f / fSlow244);
		float fSlow246 = (((fSlow243 + fSlow245) / fSlow244) + 1.0f);
		float fSlow247 = ((std::pow(10.0f, (0.0500000007f * float(fHslider38))) * (1.0f - float(fCheckbox2))) / fSlow246);
		float fSlow248 = float(fHslider41);
		float fSlow249 = std::tan((fConst1 * fSlow248));
		float fSlow250 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow249))));
		float fSlow251 = float(fHslider42);
		float fSlow252 = std::tan((fConst1 * fSlow251));
		float fSlow253 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow252))));
		float fSlow254 = float(fHslider43);
		float fSlow255 = float((fSlow254 > 0.0f));
		float fSlow256 = (0.0f - fSlow254);
		float fSlow257 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider44)));
		float fSlow258 = ((fSlow257 * fSlow90) * ((0.0500000007f * fSlow92) + 1.0f));
		float fSlow259 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider45))) * fSlow90) * (1.0f - (0.0299999993f * fSlow92)));
		float fSlow260 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider46))) * fSlow90) * ((0.310000002f * fSlow92) + 1.0f));
		float fSlow261 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider47))) * fSlow90) * (1.0f - (0.270000011f * fSlow92)));
		float fSlow262 = (fConst65 * ((1.0f / float(fHslider48)) + -1.0f));
		float fSlow263 = float(fHslider49);
		int iSlow264 = (std::fabs(fSlow263) < 1.1920929e-07f);
		float fSlow265 = (iSlow264 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow264 ? 1.0f : fSlow263)))));
		float fSlow266 = float(fHslider50);
		int iSlow267 = (std::fabs(fSlow266) < 1.1920929e-07f);
		float fSlow268 = (iSlow267 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow267 ? 1.0f : fSlow266)))));
		float fSlow269 = (1.0f - fSlow268);
		float fSlow270 = (1.0f - fSlow265);
		float fSlow271 = float(fHslider51);
		float fSlow272 = (1.0f / float(fHslider52));
		float fSlow273 = std::tan((fConst1 * float(fHslider53)));
		float fSlow274 = (1.0f / fSlow273);
		float fSlow275 = (1.0f / (((fSlow272 + fSlow274) / fSlow273) + 1.0f));
		float fSlow276 = (((fSlow274 - fSlow272) / fSlow273) + 1.0f);
		float fSlow277 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow273))));
		float fSlow278 = (1.0f / fSlow252);
		float fSlow279 = float(fHslider54);
		int iSlow280 = (fSlow279 > 0.0f);
		float fSlow281 = (float(fHslider55) * std::sin((fConst5 * fSlow251)));
		float fSlow282 = (fConst1 * ((fSlow251 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow279)))) / fSlow281));
		float fSlow283 = (fConst1 * (fSlow251 / fSlow281));
		float fSlow284 = (iSlow280 ? fSlow283 : fSlow282);
		float fSlow285 = ((fSlow278 * (fSlow278 - fSlow284)) + 1.0f);
		float fSlow286 = ((fSlow278 * (fSlow278 + fSlow284)) + 1.0f);
		float fSlow287 = (iSlow280 ? fSlow282 : fSlow283);
		float fSlow288 = ((fSlow278 * (fSlow278 + fSlow287)) + 1.0f);
		float fSlow289 = ((fSlow278 * (fSlow278 - fSlow287)) + 1.0f);
		float fSlow290 = (1.0f / fSlow249);
		float fSlow291 = float(fHslider56);
		int iSlow292 = (fSlow291 > 0.0f);
		float fSlow293 = (float(fHslider57) * std::sin((fConst5 * fSlow248)));
		float fSlow294 = (fConst1 * ((fSlow248 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow291)))) / fSlow293));
		float fSlow295 = (fConst1 * (fSlow248 / fSlow293));
		float fSlow296 = (iSlow292 ? fSlow295 : fSlow294);
		float fSlow297 = ((fSlow290 * (fSlow290 - fSlow296)) + 1.0f);
		float fSlow298 = ((fSlow290 * (fSlow290 + fSlow296)) + 1.0f);
		float fSlow299 = (iSlow292 ? fSlow294 : fSlow295);
		float fSlow300 = ((fSlow290 * (fSlow290 + fSlow299)) + 1.0f);
		float fSlow301 = ((fSlow290 * (fSlow290 - fSlow299)) + 1.0f);
		float fSlow302 = (1.0f / fSlow246);
		float fSlow303 = (((fSlow245 - fSlow243) / fSlow244) + 1.0f);
		float fSlow304 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow244))));
		float fSlow305 = (1.0f / float(fHslider59));
		float fSlow306 = std::tan((fConst1 * float(fHslider60)));
		float fSlow307 = (1.0f / fSlow306);
		float fSlow308 = (((fSlow305 + fSlow307) / fSlow306) + 1.0f);
		float fSlow309 = ((std::pow(10.0f, (0.0500000007f * float(fHslider58))) * (1.0f - float(fCheckbox3))) / fSlow308);
		float fSlow310 = (0.300000012f * fSlow309);
		float fSlow311 = float(fHslider61);
		float fSlow312 = std::tan((fConst1 * fSlow311));
		float fSlow313 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow312))));
		float fSlow314 = float(fHslider62);
		float fSlow315 = std::tan((fConst1 * fSlow314));
		float fSlow316 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow315))));
		float fSlow317 = float(fHslider63);
		float fSlow318 = float((fSlow317 > 0.0f));
		float fSlow319 = (0.0f - fSlow317);
		float fSlow320 = (fConst65 * ((1.0f / float(fHslider64)) + -1.0f));
		float fSlow321 = float(fHslider65);
		int iSlow322 = (std::fabs(fSlow321) < 1.1920929e-07f);
		float fSlow323 = (iSlow322 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow322 ? 1.0f : fSlow321)))));
		float fSlow324 = float(fHslider66);
		int iSlow325 = (std::fabs(fSlow324) < 1.1920929e-07f);
		float fSlow326 = (iSlow325 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow325 ? 1.0f : fSlow324)))));
		float fSlow327 = (1.0f - fSlow326);
		float fSlow328 = (1.0f - fSlow323);
		float fSlow329 = float(fHslider67);
		float fSlow330 = (1.0f / float(fHslider68));
		float fSlow331 = std::tan((fConst1 * float(fHslider69)));
		float fSlow332 = (1.0f / fSlow331);
		float fSlow333 = (1.0f / (((fSlow330 + fSlow332) / fSlow331) + 1.0f));
		float fSlow334 = (((fSlow332 - fSlow330) / fSlow331) + 1.0f);
		float fSlow335 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow331))));
		float fSlow336 = (1.0f / fSlow315);
		float fSlow337 = float(fHslider70);
		int iSlow338 = (fSlow337 > 0.0f);
		float fSlow339 = (float(fHslider71) * std::sin((fConst5 * fSlow314)));
		float fSlow340 = (fConst1 * ((fSlow314 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow337)))) / fSlow339));
		float fSlow341 = (fConst1 * (fSlow314 / fSlow339));
		float fSlow342 = (iSlow338 ? fSlow341 : fSlow340);
		float fSlow343 = ((fSlow336 * (fSlow336 - fSlow342)) + 1.0f);
		float fSlow344 = ((fSlow336 * (fSlow336 + fSlow342)) + 1.0f);
		float fSlow345 = (iSlow338 ? fSlow340 : fSlow341);
		float fSlow346 = ((fSlow336 * (fSlow336 + fSlow345)) + 1.0f);
		float fSlow347 = ((fSlow336 * (fSlow336 - fSlow345)) + 1.0f);
		float fSlow348 = (1.0f / fSlow312);
		float fSlow349 = float(fHslider72);
		int iSlow350 = (fSlow349 > 0.0f);
		float fSlow351 = (float(fHslider73) * std::sin((fConst5 * fSlow311)));
		float fSlow352 = (fConst1 * ((fSlow311 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow349)))) / fSlow351));
		float fSlow353 = (fConst1 * (fSlow311 / fSlow351));
		float fSlow354 = (iSlow350 ? fSlow353 : fSlow352);
		float fSlow355 = ((fSlow348 * (fSlow348 - fSlow354)) + 1.0f);
		float fSlow356 = ((fSlow348 * (fSlow348 + fSlow354)) + 1.0f);
		float fSlow357 = (iSlow350 ? fSlow352 : fSlow353);
		float fSlow358 = ((fSlow348 * (fSlow348 + fSlow357)) + 1.0f);
		float fSlow359 = ((fSlow348 * (fSlow348 - fSlow357)) + 1.0f);
		float fSlow360 = (1.0f / fSlow308);
		float fSlow361 = (((fSlow307 - fSlow305) / fSlow306) + 1.0f);
		float fSlow362 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow306))));
		float fSlow363 = (1.0f / float(fHslider75));
		float fSlow364 = std::tan((fConst1 * float(fHslider76)));
		float fSlow365 = (1.0f / fSlow364);
		float fSlow366 = (((fSlow363 + fSlow365) / fSlow364) + 1.0f);
		float fSlow367 = ((std::pow(10.0f, (0.0500000007f * float(fHslider74))) * (1.0f - float(fCheckbox4))) / fSlow366);
		float fSlow368 = float(fHslider77);
		float fSlow369 = std::tan((fConst1 * fSlow368));
		float fSlow370 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow369))));
		float fSlow371 = float(fHslider78);
		float fSlow372 = std::tan((fConst1 * fSlow371));
		float fSlow373 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow372))));
		float fSlow374 = (0.5f * fSlow257);
		float fSlow375 = (fConst356 * fSlow90);
		float fSlow376 = (27.0f * fSlow90);
		float fSlow377 = (9.0f * fSlow90);
		float fSlow378 = (3.0f * fSlow90);
		float fSlow379 = (fConst65 * ((1.0f / float(fHslider79)) + -1.0f));
		float fSlow380 = float(fHslider80);
		int iSlow381 = (std::fabs(fSlow380) < 1.1920929e-07f);
		float fSlow382 = (iSlow381 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow381 ? 1.0f : fSlow380)))));
		float fSlow383 = float(fHslider81);
		int iSlow384 = (std::fabs(fSlow383) < 1.1920929e-07f);
		float fSlow385 = (iSlow384 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow384 ? 1.0f : fSlow383)))));
		float fSlow386 = (1.0f - fSlow385);
		float fSlow387 = (1.0f - fSlow382);
		float fSlow388 = float(fHslider82);
		float fSlow389 = (1.0f / float(fHslider83));
		float fSlow390 = std::tan((fConst1 * float(fHslider84)));
		float fSlow391 = (1.0f / fSlow390);
		float fSlow392 = (1.0f / (((fSlow389 + fSlow391) / fSlow390) + 1.0f));
		float fSlow393 = (((fSlow391 - fSlow389) / fSlow390) + 1.0f);
		float fSlow394 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow390))));
		float fSlow395 = (1.0f / fSlow372);
		float fSlow396 = float(fHslider85);
		int iSlow397 = (fSlow396 > 0.0f);
		float fSlow398 = (float(fHslider86) * std::sin((fConst5 * fSlow371)));
		float fSlow399 = (fConst1 * ((fSlow371 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow396)))) / fSlow398));
		float fSlow400 = (fConst1 * (fSlow371 / fSlow398));
		float fSlow401 = (iSlow397 ? fSlow400 : fSlow399);
		float fSlow402 = ((fSlow395 * (fSlow395 - fSlow401)) + 1.0f);
		float fSlow403 = ((fSlow395 * (fSlow395 + fSlow401)) + 1.0f);
		float fSlow404 = (iSlow397 ? fSlow399 : fSlow400);
		float fSlow405 = ((fSlow395 * (fSlow395 + fSlow404)) + 1.0f);
		float fSlow406 = ((fSlow395 * (fSlow395 - fSlow404)) + 1.0f);
		float fSlow407 = (1.0f / fSlow369);
		float fSlow408 = float(fHslider87);
		int iSlow409 = (fSlow408 > 0.0f);
		float fSlow410 = (float(fHslider88) * std::sin((fConst5 * fSlow368)));
		float fSlow411 = (fConst1 * ((fSlow368 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow408)))) / fSlow410));
		float fSlow412 = (fConst1 * (fSlow368 / fSlow410));
		float fSlow413 = (iSlow409 ? fSlow412 : fSlow411);
		float fSlow414 = ((fSlow407 * (fSlow407 - fSlow413)) + 1.0f);
		float fSlow415 = ((fSlow407 * (fSlow407 + fSlow413)) + 1.0f);
		float fSlow416 = (iSlow409 ? fSlow411 : fSlow412);
		float fSlow417 = ((fSlow407 * (fSlow407 + fSlow416)) + 1.0f);
		float fSlow418 = ((fSlow407 * (fSlow407 - fSlow416)) + 1.0f);
		float fSlow419 = (1.0f / fSlow366);
		float fSlow420 = (((fSlow365 - fSlow363) / fSlow364) + 1.0f);
		float fSlow421 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow364))));
		float fSlow422 = (1.0f / float(fHslider90));
		float fSlow423 = std::tan((fConst1 * float(fHslider91)));
		float fSlow424 = (1.0f / fSlow423);
		float fSlow425 = (((fSlow422 + fSlow424) / fSlow423) + 1.0f);
		float fSlow426 = ((std::pow(10.0f, (0.0500000007f * float(fHslider89))) * (1.0f - float(fCheckbox5))) / fSlow425);
		float fSlow427 = float(fHslider92);
		float fSlow428 = std::tan((fConst1 * fSlow427));
		float fSlow429 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow428))));
		float fSlow430 = float(fHslider93);
		float fSlow431 = std::tan((fConst1 * fSlow430));
		float fSlow432 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow431))));
		float fSlow433 = (2.0f * fSlow90);
		float fSlow434 = float(fHslider94);
		float fSlow435 = float((fSlow434 > 0.0f));
		float fSlow436 = (0.0f - fSlow434);
		float fSlow437 = (2.0f * float(fHslider95));
		float fSlow438 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider96)));
		float fSlow439 = (1.0f - fSlow438);
		float fSlow440 = (0.00999999978f * float(fHslider97));
		float fSlow441 = (fConst65 * ((1.0f / float(fHslider98)) + -1.0f));
		float fSlow442 = float(fHslider99);
		int iSlow443 = (std::fabs(fSlow442) < 1.1920929e-07f);
		float fSlow444 = (iSlow443 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow443 ? 1.0f : fSlow442)))));
		float fSlow445 = float(fHslider100);
		int iSlow446 = (std::fabs(fSlow445) < 1.1920929e-07f);
		float fSlow447 = (iSlow446 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow446 ? 1.0f : fSlow445)))));
		float fSlow448 = (1.0f - fSlow447);
		float fSlow449 = (1.0f - fSlow444);
		float fSlow450 = float(fHslider101);
		float fSlow451 = (1.0f / float(fHslider102));
		float fSlow452 = std::tan((fConst1 * float(fHslider103)));
		float fSlow453 = (1.0f / fSlow452);
		float fSlow454 = (1.0f / (((fSlow451 + fSlow453) / fSlow452) + 1.0f));
		float fSlow455 = (((fSlow453 - fSlow451) / fSlow452) + 1.0f);
		float fSlow456 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow452))));
		float fSlow457 = (1.0f / fSlow431);
		float fSlow458 = float(fHslider104);
		int iSlow459 = (fSlow458 > 0.0f);
		float fSlow460 = (float(fHslider105) * std::sin((fConst5 * fSlow430)));
		float fSlow461 = (fConst1 * ((fSlow430 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow458)))) / fSlow460));
		float fSlow462 = (fConst1 * (fSlow430 / fSlow460));
		float fSlow463 = (iSlow459 ? fSlow462 : fSlow461);
		float fSlow464 = ((fSlow457 * (fSlow457 - fSlow463)) + 1.0f);
		float fSlow465 = ((fSlow457 * (fSlow457 + fSlow463)) + 1.0f);
		float fSlow466 = (iSlow459 ? fSlow461 : fSlow462);
		float fSlow467 = ((fSlow457 * (fSlow457 + fSlow466)) + 1.0f);
		float fSlow468 = ((fSlow457 * (fSlow457 - fSlow466)) + 1.0f);
		float fSlow469 = (1.0f / fSlow428);
		float fSlow470 = float(fHslider106);
		int iSlow471 = (fSlow470 > 0.0f);
		float fSlow472 = (float(fHslider107) * std::sin((fConst5 * fSlow427)));
		float fSlow473 = (fConst1 * ((fSlow427 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow470)))) / fSlow472));
		float fSlow474 = (fConst1 * (fSlow427 / fSlow472));
		float fSlow475 = (iSlow471 ? fSlow474 : fSlow473);
		float fSlow476 = ((fSlow469 * (fSlow469 - fSlow475)) + 1.0f);
		float fSlow477 = ((fSlow469 * (fSlow469 + fSlow475)) + 1.0f);
		float fSlow478 = (iSlow471 ? fSlow473 : fSlow474);
		float fSlow479 = ((fSlow469 * (fSlow469 + fSlow478)) + 1.0f);
		float fSlow480 = ((fSlow469 * (fSlow469 - fSlow478)) + 1.0f);
		float fSlow481 = (1.0f / fSlow425);
		float fSlow482 = (((fSlow424 - fSlow422) / fSlow423) + 1.0f);
		float fSlow483 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow423))));
		float fSlow484 = (1.0f / float(fHslider109));
		float fSlow485 = std::tan((fConst1 * float(fHslider110)));
		float fSlow486 = (1.0f / fSlow485);
		float fSlow487 = (((fSlow484 + fSlow486) / fSlow485) + 1.0f);
		float fSlow488 = ((std::pow(10.0f, (0.0500000007f * float(fHslider108))) * (1.0f - float(fCheckbox6))) / fSlow487);
		float fSlow489 = float(fHslider111);
		float fSlow490 = std::tan((fConst1 * fSlow489));
		float fSlow491 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow490))));
		float fSlow492 = float(fHslider112);
		float fSlow493 = std::tan((fConst1 * fSlow492));
		float fSlow494 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow493))));
		float fSlow495 = float(fHslider113);
		float fSlow496 = float((fSlow495 > 0.0f));
		float fSlow497 = (0.0f - fSlow495);
		float fSlow498 = float(fHslider114);
		float fSlow499 = (0.00100000005f * float((fSlow498 > 310.0f)));
		float fSlow500 = (fConst66 * std::max<float>(20.0f, fSlow498));
		float fSlow501 = float(fHslider115);
		float fSlow502 = (0.00100000005f * float((fSlow501 > 610.0f)));
		float fSlow503 = (fConst66 * std::max<float>(20.0f, fSlow501));
		float fSlow504 = float(fHslider116);
		float fSlow505 = (0.00100000005f * float((fSlow504 > 1210.0f)));
		float fSlow506 = (fConst66 * std::max<float>(20.0f, fSlow504));
		float fSlow507 = (fConst65 * ((1.0f / float(fHslider117)) + -1.0f));
		float fSlow508 = float(fHslider118);
		int iSlow509 = (std::fabs(fSlow508) < 1.1920929e-07f);
		float fSlow510 = (iSlow509 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow509 ? 1.0f : fSlow508)))));
		float fSlow511 = float(fHslider119);
		int iSlow512 = (std::fabs(fSlow511) < 1.1920929e-07f);
		float fSlow513 = (iSlow512 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow512 ? 1.0f : fSlow511)))));
		float fSlow514 = (1.0f - fSlow513);
		float fSlow515 = (1.0f - fSlow510);
		float fSlow516 = float(fHslider120);
		float fSlow517 = (1.0f / float(fHslider121));
		float fSlow518 = std::tan((fConst1 * float(fHslider122)));
		float fSlow519 = (1.0f / fSlow518);
		float fSlow520 = (1.0f / (((fSlow517 + fSlow519) / fSlow518) + 1.0f));
		float fSlow521 = (((fSlow519 - fSlow517) / fSlow518) + 1.0f);
		float fSlow522 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow518))));
		float fSlow523 = (1.0f / fSlow493);
		float fSlow524 = float(fHslider123);
		int iSlow525 = (fSlow524 > 0.0f);
		float fSlow526 = (float(fHslider124) * std::sin((fConst5 * fSlow492)));
		float fSlow527 = (fConst1 * ((fSlow492 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow524)))) / fSlow526));
		float fSlow528 = (fConst1 * (fSlow492 / fSlow526));
		float fSlow529 = (iSlow525 ? fSlow528 : fSlow527);
		float fSlow530 = ((fSlow523 * (fSlow523 - fSlow529)) + 1.0f);
		float fSlow531 = ((fSlow523 * (fSlow523 + fSlow529)) + 1.0f);
		float fSlow532 = (iSlow525 ? fSlow527 : fSlow528);
		float fSlow533 = ((fSlow523 * (fSlow523 + fSlow532)) + 1.0f);
		float fSlow534 = ((fSlow523 * (fSlow523 - fSlow532)) + 1.0f);
		float fSlow535 = (1.0f / fSlow490);
		float fSlow536 = float(fHslider125);
		int iSlow537 = (fSlow536 > 0.0f);
		float fSlow538 = (float(fHslider126) * std::sin((fConst5 * fSlow489)));
		float fSlow539 = (fConst1 * ((fSlow489 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow536)))) / fSlow538));
		float fSlow540 = (fConst1 * (fSlow489 / fSlow538));
		float fSlow541 = (iSlow537 ? fSlow540 : fSlow539);
		float fSlow542 = ((fSlow535 * (fSlow535 - fSlow541)) + 1.0f);
		float fSlow543 = ((fSlow535 * (fSlow535 + fSlow541)) + 1.0f);
		float fSlow544 = (iSlow537 ? fSlow539 : fSlow540);
		float fSlow545 = ((fSlow535 * (fSlow535 + fSlow544)) + 1.0f);
		float fSlow546 = ((fSlow535 * (fSlow535 - fSlow544)) + 1.0f);
		float fSlow547 = (1.0f / fSlow487);
		float fSlow548 = (((fSlow486 - fSlow484) / fSlow485) + 1.0f);
		float fSlow549 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow485))));
		float fSlow550 = std::pow(10.0f, (0.600000024f * fSlow2));
		float fSlow551 = (fConst690 * ((0.899999976f * fSlow1) + 0.100000001f));
		float fSlow552 = (0.949999988f * fSlow309);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			iRec6[0] = ((1103515245 * iRec6[1]) + 12345);
			float fTemp0 = (4.65661287e-10f * float(iRec6[0]));
			fRec5[0] = (fTemp0 - (fSlow23 * ((fSlow26 * fRec5[2]) + (fSlow27 * fRec5[1]))));
			fRec4[0] = ((fSlow23 * (((fSlow25 * fRec5[0]) + (fSlow28 * fRec5[1])) + (fSlow25 * fRec5[2]))) - (fSlow20 * ((fSlow29 * fRec4[2]) + (fSlow30 * fRec4[1]))));
			fVec1[0] = fSlow32;
			float fTemp1 = (fSlow32 - fVec1[1]);
			fVec2[0] = fSlow34;
			float fTemp2 = (0.0f - (fSlow32 + fVec2[1]));
			float fTemp3 = std::min<float>(1.0f, ((fTemp1 * float((fTemp1 > 0.0f))) + (fSlow33 * (fTemp2 * float((fTemp2 > 0.0f))))));
			fVec3[0] = fTemp3;
			iRec7[0] = (((iRec7[1] + (iRec7[1] > 0)) * (fTemp3 <= fVec3[1])) + (fTemp3 > fVec3[1]));
			float fTemp4 = (fSlow31 * float(iRec7[0]));
			float fTemp5 = (fSlow20 * ((fRec4[2] + (fRec4[0] + (2.0f * fRec4[1]))) * std::max<float>(0.0f, std::min<float>(fTemp4, (2.0f - fTemp4)))));
			fRec8[0] = (int(fTemp3) ? fSlow32 : fRec8[1]);
			fRec3[0] = (fTemp5 - ((fConst4 * (fRec3[1] * std::cos((fConst5 * (fRec8[0] + 50.0f))))) + (fConst6 * fRec3[2])));
			fRec9[0] = (fTemp5 - ((fConst8 * (fRec9[1] * std::cos((fConst5 * (fRec8[0] + 250.0f))))) + (fConst9 * fRec9[2])));
			fRec10[0] = (fTemp5 - ((fConst11 * (fRec10[1] * std::cos((fConst5 * (fRec8[0] + 450.0f))))) + (fConst12 * fRec10[2])));
			fRec11[0] = (fTemp5 - ((fConst14 * (fRec11[1] * std::cos((fConst5 * (fRec8[0] + 650.0f))))) + (fConst15 * fRec11[2])));
			fRec12[0] = (fTemp5 - ((fConst17 * (fRec12[1] * std::cos((fConst5 * (fRec8[0] + 850.0f))))) + (fConst18 * fRec12[2])));
			fRec13[0] = (fTemp5 - ((fConst20 * (fRec13[1] * std::cos((fConst5 * (fRec8[0] + 1050.0f))))) + (fConst21 * fRec13[2])));
			fRec14[0] = (fTemp5 - ((fConst23 * (fRec14[1] * std::cos((fConst5 * (fRec8[0] + 1250.0f))))) + (fConst24 * fRec14[2])));
			fRec15[0] = (fTemp5 - ((fConst26 * (fRec15[1] * std::cos((fConst5 * (fRec8[0] + 1450.0f))))) + (fConst27 * fRec15[2])));
			fRec16[0] = (fTemp5 - ((fConst29 * (fRec16[1] * std::cos((fConst5 * (fRec8[0] + 1650.0f))))) + (fConst30 * fRec16[2])));
			fRec17[0] = (fTemp5 - ((fConst32 * (fRec17[1] * std::cos((fConst5 * (fRec8[0] + 1850.0f))))) + (fConst33 * fRec17[2])));
			fRec18[0] = (fTemp5 - ((fConst35 * (fRec18[1] * std::cos((fConst5 * (fRec8[0] + 2050.0f))))) + (fConst36 * fRec18[2])));
			fRec19[0] = (fTemp5 - ((fConst38 * (fRec19[1] * std::cos((fConst5 * (fRec8[0] + 2250.0f))))) + (fConst39 * fRec19[2])));
			fRec20[0] = (fTemp5 - ((fConst41 * (fRec20[1] * std::cos((fConst5 * (fRec8[0] + 2450.0f))))) + (fConst42 * fRec20[2])));
			fRec21[0] = (fTemp5 - ((fConst44 * (fRec21[1] * std::cos((fConst5 * (fRec8[0] + 2650.0f))))) + (fConst45 * fRec21[2])));
			fRec22[0] = (fTemp5 - ((fConst47 * (fRec22[1] * std::cos((fConst5 * (fRec8[0] + 2850.0f))))) + (fConst48 * fRec22[2])));
			fRec23[0] = (fTemp5 - ((fConst50 * (fRec23[1] * std::cos((fConst5 * (fRec8[0] + 3050.0f))))) + (fConst51 * fRec23[2])));
			fRec24[0] = (fTemp5 - ((fConst53 * (fRec24[1] * std::cos((fConst5 * (fRec8[0] + 3250.0f))))) + (fConst54 * fRec24[2])));
			fRec25[0] = (fTemp5 - ((fConst56 * (fRec25[1] * std::cos((fConst5 * (fRec8[0] + 3450.0f))))) + (fConst57 * fRec25[2])));
			fRec26[0] = (fTemp5 - ((fConst59 * (fRec26[1] * std::cos((fConst5 * (fRec8[0] + 3650.0f))))) + (fConst60 * fRec26[2])));
			fRec27[0] = (fTemp5 - ((fConst62 * (fRec27[1] * std::cos((fConst5 * (fRec8[0] + 3850.0f))))) + (fConst63 * fRec27[2])));
			float fTemp6 = ((((((((((((((((((((fRec3[0] + (0.25f * (fRec9[0] - fRec9[2]))) + (0.111111112f * (fRec10[0] - fRec10[2]))) + (0.0625f * (fRec11[0] - fRec11[2]))) + (0.0399999991f * (fRec12[0] - fRec12[2]))) + (0.027777778f * (fRec13[0] - fRec13[2]))) + (0.0204081628f * (fRec14[0] - fRec14[2]))) + (0.015625f * (fRec15[0] - fRec15[2]))) + (0.0123456791f * (fRec16[0] - fRec16[2]))) + (0.00999999978f * (fRec17[0] - fRec17[2]))) + (0.00826446246f * (fRec18[0] - fRec18[2]))) + (0.0069444445f * (fRec19[0] - fRec19[2]))) + (0.00591715984f * (fRec20[0] - fRec20[2]))) + (0.00510204071f * (fRec21[0] - fRec21[2]))) + (0.00444444455f * (fRec22[0] - fRec22[2]))) + (0.00390625f * (fRec23[0] - fRec23[2]))) + (0.00346020772f * (fRec24[0] - fRec24[2]))) + (0.00308641978f * (fRec25[0] - fRec25[2]))) + (0.00277008303f * (fRec26[0] - fRec26[2]))) + (0.00249999994f * (fRec27[0] - fRec27[2]))) - fRec3[2]);
			float fTemp7 = std::fabs((fSlow16 * fTemp6));
			fRec30[0] = std::max<float>(fTemp7, ((fRec30[1] * fSlow41) + (fTemp7 * fSlow42)));
			fRec29[0] = ((fRec29[1] * fSlow38) + (fRec30[0] * fSlow43));
			fRec28[0] = ((fConst64 * fRec28[1]) + (fSlow35 * std::max<float>(((20.0f * std::log10(fRec29[0])) - fSlow44), 0.0f)));
			float fTemp8 = (fSlow16 * (fTemp6 * std::pow(10.0f, (0.0500000007f * fRec28[0]))));
			fRec31[0] = (fTemp8 - (fSlow48 * ((fSlow49 * fRec31[2]) + (fSlow50 * fRec31[1]))));
			float fTemp9 = (fSlow14 * fRec2[1]);
			fRec2[0] = (fTemp8 - ((fSlow48 * (fRec31[2] + (fRec31[0] + (2.0f * fRec31[1])))) + (((fRec2[2] * fSlow58) + fTemp9) / fSlow59)));
			float fTemp10 = (fSlow11 * fRec1[1]);
			fRec1[0] = ((((fTemp9 + (fRec2[0] * fSlow61)) + (fRec2[2] * fSlow62)) / fSlow59) - (((fRec1[2] * fSlow70) + fTemp10) / fSlow71));
			fRec0[0] = ((((fTemp10 + (fRec1[0] * fSlow73)) + (fRec1[2] * fSlow74)) / fSlow71) - (fSlow75 * ((fSlow76 * fRec0[2]) + (fSlow77 * fRec0[1]))));
			fRec38[0] = (fConst73 + (fRec38[1] - std::floor((fConst73 + fRec38[1]))));
			float fTemp11 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec38[0]))]) + 1.0f);
			fRec36[0] = ((fConst71 * fRec36[1]) + (fSlow89 * fTemp11));
			fRec41[0] = (fConst75 + (fRec41[1] - std::floor((fConst75 + fRec41[1]))));
			fRec40[0] = ((0.999000013f * fRec40[1]) + (0.00100000005f * ((fSlow93 * ftbl0mydspSIG0[int((65536.0f * fRec41[0]))]) + 1.0f)));
			fRec39[0] = ((fConst71 * fRec39[1]) + (fSlow91 * (fRec40[0] * fTemp11)));
			float fTemp12 = ((0.00366666657f * (400.0f - fRec39[0])) + 3.0f);
			float fTemp13 = std::max<float>(1.1920929e-07f, std::fabs(fRec39[0]));
			float fTemp14 = (fRec43[1] + (fConst66 * fTemp13));
			float fTemp15 = (fTemp14 + -1.0f);
			int iTemp16 = (fTemp15 < 0.0f);
			fRec43[0] = (iTemp16 ? fTemp14 : fTemp15);
			float fRec44 = (iTemp16 ? fTemp14 : (fTemp14 + ((1.0f - (fConst0 / fTemp13)) * fTemp15)));
			float fTemp17 = (0.400000006f * ((2.0f * fRec44) + -1.0f));
			float fTemp18 = ((fSlow145 <= fRec39[0]) ? fRec39[0] : fSlow145);
			float fTemp19 = std::tan((fConst1 * fTemp18));
			float fTemp20 = (1.0f / fTemp19);
			float fTemp21 = (fSlow153 / fTemp18);
			float fTemp22 = (((fTemp20 + fTemp21) / fTemp19) + 1.0f);
			fRec42[0] = (fTemp17 - (((fRec42[2] * (((fTemp20 - fTemp21) / fTemp19) + 1.0f)) + (2.0f * (fRec42[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp19)))))) / fTemp22));
			float fTemp23 = (fTemp19 * fTemp22);
			float fTemp24 = ((2.0f * fRec39[0]) + 30.0f);
			float fTemp25 = ((iSlow155 & (fRec39[0] >= 200.0f)) ? (fSlow154 - (0.000952380942f * ((fRec39[0] + -200.0f) * fSlow156))) : ((fSlow154 <= fTemp24) ? fTemp24 : fSlow154));
			float fTemp26 = std::tan((fConst1 * fTemp25));
			float fTemp27 = (1.0f / fTemp26);
			float fTemp28 = (fSlow161 / fTemp25);
			float fTemp29 = (((fTemp27 + fTemp28) / fTemp26) + 1.0f);
			fRec45[0] = (fTemp17 - (((fRec45[2] * (((fTemp27 - fTemp28) / fTemp26) + 1.0f)) + (2.0f * (fRec45[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp26)))))) / fTemp29));
			float fTemp30 = (fTemp26 * fTemp29);
			fRec46[0] = (fTemp17 - (((fRec46[2] * fSlow174) + (2.0f * (fRec46[1] * fSlow175))) / fSlow176));
			fRec47[0] = (fTemp17 - (((fRec47[2] * fSlow185) + (2.0f * (fRec47[1] * fSlow186))) / fSlow187));
			fRec48[0] = (fTemp17 - (((fRec48[2] * fSlow195) + (2.0f * (fRec48[1] * fSlow196))) / fSlow197));
			fRec35[0] = ((fRec36[0] * ((fTemp12 * (((((fRec42[0] / fTemp23) + ((fRec45[0] * fSlow162) / fTemp30)) + ((fRec46[0] * fSlow178) / fSlow179)) + ((fRec47[0] * fSlow189) / fSlow190)) + ((fRec48[0] * fSlow198) / fSlow199))) + (((((fRec42[2] * (0.0f - (fTemp12 / fTemp23))) + (fRec45[2] * (0.0f - ((fTemp12 * fSlow162) / fTemp30)))) + (fRec46[2] * (0.0f - ((fTemp12 * fSlow178) / fSlow179)))) + (fRec47[2] * (0.0f - ((fTemp12 * fSlow189) / fSlow190)))) + (fRec48[2] * (0.0f - ((fTemp12 * fSlow198) / fSlow199)))))) - (fConst76 * ((fConst77 * fRec35[2]) + (fConst78 * fRec35[1]))));
			float fTemp31 = (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])));
			float fTemp32 = std::fabs((fConst70 * fTemp31));
			fRec51[0] = std::max<float>(fTemp32, ((fRec51[1] * fSlow206) + (fTemp32 * fSlow207)));
			fRec50[0] = ((fRec50[1] * fSlow203) + (fRec51[0] * fSlow208));
			fRec49[0] = ((fConst64 * fRec49[1]) + (fSlow200 * std::max<float>(((20.0f * std::log10(fRec50[0])) - fSlow209), 0.0f)));
			float fTemp33 = (fConst70 * (fTemp31 * std::pow(10.0f, (0.0500000007f * fRec49[0]))));
			fRec52[0] = (fTemp33 - (fSlow213 * ((fSlow214 * fRec52[2]) + (fSlow215 * fRec52[1]))));
			float fTemp34 = (fSlow88 * fRec34[1]);
			fRec34[0] = (fTemp33 - ((fSlow213 * (fRec52[2] + (fRec52[0] + (2.0f * fRec52[1])))) + (((fRec34[2] * fSlow223) + fTemp34) / fSlow224)));
			float fTemp35 = (fSlow85 * fRec33[1]);
			fRec33[0] = ((((fTemp34 + (fRec34[0] * fSlow226)) + (fRec34[2] * fSlow227)) / fSlow224) - (((fRec33[2] * fSlow235) + fTemp35) / fSlow236));
			fRec32[0] = ((((fTemp35 + (fRec33[0] * fSlow238)) + (fRec33[2] * fSlow239)) / fSlow236) - (fSlow240 * ((fSlow241 * fRec32[2]) + (fSlow242 * fRec32[1]))));
			float fTemp36 = (fSlow82 * (fRec32[2] + (fRec32[0] + (2.0f * fRec32[1]))));
			float fTemp37 = ((fSlow8 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + fTemp36);
			fVec4[0] = fSlow254;
			float fTemp38 = (fSlow254 - fVec4[1]);
			fVec5[0] = fSlow256;
			float fTemp39 = (0.0f - (fSlow254 + fVec5[1]));
			float fTemp40 = std::min<float>(1.0f, ((fTemp38 * float((fTemp38 > 0.0f))) + (fSlow255 * (fTemp39 * float((fTemp39 > 0.0f))))));
			fVec6[0] = fTemp40;
			iRec57[0] = (((iRec57[1] + (iRec57[1] > 0)) * (fTemp40 <= fVec6[1])) + (fTemp40 > fVec6[1]));
			float fTemp41 = float(iRec57[0]);
			float fTemp42 = (fConst82 * fTemp41);
			float fTemp43 = (fConst81 - fTemp41);
			fRec56[0] = ((fConst79 * fRec56[1]) + (fConst80 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp42, ((fConst83 * fTemp43) + 1.0f))))));
			int iTemp44 = int(fTemp40);
			fRec60[0] = (iTemp44 ? fSlow258 : fRec60[1]);
			iRec59[0] = ((iVec0[1] + iRec59[1]) % int((fConst0 / fRec60[0])));
			float fTemp45 = float(iRec59[0]);
			int iTemp46 = (fTemp40 > 0.0f);
			float fTemp47 = (0.144717798f * (iTemp46 ? 0.00100000005f : 1.60000002f));
			int iTemp48 = (std::fabs(fTemp47) < 1.1920929e-07f);
			float fTemp49 = (iTemp48 ? 0.0f : std::exp((0.0f - (fConst66 / (iTemp48 ? 1.0f : fTemp47)))));
			fRec62[0] = ((fRec62[1] * fTemp49) + ((iTemp46 ? fTemp40 : 0.400000006f) * (1.0f - fTemp49)));
			fRec61[0] = ((fConst87 * fRec61[1]) + (fConst88 * fRec62[0]));
			float fTemp50 = (fRec61[0] + 0.00999999978f);
			float fTemp51 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fRec60[0] * fTemp50) * (1.0f - (0.000500000024f * std::min<float>(fRec60[0], 1000.0f)))))))));
			float fTemp52 = (1.0f / fTemp51);
			float fTemp53 = (((fTemp52 + 1.41421354f) / fTemp51) + 1.0f);
			fRec58[0] = ((2.0f * (((float((fTemp45 < (fConst84 / fRec60[0]))) + -0.5f) + (float((fTemp45 < (fConst85 / fRec60[0]))) + -0.5f)) + (float((fTemp45 < (fConst86 / fRec60[0]))) + -0.5f))) - (((fRec58[2] * (((fTemp52 + -1.41421354f) / fTemp51) + 1.0f)) + (2.0f * (fRec58[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp51)))))) / fTemp53));
			fRec65[0] = (iTemp44 ? fSlow259 : fRec65[1]);
			iRec64[0] = ((iVec0[1] + iRec64[1]) % int((fConst0 / fRec65[0])));
			float fTemp54 = float(iRec64[0]);
			float fTemp55 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fRec65[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec65[0], 1000.0f)))))))));
			float fTemp56 = (1.0f / fTemp55);
			float fTemp57 = (((fTemp56 + 1.41421354f) / fTemp55) + 1.0f);
			fRec63[0] = ((2.0f * (((float((fTemp54 < (fConst84 / fRec65[0]))) + -0.5f) + (float((fTemp54 < (fConst85 / fRec65[0]))) + -0.5f)) + (float((fTemp54 < (fConst86 / fRec65[0]))) + -0.5f))) - (((fRec63[2] * (((fTemp56 + -1.41421354f) / fTemp55) + 1.0f)) + (2.0f * (fRec63[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp55)))))) / fTemp57));
			fRec68[0] = (iTemp44 ? fSlow260 : fRec68[1]);
			iRec67[0] = ((iVec0[1] + iRec67[1]) % int((fConst0 / fRec68[0])));
			float fTemp58 = float(iRec67[0]);
			float fTemp59 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fRec68[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec68[0], 1000.0f)))))))));
			float fTemp60 = (1.0f / fTemp59);
			float fTemp61 = (((fTemp60 + 1.41421354f) / fTemp59) + 1.0f);
			fRec66[0] = ((2.0f * (((float((fTemp58 < (fConst84 / fRec68[0]))) + -0.5f) + (float((fTemp58 < (fConst85 / fRec68[0]))) + -0.5f)) + (float((fTemp58 < (fConst86 / fRec68[0]))) + -0.5f))) - (((fRec66[2] * (((fTemp60 + -1.41421354f) / fTemp59) + 1.0f)) + (2.0f * (fRec66[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp59)))))) / fTemp61));
			fRec71[0] = (iTemp44 ? fSlow261 : fRec71[1]);
			iRec70[0] = ((iVec0[1] + iRec70[1]) % int((fConst0 / fRec71[0])));
			float fTemp62 = float(iRec70[0]);
			float fTemp63 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fRec71[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec71[0], 1000.0f)))))))));
			float fTemp64 = (1.0f / fTemp63);
			float fTemp65 = (((fTemp64 + 1.41421354f) / fTemp63) + 1.0f);
			fRec69[0] = ((2.0f * (((float((fTemp62 < (fConst84 / fRec71[0]))) + -0.5f) + (float((fTemp62 < (fConst85 / fRec71[0]))) + -0.5f)) + (float((fTemp62 < (fConst86 / fRec71[0]))) + -0.5f))) - (((fRec69[2] * (((fTemp64 + -1.41421354f) / fTemp63) + 1.0f)) + (2.0f * (fRec69[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp63)))))) / fTemp65));
			float fTemp66 = (fRec56[0] * (((((fRec58[2] + (fRec58[0] + (2.0f * fRec58[1]))) / fTemp53) + ((fRec63[2] + (fRec63[0] + (2.0f * fRec63[1]))) / fTemp57)) + ((fRec66[2] + (fRec66[0] + (2.0f * fRec66[1]))) / fTemp61)) + ((fRec69[2] + (fRec69[0] + (2.0f * fRec69[1]))) / fTemp65)));
			float fTemp67 = std::fabs((0.25f * fTemp66));
			fRec74[0] = std::max<float>(fTemp67, ((fRec74[1] * fSlow268) + (fTemp67 * fSlow269)));
			fRec73[0] = ((fRec73[1] * fSlow265) + (fRec74[0] * fSlow270));
			fRec72[0] = ((fConst64 * fRec72[1]) + (fSlow262 * std::max<float>(((20.0f * std::log10(fRec73[0])) - fSlow271), 0.0f)));
			float fTemp68 = (0.25f * (fTemp66 * std::pow(10.0f, (0.0500000007f * fRec72[0]))));
			fRec75[0] = (fTemp68 - (fSlow275 * ((fSlow276 * fRec75[2]) + (fSlow277 * fRec75[1]))));
			float fTemp69 = (fSlow253 * fRec55[1]);
			fRec55[0] = (fTemp68 - ((fSlow275 * (fRec75[2] + (fRec75[0] + (2.0f * fRec75[1])))) + (((fRec55[2] * fSlow285) + fTemp69) / fSlow286)));
			float fTemp70 = (fSlow250 * fRec54[1]);
			fRec54[0] = ((((fTemp69 + (fRec55[0] * fSlow288)) + (fRec55[2] * fSlow289)) / fSlow286) - (((fRec54[2] * fSlow297) + fTemp70) / fSlow298));
			fRec53[0] = ((((fTemp70 + (fRec54[0] * fSlow300)) + (fRec54[2] * fSlow301)) / fSlow298) - (fSlow302 * ((fSlow303 * fRec53[2]) + (fSlow304 * fRec53[1]))));
			float fTemp71 = (fSlow247 * (fRec53[2] + (fRec53[0] + (2.0f * fRec53[1]))));
			iRec79[0] = 0;
			fRec83[0] = ((0.949999988f * fRec83[1]) + (0.0500000007f * fRec82[1]));
			float fTemp72 = ((0.99000001f * fRec83[0]) + float(iRec79[1]));
			fVec7[(IOTA & 2047)] = fTemp72;
			fVec8[0] = fSlow317;
			float fTemp73 = (fSlow317 - fVec8[1]);
			fVec9[0] = fSlow319;
			float fTemp74 = (0.0f - (fSlow317 + fVec9[1]));
			float fTemp75 = std::min<float>(1.0f, ((fTemp73 * float((fTemp73 > 0.0f))) + (fSlow318 * (fTemp74 * float((fTemp74 > 0.0f))))));
			fVec10[0] = fTemp75;
			fRec84[0] = (int(fTemp75) ? fSlow317 : fRec84[1]);
			float fTemp76 = (fRec84[0] + 100.0f);
			float fTemp77 = (fConst90 * ((340.0f / fTemp76) + -0.0399999991f));
			float fTemp78 = (fTemp77 + -1.49999499f);
			int iTemp79 = int(fTemp78);
			int iTemp80 = int(std::min<float>(fConst89, float(std::max<int>(0, iTemp79))));
			float fTemp81 = std::floor(fTemp78);
			float fTemp82 = (fTemp77 + (-1.0f - fTemp81));
			float fTemp83 = (0.0f - fTemp82);
			float fTemp84 = (fTemp77 + (-2.0f - fTemp81));
			float fTemp85 = (0.0f - (0.5f * fTemp84));
			float fTemp86 = (fTemp77 + (-3.0f - fTemp81));
			float fTemp87 = (0.0f - (0.333333343f * fTemp86));
			float fTemp88 = (fTemp77 + (-4.0f - fTemp81));
			float fTemp89 = (0.0f - (0.25f * fTemp88));
			float fTemp90 = (fTemp77 - fTemp81);
			int iTemp91 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp79 + 1)))));
			float fTemp92 = (0.0f - fTemp84);
			float fTemp93 = (0.0f - (0.5f * fTemp86));
			float fTemp94 = (0.0f - (0.333333343f * fTemp88));
			int iTemp95 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp79 + 2)))));
			float fTemp96 = (0.0f - fTemp86);
			float fTemp97 = (0.0f - (0.5f * fTemp88));
			float fTemp98 = (fTemp82 * fTemp84);
			int iTemp99 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp79 + 3)))));
			float fTemp100 = (0.0f - fTemp88);
			float fTemp101 = (fTemp98 * fTemp86);
			int iTemp102 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp79 + 4)))));
			fRec81[0] = (((((fVec7[((IOTA - iTemp80) & 2047)] * fTemp83) * fTemp85) * fTemp87) * fTemp89) + (fTemp90 * ((((((fVec7[((IOTA - iTemp91) & 2047)] * fTemp92) * fTemp93) * fTemp94) + (0.5f * (((fTemp82 * fVec7[((IOTA - iTemp95) & 2047)]) * fTemp96) * fTemp97))) + (0.166666672f * ((fTemp98 * fVec7[((IOTA - iTemp99) & 2047)]) * fTemp100))) + (0.0416666679f * (fTemp101 * fVec7[((IOTA - iTemp102) & 2047)])))));
			fRec85[0] = ((0.949999988f * fRec85[1]) + (0.0500000007f * fRec81[1]));
			fRec88[0] = (fTemp0 - (fConst96 * ((fConst99 * fRec88[2]) + (fConst100 * fRec88[1]))));
			float fTemp103 = (fConst96 * (((fConst98 * fRec88[0]) + (fConst101 * fRec88[1])) + (fConst98 * fRec88[2])));
			fRec87[0] = (fTemp103 - (fConst93 * ((fConst102 * fRec87[2]) + (fConst103 * fRec87[1]))));
			iRec89[0] = (((iRec89[1] + (iRec89[1] > 0)) * (fTemp75 <= fVec10[1])) + (fTemp75 > fVec10[1]));
			float fTemp104 = (fConst104 * float(iRec89[0]));
			float fTemp105 = (fConst93 * ((fRec87[2] + (fRec87[0] + (2.0f * fRec87[1]))) * std::max<float>(0.0f, std::min<float>(fTemp104, (2.0f - fTemp104)))));
			fRec86[0] = (fTemp105 - ((fConst106 * (fRec86[1] * std::cos((fConst5 * fTemp76)))) + (fConst107 * fRec86[2])));
			fRec90[0] = (fTemp105 - ((fConst111 * (fRec90[1] * std::cos((fConst112 * fTemp76)))) + (fConst113 * fRec90[2])));
			fRec91[0] = (fTemp105 - ((fConst116 * (fRec91[1] * std::cos((fConst117 * fTemp76)))) + (fConst118 * fRec91[2])));
			fRec92[0] = (fTemp105 - ((fConst121 * (fRec92[1] * std::cos((fConst122 * fTemp76)))) + (fConst123 * fRec92[2])));
			fRec93[0] = (fTemp105 - ((fConst126 * (fRec93[1] * std::cos((fConst127 * fTemp76)))) + (fConst128 * fRec93[2])));
			fRec94[0] = (fTemp105 - ((fConst131 * (fRec94[1] * std::cos((fConst132 * fTemp76)))) + (fConst133 * fRec94[2])));
			fRec95[0] = (fTemp105 - ((fConst136 * (fRec95[1] * std::cos((fConst137 * fTemp76)))) + (fConst138 * fRec95[2])));
			fRec96[0] = (fTemp105 - ((fConst141 * (fRec96[1] * std::cos((fConst142 * fTemp76)))) + (fConst143 * fRec96[2])));
			fRec97[0] = (fTemp105 - ((fConst146 * (fRec97[1] * std::cos((fConst147 * fTemp76)))) + (fConst148 * fRec97[2])));
			fRec98[0] = (fTemp105 - ((fConst151 * (fRec98[1] * std::cos((fConst152 * fTemp76)))) + (fConst153 * fRec98[2])));
			fRec99[0] = (fTemp105 - ((fConst156 * (fRec99[1] * std::cos((fConst157 * fTemp76)))) + (fConst158 * fRec99[2])));
			fRec100[0] = (fTemp105 - ((fConst161 * (fRec100[1] * std::cos((fConst162 * fTemp76)))) + (fConst163 * fRec100[2])));
			fRec101[0] = (fTemp105 - ((fConst166 * (fRec101[1] * std::cos((fConst167 * fTemp76)))) + (fConst168 * fRec101[2])));
			fRec102[0] = (fTemp105 - ((fConst171 * (fRec102[1] * std::cos((fConst172 * fTemp76)))) + (fConst173 * fRec102[2])));
			fRec103[0] = (fTemp105 - ((fConst176 * (fRec103[1] * std::cos((fConst177 * fTemp76)))) + (fConst178 * fRec103[2])));
			fRec104[0] = (fTemp105 - ((fConst181 * (fRec104[1] * std::cos((fConst182 * fTemp76)))) + (fConst183 * fRec104[2])));
			fRec105[0] = (fTemp105 - ((fConst186 * (fRec105[1] * std::cos((fConst187 * fTemp76)))) + (fConst188 * fRec105[2])));
			fRec106[0] = (fTemp105 - ((fConst191 * (fRec106[1] * std::cos((fConst192 * fTemp76)))) + (fConst193 * fRec106[2])));
			fRec107[0] = (fTemp105 - ((fConst196 * (fRec107[1] * std::cos((fConst197 * fTemp76)))) + (fConst198 * fRec107[2])));
			fRec108[0] = (fTemp105 - ((fConst201 * (fRec108[1] * std::cos((fConst202 * fTemp76)))) + (fConst203 * fRec108[2])));
			fRec109[0] = (fTemp105 - ((fConst206 * (fRec109[1] * std::cos((fConst207 * fTemp76)))) + (fConst208 * fRec109[2])));
			fRec110[0] = (fTemp105 - ((fConst211 * (fRec110[1] * std::cos((fConst212 * fTemp76)))) + (fConst213 * fRec110[2])));
			fRec111[0] = (fTemp105 - ((fConst216 * (fRec111[1] * std::cos((fConst217 * fTemp76)))) + (fConst218 * fRec111[2])));
			fRec112[0] = (fTemp105 - ((fConst221 * (fRec112[1] * std::cos((fConst222 * fTemp76)))) + (fConst223 * fRec112[2])));
			fRec113[0] = (fTemp105 - ((fConst226 * (fRec113[1] * std::cos((fConst227 * fTemp76)))) + (fConst228 * fRec113[2])));
			fRec114[0] = (fTemp105 - ((fConst231 * (fRec114[1] * std::cos((fConst232 * fTemp76)))) + (fConst233 * fRec114[2])));
			fRec115[0] = (fTemp105 - ((fConst236 * (fRec115[1] * std::cos((fConst237 * fTemp76)))) + (fConst238 * fRec115[2])));
			fRec116[0] = (fTemp105 - ((fConst241 * (fRec116[1] * std::cos((fConst242 * fTemp76)))) + (fConst243 * fRec116[2])));
			fRec117[0] = (fTemp105 - ((fConst246 * (fRec117[1] * std::cos((fConst247 * fTemp76)))) + (fConst248 * fRec117[2])));
			fRec118[0] = (fTemp105 - ((fConst251 * (fRec118[1] * std::cos((fConst252 * fTemp76)))) + (fConst253 * fRec118[2])));
			fRec119[0] = (fTemp105 - ((fConst256 * (fRec119[1] * std::cos((fConst257 * fTemp76)))) + (fConst258 * fRec119[2])));
			fRec120[0] = (fTemp105 - ((fConst261 * (fRec120[1] * std::cos((fConst262 * fTemp76)))) + (fConst263 * fRec120[2])));
			fRec121[0] = (fTemp105 - ((fConst266 * (fRec121[1] * std::cos((fConst267 * fTemp76)))) + (fConst268 * fRec121[2])));
			fRec122[0] = (fTemp105 - ((fConst271 * (fRec122[1] * std::cos((fConst272 * fTemp76)))) + (fConst273 * fRec122[2])));
			fRec123[0] = (fTemp105 - ((fConst276 * (fRec123[1] * std::cos((fConst277 * fTemp76)))) + (fConst278 * fRec123[2])));
			fRec124[0] = (fTemp105 - ((fConst281 * (fRec124[1] * std::cos((fConst282 * fTemp76)))) + (fConst283 * fRec124[2])));
			fRec125[0] = (fTemp105 - ((fConst286 * (fRec125[1] * std::cos((fConst287 * fTemp76)))) + (fConst288 * fRec125[2])));
			fRec126[0] = (fTemp105 - ((fConst291 * (fRec126[1] * std::cos((fConst292 * fTemp76)))) + (fConst293 * fRec126[2])));
			fRec127[0] = (fTemp105 - ((fConst296 * (fRec127[1] * std::cos((fConst297 * fTemp76)))) + (fConst298 * fRec127[2])));
			fRec128[0] = (fTemp105 - ((fConst301 * (fRec128[1] * std::cos((fConst302 * fTemp76)))) + (fConst303 * fRec128[2])));
			fRec129[0] = (fTemp105 - ((fConst306 * (fRec129[1] * std::cos((fConst307 * fTemp76)))) + (fConst308 * fRec129[2])));
			fRec130[0] = (fTemp105 - ((fConst311 * (fRec130[1] * std::cos((fConst312 * fTemp76)))) + (fConst313 * fRec130[2])));
			fRec131[0] = (fTemp105 - ((fConst316 * (fRec131[1] * std::cos((fConst317 * fTemp76)))) + (fConst318 * fRec131[2])));
			fRec132[0] = (fTemp105 - ((fConst321 * (fRec132[1] * std::cos((fConst322 * fTemp76)))) + (fConst323 * fRec132[2])));
			fRec133[0] = (fTemp105 - ((fConst326 * (fRec133[1] * std::cos((fConst327 * fTemp76)))) + (fConst328 * fRec133[2])));
			fRec134[0] = (fTemp105 - ((fConst331 * (fRec134[1] * std::cos((fConst332 * fTemp76)))) + (fConst333 * fRec134[2])));
			fRec135[0] = (fTemp105 - ((fConst336 * (fRec135[1] * std::cos((fConst337 * fTemp76)))) + (fConst338 * fRec135[2])));
			fRec136[0] = (fTemp105 - ((fConst341 * (fRec136[1] * std::cos((fConst342 * fTemp76)))) + (fConst343 * fRec136[2])));
			fRec137[0] = (fTemp105 - ((fConst346 * (fRec137[1] * std::cos((fConst347 * fTemp76)))) + (fConst348 * fRec137[2])));
			fRec138[0] = (fTemp105 - ((fConst351 * (fRec138[1] * std::cos((fConst352 * fTemp76)))) + (fConst353 * fRec138[2])));
			fVec11[0] = (((((((((((((((((((((((((((((((((((((((((((((((((((fRec86[0] - fRec86[2]) * ((fTemp76 < fConst108) ? fConst109 : 0.0f)) + ((fRec90[0] - fRec90[2]) * (((3.31356001f * fTemp76) < fConst108) ? fConst114 : 0.0f))) + ((fRec91[0] - fRec91[2]) * (((3.83469009f * fTemp76) < fConst108) ? fConst119 : 0.0f))) + ((fRec92[0] - fRec92[2]) * (((8.06313038f * fTemp76) < fConst108) ? fConst124 : 0.0f))) + ((fRec93[0] - fRec93[2]) * (((9.44777966f * fTemp76) < fConst108) ? fConst129 : 0.0f))) + ((fRec94[0] - fRec94[2]) * (((14.1169004f * fTemp76) < fConst108) ? fConst134 : 0.0f))) + ((fRec95[0] - fRec95[2]) * (((18.3840008f * fTemp76) < fConst108) ? fConst139 : 0.0f))) + ((fRec96[0] - fRec96[2]) * (((21.0102005f * fTemp76) < fConst108) ? fConst144 : 0.0f))) + ((fRec97[0] - fRec97[2]) * (((26.1774998f * fTemp76) < fConst108) ? fConst149 : 0.0f))) + ((fRec98[0] - fRec98[2]) * (((28.9944f * fTemp76) < fConst108) ? fConst154 : 0.0f))) + ((fRec99[0] - fRec99[2]) * (((37.0727997f * fTemp76) < fConst108) ? fConst159 : 0.0f))) + ((fRec100[0] - fRec100[2]) * (((37.8703003f * fTemp76) < fConst108) ? fConst164 : 0.0f))) + ((fRec101[0] - fRec101[2]) * (((40.0634003f * fTemp76) < fConst108) ? fConst169 : 0.0f))) + ((fRec102[0] - fRec102[2]) * (((47.6439018f * fTemp76) < fConst108) ? fConst174 : 0.0f))) + ((fRec103[0] - fRec103[2]) * (((51.019001f * fTemp76) < fConst108) ? fConst179 : 0.0f))) + ((fRec104[0] - fRec104[2]) * (((52.4300003f * fTemp76) < fConst108) ? fConst184 : 0.0f))) + ((fRec105[0] - fRec105[2]) * (((58.2859993f * fTemp76) < fConst108) ? fConst189 : 0.0f))) + ((fRec106[0] - fRec106[2]) * (((63.5485992f * fTemp76) < fConst108) ? fConst194 : 0.0f))) + ((fRec107[0] - fRec107[2]) * (((65.3628006f * fTemp76) < fConst108) ? fConst199 : 0.0f))) + ((fRec108[0] - fRec108[2]) * (((66.9587021f * fTemp76) < fConst108) ? fConst204 : 0.0f))) + ((fRec109[0] - fRec109[2]) * (((74.530098f * fTemp76) < fConst108) ? fConst209 : 0.0f))) + ((fRec110[0] - fRec110[2]) * (((78.6920013f * fTemp76) < fConst108) ? fConst214 : 0.0f))) + ((fRec111[0] - fRec111[2]) * (((80.8375015f * fTemp76) < fConst108) ? fConst219 : 0.0f))) + ((fRec112[0] - fRec112[2]) * (((89.9779968f * fTemp76) < fConst108) ? fConst224 : 0.0f))) + ((fRec113[0] - fRec113[2]) * (((92.9661026f * fTemp76) < fConst108) ? fConst229 : 0.0f))) + ((fRec114[0] - fRec114[2]) * (((95.1913986f * fTemp76) < fConst108) ? fConst234 : 0.0f))) + ((fRec115[0] - fRec115[2]) * (((97.4806976f * fTemp76) < fConst108) ? fConst239 : 0.0f))) + ((fRec116[0] - fRec116[2]) * (((110.620003f * fTemp76) < fConst108) ? fConst244 : 0.0f))) + ((fRec117[0] - fRec117[2]) * (((112.069f * fTemp76) < fConst108) ? fConst249 : 0.0f))) + ((fRec118[0] - fRec118[2]) * (((113.825996f * fTemp76) < fConst108) ? fConst254 : 0.0f))) + ((fRec119[0] - fRec119[2]) * (((119.356003f * fTemp76) < fConst108) ? fConst259 : 0.0f))) + ((fRec120[0] - fRec120[2]) * (((127.044998f * fTemp76) < fConst108) ? fConst264 : 0.0f))) + ((fRec121[0] - fRec121[2]) * (((129.981995f * fTemp76) < fConst108) ? fConst269 : 0.0f))) + ((fRec122[0] - fRec122[2]) * (((132.259003f * fTemp76) < fConst108) ? fConst274 : 0.0f))) + ((fRec123[0] - fRec123[2]) * (((133.477005f * fTemp76) < fConst108) ? fConst279 : 0.0f))) + ((fRec124[0] - fRec124[2]) * (((144.548996f * fTemp76) < fConst108) ? fConst284 : 0.0f))) + ((fRec125[0] - fRec125[2]) * (((149.438004f * fTemp76) < fConst108) ? fConst289 : 0.0f))) + ((fRec126[0] - fRec126[2]) * (((152.033005f * fTemp76) < fConst108) ? fConst294 : 0.0f))) + ((fRec127[0] - fRec127[2]) * (((153.166f * fTemp76) < fConst108) ? fConst299 : 0.0f))) + ((fRec128[0] - fRec128[2]) * (((155.597f * fTemp76) < fConst108) ? fConst304 : 0.0f))) + ((fRec129[0] - fRec129[2]) * (((158.182999f * fTemp76) < fConst108) ? fConst309 : 0.0f))) + ((fRec130[0] - fRec130[2]) * (((168.104996f * fTemp76) < fConst108) ? fConst314 : 0.0f))) + ((fRec131[0] - fRec131[2]) * (((171.863007f * fTemp76) < fConst108) ? fConst319 : 0.0f))) + ((fRec132[0] - fRec132[2]) * (((174.464005f * fTemp76) < fConst108) ? fConst324 : 0.0f))) + ((fRec133[0] - fRec133[2]) * (((178.936996f * fTemp76) < fConst108) ? fConst329 : 0.0f))) + ((fRec134[0] - fRec134[2]) * (((181.481995f * fTemp76) < fConst108) ? fConst334 : 0.0f))) + ((fRec135[0] - fRec135[2]) * (((185.397995f * fTemp76) < fConst108) ? fConst339 : 0.0f))) + ((fRec136[0] - fRec136[2]) * (((190.369003f * fTemp76) < fConst108) ? fConst344 : 0.0f))) + ((fRec137[0] - fRec137[2]) * (((192.190002f * fTemp76) < fConst108) ? fConst349 : 0.0f))) + ((fRec138[0] - fRec138[2]) * (((195.505005f * fTemp76) < fConst108) ? fConst354 : 0.0f)));
			float fTemp106 = ((0.99000001f * fRec85[0]) + (0.0199999996f * fVec11[1]));
			fVec12[(IOTA & 2047)] = fTemp106;
			fRec82[0] = (((((fTemp83 * fTemp85) * fTemp87) * fTemp89) * fVec12[((IOTA - iTemp80) & 2047)]) + (fTemp90 * ((((((fTemp92 * fTemp93) * fTemp94) * fVec12[((IOTA - iTemp91) & 2047)]) + (0.5f * (((fTemp82 * fTemp96) * fTemp97) * fVec12[((IOTA - iTemp95) & 2047)]))) + (0.166666672f * ((fTemp98 * fTemp100) * fVec12[((IOTA - iTemp99) & 2047)]))) + (0.0416666679f * (fTemp101 * fVec12[((IOTA - iTemp102) & 2047)])))));
			float fRec80 = fRec82[0];
			float fTemp107 = std::fabs((0.25f * fRec80));
			fRec141[0] = std::max<float>(fTemp107, ((fRec141[1] * fSlow326) + (fTemp107 * fSlow327)));
			fRec140[0] = ((fRec140[1] * fSlow323) + (fRec141[0] * fSlow328));
			fRec139[0] = ((fConst64 * fRec139[1]) + (fSlow320 * std::max<float>(((20.0f * std::log10(fRec140[0])) - fSlow329), 0.0f)));
			float fTemp108 = (0.25f * (fRec80 * std::pow(10.0f, (0.0500000007f * fRec139[0]))));
			fRec142[0] = (fTemp108 - (fSlow333 * ((fSlow334 * fRec142[2]) + (fSlow335 * fRec142[1]))));
			float fTemp109 = (fSlow316 * fRec78[1]);
			fRec78[0] = (fTemp108 - ((fSlow333 * (fRec142[2] + (fRec142[0] + (2.0f * fRec142[1])))) + (((fRec78[2] * fSlow343) + fTemp109) / fSlow344)));
			float fTemp110 = (fSlow313 * fRec77[1]);
			fRec77[0] = ((((fTemp109 + (fRec78[0] * fSlow346)) + (fRec78[2] * fSlow347)) / fSlow344) - (((fRec77[2] * fSlow355) + fTemp110) / fSlow356));
			fRec76[0] = ((((fTemp110 + (fRec77[0] * fSlow358)) + (fRec77[2] * fSlow359)) / fSlow356) - (fSlow360 * ((fSlow361 * fRec76[2]) + (fSlow362 * fRec76[1]))));
			float fTemp111 = (fRec76[2] + (fRec76[0] + (2.0f * fRec76[1])));
			fRec146[0] = ((0.999000013f * fRec146[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp42, ((fConst355 * fTemp43) + 1.0f))))));
			fRec148[0] = (iTemp44 ? fSlow374 : fRec148[1]);
			float fTemp112 = (fRec152[1] + (fSlow375 * fRec148[0]));
			fRec152[0] = (fTemp112 - std::floor(fTemp112));
			float fTemp113 = (fRec151[1] + (fConst66 * ((315.0f * (fRec146[0] * ftbl0mydspSIG0[int((65536.0f * fRec152[0]))])) + (fSlow376 * fRec148[0]))));
			fRec151[0] = (fTemp113 - std::floor(fTemp113));
			float fTemp114 = (fRec150[1] + (fConst66 * ((315.0f * (fRec146[0] * ftbl0mydspSIG0[int((65536.0f * fRec151[0]))])) + (fSlow377 * fRec148[0]))));
			fRec150[0] = (fTemp114 - std::floor(fTemp114));
			float fTemp115 = (fRec149[1] + (fConst66 * ((315.0f * (fRec146[0] * ftbl0mydspSIG0[int((65536.0f * fRec150[0]))])) + (fSlow378 * fRec148[0]))));
			fRec149[0] = (fTemp115 - std::floor(fTemp115));
			float fTemp116 = (fRec147[1] + (fConst66 * ((fSlow90 * fRec148[0]) + (315.0f * (fRec146[0] * ftbl0mydspSIG0[int((65536.0f * fRec149[0]))])))));
			fRec147[0] = (fTemp116 - std::floor(fTemp116));
			float fTemp117 = ftbl0mydspSIG0[int((65536.0f * fRec147[0]))];
			float fTemp118 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp117)));
			float fTemp119 = (fRec146[0] * (fTemp117 + (0.0500000007f * (fTemp118 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp118)))))));
			float fTemp120 = std::fabs((0.25f * fTemp119));
			fRec155[0] = std::max<float>(fTemp120, ((fRec155[1] * fSlow385) + (fTemp120 * fSlow386)));
			fRec154[0] = ((fRec154[1] * fSlow382) + (fRec155[0] * fSlow387));
			fRec153[0] = ((fConst64 * fRec153[1]) + (fSlow379 * std::max<float>(((20.0f * std::log10(fRec154[0])) - fSlow388), 0.0f)));
			float fTemp121 = (0.25f * (fTemp119 * std::pow(10.0f, (0.0500000007f * fRec153[0]))));
			fRec156[0] = (fTemp121 - (fSlow392 * ((fSlow393 * fRec156[2]) + (fSlow394 * fRec156[1]))));
			float fTemp122 = (fSlow373 * fRec145[1]);
			fRec145[0] = (fTemp121 - ((fSlow392 * (fRec156[2] + (fRec156[0] + (2.0f * fRec156[1])))) + (((fRec145[2] * fSlow402) + fTemp122) / fSlow403)));
			float fTemp123 = (fSlow370 * fRec144[1]);
			fRec144[0] = ((((fTemp122 + (fRec145[0] * fSlow405)) + (fRec145[2] * fSlow406)) / fSlow403) - (((fRec144[2] * fSlow414) + fTemp123) / fSlow415));
			fRec143[0] = ((((fTemp123 + (fRec144[0] * fSlow417)) + (fRec144[2] * fSlow418)) / fSlow415) - (fSlow419 * ((fSlow420 * fRec143[2]) + (fSlow421 * fRec143[1]))));
			float fTemp124 = (fSlow367 * (fRec143[2] + (fRec143[0] + (2.0f * fRec143[1]))));
			float fRec167 = (0.0f - (0.997305274f * ((0.899999976f * fRec168[2]) + (0.0500000007f * (fRec168[1] + fRec168[3])))));
			fVec13[0] = fSlow434;
			float fTemp125 = (fSlow434 - fVec13[1]);
			fVec14[0] = fSlow436;
			float fTemp126 = (0.0f - (fSlow434 + fVec14[1]));
			float fTemp127 = std::min<float>(1.0f, ((fTemp125 * float((fTemp125 > 0.0f))) + (fSlow435 * (fTemp126 * float((fTemp126 > 0.0f))))));
			fVec15[0] = fTemp127;
			int iTemp128 = int(fTemp127);
			fRec186[0] = (iTemp128 ? fSlow437 : fRec186[1]);
			float fTemp129 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow433 * (fRec40[0] * fRec186[0]))))) + -0.109999999f);
			float fTemp130 = (fConst357 * fTemp129);
			float fTemp131 = (fTemp130 + -1.49999499f);
			int iTemp132 = int(fTemp131);
			int iTemp133 = int(std::min<float>(fConst89, float(std::max<int>(0, iTemp132))));
			float fTemp134 = std::floor(fTemp131);
			float fTemp135 = (fTemp130 + (-1.0f - fTemp134));
			float fTemp136 = (0.0f - fTemp135);
			float fTemp137 = (fTemp130 + (-2.0f - fTemp134));
			float fTemp138 = (0.0f - (0.5f * fTemp137));
			float fTemp139 = (fTemp130 + (-3.0f - fTemp134));
			float fTemp140 = (0.0f - (0.333333343f * fTemp139));
			float fTemp141 = (fTemp130 + (-4.0f - fTemp134));
			float fTemp142 = (0.0f - (0.25f * fTemp141));
			float fTemp143 = (fTemp130 - fTemp134);
			int iTemp144 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp132 + 1)))));
			float fTemp145 = (0.0f - fTemp137);
			float fTemp146 = (0.0f - (0.5f * fTemp139));
			float fTemp147 = (0.0f - (0.333333343f * fTemp141));
			int iTemp148 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp132 + 2)))));
			float fTemp149 = (0.0f - fTemp139);
			float fTemp150 = (0.0f - (0.5f * fTemp141));
			float fTemp151 = (fTemp135 * fTemp137);
			int iTemp152 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp132 + 3)))));
			float fTemp153 = (0.0f - fTemp141);
			float fTemp154 = (fTemp151 * fTemp139);
			int iTemp155 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp132 + 4)))));
			fRec182[0] = (((((fRec163[((IOTA - (iTemp133 + 1)) & 2047)] * fTemp136) * fTemp138) * fTemp140) * fTemp142) + (fTemp143 * ((((((fRec163[((IOTA - (iTemp144 + 1)) & 2047)] * fTemp145) * fTemp146) * fTemp147) + (0.5f * (((fTemp135 * fRec163[((IOTA - (iTemp148 + 1)) & 2047)]) * fTemp149) * fTemp150))) + (0.166666672f * ((fTemp151 * fRec163[((IOTA - (iTemp152 + 1)) & 2047)]) * fTemp153))) + (0.0416666679f * (fTemp154 * fRec163[((IOTA - (iTemp155 + 1)) & 2047)])))));
			fRec187[0] = ((fSlow438 * fRec187[1]) + (fSlow439 * fRec182[1]));
			float fRec183 = fRec187[0];
			fRec189[0] = fRec161[1];
			fRec190[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec189[2]) + (0.0500000007f * (fRec189[1] + fRec189[3])))));
			float fTemp156 = (fConst358 * fTemp129);
			float fTemp157 = (fTemp156 + -1.49999499f);
			int iTemp158 = int(fTemp157);
			int iTemp159 = int(std::min<float>(fConst89, float(std::max<int>(0, iTemp158))));
			float fTemp160 = std::floor(fTemp157);
			float fTemp161 = (fTemp156 + (-1.0f - fTemp160));
			float fTemp162 = (0.0f - fTemp161);
			float fTemp163 = (fTemp156 + (-2.0f - fTemp160));
			float fTemp164 = (0.0f - (0.5f * fTemp163));
			float fTemp165 = (fTemp156 + (-3.0f - fTemp160));
			float fTemp166 = (0.0f - (0.333333343f * fTemp165));
			float fTemp167 = (fTemp156 + (-4.0f - fTemp160));
			float fTemp168 = (0.0f - (0.25f * fTemp167));
			float fTemp169 = (fTemp156 - fTemp160);
			int iTemp170 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp158 + 1)))));
			float fTemp171 = (0.0f - fTemp163);
			float fTemp172 = (0.0f - (0.5f * fTemp165));
			float fTemp173 = (0.0f - (0.333333343f * fTemp167));
			int iTemp174 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp158 + 2)))));
			float fTemp175 = (0.0f - fTemp165);
			float fTemp176 = (0.0f - (0.5f * fTemp167));
			float fTemp177 = (fTemp161 * fTemp163);
			int iTemp178 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp158 + 3)))));
			float fTemp179 = (0.0f - fTemp167);
			float fTemp180 = (fTemp177 * fTemp165);
			int iTemp181 = int(std::min<float>(fConst89, float(std::max<int>(0, (iTemp158 + 4)))));
			fVec16[0] = (((((fRec190[((IOTA - (iTemp159 + 2)) & 1023)] * fTemp162) * fTemp164) * fTemp166) * fTemp168) + (fTemp169 * ((((((fRec190[((IOTA - (iTemp170 + 2)) & 1023)] * fTemp171) * fTemp172) * fTemp173) + (0.5f * (((fTemp161 * fRec190[((IOTA - (iTemp174 + 2)) & 1023)]) * fTemp175) * fTemp176))) + (0.166666672f * ((fTemp177 * fRec190[((IOTA - (iTemp178 + 2)) & 1023)]) * fTemp179))) + (0.0416666679f * (fTemp180 * fRec190[((IOTA - (iTemp181 + 2)) & 1023)])))));
			float fTemp182 = (fTemp127 + fVec16[1]);
			fVec17[0] = fTemp182;
			fRec188[(IOTA & 2047)] = ((fSlow438 * fRec188[((IOTA - 1) & 2047)]) + (fSlow439 * fVec17[2]));
			float fRec184 = (((((fTemp136 * fTemp138) * fTemp140) * fTemp142) * fRec188[((IOTA - iTemp133) & 2047)]) + (fTemp143 * ((((((fTemp145 * fTemp146) * fTemp147) * fRec188[((IOTA - iTemp144) & 2047)]) + (0.5f * (((fTemp135 * fTemp149) * fTemp150) * fRec188[((IOTA - iTemp148) & 2047)]))) + (0.166666672f * ((fTemp151 * fTemp153) * fRec188[((IOTA - iTemp152) & 2047)]))) + (0.0416666679f * (fTemp154 * fRec188[((IOTA - iTemp155) & 2047)])))));
			float fRec185 = (fVec17[1] + fRec178[1]);
			fRec178[0] = fRec183;
			float fRec179 = fRec178[1];
			float fRec180 = fRec184;
			float fRec181 = fRec185;
			fRec174[0] = fRec179;
			float fRec175 = (fTemp127 + fRec174[1]);
			float fRec176 = fRec180;
			float fRec177 = fRec181;
			fRec170[(IOTA & 1023)] = fRec175;
			float fRec171 = (((((fTemp162 * fTemp164) * fTemp166) * fTemp168) * fRec170[((IOTA - (iTemp159 + 1)) & 1023)]) + (fTemp169 * ((((((fTemp171 * fTemp172) * fTemp173) * fRec170[((IOTA - (iTemp170 + 1)) & 1023)]) + (0.5f * (((fTemp161 * fTemp175) * fTemp176) * fRec170[((IOTA - (iTemp174 + 1)) & 1023)]))) + (0.166666672f * ((fTemp177 * fTemp179) * fRec170[((IOTA - (iTemp178 + 1)) & 1023)]))) + (0.0416666679f * (fTemp180 * fRec170[((IOTA - (iTemp181 + 1)) & 1023)])))));
			float fRec172 = fRec176;
			float fRec173 = fRec177;
			fRec168[0] = fRec172;
			float fRec169 = fRec173;
			fRec163[(IOTA & 2047)] = fRec167;
			float fRec164 = fRec171;
			float fRec165 = fRec168[0];
			float fRec166 = fRec169;
			fRec161[0] = fRec164;
			float fRec162 = fRec166;
			fRec191[0] = (fSlow440 + (0.999000013f * fRec191[1]));
			float fTemp183 = std::min<float>(15000.0f, std::max<float>(500.0f, fRec191[0]));
			float fTemp184 = std::tan((fConst1 * fTemp183));
			float fTemp185 = (1.0f / fTemp184);
			float fTemp186 = (((fTemp185 + 1.42857146f) / fTemp184) + 1.0f);
			fRec160[0] = ((1.5f * fRec162) - (((fRec160[2] * (((fTemp185 + -1.42857146f) / fTemp184) + 1.0f)) + (2.0f * (fRec160[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp184)))))) / fTemp186));
			iRec192[0] = (((iRec192[1] + (iRec192[1] > 0)) * (fTemp127 <= fVec15[1])) + (fTemp127 > fVec15[1]));
			float fTemp187 = float(iRec192[0]);
			iRec193[0] = (iTemp128 ? 7 : iRec193[1]);
			float fTemp188 = ((((fRec160[2] + (fRec160[0] + (2.0f * fRec160[1]))) * ((6.66666674e-05f * (fTemp183 + -500.0f)) + 1.0f)) * std::max<float>(0.0f, std::min<float>((fConst82 * fTemp187), ((fConst83 * (fConst81 - fTemp187)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec193[0] + -10)))));
			float fTemp189 = std::fabs((0.25f * (fTemp188 / fTemp186)));
			fRec196[0] = std::max<float>(fTemp189, ((fRec196[1] * fSlow447) + (fTemp189 * fSlow448)));
			fRec195[0] = ((fRec195[1] * fSlow444) + (fRec196[0] * fSlow449));
			fRec194[0] = ((fConst64 * fRec194[1]) + (fSlow441 * std::max<float>(((20.0f * std::log10(fRec195[0])) - fSlow450), 0.0f)));
			float fTemp190 = (0.25f * ((fTemp188 * std::pow(10.0f, (0.0500000007f * fRec194[0]))) / fTemp186));
			fRec197[0] = (fTemp190 - (fSlow454 * ((fSlow455 * fRec197[2]) + (fSlow456 * fRec197[1]))));
			float fTemp191 = (fSlow432 * fRec159[1]);
			fRec159[0] = (fTemp190 - ((fSlow454 * (fRec197[2] + (fRec197[0] + (2.0f * fRec197[1])))) + (((fRec159[2] * fSlow464) + fTemp191) / fSlow465)));
			float fTemp192 = (fSlow429 * fRec158[1]);
			fRec158[0] = ((((fTemp191 + (fRec159[0] * fSlow467)) + (fRec159[2] * fSlow468)) / fSlow465) - (((fRec158[2] * fSlow476) + fTemp192) / fSlow477));
			fRec157[0] = ((((fTemp192 + (fRec158[0] * fSlow479)) + (fRec158[2] * fSlow480)) / fSlow477) - (fSlow481 * ((fSlow482 * fRec157[2]) + (fSlow483 * fRec157[1]))));
			float fTemp193 = (fSlow426 * (fRec157[2] + (fRec157[0] + (2.0f * fRec157[1]))));
			fRec202[0] = (fTemp103 - (fConst362 * ((fConst363 * fRec202[2]) + (fConst364 * fRec202[1]))));
			fVec18[0] = fSlow495;
			float fTemp194 = (fSlow495 - fVec18[1]);
			fVec19[0] = fSlow497;
			float fTemp195 = (0.0f - (fSlow495 + fVec19[1]));
			float fTemp196 = std::min<float>(1.0f, ((fTemp194 * float((fTemp194 > 0.0f))) + (fSlow496 * (fTemp195 * float((fTemp195 > 0.0f))))));
			fVec20[0] = fTemp196;
			iRec203[0] = (((iRec203[1] + (iRec203[1] > 0)) * (fTemp196 <= fVec20[1])) + (fTemp196 > fVec20[1]));
			float fTemp197 = float(iRec203[0]);
			float fTemp198 = (fConst365 * fTemp197);
			float fTemp199 = (fConst362 * ((fRec202[2] + (fRec202[0] + (2.0f * fRec202[1]))) * std::max<float>(0.0f, std::min<float>(fTemp198, (2.0f - fTemp198)))));
			fRec201[0] = (fTemp199 - ((fConst367 * fRec201[1]) + (fConst368 * fRec201[2])));
			fRec204[0] = (fTemp199 - ((fConst371 * fRec204[1]) + (fConst372 * fRec204[2])));
			fRec205[0] = (fTemp199 - ((fConst375 * fRec205[1]) + (fConst376 * fRec205[2])));
			fRec206[0] = (fTemp199 - ((fConst379 * fRec206[1]) + (fConst380 * fRec206[2])));
			fRec207[0] = (fTemp199 - ((fConst383 * fRec207[1]) + (fConst384 * fRec207[2])));
			fRec208[0] = (fTemp199 - ((fConst387 * fRec208[1]) + (fConst388 * fRec208[2])));
			fRec209[0] = (fTemp199 - ((fConst391 * fRec209[1]) + (fConst392 * fRec209[2])));
			fRec210[0] = (fTemp199 - ((fConst395 * fRec210[1]) + (fConst396 * fRec210[2])));
			fRec211[0] = (fTemp199 - ((fConst399 * fRec211[1]) + (fConst400 * fRec211[2])));
			fRec212[0] = (fTemp199 - ((fConst403 * fRec212[1]) + (fConst404 * fRec212[2])));
			fRec213[0] = (fTemp199 - ((fConst407 * fRec213[1]) + (fConst408 * fRec213[2])));
			fRec214[0] = (fTemp199 - ((fConst411 * fRec214[1]) + (fConst412 * fRec214[2])));
			fRec215[0] = (fTemp199 - ((fConst415 * fRec215[1]) + (fConst416 * fRec215[2])));
			fRec216[0] = (fTemp199 - ((fConst419 * fRec216[1]) + (fConst420 * fRec216[2])));
			fRec217[0] = (fTemp199 - ((fConst423 * fRec217[1]) + (fConst424 * fRec217[2])));
			fRec218[0] = (fTemp199 - ((fConst427 * fRec218[1]) + (fConst428 * fRec218[2])));
			fRec219[0] = (fTemp199 - ((fConst431 * fRec219[1]) + (fConst432 * fRec219[2])));
			fRec220[0] = (fTemp199 - ((fConst435 * fRec220[1]) + (fConst436 * fRec220[2])));
			fRec221[0] = (fTemp199 - ((fConst439 * fRec221[1]) + (fConst440 * fRec221[2])));
			fRec222[0] = (fTemp199 - ((fConst443 * fRec222[1]) + (fConst444 * fRec222[2])));
			fRec223[0] = (fTemp199 - ((fConst447 * fRec223[1]) + (fConst448 * fRec223[2])));
			fRec224[0] = (fTemp199 - ((fConst451 * fRec224[1]) + (fConst452 * fRec224[2])));
			fRec225[0] = (fTemp199 - ((fConst455 * fRec225[1]) + (fConst456 * fRec225[2])));
			fRec226[0] = (fTemp199 - ((fConst459 * fRec226[1]) + (fConst460 * fRec226[2])));
			fRec227[0] = (fTemp199 - ((fConst463 * fRec227[1]) + (fConst464 * fRec227[2])));
			fRec228[0] = (fTemp199 - ((fConst467 * fRec228[1]) + (fConst468 * fRec228[2])));
			fRec229[0] = (fTemp199 - ((fConst471 * fRec229[1]) + (fConst472 * fRec229[2])));
			fRec230[0] = (fTemp199 - ((fConst475 * fRec230[1]) + (fConst476 * fRec230[2])));
			fRec231[0] = (fTemp199 - ((fConst479 * fRec231[1]) + (fConst480 * fRec231[2])));
			fRec232[0] = (fTemp199 - ((fConst483 * fRec232[1]) + (fConst484 * fRec232[2])));
			fRec233[0] = (fTemp199 - ((fConst487 * fRec233[1]) + (fConst488 * fRec233[2])));
			fRec234[0] = (fTemp199 - ((fConst491 * fRec234[1]) + (fConst492 * fRec234[2])));
			fRec235[0] = (fTemp199 - ((fConst495 * fRec235[1]) + (fConst496 * fRec235[2])));
			fRec236[0] = (fTemp199 - ((fConst499 * fRec236[1]) + (fConst500 * fRec236[2])));
			fRec237[0] = (fTemp199 - ((fConst503 * fRec237[1]) + (fConst504 * fRec237[2])));
			fRec238[0] = (fTemp199 - ((fConst507 * fRec238[1]) + (fConst508 * fRec238[2])));
			fRec239[0] = (fTemp199 - ((fConst511 * fRec239[1]) + (fConst512 * fRec239[2])));
			fRec240[0] = (fTemp199 - ((fConst515 * fRec240[1]) + (fConst516 * fRec240[2])));
			fRec241[0] = (fTemp199 - ((fConst519 * fRec241[1]) + (fConst520 * fRec241[2])));
			fRec242[0] = (fTemp199 - ((fConst523 * fRec242[1]) + (fConst524 * fRec242[2])));
			fRec243[0] = (fTemp199 - ((fConst527 * fRec243[1]) + (fConst528 * fRec243[2])));
			fRec244[0] = (fTemp199 - ((fConst531 * fRec244[1]) + (fConst532 * fRec244[2])));
			fRec245[0] = (fTemp199 - ((fConst535 * fRec245[1]) + (fConst536 * fRec245[2])));
			fRec246[0] = (fTemp199 - ((fConst539 * fRec246[1]) + (fConst540 * fRec246[2])));
			fRec247[0] = (fTemp199 - ((fConst543 * fRec247[1]) + (fConst544 * fRec247[2])));
			fRec248[0] = (fTemp199 - ((fConst547 * fRec248[1]) + (fConst548 * fRec248[2])));
			fRec249[0] = (fTemp199 - ((fConst551 * fRec249[1]) + (fConst552 * fRec249[2])));
			fRec250[0] = (fTemp199 - ((fConst555 * fRec250[1]) + (fConst556 * fRec250[2])));
			fRec251[0] = (fTemp199 - ((fConst559 * fRec251[1]) + (fConst560 * fRec251[2])));
			fRec252[0] = (fTemp199 - ((fConst563 * fRec252[1]) + (fConst564 * fRec252[2])));
			fRec253[0] = (fSlow499 + (0.999000013f * fRec253[1]));
			fRec254[0] = (fSlow500 + (fRec254[1] - std::floor((fSlow500 + fRec254[1]))));
			fRec255[0] = (fSlow502 + (0.999000013f * fRec255[1]));
			fRec256[0] = (fSlow503 + (fRec256[1] - std::floor((fSlow503 + fRec256[1]))));
			fRec257[0] = (fSlow505 + (0.999000013f * fRec257[1]));
			fRec258[0] = (fSlow506 + (fRec258[1] - std::floor((fSlow506 + fRec258[1]))));
			float fTemp200 = ((0.0199999996f * (((((((((((((((((((((((((((((((((((((((((((((((((((fConst359 * (fRec201[0] - fRec201[2])) + (fConst369 * (fRec204[0] - fRec204[2]))) + (fConst373 * (fRec205[0] - fRec205[2]))) + (fConst377 * (fRec206[0] - fRec206[2]))) + (fConst381 * (fRec207[0] - fRec207[2]))) + (fConst385 * (fRec208[0] - fRec208[2]))) + (fConst389 * (fRec209[0] - fRec209[2]))) + (fConst393 * (fRec210[0] - fRec210[2]))) + (fConst397 * (fRec211[0] - fRec211[2]))) + (fConst401 * (fRec212[0] - fRec212[2]))) + (fConst405 * (fRec213[0] - fRec213[2]))) + (fConst409 * (fRec214[0] - fRec214[2]))) + (fConst413 * (fRec215[0] - fRec215[2]))) + (fConst417 * (fRec216[0] - fRec216[2]))) + (fConst421 * (fRec217[0] - fRec217[2]))) + (fConst425 * (fRec218[0] - fRec218[2]))) + (fConst429 * (fRec219[0] - fRec219[2]))) + (fConst433 * (fRec220[0] - fRec220[2]))) + (fConst437 * (fRec221[0] - fRec221[2]))) + (fConst441 * (fRec222[0] - fRec222[2]))) + (fConst445 * (fRec223[0] - fRec223[2]))) + (fConst449 * (fRec224[0] - fRec224[2]))) + (fConst453 * (fRec225[0] - fRec225[2]))) + (fConst457 * (fRec226[0] - fRec226[2]))) + (fConst461 * (fRec227[0] - fRec227[2]))) + (fConst465 * (fRec228[0] - fRec228[2]))) + (fConst469 * (fRec229[0] - fRec229[2]))) + (fConst473 * (fRec230[0] - fRec230[2]))) + (fConst477 * (fRec231[0] - fRec231[2]))) + (fConst481 * (fRec232[0] - fRec232[2]))) + (fConst485 * (fRec233[0] - fRec233[2]))) + (fConst489 * (fRec234[0] - fRec234[2]))) + (fConst493 * (fRec235[0] - fRec235[2]))) + (fConst497 * (fRec236[0] - fRec236[2]))) + (fConst501 * (fRec237[0] - fRec237[2]))) + (fConst505 * (fRec238[0] - fRec238[2]))) + (fConst509 * (fRec239[0] - fRec239[2]))) + (fConst513 * (fRec240[0] - fRec240[2]))) + (fConst517 * (fRec241[0] - fRec241[2]))) + (fConst521 * (fRec242[0] - fRec242[2]))) + (fConst525 * (fRec243[0] - fRec243[2]))) + (fConst529 * (fRec244[0] - fRec244[2]))) + (fConst533 * (fRec245[0] - fRec245[2]))) + (fConst537 * (fRec246[0] - fRec246[2]))) + (fConst541 * (fRec247[0] - fRec247[2]))) + (fConst545 * (fRec248[0] - fRec248[2]))) + (fConst549 * (fRec249[0] - fRec249[2]))) + (fConst553 * (fRec250[0] - fRec250[2]))) + (fConst557 * (fRec251[0] - fRec251[2]))) + (fConst561 * (fRec252[0] - fRec252[2]))) * std::max<float>(0.0f, std::min<float>((fConst82 * fTemp197), ((fConst566 * (fConst81 - fTemp197)) + 1.0f))))) + (2.0f * (((fRec253[0] * ftbl0mydspSIG0[int((65536.0f * fRec254[0]))]) + (fRec255[0] * ftbl0mydspSIG0[int((65536.0f * fRec256[0]))])) + (fRec257[0] * ftbl0mydspSIG0[int((65536.0f * fRec258[0]))]))));
			float fTemp201 = std::fabs((0.25f * fTemp200));
			fRec261[0] = std::max<float>(fTemp201, ((fRec261[1] * fSlow513) + (fTemp201 * fSlow514)));
			fRec260[0] = ((fRec260[1] * fSlow510) + (fRec261[0] * fSlow515));
			fRec259[0] = ((fConst64 * fRec259[1]) + (fSlow507 * std::max<float>(((20.0f * std::log10(fRec260[0])) - fSlow516), 0.0f)));
			float fTemp202 = (0.25f * (fTemp200 * std::pow(10.0f, (0.0500000007f * fRec259[0]))));
			fRec262[0] = (fTemp202 - (fSlow520 * ((fSlow521 * fRec262[2]) + (fSlow522 * fRec262[1]))));
			float fTemp203 = (fSlow494 * fRec200[1]);
			fRec200[0] = (fTemp202 - ((fSlow520 * (fRec262[2] + (fRec262[0] + (2.0f * fRec262[1])))) + (((fRec200[2] * fSlow530) + fTemp203) / fSlow531)));
			float fTemp204 = (fSlow491 * fRec199[1]);
			fRec199[0] = ((((fTemp203 + (fRec200[0] * fSlow533)) + (fRec200[2] * fSlow534)) / fSlow531) - (((fRec199[2] * fSlow542) + fTemp204) / fSlow543));
			fRec198[0] = ((((fTemp204 + (fRec199[0] * fSlow545)) + (fRec199[2] * fSlow546)) / fSlow543) - (fSlow547 * ((fSlow548 * fRec198[2]) + (fSlow549 * fRec198[1]))));
			float fTemp205 = (fSlow488 * (fRec198[2] + (fRec198[0] + (2.0f * fRec198[1]))));
			fRec275[0] = (0.0f - (fConst581 * ((fConst582 * fRec275[1]) - (fRec271[1] + fRec271[2]))));
			fRec274[0] = ((fConst577 * fRec274[1]) + (fConst578 * (fRec271[1] + (fConst579 * fRec275[0]))));
			fVec21[(IOTA & 32767)] = ((0.353553385f * fRec274[0]) + 9.99999968e-21f);
			float fTemp206 = (fRec60[0] + 0.5f);
			iRec280[0] = ((iVec0[1] + iRec280[1]) % int((fConst0 / fTemp206)));
			float fTemp207 = float(iRec280[0]);
			float fTemp208 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fTemp206) * (1.0f - (0.000500000024f * std::min<float>(fTemp206, 1000.0f)))))))));
			float fTemp209 = (1.0f / fTemp208);
			float fTemp210 = (((fTemp209 + 1.41421354f) / fTemp208) + 1.0f);
			fRec279[0] = ((2.0f * (((float((fTemp207 < (fConst84 / fTemp206))) + -0.5f) + (float((fTemp207 < (fConst85 / fTemp206))) + -0.5f)) + (float((fTemp207 < (fConst86 / fTemp206))) + -0.5f))) - (((fRec279[2] * (((fTemp209 + -1.41421354f) / fTemp208) + 1.0f)) + (2.0f * (fRec279[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp208)))))) / fTemp210));
			float fTemp211 = (fRec65[0] + 0.5f);
			iRec282[0] = ((iVec0[1] + iRec282[1]) % int((fConst0 / fTemp211)));
			float fTemp212 = float(iRec282[0]);
			float fTemp213 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fTemp211) * (1.0f - (0.000500000024f * std::min<float>(fTemp211, 1000.0f)))))))));
			float fTemp214 = (1.0f / fTemp213);
			float fTemp215 = (((fTemp214 + 1.41421354f) / fTemp213) + 1.0f);
			fRec281[0] = ((2.0f * (((float((fTemp212 < (fConst84 / fTemp211))) + -0.5f) + (float((fTemp212 < (fConst85 / fTemp211))) + -0.5f)) + (float((fTemp212 < (fConst86 / fTemp211))) + -0.5f))) - (((fRec281[2] * (((fTemp214 + -1.41421354f) / fTemp213) + 1.0f)) + (2.0f * (fRec281[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp213)))))) / fTemp215));
			float fTemp216 = (fRec68[0] + 0.5f);
			iRec284[0] = ((iVec0[1] + iRec284[1]) % int((fConst0 / fTemp216)));
			float fTemp217 = float(iRec284[0]);
			float fTemp218 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fTemp216) * (1.0f - (0.000500000024f * std::min<float>(fTemp216, 1000.0f)))))))));
			float fTemp219 = (1.0f / fTemp218);
			float fTemp220 = (((fTemp219 + 1.41421354f) / fTemp218) + 1.0f);
			fRec283[0] = ((2.0f * (((float((fTemp217 < (fConst84 / fTemp216))) + -0.5f) + (float((fTemp217 < (fConst85 / fTemp216))) + -0.5f)) + (float((fTemp217 < (fConst86 / fTemp216))) + -0.5f))) - (((fRec283[2] * (((fTemp219 + -1.41421354f) / fTemp218) + 1.0f)) + (2.0f * (fRec283[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp218)))))) / fTemp220));
			float fTemp221 = (fRec71[0] + 0.5f);
			iRec286[0] = ((iVec0[1] + iRec286[1]) % int((fConst0 / fTemp221)));
			float fTemp222 = float(iRec286[0]);
			float fTemp223 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (11.666667f * ((fTemp50 * fTemp221) * (1.0f - (0.000500000024f * std::min<float>(fTemp221, 1000.0f)))))))));
			float fTemp224 = (1.0f / fTemp223);
			float fTemp225 = (((fTemp224 + 1.41421354f) / fTemp223) + 1.0f);
			fRec285[0] = ((2.0f * (((float((fTemp222 < (fConst84 / fTemp221))) + -0.5f) + (float((fTemp222 < (fConst85 / fTemp221))) + -0.5f)) + (float((fTemp222 < (fConst86 / fTemp221))) + -0.5f))) - (((fRec285[2] * (((fTemp224 + -1.41421354f) / fTemp223) + 1.0f)) + (2.0f * (fRec285[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp223)))))) / fTemp225));
			float fTemp226 = (fRec56[0] * (((((fRec279[2] + (fRec279[0] + (2.0f * fRec279[1]))) / fTemp210) + ((fRec281[2] + (fRec281[0] + (2.0f * fRec281[1]))) / fTemp215)) + ((fRec283[2] + (fRec283[0] + (2.0f * fRec283[1]))) / fTemp220)) + ((fRec285[2] + (fRec285[0] + (2.0f * fRec285[1]))) / fTemp225)));
			float fTemp227 = std::fabs((0.25f * fTemp226));
			fRec289[0] = std::max<float>(fTemp227, ((fSlow268 * fRec289[1]) + (fSlow269 * fTemp227)));
			fRec288[0] = ((fSlow265 * fRec288[1]) + (fRec289[0] * fSlow270));
			fRec287[0] = ((fConst64 * fRec287[1]) + (fSlow262 * std::max<float>(((20.0f * std::log10(fRec288[0])) - fSlow271), 0.0f)));
			float fTemp228 = (0.25f * (fTemp226 * std::pow(10.0f, (0.0500000007f * fRec287[0]))));
			fRec290[0] = (fTemp228 - (fSlow275 * ((fSlow276 * fRec290[2]) + (fSlow277 * fRec290[1]))));
			float fTemp229 = (fSlow253 * fRec278[1]);
			fRec278[0] = (fTemp228 - ((fSlow275 * (fRec290[2] + (fRec290[0] + (2.0f * fRec290[1])))) + (((fSlow285 * fRec278[2]) + fTemp229) / fSlow286)));
			float fTemp230 = (fSlow250 * fRec277[1]);
			fRec277[0] = ((((fTemp229 + (fRec278[0] * fSlow288)) + (fSlow289 * fRec278[2])) / fSlow286) - (((fSlow297 * fRec277[2]) + fTemp230) / fSlow298));
			fRec276[0] = ((((fTemp230 + (fRec277[0] * fSlow300)) + (fSlow301 * fRec277[2])) / fSlow298) - (fSlow302 * ((fSlow303 * fRec276[2]) + (fSlow304 * fRec276[1]))));
			float fTemp231 = (fSlow247 * (fRec276[2] + (fRec276[0] + (2.0f * fRec276[1]))));
			fVec22[(IOTA & 2047)] = (fTemp231 + (fTemp36 + fTemp193));
			float fTemp232 = (0.300000012f * fVec22[((IOTA - iConst585) & 2047)]);
			float fTemp233 = (((0.600000024f * fRec272[1]) + fVec21[((IOTA - iConst584) & 32767)]) - fTemp232);
			fVec23[(IOTA & 2047)] = fTemp233;
			fRec272[0] = fVec23[((IOTA - iConst586) & 2047)];
			float fRec273 = (0.0f - (0.600000024f * fTemp233));
			fRec294[0] = (0.0f - (fConst581 * ((fConst582 * fRec294[1]) - (fRec267[1] + fRec267[2]))));
			fRec293[0] = ((fConst595 * fRec293[1]) + (fConst596 * (fRec267[1] + (fConst597 * fRec294[0]))));
			fVec24[(IOTA & 32767)] = ((0.353553385f * fRec293[0]) + 9.99999968e-21f);
			float fTemp234 = (((0.600000024f * fRec291[1]) + fVec24[((IOTA - iConst599) & 32767)]) - fTemp232);
			fVec25[(IOTA & 4095)] = fTemp234;
			fRec291[0] = fVec25[((IOTA - iConst600) & 4095)];
			float fRec292 = (0.0f - (0.600000024f * fTemp234));
			fRec298[0] = (0.0f - (fConst581 * ((fConst582 * fRec298[1]) - (fRec269[1] + fRec269[2]))));
			fRec297[0] = ((fConst609 * fRec297[1]) + (fConst610 * (fRec269[1] + (fConst611 * fRec298[0]))));
			fVec26[(IOTA & 16383)] = ((0.353553385f * fRec297[0]) + 9.99999968e-21f);
			float fTemp235 = (fVec26[((IOTA - iConst613) & 16383)] + (fTemp232 + (0.600000024f * fRec295[1])));
			fVec27[(IOTA & 4095)] = fTemp235;
			fRec295[0] = fVec27[((IOTA - iConst614) & 4095)];
			float fRec296 = (0.0f - (0.600000024f * fTemp235));
			fRec302[0] = (0.0f - (fConst581 * ((fConst582 * fRec302[1]) - (fRec265[1] + fRec265[2]))));
			fRec301[0] = ((fConst623 * fRec301[1]) + (fConst624 * (fRec265[1] + (fConst625 * fRec302[0]))));
			fVec28[(IOTA & 32767)] = ((0.353553385f * fRec301[0]) + 9.99999968e-21f);
			float fTemp236 = (fTemp232 + ((0.600000024f * fRec299[1]) + fVec28[((IOTA - iConst627) & 32767)]));
			fVec29[(IOTA & 4095)] = fTemp236;
			fRec299[0] = fVec29[((IOTA - iConst628) & 4095)];
			float fRec300 = (0.0f - (0.600000024f * fTemp236));
			fRec306[0] = (0.0f - (fConst581 * ((fConst582 * fRec306[1]) - (fRec270[1] + fRec270[2]))));
			fRec305[0] = ((fConst637 * fRec305[1]) + (fConst638 * (fRec270[1] + (fConst639 * fRec306[0]))));
			fVec30[(IOTA & 16383)] = ((0.353553385f * fRec305[0]) + 9.99999968e-21f);
			fVec31[(IOTA & 2047)] = (fTemp193 + (fTemp36 + fTemp71));
			float fTemp237 = (0.300000012f * fVec31[((IOTA - iConst585) & 2047)]);
			float fTemp238 = (fVec30[((IOTA - iConst641) & 16383)] - (fTemp237 + (0.600000024f * fRec303[1])));
			fVec32[(IOTA & 2047)] = fTemp238;
			fRec303[0] = fVec32[((IOTA - iConst642) & 2047)];
			float fRec304 = (0.600000024f * fTemp238);
			fRec310[0] = (0.0f - (fConst581 * ((fConst582 * fRec310[1]) - (fRec266[1] + fRec266[2]))));
			fRec309[0] = ((fConst651 * fRec309[1]) + (fConst652 * (fRec266[1] + (fConst653 * fRec310[0]))));
			fVec33[(IOTA & 16383)] = ((0.353553385f * fRec309[0]) + 9.99999968e-21f);
			float fTemp239 = (fVec33[((IOTA - iConst655) & 16383)] - (fTemp237 + (0.600000024f * fRec307[1])));
			fVec34[(IOTA & 4095)] = fTemp239;
			fRec307[0] = fVec34[((IOTA - iConst656) & 4095)];
			float fRec308 = (0.600000024f * fTemp239);
			fRec314[0] = (0.0f - (fConst581 * ((fConst582 * fRec314[1]) - (fRec268[1] + fRec268[2]))));
			fRec313[0] = ((fConst665 * fRec313[1]) + (fConst666 * (fRec268[1] + (fConst667 * fRec314[0]))));
			fVec35[(IOTA & 16383)] = ((0.353553385f * fRec313[0]) + 9.99999968e-21f);
			float fTemp240 = ((fTemp237 + fVec35[((IOTA - iConst669) & 16383)]) - (0.600000024f * fRec311[1]));
			fVec36[(IOTA & 4095)] = fTemp240;
			fRec311[0] = fVec36[((IOTA - iConst670) & 4095)];
			float fRec312 = (0.600000024f * fTemp240);
			fRec318[0] = (0.0f - (fConst581 * ((fConst582 * fRec318[1]) - (fRec264[1] + fRec264[2]))));
			fRec317[0] = ((fConst679 * fRec317[1]) + (fConst680 * (fRec264[1] + (fConst681 * fRec318[0]))));
			fVec37[(IOTA & 16383)] = ((0.353553385f * fRec317[0]) + 9.99999968e-21f);
			float fTemp241 = ((fVec37[((IOTA - iConst683) & 16383)] + fTemp237) - (0.600000024f * fRec315[1]));
			fVec38[(IOTA & 2047)] = fTemp241;
			fRec315[0] = fVec38[((IOTA - iConst684) & 2047)];
			float fRec316 = (0.600000024f * fTemp241);
			float fTemp242 = (fRec316 + fRec312);
			float fTemp243 = (fRec304 + (fRec308 + fTemp242));
			fRec264[0] = (fRec272[1] + (fRec291[1] + (fRec295[1] + (fRec299[1] + (fRec303[1] + (fRec307[1] + (fRec311[1] + (fRec315[1] + (fRec273 + (fRec292 + (fRec296 + (fRec300 + fTemp243))))))))))));
			fRec265[0] = ((fRec303[1] + (fRec307[1] + (fRec311[1] + (fRec315[1] + fTemp243)))) - (fRec272[1] + (fRec291[1] + (fRec295[1] + (fRec299[1] + (fRec273 + (fRec292 + (fRec300 + fRec296))))))));
			float fTemp244 = (fRec308 + fRec304);
			fRec266[0] = ((fRec295[1] + (fRec299[1] + (fRec311[1] + (fRec315[1] + (fRec296 + (fRec300 + fTemp242)))))) - (fRec272[1] + (fRec291[1] + (fRec303[1] + (fRec307[1] + (fRec273 + (fRec292 + fTemp244)))))));
			fRec267[0] = ((fRec272[1] + (fRec291[1] + (fRec311[1] + (fRec315[1] + (fRec273 + (fRec292 + fTemp242)))))) - (fRec295[1] + (fRec299[1] + (fRec303[1] + (fRec307[1] + (fRec296 + (fRec300 + fTemp244)))))));
			float fTemp245 = (fRec316 + fRec308);
			float fTemp246 = (fRec312 + fRec304);
			fRec268[0] = ((fRec291[1] + (fRec299[1] + (fRec307[1] + (fRec315[1] + (fRec292 + (fRec300 + fTemp245)))))) - (fRec272[1] + (fRec295[1] + (fRec303[1] + (fRec311[1] + (fRec273 + (fRec296 + fTemp246)))))));
			fRec269[0] = ((fRec272[1] + (fRec295[1] + (fRec307[1] + (fRec315[1] + (fRec273 + (fRec296 + fTemp245)))))) - (fRec291[1] + (fRec299[1] + (fRec303[1] + (fRec311[1] + (fRec292 + (fRec300 + fTemp246)))))));
			float fTemp247 = (fRec316 + fRec304);
			float fTemp248 = (fRec312 + fRec308);
			fRec270[0] = ((fRec272[1] + (fRec299[1] + (fRec303[1] + (fRec315[1] + (fRec273 + (fRec300 + fTemp247)))))) - (fRec291[1] + (fRec295[1] + (fRec307[1] + (fRec311[1] + (fRec292 + (fRec296 + fTemp248)))))));
			fRec271[0] = ((fRec291[1] + (fRec295[1] + (fRec303[1] + (fRec315[1] + (fRec292 + (fRec296 + fTemp247)))))) - (fRec272[1] + (fRec299[1] + (fRec307[1] + (fRec311[1] + (fRec273 + (fRec300 + fTemp248)))))));
			fRec319[0] = (fSlow551 + (fConst688 * fRec319[1]));
			float fTemp249 = std::tan((fConst1 * fRec319[0]));
			float fTemp250 = mydsp_faustpower2_f(std::sqrt((fConst687 * fTemp249)));
			float fTemp251 = ((fConst686 * fTemp250) + -8.0f);
			float fTemp252 = (fConst685 * fTemp250);
			float fTemp253 = ((fConst565 * fTemp249) - (fConst691 * (fTemp250 / fTemp249)));
			float fTemp254 = (fConst567 * fTemp253);
			float fTemp255 = (fTemp252 + (4.0f - fTemp254));
			float fTemp256 = ((fTemp252 + fTemp254) + 4.0f);
			fRec263[0] = ((0.370000005f * (fRec265[0] + fRec266[0])) - (((fRec263[1] * fTemp251) + (fRec263[2] * fTemp255)) / fTemp256));
			float fTemp257 = (0.0f - (fConst567 * (fTemp253 / fTemp256)));
			float fTemp258 = ((fSlow3 * (((((fTemp37 + fTemp71) + (fSlow310 * fTemp111)) + fTemp124) + fTemp193) + fTemp205)) + (fSlow550 * ((fConst567 * ((fRec263[0] * fTemp253) / fTemp256)) + (fRec263[2] * fTemp257))));
			float fTemp259 = std::fabs(fTemp258);
			fRec322[0] = std::max<float>(fTemp259, ((fConst694 * fRec322[1]) + (fConst695 * fTemp259)));
			fRec321[0] = ((fConst692 * fRec321[1]) + (fConst693 * fRec322[0]));
			fRec320[0] = ((fConst64 * fRec320[1]) + (fConst65 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec321[0]) + 1.0f)), 0.0f)))));
			float fTemp260 = (fTemp258 * std::pow(10.0f, (0.0500000007f * fRec320[0])));
			float fTemp261 = std::fabs(fTemp260);
			fRec325[0] = std::max<float>(fTemp261, ((fConst698 * fRec325[1]) + (fConst699 * fTemp261)));
			fRec324[0] = ((fConst696 * fRec324[1]) + (fConst697 * fRec325[0]));
			fRec323[0] = ((fConst64 * fRec323[1]) + (fConst65 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec324[0])) + 5.0f), 0.0f)))));
			float fTemp262 = (fTemp260 * std::pow(10.0f, (0.0500000007f * fRec323[0])));
			float fTemp263 = std::fabs((fSlow0 * fTemp262));
			fRec328[0] = std::max<float>(fTemp263, ((fConst64 * fRec328[1]) + (fConst65 * fTemp263)));
			fRec327[0] = ((fConst688 * fRec327[1]) + (fConst689 * fRec328[0]));
			fRec326[0] = ((fConst64 * fRec326[1]) + (fConst65 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec327[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp262 * std::pow(10.0f, (0.0500000007f * fRec326[0])))))));
			fRec329[0] = ((0.370000005f * (fRec265[0] - fRec266[0])) - (((fTemp251 * fRec329[1]) + (fTemp255 * fRec329[2])) / fTemp256));
			float fTemp264 = ((fSlow3 * (fTemp231 + (fTemp205 + (fTemp193 + (fTemp124 + (fTemp37 + (fSlow552 * fTemp111))))))) + (fSlow550 * ((fConst567 * ((fRec329[0] * fTemp253) / fTemp256)) + (fTemp257 * fRec329[2]))));
			float fTemp265 = std::fabs(fTemp264);
			fRec332[0] = std::max<float>(fTemp265, ((fConst694 * fRec332[1]) + (fConst695 * fTemp265)));
			fRec331[0] = ((fConst692 * fRec331[1]) + (fConst693 * fRec332[0]));
			fRec330[0] = ((fConst64 * fRec330[1]) + (fConst65 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec331[0]) + 1.0f)), 0.0f)))));
			float fTemp266 = (fTemp264 * std::pow(10.0f, (0.0500000007f * fRec330[0])));
			float fTemp267 = std::fabs(fTemp266);
			fRec335[0] = std::max<float>(fTemp267, ((fConst698 * fRec335[1]) + (fConst699 * fTemp267)));
			fRec334[0] = ((fConst696 * fRec334[1]) + (fConst697 * fRec335[0]));
			fRec333[0] = ((fConst64 * fRec333[1]) + (fConst65 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec334[0])) + 5.0f), 0.0f)))));
			float fTemp268 = (fTemp266 * std::pow(10.0f, (0.0500000007f * fRec333[0])));
			float fTemp269 = std::fabs((fSlow0 * fTemp268));
			fRec338[0] = std::max<float>(fTemp269, ((fConst64 * fRec338[1]) + (fConst65 * fTemp269)));
			fRec337[0] = ((fConst688 * fRec337[1]) + (fConst689 * fRec338[0]));
			fRec336[0] = ((fConst64 * fRec336[1]) + (fConst65 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec337[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp268 * std::pow(10.0f, (0.0500000007f * fRec336[0])))))));
			iVec0[1] = iVec0[0];
			iRec6[1] = iRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			iRec7[1] = iRec7[0];
			fRec8[1] = fRec8[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec38[1] = fRec38[0];
			fRec36[1] = fRec36[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec43[1] = fRec43[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			iRec57[1] = iRec57[0];
			fRec56[1] = fRec56[0];
			fRec60[1] = fRec60[0];
			iRec59[1] = iRec59[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec65[1] = fRec65[0];
			iRec64[1] = iRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec68[1] = fRec68[0];
			iRec67[1] = iRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec71[1] = fRec71[0];
			iRec70[1] = iRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec74[1] = fRec74[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			iRec79[1] = iRec79[0];
			fRec83[1] = fRec83[0];
			IOTA = (IOTA + 1);
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			fVec10[1] = fVec10[0];
			fRec84[1] = fRec84[0];
			fRec81[1] = fRec81[0];
			fRec85[1] = fRec85[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			iRec89[1] = iRec89[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
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
			fRec106[2] = fRec106[1];
			fRec106[1] = fRec106[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec108[2] = fRec108[1];
			fRec108[1] = fRec108[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fRec111[2] = fRec111[1];
			fRec111[1] = fRec111[0];
			fRec112[2] = fRec112[1];
			fRec112[1] = fRec112[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec116[2] = fRec116[1];
			fRec116[1] = fRec116[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec118[2] = fRec118[1];
			fRec118[1] = fRec118[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fRec124[2] = fRec124[1];
			fRec124[1] = fRec124[0];
			fRec125[2] = fRec125[1];
			fRec125[1] = fRec125[0];
			fRec126[2] = fRec126[1];
			fRec126[1] = fRec126[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fRec128[2] = fRec128[1];
			fRec128[1] = fRec128[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			fRec132[2] = fRec132[1];
			fRec132[1] = fRec132[0];
			fRec133[2] = fRec133[1];
			fRec133[1] = fRec133[0];
			fRec134[2] = fRec134[1];
			fRec134[1] = fRec134[0];
			fRec135[2] = fRec135[1];
			fRec135[1] = fRec135[0];
			fRec136[2] = fRec136[1];
			fRec136[1] = fRec136[0];
			fRec137[2] = fRec137[1];
			fRec137[1] = fRec137[0];
			fRec138[2] = fRec138[1];
			fRec138[1] = fRec138[0];
			fVec11[1] = fVec11[0];
			fRec82[1] = fRec82[0];
			fRec141[1] = fRec141[0];
			fRec140[1] = fRec140[0];
			fRec139[1] = fRec139[0];
			fRec142[2] = fRec142[1];
			fRec142[1] = fRec142[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec146[1] = fRec146[0];
			fRec148[1] = fRec148[0];
			fRec152[1] = fRec152[0];
			fRec151[1] = fRec151[0];
			fRec150[1] = fRec150[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec155[1] = fRec155[0];
			fRec154[1] = fRec154[0];
			fRec153[1] = fRec153[0];
			fRec156[2] = fRec156[1];
			fRec156[1] = fRec156[0];
			fRec145[2] = fRec145[1];
			fRec145[1] = fRec145[0];
			fRec144[2] = fRec144[1];
			fRec144[1] = fRec144[0];
			fRec143[2] = fRec143[1];
			fRec143[1] = fRec143[0];
			fVec13[1] = fVec13[0];
			fVec14[1] = fVec14[0];
			fVec15[1] = fVec15[0];
			fRec186[1] = fRec186[0];
			fRec182[1] = fRec182[0];
			fRec187[1] = fRec187[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec189[j0] = fRec189[(j0 - 1)];
			}
			fVec16[1] = fVec16[0];
			fVec17[2] = fVec17[1];
			fVec17[1] = fVec17[0];
			fRec178[1] = fRec178[0];
			fRec174[1] = fRec174[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec168[j1] = fRec168[(j1 - 1)];
			}
			fRec161[1] = fRec161[0];
			fRec191[1] = fRec191[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			iRec192[1] = iRec192[0];
			iRec193[1] = iRec193[0];
			fRec196[1] = fRec196[0];
			fRec195[1] = fRec195[0];
			fRec194[1] = fRec194[0];
			fRec197[2] = fRec197[1];
			fRec197[1] = fRec197[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec157[2] = fRec157[1];
			fRec157[1] = fRec157[0];
			fRec202[2] = fRec202[1];
			fRec202[1] = fRec202[0];
			fVec18[1] = fVec18[0];
			fVec19[1] = fVec19[0];
			fVec20[1] = fVec20[0];
			iRec203[1] = iRec203[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec204[2] = fRec204[1];
			fRec204[1] = fRec204[0];
			fRec205[2] = fRec205[1];
			fRec205[1] = fRec205[0];
			fRec206[2] = fRec206[1];
			fRec206[1] = fRec206[0];
			fRec207[2] = fRec207[1];
			fRec207[1] = fRec207[0];
			fRec208[2] = fRec208[1];
			fRec208[1] = fRec208[0];
			fRec209[2] = fRec209[1];
			fRec209[1] = fRec209[0];
			fRec210[2] = fRec210[1];
			fRec210[1] = fRec210[0];
			fRec211[2] = fRec211[1];
			fRec211[1] = fRec211[0];
			fRec212[2] = fRec212[1];
			fRec212[1] = fRec212[0];
			fRec213[2] = fRec213[1];
			fRec213[1] = fRec213[0];
			fRec214[2] = fRec214[1];
			fRec214[1] = fRec214[0];
			fRec215[2] = fRec215[1];
			fRec215[1] = fRec215[0];
			fRec216[2] = fRec216[1];
			fRec216[1] = fRec216[0];
			fRec217[2] = fRec217[1];
			fRec217[1] = fRec217[0];
			fRec218[2] = fRec218[1];
			fRec218[1] = fRec218[0];
			fRec219[2] = fRec219[1];
			fRec219[1] = fRec219[0];
			fRec220[2] = fRec220[1];
			fRec220[1] = fRec220[0];
			fRec221[2] = fRec221[1];
			fRec221[1] = fRec221[0];
			fRec222[2] = fRec222[1];
			fRec222[1] = fRec222[0];
			fRec223[2] = fRec223[1];
			fRec223[1] = fRec223[0];
			fRec224[2] = fRec224[1];
			fRec224[1] = fRec224[0];
			fRec225[2] = fRec225[1];
			fRec225[1] = fRec225[0];
			fRec226[2] = fRec226[1];
			fRec226[1] = fRec226[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
			fRec228[2] = fRec228[1];
			fRec228[1] = fRec228[0];
			fRec229[2] = fRec229[1];
			fRec229[1] = fRec229[0];
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
			fRec253[1] = fRec253[0];
			fRec254[1] = fRec254[0];
			fRec255[1] = fRec255[0];
			fRec256[1] = fRec256[0];
			fRec257[1] = fRec257[0];
			fRec258[1] = fRec258[0];
			fRec261[1] = fRec261[0];
			fRec260[1] = fRec260[0];
			fRec259[1] = fRec259[0];
			fRec262[2] = fRec262[1];
			fRec262[1] = fRec262[0];
			fRec200[2] = fRec200[1];
			fRec200[1] = fRec200[0];
			fRec199[2] = fRec199[1];
			fRec199[1] = fRec199[0];
			fRec198[2] = fRec198[1];
			fRec198[1] = fRec198[0];
			fRec275[1] = fRec275[0];
			fRec274[1] = fRec274[0];
			iRec280[1] = iRec280[0];
			fRec279[2] = fRec279[1];
			fRec279[1] = fRec279[0];
			iRec282[1] = iRec282[0];
			fRec281[2] = fRec281[1];
			fRec281[1] = fRec281[0];
			iRec284[1] = iRec284[0];
			fRec283[2] = fRec283[1];
			fRec283[1] = fRec283[0];
			iRec286[1] = iRec286[0];
			fRec285[2] = fRec285[1];
			fRec285[1] = fRec285[0];
			fRec289[1] = fRec289[0];
			fRec288[1] = fRec288[0];
			fRec287[1] = fRec287[0];
			fRec290[2] = fRec290[1];
			fRec290[1] = fRec290[0];
			fRec278[2] = fRec278[1];
			fRec278[1] = fRec278[0];
			fRec277[2] = fRec277[1];
			fRec277[1] = fRec277[0];
			fRec276[2] = fRec276[1];
			fRec276[1] = fRec276[0];
			fRec272[1] = fRec272[0];
			fRec294[1] = fRec294[0];
			fRec293[1] = fRec293[0];
			fRec291[1] = fRec291[0];
			fRec298[1] = fRec298[0];
			fRec297[1] = fRec297[0];
			fRec295[1] = fRec295[0];
			fRec302[1] = fRec302[0];
			fRec301[1] = fRec301[0];
			fRec299[1] = fRec299[0];
			fRec306[1] = fRec306[0];
			fRec305[1] = fRec305[0];
			fRec303[1] = fRec303[0];
			fRec310[1] = fRec310[0];
			fRec309[1] = fRec309[0];
			fRec307[1] = fRec307[0];
			fRec314[1] = fRec314[0];
			fRec313[1] = fRec313[0];
			fRec311[1] = fRec311[0];
			fRec318[1] = fRec318[0];
			fRec317[1] = fRec317[0];
			fRec315[1] = fRec315[0];
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
			fRec319[1] = fRec319[0];
			fRec263[2] = fRec263[1];
			fRec263[1] = fRec263[0];
			fRec322[1] = fRec322[0];
			fRec321[1] = fRec321[0];
			fRec320[1] = fRec320[0];
			fRec325[1] = fRec325[0];
			fRec324[1] = fRec324[0];
			fRec323[1] = fRec323[0];
			fRec328[1] = fRec328[0];
			fRec327[1] = fRec327[0];
			fRec326[1] = fRec326[0];
			fRec329[2] = fRec329[1];
			fRec329[1] = fRec329[0];
			fRec332[1] = fRec332[0];
			fRec331[1] = fRec331[0];
			fRec330[1] = fRec330[0];
			fRec335[1] = fRec335[0];
			fRec334[1] = fRec334[0];
			fRec333[1] = fRec333[0];
			fRec338[1] = fRec338[0];
			fRec337[1] = fRec337[0];
			fRec336[1] = fRec336[0];
		}
	}

};

#endif
