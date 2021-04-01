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
	
	int iRec17[2];
	
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
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec17[l4] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec17[0] = (iRec17[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec17[0] + -1))));
			iRec17[1] = iRec17[0];
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
	int fSampleRate;
	float fConst0;
	float fConst5;
	float fConst9;
	float fConst10;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst24;
	float fConst25;
	float fRec12[2];
	float fRec11[2];
	int IOTA;
	float fVec1[32768];
	int iConst27;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider1;
	float fConst28;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst32;
	float fConst33;
	float fRec18[2];
	float fConst34;
	float fConst35;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	float fVec2[2];
	float fVec3[2];
	float fVec4[2];
	FAUSTFLOAT fHslider7;
	float fRec20[2];
	FAUSTFLOAT fHslider8;
	float fConst36;
	float fRec22[2];
	float fRec21[2];
	float fRec19[2];
	float fRec24[2];
	FAUSTFLOAT fHslider9;
	float fRec23[3];
	float fRec26[3];
	float fRec27[3];
	float fRec28[3];
	float fRec29[3];
	float fConst37;
	float fConst38;
	int iRec30[2];
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec16[3];
	float fConst43;
	float fConst44;
	float fConst45;
	int iRec31[2];
	float fConst46;
	float fConst47;
	FAUSTFLOAT fHslider10;
	FAUSTFLOAT fHslider11;
	FAUSTFLOAT fHslider12;
	float fRec34[2];
	float fRec33[2];
	FAUSTFLOAT fHslider13;
	float fRec32[2];
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
	float fRec35[3];
	FAUSTFLOAT fHslider16;
	FAUSTFLOAT fHslider17;
	float fConst48;
	float fRec15[3];
	FAUSTFLOAT fHslider18;
	FAUSTFLOAT fHslider19;
	float fRec14[3];
	float fRec13[3];
	FAUSTFLOAT fHslider20;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider21;
	FAUSTFLOAT fHslider22;
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fHslider24;
	float fConst49;
	float fConst50;
	FAUSTFLOAT fHslider25;
	float fVec5[2];
	float fVec6[2];
	float fVec7[2];
	FAUSTFLOAT fHslider26;
	float fRec65[2];
	float fRec61[2];
	FAUSTFLOAT fHslider27;
	float fRec66[2];
	float fRec68[4];
	float fRec69[1024];
	float fConst51;
	float fVec8[2];
	float fVec9[3];
	float fRec67[2048];
	float fRec57[2];
	float fRec53[2];
	float fRec49[1024];
	float fRec47[4];
	float fRec42[2048];
	float fRec40[2];
	FAUSTFLOAT fHslider28;
	float fRec70[2];
	float fRec39[3];
	int iRec71[2];
	int iRec72[2];
	FAUSTFLOAT fHslider29;
	FAUSTFLOAT fHslider30;
	FAUSTFLOAT fHslider31;
	float fRec75[2];
	float fRec74[2];
	FAUSTFLOAT fHslider32;
	float fRec73[2];
	FAUSTFLOAT fHslider33;
	FAUSTFLOAT fHslider34;
	float fRec76[3];
	FAUSTFLOAT fHslider35;
	FAUSTFLOAT fHslider36;
	float fRec38[3];
	FAUSTFLOAT fHslider37;
	FAUSTFLOAT fHslider38;
	float fRec37[3];
	float fRec36[3];
	float fVec10[2048];
	int iConst52;
	float fVec11[2048];
	int iConst53;
	float fRec9[2];
	float fConst62;
	float fConst63;
	float fConst64;
	float fRec80[2];
	float fRec79[2];
	float fVec12[32768];
	int iConst66;
	float fVec13[4096];
	int iConst67;
	float fRec77[2];
	float fConst76;
	float fConst77;
	float fConst78;
	float fRec84[2];
	float fRec83[2];
	float fVec14[16384];
	int iConst80;
	float fVec15[4096];
	int iConst81;
	float fRec81[2];
	float fConst90;
	float fConst91;
	float fConst92;
	float fRec88[2];
	float fRec87[2];
	float fVec16[32768];
	int iConst94;
	float fVec17[4096];
	int iConst95;
	float fRec85[2];
	float fConst104;
	float fConst105;
	float fConst106;
	float fRec92[2];
	float fRec91[2];
	float fVec18[16384];
	int iConst108;
	float fVec19[2048];
	int iConst109;
	float fRec89[2];
	float fConst118;
	float fConst119;
	float fConst120;
	float fRec96[2];
	float fRec95[2];
	float fVec20[16384];
	int iConst122;
	float fVec21[4096];
	int iConst123;
	float fRec93[2];
	float fConst132;
	float fConst133;
	float fConst134;
	float fRec100[2];
	float fRec99[2];
	float fVec22[16384];
	int iConst136;
	float fVec23[4096];
	int iConst137;
	float fRec97[2];
	float fConst146;
	float fConst147;
	float fConst148;
	float fRec104[2];
	float fRec103[2];
	float fVec24[16384];
	int iConst150;
	float fVec25[2048];
	int iConst151;
	float fRec101[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fConst152;
	float fConst153;
	float fConst154;
	float fRec0[3];
	FAUSTFLOAT fHslider39;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fHslider42;
	FAUSTFLOAT fHslider43;
	FAUSTFLOAT fHslider44;
	int iRec111[2];
	float fRec110[3];
	float fRec109[3];
	float fConst155;
	FAUSTFLOAT fHslider45;
	float fVec26[2];
	float fVec27[2];
	float fVec28[2];
	int iRec112[2];
	float fConst157;
	float fRec113[2];
	float fConst158;
	float fRec108[3];
	float fConst160;
	float fConst161;
	float fRec114[3];
	float fConst163;
	float fConst164;
	float fRec115[3];
	float fConst166;
	float fConst167;
	float fRec116[3];
	float fConst169;
	float fConst170;
	float fRec117[3];
	float fConst172;
	float fConst173;
	float fRec118[3];
	float fConst175;
	float fConst176;
	float fRec119[3];
	float fConst178;
	float fConst179;
	float fRec120[3];
	float fConst181;
	float fConst182;
	float fRec121[3];
	float fConst184;
	float fConst185;
	float fRec122[3];
	float fConst187;
	float fConst188;
	float fRec123[3];
	float fConst190;
	float fConst191;
	float fRec124[3];
	float fConst193;
	float fConst194;
	float fRec125[3];
	float fConst196;
	float fConst197;
	float fRec126[3];
	float fConst199;
	float fConst200;
	float fRec127[3];
	float fConst202;
	float fConst203;
	float fRec128[3];
	float fConst205;
	float fConst206;
	float fRec129[3];
	float fConst208;
	float fConst209;
	float fRec130[3];
	float fConst211;
	float fConst212;
	float fRec131[3];
	float fConst214;
	float fConst215;
	float fRec132[3];
	FAUSTFLOAT fHslider46;
	FAUSTFLOAT fHslider47;
	FAUSTFLOAT fHslider48;
	float fRec135[2];
	float fRec134[2];
	FAUSTFLOAT fHslider49;
	float fRec133[2];
	FAUSTFLOAT fHslider50;
	FAUSTFLOAT fHslider51;
	float fRec136[3];
	FAUSTFLOAT fHslider52;
	FAUSTFLOAT fHslider53;
	float fRec107[3];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec106[3];
	float fRec105[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider57;
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fHslider59;
	FAUSTFLOAT fHslider60;
	float fConst216;
	float fConst217;
	FAUSTFLOAT fHslider61;
	float fVec29[2];
	float fVec30[2];
	float fVec31[2];
	int iRec141[2];
	float fRec140[2];
	FAUSTFLOAT fHslider62;
	float fRec144[2];
	int iRec143[2];
	float fConst218;
	float fConst219;
	float fConst220;
	float fConst221;
	float fConst222;
	float fRec146[2];
	float fRec145[2];
	float fRec142[3];
	FAUSTFLOAT fHslider63;
	float fRec149[2];
	int iRec148[2];
	float fRec147[3];
	FAUSTFLOAT fHslider64;
	float fRec152[2];
	int iRec151[2];
	float fRec150[3];
	FAUSTFLOAT fHslider65;
	float fRec155[2];
	int iRec154[2];
	float fRec153[3];
	FAUSTFLOAT fHslider66;
	FAUSTFLOAT fHslider67;
	FAUSTFLOAT fHslider68;
	float fRec158[2];
	float fRec157[2];
	FAUSTFLOAT fHslider69;
	float fRec156[2];
	FAUSTFLOAT fHslider70;
	FAUSTFLOAT fHslider71;
	float fRec159[3];
	FAUSTFLOAT fHslider72;
	FAUSTFLOAT fHslider73;
	float fRec139[3];
	FAUSTFLOAT fHslider74;
	FAUSTFLOAT fHslider75;
	float fRec138[3];
	float fRec137[3];
	FAUSTFLOAT fHslider76;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fHslider78;
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	int iRec163[2];
	float fRec167[2];
	float fVec32[2048];
	float fConst223;
	FAUSTFLOAT fHslider81;
	float fVec33[2];
	float fVec34[2];
	float fVec35[2];
	float fRec168[2];
	float fRec165[2];
	float fRec169[2];
	float fConst226;
	float fConst229;
	float fConst231;
	float fConst232;
	float fConst233;
	float fRec172[3];
	float fConst234;
	float fConst235;
	float fConst236;
	float fRec171[3];
	float fConst237;
	int iRec173[2];
	float fConst239;
	float fConst240;
	float fRec170[3];
	float fConst241;
	float fConst242;
	float fConst244;
	float fConst245;
	float fConst246;
	float fRec174[3];
	float fConst247;
	float fConst249;
	float fConst250;
	float fConst251;
	float fRec175[3];
	float fConst252;
	float fConst254;
	float fConst255;
	float fConst256;
	float fRec176[3];
	float fConst257;
	float fConst259;
	float fConst260;
	float fConst261;
	float fRec177[3];
	float fConst262;
	float fConst264;
	float fConst265;
	float fConst266;
	float fRec178[3];
	float fConst267;
	float fConst269;
	float fConst270;
	float fConst271;
	float fRec179[3];
	float fConst272;
	float fConst274;
	float fConst275;
	float fConst276;
	float fRec180[3];
	float fConst277;
	float fConst279;
	float fConst280;
	float fConst281;
	float fRec181[3];
	float fConst282;
	float fConst284;
	float fConst285;
	float fConst286;
	float fRec182[3];
	float fConst287;
	float fConst289;
	float fConst290;
	float fConst291;
	float fRec183[3];
	float fConst292;
	float fConst294;
	float fConst295;
	float fConst296;
	float fRec184[3];
	float fConst297;
	float fConst299;
	float fConst300;
	float fConst301;
	float fRec185[3];
	float fConst302;
	float fConst304;
	float fConst305;
	float fConst306;
	float fRec186[3];
	float fConst307;
	float fConst309;
	float fConst310;
	float fConst311;
	float fRec187[3];
	float fConst312;
	float fConst314;
	float fConst315;
	float fConst316;
	float fRec188[3];
	float fConst317;
	float fConst319;
	float fConst320;
	float fConst321;
	float fRec189[3];
	float fConst322;
	float fConst324;
	float fConst325;
	float fConst326;
	float fRec190[3];
	float fConst327;
	float fConst329;
	float fConst330;
	float fConst331;
	float fRec191[3];
	float fConst332;
	float fConst334;
	float fConst335;
	float fConst336;
	float fRec192[3];
	float fConst337;
	float fConst339;
	float fConst340;
	float fConst341;
	float fRec193[3];
	float fConst342;
	float fConst344;
	float fConst345;
	float fConst346;
	float fRec194[3];
	float fConst347;
	float fConst349;
	float fConst350;
	float fConst351;
	float fRec195[3];
	float fConst352;
	float fConst354;
	float fConst355;
	float fConst356;
	float fRec196[3];
	float fConst357;
	float fConst359;
	float fConst360;
	float fConst361;
	float fRec197[3];
	float fConst362;
	float fConst364;
	float fConst365;
	float fConst366;
	float fRec198[3];
	float fConst367;
	float fConst369;
	float fConst370;
	float fConst371;
	float fRec199[3];
	float fConst372;
	float fConst374;
	float fConst375;
	float fConst376;
	float fRec200[3];
	float fConst377;
	float fConst379;
	float fConst380;
	float fConst381;
	float fRec201[3];
	float fConst382;
	float fConst384;
	float fConst385;
	float fConst386;
	float fRec202[3];
	float fConst387;
	float fConst389;
	float fConst390;
	float fConst391;
	float fRec203[3];
	float fConst392;
	float fConst394;
	float fConst395;
	float fConst396;
	float fRec204[3];
	float fConst397;
	float fConst399;
	float fConst400;
	float fConst401;
	float fRec205[3];
	float fConst402;
	float fConst404;
	float fConst405;
	float fConst406;
	float fRec206[3];
	float fConst407;
	float fConst409;
	float fConst410;
	float fConst411;
	float fRec207[3];
	float fConst412;
	float fConst414;
	float fConst415;
	float fConst416;
	float fRec208[3];
	float fConst417;
	float fConst419;
	float fConst420;
	float fConst421;
	float fRec209[3];
	float fConst422;
	float fConst424;
	float fConst425;
	float fConst426;
	float fRec210[3];
	float fConst427;
	float fConst429;
	float fConst430;
	float fConst431;
	float fRec211[3];
	float fConst432;
	float fConst434;
	float fConst435;
	float fConst436;
	float fRec212[3];
	float fConst437;
	float fConst439;
	float fConst440;
	float fConst441;
	float fRec213[3];
	float fConst442;
	float fConst444;
	float fConst445;
	float fConst446;
	float fRec214[3];
	float fConst447;
	float fConst449;
	float fConst450;
	float fConst451;
	float fRec215[3];
	float fConst452;
	float fConst454;
	float fConst455;
	float fConst456;
	float fRec216[3];
	float fConst457;
	float fConst459;
	float fConst460;
	float fConst461;
	float fRec217[3];
	float fConst462;
	float fConst464;
	float fConst465;
	float fConst466;
	float fRec218[3];
	float fConst467;
	float fConst469;
	float fConst470;
	float fConst471;
	float fRec219[3];
	float fConst472;
	float fConst474;
	float fConst475;
	float fConst476;
	float fRec220[3];
	float fConst477;
	float fConst479;
	float fConst480;
	float fConst481;
	float fRec221[3];
	float fConst482;
	float fConst484;
	float fConst485;
	float fConst486;
	float fRec222[3];
	float fConst487;
	float fVec36[2];
	float fVec37[2048];
	float fRec166[2];
	FAUSTFLOAT fHslider82;
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec225[2];
	float fRec224[2];
	FAUSTFLOAT fHslider85;
	float fRec223[2];
	FAUSTFLOAT fHslider86;
	FAUSTFLOAT fHslider87;
	float fRec226[3];
	FAUSTFLOAT fHslider88;
	FAUSTFLOAT fHslider89;
	float fRec162[3];
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	float fRec161[3];
	float fRec160[3];
	FAUSTFLOAT fHslider92;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider93;
	FAUSTFLOAT fHslider94;
	FAUSTFLOAT fHslider95;
	FAUSTFLOAT fHslider96;
	float fConst488;
	float fRec230[2];
	float fRec232[2];
	float fConst489;
	float fRec236[2];
	float fRec235[2];
	float fRec234[2];
	float fRec233[2];
	float fRec231[2];
	FAUSTFLOAT fHslider97;
	FAUSTFLOAT fHslider98;
	FAUSTFLOAT fHslider99;
	float fRec239[2];
	float fRec238[2];
	FAUSTFLOAT fHslider100;
	float fRec237[2];
	FAUSTFLOAT fHslider101;
	FAUSTFLOAT fHslider102;
	float fRec240[3];
	FAUSTFLOAT fHslider103;
	FAUSTFLOAT fHslider104;
	float fRec229[3];
	FAUSTFLOAT fHslider105;
	FAUSTFLOAT fHslider106;
	float fRec228[3];
	float fRec227[3];
	FAUSTFLOAT fHslider107;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider108;
	FAUSTFLOAT fHslider109;
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fHslider111;
	float fConst490;
	float fConst493;
	float fConst494;
	float fConst495;
	float fRec245[3];
	float fConst496;
	FAUSTFLOAT fHslider112;
	float fVec38[2];
	float fVec39[2];
	float fVec40[2];
	int iRec246[2];
	float fConst498;
	float fConst499;
	float fRec244[3];
	float fConst500;
	float fConst502;
	float fConst503;
	float fRec247[3];
	float fConst504;
	float fConst506;
	float fConst507;
	float fRec248[3];
	float fConst508;
	float fConst510;
	float fConst511;
	float fRec249[3];
	float fConst512;
	float fConst514;
	float fConst515;
	float fRec250[3];
	float fConst516;
	float fConst518;
	float fConst519;
	float fRec251[3];
	float fConst520;
	float fConst522;
	float fConst523;
	float fRec252[3];
	float fConst524;
	float fConst526;
	float fConst527;
	float fRec253[3];
	float fConst528;
	float fConst530;
	float fConst531;
	float fRec254[3];
	float fConst532;
	float fConst534;
	float fConst535;
	float fRec255[3];
	float fConst536;
	float fConst538;
	float fConst539;
	float fRec256[3];
	float fConst540;
	float fConst542;
	float fConst543;
	float fRec257[3];
	float fConst544;
	float fConst546;
	float fConst547;
	float fRec258[3];
	float fConst548;
	float fConst550;
	float fConst551;
	float fRec259[3];
	float fConst552;
	float fConst554;
	float fConst555;
	float fRec260[3];
	float fConst556;
	float fConst558;
	float fConst559;
	float fRec261[3];
	float fConst560;
	float fConst562;
	float fConst563;
	float fRec262[3];
	float fConst564;
	float fConst566;
	float fConst567;
	float fRec263[3];
	float fConst568;
	float fConst570;
	float fConst571;
	float fRec264[3];
	float fConst572;
	float fConst574;
	float fConst575;
	float fRec265[3];
	float fConst576;
	float fConst578;
	float fConst579;
	float fRec266[3];
	float fConst580;
	float fConst582;
	float fConst583;
	float fRec267[3];
	float fConst584;
	float fConst586;
	float fConst587;
	float fRec268[3];
	float fConst588;
	float fConst590;
	float fConst591;
	float fRec269[3];
	float fConst592;
	float fConst594;
	float fConst595;
	float fRec270[3];
	float fConst596;
	float fConst598;
	float fConst599;
	float fRec271[3];
	float fConst600;
	float fConst602;
	float fConst603;
	float fRec272[3];
	float fConst604;
	float fConst606;
	float fConst607;
	float fRec273[3];
	float fConst608;
	float fConst610;
	float fConst611;
	float fRec274[3];
	float fConst612;
	float fConst614;
	float fConst615;
	float fRec275[3];
	float fConst616;
	float fConst618;
	float fConst619;
	float fRec276[3];
	float fConst620;
	float fConst622;
	float fConst623;
	float fRec277[3];
	float fConst624;
	float fConst626;
	float fConst627;
	float fRec278[3];
	float fConst628;
	float fConst630;
	float fConst631;
	float fRec279[3];
	float fConst632;
	float fConst634;
	float fConst635;
	float fRec280[3];
	float fConst636;
	float fConst638;
	float fConst639;
	float fRec281[3];
	float fConst640;
	float fConst642;
	float fConst643;
	float fRec282[3];
	float fConst644;
	float fConst646;
	float fConst647;
	float fRec283[3];
	float fConst648;
	float fConst650;
	float fConst651;
	float fRec284[3];
	float fConst652;
	float fConst654;
	float fConst655;
	float fRec285[3];
	float fConst656;
	float fConst658;
	float fConst659;
	float fRec286[3];
	float fConst660;
	float fConst662;
	float fConst663;
	float fRec287[3];
	float fConst664;
	float fConst666;
	float fConst667;
	float fRec288[3];
	float fConst668;
	float fConst670;
	float fConst671;
	float fRec289[3];
	float fConst672;
	float fConst674;
	float fConst675;
	float fRec290[3];
	float fConst676;
	float fConst678;
	float fConst679;
	float fRec291[3];
	float fConst680;
	float fConst682;
	float fConst683;
	float fRec292[3];
	float fConst684;
	float fConst686;
	float fConst687;
	float fRec293[3];
	float fConst688;
	float fConst690;
	float fConst691;
	float fRec294[3];
	float fConst692;
	float fConst694;
	float fConst695;
	float fRec295[3];
	float fConst696;
	FAUSTFLOAT fHslider113;
	float fRec296[2];
	float fRec297[2];
	FAUSTFLOAT fHslider114;
	float fRec298[2];
	float fRec299[2];
	FAUSTFLOAT fHslider115;
	FAUSTFLOAT fHslider116;
	FAUSTFLOAT fHslider117;
	float fRec302[2];
	float fRec301[2];
	FAUSTFLOAT fHslider118;
	float fRec300[2];
	FAUSTFLOAT fHslider119;
	FAUSTFLOAT fHslider120;
	float fRec303[3];
	FAUSTFLOAT fHslider121;
	FAUSTFLOAT fHslider122;
	float fRec243[3];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	float fRec242[3];
	float fRec241[3];
	float fConst697;
	float fConst698;
	float fConst699;
	float fConst700;
	float fRec306[2];
	float fRec305[2];
	float fRec304[2];
	float fConst701;
	float fConst702;
	float fConst703;
	float fConst704;
	float fRec309[2];
	float fRec308[2];
	float fRec307[2];
	float fConst705;
	float fConst706;
	float fRec312[2];
	float fRec311[2];
	float fRec310[2];
	float fRec313[3];
	int iRec318[2];
	float fRec317[3];
	int iRec320[2];
	float fRec319[3];
	int iRec322[2];
	float fRec321[3];
	int iRec324[2];
	float fRec323[3];
	float fRec327[2];
	float fRec326[2];
	float fRec325[2];
	float fRec328[3];
	float fRec316[3];
	float fRec315[3];
	float fRec314[3];
	float fRec331[2];
	float fRec330[2];
	float fRec329[2];
	float fRec334[2];
	float fRec333[2];
	float fRec332[2];
	float fRec337[2];
	float fRec336[2];
	float fRec335[2];
	
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
		float fConst1 = std::tan((10995.5742f / fConst0));
		float fConst2 = (fConst0 * fConst1);
		float fConst3 = mydsp_faustpower2_f(std::sqrt((4.0f * ((mydsp_faustpower2_f(fConst0) * fConst1) * std::tan((628.318542f / fConst0))))));
		float fConst4 = ((2.0f * fConst2) - (0.5f * (fConst3 / fConst2)));
		fConst5 = (1.0f / fConst0);
		float fConst6 = (mydsp_faustpower2_f(fConst5) * fConst3);
		float fConst7 = (2.0f * (fConst4 / fConst0));
		float fConst8 = ((fConst6 + fConst7) + 4.0f);
		fConst9 = (2.0f * (fConst4 / (fConst0 * fConst8)));
		fConst10 = (0.0f - fConst9);
		float fConst11 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst12 = ((0.0f - (6.90775537f * fConst11)) / fConst0);
		float fConst13 = std::exp((0.25f * fConst12));
		float fConst14 = mydsp_faustpower2_f(fConst13);
		float fConst15 = std::cos((12566.3711f / fConst0));
		float fConst16 = (1.0f - (fConst14 * fConst15));
		float fConst17 = (1.0f - fConst14);
		float fConst18 = (fConst16 / fConst17);
		float fConst19 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst16) / mydsp_faustpower2_f(fConst17)) + -1.0f)));
		fConst20 = (fConst18 - fConst19);
		fConst21 = (fConst13 * (fConst19 + (1.0f - fConst18)));
		fConst22 = ((std::exp(fConst12) / fConst13) + -1.0f);
		float fConst23 = (1.0f / std::tan((314.159271f / fConst0)));
		fConst24 = (1.0f / (fConst23 + 1.0f));
		fConst25 = (1.0f - fConst23);
		float fConst26 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst27 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst11 - fConst26))));
		fConst28 = (3.14159274f / fConst0);
		float fConst29 = std::tan((25132.7422f / fConst0));
		float fConst30 = (1.0f / fConst29);
		float fConst31 = (((fConst30 + 0.333333343f) / fConst29) + 1.0f);
		fConst32 = (0.25f / fConst31);
		fConst33 = (6.0f / fConst0);
		fConst34 = std::exp((0.0f - (50.0f / fConst0)));
		fConst35 = (1.0f - fConst34);
		fConst36 = (8.0f / fConst0);
		fConst37 = std::max<float>(1.0f, (0.0199999996f * fConst0));
		fConst38 = (1.0f / fConst37);
		fConst39 = (1.0f / std::max<float>(1.0f, (0.800000012f * fConst0)));
		fConst40 = (1.0f / fConst31);
		fConst41 = (((fConst30 + -0.333333343f) / fConst29) + 1.0f);
		fConst42 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst29))));
		fConst43 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst44 = (1.0f / fConst43);
		fConst45 = (1.0f / std::max<float>(1.0f, (4.0f * fConst0)));
		fConst46 = std::exp((0.0f - (20.0f / fConst0)));
		fConst47 = (1.0f - fConst46);
		fConst48 = (6.28318548f / fConst0);
		fConst49 = (0.00882352982f * fConst0);
		fConst50 = (0.00117647054f * fConst0);
		fConst51 = (0.000294117635f * fConst0);
		iConst52 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst53 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		float fConst54 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst55 = ((0.0f - (6.90775537f * fConst54)) / fConst0);
		float fConst56 = std::exp((0.25f * fConst55));
		float fConst57 = mydsp_faustpower2_f(fConst56);
		float fConst58 = (1.0f - (fConst57 * fConst15));
		float fConst59 = (1.0f - fConst57);
		float fConst60 = (fConst58 / fConst59);
		float fConst61 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst58) / mydsp_faustpower2_f(fConst59)) + -1.0f)));
		fConst62 = (fConst60 - fConst61);
		fConst63 = (fConst56 * (fConst61 + (1.0f - fConst60)));
		fConst64 = ((std::exp(fConst55) / fConst56) + -1.0f);
		float fConst65 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst66 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst54 - fConst65))));
		iConst67 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst65 + -1.0f))));
		float fConst68 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst69 = ((0.0f - (6.90775537f * fConst68)) / fConst0);
		float fConst70 = std::exp((0.25f * fConst69));
		float fConst71 = mydsp_faustpower2_f(fConst70);
		float fConst72 = (1.0f - (fConst71 * fConst15));
		float fConst73 = (1.0f - fConst71);
		float fConst74 = (fConst72 / fConst73);
		float fConst75 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst72) / mydsp_faustpower2_f(fConst73)) + -1.0f)));
		fConst76 = (fConst74 - fConst75);
		fConst77 = (fConst70 * (fConst75 + (1.0f - fConst74)));
		fConst78 = ((std::exp(fConst69) / fConst70) + -1.0f);
		float fConst79 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst80 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst68 - fConst79))));
		iConst81 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst79 + -1.0f))));
		float fConst82 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst83 = ((0.0f - (6.90775537f * fConst82)) / fConst0);
		float fConst84 = std::exp((0.25f * fConst83));
		float fConst85 = mydsp_faustpower2_f(fConst84);
		float fConst86 = (1.0f - (fConst85 * fConst15));
		float fConst87 = (1.0f - fConst85);
		float fConst88 = (fConst86 / fConst87);
		float fConst89 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst86) / mydsp_faustpower2_f(fConst87)) + -1.0f)));
		fConst90 = (fConst88 - fConst89);
		fConst91 = (fConst84 * (fConst89 + (1.0f - fConst88)));
		fConst92 = ((std::exp(fConst83) / fConst84) + -1.0f);
		float fConst93 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst94 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst82 - fConst93))));
		iConst95 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst93 + -1.0f))));
		float fConst96 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst97 = ((0.0f - (6.90775537f * fConst96)) / fConst0);
		float fConst98 = std::exp((0.25f * fConst97));
		float fConst99 = mydsp_faustpower2_f(fConst98);
		float fConst100 = (1.0f - (fConst99 * fConst15));
		float fConst101 = (1.0f - fConst99);
		float fConst102 = (fConst100 / fConst101);
		float fConst103 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst100) / mydsp_faustpower2_f(fConst101)) + -1.0f)));
		fConst104 = (fConst102 - fConst103);
		fConst105 = (fConst98 * (fConst103 + (1.0f - fConst102)));
		fConst106 = ((std::exp(fConst97) / fConst98) + -1.0f);
		float fConst107 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst108 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst96 - fConst107))));
		iConst109 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst107 + -1.0f))));
		float fConst110 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst111 = ((0.0f - (6.90775537f * fConst110)) / fConst0);
		float fConst112 = std::exp((0.25f * fConst111));
		float fConst113 = mydsp_faustpower2_f(fConst112);
		float fConst114 = (1.0f - (fConst113 * fConst15));
		float fConst115 = (1.0f - fConst113);
		float fConst116 = (fConst114 / fConst115);
		float fConst117 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst114) / mydsp_faustpower2_f(fConst115)) + -1.0f)));
		fConst118 = (fConst116 - fConst117);
		fConst119 = (fConst112 * (fConst117 + (1.0f - fConst116)));
		fConst120 = ((std::exp(fConst111) / fConst112) + -1.0f);
		float fConst121 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst122 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst110 - fConst121))));
		iConst123 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst121 + -1.0f))));
		float fConst124 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst125 = ((0.0f - (6.90775537f * fConst124)) / fConst0);
		float fConst126 = std::exp((0.25f * fConst125));
		float fConst127 = mydsp_faustpower2_f(fConst126);
		float fConst128 = (1.0f - (fConst127 * fConst15));
		float fConst129 = (1.0f - fConst127);
		float fConst130 = (fConst128 / fConst129);
		float fConst131 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst128) / mydsp_faustpower2_f(fConst129)) + -1.0f)));
		fConst132 = (fConst130 - fConst131);
		fConst133 = (fConst126 * (fConst131 + (1.0f - fConst130)));
		fConst134 = ((std::exp(fConst125) / fConst126) + -1.0f);
		float fConst135 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst136 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst124 - fConst135))));
		iConst137 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst135 + -1.0f))));
		float fConst138 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst139 = ((0.0f - (6.90775537f * fConst138)) / fConst0);
		float fConst140 = std::exp((0.25f * fConst139));
		float fConst141 = mydsp_faustpower2_f(fConst140);
		float fConst142 = (1.0f - (fConst141 * fConst15));
		float fConst143 = (1.0f - fConst141);
		float fConst144 = (fConst142 / fConst143);
		float fConst145 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst142) / mydsp_faustpower2_f(fConst143)) + -1.0f)));
		fConst146 = (fConst144 - fConst145);
		fConst147 = (fConst140 * (fConst145 + (1.0f - fConst144)));
		fConst148 = ((std::exp(fConst139) / fConst140) + -1.0f);
		float fConst149 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst150 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst138 - fConst149))));
		iConst151 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst149 + -1.0f))));
		fConst152 = (1.0f / fConst8);
		fConst153 = ((2.0f * fConst6) + -8.0f);
		fConst154 = (fConst6 + (4.0f - fConst7));
		fConst155 = (0.00200000009f * fConst0);
		float fConst156 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst157 = (0.0f - (2.0f * fConst156));
		fConst158 = mydsp_faustpower2_f(fConst156);
		float fConst159 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst160 = (0.0f - (2.0f * fConst159));
		fConst161 = mydsp_faustpower2_f(fConst159);
		float fConst162 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst163 = (0.0f - (2.0f * fConst162));
		fConst164 = mydsp_faustpower2_f(fConst162);
		float fConst165 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst166 = (0.0f - (2.0f * fConst165));
		fConst167 = mydsp_faustpower2_f(fConst165);
		float fConst168 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst169 = (0.0f - (2.0f * fConst168));
		fConst170 = mydsp_faustpower2_f(fConst168);
		float fConst171 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst172 = (0.0f - (2.0f * fConst171));
		fConst173 = mydsp_faustpower2_f(fConst171);
		float fConst174 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst175 = (0.0f - (2.0f * fConst174));
		fConst176 = mydsp_faustpower2_f(fConst174);
		float fConst177 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst178 = (0.0f - (2.0f * fConst177));
		fConst179 = mydsp_faustpower2_f(fConst177);
		float fConst180 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst181 = (0.0f - (2.0f * fConst180));
		fConst182 = mydsp_faustpower2_f(fConst180);
		float fConst183 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst184 = (0.0f - (2.0f * fConst183));
		fConst185 = mydsp_faustpower2_f(fConst183);
		float fConst186 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst187 = (0.0f - (2.0f * fConst186));
		fConst188 = mydsp_faustpower2_f(fConst186);
		float fConst189 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst190 = (0.0f - (2.0f * fConst189));
		fConst191 = mydsp_faustpower2_f(fConst189);
		float fConst192 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst193 = (0.0f - (2.0f * fConst192));
		fConst194 = mydsp_faustpower2_f(fConst192);
		float fConst195 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst196 = (0.0f - (2.0f * fConst195));
		fConst197 = mydsp_faustpower2_f(fConst195);
		float fConst198 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst199 = (0.0f - (2.0f * fConst198));
		fConst200 = mydsp_faustpower2_f(fConst198);
		float fConst201 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst202 = (0.0f - (2.0f * fConst201));
		fConst203 = mydsp_faustpower2_f(fConst201);
		float fConst204 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst205 = (0.0f - (2.0f * fConst204));
		fConst206 = mydsp_faustpower2_f(fConst204);
		float fConst207 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst208 = (0.0f - (2.0f * fConst207));
		fConst209 = mydsp_faustpower2_f(fConst207);
		float fConst210 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst211 = (0.0f - (2.0f * fConst210));
		fConst212 = mydsp_faustpower2_f(fConst210);
		float fConst213 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst214 = (0.0f - (2.0f * fConst213));
		fConst215 = mydsp_faustpower2_f(fConst213);
		fConst216 = std::exp((0.0f - (500.0f / fConst0)));
		fConst217 = (1.0f - fConst216);
		fConst218 = (0.330000013f * fConst0);
		fConst219 = (0.100000001f * fConst0);
		fConst220 = (0.660000026f * fConst0);
		fConst221 = std::exp((0.0f - (100.0f / fConst0)));
		fConst222 = (0.699999988f * (1.0f - fConst221));
		fConst223 = (0.00147058826f * fConst0);
		float fConst224 = std::tan((37699.1133f / fConst0));
		float fConst225 = (1.0f / fConst224);
		fConst226 = (1.0f / (((fConst225 + 1.41421354f) / fConst224) + 1.0f));
		float fConst227 = std::tan((31.415926f / fConst0));
		float fConst228 = (1.0f / fConst227);
		fConst229 = (1.0f / (((fConst228 + 1.41421354f) / fConst227) + 1.0f));
		float fConst230 = mydsp_faustpower2_f(fConst227);
		fConst231 = (1.0f / fConst230);
		fConst232 = (((fConst228 + -1.41421354f) / fConst227) + 1.0f);
		fConst233 = (2.0f * (1.0f - fConst231));
		fConst234 = (0.0f - (2.0f / fConst230));
		fConst235 = (((fConst225 + -1.41421354f) / fConst224) + 1.0f);
		fConst236 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst224))));
		fConst237 = (1.0f / std::max<float>(1.0f, (0.00400000019f * fConst0)));
		float fConst238 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst239 = (0.0f - (2.0f * fConst238));
		fConst240 = mydsp_faustpower2_f(fConst238);
		fConst241 = ((0.5f * fConst0) + -1.0f);
		fConst242 = ftbl1mydspSIG1[0];
		float fConst243 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst244 = (0.0f - (2.0f * fConst243));
		fConst245 = (20.8197117f / fConst0);
		fConst246 = mydsp_faustpower2_f(fConst243);
		fConst247 = ftbl1mydspSIG1[1];
		float fConst248 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst249 = (0.0f - (2.0f * fConst248));
		fConst250 = (24.0940685f / fConst0);
		fConst251 = mydsp_faustpower2_f(fConst248);
		fConst252 = ftbl1mydspSIG1[2];
		float fConst253 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst254 = (0.0f - (2.0f * fConst253));
		fConst255 = (50.6621399f / fConst0);
		fConst256 = mydsp_faustpower2_f(fConst253);
		fConst257 = ftbl1mydspSIG1[3];
		float fConst258 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst259 = (0.0f - (2.0f * fConst258));
		fConst260 = (59.3621521f / fConst0);
		fConst261 = mydsp_faustpower2_f(fConst258);
		fConst262 = ftbl1mydspSIG1[4];
		float fConst263 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst264 = (0.0f - (2.0f * fConst263));
		fConst265 = (88.6990967f / fConst0);
		fConst266 = mydsp_faustpower2_f(fConst263);
		fConst267 = ftbl1mydspSIG1[5];
		float fConst268 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst269 = (0.0f - (2.0f * fConst268));
		fConst270 = (115.510078f / fConst0);
		fConst271 = mydsp_faustpower2_f(fConst268);
		fConst272 = ftbl1mydspSIG1[6];
		float fConst273 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst274 = (0.0f - (2.0f * fConst273));
		fConst275 = (132.010986f / fConst0);
		fConst276 = mydsp_faustpower2_f(fConst273);
		fConst277 = ftbl1mydspSIG1[7];
		float fConst278 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst279 = (0.0f - (2.0f * fConst278));
		fConst280 = (164.478088f / fConst0);
		fConst281 = mydsp_faustpower2_f(fConst278);
		fConst282 = ftbl1mydspSIG1[8];
		float fConst283 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst284 = (0.0f - (2.0f * fConst283));
		fConst285 = (182.177185f / fConst0);
		fConst286 = mydsp_faustpower2_f(fConst283);
		fConst287 = ftbl1mydspSIG1[9];
		float fConst288 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst289 = (0.0f - (2.0f * fConst288));
		fConst290 = (232.935272f / fConst0);
		fConst291 = mydsp_faustpower2_f(fConst288);
		fConst292 = ftbl1mydspSIG1[10];
		float fConst293 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst294 = (0.0f - (2.0f * fConst293));
		fConst295 = (237.946106f / fConst0);
		fConst296 = mydsp_faustpower2_f(fConst293);
		fConst297 = ftbl1mydspSIG1[11];
		float fConst298 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst299 = (0.0f - (2.0f * fConst298));
		fConst300 = (251.725769f / fConst0);
		fConst301 = mydsp_faustpower2_f(fConst298);
		fConst302 = ftbl1mydspSIG1[12];
		float fConst303 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst304 = (0.0f - (2.0f * fConst303));
		fConst305 = (299.355438f / fConst0);
		fConst306 = mydsp_faustpower2_f(fConst303);
		fConst307 = ftbl1mydspSIG1[13];
		float fConst308 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst309 = (0.0f - (2.0f * fConst308));
		fConst310 = (320.561829f / fConst0);
		fConst311 = mydsp_faustpower2_f(fConst308);
		fConst312 = ftbl1mydspSIG1[14];
		float fConst313 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst314 = (0.0f - (2.0f * fConst313));
		fConst315 = (329.427399f / fConst0);
		fConst316 = mydsp_faustpower2_f(fConst313);
		fConst317 = ftbl1mydspSIG1[15];
		float fConst318 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst319 = (0.0f - (2.0f * fConst318));
		fConst320 = (366.221741f / fConst0);
		fConst321 = mydsp_faustpower2_f(fConst318);
		fConst322 = ftbl1mydspSIG1[16];
		float fConst323 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst324 = (0.0f - (2.0f * fConst323));
		fConst325 = (399.287628f / fConst0);
		fConst326 = mydsp_faustpower2_f(fConst323);
		fConst327 = ftbl1mydspSIG1[17];
		float fConst328 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst329 = (0.0f - (2.0f * fConst328));
		fConst330 = (410.686584f / fConst0);
		fConst331 = mydsp_faustpower2_f(fConst328);
		fConst332 = ftbl1mydspSIG1[18];
		float fConst333 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst334 = (0.0f - (2.0f * fConst333));
		fConst335 = (420.713928f / fConst0);
		fConst336 = mydsp_faustpower2_f(fConst333);
		fConst337 = ftbl1mydspSIG1[19];
		float fConst338 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst339 = (0.0f - (2.0f * fConst338));
		fConst340 = (468.286438f / fConst0);
		fConst341 = mydsp_faustpower2_f(fConst338);
		fConst342 = ftbl1mydspSIG1[20];
		float fConst343 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst344 = (0.0f - (2.0f * fConst343));
		fConst345 = (494.436432f / fConst0);
		fConst346 = mydsp_faustpower2_f(fConst343);
		fConst347 = ftbl1mydspSIG1[21];
		float fConst348 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst349 = (0.0f - (2.0f * fConst348));
		fConst350 = (507.916992f / fConst0);
		fConst351 = mydsp_faustpower2_f(fConst348);
		fConst352 = ftbl1mydspSIG1[22];
		float fConst353 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst354 = (0.0f - (2.0f * fConst353));
		fConst355 = (565.34845f / fConst0);
		fConst356 = mydsp_faustpower2_f(fConst353);
		fConst357 = ftbl1mydspSIG1[23];
		float fConst358 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst359 = (0.0f - (2.0f * fConst358));
		fConst360 = (584.12323f / fConst0);
		fConst361 = mydsp_faustpower2_f(fConst358);
		fConst362 = ftbl1mydspSIG1[24];
		float fConst363 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst364 = (0.0f - (2.0f * fConst363));
		fConst365 = (598.105225f / fConst0);
		fConst366 = mydsp_faustpower2_f(fConst363);
		fConst367 = ftbl1mydspSIG1[25];
		float fConst368 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst369 = (0.0f - (2.0f * fConst368));
		fConst370 = (612.489319f / fConst0);
		fConst371 = mydsp_faustpower2_f(fConst368);
		fConst372 = ftbl1mydspSIG1[26];
		float fConst373 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst374 = (0.0f - (2.0f * fConst373));
		fConst375 = (695.045959f / fConst0);
		fConst376 = mydsp_faustpower2_f(fConst373);
		fConst377 = ftbl1mydspSIG1[27];
		float fConst378 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst379 = (0.0f - (2.0f * fConst378));
		fConst380 = (704.150269f / fConst0);
		fConst381 = mydsp_faustpower2_f(fConst378);
		fConst382 = ftbl1mydspSIG1[28];
		float fConst383 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst384 = (0.0f - (2.0f * fConst383));
		fConst385 = (715.18988f / fConst0);
		fConst386 = mydsp_faustpower2_f(fConst383);
		fConst387 = ftbl1mydspSIG1[29];
		float fConst388 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst389 = (0.0f - (2.0f * fConst388));
		fConst390 = (749.935852f / fConst0);
		fConst391 = mydsp_faustpower2_f(fConst388);
		fConst392 = ftbl1mydspSIG1[30];
		float fConst393 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst394 = (0.0f - (2.0f * fConst393));
		fConst395 = (798.247253f / fConst0);
		fConst396 = mydsp_faustpower2_f(fConst393);
		fConst397 = ftbl1mydspSIG1[31];
		float fConst398 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst399 = (0.0f - (2.0f * fConst398));
		fConst400 = (816.700989f / fConst0);
		fConst401 = mydsp_faustpower2_f(fConst398);
		fConst402 = ftbl1mydspSIG1[32];
		float fConst403 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst404 = (0.0f - (2.0f * fConst403));
		fConst405 = (831.007812f / fConst0);
		fConst406 = mydsp_faustpower2_f(fConst403);
		fConst407 = ftbl1mydspSIG1[33];
		float fConst408 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst409 = (0.0f - (2.0f * fConst408));
		fConst410 = (838.660706f / fConst0);
		fConst411 = mydsp_faustpower2_f(fConst408);
		fConst412 = ftbl1mydspSIG1[34];
		float fConst413 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst414 = (0.0f - (2.0f * fConst413));
		fConst415 = (908.228149f / fConst0);
		fConst416 = mydsp_faustpower2_f(fConst413);
		fConst417 = ftbl1mydspSIG1[35];
		float fConst418 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst419 = (0.0f - (2.0f * fConst418));
		fConst420 = (938.946655f / fConst0);
		fConst421 = mydsp_faustpower2_f(fConst418);
		fConst422 = ftbl1mydspSIG1[36];
		float fConst423 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst424 = (0.0f - (2.0f * fConst423));
		fConst425 = (955.251526f / fConst0);
		fConst426 = mydsp_faustpower2_f(fConst423);
		fConst427 = ftbl1mydspSIG1[37];
		float fConst428 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst429 = (0.0f - (2.0f * fConst428));
		fConst430 = (962.370361f / fConst0);
		fConst431 = mydsp_faustpower2_f(fConst428);
		fConst432 = ftbl1mydspSIG1[38];
		float fConst433 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst434 = (0.0f - (2.0f * fConst433));
		fConst435 = (977.644775f / fConst0);
		fConst436 = mydsp_faustpower2_f(fConst433);
		fConst437 = ftbl1mydspSIG1[39];
		float fConst438 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst439 = (0.0f - (2.0f * fConst438));
		fConst440 = (993.893127f / fConst0);
		fConst441 = mydsp_faustpower2_f(fConst438);
		fConst442 = ftbl1mydspSIG1[40];
		float fConst443 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst444 = (0.0f - (2.0f * fConst443));
		fConst445 = (1056.23486f / fConst0);
		fConst446 = mydsp_faustpower2_f(fConst443);
		fConst447 = ftbl1mydspSIG1[41];
		float fConst448 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst449 = (0.0f - (2.0f * fConst448));
		fConst450 = (1079.84705f / fConst0);
		fConst451 = mydsp_faustpower2_f(fConst448);
		fConst452 = ftbl1mydspSIG1[42];
		float fConst453 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst454 = (0.0f - (2.0f * fConst453));
		fConst455 = (1096.1897f / fConst0);
		fConst456 = mydsp_faustpower2_f(fConst453);
		fConst457 = ftbl1mydspSIG1[43];
		float fConst458 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst459 = (0.0f - (2.0f * fConst458));
		fConst460 = (1124.29431f / fConst0);
		fConst461 = mydsp_faustpower2_f(fConst458);
		fConst462 = ftbl1mydspSIG1[44];
		float fConst463 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst464 = (0.0f - (2.0f * fConst463));
		fConst465 = (1140.28503f / fConst0);
		fConst466 = mydsp_faustpower2_f(fConst463);
		fConst467 = ftbl1mydspSIG1[45];
		float fConst468 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst469 = (0.0f - (2.0f * fConst468));
		fConst470 = (1164.89001f / fConst0);
		fConst471 = mydsp_faustpower2_f(fConst468);
		fConst472 = ftbl1mydspSIG1[46];
		float fConst473 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst474 = (0.0f - (2.0f * fConst473));
		fConst475 = (1196.12366f / fConst0);
		fConst476 = mydsp_faustpower2_f(fConst473);
		fConst477 = ftbl1mydspSIG1[47];
		float fConst478 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst479 = (0.0f - (2.0f * fConst478));
		fConst480 = (1207.56543f / fConst0);
		fConst481 = mydsp_faustpower2_f(fConst478);
		fConst482 = ftbl1mydspSIG1[48];
		float fConst483 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst484 = (0.0f - (2.0f * fConst483));
		fConst485 = (1228.39417f / fConst0);
		fConst486 = mydsp_faustpower2_f(fConst483);
		fConst487 = ftbl1mydspSIG1[49];
		fConst488 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst489 = (81.0f / fConst0);
		fConst490 = ftbl2mydspSIG2[50];
		float fConst491 = std::tan((31415.9258f / fConst0));
		float fConst492 = (1.0f / fConst491);
		fConst493 = (1.0f / (((fConst492 + 1.41421354f) / fConst491) + 1.0f));
		fConst494 = (((fConst492 + -1.41421354f) / fConst491) + 1.0f);
		fConst495 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst491))));
		fConst496 = (1.0f / std::max<float>(1.0f, (0.00159999996f * fConst0)));
		float fConst497 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst498 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst497)));
		fConst499 = mydsp_faustpower2_f(fConst497);
		fConst500 = ftbl2mydspSIG2[51];
		float fConst501 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst502 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst501)));
		fConst503 = mydsp_faustpower2_f(fConst501);
		fConst504 = ftbl2mydspSIG2[52];
		float fConst505 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst506 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst505)));
		fConst507 = mydsp_faustpower2_f(fConst505);
		fConst508 = ftbl2mydspSIG2[53];
		float fConst509 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst510 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst509)));
		fConst511 = mydsp_faustpower2_f(fConst509);
		fConst512 = ftbl2mydspSIG2[54];
		float fConst513 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst514 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst513)));
		fConst515 = mydsp_faustpower2_f(fConst513);
		fConst516 = ftbl2mydspSIG2[55];
		float fConst517 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst518 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst517)));
		fConst519 = mydsp_faustpower2_f(fConst517);
		fConst520 = ftbl2mydspSIG2[56];
		float fConst521 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst522 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst521)));
		fConst523 = mydsp_faustpower2_f(fConst521);
		fConst524 = ftbl2mydspSIG2[57];
		float fConst525 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst526 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst525)));
		fConst527 = mydsp_faustpower2_f(fConst525);
		fConst528 = ftbl2mydspSIG2[58];
		float fConst529 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst530 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst529)));
		fConst531 = mydsp_faustpower2_f(fConst529);
		fConst532 = ftbl2mydspSIG2[59];
		float fConst533 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst534 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst533)));
		fConst535 = mydsp_faustpower2_f(fConst533);
		fConst536 = ftbl2mydspSIG2[60];
		float fConst537 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst538 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst537)));
		fConst539 = mydsp_faustpower2_f(fConst537);
		fConst540 = ftbl2mydspSIG2[61];
		float fConst541 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst542 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst541)));
		fConst543 = mydsp_faustpower2_f(fConst541);
		fConst544 = ftbl2mydspSIG2[62];
		float fConst545 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst546 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst545)));
		fConst547 = mydsp_faustpower2_f(fConst545);
		fConst548 = ftbl2mydspSIG2[63];
		float fConst549 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst550 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst549)));
		fConst551 = mydsp_faustpower2_f(fConst549);
		fConst552 = ftbl2mydspSIG2[64];
		float fConst553 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst554 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst553)));
		fConst555 = mydsp_faustpower2_f(fConst553);
		fConst556 = ftbl2mydspSIG2[65];
		float fConst557 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst558 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst557)));
		fConst559 = mydsp_faustpower2_f(fConst557);
		fConst560 = ftbl2mydspSIG2[66];
		float fConst561 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst562 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst561)));
		fConst563 = mydsp_faustpower2_f(fConst561);
		fConst564 = ftbl2mydspSIG2[67];
		float fConst565 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst566 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst565)));
		fConst567 = mydsp_faustpower2_f(fConst565);
		fConst568 = ftbl2mydspSIG2[68];
		float fConst569 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst570 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst569)));
		fConst571 = mydsp_faustpower2_f(fConst569);
		fConst572 = ftbl2mydspSIG2[69];
		float fConst573 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst574 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst573)));
		fConst575 = mydsp_faustpower2_f(fConst573);
		fConst576 = ftbl2mydspSIG2[70];
		float fConst577 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst578 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst577)));
		fConst579 = mydsp_faustpower2_f(fConst577);
		fConst580 = ftbl2mydspSIG2[71];
		float fConst581 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst582 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst581)));
		fConst583 = mydsp_faustpower2_f(fConst581);
		fConst584 = ftbl2mydspSIG2[72];
		float fConst585 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst586 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst585)));
		fConst587 = mydsp_faustpower2_f(fConst585);
		fConst588 = ftbl2mydspSIG2[73];
		float fConst589 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst590 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst589)));
		fConst591 = mydsp_faustpower2_f(fConst589);
		fConst592 = ftbl2mydspSIG2[74];
		float fConst593 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst594 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst593)));
		fConst595 = mydsp_faustpower2_f(fConst593);
		fConst596 = ftbl2mydspSIG2[75];
		float fConst597 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst598 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst597)));
		fConst599 = mydsp_faustpower2_f(fConst597);
		fConst600 = ftbl2mydspSIG2[76];
		float fConst601 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst602 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst601)));
		fConst603 = mydsp_faustpower2_f(fConst601);
		fConst604 = ftbl2mydspSIG2[77];
		float fConst605 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst606 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst605)));
		fConst607 = mydsp_faustpower2_f(fConst605);
		fConst608 = ftbl2mydspSIG2[78];
		float fConst609 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst610 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst609)));
		fConst611 = mydsp_faustpower2_f(fConst609);
		fConst612 = ftbl2mydspSIG2[79];
		float fConst613 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst614 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst613)));
		fConst615 = mydsp_faustpower2_f(fConst613);
		fConst616 = ftbl2mydspSIG2[80];
		float fConst617 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst618 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst617)));
		fConst619 = mydsp_faustpower2_f(fConst617);
		fConst620 = ftbl2mydspSIG2[81];
		float fConst621 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst622 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst621)));
		fConst623 = mydsp_faustpower2_f(fConst621);
		fConst624 = ftbl2mydspSIG2[82];
		float fConst625 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst626 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst625)));
		fConst627 = mydsp_faustpower2_f(fConst625);
		fConst628 = ftbl2mydspSIG2[83];
		float fConst629 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst630 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst629)));
		fConst631 = mydsp_faustpower2_f(fConst629);
		fConst632 = ftbl2mydspSIG2[84];
		float fConst633 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst634 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst633)));
		fConst635 = mydsp_faustpower2_f(fConst633);
		fConst636 = ftbl2mydspSIG2[85];
		float fConst637 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst638 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst637)));
		fConst639 = mydsp_faustpower2_f(fConst637);
		fConst640 = ftbl2mydspSIG2[86];
		float fConst641 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst642 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst641)));
		fConst643 = mydsp_faustpower2_f(fConst641);
		fConst644 = ftbl2mydspSIG2[87];
		float fConst645 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst646 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst645)));
		fConst647 = mydsp_faustpower2_f(fConst645);
		fConst648 = ftbl2mydspSIG2[88];
		float fConst649 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst650 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst649)));
		fConst651 = mydsp_faustpower2_f(fConst649);
		fConst652 = ftbl2mydspSIG2[89];
		float fConst653 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst654 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst653)));
		fConst655 = mydsp_faustpower2_f(fConst653);
		fConst656 = ftbl2mydspSIG2[90];
		float fConst657 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst658 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst657)));
		fConst659 = mydsp_faustpower2_f(fConst657);
		fConst660 = ftbl2mydspSIG2[91];
		float fConst661 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst662 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst661)));
		fConst663 = mydsp_faustpower2_f(fConst661);
		fConst664 = ftbl2mydspSIG2[92];
		float fConst665 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst666 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst665)));
		fConst667 = mydsp_faustpower2_f(fConst665);
		fConst668 = ftbl2mydspSIG2[93];
		float fConst669 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst670 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst669)));
		fConst671 = mydsp_faustpower2_f(fConst669);
		fConst672 = ftbl2mydspSIG2[94];
		float fConst673 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst674 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst673)));
		fConst675 = mydsp_faustpower2_f(fConst673);
		fConst676 = ftbl2mydspSIG2[95];
		float fConst677 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst678 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst677)));
		fConst679 = mydsp_faustpower2_f(fConst677);
		fConst680 = ftbl2mydspSIG2[96];
		float fConst681 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst682 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst681)));
		fConst683 = mydsp_faustpower2_f(fConst681);
		fConst684 = ftbl2mydspSIG2[97];
		float fConst685 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst686 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst685)));
		fConst687 = mydsp_faustpower2_f(fConst685);
		fConst688 = ftbl2mydspSIG2[98];
		float fConst689 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst690 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst689)));
		fConst691 = mydsp_faustpower2_f(fConst689);
		fConst692 = ftbl2mydspSIG2[99];
		float fConst693 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst694 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst693)));
		fConst695 = mydsp_faustpower2_f(fConst693);
		fConst696 = (1.0f / std::max<float>(1.0f, (2.0f * fConst0)));
		fConst697 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst698 = (1.0f - fConst697);
		fConst699 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst700 = (1.0f - fConst699);
		fConst701 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst702 = (1.0f - fConst701);
		fConst703 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst704 = (1.0f - fConst703);
		fConst705 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst706 = (1.0f - fConst705);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(-6.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.69999999999999996f);
		fHslider2 = FAUSTFLOAT(20000.0f);
		fHslider3 = FAUSTFLOAT(2000.0f);
		fHslider4 = FAUSTFLOAT(500.0f);
		fHslider5 = FAUSTFLOAT(0.5f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(440.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(1.0f);
		fHslider11 = FAUSTFLOAT(0.10000000000000001f);
		fHslider12 = FAUSTFLOAT(0.10000000000000001f);
		fHslider13 = FAUSTFLOAT(0.0f);
		fHslider14 = FAUSTFLOAT(0.69999999999999996f);
		fHslider15 = FAUSTFLOAT(50.0f);
		fHslider16 = FAUSTFLOAT(0.0f);
		fHslider17 = FAUSTFLOAT(0.69999999999999996f);
		fHslider18 = FAUSTFLOAT(0.0f);
		fHslider19 = FAUSTFLOAT(0.69999999999999996f);
		fHslider20 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider21 = FAUSTFLOAT(0.69999999999999996f);
		fHslider22 = FAUSTFLOAT(20000.0f);
		fHslider23 = FAUSTFLOAT(2000.0f);
		fHslider24 = FAUSTFLOAT(500.0f);
		fHslider25 = FAUSTFLOAT(0.0f);
		fHslider26 = FAUSTFLOAT(440.0f);
		fHslider27 = FAUSTFLOAT(0.01f);
		fHslider28 = FAUSTFLOAT(1500.0f);
		fHslider29 = FAUSTFLOAT(1.0f);
		fHslider30 = FAUSTFLOAT(0.10000000000000001f);
		fHslider31 = FAUSTFLOAT(0.10000000000000001f);
		fHslider32 = FAUSTFLOAT(0.0f);
		fHslider33 = FAUSTFLOAT(0.69999999999999996f);
		fHslider34 = FAUSTFLOAT(50.0f);
		fHslider35 = FAUSTFLOAT(0.0f);
		fHslider36 = FAUSTFLOAT(0.69999999999999996f);
		fHslider37 = FAUSTFLOAT(0.0f);
		fHslider38 = FAUSTFLOAT(0.69999999999999996f);
		fHslider39 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider40 = FAUSTFLOAT(0.69999999999999996f);
		fHslider41 = FAUSTFLOAT(20000.0f);
		fHslider42 = FAUSTFLOAT(2000.0f);
		fHslider43 = FAUSTFLOAT(500.0f);
		fHslider44 = FAUSTFLOAT(1.0f);
		fHslider45 = FAUSTFLOAT(0.0f);
		fHslider46 = FAUSTFLOAT(1.0f);
		fHslider47 = FAUSTFLOAT(0.10000000000000001f);
		fHslider48 = FAUSTFLOAT(0.10000000000000001f);
		fHslider49 = FAUSTFLOAT(0.0f);
		fHslider50 = FAUSTFLOAT(0.69999999999999996f);
		fHslider51 = FAUSTFLOAT(50.0f);
		fHslider52 = FAUSTFLOAT(0.0f);
		fHslider53 = FAUSTFLOAT(0.69999999999999996f);
		fHslider54 = FAUSTFLOAT(0.0f);
		fHslider55 = FAUSTFLOAT(0.69999999999999996f);
		fHslider56 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider57 = FAUSTFLOAT(0.69999999999999996f);
		fHslider58 = FAUSTFLOAT(20000.0f);
		fHslider59 = FAUSTFLOAT(2000.0f);
		fHslider60 = FAUSTFLOAT(500.0f);
		fHslider61 = FAUSTFLOAT(0.0f);
		fHslider62 = FAUSTFLOAT(440.0f);
		fHslider63 = FAUSTFLOAT(440.0f);
		fHslider64 = FAUSTFLOAT(440.0f);
		fHslider65 = FAUSTFLOAT(440.0f);
		fHslider66 = FAUSTFLOAT(1.0f);
		fHslider67 = FAUSTFLOAT(0.10000000000000001f);
		fHslider68 = FAUSTFLOAT(0.10000000000000001f);
		fHslider69 = FAUSTFLOAT(0.0f);
		fHslider70 = FAUSTFLOAT(0.69999999999999996f);
		fHslider71 = FAUSTFLOAT(50.0f);
		fHslider72 = FAUSTFLOAT(0.0f);
		fHslider73 = FAUSTFLOAT(0.69999999999999996f);
		fHslider74 = FAUSTFLOAT(0.0f);
		fHslider75 = FAUSTFLOAT(0.69999999999999996f);
		fHslider76 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider77 = FAUSTFLOAT(0.69999999999999996f);
		fHslider78 = FAUSTFLOAT(20000.0f);
		fHslider79 = FAUSTFLOAT(2000.0f);
		fHslider80 = FAUSTFLOAT(500.0f);
		fHslider81 = FAUSTFLOAT(0.0f);
		fHslider82 = FAUSTFLOAT(1.0f);
		fHslider83 = FAUSTFLOAT(0.10000000000000001f);
		fHslider84 = FAUSTFLOAT(0.10000000000000001f);
		fHslider85 = FAUSTFLOAT(0.0f);
		fHslider86 = FAUSTFLOAT(0.69999999999999996f);
		fHslider87 = FAUSTFLOAT(50.0f);
		fHslider88 = FAUSTFLOAT(0.0f);
		fHslider89 = FAUSTFLOAT(0.69999999999999996f);
		fHslider90 = FAUSTFLOAT(0.0f);
		fHslider91 = FAUSTFLOAT(0.69999999999999996f);
		fHslider92 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider93 = FAUSTFLOAT(0.69999999999999996f);
		fHslider94 = FAUSTFLOAT(20000.0f);
		fHslider95 = FAUSTFLOAT(2000.0f);
		fHslider96 = FAUSTFLOAT(500.0f);
		fHslider97 = FAUSTFLOAT(1.0f);
		fHslider98 = FAUSTFLOAT(0.10000000000000001f);
		fHslider99 = FAUSTFLOAT(0.10000000000000001f);
		fHslider100 = FAUSTFLOAT(0.0f);
		fHslider101 = FAUSTFLOAT(0.69999999999999996f);
		fHslider102 = FAUSTFLOAT(50.0f);
		fHslider103 = FAUSTFLOAT(0.0f);
		fHslider104 = FAUSTFLOAT(0.69999999999999996f);
		fHslider105 = FAUSTFLOAT(0.0f);
		fHslider106 = FAUSTFLOAT(0.69999999999999996f);
		fHslider107 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider108 = FAUSTFLOAT(0.69999999999999996f);
		fHslider109 = FAUSTFLOAT(20000.0f);
		fHslider110 = FAUSTFLOAT(2000.0f);
		fHslider111 = FAUSTFLOAT(500.0f);
		fHslider112 = FAUSTFLOAT(0.0f);
		fHslider113 = FAUSTFLOAT(10.0f);
		fHslider114 = FAUSTFLOAT(10.0f);
		fHslider115 = FAUSTFLOAT(1.0f);
		fHslider116 = FAUSTFLOAT(0.10000000000000001f);
		fHslider117 = FAUSTFLOAT(0.10000000000000001f);
		fHslider118 = FAUSTFLOAT(0.0f);
		fHslider119 = FAUSTFLOAT(0.69999999999999996f);
		fHslider120 = FAUSTFLOAT(50.0f);
		fHslider121 = FAUSTFLOAT(0.0f);
		fHslider122 = FAUSTFLOAT(0.69999999999999996f);
		fHslider123 = FAUSTFLOAT(0.0f);
		fHslider124 = FAUSTFLOAT(0.69999999999999996f);
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
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec18[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec4[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec22[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec21[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec19[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec24[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec23[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec26[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec27[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec28[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec29[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			iRec30[l19] = 0;
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec16[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec31[l21] = 0;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec34[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec33[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec32[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec35[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec15[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec14[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec13[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fVec5[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fVec6[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fVec7[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec65[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec61[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec66[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 4); l35 = (l35 + 1)) {
			fRec68[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 1024); l36 = (l36 + 1)) {
			fRec69[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec8[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fVec9[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2048); l39 = (l39 + 1)) {
			fRec67[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec57[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec53[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 1024); l42 = (l42 + 1)) {
			fRec49[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 4); l43 = (l43 + 1)) {
			fRec47[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2048); l44 = (l44 + 1)) {
			fRec42[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec40[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec70[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec39[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			iRec71[l48] = 0;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			iRec72[l49] = 0;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec75[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec74[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec73[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec76[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec38[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec37[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec36[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2048); l57 = (l57 + 1)) {
			fVec10[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2048); l58 = (l58 + 1)) {
			fVec11[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec9[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec80[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec79[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 32768); l62 = (l62 + 1)) {
			fVec12[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 4096); l63 = (l63 + 1)) {
			fVec13[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec77[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec84[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec83[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 16384); l67 = (l67 + 1)) {
			fVec14[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 4096); l68 = (l68 + 1)) {
			fVec15[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec81[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec88[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec87[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 32768); l72 = (l72 + 1)) {
			fVec16[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 4096); l73 = (l73 + 1)) {
			fVec17[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec85[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec92[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec91[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 16384); l77 = (l77 + 1)) {
			fVec18[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2048); l78 = (l78 + 1)) {
			fVec19[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec89[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec96[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec95[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 16384); l82 = (l82 + 1)) {
			fVec20[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 4096); l83 = (l83 + 1)) {
			fVec21[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec93[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec100[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec99[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 16384); l87 = (l87 + 1)) {
			fVec22[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 4096); l88 = (l88 + 1)) {
			fVec23[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec97[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec104[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec103[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 16384); l92 = (l92 + 1)) {
			fVec24[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2048); l93 = (l93 + 1)) {
			fVec25[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec101[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec1[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec2[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec3[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec4[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec5[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec6[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec7[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec8[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec0[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			iRec111[l104] = 0;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec110[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec109[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fVec26[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fVec27[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fVec28[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			iRec112[l110] = 0;
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec113[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 3); l112 = (l112 + 1)) {
			fRec108[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 3); l113 = (l113 + 1)) {
			fRec114[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 3); l114 = (l114 + 1)) {
			fRec115[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 3); l115 = (l115 + 1)) {
			fRec116[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 3); l116 = (l116 + 1)) {
			fRec117[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 3); l117 = (l117 + 1)) {
			fRec118[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 3); l118 = (l118 + 1)) {
			fRec119[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 3); l119 = (l119 + 1)) {
			fRec120[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 3); l120 = (l120 + 1)) {
			fRec121[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 3); l121 = (l121 + 1)) {
			fRec122[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 3); l122 = (l122 + 1)) {
			fRec123[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 3); l123 = (l123 + 1)) {
			fRec124[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 3); l124 = (l124 + 1)) {
			fRec125[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 3); l125 = (l125 + 1)) {
			fRec126[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 3); l126 = (l126 + 1)) {
			fRec127[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 3); l127 = (l127 + 1)) {
			fRec128[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 3); l128 = (l128 + 1)) {
			fRec129[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 3); l129 = (l129 + 1)) {
			fRec130[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 3); l130 = (l130 + 1)) {
			fRec131[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 3); l131 = (l131 + 1)) {
			fRec132[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec135[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec134[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec133[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec136[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 3); l136 = (l136 + 1)) {
			fRec107[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 3); l137 = (l137 + 1)) {
			fRec106[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 3); l138 = (l138 + 1)) {
			fRec105[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fVec29[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 2); l140 = (l140 + 1)) {
			fVec30[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fVec31[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			iRec141[l142] = 0;
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec140[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fRec144[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			iRec143[l145] = 0;
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec146[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec145[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 3); l148 = (l148 + 1)) {
			fRec142[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec149[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 2); l150 = (l150 + 1)) {
			iRec148[l150] = 0;
		}
		for (int l151 = 0; (l151 < 3); l151 = (l151 + 1)) {
			fRec147[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec152[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			iRec151[l153] = 0;
		}
		for (int l154 = 0; (l154 < 3); l154 = (l154 + 1)) {
			fRec150[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec155[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			iRec154[l156] = 0;
		}
		for (int l157 = 0; (l157 < 3); l157 = (l157 + 1)) {
			fRec153[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec158[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec157[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec156[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 3); l161 = (l161 + 1)) {
			fRec159[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 3); l162 = (l162 + 1)) {
			fRec139[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 3); l163 = (l163 + 1)) {
			fRec138[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 3); l164 = (l164 + 1)) {
			fRec137[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			iRec163[l165] = 0;
		}
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec167[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 2048); l167 = (l167 + 1)) {
			fVec32[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fVec33[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fVec34[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fVec35[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec168[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 2); l172 = (l172 + 1)) {
			fRec165[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fRec169[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 3); l174 = (l174 + 1)) {
			fRec172[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 3); l175 = (l175 + 1)) {
			fRec171[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			iRec173[l176] = 0;
		}
		for (int l177 = 0; (l177 < 3); l177 = (l177 + 1)) {
			fRec170[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 3); l178 = (l178 + 1)) {
			fRec174[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 3); l179 = (l179 + 1)) {
			fRec175[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 3); l180 = (l180 + 1)) {
			fRec176[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 3); l181 = (l181 + 1)) {
			fRec177[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 3); l182 = (l182 + 1)) {
			fRec178[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 3); l183 = (l183 + 1)) {
			fRec179[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 3); l184 = (l184 + 1)) {
			fRec180[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 3); l185 = (l185 + 1)) {
			fRec181[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 3); l186 = (l186 + 1)) {
			fRec182[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 3); l187 = (l187 + 1)) {
			fRec183[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 3); l188 = (l188 + 1)) {
			fRec184[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 3); l189 = (l189 + 1)) {
			fRec185[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 3); l190 = (l190 + 1)) {
			fRec186[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 3); l191 = (l191 + 1)) {
			fRec187[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 3); l192 = (l192 + 1)) {
			fRec188[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 3); l193 = (l193 + 1)) {
			fRec189[l193] = 0.0f;
		}
		for (int l194 = 0; (l194 < 3); l194 = (l194 + 1)) {
			fRec190[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 3); l195 = (l195 + 1)) {
			fRec191[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec192[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec193[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec194[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec195[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 3); l200 = (l200 + 1)) {
			fRec196[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec197[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 3); l202 = (l202 + 1)) {
			fRec198[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 3); l203 = (l203 + 1)) {
			fRec199[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 3); l204 = (l204 + 1)) {
			fRec200[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 3); l205 = (l205 + 1)) {
			fRec201[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 3); l206 = (l206 + 1)) {
			fRec202[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 3); l207 = (l207 + 1)) {
			fRec203[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 3); l208 = (l208 + 1)) {
			fRec204[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 3); l209 = (l209 + 1)) {
			fRec205[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 3); l210 = (l210 + 1)) {
			fRec206[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 3); l211 = (l211 + 1)) {
			fRec207[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 3); l212 = (l212 + 1)) {
			fRec208[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 3); l213 = (l213 + 1)) {
			fRec209[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 3); l214 = (l214 + 1)) {
			fRec210[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 3); l215 = (l215 + 1)) {
			fRec211[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 3); l216 = (l216 + 1)) {
			fRec212[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 3); l217 = (l217 + 1)) {
			fRec213[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 3); l218 = (l218 + 1)) {
			fRec214[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 3); l219 = (l219 + 1)) {
			fRec215[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 3); l220 = (l220 + 1)) {
			fRec216[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 3); l221 = (l221 + 1)) {
			fRec217[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 3); l222 = (l222 + 1)) {
			fRec218[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec219[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec220[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 3); l225 = (l225 + 1)) {
			fRec221[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 3); l226 = (l226 + 1)) {
			fRec222[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fVec36[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 2048); l228 = (l228 + 1)) {
			fVec37[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec166[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 2); l230 = (l230 + 1)) {
			fRec225[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fRec224[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec223[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 3); l233 = (l233 + 1)) {
			fRec226[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fRec162[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 3); l235 = (l235 + 1)) {
			fRec161[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 3); l236 = (l236 + 1)) {
			fRec160[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 2); l237 = (l237 + 1)) {
			fRec230[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 2); l238 = (l238 + 1)) {
			fRec232[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 2); l239 = (l239 + 1)) {
			fRec236[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 2); l240 = (l240 + 1)) {
			fRec235[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 2); l241 = (l241 + 1)) {
			fRec234[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 2); l242 = (l242 + 1)) {
			fRec233[l242] = 0.0f;
		}
		for (int l243 = 0; (l243 < 2); l243 = (l243 + 1)) {
			fRec231[l243] = 0.0f;
		}
		for (int l244 = 0; (l244 < 2); l244 = (l244 + 1)) {
			fRec239[l244] = 0.0f;
		}
		for (int l245 = 0; (l245 < 2); l245 = (l245 + 1)) {
			fRec238[l245] = 0.0f;
		}
		for (int l246 = 0; (l246 < 2); l246 = (l246 + 1)) {
			fRec237[l246] = 0.0f;
		}
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec240[l247] = 0.0f;
		}
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec229[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec228[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec227[l250] = 0.0f;
		}
		for (int l251 = 0; (l251 < 3); l251 = (l251 + 1)) {
			fRec245[l251] = 0.0f;
		}
		for (int l252 = 0; (l252 < 2); l252 = (l252 + 1)) {
			fVec38[l252] = 0.0f;
		}
		for (int l253 = 0; (l253 < 2); l253 = (l253 + 1)) {
			fVec39[l253] = 0.0f;
		}
		for (int l254 = 0; (l254 < 2); l254 = (l254 + 1)) {
			fVec40[l254] = 0.0f;
		}
		for (int l255 = 0; (l255 < 2); l255 = (l255 + 1)) {
			iRec246[l255] = 0;
		}
		for (int l256 = 0; (l256 < 3); l256 = (l256 + 1)) {
			fRec244[l256] = 0.0f;
		}
		for (int l257 = 0; (l257 < 3); l257 = (l257 + 1)) {
			fRec247[l257] = 0.0f;
		}
		for (int l258 = 0; (l258 < 3); l258 = (l258 + 1)) {
			fRec248[l258] = 0.0f;
		}
		for (int l259 = 0; (l259 < 3); l259 = (l259 + 1)) {
			fRec249[l259] = 0.0f;
		}
		for (int l260 = 0; (l260 < 3); l260 = (l260 + 1)) {
			fRec250[l260] = 0.0f;
		}
		for (int l261 = 0; (l261 < 3); l261 = (l261 + 1)) {
			fRec251[l261] = 0.0f;
		}
		for (int l262 = 0; (l262 < 3); l262 = (l262 + 1)) {
			fRec252[l262] = 0.0f;
		}
		for (int l263 = 0; (l263 < 3); l263 = (l263 + 1)) {
			fRec253[l263] = 0.0f;
		}
		for (int l264 = 0; (l264 < 3); l264 = (l264 + 1)) {
			fRec254[l264] = 0.0f;
		}
		for (int l265 = 0; (l265 < 3); l265 = (l265 + 1)) {
			fRec255[l265] = 0.0f;
		}
		for (int l266 = 0; (l266 < 3); l266 = (l266 + 1)) {
			fRec256[l266] = 0.0f;
		}
		for (int l267 = 0; (l267 < 3); l267 = (l267 + 1)) {
			fRec257[l267] = 0.0f;
		}
		for (int l268 = 0; (l268 < 3); l268 = (l268 + 1)) {
			fRec258[l268] = 0.0f;
		}
		for (int l269 = 0; (l269 < 3); l269 = (l269 + 1)) {
			fRec259[l269] = 0.0f;
		}
		for (int l270 = 0; (l270 < 3); l270 = (l270 + 1)) {
			fRec260[l270] = 0.0f;
		}
		for (int l271 = 0; (l271 < 3); l271 = (l271 + 1)) {
			fRec261[l271] = 0.0f;
		}
		for (int l272 = 0; (l272 < 3); l272 = (l272 + 1)) {
			fRec262[l272] = 0.0f;
		}
		for (int l273 = 0; (l273 < 3); l273 = (l273 + 1)) {
			fRec263[l273] = 0.0f;
		}
		for (int l274 = 0; (l274 < 3); l274 = (l274 + 1)) {
			fRec264[l274] = 0.0f;
		}
		for (int l275 = 0; (l275 < 3); l275 = (l275 + 1)) {
			fRec265[l275] = 0.0f;
		}
		for (int l276 = 0; (l276 < 3); l276 = (l276 + 1)) {
			fRec266[l276] = 0.0f;
		}
		for (int l277 = 0; (l277 < 3); l277 = (l277 + 1)) {
			fRec267[l277] = 0.0f;
		}
		for (int l278 = 0; (l278 < 3); l278 = (l278 + 1)) {
			fRec268[l278] = 0.0f;
		}
		for (int l279 = 0; (l279 < 3); l279 = (l279 + 1)) {
			fRec269[l279] = 0.0f;
		}
		for (int l280 = 0; (l280 < 3); l280 = (l280 + 1)) {
			fRec270[l280] = 0.0f;
		}
		for (int l281 = 0; (l281 < 3); l281 = (l281 + 1)) {
			fRec271[l281] = 0.0f;
		}
		for (int l282 = 0; (l282 < 3); l282 = (l282 + 1)) {
			fRec272[l282] = 0.0f;
		}
		for (int l283 = 0; (l283 < 3); l283 = (l283 + 1)) {
			fRec273[l283] = 0.0f;
		}
		for (int l284 = 0; (l284 < 3); l284 = (l284 + 1)) {
			fRec274[l284] = 0.0f;
		}
		for (int l285 = 0; (l285 < 3); l285 = (l285 + 1)) {
			fRec275[l285] = 0.0f;
		}
		for (int l286 = 0; (l286 < 3); l286 = (l286 + 1)) {
			fRec276[l286] = 0.0f;
		}
		for (int l287 = 0; (l287 < 3); l287 = (l287 + 1)) {
			fRec277[l287] = 0.0f;
		}
		for (int l288 = 0; (l288 < 3); l288 = (l288 + 1)) {
			fRec278[l288] = 0.0f;
		}
		for (int l289 = 0; (l289 < 3); l289 = (l289 + 1)) {
			fRec279[l289] = 0.0f;
		}
		for (int l290 = 0; (l290 < 3); l290 = (l290 + 1)) {
			fRec280[l290] = 0.0f;
		}
		for (int l291 = 0; (l291 < 3); l291 = (l291 + 1)) {
			fRec281[l291] = 0.0f;
		}
		for (int l292 = 0; (l292 < 3); l292 = (l292 + 1)) {
			fRec282[l292] = 0.0f;
		}
		for (int l293 = 0; (l293 < 3); l293 = (l293 + 1)) {
			fRec283[l293] = 0.0f;
		}
		for (int l294 = 0; (l294 < 3); l294 = (l294 + 1)) {
			fRec284[l294] = 0.0f;
		}
		for (int l295 = 0; (l295 < 3); l295 = (l295 + 1)) {
			fRec285[l295] = 0.0f;
		}
		for (int l296 = 0; (l296 < 3); l296 = (l296 + 1)) {
			fRec286[l296] = 0.0f;
		}
		for (int l297 = 0; (l297 < 3); l297 = (l297 + 1)) {
			fRec287[l297] = 0.0f;
		}
		for (int l298 = 0; (l298 < 3); l298 = (l298 + 1)) {
			fRec288[l298] = 0.0f;
		}
		for (int l299 = 0; (l299 < 3); l299 = (l299 + 1)) {
			fRec289[l299] = 0.0f;
		}
		for (int l300 = 0; (l300 < 3); l300 = (l300 + 1)) {
			fRec290[l300] = 0.0f;
		}
		for (int l301 = 0; (l301 < 3); l301 = (l301 + 1)) {
			fRec291[l301] = 0.0f;
		}
		for (int l302 = 0; (l302 < 3); l302 = (l302 + 1)) {
			fRec292[l302] = 0.0f;
		}
		for (int l303 = 0; (l303 < 3); l303 = (l303 + 1)) {
			fRec293[l303] = 0.0f;
		}
		for (int l304 = 0; (l304 < 3); l304 = (l304 + 1)) {
			fRec294[l304] = 0.0f;
		}
		for (int l305 = 0; (l305 < 3); l305 = (l305 + 1)) {
			fRec295[l305] = 0.0f;
		}
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec296[l306] = 0.0f;
		}
		for (int l307 = 0; (l307 < 2); l307 = (l307 + 1)) {
			fRec297[l307] = 0.0f;
		}
		for (int l308 = 0; (l308 < 2); l308 = (l308 + 1)) {
			fRec298[l308] = 0.0f;
		}
		for (int l309 = 0; (l309 < 2); l309 = (l309 + 1)) {
			fRec299[l309] = 0.0f;
		}
		for (int l310 = 0; (l310 < 2); l310 = (l310 + 1)) {
			fRec302[l310] = 0.0f;
		}
		for (int l311 = 0; (l311 < 2); l311 = (l311 + 1)) {
			fRec301[l311] = 0.0f;
		}
		for (int l312 = 0; (l312 < 2); l312 = (l312 + 1)) {
			fRec300[l312] = 0.0f;
		}
		for (int l313 = 0; (l313 < 3); l313 = (l313 + 1)) {
			fRec303[l313] = 0.0f;
		}
		for (int l314 = 0; (l314 < 3); l314 = (l314 + 1)) {
			fRec243[l314] = 0.0f;
		}
		for (int l315 = 0; (l315 < 3); l315 = (l315 + 1)) {
			fRec242[l315] = 0.0f;
		}
		for (int l316 = 0; (l316 < 3); l316 = (l316 + 1)) {
			fRec241[l316] = 0.0f;
		}
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec306[l317] = 0.0f;
		}
		for (int l318 = 0; (l318 < 2); l318 = (l318 + 1)) {
			fRec305[l318] = 0.0f;
		}
		for (int l319 = 0; (l319 < 2); l319 = (l319 + 1)) {
			fRec304[l319] = 0.0f;
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
		for (int l326 = 0; (l326 < 3); l326 = (l326 + 1)) {
			fRec313[l326] = 0.0f;
		}
		for (int l327 = 0; (l327 < 2); l327 = (l327 + 1)) {
			iRec318[l327] = 0;
		}
		for (int l328 = 0; (l328 < 3); l328 = (l328 + 1)) {
			fRec317[l328] = 0.0f;
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
			fRec327[l335] = 0.0f;
		}
		for (int l336 = 0; (l336 < 2); l336 = (l336 + 1)) {
			fRec326[l336] = 0.0f;
		}
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec325[l337] = 0.0f;
		}
		for (int l338 = 0; (l338 < 3); l338 = (l338 + 1)) {
			fRec328[l338] = 0.0f;
		}
		for (int l339 = 0; (l339 < 3); l339 = (l339 + 1)) {
			fRec316[l339] = 0.0f;
		}
		for (int l340 = 0; (l340 < 3); l340 = (l340 + 1)) {
			fRec315[l340] = 0.0f;
		}
		for (int l341 = 0; (l341 < 3); l341 = (l341 + 1)) {
			fRec314[l341] = 0.0f;
		}
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec331[l342] = 0.0f;
		}
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec330[l343] = 0.0f;
		}
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec329[l344] = 0.0f;
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
		ui_interface->declare(&fHslider47, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider47, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider46, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider46, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider48, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider48, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider49, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider49, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider11, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider11, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider10, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider10, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider12, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider12, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider13, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider67, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider67, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider66, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider66, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider68, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider68, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider69, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider69, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->declare(&fHslider83, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider83, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider82, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider82, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider84, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider84, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider85, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider85, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->declare(&fHslider98, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider98, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider97, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider97, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider99, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider99, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider100, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->declare(&fHslider30, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider30, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider29, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider29, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider31, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider31, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider32, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->declare(&fHslider116, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider116, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider115, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider115, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider117, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider117, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider118, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider118, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("EQ_Section");
		ui_interface->openHorizontalBox("Track 1");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider51, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider50, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider43, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider43, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider52, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider53, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider53, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider42, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider42, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider54, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider55, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider41, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider41, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider40, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider40, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider15, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider14, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider16, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider17, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider3, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider18, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider18, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider19, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider2, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider1, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider71, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider71, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider70, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider70, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider60, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider60, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider72, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider72, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider73, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider73, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider59, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider59, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider74, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider74, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider75, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider75, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider58, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider58, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider57, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider87, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider87, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider86, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider86, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider80, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider80, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider88, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider88, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider89, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider89, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider79, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider79, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider90, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider91, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider78, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider78, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider77, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider77, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider102, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider101, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider96, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider96, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider103, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider104, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider95, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider95, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider105, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider106, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider106, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider94, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider94, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider93, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider34, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider33, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider33, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider24, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider24, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider35, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider36, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider23, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider23, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider37, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider38, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider38, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider22, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider22, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider21, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 7");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider120, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider120, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider119, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider119, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider111, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider111, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider121, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider121, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider122, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider122, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider110, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider110, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider123, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider123, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider124, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider124, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider109, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider109, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider108, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider108, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider39, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider0, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider56, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider76, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 5", &fHslider92, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 6", &fHslider20, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 7", &fHslider107, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox2);
		ui_interface->addCheckButton("2", &fCheckbox0);
		ui_interface->addCheckButton("3", &fCheckbox3);
		ui_interface->addCheckButton("4", &fCheckbox4);
		ui_interface->addCheckButton("5", &fCheckbox5);
		ui_interface->addCheckButton("6", &fCheckbox1);
		ui_interface->addCheckButton("7", &fCheckbox6);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider45, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider5, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider9, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider25, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider27, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 15");
		ui_interface->addHorizontalSlider("Control 0", &fHslider44, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 17");
		ui_interface->addHorizontalSlider("Control 0", &fHslider112, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 18");
		ui_interface->addHorizontalSlider("Control 0", &fHslider28, 1500.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 19");
		ui_interface->addHorizontalSlider("Control 0", &fHslider26, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 20");
		ui_interface->addHorizontalSlider("Control 0", &fHslider113, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 21");
		ui_interface->addHorizontalSlider("Control 0", &fHslider114, 10.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider62, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider63, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider64, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider65, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider61, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider8, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider81, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (1.25892544f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = (1.0f / float(fHslider1));
		float fSlow2 = std::tan((fConst28 * float(fHslider2)));
		float fSlow3 = (1.0f / fSlow2);
		float fSlow4 = (((fSlow1 + fSlow3) / fSlow2) + 1.0f);
		float fSlow5 = ((std::pow(10.0f, (0.0500000007f * float(fHslider0))) * (1.0f - float(fCheckbox0))) / fSlow4);
		float fSlow6 = float(fHslider3);
		float fSlow7 = std::tan((fConst28 * fSlow6));
		float fSlow8 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow7))));
		float fSlow9 = float(fHslider4);
		float fSlow10 = std::tan((fConst28 * fSlow9));
		float fSlow11 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow10))));
		float fSlow12 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider5))) + -0.5f)));
		float fSlow13 = (2.0f * fSlow12);
		float fSlow14 = float(fHslider6);
		float fSlow15 = float((fSlow14 > 0.0f));
		float fSlow16 = (0.0f - fSlow14);
		float fSlow17 = float(fHslider7);
		float fSlow18 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider8)));
		float fSlow19 = (0.5f * fSlow18);
		float fSlow20 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider9)));
		float fSlow21 = (fSlow20 + 5.0f);
		int iSlow22 = (fSlow21 < 23.0f);
		int iSlow23 = (fSlow21 < 24.0f);
		float fSlow24 = (fSlow20 + -18.0f);
		int iSlow25 = (fSlow21 < 22.0f);
		float fSlow26 = (fSlow20 + -17.0f);
		int iSlow27 = (fSlow21 < 21.0f);
		float fSlow28 = (fSlow20 + -16.0f);
		int iSlow29 = (fSlow21 < 20.0f);
		float fSlow30 = (fSlow20 + -15.0f);
		int iSlow31 = (fSlow21 < 19.0f);
		float fSlow32 = (fSlow20 + -14.0f);
		int iSlow33 = (fSlow21 < 18.0f);
		float fSlow34 = (fSlow20 + -13.0f);
		int iSlow35 = (fSlow21 < 17.0f);
		float fSlow36 = (fSlow20 + -12.0f);
		int iSlow37 = (fSlow21 < 16.0f);
		float fSlow38 = (fSlow20 + -11.0f);
		float fSlow39 = (80.0f * fSlow38);
		int iSlow40 = (fSlow21 < 15.0f);
		float fSlow41 = (fSlow20 + -10.0f);
		int iSlow42 = (fSlow21 < 14.0f);
		float fSlow43 = (fSlow20 + -9.0f);
		int iSlow44 = (fSlow21 < 13.0f);
		float fSlow45 = (fSlow20 + -8.0f);
		int iSlow46 = (fSlow21 < 12.0f);
		float fSlow47 = (fSlow20 + -7.0f);
		int iSlow48 = (fSlow21 < 11.0f);
		float fSlow49 = (fSlow20 + -6.0f);
		int iSlow50 = (fSlow21 < 10.0f);
		float fSlow51 = (fSlow20 + -5.0f);
		int iSlow52 = (fSlow21 < 9.0f);
		float fSlow53 = (fSlow20 + -4.0f);
		int iSlow54 = (fSlow21 < 8.0f);
		float fSlow55 = (fSlow20 + -3.0f);
		float fSlow56 = (50.0f * fSlow55);
		int iSlow57 = (fSlow21 < 7.0f);
		float fSlow58 = (fSlow20 + -2.0f);
		int iSlow59 = (fSlow21 < 6.0f);
		float fSlow60 = (fSlow20 + -1.0f);
		int iSlow61 = (fSlow21 < 5.0f);
		int iSlow62 = (fSlow21 < 4.0f);
		float fSlow63 = (fSlow20 + 1.0f);
		int iSlow64 = (fSlow21 < 3.0f);
		float fSlow65 = (fSlow20 + 2.0f);
		int iSlow66 = (fSlow21 < 2.0f);
		float fSlow67 = (fSlow20 + 3.0f);
		int iSlow68 = (fSlow21 < 1.0f);
		float fSlow69 = (fSlow20 + 4.0f);
		int iSlow70 = (fSlow21 < 0.0f);
		float fSlow71 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 800.0f : (iSlow68 ? (800.0f - (400.0f * fSlow21)) : 400.0f)) : (iSlow66 ? (400.0f - (50.0f * fSlow69)) : 350.0f)) : (iSlow64 ? ((100.0f * fSlow67) + 350.0f) : 450.0f)) : (iSlow62 ? (450.0f - (125.0f * fSlow65)) : 325.0f)) : (iSlow61 ? ((275.0f * fSlow63) + 325.0f) : 600.0f)) : (iSlow59 ? (600.0f - (200.0f * fSlow20)) : 400.0f)) : (iSlow57 ? (400.0f - (150.0f * fSlow60)) : 250.0f)) : (iSlow54 ? ((150.0f * fSlow58) + 250.0f) : 400.0f)) : (iSlow52 ? (400.0f - fSlow56) : 350.0f)) : (iSlow50 ? ((310.0f * fSlow53) + 350.0f) : 660.0f)) : (iSlow48 ? (660.0f - (220.0f * fSlow51)) : 440.0f)) : (iSlow46 ? (440.0f - (170.0f * fSlow49)) : 270.0f)) : (iSlow44 ? ((160.0f * fSlow47) + 270.0f) : 430.0f)) : (iSlow42 ? (430.0f - (60.0f * fSlow45)) : 370.0f)) : (iSlow40 ? ((430.0f * fSlow43) + 370.0f) : 800.0f)) : (iSlow37 ? (800.0f - (450.0f * fSlow41)) : 350.0f)) : (iSlow35 ? (350.0f - fSlow39) : 270.0f)) : (iSlow33 ? ((180.0f * fSlow36) + 270.0f) : 450.0f)) : (iSlow31 ? (450.0f - (125.0f * fSlow34)) : 325.0f)) : (iSlow29 ? (325.0f * (fSlow32 + 1.0f)) : 650.0f)) : (iSlow27 ? (650.0f - (250.0f * fSlow30)) : 400.0f)) : (iSlow25 ? (400.0f - (110.0f * fSlow28)) : 290.0f)) : (iSlow22 ? ((110.0f * fSlow26) + 290.0f) : 400.0f)) : (iSlow23 ? (400.0f - (50.0f * fSlow24)) : 350.0f));
		float fSlow72 = (20.0f * fSlow30);
		float fSlow73 = (20.0f * fSlow41);
		float fSlow74 = (10.0f * fSlow51);
		float fSlow75 = (10.0f * fSlow63);
		float fSlow76 = (20.0f * fSlow65);
		float fSlow77 = (20.0f * fSlow67);
		float fSlow78 = (20.0f * fSlow21);
		float fSlow79 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 80.0f : (iSlow68 ? (80.0f - fSlow78) : 60.0f)) : (iSlow66 ? (60.0f - (10.0f * fSlow69)) : 50.0f)) : (iSlow64 ? (fSlow77 + 50.0f) : 70.0f)) : (iSlow62 ? (70.0f - fSlow76) : 50.0f)) : (iSlow61 ? (fSlow75 + 50.0f) : 60.0f)) : (iSlow59 ? (60.0f - (20.0f * fSlow20)) : 40.0f)) : (iSlow57 ? ((20.0f * fSlow60) + 40.0f) : 60.0f)) : (iSlow54 ? (60.0f - (20.0f * fSlow58)) : 40.0f)) : 40.0f) : (iSlow50 ? (40.0f * (fSlow53 + 1.0f)) : 80.0f)) : (iSlow48 ? (80.0f - fSlow74) : 70.0f)) : (iSlow46 ? (70.0f - (30.0f * fSlow49)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow40 ? (40.0f * (fSlow43 + 1.0f)) : 80.0f)) : (iSlow37 ? (80.0f - fSlow73) : 60.0f)) : 60.0f) : (iSlow33 ? (60.0f - (20.0f * fSlow36)) : 40.0f)) : (iSlow31 ? ((10.0f * fSlow34) + 40.0f) : 50.0f)) : 50.0f) : (iSlow27 ? (fSlow72 + 50.0f) : 70.0f)) : (iSlow25 ? (70.0f - (30.0f * fSlow28)) : 40.0f)) : (iSlow22 ? ((30.0f * fSlow26) + 40.0f) : 70.0f)) : (iSlow23 ? (70.0f - (30.0f * fSlow24)) : 40.0f));
		float fSlow80 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 1150.0f : (iSlow68 ? ((450.0f * fSlow21) + 1150.0f) : 1600.0f)) : (iSlow66 ? ((100.0f * fSlow69) + 1600.0f) : 1700.0f)) : (iSlow64 ? (1700.0f - (900.0f * fSlow67)) : 800.0f)) : (iSlow62 ? (800.0f - (100.0f * fSlow65)) : 700.0f)) : (iSlow61 ? ((340.0f * fSlow63) + 700.0f) : 1040.0f)) : (iSlow59 ? ((580.0f * fSlow20) + 1040.0f) : 1620.0f)) : (iSlow57 ? ((130.0f * fSlow60) + 1620.0f) : 1750.0f)) : (iSlow54 ? (1750.0f - (1000.0f * fSlow58)) : 750.0f)) : (iSlow52 ? (750.0f - (150.0f * fSlow55)) : 600.0f)) : (iSlow50 ? ((520.0f * fSlow53) + 600.0f) : 1120.0f)) : (iSlow48 ? ((680.0f * fSlow51) + 1120.0f) : 1800.0f)) : (iSlow46 ? ((50.0f * fSlow49) + 1800.0f) : 1850.0f)) : (iSlow44 ? (1850.0f - (1030.0f * fSlow47)) : 820.0f)) : (iSlow42 ? (820.0f - (190.0f * fSlow45)) : 630.0f)) : (iSlow40 ? ((520.0f * fSlow43) + 630.0f) : 1150.0f)) : (iSlow37 ? ((850.0f * fSlow41) + 1150.0f) : 2000.0f)) : (iSlow35 ? ((140.0f * fSlow38) + 2000.0f) : 2140.0f)) : (iSlow33 ? (2140.0f - (1340.0f * fSlow36)) : 800.0f)) : (iSlow31 ? (800.0f - (100.0f * fSlow34)) : 700.0f)) : (iSlow29 ? ((380.0f * fSlow32) + 700.0f) : 1080.0f)) : (iSlow27 ? ((620.0f * fSlow30) + 1080.0f) : 1700.0f)) : (iSlow25 ? ((170.0f * fSlow28) + 1700.0f) : 1870.0f)) : (iSlow22 ? (1870.0f - (1070.0f * fSlow26)) : 800.0f)) : (iSlow23 ? (800.0f - (200.0f * fSlow24)) : 600.0f));
		int iSlow81 = (fSlow80 >= 1300.0f);
		float fSlow82 = (fSlow80 + -1300.0f);
		float fSlow83 = (10.0f * fSlow26);
		float fSlow84 = (10.0f * fSlow30);
		float fSlow85 = (10.0f * fSlow53);
		float fSlow86 = (10.0f * fSlow20);
		float fSlow87 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 90.0f : (iSlow68 ? (90.0f - (10.0f * fSlow21)) : 80.0f)) : (iSlow66 ? ((20.0f * fSlow69) + 80.0f) : 100.0f)) : (iSlow64 ? (100.0f - fSlow77) : 80.0f)) : (iSlow62 ? (80.0f - fSlow76) : 60.0f)) : (iSlow61 ? (fSlow75 + 60.0f) : 70.0f)) : (iSlow59 ? (fSlow86 + 70.0f) : 80.0f)) : (iSlow57 ? ((10.0f * fSlow60) + 80.0f) : 90.0f)) : (iSlow54 ? (90.0f - (10.0f * fSlow58)) : 80.0f)) : 80.0f) : (iSlow50 ? (fSlow85 + 80.0f) : 90.0f)) : (iSlow48 ? (90.0f - fSlow74) : 80.0f)) : (iSlow46 ? ((10.0f * fSlow49) + 80.0f) : 90.0f)) : (iSlow44 ? (90.0f - (10.0f * fSlow47)) : 80.0f)) : (iSlow42 ? (80.0f - (20.0f * fSlow45)) : 60.0f)) : (iSlow40 ? ((30.0f * fSlow43) + 60.0f) : 90.0f)) : (iSlow37 ? ((10.0f * fSlow41) + 90.0f) : 100.0f)) : (iSlow35 ? (100.0f - (10.0f * fSlow38)) : 90.0f)) : (iSlow33 ? (90.0f - (10.0f * fSlow36)) : 80.0f)) : (iSlow31 ? (80.0f - (20.0f * fSlow34)) : 60.0f)) : (iSlow29 ? ((30.0f * fSlow32) + 60.0f) : 90.0f)) : (iSlow27 ? (90.0f - fSlow84) : 80.0f)) : (iSlow25 ? ((10.0f * fSlow28) + 80.0f) : 90.0f)) : (iSlow22 ? (90.0f - fSlow83) : 80.0f)) : (iSlow23 ? (80.0f - (20.0f * fSlow24)) : 60.0f));
		float fSlow88 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 0.630957007f : (iSlow68 ? (0.630957007f - (0.567861021f * fSlow21)) : 0.0630960017f)) : (iSlow66 ? ((0.0369039997f * fSlow69) + 0.0630960017f) : 0.100000001f)) : (iSlow64 ? ((0.254812986f * fSlow67) + 0.100000001f) : 0.35481301f)) : (iSlow62 ? (0.35481301f - (0.103624001f * fSlow65)) : 0.251188993f)) : (iSlow61 ? ((0.195494995f * fSlow63) + 0.251188993f) : 0.446684003f)) : (iSlow59 ? (0.446684003f - (0.195494995f * fSlow20)) : 0.251188993f)) : (iSlow57 ? (0.251188993f - (0.219566002f * fSlow60)) : 0.0316229984f)) : (iSlow54 ? ((0.250214994f * fSlow58) + 0.0316229984f) : 0.281838f)) : (iSlow52 ? (0.281838f - (0.181838006f * fSlow55)) : 0.100000001f)) : (iSlow50 ? ((0.401187003f * fSlow53) + 0.100000001f) : 0.501187027f)) : (iSlow48 ? (0.501187027f - (0.301661015f * fSlow51)) : 0.199525997f)) : (iSlow46 ? (0.199525997f - (0.136429995f * fSlow49)) : 0.0630960017f)) : (iSlow44 ? ((0.253131986f * fSlow47) + 0.0630960017f) : 0.316228002f)) : (iSlow42 ? (0.316228002f - (0.216227993f * fSlow45)) : 0.100000001f)) : (iSlow40 ? ((0.401187003f * fSlow43) + 0.100000001f) : 0.501187027f)) : (iSlow37 ? (0.501187027f - (0.401187003f * fSlow41)) : 0.100000001f)) : (iSlow35 ? ((0.151188999f * fSlow38) + 0.100000001f) : 0.251188993f)) : (iSlow33 ? ((0.0306490008f * fSlow36) + 0.251188993f) : 0.281838f)) : (iSlow31 ? (0.281838f - (0.123349003f * fSlow34)) : 0.158489004f)) : (iSlow29 ? ((0.342698008f * fSlow32) + 0.158489004f) : 0.501187027f)) : (iSlow27 ? (0.501187027f - (0.301661015f * fSlow30)) : 0.199525997f)) : (iSlow25 ? (0.199525997f - (0.0216979999f * fSlow28)) : 0.177827999f)) : (iSlow22 ? ((0.138400003f * fSlow26) + 0.177827999f) : 0.316228002f)) : (iSlow23 ? (0.316228002f - (0.216227993f * fSlow24)) : 0.100000001f));
		float fSlow89 = (100.0f * fSlow24);
		float fSlow90 = (50.0f * fSlow32);
		float fSlow91 = (50.0f * fSlow51);
		float fSlow92 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 2800.0f : (iSlow68 ? (2800.0f - (100.0f * fSlow21)) : 2700.0f)) : 2700.0f) : (iSlow64 ? ((130.0f * fSlow67) + 2700.0f) : 2830.0f)) : (iSlow62 ? (2830.0f - (300.0f * fSlow65)) : 2530.0f)) : (iSlow61 ? (2530.0f - (280.0f * fSlow63)) : 2250.0f)) : (iSlow59 ? ((150.0f * fSlow20) + 2250.0f) : 2400.0f)) : (iSlow57 ? ((200.0f * fSlow60) + 2400.0f) : 2600.0f)) : (iSlow54 ? (2600.0f - (200.0f * fSlow58)) : 2400.0f)) : 2400.0f) : (iSlow50 ? ((350.0f * fSlow53) + 2400.0f) : 2750.0f)) : (iSlow48 ? (2750.0f - fSlow91) : 2700.0f)) : (iSlow46 ? ((200.0f * fSlow49) + 2700.0f) : 2900.0f)) : (iSlow44 ? (2900.0f - (200.0f * fSlow47)) : 2700.0f)) : (iSlow42 ? ((50.0f * fSlow45) + 2700.0f) : 2750.0f)) : (iSlow40 ? ((150.0f * fSlow43) + 2750.0f) : 2900.0f)) : (iSlow37 ? (2900.0f - (100.0f * fSlow41)) : 2800.0f)) : (iSlow35 ? ((150.0f * fSlow38) + 2800.0f) : 2950.0f)) : (iSlow33 ? (2950.0f - (120.0f * fSlow36)) : 2830.0f)) : (iSlow31 ? (2830.0f - (130.0f * fSlow34)) : 2700.0f)) : (iSlow29 ? (2700.0f - fSlow90) : 2650.0f)) : (iSlow27 ? (2650.0f - (50.0f * fSlow30)) : 2600.0f)) : (iSlow25 ? ((200.0f * fSlow28) + 2600.0f) : 2800.0f)) : (iSlow22 ? (2800.0f - (200.0f * fSlow26)) : 2600.0f)) : (iSlow23 ? (fSlow89 + 2600.0f) : 2700.0f));
		float fSlow93 = std::tan((fConst28 * fSlow92));
		float fSlow94 = (1.0f / fSlow93);
		float fSlow95 = (20.0f * fSlow43);
		float fSlow96 = (20.0f * fSlow51);
		float fSlow97 = (20.0f * fSlow53);
		float fSlow98 = (60.0f * fSlow63);
		float fSlow99 = ((iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? 120.0f : (iSlow64 ? (120.0f - fSlow77) : 100.0f)) : (iSlow62 ? ((70.0f * fSlow65) + 100.0f) : 170.0f)) : (iSlow61 ? (170.0f - fSlow98) : 110.0f)) : (iSlow59 ? (110.0f - fSlow86) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow50 ? (fSlow97 + 100.0f) : 120.0f)) : (iSlow48 ? (120.0f - fSlow96) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow40 ? (fSlow95 + 100.0f) : 120.0f)) : 120.0f) : (iSlow35 ? (120.0f - (20.0f * fSlow38)) : 100.0f)) : 100.0f) : (iSlow31 ? ((70.0f * fSlow34) + 100.0f) : 170.0f)) : (iSlow29 ? (170.0f - fSlow90) : 120.0f)) : (iSlow27 ? (120.0f - fSlow72) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow92);
		float fSlow100 = (((fSlow94 - fSlow99) / fSlow93) + 1.0f);
		float fSlow101 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow93)));
		float fSlow102 = (((fSlow94 + fSlow99) / fSlow93) + 1.0f);
		float fSlow103 = (iSlow33 ? ((0.0293140002f * fSlow36) + 0.0501190014f) : 0.0794330016f);
		float fSlow104 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 0.100000001f : (iSlow68 ? (0.100000001f - (0.0683770031f * fSlow21)) : 0.0316229984f)) : 0.0316229984f) : (iSlow64 ? ((0.126865998f * fSlow67) + 0.0316229984f) : 0.158489004f)) : (iSlow62 ? (0.158489004f - (0.126865998f * fSlow65)) : 0.0316229984f)) : (iSlow61 ? ((0.323190004f * fSlow63) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow57 ? (0.35481301f - (0.196324006f * fSlow60)) : 0.158489004f)) : (iSlow54 ? (0.158489004f - (0.0693639964f * fSlow58)) : 0.0891249999f)) : (iSlow52 ? (0.0891249999f - (0.0640060008f * fSlow55)) : 0.0251189992f)) : (iSlow50 ? ((0.0456760004f * fSlow53) + 0.0251189992f) : 0.0707949996f)) : (iSlow48 ? ((0.0550980009f * fSlow51) + 0.0707949996f) : 0.125892997f)) : (iSlow46 ? (0.125892997f - (0.0627970025f * fSlow49)) : 0.0630960017f)) : (iSlow44 ? (0.0630960017f - (0.0129770003f * fSlow47)) : 0.0501190014f)) : (iSlow42 ? ((0.020676f * fSlow45) + 0.0501190014f) : 0.0707949996f)) : (iSlow40 ? (0.0707949996f - (0.0456760004f * fSlow43)) : 0.0251189992f)) : (iSlow37 ? ((0.152709007f * fSlow41) + 0.0251189992f) : 0.177827999f)) : (iSlow35 ? (0.177827999f - (0.127709001f * fSlow38)) : 0.0501190014f)) : fSlow103) : (iSlow31 ? (0.0794330016f - (0.0616500005f * fSlow34)) : 0.0177829992f)) : (iSlow29 ? ((0.428900987f * fSlow32) + 0.0177829992f) : 0.446684003f)) : (iSlow27 ? (0.446684003f - (0.195494995f * fSlow30)) : 0.251188993f)) : (iSlow25 ? (0.251188993f - (0.125295997f * fSlow28)) : 0.125892997f)) : (iSlow22 ? ((0.125295997f * fSlow26) + 0.125892997f) : 0.251188993f)) : (iSlow23 ? (0.251188993f - (0.109935001f * fSlow24)) : 0.141253993f));
		float fSlow105 = (fSlow93 * fSlow102);
		float fSlow106 = (350.0f * fSlow20);
		float fSlow107 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 3500.0f : (iSlow68 ? (3500.0f - (200.0f * fSlow21)) : 3300.0f)) : (iSlow66 ? ((400.0f * fSlow69) + 3300.0f) : 3700.0f)) : (iSlow64 ? (3700.0f - (200.0f * fSlow67)) : 3500.0f)) : 3500.0f) : (iSlow61 ? (3500.0f - (1050.0f * fSlow63)) : 2450.0f)) : (iSlow59 ? (fSlow106 + 2450.0f) : 2800.0f)) : (iSlow57 ? ((250.0f * fSlow60) + 2800.0f) : 3050.0f)) : (iSlow54 ? (3050.0f - (450.0f * fSlow58)) : 2600.0f)) : (iSlow52 ? ((75.0f * fSlow55) + 2600.0f) : 2675.0f)) : (iSlow50 ? ((325.0f * fSlow53) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow46 ? ((350.0f * fSlow49) + 3000.0f) : 3350.0f)) : (iSlow44 ? (3350.0f - (350.0f * fSlow47)) : 3000.0f)) : 3000.0f) : (iSlow40 ? ((900.0f * fSlow43) + 3000.0f) : 3900.0f)) : (iSlow37 ? (3900.0f - (300.0f * fSlow41)) : 3600.0f)) : (iSlow35 ? ((300.0f * fSlow38) + 3600.0f) : 3900.0f)) : (iSlow33 ? (3900.0f - (100.0f * fSlow36)) : 3800.0f)) : 3800.0f) : (iSlow29 ? (3800.0f - (900.0f * fSlow32)) : 2900.0f)) : (iSlow27 ? ((300.0f * fSlow30) + 2900.0f) : 3200.0f)) : (iSlow25 ? ((50.0f * fSlow28) + 3200.0f) : 3250.0f)) : (iSlow22 ? (3250.0f - (450.0f * fSlow26)) : 2800.0f)) : (iSlow23 ? (fSlow89 + 2800.0f) : 2900.0f));
		float fSlow108 = std::tan((fConst28 * fSlow107));
		float fSlow109 = (1.0f / fSlow108);
		float fSlow110 = ((iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 130.0f : (iSlow68 ? (fSlow78 + 130.0f) : 150.0f)) : 150.0f) : (iSlow64 ? (150.0f - fSlow77) : 130.0f)) : (iSlow62 ? ((50.0f * fSlow65) + 130.0f) : 180.0f)) : (iSlow61 ? (180.0f - fSlow98) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow50 ? (fSlow85 + 120.0f) : 130.0f)) : (iSlow48 ? (130.0f - fSlow74) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow40 ? ((10.0f * fSlow43) + 120.0f) : 130.0f)) : (iSlow37 ? (fSlow73 + 130.0f) : 150.0f)) : (iSlow35 ? (150.0f - (30.0f * fSlow38)) : 120.0f)) : 120.0f) : (iSlow31 ? ((60.0f * fSlow34) + 120.0f) : 180.0f)) : (iSlow29 ? (180.0f - fSlow90) : 130.0f)) : (iSlow27 ? (130.0f - fSlow84) : 120.0f)) : 120.0f) : (iSlow22 ? (fSlow83 + 120.0f) : 130.0f)) : (iSlow23 ? (130.0f - (10.0f * fSlow24)) : 120.0f)) / fSlow107);
		float fSlow111 = (((fSlow109 - fSlow110) / fSlow108) + 1.0f);
		float fSlow112 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow108)));
		float fSlow113 = (((fSlow109 + fSlow110) / fSlow108) + 1.0f);
		float fSlow114 = (iSlow46 ? (0.100000001f - (0.0841509998f * fSlow49)) : 0.0158489998f);
		float fSlow115 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 0.0158489998f : (iSlow68 ? ((0.00193400006f * fSlow21) + 0.0158489998f) : 0.0177829992f)) : (iSlow66 ? (0.0177829992f - (0.00193400006f * fSlow69)) : 0.0158489998f)) : (iSlow64 ? ((0.0239620004f * fSlow67) + 0.0158489998f) : 0.0398110002f)) : (iSlow62 ? (0.0398110002f - (0.0298110005f * fSlow65)) : 0.00999999978f)) : (iSlow61 ? ((0.344812989f * fSlow63) + 0.00999999978f) : 0.35481301f)) : (iSlow59 ? (0.35481301f - (0.103624001f * fSlow20)) : 0.251188993f)) : (iSlow57 ? (0.251188993f - (0.171755999f * fSlow60)) : 0.0794330016f)) : (iSlow54 ? ((0.0205669999f * fSlow58) + 0.0794330016f) : 0.100000001f)) : (iSlow52 ? (0.100000001f - (0.0601890013f * fSlow55)) : 0.0398110002f)) : (iSlow50 ? ((0.0232849997f * fSlow53) + 0.0398110002f) : 0.0630960017f)) : (iSlow48 ? ((0.0369039997f * fSlow51) + 0.0630960017f) : 0.100000001f)) : fSlow114) : (iSlow44 ? ((0.0635839999f * fSlow47) + 0.0158489998f) : 0.0794330016f)) : (iSlow42 ? (0.0794330016f - (0.0478099994f * fSlow45)) : 0.0316229984f)) : (iSlow40 ? ((0.0683770031f * fSlow43) + 0.0316229984f) : 0.100000001f)) : (iSlow37 ? (0.100000001f - (0.0900000036f * fSlow41)) : 0.00999999978f)) : (iSlow35 ? ((0.0401189998f * fSlow38) + 0.00999999978f) : 0.0501190014f)) : fSlow103) : (iSlow31 ? (0.0794330016f - (0.0694330037f * fSlow34)) : 0.00999999978f)) : (iSlow29 ? ((0.388107002f * fSlow32) + 0.00999999978f) : 0.398106992f)) : (iSlow27 ? (0.398106992f - (0.198580995f * fSlow30)) : 0.199525997f)) : (iSlow25 ? (0.199525997f - (0.099526003f * fSlow28)) : 0.100000001f)) : (iSlow22 ? ((0.151188999f * fSlow26) + 0.100000001f) : 0.251188993f)) : (iSlow23 ? (0.251188993f - (0.0516630001f * fSlow24)) : 0.199525997f));
		float fSlow116 = (fSlow108 * fSlow113);
		float fSlow117 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? 4950.0f : (iSlow61 ? (4950.0f - (2200.0f * fSlow63)) : 2750.0f)) : (iSlow59 ? (fSlow106 + 2750.0f) : 3100.0f)) : (iSlow57 ? ((240.0f * fSlow60) + 3100.0f) : 3340.0f)) : (iSlow54 ? (3340.0f - (440.0f * fSlow58)) : 2900.0f)) : (iSlow52 ? (fSlow56 + 2900.0f) : 2950.0f)) : (iSlow50 ? ((400.0f * fSlow53) + 2950.0f) : 3350.0f)) : (iSlow48 ? (3350.0f - fSlow91) : 3300.0f)) : (iSlow46 ? ((290.0f * fSlow49) + 3300.0f) : 3590.0f)) : (iSlow44 ? (3590.0f - (290.0f * fSlow47)) : 3300.0f)) : (iSlow42 ? ((100.0f * fSlow45) + 3300.0f) : 3400.0f)) : (iSlow40 ? ((1550.0f * fSlow43) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow29 ? (4950.0f - (1700.0f * fSlow32)) : 3250.0f)) : (iSlow27 ? ((330.0f * fSlow30) + 3250.0f) : 3580.0f)) : (iSlow25 ? (3580.0f - (40.0f * fSlow28)) : 3540.0f)) : (iSlow22 ? (3540.0f - (540.0f * fSlow26)) : 3000.0f)) : (iSlow23 ? ((300.0f * fSlow24) + 3000.0f) : 3300.0f));
		float fSlow118 = std::tan((fConst28 * fSlow117));
		float fSlow119 = (1.0f / fSlow118);
		float fSlow120 = ((iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? 140.0f : (iSlow68 ? ((60.0f * fSlow21) + 140.0f) : 200.0f)) : 200.0f) : (iSlow64 ? (200.0f - (65.0f * fSlow67)) : 135.0f)) : (iSlow62 ? ((65.0f * fSlow65) + 135.0f) : 200.0f)) : (iSlow61 ? (200.0f - (70.0f * fSlow63)) : 130.0f)) : (iSlow59 ? (130.0f - fSlow86) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow50 ? (fSlow97 + 120.0f) : 140.0f)) : (iSlow48 ? (140.0f - fSlow96) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow40 ? (fSlow95 + 120.0f) : 140.0f)) : (iSlow37 ? ((60.0f * fSlow41) + 140.0f) : 200.0f)) : (iSlow35 ? (200.0f - fSlow39) : 120.0f)) : 120.0f) : (iSlow31 ? ((80.0f * fSlow34) + 120.0f) : 200.0f)) : (iSlow29 ? (200.0f - (60.0f * fSlow32)) : 140.0f)) : (iSlow27 ? (140.0f - fSlow72) : 120.0f)) : 120.0f) : (iSlow22 ? ((15.0f * fSlow26) + 120.0f) : 135.0f)) : (iSlow23 ? (135.0f - (15.0f * fSlow24)) : 120.0f)) / fSlow117);
		float fSlow121 = (((fSlow119 - fSlow120) / fSlow118) + 1.0f);
		float fSlow122 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow118)));
		float fSlow123 = (((fSlow119 + fSlow120) / fSlow118) + 1.0f);
		float fSlow124 = (iSlow22 ? (iSlow25 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow40 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow57 ? (iSlow59 ? (iSlow61 ? (iSlow62 ? (iSlow64 ? (iSlow66 ? 0.00100000005f : (iSlow64 ? ((0.000777999987f * fSlow67) + 0.00100000005f) : 0.00177800003f)) : (iSlow62 ? (0.00177800003f - (0.001147f * fSlow65)) : 0.000630999974f)) : (iSlow61 ? ((0.0993689969f * fSlow63) + 0.000630999974f) : 0.100000001f)) : (iSlow59 ? ((0.0258930009f * fSlow20) + 0.100000001f) : 0.125892997f)) : (iSlow57 ? (0.125892997f - (0.0860819966f * fSlow60)) : 0.0398110002f)) : (iSlow54 ? (0.0398110002f - (0.0298110005f * fSlow58)) : 0.00999999978f)) : (iSlow52 ? ((0.00584900007f * fSlow55) + 0.00999999978f) : 0.0158489998f)) : (iSlow50 ? (0.0158489998f - (0.00325999991f * fSlow53)) : 0.0125890002f)) : (iSlow48 ? ((0.0874110013f * fSlow51) + 0.0125890002f) : 0.100000001f)) : fSlow114) : (iSlow44 ? ((0.00410400005f * fSlow47) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow40 ? (0.0199529994f - (0.0167909991f * fSlow43)) : 0.0031620001f)) : (iSlow37 ? (0.0031620001f - (0.00157700002f * fSlow41)) : 0.00158499996f)) : (iSlow35 ? ((0.00472499989f * fSlow38) + 0.00158499996f) : 0.00631000008f)) : (iSlow33 ? (0.00631000008f - (0.00314799999f * fSlow36)) : 0.0031620001f)) : (iSlow31 ? (0.0031620001f - (0.00216199993f * fSlow34)) : 0.00100000005f)) : (iSlow29 ? ((0.0784329996f * fSlow32) + 0.00100000005f) : 0.0794330016f)) : (iSlow27 ? ((0.0205669999f * fSlow30) + 0.0794330016f) : 0.100000001f)) : (iSlow25 ? (0.100000001f - (0.0683770031f * fSlow28)) : 0.0316229984f)) : (iSlow22 ? ((0.0184959993f * fSlow26) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow125 = (fSlow118 * fSlow123);
		float fSlow126 = (fConst47 * ((1.0f / float(fHslider10)) + -1.0f));
		float fSlow127 = float(fHslider11);
		int iSlow128 = (std::fabs(fSlow127) < 1.1920929e-07f);
		float fSlow129 = (iSlow128 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow128 ? 1.0f : fSlow127)))));
		float fSlow130 = float(fHslider12);
		int iSlow131 = (std::fabs(fSlow130) < 1.1920929e-07f);
		float fSlow132 = (iSlow131 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow131 ? 1.0f : fSlow130)))));
		float fSlow133 = (1.0f - fSlow132);
		float fSlow134 = (1.0f - fSlow129);
		float fSlow135 = float(fHslider13);
		float fSlow136 = (1.0f / float(fHslider14));
		float fSlow137 = std::tan((fConst28 * float(fHslider15)));
		float fSlow138 = (1.0f / fSlow137);
		float fSlow139 = (1.0f / (((fSlow136 + fSlow138) / fSlow137) + 1.0f));
		float fSlow140 = (((fSlow138 - fSlow136) / fSlow137) + 1.0f);
		float fSlow141 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow137))));
		float fSlow142 = (1.0f / fSlow10);
		float fSlow143 = float(fHslider16);
		int iSlow144 = (fSlow143 > 0.0f);
		float fSlow145 = (float(fHslider17) * std::sin((fConst48 * fSlow9)));
		float fSlow146 = (fConst28 * ((fSlow9 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow143)))) / fSlow145));
		float fSlow147 = (fConst28 * (fSlow9 / fSlow145));
		float fSlow148 = (iSlow144 ? fSlow147 : fSlow146);
		float fSlow149 = ((fSlow142 * (fSlow142 - fSlow148)) + 1.0f);
		float fSlow150 = ((fSlow142 * (fSlow142 + fSlow148)) + 1.0f);
		float fSlow151 = (iSlow144 ? fSlow146 : fSlow147);
		float fSlow152 = ((fSlow142 * (fSlow142 + fSlow151)) + 1.0f);
		float fSlow153 = ((fSlow142 * (fSlow142 - fSlow151)) + 1.0f);
		float fSlow154 = (1.0f / fSlow7);
		float fSlow155 = float(fHslider18);
		int iSlow156 = (fSlow155 > 0.0f);
		float fSlow157 = (float(fHslider19) * std::sin((fConst48 * fSlow6)));
		float fSlow158 = (fConst28 * ((fSlow6 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow155)))) / fSlow157));
		float fSlow159 = (fConst28 * (fSlow6 / fSlow157));
		float fSlow160 = (iSlow156 ? fSlow159 : fSlow158);
		float fSlow161 = ((fSlow154 * (fSlow154 - fSlow160)) + 1.0f);
		float fSlow162 = ((fSlow154 * (fSlow154 + fSlow160)) + 1.0f);
		float fSlow163 = (iSlow156 ? fSlow158 : fSlow159);
		float fSlow164 = ((fSlow154 * (fSlow154 + fSlow163)) + 1.0f);
		float fSlow165 = ((fSlow154 * (fSlow154 - fSlow163)) + 1.0f);
		float fSlow166 = (1.0f / fSlow4);
		float fSlow167 = (((fSlow3 - fSlow1) / fSlow2) + 1.0f);
		float fSlow168 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow2))));
		float fSlow169 = (1.0f / float(fHslider21));
		float fSlow170 = std::tan((fConst28 * float(fHslider22)));
		float fSlow171 = (1.0f / fSlow170);
		float fSlow172 = (((fSlow169 + fSlow171) / fSlow170) + 1.0f);
		float fSlow173 = ((std::pow(10.0f, (0.0500000007f * float(fHslider20))) * (1.0f - float(fCheckbox1))) / fSlow172);
		float fSlow174 = float(fHslider23);
		float fSlow175 = std::tan((fConst28 * fSlow174));
		float fSlow176 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow175))));
		float fSlow177 = float(fHslider24);
		float fSlow178 = std::tan((fConst28 * fSlow177));
		float fSlow179 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow178))));
		float fSlow180 = float(fHslider25);
		float fSlow181 = float((fSlow180 > 0.0f));
		float fSlow182 = (0.0f - fSlow180);
		float fSlow183 = (2.0f * float(fHslider26));
		float fSlow184 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider27)));
		float fSlow185 = (1.0f - fSlow184);
		float fSlow186 = (0.00999999978f * float(fHslider28));
		float fSlow187 = (fConst47 * ((1.0f / float(fHslider29)) + -1.0f));
		float fSlow188 = float(fHslider30);
		int iSlow189 = (std::fabs(fSlow188) < 1.1920929e-07f);
		float fSlow190 = (iSlow189 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow189 ? 1.0f : fSlow188)))));
		float fSlow191 = float(fHslider31);
		int iSlow192 = (std::fabs(fSlow191) < 1.1920929e-07f);
		float fSlow193 = (iSlow192 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow192 ? 1.0f : fSlow191)))));
		float fSlow194 = (1.0f - fSlow193);
		float fSlow195 = (1.0f - fSlow190);
		float fSlow196 = float(fHslider32);
		float fSlow197 = (1.0f / float(fHslider33));
		float fSlow198 = std::tan((fConst28 * float(fHslider34)));
		float fSlow199 = (1.0f / fSlow198);
		float fSlow200 = (1.0f / (((fSlow197 + fSlow199) / fSlow198) + 1.0f));
		float fSlow201 = (((fSlow199 - fSlow197) / fSlow198) + 1.0f);
		float fSlow202 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow198))));
		float fSlow203 = (1.0f / fSlow178);
		float fSlow204 = float(fHslider35);
		int iSlow205 = (fSlow204 > 0.0f);
		float fSlow206 = (float(fHslider36) * std::sin((fConst48 * fSlow177)));
		float fSlow207 = (fConst28 * ((fSlow177 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow204)))) / fSlow206));
		float fSlow208 = (fConst28 * (fSlow177 / fSlow206));
		float fSlow209 = (iSlow205 ? fSlow208 : fSlow207);
		float fSlow210 = ((fSlow203 * (fSlow203 - fSlow209)) + 1.0f);
		float fSlow211 = ((fSlow203 * (fSlow203 + fSlow209)) + 1.0f);
		float fSlow212 = (iSlow205 ? fSlow207 : fSlow208);
		float fSlow213 = ((fSlow203 * (fSlow203 + fSlow212)) + 1.0f);
		float fSlow214 = ((fSlow203 * (fSlow203 - fSlow212)) + 1.0f);
		float fSlow215 = (1.0f / fSlow175);
		float fSlow216 = float(fHslider37);
		int iSlow217 = (fSlow216 > 0.0f);
		float fSlow218 = (float(fHslider38) * std::sin((fConst48 * fSlow174)));
		float fSlow219 = (fConst28 * ((fSlow174 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow216)))) / fSlow218));
		float fSlow220 = (fConst28 * (fSlow174 / fSlow218));
		float fSlow221 = (iSlow217 ? fSlow220 : fSlow219);
		float fSlow222 = ((fSlow215 * (fSlow215 - fSlow221)) + 1.0f);
		float fSlow223 = ((fSlow215 * (fSlow215 + fSlow221)) + 1.0f);
		float fSlow224 = (iSlow217 ? fSlow219 : fSlow220);
		float fSlow225 = ((fSlow215 * (fSlow215 + fSlow224)) + 1.0f);
		float fSlow226 = ((fSlow215 * (fSlow215 - fSlow224)) + 1.0f);
		float fSlow227 = (1.0f / fSlow172);
		float fSlow228 = (((fSlow171 - fSlow169) / fSlow170) + 1.0f);
		float fSlow229 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow170))));
		float fSlow230 = (1.0f / float(fHslider40));
		float fSlow231 = std::tan((fConst28 * float(fHslider41)));
		float fSlow232 = (1.0f / fSlow231);
		float fSlow233 = (((fSlow230 + fSlow232) / fSlow231) + 1.0f);
		float fSlow234 = ((std::pow(10.0f, (0.0500000007f * float(fHslider39))) * (1.0f - float(fCheckbox2))) / fSlow233);
		float fSlow235 = float(fHslider42);
		float fSlow236 = std::tan((fConst28 * fSlow235));
		float fSlow237 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow236))));
		float fSlow238 = float(fHslider43);
		float fSlow239 = std::tan((fConst28 * fSlow238));
		float fSlow240 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow239))));
		float fSlow241 = std::min<float>(5.0f, std::max<float>(0.00999999978f, float(fHslider44)));
		float fSlow242 = (0.0125000002f * (std::pow(10.0f, (0.0500000007f * fSlow241)) + 1.0f));
		float fSlow243 = ((2.0f * fSlow241) + 1.0f);
		float fSlow244 = std::tan((fConst28 * ((1875.0f * fSlow243) + 500.0f)));
		float fSlow245 = (1.0f / fSlow244);
		float fSlow246 = (1.0f / (((fSlow245 + 1.41421354f) / fSlow244) + 1.0f));
		float fSlow247 = std::tan((fConst28 * ((62.5f * fSlow243) + 40.0f)));
		float fSlow248 = (1.0f / fSlow247);
		float fSlow249 = (1.0f / (((fSlow248 + 1.41421354f) / fSlow247) + 1.0f));
		float fSlow250 = mydsp_faustpower2_f(fSlow247);
		float fSlow251 = (1.0f / fSlow250);
		float fSlow252 = (((fSlow248 + -1.41421354f) / fSlow247) + 1.0f);
		float fSlow253 = (2.0f * (1.0f - fSlow251));
		float fSlow254 = (0.0f - (2.0f / fSlow250));
		float fSlow255 = (((fSlow245 + -1.41421354f) / fSlow244) + 1.0f);
		float fSlow256 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow244))));
		float fSlow257 = (1.0f / std::max<float>(1.0f, (fConst155 * fSlow243)));
		float fSlow258 = float(fHslider45);
		float fSlow259 = float((fSlow258 > 0.0f));
		float fSlow260 = (0.0f - fSlow258);
		float fSlow261 = (fConst47 * ((1.0f / float(fHslider46)) + -1.0f));
		float fSlow262 = float(fHslider47);
		int iSlow263 = (std::fabs(fSlow262) < 1.1920929e-07f);
		float fSlow264 = (iSlow263 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow263 ? 1.0f : fSlow262)))));
		float fSlow265 = float(fHslider48);
		int iSlow266 = (std::fabs(fSlow265) < 1.1920929e-07f);
		float fSlow267 = (iSlow266 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow266 ? 1.0f : fSlow265)))));
		float fSlow268 = (1.0f - fSlow267);
		float fSlow269 = (1.0f - fSlow264);
		float fSlow270 = float(fHslider49);
		float fSlow271 = (1.0f / float(fHslider50));
		float fSlow272 = std::tan((fConst28 * float(fHslider51)));
		float fSlow273 = (1.0f / fSlow272);
		float fSlow274 = (1.0f / (((fSlow271 + fSlow273) / fSlow272) + 1.0f));
		float fSlow275 = (((fSlow273 - fSlow271) / fSlow272) + 1.0f);
		float fSlow276 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow272))));
		float fSlow277 = (1.0f / fSlow239);
		float fSlow278 = float(fHslider52);
		int iSlow279 = (fSlow278 > 0.0f);
		float fSlow280 = (float(fHslider53) * std::sin((fConst48 * fSlow238)));
		float fSlow281 = (fConst28 * ((fSlow238 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow278)))) / fSlow280));
		float fSlow282 = (fConst28 * (fSlow238 / fSlow280));
		float fSlow283 = (iSlow279 ? fSlow282 : fSlow281);
		float fSlow284 = ((fSlow277 * (fSlow277 - fSlow283)) + 1.0f);
		float fSlow285 = ((fSlow277 * (fSlow277 + fSlow283)) + 1.0f);
		float fSlow286 = (iSlow279 ? fSlow281 : fSlow282);
		float fSlow287 = ((fSlow277 * (fSlow277 + fSlow286)) + 1.0f);
		float fSlow288 = ((fSlow277 * (fSlow277 - fSlow286)) + 1.0f);
		float fSlow289 = (1.0f / fSlow236);
		float fSlow290 = float(fHslider54);
		int iSlow291 = (fSlow290 > 0.0f);
		float fSlow292 = (float(fHslider55) * std::sin((fConst48 * fSlow235)));
		float fSlow293 = (fConst28 * ((fSlow235 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow290)))) / fSlow292));
		float fSlow294 = (fConst28 * (fSlow235 / fSlow292));
		float fSlow295 = (iSlow291 ? fSlow294 : fSlow293);
		float fSlow296 = ((fSlow289 * (fSlow289 - fSlow295)) + 1.0f);
		float fSlow297 = ((fSlow289 * (fSlow289 + fSlow295)) + 1.0f);
		float fSlow298 = (iSlow291 ? fSlow293 : fSlow294);
		float fSlow299 = ((fSlow289 * (fSlow289 + fSlow298)) + 1.0f);
		float fSlow300 = ((fSlow289 * (fSlow289 - fSlow298)) + 1.0f);
		float fSlow301 = (1.0f / fSlow233);
		float fSlow302 = (((fSlow232 - fSlow230) / fSlow231) + 1.0f);
		float fSlow303 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow231))));
		float fSlow304 = (1.0f / float(fHslider57));
		float fSlow305 = std::tan((fConst28 * float(fHslider58)));
		float fSlow306 = (1.0f / fSlow305);
		float fSlow307 = (((fSlow304 + fSlow306) / fSlow305) + 1.0f);
		float fSlow308 = ((std::pow(10.0f, (0.0500000007f * float(fHslider56))) * (1.0f - float(fCheckbox3))) / fSlow307);
		float fSlow309 = float(fHslider59);
		float fSlow310 = std::tan((fConst28 * fSlow309));
		float fSlow311 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow310))));
		float fSlow312 = float(fHslider60);
		float fSlow313 = std::tan((fConst28 * fSlow312));
		float fSlow314 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow313))));
		float fSlow315 = float(fHslider61);
		float fSlow316 = float((fSlow315 > 0.0f));
		float fSlow317 = (0.0f - fSlow315);
		float fSlow318 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider62)));
		float fSlow319 = ((fSlow318 * fSlow12) * ((0.0500000007f * fSlow18) + 1.0f));
		float fSlow320 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider63))) * fSlow12) * (1.0f - (0.0299999993f * fSlow18)));
		float fSlow321 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider64))) * fSlow12) * ((0.310000002f * fSlow18) + 1.0f));
		float fSlow322 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider65))) * fSlow12) * (1.0f - (0.270000011f * fSlow18)));
		float fSlow323 = (fConst47 * ((1.0f / float(fHslider66)) + -1.0f));
		float fSlow324 = float(fHslider67);
		int iSlow325 = (std::fabs(fSlow324) < 1.1920929e-07f);
		float fSlow326 = (iSlow325 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow325 ? 1.0f : fSlow324)))));
		float fSlow327 = float(fHslider68);
		int iSlow328 = (std::fabs(fSlow327) < 1.1920929e-07f);
		float fSlow329 = (iSlow328 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow328 ? 1.0f : fSlow327)))));
		float fSlow330 = (1.0f - fSlow329);
		float fSlow331 = (1.0f - fSlow326);
		float fSlow332 = float(fHslider69);
		float fSlow333 = (1.0f / float(fHslider70));
		float fSlow334 = std::tan((fConst28 * float(fHslider71)));
		float fSlow335 = (1.0f / fSlow334);
		float fSlow336 = (1.0f / (((fSlow333 + fSlow335) / fSlow334) + 1.0f));
		float fSlow337 = (((fSlow335 - fSlow333) / fSlow334) + 1.0f);
		float fSlow338 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow334))));
		float fSlow339 = (1.0f / fSlow313);
		float fSlow340 = float(fHslider72);
		int iSlow341 = (fSlow340 > 0.0f);
		float fSlow342 = (float(fHslider73) * std::sin((fConst48 * fSlow312)));
		float fSlow343 = (fConst28 * ((fSlow312 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow340)))) / fSlow342));
		float fSlow344 = (fConst28 * (fSlow312 / fSlow342));
		float fSlow345 = (iSlow341 ? fSlow344 : fSlow343);
		float fSlow346 = ((fSlow339 * (fSlow339 - fSlow345)) + 1.0f);
		float fSlow347 = ((fSlow339 * (fSlow339 + fSlow345)) + 1.0f);
		float fSlow348 = (iSlow341 ? fSlow343 : fSlow344);
		float fSlow349 = ((fSlow339 * (fSlow339 + fSlow348)) + 1.0f);
		float fSlow350 = ((fSlow339 * (fSlow339 - fSlow348)) + 1.0f);
		float fSlow351 = (1.0f / fSlow310);
		float fSlow352 = float(fHslider74);
		int iSlow353 = (fSlow352 > 0.0f);
		float fSlow354 = (float(fHslider75) * std::sin((fConst48 * fSlow309)));
		float fSlow355 = (fConst28 * ((fSlow309 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow352)))) / fSlow354));
		float fSlow356 = (fConst28 * (fSlow309 / fSlow354));
		float fSlow357 = (iSlow353 ? fSlow356 : fSlow355);
		float fSlow358 = ((fSlow351 * (fSlow351 - fSlow357)) + 1.0f);
		float fSlow359 = ((fSlow351 * (fSlow351 + fSlow357)) + 1.0f);
		float fSlow360 = (iSlow353 ? fSlow355 : fSlow356);
		float fSlow361 = ((fSlow351 * (fSlow351 + fSlow360)) + 1.0f);
		float fSlow362 = ((fSlow351 * (fSlow351 - fSlow360)) + 1.0f);
		float fSlow363 = (1.0f / fSlow307);
		float fSlow364 = (((fSlow306 - fSlow304) / fSlow305) + 1.0f);
		float fSlow365 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow305))));
		float fSlow366 = (1.0f / float(fHslider77));
		float fSlow367 = std::tan((fConst28 * float(fHslider78)));
		float fSlow368 = (1.0f / fSlow367);
		float fSlow369 = (((fSlow366 + fSlow368) / fSlow367) + 1.0f);
		float fSlow370 = ((std::pow(10.0f, (0.0500000007f * float(fHslider76))) * (1.0f - float(fCheckbox4))) / fSlow369);
		float fSlow371 = (0.300000012f * fSlow370);
		float fSlow372 = float(fHslider79);
		float fSlow373 = std::tan((fConst28 * fSlow372));
		float fSlow374 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow373))));
		float fSlow375 = float(fHslider80);
		float fSlow376 = std::tan((fConst28 * fSlow375));
		float fSlow377 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow376))));
		float fSlow378 = float(fHslider81);
		float fSlow379 = float((fSlow378 > 0.0f));
		float fSlow380 = (0.0f - fSlow378);
		float fSlow381 = (fConst47 * ((1.0f / float(fHslider82)) + -1.0f));
		float fSlow382 = float(fHslider83);
		int iSlow383 = (std::fabs(fSlow382) < 1.1920929e-07f);
		float fSlow384 = (iSlow383 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow383 ? 1.0f : fSlow382)))));
		float fSlow385 = float(fHslider84);
		int iSlow386 = (std::fabs(fSlow385) < 1.1920929e-07f);
		float fSlow387 = (iSlow386 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow386 ? 1.0f : fSlow385)))));
		float fSlow388 = (1.0f - fSlow387);
		float fSlow389 = (1.0f - fSlow384);
		float fSlow390 = float(fHslider85);
		float fSlow391 = (1.0f / float(fHslider86));
		float fSlow392 = std::tan((fConst28 * float(fHslider87)));
		float fSlow393 = (1.0f / fSlow392);
		float fSlow394 = (1.0f / (((fSlow391 + fSlow393) / fSlow392) + 1.0f));
		float fSlow395 = (((fSlow393 - fSlow391) / fSlow392) + 1.0f);
		float fSlow396 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow392))));
		float fSlow397 = (1.0f / fSlow376);
		float fSlow398 = float(fHslider88);
		int iSlow399 = (fSlow398 > 0.0f);
		float fSlow400 = (float(fHslider89) * std::sin((fConst48 * fSlow375)));
		float fSlow401 = (fConst28 * ((fSlow375 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow398)))) / fSlow400));
		float fSlow402 = (fConst28 * (fSlow375 / fSlow400));
		float fSlow403 = (iSlow399 ? fSlow402 : fSlow401);
		float fSlow404 = ((fSlow397 * (fSlow397 - fSlow403)) + 1.0f);
		float fSlow405 = ((fSlow397 * (fSlow397 + fSlow403)) + 1.0f);
		float fSlow406 = (iSlow399 ? fSlow401 : fSlow402);
		float fSlow407 = ((fSlow397 * (fSlow397 + fSlow406)) + 1.0f);
		float fSlow408 = ((fSlow397 * (fSlow397 - fSlow406)) + 1.0f);
		float fSlow409 = (1.0f / fSlow373);
		float fSlow410 = float(fHslider90);
		int iSlow411 = (fSlow410 > 0.0f);
		float fSlow412 = (float(fHslider91) * std::sin((fConst48 * fSlow372)));
		float fSlow413 = (fConst28 * ((fSlow372 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow410)))) / fSlow412));
		float fSlow414 = (fConst28 * (fSlow372 / fSlow412));
		float fSlow415 = (iSlow411 ? fSlow414 : fSlow413);
		float fSlow416 = ((fSlow409 * (fSlow409 - fSlow415)) + 1.0f);
		float fSlow417 = ((fSlow409 * (fSlow409 + fSlow415)) + 1.0f);
		float fSlow418 = (iSlow411 ? fSlow413 : fSlow414);
		float fSlow419 = ((fSlow409 * (fSlow409 + fSlow418)) + 1.0f);
		float fSlow420 = ((fSlow409 * (fSlow409 - fSlow418)) + 1.0f);
		float fSlow421 = (1.0f / fSlow369);
		float fSlow422 = (((fSlow368 - fSlow366) / fSlow367) + 1.0f);
		float fSlow423 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow367))));
		float fSlow424 = (1.0f / float(fHslider93));
		float fSlow425 = std::tan((fConst28 * float(fHslider94)));
		float fSlow426 = (1.0f / fSlow425);
		float fSlow427 = (((fSlow424 + fSlow426) / fSlow425) + 1.0f);
		float fSlow428 = ((std::pow(10.0f, (0.0500000007f * float(fHslider92))) * (1.0f - float(fCheckbox5))) / fSlow427);
		float fSlow429 = float(fHslider95);
		float fSlow430 = std::tan((fConst28 * fSlow429));
		float fSlow431 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow430))));
		float fSlow432 = float(fHslider96);
		float fSlow433 = std::tan((fConst28 * fSlow432));
		float fSlow434 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow433))));
		float fSlow435 = (0.5f * fSlow318);
		float fSlow436 = (fConst489 * fSlow12);
		float fSlow437 = (27.0f * fSlow12);
		float fSlow438 = (9.0f * fSlow12);
		float fSlow439 = (3.0f * fSlow12);
		float fSlow440 = (fConst47 * ((1.0f / float(fHslider97)) + -1.0f));
		float fSlow441 = float(fHslider98);
		int iSlow442 = (std::fabs(fSlow441) < 1.1920929e-07f);
		float fSlow443 = (iSlow442 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow442 ? 1.0f : fSlow441)))));
		float fSlow444 = float(fHslider99);
		int iSlow445 = (std::fabs(fSlow444) < 1.1920929e-07f);
		float fSlow446 = (iSlow445 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow445 ? 1.0f : fSlow444)))));
		float fSlow447 = (1.0f - fSlow446);
		float fSlow448 = (1.0f - fSlow443);
		float fSlow449 = float(fHslider100);
		float fSlow450 = (1.0f / float(fHslider101));
		float fSlow451 = std::tan((fConst28 * float(fHslider102)));
		float fSlow452 = (1.0f / fSlow451);
		float fSlow453 = (1.0f / (((fSlow450 + fSlow452) / fSlow451) + 1.0f));
		float fSlow454 = (((fSlow452 - fSlow450) / fSlow451) + 1.0f);
		float fSlow455 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow451))));
		float fSlow456 = (1.0f / fSlow433);
		float fSlow457 = float(fHslider103);
		int iSlow458 = (fSlow457 > 0.0f);
		float fSlow459 = (float(fHslider104) * std::sin((fConst48 * fSlow432)));
		float fSlow460 = (fConst28 * ((fSlow432 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow457)))) / fSlow459));
		float fSlow461 = (fConst28 * (fSlow432 / fSlow459));
		float fSlow462 = (iSlow458 ? fSlow461 : fSlow460);
		float fSlow463 = ((fSlow456 * (fSlow456 - fSlow462)) + 1.0f);
		float fSlow464 = ((fSlow456 * (fSlow456 + fSlow462)) + 1.0f);
		float fSlow465 = (iSlow458 ? fSlow460 : fSlow461);
		float fSlow466 = ((fSlow456 * (fSlow456 + fSlow465)) + 1.0f);
		float fSlow467 = ((fSlow456 * (fSlow456 - fSlow465)) + 1.0f);
		float fSlow468 = (1.0f / fSlow430);
		float fSlow469 = float(fHslider105);
		int iSlow470 = (fSlow469 > 0.0f);
		float fSlow471 = (float(fHslider106) * std::sin((fConst48 * fSlow429)));
		float fSlow472 = (fConst28 * ((fSlow429 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow469)))) / fSlow471));
		float fSlow473 = (fConst28 * (fSlow429 / fSlow471));
		float fSlow474 = (iSlow470 ? fSlow473 : fSlow472);
		float fSlow475 = ((fSlow468 * (fSlow468 - fSlow474)) + 1.0f);
		float fSlow476 = ((fSlow468 * (fSlow468 + fSlow474)) + 1.0f);
		float fSlow477 = (iSlow470 ? fSlow472 : fSlow473);
		float fSlow478 = ((fSlow468 * (fSlow468 + fSlow477)) + 1.0f);
		float fSlow479 = ((fSlow468 * (fSlow468 - fSlow477)) + 1.0f);
		float fSlow480 = (1.0f / fSlow427);
		float fSlow481 = (((fSlow426 - fSlow424) / fSlow425) + 1.0f);
		float fSlow482 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow425))));
		float fSlow483 = (1.0f / float(fHslider108));
		float fSlow484 = std::tan((fConst28 * float(fHslider109)));
		float fSlow485 = (1.0f / fSlow484);
		float fSlow486 = (((fSlow483 + fSlow485) / fSlow484) + 1.0f);
		float fSlow487 = ((std::pow(10.0f, (0.0500000007f * float(fHslider107))) * (1.0f - float(fCheckbox6))) / fSlow486);
		float fSlow488 = float(fHslider110);
		float fSlow489 = std::tan((fConst28 * fSlow488));
		float fSlow490 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow489))));
		float fSlow491 = float(fHslider111);
		float fSlow492 = std::tan((fConst28 * fSlow491));
		float fSlow493 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow492))));
		float fSlow494 = float(fHslider112);
		float fSlow495 = float((fSlow494 > 0.0f));
		float fSlow496 = (0.0f - fSlow494);
		float fSlow497 = float(fHslider113);
		float fSlow498 = (0.00100000005f * float((fSlow497 > 20.0f)));
		float fSlow499 = (fConst5 * std::max<float>(20.0f, fSlow497));
		float fSlow500 = float(fHslider114);
		float fSlow501 = (0.00100000005f * float((fSlow500 > 20.0f)));
		float fSlow502 = (fConst5 * std::max<float>(20.0f, fSlow500));
		float fSlow503 = (fConst47 * ((1.0f / float(fHslider115)) + -1.0f));
		float fSlow504 = float(fHslider116);
		int iSlow505 = (std::fabs(fSlow504) < 1.1920929e-07f);
		float fSlow506 = (iSlow505 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow505 ? 1.0f : fSlow504)))));
		float fSlow507 = float(fHslider117);
		int iSlow508 = (std::fabs(fSlow507) < 1.1920929e-07f);
		float fSlow509 = (iSlow508 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow508 ? 1.0f : fSlow507)))));
		float fSlow510 = (1.0f - fSlow509);
		float fSlow511 = (1.0f - fSlow506);
		float fSlow512 = float(fHslider118);
		float fSlow513 = (1.0f / float(fHslider119));
		float fSlow514 = std::tan((fConst28 * float(fHslider120)));
		float fSlow515 = (1.0f / fSlow514);
		float fSlow516 = (1.0f / (((fSlow513 + fSlow515) / fSlow514) + 1.0f));
		float fSlow517 = (((fSlow515 - fSlow513) / fSlow514) + 1.0f);
		float fSlow518 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow514))));
		float fSlow519 = (1.0f / fSlow492);
		float fSlow520 = float(fHslider121);
		int iSlow521 = (fSlow520 > 0.0f);
		float fSlow522 = (float(fHslider122) * std::sin((fConst48 * fSlow491)));
		float fSlow523 = (fConst28 * ((fSlow491 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow520)))) / fSlow522));
		float fSlow524 = (fConst28 * (fSlow491 / fSlow522));
		float fSlow525 = (iSlow521 ? fSlow524 : fSlow523);
		float fSlow526 = ((fSlow519 * (fSlow519 - fSlow525)) + 1.0f);
		float fSlow527 = ((fSlow519 * (fSlow519 + fSlow525)) + 1.0f);
		float fSlow528 = (iSlow521 ? fSlow523 : fSlow524);
		float fSlow529 = ((fSlow519 * (fSlow519 + fSlow528)) + 1.0f);
		float fSlow530 = ((fSlow519 * (fSlow519 - fSlow528)) + 1.0f);
		float fSlow531 = (1.0f / fSlow489);
		float fSlow532 = float(fHslider123);
		int iSlow533 = (fSlow532 > 0.0f);
		float fSlow534 = (float(fHslider124) * std::sin((fConst48 * fSlow488)));
		float fSlow535 = (fConst28 * ((fSlow488 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow532)))) / fSlow534));
		float fSlow536 = (fConst28 * (fSlow488 / fSlow534));
		float fSlow537 = (iSlow533 ? fSlow536 : fSlow535);
		float fSlow538 = ((fSlow531 * (fSlow531 - fSlow537)) + 1.0f);
		float fSlow539 = ((fSlow531 * (fSlow531 + fSlow537)) + 1.0f);
		float fSlow540 = (iSlow533 ? fSlow535 : fSlow536);
		float fSlow541 = ((fSlow531 * (fSlow531 + fSlow540)) + 1.0f);
		float fSlow542 = ((fSlow531 * (fSlow531 - fSlow540)) + 1.0f);
		float fSlow543 = (1.0f / fSlow486);
		float fSlow544 = (((fSlow485 - fSlow483) / fSlow484) + 1.0f);
		float fSlow545 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow484))));
		float fSlow546 = (0.949999988f * fSlow370);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec12[0] = (0.0f - (fConst24 * ((fConst25 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fConst20 * fRec11[1]) + (fConst21 * (fRec8[1] + (fConst22 * fRec12[0]))));
			fVec1[(IOTA & 32767)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fRec18[0] = (fConst33 + (fRec18[1] - std::floor((fConst33 + fRec18[1]))));
			float fTemp0 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec18[0]))]) + 1.0f);
			fVec2[0] = fSlow14;
			float fTemp1 = (fSlow14 - fVec2[1]);
			fVec3[0] = fSlow16;
			float fTemp2 = (0.0f - (fSlow14 + fVec3[1]));
			float fTemp3 = std::min<float>(1.0f, ((fTemp1 * float((fTemp1 > 0.0f))) + (fSlow15 * (fTemp2 * float((fTemp2 > 0.0f))))));
			fVec4[0] = fTemp3;
			int iTemp4 = int(fTemp3);
			fRec20[0] = (iTemp4 ? fSlow17 : fRec20[1]);
			fRec22[0] = (fConst36 + (fRec22[1] - std::floor((fConst36 + fRec22[1]))));
			fRec21[0] = ((0.999000013f * fRec21[1]) + (0.00100000005f * ((fSlow19 * ftbl0mydspSIG0[int((65536.0f * fRec22[0]))]) + 1.0f)));
			fRec19[0] = ((fConst34 * fRec19[1]) + (fConst35 * (std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow13 * (fRec20[0] * fRec21[0])))) * fTemp0)));
			float fTemp5 = ((0.00366666657f * (400.0f - fRec19[0])) + 3.0f);
			float fTemp6 = std::max<float>(1.1920929e-07f, std::fabs(fRec19[0]));
			float fTemp7 = (fRec24[1] + (fConst5 * fTemp6));
			float fTemp8 = (fTemp7 + -1.0f);
			int iTemp9 = (fTemp8 < 0.0f);
			fRec24[0] = (iTemp9 ? fTemp7 : fTemp8);
			float fRec25 = (iTemp9 ? fTemp7 : (fTemp7 + ((1.0f - (fConst0 / fTemp6)) * fTemp8)));
			float fTemp10 = (0.400000006f * ((2.0f * fRec25) + -1.0f));
			float fTemp11 = ((fSlow71 <= fRec19[0]) ? fRec19[0] : fSlow71);
			float fTemp12 = std::tan((fConst28 * fTemp11));
			float fTemp13 = (1.0f / fTemp12);
			float fTemp14 = (fSlow79 / fTemp11);
			float fTemp15 = (((fTemp13 + fTemp14) / fTemp12) + 1.0f);
			fRec23[0] = (fTemp10 - (((fRec23[2] * (((fTemp13 - fTemp14) / fTemp12) + 1.0f)) + (2.0f * (fRec23[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp12)))))) / fTemp15));
			float fTemp16 = (fTemp12 * fTemp15);
			float fTemp17 = ((2.0f * fRec19[0]) + 30.0f);
			float fTemp18 = ((iSlow81 & (fRec19[0] >= 200.0f)) ? (fSlow80 - (0.000952380942f * ((fRec19[0] + -200.0f) * fSlow82))) : ((fSlow80 <= fTemp17) ? fTemp17 : fSlow80));
			float fTemp19 = std::tan((fConst28 * fTemp18));
			float fTemp20 = (1.0f / fTemp19);
			float fTemp21 = (fSlow87 / fTemp18);
			float fTemp22 = (((fTemp20 + fTemp21) / fTemp19) + 1.0f);
			fRec26[0] = (fTemp10 - (((fRec26[2] * (((fTemp20 - fTemp21) / fTemp19) + 1.0f)) + (2.0f * (fRec26[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp19)))))) / fTemp22));
			float fTemp23 = (fTemp19 * fTemp22);
			fRec27[0] = (fTemp10 - (((fRec27[2] * fSlow100) + (2.0f * (fRec27[1] * fSlow101))) / fSlow102));
			fRec28[0] = (fTemp10 - (((fRec28[2] * fSlow111) + (2.0f * (fRec28[1] * fSlow112))) / fSlow113));
			fRec29[0] = (fTemp10 - (((fRec29[2] * fSlow121) + (2.0f * (fRec29[1] * fSlow122))) / fSlow123));
			iRec30[0] = (((iRec30[1] + (iRec30[1] > 0)) * (fTemp3 <= fVec4[1])) + (fTemp3 > fVec4[1]));
			float fTemp24 = float(iRec30[0]);
			fRec16[0] = (((fTemp0 * ((fTemp5 * (((((fRec23[0] / fTemp16) + ((fRec26[0] * fSlow88) / fTemp23)) + ((fRec27[0] * fSlow104) / fSlow105)) + ((fRec28[0] * fSlow115) / fSlow116)) + ((fRec29[0] * fSlow124) / fSlow125))) + (((((fRec23[2] * (0.0f - (fTemp5 / fTemp16))) + (fRec26[2] * (0.0f - ((fTemp5 * fSlow88) / fTemp23)))) + (fRec27[2] * (0.0f - ((fTemp5 * fSlow104) / fSlow105)))) + (fRec28[2] * (0.0f - ((fTemp5 * fSlow115) / fSlow116)))) + (fRec29[2] * (0.0f - ((fTemp5 * fSlow124) / fSlow125)))))) * std::sqrt(std::max<float>(0.0f, std::min<float>((fConst38 * fTemp24), ((fConst39 * (fConst37 - fTemp24)) + 1.0f))))) - (fConst40 * ((fConst41 * fRec16[2]) + (fConst42 * fRec16[1]))));
			iRec31[0] = (iTemp4 ? 7 : iRec31[1]);
			float fTemp25 = (((fRec16[2] + (fRec16[0] + (2.0f * fRec16[1]))) * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp24), ((fConst45 * (fConst43 - fTemp24)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec31[0] + -10)))));
			float fTemp26 = std::fabs((fConst32 * fTemp25));
			fRec34[0] = std::max<float>(fTemp26, ((fRec34[1] * fSlow132) + (fTemp26 * fSlow133)));
			fRec33[0] = ((fRec33[1] * fSlow129) + (fRec34[0] * fSlow134));
			fRec32[0] = ((fConst46 * fRec32[1]) + (fSlow126 * std::max<float>(((20.0f * std::log10(fRec33[0])) - fSlow135), 0.0f)));
			float fTemp27 = (fConst32 * (fTemp25 * std::pow(10.0f, (0.0500000007f * fRec32[0]))));
			fRec35[0] = (fTemp27 - (fSlow139 * ((fSlow140 * fRec35[2]) + (fSlow141 * fRec35[1]))));
			float fTemp28 = (fSlow11 * fRec15[1]);
			fRec15[0] = (fTemp27 - ((fSlow139 * (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])))) + (((fRec15[2] * fSlow149) + fTemp28) / fSlow150)));
			float fTemp29 = (fSlow8 * fRec14[1]);
			fRec14[0] = ((((fTemp28 + (fRec15[0] * fSlow152)) + (fRec15[2] * fSlow153)) / fSlow150) - (((fRec14[2] * fSlow161) + fTemp29) / fSlow162));
			fRec13[0] = ((((fTemp29 + (fRec14[0] * fSlow164)) + (fRec14[2] * fSlow165)) / fSlow162) - (fSlow166 * ((fSlow167 * fRec13[2]) + (fSlow168 * fRec13[1]))));
			float fTemp30 = (fSlow5 * (fRec13[2] + (fRec13[0] + (2.0f * fRec13[1]))));
			float fRec46 = (0.0f - (0.997305274f * ((0.899999976f * fRec47[2]) + (0.0500000007f * (fRec47[1] + fRec47[3])))));
			fVec5[0] = fSlow180;
			float fTemp31 = (fSlow180 - fVec5[1]);
			fVec6[0] = fSlow182;
			float fTemp32 = (0.0f - (fSlow180 + fVec6[1]));
			float fTemp33 = std::min<float>(1.0f, ((fTemp31 * float((fTemp31 > 0.0f))) + (fSlow181 * (fTemp32 * float((fTemp32 > 0.0f))))));
			fVec7[0] = fTemp33;
			int iTemp34 = int(fTemp33);
			fRec65[0] = (iTemp34 ? fSlow183 : fRec65[1]);
			float fTemp35 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow13 * (fRec21[0] * fRec65[0]))))) + -0.109999999f);
			float fTemp36 = (fConst50 * fTemp35);
			float fTemp37 = (fTemp36 + -1.49999499f);
			int iTemp38 = int(fTemp37);
			int iTemp39 = int(std::min<float>(fConst49, float(std::max<int>(0, iTemp38))));
			float fTemp40 = std::floor(fTemp37);
			float fTemp41 = (fTemp36 + (-1.0f - fTemp40));
			float fTemp42 = (0.0f - fTemp41);
			float fTemp43 = (fTemp36 + (-2.0f - fTemp40));
			float fTemp44 = (0.0f - (0.5f * fTemp43));
			float fTemp45 = (fTemp36 + (-3.0f - fTemp40));
			float fTemp46 = (0.0f - (0.333333343f * fTemp45));
			float fTemp47 = (fTemp36 + (-4.0f - fTemp40));
			float fTemp48 = (0.0f - (0.25f * fTemp47));
			float fTemp49 = (fTemp36 - fTemp40);
			int iTemp50 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp38 + 1)))));
			float fTemp51 = (0.0f - fTemp43);
			float fTemp52 = (0.0f - (0.5f * fTemp45));
			float fTemp53 = (0.0f - (0.333333343f * fTemp47));
			int iTemp54 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp38 + 2)))));
			float fTemp55 = (0.0f - fTemp45);
			float fTemp56 = (0.0f - (0.5f * fTemp47));
			float fTemp57 = (fTemp41 * fTemp43);
			int iTemp58 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp38 + 3)))));
			float fTemp59 = (0.0f - fTemp47);
			float fTemp60 = (fTemp57 * fTemp45);
			int iTemp61 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp38 + 4)))));
			fRec61[0] = (((((fRec42[((IOTA - (iTemp39 + 1)) & 2047)] * fTemp42) * fTemp44) * fTemp46) * fTemp48) + (fTemp49 * ((((((fRec42[((IOTA - (iTemp50 + 1)) & 2047)] * fTemp51) * fTemp52) * fTemp53) + (0.5f * (((fTemp41 * fRec42[((IOTA - (iTemp54 + 1)) & 2047)]) * fTemp55) * fTemp56))) + (0.166666672f * ((fTemp57 * fRec42[((IOTA - (iTemp58 + 1)) & 2047)]) * fTemp59))) + (0.0416666679f * (fTemp60 * fRec42[((IOTA - (iTemp61 + 1)) & 2047)])))));
			fRec66[0] = ((fSlow184 * fRec66[1]) + (fSlow185 * fRec61[1]));
			float fRec62 = fRec66[0];
			fRec68[0] = fRec40[1];
			fRec69[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec68[2]) + (0.0500000007f * (fRec68[1] + fRec68[3])))));
			float fTemp62 = (fConst51 * fTemp35);
			float fTemp63 = (fTemp62 + -1.49999499f);
			int iTemp64 = int(fTemp63);
			int iTemp65 = int(std::min<float>(fConst49, float(std::max<int>(0, iTemp64))));
			float fTemp66 = std::floor(fTemp63);
			float fTemp67 = (fTemp62 + (-1.0f - fTemp66));
			float fTemp68 = (0.0f - fTemp67);
			float fTemp69 = (fTemp62 + (-2.0f - fTemp66));
			float fTemp70 = (0.0f - (0.5f * fTemp69));
			float fTemp71 = (fTemp62 + (-3.0f - fTemp66));
			float fTemp72 = (0.0f - (0.333333343f * fTemp71));
			float fTemp73 = (fTemp62 + (-4.0f - fTemp66));
			float fTemp74 = (0.0f - (0.25f * fTemp73));
			float fTemp75 = (fTemp62 - fTemp66);
			int iTemp76 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp64 + 1)))));
			float fTemp77 = (0.0f - fTemp69);
			float fTemp78 = (0.0f - (0.5f * fTemp71));
			float fTemp79 = (0.0f - (0.333333343f * fTemp73));
			int iTemp80 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp64 + 2)))));
			float fTemp81 = (0.0f - fTemp71);
			float fTemp82 = (0.0f - (0.5f * fTemp73));
			float fTemp83 = (fTemp67 * fTemp69);
			int iTemp84 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp64 + 3)))));
			float fTemp85 = (0.0f - fTemp73);
			float fTemp86 = (fTemp83 * fTemp71);
			int iTemp87 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp64 + 4)))));
			fVec8[0] = (((((fRec69[((IOTA - (iTemp65 + 2)) & 1023)] * fTemp68) * fTemp70) * fTemp72) * fTemp74) + (fTemp75 * ((((((fRec69[((IOTA - (iTemp76 + 2)) & 1023)] * fTemp77) * fTemp78) * fTemp79) + (0.5f * (((fTemp67 * fRec69[((IOTA - (iTemp80 + 2)) & 1023)]) * fTemp81) * fTemp82))) + (0.166666672f * ((fTemp83 * fRec69[((IOTA - (iTemp84 + 2)) & 1023)]) * fTemp85))) + (0.0416666679f * (fTemp86 * fRec69[((IOTA - (iTemp87 + 2)) & 1023)])))));
			float fTemp88 = (fTemp33 + fVec8[1]);
			fVec9[0] = fTemp88;
			fRec67[(IOTA & 2047)] = ((fSlow184 * fRec67[((IOTA - 1) & 2047)]) + (fSlow185 * fVec9[2]));
			float fRec63 = (((((fTemp42 * fTemp44) * fTemp46) * fTemp48) * fRec67[((IOTA - iTemp39) & 2047)]) + (fTemp49 * ((((((fTemp51 * fTemp52) * fTemp53) * fRec67[((IOTA - iTemp50) & 2047)]) + (0.5f * (((fTemp41 * fTemp55) * fTemp56) * fRec67[((IOTA - iTemp54) & 2047)]))) + (0.166666672f * ((fTemp57 * fTemp59) * fRec67[((IOTA - iTemp58) & 2047)]))) + (0.0416666679f * (fTemp60 * fRec67[((IOTA - iTemp61) & 2047)])))));
			float fRec64 = (fVec9[1] + fRec57[1]);
			fRec57[0] = fRec62;
			float fRec58 = fRec57[1];
			float fRec59 = fRec63;
			float fRec60 = fRec64;
			fRec53[0] = fRec58;
			float fRec54 = (fTemp33 + fRec53[1]);
			float fRec55 = fRec59;
			float fRec56 = fRec60;
			fRec49[(IOTA & 1023)] = fRec54;
			float fRec50 = (((((fTemp68 * fTemp70) * fTemp72) * fTemp74) * fRec49[((IOTA - (iTemp65 + 1)) & 1023)]) + (fTemp75 * ((((((fTemp77 * fTemp78) * fTemp79) * fRec49[((IOTA - (iTemp76 + 1)) & 1023)]) + (0.5f * (((fTemp67 * fTemp81) * fTemp82) * fRec49[((IOTA - (iTemp80 + 1)) & 1023)]))) + (0.166666672f * ((fTemp83 * fTemp85) * fRec49[((IOTA - (iTemp84 + 1)) & 1023)]))) + (0.0416666679f * (fTemp86 * fRec49[((IOTA - (iTemp87 + 1)) & 1023)])))));
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
			fRec70[0] = (fSlow186 + (0.999000013f * fRec70[1]));
			float fTemp89 = std::min<float>(15000.0f, std::max<float>(500.0f, fRec70[0]));
			float fTemp90 = std::tan((fConst28 * fTemp89));
			float fTemp91 = (1.0f / fTemp90);
			float fTemp92 = (((fTemp91 + 1.42857146f) / fTemp90) + 1.0f);
			fRec39[0] = ((1.5f * fRec41) - (((fRec39[2] * (((fTemp91 + -1.42857146f) / fTemp90) + 1.0f)) + (2.0f * (fRec39[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp90)))))) / fTemp92));
			iRec71[0] = (((iRec71[1] + (iRec71[1] > 0)) * (fTemp33 <= fVec7[1])) + (fTemp33 > fVec7[1]));
			float fTemp93 = float(iRec71[0]);
			iRec72[0] = (iTemp34 ? 7 : iRec72[1]);
			float fTemp94 = ((((fRec39[2] + (fRec39[0] + (2.0f * fRec39[1]))) * ((6.66666674e-05f * (fTemp89 + -500.0f)) + 1.0f)) * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp93), ((fConst45 * (fConst43 - fTemp93)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * float((iRec72[0] + -10)))));
			float fTemp95 = std::fabs((0.25f * (fTemp94 / fTemp92)));
			fRec75[0] = std::max<float>(fTemp95, ((fRec75[1] * fSlow193) + (fTemp95 * fSlow194)));
			fRec74[0] = ((fRec74[1] * fSlow190) + (fRec75[0] * fSlow195));
			fRec73[0] = ((fConst46 * fRec73[1]) + (fSlow187 * std::max<float>(((20.0f * std::log10(fRec74[0])) - fSlow196), 0.0f)));
			float fTemp96 = (0.25f * ((fTemp94 * std::pow(10.0f, (0.0500000007f * fRec73[0]))) / fTemp92));
			fRec76[0] = (fTemp96 - (fSlow200 * ((fSlow201 * fRec76[2]) + (fSlow202 * fRec76[1]))));
			float fTemp97 = (fSlow179 * fRec38[1]);
			fRec38[0] = (fTemp96 - ((fSlow200 * (fRec76[2] + (fRec76[0] + (2.0f * fRec76[1])))) + (((fRec38[2] * fSlow210) + fTemp97) / fSlow211)));
			float fTemp98 = (fSlow176 * fRec37[1]);
			fRec37[0] = ((((fTemp97 + (fRec38[0] * fSlow213)) + (fRec38[2] * fSlow214)) / fSlow211) - (((fRec37[2] * fSlow222) + fTemp98) / fSlow223));
			fRec36[0] = ((((fTemp98 + (fRec37[0] * fSlow225)) + (fRec37[2] * fSlow226)) / fSlow223) - (fSlow227 * ((fSlow228 * fRec36[2]) + (fSlow229 * fRec36[1]))));
			float fTemp99 = (fSlow173 * (fRec36[2] + (fRec36[0] + (2.0f * fRec36[1]))));
			fVec10[(IOTA & 2047)] = (fTemp30 + fTemp99);
			float fTemp100 = (0.300000012f * fVec10[((IOTA - iConst52) & 2047)]);
			float fTemp101 = (((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst27) & 32767)]) - fTemp100);
			fVec11[(IOTA & 2047)] = fTemp101;
			fRec9[0] = fVec11[((IOTA - iConst53) & 2047)];
			float fRec10 = (0.0f - (0.600000024f * fTemp101));
			fRec80[0] = (0.0f - (fConst24 * ((fConst25 * fRec80[1]) - (fRec4[1] + fRec4[2]))));
			fRec79[0] = ((fConst62 * fRec79[1]) + (fConst63 * (fRec4[1] + (fConst64 * fRec80[0]))));
			fVec12[(IOTA & 32767)] = ((0.353553385f * fRec79[0]) + 9.99999968e-21f);
			float fTemp102 = (((0.600000024f * fRec77[1]) + fVec12[((IOTA - iConst66) & 32767)]) - fTemp100);
			fVec13[(IOTA & 4095)] = fTemp102;
			fRec77[0] = fVec13[((IOTA - iConst67) & 4095)];
			float fRec78 = (0.0f - (0.600000024f * fTemp102));
			fRec84[0] = (0.0f - (fConst24 * ((fConst25 * fRec84[1]) - (fRec6[1] + fRec6[2]))));
			fRec83[0] = ((fConst76 * fRec83[1]) + (fConst77 * (fRec6[1] + (fConst78 * fRec84[0]))));
			fVec14[(IOTA & 16383)] = ((0.353553385f * fRec83[0]) + 9.99999968e-21f);
			float fTemp103 = (fVec14[((IOTA - iConst80) & 16383)] + (fTemp100 + (0.600000024f * fRec81[1])));
			fVec15[(IOTA & 4095)] = fTemp103;
			fRec81[0] = fVec15[((IOTA - iConst81) & 4095)];
			float fRec82 = (0.0f - (0.600000024f * fTemp103));
			fRec88[0] = (0.0f - (fConst24 * ((fConst25 * fRec88[1]) - (fRec2[1] + fRec2[2]))));
			fRec87[0] = ((fConst90 * fRec87[1]) + (fConst91 * (fRec2[1] + (fConst92 * fRec88[0]))));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec87[0]) + 9.99999968e-21f);
			float fTemp104 = (fVec16[((IOTA - iConst94) & 32767)] + (fTemp100 + (0.600000024f * fRec85[1])));
			fVec17[(IOTA & 4095)] = fTemp104;
			fRec85[0] = fVec17[((IOTA - iConst95) & 4095)];
			float fRec86 = (0.0f - (0.600000024f * fTemp104));
			fRec92[0] = (0.0f - (fConst24 * ((fConst25 * fRec92[1]) - (fRec7[1] + fRec7[2]))));
			fRec91[0] = ((fConst104 * fRec91[1]) + (fConst105 * (fRec7[1] + (fConst106 * fRec92[0]))));
			fVec18[(IOTA & 16383)] = ((0.353553385f * fRec91[0]) + 9.99999968e-21f);
			float fTemp105 = (fVec18[((IOTA - iConst108) & 16383)] - (fTemp100 + (0.600000024f * fRec89[1])));
			fVec19[(IOTA & 2047)] = fTemp105;
			fRec89[0] = fVec19[((IOTA - iConst109) & 2047)];
			float fRec90 = (0.600000024f * fTemp105);
			fRec96[0] = (0.0f - (fConst24 * ((fConst25 * fRec96[1]) - (fRec3[1] + fRec3[2]))));
			fRec95[0] = ((fConst118 * fRec95[1]) + (fConst119 * (fRec3[1] + (fConst120 * fRec96[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec95[0]) + 9.99999968e-21f);
			float fTemp106 = (fVec20[((IOTA - iConst122) & 16383)] - (fTemp100 + (0.600000024f * fRec93[1])));
			fVec21[(IOTA & 4095)] = fTemp106;
			fRec93[0] = fVec21[((IOTA - iConst123) & 4095)];
			float fRec94 = (0.600000024f * fTemp106);
			fRec100[0] = (0.0f - (fConst24 * ((fConst25 * fRec100[1]) - (fRec5[1] + fRec5[2]))));
			fRec99[0] = ((fConst132 * fRec99[1]) + (fConst133 * (fRec5[1] + (fConst134 * fRec100[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec99[0]) + 9.99999968e-21f);
			float fTemp107 = ((fTemp100 + fVec22[((IOTA - iConst136) & 16383)]) - (0.600000024f * fRec97[1]));
			fVec23[(IOTA & 4095)] = fTemp107;
			fRec97[0] = fVec23[((IOTA - iConst137) & 4095)];
			float fRec98 = (0.600000024f * fTemp107);
			fRec104[0] = (0.0f - (fConst24 * ((fConst25 * fRec104[1]) - (fRec1[1] + fRec1[2]))));
			fRec103[0] = ((fConst146 * fRec103[1]) + (fConst147 * (fRec1[1] + (fConst148 * fRec104[0]))));
			fVec24[(IOTA & 16383)] = ((0.353553385f * fRec103[0]) + 9.99999968e-21f);
			float fTemp108 = ((fVec24[((IOTA - iConst150) & 16383)] + fTemp100) - (0.600000024f * fRec101[1]));
			fVec25[(IOTA & 2047)] = fTemp108;
			fRec101[0] = fVec25[((IOTA - iConst151) & 2047)];
			float fRec102 = (0.600000024f * fTemp108);
			float fTemp109 = (fRec102 + fRec98);
			float fTemp110 = (fRec90 + (fRec94 + fTemp109));
			fRec1[0] = (fRec9[1] + (fRec77[1] + (fRec81[1] + (fRec85[1] + (fRec89[1] + (fRec93[1] + (fRec97[1] + (fRec101[1] + (fRec10 + (fRec78 + (fRec82 + (fRec86 + fTemp110))))))))))));
			fRec2[0] = ((fRec89[1] + (fRec93[1] + (fRec97[1] + (fRec101[1] + fTemp110)))) - (fRec9[1] + (fRec77[1] + (fRec81[1] + (fRec85[1] + (fRec10 + (fRec78 + (fRec86 + fRec82))))))));
			float fTemp111 = (fRec94 + fRec90);
			fRec3[0] = ((fRec81[1] + (fRec85[1] + (fRec97[1] + (fRec101[1] + (fRec82 + (fRec86 + fTemp109)))))) - (fRec9[1] + (fRec77[1] + (fRec89[1] + (fRec93[1] + (fRec10 + (fRec78 + fTemp111)))))));
			fRec4[0] = ((fRec9[1] + (fRec77[1] + (fRec97[1] + (fRec101[1] + (fRec10 + (fRec78 + fTemp109)))))) - (fRec81[1] + (fRec85[1] + (fRec89[1] + (fRec93[1] + (fRec82 + (fRec86 + fTemp111)))))));
			float fTemp112 = (fRec102 + fRec94);
			float fTemp113 = (fRec98 + fRec90);
			fRec5[0] = ((fRec77[1] + (fRec85[1] + (fRec93[1] + (fRec101[1] + (fRec78 + (fRec86 + fTemp112)))))) - (fRec9[1] + (fRec81[1] + (fRec89[1] + (fRec97[1] + (fRec10 + (fRec82 + fTemp113)))))));
			fRec6[0] = ((fRec9[1] + (fRec81[1] + (fRec93[1] + (fRec101[1] + (fRec10 + (fRec82 + fTemp112)))))) - (fRec77[1] + (fRec85[1] + (fRec89[1] + (fRec97[1] + (fRec78 + (fRec86 + fTemp113)))))));
			float fTemp114 = (fRec102 + fRec90);
			float fTemp115 = (fRec98 + fRec94);
			fRec7[0] = ((fRec9[1] + (fRec85[1] + (fRec89[1] + (fRec101[1] + (fRec10 + (fRec86 + fTemp114)))))) - (fRec77[1] + (fRec81[1] + (fRec93[1] + (fRec97[1] + (fRec78 + (fRec82 + fTemp115)))))));
			fRec8[0] = ((fRec77[1] + (fRec81[1] + (fRec89[1] + (fRec101[1] + (fRec78 + (fRec82 + fTemp114)))))) - (fRec9[1] + (fRec85[1] + (fRec93[1] + (fRec97[1] + (fRec10 + (fRec86 + fTemp115)))))));
			fRec0[0] = ((0.370000005f * (fRec2[0] + fRec3[0])) - (fConst152 * ((fConst153 * fRec0[1]) + (fConst154 * fRec0[2]))));
			iRec111[0] = ((1103515245 * iRec111[1]) + 12345);
			float fTemp116 = (4.65661287e-10f * float(iRec111[0]));
			fRec110[0] = (fTemp116 - (fSlow249 * ((fSlow252 * fRec110[2]) + (fSlow253 * fRec110[1]))));
			fRec109[0] = ((fSlow249 * (((fSlow251 * fRec110[0]) + (fSlow254 * fRec110[1])) + (fSlow251 * fRec110[2]))) - (fSlow246 * ((fSlow255 * fRec109[2]) + (fSlow256 * fRec109[1]))));
			fVec26[0] = fSlow258;
			float fTemp117 = (fSlow258 - fVec26[1]);
			fVec27[0] = fSlow260;
			float fTemp118 = (0.0f - (fSlow258 + fVec27[1]));
			float fTemp119 = std::min<float>(1.0f, ((fTemp117 * float((fTemp117 > 0.0f))) + (fSlow259 * (fTemp118 * float((fTemp118 > 0.0f))))));
			fVec28[0] = fTemp119;
			iRec112[0] = (((iRec112[1] + (iRec112[1] > 0)) * (fTemp119 <= fVec28[1])) + (fTemp119 > fVec28[1]));
			float fTemp120 = (fSlow257 * float(iRec112[0]));
			float fTemp121 = (fSlow246 * ((fRec109[2] + (fRec109[0] + (2.0f * fRec109[1]))) * std::max<float>(0.0f, std::min<float>(fTemp120, (2.0f - fTemp120)))));
			fRec113[0] = (int(fTemp119) ? fSlow258 : fRec113[1]);
			fRec108[0] = (fTemp121 - ((fConst157 * (fRec108[1] * std::cos((fConst48 * (fRec113[0] + 50.0f))))) + (fConst158 * fRec108[2])));
			fRec114[0] = (fTemp121 - ((fConst160 * (fRec114[1] * std::cos((fConst48 * (fRec113[0] + 250.0f))))) + (fConst161 * fRec114[2])));
			fRec115[0] = (fTemp121 - ((fConst163 * (fRec115[1] * std::cos((fConst48 * (fRec113[0] + 450.0f))))) + (fConst164 * fRec115[2])));
			fRec116[0] = (fTemp121 - ((fConst166 * (fRec116[1] * std::cos((fConst48 * (fRec113[0] + 650.0f))))) + (fConst167 * fRec116[2])));
			fRec117[0] = (fTemp121 - ((fConst169 * (fRec117[1] * std::cos((fConst48 * (fRec113[0] + 850.0f))))) + (fConst170 * fRec117[2])));
			fRec118[0] = (fTemp121 - ((fConst172 * (fRec118[1] * std::cos((fConst48 * (fRec113[0] + 1050.0f))))) + (fConst173 * fRec118[2])));
			fRec119[0] = (fTemp121 - ((fConst175 * (fRec119[1] * std::cos((fConst48 * (fRec113[0] + 1250.0f))))) + (fConst176 * fRec119[2])));
			fRec120[0] = (fTemp121 - ((fConst178 * (fRec120[1] * std::cos((fConst48 * (fRec113[0] + 1450.0f))))) + (fConst179 * fRec120[2])));
			fRec121[0] = (fTemp121 - ((fConst181 * (fRec121[1] * std::cos((fConst48 * (fRec113[0] + 1650.0f))))) + (fConst182 * fRec121[2])));
			fRec122[0] = (fTemp121 - ((fConst184 * (fRec122[1] * std::cos((fConst48 * (fRec113[0] + 1850.0f))))) + (fConst185 * fRec122[2])));
			fRec123[0] = (fTemp121 - ((fConst187 * (fRec123[1] * std::cos((fConst48 * (fRec113[0] + 2050.0f))))) + (fConst188 * fRec123[2])));
			fRec124[0] = (fTemp121 - ((fConst190 * (fRec124[1] * std::cos((fConst48 * (fRec113[0] + 2250.0f))))) + (fConst191 * fRec124[2])));
			fRec125[0] = (fTemp121 - ((fConst193 * (fRec125[1] * std::cos((fConst48 * (fRec113[0] + 2450.0f))))) + (fConst194 * fRec125[2])));
			fRec126[0] = (fTemp121 - ((fConst196 * (fRec126[1] * std::cos((fConst48 * (fRec113[0] + 2650.0f))))) + (fConst197 * fRec126[2])));
			fRec127[0] = (fTemp121 - ((fConst199 * (fRec127[1] * std::cos((fConst48 * (fRec113[0] + 2850.0f))))) + (fConst200 * fRec127[2])));
			fRec128[0] = (fTemp121 - ((fConst202 * (fRec128[1] * std::cos((fConst48 * (fRec113[0] + 3050.0f))))) + (fConst203 * fRec128[2])));
			fRec129[0] = (fTemp121 - ((fConst205 * (fRec129[1] * std::cos((fConst48 * (fRec113[0] + 3250.0f))))) + (fConst206 * fRec129[2])));
			fRec130[0] = (fTemp121 - ((fConst208 * (fRec130[1] * std::cos((fConst48 * (fRec113[0] + 3450.0f))))) + (fConst209 * fRec130[2])));
			fRec131[0] = (fTemp121 - ((fConst211 * (fRec131[1] * std::cos((fConst48 * (fRec113[0] + 3650.0f))))) + (fConst212 * fRec131[2])));
			fRec132[0] = (fTemp121 - ((fConst214 * (fRec132[1] * std::cos((fConst48 * (fRec113[0] + 3850.0f))))) + (fConst215 * fRec132[2])));
			float fTemp122 = ((((((((((((((((((((fRec108[0] + (0.25f * (fRec114[0] - fRec114[2]))) + (0.111111112f * (fRec115[0] - fRec115[2]))) + (0.0625f * (fRec116[0] - fRec116[2]))) + (0.0399999991f * (fRec117[0] - fRec117[2]))) + (0.027777778f * (fRec118[0] - fRec118[2]))) + (0.0204081628f * (fRec119[0] - fRec119[2]))) + (0.015625f * (fRec120[0] - fRec120[2]))) + (0.0123456791f * (fRec121[0] - fRec121[2]))) + (0.00999999978f * (fRec122[0] - fRec122[2]))) + (0.00826446246f * (fRec123[0] - fRec123[2]))) + (0.0069444445f * (fRec124[0] - fRec124[2]))) + (0.00591715984f * (fRec125[0] - fRec125[2]))) + (0.00510204071f * (fRec126[0] - fRec126[2]))) + (0.00444444455f * (fRec127[0] - fRec127[2]))) + (0.00390625f * (fRec128[0] - fRec128[2]))) + (0.00346020772f * (fRec129[0] - fRec129[2]))) + (0.00308641978f * (fRec130[0] - fRec130[2]))) + (0.00277008303f * (fRec131[0] - fRec131[2]))) + (0.00249999994f * (fRec132[0] - fRec132[2]))) - fRec108[2]);
			float fTemp123 = std::fabs((fSlow242 * fTemp122));
			fRec135[0] = std::max<float>(fTemp123, ((fRec135[1] * fSlow267) + (fTemp123 * fSlow268)));
			fRec134[0] = ((fRec134[1] * fSlow264) + (fRec135[0] * fSlow269));
			fRec133[0] = ((fConst46 * fRec133[1]) + (fSlow261 * std::max<float>(((20.0f * std::log10(fRec134[0])) - fSlow270), 0.0f)));
			float fTemp124 = (fSlow242 * (fTemp122 * std::pow(10.0f, (0.0500000007f * fRec133[0]))));
			fRec136[0] = (fTemp124 - (fSlow274 * ((fSlow275 * fRec136[2]) + (fSlow276 * fRec136[1]))));
			float fTemp125 = (fSlow240 * fRec107[1]);
			fRec107[0] = (fTemp124 - ((fSlow274 * (fRec136[2] + (fRec136[0] + (2.0f * fRec136[1])))) + (((fRec107[2] * fSlow284) + fTemp125) / fSlow285)));
			float fTemp126 = (fSlow237 * fRec106[1]);
			fRec106[0] = ((((fTemp125 + (fRec107[0] * fSlow287)) + (fRec107[2] * fSlow288)) / fSlow285) - (((fRec106[2] * fSlow296) + fTemp126) / fSlow297));
			fRec105[0] = ((((fTemp126 + (fRec106[0] * fSlow299)) + (fRec106[2] * fSlow300)) / fSlow297) - (fSlow301 * ((fSlow302 * fRec105[2]) + (fSlow303 * fRec105[1]))));
			float fTemp127 = ((fSlow234 * (fRec105[2] + (fRec105[0] + (2.0f * fRec105[1])))) + fTemp30);
			fVec29[0] = fSlow315;
			float fTemp128 = (fSlow315 - fVec29[1]);
			fVec30[0] = fSlow317;
			float fTemp129 = (0.0f - (fSlow315 + fVec30[1]));
			float fTemp130 = std::min<float>(1.0f, ((fTemp128 * float((fTemp128 > 0.0f))) + (fSlow316 * (fTemp129 * float((fTemp129 > 0.0f))))));
			fVec31[0] = fTemp130;
			iRec141[0] = (((iRec141[1] + (iRec141[1] > 0)) * (fTemp130 <= fVec31[1])) + (fTemp130 > fVec31[1]));
			float fTemp131 = float(iRec141[0]);
			float fTemp132 = (fConst44 * fTemp131);
			float fTemp133 = (fConst43 - fTemp131);
			fRec140[0] = ((fConst216 * fRec140[1]) + (fConst217 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp132, ((fConst45 * fTemp133) + 1.0f))))));
			int iTemp134 = int(fTemp130);
			fRec144[0] = (iTemp134 ? fSlow319 : fRec144[1]);
			iRec143[0] = ((iVec0[1] + iRec143[1]) % int((fConst0 / fRec144[0])));
			float fTemp135 = float(iRec143[0]);
			int iTemp136 = (fTemp130 > 0.0f);
			float fTemp137 = (0.144717798f * (iTemp136 ? 0.00100000005f : 1.60000002f));
			int iTemp138 = (std::fabs(fTemp137) < 1.1920929e-07f);
			float fTemp139 = (iTemp138 ? 0.0f : std::exp((0.0f - (fConst5 / (iTemp138 ? 1.0f : fTemp137)))));
			fRec146[0] = ((fRec146[1] * fTemp139) + ((iTemp136 ? fTemp130 : 0.400000006f) * (1.0f - fTemp139)));
			fRec145[0] = ((fConst221 * fRec145[1]) + (fConst222 * fRec146[0]));
			float fTemp140 = (fRec145[0] + 0.00999999978f);
			float fTemp141 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec144[0] * fTemp140) * (1.0f - (0.000500000024f * std::min<float>(fRec144[0], 1000.0f)))))))));
			float fTemp142 = (1.0f / fTemp141);
			float fTemp143 = (((fTemp142 + 1.41421354f) / fTemp141) + 1.0f);
			fRec142[0] = ((2.0f * (((float((fTemp135 < (fConst218 / fRec144[0]))) + -0.5f) + (float((fTemp135 < (fConst219 / fRec144[0]))) + -0.5f)) + (float((fTemp135 < (fConst220 / fRec144[0]))) + -0.5f))) - (((fRec142[2] * (((fTemp142 + -1.41421354f) / fTemp141) + 1.0f)) + (2.0f * (fRec142[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp141)))))) / fTemp143));
			fRec149[0] = (iTemp134 ? fSlow320 : fRec149[1]);
			iRec148[0] = ((iVec0[1] + iRec148[1]) % int((fConst0 / fRec149[0])));
			float fTemp144 = float(iRec148[0]);
			float fTemp145 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fRec149[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec149[0], 1000.0f)))))))));
			float fTemp146 = (1.0f / fTemp145);
			float fTemp147 = (((fTemp146 + 1.41421354f) / fTemp145) + 1.0f);
			fRec147[0] = ((2.0f * (((float((fTemp144 < (fConst218 / fRec149[0]))) + -0.5f) + (float((fTemp144 < (fConst219 / fRec149[0]))) + -0.5f)) + (float((fTemp144 < (fConst220 / fRec149[0]))) + -0.5f))) - (((fRec147[2] * (((fTemp146 + -1.41421354f) / fTemp145) + 1.0f)) + (2.0f * (fRec147[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp145)))))) / fTemp147));
			fRec152[0] = (iTemp134 ? fSlow321 : fRec152[1]);
			iRec151[0] = ((iVec0[1] + iRec151[1]) % int((fConst0 / fRec152[0])));
			float fTemp148 = float(iRec151[0]);
			float fTemp149 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fRec152[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec152[0], 1000.0f)))))))));
			float fTemp150 = (1.0f / fTemp149);
			float fTemp151 = (((fTemp150 + 1.41421354f) / fTemp149) + 1.0f);
			fRec150[0] = ((2.0f * (((float((fTemp148 < (fConst218 / fRec152[0]))) + -0.5f) + (float((fTemp148 < (fConst219 / fRec152[0]))) + -0.5f)) + (float((fTemp148 < (fConst220 / fRec152[0]))) + -0.5f))) - (((fRec150[2] * (((fTemp150 + -1.41421354f) / fTemp149) + 1.0f)) + (2.0f * (fRec150[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp149)))))) / fTemp151));
			fRec155[0] = (iTemp134 ? fSlow322 : fRec155[1]);
			iRec154[0] = ((iVec0[1] + iRec154[1]) % int((fConst0 / fRec155[0])));
			float fTemp152 = float(iRec154[0]);
			float fTemp153 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fRec155[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec155[0], 1000.0f)))))))));
			float fTemp154 = (1.0f / fTemp153);
			float fTemp155 = (((fTemp154 + 1.41421354f) / fTemp153) + 1.0f);
			fRec153[0] = ((2.0f * (((float((fTemp152 < (fConst218 / fRec155[0]))) + -0.5f) + (float((fTemp152 < (fConst219 / fRec155[0]))) + -0.5f)) + (float((fTemp152 < (fConst220 / fRec155[0]))) + -0.5f))) - (((fRec153[2] * (((fTemp154 + -1.41421354f) / fTemp153) + 1.0f)) + (2.0f * (fRec153[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp153)))))) / fTemp155));
			float fTemp156 = (fRec140[0] * (((((fRec142[2] + (fRec142[0] + (2.0f * fRec142[1]))) / fTemp143) + ((fRec147[2] + (fRec147[0] + (2.0f * fRec147[1]))) / fTemp147)) + ((fRec150[2] + (fRec150[0] + (2.0f * fRec150[1]))) / fTemp151)) + ((fRec153[2] + (fRec153[0] + (2.0f * fRec153[1]))) / fTemp155)));
			float fTemp157 = std::fabs((0.25f * fTemp156));
			fRec158[0] = std::max<float>(fTemp157, ((fRec158[1] * fSlow329) + (fTemp157 * fSlow330)));
			fRec157[0] = ((fRec157[1] * fSlow326) + (fRec158[0] * fSlow331));
			fRec156[0] = ((fConst46 * fRec156[1]) + (fSlow323 * std::max<float>(((20.0f * std::log10(fRec157[0])) - fSlow332), 0.0f)));
			float fTemp158 = (0.25f * (fTemp156 * std::pow(10.0f, (0.0500000007f * fRec156[0]))));
			fRec159[0] = (fTemp158 - (fSlow336 * ((fSlow337 * fRec159[2]) + (fSlow338 * fRec159[1]))));
			float fTemp159 = (fSlow314 * fRec139[1]);
			fRec139[0] = (fTemp158 - ((fSlow336 * (fRec159[2] + (fRec159[0] + (2.0f * fRec159[1])))) + (((fRec139[2] * fSlow346) + fTemp159) / fSlow347)));
			float fTemp160 = (fSlow311 * fRec138[1]);
			fRec138[0] = ((((fTemp159 + (fRec139[0] * fSlow349)) + (fRec139[2] * fSlow350)) / fSlow347) - (((fRec138[2] * fSlow358) + fTemp160) / fSlow359));
			fRec137[0] = ((((fTemp160 + (fRec138[0] * fSlow361)) + (fRec138[2] * fSlow362)) / fSlow359) - (fSlow363 * ((fSlow364 * fRec137[2]) + (fSlow365 * fRec137[1]))));
			iRec163[0] = 0;
			fRec167[0] = ((0.949999988f * fRec167[1]) + (0.0500000007f * fRec166[1]));
			float fTemp161 = ((0.99000001f * fRec167[0]) + float(iRec163[1]));
			fVec32[(IOTA & 2047)] = fTemp161;
			fVec33[0] = fSlow378;
			float fTemp162 = (fSlow378 - fVec33[1]);
			fVec34[0] = fSlow380;
			float fTemp163 = (0.0f - (fSlow378 + fVec34[1]));
			float fTemp164 = std::min<float>(1.0f, ((fTemp162 * float((fTemp162 > 0.0f))) + (fSlow379 * (fTemp163 * float((fTemp163 > 0.0f))))));
			fVec35[0] = fTemp164;
			fRec168[0] = (int(fTemp164) ? fSlow378 : fRec168[1]);
			float fTemp165 = (fRec168[0] + 100.0f);
			float fTemp166 = (fConst223 * ((340.0f / fTemp165) + -0.0399999991f));
			float fTemp167 = (fTemp166 + -1.49999499f);
			int iTemp168 = int(fTemp167);
			int iTemp169 = int(std::min<float>(fConst49, float(std::max<int>(0, iTemp168))));
			float fTemp170 = std::floor(fTemp167);
			float fTemp171 = (fTemp166 + (-1.0f - fTemp170));
			float fTemp172 = (0.0f - fTemp171);
			float fTemp173 = (fTemp166 + (-2.0f - fTemp170));
			float fTemp174 = (0.0f - (0.5f * fTemp173));
			float fTemp175 = (fTemp166 + (-3.0f - fTemp170));
			float fTemp176 = (0.0f - (0.333333343f * fTemp175));
			float fTemp177 = (fTemp166 + (-4.0f - fTemp170));
			float fTemp178 = (0.0f - (0.25f * fTemp177));
			float fTemp179 = (fTemp166 - fTemp170);
			int iTemp180 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp168 + 1)))));
			float fTemp181 = (0.0f - fTemp173);
			float fTemp182 = (0.0f - (0.5f * fTemp175));
			float fTemp183 = (0.0f - (0.333333343f * fTemp177));
			int iTemp184 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp168 + 2)))));
			float fTemp185 = (0.0f - fTemp175);
			float fTemp186 = (0.0f - (0.5f * fTemp177));
			float fTemp187 = (fTemp171 * fTemp173);
			int iTemp188 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp168 + 3)))));
			float fTemp189 = (0.0f - fTemp177);
			float fTemp190 = (fTemp187 * fTemp175);
			int iTemp191 = int(std::min<float>(fConst49, float(std::max<int>(0, (iTemp168 + 4)))));
			fRec165[0] = (((((fVec32[((IOTA - iTemp169) & 2047)] * fTemp172) * fTemp174) * fTemp176) * fTemp178) + (fTemp179 * ((((((fVec32[((IOTA - iTemp180) & 2047)] * fTemp181) * fTemp182) * fTemp183) + (0.5f * (((fTemp171 * fVec32[((IOTA - iTemp184) & 2047)]) * fTemp185) * fTemp186))) + (0.166666672f * ((fTemp187 * fVec32[((IOTA - iTemp188) & 2047)]) * fTemp189))) + (0.0416666679f * (fTemp190 * fVec32[((IOTA - iTemp191) & 2047)])))));
			fRec169[0] = ((0.949999988f * fRec169[1]) + (0.0500000007f * fRec165[1]));
			fRec172[0] = (fTemp116 - (fConst229 * ((fConst232 * fRec172[2]) + (fConst233 * fRec172[1]))));
			float fTemp192 = (fConst229 * (((fConst231 * fRec172[0]) + (fConst234 * fRec172[1])) + (fConst231 * fRec172[2])));
			fRec171[0] = (fTemp192 - (fConst226 * ((fConst235 * fRec171[2]) + (fConst236 * fRec171[1]))));
			iRec173[0] = (((iRec173[1] + (iRec173[1] > 0)) * (fTemp164 <= fVec35[1])) + (fTemp164 > fVec35[1]));
			float fTemp193 = (fConst237 * float(iRec173[0]));
			float fTemp194 = (fConst226 * ((fRec171[2] + (fRec171[0] + (2.0f * fRec171[1]))) * std::max<float>(0.0f, std::min<float>(fTemp193, (2.0f - fTemp193)))));
			fRec170[0] = (fTemp194 - ((fConst239 * (fRec170[1] * std::cos((fConst48 * fTemp165)))) + (fConst240 * fRec170[2])));
			fRec174[0] = (fTemp194 - ((fConst244 * (fRec174[1] * std::cos((fConst245 * fTemp165)))) + (fConst246 * fRec174[2])));
			fRec175[0] = (fTemp194 - ((fConst249 * (fRec175[1] * std::cos((fConst250 * fTemp165)))) + (fConst251 * fRec175[2])));
			fRec176[0] = (fTemp194 - ((fConst254 * (fRec176[1] * std::cos((fConst255 * fTemp165)))) + (fConst256 * fRec176[2])));
			fRec177[0] = (fTemp194 - ((fConst259 * (fRec177[1] * std::cos((fConst260 * fTemp165)))) + (fConst261 * fRec177[2])));
			fRec178[0] = (fTemp194 - ((fConst264 * (fRec178[1] * std::cos((fConst265 * fTemp165)))) + (fConst266 * fRec178[2])));
			fRec179[0] = (fTemp194 - ((fConst269 * (fRec179[1] * std::cos((fConst270 * fTemp165)))) + (fConst271 * fRec179[2])));
			fRec180[0] = (fTemp194 - ((fConst274 * (fRec180[1] * std::cos((fConst275 * fTemp165)))) + (fConst276 * fRec180[2])));
			fRec181[0] = (fTemp194 - ((fConst279 * (fRec181[1] * std::cos((fConst280 * fTemp165)))) + (fConst281 * fRec181[2])));
			fRec182[0] = (fTemp194 - ((fConst284 * (fRec182[1] * std::cos((fConst285 * fTemp165)))) + (fConst286 * fRec182[2])));
			fRec183[0] = (fTemp194 - ((fConst289 * (fRec183[1] * std::cos((fConst290 * fTemp165)))) + (fConst291 * fRec183[2])));
			fRec184[0] = (fTemp194 - ((fConst294 * (fRec184[1] * std::cos((fConst295 * fTemp165)))) + (fConst296 * fRec184[2])));
			fRec185[0] = (fTemp194 - ((fConst299 * (fRec185[1] * std::cos((fConst300 * fTemp165)))) + (fConst301 * fRec185[2])));
			fRec186[0] = (fTemp194 - ((fConst304 * (fRec186[1] * std::cos((fConst305 * fTemp165)))) + (fConst306 * fRec186[2])));
			fRec187[0] = (fTemp194 - ((fConst309 * (fRec187[1] * std::cos((fConst310 * fTemp165)))) + (fConst311 * fRec187[2])));
			fRec188[0] = (fTemp194 - ((fConst314 * (fRec188[1] * std::cos((fConst315 * fTemp165)))) + (fConst316 * fRec188[2])));
			fRec189[0] = (fTemp194 - ((fConst319 * (fRec189[1] * std::cos((fConst320 * fTemp165)))) + (fConst321 * fRec189[2])));
			fRec190[0] = (fTemp194 - ((fConst324 * (fRec190[1] * std::cos((fConst325 * fTemp165)))) + (fConst326 * fRec190[2])));
			fRec191[0] = (fTemp194 - ((fConst329 * (fRec191[1] * std::cos((fConst330 * fTemp165)))) + (fConst331 * fRec191[2])));
			fRec192[0] = (fTemp194 - ((fConst334 * (fRec192[1] * std::cos((fConst335 * fTemp165)))) + (fConst336 * fRec192[2])));
			fRec193[0] = (fTemp194 - ((fConst339 * (fRec193[1] * std::cos((fConst340 * fTemp165)))) + (fConst341 * fRec193[2])));
			fRec194[0] = (fTemp194 - ((fConst344 * (fRec194[1] * std::cos((fConst345 * fTemp165)))) + (fConst346 * fRec194[2])));
			fRec195[0] = (fTemp194 - ((fConst349 * (fRec195[1] * std::cos((fConst350 * fTemp165)))) + (fConst351 * fRec195[2])));
			fRec196[0] = (fTemp194 - ((fConst354 * (fRec196[1] * std::cos((fConst355 * fTemp165)))) + (fConst356 * fRec196[2])));
			fRec197[0] = (fTemp194 - ((fConst359 * (fRec197[1] * std::cos((fConst360 * fTemp165)))) + (fConst361 * fRec197[2])));
			fRec198[0] = (fTemp194 - ((fConst364 * (fRec198[1] * std::cos((fConst365 * fTemp165)))) + (fConst366 * fRec198[2])));
			fRec199[0] = (fTemp194 - ((fConst369 * (fRec199[1] * std::cos((fConst370 * fTemp165)))) + (fConst371 * fRec199[2])));
			fRec200[0] = (fTemp194 - ((fConst374 * (fRec200[1] * std::cos((fConst375 * fTemp165)))) + (fConst376 * fRec200[2])));
			fRec201[0] = (fTemp194 - ((fConst379 * (fRec201[1] * std::cos((fConst380 * fTemp165)))) + (fConst381 * fRec201[2])));
			fRec202[0] = (fTemp194 - ((fConst384 * (fRec202[1] * std::cos((fConst385 * fTemp165)))) + (fConst386 * fRec202[2])));
			fRec203[0] = (fTemp194 - ((fConst389 * (fRec203[1] * std::cos((fConst390 * fTemp165)))) + (fConst391 * fRec203[2])));
			fRec204[0] = (fTemp194 - ((fConst394 * (fRec204[1] * std::cos((fConst395 * fTemp165)))) + (fConst396 * fRec204[2])));
			fRec205[0] = (fTemp194 - ((fConst399 * (fRec205[1] * std::cos((fConst400 * fTemp165)))) + (fConst401 * fRec205[2])));
			fRec206[0] = (fTemp194 - ((fConst404 * (fRec206[1] * std::cos((fConst405 * fTemp165)))) + (fConst406 * fRec206[2])));
			fRec207[0] = (fTemp194 - ((fConst409 * (fRec207[1] * std::cos((fConst410 * fTemp165)))) + (fConst411 * fRec207[2])));
			fRec208[0] = (fTemp194 - ((fConst414 * (fRec208[1] * std::cos((fConst415 * fTemp165)))) + (fConst416 * fRec208[2])));
			fRec209[0] = (fTemp194 - ((fConst419 * (fRec209[1] * std::cos((fConst420 * fTemp165)))) + (fConst421 * fRec209[2])));
			fRec210[0] = (fTemp194 - ((fConst424 * (fRec210[1] * std::cos((fConst425 * fTemp165)))) + (fConst426 * fRec210[2])));
			fRec211[0] = (fTemp194 - ((fConst429 * (fRec211[1] * std::cos((fConst430 * fTemp165)))) + (fConst431 * fRec211[2])));
			fRec212[0] = (fTemp194 - ((fConst434 * (fRec212[1] * std::cos((fConst435 * fTemp165)))) + (fConst436 * fRec212[2])));
			fRec213[0] = (fTemp194 - ((fConst439 * (fRec213[1] * std::cos((fConst440 * fTemp165)))) + (fConst441 * fRec213[2])));
			fRec214[0] = (fTemp194 - ((fConst444 * (fRec214[1] * std::cos((fConst445 * fTemp165)))) + (fConst446 * fRec214[2])));
			fRec215[0] = (fTemp194 - ((fConst449 * (fRec215[1] * std::cos((fConst450 * fTemp165)))) + (fConst451 * fRec215[2])));
			fRec216[0] = (fTemp194 - ((fConst454 * (fRec216[1] * std::cos((fConst455 * fTemp165)))) + (fConst456 * fRec216[2])));
			fRec217[0] = (fTemp194 - ((fConst459 * (fRec217[1] * std::cos((fConst460 * fTemp165)))) + (fConst461 * fRec217[2])));
			fRec218[0] = (fTemp194 - ((fConst464 * (fRec218[1] * std::cos((fConst465 * fTemp165)))) + (fConst466 * fRec218[2])));
			fRec219[0] = (fTemp194 - ((fConst469 * (fRec219[1] * std::cos((fConst470 * fTemp165)))) + (fConst471 * fRec219[2])));
			fRec220[0] = (fTemp194 - ((fConst474 * (fRec220[1] * std::cos((fConst475 * fTemp165)))) + (fConst476 * fRec220[2])));
			fRec221[0] = (fTemp194 - ((fConst479 * (fRec221[1] * std::cos((fConst480 * fTemp165)))) + (fConst481 * fRec221[2])));
			fRec222[0] = (fTemp194 - ((fConst484 * (fRec222[1] * std::cos((fConst485 * fTemp165)))) + (fConst486 * fRec222[2])));
			fVec36[0] = (((((((((((((((((((((((((((((((((((((((((((((((((((fRec170[0] - fRec170[2]) * ((fTemp165 < fConst241) ? fConst242 : 0.0f)) + ((fRec174[0] - fRec174[2]) * (((3.31356001f * fTemp165) < fConst241) ? fConst247 : 0.0f))) + ((fRec175[0] - fRec175[2]) * (((3.83469009f * fTemp165) < fConst241) ? fConst252 : 0.0f))) + ((fRec176[0] - fRec176[2]) * (((8.06313038f * fTemp165) < fConst241) ? fConst257 : 0.0f))) + ((fRec177[0] - fRec177[2]) * (((9.44777966f * fTemp165) < fConst241) ? fConst262 : 0.0f))) + ((fRec178[0] - fRec178[2]) * (((14.1169004f * fTemp165) < fConst241) ? fConst267 : 0.0f))) + ((fRec179[0] - fRec179[2]) * (((18.3840008f * fTemp165) < fConst241) ? fConst272 : 0.0f))) + ((fRec180[0] - fRec180[2]) * (((21.0102005f * fTemp165) < fConst241) ? fConst277 : 0.0f))) + ((fRec181[0] - fRec181[2]) * (((26.1774998f * fTemp165) < fConst241) ? fConst282 : 0.0f))) + ((fRec182[0] - fRec182[2]) * (((28.9944f * fTemp165) < fConst241) ? fConst287 : 0.0f))) + ((fRec183[0] - fRec183[2]) * (((37.0727997f * fTemp165) < fConst241) ? fConst292 : 0.0f))) + ((fRec184[0] - fRec184[2]) * (((37.8703003f * fTemp165) < fConst241) ? fConst297 : 0.0f))) + ((fRec185[0] - fRec185[2]) * (((40.0634003f * fTemp165) < fConst241) ? fConst302 : 0.0f))) + ((fRec186[0] - fRec186[2]) * (((47.6439018f * fTemp165) < fConst241) ? fConst307 : 0.0f))) + ((fRec187[0] - fRec187[2]) * (((51.019001f * fTemp165) < fConst241) ? fConst312 : 0.0f))) + ((fRec188[0] - fRec188[2]) * (((52.4300003f * fTemp165) < fConst241) ? fConst317 : 0.0f))) + ((fRec189[0] - fRec189[2]) * (((58.2859993f * fTemp165) < fConst241) ? fConst322 : 0.0f))) + ((fRec190[0] - fRec190[2]) * (((63.5485992f * fTemp165) < fConst241) ? fConst327 : 0.0f))) + ((fRec191[0] - fRec191[2]) * (((65.3628006f * fTemp165) < fConst241) ? fConst332 : 0.0f))) + ((fRec192[0] - fRec192[2]) * (((66.9587021f * fTemp165) < fConst241) ? fConst337 : 0.0f))) + ((fRec193[0] - fRec193[2]) * (((74.530098f * fTemp165) < fConst241) ? fConst342 : 0.0f))) + ((fRec194[0] - fRec194[2]) * (((78.6920013f * fTemp165) < fConst241) ? fConst347 : 0.0f))) + ((fRec195[0] - fRec195[2]) * (((80.8375015f * fTemp165) < fConst241) ? fConst352 : 0.0f))) + ((fRec196[0] - fRec196[2]) * (((89.9779968f * fTemp165) < fConst241) ? fConst357 : 0.0f))) + ((fRec197[0] - fRec197[2]) * (((92.9661026f * fTemp165) < fConst241) ? fConst362 : 0.0f))) + ((fRec198[0] - fRec198[2]) * (((95.1913986f * fTemp165) < fConst241) ? fConst367 : 0.0f))) + ((fRec199[0] - fRec199[2]) * (((97.4806976f * fTemp165) < fConst241) ? fConst372 : 0.0f))) + ((fRec200[0] - fRec200[2]) * (((110.620003f * fTemp165) < fConst241) ? fConst377 : 0.0f))) + ((fRec201[0] - fRec201[2]) * (((112.069f * fTemp165) < fConst241) ? fConst382 : 0.0f))) + ((fRec202[0] - fRec202[2]) * (((113.825996f * fTemp165) < fConst241) ? fConst387 : 0.0f))) + ((fRec203[0] - fRec203[2]) * (((119.356003f * fTemp165) < fConst241) ? fConst392 : 0.0f))) + ((fRec204[0] - fRec204[2]) * (((127.044998f * fTemp165) < fConst241) ? fConst397 : 0.0f))) + ((fRec205[0] - fRec205[2]) * (((129.981995f * fTemp165) < fConst241) ? fConst402 : 0.0f))) + ((fRec206[0] - fRec206[2]) * (((132.259003f * fTemp165) < fConst241) ? fConst407 : 0.0f))) + ((fRec207[0] - fRec207[2]) * (((133.477005f * fTemp165) < fConst241) ? fConst412 : 0.0f))) + ((fRec208[0] - fRec208[2]) * (((144.548996f * fTemp165) < fConst241) ? fConst417 : 0.0f))) + ((fRec209[0] - fRec209[2]) * (((149.438004f * fTemp165) < fConst241) ? fConst422 : 0.0f))) + ((fRec210[0] - fRec210[2]) * (((152.033005f * fTemp165) < fConst241) ? fConst427 : 0.0f))) + ((fRec211[0] - fRec211[2]) * (((153.166f * fTemp165) < fConst241) ? fConst432 : 0.0f))) + ((fRec212[0] - fRec212[2]) * (((155.597f * fTemp165) < fConst241) ? fConst437 : 0.0f))) + ((fRec213[0] - fRec213[2]) * (((158.182999f * fTemp165) < fConst241) ? fConst442 : 0.0f))) + ((fRec214[0] - fRec214[2]) * (((168.104996f * fTemp165) < fConst241) ? fConst447 : 0.0f))) + ((fRec215[0] - fRec215[2]) * (((171.863007f * fTemp165) < fConst241) ? fConst452 : 0.0f))) + ((fRec216[0] - fRec216[2]) * (((174.464005f * fTemp165) < fConst241) ? fConst457 : 0.0f))) + ((fRec217[0] - fRec217[2]) * (((178.936996f * fTemp165) < fConst241) ? fConst462 : 0.0f))) + ((fRec218[0] - fRec218[2]) * (((181.481995f * fTemp165) < fConst241) ? fConst467 : 0.0f))) + ((fRec219[0] - fRec219[2]) * (((185.397995f * fTemp165) < fConst241) ? fConst472 : 0.0f))) + ((fRec220[0] - fRec220[2]) * (((190.369003f * fTemp165) < fConst241) ? fConst477 : 0.0f))) + ((fRec221[0] - fRec221[2]) * (((192.190002f * fTemp165) < fConst241) ? fConst482 : 0.0f))) + ((fRec222[0] - fRec222[2]) * (((195.505005f * fTemp165) < fConst241) ? fConst487 : 0.0f)));
			float fTemp195 = ((0.99000001f * fRec169[0]) + (0.0199999996f * fVec36[1]));
			fVec37[(IOTA & 2047)] = fTemp195;
			fRec166[0] = (((((fTemp172 * fTemp174) * fTemp176) * fTemp178) * fVec37[((IOTA - iTemp169) & 2047)]) + (fTemp179 * ((((((fTemp181 * fTemp182) * fTemp183) * fVec37[((IOTA - iTemp180) & 2047)]) + (0.5f * (((fTemp171 * fTemp185) * fTemp186) * fVec37[((IOTA - iTemp184) & 2047)]))) + (0.166666672f * ((fTemp187 * fTemp189) * fVec37[((IOTA - iTemp188) & 2047)]))) + (0.0416666679f * (fTemp190 * fVec37[((IOTA - iTemp191) & 2047)])))));
			float fRec164 = fRec166[0];
			float fTemp196 = std::fabs((0.25f * fRec164));
			fRec225[0] = std::max<float>(fTemp196, ((fRec225[1] * fSlow387) + (fTemp196 * fSlow388)));
			fRec224[0] = ((fRec224[1] * fSlow384) + (fRec225[0] * fSlow389));
			fRec223[0] = ((fConst46 * fRec223[1]) + (fSlow381 * std::max<float>(((20.0f * std::log10(fRec224[0])) - fSlow390), 0.0f)));
			float fTemp197 = (0.25f * (fRec164 * std::pow(10.0f, (0.0500000007f * fRec223[0]))));
			fRec226[0] = (fTemp197 - (fSlow394 * ((fSlow395 * fRec226[2]) + (fSlow396 * fRec226[1]))));
			float fTemp198 = (fSlow377 * fRec162[1]);
			fRec162[0] = (fTemp197 - ((fSlow394 * (fRec226[2] + (fRec226[0] + (2.0f * fRec226[1])))) + (((fRec162[2] * fSlow404) + fTemp198) / fSlow405)));
			float fTemp199 = (fSlow374 * fRec161[1]);
			fRec161[0] = ((((fTemp198 + (fRec162[0] * fSlow407)) + (fRec162[2] * fSlow408)) / fSlow405) - (((fRec161[2] * fSlow416) + fTemp199) / fSlow417));
			fRec160[0] = ((((fTemp199 + (fRec161[0] * fSlow419)) + (fRec161[2] * fSlow420)) / fSlow417) - (fSlow421 * ((fSlow422 * fRec160[2]) + (fSlow423 * fRec160[1]))));
			float fTemp200 = (fRec160[2] + (fRec160[0] + (2.0f * fRec160[1])));
			fRec230[0] = ((0.999000013f * fRec230[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp132, ((fConst488 * fTemp133) + 1.0f))))));
			fRec232[0] = (iTemp134 ? fSlow435 : fRec232[1]);
			float fTemp201 = (fRec236[1] + (fSlow436 * fRec232[0]));
			fRec236[0] = (fTemp201 - std::floor(fTemp201));
			float fTemp202 = (fRec235[1] + (fConst5 * ((315.0f * (fRec230[0] * ftbl0mydspSIG0[int((65536.0f * fRec236[0]))])) + (fSlow437 * fRec232[0]))));
			fRec235[0] = (fTemp202 - std::floor(fTemp202));
			float fTemp203 = (fRec234[1] + (fConst5 * ((315.0f * (fRec230[0] * ftbl0mydspSIG0[int((65536.0f * fRec235[0]))])) + (fSlow438 * fRec232[0]))));
			fRec234[0] = (fTemp203 - std::floor(fTemp203));
			float fTemp204 = (fRec233[1] + (fConst5 * ((315.0f * (fRec230[0] * ftbl0mydspSIG0[int((65536.0f * fRec234[0]))])) + (fSlow439 * fRec232[0]))));
			fRec233[0] = (fTemp204 - std::floor(fTemp204));
			float fTemp205 = (fRec231[1] + (fConst5 * ((fSlow12 * fRec232[0]) + (315.0f * (fRec230[0] * ftbl0mydspSIG0[int((65536.0f * fRec233[0]))])))));
			fRec231[0] = (fTemp205 - std::floor(fTemp205));
			float fTemp206 = ftbl0mydspSIG0[int((65536.0f * fRec231[0]))];
			float fTemp207 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp206)));
			float fTemp208 = (fRec230[0] * (fTemp206 + (0.0500000007f * (fTemp207 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp207)))))));
			float fTemp209 = std::fabs((0.25f * fTemp208));
			fRec239[0] = std::max<float>(fTemp209, ((fRec239[1] * fSlow446) + (fTemp209 * fSlow447)));
			fRec238[0] = ((fRec238[1] * fSlow443) + (fRec239[0] * fSlow448));
			fRec237[0] = ((fConst46 * fRec237[1]) + (fSlow440 * std::max<float>(((20.0f * std::log10(fRec238[0])) - fSlow449), 0.0f)));
			float fTemp210 = (0.25f * (fTemp208 * std::pow(10.0f, (0.0500000007f * fRec237[0]))));
			fRec240[0] = (fTemp210 - (fSlow453 * ((fSlow454 * fRec240[2]) + (fSlow455 * fRec240[1]))));
			float fTemp211 = (fSlow434 * fRec229[1]);
			fRec229[0] = (fTemp210 - ((fSlow453 * (fRec240[2] + (fRec240[0] + (2.0f * fRec240[1])))) + (((fRec229[2] * fSlow463) + fTemp211) / fSlow464)));
			float fTemp212 = (fSlow431 * fRec228[1]);
			fRec228[0] = ((((fTemp211 + (fRec229[0] * fSlow466)) + (fRec229[2] * fSlow467)) / fSlow464) - (((fRec228[2] * fSlow475) + fTemp212) / fSlow476));
			fRec227[0] = ((((fTemp212 + (fRec228[0] * fSlow478)) + (fRec228[2] * fSlow479)) / fSlow476) - (fSlow480 * ((fSlow481 * fRec227[2]) + (fSlow482 * fRec227[1]))));
			float fTemp213 = (fSlow428 * (fRec227[2] + (fRec227[0] + (2.0f * fRec227[1]))));
			fRec245[0] = (fTemp192 - (fConst493 * ((fConst494 * fRec245[2]) + (fConst495 * fRec245[1]))));
			fVec38[0] = fSlow494;
			float fTemp214 = (fSlow494 - fVec38[1]);
			fVec39[0] = fSlow496;
			float fTemp215 = (0.0f - (fSlow494 + fVec39[1]));
			float fTemp216 = std::min<float>(1.0f, ((fTemp214 * float((fTemp214 > 0.0f))) + (fSlow495 * (fTemp215 * float((fTemp215 > 0.0f))))));
			fVec40[0] = fTemp216;
			iRec246[0] = (((iRec246[1] + (iRec246[1] > 0)) * (fTemp216 <= fVec40[1])) + (fTemp216 > fVec40[1]));
			float fTemp217 = float(iRec246[0]);
			float fTemp218 = (fConst496 * fTemp217);
			float fTemp219 = (fConst493 * ((fRec245[2] + (fRec245[0] + (2.0f * fRec245[1]))) * std::max<float>(0.0f, std::min<float>(fTemp218, (2.0f - fTemp218)))));
			fRec244[0] = (fTemp219 - ((fConst498 * fRec244[1]) + (fConst499 * fRec244[2])));
			fRec247[0] = (fTemp219 - ((fConst502 * fRec247[1]) + (fConst503 * fRec247[2])));
			fRec248[0] = (fTemp219 - ((fConst506 * fRec248[1]) + (fConst507 * fRec248[2])));
			fRec249[0] = (fTemp219 - ((fConst510 * fRec249[1]) + (fConst511 * fRec249[2])));
			fRec250[0] = (fTemp219 - ((fConst514 * fRec250[1]) + (fConst515 * fRec250[2])));
			fRec251[0] = (fTemp219 - ((fConst518 * fRec251[1]) + (fConst519 * fRec251[2])));
			fRec252[0] = (fTemp219 - ((fConst522 * fRec252[1]) + (fConst523 * fRec252[2])));
			fRec253[0] = (fTemp219 - ((fConst526 * fRec253[1]) + (fConst527 * fRec253[2])));
			fRec254[0] = (fTemp219 - ((fConst530 * fRec254[1]) + (fConst531 * fRec254[2])));
			fRec255[0] = (fTemp219 - ((fConst534 * fRec255[1]) + (fConst535 * fRec255[2])));
			fRec256[0] = (fTemp219 - ((fConst538 * fRec256[1]) + (fConst539 * fRec256[2])));
			fRec257[0] = (fTemp219 - ((fConst542 * fRec257[1]) + (fConst543 * fRec257[2])));
			fRec258[0] = (fTemp219 - ((fConst546 * fRec258[1]) + (fConst547 * fRec258[2])));
			fRec259[0] = (fTemp219 - ((fConst550 * fRec259[1]) + (fConst551 * fRec259[2])));
			fRec260[0] = (fTemp219 - ((fConst554 * fRec260[1]) + (fConst555 * fRec260[2])));
			fRec261[0] = (fTemp219 - ((fConst558 * fRec261[1]) + (fConst559 * fRec261[2])));
			fRec262[0] = (fTemp219 - ((fConst562 * fRec262[1]) + (fConst563 * fRec262[2])));
			fRec263[0] = (fTemp219 - ((fConst566 * fRec263[1]) + (fConst567 * fRec263[2])));
			fRec264[0] = (fTemp219 - ((fConst570 * fRec264[1]) + (fConst571 * fRec264[2])));
			fRec265[0] = (fTemp219 - ((fConst574 * fRec265[1]) + (fConst575 * fRec265[2])));
			fRec266[0] = (fTemp219 - ((fConst578 * fRec266[1]) + (fConst579 * fRec266[2])));
			fRec267[0] = (fTemp219 - ((fConst582 * fRec267[1]) + (fConst583 * fRec267[2])));
			fRec268[0] = (fTemp219 - ((fConst586 * fRec268[1]) + (fConst587 * fRec268[2])));
			fRec269[0] = (fTemp219 - ((fConst590 * fRec269[1]) + (fConst591 * fRec269[2])));
			fRec270[0] = (fTemp219 - ((fConst594 * fRec270[1]) + (fConst595 * fRec270[2])));
			fRec271[0] = (fTemp219 - ((fConst598 * fRec271[1]) + (fConst599 * fRec271[2])));
			fRec272[0] = (fTemp219 - ((fConst602 * fRec272[1]) + (fConst603 * fRec272[2])));
			fRec273[0] = (fTemp219 - ((fConst606 * fRec273[1]) + (fConst607 * fRec273[2])));
			fRec274[0] = (fTemp219 - ((fConst610 * fRec274[1]) + (fConst611 * fRec274[2])));
			fRec275[0] = (fTemp219 - ((fConst614 * fRec275[1]) + (fConst615 * fRec275[2])));
			fRec276[0] = (fTemp219 - ((fConst618 * fRec276[1]) + (fConst619 * fRec276[2])));
			fRec277[0] = (fTemp219 - ((fConst622 * fRec277[1]) + (fConst623 * fRec277[2])));
			fRec278[0] = (fTemp219 - ((fConst626 * fRec278[1]) + (fConst627 * fRec278[2])));
			fRec279[0] = (fTemp219 - ((fConst630 * fRec279[1]) + (fConst631 * fRec279[2])));
			fRec280[0] = (fTemp219 - ((fConst634 * fRec280[1]) + (fConst635 * fRec280[2])));
			fRec281[0] = (fTemp219 - ((fConst638 * fRec281[1]) + (fConst639 * fRec281[2])));
			fRec282[0] = (fTemp219 - ((fConst642 * fRec282[1]) + (fConst643 * fRec282[2])));
			fRec283[0] = (fTemp219 - ((fConst646 * fRec283[1]) + (fConst647 * fRec283[2])));
			fRec284[0] = (fTemp219 - ((fConst650 * fRec284[1]) + (fConst651 * fRec284[2])));
			fRec285[0] = (fTemp219 - ((fConst654 * fRec285[1]) + (fConst655 * fRec285[2])));
			fRec286[0] = (fTemp219 - ((fConst658 * fRec286[1]) + (fConst659 * fRec286[2])));
			fRec287[0] = (fTemp219 - ((fConst662 * fRec287[1]) + (fConst663 * fRec287[2])));
			fRec288[0] = (fTemp219 - ((fConst666 * fRec288[1]) + (fConst667 * fRec288[2])));
			fRec289[0] = (fTemp219 - ((fConst670 * fRec289[1]) + (fConst671 * fRec289[2])));
			fRec290[0] = (fTemp219 - ((fConst674 * fRec290[1]) + (fConst675 * fRec290[2])));
			fRec291[0] = (fTemp219 - ((fConst678 * fRec291[1]) + (fConst679 * fRec291[2])));
			fRec292[0] = (fTemp219 - ((fConst682 * fRec292[1]) + (fConst683 * fRec292[2])));
			fRec293[0] = (fTemp219 - ((fConst686 * fRec293[1]) + (fConst687 * fRec293[2])));
			fRec294[0] = (fTemp219 - ((fConst690 * fRec294[1]) + (fConst691 * fRec294[2])));
			fRec295[0] = (fTemp219 - ((fConst694 * fRec295[1]) + (fConst695 * fRec295[2])));
			fRec296[0] = (fSlow498 + (0.999000013f * fRec296[1]));
			fRec297[0] = (fSlow499 + (fRec297[1] - std::floor((fSlow499 + fRec297[1]))));
			fRec298[0] = (fSlow501 + (0.999000013f * fRec298[1]));
			fRec299[0] = (fSlow502 + (fRec299[1] - std::floor((fSlow502 + fRec299[1]))));
			float fTemp220 = ((0.0199999996f * (((((((((((((((((((((((((((((((((((((((((((((((((((fConst490 * (fRec244[0] - fRec244[2])) + (fConst500 * (fRec247[0] - fRec247[2]))) + (fConst504 * (fRec248[0] - fRec248[2]))) + (fConst508 * (fRec249[0] - fRec249[2]))) + (fConst512 * (fRec250[0] - fRec250[2]))) + (fConst516 * (fRec251[0] - fRec251[2]))) + (fConst520 * (fRec252[0] - fRec252[2]))) + (fConst524 * (fRec253[0] - fRec253[2]))) + (fConst528 * (fRec254[0] - fRec254[2]))) + (fConst532 * (fRec255[0] - fRec255[2]))) + (fConst536 * (fRec256[0] - fRec256[2]))) + (fConst540 * (fRec257[0] - fRec257[2]))) + (fConst544 * (fRec258[0] - fRec258[2]))) + (fConst548 * (fRec259[0] - fRec259[2]))) + (fConst552 * (fRec260[0] - fRec260[2]))) + (fConst556 * (fRec261[0] - fRec261[2]))) + (fConst560 * (fRec262[0] - fRec262[2]))) + (fConst564 * (fRec263[0] - fRec263[2]))) + (fConst568 * (fRec264[0] - fRec264[2]))) + (fConst572 * (fRec265[0] - fRec265[2]))) + (fConst576 * (fRec266[0] - fRec266[2]))) + (fConst580 * (fRec267[0] - fRec267[2]))) + (fConst584 * (fRec268[0] - fRec268[2]))) + (fConst588 * (fRec269[0] - fRec269[2]))) + (fConst592 * (fRec270[0] - fRec270[2]))) + (fConst596 * (fRec271[0] - fRec271[2]))) + (fConst600 * (fRec272[0] - fRec272[2]))) + (fConst604 * (fRec273[0] - fRec273[2]))) + (fConst608 * (fRec274[0] - fRec274[2]))) + (fConst612 * (fRec275[0] - fRec275[2]))) + (fConst616 * (fRec276[0] - fRec276[2]))) + (fConst620 * (fRec277[0] - fRec277[2]))) + (fConst624 * (fRec278[0] - fRec278[2]))) + (fConst628 * (fRec279[0] - fRec279[2]))) + (fConst632 * (fRec280[0] - fRec280[2]))) + (fConst636 * (fRec281[0] - fRec281[2]))) + (fConst640 * (fRec282[0] - fRec282[2]))) + (fConst644 * (fRec283[0] - fRec283[2]))) + (fConst648 * (fRec284[0] - fRec284[2]))) + (fConst652 * (fRec285[0] - fRec285[2]))) + (fConst656 * (fRec286[0] - fRec286[2]))) + (fConst660 * (fRec287[0] - fRec287[2]))) + (fConst664 * (fRec288[0] - fRec288[2]))) + (fConst668 * (fRec289[0] - fRec289[2]))) + (fConst672 * (fRec290[0] - fRec290[2]))) + (fConst676 * (fRec291[0] - fRec291[2]))) + (fConst680 * (fRec292[0] - fRec292[2]))) + (fConst684 * (fRec293[0] - fRec293[2]))) + (fConst688 * (fRec294[0] - fRec294[2]))) + (fConst692 * (fRec295[0] - fRec295[2]))) * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp217), ((fConst696 * (fConst43 - fTemp217)) + 1.0f))))) + (2.0f * ((fRec296[0] * ftbl0mydspSIG0[int((65536.0f * fRec297[0]))]) + (fRec298[0] * ftbl0mydspSIG0[int((65536.0f * fRec299[0]))]))));
			float fTemp221 = std::fabs((0.25f * fTemp220));
			fRec302[0] = std::max<float>(fTemp221, ((fRec302[1] * fSlow509) + (fTemp221 * fSlow510)));
			fRec301[0] = ((fRec301[1] * fSlow506) + (fRec302[0] * fSlow511));
			fRec300[0] = ((fConst46 * fRec300[1]) + (fSlow503 * std::max<float>(((20.0f * std::log10(fRec301[0])) - fSlow512), 0.0f)));
			float fTemp222 = (0.25f * (fTemp220 * std::pow(10.0f, (0.0500000007f * fRec300[0]))));
			fRec303[0] = (fTemp222 - (fSlow516 * ((fSlow517 * fRec303[2]) + (fSlow518 * fRec303[1]))));
			float fTemp223 = (fSlow493 * fRec243[1]);
			fRec243[0] = (fTemp222 - ((fSlow516 * (fRec303[2] + (fRec303[0] + (2.0f * fRec303[1])))) + (((fRec243[2] * fSlow526) + fTemp223) / fSlow527)));
			float fTemp224 = (fSlow490 * fRec242[1]);
			fRec242[0] = ((((fTemp223 + (fRec243[0] * fSlow529)) + (fRec243[2] * fSlow530)) / fSlow527) - (((fRec242[2] * fSlow538) + fTemp224) / fSlow539));
			fRec241[0] = ((((fTemp224 + (fRec242[0] * fSlow541)) + (fRec242[2] * fSlow542)) / fSlow539) - (fSlow543 * ((fSlow544 * fRec241[2]) + (fSlow545 * fRec241[1]))));
			float fTemp225 = (fSlow487 * (fRec241[2] + (fRec241[0] + (2.0f * fRec241[1]))));
			float fTemp226 = ((fConst10 * fRec0[2]) + ((((((fTemp127 + (fSlow308 * (fRec137[2] + (fRec137[0] + (2.0f * fRec137[1]))))) + (fSlow371 * fTemp200)) + fTemp213) + fTemp99) + fTemp225) + (fConst9 * fRec0[0])));
			float fTemp227 = std::fabs(fTemp226);
			fRec306[0] = std::max<float>(fTemp227, ((fConst699 * fRec306[1]) + (fConst700 * fTemp227)));
			fRec305[0] = ((fConst697 * fRec305[1]) + (fConst698 * fRec306[0]));
			fRec304[0] = ((fConst46 * fRec304[1]) + (fConst47 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec305[0]) + 1.0f)), 0.0f)))));
			float fTemp228 = (fTemp226 * std::pow(10.0f, (0.0500000007f * fRec304[0])));
			float fTemp229 = std::fabs(fTemp228);
			fRec309[0] = std::max<float>(fTemp229, ((fConst703 * fRec309[1]) + (fConst704 * fTemp229)));
			fRec308[0] = ((fConst701 * fRec308[1]) + (fConst702 * fRec309[0]));
			fRec307[0] = ((fConst46 * fRec307[1]) + (fConst47 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec308[0])) + 5.0f), 0.0f)))));
			float fTemp230 = (fTemp228 * std::pow(10.0f, (0.0500000007f * fRec307[0])));
			float fTemp231 = std::fabs((fSlow0 * fTemp230));
			fRec312[0] = std::max<float>(fTemp231, ((fConst46 * fRec312[1]) + (fConst47 * fTemp231)));
			fRec311[0] = ((fConst705 * fRec311[1]) + (fConst706 * fRec312[0]));
			fRec310[0] = ((fConst46 * fRec310[1]) + (fConst47 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec311[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp230 * std::pow(10.0f, (0.0500000007f * fRec310[0])))))));
			fRec313[0] = ((0.370000005f * (fRec2[0] - fRec3[0])) - (fConst152 * ((fConst153 * fRec313[1]) + (fConst154 * fRec313[2]))));
			float fTemp232 = (fRec144[0] + 0.5f);
			iRec318[0] = ((iVec0[1] + iRec318[1]) % int((fConst0 / fTemp232)));
			float fTemp233 = float(iRec318[0]);
			float fTemp234 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fTemp232) * (1.0f - (0.000500000024f * std::min<float>(fTemp232, 1000.0f)))))))));
			float fTemp235 = (1.0f / fTemp234);
			float fTemp236 = (((fTemp235 + 1.41421354f) / fTemp234) + 1.0f);
			fRec317[0] = ((2.0f * (((float((fTemp233 < (fConst218 / fTemp232))) + -0.5f) + (float((fTemp233 < (fConst219 / fTemp232))) + -0.5f)) + (float((fTemp233 < (fConst220 / fTemp232))) + -0.5f))) - (((fRec317[2] * (((fTemp235 + -1.41421354f) / fTemp234) + 1.0f)) + (2.0f * (fRec317[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp234)))))) / fTemp236));
			float fTemp237 = (fRec149[0] + 0.5f);
			iRec320[0] = ((iVec0[1] + iRec320[1]) % int((fConst0 / fTemp237)));
			float fTemp238 = float(iRec320[0]);
			float fTemp239 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fTemp237) * (1.0f - (0.000500000024f * std::min<float>(fTemp237, 1000.0f)))))))));
			float fTemp240 = (1.0f / fTemp239);
			float fTemp241 = (((fTemp240 + 1.41421354f) / fTemp239) + 1.0f);
			fRec319[0] = ((2.0f * (((float((fTemp238 < (fConst218 / fTemp237))) + -0.5f) + (float((fTemp238 < (fConst219 / fTemp237))) + -0.5f)) + (float((fTemp238 < (fConst220 / fTemp237))) + -0.5f))) - (((fRec319[2] * (((fTemp240 + -1.41421354f) / fTemp239) + 1.0f)) + (2.0f * (fRec319[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp239)))))) / fTemp241));
			float fTemp242 = (fRec152[0] + 0.5f);
			iRec322[0] = ((iVec0[1] + iRec322[1]) % int((fConst0 / fTemp242)));
			float fTemp243 = float(iRec322[0]);
			float fTemp244 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fTemp242) * (1.0f - (0.000500000024f * std::min<float>(fTemp242, 1000.0f)))))))));
			float fTemp245 = (1.0f / fTemp244);
			float fTemp246 = (((fTemp245 + 1.41421354f) / fTemp244) + 1.0f);
			fRec321[0] = ((2.0f * (((float((fTemp243 < (fConst218 / fTemp242))) + -0.5f) + (float((fTemp243 < (fConst219 / fTemp242))) + -0.5f)) + (float((fTemp243 < (fConst220 / fTemp242))) + -0.5f))) - (((fRec321[2] * (((fTemp245 + -1.41421354f) / fTemp244) + 1.0f)) + (2.0f * (fRec321[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp244)))))) / fTemp246));
			float fTemp247 = (fRec155[0] + 0.5f);
			iRec324[0] = ((iVec0[1] + iRec324[1]) % int((fConst0 / fTemp247)));
			float fTemp248 = float(iRec324[0]);
			float fTemp249 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp140 * fTemp247) * (1.0f - (0.000500000024f * std::min<float>(fTemp247, 1000.0f)))))))));
			float fTemp250 = (1.0f / fTemp249);
			float fTemp251 = (((fTemp250 + 1.41421354f) / fTemp249) + 1.0f);
			fRec323[0] = ((2.0f * (((float((fTemp248 < (fConst218 / fTemp247))) + -0.5f) + (float((fTemp248 < (fConst219 / fTemp247))) + -0.5f)) + (float((fTemp248 < (fConst220 / fTemp247))) + -0.5f))) - (((fRec323[2] * (((fTemp250 + -1.41421354f) / fTemp249) + 1.0f)) + (2.0f * (fRec323[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp249)))))) / fTemp251));
			float fTemp252 = (fRec140[0] * (((((fRec317[2] + (fRec317[0] + (2.0f * fRec317[1]))) / fTemp236) + ((fRec319[2] + (fRec319[0] + (2.0f * fRec319[1]))) / fTemp241)) + ((fRec321[2] + (fRec321[0] + (2.0f * fRec321[1]))) / fTemp246)) + ((fRec323[2] + (fRec323[0] + (2.0f * fRec323[1]))) / fTemp251)));
			float fTemp253 = std::fabs((0.25f * fTemp252));
			fRec327[0] = std::max<float>(fTemp253, ((fSlow329 * fRec327[1]) + (fSlow330 * fTemp253)));
			fRec326[0] = ((fSlow326 * fRec326[1]) + (fRec327[0] * fSlow331));
			fRec325[0] = ((fConst46 * fRec325[1]) + (fSlow323 * std::max<float>(((20.0f * std::log10(fRec326[0])) - fSlow332), 0.0f)));
			float fTemp254 = (0.25f * (fTemp252 * std::pow(10.0f, (0.0500000007f * fRec325[0]))));
			fRec328[0] = (fTemp254 - (fSlow336 * ((fSlow337 * fRec328[2]) + (fSlow338 * fRec328[1]))));
			float fTemp255 = (fSlow314 * fRec316[1]);
			fRec316[0] = (fTemp254 - ((fSlow336 * (fRec328[2] + (fRec328[0] + (2.0f * fRec328[1])))) + (((fSlow346 * fRec316[2]) + fTemp255) / fSlow347)));
			float fTemp256 = (fSlow311 * fRec315[1]);
			fRec315[0] = ((((fTemp255 + (fRec316[0] * fSlow349)) + (fSlow350 * fRec316[2])) / fSlow347) - (((fSlow358 * fRec315[2]) + fTemp256) / fSlow359));
			fRec314[0] = ((((fTemp256 + (fRec315[0] * fSlow361)) + (fSlow362 * fRec315[2])) / fSlow359) - (fSlow363 * ((fSlow364 * fRec314[2]) + (fSlow365 * fRec314[1]))));
			float fTemp257 = ((fConst10 * fRec313[2]) + (((fSlow308 * (fRec314[2] + (fRec314[0] + (2.0f * fRec314[1])))) + (fTemp225 + (fTemp99 + (fTemp213 + (fTemp127 + (fSlow546 * fTemp200)))))) + (fConst9 * fRec313[0])));
			float fTemp258 = std::fabs(fTemp257);
			fRec331[0] = std::max<float>(fTemp258, ((fConst699 * fRec331[1]) + (fConst700 * fTemp258)));
			fRec330[0] = ((fConst697 * fRec330[1]) + (fConst698 * fRec331[0]));
			fRec329[0] = ((fConst46 * fRec329[1]) + (fConst47 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec330[0]) + 1.0f)), 0.0f)))));
			float fTemp259 = (fTemp257 * std::pow(10.0f, (0.0500000007f * fRec329[0])));
			float fTemp260 = std::fabs(fTemp259);
			fRec334[0] = std::max<float>(fTemp260, ((fConst703 * fRec334[1]) + (fConst704 * fTemp260)));
			fRec333[0] = ((fConst701 * fRec333[1]) + (fConst702 * fRec334[0]));
			fRec332[0] = ((fConst46 * fRec332[1]) + (fConst47 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec333[0])) + 5.0f), 0.0f)))));
			float fTemp261 = (fTemp259 * std::pow(10.0f, (0.0500000007f * fRec332[0])));
			float fTemp262 = std::fabs((fSlow0 * fTemp261));
			fRec337[0] = std::max<float>(fTemp262, ((fConst46 * fRec337[1]) + (fConst47 * fTemp262)));
			fRec336[0] = ((fConst705 * fRec336[1]) + (fConst706 * fRec337[0]));
			fRec335[0] = ((fConst46 * fRec335[1]) + (fConst47 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec336[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT(std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow0 * (fTemp261 * std::pow(10.0f, (0.0500000007f * fRec335[0])))))));
			iVec0[1] = iVec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fRec18[1] = fRec18[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			fRec20[1] = fRec20[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			iRec30[1] = iRec30[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			iRec31[1] = iRec31[0];
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
			fVec5[1] = fVec5[0];
			fVec6[1] = fVec6[0];
			fVec7[1] = fVec7[0];
			fRec65[1] = fRec65[0];
			fRec61[1] = fRec61[0];
			fRec66[1] = fRec66[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec68[j0] = fRec68[(j0 - 1)];
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
			fRec70[1] = fRec70[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			iRec71[1] = iRec71[0];
			iRec72[1] = iRec72[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec73[1] = fRec73[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec9[1] = fRec9[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec77[1] = fRec77[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec81[1] = fRec81[0];
			fRec88[1] = fRec88[0];
			fRec87[1] = fRec87[0];
			fRec85[1] = fRec85[0];
			fRec92[1] = fRec92[0];
			fRec91[1] = fRec91[0];
			fRec89[1] = fRec89[0];
			fRec96[1] = fRec96[0];
			fRec95[1] = fRec95[0];
			fRec93[1] = fRec93[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec97[1] = fRec97[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			fRec101[1] = fRec101[0];
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
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			iRec111[1] = iRec111[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fVec26[1] = fVec26[0];
			fVec27[1] = fVec27[0];
			fVec28[1] = fVec28[0];
			iRec112[1] = iRec112[0];
			fRec113[1] = fRec113[0];
			fRec108[2] = fRec108[1];
			fRec108[1] = fRec108[0];
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
			fRec135[1] = fRec135[0];
			fRec134[1] = fRec134[0];
			fRec133[1] = fRec133[0];
			fRec136[2] = fRec136[1];
			fRec136[1] = fRec136[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec106[2] = fRec106[1];
			fRec106[1] = fRec106[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fVec29[1] = fVec29[0];
			fVec30[1] = fVec30[0];
			fVec31[1] = fVec31[0];
			iRec141[1] = iRec141[0];
			fRec140[1] = fRec140[0];
			fRec144[1] = fRec144[0];
			iRec143[1] = iRec143[0];
			fRec146[1] = fRec146[0];
			fRec145[1] = fRec145[0];
			fRec142[2] = fRec142[1];
			fRec142[1] = fRec142[0];
			fRec149[1] = fRec149[0];
			iRec148[1] = iRec148[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec152[1] = fRec152[0];
			iRec151[1] = iRec151[0];
			fRec150[2] = fRec150[1];
			fRec150[1] = fRec150[0];
			fRec155[1] = fRec155[0];
			iRec154[1] = iRec154[0];
			fRec153[2] = fRec153[1];
			fRec153[1] = fRec153[0];
			fRec158[1] = fRec158[0];
			fRec157[1] = fRec157[0];
			fRec156[1] = fRec156[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec139[2] = fRec139[1];
			fRec139[1] = fRec139[0];
			fRec138[2] = fRec138[1];
			fRec138[1] = fRec138[0];
			fRec137[2] = fRec137[1];
			fRec137[1] = fRec137[0];
			iRec163[1] = iRec163[0];
			fRec167[1] = fRec167[0];
			fVec33[1] = fVec33[0];
			fVec34[1] = fVec34[0];
			fVec35[1] = fVec35[0];
			fRec168[1] = fRec168[0];
			fRec165[1] = fRec165[0];
			fRec169[1] = fRec169[0];
			fRec172[2] = fRec172[1];
			fRec172[1] = fRec172[0];
			fRec171[2] = fRec171[1];
			fRec171[1] = fRec171[0];
			iRec173[1] = iRec173[0];
			fRec170[2] = fRec170[1];
			fRec170[1] = fRec170[0];
			fRec174[2] = fRec174[1];
			fRec174[1] = fRec174[0];
			fRec175[2] = fRec175[1];
			fRec175[1] = fRec175[0];
			fRec176[2] = fRec176[1];
			fRec176[1] = fRec176[0];
			fRec177[2] = fRec177[1];
			fRec177[1] = fRec177[0];
			fRec178[2] = fRec178[1];
			fRec178[1] = fRec178[0];
			fRec179[2] = fRec179[1];
			fRec179[1] = fRec179[0];
			fRec180[2] = fRec180[1];
			fRec180[1] = fRec180[0];
			fRec181[2] = fRec181[1];
			fRec181[1] = fRec181[0];
			fRec182[2] = fRec182[1];
			fRec182[1] = fRec182[0];
			fRec183[2] = fRec183[1];
			fRec183[1] = fRec183[0];
			fRec184[2] = fRec184[1];
			fRec184[1] = fRec184[0];
			fRec185[2] = fRec185[1];
			fRec185[1] = fRec185[0];
			fRec186[2] = fRec186[1];
			fRec186[1] = fRec186[0];
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
			fVec36[1] = fVec36[0];
			fRec166[1] = fRec166[0];
			fRec225[1] = fRec225[0];
			fRec224[1] = fRec224[0];
			fRec223[1] = fRec223[0];
			fRec226[2] = fRec226[1];
			fRec226[1] = fRec226[0];
			fRec162[2] = fRec162[1];
			fRec162[1] = fRec162[0];
			fRec161[2] = fRec161[1];
			fRec161[1] = fRec161[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			fRec230[1] = fRec230[0];
			fRec232[1] = fRec232[0];
			fRec236[1] = fRec236[0];
			fRec235[1] = fRec235[0];
			fRec234[1] = fRec234[0];
			fRec233[1] = fRec233[0];
			fRec231[1] = fRec231[0];
			fRec239[1] = fRec239[0];
			fRec238[1] = fRec238[0];
			fRec237[1] = fRec237[0];
			fRec240[2] = fRec240[1];
			fRec240[1] = fRec240[0];
			fRec229[2] = fRec229[1];
			fRec229[1] = fRec229[0];
			fRec228[2] = fRec228[1];
			fRec228[1] = fRec228[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
			fRec245[2] = fRec245[1];
			fRec245[1] = fRec245[0];
			fVec38[1] = fVec38[0];
			fVec39[1] = fVec39[0];
			fVec40[1] = fVec40[0];
			iRec246[1] = iRec246[0];
			fRec244[2] = fRec244[1];
			fRec244[1] = fRec244[0];
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
			fRec280[2] = fRec280[1];
			fRec280[1] = fRec280[0];
			fRec281[2] = fRec281[1];
			fRec281[1] = fRec281[0];
			fRec282[2] = fRec282[1];
			fRec282[1] = fRec282[0];
			fRec283[2] = fRec283[1];
			fRec283[1] = fRec283[0];
			fRec284[2] = fRec284[1];
			fRec284[1] = fRec284[0];
			fRec285[2] = fRec285[1];
			fRec285[1] = fRec285[0];
			fRec286[2] = fRec286[1];
			fRec286[1] = fRec286[0];
			fRec287[2] = fRec287[1];
			fRec287[1] = fRec287[0];
			fRec288[2] = fRec288[1];
			fRec288[1] = fRec288[0];
			fRec289[2] = fRec289[1];
			fRec289[1] = fRec289[0];
			fRec290[2] = fRec290[1];
			fRec290[1] = fRec290[0];
			fRec291[2] = fRec291[1];
			fRec291[1] = fRec291[0];
			fRec292[2] = fRec292[1];
			fRec292[1] = fRec292[0];
			fRec293[2] = fRec293[1];
			fRec293[1] = fRec293[0];
			fRec294[2] = fRec294[1];
			fRec294[1] = fRec294[0];
			fRec295[2] = fRec295[1];
			fRec295[1] = fRec295[0];
			fRec296[1] = fRec296[0];
			fRec297[1] = fRec297[0];
			fRec298[1] = fRec298[0];
			fRec299[1] = fRec299[0];
			fRec302[1] = fRec302[0];
			fRec301[1] = fRec301[0];
			fRec300[1] = fRec300[0];
			fRec303[2] = fRec303[1];
			fRec303[1] = fRec303[0];
			fRec243[2] = fRec243[1];
			fRec243[1] = fRec243[0];
			fRec242[2] = fRec242[1];
			fRec242[1] = fRec242[0];
			fRec241[2] = fRec241[1];
			fRec241[1] = fRec241[0];
			fRec306[1] = fRec306[0];
			fRec305[1] = fRec305[0];
			fRec304[1] = fRec304[0];
			fRec309[1] = fRec309[0];
			fRec308[1] = fRec308[0];
			fRec307[1] = fRec307[0];
			fRec312[1] = fRec312[0];
			fRec311[1] = fRec311[0];
			fRec310[1] = fRec310[0];
			fRec313[2] = fRec313[1];
			fRec313[1] = fRec313[0];
			iRec318[1] = iRec318[0];
			fRec317[2] = fRec317[1];
			fRec317[1] = fRec317[0];
			iRec320[1] = iRec320[0];
			fRec319[2] = fRec319[1];
			fRec319[1] = fRec319[0];
			iRec322[1] = iRec322[0];
			fRec321[2] = fRec321[1];
			fRec321[1] = fRec321[0];
			iRec324[1] = iRec324[0];
			fRec323[2] = fRec323[1];
			fRec323[1] = fRec323[0];
			fRec327[1] = fRec327[0];
			fRec326[1] = fRec326[0];
			fRec325[1] = fRec325[0];
			fRec328[2] = fRec328[1];
			fRec328[1] = fRec328[0];
			fRec316[2] = fRec316[1];
			fRec316[1] = fRec316[0];
			fRec315[2] = fRec315[1];
			fRec315[1] = fRec315[0];
			fRec314[2] = fRec314[1];
			fRec314[1] = fRec314[0];
			fRec331[1] = fRec331[0];
			fRec330[1] = fRec330[0];
			fRec329[1] = fRec329[0];
			fRec334[1] = fRec334[0];
			fRec333[1] = fRec333[0];
			fRec332[1] = fRec332[0];
			fRec337[1] = fRec337[0];
			fRec336[1] = fRec336[0];
			fRec335[1] = fRec335[0];
		}
	}

};

#endif
