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
	
	int iRec41[2];
	
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
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			iRec41[l43] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec41[0] = (iRec41[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec41[0] + -1))));
			iRec41[1] = iRec41[0];
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

static float mydsp_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower6_f(float value) {
	return (((((value * value) * value) * value) * value) * value);
}
static float ftbl1mydspSIG1[250];

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
	FAUSTFLOAT fHslider0;
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
	float fConst4;
	float fConst7;
	float fConst9;
	int iRec6[2];
	float fConst10;
	float fConst11;
	float fRec5[3];
	float fConst12;
	float fConst13;
	float fConst14;
	float fRec4[3];
	FAUSTFLOAT fHslider6;
	float fVec1[2];
	float fVec2[2];
	float fVec3[2];
	int iRec7[2];
	float fConst15;
	float fRec8[2];
	float fConst17;
	float fConst18;
	float fConst19;
	float fRec3[3];
	float fConst21;
	float fConst22;
	float fRec9[3];
	float fConst24;
	float fConst25;
	float fRec10[3];
	float fConst27;
	float fConst28;
	float fRec11[3];
	float fConst30;
	float fConst31;
	float fRec12[3];
	float fConst33;
	float fConst34;
	float fRec13[3];
	float fConst36;
	float fConst37;
	float fRec14[3];
	float fConst39;
	float fConst40;
	float fRec15[3];
	float fConst42;
	float fConst43;
	float fRec16[3];
	float fConst45;
	float fConst46;
	float fRec17[3];
	float fConst48;
	float fConst49;
	float fRec18[3];
	float fConst51;
	float fConst52;
	float fRec19[3];
	float fConst54;
	float fConst55;
	float fRec20[3];
	float fConst57;
	float fConst58;
	float fRec21[3];
	float fConst60;
	float fConst61;
	float fRec22[3];
	float fConst63;
	float fConst64;
	float fRec23[3];
	float fConst66;
	float fConst67;
	float fRec24[3];
	float fConst69;
	float fConst70;
	float fRec25[3];
	float fConst72;
	float fConst73;
	float fRec26[3];
	float fConst75;
	float fConst76;
	float fRec27[3];
	FAUSTFLOAT fHslider7;
	float fRec28[2];
	float fConst77;
	float fConst78;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst79;
	FAUSTFLOAT fHslider10;
	float fRec31[2];
	float fRec30[2];
	FAUSTFLOAT fHslider11;
	float fRec29[2];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec32[3];
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
	float fConst83;
	float fConst84;
	float fConst85;
	FAUSTFLOAT fHslider23;
	float fVec4[2];
	float fVec5[2];
	float fVec6[2];
	int iRec38[2];
	float fConst86;
	float fRec37[2];
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	float fRec39[2];
	FAUSTFLOAT fHslider26;
	float fConst87;
	float fRec42[2];
	float fRec40[2];
	float fRec44[2];
	FAUSTFLOAT fHslider27;
	float fRec46[2];
	float fRec43[3];
	float fRec47[3];
	float fRec48[3];
	float fRec49[3];
	float fRec50[3];
	float fConst88;
	float fConst89;
	float fConst90;
	float fRec36[3];
	float fConst91;
	float fRec51[2];
	FAUSTFLOAT fHslider28;
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	float fRec54[2];
	float fRec53[2];
	FAUSTFLOAT fHslider31;
	float fRec52[2];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec55[3];
	FAUSTFLOAT fHslider34;
	FAUSTFLOAT fHslider35;
	float fRec35[3];
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fRec34[3];
	float fRec33[3];
	FAUSTFLOAT fHslider38;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider39;
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fHslider42;
	float fConst92;
	float fConst93;
	FAUSTFLOAT fHslider43;
	float fVec7[2];
	float fVec8[2];
	float fVec9[2];
	int iRec60[2];
	float fRec59[2];
	float fRec61[2];
	FAUSTFLOAT fHslider44;
	float fRec64[2];
	int iRec63[2];
	float fConst94;
	float fConst95;
	float fConst96;
	float fConst97;
	float fConst98;
	float fRec66[2];
	float fRec65[2];
	float fRec62[3];
	FAUSTFLOAT fHslider45;
	float fRec69[2];
	int iRec68[2];
	float fRec67[3];
	FAUSTFLOAT fHslider46;
	float fRec72[2];
	int iRec71[2];
	float fRec70[3];
	FAUSTFLOAT fHslider47;
	float fRec75[2];
	int iRec74[2];
	float fRec73[3];
	FAUSTFLOAT fHslider48;
	FAUSTFLOAT fHslider49;
	FAUSTFLOAT fHslider50;
	float fRec78[2];
	float fRec77[2];
	FAUSTFLOAT fHslider51;
	float fRec76[2];
	FAUSTFLOAT fHslider52;
	FAUSTFLOAT fHslider53;
	float fRec79[3];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec58[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec57[3];
	float fRec56[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	int iRec83[2];
	float fRec87[2];
	int IOTA;
	float fVec10[2048];
	float fConst99;
	float fConst100;
	FAUSTFLOAT fHslider63;
	float fVec11[2];
	float fVec12[2];
	float fVec13[2];
	float fRec88[2];
	float fRec85[2];
	float fRec89[2];
	float fConst103;
	float fConst106;
	float fConst108;
	float fConst109;
	float fConst110;
	float fRec92[3];
	float fConst111;
	float fConst112;
	float fConst113;
	float fRec91[3];
	int iRec93[2];
	float fConst115;
	float fConst116;
	float fRec90[3];
	float fConst117;
	float fConst118;
	float fConst120;
	float fConst121;
	float fConst122;
	float fRec94[3];
	float fConst123;
	float fConst125;
	float fConst126;
	float fConst127;
	float fRec95[3];
	float fConst128;
	float fConst130;
	float fConst131;
	float fConst132;
	float fRec96[3];
	float fConst133;
	float fConst135;
	float fConst136;
	float fConst137;
	float fRec97[3];
	float fConst138;
	float fConst140;
	float fConst141;
	float fConst142;
	float fRec98[3];
	float fConst143;
	float fConst145;
	float fConst146;
	float fConst147;
	float fRec99[3];
	float fConst148;
	float fConst150;
	float fConst151;
	float fConst152;
	float fRec100[3];
	float fConst153;
	float fConst155;
	float fConst156;
	float fConst157;
	float fRec101[3];
	float fConst158;
	float fConst160;
	float fConst161;
	float fConst162;
	float fRec102[3];
	float fConst163;
	float fConst165;
	float fConst166;
	float fConst167;
	float fRec103[3];
	float fConst168;
	float fConst170;
	float fConst171;
	float fConst172;
	float fRec104[3];
	float fConst173;
	float fConst175;
	float fConst176;
	float fConst177;
	float fRec105[3];
	float fConst178;
	float fConst180;
	float fConst181;
	float fConst182;
	float fRec106[3];
	float fConst183;
	float fConst185;
	float fConst186;
	float fConst187;
	float fRec107[3];
	float fConst188;
	float fConst190;
	float fConst191;
	float fConst192;
	float fRec108[3];
	float fConst193;
	float fConst195;
	float fConst196;
	float fConst197;
	float fRec109[3];
	float fConst198;
	float fConst200;
	float fConst201;
	float fConst202;
	float fRec110[3];
	float fConst203;
	float fConst205;
	float fConst206;
	float fConst207;
	float fRec111[3];
	float fConst208;
	float fConst210;
	float fConst211;
	float fConst212;
	float fRec112[3];
	float fConst213;
	float fConst215;
	float fConst216;
	float fConst217;
	float fRec113[3];
	float fConst218;
	float fConst220;
	float fConst221;
	float fConst222;
	float fRec114[3];
	float fConst223;
	float fConst225;
	float fConst226;
	float fConst227;
	float fRec115[3];
	float fConst228;
	float fConst230;
	float fConst231;
	float fConst232;
	float fRec116[3];
	float fConst233;
	float fConst235;
	float fConst236;
	float fConst237;
	float fRec117[3];
	float fConst238;
	float fConst240;
	float fConst241;
	float fConst242;
	float fRec118[3];
	float fConst243;
	float fConst245;
	float fConst246;
	float fConst247;
	float fRec119[3];
	float fConst248;
	float fConst250;
	float fConst251;
	float fConst252;
	float fRec120[3];
	float fConst253;
	float fConst255;
	float fConst256;
	float fConst257;
	float fRec121[3];
	float fConst258;
	float fConst260;
	float fConst261;
	float fConst262;
	float fRec122[3];
	float fConst263;
	float fConst265;
	float fConst266;
	float fConst267;
	float fRec123[3];
	float fConst268;
	float fConst270;
	float fConst271;
	float fConst272;
	float fRec124[3];
	float fConst273;
	float fConst275;
	float fConst276;
	float fConst277;
	float fRec125[3];
	float fConst278;
	float fConst280;
	float fConst281;
	float fConst282;
	float fRec126[3];
	float fConst283;
	float fConst285;
	float fConst286;
	float fConst287;
	float fRec127[3];
	float fConst288;
	float fConst290;
	float fConst291;
	float fConst292;
	float fRec128[3];
	float fConst293;
	float fConst295;
	float fConst296;
	float fConst297;
	float fRec129[3];
	float fConst298;
	float fConst300;
	float fConst301;
	float fConst302;
	float fRec130[3];
	float fConst303;
	float fConst305;
	float fConst306;
	float fConst307;
	float fRec131[3];
	float fConst308;
	float fConst310;
	float fConst311;
	float fConst312;
	float fRec132[3];
	float fConst313;
	float fConst315;
	float fConst316;
	float fConst317;
	float fRec133[3];
	float fConst318;
	float fConst320;
	float fConst321;
	float fConst322;
	float fRec134[3];
	float fConst323;
	float fConst325;
	float fConst326;
	float fConst327;
	float fRec135[3];
	float fConst328;
	float fConst330;
	float fConst331;
	float fConst332;
	float fRec136[3];
	float fConst333;
	float fConst335;
	float fConst336;
	float fConst337;
	float fRec137[3];
	float fConst338;
	float fConst340;
	float fConst341;
	float fConst342;
	float fRec138[3];
	float fConst343;
	float fConst345;
	float fConst346;
	float fConst347;
	float fRec139[3];
	float fConst348;
	float fConst350;
	float fConst351;
	float fConst352;
	float fRec140[3];
	float fConst353;
	float fConst355;
	float fConst356;
	float fConst357;
	float fRec141[3];
	float fConst358;
	float fConst360;
	float fConst361;
	float fConst362;
	float fRec142[3];
	float fConst363;
	float fVec14[2];
	float fVec15[2048];
	float fRec86[2];
	float fRec143[2];
	FAUSTFLOAT fHslider64;
	FAUSTFLOAT fHslider65;
	FAUSTFLOAT fHslider66;
	float fRec146[2];
	float fRec145[2];
	FAUSTFLOAT fHslider67;
	float fRec144[2];
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	float fRec147[3];
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec82[3];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec81[3];
	float fRec80[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	float fConst364;
	float fRec151[2];
	float fRec153[2];
	float fConst365;
	float fRec157[2];
	float fRec156[2];
	float fRec155[2];
	float fRec154[2];
	float fRec152[2];
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	float fRec160[2];
	float fRec159[2];
	FAUSTFLOAT fHslider82;
	float fRec158[2];
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec161[3];
	FAUSTFLOAT fHslider85;
	FAUSTFLOAT fHslider86;
	float fRec150[3];
	FAUSTFLOAT fHslider87;
	FAUSTFLOAT fHslider88;
	float fRec149[3];
	float fRec148[3];
	FAUSTFLOAT fHslider89;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	FAUSTFLOAT fHslider92;
	FAUSTFLOAT fHslider93;
	float fConst366;
	FAUSTFLOAT fHslider94;
	float fVec16[2];
	float fVec17[2];
	float fVec18[2];
	float fRec190[2];
	float fRec186[2];
	FAUSTFLOAT fHslider95;
	float fRec191[2];
	float fRec193[4];
	float fRec194[1024];
	float fConst367;
	float fVec19[2];
	float fVec20[3];
	float fRec192[2048];
	float fRec182[2];
	float fRec178[2];
	float fRec174[1024];
	float fRec172[4];
	float fRec167[2048];
	float fRec165[2];
	int iRec195[2];
	float fRec196[2];
	FAUSTFLOAT fHslider96;
	FAUSTFLOAT fHslider97;
	FAUSTFLOAT fHslider98;
	float fRec199[2];
	float fRec198[2];
	FAUSTFLOAT fHslider99;
	float fRec197[2];
	FAUSTFLOAT fHslider100;
	FAUSTFLOAT fHslider101;
	float fRec200[3];
	FAUSTFLOAT fHslider102;
	FAUSTFLOAT fHslider103;
	float fRec164[3];
	FAUSTFLOAT fHslider104;
	FAUSTFLOAT fHslider105;
	float fRec163[3];
	float fRec162[3];
	float fConst368;
	float fConst369;
	float fConst370;
	float fConst371;
	float fRec203[2];
	float fRec202[2];
	float fRec201[2];
	float fConst372;
	float fConst373;
	float fConst374;
	float fConst375;
	float fRec206[2];
	float fRec205[2];
	float fRec204[2];
	float fConst376;
	float fConst377;
	float fRec209[2];
	float fRec208[2];
	float fRec207[2];
	int iRec214[2];
	float fRec213[3];
	int iRec216[2];
	float fRec215[3];
	int iRec218[2];
	float fRec217[3];
	int iRec220[2];
	float fRec219[3];
	float fRec223[2];
	float fRec222[2];
	float fRec221[2];
	float fRec224[3];
	float fRec212[3];
	float fRec211[3];
	float fRec210[3];
	float fRec227[2];
	float fRec226[2];
	float fRec225[2];
	float fRec230[2];
	float fRec229[2];
	float fRec228[2];
	float fRec233[2];
	float fRec232[2];
	float fRec231[2];
	
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
		deletemydspSIG0(sig0);
		deletemydspSIG1(sig1);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (3.14159274f / fConst0);
		float fConst2 = std::tan((22776.5469f / fConst0));
		float fConst3 = (1.0f / fConst2);
		fConst4 = (1.0f / (((fConst3 + 1.41421354f) / fConst2) + 1.0f));
		float fConst5 = std::tan((832.522034f / fConst0));
		float fConst6 = (1.0f / fConst5);
		fConst7 = (1.0f / (((fConst6 + 1.41421354f) / fConst5) + 1.0f));
		float fConst8 = mydsp_faustpower2_f(fConst5);
		fConst9 = (1.0f / fConst8);
		fConst10 = (((fConst6 + -1.41421354f) / fConst5) + 1.0f);
		fConst11 = (2.0f * (1.0f - fConst9));
		fConst12 = (0.0f - (2.0f / fConst8));
		fConst13 = (((fConst3 + -1.41421354f) / fConst2) + 1.0f);
		fConst14 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst2))));
		fConst15 = (0.00200000009f * fConst0);
		float fConst16 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst17 = (0.0f - (2.0f * fConst16));
		fConst18 = (6.28318548f / fConst0);
		fConst19 = mydsp_faustpower2_f(fConst16);
		float fConst20 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst21 = (0.0f - (2.0f * fConst20));
		fConst22 = mydsp_faustpower2_f(fConst20);
		float fConst23 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst24 = (0.0f - (2.0f * fConst23));
		fConst25 = mydsp_faustpower2_f(fConst23);
		float fConst26 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst27 = (0.0f - (2.0f * fConst26));
		fConst28 = mydsp_faustpower2_f(fConst26);
		float fConst29 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst30 = (0.0f - (2.0f * fConst29));
		fConst31 = mydsp_faustpower2_f(fConst29);
		float fConst32 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst33 = (0.0f - (2.0f * fConst32));
		fConst34 = mydsp_faustpower2_f(fConst32);
		float fConst35 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst36 = (0.0f - (2.0f * fConst35));
		fConst37 = mydsp_faustpower2_f(fConst35);
		float fConst38 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst39 = (0.0f - (2.0f * fConst38));
		fConst40 = mydsp_faustpower2_f(fConst38);
		float fConst41 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst42 = (0.0f - (2.0f * fConst41));
		fConst43 = mydsp_faustpower2_f(fConst41);
		float fConst44 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst45 = (0.0f - (2.0f * fConst44));
		fConst46 = mydsp_faustpower2_f(fConst44);
		float fConst47 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst48 = (0.0f - (2.0f * fConst47));
		fConst49 = mydsp_faustpower2_f(fConst47);
		float fConst50 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst51 = (0.0f - (2.0f * fConst50));
		fConst52 = mydsp_faustpower2_f(fConst50);
		float fConst53 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst54 = (0.0f - (2.0f * fConst53));
		fConst55 = mydsp_faustpower2_f(fConst53);
		float fConst56 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst57 = (0.0f - (2.0f * fConst56));
		fConst58 = mydsp_faustpower2_f(fConst56);
		float fConst59 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst60 = (0.0f - (2.0f * fConst59));
		fConst61 = mydsp_faustpower2_f(fConst59);
		float fConst62 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst63 = (0.0f - (2.0f * fConst62));
		fConst64 = mydsp_faustpower2_f(fConst62);
		float fConst65 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst66 = (0.0f - (2.0f * fConst65));
		fConst67 = mydsp_faustpower2_f(fConst65);
		float fConst68 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst69 = (0.0f - (2.0f * fConst68));
		fConst70 = mydsp_faustpower2_f(fConst68);
		float fConst71 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst72 = (0.0f - (2.0f * fConst71));
		fConst73 = mydsp_faustpower2_f(fConst71);
		float fConst74 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst75 = (0.0f - (2.0f * fConst74));
		fConst76 = mydsp_faustpower2_f(fConst74);
		fConst77 = std::exp((0.0f - (20.0f / fConst0)));
		fConst78 = (1.0f - fConst77);
		fConst79 = (1.0f / fConst0);
		float fConst80 = std::tan((25132.7422f / fConst0));
		float fConst81 = (1.0f / fConst80);
		float fConst82 = (((fConst81 + 0.333333343f) / fConst80) + 1.0f);
		fConst83 = (0.25f / fConst82);
		fConst84 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst85 = (1.0f / fConst84);
		fConst86 = (1.0f / std::max<float>(1.0f, (2.0f * fConst0)));
		fConst87 = (8.0f / fConst0);
		fConst88 = (1.0f / fConst82);
		fConst89 = (((fConst81 + -0.333333343f) / fConst80) + 1.0f);
		fConst90 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst80))));
		fConst91 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst92 = std::exp((0.0f - (500.0f / fConst0)));
		fConst93 = (1.0f - fConst92);
		fConst94 = (0.330000013f * fConst0);
		fConst95 = (0.100000001f * fConst0);
		fConst96 = (0.660000026f * fConst0);
		fConst97 = std::exp((0.0f - (100.0f / fConst0)));
		fConst98 = (1.0f - fConst97);
		fConst99 = (0.00882352982f * fConst0);
		fConst100 = (0.00147058826f * fConst0);
		float fConst101 = std::tan((37699.1133f / fConst0));
		float fConst102 = (1.0f / fConst101);
		fConst103 = (1.0f / (((fConst102 + 1.41421354f) / fConst101) + 1.0f));
		float fConst104 = std::tan((31.415926f / fConst0));
		float fConst105 = (1.0f / fConst104);
		fConst106 = (1.0f / (((fConst105 + 1.41421354f) / fConst104) + 1.0f));
		float fConst107 = mydsp_faustpower2_f(fConst104);
		fConst108 = (1.0f / fConst107);
		fConst109 = (((fConst105 + -1.41421354f) / fConst104) + 1.0f);
		fConst110 = (2.0f * (1.0f - fConst108));
		fConst111 = (0.0f - (2.0f / fConst107));
		fConst112 = (((fConst102 + -1.41421354f) / fConst101) + 1.0f);
		fConst113 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst101))));
		float fConst114 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst115 = (0.0f - (2.0f * fConst114));
		fConst116 = mydsp_faustpower2_f(fConst114);
		fConst117 = ((0.5f * fConst0) + -1.0f);
		fConst118 = ftbl1mydspSIG1[0];
		float fConst119 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst120 = (0.0f - (2.0f * fConst119));
		fConst121 = (20.8197117f / fConst0);
		fConst122 = mydsp_faustpower2_f(fConst119);
		fConst123 = ftbl1mydspSIG1[1];
		float fConst124 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst125 = (0.0f - (2.0f * fConst124));
		fConst126 = (24.0940685f / fConst0);
		fConst127 = mydsp_faustpower2_f(fConst124);
		fConst128 = ftbl1mydspSIG1[2];
		float fConst129 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst130 = (0.0f - (2.0f * fConst129));
		fConst131 = (50.6621399f / fConst0);
		fConst132 = mydsp_faustpower2_f(fConst129);
		fConst133 = ftbl1mydspSIG1[3];
		float fConst134 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst135 = (0.0f - (2.0f * fConst134));
		fConst136 = (59.3621521f / fConst0);
		fConst137 = mydsp_faustpower2_f(fConst134);
		fConst138 = ftbl1mydspSIG1[4];
		float fConst139 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst140 = (0.0f - (2.0f * fConst139));
		fConst141 = (88.6990967f / fConst0);
		fConst142 = mydsp_faustpower2_f(fConst139);
		fConst143 = ftbl1mydspSIG1[5];
		float fConst144 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst145 = (0.0f - (2.0f * fConst144));
		fConst146 = (115.510078f / fConst0);
		fConst147 = mydsp_faustpower2_f(fConst144);
		fConst148 = ftbl1mydspSIG1[6];
		float fConst149 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst150 = (0.0f - (2.0f * fConst149));
		fConst151 = (132.010986f / fConst0);
		fConst152 = mydsp_faustpower2_f(fConst149);
		fConst153 = ftbl1mydspSIG1[7];
		float fConst154 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst155 = (0.0f - (2.0f * fConst154));
		fConst156 = (164.478088f / fConst0);
		fConst157 = mydsp_faustpower2_f(fConst154);
		fConst158 = ftbl1mydspSIG1[8];
		float fConst159 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst160 = (0.0f - (2.0f * fConst159));
		fConst161 = (182.177185f / fConst0);
		fConst162 = mydsp_faustpower2_f(fConst159);
		fConst163 = ftbl1mydspSIG1[9];
		float fConst164 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst165 = (0.0f - (2.0f * fConst164));
		fConst166 = (232.935272f / fConst0);
		fConst167 = mydsp_faustpower2_f(fConst164);
		fConst168 = ftbl1mydspSIG1[10];
		float fConst169 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst170 = (0.0f - (2.0f * fConst169));
		fConst171 = (237.946106f / fConst0);
		fConst172 = mydsp_faustpower2_f(fConst169);
		fConst173 = ftbl1mydspSIG1[11];
		float fConst174 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst175 = (0.0f - (2.0f * fConst174));
		fConst176 = (251.725769f / fConst0);
		fConst177 = mydsp_faustpower2_f(fConst174);
		fConst178 = ftbl1mydspSIG1[12];
		float fConst179 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst180 = (0.0f - (2.0f * fConst179));
		fConst181 = (299.355438f / fConst0);
		fConst182 = mydsp_faustpower2_f(fConst179);
		fConst183 = ftbl1mydspSIG1[13];
		float fConst184 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst185 = (0.0f - (2.0f * fConst184));
		fConst186 = (320.561829f / fConst0);
		fConst187 = mydsp_faustpower2_f(fConst184);
		fConst188 = ftbl1mydspSIG1[14];
		float fConst189 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst190 = (0.0f - (2.0f * fConst189));
		fConst191 = (329.427399f / fConst0);
		fConst192 = mydsp_faustpower2_f(fConst189);
		fConst193 = ftbl1mydspSIG1[15];
		float fConst194 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst195 = (0.0f - (2.0f * fConst194));
		fConst196 = (366.221741f / fConst0);
		fConst197 = mydsp_faustpower2_f(fConst194);
		fConst198 = ftbl1mydspSIG1[16];
		float fConst199 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst200 = (0.0f - (2.0f * fConst199));
		fConst201 = (399.287628f / fConst0);
		fConst202 = mydsp_faustpower2_f(fConst199);
		fConst203 = ftbl1mydspSIG1[17];
		float fConst204 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst205 = (0.0f - (2.0f * fConst204));
		fConst206 = (410.686584f / fConst0);
		fConst207 = mydsp_faustpower2_f(fConst204);
		fConst208 = ftbl1mydspSIG1[18];
		float fConst209 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst210 = (0.0f - (2.0f * fConst209));
		fConst211 = (420.713928f / fConst0);
		fConst212 = mydsp_faustpower2_f(fConst209);
		fConst213 = ftbl1mydspSIG1[19];
		float fConst214 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst215 = (0.0f - (2.0f * fConst214));
		fConst216 = (468.286438f / fConst0);
		fConst217 = mydsp_faustpower2_f(fConst214);
		fConst218 = ftbl1mydspSIG1[20];
		float fConst219 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst220 = (0.0f - (2.0f * fConst219));
		fConst221 = (494.436432f / fConst0);
		fConst222 = mydsp_faustpower2_f(fConst219);
		fConst223 = ftbl1mydspSIG1[21];
		float fConst224 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst225 = (0.0f - (2.0f * fConst224));
		fConst226 = (507.916992f / fConst0);
		fConst227 = mydsp_faustpower2_f(fConst224);
		fConst228 = ftbl1mydspSIG1[22];
		float fConst229 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst230 = (0.0f - (2.0f * fConst229));
		fConst231 = (565.34845f / fConst0);
		fConst232 = mydsp_faustpower2_f(fConst229);
		fConst233 = ftbl1mydspSIG1[23];
		float fConst234 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst235 = (0.0f - (2.0f * fConst234));
		fConst236 = (584.12323f / fConst0);
		fConst237 = mydsp_faustpower2_f(fConst234);
		fConst238 = ftbl1mydspSIG1[24];
		float fConst239 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst240 = (0.0f - (2.0f * fConst239));
		fConst241 = (598.105225f / fConst0);
		fConst242 = mydsp_faustpower2_f(fConst239);
		fConst243 = ftbl1mydspSIG1[25];
		float fConst244 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst245 = (0.0f - (2.0f * fConst244));
		fConst246 = (612.489319f / fConst0);
		fConst247 = mydsp_faustpower2_f(fConst244);
		fConst248 = ftbl1mydspSIG1[26];
		float fConst249 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst250 = (0.0f - (2.0f * fConst249));
		fConst251 = (695.045959f / fConst0);
		fConst252 = mydsp_faustpower2_f(fConst249);
		fConst253 = ftbl1mydspSIG1[27];
		float fConst254 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst255 = (0.0f - (2.0f * fConst254));
		fConst256 = (704.150269f / fConst0);
		fConst257 = mydsp_faustpower2_f(fConst254);
		fConst258 = ftbl1mydspSIG1[28];
		float fConst259 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst260 = (0.0f - (2.0f * fConst259));
		fConst261 = (715.18988f / fConst0);
		fConst262 = mydsp_faustpower2_f(fConst259);
		fConst263 = ftbl1mydspSIG1[29];
		float fConst264 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst265 = (0.0f - (2.0f * fConst264));
		fConst266 = (749.935852f / fConst0);
		fConst267 = mydsp_faustpower2_f(fConst264);
		fConst268 = ftbl1mydspSIG1[30];
		float fConst269 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst270 = (0.0f - (2.0f * fConst269));
		fConst271 = (798.247253f / fConst0);
		fConst272 = mydsp_faustpower2_f(fConst269);
		fConst273 = ftbl1mydspSIG1[31];
		float fConst274 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst275 = (0.0f - (2.0f * fConst274));
		fConst276 = (816.700989f / fConst0);
		fConst277 = mydsp_faustpower2_f(fConst274);
		fConst278 = ftbl1mydspSIG1[32];
		float fConst279 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst280 = (0.0f - (2.0f * fConst279));
		fConst281 = (831.007812f / fConst0);
		fConst282 = mydsp_faustpower2_f(fConst279);
		fConst283 = ftbl1mydspSIG1[33];
		float fConst284 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst285 = (0.0f - (2.0f * fConst284));
		fConst286 = (838.660706f / fConst0);
		fConst287 = mydsp_faustpower2_f(fConst284);
		fConst288 = ftbl1mydspSIG1[34];
		float fConst289 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst290 = (0.0f - (2.0f * fConst289));
		fConst291 = (908.228149f / fConst0);
		fConst292 = mydsp_faustpower2_f(fConst289);
		fConst293 = ftbl1mydspSIG1[35];
		float fConst294 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst295 = (0.0f - (2.0f * fConst294));
		fConst296 = (938.946655f / fConst0);
		fConst297 = mydsp_faustpower2_f(fConst294);
		fConst298 = ftbl1mydspSIG1[36];
		float fConst299 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst300 = (0.0f - (2.0f * fConst299));
		fConst301 = (955.251526f / fConst0);
		fConst302 = mydsp_faustpower2_f(fConst299);
		fConst303 = ftbl1mydspSIG1[37];
		float fConst304 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst305 = (0.0f - (2.0f * fConst304));
		fConst306 = (962.370361f / fConst0);
		fConst307 = mydsp_faustpower2_f(fConst304);
		fConst308 = ftbl1mydspSIG1[38];
		float fConst309 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst310 = (0.0f - (2.0f * fConst309));
		fConst311 = (977.644775f / fConst0);
		fConst312 = mydsp_faustpower2_f(fConst309);
		fConst313 = ftbl1mydspSIG1[39];
		float fConst314 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst315 = (0.0f - (2.0f * fConst314));
		fConst316 = (993.893127f / fConst0);
		fConst317 = mydsp_faustpower2_f(fConst314);
		fConst318 = ftbl1mydspSIG1[40];
		float fConst319 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst320 = (0.0f - (2.0f * fConst319));
		fConst321 = (1056.23486f / fConst0);
		fConst322 = mydsp_faustpower2_f(fConst319);
		fConst323 = ftbl1mydspSIG1[41];
		float fConst324 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst325 = (0.0f - (2.0f * fConst324));
		fConst326 = (1079.84705f / fConst0);
		fConst327 = mydsp_faustpower2_f(fConst324);
		fConst328 = ftbl1mydspSIG1[42];
		float fConst329 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst330 = (0.0f - (2.0f * fConst329));
		fConst331 = (1096.1897f / fConst0);
		fConst332 = mydsp_faustpower2_f(fConst329);
		fConst333 = ftbl1mydspSIG1[43];
		float fConst334 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst335 = (0.0f - (2.0f * fConst334));
		fConst336 = (1124.29431f / fConst0);
		fConst337 = mydsp_faustpower2_f(fConst334);
		fConst338 = ftbl1mydspSIG1[44];
		float fConst339 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst340 = (0.0f - (2.0f * fConst339));
		fConst341 = (1140.28503f / fConst0);
		fConst342 = mydsp_faustpower2_f(fConst339);
		fConst343 = ftbl1mydspSIG1[45];
		float fConst344 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst345 = (0.0f - (2.0f * fConst344));
		fConst346 = (1164.89001f / fConst0);
		fConst347 = mydsp_faustpower2_f(fConst344);
		fConst348 = ftbl1mydspSIG1[46];
		float fConst349 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst350 = (0.0f - (2.0f * fConst349));
		fConst351 = (1196.12366f / fConst0);
		fConst352 = mydsp_faustpower2_f(fConst349);
		fConst353 = ftbl1mydspSIG1[47];
		float fConst354 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst355 = (0.0f - (2.0f * fConst354));
		fConst356 = (1207.56543f / fConst0);
		fConst357 = mydsp_faustpower2_f(fConst354);
		fConst358 = ftbl1mydspSIG1[48];
		float fConst359 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst360 = (0.0f - (2.0f * fConst359));
		fConst361 = (1228.39417f / fConst0);
		fConst362 = mydsp_faustpower2_f(fConst359);
		fConst363 = ftbl1mydspSIG1[49];
		fConst364 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst365 = (81.0f / fConst0);
		fConst366 = (0.00117647054f * fConst0);
		fConst367 = (0.000294117635f * fConst0);
		fConst368 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst369 = (1.0f - fConst368);
		fConst370 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst371 = (1.0f - fConst370);
		fConst372 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst373 = (1.0f - fConst372);
		fConst374 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst375 = (1.0f - fConst374);
		fConst376 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst377 = (1.0f - fConst376);
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
		fHslider23 = FAUSTFLOAT(0.0f);
		fHslider24 = FAUSTFLOAT(0.5f);
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
		fHslider95 = FAUSTFLOAT(0.01f);
		fHslider96 = FAUSTFLOAT(1.0f);
		fHslider97 = FAUSTFLOAT(0.10000000000000001f);
		fHslider98 = FAUSTFLOAT(0.10000000000000001f);
		fHslider99 = FAUSTFLOAT(0.0f);
		fHslider100 = FAUSTFLOAT(0.69999999999999996f);
		fHslider101 = FAUSTFLOAT(50.0f);
		fHslider102 = FAUSTFLOAT(0.0f);
		fHslider103 = FAUSTFLOAT(0.69999999999999996f);
		fHslider104 = FAUSTFLOAT(0.0f);
		fHslider105 = FAUSTFLOAT(0.69999999999999996f);
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
			fRec28[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec31[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec30[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec29[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec32[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec2[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec1[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec0[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec4[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fVec5[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fVec6[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			iRec38[l40] = 0;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec37[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec39[l42] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec42[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec40[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec44[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec46[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec43[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec47[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec48[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec49[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec50[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec36[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec51[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec54[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec53[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec52[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec55[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec35[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec34[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 3); l61 = (l61 + 1)) {
			fRec33[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec7[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fVec8[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fVec9[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			iRec60[l65] = 0;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec59[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec61[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec64[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			iRec63[l69] = 0;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec66[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec65[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec62[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec69[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			iRec68[l74] = 0;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec67[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec72[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			iRec71[l77] = 0;
		}
		for (int l78 = 0; (l78 < 3); l78 = (l78 + 1)) {
			fRec70[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec75[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			iRec74[l80] = 0;
		}
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec73[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec78[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec77[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec76[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec79[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec58[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec57[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 3); l88 = (l88 + 1)) {
			fRec56[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			iRec83[l89] = 0;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec87[l90] = 0.0f;
		}
		IOTA = 0;
		for (int l91 = 0; (l91 < 2048); l91 = (l91 + 1)) {
			fVec10[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec11[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fVec12[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fVec13[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec88[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec85[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec89[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec92[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec91[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			iRec93[l100] = 0;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec90[l101] = 0.0f;
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
		for (int l149 = 0; (l149 < 3); l149 = (l149 + 1)) {
			fRec141[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 3); l150 = (l150 + 1)) {
			fRec142[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fVec14[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2048); l152 = (l152 + 1)) {
			fVec15[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec86[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 2); l154 = (l154 + 1)) {
			fRec143[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec146[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec145[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec144[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 3); l158 = (l158 + 1)) {
			fRec147[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 3); l159 = (l159 + 1)) {
			fRec82[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 3); l160 = (l160 + 1)) {
			fRec81[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 3); l161 = (l161 + 1)) {
			fRec80[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec151[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 2); l163 = (l163 + 1)) {
			fRec153[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			fRec157[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec156[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec155[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec154[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec152[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fRec160[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fRec159[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec158[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec161[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 3); l173 = (l173 + 1)) {
			fRec150[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 3); l174 = (l174 + 1)) {
			fRec149[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 3); l175 = (l175 + 1)) {
			fRec148[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fVec16[l176] = 0.0f;
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fVec17[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 2); l178 = (l178 + 1)) {
			fVec18[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec190[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 2); l180 = (l180 + 1)) {
			fRec186[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fRec191[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 4); l182 = (l182 + 1)) {
			fRec193[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 1024); l183 = (l183 + 1)) {
			fRec194[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 2); l184 = (l184 + 1)) {
			fVec19[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 3); l185 = (l185 + 1)) {
			fVec20[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 2048); l186 = (l186 + 1)) {
			fRec192[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			fRec182[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 2); l188 = (l188 + 1)) {
			fRec178[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 1024); l189 = (l189 + 1)) {
			fRec174[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 4); l190 = (l190 + 1)) {
			fRec172[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 2048); l191 = (l191 + 1)) {
			fRec167[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			fRec165[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 2); l193 = (l193 + 1)) {
			iRec195[l193] = 0;
		}
		for (int l194 = 0; (l194 < 2); l194 = (l194 + 1)) {
			fRec196[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec199[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 2); l196 = (l196 + 1)) {
			fRec198[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 2); l197 = (l197 + 1)) {
			fRec197[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec200[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec164[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 3); l200 = (l200 + 1)) {
			fRec163[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec162[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 2); l202 = (l202 + 1)) {
			fRec203[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fRec202[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec201[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec206[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec205[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec204[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec209[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fRec208[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec207[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			iRec214[l211] = 0;
		}
		for (int l212 = 0; (l212 < 3); l212 = (l212 + 1)) {
			fRec213[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			iRec216[l213] = 0;
		}
		for (int l214 = 0; (l214 < 3); l214 = (l214 + 1)) {
			fRec215[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 2); l215 = (l215 + 1)) {
			iRec218[l215] = 0;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec217[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			iRec220[l217] = 0;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec219[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fRec223[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 2); l220 = (l220 + 1)) {
			fRec222[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fRec221[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 3); l222 = (l222 + 1)) {
			fRec224[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec212[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec211[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec210[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fRec227[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fRec226[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 2); l228 = (l228 + 1)) {
			fRec225[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec230[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 2); l230 = (l230 + 1)) {
			fRec229[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fRec228[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec233[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 2); l233 = (l233 + 1)) {
			fRec232[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 2); l234 = (l234 + 1)) {
			fRec231[l234] = 0.0f;
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
		ui_interface->declare(&fHslider97, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider97, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider96, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider96, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider98, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider98, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider99, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider99, 0.0f, -96.0f, 0.0f, 0.100000001f);
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
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider101, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider100, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider93, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider102, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider103, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider92, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider92, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider104, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider105, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider91, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider90, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
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
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox0);
		ui_interface->addCheckButton("2", &fCheckbox1);
		ui_interface->addCheckButton("3", &fCheckbox2);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->addCheckButton("5", &fCheckbox4);
		ui_interface->addCheckButton("6", &fCheckbox5);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider24, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider27, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider94, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider95, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider25, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
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
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider0)));
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
		float fSlow15 = float((fSlow14 > 0.0f));
		float fSlow16 = (0.0f - fSlow14);
		float fSlow17 = std::min<float>(10.0f, std::max<float>(7.0f, float(fHslider7)));
		float fSlow18 = (fConst78 * ((1.0f / float(fHslider8)) + -1.0f));
		float fSlow19 = float(fHslider9);
		int iSlow20 = (std::fabs(fSlow19) < 1.1920929e-07f);
		float fSlow21 = (iSlow20 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow20 ? 1.0f : fSlow19)))));
		float fSlow22 = float(fHslider10);
		int iSlow23 = (std::fabs(fSlow22) < 1.1920929e-07f);
		float fSlow24 = (iSlow23 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow23 ? 1.0f : fSlow22)))));
		float fSlow25 = (1.0f - fSlow24);
		float fSlow26 = (1.0f - fSlow21);
		float fSlow27 = float(fHslider11);
		float fSlow28 = (1.0f / float(fHslider12));
		float fSlow29 = std::tan((fConst1 * float(fHslider13)));
		float fSlow30 = (1.0f / fSlow29);
		float fSlow31 = (1.0f / (((fSlow28 + fSlow30) / fSlow29) + 1.0f));
		float fSlow32 = (((fSlow30 - fSlow28) / fSlow29) + 1.0f);
		float fSlow33 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow29))));
		float fSlow34 = (1.0f / fSlow12);
		float fSlow35 = float(fHslider14);
		int iSlow36 = (fSlow35 > 0.0f);
		float fSlow37 = (float(fHslider15) * std::sin((fConst18 * fSlow11)));
		float fSlow38 = (fConst1 * ((fSlow11 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow35)))) / fSlow37));
		float fSlow39 = (fConst1 * (fSlow11 / fSlow37));
		float fSlow40 = (iSlow36 ? fSlow39 : fSlow38);
		float fSlow41 = ((fSlow34 * (fSlow34 - fSlow40)) + 1.0f);
		float fSlow42 = ((fSlow34 * (fSlow34 + fSlow40)) + 1.0f);
		float fSlow43 = (iSlow36 ? fSlow38 : fSlow39);
		float fSlow44 = ((fSlow34 * (fSlow34 + fSlow43)) + 1.0f);
		float fSlow45 = ((fSlow34 * (fSlow34 - fSlow43)) + 1.0f);
		float fSlow46 = (1.0f / fSlow9);
		float fSlow47 = float(fHslider16);
		int iSlow48 = (fSlow47 > 0.0f);
		float fSlow49 = (float(fHslider17) * std::sin((fConst18 * fSlow8)));
		float fSlow50 = (fConst1 * ((fSlow8 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow47)))) / fSlow49));
		float fSlow51 = (fConst1 * (fSlow8 / fSlow49));
		float fSlow52 = (iSlow48 ? fSlow51 : fSlow50);
		float fSlow53 = ((fSlow46 * (fSlow46 - fSlow52)) + 1.0f);
		float fSlow54 = ((fSlow46 * (fSlow46 + fSlow52)) + 1.0f);
		float fSlow55 = (iSlow48 ? fSlow50 : fSlow51);
		float fSlow56 = ((fSlow46 * (fSlow46 + fSlow55)) + 1.0f);
		float fSlow57 = ((fSlow46 * (fSlow46 - fSlow55)) + 1.0f);
		float fSlow58 = (1.0f / fSlow6);
		float fSlow59 = (((fSlow5 - fSlow3) / fSlow4) + 1.0f);
		float fSlow60 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow4))));
		float fSlow61 = (1.0f / float(fHslider19));
		float fSlow62 = std::tan((fConst1 * float(fHslider20)));
		float fSlow63 = (1.0f / fSlow62);
		float fSlow64 = (((fSlow61 + fSlow63) / fSlow62) + 1.0f);
		float fSlow65 = ((std::pow(10.0f, (0.0500000007f * float(fHslider18))) * (1.0f - float(fCheckbox1))) / fSlow64);
		float fSlow66 = float(fHslider21);
		float fSlow67 = std::tan((fConst1 * fSlow66));
		float fSlow68 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow67))));
		float fSlow69 = float(fHslider22);
		float fSlow70 = std::tan((fConst1 * fSlow69));
		float fSlow71 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow70))));
		float fSlow72 = float(fHslider23);
		float fSlow73 = float((fSlow72 > 0.0f));
		float fSlow74 = (0.0f - fSlow72);
		float fSlow75 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider24))) + -0.5f)));
		float fSlow76 = (2.0f * fSlow75);
		float fSlow77 = float(fHslider25);
		float fSlow78 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider26)));
		float fSlow79 = (0.5f * fSlow78);
		float fSlow80 = (0.00100000005f * std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider27))));
		float fSlow81 = (fConst78 * ((1.0f / float(fHslider28)) + -1.0f));
		float fSlow82 = float(fHslider29);
		int iSlow83 = (std::fabs(fSlow82) < 1.1920929e-07f);
		float fSlow84 = (iSlow83 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow83 ? 1.0f : fSlow82)))));
		float fSlow85 = float(fHslider30);
		int iSlow86 = (std::fabs(fSlow85) < 1.1920929e-07f);
		float fSlow87 = (iSlow86 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow86 ? 1.0f : fSlow85)))));
		float fSlow88 = (1.0f - fSlow87);
		float fSlow89 = (1.0f - fSlow84);
		float fSlow90 = float(fHslider31);
		float fSlow91 = (1.0f / float(fHslider32));
		float fSlow92 = std::tan((fConst1 * float(fHslider33)));
		float fSlow93 = (1.0f / fSlow92);
		float fSlow94 = (1.0f / (((fSlow91 + fSlow93) / fSlow92) + 1.0f));
		float fSlow95 = (((fSlow93 - fSlow91) / fSlow92) + 1.0f);
		float fSlow96 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow92))));
		float fSlow97 = (1.0f / fSlow70);
		float fSlow98 = float(fHslider34);
		int iSlow99 = (fSlow98 > 0.0f);
		float fSlow100 = (float(fHslider35) * std::sin((fConst18 * fSlow69)));
		float fSlow101 = (fConst1 * ((fSlow69 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow98)))) / fSlow100));
		float fSlow102 = (fConst1 * (fSlow69 / fSlow100));
		float fSlow103 = (iSlow99 ? fSlow102 : fSlow101);
		float fSlow104 = ((fSlow97 * (fSlow97 - fSlow103)) + 1.0f);
		float fSlow105 = ((fSlow97 * (fSlow97 + fSlow103)) + 1.0f);
		float fSlow106 = (iSlow99 ? fSlow101 : fSlow102);
		float fSlow107 = ((fSlow97 * (fSlow97 + fSlow106)) + 1.0f);
		float fSlow108 = ((fSlow97 * (fSlow97 - fSlow106)) + 1.0f);
		float fSlow109 = (1.0f / fSlow67);
		float fSlow110 = float(fHslider36);
		int iSlow111 = (fSlow110 > 0.0f);
		float fSlow112 = (float(fHslider37) * std::sin((fConst18 * fSlow66)));
		float fSlow113 = (fConst1 * ((fSlow66 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow110)))) / fSlow112));
		float fSlow114 = (fConst1 * (fSlow66 / fSlow112));
		float fSlow115 = (iSlow111 ? fSlow114 : fSlow113);
		float fSlow116 = ((fSlow109 * (fSlow109 - fSlow115)) + 1.0f);
		float fSlow117 = ((fSlow109 * (fSlow109 + fSlow115)) + 1.0f);
		float fSlow118 = (iSlow111 ? fSlow113 : fSlow114);
		float fSlow119 = ((fSlow109 * (fSlow109 + fSlow118)) + 1.0f);
		float fSlow120 = ((fSlow109 * (fSlow109 - fSlow118)) + 1.0f);
		float fSlow121 = (1.0f / fSlow64);
		float fSlow122 = (((fSlow63 - fSlow61) / fSlow62) + 1.0f);
		float fSlow123 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow62))));
		float fSlow124 = (1.0f / float(fHslider39));
		float fSlow125 = std::tan((fConst1 * float(fHslider40)));
		float fSlow126 = (1.0f / fSlow125);
		float fSlow127 = (((fSlow124 + fSlow126) / fSlow125) + 1.0f);
		float fSlow128 = ((std::pow(10.0f, (0.0500000007f * float(fHslider38))) * (1.0f - float(fCheckbox2))) / fSlow127);
		float fSlow129 = float(fHslider41);
		float fSlow130 = std::tan((fConst1 * fSlow129));
		float fSlow131 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow130))));
		float fSlow132 = float(fHslider42);
		float fSlow133 = std::tan((fConst1 * fSlow132));
		float fSlow134 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow133))));
		float fSlow135 = float(fHslider43);
		float fSlow136 = float((fSlow135 > 0.0f));
		float fSlow137 = (0.0f - fSlow135);
		float fSlow138 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider44)));
		float fSlow139 = ((fSlow138 * fSlow75) * ((0.0500000007f * fSlow78) + 1.0f));
		float fSlow140 = (fConst98 * ((0.200000003f * ((0.5f * fSlow17) + -5.0f)) + 1.0f));
		float fSlow141 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider45))) * fSlow75) * (1.0f - (0.0299999993f * fSlow78)));
		float fSlow142 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider46))) * fSlow75) * ((0.310000002f * fSlow78) + 1.0f));
		float fSlow143 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider47))) * fSlow75) * (1.0f - (0.270000011f * fSlow78)));
		float fSlow144 = (fConst78 * ((1.0f / float(fHslider48)) + -1.0f));
		float fSlow145 = float(fHslider49);
		int iSlow146 = (std::fabs(fSlow145) < 1.1920929e-07f);
		float fSlow147 = (iSlow146 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow146 ? 1.0f : fSlow145)))));
		float fSlow148 = float(fHslider50);
		int iSlow149 = (std::fabs(fSlow148) < 1.1920929e-07f);
		float fSlow150 = (iSlow149 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow149 ? 1.0f : fSlow148)))));
		float fSlow151 = (1.0f - fSlow150);
		float fSlow152 = (1.0f - fSlow147);
		float fSlow153 = float(fHslider51);
		float fSlow154 = (1.0f / float(fHslider52));
		float fSlow155 = std::tan((fConst1 * float(fHslider53)));
		float fSlow156 = (1.0f / fSlow155);
		float fSlow157 = (1.0f / (((fSlow154 + fSlow156) / fSlow155) + 1.0f));
		float fSlow158 = (((fSlow156 - fSlow154) / fSlow155) + 1.0f);
		float fSlow159 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow155))));
		float fSlow160 = (1.0f / fSlow133);
		float fSlow161 = float(fHslider54);
		int iSlow162 = (fSlow161 > 0.0f);
		float fSlow163 = (float(fHslider55) * std::sin((fConst18 * fSlow132)));
		float fSlow164 = (fConst1 * ((fSlow132 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow161)))) / fSlow163));
		float fSlow165 = (fConst1 * (fSlow132 / fSlow163));
		float fSlow166 = (iSlow162 ? fSlow165 : fSlow164);
		float fSlow167 = ((fSlow160 * (fSlow160 - fSlow166)) + 1.0f);
		float fSlow168 = ((fSlow160 * (fSlow160 + fSlow166)) + 1.0f);
		float fSlow169 = (iSlow162 ? fSlow164 : fSlow165);
		float fSlow170 = ((fSlow160 * (fSlow160 + fSlow169)) + 1.0f);
		float fSlow171 = ((fSlow160 * (fSlow160 - fSlow169)) + 1.0f);
		float fSlow172 = (1.0f / fSlow130);
		float fSlow173 = float(fHslider56);
		int iSlow174 = (fSlow173 > 0.0f);
		float fSlow175 = (float(fHslider57) * std::sin((fConst18 * fSlow129)));
		float fSlow176 = (fConst1 * ((fSlow129 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow173)))) / fSlow175));
		float fSlow177 = (fConst1 * (fSlow129 / fSlow175));
		float fSlow178 = (iSlow174 ? fSlow177 : fSlow176);
		float fSlow179 = ((fSlow172 * (fSlow172 - fSlow178)) + 1.0f);
		float fSlow180 = ((fSlow172 * (fSlow172 + fSlow178)) + 1.0f);
		float fSlow181 = (iSlow174 ? fSlow176 : fSlow177);
		float fSlow182 = ((fSlow172 * (fSlow172 + fSlow181)) + 1.0f);
		float fSlow183 = ((fSlow172 * (fSlow172 - fSlow181)) + 1.0f);
		float fSlow184 = (1.0f / fSlow127);
		float fSlow185 = (((fSlow126 - fSlow124) / fSlow125) + 1.0f);
		float fSlow186 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow125))));
		float fSlow187 = (1.0f / float(fHslider59));
		float fSlow188 = std::tan((fConst1 * float(fHslider60)));
		float fSlow189 = (1.0f / fSlow188);
		float fSlow190 = (((fSlow187 + fSlow189) / fSlow188) + 1.0f);
		float fSlow191 = ((std::pow(10.0f, (0.0500000007f * float(fHslider58))) * (1.0f - float(fCheckbox3))) / fSlow190);
		float fSlow192 = (0.300000012f * fSlow191);
		float fSlow193 = float(fHslider61);
		float fSlow194 = std::tan((fConst1 * fSlow193));
		float fSlow195 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow194))));
		float fSlow196 = float(fHslider62);
		float fSlow197 = std::tan((fConst1 * fSlow196));
		float fSlow198 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow197))));
		float fSlow199 = float(fHslider63);
		float fSlow200 = float((fSlow199 > 0.0f));
		float fSlow201 = (0.0f - fSlow199);
		float fSlow202 = (fConst78 * ((1.0f / float(fHslider64)) + -1.0f));
		float fSlow203 = float(fHslider65);
		int iSlow204 = (std::fabs(fSlow203) < 1.1920929e-07f);
		float fSlow205 = (iSlow204 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow204 ? 1.0f : fSlow203)))));
		float fSlow206 = float(fHslider66);
		int iSlow207 = (std::fabs(fSlow206) < 1.1920929e-07f);
		float fSlow208 = (iSlow207 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow207 ? 1.0f : fSlow206)))));
		float fSlow209 = (1.0f - fSlow208);
		float fSlow210 = (1.0f - fSlow205);
		float fSlow211 = float(fHslider67);
		float fSlow212 = (1.0f / float(fHslider68));
		float fSlow213 = std::tan((fConst1 * float(fHslider69)));
		float fSlow214 = (1.0f / fSlow213);
		float fSlow215 = (1.0f / (((fSlow212 + fSlow214) / fSlow213) + 1.0f));
		float fSlow216 = (((fSlow214 - fSlow212) / fSlow213) + 1.0f);
		float fSlow217 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow213))));
		float fSlow218 = (1.0f / fSlow197);
		float fSlow219 = float(fHslider70);
		int iSlow220 = (fSlow219 > 0.0f);
		float fSlow221 = (float(fHslider71) * std::sin((fConst18 * fSlow196)));
		float fSlow222 = (fConst1 * ((fSlow196 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow219)))) / fSlow221));
		float fSlow223 = (fConst1 * (fSlow196 / fSlow221));
		float fSlow224 = (iSlow220 ? fSlow223 : fSlow222);
		float fSlow225 = ((fSlow218 * (fSlow218 - fSlow224)) + 1.0f);
		float fSlow226 = ((fSlow218 * (fSlow218 + fSlow224)) + 1.0f);
		float fSlow227 = (iSlow220 ? fSlow222 : fSlow223);
		float fSlow228 = ((fSlow218 * (fSlow218 + fSlow227)) + 1.0f);
		float fSlow229 = ((fSlow218 * (fSlow218 - fSlow227)) + 1.0f);
		float fSlow230 = (1.0f / fSlow194);
		float fSlow231 = float(fHslider72);
		int iSlow232 = (fSlow231 > 0.0f);
		float fSlow233 = (float(fHslider73) * std::sin((fConst18 * fSlow193)));
		float fSlow234 = (fConst1 * ((fSlow193 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow231)))) / fSlow233));
		float fSlow235 = (fConst1 * (fSlow193 / fSlow233));
		float fSlow236 = (iSlow232 ? fSlow235 : fSlow234);
		float fSlow237 = ((fSlow230 * (fSlow230 - fSlow236)) + 1.0f);
		float fSlow238 = ((fSlow230 * (fSlow230 + fSlow236)) + 1.0f);
		float fSlow239 = (iSlow232 ? fSlow234 : fSlow235);
		float fSlow240 = ((fSlow230 * (fSlow230 + fSlow239)) + 1.0f);
		float fSlow241 = ((fSlow230 * (fSlow230 - fSlow239)) + 1.0f);
		float fSlow242 = (1.0f / fSlow190);
		float fSlow243 = (((fSlow189 - fSlow187) / fSlow188) + 1.0f);
		float fSlow244 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow188))));
		float fSlow245 = (1.0f / float(fHslider75));
		float fSlow246 = std::tan((fConst1 * float(fHslider76)));
		float fSlow247 = (1.0f / fSlow246);
		float fSlow248 = (((fSlow245 + fSlow247) / fSlow246) + 1.0f);
		float fSlow249 = ((std::pow(10.0f, (0.0500000007f * float(fHslider74))) * (1.0f - float(fCheckbox4))) / fSlow248);
		float fSlow250 = float(fHslider77);
		float fSlow251 = std::tan((fConst1 * fSlow250));
		float fSlow252 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow251))));
		float fSlow253 = float(fHslider78);
		float fSlow254 = std::tan((fConst1 * fSlow253));
		float fSlow255 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow254))));
		float fSlow256 = (0.5f * fSlow138);
		float fSlow257 = (fConst365 * fSlow75);
		float fSlow258 = (27.0f * fSlow75);
		float fSlow259 = (9.0f * fSlow75);
		float fSlow260 = (3.0f * fSlow75);
		float fSlow261 = (fConst78 * ((1.0f / float(fHslider79)) + -1.0f));
		float fSlow262 = float(fHslider80);
		int iSlow263 = (std::fabs(fSlow262) < 1.1920929e-07f);
		float fSlow264 = (iSlow263 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow263 ? 1.0f : fSlow262)))));
		float fSlow265 = float(fHslider81);
		int iSlow266 = (std::fabs(fSlow265) < 1.1920929e-07f);
		float fSlow267 = (iSlow266 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow266 ? 1.0f : fSlow265)))));
		float fSlow268 = (1.0f - fSlow267);
		float fSlow269 = (1.0f - fSlow264);
		float fSlow270 = float(fHslider82);
		float fSlow271 = (1.0f / float(fHslider83));
		float fSlow272 = std::tan((fConst1 * float(fHslider84)));
		float fSlow273 = (1.0f / fSlow272);
		float fSlow274 = (1.0f / (((fSlow271 + fSlow273) / fSlow272) + 1.0f));
		float fSlow275 = (((fSlow273 - fSlow271) / fSlow272) + 1.0f);
		float fSlow276 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow272))));
		float fSlow277 = (1.0f / fSlow254);
		float fSlow278 = float(fHslider85);
		int iSlow279 = (fSlow278 > 0.0f);
		float fSlow280 = (float(fHslider86) * std::sin((fConst18 * fSlow253)));
		float fSlow281 = (fConst1 * ((fSlow253 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow278)))) / fSlow280));
		float fSlow282 = (fConst1 * (fSlow253 / fSlow280));
		float fSlow283 = (iSlow279 ? fSlow282 : fSlow281);
		float fSlow284 = ((fSlow277 * (fSlow277 - fSlow283)) + 1.0f);
		float fSlow285 = ((fSlow277 * (fSlow277 + fSlow283)) + 1.0f);
		float fSlow286 = (iSlow279 ? fSlow281 : fSlow282);
		float fSlow287 = ((fSlow277 * (fSlow277 + fSlow286)) + 1.0f);
		float fSlow288 = ((fSlow277 * (fSlow277 - fSlow286)) + 1.0f);
		float fSlow289 = (1.0f / fSlow251);
		float fSlow290 = float(fHslider87);
		int iSlow291 = (fSlow290 > 0.0f);
		float fSlow292 = (float(fHslider88) * std::sin((fConst18 * fSlow250)));
		float fSlow293 = (fConst1 * ((fSlow250 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow290)))) / fSlow292));
		float fSlow294 = (fConst1 * (fSlow250 / fSlow292));
		float fSlow295 = (iSlow291 ? fSlow294 : fSlow293);
		float fSlow296 = ((fSlow289 * (fSlow289 - fSlow295)) + 1.0f);
		float fSlow297 = ((fSlow289 * (fSlow289 + fSlow295)) + 1.0f);
		float fSlow298 = (iSlow291 ? fSlow293 : fSlow294);
		float fSlow299 = ((fSlow289 * (fSlow289 + fSlow298)) + 1.0f);
		float fSlow300 = ((fSlow289 * (fSlow289 - fSlow298)) + 1.0f);
		float fSlow301 = (1.0f / fSlow248);
		float fSlow302 = (((fSlow247 - fSlow245) / fSlow246) + 1.0f);
		float fSlow303 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow246))));
		float fSlow304 = (1.0f / float(fHslider90));
		float fSlow305 = std::tan((fConst1 * float(fHslider91)));
		float fSlow306 = (1.0f / fSlow305);
		float fSlow307 = (((fSlow304 + fSlow306) / fSlow305) + 1.0f);
		float fSlow308 = ((std::pow(10.0f, (0.0500000007f * float(fHslider89))) * (1.0f - float(fCheckbox5))) / fSlow307);
		float fSlow309 = float(fHslider92);
		float fSlow310 = std::tan((fConst1 * fSlow309));
		float fSlow311 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow310))));
		float fSlow312 = float(fHslider93);
		float fSlow313 = std::tan((fConst1 * fSlow312));
		float fSlow314 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow313))));
		float fSlow315 = float(fHslider94);
		float fSlow316 = float((fSlow315 > 0.0f));
		float fSlow317 = (0.0f - fSlow315);
		float fSlow318 = (2.0f * fSlow77);
		float fSlow319 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider95)));
		float fSlow320 = (1.0f - fSlow319);
		float fSlow321 = (fConst78 * ((1.0f / float(fHslider96)) + -1.0f));
		float fSlow322 = float(fHslider97);
		int iSlow323 = (std::fabs(fSlow322) < 1.1920929e-07f);
		float fSlow324 = (iSlow323 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow323 ? 1.0f : fSlow322)))));
		float fSlow325 = float(fHslider98);
		int iSlow326 = (std::fabs(fSlow325) < 1.1920929e-07f);
		float fSlow327 = (iSlow326 ? 0.0f : std::exp((0.0f - (fConst79 / (iSlow326 ? 1.0f : fSlow325)))));
		float fSlow328 = (1.0f - fSlow327);
		float fSlow329 = (1.0f - fSlow324);
		float fSlow330 = float(fHslider99);
		float fSlow331 = (1.0f / float(fHslider100));
		float fSlow332 = std::tan((fConst1 * float(fHslider101)));
		float fSlow333 = (1.0f / fSlow332);
		float fSlow334 = (1.0f / (((fSlow331 + fSlow333) / fSlow332) + 1.0f));
		float fSlow335 = (((fSlow333 - fSlow331) / fSlow332) + 1.0f);
		float fSlow336 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow332))));
		float fSlow337 = (1.0f / fSlow313);
		float fSlow338 = float(fHslider102);
		int iSlow339 = (fSlow338 > 0.0f);
		float fSlow340 = (float(fHslider103) * std::sin((fConst18 * fSlow312)));
		float fSlow341 = (fConst1 * ((fSlow312 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow338)))) / fSlow340));
		float fSlow342 = (fConst1 * (fSlow312 / fSlow340));
		float fSlow343 = (iSlow339 ? fSlow342 : fSlow341);
		float fSlow344 = ((fSlow337 * (fSlow337 - fSlow343)) + 1.0f);
		float fSlow345 = ((fSlow337 * (fSlow337 + fSlow343)) + 1.0f);
		float fSlow346 = (iSlow339 ? fSlow341 : fSlow342);
		float fSlow347 = ((fSlow337 * (fSlow337 + fSlow346)) + 1.0f);
		float fSlow348 = ((fSlow337 * (fSlow337 - fSlow346)) + 1.0f);
		float fSlow349 = (1.0f / fSlow310);
		float fSlow350 = float(fHslider104);
		int iSlow351 = (fSlow350 > 0.0f);
		float fSlow352 = (float(fHslider105) * std::sin((fConst18 * fSlow309)));
		float fSlow353 = (fConst1 * ((fSlow309 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow350)))) / fSlow352));
		float fSlow354 = (fConst1 * (fSlow309 / fSlow352));
		float fSlow355 = (iSlow351 ? fSlow354 : fSlow353);
		float fSlow356 = ((fSlow349 * (fSlow349 - fSlow355)) + 1.0f);
		float fSlow357 = ((fSlow349 * (fSlow349 + fSlow355)) + 1.0f);
		float fSlow358 = (iSlow351 ? fSlow353 : fSlow354);
		float fSlow359 = ((fSlow349 * (fSlow349 + fSlow358)) + 1.0f);
		float fSlow360 = ((fSlow349 * (fSlow349 - fSlow358)) + 1.0f);
		float fSlow361 = (1.0f / fSlow307);
		float fSlow362 = (((fSlow306 - fSlow304) / fSlow305) + 1.0f);
		float fSlow363 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow305))));
		float fSlow364 = (1.41400003f * std::sqrt(fSlow0));
		float fSlow365 = (0.949999988f * fSlow191);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			iRec6[0] = ((1103515245 * iRec6[1]) + 12345);
			float fTemp0 = (4.65661287e-10f * float(iRec6[0]));
			fRec5[0] = (fTemp0 - (fConst7 * ((fConst10 * fRec5[2]) + (fConst11 * fRec5[1]))));
			fRec4[0] = ((fConst7 * (((fConst9 * fRec5[0]) + (fConst12 * fRec5[1])) + (fConst9 * fRec5[2]))) - (fConst4 * ((fConst13 * fRec4[2]) + (fConst14 * fRec4[1]))));
			fVec1[0] = fSlow14;
			float fTemp1 = (fSlow14 - fVec1[1]);
			fVec2[0] = fSlow16;
			float fTemp2 = (0.0f - (fSlow14 + fVec2[1]));
			float fTemp3 = std::min<float>(1.0f, ((fTemp1 * float((fTemp1 > 0.0f))) + (fSlow15 * (fTemp2 * float((fTemp2 > 0.0f))))));
			fVec3[0] = fTemp3;
			iRec7[0] = (((iRec7[1] + (iRec7[1] > 0)) * (fTemp3 <= fVec3[1])) + (fTemp3 > fVec3[1]));
			int iTemp4 = int(fTemp3);
			fRec8[0] = (iTemp4 ? fSlow14 : fRec8[1]);
			float fTemp5 = (float(iRec7[0]) / std::max<float>(1.0f, (fConst15 * ((0.00511111133f * (fRec8[0] + -100.0f)) + 1.0f))));
			float fTemp6 = (fConst4 * ((fRec4[2] + (fRec4[0] + (2.0f * fRec4[1]))) * std::max<float>(0.0f, std::min<float>(fTemp5, (2.0f - fTemp5)))));
			fRec3[0] = (fTemp6 - ((fConst17 * (fRec3[1] * std::cos((fConst18 * (fRec8[0] + 50.0f))))) + (fConst19 * fRec3[2])));
			fRec9[0] = (fTemp6 - ((fConst21 * (fRec9[1] * std::cos((fConst18 * (fRec8[0] + 250.0f))))) + (fConst22 * fRec9[2])));
			fRec10[0] = (fTemp6 - ((fConst24 * (fRec10[1] * std::cos((fConst18 * (fRec8[0] + 450.0f))))) + (fConst25 * fRec10[2])));
			fRec11[0] = (fTemp6 - ((fConst27 * (fRec11[1] * std::cos((fConst18 * (fRec8[0] + 650.0f))))) + (fConst28 * fRec11[2])));
			fRec12[0] = (fTemp6 - ((fConst30 * (fRec12[1] * std::cos((fConst18 * (fRec8[0] + 850.0f))))) + (fConst31 * fRec12[2])));
			fRec13[0] = (fTemp6 - ((fConst33 * (fRec13[1] * std::cos((fConst18 * (fRec8[0] + 1050.0f))))) + (fConst34 * fRec13[2])));
			fRec14[0] = (fTemp6 - ((fConst36 * (fRec14[1] * std::cos((fConst18 * (fRec8[0] + 1250.0f))))) + (fConst37 * fRec14[2])));
			fRec15[0] = (fTemp6 - ((fConst39 * (fRec15[1] * std::cos((fConst18 * (fRec8[0] + 1450.0f))))) + (fConst40 * fRec15[2])));
			fRec16[0] = (fTemp6 - ((fConst42 * (fRec16[1] * std::cos((fConst18 * (fRec8[0] + 1650.0f))))) + (fConst43 * fRec16[2])));
			fRec17[0] = (fTemp6 - ((fConst45 * (fRec17[1] * std::cos((fConst18 * (fRec8[0] + 1850.0f))))) + (fConst46 * fRec17[2])));
			fRec18[0] = (fTemp6 - ((fConst48 * (fRec18[1] * std::cos((fConst18 * (fRec8[0] + 2050.0f))))) + (fConst49 * fRec18[2])));
			fRec19[0] = (fTemp6 - ((fConst51 * (fRec19[1] * std::cos((fConst18 * (fRec8[0] + 2250.0f))))) + (fConst52 * fRec19[2])));
			fRec20[0] = (fTemp6 - ((fConst54 * (fRec20[1] * std::cos((fConst18 * (fRec8[0] + 2450.0f))))) + (fConst55 * fRec20[2])));
			fRec21[0] = (fTemp6 - ((fConst57 * (fRec21[1] * std::cos((fConst18 * (fRec8[0] + 2650.0f))))) + (fConst58 * fRec21[2])));
			fRec22[0] = (fTemp6 - ((fConst60 * (fRec22[1] * std::cos((fConst18 * (fRec8[0] + 2850.0f))))) + (fConst61 * fRec22[2])));
			fRec23[0] = (fTemp6 - ((fConst63 * (fRec23[1] * std::cos((fConst18 * (fRec8[0] + 3050.0f))))) + (fConst64 * fRec23[2])));
			fRec24[0] = (fTemp6 - ((fConst66 * (fRec24[1] * std::cos((fConst18 * (fRec8[0] + 3250.0f))))) + (fConst67 * fRec24[2])));
			fRec25[0] = (fTemp6 - ((fConst69 * (fRec25[1] * std::cos((fConst18 * (fRec8[0] + 3450.0f))))) + (fConst70 * fRec25[2])));
			fRec26[0] = (fTemp6 - ((fConst72 * (fRec26[1] * std::cos((fConst18 * (fRec8[0] + 3650.0f))))) + (fConst73 * fRec26[2])));
			fRec27[0] = (fTemp6 - ((fConst75 * (fRec27[1] * std::cos((fConst18 * (fRec8[0] + 3850.0f))))) + (fConst76 * fRec27[2])));
			fRec28[0] = (iTemp4 ? fSlow17 : fRec28[1]);
			float fTemp7 = (((((((((((((((((((((fRec3[0] + (0.25f * (fRec9[0] - fRec9[2]))) + (0.111111112f * (fRec10[0] - fRec10[2]))) + (0.0625f * (fRec11[0] - fRec11[2]))) + (0.0399999991f * (fRec12[0] - fRec12[2]))) + (0.027777778f * (fRec13[0] - fRec13[2]))) + (0.0204081628f * (fRec14[0] - fRec14[2]))) + (0.015625f * (fRec15[0] - fRec15[2]))) + (0.0123456791f * (fRec16[0] - fRec16[2]))) + (0.00999999978f * (fRec17[0] - fRec17[2]))) + (0.00826446246f * (fRec18[0] - fRec18[2]))) + (0.0069444445f * (fRec19[0] - fRec19[2]))) + (0.00591715984f * (fRec20[0] - fRec20[2]))) + (0.00510204071f * (fRec21[0] - fRec21[2]))) + (0.00444444455f * (fRec22[0] - fRec22[2]))) + (0.00390625f * (fRec23[0] - fRec23[2]))) + (0.00346020772f * (fRec24[0] - fRec24[2]))) + (0.00308641978f * (fRec25[0] - fRec25[2]))) + (0.00277008303f * (fRec26[0] - fRec26[2]))) + (0.00249999994f * (fRec27[0] - fRec27[2]))) - fRec3[2]) * std::pow(10.0f, (0.144444451f * (fRec28[0] + -10.0f))));
			float fTemp8 = std::fabs((0.0125000002f * fTemp7));
			fRec31[0] = std::max<float>(fTemp8, ((fRec31[1] * fSlow24) + (fTemp8 * fSlow25)));
			fRec30[0] = ((fRec30[1] * fSlow21) + (fRec31[0] * fSlow26));
			fRec29[0] = ((fConst77 * fRec29[1]) + (fSlow18 * std::max<float>(((20.0f * std::log10(fRec30[0])) - fSlow27), 0.0f)));
			float fTemp9 = (0.0125000002f * (fTemp7 * std::pow(10.0f, (0.0500000007f * fRec29[0]))));
			fRec32[0] = (fTemp9 - (fSlow31 * ((fSlow32 * fRec32[2]) + (fSlow33 * fRec32[1]))));
			float fTemp10 = (fSlow13 * fRec2[1]);
			fRec2[0] = (fTemp9 - ((fSlow31 * (fRec32[2] + (fRec32[0] + (2.0f * fRec32[1])))) + (((fRec2[2] * fSlow41) + fTemp10) / fSlow42)));
			float fTemp11 = (fSlow10 * fRec1[1]);
			fRec1[0] = ((((fTemp10 + (fRec2[0] * fSlow44)) + (fRec2[2] * fSlow45)) / fSlow42) - (((fRec1[2] * fSlow53) + fTemp11) / fSlow54));
			fRec0[0] = ((((fTemp11 + (fRec1[0] * fSlow56)) + (fRec1[2] * fSlow57)) / fSlow54) - (fSlow58 * ((fSlow59 * fRec0[2]) + (fSlow60 * fRec0[1]))));
			fVec4[0] = fSlow72;
			float fTemp12 = (fSlow72 - fVec4[1]);
			fVec5[0] = fSlow74;
			float fTemp13 = (0.0f - (fSlow72 + fVec5[1]));
			float fTemp14 = std::min<float>(1.0f, ((fTemp12 * float((fTemp12 > 0.0f))) + (fSlow73 * (fTemp13 * float((fTemp13 > 0.0f))))));
			fVec6[0] = fTemp14;
			iRec38[0] = (((iRec38[1] + (iRec38[1] > 0)) * (fTemp14 <= fVec6[1])) + (fTemp14 > fVec6[1]));
			float fTemp15 = float(iRec38[0]);
			float fTemp16 = (fConst85 * fTemp15);
			float fTemp17 = (fConst84 - fTemp15);
			fRec37[0] = ((0.999000013f * fRec37[1]) + (0.00100000005f * std::max<float>(0.0f, std::min<float>(fTemp16, ((fConst86 * fTemp17) + 1.0f)))));
			int iTemp18 = int(fTemp14);
			fRec39[0] = (iTemp18 ? fSlow77 : fRec39[1]);
			fRec42[0] = (fConst87 + (fRec42[1] - std::floor((fConst87 + fRec42[1]))));
			fRec40[0] = ((0.999000013f * fRec40[1]) + (0.00100000005f * ((fSlow79 * ftbl0mydspSIG0[int((65536.0f * fRec42[0]))]) + 1.0f)));
			float fTemp19 = std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow76 * (fRec39[0] * fRec40[0]))));
			float fTemp20 = ((0.00366666657f * (400.0f - fTemp19)) + 3.0f);
			float fTemp21 = std::max<float>(1.1920929e-07f, std::fabs(fTemp19));
			float fTemp22 = (fRec44[1] + (fConst79 * fTemp21));
			float fTemp23 = (fTemp22 + -1.0f);
			int iTemp24 = (fTemp23 < 0.0f);
			fRec44[0] = (iTemp24 ? fTemp22 : fTemp23);
			float fRec45 = (iTemp24 ? fTemp22 : (fTemp22 + (fTemp23 * (1.0f - (fConst0 / fTemp21)))));
			float fTemp25 = (0.400000006f * ((2.0f * fRec45) + -1.0f));
			fRec46[0] = (fSlow80 + (0.999000013f * fRec46[1]));
			float fTemp26 = (fRec46[0] + 10.0f);
			int iTemp27 = (fTemp26 < 23.0f);
			int iTemp28 = (fTemp26 < 24.0f);
			float fTemp29 = (fRec46[0] + -13.0f);
			int iTemp30 = (fTemp26 < 22.0f);
			float fTemp31 = (fRec46[0] + -12.0f);
			int iTemp32 = (fTemp26 < 21.0f);
			float fTemp33 = (fRec46[0] + -11.0f);
			int iTemp34 = (fTemp26 < 20.0f);
			float fTemp35 = (fRec46[0] + -10.0f);
			int iTemp36 = (fTemp26 < 19.0f);
			float fTemp37 = (fRec46[0] + -9.0f);
			int iTemp38 = (fTemp26 < 18.0f);
			float fTemp39 = (fRec46[0] + -8.0f);
			int iTemp40 = (fTemp26 < 17.0f);
			float fTemp41 = (fRec46[0] + -7.0f);
			int iTemp42 = (fTemp26 < 16.0f);
			float fTemp43 = (fRec46[0] + -6.0f);
			float fTemp44 = (80.0f * fTemp43);
			int iTemp45 = (fTemp26 < 15.0f);
			float fTemp46 = (fRec46[0] + -5.0f);
			int iTemp47 = (fTemp26 < 14.0f);
			float fTemp48 = (fRec46[0] + -4.0f);
			int iTemp49 = (fTemp26 < 13.0f);
			float fTemp50 = (fRec46[0] + -3.0f);
			int iTemp51 = (fTemp26 < 12.0f);
			float fTemp52 = (fRec46[0] + -2.0f);
			int iTemp53 = (fTemp26 < 11.0f);
			float fTemp54 = (fRec46[0] + -1.0f);
			int iTemp55 = (fTemp26 < 10.0f);
			int iTemp56 = (fTemp26 < 9.0f);
			float fTemp57 = (fRec46[0] + 1.0f);
			int iTemp58 = (fTemp26 < 8.0f);
			float fTemp59 = (fRec46[0] + 2.0f);
			float fTemp60 = (50.0f * fTemp59);
			int iTemp61 = (fTemp26 < 7.0f);
			float fTemp62 = (fRec46[0] + 3.0f);
			int iTemp63 = (fTemp26 < 6.0f);
			float fTemp64 = (fRec46[0] + 4.0f);
			int iTemp65 = (fTemp26 < 5.0f);
			float fTemp66 = (fRec46[0] + 5.0f);
			int iTemp67 = (fTemp26 < 4.0f);
			float fTemp68 = (fRec46[0] + 6.0f);
			int iTemp69 = (fTemp26 < 3.0f);
			float fTemp70 = (fRec46[0] + 7.0f);
			int iTemp71 = (fTemp26 < 2.0f);
			float fTemp72 = (fRec46[0] + 8.0f);
			int iTemp73 = (fTemp26 < 1.0f);
			float fTemp74 = (fRec46[0] + 9.0f);
			int iTemp75 = (fTemp26 < 0.0f);
			float fTemp76 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 800.0f : (iTemp73 ? (800.0f - (400.0f * fTemp26)) : 400.0f)) : (iTemp71 ? (400.0f - (50.0f * fTemp74)) : 350.0f)) : (iTemp69 ? ((100.0f * fTemp72) + 350.0f) : 450.0f)) : (iTemp67 ? (450.0f - (125.0f * fTemp70)) : 325.0f)) : (iTemp65 ? ((275.0f * fTemp68) + 325.0f) : 600.0f)) : (iTemp63 ? (600.0f - (200.0f * fTemp66)) : 400.0f)) : (iTemp61 ? (400.0f - (150.0f * fTemp64)) : 250.0f)) : (iTemp58 ? ((150.0f * fTemp62) + 250.0f) : 400.0f)) : (iTemp56 ? (400.0f - fTemp60) : 350.0f)) : (iTemp55 ? ((310.0f * fTemp57) + 350.0f) : 660.0f)) : (iTemp53 ? (660.0f - (220.0f * fRec46[0])) : 440.0f)) : (iTemp51 ? (440.0f - (170.0f * fTemp54)) : 270.0f)) : (iTemp49 ? ((160.0f * fTemp52) + 270.0f) : 430.0f)) : (iTemp47 ? (430.0f - (60.0f * fTemp50)) : 370.0f)) : (iTemp45 ? ((430.0f * fTemp48) + 370.0f) : 800.0f)) : (iTemp42 ? (800.0f - (450.0f * fTemp46)) : 350.0f)) : (iTemp40 ? (350.0f - fTemp44) : 270.0f)) : (iTemp38 ? ((180.0f * fTemp41) + 270.0f) : 450.0f)) : (iTemp36 ? (450.0f - (125.0f * fTemp39)) : 325.0f)) : (iTemp34 ? (325.0f * (fTemp37 + 1.0f)) : 650.0f)) : (iTemp32 ? (650.0f - (250.0f * fTemp35)) : 400.0f)) : (iTemp30 ? (400.0f - (110.0f * fTemp33)) : 290.0f)) : (iTemp27 ? ((110.0f * fTemp31) + 290.0f) : 400.0f)) : (iTemp28 ? (400.0f - (50.0f * fTemp29)) : 350.0f));
			float fTemp77 = ((fTemp76 <= fTemp19) ? fTemp19 : fTemp76);
			float fTemp78 = std::tan((fConst1 * fTemp77));
			float fTemp79 = (1.0f / fTemp78);
			float fTemp80 = (20.0f * fTemp35);
			float fTemp81 = (20.0f * fTemp46);
			float fTemp82 = (10.0f * fRec46[0]);
			float fTemp83 = (10.0f * fTemp68);
			float fTemp84 = (20.0f * fTemp70);
			float fTemp85 = (20.0f * fTemp72);
			float fTemp86 = (20.0f * fTemp26);
			float fTemp87 = ((iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 80.0f : (iTemp73 ? (80.0f - fTemp86) : 60.0f)) : (iTemp71 ? (60.0f - (10.0f * fTemp74)) : 50.0f)) : (iTemp69 ? (fTemp85 + 50.0f) : 70.0f)) : (iTemp67 ? (70.0f - fTemp84) : 50.0f)) : (iTemp65 ? (fTemp83 + 50.0f) : 60.0f)) : (iTemp63 ? (60.0f - (20.0f * fTemp66)) : 40.0f)) : (iTemp61 ? ((20.0f * fTemp64) + 40.0f) : 60.0f)) : (iTemp58 ? (60.0f - (20.0f * fTemp62)) : 40.0f)) : 40.0f) : (iTemp55 ? (40.0f * (fTemp57 + 1.0f)) : 80.0f)) : (iTemp53 ? (80.0f - fTemp82) : 70.0f)) : (iTemp51 ? (70.0f - (30.0f * fTemp54)) : 40.0f)) : 40.0f) : 40.0f) : (iTemp45 ? (40.0f * (fTemp48 + 1.0f)) : 80.0f)) : (iTemp42 ? (80.0f - fTemp81) : 60.0f)) : 60.0f) : (iTemp38 ? (60.0f - (20.0f * fTemp41)) : 40.0f)) : (iTemp36 ? ((10.0f * fTemp39) + 40.0f) : 50.0f)) : 50.0f) : (iTemp32 ? (fTemp80 + 50.0f) : 70.0f)) : (iTemp30 ? (70.0f - (30.0f * fTemp33)) : 40.0f)) : (iTemp27 ? ((30.0f * fTemp31) + 40.0f) : 70.0f)) : (iTemp28 ? (70.0f - (30.0f * fTemp29)) : 40.0f)) / fTemp77);
			float fTemp88 = (((fTemp79 + fTemp87) / fTemp78) + 1.0f);
			fRec43[0] = (fTemp25 - (((fRec43[2] * (((fTemp79 - fTemp87) / fTemp78) + 1.0f)) + (2.0f * (fRec43[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp78)))))) / fTemp88));
			float fTemp89 = (fTemp78 * fTemp88);
			float fTemp90 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 1150.0f : (iTemp73 ? ((450.0f * fTemp26) + 1150.0f) : 1600.0f)) : (iTemp71 ? ((100.0f * fTemp74) + 1600.0f) : 1700.0f)) : (iTemp69 ? (1700.0f - (900.0f * fTemp72)) : 800.0f)) : (iTemp67 ? (800.0f - (100.0f * fTemp70)) : 700.0f)) : (iTemp65 ? ((340.0f * fTemp68) + 700.0f) : 1040.0f)) : (iTemp63 ? ((580.0f * fTemp66) + 1040.0f) : 1620.0f)) : (iTemp61 ? ((130.0f * fTemp64) + 1620.0f) : 1750.0f)) : (iTemp58 ? (1750.0f - (1000.0f * fTemp62)) : 750.0f)) : (iTemp56 ? (750.0f - (150.0f * fTemp59)) : 600.0f)) : (iTemp55 ? ((520.0f * fTemp57) + 600.0f) : 1120.0f)) : (iTemp53 ? ((680.0f * fRec46[0]) + 1120.0f) : 1800.0f)) : (iTemp51 ? ((50.0f * fTemp54) + 1800.0f) : 1850.0f)) : (iTemp49 ? (1850.0f - (1030.0f * fTemp52)) : 820.0f)) : (iTemp47 ? (820.0f - (190.0f * fTemp50)) : 630.0f)) : (iTemp45 ? ((520.0f * fTemp48) + 630.0f) : 1150.0f)) : (iTemp42 ? ((850.0f * fTemp46) + 1150.0f) : 2000.0f)) : (iTemp40 ? ((140.0f * fTemp43) + 2000.0f) : 2140.0f)) : (iTemp38 ? (2140.0f - (1340.0f * fTemp41)) : 800.0f)) : (iTemp36 ? (800.0f - (100.0f * fTemp39)) : 700.0f)) : (iTemp34 ? ((380.0f * fTemp37) + 700.0f) : 1080.0f)) : (iTemp32 ? ((620.0f * fTemp35) + 1080.0f) : 1700.0f)) : (iTemp30 ? ((170.0f * fTemp33) + 1700.0f) : 1870.0f)) : (iTemp27 ? (1870.0f - (1070.0f * fTemp31)) : 800.0f)) : (iTemp28 ? (800.0f - (200.0f * fTemp29)) : 600.0f));
			float fTemp91 = std::tan((fConst1 * fTemp90));
			float fTemp92 = (1.0f / fTemp91);
			float fTemp93 = (10.0f * fTemp31);
			float fTemp94 = (10.0f * fTemp35);
			float fTemp95 = (10.0f * fTemp57);
			float fTemp96 = (10.0f * fTemp66);
			float fTemp97 = ((iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 90.0f : (iTemp73 ? (90.0f - (10.0f * fTemp26)) : 80.0f)) : (iTemp71 ? ((20.0f * fTemp74) + 80.0f) : 100.0f)) : (iTemp69 ? (100.0f - fTemp85) : 80.0f)) : (iTemp67 ? (80.0f - fTemp84) : 60.0f)) : (iTemp65 ? (fTemp83 + 60.0f) : 70.0f)) : (iTemp63 ? (fTemp96 + 70.0f) : 80.0f)) : (iTemp61 ? ((10.0f * fTemp64) + 80.0f) : 90.0f)) : (iTemp58 ? (90.0f - (10.0f * fTemp62)) : 80.0f)) : 80.0f) : (iTemp55 ? (fTemp95 + 80.0f) : 90.0f)) : (iTemp53 ? (90.0f - fTemp82) : 80.0f)) : (iTemp51 ? ((10.0f * fTemp54) + 80.0f) : 90.0f)) : (iTemp49 ? (90.0f - (10.0f * fTemp52)) : 80.0f)) : (iTemp47 ? (80.0f - (20.0f * fTemp50)) : 60.0f)) : (iTemp45 ? ((30.0f * fTemp48) + 60.0f) : 90.0f)) : (iTemp42 ? ((10.0f * fTemp46) + 90.0f) : 100.0f)) : (iTemp40 ? (100.0f - (10.0f * fTemp43)) : 90.0f)) : (iTemp38 ? (90.0f - (10.0f * fTemp41)) : 80.0f)) : (iTemp36 ? (80.0f - (20.0f * fTemp39)) : 60.0f)) : (iTemp34 ? ((30.0f * fTemp37) + 60.0f) : 90.0f)) : (iTemp32 ? (90.0f - fTemp94) : 80.0f)) : (iTemp30 ? ((10.0f * fTemp33) + 80.0f) : 90.0f)) : (iTemp27 ? (90.0f - fTemp93) : 80.0f)) : (iTemp28 ? (80.0f - (20.0f * fTemp29)) : 60.0f)) / fTemp90);
			float fTemp98 = (((fTemp92 + fTemp97) / fTemp91) + 1.0f);
			fRec47[0] = (fTemp25 - (((fRec47[2] * (((fTemp92 - fTemp97) / fTemp91) + 1.0f)) + (2.0f * (fRec47[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp91)))))) / fTemp98));
			float fTemp99 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 0.630957007f : (iTemp73 ? (0.630957007f - (0.567861021f * fTemp26)) : 0.0630960017f)) : (iTemp71 ? ((0.0369039997f * fTemp74) + 0.0630960017f) : 0.100000001f)) : (iTemp69 ? ((0.254812986f * fTemp72) + 0.100000001f) : 0.35481301f)) : (iTemp67 ? (0.35481301f - (0.103624001f * fTemp70)) : 0.251188993f)) : (iTemp65 ? ((0.195494995f * fTemp68) + 0.251188993f) : 0.446684003f)) : (iTemp63 ? (0.446684003f - (0.195494995f * fTemp66)) : 0.251188993f)) : (iTemp61 ? (0.251188993f - (0.219566002f * fTemp64)) : 0.0316229984f)) : (iTemp58 ? ((0.250214994f * fTemp62) + 0.0316229984f) : 0.281838f)) : (iTemp56 ? (0.281838f - (0.181838006f * fTemp59)) : 0.100000001f)) : (iTemp55 ? ((0.401187003f * fTemp57) + 0.100000001f) : 0.501187027f)) : (iTemp53 ? (0.501187027f - (0.301661015f * fRec46[0])) : 0.199525997f)) : (iTemp51 ? (0.199525997f - (0.136429995f * fTemp54)) : 0.0630960017f)) : (iTemp49 ? ((0.253131986f * fTemp52) + 0.0630960017f) : 0.316228002f)) : (iTemp47 ? (0.316228002f - (0.216227993f * fTemp50)) : 0.100000001f)) : (iTemp45 ? ((0.401187003f * fTemp48) + 0.100000001f) : 0.501187027f)) : (iTemp42 ? (0.501187027f - (0.401187003f * fTemp46)) : 0.100000001f)) : (iTemp40 ? ((0.151188999f * fTemp43) + 0.100000001f) : 0.251188993f)) : (iTemp38 ? ((0.0306490008f * fTemp41) + 0.251188993f) : 0.281838f)) : (iTemp36 ? (0.281838f - (0.123349003f * fTemp39)) : 0.158489004f)) : (iTemp34 ? ((0.342698008f * fTemp37) + 0.158489004f) : 0.501187027f)) : (iTemp32 ? (0.501187027f - (0.301661015f * fTemp35)) : 0.199525997f)) : (iTemp30 ? (0.199525997f - (0.0216979999f * fTemp33)) : 0.177827999f)) : (iTemp27 ? ((0.138400003f * fTemp31) + 0.177827999f) : 0.316228002f)) : (iTemp28 ? (0.316228002f - (0.216227993f * fTemp29)) : 0.100000001f));
			float fTemp100 = (fTemp91 * fTemp98);
			float fTemp101 = (100.0f * fTemp29);
			float fTemp102 = (50.0f * fTemp37);
			float fTemp103 = (50.0f * fRec46[0]);
			float fTemp104 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 2800.0f : (iTemp73 ? (2800.0f - (100.0f * fTemp26)) : 2700.0f)) : 2700.0f) : (iTemp69 ? ((130.0f * fTemp72) + 2700.0f) : 2830.0f)) : (iTemp67 ? (2830.0f - (300.0f * fTemp70)) : 2530.0f)) : (iTemp65 ? (2530.0f - (280.0f * fTemp68)) : 2250.0f)) : (iTemp63 ? ((150.0f * fTemp66) + 2250.0f) : 2400.0f)) : (iTemp61 ? ((200.0f * fTemp64) + 2400.0f) : 2600.0f)) : (iTemp58 ? (2600.0f - (200.0f * fTemp62)) : 2400.0f)) : 2400.0f) : (iTemp55 ? ((350.0f * fTemp57) + 2400.0f) : 2750.0f)) : (iTemp53 ? (2750.0f - fTemp103) : 2700.0f)) : (iTemp51 ? ((200.0f * fTemp54) + 2700.0f) : 2900.0f)) : (iTemp49 ? (2900.0f - (200.0f * fTemp52)) : 2700.0f)) : (iTemp47 ? ((50.0f * fTemp50) + 2700.0f) : 2750.0f)) : (iTemp45 ? ((150.0f * fTemp48) + 2750.0f) : 2900.0f)) : (iTemp42 ? (2900.0f - (100.0f * fTemp46)) : 2800.0f)) : (iTemp40 ? ((150.0f * fTemp43) + 2800.0f) : 2950.0f)) : (iTemp38 ? (2950.0f - (120.0f * fTemp41)) : 2830.0f)) : (iTemp36 ? (2830.0f - (130.0f * fTemp39)) : 2700.0f)) : (iTemp34 ? (2700.0f - fTemp102) : 2650.0f)) : (iTemp32 ? (2650.0f - (50.0f * fTemp35)) : 2600.0f)) : (iTemp30 ? ((200.0f * fTemp33) + 2600.0f) : 2800.0f)) : (iTemp27 ? (2800.0f - (200.0f * fTemp31)) : 2600.0f)) : (iTemp28 ? (fTemp101 + 2600.0f) : 2700.0f));
			float fTemp105 = std::tan((fConst1 * fTemp104));
			float fTemp106 = (1.0f / fTemp105);
			float fTemp107 = (20.0f * fTemp48);
			float fTemp108 = (20.0f * fRec46[0]);
			float fTemp109 = (20.0f * fTemp57);
			float fTemp110 = (60.0f * fTemp68);
			float fTemp111 = ((iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? 120.0f : (iTemp69 ? (120.0f - fTemp85) : 100.0f)) : (iTemp67 ? ((70.0f * fTemp70) + 100.0f) : 170.0f)) : (iTemp65 ? (170.0f - fTemp110) : 110.0f)) : (iTemp63 ? (110.0f - fTemp96) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp55 ? (fTemp109 + 100.0f) : 120.0f)) : (iTemp53 ? (120.0f - fTemp108) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iTemp45 ? (fTemp107 + 100.0f) : 120.0f)) : 120.0f) : (iTemp40 ? (120.0f - (20.0f * fTemp43)) : 100.0f)) : 100.0f) : (iTemp36 ? ((70.0f * fTemp39) + 100.0f) : 170.0f)) : (iTemp34 ? (170.0f - fTemp102) : 120.0f)) : (iTemp32 ? (120.0f - fTemp80) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fTemp104);
			float fTemp112 = (((fTemp106 + fTemp111) / fTemp105) + 1.0f);
			fRec48[0] = (fTemp25 - (((fRec48[2] * (((fTemp106 - fTemp111) / fTemp105) + 1.0f)) + (2.0f * (fRec48[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp105)))))) / fTemp112));
			float fTemp113 = (iTemp38 ? ((0.0293140002f * fTemp41) + 0.0501190014f) : 0.0794330016f);
			float fTemp114 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 0.100000001f : (iTemp73 ? (0.100000001f - (0.0683770031f * fTemp26)) : 0.0316229984f)) : 0.0316229984f) : (iTemp69 ? ((0.126865998f * fTemp72) + 0.0316229984f) : 0.158489004f)) : (iTemp67 ? (0.158489004f - (0.126865998f * fTemp70)) : 0.0316229984f)) : (iTemp65 ? ((0.323190004f * fTemp68) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iTemp61 ? (0.35481301f - (0.196324006f * fTemp64)) : 0.158489004f)) : (iTemp58 ? (0.158489004f - (0.0693639964f * fTemp62)) : 0.0891249999f)) : (iTemp56 ? (0.0891249999f - (0.0640060008f * fTemp59)) : 0.0251189992f)) : (iTemp55 ? ((0.0456760004f * fTemp57) + 0.0251189992f) : 0.0707949996f)) : (iTemp53 ? ((0.0550980009f * fRec46[0]) + 0.0707949996f) : 0.125892997f)) : (iTemp51 ? (0.125892997f - (0.0627970025f * fTemp54)) : 0.0630960017f)) : (iTemp49 ? (0.0630960017f - (0.0129770003f * fTemp52)) : 0.0501190014f)) : (iTemp47 ? ((0.020676f * fTemp50) + 0.0501190014f) : 0.0707949996f)) : (iTemp45 ? (0.0707949996f - (0.0456760004f * fTemp48)) : 0.0251189992f)) : (iTemp42 ? ((0.152709007f * fTemp46) + 0.0251189992f) : 0.177827999f)) : (iTemp40 ? (0.177827999f - (0.127709001f * fTemp43)) : 0.0501190014f)) : fTemp113) : (iTemp36 ? (0.0794330016f - (0.0616500005f * fTemp39)) : 0.0177829992f)) : (iTemp34 ? ((0.428900987f * fTemp37) + 0.0177829992f) : 0.446684003f)) : (iTemp32 ? (0.446684003f - (0.195494995f * fTemp35)) : 0.251188993f)) : (iTemp30 ? (0.251188993f - (0.125295997f * fTemp33)) : 0.125892997f)) : (iTemp27 ? ((0.125295997f * fTemp31) + 0.125892997f) : 0.251188993f)) : (iTemp28 ? (0.251188993f - (0.109935001f * fTemp29)) : 0.141253993f));
			float fTemp115 = (fTemp105 * fTemp112);
			float fTemp116 = (350.0f * fTemp66);
			float fTemp117 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 3500.0f : (iTemp73 ? (3500.0f - (200.0f * fTemp26)) : 3300.0f)) : (iTemp71 ? ((400.0f * fTemp74) + 3300.0f) : 3700.0f)) : (iTemp69 ? (3700.0f - (200.0f * fTemp72)) : 3500.0f)) : 3500.0f) : (iTemp65 ? (3500.0f - (1050.0f * fTemp68)) : 2450.0f)) : (iTemp63 ? (fTemp116 + 2450.0f) : 2800.0f)) : (iTemp61 ? ((250.0f * fTemp64) + 2800.0f) : 3050.0f)) : (iTemp58 ? (3050.0f - (450.0f * fTemp62)) : 2600.0f)) : (iTemp56 ? ((75.0f * fTemp59) + 2600.0f) : 2675.0f)) : (iTemp55 ? ((325.0f * fTemp57) + 2675.0f) : 3000.0f)) : 3000.0f) : (iTemp51 ? ((350.0f * fTemp54) + 3000.0f) : 3350.0f)) : (iTemp49 ? (3350.0f - (350.0f * fTemp52)) : 3000.0f)) : 3000.0f) : (iTemp45 ? ((900.0f * fTemp48) + 3000.0f) : 3900.0f)) : (iTemp42 ? (3900.0f - (300.0f * fTemp46)) : 3600.0f)) : (iTemp40 ? ((300.0f * fTemp43) + 3600.0f) : 3900.0f)) : (iTemp38 ? (3900.0f - (100.0f * fTemp41)) : 3800.0f)) : 3800.0f) : (iTemp34 ? (3800.0f - (900.0f * fTemp37)) : 2900.0f)) : (iTemp32 ? ((300.0f * fTemp35) + 2900.0f) : 3200.0f)) : (iTemp30 ? ((50.0f * fTemp33) + 3200.0f) : 3250.0f)) : (iTemp27 ? (3250.0f - (450.0f * fTemp31)) : 2800.0f)) : (iTemp28 ? (fTemp101 + 2800.0f) : 2900.0f));
			float fTemp118 = std::tan((fConst1 * fTemp117));
			float fTemp119 = (1.0f / fTemp118);
			float fTemp120 = ((iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 130.0f : (iTemp73 ? (fTemp86 + 130.0f) : 150.0f)) : 150.0f) : (iTemp69 ? (150.0f - fTemp85) : 130.0f)) : (iTemp67 ? ((50.0f * fTemp70) + 130.0f) : 180.0f)) : (iTemp65 ? (180.0f - fTemp110) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iTemp55 ? (fTemp95 + 120.0f) : 130.0f)) : (iTemp53 ? (130.0f - fTemp82) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp45 ? ((10.0f * fTemp48) + 120.0f) : 130.0f)) : (iTemp42 ? (fTemp81 + 130.0f) : 150.0f)) : (iTemp40 ? (150.0f - (30.0f * fTemp43)) : 120.0f)) : 120.0f) : (iTemp36 ? ((60.0f * fTemp39) + 120.0f) : 180.0f)) : (iTemp34 ? (180.0f - fTemp102) : 130.0f)) : (iTemp32 ? (130.0f - fTemp94) : 120.0f)) : 120.0f) : (iTemp27 ? (fTemp93 + 120.0f) : 130.0f)) : (iTemp28 ? (130.0f - (10.0f * fTemp29)) : 120.0f)) / fTemp117);
			float fTemp121 = (((fTemp119 + fTemp120) / fTemp118) + 1.0f);
			fRec49[0] = (fTemp25 - (((fRec49[2] * (((fTemp119 - fTemp120) / fTemp118) + 1.0f)) + (2.0f * (fRec49[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp118)))))) / fTemp121));
			float fTemp122 = (iTemp51 ? (0.100000001f - (0.0841509998f * fTemp54)) : 0.0158489998f);
			float fTemp123 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 0.0158489998f : (iTemp73 ? ((0.00193400006f * fTemp26) + 0.0158489998f) : 0.0177829992f)) : (iTemp71 ? (0.0177829992f - (0.00193400006f * fTemp74)) : 0.0158489998f)) : (iTemp69 ? ((0.0239620004f * fTemp72) + 0.0158489998f) : 0.0398110002f)) : (iTemp67 ? (0.0398110002f - (0.0298110005f * fTemp70)) : 0.00999999978f)) : (iTemp65 ? ((0.344812989f * fTemp68) + 0.00999999978f) : 0.35481301f)) : (iTemp63 ? (0.35481301f - (0.103624001f * fTemp66)) : 0.251188993f)) : (iTemp61 ? (0.251188993f - (0.171755999f * fTemp64)) : 0.0794330016f)) : (iTemp58 ? ((0.0205669999f * fTemp62) + 0.0794330016f) : 0.100000001f)) : (iTemp56 ? (0.100000001f - (0.0601890013f * fTemp59)) : 0.0398110002f)) : (iTemp55 ? ((0.0232849997f * fTemp57) + 0.0398110002f) : 0.0630960017f)) : (iTemp53 ? ((0.0369039997f * fRec46[0]) + 0.0630960017f) : 0.100000001f)) : fTemp122) : (iTemp49 ? ((0.0635839999f * fTemp52) + 0.0158489998f) : 0.0794330016f)) : (iTemp47 ? (0.0794330016f - (0.0478099994f * fTemp50)) : 0.0316229984f)) : (iTemp45 ? ((0.0683770031f * fTemp48) + 0.0316229984f) : 0.100000001f)) : (iTemp42 ? (0.100000001f - (0.0900000036f * fTemp46)) : 0.00999999978f)) : (iTemp40 ? ((0.0401189998f * fTemp43) + 0.00999999978f) : 0.0501190014f)) : fTemp113) : (iTemp36 ? (0.0794330016f - (0.0694330037f * fTemp39)) : 0.00999999978f)) : (iTemp34 ? ((0.388107002f * fTemp37) + 0.00999999978f) : 0.398106992f)) : (iTemp32 ? (0.398106992f - (0.198580995f * fTemp35)) : 0.199525997f)) : (iTemp30 ? (0.199525997f - (0.099526003f * fTemp33)) : 0.100000001f)) : (iTemp27 ? ((0.151188999f * fTemp31) + 0.100000001f) : 0.251188993f)) : (iTemp28 ? (0.251188993f - (0.0516630001f * fTemp29)) : 0.199525997f));
			float fTemp124 = (fTemp118 * fTemp121);
			float fTemp125 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? 4950.0f : (iTemp65 ? (4950.0f - (2200.0f * fTemp68)) : 2750.0f)) : (iTemp63 ? (fTemp116 + 2750.0f) : 3100.0f)) : (iTemp61 ? ((240.0f * fTemp64) + 3100.0f) : 3340.0f)) : (iTemp58 ? (3340.0f - (440.0f * fTemp62)) : 2900.0f)) : (iTemp56 ? (fTemp60 + 2900.0f) : 2950.0f)) : (iTemp55 ? ((400.0f * fTemp57) + 2950.0f) : 3350.0f)) : (iTemp53 ? (3350.0f - fTemp103) : 3300.0f)) : (iTemp51 ? ((290.0f * fTemp54) + 3300.0f) : 3590.0f)) : (iTemp49 ? (3590.0f - (290.0f * fTemp52)) : 3300.0f)) : (iTemp47 ? ((100.0f * fTemp50) + 3300.0f) : 3400.0f)) : (iTemp45 ? ((1550.0f * fTemp48) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iTemp34 ? (4950.0f - (1700.0f * fTemp37)) : 3250.0f)) : (iTemp32 ? ((330.0f * fTemp35) + 3250.0f) : 3580.0f)) : (iTemp30 ? (3580.0f - (40.0f * fTemp33)) : 3540.0f)) : (iTemp27 ? (3540.0f - (540.0f * fTemp31)) : 3000.0f)) : (iTemp28 ? ((300.0f * fTemp29) + 3000.0f) : 3300.0f));
			float fTemp126 = std::tan((fConst1 * fTemp125));
			float fTemp127 = (1.0f / fTemp126);
			float fTemp128 = ((iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? (iTemp73 ? (iTemp75 ? 140.0f : (iTemp73 ? ((60.0f * fTemp26) + 140.0f) : 200.0f)) : 200.0f) : (iTemp69 ? (200.0f - (65.0f * fTemp72)) : 135.0f)) : (iTemp67 ? ((65.0f * fTemp70) + 135.0f) : 200.0f)) : (iTemp65 ? (200.0f - (70.0f * fTemp68)) : 130.0f)) : (iTemp63 ? (130.0f - fTemp96) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp55 ? (fTemp109 + 120.0f) : 140.0f)) : (iTemp53 ? (140.0f - fTemp108) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iTemp45 ? (fTemp107 + 120.0f) : 140.0f)) : (iTemp42 ? ((60.0f * fTemp46) + 140.0f) : 200.0f)) : (iTemp40 ? (200.0f - fTemp44) : 120.0f)) : 120.0f) : (iTemp36 ? ((80.0f * fTemp39) + 120.0f) : 200.0f)) : (iTemp34 ? (200.0f - (60.0f * fTemp37)) : 140.0f)) : (iTemp32 ? (140.0f - fTemp80) : 120.0f)) : 120.0f) : (iTemp27 ? ((15.0f * fTemp31) + 120.0f) : 135.0f)) : (iTemp28 ? (135.0f - (15.0f * fTemp29)) : 120.0f)) / fTemp125);
			float fTemp129 = (((fTemp127 + fTemp128) / fTemp126) + 1.0f);
			fRec50[0] = (fTemp25 - (((fRec50[2] * (((fTemp127 - fTemp128) / fTemp126) + 1.0f)) + (2.0f * (fRec50[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp126)))))) / fTemp129));
			float fTemp130 = (iTemp27 ? (iTemp30 ? (iTemp32 ? (iTemp34 ? (iTemp36 ? (iTemp38 ? (iTemp40 ? (iTemp42 ? (iTemp45 ? (iTemp47 ? (iTemp49 ? (iTemp51 ? (iTemp53 ? (iTemp55 ? (iTemp56 ? (iTemp58 ? (iTemp61 ? (iTemp63 ? (iTemp65 ? (iTemp67 ? (iTemp69 ? (iTemp71 ? 0.00100000005f : (iTemp69 ? ((0.000777999987f * fTemp72) + 0.00100000005f) : 0.00177800003f)) : (iTemp67 ? (0.00177800003f - (0.001147f * fTemp70)) : 0.000630999974f)) : (iTemp65 ? ((0.0993689969f * fTemp68) + 0.000630999974f) : 0.100000001f)) : (iTemp63 ? ((0.0258930009f * fTemp66) + 0.100000001f) : 0.125892997f)) : (iTemp61 ? (0.125892997f - (0.0860819966f * fTemp64)) : 0.0398110002f)) : (iTemp58 ? (0.0398110002f - (0.0298110005f * fTemp62)) : 0.00999999978f)) : (iTemp56 ? ((0.00584900007f * fTemp59) + 0.00999999978f) : 0.0158489998f)) : (iTemp55 ? (0.0158489998f - (0.00325999991f * fTemp57)) : 0.0125890002f)) : (iTemp53 ? ((0.0874110013f * fRec46[0]) + 0.0125890002f) : 0.100000001f)) : fTemp122) : (iTemp49 ? ((0.00410400005f * fTemp52) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iTemp45 ? (0.0199529994f - (0.0167909991f * fTemp48)) : 0.0031620001f)) : (iTemp42 ? (0.0031620001f - (0.00157700002f * fTemp46)) : 0.00158499996f)) : (iTemp40 ? ((0.00472499989f * fTemp43) + 0.00158499996f) : 0.00631000008f)) : (iTemp38 ? (0.00631000008f - (0.00314799999f * fTemp41)) : 0.0031620001f)) : (iTemp36 ? (0.0031620001f - (0.00216199993f * fTemp39)) : 0.00100000005f)) : (iTemp34 ? ((0.0784329996f * fTemp37) + 0.00100000005f) : 0.0794330016f)) : (iTemp32 ? ((0.0205669999f * fTemp35) + 0.0794330016f) : 0.100000001f)) : (iTemp30 ? (0.100000001f - (0.0683770031f * fTemp33)) : 0.0316229984f)) : (iTemp27 ? ((0.0184959993f * fTemp31) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
			float fTemp131 = (fTemp126 * fTemp129);
			fRec36[0] = ((fRec37[0] * ((fTemp20 * (((((fRec43[0] / fTemp89) + ((fRec47[0] * fTemp99) / fTemp100)) + ((fRec48[0] * fTemp114) / fTemp115)) + ((fRec49[0] * fTemp123) / fTemp124)) + ((fRec50[0] * fTemp130) / fTemp131))) + (((((fRec43[2] * (0.0f - (fTemp20 / fTemp89))) + (fRec47[2] * (0.0f - ((fTemp20 * fTemp99) / fTemp100)))) + (fRec48[2] * (0.0f - ((fTemp20 * fTemp114) / fTemp115)))) + (fRec49[2] * (0.0f - ((fTemp20 * fTemp123) / fTemp124)))) + (fRec50[2] * (0.0f - ((fTemp20 * fTemp130) / fTemp131)))))) - (fConst88 * ((fConst89 * fRec36[2]) + (fConst90 * fRec36[1]))));
			fRec51[0] = (iTemp18 ? fSlow17 : fRec51[1]);
			float fTemp132 = (((fRec36[2] + (fRec36[0] + (2.0f * fRec36[1]))) * std::max<float>(0.0f, std::min<float>(fTemp16, ((fConst91 * fTemp17) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec51[0] + -10.0f))));
			float fTemp133 = std::fabs((fConst83 * fTemp132));
			fRec54[0] = std::max<float>(fTemp133, ((fRec54[1] * fSlow87) + (fTemp133 * fSlow88)));
			fRec53[0] = ((fRec53[1] * fSlow84) + (fRec54[0] * fSlow89));
			fRec52[0] = ((fConst77 * fRec52[1]) + (fSlow81 * std::max<float>(((20.0f * std::log10(fRec53[0])) - fSlow90), 0.0f)));
			float fTemp134 = (fConst83 * (fTemp132 * std::pow(10.0f, (0.0500000007f * fRec52[0]))));
			fRec55[0] = (fTemp134 - (fSlow94 * ((fSlow95 * fRec55[2]) + (fSlow96 * fRec55[1]))));
			float fTemp135 = (fSlow71 * fRec35[1]);
			fRec35[0] = (fTemp134 - ((fSlow94 * (fRec55[2] + (fRec55[0] + (2.0f * fRec55[1])))) + (((fRec35[2] * fSlow104) + fTemp135) / fSlow105)));
			float fTemp136 = (fSlow68 * fRec34[1]);
			fRec34[0] = ((((fTemp135 + (fRec35[0] * fSlow107)) + (fRec35[2] * fSlow108)) / fSlow105) - (((fRec34[2] * fSlow116) + fTemp136) / fSlow117));
			fRec33[0] = ((((fTemp136 + (fRec34[0] * fSlow119)) + (fRec34[2] * fSlow120)) / fSlow117) - (fSlow121 * ((fSlow122 * fRec33[2]) + (fSlow123 * fRec33[1]))));
			float fTemp137 = ((fSlow7 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + (fSlow65 * (fRec33[2] + (fRec33[0] + (2.0f * fRec33[1])))));
			fVec7[0] = fSlow135;
			float fTemp138 = (fSlow135 - fVec7[1]);
			fVec8[0] = fSlow137;
			float fTemp139 = (0.0f - (fSlow135 + fVec8[1]));
			float fTemp140 = std::min<float>(1.0f, ((fTemp138 * float((fTemp138 > 0.0f))) + (fSlow136 * (fTemp139 * float((fTemp139 > 0.0f))))));
			fVec9[0] = fTemp140;
			iRec60[0] = (((iRec60[1] + (iRec60[1] > 0)) * (fTemp140 <= fVec9[1])) + (fTemp140 > fVec9[1]));
			float fTemp141 = float(iRec60[0]);
			float fTemp142 = (fConst85 * fTemp141);
			float fTemp143 = (fConst84 - fTemp141);
			fRec59[0] = ((fConst92 * fRec59[1]) + (fConst93 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp142, ((fConst91 * fTemp143) + 1.0f))))));
			int iTemp144 = int(fTemp140);
			fRec61[0] = (iTemp144 ? fSlow17 : fRec61[1]);
			float fTemp145 = std::pow(10.0f, (0.144444451f * (fRec61[0] + -10.0f)));
			float fTemp146 = (fRec59[0] * fTemp145);
			fRec64[0] = (iTemp144 ? fSlow139 : fRec64[1]);
			iRec63[0] = ((iVec0[1] + iRec63[1]) % int((fConst0 / fRec64[0])));
			float fTemp147 = float(iRec63[0]);
			int iTemp148 = (fTemp140 > 0.0f);
			float fTemp149 = (0.144717798f * (iTemp148 ? 0.00100000005f : 1.60000002f));
			int iTemp150 = (std::fabs(fTemp149) < 1.1920929e-07f);
			float fTemp151 = (iTemp150 ? 0.0f : std::exp((0.0f - (fConst79 / (iTemp150 ? 1.0f : fTemp149)))));
			fRec66[0] = ((fRec66[1] * fTemp151) + ((iTemp148 ? fTemp140 : 0.400000006f) * (1.0f - fTemp151)));
			fRec65[0] = ((fConst97 * fRec65[1]) + (fSlow140 * fRec66[0]));
			float fTemp152 = (fRec65[0] + 0.00999999978f);
			float fTemp153 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec64[0] * fTemp152) * (1.0f - (0.000500000024f * std::min<float>(fRec64[0], 1000.0f)))))))));
			float fTemp154 = (1.0f / fTemp153);
			float fTemp155 = (((fTemp154 + 1.41421354f) / fTemp153) + 1.0f);
			fRec62[0] = ((2.0f * (((float((fTemp147 < (fConst94 / fRec64[0]))) + -0.5f) + (float((fTemp147 < (fConst95 / fRec64[0]))) + -0.5f)) + (float((fTemp147 < (fConst96 / fRec64[0]))) + -0.5f))) - (((fRec62[2] * (((fTemp154 + -1.41421354f) / fTemp153) + 1.0f)) + (2.0f * (fRec62[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp153)))))) / fTemp155));
			fRec69[0] = (iTemp144 ? fSlow141 : fRec69[1]);
			iRec68[0] = ((iVec0[1] + iRec68[1]) % int((fConst0 / fRec69[0])));
			float fTemp156 = float(iRec68[0]);
			float fTemp157 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fRec69[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec69[0], 1000.0f)))))))));
			float fTemp158 = (1.0f / fTemp157);
			float fTemp159 = (((fTemp158 + 1.41421354f) / fTemp157) + 1.0f);
			fRec67[0] = ((2.0f * (((float((fTemp156 < (fConst94 / fRec69[0]))) + -0.5f) + (float((fTemp156 < (fConst95 / fRec69[0]))) + -0.5f)) + (float((fTemp156 < (fConst96 / fRec69[0]))) + -0.5f))) - (((fRec67[2] * (((fTemp158 + -1.41421354f) / fTemp157) + 1.0f)) + (2.0f * (fRec67[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp157)))))) / fTemp159));
			fRec72[0] = (iTemp144 ? fSlow142 : fRec72[1]);
			iRec71[0] = ((iVec0[1] + iRec71[1]) % int((fConst0 / fRec72[0])));
			float fTemp160 = float(iRec71[0]);
			float fTemp161 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fRec72[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec72[0], 1000.0f)))))))));
			float fTemp162 = (1.0f / fTemp161);
			float fTemp163 = (((fTemp162 + 1.41421354f) / fTemp161) + 1.0f);
			fRec70[0] = ((2.0f * (((float((fTemp160 < (fConst94 / fRec72[0]))) + -0.5f) + (float((fTemp160 < (fConst95 / fRec72[0]))) + -0.5f)) + (float((fTemp160 < (fConst96 / fRec72[0]))) + -0.5f))) - (((fRec70[2] * (((fTemp162 + -1.41421354f) / fTemp161) + 1.0f)) + (2.0f * (fRec70[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp161)))))) / fTemp163));
			fRec75[0] = (iTemp144 ? fSlow143 : fRec75[1]);
			iRec74[0] = ((iVec0[1] + iRec74[1]) % int((fConst0 / fRec75[0])));
			float fTemp164 = float(iRec74[0]);
			float fTemp165 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fRec75[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec75[0], 1000.0f)))))))));
			float fTemp166 = (1.0f / fTemp165);
			float fTemp167 = (((fTemp166 + 1.41421354f) / fTemp165) + 1.0f);
			fRec73[0] = ((2.0f * (((float((fTemp164 < (fConst94 / fRec75[0]))) + -0.5f) + (float((fTemp164 < (fConst95 / fRec75[0]))) + -0.5f)) + (float((fTemp164 < (fConst96 / fRec75[0]))) + -0.5f))) - (((fRec73[2] * (((fTemp166 + -1.41421354f) / fTemp165) + 1.0f)) + (2.0f * (fRec73[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp165)))))) / fTemp167));
			float fTemp168 = (fTemp146 * (((((fRec62[2] + (fRec62[0] + (2.0f * fRec62[1]))) / fTemp155) + ((fRec67[2] + (fRec67[0] + (2.0f * fRec67[1]))) / fTemp159)) + ((fRec70[2] + (fRec70[0] + (2.0f * fRec70[1]))) / fTemp163)) + ((fRec73[2] + (fRec73[0] + (2.0f * fRec73[1]))) / fTemp167)));
			float fTemp169 = std::fabs((0.25f * fTemp168));
			fRec78[0] = std::max<float>(fTemp169, ((fRec78[1] * fSlow150) + (fTemp169 * fSlow151)));
			fRec77[0] = ((fRec77[1] * fSlow147) + (fRec78[0] * fSlow152));
			fRec76[0] = ((fConst77 * fRec76[1]) + (fSlow144 * std::max<float>(((20.0f * std::log10(fRec77[0])) - fSlow153), 0.0f)));
			float fTemp170 = (0.25f * (fTemp168 * std::pow(10.0f, (0.0500000007f * fRec76[0]))));
			fRec79[0] = (fTemp170 - (fSlow157 * ((fSlow158 * fRec79[2]) + (fSlow159 * fRec79[1]))));
			float fTemp171 = (fSlow134 * fRec58[1]);
			fRec58[0] = (fTemp170 - ((fSlow157 * (fRec79[2] + (fRec79[0] + (2.0f * fRec79[1])))) + (((fRec58[2] * fSlow167) + fTemp171) / fSlow168)));
			float fTemp172 = (fSlow131 * fRec57[1]);
			fRec57[0] = ((((fTemp171 + (fRec58[0] * fSlow170)) + (fRec58[2] * fSlow171)) / fSlow168) - (((fRec57[2] * fSlow179) + fTemp172) / fSlow180));
			fRec56[0] = ((((fTemp172 + (fRec57[0] * fSlow182)) + (fRec57[2] * fSlow183)) / fSlow180) - (fSlow184 * ((fSlow185 * fRec56[2]) + (fSlow186 * fRec56[1]))));
			iRec83[0] = 0;
			fRec87[0] = ((0.949999988f * fRec87[1]) + (0.0500000007f * fRec86[1]));
			float fTemp173 = ((0.99000001f * fRec87[0]) + float(iRec83[1]));
			fVec10[(IOTA & 2047)] = fTemp173;
			fVec11[0] = fSlow199;
			float fTemp174 = (fSlow199 - fVec11[1]);
			fVec12[0] = fSlow201;
			float fTemp175 = (0.0f - (fSlow199 + fVec12[1]));
			float fTemp176 = std::min<float>(1.0f, ((fTemp174 * float((fTemp174 > 0.0f))) + (fSlow200 * (fTemp175 * float((fTemp175 > 0.0f))))));
			fVec13[0] = fTemp176;
			int iTemp177 = int(fTemp176);
			fRec88[0] = (iTemp177 ? fSlow199 : fRec88[1]);
			float fTemp178 = (fRec88[0] + 100.0f);
			float fTemp179 = (fConst100 * ((340.0f / fTemp178) + -0.0399999991f));
			float fTemp180 = (fTemp179 + -1.49999499f);
			int iTemp181 = int(fTemp180);
			int iTemp182 = int(std::min<float>(fConst99, float(std::max<int>(0, iTemp181))));
			float fTemp183 = std::floor(fTemp180);
			float fTemp184 = (fTemp179 + (-1.0f - fTemp183));
			float fTemp185 = (0.0f - fTemp184);
			float fTemp186 = (fTemp179 + (-2.0f - fTemp183));
			float fTemp187 = (0.0f - (0.5f * fTemp186));
			float fTemp188 = (fTemp179 + (-3.0f - fTemp183));
			float fTemp189 = (0.0f - (0.333333343f * fTemp188));
			float fTemp190 = (fTemp179 + (-4.0f - fTemp183));
			float fTemp191 = (0.0f - (0.25f * fTemp190));
			float fTemp192 = (fTemp179 - fTemp183);
			int iTemp193 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp181 + 1)))));
			float fTemp194 = (0.0f - fTemp186);
			float fTemp195 = (0.0f - (0.5f * fTemp188));
			float fTemp196 = (0.0f - (0.333333343f * fTemp190));
			int iTemp197 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp181 + 2)))));
			float fTemp198 = (0.0f - fTemp188);
			float fTemp199 = (0.0f - (0.5f * fTemp190));
			float fTemp200 = (fTemp184 * fTemp186);
			int iTemp201 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp181 + 3)))));
			float fTemp202 = (0.0f - fTemp190);
			float fTemp203 = (fTemp200 * fTemp188);
			int iTemp204 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp181 + 4)))));
			fRec85[0] = (((((fVec10[((IOTA - iTemp182) & 2047)] * fTemp185) * fTemp187) * fTemp189) * fTemp191) + (fTemp192 * ((((((fVec10[((IOTA - iTemp193) & 2047)] * fTemp194) * fTemp195) * fTemp196) + (0.5f * (((fTemp184 * fVec10[((IOTA - iTemp197) & 2047)]) * fTemp198) * fTemp199))) + (0.166666672f * ((fTemp200 * fVec10[((IOTA - iTemp201) & 2047)]) * fTemp202))) + (0.0416666679f * (fTemp203 * fVec10[((IOTA - iTemp204) & 2047)])))));
			fRec89[0] = ((0.949999988f * fRec89[1]) + (0.0500000007f * fRec85[1]));
			fRec92[0] = (fTemp0 - (fConst106 * ((fConst109 * fRec92[2]) + (fConst110 * fRec92[1]))));
			fRec91[0] = ((fConst106 * (((fConst108 * fRec92[0]) + (fConst111 * fRec92[1])) + (fConst108 * fRec92[2]))) - (fConst103 * ((fConst112 * fRec91[2]) + (fConst113 * fRec91[1]))));
			iRec93[0] = (((iRec93[1] + (iRec93[1] > 0)) * (fTemp176 <= fVec13[1])) + (fTemp176 > fVec13[1]));
			float fTemp205 = (float(iRec93[0]) / std::max<float>(1.0f, (fConst15 * ((0.00267999992f * (fRec88[0] + -600.0f)) + 1.0f))));
			float fTemp206 = (fConst103 * ((fRec91[2] + (fRec91[0] + (2.0f * fRec91[1]))) * std::max<float>(0.0f, std::min<float>(fTemp205, (2.0f - fTemp205)))));
			fRec90[0] = (fTemp206 - ((fConst115 * (fRec90[1] * std::cos((fConst18 * fTemp178)))) + (fConst116 * fRec90[2])));
			fRec94[0] = (fTemp206 - ((fConst120 * (fRec94[1] * std::cos((fConst121 * fTemp178)))) + (fConst122 * fRec94[2])));
			fRec95[0] = (fTemp206 - ((fConst125 * (fRec95[1] * std::cos((fConst126 * fTemp178)))) + (fConst127 * fRec95[2])));
			fRec96[0] = (fTemp206 - ((fConst130 * (fRec96[1] * std::cos((fConst131 * fTemp178)))) + (fConst132 * fRec96[2])));
			fRec97[0] = (fTemp206 - ((fConst135 * (fRec97[1] * std::cos((fConst136 * fTemp178)))) + (fConst137 * fRec97[2])));
			fRec98[0] = (fTemp206 - ((fConst140 * (fRec98[1] * std::cos((fConst141 * fTemp178)))) + (fConst142 * fRec98[2])));
			fRec99[0] = (fTemp206 - ((fConst145 * (fRec99[1] * std::cos((fConst146 * fTemp178)))) + (fConst147 * fRec99[2])));
			fRec100[0] = (fTemp206 - ((fConst150 * (fRec100[1] * std::cos((fConst151 * fTemp178)))) + (fConst152 * fRec100[2])));
			fRec101[0] = (fTemp206 - ((fConst155 * (fRec101[1] * std::cos((fConst156 * fTemp178)))) + (fConst157 * fRec101[2])));
			fRec102[0] = (fTemp206 - ((fConst160 * (fRec102[1] * std::cos((fConst161 * fTemp178)))) + (fConst162 * fRec102[2])));
			fRec103[0] = (fTemp206 - ((fConst165 * (fRec103[1] * std::cos((fConst166 * fTemp178)))) + (fConst167 * fRec103[2])));
			fRec104[0] = (fTemp206 - ((fConst170 * (fRec104[1] * std::cos((fConst171 * fTemp178)))) + (fConst172 * fRec104[2])));
			fRec105[0] = (fTemp206 - ((fConst175 * (fRec105[1] * std::cos((fConst176 * fTemp178)))) + (fConst177 * fRec105[2])));
			fRec106[0] = (fTemp206 - ((fConst180 * (fRec106[1] * std::cos((fConst181 * fTemp178)))) + (fConst182 * fRec106[2])));
			fRec107[0] = (fTemp206 - ((fConst185 * (fRec107[1] * std::cos((fConst186 * fTemp178)))) + (fConst187 * fRec107[2])));
			fRec108[0] = (fTemp206 - ((fConst190 * (fRec108[1] * std::cos((fConst191 * fTemp178)))) + (fConst192 * fRec108[2])));
			fRec109[0] = (fTemp206 - ((fConst195 * (fRec109[1] * std::cos((fConst196 * fTemp178)))) + (fConst197 * fRec109[2])));
			fRec110[0] = (fTemp206 - ((fConst200 * (fRec110[1] * std::cos((fConst201 * fTemp178)))) + (fConst202 * fRec110[2])));
			fRec111[0] = (fTemp206 - ((fConst205 * (fRec111[1] * std::cos((fConst206 * fTemp178)))) + (fConst207 * fRec111[2])));
			fRec112[0] = (fTemp206 - ((fConst210 * (fRec112[1] * std::cos((fConst211 * fTemp178)))) + (fConst212 * fRec112[2])));
			fRec113[0] = (fTemp206 - ((fConst215 * (fRec113[1] * std::cos((fConst216 * fTemp178)))) + (fConst217 * fRec113[2])));
			fRec114[0] = (fTemp206 - ((fConst220 * (fRec114[1] * std::cos((fConst221 * fTemp178)))) + (fConst222 * fRec114[2])));
			fRec115[0] = (fTemp206 - ((fConst225 * (fRec115[1] * std::cos((fConst226 * fTemp178)))) + (fConst227 * fRec115[2])));
			fRec116[0] = (fTemp206 - ((fConst230 * (fRec116[1] * std::cos((fConst231 * fTemp178)))) + (fConst232 * fRec116[2])));
			fRec117[0] = (fTemp206 - ((fConst235 * (fRec117[1] * std::cos((fConst236 * fTemp178)))) + (fConst237 * fRec117[2])));
			fRec118[0] = (fTemp206 - ((fConst240 * (fRec118[1] * std::cos((fConst241 * fTemp178)))) + (fConst242 * fRec118[2])));
			fRec119[0] = (fTemp206 - ((fConst245 * (fRec119[1] * std::cos((fConst246 * fTemp178)))) + (fConst247 * fRec119[2])));
			fRec120[0] = (fTemp206 - ((fConst250 * (fRec120[1] * std::cos((fConst251 * fTemp178)))) + (fConst252 * fRec120[2])));
			fRec121[0] = (fTemp206 - ((fConst255 * (fRec121[1] * std::cos((fConst256 * fTemp178)))) + (fConst257 * fRec121[2])));
			fRec122[0] = (fTemp206 - ((fConst260 * (fRec122[1] * std::cos((fConst261 * fTemp178)))) + (fConst262 * fRec122[2])));
			fRec123[0] = (fTemp206 - ((fConst265 * (fRec123[1] * std::cos((fConst266 * fTemp178)))) + (fConst267 * fRec123[2])));
			fRec124[0] = (fTemp206 - ((fConst270 * (fRec124[1] * std::cos((fConst271 * fTemp178)))) + (fConst272 * fRec124[2])));
			fRec125[0] = (fTemp206 - ((fConst275 * (fRec125[1] * std::cos((fConst276 * fTemp178)))) + (fConst277 * fRec125[2])));
			fRec126[0] = (fTemp206 - ((fConst280 * (fRec126[1] * std::cos((fConst281 * fTemp178)))) + (fConst282 * fRec126[2])));
			fRec127[0] = (fTemp206 - ((fConst285 * (fRec127[1] * std::cos((fConst286 * fTemp178)))) + (fConst287 * fRec127[2])));
			fRec128[0] = (fTemp206 - ((fConst290 * (fRec128[1] * std::cos((fConst291 * fTemp178)))) + (fConst292 * fRec128[2])));
			fRec129[0] = (fTemp206 - ((fConst295 * (fRec129[1] * std::cos((fConst296 * fTemp178)))) + (fConst297 * fRec129[2])));
			fRec130[0] = (fTemp206 - ((fConst300 * (fRec130[1] * std::cos((fConst301 * fTemp178)))) + (fConst302 * fRec130[2])));
			fRec131[0] = (fTemp206 - ((fConst305 * (fRec131[1] * std::cos((fConst306 * fTemp178)))) + (fConst307 * fRec131[2])));
			fRec132[0] = (fTemp206 - ((fConst310 * (fRec132[1] * std::cos((fConst311 * fTemp178)))) + (fConst312 * fRec132[2])));
			fRec133[0] = (fTemp206 - ((fConst315 * (fRec133[1] * std::cos((fConst316 * fTemp178)))) + (fConst317 * fRec133[2])));
			fRec134[0] = (fTemp206 - ((fConst320 * (fRec134[1] * std::cos((fConst321 * fTemp178)))) + (fConst322 * fRec134[2])));
			fRec135[0] = (fTemp206 - ((fConst325 * (fRec135[1] * std::cos((fConst326 * fTemp178)))) + (fConst327 * fRec135[2])));
			fRec136[0] = (fTemp206 - ((fConst330 * (fRec136[1] * std::cos((fConst331 * fTemp178)))) + (fConst332 * fRec136[2])));
			fRec137[0] = (fTemp206 - ((fConst335 * (fRec137[1] * std::cos((fConst336 * fTemp178)))) + (fConst337 * fRec137[2])));
			fRec138[0] = (fTemp206 - ((fConst340 * (fRec138[1] * std::cos((fConst341 * fTemp178)))) + (fConst342 * fRec138[2])));
			fRec139[0] = (fTemp206 - ((fConst345 * (fRec139[1] * std::cos((fConst346 * fTemp178)))) + (fConst347 * fRec139[2])));
			fRec140[0] = (fTemp206 - ((fConst350 * (fRec140[1] * std::cos((fConst351 * fTemp178)))) + (fConst352 * fRec140[2])));
			fRec141[0] = (fTemp206 - ((fConst355 * (fRec141[1] * std::cos((fConst356 * fTemp178)))) + (fConst357 * fRec141[2])));
			fRec142[0] = (fTemp206 - ((fConst360 * (fRec142[1] * std::cos((fConst361 * fTemp178)))) + (fConst362 * fRec142[2])));
			fVec14[0] = (((((((((((((((((((((((((((((((((((((((((((((((((((fRec90[0] - fRec90[2]) * ((fTemp178 < fConst117) ? fConst118 : 0.0f)) + ((fRec94[0] - fRec94[2]) * (((3.31356001f * fTemp178) < fConst117) ? fConst123 : 0.0f))) + ((fRec95[0] - fRec95[2]) * (((3.83469009f * fTemp178) < fConst117) ? fConst128 : 0.0f))) + ((fRec96[0] - fRec96[2]) * (((8.06313038f * fTemp178) < fConst117) ? fConst133 : 0.0f))) + ((fRec97[0] - fRec97[2]) * (((9.44777966f * fTemp178) < fConst117) ? fConst138 : 0.0f))) + ((fRec98[0] - fRec98[2]) * (((14.1169004f * fTemp178) < fConst117) ? fConst143 : 0.0f))) + ((fRec99[0] - fRec99[2]) * (((18.3840008f * fTemp178) < fConst117) ? fConst148 : 0.0f))) + ((fRec100[0] - fRec100[2]) * (((21.0102005f * fTemp178) < fConst117) ? fConst153 : 0.0f))) + ((fRec101[0] - fRec101[2]) * (((26.1774998f * fTemp178) < fConst117) ? fConst158 : 0.0f))) + ((fRec102[0] - fRec102[2]) * (((28.9944f * fTemp178) < fConst117) ? fConst163 : 0.0f))) + ((fRec103[0] - fRec103[2]) * (((37.0727997f * fTemp178) < fConst117) ? fConst168 : 0.0f))) + ((fRec104[0] - fRec104[2]) * (((37.8703003f * fTemp178) < fConst117) ? fConst173 : 0.0f))) + ((fRec105[0] - fRec105[2]) * (((40.0634003f * fTemp178) < fConst117) ? fConst178 : 0.0f))) + ((fRec106[0] - fRec106[2]) * (((47.6439018f * fTemp178) < fConst117) ? fConst183 : 0.0f))) + ((fRec107[0] - fRec107[2]) * (((51.019001f * fTemp178) < fConst117) ? fConst188 : 0.0f))) + ((fRec108[0] - fRec108[2]) * (((52.4300003f * fTemp178) < fConst117) ? fConst193 : 0.0f))) + ((fRec109[0] - fRec109[2]) * (((58.2859993f * fTemp178) < fConst117) ? fConst198 : 0.0f))) + ((fRec110[0] - fRec110[2]) * (((63.5485992f * fTemp178) < fConst117) ? fConst203 : 0.0f))) + ((fRec111[0] - fRec111[2]) * (((65.3628006f * fTemp178) < fConst117) ? fConst208 : 0.0f))) + ((fRec112[0] - fRec112[2]) * (((66.9587021f * fTemp178) < fConst117) ? fConst213 : 0.0f))) + ((fRec113[0] - fRec113[2]) * (((74.530098f * fTemp178) < fConst117) ? fConst218 : 0.0f))) + ((fRec114[0] - fRec114[2]) * (((78.6920013f * fTemp178) < fConst117) ? fConst223 : 0.0f))) + ((fRec115[0] - fRec115[2]) * (((80.8375015f * fTemp178) < fConst117) ? fConst228 : 0.0f))) + ((fRec116[0] - fRec116[2]) * (((89.9779968f * fTemp178) < fConst117) ? fConst233 : 0.0f))) + ((fRec117[0] - fRec117[2]) * (((92.9661026f * fTemp178) < fConst117) ? fConst238 : 0.0f))) + ((fRec118[0] - fRec118[2]) * (((95.1913986f * fTemp178) < fConst117) ? fConst243 : 0.0f))) + ((fRec119[0] - fRec119[2]) * (((97.4806976f * fTemp178) < fConst117) ? fConst248 : 0.0f))) + ((fRec120[0] - fRec120[2]) * (((110.620003f * fTemp178) < fConst117) ? fConst253 : 0.0f))) + ((fRec121[0] - fRec121[2]) * (((112.069f * fTemp178) < fConst117) ? fConst258 : 0.0f))) + ((fRec122[0] - fRec122[2]) * (((113.825996f * fTemp178) < fConst117) ? fConst263 : 0.0f))) + ((fRec123[0] - fRec123[2]) * (((119.356003f * fTemp178) < fConst117) ? fConst268 : 0.0f))) + ((fRec124[0] - fRec124[2]) * (((127.044998f * fTemp178) < fConst117) ? fConst273 : 0.0f))) + ((fRec125[0] - fRec125[2]) * (((129.981995f * fTemp178) < fConst117) ? fConst278 : 0.0f))) + ((fRec126[0] - fRec126[2]) * (((132.259003f * fTemp178) < fConst117) ? fConst283 : 0.0f))) + ((fRec127[0] - fRec127[2]) * (((133.477005f * fTemp178) < fConst117) ? fConst288 : 0.0f))) + ((fRec128[0] - fRec128[2]) * (((144.548996f * fTemp178) < fConst117) ? fConst293 : 0.0f))) + ((fRec129[0] - fRec129[2]) * (((149.438004f * fTemp178) < fConst117) ? fConst298 : 0.0f))) + ((fRec130[0] - fRec130[2]) * (((152.033005f * fTemp178) < fConst117) ? fConst303 : 0.0f))) + ((fRec131[0] - fRec131[2]) * (((153.166f * fTemp178) < fConst117) ? fConst308 : 0.0f))) + ((fRec132[0] - fRec132[2]) * (((155.597f * fTemp178) < fConst117) ? fConst313 : 0.0f))) + ((fRec133[0] - fRec133[2]) * (((158.182999f * fTemp178) < fConst117) ? fConst318 : 0.0f))) + ((fRec134[0] - fRec134[2]) * (((168.104996f * fTemp178) < fConst117) ? fConst323 : 0.0f))) + ((fRec135[0] - fRec135[2]) * (((171.863007f * fTemp178) < fConst117) ? fConst328 : 0.0f))) + ((fRec136[0] - fRec136[2]) * (((174.464005f * fTemp178) < fConst117) ? fConst333 : 0.0f))) + ((fRec137[0] - fRec137[2]) * (((178.936996f * fTemp178) < fConst117) ? fConst338 : 0.0f))) + ((fRec138[0] - fRec138[2]) * (((181.481995f * fTemp178) < fConst117) ? fConst343 : 0.0f))) + ((fRec139[0] - fRec139[2]) * (((185.397995f * fTemp178) < fConst117) ? fConst348 : 0.0f))) + ((fRec140[0] - fRec140[2]) * (((190.369003f * fTemp178) < fConst117) ? fConst353 : 0.0f))) + ((fRec141[0] - fRec141[2]) * (((192.190002f * fTemp178) < fConst117) ? fConst358 : 0.0f))) + ((fRec142[0] - fRec142[2]) * (((195.505005f * fTemp178) < fConst117) ? fConst363 : 0.0f)));
			float fTemp207 = ((0.99000001f * fRec89[0]) + (0.0199999996f * fVec14[1]));
			fVec15[(IOTA & 2047)] = fTemp207;
			fRec86[0] = (((((fTemp185 * fTemp187) * fTemp189) * fTemp191) * fVec15[((IOTA - iTemp182) & 2047)]) + (fTemp192 * ((((((fTemp194 * fTemp195) * fTemp196) * fVec15[((IOTA - iTemp193) & 2047)]) + (0.5f * (((fTemp184 * fTemp198) * fTemp199) * fVec15[((IOTA - iTemp197) & 2047)]))) + (0.166666672f * ((fTemp200 * fTemp202) * fVec15[((IOTA - iTemp201) & 2047)]))) + (0.0416666679f * (fTemp203 * fVec15[((IOTA - iTemp204) & 2047)])))));
			float fRec84 = fRec86[0];
			fRec143[0] = (iTemp177 ? fSlow17 : fRec143[1]);
			float fTemp208 = (fRec84 * std::pow(10.0f, (0.144444451f * (fRec143[0] + -10.0f))));
			float fTemp209 = std::fabs((0.25f * fTemp208));
			fRec146[0] = std::max<float>(fTemp209, ((fRec146[1] * fSlow208) + (fTemp209 * fSlow209)));
			fRec145[0] = ((fRec145[1] * fSlow205) + (fRec146[0] * fSlow210));
			fRec144[0] = ((fConst77 * fRec144[1]) + (fSlow202 * std::max<float>(((20.0f * std::log10(fRec145[0])) - fSlow211), 0.0f)));
			float fTemp210 = (0.25f * (fTemp208 * std::pow(10.0f, (0.0500000007f * fRec144[0]))));
			fRec147[0] = (fTemp210 - (fSlow215 * ((fSlow216 * fRec147[2]) + (fSlow217 * fRec147[1]))));
			float fTemp211 = (fSlow198 * fRec82[1]);
			fRec82[0] = (fTemp210 - ((fSlow215 * (fRec147[2] + (fRec147[0] + (2.0f * fRec147[1])))) + (((fRec82[2] * fSlow225) + fTemp211) / fSlow226)));
			float fTemp212 = (fSlow195 * fRec81[1]);
			fRec81[0] = ((((fTemp211 + (fRec82[0] * fSlow228)) + (fRec82[2] * fSlow229)) / fSlow226) - (((fRec81[2] * fSlow237) + fTemp212) / fSlow238));
			fRec80[0] = ((((fTemp212 + (fRec81[0] * fSlow240)) + (fRec81[2] * fSlow241)) / fSlow238) - (fSlow242 * ((fSlow243 * fRec80[2]) + (fSlow244 * fRec80[1]))));
			float fTemp213 = (fRec80[2] + (fRec80[0] + (2.0f * fRec80[1])));
			fRec151[0] = ((0.999000013f * fRec151[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp142, ((fConst364 * fTemp143) + 1.0f))))));
			fRec153[0] = (iTemp144 ? fSlow256 : fRec153[1]);
			float fTemp214 = (fRec157[1] + (fSlow257 * fRec153[0]));
			fRec157[0] = (fTemp214 - std::floor(fTemp214));
			float fTemp215 = (fRec156[1] + (fConst79 * ((315.0f * (fRec151[0] * ftbl0mydspSIG0[int((65536.0f * fRec157[0]))])) + (fSlow258 * fRec153[0]))));
			fRec156[0] = (fTemp215 - std::floor(fTemp215));
			float fTemp216 = (fRec155[1] + (fConst79 * ((315.0f * (fRec151[0] * ftbl0mydspSIG0[int((65536.0f * fRec156[0]))])) + (fSlow259 * fRec153[0]))));
			fRec155[0] = (fTemp216 - std::floor(fTemp216));
			float fTemp217 = (fRec154[1] + (fConst79 * ((315.0f * (fRec151[0] * ftbl0mydspSIG0[int((65536.0f * fRec155[0]))])) + (fSlow260 * fRec153[0]))));
			fRec154[0] = (fTemp217 - std::floor(fTemp217));
			float fTemp218 = (fRec152[1] + (fConst79 * ((fSlow75 * fRec153[0]) + (315.0f * (fRec151[0] * ftbl0mydspSIG0[int((65536.0f * fRec154[0]))])))));
			fRec152[0] = (fTemp218 - std::floor(fTemp218));
			float fTemp219 = ftbl0mydspSIG0[int((65536.0f * fRec152[0]))];
			float fTemp220 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp219)));
			float fTemp221 = ((fRec151[0] * fTemp145) * (fTemp219 + (0.0500000007f * (fTemp220 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp220)))))));
			float fTemp222 = std::fabs((0.25f * fTemp221));
			fRec160[0] = std::max<float>(fTemp222, ((fRec160[1] * fSlow267) + (fTemp222 * fSlow268)));
			fRec159[0] = ((fRec159[1] * fSlow264) + (fRec160[0] * fSlow269));
			fRec158[0] = ((fConst77 * fRec158[1]) + (fSlow261 * std::max<float>(((20.0f * std::log10(fRec159[0])) - fSlow270), 0.0f)));
			float fTemp223 = (0.25f * (fTemp221 * std::pow(10.0f, (0.0500000007f * fRec158[0]))));
			fRec161[0] = (fTemp223 - (fSlow274 * ((fSlow275 * fRec161[2]) + (fSlow276 * fRec161[1]))));
			float fTemp224 = (fSlow255 * fRec150[1]);
			fRec150[0] = (fTemp223 - ((fSlow274 * (fRec161[2] + (fRec161[0] + (2.0f * fRec161[1])))) + (((fRec150[2] * fSlow284) + fTemp224) / fSlow285)));
			float fTemp225 = (fSlow252 * fRec149[1]);
			fRec149[0] = ((((fTemp224 + (fRec150[0] * fSlow287)) + (fRec150[2] * fSlow288)) / fSlow285) - (((fRec149[2] * fSlow296) + fTemp225) / fSlow297));
			fRec148[0] = ((((fTemp225 + (fRec149[0] * fSlow299)) + (fRec149[2] * fSlow300)) / fSlow297) - (fSlow301 * ((fSlow302 * fRec148[2]) + (fSlow303 * fRec148[1]))));
			float fTemp226 = (fSlow249 * (fRec148[2] + (fRec148[0] + (2.0f * fRec148[1]))));
			float fRec171 = (0.0f - (0.997305274f * ((0.899999976f * fRec172[2]) + (0.0500000007f * (fRec172[1] + fRec172[3])))));
			fVec16[0] = fSlow315;
			float fTemp227 = (fSlow315 - fVec16[1]);
			fVec17[0] = fSlow317;
			float fTemp228 = (0.0f - (fSlow315 + fVec17[1]));
			float fTemp229 = std::min<float>(1.0f, ((fTemp227 * float((fTemp227 > 0.0f))) + (fSlow316 * (fTemp228 * float((fTemp228 > 0.0f))))));
			fVec18[0] = fTemp229;
			int iTemp230 = int(fTemp229);
			fRec190[0] = (iTemp230 ? fSlow318 : fRec190[1]);
			float fTemp231 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow76 * (fRec40[0] * fRec190[0]))))) + -0.109999999f);
			float fTemp232 = (fConst366 * fTemp231);
			float fTemp233 = (fTemp232 + -1.49999499f);
			int iTemp234 = int(fTemp233);
			int iTemp235 = int(std::min<float>(fConst99, float(std::max<int>(0, iTemp234))));
			float fTemp236 = std::floor(fTemp233);
			float fTemp237 = (fTemp232 + (-1.0f - fTemp236));
			float fTemp238 = (0.0f - fTemp237);
			float fTemp239 = (fTemp232 + (-2.0f - fTemp236));
			float fTemp240 = (0.0f - (0.5f * fTemp239));
			float fTemp241 = (fTemp232 + (-3.0f - fTemp236));
			float fTemp242 = (0.0f - (0.333333343f * fTemp241));
			float fTemp243 = (fTemp232 + (-4.0f - fTemp236));
			float fTemp244 = (0.0f - (0.25f * fTemp243));
			float fTemp245 = (fTemp232 - fTemp236);
			int iTemp246 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp234 + 1)))));
			float fTemp247 = (0.0f - fTemp239);
			float fTemp248 = (0.0f - (0.5f * fTemp241));
			float fTemp249 = (0.0f - (0.333333343f * fTemp243));
			int iTemp250 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp234 + 2)))));
			float fTemp251 = (0.0f - fTemp241);
			float fTemp252 = (0.0f - (0.5f * fTemp243));
			float fTemp253 = (fTemp237 * fTemp239);
			int iTemp254 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp234 + 3)))));
			float fTemp255 = (0.0f - fTemp243);
			float fTemp256 = (fTemp253 * fTemp241);
			int iTemp257 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp234 + 4)))));
			fRec186[0] = (((((fRec167[((IOTA - (iTemp235 + 1)) & 2047)] * fTemp238) * fTemp240) * fTemp242) * fTemp244) + (fTemp245 * ((((((fRec167[((IOTA - (iTemp246 + 1)) & 2047)] * fTemp247) * fTemp248) * fTemp249) + (0.5f * (((fTemp237 * fRec167[((IOTA - (iTemp250 + 1)) & 2047)]) * fTemp251) * fTemp252))) + (0.166666672f * ((fTemp253 * fRec167[((IOTA - (iTemp254 + 1)) & 2047)]) * fTemp255))) + (0.0416666679f * (fTemp256 * fRec167[((IOTA - (iTemp257 + 1)) & 2047)])))));
			fRec191[0] = ((fSlow319 * fRec191[1]) + (fSlow320 * fRec186[1]));
			float fRec187 = fRec191[0];
			fRec193[0] = fRec165[1];
			fRec194[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec193[2]) + (0.0500000007f * (fRec193[1] + fRec193[3])))));
			float fTemp258 = (fConst367 * fTemp231);
			float fTemp259 = (fTemp258 + -1.49999499f);
			int iTemp260 = int(fTemp259);
			int iTemp261 = int(std::min<float>(fConst99, float(std::max<int>(0, iTemp260))));
			float fTemp262 = std::floor(fTemp259);
			float fTemp263 = (fTemp258 + (-1.0f - fTemp262));
			float fTemp264 = (0.0f - fTemp263);
			float fTemp265 = (fTemp258 + (-2.0f - fTemp262));
			float fTemp266 = (0.0f - (0.5f * fTemp265));
			float fTemp267 = (fTemp258 + (-3.0f - fTemp262));
			float fTemp268 = (0.0f - (0.333333343f * fTemp267));
			float fTemp269 = (fTemp258 + (-4.0f - fTemp262));
			float fTemp270 = (0.0f - (0.25f * fTemp269));
			float fTemp271 = (fTemp258 - fTemp262);
			int iTemp272 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp260 + 1)))));
			float fTemp273 = (0.0f - fTemp265);
			float fTemp274 = (0.0f - (0.5f * fTemp267));
			float fTemp275 = (0.0f - (0.333333343f * fTemp269));
			int iTemp276 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp260 + 2)))));
			float fTemp277 = (0.0f - fTemp267);
			float fTemp278 = (0.0f - (0.5f * fTemp269));
			float fTemp279 = (fTemp263 * fTemp265);
			int iTemp280 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp260 + 3)))));
			float fTemp281 = (0.0f - fTemp269);
			float fTemp282 = (fTemp279 * fTemp267);
			int iTemp283 = int(std::min<float>(fConst99, float(std::max<int>(0, (iTemp260 + 4)))));
			fVec19[0] = (((((fRec194[((IOTA - (iTemp261 + 2)) & 1023)] * fTemp264) * fTemp266) * fTemp268) * fTemp270) + (fTemp271 * ((((((fRec194[((IOTA - (iTemp272 + 2)) & 1023)] * fTemp273) * fTemp274) * fTemp275) + (0.5f * (((fTemp263 * fRec194[((IOTA - (iTemp276 + 2)) & 1023)]) * fTemp277) * fTemp278))) + (0.166666672f * ((fTemp279 * fRec194[((IOTA - (iTemp280 + 2)) & 1023)]) * fTemp281))) + (0.0416666679f * (fTemp282 * fRec194[((IOTA - (iTemp283 + 2)) & 1023)])))));
			float fTemp284 = (fTemp229 + fVec19[1]);
			fVec20[0] = fTemp284;
			fRec192[(IOTA & 2047)] = ((fSlow319 * fRec192[((IOTA - 1) & 2047)]) + (fSlow320 * fVec20[2]));
			float fRec188 = (((((fTemp238 * fTemp240) * fTemp242) * fTemp244) * fRec192[((IOTA - iTemp235) & 2047)]) + (fTemp245 * ((((((fTemp247 * fTemp248) * fTemp249) * fRec192[((IOTA - iTemp246) & 2047)]) + (0.5f * (((fTemp237 * fTemp251) * fTemp252) * fRec192[((IOTA - iTemp250) & 2047)]))) + (0.166666672f * ((fTemp253 * fTemp255) * fRec192[((IOTA - iTemp254) & 2047)]))) + (0.0416666679f * (fTemp256 * fRec192[((IOTA - iTemp257) & 2047)])))));
			float fRec189 = (fVec20[1] + fRec182[1]);
			fRec182[0] = fRec187;
			float fRec183 = fRec182[1];
			float fRec184 = fRec188;
			float fRec185 = fRec189;
			fRec178[0] = fRec183;
			float fRec179 = (fTemp229 + fRec178[1]);
			float fRec180 = fRec184;
			float fRec181 = fRec185;
			fRec174[(IOTA & 1023)] = fRec179;
			float fRec175 = (((((fTemp264 * fTemp266) * fTemp268) * fTemp270) * fRec174[((IOTA - (iTemp261 + 1)) & 1023)]) + (fTemp271 * ((((((fTemp273 * fTemp274) * fTemp275) * fRec174[((IOTA - (iTemp272 + 1)) & 1023)]) + (0.5f * (((fTemp263 * fTemp277) * fTemp278) * fRec174[((IOTA - (iTemp276 + 1)) & 1023)]))) + (0.166666672f * ((fTemp279 * fTemp281) * fRec174[((IOTA - (iTemp280 + 1)) & 1023)]))) + (0.0416666679f * (fTemp282 * fRec174[((IOTA - (iTemp283 + 1)) & 1023)])))));
			float fRec176 = fRec180;
			float fRec177 = fRec181;
			fRec172[0] = fRec176;
			float fRec173 = fRec177;
			fRec167[(IOTA & 2047)] = fRec171;
			float fRec168 = fRec175;
			float fRec169 = fRec172[0];
			float fRec170 = fRec173;
			fRec165[0] = fRec168;
			float fRec166 = fRec170;
			iRec195[0] = (((iRec195[1] + (iRec195[1] > 0)) * (fTemp229 <= fVec18[1])) + (fTemp229 > fVec18[1]));
			float fTemp285 = float(iRec195[0]);
			fRec196[0] = (iTemp230 ? fSlow17 : fRec196[1]);
			float fTemp286 = ((fRec166 * std::max<float>(0.0f, std::min<float>((fConst85 * fTemp285), ((fConst91 * (fConst84 - fTemp285)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec196[0] + -10.0f))));
			float fTemp287 = std::fabs((0.375f * fTemp286));
			fRec199[0] = std::max<float>(fTemp287, ((fRec199[1] * fSlow327) + (fTemp287 * fSlow328)));
			fRec198[0] = ((fRec198[1] * fSlow324) + (fRec199[0] * fSlow329));
			fRec197[0] = ((fConst77 * fRec197[1]) + (fSlow321 * std::max<float>(((20.0f * std::log10(fRec198[0])) - fSlow330), 0.0f)));
			float fTemp288 = (0.375f * (fTemp286 * std::pow(10.0f, (0.0500000007f * fRec197[0]))));
			fRec200[0] = (fTemp288 - (fSlow334 * ((fSlow335 * fRec200[2]) + (fSlow336 * fRec200[1]))));
			float fTemp289 = (fSlow314 * fRec164[1]);
			fRec164[0] = (fTemp288 - ((fSlow334 * (fRec200[2] + (fRec200[0] + (2.0f * fRec200[1])))) + (((fRec164[2] * fSlow344) + fTemp289) / fSlow345)));
			float fTemp290 = (fSlow311 * fRec163[1]);
			fRec163[0] = ((((fTemp289 + (fRec164[0] * fSlow347)) + (fRec164[2] * fSlow348)) / fSlow345) - (((fRec163[2] * fSlow356) + fTemp290) / fSlow357));
			fRec162[0] = ((((fTemp290 + (fRec163[0] * fSlow359)) + (fRec163[2] * fSlow360)) / fSlow357) - (fSlow361 * ((fSlow362 * fRec162[2]) + (fSlow363 * fRec162[1]))));
			float fTemp291 = (fSlow308 * (fRec162[2] + (fRec162[0] + (2.0f * fRec162[1]))));
			float fTemp292 = ((((fTemp137 + (fSlow128 * (fRec56[2] + (fRec56[0] + (2.0f * fRec56[1]))))) + (fSlow192 * fTemp213)) + fTemp226) + fTemp291);
			float fTemp293 = std::fabs(fTemp292);
			fRec203[0] = std::max<float>(fTemp293, ((fConst370 * fRec203[1]) + (fConst371 * fTemp293)));
			fRec202[0] = ((fConst368 * fRec202[1]) + (fConst369 * fRec203[0]));
			fRec201[0] = ((fConst77 * fRec201[1]) + (fConst78 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec202[0]) + 1.0f)), 0.0f)))));
			float fTemp294 = (fTemp292 * std::pow(10.0f, (0.0500000007f * fRec201[0])));
			float fTemp295 = std::fabs(fTemp294);
			fRec206[0] = std::max<float>(fTemp295, ((fConst374 * fRec206[1]) + (fConst375 * fTemp295)));
			fRec205[0] = ((fConst372 * fRec205[1]) + (fConst373 * fRec206[0]));
			fRec204[0] = ((fConst77 * fRec204[1]) + (fConst78 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec205[0])) + 5.0f), 0.0f)))));
			float fTemp296 = (fTemp294 * std::pow(10.0f, (0.0500000007f * fRec204[0])));
			float fTemp297 = std::fabs((fSlow2 * fTemp296));
			fRec209[0] = std::max<float>(fTemp297, ((fConst77 * fRec209[1]) + (fConst78 * fTemp297)));
			fRec208[0] = ((fConst376 * fRec208[1]) + (fConst377 * fRec209[0]));
			fRec207[0] = ((fConst77 * fRec207[1]) + (fConst78 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec208[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT((fSlow1 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp296 * std::pow(10.0f, (0.0500000007f * fRec207[0]))))))));
			float fTemp298 = (fRec64[0] + 0.5f);
			iRec214[0] = ((iVec0[1] + iRec214[1]) % int((fConst0 / fTemp298)));
			float fTemp299 = float(iRec214[0]);
			float fTemp300 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fTemp298) * (1.0f - (0.000500000024f * std::min<float>(fTemp298, 1000.0f)))))))));
			float fTemp301 = (1.0f / fTemp300);
			float fTemp302 = (((fTemp301 + 1.41421354f) / fTemp300) + 1.0f);
			fRec213[0] = ((2.0f * (((float((fTemp299 < (fConst94 / fTemp298))) + -0.5f) + (float((fTemp299 < (fConst95 / fTemp298))) + -0.5f)) + (float((fTemp299 < (fConst96 / fTemp298))) + -0.5f))) - (((fRec213[2] * (((fTemp301 + -1.41421354f) / fTemp300) + 1.0f)) + (2.0f * (fRec213[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp300)))))) / fTemp302));
			float fTemp303 = (fRec69[0] + 0.5f);
			iRec216[0] = ((iVec0[1] + iRec216[1]) % int((fConst0 / fTemp303)));
			float fTemp304 = float(iRec216[0]);
			float fTemp305 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fTemp303) * (1.0f - (0.000500000024f * std::min<float>(fTemp303, 1000.0f)))))))));
			float fTemp306 = (1.0f / fTemp305);
			float fTemp307 = (((fTemp306 + 1.41421354f) / fTemp305) + 1.0f);
			fRec215[0] = ((2.0f * (((float((fTemp304 < (fConst94 / fTemp303))) + -0.5f) + (float((fTemp304 < (fConst95 / fTemp303))) + -0.5f)) + (float((fTemp304 < (fConst96 / fTemp303))) + -0.5f))) - (((fRec215[2] * (((fTemp306 + -1.41421354f) / fTemp305) + 1.0f)) + (2.0f * (fRec215[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp305)))))) / fTemp307));
			float fTemp308 = (fRec72[0] + 0.5f);
			iRec218[0] = ((iVec0[1] + iRec218[1]) % int((fConst0 / fTemp308)));
			float fTemp309 = float(iRec218[0]);
			float fTemp310 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fTemp308) * (1.0f - (0.000500000024f * std::min<float>(fTemp308, 1000.0f)))))))));
			float fTemp311 = (1.0f / fTemp310);
			float fTemp312 = (((fTemp311 + 1.41421354f) / fTemp310) + 1.0f);
			fRec217[0] = ((2.0f * (((float((fTemp309 < (fConst94 / fTemp308))) + -0.5f) + (float((fTemp309 < (fConst95 / fTemp308))) + -0.5f)) + (float((fTemp309 < (fConst96 / fTemp308))) + -0.5f))) - (((fRec217[2] * (((fTemp311 + -1.41421354f) / fTemp310) + 1.0f)) + (2.0f * (fRec217[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp310)))))) / fTemp312));
			float fTemp313 = (fRec75[0] + 0.5f);
			iRec220[0] = ((iVec0[1] + iRec220[1]) % int((fConst0 / fTemp313)));
			float fTemp314 = float(iRec220[0]);
			float fTemp315 = std::tan((fConst1 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp152 * fTemp313) * (1.0f - (0.000500000024f * std::min<float>(fTemp313, 1000.0f)))))))));
			float fTemp316 = (1.0f / fTemp315);
			float fTemp317 = (((fTemp316 + 1.41421354f) / fTemp315) + 1.0f);
			fRec219[0] = ((2.0f * (((float((fTemp314 < (fConst94 / fTemp313))) + -0.5f) + (float((fTemp314 < (fConst95 / fTemp313))) + -0.5f)) + (float((fTemp314 < (fConst96 / fTemp313))) + -0.5f))) - (((fRec219[2] * (((fTemp316 + -1.41421354f) / fTemp315) + 1.0f)) + (2.0f * (fRec219[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp315)))))) / fTemp317));
			float fTemp318 = (fTemp146 * (((((fRec213[2] + (fRec213[0] + (2.0f * fRec213[1]))) / fTemp302) + ((fRec215[2] + (fRec215[0] + (2.0f * fRec215[1]))) / fTemp307)) + ((fRec217[2] + (fRec217[0] + (2.0f * fRec217[1]))) / fTemp312)) + ((fRec219[2] + (fRec219[0] + (2.0f * fRec219[1]))) / fTemp317)));
			float fTemp319 = std::fabs((0.25f * fTemp318));
			fRec223[0] = std::max<float>(fTemp319, ((fSlow150 * fRec223[1]) + (fSlow151 * fTemp319)));
			fRec222[0] = ((fSlow147 * fRec222[1]) + (fRec223[0] * fSlow152));
			fRec221[0] = ((fConst77 * fRec221[1]) + (fSlow144 * std::max<float>(((20.0f * std::log10(fRec222[0])) - fSlow153), 0.0f)));
			float fTemp320 = (0.25f * (fTemp318 * std::pow(10.0f, (0.0500000007f * fRec221[0]))));
			fRec224[0] = (fTemp320 - (fSlow157 * ((fSlow158 * fRec224[2]) + (fSlow159 * fRec224[1]))));
			float fTemp321 = (fSlow134 * fRec212[1]);
			fRec212[0] = (fTemp320 - ((fSlow157 * (fRec224[2] + (fRec224[0] + (2.0f * fRec224[1])))) + (((fSlow167 * fRec212[2]) + fTemp321) / fSlow168)));
			float fTemp322 = (fSlow131 * fRec211[1]);
			fRec211[0] = ((((fTemp321 + (fRec212[0] * fSlow170)) + (fSlow171 * fRec212[2])) / fSlow168) - (((fSlow179 * fRec211[2]) + fTemp322) / fSlow180));
			fRec210[0] = ((((fTemp322 + (fRec211[0] * fSlow182)) + (fSlow183 * fRec211[2])) / fSlow180) - (fSlow184 * ((fSlow185 * fRec210[2]) + (fSlow186 * fRec210[1]))));
			float fTemp323 = ((fSlow128 * (fRec210[2] + (fRec210[0] + (2.0f * fRec210[1])))) + (fTemp291 + (fTemp226 + (fTemp137 + (fSlow365 * fTemp213)))));
			float fTemp324 = std::fabs(fTemp323);
			fRec227[0] = std::max<float>(fTemp324, ((fConst370 * fRec227[1]) + (fConst371 * fTemp324)));
			fRec226[0] = ((fConst368 * fRec226[1]) + (fConst369 * fRec227[0]));
			fRec225[0] = ((fConst77 * fRec225[1]) + (fConst78 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec226[0]) + 1.0f)), 0.0f)))));
			float fTemp325 = (fTemp323 * std::pow(10.0f, (0.0500000007f * fRec225[0])));
			float fTemp326 = std::fabs(fTemp325);
			fRec230[0] = std::max<float>(fTemp326, ((fConst374 * fRec230[1]) + (fConst375 * fTemp326)));
			fRec229[0] = ((fConst372 * fRec229[1]) + (fConst373 * fRec230[0]));
			fRec228[0] = ((fConst77 * fRec228[1]) + (fConst78 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec229[0])) + 5.0f), 0.0f)))));
			float fTemp327 = (fTemp325 * std::pow(10.0f, (0.0500000007f * fRec228[0])));
			float fTemp328 = std::fabs((fSlow2 * fTemp327));
			fRec233[0] = std::max<float>(fTemp328, ((fConst77 * fRec233[1]) + (fConst78 * fTemp328)));
			fRec232[0] = ((fConst376 * fRec232[1]) + (fConst377 * fRec233[0]));
			fRec231[0] = ((fConst77 * fRec231[1]) + (fConst78 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec232[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT((fSlow364 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp327 * std::pow(10.0f, (0.0500000007f * fRec231[0]))))))));
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
			fRec28[1] = fRec28[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			iRec38[1] = iRec38[0];
			fRec37[1] = fRec37[0];
			fRec39[1] = fRec39[0];
			fRec42[1] = fRec42[0];
			fRec40[1] = fRec40[0];
			fRec44[1] = fRec44[0];
			fRec46[1] = fRec46[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec51[1] = fRec51[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			iRec60[1] = iRec60[0];
			fRec59[1] = fRec59[0];
			fRec61[1] = fRec61[0];
			fRec64[1] = fRec64[0];
			iRec63[1] = iRec63[0];
			fRec66[1] = fRec66[0];
			fRec65[1] = fRec65[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec69[1] = fRec69[0];
			iRec68[1] = iRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec72[1] = fRec72[0];
			iRec71[1] = iRec71[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec75[1] = fRec75[0];
			iRec74[1] = iRec74[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec78[1] = fRec78[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			iRec83[1] = iRec83[0];
			fRec87[1] = fRec87[0];
			IOTA = (IOTA + 1);
			fVec11[1] = fVec11[0];
			fVec12[1] = fVec12[0];
			fVec13[1] = fVec13[0];
			fRec88[1] = fRec88[0];
			fRec85[1] = fRec85[0];
			fRec89[1] = fRec89[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			iRec93[1] = iRec93[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
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
			fRec141[2] = fRec141[1];
			fRec141[1] = fRec141[0];
			fRec142[2] = fRec142[1];
			fRec142[1] = fRec142[0];
			fVec14[1] = fVec14[0];
			fRec86[1] = fRec86[0];
			fRec143[1] = fRec143[0];
			fRec146[1] = fRec146[0];
			fRec145[1] = fRec145[0];
			fRec144[1] = fRec144[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec151[1] = fRec151[0];
			fRec153[1] = fRec153[0];
			fRec157[1] = fRec157[0];
			fRec156[1] = fRec156[0];
			fRec155[1] = fRec155[0];
			fRec154[1] = fRec154[0];
			fRec152[1] = fRec152[0];
			fRec160[1] = fRec160[0];
			fRec159[1] = fRec159[0];
			fRec158[1] = fRec158[0];
			fRec161[2] = fRec161[1];
			fRec161[1] = fRec161[0];
			fRec150[2] = fRec150[1];
			fRec150[1] = fRec150[0];
			fRec149[2] = fRec149[1];
			fRec149[1] = fRec149[0];
			fRec148[2] = fRec148[1];
			fRec148[1] = fRec148[0];
			fVec16[1] = fVec16[0];
			fVec17[1] = fVec17[0];
			fVec18[1] = fVec18[0];
			fRec190[1] = fRec190[0];
			fRec186[1] = fRec186[0];
			fRec191[1] = fRec191[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec193[j0] = fRec193[(j0 - 1)];
			}
			fVec19[1] = fVec19[0];
			fVec20[2] = fVec20[1];
			fVec20[1] = fVec20[0];
			fRec182[1] = fRec182[0];
			fRec178[1] = fRec178[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec172[j1] = fRec172[(j1 - 1)];
			}
			fRec165[1] = fRec165[0];
			iRec195[1] = iRec195[0];
			fRec196[1] = fRec196[0];
			fRec199[1] = fRec199[0];
			fRec198[1] = fRec198[0];
			fRec197[1] = fRec197[0];
			fRec200[2] = fRec200[1];
			fRec200[1] = fRec200[0];
			fRec164[2] = fRec164[1];
			fRec164[1] = fRec164[0];
			fRec163[2] = fRec163[1];
			fRec163[1] = fRec163[0];
			fRec162[2] = fRec162[1];
			fRec162[1] = fRec162[0];
			fRec203[1] = fRec203[0];
			fRec202[1] = fRec202[0];
			fRec201[1] = fRec201[0];
			fRec206[1] = fRec206[0];
			fRec205[1] = fRec205[0];
			fRec204[1] = fRec204[0];
			fRec209[1] = fRec209[0];
			fRec208[1] = fRec208[0];
			fRec207[1] = fRec207[0];
			iRec214[1] = iRec214[0];
			fRec213[2] = fRec213[1];
			fRec213[1] = fRec213[0];
			iRec216[1] = iRec216[0];
			fRec215[2] = fRec215[1];
			fRec215[1] = fRec215[0];
			iRec218[1] = iRec218[0];
			fRec217[2] = fRec217[1];
			fRec217[1] = fRec217[0];
			iRec220[1] = iRec220[0];
			fRec219[2] = fRec219[1];
			fRec219[1] = fRec219[0];
			fRec223[1] = fRec223[0];
			fRec222[1] = fRec222[0];
			fRec221[1] = fRec221[0];
			fRec224[2] = fRec224[1];
			fRec224[1] = fRec224[0];
			fRec212[2] = fRec212[1];
			fRec212[1] = fRec212[0];
			fRec211[2] = fRec211[1];
			fRec211[1] = fRec211[0];
			fRec210[2] = fRec210[1];
			fRec210[1] = fRec210[0];
			fRec227[1] = fRec227[0];
			fRec226[1] = fRec226[0];
			fRec225[1] = fRec225[0];
			fRec230[1] = fRec230[0];
			fRec229[1] = fRec229[0];
			fRec228[1] = fRec228[0];
			fRec233[1] = fRec233[0];
			fRec232[1] = fRec232[0];
			fRec231[1] = fRec231[0];
		}
	}

};

#endif
