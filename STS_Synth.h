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
	
	int iRec36[2];
	
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
			iRec36[l36] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec36[0] = (iRec36[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec36[0] + -1))));
			iRec36[1] = iRec36[0];
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
	float fRec37[2];
	float fConst72;
	float fConst73;
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fHslider24;
	float fVec4[2];
	float fVec5[2];
	float fVec6[2];
	FAUSTFLOAT fHslider25;
	float fRec39[2];
	FAUSTFLOAT fHslider26;
	float fConst74;
	float fRec41[2];
	float fRec40[2];
	float fRec38[2];
	float fRec43[2];
	FAUSTFLOAT fHslider27;
	float fRec42[3];
	float fRec45[3];
	float fRec46[3];
	float fRec47[3];
	float fRec48[3];
	float fConst75;
	float fConst76;
	int iRec49[2];
	float fConst77;
	float fConst78;
	float fConst79;
	float fConst80;
	float fRec35[3];
	float fConst81;
	float fConst82;
	float fConst83;
	int iRec50[2];
	FAUSTFLOAT fHslider28;
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	float fRec53[2];
	float fRec52[2];
	FAUSTFLOAT fHslider31;
	float fRec51[2];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec54[3];
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
	float fConst84;
	float fConst85;
	FAUSTFLOAT fHslider43;
	float fVec7[2];
	float fVec8[2];
	float fVec9[2];
	int iRec59[2];
	float fRec58[2];
	FAUSTFLOAT fHslider44;
	float fRec62[2];
	int iRec61[2];
	float fConst86;
	float fConst87;
	float fConst88;
	float fConst89;
	float fConst90;
	float fRec64[2];
	float fRec63[2];
	float fRec60[3];
	FAUSTFLOAT fHslider45;
	float fRec67[2];
	int iRec66[2];
	float fRec65[3];
	FAUSTFLOAT fHslider46;
	float fRec70[2];
	int iRec69[2];
	float fRec68[3];
	FAUSTFLOAT fHslider47;
	float fRec73[2];
	int iRec72[2];
	float fRec71[3];
	FAUSTFLOAT fHslider48;
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	float fRec76[2];
	float fRec75[2];
	FAUSTFLOAT fHslider51;
	float fRec74[2];
	FAUSTFLOAT fHslider52;
	FAUSTFLOAT fHslider53;
	float fRec77[3];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec57[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec56[3];
	float fRec55[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	int iRec81[2];
	float fRec85[2];
	int IOTA;
	float fVec10[2048];
	float fConst91;
	float fConst92;
	FAUSTFLOAT fHslider63;
	float fVec11[2];
	float fVec12[2];
	float fVec13[2];
	float fRec86[2];
	float fRec83[2];
	float fRec87[2];
	float fConst95;
	float fConst98;
	float fConst100;
	float fConst101;
	float fConst102;
	float fRec90[3];
	float fConst103;
	float fConst104;
	float fConst105;
	float fRec89[3];
	float fConst106;
	int iRec91[2];
	float fConst108;
	float fConst109;
	float fRec88[3];
	float fConst110;
	float fConst111;
	float fConst113;
	float fConst114;
	float fConst115;
	float fRec92[3];
	float fConst116;
	float fConst118;
	float fConst119;
	float fConst120;
	float fRec93[3];
	float fConst121;
	float fConst123;
	float fConst124;
	float fConst125;
	float fRec94[3];
	float fConst126;
	float fConst128;
	float fConst129;
	float fConst130;
	float fRec95[3];
	float fConst131;
	float fConst133;
	float fConst134;
	float fConst135;
	float fRec96[3];
	float fConst136;
	float fConst138;
	float fConst139;
	float fConst140;
	float fRec97[3];
	float fConst141;
	float fConst143;
	float fConst144;
	float fConst145;
	float fRec98[3];
	float fConst146;
	float fConst148;
	float fConst149;
	float fConst150;
	float fRec99[3];
	float fConst151;
	float fConst153;
	float fConst154;
	float fConst155;
	float fRec100[3];
	float fConst156;
	float fConst158;
	float fConst159;
	float fConst160;
	float fRec101[3];
	float fConst161;
	float fConst163;
	float fConst164;
	float fConst165;
	float fRec102[3];
	float fConst166;
	float fConst168;
	float fConst169;
	float fConst170;
	float fRec103[3];
	float fConst171;
	float fConst173;
	float fConst174;
	float fConst175;
	float fRec104[3];
	float fConst176;
	float fConst178;
	float fConst179;
	float fConst180;
	float fRec105[3];
	float fConst181;
	float fConst183;
	float fConst184;
	float fConst185;
	float fRec106[3];
	float fConst186;
	float fConst188;
	float fConst189;
	float fConst190;
	float fRec107[3];
	float fConst191;
	float fConst193;
	float fConst194;
	float fConst195;
	float fRec108[3];
	float fConst196;
	float fConst198;
	float fConst199;
	float fConst200;
	float fRec109[3];
	float fConst201;
	float fConst203;
	float fConst204;
	float fConst205;
	float fRec110[3];
	float fConst206;
	float fConst208;
	float fConst209;
	float fConst210;
	float fRec111[3];
	float fConst211;
	float fConst213;
	float fConst214;
	float fConst215;
	float fRec112[3];
	float fConst216;
	float fConst218;
	float fConst219;
	float fConst220;
	float fRec113[3];
	float fConst221;
	float fConst223;
	float fConst224;
	float fConst225;
	float fRec114[3];
	float fConst226;
	float fConst228;
	float fConst229;
	float fConst230;
	float fRec115[3];
	float fConst231;
	float fConst233;
	float fConst234;
	float fConst235;
	float fRec116[3];
	float fConst236;
	float fConst238;
	float fConst239;
	float fConst240;
	float fRec117[3];
	float fConst241;
	float fConst243;
	float fConst244;
	float fConst245;
	float fRec118[3];
	float fConst246;
	float fConst248;
	float fConst249;
	float fConst250;
	float fRec119[3];
	float fConst251;
	float fConst253;
	float fConst254;
	float fConst255;
	float fRec120[3];
	float fConst256;
	float fConst258;
	float fConst259;
	float fConst260;
	float fRec121[3];
	float fConst261;
	float fConst263;
	float fConst264;
	float fConst265;
	float fRec122[3];
	float fConst266;
	float fConst268;
	float fConst269;
	float fConst270;
	float fRec123[3];
	float fConst271;
	float fConst273;
	float fConst274;
	float fConst275;
	float fRec124[3];
	float fConst276;
	float fConst278;
	float fConst279;
	float fConst280;
	float fRec125[3];
	float fConst281;
	float fConst283;
	float fConst284;
	float fConst285;
	float fRec126[3];
	float fConst286;
	float fConst288;
	float fConst289;
	float fConst290;
	float fRec127[3];
	float fConst291;
	float fConst293;
	float fConst294;
	float fConst295;
	float fRec128[3];
	float fConst296;
	float fConst298;
	float fConst299;
	float fConst300;
	float fRec129[3];
	float fConst301;
	float fConst303;
	float fConst304;
	float fConst305;
	float fRec130[3];
	float fConst306;
	float fConst308;
	float fConst309;
	float fConst310;
	float fRec131[3];
	float fConst311;
	float fConst313;
	float fConst314;
	float fConst315;
	float fRec132[3];
	float fConst316;
	float fConst318;
	float fConst319;
	float fConst320;
	float fRec133[3];
	float fConst321;
	float fConst323;
	float fConst324;
	float fConst325;
	float fRec134[3];
	float fConst326;
	float fConst328;
	float fConst329;
	float fConst330;
	float fRec135[3];
	float fConst331;
	float fConst333;
	float fConst334;
	float fConst335;
	float fRec136[3];
	float fConst336;
	float fConst338;
	float fConst339;
	float fConst340;
	float fRec137[3];
	float fConst341;
	float fConst343;
	float fConst344;
	float fConst345;
	float fRec138[3];
	float fConst346;
	float fConst348;
	float fConst349;
	float fConst350;
	float fRec139[3];
	float fConst351;
	float fConst353;
	float fConst354;
	float fConst355;
	float fRec140[3];
	float fConst356;
	float fVec14[2];
	float fVec15[2048];
	float fRec84[2];
	FAUSTFLOAT fHslider64;
	FAUSTFLOAT fHslider65;
	FAUSTFLOAT fHslider66;
	float fRec143[2];
	float fRec142[2];
	FAUSTFLOAT fHslider67;
	float fRec141[2];
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	float fRec144[3];
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec80[3];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec79[3];
	float fRec78[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	float fConst357;
	float fRec148[2];
	float fRec150[2];
	float fConst358;
	float fRec154[2];
	float fRec153[2];
	float fRec152[2];
	float fRec151[2];
	float fRec149[2];
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	float fRec157[2];
	float fRec156[2];
	FAUSTFLOAT fHslider82;
	float fRec155[2];
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec158[3];
	FAUSTFLOAT fHslider85;
	FAUSTFLOAT fHslider86;
	float fRec147[3];
	FAUSTFLOAT fHslider87;
	FAUSTFLOAT fHslider88;
	float fRec146[3];
	float fRec145[3];
	FAUSTFLOAT fHslider89;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	FAUSTFLOAT fHslider92;
	FAUSTFLOAT fHslider93;
	float fConst359;
	FAUSTFLOAT fHslider94;
	float fVec16[2];
	float fVec17[2];
	float fVec18[2];
	FAUSTFLOAT fHslider95;
	float fRec188[2];
	float fRec184[2];
	FAUSTFLOAT fHslider96;
	float fRec189[2];
	float fRec191[4];
	float fRec192[1024];
	float fConst360;
	float fVec19[2];
	float fVec20[3];
	float fRec190[2048];
	float fRec180[2];
	float fRec176[2];
	float fRec172[1024];
	float fRec170[4];
	float fRec165[2048];
	float fRec163[2];
	FAUSTFLOAT fHslider97;
	float fRec193[2];
	float fRec162[3];
	int iRec194[2];
	int iRec195[2];
	FAUSTFLOAT fHslider98;
	FAUSTFLOAT fHslider99;
	FAUSTFLOAT fHslider100;
	float fRec198[2];
	float fRec197[2];
	FAUSTFLOAT fHslider101;
	float fRec196[2];
	FAUSTFLOAT fHslider102;
	FAUSTFLOAT fHslider103;
	float fRec199[3];
	FAUSTFLOAT fHslider104;
	FAUSTFLOAT fHslider105;
	float fRec161[3];
	FAUSTFLOAT fHslider106;
	FAUSTFLOAT fHslider107;
	float fRec160[3];
	float fRec159[3];
	FAUSTFLOAT fHslider108;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider109;
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fHslider111;
	FAUSTFLOAT fHslider112;
	float fConst361;
	float fConst364;
	float fConst365;
	float fConst366;
	float fRec204[3];
	float fConst367;
	FAUSTFLOAT fHslider113;
	float fVec21[2];
	float fVec22[2];
	float fVec23[2];
	int iRec205[2];
	float fConst369;
	float fConst370;
	float fRec203[3];
	float fConst371;
	float fConst373;
	float fConst374;
	float fRec206[3];
	float fConst375;
	float fConst377;
	float fConst378;
	float fRec207[3];
	float fConst379;
	float fConst381;
	float fConst382;
	float fRec208[3];
	float fConst383;
	float fConst385;
	float fConst386;
	float fRec209[3];
	float fConst387;
	float fConst389;
	float fConst390;
	float fRec210[3];
	float fConst391;
	float fConst393;
	float fConst394;
	float fRec211[3];
	float fConst395;
	float fConst397;
	float fConst398;
	float fRec212[3];
	float fConst399;
	float fConst401;
	float fConst402;
	float fRec213[3];
	float fConst403;
	float fConst405;
	float fConst406;
	float fRec214[3];
	float fConst407;
	float fConst409;
	float fConst410;
	float fRec215[3];
	float fConst411;
	float fConst413;
	float fConst414;
	float fRec216[3];
	float fConst415;
	float fConst417;
	float fConst418;
	float fRec217[3];
	float fConst419;
	float fConst421;
	float fConst422;
	float fRec218[3];
	float fConst423;
	float fConst425;
	float fConst426;
	float fRec219[3];
	float fConst427;
	float fConst429;
	float fConst430;
	float fRec220[3];
	float fConst431;
	float fConst433;
	float fConst434;
	float fRec221[3];
	float fConst435;
	float fConst437;
	float fConst438;
	float fRec222[3];
	float fConst439;
	float fConst441;
	float fConst442;
	float fRec223[3];
	float fConst443;
	float fConst445;
	float fConst446;
	float fRec224[3];
	float fConst447;
	float fConst449;
	float fConst450;
	float fRec225[3];
	float fConst451;
	float fConst453;
	float fConst454;
	float fRec226[3];
	float fConst455;
	float fConst457;
	float fConst458;
	float fRec227[3];
	float fConst459;
	float fConst461;
	float fConst462;
	float fRec228[3];
	float fConst463;
	float fConst465;
	float fConst466;
	float fRec229[3];
	float fConst467;
	float fConst469;
	float fConst470;
	float fRec230[3];
	float fConst471;
	float fConst473;
	float fConst474;
	float fRec231[3];
	float fConst475;
	float fConst477;
	float fConst478;
	float fRec232[3];
	float fConst479;
	float fConst481;
	float fConst482;
	float fRec233[3];
	float fConst483;
	float fConst485;
	float fConst486;
	float fRec234[3];
	float fConst487;
	float fConst489;
	float fConst490;
	float fRec235[3];
	float fConst491;
	float fConst493;
	float fConst494;
	float fRec236[3];
	float fConst495;
	float fConst497;
	float fConst498;
	float fRec237[3];
	float fConst499;
	float fConst501;
	float fConst502;
	float fRec238[3];
	float fConst503;
	float fConst505;
	float fConst506;
	float fRec239[3];
	float fConst507;
	float fConst509;
	float fConst510;
	float fRec240[3];
	float fConst511;
	float fConst513;
	float fConst514;
	float fRec241[3];
	float fConst515;
	float fConst517;
	float fConst518;
	float fRec242[3];
	float fConst519;
	float fConst521;
	float fConst522;
	float fRec243[3];
	float fConst523;
	float fConst525;
	float fConst526;
	float fRec244[3];
	float fConst527;
	float fConst529;
	float fConst530;
	float fRec245[3];
	float fConst531;
	float fConst533;
	float fConst534;
	float fRec246[3];
	float fConst535;
	float fConst537;
	float fConst538;
	float fRec247[3];
	float fConst539;
	float fConst541;
	float fConst542;
	float fRec248[3];
	float fConst543;
	float fConst545;
	float fConst546;
	float fRec249[3];
	float fConst547;
	float fConst549;
	float fConst550;
	float fRec250[3];
	float fConst551;
	float fConst553;
	float fConst554;
	float fRec251[3];
	float fConst555;
	float fConst557;
	float fConst558;
	float fRec252[3];
	float fConst559;
	float fConst561;
	float fConst562;
	float fRec253[3];
	float fConst563;
	float fConst565;
	float fConst566;
	float fRec254[3];
	float fConst567;
	float fConst568;
	FAUSTFLOAT fHslider114;
	float fRec255[2];
	float fRec256[2];
	FAUSTFLOAT fHslider115;
	float fRec257[2];
	float fRec258[2];
	FAUSTFLOAT fHslider116;
	float fRec259[2];
	float fRec260[2];
	FAUSTFLOAT fHslider117;
	FAUSTFLOAT fHslider118;
	FAUSTFLOAT fHslider119;
	float fRec263[2];
	float fRec262[2];
	FAUSTFLOAT fHslider120;
	float fRec261[2];
	FAUSTFLOAT fHslider121;
	FAUSTFLOAT fHslider122;
	float fRec264[3];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	float fRec202[3];
	FAUSTFLOAT fHslider125;
	FAUSTFLOAT fHslider126;
	float fRec201[3];
	float fRec200[3];
	float fConst569;
	float fConst579;
	float fConst580;
	float fConst581;
	float fConst583;
	float fConst584;
	float fRec277[2];
	float fRec276[2];
	float fVec24[32768];
	int iConst586;
	float fVec25[2048];
	int iConst587;
	float fVec26[2048];
	int iConst588;
	float fRec274[2];
	float fConst597;
	float fConst598;
	float fConst599;
	float fRec281[2];
	float fRec280[2];
	float fVec27[32768];
	int iConst601;
	float fVec28[4096];
	int iConst602;
	float fRec278[2];
	float fConst611;
	float fConst612;
	float fConst613;
	float fRec285[2];
	float fRec284[2];
	float fVec29[16384];
	int iConst615;
	float fVec30[4096];
	int iConst616;
	float fRec282[2];
	float fConst625;
	float fConst626;
	float fConst627;
	float fRec289[2];
	float fRec288[2];
	float fVec31[32768];
	int iConst629;
	float fVec32[4096];
	int iConst630;
	float fRec286[2];
	float fConst639;
	float fConst640;
	float fConst641;
	float fRec293[2];
	float fRec292[2];
	float fVec33[16384];
	int iConst643;
	float fVec34[2048];
	int iConst644;
	float fRec290[2];
	float fConst653;
	float fConst654;
	float fConst655;
	float fRec297[2];
	float fRec296[2];
	float fVec35[16384];
	int iConst657;
	float fVec36[4096];
	int iConst658;
	float fRec294[2];
	float fConst667;
	float fConst668;
	float fConst669;
	float fRec301[2];
	float fRec300[2];
	float fVec37[16384];
	int iConst671;
	float fVec38[4096];
	int iConst672;
	float fRec298[2];
	float fConst681;
	float fConst682;
	float fConst683;
	float fRec305[2];
	float fRec304[2];
	float fVec39[16384];
	int iConst685;
	float fVec40[2048];
	int iConst686;
	float fRec302[2];
	float fRec266[3];
	float fRec267[3];
	float fRec268[3];
	float fRec269[3];
	float fRec270[3];
	float fRec271[3];
	float fRec272[3];
	float fRec273[3];
	float fConst687;
	float fConst688;
	float fConst689;
	float fConst690;
	float fConst691;
	float fConst692;
	float fRec306[2];
	float fConst693;
	float fRec265[3];
	float fConst694;
	float fConst695;
	float fConst696;
	float fConst697;
	float fRec309[2];
	float fRec308[2];
	float fRec307[2];
	float fConst698;
	float fConst699;
	float fConst700;
	float fConst701;
	float fRec312[2];
	float fRec311[2];
	float fRec310[2];
	float fRec315[2];
	float fRec314[2];
	float fRec313[2];
	int iRec320[2];
	float fRec319[3];
	int iRec322[2];
	float fRec321[3];
	int iRec324[2];
	float fRec323[3];
	int iRec326[2];
	float fRec325[3];
	float fRec329[2];
	float fRec328[2];
	float fRec327[2];
	float fRec330[3];
	float fRec318[3];
	float fRec317[3];
	float fRec316[3];
	float fRec331[3];
	float fRec334[2];
	float fRec333[2];
	float fRec332[2];
	float fRec337[2];
	float fRec336[2];
	float fRec335[2];
	float fRec340[2];
	float fRec339[2];
	float fRec338[2];
	
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
		fConst71 = (6.0f / fConst0);
		fConst72 = std::exp((0.0f - (50.0f / fConst0)));
		fConst73 = (1.0f - fConst72);
		fConst74 = (8.0f / fConst0);
		fConst75 = std::max<float>(1.0f, (0.0199999996f * fConst0));
		fConst76 = (1.0f / fConst75);
		fConst77 = (1.0f / std::max<float>(1.0f, (0.800000012f * fConst0)));
		fConst78 = (1.0f / fConst69);
		fConst79 = (((fConst68 + -0.333333343f) / fConst67) + 1.0f);
		fConst80 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst67))));
		fConst81 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst82 = (1.0f / fConst81);
		fConst83 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst84 = std::exp((0.0f - (500.0f / fConst0)));
		fConst85 = (1.0f - fConst84);
		fConst86 = (0.330000013f * fConst0);
		fConst87 = (0.100000001f * fConst0);
		fConst88 = (0.660000026f * fConst0);
		fConst89 = std::exp((0.0f - (100.0f / fConst0)));
		fConst90 = (0.699999988f * (1.0f - fConst89));
		fConst91 = (0.00882352982f * fConst0);
		fConst92 = (0.00147058826f * fConst0);
		float fConst93 = std::tan((37699.1133f / fConst0));
		float fConst94 = (1.0f / fConst93);
		fConst95 = (1.0f / (((fConst94 + 1.41421354f) / fConst93) + 1.0f));
		float fConst96 = std::tan((31.415926f / fConst0));
		float fConst97 = (1.0f / fConst96);
		fConst98 = (1.0f / (((fConst97 + 1.41421354f) / fConst96) + 1.0f));
		float fConst99 = mydsp_faustpower2_f(fConst96);
		fConst100 = (1.0f / fConst99);
		fConst101 = (((fConst97 + -1.41421354f) / fConst96) + 1.0f);
		fConst102 = (2.0f * (1.0f - fConst100));
		fConst103 = (0.0f - (2.0f / fConst99));
		fConst104 = (((fConst94 + -1.41421354f) / fConst93) + 1.0f);
		fConst105 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst93))));
		fConst106 = (1.0f / std::max<float>(1.0f, (0.00400000019f * fConst0)));
		float fConst107 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst108 = (0.0f - (2.0f * fConst107));
		fConst109 = mydsp_faustpower2_f(fConst107);
		fConst110 = ((0.5f * fConst0) + -1.0f);
		fConst111 = ftbl1mydspSIG1[0];
		float fConst112 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst113 = (0.0f - (2.0f * fConst112));
		fConst114 = (20.8197117f / fConst0);
		fConst115 = mydsp_faustpower2_f(fConst112);
		fConst116 = ftbl1mydspSIG1[1];
		float fConst117 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst118 = (0.0f - (2.0f * fConst117));
		fConst119 = (24.0940685f / fConst0);
		fConst120 = mydsp_faustpower2_f(fConst117);
		fConst121 = ftbl1mydspSIG1[2];
		float fConst122 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst123 = (0.0f - (2.0f * fConst122));
		fConst124 = (50.6621399f / fConst0);
		fConst125 = mydsp_faustpower2_f(fConst122);
		fConst126 = ftbl1mydspSIG1[3];
		float fConst127 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst128 = (0.0f - (2.0f * fConst127));
		fConst129 = (59.3621521f / fConst0);
		fConst130 = mydsp_faustpower2_f(fConst127);
		fConst131 = ftbl1mydspSIG1[4];
		float fConst132 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst133 = (0.0f - (2.0f * fConst132));
		fConst134 = (88.6990967f / fConst0);
		fConst135 = mydsp_faustpower2_f(fConst132);
		fConst136 = ftbl1mydspSIG1[5];
		float fConst137 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst138 = (0.0f - (2.0f * fConst137));
		fConst139 = (115.510078f / fConst0);
		fConst140 = mydsp_faustpower2_f(fConst137);
		fConst141 = ftbl1mydspSIG1[6];
		float fConst142 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst143 = (0.0f - (2.0f * fConst142));
		fConst144 = (132.010986f / fConst0);
		fConst145 = mydsp_faustpower2_f(fConst142);
		fConst146 = ftbl1mydspSIG1[7];
		float fConst147 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst148 = (0.0f - (2.0f * fConst147));
		fConst149 = (164.478088f / fConst0);
		fConst150 = mydsp_faustpower2_f(fConst147);
		fConst151 = ftbl1mydspSIG1[8];
		float fConst152 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst153 = (0.0f - (2.0f * fConst152));
		fConst154 = (182.177185f / fConst0);
		fConst155 = mydsp_faustpower2_f(fConst152);
		fConst156 = ftbl1mydspSIG1[9];
		float fConst157 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst158 = (0.0f - (2.0f * fConst157));
		fConst159 = (232.935272f / fConst0);
		fConst160 = mydsp_faustpower2_f(fConst157);
		fConst161 = ftbl1mydspSIG1[10];
		float fConst162 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst163 = (0.0f - (2.0f * fConst162));
		fConst164 = (237.946106f / fConst0);
		fConst165 = mydsp_faustpower2_f(fConst162);
		fConst166 = ftbl1mydspSIG1[11];
		float fConst167 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst168 = (0.0f - (2.0f * fConst167));
		fConst169 = (251.725769f / fConst0);
		fConst170 = mydsp_faustpower2_f(fConst167);
		fConst171 = ftbl1mydspSIG1[12];
		float fConst172 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst173 = (0.0f - (2.0f * fConst172));
		fConst174 = (299.355438f / fConst0);
		fConst175 = mydsp_faustpower2_f(fConst172);
		fConst176 = ftbl1mydspSIG1[13];
		float fConst177 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst178 = (0.0f - (2.0f * fConst177));
		fConst179 = (320.561829f / fConst0);
		fConst180 = mydsp_faustpower2_f(fConst177);
		fConst181 = ftbl1mydspSIG1[14];
		float fConst182 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst183 = (0.0f - (2.0f * fConst182));
		fConst184 = (329.427399f / fConst0);
		fConst185 = mydsp_faustpower2_f(fConst182);
		fConst186 = ftbl1mydspSIG1[15];
		float fConst187 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst188 = (0.0f - (2.0f * fConst187));
		fConst189 = (366.221741f / fConst0);
		fConst190 = mydsp_faustpower2_f(fConst187);
		fConst191 = ftbl1mydspSIG1[16];
		float fConst192 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst193 = (0.0f - (2.0f * fConst192));
		fConst194 = (399.287628f / fConst0);
		fConst195 = mydsp_faustpower2_f(fConst192);
		fConst196 = ftbl1mydspSIG1[17];
		float fConst197 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst198 = (0.0f - (2.0f * fConst197));
		fConst199 = (410.686584f / fConst0);
		fConst200 = mydsp_faustpower2_f(fConst197);
		fConst201 = ftbl1mydspSIG1[18];
		float fConst202 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst203 = (0.0f - (2.0f * fConst202));
		fConst204 = (420.713928f / fConst0);
		fConst205 = mydsp_faustpower2_f(fConst202);
		fConst206 = ftbl1mydspSIG1[19];
		float fConst207 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst208 = (0.0f - (2.0f * fConst207));
		fConst209 = (468.286438f / fConst0);
		fConst210 = mydsp_faustpower2_f(fConst207);
		fConst211 = ftbl1mydspSIG1[20];
		float fConst212 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst213 = (0.0f - (2.0f * fConst212));
		fConst214 = (494.436432f / fConst0);
		fConst215 = mydsp_faustpower2_f(fConst212);
		fConst216 = ftbl1mydspSIG1[21];
		float fConst217 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst218 = (0.0f - (2.0f * fConst217));
		fConst219 = (507.916992f / fConst0);
		fConst220 = mydsp_faustpower2_f(fConst217);
		fConst221 = ftbl1mydspSIG1[22];
		float fConst222 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst223 = (0.0f - (2.0f * fConst222));
		fConst224 = (565.34845f / fConst0);
		fConst225 = mydsp_faustpower2_f(fConst222);
		fConst226 = ftbl1mydspSIG1[23];
		float fConst227 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst228 = (0.0f - (2.0f * fConst227));
		fConst229 = (584.12323f / fConst0);
		fConst230 = mydsp_faustpower2_f(fConst227);
		fConst231 = ftbl1mydspSIG1[24];
		float fConst232 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst233 = (0.0f - (2.0f * fConst232));
		fConst234 = (598.105225f / fConst0);
		fConst235 = mydsp_faustpower2_f(fConst232);
		fConst236 = ftbl1mydspSIG1[25];
		float fConst237 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst238 = (0.0f - (2.0f * fConst237));
		fConst239 = (612.489319f / fConst0);
		fConst240 = mydsp_faustpower2_f(fConst237);
		fConst241 = ftbl1mydspSIG1[26];
		float fConst242 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst243 = (0.0f - (2.0f * fConst242));
		fConst244 = (695.045959f / fConst0);
		fConst245 = mydsp_faustpower2_f(fConst242);
		fConst246 = ftbl1mydspSIG1[27];
		float fConst247 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst248 = (0.0f - (2.0f * fConst247));
		fConst249 = (704.150269f / fConst0);
		fConst250 = mydsp_faustpower2_f(fConst247);
		fConst251 = ftbl1mydspSIG1[28];
		float fConst252 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst253 = (0.0f - (2.0f * fConst252));
		fConst254 = (715.18988f / fConst0);
		fConst255 = mydsp_faustpower2_f(fConst252);
		fConst256 = ftbl1mydspSIG1[29];
		float fConst257 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst258 = (0.0f - (2.0f * fConst257));
		fConst259 = (749.935852f / fConst0);
		fConst260 = mydsp_faustpower2_f(fConst257);
		fConst261 = ftbl1mydspSIG1[30];
		float fConst262 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst263 = (0.0f - (2.0f * fConst262));
		fConst264 = (798.247253f / fConst0);
		fConst265 = mydsp_faustpower2_f(fConst262);
		fConst266 = ftbl1mydspSIG1[31];
		float fConst267 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst268 = (0.0f - (2.0f * fConst267));
		fConst269 = (816.700989f / fConst0);
		fConst270 = mydsp_faustpower2_f(fConst267);
		fConst271 = ftbl1mydspSIG1[32];
		float fConst272 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst273 = (0.0f - (2.0f * fConst272));
		fConst274 = (831.007812f / fConst0);
		fConst275 = mydsp_faustpower2_f(fConst272);
		fConst276 = ftbl1mydspSIG1[33];
		float fConst277 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst278 = (0.0f - (2.0f * fConst277));
		fConst279 = (838.660706f / fConst0);
		fConst280 = mydsp_faustpower2_f(fConst277);
		fConst281 = ftbl1mydspSIG1[34];
		float fConst282 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst283 = (0.0f - (2.0f * fConst282));
		fConst284 = (908.228149f / fConst0);
		fConst285 = mydsp_faustpower2_f(fConst282);
		fConst286 = ftbl1mydspSIG1[35];
		float fConst287 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst288 = (0.0f - (2.0f * fConst287));
		fConst289 = (938.946655f / fConst0);
		fConst290 = mydsp_faustpower2_f(fConst287);
		fConst291 = ftbl1mydspSIG1[36];
		float fConst292 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst293 = (0.0f - (2.0f * fConst292));
		fConst294 = (955.251526f / fConst0);
		fConst295 = mydsp_faustpower2_f(fConst292);
		fConst296 = ftbl1mydspSIG1[37];
		float fConst297 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst298 = (0.0f - (2.0f * fConst297));
		fConst299 = (962.370361f / fConst0);
		fConst300 = mydsp_faustpower2_f(fConst297);
		fConst301 = ftbl1mydspSIG1[38];
		float fConst302 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst303 = (0.0f - (2.0f * fConst302));
		fConst304 = (977.644775f / fConst0);
		fConst305 = mydsp_faustpower2_f(fConst302);
		fConst306 = ftbl1mydspSIG1[39];
		float fConst307 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst308 = (0.0f - (2.0f * fConst307));
		fConst309 = (993.893127f / fConst0);
		fConst310 = mydsp_faustpower2_f(fConst307);
		fConst311 = ftbl1mydspSIG1[40];
		float fConst312 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst313 = (0.0f - (2.0f * fConst312));
		fConst314 = (1056.23486f / fConst0);
		fConst315 = mydsp_faustpower2_f(fConst312);
		fConst316 = ftbl1mydspSIG1[41];
		float fConst317 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst318 = (0.0f - (2.0f * fConst317));
		fConst319 = (1079.84705f / fConst0);
		fConst320 = mydsp_faustpower2_f(fConst317);
		fConst321 = ftbl1mydspSIG1[42];
		float fConst322 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst323 = (0.0f - (2.0f * fConst322));
		fConst324 = (1096.1897f / fConst0);
		fConst325 = mydsp_faustpower2_f(fConst322);
		fConst326 = ftbl1mydspSIG1[43];
		float fConst327 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst328 = (0.0f - (2.0f * fConst327));
		fConst329 = (1124.29431f / fConst0);
		fConst330 = mydsp_faustpower2_f(fConst327);
		fConst331 = ftbl1mydspSIG1[44];
		float fConst332 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst333 = (0.0f - (2.0f * fConst332));
		fConst334 = (1140.28503f / fConst0);
		fConst335 = mydsp_faustpower2_f(fConst332);
		fConst336 = ftbl1mydspSIG1[45];
		float fConst337 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst338 = (0.0f - (2.0f * fConst337));
		fConst339 = (1164.89001f / fConst0);
		fConst340 = mydsp_faustpower2_f(fConst337);
		fConst341 = ftbl1mydspSIG1[46];
		float fConst342 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst343 = (0.0f - (2.0f * fConst342));
		fConst344 = (1196.12366f / fConst0);
		fConst345 = mydsp_faustpower2_f(fConst342);
		fConst346 = ftbl1mydspSIG1[47];
		float fConst347 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst348 = (0.0f - (2.0f * fConst347));
		fConst349 = (1207.56543f / fConst0);
		fConst350 = mydsp_faustpower2_f(fConst347);
		fConst351 = ftbl1mydspSIG1[48];
		float fConst352 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst353 = (0.0f - (2.0f * fConst352));
		fConst354 = (1228.39417f / fConst0);
		fConst355 = mydsp_faustpower2_f(fConst352);
		fConst356 = ftbl1mydspSIG1[49];
		fConst357 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst358 = (81.0f / fConst0);
		fConst359 = (0.00117647054f * fConst0);
		fConst360 = (0.000294117635f * fConst0);
		fConst361 = ftbl2mydspSIG2[50];
		float fConst362 = std::tan((31415.9258f / fConst0));
		float fConst363 = (1.0f / fConst362);
		fConst364 = (1.0f / (((fConst363 + 1.41421354f) / fConst362) + 1.0f));
		fConst365 = (((fConst363 + -1.41421354f) / fConst362) + 1.0f);
		fConst366 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst362))));
		fConst367 = (1.0f / std::max<float>(1.0f, (0.00159999996f * fConst0)));
		float fConst368 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst369 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst368)));
		fConst370 = mydsp_faustpower2_f(fConst368);
		fConst371 = ftbl2mydspSIG2[51];
		float fConst372 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst373 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst372)));
		fConst374 = mydsp_faustpower2_f(fConst372);
		fConst375 = ftbl2mydspSIG2[52];
		float fConst376 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst377 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst376)));
		fConst378 = mydsp_faustpower2_f(fConst376);
		fConst379 = ftbl2mydspSIG2[53];
		float fConst380 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst381 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst380)));
		fConst382 = mydsp_faustpower2_f(fConst380);
		fConst383 = ftbl2mydspSIG2[54];
		float fConst384 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst385 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst384)));
		fConst386 = mydsp_faustpower2_f(fConst384);
		fConst387 = ftbl2mydspSIG2[55];
		float fConst388 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst389 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst388)));
		fConst390 = mydsp_faustpower2_f(fConst388);
		fConst391 = ftbl2mydspSIG2[56];
		float fConst392 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst393 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst392)));
		fConst394 = mydsp_faustpower2_f(fConst392);
		fConst395 = ftbl2mydspSIG2[57];
		float fConst396 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst397 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst396)));
		fConst398 = mydsp_faustpower2_f(fConst396);
		fConst399 = ftbl2mydspSIG2[58];
		float fConst400 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst401 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst400)));
		fConst402 = mydsp_faustpower2_f(fConst400);
		fConst403 = ftbl2mydspSIG2[59];
		float fConst404 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst405 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst404)));
		fConst406 = mydsp_faustpower2_f(fConst404);
		fConst407 = ftbl2mydspSIG2[60];
		float fConst408 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst409 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst408)));
		fConst410 = mydsp_faustpower2_f(fConst408);
		fConst411 = ftbl2mydspSIG2[61];
		float fConst412 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst413 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst412)));
		fConst414 = mydsp_faustpower2_f(fConst412);
		fConst415 = ftbl2mydspSIG2[62];
		float fConst416 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst417 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst416)));
		fConst418 = mydsp_faustpower2_f(fConst416);
		fConst419 = ftbl2mydspSIG2[63];
		float fConst420 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst421 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst420)));
		fConst422 = mydsp_faustpower2_f(fConst420);
		fConst423 = ftbl2mydspSIG2[64];
		float fConst424 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst425 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst424)));
		fConst426 = mydsp_faustpower2_f(fConst424);
		fConst427 = ftbl2mydspSIG2[65];
		float fConst428 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst429 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst428)));
		fConst430 = mydsp_faustpower2_f(fConst428);
		fConst431 = ftbl2mydspSIG2[66];
		float fConst432 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst433 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst432)));
		fConst434 = mydsp_faustpower2_f(fConst432);
		fConst435 = ftbl2mydspSIG2[67];
		float fConst436 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst437 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst436)));
		fConst438 = mydsp_faustpower2_f(fConst436);
		fConst439 = ftbl2mydspSIG2[68];
		float fConst440 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst441 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst440)));
		fConst442 = mydsp_faustpower2_f(fConst440);
		fConst443 = ftbl2mydspSIG2[69];
		float fConst444 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst445 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst444)));
		fConst446 = mydsp_faustpower2_f(fConst444);
		fConst447 = ftbl2mydspSIG2[70];
		float fConst448 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst449 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst448)));
		fConst450 = mydsp_faustpower2_f(fConst448);
		fConst451 = ftbl2mydspSIG2[71];
		float fConst452 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst453 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst452)));
		fConst454 = mydsp_faustpower2_f(fConst452);
		fConst455 = ftbl2mydspSIG2[72];
		float fConst456 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst457 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst456)));
		fConst458 = mydsp_faustpower2_f(fConst456);
		fConst459 = ftbl2mydspSIG2[73];
		float fConst460 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst461 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst460)));
		fConst462 = mydsp_faustpower2_f(fConst460);
		fConst463 = ftbl2mydspSIG2[74];
		float fConst464 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst465 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst464)));
		fConst466 = mydsp_faustpower2_f(fConst464);
		fConst467 = ftbl2mydspSIG2[75];
		float fConst468 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst469 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst468)));
		fConst470 = mydsp_faustpower2_f(fConst468);
		fConst471 = ftbl2mydspSIG2[76];
		float fConst472 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst473 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst472)));
		fConst474 = mydsp_faustpower2_f(fConst472);
		fConst475 = ftbl2mydspSIG2[77];
		float fConst476 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst477 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst476)));
		fConst478 = mydsp_faustpower2_f(fConst476);
		fConst479 = ftbl2mydspSIG2[78];
		float fConst480 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst481 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst480)));
		fConst482 = mydsp_faustpower2_f(fConst480);
		fConst483 = ftbl2mydspSIG2[79];
		float fConst484 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst485 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst484)));
		fConst486 = mydsp_faustpower2_f(fConst484);
		fConst487 = ftbl2mydspSIG2[80];
		float fConst488 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst489 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst488)));
		fConst490 = mydsp_faustpower2_f(fConst488);
		fConst491 = ftbl2mydspSIG2[81];
		float fConst492 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst493 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst492)));
		fConst494 = mydsp_faustpower2_f(fConst492);
		fConst495 = ftbl2mydspSIG2[82];
		float fConst496 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst497 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst496)));
		fConst498 = mydsp_faustpower2_f(fConst496);
		fConst499 = ftbl2mydspSIG2[83];
		float fConst500 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst501 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst500)));
		fConst502 = mydsp_faustpower2_f(fConst500);
		fConst503 = ftbl2mydspSIG2[84];
		float fConst504 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst505 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst504)));
		fConst506 = mydsp_faustpower2_f(fConst504);
		fConst507 = ftbl2mydspSIG2[85];
		float fConst508 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst509 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst508)));
		fConst510 = mydsp_faustpower2_f(fConst508);
		fConst511 = ftbl2mydspSIG2[86];
		float fConst512 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst513 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst512)));
		fConst514 = mydsp_faustpower2_f(fConst512);
		fConst515 = ftbl2mydspSIG2[87];
		float fConst516 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst517 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst516)));
		fConst518 = mydsp_faustpower2_f(fConst516);
		fConst519 = ftbl2mydspSIG2[88];
		float fConst520 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst521 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst520)));
		fConst522 = mydsp_faustpower2_f(fConst520);
		fConst523 = ftbl2mydspSIG2[89];
		float fConst524 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst525 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst524)));
		fConst526 = mydsp_faustpower2_f(fConst524);
		fConst527 = ftbl2mydspSIG2[90];
		float fConst528 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst529 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst528)));
		fConst530 = mydsp_faustpower2_f(fConst528);
		fConst531 = ftbl2mydspSIG2[91];
		float fConst532 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst533 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst532)));
		fConst534 = mydsp_faustpower2_f(fConst532);
		fConst535 = ftbl2mydspSIG2[92];
		float fConst536 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst537 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst536)));
		fConst538 = mydsp_faustpower2_f(fConst536);
		fConst539 = ftbl2mydspSIG2[93];
		float fConst540 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst541 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst540)));
		fConst542 = mydsp_faustpower2_f(fConst540);
		fConst543 = ftbl2mydspSIG2[94];
		float fConst544 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst545 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst544)));
		fConst546 = mydsp_faustpower2_f(fConst544);
		fConst547 = ftbl2mydspSIG2[95];
		float fConst548 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst549 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst548)));
		fConst550 = mydsp_faustpower2_f(fConst548);
		fConst551 = ftbl2mydspSIG2[96];
		float fConst552 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst553 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst552)));
		fConst554 = mydsp_faustpower2_f(fConst552);
		fConst555 = ftbl2mydspSIG2[97];
		float fConst556 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst557 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst556)));
		fConst558 = mydsp_faustpower2_f(fConst556);
		fConst559 = ftbl2mydspSIG2[98];
		float fConst560 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst561 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst560)));
		fConst562 = mydsp_faustpower2_f(fConst560);
		fConst563 = ftbl2mydspSIG2[99];
		float fConst564 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst565 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst564)));
		fConst566 = mydsp_faustpower2_f(fConst564);
		fConst567 = (2.0f * fConst0);
		fConst568 = (1.0f / std::max<float>(1.0f, fConst567));
		fConst569 = (2.0f / fConst0);
		float fConst570 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst571 = ((0.0f - (6.90775537f * fConst570)) / fConst0);
		float fConst572 = std::exp((0.25f * fConst571));
		float fConst573 = mydsp_faustpower2_f(fConst572);
		float fConst574 = std::cos((12566.3711f / fConst0));
		float fConst575 = (1.0f - (fConst573 * fConst574));
		float fConst576 = (1.0f - fConst573);
		float fConst577 = (fConst575 / fConst576);
		float fConst578 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst575) / mydsp_faustpower2_f(fConst576)) + -1.0f)));
		fConst579 = (fConst577 - fConst578);
		fConst580 = (fConst572 * (fConst578 + (1.0f - fConst577)));
		fConst581 = ((std::exp(fConst571) / fConst572) + -1.0f);
		float fConst582 = (1.0f / std::tan((314.159271f / fConst0)));
		fConst583 = (1.0f / (fConst582 + 1.0f));
		fConst584 = (1.0f - fConst582);
		float fConst585 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst586 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst570 - fConst585))));
		iConst587 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst588 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst585 + -1.0f))));
		float fConst589 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst590 = ((0.0f - (6.90775537f * fConst589)) / fConst0);
		float fConst591 = std::exp((0.25f * fConst590));
		float fConst592 = mydsp_faustpower2_f(fConst591);
		float fConst593 = (1.0f - (fConst592 * fConst574));
		float fConst594 = (1.0f - fConst592);
		float fConst595 = (fConst593 / fConst594);
		float fConst596 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst593) / mydsp_faustpower2_f(fConst594)) + -1.0f)));
		fConst597 = (fConst595 - fConst596);
		fConst598 = (fConst591 * (fConst596 + (1.0f - fConst595)));
		fConst599 = ((std::exp(fConst590) / fConst591) + -1.0f);
		float fConst600 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst601 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst589 - fConst600))));
		iConst602 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst600 + -1.0f))));
		float fConst603 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst604 = ((0.0f - (6.90775537f * fConst603)) / fConst0);
		float fConst605 = std::exp((0.25f * fConst604));
		float fConst606 = mydsp_faustpower2_f(fConst605);
		float fConst607 = (1.0f - (fConst606 * fConst574));
		float fConst608 = (1.0f - fConst606);
		float fConst609 = (fConst607 / fConst608);
		float fConst610 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst607) / mydsp_faustpower2_f(fConst608)) + -1.0f)));
		fConst611 = (fConst609 - fConst610);
		fConst612 = (fConst605 * (fConst610 + (1.0f - fConst609)));
		fConst613 = ((std::exp(fConst604) / fConst605) + -1.0f);
		float fConst614 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst615 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst603 - fConst614))));
		iConst616 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst614 + -1.0f))));
		float fConst617 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst618 = ((0.0f - (6.90775537f * fConst617)) / fConst0);
		float fConst619 = std::exp((0.25f * fConst618));
		float fConst620 = mydsp_faustpower2_f(fConst619);
		float fConst621 = (1.0f - (fConst620 * fConst574));
		float fConst622 = (1.0f - fConst620);
		float fConst623 = (fConst621 / fConst622);
		float fConst624 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst621) / mydsp_faustpower2_f(fConst622)) + -1.0f)));
		fConst625 = (fConst623 - fConst624);
		fConst626 = (fConst619 * (fConst624 + (1.0f - fConst623)));
		fConst627 = ((std::exp(fConst618) / fConst619) + -1.0f);
		float fConst628 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst629 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst617 - fConst628))));
		iConst630 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst628 + -1.0f))));
		float fConst631 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst632 = ((0.0f - (6.90775537f * fConst631)) / fConst0);
		float fConst633 = std::exp((0.25f * fConst632));
		float fConst634 = mydsp_faustpower2_f(fConst633);
		float fConst635 = (1.0f - (fConst634 * fConst574));
		float fConst636 = (1.0f - fConst634);
		float fConst637 = (fConst635 / fConst636);
		float fConst638 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst635) / mydsp_faustpower2_f(fConst636)) + -1.0f)));
		fConst639 = (fConst637 - fConst638);
		fConst640 = (fConst633 * (fConst638 + (1.0f - fConst637)));
		fConst641 = ((std::exp(fConst632) / fConst633) + -1.0f);
		float fConst642 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst643 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst631 - fConst642))));
		iConst644 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst642 + -1.0f))));
		float fConst645 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst646 = ((0.0f - (6.90775537f * fConst645)) / fConst0);
		float fConst647 = std::exp((0.25f * fConst646));
		float fConst648 = mydsp_faustpower2_f(fConst647);
		float fConst649 = (1.0f - (fConst648 * fConst574));
		float fConst650 = (1.0f - fConst648);
		float fConst651 = (fConst649 / fConst650);
		float fConst652 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst649) / mydsp_faustpower2_f(fConst650)) + -1.0f)));
		fConst653 = (fConst651 - fConst652);
		fConst654 = (fConst647 * (fConst652 + (1.0f - fConst651)));
		fConst655 = ((std::exp(fConst646) / fConst647) + -1.0f);
		float fConst656 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst657 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst645 - fConst656))));
		iConst658 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst656 + -1.0f))));
		float fConst659 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst660 = ((0.0f - (6.90775537f * fConst659)) / fConst0);
		float fConst661 = std::exp((0.25f * fConst660));
		float fConst662 = mydsp_faustpower2_f(fConst661);
		float fConst663 = (1.0f - (fConst662 * fConst574));
		float fConst664 = (1.0f - fConst662);
		float fConst665 = (fConst663 / fConst664);
		float fConst666 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst663) / mydsp_faustpower2_f(fConst664)) + -1.0f)));
		fConst667 = (fConst665 - fConst666);
		fConst668 = (fConst661 * (fConst666 + (1.0f - fConst665)));
		fConst669 = ((std::exp(fConst660) / fConst661) + -1.0f);
		float fConst670 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst671 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst659 - fConst670))));
		iConst672 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst670 + -1.0f))));
		float fConst673 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst674 = ((0.0f - (6.90775537f * fConst673)) / fConst0);
		float fConst675 = std::exp((0.25f * fConst674));
		float fConst676 = mydsp_faustpower2_f(fConst675);
		float fConst677 = (1.0f - (fConst676 * fConst574));
		float fConst678 = (1.0f - fConst676);
		float fConst679 = (fConst677 / fConst678);
		float fConst680 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst677) / mydsp_faustpower2_f(fConst678)) + -1.0f)));
		fConst681 = (fConst679 - fConst680);
		fConst682 = (fConst675 * (fConst680 + (1.0f - fConst679)));
		fConst683 = ((std::exp(fConst674) / fConst675) + -1.0f);
		float fConst684 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst685 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst673 - fConst684))));
		iConst686 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst684 + -1.0f))));
		fConst687 = mydsp_faustpower2_f(fConst66);
		fConst688 = (2.0f * fConst687);
		fConst689 = (4.0f * (mydsp_faustpower2_f(fConst0) * std::tan((628.318542f / fConst0))));
		fConst690 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst691 = (1.0f - fConst690);
		fConst692 = (3500.0f * fConst691);
		fConst693 = (0.5f / fConst0);
		fConst694 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst695 = (1.0f - fConst694);
		fConst696 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst697 = (1.0f - fConst696);
		fConst698 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst699 = (1.0f - fConst698);
		fConst700 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst701 = (1.0f - fConst700);
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
			fRec37[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fVec4[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fVec5[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fVec6[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec41[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec40[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec38[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec43[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec42[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec45[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec46[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec47[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec48[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			iRec49[l51] = 0;
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec35[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			iRec50[l53] = 0;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec53[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec52[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec51[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec54[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec34[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec33[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec32[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fVec7[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec8[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fVec9[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			iRec59[l64] = 0;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec58[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec62[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			iRec61[l67] = 0;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec64[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec63[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec60[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec67[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec66[l72] = 0;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec65[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec70[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			iRec69[l75] = 0;
		}
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec68[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec73[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			iRec72[l78] = 0;
		}
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec71[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec76[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec75[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec74[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec77[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec57[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec56[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec55[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			iRec81[l87] = 0;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec85[l88] = 0.0f;
		}
		IOTA = 0;
		for (int l89 = 0; (l89 < 2048); l89 = (l89 + 1)) {
			fVec10[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fVec11[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fVec12[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec13[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec86[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec83[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec87[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec90[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec89[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			iRec91[l98] = 0;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec88[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec92[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec93[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec94[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec95[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec96[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec97[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec98[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec99[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec100[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec101[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 3); l110 = (l110 + 1)) {
			fRec102[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 3); l111 = (l111 + 1)) {
			fRec103[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 3); l112 = (l112 + 1)) {
			fRec104[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 3); l113 = (l113 + 1)) {
			fRec105[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 3); l114 = (l114 + 1)) {
			fRec106[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 3); l115 = (l115 + 1)) {
			fRec107[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 3); l116 = (l116 + 1)) {
			fRec108[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 3); l117 = (l117 + 1)) {
			fRec109[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 3); l118 = (l118 + 1)) {
			fRec110[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 3); l119 = (l119 + 1)) {
			fRec111[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 3); l120 = (l120 + 1)) {
			fRec112[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 3); l121 = (l121 + 1)) {
			fRec113[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 3); l122 = (l122 + 1)) {
			fRec114[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 3); l123 = (l123 + 1)) {
			fRec115[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 3); l124 = (l124 + 1)) {
			fRec116[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 3); l125 = (l125 + 1)) {
			fRec117[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 3); l126 = (l126 + 1)) {
			fRec118[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 3); l127 = (l127 + 1)) {
			fRec119[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 3); l128 = (l128 + 1)) {
			fRec120[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 3); l129 = (l129 + 1)) {
			fRec121[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 3); l130 = (l130 + 1)) {
			fRec122[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 3); l131 = (l131 + 1)) {
			fRec123[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 3); l132 = (l132 + 1)) {
			fRec124[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 3); l133 = (l133 + 1)) {
			fRec125[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 3); l134 = (l134 + 1)) {
			fRec126[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec127[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 3); l136 = (l136 + 1)) {
			fRec128[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 3); l137 = (l137 + 1)) {
			fRec129[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 3); l138 = (l138 + 1)) {
			fRec130[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 3); l139 = (l139 + 1)) {
			fRec131[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 3); l140 = (l140 + 1)) {
			fRec132[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 3); l141 = (l141 + 1)) {
			fRec133[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 3); l142 = (l142 + 1)) {
			fRec134[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 3); l143 = (l143 + 1)) {
			fRec135[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 3); l144 = (l144 + 1)) {
			fRec136[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 3); l145 = (l145 + 1)) {
			fRec137[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 3); l146 = (l146 + 1)) {
			fRec138[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 3); l147 = (l147 + 1)) {
			fRec139[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 3); l148 = (l148 + 1)) {
			fRec140[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fVec14[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 2048); l150 = (l150 + 1)) {
			fVec15[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fRec84[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec143[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec142[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 2); l154 = (l154 + 1)) {
			fRec141[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 3); l155 = (l155 + 1)) {
			fRec144[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 3); l156 = (l156 + 1)) {
			fRec80[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 3); l157 = (l157 + 1)) {
			fRec79[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 3); l158 = (l158 + 1)) {
			fRec78[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec148[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec150[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec154[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec153[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 2); l163 = (l163 + 1)) {
			fRec152[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			fRec151[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec149[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec157[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec156[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec155[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 3); l169 = (l169 + 1)) {
			fRec158[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 3); l170 = (l170 + 1)) {
			fRec147[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 3); l171 = (l171 + 1)) {
			fRec146[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec145[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fVec16[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 2); l174 = (l174 + 1)) {
			fVec17[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 2); l175 = (l175 + 1)) {
			fVec18[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fRec188[l176] = 0.0f;
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec184[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 2); l178 = (l178 + 1)) {
			fRec189[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 4); l179 = (l179 + 1)) {
			fRec191[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 1024); l180 = (l180 + 1)) {
			fRec192[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fVec19[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 3); l182 = (l182 + 1)) {
			fVec20[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 2048); l183 = (l183 + 1)) {
			fRec190[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 2); l184 = (l184 + 1)) {
			fRec180[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fRec176[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 1024); l186 = (l186 + 1)) {
			fRec172[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 4); l187 = (l187 + 1)) {
			fRec170[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 2048); l188 = (l188 + 1)) {
			fRec165[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec163[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 2); l190 = (l190 + 1)) {
			fRec193[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 3); l191 = (l191 + 1)) {
			fRec162[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			iRec194[l192] = 0;
		}
		for (int l193 = 0; (l193 < 2); l193 = (l193 + 1)) {
			iRec195[l193] = 0;
		}
		for (int l194 = 0; (l194 < 2); l194 = (l194 + 1)) {
			fRec198[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec197[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 2); l196 = (l196 + 1)) {
			fRec196[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec199[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec161[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec160[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 3); l200 = (l200 + 1)) {
			fRec159[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec204[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 2); l202 = (l202 + 1)) {
			fVec21[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fVec22[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fVec23[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			iRec205[l205] = 0;
		}
		for (int l206 = 0; (l206 < 3); l206 = (l206 + 1)) {
			fRec203[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 3); l207 = (l207 + 1)) {
			fRec206[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 3); l208 = (l208 + 1)) {
			fRec207[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 3); l209 = (l209 + 1)) {
			fRec208[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 3); l210 = (l210 + 1)) {
			fRec209[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 3); l211 = (l211 + 1)) {
			fRec210[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 3); l212 = (l212 + 1)) {
			fRec211[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 3); l213 = (l213 + 1)) {
			fRec212[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 3); l214 = (l214 + 1)) {
			fRec213[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 3); l215 = (l215 + 1)) {
			fRec214[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec215[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 3); l217 = (l217 + 1)) {
			fRec216[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec217[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 3); l219 = (l219 + 1)) {
			fRec218[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 3); l220 = (l220 + 1)) {
			fRec219[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 3); l221 = (l221 + 1)) {
			fRec220[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 3); l222 = (l222 + 1)) {
			fRec221[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec222[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec223[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec224[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 3); l226 = (l226 + 1)) {
			fRec225[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 3); l227 = (l227 + 1)) {
			fRec226[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 3); l228 = (l228 + 1)) {
			fRec227[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 3); l229 = (l229 + 1)) {
			fRec228[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 3); l230 = (l230 + 1)) {
			fRec229[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 3); l231 = (l231 + 1)) {
			fRec230[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 3); l232 = (l232 + 1)) {
			fRec231[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 3); l233 = (l233 + 1)) {
			fRec232[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fRec233[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 3); l235 = (l235 + 1)) {
			fRec234[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 3); l236 = (l236 + 1)) {
			fRec235[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 3); l237 = (l237 + 1)) {
			fRec236[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 3); l238 = (l238 + 1)) {
			fRec237[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 3); l239 = (l239 + 1)) {
			fRec238[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 3); l240 = (l240 + 1)) {
			fRec239[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 3); l241 = (l241 + 1)) {
			fRec240[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 3); l242 = (l242 + 1)) {
			fRec241[l242] = 0.0f;
		}
		for (int l243 = 0; (l243 < 3); l243 = (l243 + 1)) {
			fRec242[l243] = 0.0f;
		}
		for (int l244 = 0; (l244 < 3); l244 = (l244 + 1)) {
			fRec243[l244] = 0.0f;
		}
		for (int l245 = 0; (l245 < 3); l245 = (l245 + 1)) {
			fRec244[l245] = 0.0f;
		}
		for (int l246 = 0; (l246 < 3); l246 = (l246 + 1)) {
			fRec245[l246] = 0.0f;
		}
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec246[l247] = 0.0f;
		}
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec247[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec248[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec249[l250] = 0.0f;
		}
		for (int l251 = 0; (l251 < 3); l251 = (l251 + 1)) {
			fRec250[l251] = 0.0f;
		}
		for (int l252 = 0; (l252 < 3); l252 = (l252 + 1)) {
			fRec251[l252] = 0.0f;
		}
		for (int l253 = 0; (l253 < 3); l253 = (l253 + 1)) {
			fRec252[l253] = 0.0f;
		}
		for (int l254 = 0; (l254 < 3); l254 = (l254 + 1)) {
			fRec253[l254] = 0.0f;
		}
		for (int l255 = 0; (l255 < 3); l255 = (l255 + 1)) {
			fRec254[l255] = 0.0f;
		}
		for (int l256 = 0; (l256 < 2); l256 = (l256 + 1)) {
			fRec255[l256] = 0.0f;
		}
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fRec256[l257] = 0.0f;
		}
		for (int l258 = 0; (l258 < 2); l258 = (l258 + 1)) {
			fRec257[l258] = 0.0f;
		}
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec258[l259] = 0.0f;
		}
		for (int l260 = 0; (l260 < 2); l260 = (l260 + 1)) {
			fRec259[l260] = 0.0f;
		}
		for (int l261 = 0; (l261 < 2); l261 = (l261 + 1)) {
			fRec260[l261] = 0.0f;
		}
		for (int l262 = 0; (l262 < 2); l262 = (l262 + 1)) {
			fRec263[l262] = 0.0f;
		}
		for (int l263 = 0; (l263 < 2); l263 = (l263 + 1)) {
			fRec262[l263] = 0.0f;
		}
		for (int l264 = 0; (l264 < 2); l264 = (l264 + 1)) {
			fRec261[l264] = 0.0f;
		}
		for (int l265 = 0; (l265 < 3); l265 = (l265 + 1)) {
			fRec264[l265] = 0.0f;
		}
		for (int l266 = 0; (l266 < 3); l266 = (l266 + 1)) {
			fRec202[l266] = 0.0f;
		}
		for (int l267 = 0; (l267 < 3); l267 = (l267 + 1)) {
			fRec201[l267] = 0.0f;
		}
		for (int l268 = 0; (l268 < 3); l268 = (l268 + 1)) {
			fRec200[l268] = 0.0f;
		}
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fRec277[l269] = 0.0f;
		}
		for (int l270 = 0; (l270 < 2); l270 = (l270 + 1)) {
			fRec276[l270] = 0.0f;
		}
		for (int l271 = 0; (l271 < 32768); l271 = (l271 + 1)) {
			fVec24[l271] = 0.0f;
		}
		for (int l272 = 0; (l272 < 2048); l272 = (l272 + 1)) {
			fVec25[l272] = 0.0f;
		}
		for (int l273 = 0; (l273 < 2048); l273 = (l273 + 1)) {
			fVec26[l273] = 0.0f;
		}
		for (int l274 = 0; (l274 < 2); l274 = (l274 + 1)) {
			fRec274[l274] = 0.0f;
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec281[l275] = 0.0f;
		}
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec280[l276] = 0.0f;
		}
		for (int l277 = 0; (l277 < 32768); l277 = (l277 + 1)) {
			fVec27[l277] = 0.0f;
		}
		for (int l278 = 0; (l278 < 4096); l278 = (l278 + 1)) {
			fVec28[l278] = 0.0f;
		}
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec278[l279] = 0.0f;
		}
		for (int l280 = 0; (l280 < 2); l280 = (l280 + 1)) {
			fRec285[l280] = 0.0f;
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec284[l281] = 0.0f;
		}
		for (int l282 = 0; (l282 < 16384); l282 = (l282 + 1)) {
			fVec29[l282] = 0.0f;
		}
		for (int l283 = 0; (l283 < 4096); l283 = (l283 + 1)) {
			fVec30[l283] = 0.0f;
		}
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec282[l284] = 0.0f;
		}
		for (int l285 = 0; (l285 < 2); l285 = (l285 + 1)) {
			fRec289[l285] = 0.0f;
		}
		for (int l286 = 0; (l286 < 2); l286 = (l286 + 1)) {
			fRec288[l286] = 0.0f;
		}
		for (int l287 = 0; (l287 < 32768); l287 = (l287 + 1)) {
			fVec31[l287] = 0.0f;
		}
		for (int l288 = 0; (l288 < 4096); l288 = (l288 + 1)) {
			fVec32[l288] = 0.0f;
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec286[l289] = 0.0f;
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec293[l290] = 0.0f;
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec292[l291] = 0.0f;
		}
		for (int l292 = 0; (l292 < 16384); l292 = (l292 + 1)) {
			fVec33[l292] = 0.0f;
		}
		for (int l293 = 0; (l293 < 2048); l293 = (l293 + 1)) {
			fVec34[l293] = 0.0f;
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec290[l294] = 0.0f;
		}
		for (int l295 = 0; (l295 < 2); l295 = (l295 + 1)) {
			fRec297[l295] = 0.0f;
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec296[l296] = 0.0f;
		}
		for (int l297 = 0; (l297 < 16384); l297 = (l297 + 1)) {
			fVec35[l297] = 0.0f;
		}
		for (int l298 = 0; (l298 < 4096); l298 = (l298 + 1)) {
			fVec36[l298] = 0.0f;
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec294[l299] = 0.0f;
		}
		for (int l300 = 0; (l300 < 2); l300 = (l300 + 1)) {
			fRec301[l300] = 0.0f;
		}
		for (int l301 = 0; (l301 < 2); l301 = (l301 + 1)) {
			fRec300[l301] = 0.0f;
		}
		for (int l302 = 0; (l302 < 16384); l302 = (l302 + 1)) {
			fVec37[l302] = 0.0f;
		}
		for (int l303 = 0; (l303 < 4096); l303 = (l303 + 1)) {
			fVec38[l303] = 0.0f;
		}
		for (int l304 = 0; (l304 < 2); l304 = (l304 + 1)) {
			fRec298[l304] = 0.0f;
		}
		for (int l305 = 0; (l305 < 2); l305 = (l305 + 1)) {
			fRec305[l305] = 0.0f;
		}
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec304[l306] = 0.0f;
		}
		for (int l307 = 0; (l307 < 16384); l307 = (l307 + 1)) {
			fVec39[l307] = 0.0f;
		}
		for (int l308 = 0; (l308 < 2048); l308 = (l308 + 1)) {
			fVec40[l308] = 0.0f;
		}
		for (int l309 = 0; (l309 < 2); l309 = (l309 + 1)) {
			fRec302[l309] = 0.0f;
		}
		for (int l310 = 0; (l310 < 3); l310 = (l310 + 1)) {
			fRec266[l310] = 0.0f;
		}
		for (int l311 = 0; (l311 < 3); l311 = (l311 + 1)) {
			fRec267[l311] = 0.0f;
		}
		for (int l312 = 0; (l312 < 3); l312 = (l312 + 1)) {
			fRec268[l312] = 0.0f;
		}
		for (int l313 = 0; (l313 < 3); l313 = (l313 + 1)) {
			fRec269[l313] = 0.0f;
		}
		for (int l314 = 0; (l314 < 3); l314 = (l314 + 1)) {
			fRec270[l314] = 0.0f;
		}
		for (int l315 = 0; (l315 < 3); l315 = (l315 + 1)) {
			fRec271[l315] = 0.0f;
		}
		for (int l316 = 0; (l316 < 3); l316 = (l316 + 1)) {
			fRec272[l316] = 0.0f;
		}
		for (int l317 = 0; (l317 < 3); l317 = (l317 + 1)) {
			fRec273[l317] = 0.0f;
		}
		for (int l318 = 0; (l318 < 2); l318 = (l318 + 1)) {
			fRec306[l318] = 0.0f;
		}
		for (int l319 = 0; (l319 < 3); l319 = (l319 + 1)) {
			fRec265[l319] = 0.0f;
		}
		for (int l320 = 0; (l320 < 2); l320 = (l320 + 1)) {
			fRec309[l320] = 0.0f;
		}
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec308[l321] = 0.0f;
		}
		for (int l322 = 0; (l322 < 2); l322 = (l322 + 1)) {
			fRec307[l322] = 0.0f;
		}
		for (int l323 = 0; (l323 < 2); l323 = (l323 + 1)) {
			fRec312[l323] = 0.0f;
		}
		for (int l324 = 0; (l324 < 2); l324 = (l324 + 1)) {
			fRec311[l324] = 0.0f;
		}
		for (int l325 = 0; (l325 < 2); l325 = (l325 + 1)) {
			fRec310[l325] = 0.0f;
		}
		for (int l326 = 0; (l326 < 2); l326 = (l326 + 1)) {
			fRec315[l326] = 0.0f;
		}
		for (int l327 = 0; (l327 < 2); l327 = (l327 + 1)) {
			fRec314[l327] = 0.0f;
		}
		for (int l328 = 0; (l328 < 2); l328 = (l328 + 1)) {
			fRec313[l328] = 0.0f;
		}
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			iRec320[l329] = 0;
		}
		for (int l330 = 0; (l330 < 3); l330 = (l330 + 1)) {
			fRec319[l330] = 0.0f;
		}
		for (int l331 = 0; (l331 < 2); l331 = (l331 + 1)) {
			iRec322[l331] = 0;
		}
		for (int l332 = 0; (l332 < 3); l332 = (l332 + 1)) {
			fRec321[l332] = 0.0f;
		}
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			iRec324[l333] = 0;
		}
		for (int l334 = 0; (l334 < 3); l334 = (l334 + 1)) {
			fRec323[l334] = 0.0f;
		}
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			iRec326[l335] = 0;
		}
		for (int l336 = 0; (l336 < 3); l336 = (l336 + 1)) {
			fRec325[l336] = 0.0f;
		}
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec329[l337] = 0.0f;
		}
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec328[l338] = 0.0f;
		}
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fRec327[l339] = 0.0f;
		}
		for (int l340 = 0; (l340 < 3); l340 = (l340 + 1)) {
			fRec330[l340] = 0.0f;
		}
		for (int l341 = 0; (l341 < 3); l341 = (l341 + 1)) {
			fRec318[l341] = 0.0f;
		}
		for (int l342 = 0; (l342 < 3); l342 = (l342 + 1)) {
			fRec317[l342] = 0.0f;
		}
		for (int l343 = 0; (l343 < 3); l343 = (l343 + 1)) {
			fRec316[l343] = 0.0f;
		}
		for (int l344 = 0; (l344 < 3); l344 = (l344 + 1)) {
			fRec331[l344] = 0.0f;
		}
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fRec334[l345] = 0.0f;
		}
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec333[l346] = 0.0f;
		}
		for (int l347 = 0; (l347 < 2); l347 = (l347 + 1)) {
			fRec332[l347] = 0.0f;
		}
		for (int l348 = 0; (l348 < 2); l348 = (l348 + 1)) {
			fRec337[l348] = 0.0f;
		}
		for (int l349 = 0; (l349 < 2); l349 = (l349 + 1)) {
			fRec336[l349] = 0.0f;
		}
		for (int l350 = 0; (l350 < 2); l350 = (l350 + 1)) {
			fRec335[l350] = 0.0f;
		}
		for (int l351 = 0; (l351 < 2); l351 = (l351 + 1)) {
			fRec340[l351] = 0.0f;
		}
		for (int l352 = 0; (l352 < 2); l352 = (l352 + 1)) {
			fRec339[l352] = 0.0f;
		}
		for (int l353 = 0; (l353 < 2); l353 = (l353 + 1)) {
			fRec338[l353] = 0.0f;
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
		ui_interface->addHorizontalSlider("Control 0", &fHslider23, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
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
		ui_interface->addHorizontalSlider("Control 0", &fHslider25, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
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
		float fSlow89 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider23))) + -0.5f)));
		float fSlow90 = (2.0f * fSlow89);
		float fSlow91 = float(fHslider24);
		float fSlow92 = float((fSlow91 > 0.0f));
		float fSlow93 = (0.0f - fSlow91);
		float fSlow94 = float(fHslider25);
		float fSlow95 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider26)));
		float fSlow96 = (0.5f * fSlow95);
		float fSlow97 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider27)));
		float fSlow98 = (fSlow97 + 5.0f);
		int iSlow99 = (fSlow98 < 23.0f);
		int iSlow100 = (fSlow98 < 24.0f);
		float fSlow101 = (fSlow97 + -18.0f);
		int iSlow102 = (fSlow98 < 22.0f);
		float fSlow103 = (fSlow97 + -17.0f);
		int iSlow104 = (fSlow98 < 21.0f);
		float fSlow105 = (fSlow97 + -16.0f);
		int iSlow106 = (fSlow98 < 20.0f);
		float fSlow107 = (fSlow97 + -15.0f);
		int iSlow108 = (fSlow98 < 19.0f);
		float fSlow109 = (fSlow97 + -14.0f);
		int iSlow110 = (fSlow98 < 18.0f);
		float fSlow111 = (fSlow97 + -13.0f);
		int iSlow112 = (fSlow98 < 17.0f);
		float fSlow113 = (fSlow97 + -12.0f);
		int iSlow114 = (fSlow98 < 16.0f);
		float fSlow115 = (fSlow97 + -11.0f);
		float fSlow116 = (80.0f * fSlow115);
		int iSlow117 = (fSlow98 < 15.0f);
		float fSlow118 = (fSlow97 + -10.0f);
		int iSlow119 = (fSlow98 < 14.0f);
		float fSlow120 = (fSlow97 + -9.0f);
		int iSlow121 = (fSlow98 < 13.0f);
		float fSlow122 = (fSlow97 + -8.0f);
		int iSlow123 = (fSlow98 < 12.0f);
		float fSlow124 = (fSlow97 + -7.0f);
		int iSlow125 = (fSlow98 < 11.0f);
		float fSlow126 = (fSlow97 + -6.0f);
		int iSlow127 = (fSlow98 < 10.0f);
		float fSlow128 = (fSlow97 + -5.0f);
		int iSlow129 = (fSlow98 < 9.0f);
		float fSlow130 = (fSlow97 + -4.0f);
		int iSlow131 = (fSlow98 < 8.0f);
		float fSlow132 = (fSlow97 + -3.0f);
		float fSlow133 = (50.0f * fSlow132);
		int iSlow134 = (fSlow98 < 7.0f);
		float fSlow135 = (fSlow97 + -2.0f);
		int iSlow136 = (fSlow98 < 6.0f);
		float fSlow137 = (fSlow97 + -1.0f);
		int iSlow138 = (fSlow98 < 5.0f);
		int iSlow139 = (fSlow98 < 4.0f);
		float fSlow140 = (fSlow97 + 1.0f);
		int iSlow141 = (fSlow98 < 3.0f);
		float fSlow142 = (fSlow97 + 2.0f);
		int iSlow143 = (fSlow98 < 2.0f);
		float fSlow144 = (fSlow97 + 3.0f);
		int iSlow145 = (fSlow98 < 1.0f);
		float fSlow146 = (fSlow97 + 4.0f);
		int iSlow147 = (fSlow98 < 0.0f);
		float fSlow148 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 800.0f : (iSlow145 ? (800.0f - (400.0f * fSlow98)) : 400.0f)) : (iSlow143 ? (400.0f - (50.0f * fSlow146)) : 350.0f)) : (iSlow141 ? ((100.0f * fSlow144) + 350.0f) : 450.0f)) : (iSlow139 ? (450.0f - (125.0f * fSlow142)) : 325.0f)) : (iSlow138 ? ((275.0f * fSlow140) + 325.0f) : 600.0f)) : (iSlow136 ? (600.0f - (200.0f * fSlow97)) : 400.0f)) : (iSlow134 ? (400.0f - (150.0f * fSlow137)) : 250.0f)) : (iSlow131 ? ((150.0f * fSlow135) + 250.0f) : 400.0f)) : (iSlow129 ? (400.0f - fSlow133) : 350.0f)) : (iSlow127 ? ((310.0f * fSlow130) + 350.0f) : 660.0f)) : (iSlow125 ? (660.0f - (220.0f * fSlow128)) : 440.0f)) : (iSlow123 ? (440.0f - (170.0f * fSlow126)) : 270.0f)) : (iSlow121 ? ((160.0f * fSlow124) + 270.0f) : 430.0f)) : (iSlow119 ? (430.0f - (60.0f * fSlow122)) : 370.0f)) : (iSlow117 ? ((430.0f * fSlow120) + 370.0f) : 800.0f)) : (iSlow114 ? (800.0f - (450.0f * fSlow118)) : 350.0f)) : (iSlow112 ? (350.0f - fSlow116) : 270.0f)) : (iSlow110 ? ((180.0f * fSlow113) + 270.0f) : 450.0f)) : (iSlow108 ? (450.0f - (125.0f * fSlow111)) : 325.0f)) : (iSlow106 ? (325.0f * (fSlow109 + 1.0f)) : 650.0f)) : (iSlow104 ? (650.0f - (250.0f * fSlow107)) : 400.0f)) : (iSlow102 ? (400.0f - (110.0f * fSlow105)) : 290.0f)) : (iSlow99 ? ((110.0f * fSlow103) + 290.0f) : 400.0f)) : (iSlow100 ? (400.0f - (50.0f * fSlow101)) : 350.0f));
		float fSlow149 = (20.0f * fSlow107);
		float fSlow150 = (20.0f * fSlow118);
		float fSlow151 = (10.0f * fSlow128);
		float fSlow152 = (10.0f * fSlow140);
		float fSlow153 = (20.0f * fSlow142);
		float fSlow154 = (20.0f * fSlow144);
		float fSlow155 = (20.0f * fSlow98);
		float fSlow156 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 80.0f : (iSlow145 ? (80.0f - fSlow155) : 60.0f)) : (iSlow143 ? (60.0f - (10.0f * fSlow146)) : 50.0f)) : (iSlow141 ? (fSlow154 + 50.0f) : 70.0f)) : (iSlow139 ? (70.0f - fSlow153) : 50.0f)) : (iSlow138 ? (fSlow152 + 50.0f) : 60.0f)) : (iSlow136 ? (60.0f - (20.0f * fSlow97)) : 40.0f)) : (iSlow134 ? ((20.0f * fSlow137) + 40.0f) : 60.0f)) : (iSlow131 ? (60.0f - (20.0f * fSlow135)) : 40.0f)) : 40.0f) : (iSlow127 ? (40.0f * (fSlow130 + 1.0f)) : 80.0f)) : (iSlow125 ? (80.0f - fSlow151) : 70.0f)) : (iSlow123 ? (70.0f - (30.0f * fSlow126)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow117 ? (40.0f * (fSlow120 + 1.0f)) : 80.0f)) : (iSlow114 ? (80.0f - fSlow150) : 60.0f)) : 60.0f) : (iSlow110 ? (60.0f - (20.0f * fSlow113)) : 40.0f)) : (iSlow108 ? ((10.0f * fSlow111) + 40.0f) : 50.0f)) : 50.0f) : (iSlow104 ? (fSlow149 + 50.0f) : 70.0f)) : (iSlow102 ? (70.0f - (30.0f * fSlow105)) : 40.0f)) : (iSlow99 ? ((30.0f * fSlow103) + 40.0f) : 70.0f)) : (iSlow100 ? (70.0f - (30.0f * fSlow101)) : 40.0f));
		float fSlow157 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 1150.0f : (iSlow145 ? ((450.0f * fSlow98) + 1150.0f) : 1600.0f)) : (iSlow143 ? ((100.0f * fSlow146) + 1600.0f) : 1700.0f)) : (iSlow141 ? (1700.0f - (900.0f * fSlow144)) : 800.0f)) : (iSlow139 ? (800.0f - (100.0f * fSlow142)) : 700.0f)) : (iSlow138 ? ((340.0f * fSlow140) + 700.0f) : 1040.0f)) : (iSlow136 ? ((580.0f * fSlow97) + 1040.0f) : 1620.0f)) : (iSlow134 ? ((130.0f * fSlow137) + 1620.0f) : 1750.0f)) : (iSlow131 ? (1750.0f - (1000.0f * fSlow135)) : 750.0f)) : (iSlow129 ? (750.0f - (150.0f * fSlow132)) : 600.0f)) : (iSlow127 ? ((520.0f * fSlow130) + 600.0f) : 1120.0f)) : (iSlow125 ? ((680.0f * fSlow128) + 1120.0f) : 1800.0f)) : (iSlow123 ? ((50.0f * fSlow126) + 1800.0f) : 1850.0f)) : (iSlow121 ? (1850.0f - (1030.0f * fSlow124)) : 820.0f)) : (iSlow119 ? (820.0f - (190.0f * fSlow122)) : 630.0f)) : (iSlow117 ? ((520.0f * fSlow120) + 630.0f) : 1150.0f)) : (iSlow114 ? ((850.0f * fSlow118) + 1150.0f) : 2000.0f)) : (iSlow112 ? ((140.0f * fSlow115) + 2000.0f) : 2140.0f)) : (iSlow110 ? (2140.0f - (1340.0f * fSlow113)) : 800.0f)) : (iSlow108 ? (800.0f - (100.0f * fSlow111)) : 700.0f)) : (iSlow106 ? ((380.0f * fSlow109) + 700.0f) : 1080.0f)) : (iSlow104 ? ((620.0f * fSlow107) + 1080.0f) : 1700.0f)) : (iSlow102 ? ((170.0f * fSlow105) + 1700.0f) : 1870.0f)) : (iSlow99 ? (1870.0f - (1070.0f * fSlow103)) : 800.0f)) : (iSlow100 ? (800.0f - (200.0f * fSlow101)) : 600.0f));
		int iSlow158 = (fSlow157 >= 1300.0f);
		float fSlow159 = (fSlow157 + -1300.0f);
		float fSlow160 = (10.0f * fSlow103);
		float fSlow161 = (10.0f * fSlow107);
		float fSlow162 = (10.0f * fSlow130);
		float fSlow163 = (10.0f * fSlow97);
		float fSlow164 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 90.0f : (iSlow145 ? (90.0f - (10.0f * fSlow98)) : 80.0f)) : (iSlow143 ? ((20.0f * fSlow146) + 80.0f) : 100.0f)) : (iSlow141 ? (100.0f - fSlow154) : 80.0f)) : (iSlow139 ? (80.0f - fSlow153) : 60.0f)) : (iSlow138 ? (fSlow152 + 60.0f) : 70.0f)) : (iSlow136 ? (fSlow163 + 70.0f) : 80.0f)) : (iSlow134 ? ((10.0f * fSlow137) + 80.0f) : 90.0f)) : (iSlow131 ? (90.0f - (10.0f * fSlow135)) : 80.0f)) : 80.0f) : (iSlow127 ? (fSlow162 + 80.0f) : 90.0f)) : (iSlow125 ? (90.0f - fSlow151) : 80.0f)) : (iSlow123 ? ((10.0f * fSlow126) + 80.0f) : 90.0f)) : (iSlow121 ? (90.0f - (10.0f * fSlow124)) : 80.0f)) : (iSlow119 ? (80.0f - (20.0f * fSlow122)) : 60.0f)) : (iSlow117 ? ((30.0f * fSlow120) + 60.0f) : 90.0f)) : (iSlow114 ? ((10.0f * fSlow118) + 90.0f) : 100.0f)) : (iSlow112 ? (100.0f - (10.0f * fSlow115)) : 90.0f)) : (iSlow110 ? (90.0f - (10.0f * fSlow113)) : 80.0f)) : (iSlow108 ? (80.0f - (20.0f * fSlow111)) : 60.0f)) : (iSlow106 ? ((30.0f * fSlow109) + 60.0f) : 90.0f)) : (iSlow104 ? (90.0f - fSlow161) : 80.0f)) : (iSlow102 ? ((10.0f * fSlow105) + 80.0f) : 90.0f)) : (iSlow99 ? (90.0f - fSlow160) : 80.0f)) : (iSlow100 ? (80.0f - (20.0f * fSlow101)) : 60.0f));
		float fSlow165 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 0.630957007f : (iSlow145 ? (0.630957007f - (0.567861021f * fSlow98)) : 0.0630960017f)) : (iSlow143 ? ((0.0369039997f * fSlow146) + 0.0630960017f) : 0.100000001f)) : (iSlow141 ? ((0.254812986f * fSlow144) + 0.100000001f) : 0.35481301f)) : (iSlow139 ? (0.35481301f - (0.103624001f * fSlow142)) : 0.251188993f)) : (iSlow138 ? ((0.195494995f * fSlow140) + 0.251188993f) : 0.446684003f)) : (iSlow136 ? (0.446684003f - (0.195494995f * fSlow97)) : 0.251188993f)) : (iSlow134 ? (0.251188993f - (0.219566002f * fSlow137)) : 0.0316229984f)) : (iSlow131 ? ((0.250214994f * fSlow135) + 0.0316229984f) : 0.281838f)) : (iSlow129 ? (0.281838f - (0.181838006f * fSlow132)) : 0.100000001f)) : (iSlow127 ? ((0.401187003f * fSlow130) + 0.100000001f) : 0.501187027f)) : (iSlow125 ? (0.501187027f - (0.301661015f * fSlow128)) : 0.199525997f)) : (iSlow123 ? (0.199525997f - (0.136429995f * fSlow126)) : 0.0630960017f)) : (iSlow121 ? ((0.253131986f * fSlow124) + 0.0630960017f) : 0.316228002f)) : (iSlow119 ? (0.316228002f - (0.216227993f * fSlow122)) : 0.100000001f)) : (iSlow117 ? ((0.401187003f * fSlow120) + 0.100000001f) : 0.501187027f)) : (iSlow114 ? (0.501187027f - (0.401187003f * fSlow118)) : 0.100000001f)) : (iSlow112 ? ((0.151188999f * fSlow115) + 0.100000001f) : 0.251188993f)) : (iSlow110 ? ((0.0306490008f * fSlow113) + 0.251188993f) : 0.281838f)) : (iSlow108 ? (0.281838f - (0.123349003f * fSlow111)) : 0.158489004f)) : (iSlow106 ? ((0.342698008f * fSlow109) + 0.158489004f) : 0.501187027f)) : (iSlow104 ? (0.501187027f - (0.301661015f * fSlow107)) : 0.199525997f)) : (iSlow102 ? (0.199525997f - (0.0216979999f * fSlow105)) : 0.177827999f)) : (iSlow99 ? ((0.138400003f * fSlow103) + 0.177827999f) : 0.316228002f)) : (iSlow100 ? (0.316228002f - (0.216227993f * fSlow101)) : 0.100000001f));
		float fSlow166 = (100.0f * fSlow101);
		float fSlow167 = (50.0f * fSlow109);
		float fSlow168 = (50.0f * fSlow128);
		float fSlow169 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 2800.0f : (iSlow145 ? (2800.0f - (100.0f * fSlow98)) : 2700.0f)) : 2700.0f) : (iSlow141 ? ((130.0f * fSlow144) + 2700.0f) : 2830.0f)) : (iSlow139 ? (2830.0f - (300.0f * fSlow142)) : 2530.0f)) : (iSlow138 ? (2530.0f - (280.0f * fSlow140)) : 2250.0f)) : (iSlow136 ? ((150.0f * fSlow97) + 2250.0f) : 2400.0f)) : (iSlow134 ? ((200.0f * fSlow137) + 2400.0f) : 2600.0f)) : (iSlow131 ? (2600.0f - (200.0f * fSlow135)) : 2400.0f)) : 2400.0f) : (iSlow127 ? ((350.0f * fSlow130) + 2400.0f) : 2750.0f)) : (iSlow125 ? (2750.0f - fSlow168) : 2700.0f)) : (iSlow123 ? ((200.0f * fSlow126) + 2700.0f) : 2900.0f)) : (iSlow121 ? (2900.0f - (200.0f * fSlow124)) : 2700.0f)) : (iSlow119 ? ((50.0f * fSlow122) + 2700.0f) : 2750.0f)) : (iSlow117 ? ((150.0f * fSlow120) + 2750.0f) : 2900.0f)) : (iSlow114 ? (2900.0f - (100.0f * fSlow118)) : 2800.0f)) : (iSlow112 ? ((150.0f * fSlow115) + 2800.0f) : 2950.0f)) : (iSlow110 ? (2950.0f - (120.0f * fSlow113)) : 2830.0f)) : (iSlow108 ? (2830.0f - (130.0f * fSlow111)) : 2700.0f)) : (iSlow106 ? (2700.0f - fSlow167) : 2650.0f)) : (iSlow104 ? (2650.0f - (50.0f * fSlow107)) : 2600.0f)) : (iSlow102 ? ((200.0f * fSlow105) + 2600.0f) : 2800.0f)) : (iSlow99 ? (2800.0f - (200.0f * fSlow103)) : 2600.0f)) : (iSlow100 ? (fSlow166 + 2600.0f) : 2700.0f));
		float fSlow170 = std::tan((fConst1 * fSlow169));
		float fSlow171 = (1.0f / fSlow170);
		float fSlow172 = (20.0f * fSlow120);
		float fSlow173 = (20.0f * fSlow128);
		float fSlow174 = (20.0f * fSlow130);
		float fSlow175 = (60.0f * fSlow140);
		float fSlow176 = ((iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? 120.0f : (iSlow141 ? (120.0f - fSlow154) : 100.0f)) : (iSlow139 ? ((70.0f * fSlow142) + 100.0f) : 170.0f)) : (iSlow138 ? (170.0f - fSlow175) : 110.0f)) : (iSlow136 ? (110.0f - fSlow163) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow127 ? (fSlow174 + 100.0f) : 120.0f)) : (iSlow125 ? (120.0f - fSlow173) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow117 ? (fSlow172 + 100.0f) : 120.0f)) : 120.0f) : (iSlow112 ? (120.0f - (20.0f * fSlow115)) : 100.0f)) : 100.0f) : (iSlow108 ? ((70.0f * fSlow111) + 100.0f) : 170.0f)) : (iSlow106 ? (170.0f - fSlow167) : 120.0f)) : (iSlow104 ? (120.0f - fSlow149) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow169);
		float fSlow177 = (((fSlow171 - fSlow176) / fSlow170) + 1.0f);
		float fSlow178 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow170)));
		float fSlow179 = (((fSlow171 + fSlow176) / fSlow170) + 1.0f);
		float fSlow180 = (iSlow110 ? ((0.0293140002f * fSlow113) + 0.0501190014f) : 0.0794330016f);
		float fSlow181 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 0.100000001f : (iSlow145 ? (0.100000001f - (0.0683770031f * fSlow98)) : 0.0316229984f)) : 0.0316229984f) : (iSlow141 ? ((0.126865998f * fSlow144) + 0.0316229984f) : 0.158489004f)) : (iSlow139 ? (0.158489004f - (0.126865998f * fSlow142)) : 0.0316229984f)) : (iSlow138 ? ((0.323190004f * fSlow140) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow134 ? (0.35481301f - (0.196324006f * fSlow137)) : 0.158489004f)) : (iSlow131 ? (0.158489004f - (0.0693639964f * fSlow135)) : 0.0891249999f)) : (iSlow129 ? (0.0891249999f - (0.0640060008f * fSlow132)) : 0.0251189992f)) : (iSlow127 ? ((0.0456760004f * fSlow130) + 0.0251189992f) : 0.0707949996f)) : (iSlow125 ? ((0.0550980009f * fSlow128) + 0.0707949996f) : 0.125892997f)) : (iSlow123 ? (0.125892997f - (0.0627970025f * fSlow126)) : 0.0630960017f)) : (iSlow121 ? (0.0630960017f - (0.0129770003f * fSlow124)) : 0.0501190014f)) : (iSlow119 ? ((0.020676f * fSlow122) + 0.0501190014f) : 0.0707949996f)) : (iSlow117 ? (0.0707949996f - (0.0456760004f * fSlow120)) : 0.0251189992f)) : (iSlow114 ? ((0.152709007f * fSlow118) + 0.0251189992f) : 0.177827999f)) : (iSlow112 ? (0.177827999f - (0.127709001f * fSlow115)) : 0.0501190014f)) : fSlow180) : (iSlow108 ? (0.0794330016f - (0.0616500005f * fSlow111)) : 0.0177829992f)) : (iSlow106 ? ((0.428900987f * fSlow109) + 0.0177829992f) : 0.446684003f)) : (iSlow104 ? (0.446684003f - (0.195494995f * fSlow107)) : 0.251188993f)) : (iSlow102 ? (0.251188993f - (0.125295997f * fSlow105)) : 0.125892997f)) : (iSlow99 ? ((0.125295997f * fSlow103) + 0.125892997f) : 0.251188993f)) : (iSlow100 ? (0.251188993f - (0.109935001f * fSlow101)) : 0.141253993f));
		float fSlow182 = (fSlow170 * fSlow179);
		float fSlow183 = (350.0f * fSlow97);
		float fSlow184 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 3500.0f : (iSlow145 ? (3500.0f - (200.0f * fSlow98)) : 3300.0f)) : (iSlow143 ? ((400.0f * fSlow146) + 3300.0f) : 3700.0f)) : (iSlow141 ? (3700.0f - (200.0f * fSlow144)) : 3500.0f)) : 3500.0f) : (iSlow138 ? (3500.0f - (1050.0f * fSlow140)) : 2450.0f)) : (iSlow136 ? (fSlow183 + 2450.0f) : 2800.0f)) : (iSlow134 ? ((250.0f * fSlow137) + 2800.0f) : 3050.0f)) : (iSlow131 ? (3050.0f - (450.0f * fSlow135)) : 2600.0f)) : (iSlow129 ? ((75.0f * fSlow132) + 2600.0f) : 2675.0f)) : (iSlow127 ? ((325.0f * fSlow130) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow123 ? ((350.0f * fSlow126) + 3000.0f) : 3350.0f)) : (iSlow121 ? (3350.0f - (350.0f * fSlow124)) : 3000.0f)) : 3000.0f) : (iSlow117 ? ((900.0f * fSlow120) + 3000.0f) : 3900.0f)) : (iSlow114 ? (3900.0f - (300.0f * fSlow118)) : 3600.0f)) : (iSlow112 ? ((300.0f * fSlow115) + 3600.0f) : 3900.0f)) : (iSlow110 ? (3900.0f - (100.0f * fSlow113)) : 3800.0f)) : 3800.0f) : (iSlow106 ? (3800.0f - (900.0f * fSlow109)) : 2900.0f)) : (iSlow104 ? ((300.0f * fSlow107) + 2900.0f) : 3200.0f)) : (iSlow102 ? ((50.0f * fSlow105) + 3200.0f) : 3250.0f)) : (iSlow99 ? (3250.0f - (450.0f * fSlow103)) : 2800.0f)) : (iSlow100 ? (fSlow166 + 2800.0f) : 2900.0f));
		float fSlow185 = std::tan((fConst1 * fSlow184));
		float fSlow186 = (1.0f / fSlow185);
		float fSlow187 = ((iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 130.0f : (iSlow145 ? (fSlow155 + 130.0f) : 150.0f)) : 150.0f) : (iSlow141 ? (150.0f - fSlow154) : 130.0f)) : (iSlow139 ? ((50.0f * fSlow142) + 130.0f) : 180.0f)) : (iSlow138 ? (180.0f - fSlow175) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow127 ? (fSlow162 + 120.0f) : 130.0f)) : (iSlow125 ? (130.0f - fSlow151) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow117 ? ((10.0f * fSlow120) + 120.0f) : 130.0f)) : (iSlow114 ? (fSlow150 + 130.0f) : 150.0f)) : (iSlow112 ? (150.0f - (30.0f * fSlow115)) : 120.0f)) : 120.0f) : (iSlow108 ? ((60.0f * fSlow111) + 120.0f) : 180.0f)) : (iSlow106 ? (180.0f - fSlow167) : 130.0f)) : (iSlow104 ? (130.0f - fSlow161) : 120.0f)) : 120.0f) : (iSlow99 ? (fSlow160 + 120.0f) : 130.0f)) : (iSlow100 ? (130.0f - (10.0f * fSlow101)) : 120.0f)) / fSlow184);
		float fSlow188 = (((fSlow186 - fSlow187) / fSlow185) + 1.0f);
		float fSlow189 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow185)));
		float fSlow190 = (((fSlow186 + fSlow187) / fSlow185) + 1.0f);
		float fSlow191 = (iSlow123 ? (0.100000001f - (0.0841509998f * fSlow126)) : 0.0158489998f);
		float fSlow192 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 0.0158489998f : (iSlow145 ? ((0.00193400006f * fSlow98) + 0.0158489998f) : 0.0177829992f)) : (iSlow143 ? (0.0177829992f - (0.00193400006f * fSlow146)) : 0.0158489998f)) : (iSlow141 ? ((0.0239620004f * fSlow144) + 0.0158489998f) : 0.0398110002f)) : (iSlow139 ? (0.0398110002f - (0.0298110005f * fSlow142)) : 0.00999999978f)) : (iSlow138 ? ((0.344812989f * fSlow140) + 0.00999999978f) : 0.35481301f)) : (iSlow136 ? (0.35481301f - (0.103624001f * fSlow97)) : 0.251188993f)) : (iSlow134 ? (0.251188993f - (0.171755999f * fSlow137)) : 0.0794330016f)) : (iSlow131 ? ((0.0205669999f * fSlow135) + 0.0794330016f) : 0.100000001f)) : (iSlow129 ? (0.100000001f - (0.0601890013f * fSlow132)) : 0.0398110002f)) : (iSlow127 ? ((0.0232849997f * fSlow130) + 0.0398110002f) : 0.0630960017f)) : (iSlow125 ? ((0.0369039997f * fSlow128) + 0.0630960017f) : 0.100000001f)) : fSlow191) : (iSlow121 ? ((0.0635839999f * fSlow124) + 0.0158489998f) : 0.0794330016f)) : (iSlow119 ? (0.0794330016f - (0.0478099994f * fSlow122)) : 0.0316229984f)) : (iSlow117 ? ((0.0683770031f * fSlow120) + 0.0316229984f) : 0.100000001f)) : (iSlow114 ? (0.100000001f - (0.0900000036f * fSlow118)) : 0.00999999978f)) : (iSlow112 ? ((0.0401189998f * fSlow115) + 0.00999999978f) : 0.0501190014f)) : fSlow180) : (iSlow108 ? (0.0794330016f - (0.0694330037f * fSlow111)) : 0.00999999978f)) : (iSlow106 ? ((0.388107002f * fSlow109) + 0.00999999978f) : 0.398106992f)) : (iSlow104 ? (0.398106992f - (0.198580995f * fSlow107)) : 0.199525997f)) : (iSlow102 ? (0.199525997f - (0.099526003f * fSlow105)) : 0.100000001f)) : (iSlow99 ? ((0.151188999f * fSlow103) + 0.100000001f) : 0.251188993f)) : (iSlow100 ? (0.251188993f - (0.0516630001f * fSlow101)) : 0.199525997f));
		float fSlow193 = (fSlow185 * fSlow190);
		float fSlow194 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? 4950.0f : (iSlow138 ? (4950.0f - (2200.0f * fSlow140)) : 2750.0f)) : (iSlow136 ? (fSlow183 + 2750.0f) : 3100.0f)) : (iSlow134 ? ((240.0f * fSlow137) + 3100.0f) : 3340.0f)) : (iSlow131 ? (3340.0f - (440.0f * fSlow135)) : 2900.0f)) : (iSlow129 ? (fSlow133 + 2900.0f) : 2950.0f)) : (iSlow127 ? ((400.0f * fSlow130) + 2950.0f) : 3350.0f)) : (iSlow125 ? (3350.0f - fSlow168) : 3300.0f)) : (iSlow123 ? ((290.0f * fSlow126) + 3300.0f) : 3590.0f)) : (iSlow121 ? (3590.0f - (290.0f * fSlow124)) : 3300.0f)) : (iSlow119 ? ((100.0f * fSlow122) + 3300.0f) : 3400.0f)) : (iSlow117 ? ((1550.0f * fSlow120) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow106 ? (4950.0f - (1700.0f * fSlow109)) : 3250.0f)) : (iSlow104 ? ((330.0f * fSlow107) + 3250.0f) : 3580.0f)) : (iSlow102 ? (3580.0f - (40.0f * fSlow105)) : 3540.0f)) : (iSlow99 ? (3540.0f - (540.0f * fSlow103)) : 3000.0f)) : (iSlow100 ? ((300.0f * fSlow101) + 3000.0f) : 3300.0f));
		float fSlow195 = std::tan((fConst1 * fSlow194));
		float fSlow196 = (1.0f / fSlow195);
		float fSlow197 = ((iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? (iSlow145 ? (iSlow147 ? 140.0f : (iSlow145 ? ((60.0f * fSlow98) + 140.0f) : 200.0f)) : 200.0f) : (iSlow141 ? (200.0f - (65.0f * fSlow144)) : 135.0f)) : (iSlow139 ? ((65.0f * fSlow142) + 135.0f) : 200.0f)) : (iSlow138 ? (200.0f - (70.0f * fSlow140)) : 130.0f)) : (iSlow136 ? (130.0f - fSlow163) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow127 ? (fSlow174 + 120.0f) : 140.0f)) : (iSlow125 ? (140.0f - fSlow173) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow117 ? (fSlow172 + 120.0f) : 140.0f)) : (iSlow114 ? ((60.0f * fSlow118) + 140.0f) : 200.0f)) : (iSlow112 ? (200.0f - fSlow116) : 120.0f)) : 120.0f) : (iSlow108 ? ((80.0f * fSlow111) + 120.0f) : 200.0f)) : (iSlow106 ? (200.0f - (60.0f * fSlow109)) : 140.0f)) : (iSlow104 ? (140.0f - fSlow149) : 120.0f)) : 120.0f) : (iSlow99 ? ((15.0f * fSlow103) + 120.0f) : 135.0f)) : (iSlow100 ? (135.0f - (15.0f * fSlow101)) : 120.0f)) / fSlow194);
		float fSlow198 = (((fSlow196 - fSlow197) / fSlow195) + 1.0f);
		float fSlow199 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow195)));
		float fSlow200 = (((fSlow196 + fSlow197) / fSlow195) + 1.0f);
		float fSlow201 = (iSlow99 ? (iSlow102 ? (iSlow104 ? (iSlow106 ? (iSlow108 ? (iSlow110 ? (iSlow112 ? (iSlow114 ? (iSlow117 ? (iSlow119 ? (iSlow121 ? (iSlow123 ? (iSlow125 ? (iSlow127 ? (iSlow129 ? (iSlow131 ? (iSlow134 ? (iSlow136 ? (iSlow138 ? (iSlow139 ? (iSlow141 ? (iSlow143 ? 0.00100000005f : (iSlow141 ? ((0.000777999987f * fSlow144) + 0.00100000005f) : 0.00177800003f)) : (iSlow139 ? (0.00177800003f - (0.001147f * fSlow142)) : 0.000630999974f)) : (iSlow138 ? ((0.0993689969f * fSlow140) + 0.000630999974f) : 0.100000001f)) : (iSlow136 ? ((0.0258930009f * fSlow97) + 0.100000001f) : 0.125892997f)) : (iSlow134 ? (0.125892997f - (0.0860819966f * fSlow137)) : 0.0398110002f)) : (iSlow131 ? (0.0398110002f - (0.0298110005f * fSlow135)) : 0.00999999978f)) : (iSlow129 ? ((0.00584900007f * fSlow132) + 0.00999999978f) : 0.0158489998f)) : (iSlow127 ? (0.0158489998f - (0.00325999991f * fSlow130)) : 0.0125890002f)) : (iSlow125 ? ((0.0874110013f * fSlow128) + 0.0125890002f) : 0.100000001f)) : fSlow191) : (iSlow121 ? ((0.00410400005f * fSlow124) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow117 ? (0.0199529994f - (0.0167909991f * fSlow120)) : 0.0031620001f)) : (iSlow114 ? (0.0031620001f - (0.00157700002f * fSlow118)) : 0.00158499996f)) : (iSlow112 ? ((0.00472499989f * fSlow115) + 0.00158499996f) : 0.00631000008f)) : (iSlow110 ? (0.00631000008f - (0.00314799999f * fSlow113)) : 0.0031620001f)) : (iSlow108 ? (0.0031620001f - (0.00216199993f * fSlow111)) : 0.00100000005f)) : (iSlow106 ? ((0.0784329996f * fSlow109) + 0.00100000005f) : 0.0794330016f)) : (iSlow104 ? ((0.0205669999f * fSlow107) + 0.0794330016f) : 0.100000001f)) : (iSlow102 ? (0.100000001f - (0.0683770031f * fSlow105)) : 0.0316229984f)) : (iSlow99 ? ((0.0184959993f * fSlow103) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow202 = (fSlow195 * fSlow200);
		float fSlow203 = (fConst65 * ((1.0f / float(fHslider28)) + -1.0f));
		float fSlow204 = float(fHslider29);
		int iSlow205 = (std::fabs(fSlow204) < 1.1920929e-07f);
		float fSlow206 = (iSlow205 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow205 ? 1.0f : fSlow204)))));
		float fSlow207 = float(fHslider30);
		int iSlow208 = (std::fabs(fSlow207) < 1.1920929e-07f);
		float fSlow209 = (iSlow208 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow208 ? 1.0f : fSlow207)))));
		float fSlow210 = (1.0f - fSlow209);
		float fSlow211 = (1.0f - fSlow206);
		float fSlow212 = float(fHslider31);
		float fSlow213 = (1.0f / float(fHslider32));
		float fSlow214 = std::tan((fConst1 * float(fHslider33)));
		float fSlow215 = (1.0f / fSlow214);
		float fSlow216 = (1.0f / (((fSlow213 + fSlow215) / fSlow214) + 1.0f));
		float fSlow217 = (((fSlow215 - fSlow213) / fSlow214) + 1.0f);
		float fSlow218 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow214))));
		float fSlow219 = (1.0f / fSlow87);
		float fSlow220 = float(fHslider34);
		int iSlow221 = (fSlow220 > 0.0f);
		float fSlow222 = (float(fHslider35) * std::sin((fConst5 * fSlow86)));
		float fSlow223 = (fConst1 * ((fSlow86 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow220)))) / fSlow222));
		float fSlow224 = (fConst1 * (fSlow86 / fSlow222));
		float fSlow225 = (iSlow221 ? fSlow224 : fSlow223);
		float fSlow226 = ((fSlow219 * (fSlow219 - fSlow225)) + 1.0f);
		float fSlow227 = ((fSlow219 * (fSlow219 + fSlow225)) + 1.0f);
		float fSlow228 = (iSlow221 ? fSlow223 : fSlow224);
		float fSlow229 = ((fSlow219 * (fSlow219 + fSlow228)) + 1.0f);
		float fSlow230 = ((fSlow219 * (fSlow219 - fSlow228)) + 1.0f);
		float fSlow231 = (1.0f / fSlow84);
		float fSlow232 = float(fHslider36);
		int iSlow233 = (fSlow232 > 0.0f);
		float fSlow234 = (float(fHslider37) * std::sin((fConst5 * fSlow83)));
		float fSlow235 = (fConst1 * ((fSlow83 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow232)))) / fSlow234));
		float fSlow236 = (fConst1 * (fSlow83 / fSlow234));
		float fSlow237 = (iSlow233 ? fSlow236 : fSlow235);
		float fSlow238 = ((fSlow231 * (fSlow231 - fSlow237)) + 1.0f);
		float fSlow239 = ((fSlow231 * (fSlow231 + fSlow237)) + 1.0f);
		float fSlow240 = (iSlow233 ? fSlow235 : fSlow236);
		float fSlow241 = ((fSlow231 * (fSlow231 + fSlow240)) + 1.0f);
		float fSlow242 = ((fSlow231 * (fSlow231 - fSlow240)) + 1.0f);
		float fSlow243 = (1.0f / fSlow81);
		float fSlow244 = (((fSlow80 - fSlow78) / fSlow79) + 1.0f);
		float fSlow245 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow79))));
		float fSlow246 = (1.0f / float(fHslider39));
		float fSlow247 = std::tan((fConst1 * float(fHslider40)));
		float fSlow248 = (1.0f / fSlow247);
		float fSlow249 = (((fSlow246 + fSlow248) / fSlow247) + 1.0f);
		float fSlow250 = ((std::pow(10.0f, (0.0500000007f * float(fHslider38))) * (1.0f - float(fCheckbox2))) / fSlow249);
		float fSlow251 = float(fHslider41);
		float fSlow252 = std::tan((fConst1 * fSlow251));
		float fSlow253 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow252))));
		float fSlow254 = float(fHslider42);
		float fSlow255 = std::tan((fConst1 * fSlow254));
		float fSlow256 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow255))));
		float fSlow257 = float(fHslider43);
		float fSlow258 = float((fSlow257 > 0.0f));
		float fSlow259 = (0.0f - fSlow257);
		float fSlow260 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider44)));
		float fSlow261 = ((fSlow260 * fSlow89) * ((0.0500000007f * fSlow95) + 1.0f));
		float fSlow262 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider45))) * fSlow89) * (1.0f - (0.0299999993f * fSlow95)));
		float fSlow263 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider46))) * fSlow89) * ((0.310000002f * fSlow95) + 1.0f));
		float fSlow264 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider47))) * fSlow89) * (1.0f - (0.270000011f * fSlow95)));
		float fSlow265 = (fConst65 * ((1.0f / float(fHslider48)) + -1.0f));
		float fSlow266 = float(fHslider49);
		int iSlow267 = (std::fabs(fSlow266) < 1.1920929e-07f);
		float fSlow268 = (iSlow267 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow267 ? 1.0f : fSlow266)))));
		float fSlow269 = float(fHslider50);
		int iSlow270 = (std::fabs(fSlow269) < 1.1920929e-07f);
		float fSlow271 = (iSlow270 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow270 ? 1.0f : fSlow269)))));
		float fSlow272 = (1.0f - fSlow271);
		float fSlow273 = (1.0f - fSlow268);
		float fSlow274 = float(fHslider51);
		float fSlow275 = (1.0f / float(fHslider52));
		float fSlow276 = std::tan((fConst1 * float(fHslider53)));
		float fSlow277 = (1.0f / fSlow276);
		float fSlow278 = (1.0f / (((fSlow275 + fSlow277) / fSlow276) + 1.0f));
		float fSlow279 = (((fSlow277 - fSlow275) / fSlow276) + 1.0f);
		float fSlow280 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow276))));
		float fSlow281 = (1.0f / fSlow255);
		float fSlow282 = float(fHslider54);
		int iSlow283 = (fSlow282 > 0.0f);
		float fSlow284 = (float(fHslider55) * std::sin((fConst5 * fSlow254)));
		float fSlow285 = (fConst1 * ((fSlow254 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow282)))) / fSlow284));
		float fSlow286 = (fConst1 * (fSlow254 / fSlow284));
		float fSlow287 = (iSlow283 ? fSlow286 : fSlow285);
		float fSlow288 = ((fSlow281 * (fSlow281 - fSlow287)) + 1.0f);
		float fSlow289 = ((fSlow281 * (fSlow281 + fSlow287)) + 1.0f);
		float fSlow290 = (iSlow283 ? fSlow285 : fSlow286);
		float fSlow291 = ((fSlow281 * (fSlow281 + fSlow290)) + 1.0f);
		float fSlow292 = ((fSlow281 * (fSlow281 - fSlow290)) + 1.0f);
		float fSlow293 = (1.0f / fSlow252);
		float fSlow294 = float(fHslider56);
		int iSlow295 = (fSlow294 > 0.0f);
		float fSlow296 = (float(fHslider57) * std::sin((fConst5 * fSlow251)));
		float fSlow297 = (fConst1 * ((fSlow251 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow294)))) / fSlow296));
		float fSlow298 = (fConst1 * (fSlow251 / fSlow296));
		float fSlow299 = (iSlow295 ? fSlow298 : fSlow297);
		float fSlow300 = ((fSlow293 * (fSlow293 - fSlow299)) + 1.0f);
		float fSlow301 = ((fSlow293 * (fSlow293 + fSlow299)) + 1.0f);
		float fSlow302 = (iSlow295 ? fSlow297 : fSlow298);
		float fSlow303 = ((fSlow293 * (fSlow293 + fSlow302)) + 1.0f);
		float fSlow304 = ((fSlow293 * (fSlow293 - fSlow302)) + 1.0f);
		float fSlow305 = (1.0f / fSlow249);
		float fSlow306 = (((fSlow248 - fSlow246) / fSlow247) + 1.0f);
		float fSlow307 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow247))));
		float fSlow308 = (1.0f / float(fHslider59));
		float fSlow309 = std::tan((fConst1 * float(fHslider60)));
		float fSlow310 = (1.0f / fSlow309);
		float fSlow311 = (((fSlow308 + fSlow310) / fSlow309) + 1.0f);
		float fSlow312 = ((std::pow(10.0f, (0.0500000007f * float(fHslider58))) * (1.0f - float(fCheckbox3))) / fSlow311);
		float fSlow313 = (0.300000012f * fSlow312);
		float fSlow314 = float(fHslider61);
		float fSlow315 = std::tan((fConst1 * fSlow314));
		float fSlow316 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow315))));
		float fSlow317 = float(fHslider62);
		float fSlow318 = std::tan((fConst1 * fSlow317));
		float fSlow319 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow318))));
		float fSlow320 = float(fHslider63);
		float fSlow321 = float((fSlow320 > 0.0f));
		float fSlow322 = (0.0f - fSlow320);
		float fSlow323 = (fConst65 * ((1.0f / float(fHslider64)) + -1.0f));
		float fSlow324 = float(fHslider65);
		int iSlow325 = (std::fabs(fSlow324) < 1.1920929e-07f);
		float fSlow326 = (iSlow325 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow325 ? 1.0f : fSlow324)))));
		float fSlow327 = float(fHslider66);
		int iSlow328 = (std::fabs(fSlow327) < 1.1920929e-07f);
		float fSlow329 = (iSlow328 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow328 ? 1.0f : fSlow327)))));
		float fSlow330 = (1.0f - fSlow329);
		float fSlow331 = (1.0f - fSlow326);
		float fSlow332 = float(fHslider67);
		float fSlow333 = (1.0f / float(fHslider68));
		float fSlow334 = std::tan((fConst1 * float(fHslider69)));
		float fSlow335 = (1.0f / fSlow334);
		float fSlow336 = (1.0f / (((fSlow333 + fSlow335) / fSlow334) + 1.0f));
		float fSlow337 = (((fSlow335 - fSlow333) / fSlow334) + 1.0f);
		float fSlow338 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow334))));
		float fSlow339 = (1.0f / fSlow318);
		float fSlow340 = float(fHslider70);
		int iSlow341 = (fSlow340 > 0.0f);
		float fSlow342 = (float(fHslider71) * std::sin((fConst5 * fSlow317)));
		float fSlow343 = (fConst1 * ((fSlow317 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow340)))) / fSlow342));
		float fSlow344 = (fConst1 * (fSlow317 / fSlow342));
		float fSlow345 = (iSlow341 ? fSlow344 : fSlow343);
		float fSlow346 = ((fSlow339 * (fSlow339 - fSlow345)) + 1.0f);
		float fSlow347 = ((fSlow339 * (fSlow339 + fSlow345)) + 1.0f);
		float fSlow348 = (iSlow341 ? fSlow343 : fSlow344);
		float fSlow349 = ((fSlow339 * (fSlow339 + fSlow348)) + 1.0f);
		float fSlow350 = ((fSlow339 * (fSlow339 - fSlow348)) + 1.0f);
		float fSlow351 = (1.0f / fSlow315);
		float fSlow352 = float(fHslider72);
		int iSlow353 = (fSlow352 > 0.0f);
		float fSlow354 = (float(fHslider73) * std::sin((fConst5 * fSlow314)));
		float fSlow355 = (fConst1 * ((fSlow314 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow352)))) / fSlow354));
		float fSlow356 = (fConst1 * (fSlow314 / fSlow354));
		float fSlow357 = (iSlow353 ? fSlow356 : fSlow355);
		float fSlow358 = ((fSlow351 * (fSlow351 - fSlow357)) + 1.0f);
		float fSlow359 = ((fSlow351 * (fSlow351 + fSlow357)) + 1.0f);
		float fSlow360 = (iSlow353 ? fSlow355 : fSlow356);
		float fSlow361 = ((fSlow351 * (fSlow351 + fSlow360)) + 1.0f);
		float fSlow362 = ((fSlow351 * (fSlow351 - fSlow360)) + 1.0f);
		float fSlow363 = (1.0f / fSlow311);
		float fSlow364 = (((fSlow310 - fSlow308) / fSlow309) + 1.0f);
		float fSlow365 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow309))));
		float fSlow366 = (1.0f / float(fHslider75));
		float fSlow367 = std::tan((fConst1 * float(fHslider76)));
		float fSlow368 = (1.0f / fSlow367);
		float fSlow369 = (((fSlow366 + fSlow368) / fSlow367) + 1.0f);
		float fSlow370 = ((std::pow(10.0f, (0.0500000007f * float(fHslider74))) * (1.0f - float(fCheckbox4))) / fSlow369);
		float fSlow371 = float(fHslider77);
		float fSlow372 = std::tan((fConst1 * fSlow371));
		float fSlow373 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow372))));
		float fSlow374 = float(fHslider78);
		float fSlow375 = std::tan((fConst1 * fSlow374));
		float fSlow376 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow375))));
		float fSlow377 = (0.5f * fSlow260);
		float fSlow378 = (fConst358 * fSlow89);
		float fSlow379 = (27.0f * fSlow89);
		float fSlow380 = (9.0f * fSlow89);
		float fSlow381 = (3.0f * fSlow89);
		float fSlow382 = (fConst65 * ((1.0f / float(fHslider79)) + -1.0f));
		float fSlow383 = float(fHslider80);
		int iSlow384 = (std::fabs(fSlow383) < 1.1920929e-07f);
		float fSlow385 = (iSlow384 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow384 ? 1.0f : fSlow383)))));
		float fSlow386 = float(fHslider81);
		int iSlow387 = (std::fabs(fSlow386) < 1.1920929e-07f);
		float fSlow388 = (iSlow387 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow387 ? 1.0f : fSlow386)))));
		float fSlow389 = (1.0f - fSlow388);
		float fSlow390 = (1.0f - fSlow385);
		float fSlow391 = float(fHslider82);
		float fSlow392 = (1.0f / float(fHslider83));
		float fSlow393 = std::tan((fConst1 * float(fHslider84)));
		float fSlow394 = (1.0f / fSlow393);
		float fSlow395 = (1.0f / (((fSlow392 + fSlow394) / fSlow393) + 1.0f));
		float fSlow396 = (((fSlow394 - fSlow392) / fSlow393) + 1.0f);
		float fSlow397 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow393))));
		float fSlow398 = (1.0f / fSlow375);
		float fSlow399 = float(fHslider85);
		int iSlow400 = (fSlow399 > 0.0f);
		float fSlow401 = (float(fHslider86) * std::sin((fConst5 * fSlow374)));
		float fSlow402 = (fConst1 * ((fSlow374 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow399)))) / fSlow401));
		float fSlow403 = (fConst1 * (fSlow374 / fSlow401));
		float fSlow404 = (iSlow400 ? fSlow403 : fSlow402);
		float fSlow405 = ((fSlow398 * (fSlow398 - fSlow404)) + 1.0f);
		float fSlow406 = ((fSlow398 * (fSlow398 + fSlow404)) + 1.0f);
		float fSlow407 = (iSlow400 ? fSlow402 : fSlow403);
		float fSlow408 = ((fSlow398 * (fSlow398 + fSlow407)) + 1.0f);
		float fSlow409 = ((fSlow398 * (fSlow398 - fSlow407)) + 1.0f);
		float fSlow410 = (1.0f / fSlow372);
		float fSlow411 = float(fHslider87);
		int iSlow412 = (fSlow411 > 0.0f);
		float fSlow413 = (float(fHslider88) * std::sin((fConst5 * fSlow371)));
		float fSlow414 = (fConst1 * ((fSlow371 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow411)))) / fSlow413));
		float fSlow415 = (fConst1 * (fSlow371 / fSlow413));
		float fSlow416 = (iSlow412 ? fSlow415 : fSlow414);
		float fSlow417 = ((fSlow410 * (fSlow410 - fSlow416)) + 1.0f);
		float fSlow418 = ((fSlow410 * (fSlow410 + fSlow416)) + 1.0f);
		float fSlow419 = (iSlow412 ? fSlow414 : fSlow415);
		float fSlow420 = ((fSlow410 * (fSlow410 + fSlow419)) + 1.0f);
		float fSlow421 = ((fSlow410 * (fSlow410 - fSlow419)) + 1.0f);
		float fSlow422 = (1.0f / fSlow369);
		float fSlow423 = (((fSlow368 - fSlow366) / fSlow367) + 1.0f);
		float fSlow424 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow367))));
		float fSlow425 = (1.0f / float(fHslider90));
		float fSlow426 = std::tan((fConst1 * float(fHslider91)));
		float fSlow427 = (1.0f / fSlow426);
		float fSlow428 = (((fSlow425 + fSlow427) / fSlow426) + 1.0f);
		float fSlow429 = ((std::pow(10.0f, (0.0500000007f * float(fHslider89))) * (1.0f - float(fCheckbox5))) / fSlow428);
		float fSlow430 = float(fHslider92);
		float fSlow431 = std::tan((fConst1 * fSlow430));
		float fSlow432 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow431))));
		float fSlow433 = float(fHslider93);
		float fSlow434 = std::tan((fConst1 * fSlow433));
		float fSlow435 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow434))));
		float fSlow436 = float(fHslider94);
		float fSlow437 = float((fSlow436 > 0.0f));
		float fSlow438 = (0.0f - fSlow436);
		float fSlow439 = (2.0f * float(fHslider95));
		float fSlow440 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider96)));
		float fSlow441 = (1.0f - fSlow440);
		float fSlow442 = (0.00999999978f * float(fHslider97));
		float fSlow443 = (fConst65 * ((1.0f / float(fHslider98)) + -1.0f));
		float fSlow444 = float(fHslider99);
		int iSlow445 = (std::fabs(fSlow444) < 1.1920929e-07f);
		float fSlow446 = (iSlow445 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow445 ? 1.0f : fSlow444)))));
		float fSlow447 = float(fHslider100);
		int iSlow448 = (std::fabs(fSlow447) < 1.1920929e-07f);
		float fSlow449 = (iSlow448 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow448 ? 1.0f : fSlow447)))));
		float fSlow450 = (1.0f - fSlow449);
		float fSlow451 = (1.0f - fSlow446);
		float fSlow452 = float(fHslider101);
		float fSlow453 = (1.0f / float(fHslider102));
		float fSlow454 = std::tan((fConst1 * float(fHslider103)));
		float fSlow455 = (1.0f / fSlow454);
		float fSlow456 = (1.0f / (((fSlow453 + fSlow455) / fSlow454) + 1.0f));
		float fSlow457 = (((fSlow455 - fSlow453) / fSlow454) + 1.0f);
		float fSlow458 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow454))));
		float fSlow459 = (1.0f / fSlow434);
		float fSlow460 = float(fHslider104);
		int iSlow461 = (fSlow460 > 0.0f);
		float fSlow462 = (float(fHslider105) * std::sin((fConst5 * fSlow433)));
		float fSlow463 = (fConst1 * ((fSlow433 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow460)))) / fSlow462));
		float fSlow464 = (fConst1 * (fSlow433 / fSlow462));
		float fSlow465 = (iSlow461 ? fSlow464 : fSlow463);
		float fSlow466 = ((fSlow459 * (fSlow459 - fSlow465)) + 1.0f);
		float fSlow467 = ((fSlow459 * (fSlow459 + fSlow465)) + 1.0f);
		float fSlow468 = (iSlow461 ? fSlow463 : fSlow464);
		float fSlow469 = ((fSlow459 * (fSlow459 + fSlow468)) + 1.0f);
		float fSlow470 = ((fSlow459 * (fSlow459 - fSlow468)) + 1.0f);
		float fSlow471 = (1.0f / fSlow431);
		float fSlow472 = float(fHslider106);
		int iSlow473 = (fSlow472 > 0.0f);
		float fSlow474 = (float(fHslider107) * std::sin((fConst5 * fSlow430)));
		float fSlow475 = (fConst1 * ((fSlow430 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow472)))) / fSlow474));
		float fSlow476 = (fConst1 * (fSlow430 / fSlow474));
		float fSlow477 = (iSlow473 ? fSlow476 : fSlow475);
		float fSlow478 = ((fSlow471 * (fSlow471 - fSlow477)) + 1.0f);
		float fSlow479 = ((fSlow471 * (fSlow471 + fSlow477)) + 1.0f);
		float fSlow480 = (iSlow473 ? fSlow475 : fSlow476);
		float fSlow481 = ((fSlow471 * (fSlow471 + fSlow480)) + 1.0f);
		float fSlow482 = ((fSlow471 * (fSlow471 - fSlow480)) + 1.0f);
		float fSlow483 = (1.0f / fSlow428);
		float fSlow484 = (((fSlow427 - fSlow425) / fSlow426) + 1.0f);
		float fSlow485 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow426))));
		float fSlow486 = (1.0f / float(fHslider109));
		float fSlow487 = std::tan((fConst1 * float(fHslider110)));
		float fSlow488 = (1.0f / fSlow487);
		float fSlow489 = (((fSlow486 + fSlow488) / fSlow487) + 1.0f);
		float fSlow490 = ((std::pow(10.0f, (0.0500000007f * float(fHslider108))) * (1.0f - float(fCheckbox6))) / fSlow489);
		float fSlow491 = float(fHslider111);
		float fSlow492 = std::tan((fConst1 * fSlow491));
		float fSlow493 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow492))));
		float fSlow494 = float(fHslider112);
		float fSlow495 = std::tan((fConst1 * fSlow494));
		float fSlow496 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow495))));
		float fSlow497 = float(fHslider113);
		float fSlow498 = float((fSlow497 > 0.0f));
		float fSlow499 = (0.0f - fSlow497);
		float fSlow500 = float(fHslider114);
		float fSlow501 = (0.00100000005f * float((fSlow500 > 20.0f)));
		float fSlow502 = (fConst66 * std::max<float>(20.0f, fSlow500));
		float fSlow503 = float(fHslider115);
		float fSlow504 = (0.00100000005f * float((fSlow503 > 20.0f)));
		float fSlow505 = (fConst66 * std::max<float>(20.0f, fSlow503));
		float fSlow506 = float(fHslider116);
		float fSlow507 = (0.00100000005f * float((fSlow506 > 20.0f)));
		float fSlow508 = (fConst66 * std::max<float>(20.0f, fSlow506));
		float fSlow509 = (fConst65 * ((1.0f / float(fHslider117)) + -1.0f));
		float fSlow510 = float(fHslider118);
		int iSlow511 = (std::fabs(fSlow510) < 1.1920929e-07f);
		float fSlow512 = (iSlow511 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow511 ? 1.0f : fSlow510)))));
		float fSlow513 = float(fHslider119);
		int iSlow514 = (std::fabs(fSlow513) < 1.1920929e-07f);
		float fSlow515 = (iSlow514 ? 0.0f : std::exp((0.0f - (fConst66 / (iSlow514 ? 1.0f : fSlow513)))));
		float fSlow516 = (1.0f - fSlow515);
		float fSlow517 = (1.0f - fSlow512);
		float fSlow518 = float(fHslider120);
		float fSlow519 = (1.0f / float(fHslider121));
		float fSlow520 = std::tan((fConst1 * float(fHslider122)));
		float fSlow521 = (1.0f / fSlow520);
		float fSlow522 = (1.0f / (((fSlow519 + fSlow521) / fSlow520) + 1.0f));
		float fSlow523 = (((fSlow521 - fSlow519) / fSlow520) + 1.0f);
		float fSlow524 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow520))));
		float fSlow525 = (1.0f / fSlow495);
		float fSlow526 = float(fHslider123);
		int iSlow527 = (fSlow526 > 0.0f);
		float fSlow528 = (float(fHslider124) * std::sin((fConst5 * fSlow494)));
		float fSlow529 = (fConst1 * ((fSlow494 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow526)))) / fSlow528));
		float fSlow530 = (fConst1 * (fSlow494 / fSlow528));
		float fSlow531 = (iSlow527 ? fSlow530 : fSlow529);
		float fSlow532 = ((fSlow525 * (fSlow525 - fSlow531)) + 1.0f);
		float fSlow533 = ((fSlow525 * (fSlow525 + fSlow531)) + 1.0f);
		float fSlow534 = (iSlow527 ? fSlow529 : fSlow530);
		float fSlow535 = ((fSlow525 * (fSlow525 + fSlow534)) + 1.0f);
		float fSlow536 = ((fSlow525 * (fSlow525 - fSlow534)) + 1.0f);
		float fSlow537 = (1.0f / fSlow492);
		float fSlow538 = float(fHslider125);
		int iSlow539 = (fSlow538 > 0.0f);
		float fSlow540 = (float(fHslider126) * std::sin((fConst5 * fSlow491)));
		float fSlow541 = (fConst1 * ((fSlow491 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow538)))) / fSlow540));
		float fSlow542 = (fConst1 * (fSlow491 / fSlow540));
		float fSlow543 = (iSlow539 ? fSlow542 : fSlow541);
		float fSlow544 = ((fSlow537 * (fSlow537 - fSlow543)) + 1.0f);
		float fSlow545 = ((fSlow537 * (fSlow537 + fSlow543)) + 1.0f);
		float fSlow546 = (iSlow539 ? fSlow541 : fSlow542);
		float fSlow547 = ((fSlow537 * (fSlow537 + fSlow546)) + 1.0f);
		float fSlow548 = ((fSlow537 * (fSlow537 - fSlow546)) + 1.0f);
		float fSlow549 = (1.0f / fSlow489);
		float fSlow550 = (((fSlow488 - fSlow486) / fSlow487) + 1.0f);
		float fSlow551 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow487))));
		float fSlow552 = std::pow(10.0f, (0.600000024f * fSlow2));
		float fSlow553 = (fConst692 * ((0.899999976f * fSlow1) + 0.100000001f));
		float fSlow554 = (0.949999988f * fSlow312);
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
			fRec37[0] = (fConst71 + (fRec37[1] - std::floor((fConst71 + fRec37[1]))));
			float fTemp11 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec37[0]))]) + 1.0f);
			fVec4[0] = fSlow91;
			float fTemp12 = (fSlow91 - fVec4[1]);
			fVec5[0] = fSlow93;
			float fTemp13 = (0.0f - (fSlow91 + fVec5[1]));
			float fTemp14 = std::min<float>(1.0f, ((fTemp12 * float((fTemp12 > 0.0f))) + (fSlow92 * (fTemp13 * float((fTemp13 > 0.0f))))));
			fVec6[0] = fTemp14;
			int iTemp15 = int(fTemp14);
			fRec39[0] = (iTemp15 ? fSlow94 : fRec39[1]);
			fRec41[0] = (fConst74 + (fRec41[1] - std::floor((fConst74 + fRec41[1]))));
			fRec40[0] = ((0.999000013f * fRec40[1]) + (0.00100000005f * ((fSlow96 * ftbl0mydspSIG0[int((65536.0f * fRec41[0]))]) + 1.0f)));
			fRec38[0] = ((fConst72 * fRec38[1]) + (fConst73 * (std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow90 * (fRec39[0] * fRec40[0])))) * fTemp11)));
			float fTemp16 = ((0.00366666657f * (400.0f - fRec38[0])) + 3.0f);
			float fTemp17 = std::max<float>(1.1920929e-07f, std::fabs(fRec38[0]));
			float fTemp18 = (fRec43[1] + (fConst66 * fTemp17));
			float fTemp19 = (fTemp18 + -1.0f);
			int iTemp20 = (fTemp19 < 0.0f);
			fRec43[0] = (iTemp20 ? fTemp18 : fTemp19);
			float fRec44 = (iTemp20 ? fTemp18 : (fTemp18 + ((1.0f - (fConst0 / fTemp17)) * fTemp19)));
			float fTemp21 = (0.400000006f * ((2.0f * fRec44) + -1.0f));
			float fTemp22 = ((fSlow148 <= fRec38[0]) ? fRec38[0] : fSlow148);
			float fTemp23 = std::tan((fConst1 * fTemp22));
			float fTemp24 = (1.0f / fTemp23);
			float fTemp25 = (fSlow156 / fTemp22);
			float fTemp26 = (((fTemp24 + fTemp25) / fTemp23) + 1.0f);
			fRec42[0] = (fTemp21 - (((fRec42[2] * (((fTemp24 - fTemp25) / fTemp23) + 1.0f)) + (2.0f * (fRec42[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp23)))))) / fTemp26));
			float fTemp27 = (fTemp23 * fTemp26);
			float fTemp28 = ((2.0f * fRec38[0]) + 30.0f);
			float fTemp29 = ((iSlow158 & (fRec38[0] >= 200.0f)) ? (fSlow157 - (0.000952380942f * ((fRec38[0] + -200.0f) * fSlow159))) : ((fSlow157 <= fTemp28) ? fTemp28 : fSlow157));
			float fTemp30 = std::tan((fConst1 * fTemp29));
			float fTemp31 = (1.0f / fTemp30);
			float fTemp32 = (fSlow164 / fTemp29);
			float fTemp33 = (((fTemp31 + fTemp32) / fTemp30) + 1.0f);
			fRec45[0] = (fTemp21 - (((fRec45[2] * (((fTemp31 - fTemp32) / fTemp30) + 1.0f)) + (2.0f * (fRec45[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp30)))))) / fTemp33));
			float fTemp34 = (fTemp30 * fTemp33);
			fRec46[0] = (fTemp21 - (((fRec46[2] * fSlow177) + (2.0f * (fRec46[1] * fSlow178))) / fSlow179));
			fRec47[0] = (fTemp21 - (((fRec47[2] * fSlow188) + (2.0f * (fRec47[1] * fSlow189))) / fSlow190));
			fRec48[0] = (fTemp21 - (((fRec48[2] * fSlow198) + (2.0f * (fRec48[1] * fSlow199))) / fSlow200));
			iRec49[0] = (((iRec49[1] + (iRec49[1] > 0)) * (fTemp14 <= fVec6[1])) + (fTemp14 > fVec6[1]));
			float fTemp35 = float(iRec49[0]);
			fRec35[0] = (((fTemp11 * ((fTemp16 * (((((fRec42[0] / fTemp27) + ((fRec45[0] * fSlow165) / fTemp34)) + ((fRec46[0] * fSlow181) / fSlow182)) + ((fRec47[0] * fSlow192) / fSlow193)) + ((fRec48[0] * fSlow201) / fSlow202))) + (((((fRec42[2] * (0.0f - (fTemp16 / fTemp27))) + (fRec45[2] * (0.0f - ((fTemp16 * fSlow165) / fTemp34)))) + (fRec46[2] * (0.0f - ((fTemp16 * fSlow181) / fSlow182)))) + (fRec47[2] * (0.0f - ((fTemp16 * fSlow192) / fSlow193)))) + (fRec48[2] * (0.0f - ((fTemp16 * fSlow201) / fSlow202)))))) * std::sqrt(std::max<float>(0.0f, std::min<float>((fConst76 * fTemp35), ((fConst77 * (fConst75 - fTemp35)) + 1.0f))))) - (fConst78 * ((fConst79 * fRec35[2]) + (fConst80 * fRec35[1]))));
			iRec50[0] = (iTemp15 ? 7 : iRec50[1]);
			float fTemp36 = (((fRec35[2] + (fRec35[0] + (2.0f * fRec35[1]))) * std::max<float>(0.0f, std::min<float>((fConst82 * fTemp35), ((fConst83 * (fConst81 - fTemp35)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec50[0] + -10)))));
			float fTemp37 = std::fabs((fConst70 * fTemp36));
			fRec53[0] = std::max<float>(fTemp37, ((fRec53[1] * fSlow209) + (fTemp37 * fSlow210)));
			fRec52[0] = ((fRec52[1] * fSlow206) + (fRec53[0] * fSlow211));
			fRec51[0] = ((fConst64 * fRec51[1]) + (fSlow203 * std::max<float>(((20.0f * std::log10(fRec52[0])) - fSlow212), 0.0f)));
			float fTemp38 = (fConst70 * (fTemp36 * std::pow(10.0f, (0.0500000007f * fRec51[0]))));
			fRec54[0] = (fTemp38 - (fSlow216 * ((fSlow217 * fRec54[2]) + (fSlow218 * fRec54[1]))));
			float fTemp39 = (fSlow88 * fRec34[1]);
			fRec34[0] = (fTemp38 - ((fSlow216 * (fRec54[2] + (fRec54[0] + (2.0f * fRec54[1])))) + (((fRec34[2] * fSlow226) + fTemp39) / fSlow227)));
			float fTemp40 = (fSlow85 * fRec33[1]);
			fRec33[0] = ((((fTemp39 + (fRec34[0] * fSlow229)) + (fRec34[2] * fSlow230)) / fSlow227) - (((fRec33[2] * fSlow238) + fTemp40) / fSlow239));
			fRec32[0] = ((((fTemp40 + (fRec33[0] * fSlow241)) + (fRec33[2] * fSlow242)) / fSlow239) - (fSlow243 * ((fSlow244 * fRec32[2]) + (fSlow245 * fRec32[1]))));
			float fTemp41 = (fSlow82 * (fRec32[2] + (fRec32[0] + (2.0f * fRec32[1]))));
			float fTemp42 = ((fSlow8 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + fTemp41);
			fVec7[0] = fSlow257;
			float fTemp43 = (fSlow257 - fVec7[1]);
			fVec8[0] = fSlow259;
			float fTemp44 = (0.0f - (fSlow257 + fVec8[1]));
			float fTemp45 = std::min<float>(1.0f, ((fTemp43 * float((fTemp43 > 0.0f))) + (fSlow258 * (fTemp44 * float((fTemp44 > 0.0f))))));
			fVec9[0] = fTemp45;
			iRec59[0] = (((iRec59[1] + (iRec59[1] > 0)) * (fTemp45 <= fVec9[1])) + (fTemp45 > fVec9[1]));
			float fTemp46 = float(iRec59[0]);
			float fTemp47 = (fConst82 * fTemp46);
			float fTemp48 = (fConst81 - fTemp46);
			fRec58[0] = ((fConst84 * fRec58[1]) + (fConst85 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp47, ((fConst83 * fTemp48) + 1.0f))))));
			int iTemp49 = int(fTemp45);
			fRec62[0] = (iTemp49 ? fSlow261 : fRec62[1]);
			iRec61[0] = ((iVec0[1] + iRec61[1]) % int((fConst0 / fRec62[0])));
			float fTemp50 = float(iRec61[0]);
			int iTemp51 = (fTemp45 > 0.0f);
			float fTemp52 = (0.144717798f * (iTemp51 ? 0.00100000005f : 1.60000002f));
			int iTemp53 = (std::fabs(fTemp52) < 1.1920929e-07f);
			float fTemp54 = (iTemp53 ? 0.0f : std::exp((0.0f - (fConst66 / (iTemp53 ? 1.0f : fTemp52)))));
			fRec64[0] = ((fRec64[1] * fTemp54) + ((iTemp51 ? fTemp45 : 0.400000006f) * (1.0f - fTemp54)));
			fRec63[0] = ((fConst89 * fRec63[1]) + (fConst90 * fRec64[0]));
			float fTemp55 = (fRec63[0] + 0.00999999978f);
			float fTemp56 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec62[0] * fTemp55) * (1.0f - (0.000500000024f * std::min<float>(fRec62[0], 1000.0f)))))))));
			float fTemp57 = (1.0f / fTemp56);
			float fTemp58 = (((fTemp57 + 1.41421354f) / fTemp56) + 1.0f);
			fRec60[0] = ((2.0f * (((float((fTemp50 < (fConst86 / fRec62[0]))) + -0.5f) + (float((fTemp50 < (fConst87 / fRec62[0]))) + -0.5f)) + (float((fTemp50 < (fConst88 / fRec62[0]))) + -0.5f))) - (((fRec60[2] * (((fTemp57 + -1.41421354f) / fTemp56) + 1.0f)) + (2.0f * (fRec60[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp56)))))) / fTemp58));
			fRec67[0] = (iTemp49 ? fSlow262 : fRec67[1]);
			iRec66[0] = ((iVec0[1] + iRec66[1]) % int((fConst0 / fRec67[0])));
			float fTemp59 = float(iRec66[0]);
			float fTemp60 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fRec67[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec67[0], 1000.0f)))))))));
			float fTemp61 = (1.0f / fTemp60);
			float fTemp62 = (((fTemp61 + 1.41421354f) / fTemp60) + 1.0f);
			fRec65[0] = ((2.0f * (((float((fTemp59 < (fConst86 / fRec67[0]))) + -0.5f) + (float((fTemp59 < (fConst87 / fRec67[0]))) + -0.5f)) + (float((fTemp59 < (fConst88 / fRec67[0]))) + -0.5f))) - (((fRec65[2] * (((fTemp61 + -1.41421354f) / fTemp60) + 1.0f)) + (2.0f * (fRec65[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp60)))))) / fTemp62));
			fRec70[0] = (iTemp49 ? fSlow263 : fRec70[1]);
			iRec69[0] = ((iVec0[1] + iRec69[1]) % int((fConst0 / fRec70[0])));
			float fTemp63 = float(iRec69[0]);
			float fTemp64 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fRec70[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec70[0], 1000.0f)))))))));
			float fTemp65 = (1.0f / fTemp64);
			float fTemp66 = (((fTemp65 + 1.41421354f) / fTemp64) + 1.0f);
			fRec68[0] = ((2.0f * (((float((fTemp63 < (fConst86 / fRec70[0]))) + -0.5f) + (float((fTemp63 < (fConst87 / fRec70[0]))) + -0.5f)) + (float((fTemp63 < (fConst88 / fRec70[0]))) + -0.5f))) - (((fRec68[2] * (((fTemp65 + -1.41421354f) / fTemp64) + 1.0f)) + (2.0f * (fRec68[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp64)))))) / fTemp66));
			fRec73[0] = (iTemp49 ? fSlow264 : fRec73[1]);
			iRec72[0] = ((iVec0[1] + iRec72[1]) % int((fConst0 / fRec73[0])));
			float fTemp67 = float(iRec72[0]);
			float fTemp68 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fRec73[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec73[0], 1000.0f)))))))));
			float fTemp69 = (1.0f / fTemp68);
			float fTemp70 = (((fTemp69 + 1.41421354f) / fTemp68) + 1.0f);
			fRec71[0] = ((2.0f * (((float((fTemp67 < (fConst86 / fRec73[0]))) + -0.5f) + (float((fTemp67 < (fConst87 / fRec73[0]))) + -0.5f)) + (float((fTemp67 < (fConst88 / fRec73[0]))) + -0.5f))) - (((fRec71[2] * (((fTemp69 + -1.41421354f) / fTemp68) + 1.0f)) + (2.0f * (fRec71[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp68)))))) / fTemp70));
			float fTemp71 = (fRec58[0] * (((((fRec60[2] + (fRec60[0] + (2.0f * fRec60[1]))) / fTemp58) + ((fRec65[2] + (fRec65[0] + (2.0f * fRec65[1]))) / fTemp62)) + ((fRec68[2] + (fRec68[0] + (2.0f * fRec68[1]))) / fTemp66)) + ((fRec71[2] + (fRec71[0] + (2.0f * fRec71[1]))) / fTemp70)));
			float fTemp72 = std::fabs((0.25f * fTemp71));
			fRec76[0] = std::max<float>(fTemp72, ((fRec76[1] * fSlow271) + (fTemp72 * fSlow272)));
			fRec75[0] = ((fRec75[1] * fSlow268) + (fRec76[0] * fSlow273));
			fRec74[0] = ((fConst64 * fRec74[1]) + (fSlow265 * std::max<float>(((20.0f * std::log10(fRec75[0])) - fSlow274), 0.0f)));
			float fTemp73 = (0.25f * (fTemp71 * std::pow(10.0f, (0.0500000007f * fRec74[0]))));
			fRec77[0] = (fTemp73 - (fSlow278 * ((fSlow279 * fRec77[2]) + (fSlow280 * fRec77[1]))));
			float fTemp74 = (fSlow256 * fRec57[1]);
			fRec57[0] = (fTemp73 - ((fSlow278 * (fRec77[2] + (fRec77[0] + (2.0f * fRec77[1])))) + (((fRec57[2] * fSlow288) + fTemp74) / fSlow289)));
			float fTemp75 = (fSlow253 * fRec56[1]);
			fRec56[0] = ((((fTemp74 + (fRec57[0] * fSlow291)) + (fRec57[2] * fSlow292)) / fSlow289) - (((fRec56[2] * fSlow300) + fTemp75) / fSlow301));
			fRec55[0] = ((((fTemp75 + (fRec56[0] * fSlow303)) + (fRec56[2] * fSlow304)) / fSlow301) - (fSlow305 * ((fSlow306 * fRec55[2]) + (fSlow307 * fRec55[1]))));
			iRec81[0] = 0;
			fRec85[0] = ((0.949999988f * fRec85[1]) + (0.0500000007f * fRec84[1]));
			float fTemp76 = ((0.99000001f * fRec85[0]) + float(iRec81[1]));
			fVec10[(IOTA & 2047)] = fTemp76;
			fVec11[0] = fSlow320;
			float fTemp77 = (fSlow320 - fVec11[1]);
			fVec12[0] = fSlow322;
			float fTemp78 = (0.0f - (fSlow320 + fVec12[1]));
			float fTemp79 = std::min<float>(1.0f, ((fTemp77 * float((fTemp77 > 0.0f))) + (fSlow321 * (fTemp78 * float((fTemp78 > 0.0f))))));
			fVec13[0] = fTemp79;
			fRec86[0] = (int(fTemp79) ? fSlow320 : fRec86[1]);
			float fTemp80 = (fRec86[0] + 100.0f);
			float fTemp81 = (fConst92 * ((340.0f / fTemp80) + -0.0399999991f));
			float fTemp82 = (fTemp81 + -1.49999499f);
			int iTemp83 = int(fTemp82);
			int iTemp84 = int(std::min<float>(fConst91, float(std::max<int>(0, iTemp83))));
			float fTemp85 = std::floor(fTemp82);
			float fTemp86 = (fTemp81 + (-1.0f - fTemp85));
			float fTemp87 = (0.0f - fTemp86);
			float fTemp88 = (fTemp81 + (-2.0f - fTemp85));
			float fTemp89 = (0.0f - (0.5f * fTemp88));
			float fTemp90 = (fTemp81 + (-3.0f - fTemp85));
			float fTemp91 = (0.0f - (0.333333343f * fTemp90));
			float fTemp92 = (fTemp81 + (-4.0f - fTemp85));
			float fTemp93 = (0.0f - (0.25f * fTemp92));
			float fTemp94 = (fTemp81 - fTemp85);
			int iTemp95 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp83 + 1)))));
			float fTemp96 = (0.0f - fTemp88);
			float fTemp97 = (0.0f - (0.5f * fTemp90));
			float fTemp98 = (0.0f - (0.333333343f * fTemp92));
			int iTemp99 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp83 + 2)))));
			float fTemp100 = (0.0f - fTemp90);
			float fTemp101 = (0.0f - (0.5f * fTemp92));
			float fTemp102 = (fTemp86 * fTemp88);
			int iTemp103 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp83 + 3)))));
			float fTemp104 = (0.0f - fTemp92);
			float fTemp105 = (fTemp102 * fTemp90);
			int iTemp106 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp83 + 4)))));
			fRec83[0] = (((((fVec10[((IOTA - iTemp84) & 2047)] * fTemp87) * fTemp89) * fTemp91) * fTemp93) + (fTemp94 * ((((((fVec10[((IOTA - iTemp95) & 2047)] * fTemp96) * fTemp97) * fTemp98) + (0.5f * (((fTemp86 * fVec10[((IOTA - iTemp99) & 2047)]) * fTemp100) * fTemp101))) + (0.166666672f * ((fTemp102 * fVec10[((IOTA - iTemp103) & 2047)]) * fTemp104))) + (0.0416666679f * (fTemp105 * fVec10[((IOTA - iTemp106) & 2047)])))));
			fRec87[0] = ((0.949999988f * fRec87[1]) + (0.0500000007f * fRec83[1]));
			fRec90[0] = (fTemp0 - (fConst98 * ((fConst101 * fRec90[2]) + (fConst102 * fRec90[1]))));
			float fTemp107 = (fConst98 * (((fConst100 * fRec90[0]) + (fConst103 * fRec90[1])) + (fConst100 * fRec90[2])));
			fRec89[0] = (fTemp107 - (fConst95 * ((fConst104 * fRec89[2]) + (fConst105 * fRec89[1]))));
			iRec91[0] = (((iRec91[1] + (iRec91[1] > 0)) * (fTemp79 <= fVec13[1])) + (fTemp79 > fVec13[1]));
			float fTemp108 = (fConst106 * float(iRec91[0]));
			float fTemp109 = (fConst95 * ((fRec89[2] + (fRec89[0] + (2.0f * fRec89[1]))) * std::max<float>(0.0f, std::min<float>(fTemp108, (2.0f - fTemp108)))));
			fRec88[0] = (fTemp109 - ((fConst108 * (fRec88[1] * std::cos((fConst5 * fTemp80)))) + (fConst109 * fRec88[2])));
			fRec92[0] = (fTemp109 - ((fConst113 * (fRec92[1] * std::cos((fConst114 * fTemp80)))) + (fConst115 * fRec92[2])));
			fRec93[0] = (fTemp109 - ((fConst118 * (fRec93[1] * std::cos((fConst119 * fTemp80)))) + (fConst120 * fRec93[2])));
			fRec94[0] = (fTemp109 - ((fConst123 * (fRec94[1] * std::cos((fConst124 * fTemp80)))) + (fConst125 * fRec94[2])));
			fRec95[0] = (fTemp109 - ((fConst128 * (fRec95[1] * std::cos((fConst129 * fTemp80)))) + (fConst130 * fRec95[2])));
			fRec96[0] = (fTemp109 - ((fConst133 * (fRec96[1] * std::cos((fConst134 * fTemp80)))) + (fConst135 * fRec96[2])));
			fRec97[0] = (fTemp109 - ((fConst138 * (fRec97[1] * std::cos((fConst139 * fTemp80)))) + (fConst140 * fRec97[2])));
			fRec98[0] = (fTemp109 - ((fConst143 * (fRec98[1] * std::cos((fConst144 * fTemp80)))) + (fConst145 * fRec98[2])));
			fRec99[0] = (fTemp109 - ((fConst148 * (fRec99[1] * std::cos((fConst149 * fTemp80)))) + (fConst150 * fRec99[2])));
			fRec100[0] = (fTemp109 - ((fConst153 * (fRec100[1] * std::cos((fConst154 * fTemp80)))) + (fConst155 * fRec100[2])));
			fRec101[0] = (fTemp109 - ((fConst158 * (fRec101[1] * std::cos((fConst159 * fTemp80)))) + (fConst160 * fRec101[2])));
			fRec102[0] = (fTemp109 - ((fConst163 * (fRec102[1] * std::cos((fConst164 * fTemp80)))) + (fConst165 * fRec102[2])));
			fRec103[0] = (fTemp109 - ((fConst168 * (fRec103[1] * std::cos((fConst169 * fTemp80)))) + (fConst170 * fRec103[2])));
			fRec104[0] = (fTemp109 - ((fConst173 * (fRec104[1] * std::cos((fConst174 * fTemp80)))) + (fConst175 * fRec104[2])));
			fRec105[0] = (fTemp109 - ((fConst178 * (fRec105[1] * std::cos((fConst179 * fTemp80)))) + (fConst180 * fRec105[2])));
			fRec106[0] = (fTemp109 - ((fConst183 * (fRec106[1] * std::cos((fConst184 * fTemp80)))) + (fConst185 * fRec106[2])));
			fRec107[0] = (fTemp109 - ((fConst188 * (fRec107[1] * std::cos((fConst189 * fTemp80)))) + (fConst190 * fRec107[2])));
			fRec108[0] = (fTemp109 - ((fConst193 * (fRec108[1] * std::cos((fConst194 * fTemp80)))) + (fConst195 * fRec108[2])));
			fRec109[0] = (fTemp109 - ((fConst198 * (fRec109[1] * std::cos((fConst199 * fTemp80)))) + (fConst200 * fRec109[2])));
			fRec110[0] = (fTemp109 - ((fConst203 * (fRec110[1] * std::cos((fConst204 * fTemp80)))) + (fConst205 * fRec110[2])));
			fRec111[0] = (fTemp109 - ((fConst208 * (fRec111[1] * std::cos((fConst209 * fTemp80)))) + (fConst210 * fRec111[2])));
			fRec112[0] = (fTemp109 - ((fConst213 * (fRec112[1] * std::cos((fConst214 * fTemp80)))) + (fConst215 * fRec112[2])));
			fRec113[0] = (fTemp109 - ((fConst218 * (fRec113[1] * std::cos((fConst219 * fTemp80)))) + (fConst220 * fRec113[2])));
			fRec114[0] = (fTemp109 - ((fConst223 * (fRec114[1] * std::cos((fConst224 * fTemp80)))) + (fConst225 * fRec114[2])));
			fRec115[0] = (fTemp109 - ((fConst228 * (fRec115[1] * std::cos((fConst229 * fTemp80)))) + (fConst230 * fRec115[2])));
			fRec116[0] = (fTemp109 - ((fConst233 * (fRec116[1] * std::cos((fConst234 * fTemp80)))) + (fConst235 * fRec116[2])));
			fRec117[0] = (fTemp109 - ((fConst238 * (fRec117[1] * std::cos((fConst239 * fTemp80)))) + (fConst240 * fRec117[2])));
			fRec118[0] = (fTemp109 - ((fConst243 * (fRec118[1] * std::cos((fConst244 * fTemp80)))) + (fConst245 * fRec118[2])));
			fRec119[0] = (fTemp109 - ((fConst248 * (fRec119[1] * std::cos((fConst249 * fTemp80)))) + (fConst250 * fRec119[2])));
			fRec120[0] = (fTemp109 - ((fConst253 * (fRec120[1] * std::cos((fConst254 * fTemp80)))) + (fConst255 * fRec120[2])));
			fRec121[0] = (fTemp109 - ((fConst258 * (fRec121[1] * std::cos((fConst259 * fTemp80)))) + (fConst260 * fRec121[2])));
			fRec122[0] = (fTemp109 - ((fConst263 * (fRec122[1] * std::cos((fConst264 * fTemp80)))) + (fConst265 * fRec122[2])));
			fRec123[0] = (fTemp109 - ((fConst268 * (fRec123[1] * std::cos((fConst269 * fTemp80)))) + (fConst270 * fRec123[2])));
			fRec124[0] = (fTemp109 - ((fConst273 * (fRec124[1] * std::cos((fConst274 * fTemp80)))) + (fConst275 * fRec124[2])));
			fRec125[0] = (fTemp109 - ((fConst278 * (fRec125[1] * std::cos((fConst279 * fTemp80)))) + (fConst280 * fRec125[2])));
			fRec126[0] = (fTemp109 - ((fConst283 * (fRec126[1] * std::cos((fConst284 * fTemp80)))) + (fConst285 * fRec126[2])));
			fRec127[0] = (fTemp109 - ((fConst288 * (fRec127[1] * std::cos((fConst289 * fTemp80)))) + (fConst290 * fRec127[2])));
			fRec128[0] = (fTemp109 - ((fConst293 * (fRec128[1] * std::cos((fConst294 * fTemp80)))) + (fConst295 * fRec128[2])));
			fRec129[0] = (fTemp109 - ((fConst298 * (fRec129[1] * std::cos((fConst299 * fTemp80)))) + (fConst300 * fRec129[2])));
			fRec130[0] = (fTemp109 - ((fConst303 * (fRec130[1] * std::cos((fConst304 * fTemp80)))) + (fConst305 * fRec130[2])));
			fRec131[0] = (fTemp109 - ((fConst308 * (fRec131[1] * std::cos((fConst309 * fTemp80)))) + (fConst310 * fRec131[2])));
			fRec132[0] = (fTemp109 - ((fConst313 * (fRec132[1] * std::cos((fConst314 * fTemp80)))) + (fConst315 * fRec132[2])));
			fRec133[0] = (fTemp109 - ((fConst318 * (fRec133[1] * std::cos((fConst319 * fTemp80)))) + (fConst320 * fRec133[2])));
			fRec134[0] = (fTemp109 - ((fConst323 * (fRec134[1] * std::cos((fConst324 * fTemp80)))) + (fConst325 * fRec134[2])));
			fRec135[0] = (fTemp109 - ((fConst328 * (fRec135[1] * std::cos((fConst329 * fTemp80)))) + (fConst330 * fRec135[2])));
			fRec136[0] = (fTemp109 - ((fConst333 * (fRec136[1] * std::cos((fConst334 * fTemp80)))) + (fConst335 * fRec136[2])));
			fRec137[0] = (fTemp109 - ((fConst338 * (fRec137[1] * std::cos((fConst339 * fTemp80)))) + (fConst340 * fRec137[2])));
			fRec138[0] = (fTemp109 - ((fConst343 * (fRec138[1] * std::cos((fConst344 * fTemp80)))) + (fConst345 * fRec138[2])));
			fRec139[0] = (fTemp109 - ((fConst348 * (fRec139[1] * std::cos((fConst349 * fTemp80)))) + (fConst350 * fRec139[2])));
			fRec140[0] = (fTemp109 - ((fConst353 * (fRec140[1] * std::cos((fConst354 * fTemp80)))) + (fConst355 * fRec140[2])));
			fVec14[0] = (((((((((((((((((((((((((((((((((((((((((((((((((((fRec88[0] - fRec88[2]) * ((fTemp80 < fConst110) ? fConst111 : 0.0f)) + ((fRec92[0] - fRec92[2]) * (((3.31356001f * fTemp80) < fConst110) ? fConst116 : 0.0f))) + ((fRec93[0] - fRec93[2]) * (((3.83469009f * fTemp80) < fConst110) ? fConst121 : 0.0f))) + ((fRec94[0] - fRec94[2]) * (((8.06313038f * fTemp80) < fConst110) ? fConst126 : 0.0f))) + ((fRec95[0] - fRec95[2]) * (((9.44777966f * fTemp80) < fConst110) ? fConst131 : 0.0f))) + ((fRec96[0] - fRec96[2]) * (((14.1169004f * fTemp80) < fConst110) ? fConst136 : 0.0f))) + ((fRec97[0] - fRec97[2]) * (((18.3840008f * fTemp80) < fConst110) ? fConst141 : 0.0f))) + ((fRec98[0] - fRec98[2]) * (((21.0102005f * fTemp80) < fConst110) ? fConst146 : 0.0f))) + ((fRec99[0] - fRec99[2]) * (((26.1774998f * fTemp80) < fConst110) ? fConst151 : 0.0f))) + ((fRec100[0] - fRec100[2]) * (((28.9944f * fTemp80) < fConst110) ? fConst156 : 0.0f))) + ((fRec101[0] - fRec101[2]) * (((37.0727997f * fTemp80) < fConst110) ? fConst161 : 0.0f))) + ((fRec102[0] - fRec102[2]) * (((37.8703003f * fTemp80) < fConst110) ? fConst166 : 0.0f))) + ((fRec103[0] - fRec103[2]) * (((40.0634003f * fTemp80) < fConst110) ? fConst171 : 0.0f))) + ((fRec104[0] - fRec104[2]) * (((47.6439018f * fTemp80) < fConst110) ? fConst176 : 0.0f))) + ((fRec105[0] - fRec105[2]) * (((51.019001f * fTemp80) < fConst110) ? fConst181 : 0.0f))) + ((fRec106[0] - fRec106[2]) * (((52.4300003f * fTemp80) < fConst110) ? fConst186 : 0.0f))) + ((fRec107[0] - fRec107[2]) * (((58.2859993f * fTemp80) < fConst110) ? fConst191 : 0.0f))) + ((fRec108[0] - fRec108[2]) * (((63.5485992f * fTemp80) < fConst110) ? fConst196 : 0.0f))) + ((fRec109[0] - fRec109[2]) * (((65.3628006f * fTemp80) < fConst110) ? fConst201 : 0.0f))) + ((fRec110[0] - fRec110[2]) * (((66.9587021f * fTemp80) < fConst110) ? fConst206 : 0.0f))) + ((fRec111[0] - fRec111[2]) * (((74.530098f * fTemp80) < fConst110) ? fConst211 : 0.0f))) + ((fRec112[0] - fRec112[2]) * (((78.6920013f * fTemp80) < fConst110) ? fConst216 : 0.0f))) + ((fRec113[0] - fRec113[2]) * (((80.8375015f * fTemp80) < fConst110) ? fConst221 : 0.0f))) + ((fRec114[0] - fRec114[2]) * (((89.9779968f * fTemp80) < fConst110) ? fConst226 : 0.0f))) + ((fRec115[0] - fRec115[2]) * (((92.9661026f * fTemp80) < fConst110) ? fConst231 : 0.0f))) + ((fRec116[0] - fRec116[2]) * (((95.1913986f * fTemp80) < fConst110) ? fConst236 : 0.0f))) + ((fRec117[0] - fRec117[2]) * (((97.4806976f * fTemp80) < fConst110) ? fConst241 : 0.0f))) + ((fRec118[0] - fRec118[2]) * (((110.620003f * fTemp80) < fConst110) ? fConst246 : 0.0f))) + ((fRec119[0] - fRec119[2]) * (((112.069f * fTemp80) < fConst110) ? fConst251 : 0.0f))) + ((fRec120[0] - fRec120[2]) * (((113.825996f * fTemp80) < fConst110) ? fConst256 : 0.0f))) + ((fRec121[0] - fRec121[2]) * (((119.356003f * fTemp80) < fConst110) ? fConst261 : 0.0f))) + ((fRec122[0] - fRec122[2]) * (((127.044998f * fTemp80) < fConst110) ? fConst266 : 0.0f))) + ((fRec123[0] - fRec123[2]) * (((129.981995f * fTemp80) < fConst110) ? fConst271 : 0.0f))) + ((fRec124[0] - fRec124[2]) * (((132.259003f * fTemp80) < fConst110) ? fConst276 : 0.0f))) + ((fRec125[0] - fRec125[2]) * (((133.477005f * fTemp80) < fConst110) ? fConst281 : 0.0f))) + ((fRec126[0] - fRec126[2]) * (((144.548996f * fTemp80) < fConst110) ? fConst286 : 0.0f))) + ((fRec127[0] - fRec127[2]) * (((149.438004f * fTemp80) < fConst110) ? fConst291 : 0.0f))) + ((fRec128[0] - fRec128[2]) * (((152.033005f * fTemp80) < fConst110) ? fConst296 : 0.0f))) + ((fRec129[0] - fRec129[2]) * (((153.166f * fTemp80) < fConst110) ? fConst301 : 0.0f))) + ((fRec130[0] - fRec130[2]) * (((155.597f * fTemp80) < fConst110) ? fConst306 : 0.0f))) + ((fRec131[0] - fRec131[2]) * (((158.182999f * fTemp80) < fConst110) ? fConst311 : 0.0f))) + ((fRec132[0] - fRec132[2]) * (((168.104996f * fTemp80) < fConst110) ? fConst316 : 0.0f))) + ((fRec133[0] - fRec133[2]) * (((171.863007f * fTemp80) < fConst110) ? fConst321 : 0.0f))) + ((fRec134[0] - fRec134[2]) * (((174.464005f * fTemp80) < fConst110) ? fConst326 : 0.0f))) + ((fRec135[0] - fRec135[2]) * (((178.936996f * fTemp80) < fConst110) ? fConst331 : 0.0f))) + ((fRec136[0] - fRec136[2]) * (((181.481995f * fTemp80) < fConst110) ? fConst336 : 0.0f))) + ((fRec137[0] - fRec137[2]) * (((185.397995f * fTemp80) < fConst110) ? fConst341 : 0.0f))) + ((fRec138[0] - fRec138[2]) * (((190.369003f * fTemp80) < fConst110) ? fConst346 : 0.0f))) + ((fRec139[0] - fRec139[2]) * (((192.190002f * fTemp80) < fConst110) ? fConst351 : 0.0f))) + ((fRec140[0] - fRec140[2]) * (((195.505005f * fTemp80) < fConst110) ? fConst356 : 0.0f)));
			float fTemp110 = ((0.99000001f * fRec87[0]) + (0.0199999996f * fVec14[1]));
			fVec15[(IOTA & 2047)] = fTemp110;
			fRec84[0] = (((((fTemp87 * fTemp89) * fTemp91) * fTemp93) * fVec15[((IOTA - iTemp84) & 2047)]) + (fTemp94 * ((((((fTemp96 * fTemp97) * fTemp98) * fVec15[((IOTA - iTemp95) & 2047)]) + (0.5f * (((fTemp86 * fTemp100) * fTemp101) * fVec15[((IOTA - iTemp99) & 2047)]))) + (0.166666672f * ((fTemp102 * fTemp104) * fVec15[((IOTA - iTemp103) & 2047)]))) + (0.0416666679f * (fTemp105 * fVec15[((IOTA - iTemp106) & 2047)])))));
			float fRec82 = fRec84[0];
			float fTemp111 = std::fabs((0.25f * fRec82));
			fRec143[0] = std::max<float>(fTemp111, ((fRec143[1] * fSlow329) + (fTemp111 * fSlow330)));
			fRec142[0] = ((fRec142[1] * fSlow326) + (fRec143[0] * fSlow331));
			fRec141[0] = ((fConst64 * fRec141[1]) + (fSlow323 * std::max<float>(((20.0f * std::log10(fRec142[0])) - fSlow332), 0.0f)));
			float fTemp112 = (0.25f * (fRec82 * std::pow(10.0f, (0.0500000007f * fRec141[0]))));
			fRec144[0] = (fTemp112 - (fSlow336 * ((fSlow337 * fRec144[2]) + (fSlow338 * fRec144[1]))));
			float fTemp113 = (fSlow319 * fRec80[1]);
			fRec80[0] = (fTemp112 - ((fSlow336 * (fRec144[2] + (fRec144[0] + (2.0f * fRec144[1])))) + (((fRec80[2] * fSlow346) + fTemp113) / fSlow347)));
			float fTemp114 = (fSlow316 * fRec79[1]);
			fRec79[0] = ((((fTemp113 + (fRec80[0] * fSlow349)) + (fRec80[2] * fSlow350)) / fSlow347) - (((fRec79[2] * fSlow358) + fTemp114) / fSlow359));
			fRec78[0] = ((((fTemp114 + (fRec79[0] * fSlow361)) + (fRec79[2] * fSlow362)) / fSlow359) - (fSlow363 * ((fSlow364 * fRec78[2]) + (fSlow365 * fRec78[1]))));
			float fTemp115 = (fRec78[2] + (fRec78[0] + (2.0f * fRec78[1])));
			fRec148[0] = ((0.999000013f * fRec148[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp47, ((fConst357 * fTemp48) + 1.0f))))));
			fRec150[0] = (iTemp49 ? fSlow377 : fRec150[1]);
			float fTemp116 = (fRec154[1] + (fSlow378 * fRec150[0]));
			fRec154[0] = (fTemp116 - std::floor(fTemp116));
			float fTemp117 = (fRec153[1] + (fConst66 * ((315.0f * (fRec148[0] * ftbl0mydspSIG0[int((65536.0f * fRec154[0]))])) + (fSlow379 * fRec150[0]))));
			fRec153[0] = (fTemp117 - std::floor(fTemp117));
			float fTemp118 = (fRec152[1] + (fConst66 * ((315.0f * (fRec148[0] * ftbl0mydspSIG0[int((65536.0f * fRec153[0]))])) + (fSlow380 * fRec150[0]))));
			fRec152[0] = (fTemp118 - std::floor(fTemp118));
			float fTemp119 = (fRec151[1] + (fConst66 * ((315.0f * (fRec148[0] * ftbl0mydspSIG0[int((65536.0f * fRec152[0]))])) + (fSlow381 * fRec150[0]))));
			fRec151[0] = (fTemp119 - std::floor(fTemp119));
			float fTemp120 = (fRec149[1] + (fConst66 * ((fSlow89 * fRec150[0]) + (315.0f * (fRec148[0] * ftbl0mydspSIG0[int((65536.0f * fRec151[0]))])))));
			fRec149[0] = (fTemp120 - std::floor(fTemp120));
			float fTemp121 = ftbl0mydspSIG0[int((65536.0f * fRec149[0]))];
			float fTemp122 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp121)));
			float fTemp123 = (fRec148[0] * (fTemp121 + (0.0500000007f * (fTemp122 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp122)))))));
			float fTemp124 = std::fabs((0.25f * fTemp123));
			fRec157[0] = std::max<float>(fTemp124, ((fRec157[1] * fSlow388) + (fTemp124 * fSlow389)));
			fRec156[0] = ((fRec156[1] * fSlow385) + (fRec157[0] * fSlow390));
			fRec155[0] = ((fConst64 * fRec155[1]) + (fSlow382 * std::max<float>(((20.0f * std::log10(fRec156[0])) - fSlow391), 0.0f)));
			float fTemp125 = (0.25f * (fTemp123 * std::pow(10.0f, (0.0500000007f * fRec155[0]))));
			fRec158[0] = (fTemp125 - (fSlow395 * ((fSlow396 * fRec158[2]) + (fSlow397 * fRec158[1]))));
			float fTemp126 = (fSlow376 * fRec147[1]);
			fRec147[0] = (fTemp125 - ((fSlow395 * (fRec158[2] + (fRec158[0] + (2.0f * fRec158[1])))) + (((fRec147[2] * fSlow405) + fTemp126) / fSlow406)));
			float fTemp127 = (fSlow373 * fRec146[1]);
			fRec146[0] = ((((fTemp126 + (fRec147[0] * fSlow408)) + (fRec147[2] * fSlow409)) / fSlow406) - (((fRec146[2] * fSlow417) + fTemp127) / fSlow418));
			fRec145[0] = ((((fTemp127 + (fRec146[0] * fSlow420)) + (fRec146[2] * fSlow421)) / fSlow418) - (fSlow422 * ((fSlow423 * fRec145[2]) + (fSlow424 * fRec145[1]))));
			float fTemp128 = (fSlow370 * (fRec145[2] + (fRec145[0] + (2.0f * fRec145[1]))));
			float fRec169 = (0.0f - (0.997305274f * ((0.899999976f * fRec170[2]) + (0.0500000007f * (fRec170[1] + fRec170[3])))));
			fVec16[0] = fSlow436;
			float fTemp129 = (fSlow436 - fVec16[1]);
			fVec17[0] = fSlow438;
			float fTemp130 = (0.0f - (fSlow436 + fVec17[1]));
			float fTemp131 = std::min<float>(1.0f, ((fTemp129 * float((fTemp129 > 0.0f))) + (fSlow437 * (fTemp130 * float((fTemp130 > 0.0f))))));
			fVec18[0] = fTemp131;
			int iTemp132 = int(fTemp131);
			fRec188[0] = (iTemp132 ? fSlow439 : fRec188[1]);
			float fTemp133 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow90 * (fRec40[0] * fRec188[0]))))) + -0.109999999f);
			float fTemp134 = (fConst359 * fTemp133);
			float fTemp135 = (fTemp134 + -1.49999499f);
			int iTemp136 = int(fTemp135);
			int iTemp137 = int(std::min<float>(fConst91, float(std::max<int>(0, iTemp136))));
			float fTemp138 = std::floor(fTemp135);
			float fTemp139 = (fTemp134 + (-1.0f - fTemp138));
			float fTemp140 = (0.0f - fTemp139);
			float fTemp141 = (fTemp134 + (-2.0f - fTemp138));
			float fTemp142 = (0.0f - (0.5f * fTemp141));
			float fTemp143 = (fTemp134 + (-3.0f - fTemp138));
			float fTemp144 = (0.0f - (0.333333343f * fTemp143));
			float fTemp145 = (fTemp134 + (-4.0f - fTemp138));
			float fTemp146 = (0.0f - (0.25f * fTemp145));
			float fTemp147 = (fTemp134 - fTemp138);
			int iTemp148 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp136 + 1)))));
			float fTemp149 = (0.0f - fTemp141);
			float fTemp150 = (0.0f - (0.5f * fTemp143));
			float fTemp151 = (0.0f - (0.333333343f * fTemp145));
			int iTemp152 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp136 + 2)))));
			float fTemp153 = (0.0f - fTemp143);
			float fTemp154 = (0.0f - (0.5f * fTemp145));
			float fTemp155 = (fTemp139 * fTemp141);
			int iTemp156 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp136 + 3)))));
			float fTemp157 = (0.0f - fTemp145);
			float fTemp158 = (fTemp155 * fTemp143);
			int iTemp159 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp136 + 4)))));
			fRec184[0] = (((((fRec165[((IOTA - (iTemp137 + 1)) & 2047)] * fTemp140) * fTemp142) * fTemp144) * fTemp146) + (fTemp147 * ((((((fRec165[((IOTA - (iTemp148 + 1)) & 2047)] * fTemp149) * fTemp150) * fTemp151) + (0.5f * (((fTemp139 * fRec165[((IOTA - (iTemp152 + 1)) & 2047)]) * fTemp153) * fTemp154))) + (0.166666672f * ((fTemp155 * fRec165[((IOTA - (iTemp156 + 1)) & 2047)]) * fTemp157))) + (0.0416666679f * (fTemp158 * fRec165[((IOTA - (iTemp159 + 1)) & 2047)])))));
			fRec189[0] = ((fSlow440 * fRec189[1]) + (fSlow441 * fRec184[1]));
			float fRec185 = fRec189[0];
			fRec191[0] = fRec163[1];
			fRec192[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec191[2]) + (0.0500000007f * (fRec191[1] + fRec191[3])))));
			float fTemp160 = (fConst360 * fTemp133);
			float fTemp161 = (fTemp160 + -1.49999499f);
			int iTemp162 = int(fTemp161);
			int iTemp163 = int(std::min<float>(fConst91, float(std::max<int>(0, iTemp162))));
			float fTemp164 = std::floor(fTemp161);
			float fTemp165 = (fTemp160 + (-1.0f - fTemp164));
			float fTemp166 = (0.0f - fTemp165);
			float fTemp167 = (fTemp160 + (-2.0f - fTemp164));
			float fTemp168 = (0.0f - (0.5f * fTemp167));
			float fTemp169 = (fTemp160 + (-3.0f - fTemp164));
			float fTemp170 = (0.0f - (0.333333343f * fTemp169));
			float fTemp171 = (fTemp160 + (-4.0f - fTemp164));
			float fTemp172 = (0.0f - (0.25f * fTemp171));
			float fTemp173 = (fTemp160 - fTemp164);
			int iTemp174 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp162 + 1)))));
			float fTemp175 = (0.0f - fTemp167);
			float fTemp176 = (0.0f - (0.5f * fTemp169));
			float fTemp177 = (0.0f - (0.333333343f * fTemp171));
			int iTemp178 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp162 + 2)))));
			float fTemp179 = (0.0f - fTemp169);
			float fTemp180 = (0.0f - (0.5f * fTemp171));
			float fTemp181 = (fTemp165 * fTemp167);
			int iTemp182 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp162 + 3)))));
			float fTemp183 = (0.0f - fTemp171);
			float fTemp184 = (fTemp181 * fTemp169);
			int iTemp185 = int(std::min<float>(fConst91, float(std::max<int>(0, (iTemp162 + 4)))));
			fVec19[0] = (((((fRec192[((IOTA - (iTemp163 + 2)) & 1023)] * fTemp166) * fTemp168) * fTemp170) * fTemp172) + (fTemp173 * ((((((fRec192[((IOTA - (iTemp174 + 2)) & 1023)] * fTemp175) * fTemp176) * fTemp177) + (0.5f * (((fTemp165 * fRec192[((IOTA - (iTemp178 + 2)) & 1023)]) * fTemp179) * fTemp180))) + (0.166666672f * ((fTemp181 * fRec192[((IOTA - (iTemp182 + 2)) & 1023)]) * fTemp183))) + (0.0416666679f * (fTemp184 * fRec192[((IOTA - (iTemp185 + 2)) & 1023)])))));
			float fTemp186 = (fTemp131 + fVec19[1]);
			fVec20[0] = fTemp186;
			fRec190[(IOTA & 2047)] = ((fSlow440 * fRec190[((IOTA - 1) & 2047)]) + (fSlow441 * fVec20[2]));
			float fRec186 = (((((fTemp140 * fTemp142) * fTemp144) * fTemp146) * fRec190[((IOTA - iTemp137) & 2047)]) + (fTemp147 * ((((((fTemp149 * fTemp150) * fTemp151) * fRec190[((IOTA - iTemp148) & 2047)]) + (0.5f * (((fTemp139 * fTemp153) * fTemp154) * fRec190[((IOTA - iTemp152) & 2047)]))) + (0.166666672f * ((fTemp155 * fTemp157) * fRec190[((IOTA - iTemp156) & 2047)]))) + (0.0416666679f * (fTemp158 * fRec190[((IOTA - iTemp159) & 2047)])))));
			float fRec187 = (fVec20[1] + fRec180[1]);
			fRec180[0] = fRec185;
			float fRec181 = fRec180[1];
			float fRec182 = fRec186;
			float fRec183 = fRec187;
			fRec176[0] = fRec181;
			float fRec177 = (fTemp131 + fRec176[1]);
			float fRec178 = fRec182;
			float fRec179 = fRec183;
			fRec172[(IOTA & 1023)] = fRec177;
			float fRec173 = (((((fTemp166 * fTemp168) * fTemp170) * fTemp172) * fRec172[((IOTA - (iTemp163 + 1)) & 1023)]) + (fTemp173 * ((((((fTemp175 * fTemp176) * fTemp177) * fRec172[((IOTA - (iTemp174 + 1)) & 1023)]) + (0.5f * (((fTemp165 * fTemp179) * fTemp180) * fRec172[((IOTA - (iTemp178 + 1)) & 1023)]))) + (0.166666672f * ((fTemp181 * fTemp183) * fRec172[((IOTA - (iTemp182 + 1)) & 1023)]))) + (0.0416666679f * (fTemp184 * fRec172[((IOTA - (iTemp185 + 1)) & 1023)])))));
			float fRec174 = fRec178;
			float fRec175 = fRec179;
			fRec170[0] = fRec174;
			float fRec171 = fRec175;
			fRec165[(IOTA & 2047)] = fRec169;
			float fRec166 = fRec173;
			float fRec167 = fRec170[0];
			float fRec168 = fRec171;
			fRec163[0] = fRec166;
			float fRec164 = fRec168;
			fRec193[0] = (fSlow442 + (0.999000013f * fRec193[1]));
			float fTemp187 = std::min<float>(15000.0f, std::max<float>(500.0f, fRec193[0]));
			float fTemp188 = std::tan((fConst1 * fTemp187));
			float fTemp189 = (1.0f / fTemp188);
			float fTemp190 = (((fTemp189 + 1.42857146f) / fTemp188) + 1.0f);
			fRec162[0] = ((1.5f * fRec164) - (((fRec162[2] * (((fTemp189 + -1.42857146f) / fTemp188) + 1.0f)) + (2.0f * (fRec162[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp188)))))) / fTemp190));
			iRec194[0] = (((iRec194[1] + (iRec194[1] > 0)) * (fTemp131 <= fVec18[1])) + (fTemp131 > fVec18[1]));
			float fTemp191 = float(iRec194[0]);
			iRec195[0] = (iTemp132 ? 7 : iRec195[1]);
			float fTemp192 = ((((fRec162[2] + (fRec162[0] + (2.0f * fRec162[1]))) * ((6.66666674e-05f * (fTemp187 + -500.0f)) + 1.0f)) * std::max<float>(0.0f, std::min<float>((fConst82 * fTemp191), ((fConst83 * (fConst81 - fTemp191)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec195[0] + -10)))));
			float fTemp193 = std::fabs((0.25f * (fTemp192 / fTemp190)));
			fRec198[0] = std::max<float>(fTemp193, ((fRec198[1] * fSlow449) + (fTemp193 * fSlow450)));
			fRec197[0] = ((fRec197[1] * fSlow446) + (fRec198[0] * fSlow451));
			fRec196[0] = ((fConst64 * fRec196[1]) + (fSlow443 * std::max<float>(((20.0f * std::log10(fRec197[0])) - fSlow452), 0.0f)));
			float fTemp194 = (0.25f * ((fTemp192 * std::pow(10.0f, (0.0500000007f * fRec196[0]))) / fTemp190));
			fRec199[0] = (fTemp194 - (fSlow456 * ((fSlow457 * fRec199[2]) + (fSlow458 * fRec199[1]))));
			float fTemp195 = (fSlow435 * fRec161[1]);
			fRec161[0] = (fTemp194 - ((fSlow456 * (fRec199[2] + (fRec199[0] + (2.0f * fRec199[1])))) + (((fRec161[2] * fSlow466) + fTemp195) / fSlow467)));
			float fTemp196 = (fSlow432 * fRec160[1]);
			fRec160[0] = ((((fTemp195 + (fRec161[0] * fSlow469)) + (fRec161[2] * fSlow470)) / fSlow467) - (((fRec160[2] * fSlow478) + fTemp196) / fSlow479));
			fRec159[0] = ((((fTemp196 + (fRec160[0] * fSlow481)) + (fRec160[2] * fSlow482)) / fSlow479) - (fSlow483 * ((fSlow484 * fRec159[2]) + (fSlow485 * fRec159[1]))));
			float fTemp197 = (fSlow429 * (fRec159[2] + (fRec159[0] + (2.0f * fRec159[1]))));
			fRec204[0] = (fTemp107 - (fConst364 * ((fConst365 * fRec204[2]) + (fConst366 * fRec204[1]))));
			fVec21[0] = fSlow497;
			float fTemp198 = (fSlow497 - fVec21[1]);
			fVec22[0] = fSlow499;
			float fTemp199 = (0.0f - (fSlow497 + fVec22[1]));
			float fTemp200 = std::min<float>(1.0f, ((fTemp198 * float((fTemp198 > 0.0f))) + (fSlow498 * (fTemp199 * float((fTemp199 > 0.0f))))));
			fVec23[0] = fTemp200;
			iRec205[0] = (((iRec205[1] + (iRec205[1] > 0)) * (fTemp200 <= fVec23[1])) + (fTemp200 > fVec23[1]));
			float fTemp201 = float(iRec205[0]);
			float fTemp202 = (fConst367 * fTemp201);
			float fTemp203 = (fConst364 * ((fRec204[2] + (fRec204[0] + (2.0f * fRec204[1]))) * std::max<float>(0.0f, std::min<float>(fTemp202, (2.0f - fTemp202)))));
			fRec203[0] = (fTemp203 - ((fConst369 * fRec203[1]) + (fConst370 * fRec203[2])));
			fRec206[0] = (fTemp203 - ((fConst373 * fRec206[1]) + (fConst374 * fRec206[2])));
			fRec207[0] = (fTemp203 - ((fConst377 * fRec207[1]) + (fConst378 * fRec207[2])));
			fRec208[0] = (fTemp203 - ((fConst381 * fRec208[1]) + (fConst382 * fRec208[2])));
			fRec209[0] = (fTemp203 - ((fConst385 * fRec209[1]) + (fConst386 * fRec209[2])));
			fRec210[0] = (fTemp203 - ((fConst389 * fRec210[1]) + (fConst390 * fRec210[2])));
			fRec211[0] = (fTemp203 - ((fConst393 * fRec211[1]) + (fConst394 * fRec211[2])));
			fRec212[0] = (fTemp203 - ((fConst397 * fRec212[1]) + (fConst398 * fRec212[2])));
			fRec213[0] = (fTemp203 - ((fConst401 * fRec213[1]) + (fConst402 * fRec213[2])));
			fRec214[0] = (fTemp203 - ((fConst405 * fRec214[1]) + (fConst406 * fRec214[2])));
			fRec215[0] = (fTemp203 - ((fConst409 * fRec215[1]) + (fConst410 * fRec215[2])));
			fRec216[0] = (fTemp203 - ((fConst413 * fRec216[1]) + (fConst414 * fRec216[2])));
			fRec217[0] = (fTemp203 - ((fConst417 * fRec217[1]) + (fConst418 * fRec217[2])));
			fRec218[0] = (fTemp203 - ((fConst421 * fRec218[1]) + (fConst422 * fRec218[2])));
			fRec219[0] = (fTemp203 - ((fConst425 * fRec219[1]) + (fConst426 * fRec219[2])));
			fRec220[0] = (fTemp203 - ((fConst429 * fRec220[1]) + (fConst430 * fRec220[2])));
			fRec221[0] = (fTemp203 - ((fConst433 * fRec221[1]) + (fConst434 * fRec221[2])));
			fRec222[0] = (fTemp203 - ((fConst437 * fRec222[1]) + (fConst438 * fRec222[2])));
			fRec223[0] = (fTemp203 - ((fConst441 * fRec223[1]) + (fConst442 * fRec223[2])));
			fRec224[0] = (fTemp203 - ((fConst445 * fRec224[1]) + (fConst446 * fRec224[2])));
			fRec225[0] = (fTemp203 - ((fConst449 * fRec225[1]) + (fConst450 * fRec225[2])));
			fRec226[0] = (fTemp203 - ((fConst453 * fRec226[1]) + (fConst454 * fRec226[2])));
			fRec227[0] = (fTemp203 - ((fConst457 * fRec227[1]) + (fConst458 * fRec227[2])));
			fRec228[0] = (fTemp203 - ((fConst461 * fRec228[1]) + (fConst462 * fRec228[2])));
			fRec229[0] = (fTemp203 - ((fConst465 * fRec229[1]) + (fConst466 * fRec229[2])));
			fRec230[0] = (fTemp203 - ((fConst469 * fRec230[1]) + (fConst470 * fRec230[2])));
			fRec231[0] = (fTemp203 - ((fConst473 * fRec231[1]) + (fConst474 * fRec231[2])));
			fRec232[0] = (fTemp203 - ((fConst477 * fRec232[1]) + (fConst478 * fRec232[2])));
			fRec233[0] = (fTemp203 - ((fConst481 * fRec233[1]) + (fConst482 * fRec233[2])));
			fRec234[0] = (fTemp203 - ((fConst485 * fRec234[1]) + (fConst486 * fRec234[2])));
			fRec235[0] = (fTemp203 - ((fConst489 * fRec235[1]) + (fConst490 * fRec235[2])));
			fRec236[0] = (fTemp203 - ((fConst493 * fRec236[1]) + (fConst494 * fRec236[2])));
			fRec237[0] = (fTemp203 - ((fConst497 * fRec237[1]) + (fConst498 * fRec237[2])));
			fRec238[0] = (fTemp203 - ((fConst501 * fRec238[1]) + (fConst502 * fRec238[2])));
			fRec239[0] = (fTemp203 - ((fConst505 * fRec239[1]) + (fConst506 * fRec239[2])));
			fRec240[0] = (fTemp203 - ((fConst509 * fRec240[1]) + (fConst510 * fRec240[2])));
			fRec241[0] = (fTemp203 - ((fConst513 * fRec241[1]) + (fConst514 * fRec241[2])));
			fRec242[0] = (fTemp203 - ((fConst517 * fRec242[1]) + (fConst518 * fRec242[2])));
			fRec243[0] = (fTemp203 - ((fConst521 * fRec243[1]) + (fConst522 * fRec243[2])));
			fRec244[0] = (fTemp203 - ((fConst525 * fRec244[1]) + (fConst526 * fRec244[2])));
			fRec245[0] = (fTemp203 - ((fConst529 * fRec245[1]) + (fConst530 * fRec245[2])));
			fRec246[0] = (fTemp203 - ((fConst533 * fRec246[1]) + (fConst534 * fRec246[2])));
			fRec247[0] = (fTemp203 - ((fConst537 * fRec247[1]) + (fConst538 * fRec247[2])));
			fRec248[0] = (fTemp203 - ((fConst541 * fRec248[1]) + (fConst542 * fRec248[2])));
			fRec249[0] = (fTemp203 - ((fConst545 * fRec249[1]) + (fConst546 * fRec249[2])));
			fRec250[0] = (fTemp203 - ((fConst549 * fRec250[1]) + (fConst550 * fRec250[2])));
			fRec251[0] = (fTemp203 - ((fConst553 * fRec251[1]) + (fConst554 * fRec251[2])));
			fRec252[0] = (fTemp203 - ((fConst557 * fRec252[1]) + (fConst558 * fRec252[2])));
			fRec253[0] = (fTemp203 - ((fConst561 * fRec253[1]) + (fConst562 * fRec253[2])));
			fRec254[0] = (fTemp203 - ((fConst565 * fRec254[1]) + (fConst566 * fRec254[2])));
			fRec255[0] = (fSlow501 + (0.999000013f * fRec255[1]));
			fRec256[0] = (fSlow502 + (fRec256[1] - std::floor((fSlow502 + fRec256[1]))));
			fRec257[0] = (fSlow504 + (0.999000013f * fRec257[1]));
			fRec258[0] = (fSlow505 + (fRec258[1] - std::floor((fSlow505 + fRec258[1]))));
			fRec259[0] = (fSlow507 + (0.999000013f * fRec259[1]));
			fRec260[0] = (fSlow508 + (fRec260[1] - std::floor((fSlow508 + fRec260[1]))));
			float fTemp204 = ((0.0199999996f * (((((((((((((((((((((((((((((((((((((((((((((((((((fConst361 * (fRec203[0] - fRec203[2])) + (fConst371 * (fRec206[0] - fRec206[2]))) + (fConst375 * (fRec207[0] - fRec207[2]))) + (fConst379 * (fRec208[0] - fRec208[2]))) + (fConst383 * (fRec209[0] - fRec209[2]))) + (fConst387 * (fRec210[0] - fRec210[2]))) + (fConst391 * (fRec211[0] - fRec211[2]))) + (fConst395 * (fRec212[0] - fRec212[2]))) + (fConst399 * (fRec213[0] - fRec213[2]))) + (fConst403 * (fRec214[0] - fRec214[2]))) + (fConst407 * (fRec215[0] - fRec215[2]))) + (fConst411 * (fRec216[0] - fRec216[2]))) + (fConst415 * (fRec217[0] - fRec217[2]))) + (fConst419 * (fRec218[0] - fRec218[2]))) + (fConst423 * (fRec219[0] - fRec219[2]))) + (fConst427 * (fRec220[0] - fRec220[2]))) + (fConst431 * (fRec221[0] - fRec221[2]))) + (fConst435 * (fRec222[0] - fRec222[2]))) + (fConst439 * (fRec223[0] - fRec223[2]))) + (fConst443 * (fRec224[0] - fRec224[2]))) + (fConst447 * (fRec225[0] - fRec225[2]))) + (fConst451 * (fRec226[0] - fRec226[2]))) + (fConst455 * (fRec227[0] - fRec227[2]))) + (fConst459 * (fRec228[0] - fRec228[2]))) + (fConst463 * (fRec229[0] - fRec229[2]))) + (fConst467 * (fRec230[0] - fRec230[2]))) + (fConst471 * (fRec231[0] - fRec231[2]))) + (fConst475 * (fRec232[0] - fRec232[2]))) + (fConst479 * (fRec233[0] - fRec233[2]))) + (fConst483 * (fRec234[0] - fRec234[2]))) + (fConst487 * (fRec235[0] - fRec235[2]))) + (fConst491 * (fRec236[0] - fRec236[2]))) + (fConst495 * (fRec237[0] - fRec237[2]))) + (fConst499 * (fRec238[0] - fRec238[2]))) + (fConst503 * (fRec239[0] - fRec239[2]))) + (fConst507 * (fRec240[0] - fRec240[2]))) + (fConst511 * (fRec241[0] - fRec241[2]))) + (fConst515 * (fRec242[0] - fRec242[2]))) + (fConst519 * (fRec243[0] - fRec243[2]))) + (fConst523 * (fRec244[0] - fRec244[2]))) + (fConst527 * (fRec245[0] - fRec245[2]))) + (fConst531 * (fRec246[0] - fRec246[2]))) + (fConst535 * (fRec247[0] - fRec247[2]))) + (fConst539 * (fRec248[0] - fRec248[2]))) + (fConst543 * (fRec249[0] - fRec249[2]))) + (fConst547 * (fRec250[0] - fRec250[2]))) + (fConst551 * (fRec251[0] - fRec251[2]))) + (fConst555 * (fRec252[0] - fRec252[2]))) + (fConst559 * (fRec253[0] - fRec253[2]))) + (fConst563 * (fRec254[0] - fRec254[2]))) * std::max<float>(0.0f, std::min<float>((fConst82 * fTemp201), ((fConst568 * (fConst81 - fTemp201)) + 1.0f))))) + (2.0f * (((fRec255[0] * ftbl0mydspSIG0[int((65536.0f * fRec256[0]))]) + (fRec257[0] * ftbl0mydspSIG0[int((65536.0f * fRec258[0]))])) + (fRec259[0] * ftbl0mydspSIG0[int((65536.0f * fRec260[0]))]))));
			float fTemp205 = std::fabs((0.25f * fTemp204));
			fRec263[0] = std::max<float>(fTemp205, ((fRec263[1] * fSlow515) + (fTemp205 * fSlow516)));
			fRec262[0] = ((fRec262[1] * fSlow512) + (fRec263[0] * fSlow517));
			fRec261[0] = ((fConst64 * fRec261[1]) + (fSlow509 * std::max<float>(((20.0f * std::log10(fRec262[0])) - fSlow518), 0.0f)));
			float fTemp206 = (0.25f * (fTemp204 * std::pow(10.0f, (0.0500000007f * fRec261[0]))));
			fRec264[0] = (fTemp206 - (fSlow522 * ((fSlow523 * fRec264[2]) + (fSlow524 * fRec264[1]))));
			float fTemp207 = (fSlow496 * fRec202[1]);
			fRec202[0] = (fTemp206 - ((fSlow522 * (fRec264[2] + (fRec264[0] + (2.0f * fRec264[1])))) + (((fRec202[2] * fSlow532) + fTemp207) / fSlow533)));
			float fTemp208 = (fSlow493 * fRec201[1]);
			fRec201[0] = ((((fTemp207 + (fRec202[0] * fSlow535)) + (fRec202[2] * fSlow536)) / fSlow533) - (((fRec201[2] * fSlow544) + fTemp208) / fSlow545));
			fRec200[0] = ((((fTemp208 + (fRec201[0] * fSlow547)) + (fRec201[2] * fSlow548)) / fSlow545) - (fSlow549 * ((fSlow550 * fRec200[2]) + (fSlow551 * fRec200[1]))));
			float fTemp209 = (fSlow490 * (fRec200[2] + (fRec200[0] + (2.0f * fRec200[1]))));
			fRec277[0] = (0.0f - (fConst583 * ((fConst584 * fRec277[1]) - (fRec273[1] + fRec273[2]))));
			fRec276[0] = ((fConst579 * fRec276[1]) + (fConst580 * (fRec273[1] + (fConst581 * fRec277[0]))));
			fVec24[(IOTA & 32767)] = ((0.353553385f * fRec276[0]) + 9.99999968e-21f);
			fVec25[(IOTA & 2047)] = (fTemp41 + fTemp197);
			float fTemp210 = (0.300000012f * fVec25[((IOTA - iConst587) & 2047)]);
			float fTemp211 = (((0.600000024f * fRec274[1]) + fVec24[((IOTA - iConst586) & 32767)]) - fTemp210);
			fVec26[(IOTA & 2047)] = fTemp211;
			fRec274[0] = fVec26[((IOTA - iConst588) & 2047)];
			float fRec275 = (0.0f - (0.600000024f * fTemp211));
			fRec281[0] = (0.0f - (fConst583 * ((fConst584 * fRec281[1]) - (fRec269[1] + fRec269[2]))));
			fRec280[0] = ((fConst597 * fRec280[1]) + (fConst598 * (fRec269[1] + (fConst599 * fRec281[0]))));
			fVec27[(IOTA & 32767)] = ((0.353553385f * fRec280[0]) + 9.99999968e-21f);
			float fTemp212 = (((0.600000024f * fRec278[1]) + fVec27[((IOTA - iConst601) & 32767)]) - fTemp210);
			fVec28[(IOTA & 4095)] = fTemp212;
			fRec278[0] = fVec28[((IOTA - iConst602) & 4095)];
			float fRec279 = (0.0f - (0.600000024f * fTemp212));
			fRec285[0] = (0.0f - (fConst583 * ((fConst584 * fRec285[1]) - (fRec271[1] + fRec271[2]))));
			fRec284[0] = ((fConst611 * fRec284[1]) + (fConst612 * (fRec271[1] + (fConst613 * fRec285[0]))));
			fVec29[(IOTA & 16383)] = ((0.353553385f * fRec284[0]) + 9.99999968e-21f);
			float fTemp213 = (fVec29[((IOTA - iConst615) & 16383)] + (fTemp210 + (0.600000024f * fRec282[1])));
			fVec30[(IOTA & 4095)] = fTemp213;
			fRec282[0] = fVec30[((IOTA - iConst616) & 4095)];
			float fRec283 = (0.0f - (0.600000024f * fTemp213));
			fRec289[0] = (0.0f - (fConst583 * ((fConst584 * fRec289[1]) - (fRec267[1] + fRec267[2]))));
			fRec288[0] = ((fConst625 * fRec288[1]) + (fConst626 * (fRec267[1] + (fConst627 * fRec289[0]))));
			fVec31[(IOTA & 32767)] = ((0.353553385f * fRec288[0]) + 9.99999968e-21f);
			float fTemp214 = (fVec31[((IOTA - iConst629) & 32767)] + (fTemp210 + (0.600000024f * fRec286[1])));
			fVec32[(IOTA & 4095)] = fTemp214;
			fRec286[0] = fVec32[((IOTA - iConst630) & 4095)];
			float fRec287 = (0.0f - (0.600000024f * fTemp214));
			fRec293[0] = (0.0f - (fConst583 * ((fConst584 * fRec293[1]) - (fRec272[1] + fRec272[2]))));
			fRec292[0] = ((fConst639 * fRec292[1]) + (fConst640 * (fRec272[1] + (fConst641 * fRec293[0]))));
			fVec33[(IOTA & 16383)] = ((0.353553385f * fRec292[0]) + 9.99999968e-21f);
			float fTemp215 = (fVec33[((IOTA - iConst643) & 16383)] - (fTemp210 + (0.600000024f * fRec290[1])));
			fVec34[(IOTA & 2047)] = fTemp215;
			fRec290[0] = fVec34[((IOTA - iConst644) & 2047)];
			float fRec291 = (0.600000024f * fTemp215);
			fRec297[0] = (0.0f - (fConst583 * ((fConst584 * fRec297[1]) - (fRec268[1] + fRec268[2]))));
			fRec296[0] = ((fConst653 * fRec296[1]) + (fConst654 * (fRec268[1] + (fConst655 * fRec297[0]))));
			fVec35[(IOTA & 16383)] = ((0.353553385f * fRec296[0]) + 9.99999968e-21f);
			float fTemp216 = (fVec35[((IOTA - iConst657) & 16383)] - (fTemp210 + (0.600000024f * fRec294[1])));
			fVec36[(IOTA & 4095)] = fTemp216;
			fRec294[0] = fVec36[((IOTA - iConst658) & 4095)];
			float fRec295 = (0.600000024f * fTemp216);
			fRec301[0] = (0.0f - (fConst583 * ((fConst584 * fRec301[1]) - (fRec270[1] + fRec270[2]))));
			fRec300[0] = ((fConst667 * fRec300[1]) + (fConst668 * (fRec270[1] + (fConst669 * fRec301[0]))));
			fVec37[(IOTA & 16383)] = ((0.353553385f * fRec300[0]) + 9.99999968e-21f);
			float fTemp217 = ((fTemp210 + fVec37[((IOTA - iConst671) & 16383)]) - (0.600000024f * fRec298[1]));
			fVec38[(IOTA & 4095)] = fTemp217;
			fRec298[0] = fVec38[((IOTA - iConst672) & 4095)];
			float fRec299 = (0.600000024f * fTemp217);
			fRec305[0] = (0.0f - (fConst583 * ((fConst584 * fRec305[1]) - (fRec266[1] + fRec266[2]))));
			fRec304[0] = ((fConst681 * fRec304[1]) + (fConst682 * (fRec266[1] + (fConst683 * fRec305[0]))));
			fVec39[(IOTA & 16383)] = ((0.353553385f * fRec304[0]) + 9.99999968e-21f);
			float fTemp218 = ((fVec39[((IOTA - iConst685) & 16383)] + fTemp210) - (0.600000024f * fRec302[1]));
			fVec40[(IOTA & 2047)] = fTemp218;
			fRec302[0] = fVec40[((IOTA - iConst686) & 2047)];
			float fRec303 = (0.600000024f * fTemp218);
			float fTemp219 = (fRec303 + fRec299);
			float fTemp220 = (fRec291 + (fRec295 + fTemp219));
			fRec266[0] = (fRec274[1] + (fRec278[1] + (fRec282[1] + (fRec286[1] + (fRec290[1] + (fRec294[1] + (fRec298[1] + (fRec302[1] + (fRec275 + (fRec279 + (fRec283 + (fRec287 + fTemp220))))))))))));
			fRec267[0] = ((fRec290[1] + (fRec294[1] + (fRec298[1] + (fRec302[1] + fTemp220)))) - (fRec274[1] + (fRec278[1] + (fRec282[1] + (fRec286[1] + (fRec275 + (fRec279 + (fRec287 + fRec283))))))));
			float fTemp221 = (fRec295 + fRec291);
			fRec268[0] = ((fRec282[1] + (fRec286[1] + (fRec298[1] + (fRec302[1] + (fRec283 + (fRec287 + fTemp219)))))) - (fRec274[1] + (fRec278[1] + (fRec290[1] + (fRec294[1] + (fRec275 + (fRec279 + fTemp221)))))));
			fRec269[0] = ((fRec274[1] + (fRec278[1] + (fRec298[1] + (fRec302[1] + (fRec275 + (fRec279 + fTemp219)))))) - (fRec282[1] + (fRec286[1] + (fRec290[1] + (fRec294[1] + (fRec283 + (fRec287 + fTemp221)))))));
			float fTemp222 = (fRec303 + fRec295);
			float fTemp223 = (fRec299 + fRec291);
			fRec270[0] = ((fRec278[1] + (fRec286[1] + (fRec294[1] + (fRec302[1] + (fRec279 + (fRec287 + fTemp222)))))) - (fRec274[1] + (fRec282[1] + (fRec290[1] + (fRec298[1] + (fRec275 + (fRec283 + fTemp223)))))));
			fRec271[0] = ((fRec274[1] + (fRec282[1] + (fRec294[1] + (fRec302[1] + (fRec275 + (fRec283 + fTemp222)))))) - (fRec278[1] + (fRec286[1] + (fRec290[1] + (fRec298[1] + (fRec279 + (fRec287 + fTemp223)))))));
			float fTemp224 = (fRec303 + fRec291);
			float fTemp225 = (fRec299 + fRec295);
			fRec272[0] = ((fRec274[1] + (fRec286[1] + (fRec290[1] + (fRec302[1] + (fRec275 + (fRec287 + fTemp224)))))) - (fRec278[1] + (fRec282[1] + (fRec294[1] + (fRec298[1] + (fRec279 + (fRec283 + fTemp225)))))));
			fRec273[0] = ((fRec278[1] + (fRec282[1] + (fRec290[1] + (fRec302[1] + (fRec279 + (fRec283 + fTemp224)))))) - (fRec274[1] + (fRec286[1] + (fRec294[1] + (fRec298[1] + (fRec275 + (fRec287 + fTemp225)))))));
			fRec306[0] = (fSlow553 + (fConst690 * fRec306[1]));
			float fTemp226 = std::tan((fConst1 * fRec306[0]));
			float fTemp227 = mydsp_faustpower2_f(std::sqrt((fConst689 * fTemp226)));
			float fTemp228 = ((fConst688 * fTemp227) + -8.0f);
			float fTemp229 = (fConst687 * fTemp227);
			float fTemp230 = ((fConst567 * fTemp226) - (fConst693 * (fTemp227 / fTemp226)));
			float fTemp231 = (fConst569 * fTemp230);
			float fTemp232 = (fTemp229 + (4.0f - fTemp231));
			float fTemp233 = ((fTemp229 + fTemp231) + 4.0f);
			fRec265[0] = ((0.370000005f * (fRec267[0] + fRec268[0])) - (((fRec265[1] * fTemp228) + (fRec265[2] * fTemp232)) / fTemp233));
			float fTemp234 = (0.0f - (fConst569 * (fTemp230 / fTemp233)));
			float fTemp235 = ((fSlow3 * (((((fTemp42 + (fSlow250 * (fRec55[2] + (fRec55[0] + (2.0f * fRec55[1]))))) + (fSlow313 * fTemp115)) + fTemp128) + fTemp197) + fTemp209)) + (fSlow552 * ((fConst569 * ((fRec265[0] * fTemp230) / fTemp233)) + (fRec265[2] * fTemp234))));
			float fTemp236 = std::fabs(fTemp235);
			fRec309[0] = std::max<float>(fTemp236, ((fConst696 * fRec309[1]) + (fConst697 * fTemp236)));
			fRec308[0] = ((fConst694 * fRec308[1]) + (fConst695 * fRec309[0]));
			fRec307[0] = ((fConst64 * fRec307[1]) + (fConst65 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec308[0]) + 1.0f)), 0.0f)))));
			float fTemp237 = (fTemp235 * std::pow(10.0f, (0.0500000007f * fRec307[0])));
			float fTemp238 = std::fabs(fTemp237);
			fRec312[0] = std::max<float>(fTemp238, ((fConst700 * fRec312[1]) + (fConst701 * fTemp238)));
			fRec311[0] = ((fConst698 * fRec311[1]) + (fConst699 * fRec312[0]));
			fRec310[0] = ((fConst64 * fRec310[1]) + (fConst65 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec311[0])) + 5.0f), 0.0f)))));
			float fTemp239 = (fTemp237 * std::pow(10.0f, (0.0500000007f * fRec310[0])));
			float fTemp240 = std::fabs((fSlow0 * fTemp239));
			fRec315[0] = std::max<float>(fTemp240, ((fConst64 * fRec315[1]) + (fConst65 * fTemp240)));
			fRec314[0] = ((fConst690 * fRec314[1]) + (fConst691 * fRec315[0]));
			fRec313[0] = ((fConst64 * fRec313[1]) + (fConst65 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec314[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp239 * std::pow(10.0f, (0.0500000007f * fRec313[0])))))));
			float fTemp241 = (fRec62[0] + 0.5f);
			iRec320[0] = ((iVec0[1] + iRec320[1]) % int((fConst0 / fTemp241)));
			float fTemp242 = float(iRec320[0]);
			float fTemp243 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fTemp241) * (1.0f - (0.000500000024f * std::min<float>(fTemp241, 1000.0f)))))))));
			float fTemp244 = (1.0f / fTemp243);
			float fTemp245 = (((fTemp244 + 1.41421354f) / fTemp243) + 1.0f);
			fRec319[0] = ((2.0f * (((float((fTemp242 < (fConst86 / fTemp241))) + -0.5f) + (float((fTemp242 < (fConst87 / fTemp241))) + -0.5f)) + (float((fTemp242 < (fConst88 / fTemp241))) + -0.5f))) - (((fRec319[2] * (((fTemp244 + -1.41421354f) / fTemp243) + 1.0f)) + (2.0f * (fRec319[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp243)))))) / fTemp245));
			float fTemp246 = (fRec67[0] + 0.5f);
			iRec322[0] = ((iVec0[1] + iRec322[1]) % int((fConst0 / fTemp246)));
			float fTemp247 = float(iRec322[0]);
			float fTemp248 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fTemp246) * (1.0f - (0.000500000024f * std::min<float>(fTemp246, 1000.0f)))))))));
			float fTemp249 = (1.0f / fTemp248);
			float fTemp250 = (((fTemp249 + 1.41421354f) / fTemp248) + 1.0f);
			fRec321[0] = ((2.0f * (((float((fTemp247 < (fConst86 / fTemp246))) + -0.5f) + (float((fTemp247 < (fConst87 / fTemp246))) + -0.5f)) + (float((fTemp247 < (fConst88 / fTemp246))) + -0.5f))) - (((fRec321[2] * (((fTemp249 + -1.41421354f) / fTemp248) + 1.0f)) + (2.0f * (fRec321[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp248)))))) / fTemp250));
			float fTemp251 = (fRec70[0] + 0.5f);
			iRec324[0] = ((iVec0[1] + iRec324[1]) % int((fConst0 / fTemp251)));
			float fTemp252 = float(iRec324[0]);
			float fTemp253 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fTemp251) * (1.0f - (0.000500000024f * std::min<float>(fTemp251, 1000.0f)))))))));
			float fTemp254 = (1.0f / fTemp253);
			float fTemp255 = (((fTemp254 + 1.41421354f) / fTemp253) + 1.0f);
			fRec323[0] = ((2.0f * (((float((fTemp252 < (fConst86 / fTemp251))) + -0.5f) + (float((fTemp252 < (fConst87 / fTemp251))) + -0.5f)) + (float((fTemp252 < (fConst88 / fTemp251))) + -0.5f))) - (((fRec323[2] * (((fTemp254 + -1.41421354f) / fTemp253) + 1.0f)) + (2.0f * (fRec323[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp253)))))) / fTemp255));
			float fTemp256 = (fRec73[0] + 0.5f);
			iRec326[0] = ((iVec0[1] + iRec326[1]) % int((fConst0 / fTemp256)));
			float fTemp257 = float(iRec326[0]);
			float fTemp258 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp55 * fTemp256) * (1.0f - (0.000500000024f * std::min<float>(fTemp256, 1000.0f)))))))));
			float fTemp259 = (1.0f / fTemp258);
			float fTemp260 = (((fTemp259 + 1.41421354f) / fTemp258) + 1.0f);
			fRec325[0] = ((2.0f * (((float((fTemp257 < (fConst86 / fTemp256))) + -0.5f) + (float((fTemp257 < (fConst87 / fTemp256))) + -0.5f)) + (float((fTemp257 < (fConst88 / fTemp256))) + -0.5f))) - (((fRec325[2] * (((fTemp259 + -1.41421354f) / fTemp258) + 1.0f)) + (2.0f * (fRec325[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp258)))))) / fTemp260));
			float fTemp261 = (fRec58[0] * (((((fRec319[2] + (fRec319[0] + (2.0f * fRec319[1]))) / fTemp245) + ((fRec321[2] + (fRec321[0] + (2.0f * fRec321[1]))) / fTemp250)) + ((fRec323[2] + (fRec323[0] + (2.0f * fRec323[1]))) / fTemp255)) + ((fRec325[2] + (fRec325[0] + (2.0f * fRec325[1]))) / fTemp260)));
			float fTemp262 = std::fabs((0.25f * fTemp261));
			fRec329[0] = std::max<float>(fTemp262, ((fSlow271 * fRec329[1]) + (fSlow272 * fTemp262)));
			fRec328[0] = ((fSlow268 * fRec328[1]) + (fRec329[0] * fSlow273));
			fRec327[0] = ((fConst64 * fRec327[1]) + (fSlow265 * std::max<float>(((20.0f * std::log10(fRec328[0])) - fSlow274), 0.0f)));
			float fTemp263 = (0.25f * (fTemp261 * std::pow(10.0f, (0.0500000007f * fRec327[0]))));
			fRec330[0] = (fTemp263 - (fSlow278 * ((fSlow279 * fRec330[2]) + (fSlow280 * fRec330[1]))));
			float fTemp264 = (fSlow256 * fRec318[1]);
			fRec318[0] = (fTemp263 - ((fSlow278 * (fRec330[2] + (fRec330[0] + (2.0f * fRec330[1])))) + (((fSlow288 * fRec318[2]) + fTemp264) / fSlow289)));
			float fTemp265 = (fSlow253 * fRec317[1]);
			fRec317[0] = ((((fTemp264 + (fRec318[0] * fSlow291)) + (fSlow292 * fRec318[2])) / fSlow289) - (((fSlow300 * fRec317[2]) + fTemp265) / fSlow301));
			fRec316[0] = ((((fTemp265 + (fRec317[0] * fSlow303)) + (fSlow304 * fRec317[2])) / fSlow301) - (fSlow305 * ((fSlow306 * fRec316[2]) + (fSlow307 * fRec316[1]))));
			fRec331[0] = ((0.370000005f * (fRec267[0] - fRec268[0])) - (((fTemp228 * fRec331[1]) + (fTemp232 * fRec331[2])) / fTemp233));
			float fTemp266 = ((fSlow3 * ((fSlow250 * (fRec316[2] + (fRec316[0] + (2.0f * fRec316[1])))) + (fTemp209 + (fTemp197 + (fTemp128 + (fTemp42 + (fSlow554 * fTemp115))))))) + (fSlow552 * ((fConst569 * ((fRec331[0] * fTemp230) / fTemp233)) + (fTemp234 * fRec331[2]))));
			float fTemp267 = std::fabs(fTemp266);
			fRec334[0] = std::max<float>(fTemp267, ((fConst696 * fRec334[1]) + (fConst697 * fTemp267)));
			fRec333[0] = ((fConst694 * fRec333[1]) + (fConst695 * fRec334[0]));
			fRec332[0] = ((fConst64 * fRec332[1]) + (fConst65 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec333[0]) + 1.0f)), 0.0f)))));
			float fTemp268 = (fTemp266 * std::pow(10.0f, (0.0500000007f * fRec332[0])));
			float fTemp269 = std::fabs(fTemp268);
			fRec337[0] = std::max<float>(fTemp269, ((fConst700 * fRec337[1]) + (fConst701 * fTemp269)));
			fRec336[0] = ((fConst698 * fRec336[1]) + (fConst699 * fRec337[0]));
			fRec335[0] = ((fConst64 * fRec335[1]) + (fConst65 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec336[0])) + 5.0f), 0.0f)))));
			float fTemp270 = (fTemp268 * std::pow(10.0f, (0.0500000007f * fRec335[0])));
			float fTemp271 = std::fabs((fSlow0 * fTemp270));
			fRec340[0] = std::max<float>(fTemp271, ((fConst64 * fRec340[1]) + (fConst65 * fTemp271)));
			fRec339[0] = ((fConst690 * fRec339[1]) + (fConst691 * fRec340[0]));
			fRec338[0] = ((fConst64 * fRec338[1]) + (fConst65 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec339[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp270 * std::pow(10.0f, (0.0500000007f * fRec338[0])))))));
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
			fRec37[1] = fRec37[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			fRec39[1] = fRec39[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec38[1] = fRec38[0];
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
			iRec49[1] = iRec49[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			iRec50[1] = iRec50[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			iRec59[1] = iRec59[0];
			fRec58[1] = fRec58[0];
			fRec62[1] = fRec62[0];
			iRec61[1] = iRec61[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec67[1] = fRec67[0];
			iRec66[1] = iRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec70[1] = fRec70[0];
			iRec69[1] = iRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec73[1] = fRec73[0];
			iRec72[1] = iRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			iRec81[1] = iRec81[0];
			fRec85[1] = fRec85[0];
			IOTA = (IOTA + 1);
			fVec11[1] = fVec11[0];
			fVec12[1] = fVec12[0];
			fVec13[1] = fVec13[0];
			fRec86[1] = fRec86[0];
			fRec83[1] = fRec83[0];
			fRec87[1] = fRec87[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			iRec91[1] = iRec91[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
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
			fRec139[2] = fRec139[1];
			fRec139[1] = fRec139[0];
			fRec140[2] = fRec140[1];
			fRec140[1] = fRec140[0];
			fVec14[1] = fVec14[0];
			fRec84[1] = fRec84[0];
			fRec143[1] = fRec143[0];
			fRec142[1] = fRec142[0];
			fRec141[1] = fRec141[0];
			fRec144[2] = fRec144[1];
			fRec144[1] = fRec144[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec148[1] = fRec148[0];
			fRec150[1] = fRec150[0];
			fRec154[1] = fRec154[0];
			fRec153[1] = fRec153[0];
			fRec152[1] = fRec152[0];
			fRec151[1] = fRec151[0];
			fRec149[1] = fRec149[0];
			fRec157[1] = fRec157[0];
			fRec156[1] = fRec156[0];
			fRec155[1] = fRec155[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec146[2] = fRec146[1];
			fRec146[1] = fRec146[0];
			fRec145[2] = fRec145[1];
			fRec145[1] = fRec145[0];
			fVec16[1] = fVec16[0];
			fVec17[1] = fVec17[0];
			fVec18[1] = fVec18[0];
			fRec188[1] = fRec188[0];
			fRec184[1] = fRec184[0];
			fRec189[1] = fRec189[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec191[j0] = fRec191[(j0 - 1)];
			}
			fVec19[1] = fVec19[0];
			fVec20[2] = fVec20[1];
			fVec20[1] = fVec20[0];
			fRec180[1] = fRec180[0];
			fRec176[1] = fRec176[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec170[j1] = fRec170[(j1 - 1)];
			}
			fRec163[1] = fRec163[0];
			fRec193[1] = fRec193[0];
			fRec162[2] = fRec162[1];
			fRec162[1] = fRec162[0];
			iRec194[1] = iRec194[0];
			iRec195[1] = iRec195[0];
			fRec198[1] = fRec198[0];
			fRec197[1] = fRec197[0];
			fRec196[1] = fRec196[0];
			fRec199[2] = fRec199[1];
			fRec199[1] = fRec199[0];
			fRec161[2] = fRec161[1];
			fRec161[1] = fRec161[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec204[2] = fRec204[1];
			fRec204[1] = fRec204[0];
			fVec21[1] = fVec21[0];
			fVec22[1] = fVec22[0];
			fVec23[1] = fVec23[0];
			iRec205[1] = iRec205[0];
			fRec203[2] = fRec203[1];
			fRec203[1] = fRec203[0];
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
			fRec253[2] = fRec253[1];
			fRec253[1] = fRec253[0];
			fRec254[2] = fRec254[1];
			fRec254[1] = fRec254[0];
			fRec255[1] = fRec255[0];
			fRec256[1] = fRec256[0];
			fRec257[1] = fRec257[0];
			fRec258[1] = fRec258[0];
			fRec259[1] = fRec259[0];
			fRec260[1] = fRec260[0];
			fRec263[1] = fRec263[0];
			fRec262[1] = fRec262[0];
			fRec261[1] = fRec261[0];
			fRec264[2] = fRec264[1];
			fRec264[1] = fRec264[0];
			fRec202[2] = fRec202[1];
			fRec202[1] = fRec202[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec200[2] = fRec200[1];
			fRec200[1] = fRec200[0];
			fRec277[1] = fRec277[0];
			fRec276[1] = fRec276[0];
			fRec274[1] = fRec274[0];
			fRec281[1] = fRec281[0];
			fRec280[1] = fRec280[0];
			fRec278[1] = fRec278[0];
			fRec285[1] = fRec285[0];
			fRec284[1] = fRec284[0];
			fRec282[1] = fRec282[0];
			fRec289[1] = fRec289[0];
			fRec288[1] = fRec288[0];
			fRec286[1] = fRec286[0];
			fRec293[1] = fRec293[0];
			fRec292[1] = fRec292[0];
			fRec290[1] = fRec290[0];
			fRec297[1] = fRec297[0];
			fRec296[1] = fRec296[0];
			fRec294[1] = fRec294[0];
			fRec301[1] = fRec301[0];
			fRec300[1] = fRec300[0];
			fRec298[1] = fRec298[0];
			fRec305[1] = fRec305[0];
			fRec304[1] = fRec304[0];
			fRec302[1] = fRec302[0];
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
			fRec306[1] = fRec306[0];
			fRec265[2] = fRec265[1];
			fRec265[1] = fRec265[0];
			fRec309[1] = fRec309[0];
			fRec308[1] = fRec308[0];
			fRec307[1] = fRec307[0];
			fRec312[1] = fRec312[0];
			fRec311[1] = fRec311[0];
			fRec310[1] = fRec310[0];
			fRec315[1] = fRec315[0];
			fRec314[1] = fRec314[0];
			fRec313[1] = fRec313[0];
			iRec320[1] = iRec320[0];
			fRec319[2] = fRec319[1];
			fRec319[1] = fRec319[0];
			iRec322[1] = iRec322[0];
			fRec321[2] = fRec321[1];
			fRec321[1] = fRec321[0];
			iRec324[1] = iRec324[0];
			fRec323[2] = fRec323[1];
			fRec323[1] = fRec323[0];
			iRec326[1] = iRec326[0];
			fRec325[2] = fRec325[1];
			fRec325[1] = fRec325[0];
			fRec329[1] = fRec329[0];
			fRec328[1] = fRec328[0];
			fRec327[1] = fRec327[0];
			fRec330[2] = fRec330[1];
			fRec330[1] = fRec330[0];
			fRec318[2] = fRec318[1];
			fRec318[1] = fRec318[0];
			fRec317[2] = fRec317[1];
			fRec317[1] = fRec317[0];
			fRec316[2] = fRec316[1];
			fRec316[1] = fRec316[0];
			fRec331[2] = fRec331[1];
			fRec331[1] = fRec331[0];
			fRec334[1] = fRec334[0];
			fRec333[1] = fRec333[0];
			fRec332[1] = fRec332[0];
			fRec337[1] = fRec337[0];
			fRec336[1] = fRec336[0];
			fRec335[1] = fRec335[0];
			fRec340[1] = fRec340[0];
			fRec339[1] = fRec339[0];
			fRec338[1] = fRec338[0];
		}
	}

};

#endif
