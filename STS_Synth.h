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
	FAUSTFLOAT fHslider0;
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
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider2;
	float fConst28;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fConst32;
	float fConst33;
	float fRec18[2];
	float fConst34;
	float fConst35;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
	float fConst36;
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	FAUSTFLOAT fHslider9;
	float fRec23[2];
	int iRec25[2];
	FAUSTFLOAT fHslider10;
	float fRec22[3];
	float fRec26[3];
	float fRec27[3];
	float fRec28[3];
	float fRec29[3];
	float fConst37;
	float fConst38;
	FAUSTFLOAT fHslider11;
	float fVec2[2];
	float fVec3[2];
	float fVec4[2];
	int iRec30[2];
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec16[3];
	float fConst43;
	float fConst44;
	float fConst45;
	FAUSTFLOAT fHslider12;
	float fRec31[2];
	float fConst46;
	float fConst47;
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
	float fConst48;
	float fRec15[3];
	FAUSTFLOAT fHslider21;
	FAUSTFLOAT fHslider22;
	float fRec14[3];
	float fRec13[3];
	float fVec5[2048];
	int iConst49;
	float fVec6[2048];
	int iConst50;
	float fRec9[2];
	float fConst59;
	float fConst60;
	float fConst61;
	float fRec39[2];
	float fRec38[2];
	float fVec7[32768];
	int iConst63;
	float fVec8[4096];
	int iConst64;
	float fRec36[2];
	float fConst73;
	float fConst74;
	float fConst75;
	float fRec43[2];
	float fRec42[2];
	float fVec9[16384];
	int iConst77;
	float fVec10[4096];
	int iConst78;
	float fRec40[2];
	float fConst87;
	float fConst88;
	float fConst89;
	float fRec47[2];
	float fRec46[2];
	float fVec11[32768];
	int iConst91;
	float fVec12[4096];
	int iConst92;
	float fRec44[2];
	float fConst101;
	float fConst102;
	float fConst103;
	float fRec51[2];
	float fRec50[2];
	float fVec13[16384];
	int iConst105;
	float fVec14[2048];
	int iConst106;
	float fRec48[2];
	float fConst115;
	float fConst116;
	float fConst117;
	float fRec55[2];
	float fRec54[2];
	float fVec15[16384];
	int iConst119;
	float fVec16[4096];
	int iConst120;
	float fRec52[2];
	float fConst129;
	float fConst130;
	float fConst131;
	float fRec59[2];
	float fRec58[2];
	float fVec17[16384];
	int iConst133;
	float fVec18[4096];
	int iConst134;
	float fRec56[2];
	float fConst143;
	float fConst144;
	float fConst145;
	float fRec63[2];
	float fRec62[2];
	float fVec19[16384];
	int iConst147;
	float fVec20[2048];
	int iConst148;
	float fRec60[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fConst149;
	float fConst150;
	float fConst151;
	float fRec0[3];
	FAUSTFLOAT fHslider23;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	FAUSTFLOAT fHslider26;
	FAUSTFLOAT fHslider27;
	FAUSTFLOAT fHslider28;
	float fRec69[3];
	float fRec68[3];
	float fConst152;
	FAUSTFLOAT fHslider29;
	float fVec21[2];
	float fVec22[2];
	float fVec23[2];
	int iRec70[2];
	float fConst154;
	float fRec71[2];
	float fConst155;
	float fRec67[3];
	float fConst157;
	float fConst158;
	float fRec72[3];
	float fConst160;
	float fConst161;
	float fRec73[3];
	float fConst163;
	float fConst164;
	float fRec74[3];
	float fConst166;
	float fConst167;
	float fRec75[3];
	float fConst169;
	float fConst170;
	float fRec76[3];
	float fConst172;
	float fConst173;
	float fRec77[3];
	float fConst175;
	float fConst176;
	float fRec78[3];
	float fConst178;
	float fConst179;
	float fRec79[3];
	float fConst181;
	float fConst182;
	float fRec80[3];
	float fConst184;
	float fConst185;
	float fRec81[3];
	float fConst187;
	float fConst188;
	float fRec82[3];
	float fConst190;
	float fConst191;
	float fRec83[3];
	float fConst193;
	float fConst194;
	float fRec84[3];
	float fConst196;
	float fConst197;
	float fRec85[3];
	float fConst199;
	float fConst200;
	float fRec86[3];
	float fConst202;
	float fConst203;
	float fRec87[3];
	float fConst205;
	float fConst206;
	float fRec88[3];
	float fConst208;
	float fConst209;
	float fRec89[3];
	float fConst211;
	float fConst212;
	float fRec90[3];
	float fRec91[2];
	FAUSTFLOAT fHslider30;
	FAUSTFLOAT fHslider31;
	FAUSTFLOAT fHslider32;
	float fRec94[2];
	float fRec93[2];
	FAUSTFLOAT fHslider33;
	float fRec92[2];
	FAUSTFLOAT fHslider34;
	FAUSTFLOAT fHslider35;
	float fRec95[3];
	FAUSTFLOAT fHslider36;
	FAUSTFLOAT fHslider37;
	float fRec66[3];
	FAUSTFLOAT fHslider38;
	FAUSTFLOAT fHslider39;
	float fRec65[3];
	float fRec64[3];
	FAUSTFLOAT fHslider40;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider41;
	FAUSTFLOAT fHslider42;
	FAUSTFLOAT fHslider43;
	FAUSTFLOAT fHslider44;
	float fConst213;
	float fConst214;
	FAUSTFLOAT fHslider45;
	float fVec24[2];
	float fVec25[2];
	float fVec26[2];
	int iRec100[2];
	float fRec99[2];
	float fRec101[2];
	FAUSTFLOAT fHslider46;
	float fRec104[2];
	int iRec103[2];
	float fConst215;
	float fConst216;
	float fConst217;
	float fConst218;
	float fConst219;
	float fRec106[2];
	float fRec105[2];
	float fRec102[3];
	FAUSTFLOAT fHslider47;
	float fRec109[2];
	int iRec108[2];
	float fRec107[3];
	FAUSTFLOAT fHslider48;
	float fRec112[2];
	int iRec111[2];
	float fRec110[3];
	FAUSTFLOAT fHslider49;
	float fRec115[2];
	int iRec114[2];
	float fRec113[3];
	FAUSTFLOAT fHslider50;
	FAUSTFLOAT fHslider51;
	FAUSTFLOAT fHslider52;
	float fRec118[2];
	float fRec117[2];
	FAUSTFLOAT fHslider53;
	float fRec116[2];
	FAUSTFLOAT fHslider54;
	FAUSTFLOAT fHslider55;
	float fRec119[3];
	FAUSTFLOAT fHslider56;
	FAUSTFLOAT fHslider57;
	float fRec98[3];
	FAUSTFLOAT fHslider58;
	FAUSTFLOAT fHslider59;
	float fRec97[3];
	float fRec96[3];
	FAUSTFLOAT fHslider60;
	FAUSTFLOAT fCheckbox3;
	FAUSTFLOAT fHslider61;
	FAUSTFLOAT fHslider62;
	FAUSTFLOAT fHslider63;
	FAUSTFLOAT fHslider64;
	int iRec123[2];
	float fRec127[2];
	float fVec27[2048];
	float fConst220;
	float fConst221;
	FAUSTFLOAT fHslider65;
	float fVec28[2];
	float fVec29[2];
	float fVec30[2];
	float fRec128[2];
	float fRec125[2];
	float fRec129[2];
	float fConst224;
	float fConst227;
	float fConst229;
	float fConst230;
	float fConst231;
	float fRec132[3];
	float fConst232;
	float fConst233;
	float fConst234;
	float fRec131[3];
	FAUSTFLOAT fHslider66;
	int iRec133[2];
	float fConst236;
	float fConst237;
	float fRec130[3];
	float fConst238;
	float fConst239;
	float fConst241;
	float fConst242;
	float fConst243;
	float fRec134[3];
	float fConst244;
	float fConst246;
	float fConst247;
	float fConst248;
	float fRec135[3];
	float fConst249;
	float fConst251;
	float fConst252;
	float fConst253;
	float fRec136[3];
	float fConst254;
	float fConst256;
	float fConst257;
	float fConst258;
	float fRec137[3];
	float fConst259;
	float fConst261;
	float fConst262;
	float fConst263;
	float fRec138[3];
	float fConst264;
	float fConst266;
	float fConst267;
	float fConst268;
	float fRec139[3];
	float fConst269;
	float fConst271;
	float fConst272;
	float fConst273;
	float fRec140[3];
	float fConst274;
	float fConst276;
	float fConst277;
	float fConst278;
	float fRec141[3];
	float fConst279;
	float fConst281;
	float fConst282;
	float fConst283;
	float fRec142[3];
	float fConst284;
	float fConst286;
	float fConst287;
	float fConst288;
	float fRec143[3];
	float fConst289;
	float fConst291;
	float fConst292;
	float fConst293;
	float fRec144[3];
	float fConst294;
	float fConst296;
	float fConst297;
	float fConst298;
	float fRec145[3];
	float fConst299;
	float fConst301;
	float fConst302;
	float fConst303;
	float fRec146[3];
	float fConst304;
	float fConst306;
	float fConst307;
	float fConst308;
	float fRec147[3];
	float fConst309;
	float fConst311;
	float fConst312;
	float fConst313;
	float fRec148[3];
	float fConst314;
	float fConst316;
	float fConst317;
	float fConst318;
	float fRec149[3];
	float fConst319;
	float fConst321;
	float fConst322;
	float fConst323;
	float fRec150[3];
	float fConst324;
	float fConst326;
	float fConst327;
	float fConst328;
	float fRec151[3];
	float fConst329;
	float fConst331;
	float fConst332;
	float fConst333;
	float fRec152[3];
	float fConst334;
	float fConst336;
	float fConst337;
	float fConst338;
	float fRec153[3];
	float fConst339;
	float fConst341;
	float fConst342;
	float fConst343;
	float fRec154[3];
	float fConst344;
	float fConst346;
	float fConst347;
	float fConst348;
	float fRec155[3];
	float fConst349;
	float fConst351;
	float fConst352;
	float fConst353;
	float fRec156[3];
	float fConst354;
	float fConst356;
	float fConst357;
	float fConst358;
	float fRec157[3];
	float fConst359;
	float fConst361;
	float fConst362;
	float fConst363;
	float fRec158[3];
	float fConst364;
	float fConst366;
	float fConst367;
	float fConst368;
	float fRec159[3];
	float fConst369;
	float fConst371;
	float fConst372;
	float fConst373;
	float fRec160[3];
	float fConst374;
	float fConst376;
	float fConst377;
	float fConst378;
	float fRec161[3];
	float fConst379;
	float fConst381;
	float fConst382;
	float fConst383;
	float fRec162[3];
	float fConst384;
	float fConst386;
	float fConst387;
	float fConst388;
	float fRec163[3];
	float fConst389;
	float fConst391;
	float fConst392;
	float fConst393;
	float fRec164[3];
	float fConst394;
	float fConst396;
	float fConst397;
	float fConst398;
	float fRec165[3];
	float fConst399;
	float fConst401;
	float fConst402;
	float fConst403;
	float fRec166[3];
	float fConst404;
	float fConst406;
	float fConst407;
	float fConst408;
	float fRec167[3];
	float fConst409;
	float fConst411;
	float fConst412;
	float fConst413;
	float fRec168[3];
	float fConst414;
	float fConst416;
	float fConst417;
	float fConst418;
	float fRec169[3];
	float fConst419;
	float fConst421;
	float fConst422;
	float fConst423;
	float fRec170[3];
	float fConst424;
	float fConst426;
	float fConst427;
	float fConst428;
	float fRec171[3];
	float fConst429;
	float fConst431;
	float fConst432;
	float fConst433;
	float fRec172[3];
	float fConst434;
	float fConst436;
	float fConst437;
	float fConst438;
	float fRec173[3];
	float fConst439;
	float fConst441;
	float fConst442;
	float fConst443;
	float fRec174[3];
	float fConst444;
	float fConst446;
	float fConst447;
	float fConst448;
	float fRec175[3];
	float fConst449;
	float fConst451;
	float fConst452;
	float fConst453;
	float fRec176[3];
	float fConst454;
	float fConst456;
	float fConst457;
	float fConst458;
	float fRec177[3];
	float fConst459;
	float fConst461;
	float fConst462;
	float fConst463;
	float fRec178[3];
	float fConst464;
	float fConst466;
	float fConst467;
	float fConst468;
	float fRec179[3];
	float fConst469;
	float fConst471;
	float fConst472;
	float fConst473;
	float fRec180[3];
	float fConst474;
	float fConst476;
	float fConst477;
	float fConst478;
	float fRec181[3];
	float fConst479;
	float fConst481;
	float fConst482;
	float fConst483;
	float fRec182[3];
	float fConst484;
	float fVec31[2];
	float fVec32[2048];
	float fRec126[2];
	float fRec183[2];
	FAUSTFLOAT fHslider67;
	FAUSTFLOAT fHslider68;
	FAUSTFLOAT fHslider69;
	float fRec186[2];
	float fRec185[2];
	FAUSTFLOAT fHslider70;
	float fRec184[2];
	FAUSTFLOAT fHslider71;
	FAUSTFLOAT fHslider72;
	float fRec187[3];
	FAUSTFLOAT fHslider73;
	FAUSTFLOAT fHslider74;
	float fRec122[3];
	FAUSTFLOAT fHslider75;
	FAUSTFLOAT fHslider76;
	float fRec121[3];
	float fRec120[3];
	FAUSTFLOAT fHslider77;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fHslider78;
	FAUSTFLOAT fHslider79;
	FAUSTFLOAT fHslider80;
	FAUSTFLOAT fHslider81;
	float fConst485;
	float fRec191[2];
	float fRec193[2];
	float fConst486;
	float fRec197[2];
	float fRec196[2];
	float fRec195[2];
	float fRec194[2];
	float fRec192[2];
	FAUSTFLOAT fHslider82;
	FAUSTFLOAT fHslider83;
	FAUSTFLOAT fHslider84;
	float fRec200[2];
	float fRec199[2];
	FAUSTFLOAT fHslider85;
	float fRec198[2];
	FAUSTFLOAT fHslider86;
	FAUSTFLOAT fHslider87;
	float fRec201[3];
	FAUSTFLOAT fHslider88;
	FAUSTFLOAT fHslider89;
	float fRec190[3];
	FAUSTFLOAT fHslider90;
	FAUSTFLOAT fHslider91;
	float fRec189[3];
	float fRec188[3];
	FAUSTFLOAT fHslider92;
	FAUSTFLOAT fCheckbox5;
	FAUSTFLOAT fHslider93;
	FAUSTFLOAT fHslider94;
	FAUSTFLOAT fHslider95;
	FAUSTFLOAT fHslider96;
	float fConst487;
	FAUSTFLOAT fHslider97;
	float fVec33[2];
	float fVec34[2];
	float fVec35[2];
	float fRec230[2];
	float fRec226[2];
	FAUSTFLOAT fHslider98;
	float fRec231[2];
	float fRec233[4];
	float fRec234[1024];
	float fConst488;
	float fVec36[2];
	float fVec37[3];
	float fRec232[2048];
	float fRec222[2];
	float fRec218[2];
	float fRec214[1024];
	float fRec212[4];
	float fRec207[2048];
	float fRec205[2];
	int iRec235[2];
	float fRec236[2];
	FAUSTFLOAT fHslider99;
	FAUSTFLOAT fHslider100;
	FAUSTFLOAT fHslider101;
	float fRec239[2];
	float fRec238[2];
	FAUSTFLOAT fHslider102;
	float fRec237[2];
	FAUSTFLOAT fHslider103;
	FAUSTFLOAT fHslider104;
	float fRec240[3];
	FAUSTFLOAT fHslider105;
	FAUSTFLOAT fHslider106;
	float fRec204[3];
	FAUSTFLOAT fHslider107;
	FAUSTFLOAT fHslider108;
	float fRec203[3];
	float fRec202[3];
	FAUSTFLOAT fHslider109;
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fHslider110;
	FAUSTFLOAT fHslider111;
	FAUSTFLOAT fHslider112;
	FAUSTFLOAT fHslider113;
	float fConst489;
	float fConst492;
	float fConst493;
	float fConst494;
	float fRec245[3];
	float fConst495;
	FAUSTFLOAT fHslider114;
	float fVec38[2];
	float fVec39[2];
	float fVec40[2];
	int iRec246[2];
	float fConst497;
	float fConst498;
	float fRec244[3];
	float fConst499;
	float fConst501;
	float fConst502;
	float fRec247[3];
	float fConst503;
	float fConst505;
	float fConst506;
	float fRec248[3];
	float fConst507;
	float fConst509;
	float fConst510;
	float fRec249[3];
	float fConst511;
	float fConst513;
	float fConst514;
	float fRec250[3];
	float fConst515;
	float fConst517;
	float fConst518;
	float fRec251[3];
	float fConst519;
	float fConst521;
	float fConst522;
	float fRec252[3];
	float fConst523;
	float fConst525;
	float fConst526;
	float fRec253[3];
	float fConst527;
	float fConst529;
	float fConst530;
	float fRec254[3];
	float fConst531;
	float fConst533;
	float fConst534;
	float fRec255[3];
	float fConst535;
	float fConst537;
	float fConst538;
	float fRec256[3];
	float fConst539;
	float fConst541;
	float fConst542;
	float fRec257[3];
	float fConst543;
	float fConst545;
	float fConst546;
	float fRec258[3];
	float fConst547;
	float fConst549;
	float fConst550;
	float fRec259[3];
	float fConst551;
	float fConst553;
	float fConst554;
	float fRec260[3];
	float fConst555;
	float fConst557;
	float fConst558;
	float fRec261[3];
	float fConst559;
	float fConst561;
	float fConst562;
	float fRec262[3];
	float fConst563;
	float fConst565;
	float fConst566;
	float fRec263[3];
	float fConst567;
	float fConst569;
	float fConst570;
	float fRec264[3];
	float fConst571;
	float fConst573;
	float fConst574;
	float fRec265[3];
	float fConst575;
	float fConst577;
	float fConst578;
	float fRec266[3];
	float fConst579;
	float fConst581;
	float fConst582;
	float fRec267[3];
	float fConst583;
	float fConst585;
	float fConst586;
	float fRec268[3];
	float fConst587;
	float fConst589;
	float fConst590;
	float fRec269[3];
	float fConst591;
	float fConst593;
	float fConst594;
	float fRec270[3];
	float fConst595;
	float fConst597;
	float fConst598;
	float fRec271[3];
	float fConst599;
	float fConst601;
	float fConst602;
	float fRec272[3];
	float fConst603;
	float fConst605;
	float fConst606;
	float fRec273[3];
	float fConst607;
	float fConst609;
	float fConst610;
	float fRec274[3];
	float fConst611;
	float fConst613;
	float fConst614;
	float fRec275[3];
	float fConst615;
	float fConst617;
	float fConst618;
	float fRec276[3];
	float fConst619;
	float fConst621;
	float fConst622;
	float fRec277[3];
	float fConst623;
	float fConst625;
	float fConst626;
	float fRec278[3];
	float fConst627;
	float fConst629;
	float fConst630;
	float fRec279[3];
	float fConst631;
	float fConst633;
	float fConst634;
	float fRec280[3];
	float fConst635;
	float fConst637;
	float fConst638;
	float fRec281[3];
	float fConst639;
	float fConst641;
	float fConst642;
	float fRec282[3];
	float fConst643;
	float fConst645;
	float fConst646;
	float fRec283[3];
	float fConst647;
	float fConst649;
	float fConst650;
	float fRec284[3];
	float fConst651;
	float fConst653;
	float fConst654;
	float fRec285[3];
	float fConst655;
	float fConst657;
	float fConst658;
	float fRec286[3];
	float fConst659;
	float fConst661;
	float fConst662;
	float fRec287[3];
	float fConst663;
	float fConst665;
	float fConst666;
	float fRec288[3];
	float fConst667;
	float fConst669;
	float fConst670;
	float fRec289[3];
	float fConst671;
	float fConst673;
	float fConst674;
	float fRec290[3];
	float fConst675;
	float fConst677;
	float fConst678;
	float fRec291[3];
	float fConst679;
	float fConst681;
	float fConst682;
	float fRec292[3];
	float fConst683;
	float fConst685;
	float fConst686;
	float fRec293[3];
	float fConst687;
	float fConst689;
	float fConst690;
	float fRec294[3];
	float fConst691;
	float fConst693;
	float fConst694;
	float fRec295[3];
	float fConst695;
	FAUSTFLOAT fHslider115;
	FAUSTFLOAT fHslider116;
	FAUSTFLOAT fHslider117;
	float fRec298[2];
	float fRec297[2];
	FAUSTFLOAT fHslider118;
	float fRec296[2];
	FAUSTFLOAT fHslider119;
	FAUSTFLOAT fHslider120;
	float fRec299[3];
	FAUSTFLOAT fHslider121;
	FAUSTFLOAT fHslider122;
	float fRec243[3];
	FAUSTFLOAT fHslider123;
	FAUSTFLOAT fHslider124;
	float fRec242[3];
	float fRec241[3];
	float fConst696;
	float fConst697;
	float fConst698;
	float fConst699;
	float fRec302[2];
	float fRec301[2];
	float fRec300[2];
	float fConst700;
	float fConst701;
	float fConst702;
	float fConst703;
	float fRec305[2];
	float fRec304[2];
	float fRec303[2];
	float fConst704;
	float fConst705;
	float fRec308[2];
	float fRec307[2];
	float fRec306[2];
	float fRec309[3];
	int iRec314[2];
	float fRec313[3];
	int iRec316[2];
	float fRec315[3];
	int iRec318[2];
	float fRec317[3];
	int iRec320[2];
	float fRec319[3];
	float fRec323[2];
	float fRec322[2];
	float fRec321[2];
	float fRec324[3];
	float fRec312[3];
	float fRec311[3];
	float fRec310[3];
	float fRec327[2];
	float fRec326[2];
	float fRec325[2];
	float fRec330[2];
	float fRec329[2];
	float fRec328[2];
	float fRec333[2];
	float fRec332[2];
	float fRec331[2];
	
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
		iConst49 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.00999999978f * fConst0))));
		iConst50 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		float fConst51 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst52 = ((0.0f - (6.90775537f * fConst51)) / fConst0);
		float fConst53 = std::exp((0.25f * fConst52));
		float fConst54 = mydsp_faustpower2_f(fConst53);
		float fConst55 = (1.0f - (fConst54 * fConst15));
		float fConst56 = (1.0f - fConst54);
		float fConst57 = (fConst55 / fConst56);
		float fConst58 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst55) / mydsp_faustpower2_f(fConst56)) + -1.0f)));
		fConst59 = (fConst57 - fConst58);
		fConst60 = (fConst53 * (fConst58 + (1.0f - fConst57)));
		fConst61 = ((std::exp(fConst52) / fConst53) + -1.0f);
		float fConst62 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst63 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst51 - fConst62))));
		iConst64 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst62 + -1.0f))));
		float fConst65 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst66 = ((0.0f - (6.90775537f * fConst65)) / fConst0);
		float fConst67 = std::exp((0.25f * fConst66));
		float fConst68 = mydsp_faustpower2_f(fConst67);
		float fConst69 = (1.0f - (fConst68 * fConst15));
		float fConst70 = (1.0f - fConst68);
		float fConst71 = (fConst69 / fConst70);
		float fConst72 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst69) / mydsp_faustpower2_f(fConst70)) + -1.0f)));
		fConst73 = (fConst71 - fConst72);
		fConst74 = (fConst67 * (fConst72 + (1.0f - fConst71)));
		fConst75 = ((std::exp(fConst66) / fConst67) + -1.0f);
		float fConst76 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst77 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst65 - fConst76))));
		iConst78 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst76 + -1.0f))));
		float fConst79 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst80 = ((0.0f - (6.90775537f * fConst79)) / fConst0);
		float fConst81 = std::exp((0.25f * fConst80));
		float fConst82 = mydsp_faustpower2_f(fConst81);
		float fConst83 = (1.0f - (fConst82 * fConst15));
		float fConst84 = (1.0f - fConst82);
		float fConst85 = (fConst83 / fConst84);
		float fConst86 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst83) / mydsp_faustpower2_f(fConst84)) + -1.0f)));
		fConst87 = (fConst85 - fConst86);
		fConst88 = (fConst81 * (fConst86 + (1.0f - fConst85)));
		fConst89 = ((std::exp(fConst80) / fConst81) + -1.0f);
		float fConst90 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst91 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst79 - fConst90))));
		iConst92 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst90 + -1.0f))));
		float fConst93 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst94 = ((0.0f - (6.90775537f * fConst93)) / fConst0);
		float fConst95 = std::exp((0.25f * fConst94));
		float fConst96 = mydsp_faustpower2_f(fConst95);
		float fConst97 = (1.0f - (fConst96 * fConst15));
		float fConst98 = (1.0f - fConst96);
		float fConst99 = (fConst97 / fConst98);
		float fConst100 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst97) / mydsp_faustpower2_f(fConst98)) + -1.0f)));
		fConst101 = (fConst99 - fConst100);
		fConst102 = (fConst95 * (fConst100 + (1.0f - fConst99)));
		fConst103 = ((std::exp(fConst94) / fConst95) + -1.0f);
		float fConst104 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst105 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst93 - fConst104))));
		iConst106 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst104 + -1.0f))));
		float fConst107 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst108 = ((0.0f - (6.90775537f * fConst107)) / fConst0);
		float fConst109 = std::exp((0.25f * fConst108));
		float fConst110 = mydsp_faustpower2_f(fConst109);
		float fConst111 = (1.0f - (fConst110 * fConst15));
		float fConst112 = (1.0f - fConst110);
		float fConst113 = (fConst111 / fConst112);
		float fConst114 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst111) / mydsp_faustpower2_f(fConst112)) + -1.0f)));
		fConst115 = (fConst113 - fConst114);
		fConst116 = (fConst109 * (fConst114 + (1.0f - fConst113)));
		fConst117 = ((std::exp(fConst108) / fConst109) + -1.0f);
		float fConst118 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst119 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst107 - fConst118))));
		iConst120 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst118 + -1.0f))));
		float fConst121 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst122 = ((0.0f - (6.90775537f * fConst121)) / fConst0);
		float fConst123 = std::exp((0.25f * fConst122));
		float fConst124 = mydsp_faustpower2_f(fConst123);
		float fConst125 = (1.0f - (fConst124 * fConst15));
		float fConst126 = (1.0f - fConst124);
		float fConst127 = (fConst125 / fConst126);
		float fConst128 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst125) / mydsp_faustpower2_f(fConst126)) + -1.0f)));
		fConst129 = (fConst127 - fConst128);
		fConst130 = (fConst123 * (fConst128 + (1.0f - fConst127)));
		fConst131 = ((std::exp(fConst122) / fConst123) + -1.0f);
		float fConst132 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst133 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst121 - fConst132))));
		iConst134 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst132 + -1.0f))));
		float fConst135 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst136 = ((0.0f - (6.90775537f * fConst135)) / fConst0);
		float fConst137 = std::exp((0.25f * fConst136));
		float fConst138 = mydsp_faustpower2_f(fConst137);
		float fConst139 = (1.0f - (fConst138 * fConst15));
		float fConst140 = (1.0f - fConst138);
		float fConst141 = (fConst139 / fConst140);
		float fConst142 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst139) / mydsp_faustpower2_f(fConst140)) + -1.0f)));
		fConst143 = (fConst141 - fConst142);
		fConst144 = (fConst137 * (fConst142 + (1.0f - fConst141)));
		fConst145 = ((std::exp(fConst136) / fConst137) + -1.0f);
		float fConst146 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst147 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst135 - fConst146))));
		iConst148 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst146 + -1.0f))));
		fConst149 = (1.0f / fConst8);
		fConst150 = ((2.0f * fConst6) + -8.0f);
		fConst151 = (fConst6 + (4.0f - fConst7));
		fConst152 = (0.00200000009f * fConst0);
		float fConst153 = std::pow(0.00100000005f, (1.66666663f / fConst0));
		fConst154 = (0.0f - (2.0f * fConst153));
		fConst155 = mydsp_faustpower2_f(fConst153);
		float fConst156 = std::pow(0.00100000005f, (1.75438595f / fConst0));
		fConst157 = (0.0f - (2.0f * fConst156));
		fConst158 = mydsp_faustpower2_f(fConst156);
		float fConst159 = std::pow(0.00100000005f, (1.85185182f / fConst0));
		fConst160 = (0.0f - (2.0f * fConst159));
		fConst161 = mydsp_faustpower2_f(fConst159);
		float fConst162 = std::pow(0.00100000005f, (1.96078432f / fConst0));
		fConst163 = (0.0f - (2.0f * fConst162));
		fConst164 = mydsp_faustpower2_f(fConst162);
		float fConst165 = std::pow(0.00100000005f, (2.08333325f / fConst0));
		fConst166 = (0.0f - (2.0f * fConst165));
		fConst167 = mydsp_faustpower2_f(fConst165);
		float fConst168 = std::pow(0.00100000005f, (2.22222233f / fConst0));
		fConst169 = (0.0f - (2.0f * fConst168));
		fConst170 = mydsp_faustpower2_f(fConst168);
		float fConst171 = std::pow(0.00100000005f, (2.38095236f / fConst0));
		fConst172 = (0.0f - (2.0f * fConst171));
		fConst173 = mydsp_faustpower2_f(fConst171);
		float fConst174 = std::pow(0.00100000005f, (2.56410265f / fConst0));
		fConst175 = (0.0f - (2.0f * fConst174));
		fConst176 = mydsp_faustpower2_f(fConst174);
		float fConst177 = std::pow(0.00100000005f, (2.77777767f / fConst0));
		fConst178 = (0.0f - (2.0f * fConst177));
		fConst179 = mydsp_faustpower2_f(fConst177);
		float fConst180 = std::pow(0.00100000005f, (3.030303f / fConst0));
		fConst181 = (0.0f - (2.0f * fConst180));
		fConst182 = mydsp_faustpower2_f(fConst180);
		float fConst183 = std::pow(0.00100000005f, (3.33333325f / fConst0));
		fConst184 = (0.0f - (2.0f * fConst183));
		fConst185 = mydsp_faustpower2_f(fConst183);
		float fConst186 = std::pow(0.00100000005f, (3.70370364f / fConst0));
		fConst187 = (0.0f - (2.0f * fConst186));
		fConst188 = mydsp_faustpower2_f(fConst186);
		float fConst189 = std::pow(0.00100000005f, (4.16666651f / fConst0));
		fConst190 = (0.0f - (2.0f * fConst189));
		fConst191 = mydsp_faustpower2_f(fConst189);
		float fConst192 = std::pow(0.00100000005f, (4.76190472f / fConst0));
		fConst193 = (0.0f - (2.0f * fConst192));
		fConst194 = mydsp_faustpower2_f(fConst192);
		float fConst195 = std::pow(0.00100000005f, (5.55555534f / fConst0));
		fConst196 = (0.0f - (2.0f * fConst195));
		fConst197 = mydsp_faustpower2_f(fConst195);
		float fConst198 = std::pow(0.00100000005f, (6.66666651f / fConst0));
		fConst199 = (0.0f - (2.0f * fConst198));
		fConst200 = mydsp_faustpower2_f(fConst198);
		float fConst201 = std::pow(0.00100000005f, (8.33333302f / fConst0));
		fConst202 = (0.0f - (2.0f * fConst201));
		fConst203 = mydsp_faustpower2_f(fConst201);
		float fConst204 = std::pow(0.00100000005f, (11.1111107f / fConst0));
		fConst205 = (0.0f - (2.0f * fConst204));
		fConst206 = mydsp_faustpower2_f(fConst204);
		float fConst207 = std::pow(0.00100000005f, (16.666666f / fConst0));
		fConst208 = (0.0f - (2.0f * fConst207));
		fConst209 = mydsp_faustpower2_f(fConst207);
		float fConst210 = std::pow(0.00100000005f, (33.3333321f / fConst0));
		fConst211 = (0.0f - (2.0f * fConst210));
		fConst212 = mydsp_faustpower2_f(fConst210);
		fConst213 = std::exp((0.0f - (500.0f / fConst0)));
		fConst214 = (1.0f - fConst213);
		fConst215 = (0.330000013f * fConst0);
		fConst216 = (0.100000001f * fConst0);
		fConst217 = (0.660000026f * fConst0);
		fConst218 = std::exp((0.0f - (100.0f / fConst0)));
		fConst219 = (1.0f - fConst218);
		fConst220 = (0.00882352982f * fConst0);
		fConst221 = (0.00147058826f * fConst0);
		float fConst222 = std::tan((37699.1133f / fConst0));
		float fConst223 = (1.0f / fConst222);
		fConst224 = (1.0f / (((fConst223 + 1.41421354f) / fConst222) + 1.0f));
		float fConst225 = std::tan((31.415926f / fConst0));
		float fConst226 = (1.0f / fConst225);
		fConst227 = (1.0f / (((fConst226 + 1.41421354f) / fConst225) + 1.0f));
		float fConst228 = mydsp_faustpower2_f(fConst225);
		fConst229 = (1.0f / fConst228);
		fConst230 = (((fConst226 + -1.41421354f) / fConst225) + 1.0f);
		fConst231 = (2.0f * (1.0f - fConst229));
		fConst232 = (0.0f - (2.0f / fConst228));
		fConst233 = (((fConst223 + -1.41421354f) / fConst222) + 1.0f);
		fConst234 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst222))));
		float fConst235 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst236 = (0.0f - (2.0f * fConst235));
		fConst237 = mydsp_faustpower2_f(fConst235);
		fConst238 = ((0.5f * fConst0) + -1.0f);
		fConst239 = ftbl1mydspSIG1[0];
		float fConst240 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst241 = (0.0f - (2.0f * fConst240));
		fConst242 = (20.8197117f / fConst0);
		fConst243 = mydsp_faustpower2_f(fConst240);
		fConst244 = ftbl1mydspSIG1[1];
		float fConst245 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst246 = (0.0f - (2.0f * fConst245));
		fConst247 = (24.0940685f / fConst0);
		fConst248 = mydsp_faustpower2_f(fConst245);
		fConst249 = ftbl1mydspSIG1[2];
		float fConst250 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst251 = (0.0f - (2.0f * fConst250));
		fConst252 = (50.6621399f / fConst0);
		fConst253 = mydsp_faustpower2_f(fConst250);
		fConst254 = ftbl1mydspSIG1[3];
		float fConst255 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst256 = (0.0f - (2.0f * fConst255));
		fConst257 = (59.3621521f / fConst0);
		fConst258 = mydsp_faustpower2_f(fConst255);
		fConst259 = ftbl1mydspSIG1[4];
		float fConst260 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst261 = (0.0f - (2.0f * fConst260));
		fConst262 = (88.6990967f / fConst0);
		fConst263 = mydsp_faustpower2_f(fConst260);
		fConst264 = ftbl1mydspSIG1[5];
		float fConst265 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst266 = (0.0f - (2.0f * fConst265));
		fConst267 = (115.510078f / fConst0);
		fConst268 = mydsp_faustpower2_f(fConst265);
		fConst269 = ftbl1mydspSIG1[6];
		float fConst270 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst271 = (0.0f - (2.0f * fConst270));
		fConst272 = (132.010986f / fConst0);
		fConst273 = mydsp_faustpower2_f(fConst270);
		fConst274 = ftbl1mydspSIG1[7];
		float fConst275 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst276 = (0.0f - (2.0f * fConst275));
		fConst277 = (164.478088f / fConst0);
		fConst278 = mydsp_faustpower2_f(fConst275);
		fConst279 = ftbl1mydspSIG1[8];
		float fConst280 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst281 = (0.0f - (2.0f * fConst280));
		fConst282 = (182.177185f / fConst0);
		fConst283 = mydsp_faustpower2_f(fConst280);
		fConst284 = ftbl1mydspSIG1[9];
		float fConst285 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst286 = (0.0f - (2.0f * fConst285));
		fConst287 = (232.935272f / fConst0);
		fConst288 = mydsp_faustpower2_f(fConst285);
		fConst289 = ftbl1mydspSIG1[10];
		float fConst290 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst291 = (0.0f - (2.0f * fConst290));
		fConst292 = (237.946106f / fConst0);
		fConst293 = mydsp_faustpower2_f(fConst290);
		fConst294 = ftbl1mydspSIG1[11];
		float fConst295 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst296 = (0.0f - (2.0f * fConst295));
		fConst297 = (251.725769f / fConst0);
		fConst298 = mydsp_faustpower2_f(fConst295);
		fConst299 = ftbl1mydspSIG1[12];
		float fConst300 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst301 = (0.0f - (2.0f * fConst300));
		fConst302 = (299.355438f / fConst0);
		fConst303 = mydsp_faustpower2_f(fConst300);
		fConst304 = ftbl1mydspSIG1[13];
		float fConst305 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst306 = (0.0f - (2.0f * fConst305));
		fConst307 = (320.561829f / fConst0);
		fConst308 = mydsp_faustpower2_f(fConst305);
		fConst309 = ftbl1mydspSIG1[14];
		float fConst310 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst311 = (0.0f - (2.0f * fConst310));
		fConst312 = (329.427399f / fConst0);
		fConst313 = mydsp_faustpower2_f(fConst310);
		fConst314 = ftbl1mydspSIG1[15];
		float fConst315 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst316 = (0.0f - (2.0f * fConst315));
		fConst317 = (366.221741f / fConst0);
		fConst318 = mydsp_faustpower2_f(fConst315);
		fConst319 = ftbl1mydspSIG1[16];
		float fConst320 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst321 = (0.0f - (2.0f * fConst320));
		fConst322 = (399.287628f / fConst0);
		fConst323 = mydsp_faustpower2_f(fConst320);
		fConst324 = ftbl1mydspSIG1[17];
		float fConst325 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst326 = (0.0f - (2.0f * fConst325));
		fConst327 = (410.686584f / fConst0);
		fConst328 = mydsp_faustpower2_f(fConst325);
		fConst329 = ftbl1mydspSIG1[18];
		float fConst330 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst331 = (0.0f - (2.0f * fConst330));
		fConst332 = (420.713928f / fConst0);
		fConst333 = mydsp_faustpower2_f(fConst330);
		fConst334 = ftbl1mydspSIG1[19];
		float fConst335 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst336 = (0.0f - (2.0f * fConst335));
		fConst337 = (468.286438f / fConst0);
		fConst338 = mydsp_faustpower2_f(fConst335);
		fConst339 = ftbl1mydspSIG1[20];
		float fConst340 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst341 = (0.0f - (2.0f * fConst340));
		fConst342 = (494.436432f / fConst0);
		fConst343 = mydsp_faustpower2_f(fConst340);
		fConst344 = ftbl1mydspSIG1[21];
		float fConst345 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst346 = (0.0f - (2.0f * fConst345));
		fConst347 = (507.916992f / fConst0);
		fConst348 = mydsp_faustpower2_f(fConst345);
		fConst349 = ftbl1mydspSIG1[22];
		float fConst350 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst351 = (0.0f - (2.0f * fConst350));
		fConst352 = (565.34845f / fConst0);
		fConst353 = mydsp_faustpower2_f(fConst350);
		fConst354 = ftbl1mydspSIG1[23];
		float fConst355 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst356 = (0.0f - (2.0f * fConst355));
		fConst357 = (584.12323f / fConst0);
		fConst358 = mydsp_faustpower2_f(fConst355);
		fConst359 = ftbl1mydspSIG1[24];
		float fConst360 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst361 = (0.0f - (2.0f * fConst360));
		fConst362 = (598.105225f / fConst0);
		fConst363 = mydsp_faustpower2_f(fConst360);
		fConst364 = ftbl1mydspSIG1[25];
		float fConst365 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst366 = (0.0f - (2.0f * fConst365));
		fConst367 = (612.489319f / fConst0);
		fConst368 = mydsp_faustpower2_f(fConst365);
		fConst369 = ftbl1mydspSIG1[26];
		float fConst370 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst371 = (0.0f - (2.0f * fConst370));
		fConst372 = (695.045959f / fConst0);
		fConst373 = mydsp_faustpower2_f(fConst370);
		fConst374 = ftbl1mydspSIG1[27];
		float fConst375 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst376 = (0.0f - (2.0f * fConst375));
		fConst377 = (704.150269f / fConst0);
		fConst378 = mydsp_faustpower2_f(fConst375);
		fConst379 = ftbl1mydspSIG1[28];
		float fConst380 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst381 = (0.0f - (2.0f * fConst380));
		fConst382 = (715.18988f / fConst0);
		fConst383 = mydsp_faustpower2_f(fConst380);
		fConst384 = ftbl1mydspSIG1[29];
		float fConst385 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst386 = (0.0f - (2.0f * fConst385));
		fConst387 = (749.935852f / fConst0);
		fConst388 = mydsp_faustpower2_f(fConst385);
		fConst389 = ftbl1mydspSIG1[30];
		float fConst390 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst391 = (0.0f - (2.0f * fConst390));
		fConst392 = (798.247253f / fConst0);
		fConst393 = mydsp_faustpower2_f(fConst390);
		fConst394 = ftbl1mydspSIG1[31];
		float fConst395 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst396 = (0.0f - (2.0f * fConst395));
		fConst397 = (816.700989f / fConst0);
		fConst398 = mydsp_faustpower2_f(fConst395);
		fConst399 = ftbl1mydspSIG1[32];
		float fConst400 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst401 = (0.0f - (2.0f * fConst400));
		fConst402 = (831.007812f / fConst0);
		fConst403 = mydsp_faustpower2_f(fConst400);
		fConst404 = ftbl1mydspSIG1[33];
		float fConst405 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst406 = (0.0f - (2.0f * fConst405));
		fConst407 = (838.660706f / fConst0);
		fConst408 = mydsp_faustpower2_f(fConst405);
		fConst409 = ftbl1mydspSIG1[34];
		float fConst410 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst411 = (0.0f - (2.0f * fConst410));
		fConst412 = (908.228149f / fConst0);
		fConst413 = mydsp_faustpower2_f(fConst410);
		fConst414 = ftbl1mydspSIG1[35];
		float fConst415 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst416 = (0.0f - (2.0f * fConst415));
		fConst417 = (938.946655f / fConst0);
		fConst418 = mydsp_faustpower2_f(fConst415);
		fConst419 = ftbl1mydspSIG1[36];
		float fConst420 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst421 = (0.0f - (2.0f * fConst420));
		fConst422 = (955.251526f / fConst0);
		fConst423 = mydsp_faustpower2_f(fConst420);
		fConst424 = ftbl1mydspSIG1[37];
		float fConst425 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst426 = (0.0f - (2.0f * fConst425));
		fConst427 = (962.370361f / fConst0);
		fConst428 = mydsp_faustpower2_f(fConst425);
		fConst429 = ftbl1mydspSIG1[38];
		float fConst430 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst431 = (0.0f - (2.0f * fConst430));
		fConst432 = (977.644775f / fConst0);
		fConst433 = mydsp_faustpower2_f(fConst430);
		fConst434 = ftbl1mydspSIG1[39];
		float fConst435 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst436 = (0.0f - (2.0f * fConst435));
		fConst437 = (993.893127f / fConst0);
		fConst438 = mydsp_faustpower2_f(fConst435);
		fConst439 = ftbl1mydspSIG1[40];
		float fConst440 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst441 = (0.0f - (2.0f * fConst440));
		fConst442 = (1056.23486f / fConst0);
		fConst443 = mydsp_faustpower2_f(fConst440);
		fConst444 = ftbl1mydspSIG1[41];
		float fConst445 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst446 = (0.0f - (2.0f * fConst445));
		fConst447 = (1079.84705f / fConst0);
		fConst448 = mydsp_faustpower2_f(fConst445);
		fConst449 = ftbl1mydspSIG1[42];
		float fConst450 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst451 = (0.0f - (2.0f * fConst450));
		fConst452 = (1096.1897f / fConst0);
		fConst453 = mydsp_faustpower2_f(fConst450);
		fConst454 = ftbl1mydspSIG1[43];
		float fConst455 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst456 = (0.0f - (2.0f * fConst455));
		fConst457 = (1124.29431f / fConst0);
		fConst458 = mydsp_faustpower2_f(fConst455);
		fConst459 = ftbl1mydspSIG1[44];
		float fConst460 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst461 = (0.0f - (2.0f * fConst460));
		fConst462 = (1140.28503f / fConst0);
		fConst463 = mydsp_faustpower2_f(fConst460);
		fConst464 = ftbl1mydspSIG1[45];
		float fConst465 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst466 = (0.0f - (2.0f * fConst465));
		fConst467 = (1164.89001f / fConst0);
		fConst468 = mydsp_faustpower2_f(fConst465);
		fConst469 = ftbl1mydspSIG1[46];
		float fConst470 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst471 = (0.0f - (2.0f * fConst470));
		fConst472 = (1196.12366f / fConst0);
		fConst473 = mydsp_faustpower2_f(fConst470);
		fConst474 = ftbl1mydspSIG1[47];
		float fConst475 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst476 = (0.0f - (2.0f * fConst475));
		fConst477 = (1207.56543f / fConst0);
		fConst478 = mydsp_faustpower2_f(fConst475);
		fConst479 = ftbl1mydspSIG1[48];
		float fConst480 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst481 = (0.0f - (2.0f * fConst480));
		fConst482 = (1228.39417f / fConst0);
		fConst483 = mydsp_faustpower2_f(fConst480);
		fConst484 = ftbl1mydspSIG1[49];
		fConst485 = (1.0f / std::max<float>(1.0f, (3.0f * fConst0)));
		fConst486 = (81.0f / fConst0);
		fConst487 = (0.00117647054f * fConst0);
		fConst488 = (0.000294117635f * fConst0);
		fConst489 = ftbl2mydspSIG2[50];
		float fConst490 = std::tan((31415.9258f / fConst0));
		float fConst491 = (1.0f / fConst490);
		fConst492 = (1.0f / (((fConst491 + 1.41421354f) / fConst490) + 1.0f));
		fConst493 = (((fConst491 + -1.41421354f) / fConst490) + 1.0f);
		fConst494 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst490))));
		fConst495 = (1.0f / std::max<float>(1.0f, (0.00159999996f * fConst0)));
		float fConst496 = std::pow(0.00100000005f, (0.0448570587f / fConst0));
		fConst497 = (std::cos((2839.48462f / fConst0)) * (0.0f - (2.0f * fConst496)));
		fConst498 = mydsp_faustpower2_f(fConst496);
		fConst499 = ftbl2mydspSIG2[51];
		float fConst500 = std::pow(0.00100000005f, (0.0449536517f / fConst0));
		fConst501 = (std::cos((2858.84937f / fConst0)) * (0.0f - (2.0f * fConst500)));
		fConst502 = mydsp_faustpower2_f(fConst500);
		fConst503 = ftbl2mydspSIG2[52];
		float fConst504 = std::pow(0.00100000005f, (0.0609094799f / fConst0));
		fConst505 = (std::cos((5432.7124f / fConst0)) * (0.0f - (2.0f * fConst504)));
		fConst506 = mydsp_faustpower2_f(fConst504);
		fConst507 = ftbl2mydspSIG2[53];
		float fConst508 = std::pow(0.00100000005f, (0.0612352863f / fConst0));
		fConst509 = (std::cos((5475.18018f / fConst0)) * (0.0f - (2.0f * fConst508)));
		fConst510 = mydsp_faustpower2_f(fConst508);
		fConst511 = ftbl2mydspSIG2[54];
		float fConst512 = std::pow(0.00100000005f, (0.0721585602f / fConst0));
		fConst513 = (std::cos((6738.52783f / fConst0)) * (0.0f - (2.0f * fConst512)));
		fConst514 = mydsp_faustpower2_f(fConst512);
		fConst515 = ftbl2mydspSIG2[55];
		float fConst516 = std::pow(0.00100000005f, (0.0722505748f / fConst0));
		fConst517 = (std::cos((6748.01514f / fConst0)) * (0.0f - (2.0f * fConst516)));
		fConst518 = mydsp_faustpower2_f(fConst516);
		fConst519 = ftbl2mydspSIG2[56];
		float fConst520 = std::pow(0.00100000005f, (0.0874899849f / fConst0));
		fConst521 = (std::cos((8119.32031f / fConst0)) * (0.0f - (2.0f * fConst520)));
		fConst522 = mydsp_faustpower2_f(fConst520);
		fConst523 = ftbl2mydspSIG2[57];
		float fConst524 = std::pow(0.00100000005f, (0.0875099227f / fConst0));
		fConst525 = (std::cos((8120.89111f / fConst0)) * (0.0f - (2.0f * fConst524)));
		fConst526 = mydsp_faustpower2_f(fConst524);
		fConst527 = ftbl2mydspSIG2[58];
		float fConst528 = std::pow(0.00100000005f, (0.107014902f / fConst0));
		fConst529 = (std::cos((9453.68066f / fConst0)) * (0.0f - (2.0f * fConst528)));
		fConst530 = mydsp_faustpower2_f(fConst528);
		fConst531 = ftbl2mydspSIG2[59];
		float fConst532 = std::pow(0.00100000005f, (0.110012196f / fConst0));
		fConst533 = (std::cos((9628.41602f / fConst0)) * (0.0f - (2.0f * fConst532)));
		fConst534 = mydsp_faustpower2_f(fConst532);
		fConst535 = ftbl2mydspSIG2[60];
		float fConst536 = std::pow(0.00100000005f, (0.123582378f / fConst0));
		fConst537 = (std::cos((10343.3799f / fConst0)) * (0.0f - (2.0f * fConst536)));
		fConst538 = mydsp_faustpower2_f(fConst536);
		fConst539 = ftbl2mydspSIG2[61];
		float fConst540 = std::pow(0.00100000005f, (0.123685889f / fConst0));
		fConst541 = (std::cos((10348.4062f / fConst0)) * (0.0f - (2.0f * fConst540)));
		fConst542 = mydsp_faustpower2_f(fConst540);
		fConst543 = ftbl2mydspSIG2[62];
		float fConst544 = std::pow(0.00100000005f, (0.127769172f / fConst0));
		fConst545 = (std::cos((10542.1172f / fConst0)) * (0.0f - (2.0f * fConst544)));
		fConst546 = mydsp_faustpower2_f(fConst544);
		fConst547 = ftbl2mydspSIG2[63];
		float fConst548 = std::pow(0.00100000005f, (0.127809823f / fConst0));
		fConst549 = (std::cos((10544.002f / fConst0)) * (0.0f - (2.0f * fConst548)));
		fConst550 = mydsp_faustpower2_f(fConst548);
		fConst551 = ftbl2mydspSIG2[64];
		float fConst552 = std::pow(0.00100000005f, (0.157417208f / fConst0));
		fConst553 = (std::cos((11728.3818f / fConst0)) * (0.0f - (2.0f * fConst552)));
		fConst554 = mydsp_faustpower2_f(fConst552);
		fConst555 = ftbl2mydspSIG2[65];
		float fConst556 = std::pow(0.00100000005f, (0.160256028f / fConst0));
		fConst557 = (std::cos((11825.457f / fConst0)) * (0.0f - (2.0f * fConst556)));
		fConst558 = mydsp_faustpower2_f(fConst556);
		fConst559 = ftbl2mydspSIG2[66];
		float fConst560 = std::pow(0.00100000005f, (0.18117331f / fConst0));
		fConst561 = (std::cos((12473.3799f / fConst0)) * (0.0f - (2.0f * fConst560)));
		fConst562 = mydsp_faustpower2_f(fConst560);
		fConst563 = ftbl2mydspSIG2[67];
		float fConst564 = std::pow(0.00100000005f, (0.182209119f / fConst0));
		fConst565 = (std::cos((12502.7217f / fConst0)) * (0.0f - (2.0f * fConst564)));
		fConst566 = mydsp_faustpower2_f(fConst564);
		fConst567 = ftbl2mydspSIG2[68];
		float fConst568 = std::pow(0.00100000005f, (0.2133362f / fConst0));
		fConst569 = (std::cos((13288.4971f / fConst0)) * (0.0f - (2.0f * fConst568)));
		fConst570 = mydsp_faustpower2_f(fConst568);
		fConst571 = ftbl2mydspSIG2[69];
		float fConst572 = std::pow(0.00100000005f, (0.298688531f / fConst0));
		fConst573 = (std::cos((14808.2744f / fConst0)) * (0.0f - (2.0f * fConst572)));
		fConst574 = mydsp_faustpower2_f(fConst572);
		fConst575 = ftbl2mydspSIG2[70];
		float fConst576 = std::pow(0.00100000005f, (0.341466933f / fConst0));
		fConst577 = (std::cos((15358.1152f / fConst0)) * (0.0f - (2.0f * fConst576)));
		fConst578 = mydsp_faustpower2_f(fConst576);
		fConst579 = ftbl2mydspSIG2[71];
		float fConst580 = std::pow(0.00100000005f, (0.342488438f / fConst0));
		fConst581 = (std::cos((15370.0537f / fConst0)) * (0.0f - (2.0f * fConst580)));
		fConst582 = mydsp_faustpower2_f(fConst580);
		fConst583 = ftbl2mydspSIG2[72];
		float fConst584 = std::pow(0.00100000005f, (0.391345769f / fConst0));
		fConst585 = (std::cos((15888.7305f / fConst0)) * (0.0f - (2.0f * fConst584)));
		fConst586 = mydsp_faustpower2_f(fConst584);
		fConst587 = ftbl2mydspSIG2[73];
		float fConst588 = std::pow(0.00100000005f, (0.392745376f / fConst0));
		fConst589 = (std::cos((15902.2393f / fConst0)) * (0.0f - (2.0f * fConst588)));
		fConst590 = mydsp_faustpower2_f(fConst588);
		fConst591 = ftbl2mydspSIG2[74];
		float fConst592 = std::pow(0.00100000005f, (0.499480247f / fConst0));
		fConst593 = (std::cos((16768.9414f / fConst0)) * (0.0f - (2.0f * fConst592)));
		fConst594 = mydsp_faustpower2_f(fConst592);
		fConst595 = ftbl2mydspSIG2[75];
		float fConst596 = std::pow(0.00100000005f, (0.500184536f / fConst0));
		fConst597 = (std::cos((16773.7793f / fConst0)) * (0.0f - (2.0f * fConst596)));
		fConst598 = mydsp_faustpower2_f(fConst596);
		fConst599 = ftbl2mydspSIG2[76];
		float fConst600 = std::pow(0.00100000005f, (0.56877172f / fConst0));
		fConst601 = (std::cos((17203.7383f / fConst0)) * (0.0f - (2.0f * fConst600)));
		fConst602 = mydsp_faustpower2_f(fConst600);
		fConst603 = ftbl2mydspSIG2[77];
		float fConst604 = std::pow(0.00100000005f, (0.580974996f / fConst0));
		fConst605 = (std::cos((17272.6641f / fConst0)) * (0.0f - (2.0f * fConst604)));
		fConst606 = mydsp_faustpower2_f(fConst604);
		fConst607 = ftbl2mydspSIG2[78];
		float fConst608 = std::pow(0.00100000005f, (0.58267206f / fConst0));
		fConst609 = (std::cos((17282.0898f / fConst0)) * (0.0f - (2.0f * fConst608)));
		fConst610 = mydsp_faustpower2_f(fConst608);
		fConst611 = ftbl2mydspSIG2[79];
		float fConst612 = std::pow(0.00100000005f, (0.58560729f / fConst0));
		fConst613 = (std::cos((17298.3008f / fConst0)) * (0.0f - (2.0f * fConst612)));
		fConst614 = mydsp_faustpower2_f(fConst612);
		fConst615 = ftbl2mydspSIG2[80];
		float fConst616 = std::pow(0.00100000005f, (0.679538369f / fConst0));
		fConst617 = (std::cos((17763.7578f / fConst0)) * (0.0f - (2.0f * fConst616)));
		fConst618 = mydsp_faustpower2_f(fConst616);
		fConst619 = ftbl2mydspSIG2[81];
		float fConst620 = std::pow(0.00100000005f, (0.701286793f / fConst0));
		fConst621 = (std::cos((17858.8242f / fConst0)) * (0.0f - (2.0f * fConst620)));
		fConst622 = mydsp_faustpower2_f(fConst620);
		fConst623 = ftbl2mydspSIG2[82];
		float fConst624 = std::pow(0.00100000005f, (0.981793463f / fConst0));
		fConst625 = (std::cos((18802.8086f / fConst0)) * (0.0f - (2.0f * fConst624)));
		fConst626 = mydsp_faustpower2_f(fConst624);
		fConst627 = ftbl2mydspSIG2[83];
		float fConst628 = std::pow(0.00100000005f, (0.991939485f / fConst0));
		fConst629 = (std::cos((18829.7012f / fConst0)) * (0.0f - (2.0f * fConst628)));
		fConst630 = mydsp_faustpower2_f(fConst628);
		fConst631 = ftbl2mydspSIG2[84];
		float fConst632 = std::pow(0.00100000005f, (1.57685912f / fConst0));
		fConst633 = (std::cos((19934.0957f / fConst0)) * (0.0f - (2.0f * fConst632)));
		fConst634 = mydsp_faustpower2_f(fConst632);
		fConst635 = ftbl2mydspSIG2[85];
		float fConst636 = std::pow(0.00100000005f, (2.61620831f / fConst0));
		fConst637 = (std::cos((20927.9707f / fConst0)) * (0.0f - (2.0f * fConst636)));
		fConst638 = mydsp_faustpower2_f(fConst636);
		fConst639 = ftbl2mydspSIG2[86];
		float fConst640 = std::pow(0.00100000005f, (3.26228237f / fConst0));
		fConst641 = (std::cos((21302.0723f / fConst0)) * (0.0f - (2.0f * fConst640)));
		fConst642 = mydsp_faustpower2_f(fConst640);
		fConst643 = ftbl2mydspSIG2[87];
		float fConst644 = std::pow(0.00100000005f, (3.42948842f / fConst0));
		fConst645 = (std::cos((21382.3086f / fConst0)) * (0.0f - (2.0f * fConst644)));
		fConst646 = mydsp_faustpower2_f(fConst644);
		fConst647 = ftbl2mydspSIG2[88];
		float fConst648 = std::pow(0.00100000005f, (5.61909866f / fConst0));
		fConst649 = (std::cos((22094.3809f / fConst0)) * (0.0f - (2.0f * fConst648)));
		fConst650 = mydsp_faustpower2_f(fConst648);
		fConst651 = ftbl2mydspSIG2[89];
		float fConst652 = std::pow(0.00100000005f, (6.26517916f / fConst0));
		fConst653 = (std::cos((22233.3027f / fConst0)) * (0.0f - (2.0f * fConst652)));
		fConst654 = mydsp_faustpower2_f(fConst652);
		fConst655 = ftbl2mydspSIG2[90];
		float fConst656 = std::pow(0.00100000005f, (7.91042233f / fConst0));
		fConst657 = (std::cos((22511.334f / fConst0)) * (0.0f - (2.0f * fConst656)));
		fConst658 = mydsp_faustpower2_f(fConst656);
		fConst659 = ftbl2mydspSIG2[91];
		float fConst660 = std::pow(0.00100000005f, (7.95560217f / fConst0));
		fConst661 = (std::cos((22517.8047f / fConst0)) * (0.0f - (2.0f * fConst660)));
		fConst662 = mydsp_faustpower2_f(fConst660);
		fConst663 = ftbl2mydspSIG2[92];
		float fConst664 = std::pow(0.00100000005f, (21.1559925f / fConst0));
		fConst665 = (std::cos((23436.5332f / fConst0)) * (0.0f - (2.0f * fConst664)));
		fConst666 = mydsp_faustpower2_f(fConst664);
		fConst667 = ftbl2mydspSIG2[93];
		float fConst668 = std::pow(0.00100000005f, (22.8584423f / fConst0));
		fConst669 = (std::cos((23495.0293f / fConst0)) * (0.0f - (2.0f * fConst668)));
		fConst670 = mydsp_faustpower2_f(fConst668);
		fConst671 = ftbl2mydspSIG2[94];
		float fConst672 = std::pow(0.00100000005f, (27.0563507f / fConst0));
		fConst673 = (std::cos((23616.3574f / fConst0)) * (0.0f - (2.0f * fConst672)));
		fConst674 = mydsp_faustpower2_f(fConst672);
		fConst675 = ftbl2mydspSIG2[95];
		float fConst676 = std::pow(0.00100000005f, (173.368958f / fConst0));
		fConst677 = (std::cos((24528.1738f / fConst0)) * (0.0f - (2.0f * fConst676)));
		fConst678 = mydsp_faustpower2_f(fConst676);
		fConst679 = ftbl2mydspSIG2[96];
		float fConst680 = std::pow(0.00100000005f, (415.098541f / fConst0));
		fConst681 = (std::cos((24772.0234f / fConst0)) * (0.0f - (2.0f * fConst680)));
		fConst682 = mydsp_faustpower2_f(fConst680);
		fConst683 = ftbl2mydspSIG2[97];
		float fConst684 = std::pow(0.00100000005f, (1631.8125f / fConst0));
		fConst685 = (std::cos((25018.0098f / fConst0)) * (0.0f - (2.0f * fConst684)));
		fConst686 = mydsp_faustpower2_f(fConst684);
		fConst687 = ftbl2mydspSIG2[98];
		float fConst688 = std::pow(0.00100000005f, (1752.41809f / fConst0));
		fConst689 = (std::cos((25027.498f / fConst0)) * (0.0f - (2.0f * fConst688)));
		fConst690 = mydsp_faustpower2_f(fConst688);
		fConst691 = ftbl2mydspSIG2[99];
		float fConst692 = std::pow(0.00100000005f, (13161720.0f / fConst0));
		fConst693 = (std::cos((25346.1816f / fConst0)) * (0.0f - (2.0f * fConst692)));
		fConst694 = mydsp_faustpower2_f(fConst692);
		fConst695 = (1.0f / std::max<float>(1.0f, (2.0f * fConst0)));
		fConst696 = std::exp((0.0f - (0.00999999978f / fConst0)));
		fConst697 = (1.0f - fConst696);
		fConst698 = std::exp((0.0f - (0.00333333341f / fConst0)));
		fConst699 = (1.0f - fConst698);
		fConst700 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst701 = (1.0f - fConst700);
		fConst702 = std::exp((0.0f - (0.00499999989f / fConst0)));
		fConst703 = (1.0f - fConst702);
		fConst704 = std::exp((0.0f - (1000.0f / fConst0)));
		fConst705 = (1.0f - fConst704);
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
		fHslider6 = FAUSTFLOAT(440.0f);
		fHslider7 = FAUSTFLOAT(0.5f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(0.0f);
		fHslider11 = FAUSTFLOAT(0.0f);
		fHslider12 = FAUSTFLOAT(7.0f);
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
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider24 = FAUSTFLOAT(0.69999999999999996f);
		fHslider25 = FAUSTFLOAT(20000.0f);
		fHslider26 = FAUSTFLOAT(2000.0f);
		fHslider27 = FAUSTFLOAT(500.0f);
		fHslider28 = FAUSTFLOAT(1.0f);
		fHslider29 = FAUSTFLOAT(0.0f);
		fHslider30 = FAUSTFLOAT(1.0f);
		fHslider31 = FAUSTFLOAT(0.10000000000000001f);
		fHslider32 = FAUSTFLOAT(0.10000000000000001f);
		fHslider33 = FAUSTFLOAT(0.0f);
		fHslider34 = FAUSTFLOAT(0.69999999999999996f);
		fHslider35 = FAUSTFLOAT(50.0f);
		fHslider36 = FAUSTFLOAT(0.0f);
		fHslider37 = FAUSTFLOAT(0.69999999999999996f);
		fHslider38 = FAUSTFLOAT(0.0f);
		fHslider39 = FAUSTFLOAT(0.69999999999999996f);
		fHslider40 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider41 = FAUSTFLOAT(0.69999999999999996f);
		fHslider42 = FAUSTFLOAT(20000.0f);
		fHslider43 = FAUSTFLOAT(2000.0f);
		fHslider44 = FAUSTFLOAT(500.0f);
		fHslider45 = FAUSTFLOAT(0.0f);
		fHslider46 = FAUSTFLOAT(440.0f);
		fHslider47 = FAUSTFLOAT(440.0f);
		fHslider48 = FAUSTFLOAT(440.0f);
		fHslider49 = FAUSTFLOAT(440.0f);
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
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider61 = FAUSTFLOAT(0.69999999999999996f);
		fHslider62 = FAUSTFLOAT(20000.0f);
		fHslider63 = FAUSTFLOAT(2000.0f);
		fHslider64 = FAUSTFLOAT(500.0f);
		fHslider65 = FAUSTFLOAT(0.0f);
		fHslider66 = FAUSTFLOAT(1.0f);
		fHslider67 = FAUSTFLOAT(1.0f);
		fHslider68 = FAUSTFLOAT(0.10000000000000001f);
		fHslider69 = FAUSTFLOAT(0.10000000000000001f);
		fHslider70 = FAUSTFLOAT(0.0f);
		fHslider71 = FAUSTFLOAT(0.69999999999999996f);
		fHslider72 = FAUSTFLOAT(50.0f);
		fHslider73 = FAUSTFLOAT(0.0f);
		fHslider74 = FAUSTFLOAT(0.69999999999999996f);
		fHslider75 = FAUSTFLOAT(0.0f);
		fHslider76 = FAUSTFLOAT(0.69999999999999996f);
		fHslider77 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider78 = FAUSTFLOAT(0.69999999999999996f);
		fHslider79 = FAUSTFLOAT(20000.0f);
		fHslider80 = FAUSTFLOAT(2000.0f);
		fHslider81 = FAUSTFLOAT(500.0f);
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
		fHslider97 = FAUSTFLOAT(0.0f);
		fHslider98 = FAUSTFLOAT(0.01f);
		fHslider99 = FAUSTFLOAT(1.0f);
		fHslider100 = FAUSTFLOAT(0.10000000000000001f);
		fHslider101 = FAUSTFLOAT(0.10000000000000001f);
		fHslider102 = FAUSTFLOAT(0.0f);
		fHslider103 = FAUSTFLOAT(0.69999999999999996f);
		fHslider104 = FAUSTFLOAT(50.0f);
		fHslider105 = FAUSTFLOAT(0.0f);
		fHslider106 = FAUSTFLOAT(0.69999999999999996f);
		fHslider107 = FAUSTFLOAT(0.0f);
		fHslider108 = FAUSTFLOAT(0.69999999999999996f);
		fHslider109 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider110 = FAUSTFLOAT(0.69999999999999996f);
		fHslider111 = FAUSTFLOAT(20000.0f);
		fHslider112 = FAUSTFLOAT(2000.0f);
		fHslider113 = FAUSTFLOAT(500.0f);
		fHslider114 = FAUSTFLOAT(0.0f);
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
			fRec21[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec20[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec19[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec23[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec25[l10] = 0;
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec22[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec26[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec27[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec28[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec29[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec2[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fVec3[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec4[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			iRec30[l19] = 0;
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec16[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec31[l21] = 0.0f;
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
		for (int l29 = 0; (l29 < 2048); l29 = (l29 + 1)) {
			fVec5[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2048); l30 = (l30 + 1)) {
			fVec6[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec9[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec39[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec38[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 32768); l34 = (l34 + 1)) {
			fVec7[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 4096); l35 = (l35 + 1)) {
			fVec8[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec36[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec43[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec42[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 16384); l39 = (l39 + 1)) {
			fVec9[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 4096); l40 = (l40 + 1)) {
			fVec10[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec40[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec47[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec46[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 32768); l44 = (l44 + 1)) {
			fVec11[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 4096); l45 = (l45 + 1)) {
			fVec12[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec44[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec51[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec50[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 16384); l49 = (l49 + 1)) {
			fVec13[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2048); l50 = (l50 + 1)) {
			fVec14[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec48[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec55[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec54[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 16384); l54 = (l54 + 1)) {
			fVec15[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 4096); l55 = (l55 + 1)) {
			fVec16[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec52[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec59[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec58[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 16384); l59 = (l59 + 1)) {
			fVec17[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 4096); l60 = (l60 + 1)) {
			fVec18[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec56[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec63[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec62[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 16384); l64 = (l64 + 1)) {
			fVec19[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2048); l65 = (l65 + 1)) {
			fVec20[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec60[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec1[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec2[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec3[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec4[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec5[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec6[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec7[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec8[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec0[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec69[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec68[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fVec21[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fVec22[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fVec23[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			iRec70[l81] = 0;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec71[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec67[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec72[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec73[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec74[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec75[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 3); l88 = (l88 + 1)) {
			fRec76[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 3); l89 = (l89 + 1)) {
			fRec77[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec78[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec79[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec80[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 3); l93 = (l93 + 1)) {
			fRec81[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec82[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec83[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec85[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec86[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec87[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec88[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec89[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec90[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec91[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec94[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec93[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec92[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec95[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec66[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec65[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 3); l110 = (l110 + 1)) {
			fRec64[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fVec24[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fVec25[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fVec26[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			iRec100[l114] = 0;
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec99[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec101[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec104[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			iRec103[l118] = 0;
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec106[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec105[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 3); l121 = (l121 + 1)) {
			fRec102[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fRec109[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			iRec108[l123] = 0;
		}
		for (int l124 = 0; (l124 < 3); l124 = (l124 + 1)) {
			fRec107[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec112[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			iRec111[l126] = 0;
		}
		for (int l127 = 0; (l127 < 3); l127 = (l127 + 1)) {
			fRec110[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec115[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			iRec114[l129] = 0;
		}
		for (int l130 = 0; (l130 < 3); l130 = (l130 + 1)) {
			fRec113[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec118[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec117[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec116[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 3); l134 = (l134 + 1)) {
			fRec119[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 3); l135 = (l135 + 1)) {
			fRec98[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 3); l136 = (l136 + 1)) {
			fRec97[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 3); l137 = (l137 + 1)) {
			fRec96[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 2); l138 = (l138 + 1)) {
			iRec123[l138] = 0;
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec127[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 2048); l140 = (l140 + 1)) {
			fVec27[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fVec28[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			fVec29[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fVec30[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fRec128[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			fRec125[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec129[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 3); l147 = (l147 + 1)) {
			fRec132[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 3); l148 = (l148 + 1)) {
			fRec131[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			iRec133[l149] = 0;
		}
		for (int l150 = 0; (l150 < 3); l150 = (l150 + 1)) {
			fRec130[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 3); l151 = (l151 + 1)) {
			fRec134[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 3); l152 = (l152 + 1)) {
			fRec135[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 3); l153 = (l153 + 1)) {
			fRec136[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 3); l154 = (l154 + 1)) {
			fRec137[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 3); l155 = (l155 + 1)) {
			fRec138[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 3); l156 = (l156 + 1)) {
			fRec139[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 3); l157 = (l157 + 1)) {
			fRec140[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 3); l158 = (l158 + 1)) {
			fRec141[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 3); l159 = (l159 + 1)) {
			fRec142[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 3); l160 = (l160 + 1)) {
			fRec143[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 3); l161 = (l161 + 1)) {
			fRec144[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 3); l162 = (l162 + 1)) {
			fRec145[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 3); l163 = (l163 + 1)) {
			fRec146[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 3); l164 = (l164 + 1)) {
			fRec147[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 3); l165 = (l165 + 1)) {
			fRec148[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 3); l166 = (l166 + 1)) {
			fRec149[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 3); l167 = (l167 + 1)) {
			fRec150[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 3); l168 = (l168 + 1)) {
			fRec151[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 3); l169 = (l169 + 1)) {
			fRec152[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 3); l170 = (l170 + 1)) {
			fRec153[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 3); l171 = (l171 + 1)) {
			fRec154[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec155[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 3); l173 = (l173 + 1)) {
			fRec156[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 3); l174 = (l174 + 1)) {
			fRec157[l174] = 0.0f;
		}
		for (int l175 = 0; (l175 < 3); l175 = (l175 + 1)) {
			fRec158[l175] = 0.0f;
		}
		for (int l176 = 0; (l176 < 3); l176 = (l176 + 1)) {
			fRec159[l176] = 0.0f;
		}
		for (int l177 = 0; (l177 < 3); l177 = (l177 + 1)) {
			fRec160[l177] = 0.0f;
		}
		for (int l178 = 0; (l178 < 3); l178 = (l178 + 1)) {
			fRec161[l178] = 0.0f;
		}
		for (int l179 = 0; (l179 < 3); l179 = (l179 + 1)) {
			fRec162[l179] = 0.0f;
		}
		for (int l180 = 0; (l180 < 3); l180 = (l180 + 1)) {
			fRec163[l180] = 0.0f;
		}
		for (int l181 = 0; (l181 < 3); l181 = (l181 + 1)) {
			fRec164[l181] = 0.0f;
		}
		for (int l182 = 0; (l182 < 3); l182 = (l182 + 1)) {
			fRec165[l182] = 0.0f;
		}
		for (int l183 = 0; (l183 < 3); l183 = (l183 + 1)) {
			fRec166[l183] = 0.0f;
		}
		for (int l184 = 0; (l184 < 3); l184 = (l184 + 1)) {
			fRec167[l184] = 0.0f;
		}
		for (int l185 = 0; (l185 < 3); l185 = (l185 + 1)) {
			fRec168[l185] = 0.0f;
		}
		for (int l186 = 0; (l186 < 3); l186 = (l186 + 1)) {
			fRec169[l186] = 0.0f;
		}
		for (int l187 = 0; (l187 < 3); l187 = (l187 + 1)) {
			fRec170[l187] = 0.0f;
		}
		for (int l188 = 0; (l188 < 3); l188 = (l188 + 1)) {
			fRec171[l188] = 0.0f;
		}
		for (int l189 = 0; (l189 < 3); l189 = (l189 + 1)) {
			fRec172[l189] = 0.0f;
		}
		for (int l190 = 0; (l190 < 3); l190 = (l190 + 1)) {
			fRec173[l190] = 0.0f;
		}
		for (int l191 = 0; (l191 < 3); l191 = (l191 + 1)) {
			fRec174[l191] = 0.0f;
		}
		for (int l192 = 0; (l192 < 3); l192 = (l192 + 1)) {
			fRec175[l192] = 0.0f;
		}
		for (int l193 = 0; (l193 < 3); l193 = (l193 + 1)) {
			fRec176[l193] = 0.0f;
		}
		for (int l194 = 0; (l194 < 3); l194 = (l194 + 1)) {
			fRec177[l194] = 0.0f;
		}
		for (int l195 = 0; (l195 < 3); l195 = (l195 + 1)) {
			fRec178[l195] = 0.0f;
		}
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec179[l196] = 0.0f;
		}
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec180[l197] = 0.0f;
		}
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec181[l198] = 0.0f;
		}
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec182[l199] = 0.0f;
		}
		for (int l200 = 0; (l200 < 2); l200 = (l200 + 1)) {
			fVec31[l200] = 0.0f;
		}
		for (int l201 = 0; (l201 < 2048); l201 = (l201 + 1)) {
			fVec32[l201] = 0.0f;
		}
		for (int l202 = 0; (l202 < 2); l202 = (l202 + 1)) {
			fRec126[l202] = 0.0f;
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fRec183[l203] = 0.0f;
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec186[l204] = 0.0f;
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec185[l205] = 0.0f;
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec184[l206] = 0.0f;
		}
		for (int l207 = 0; (l207 < 3); l207 = (l207 + 1)) {
			fRec187[l207] = 0.0f;
		}
		for (int l208 = 0; (l208 < 3); l208 = (l208 + 1)) {
			fRec122[l208] = 0.0f;
		}
		for (int l209 = 0; (l209 < 3); l209 = (l209 + 1)) {
			fRec121[l209] = 0.0f;
		}
		for (int l210 = 0; (l210 < 3); l210 = (l210 + 1)) {
			fRec120[l210] = 0.0f;
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec191[l211] = 0.0f;
		}
		for (int l212 = 0; (l212 < 2); l212 = (l212 + 1)) {
			fRec193[l212] = 0.0f;
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec197[l213] = 0.0f;
		}
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec196[l214] = 0.0f;
		}
		for (int l215 = 0; (l215 < 2); l215 = (l215 + 1)) {
			fRec195[l215] = 0.0f;
		}
		for (int l216 = 0; (l216 < 2); l216 = (l216 + 1)) {
			fRec194[l216] = 0.0f;
		}
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			fRec192[l217] = 0.0f;
		}
		for (int l218 = 0; (l218 < 2); l218 = (l218 + 1)) {
			fRec200[l218] = 0.0f;
		}
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fRec199[l219] = 0.0f;
		}
		for (int l220 = 0; (l220 < 2); l220 = (l220 + 1)) {
			fRec198[l220] = 0.0f;
		}
		for (int l221 = 0; (l221 < 3); l221 = (l221 + 1)) {
			fRec201[l221] = 0.0f;
		}
		for (int l222 = 0; (l222 < 3); l222 = (l222 + 1)) {
			fRec190[l222] = 0.0f;
		}
		for (int l223 = 0; (l223 < 3); l223 = (l223 + 1)) {
			fRec189[l223] = 0.0f;
		}
		for (int l224 = 0; (l224 < 3); l224 = (l224 + 1)) {
			fRec188[l224] = 0.0f;
		}
		for (int l225 = 0; (l225 < 2); l225 = (l225 + 1)) {
			fVec33[l225] = 0.0f;
		}
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fVec34[l226] = 0.0f;
		}
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fVec35[l227] = 0.0f;
		}
		for (int l228 = 0; (l228 < 2); l228 = (l228 + 1)) {
			fRec230[l228] = 0.0f;
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec226[l229] = 0.0f;
		}
		for (int l230 = 0; (l230 < 2); l230 = (l230 + 1)) {
			fRec231[l230] = 0.0f;
		}
		for (int l231 = 0; (l231 < 4); l231 = (l231 + 1)) {
			fRec233[l231] = 0.0f;
		}
		for (int l232 = 0; (l232 < 1024); l232 = (l232 + 1)) {
			fRec234[l232] = 0.0f;
		}
		for (int l233 = 0; (l233 < 2); l233 = (l233 + 1)) {
			fVec36[l233] = 0.0f;
		}
		for (int l234 = 0; (l234 < 3); l234 = (l234 + 1)) {
			fVec37[l234] = 0.0f;
		}
		for (int l235 = 0; (l235 < 2048); l235 = (l235 + 1)) {
			fRec232[l235] = 0.0f;
		}
		for (int l236 = 0; (l236 < 2); l236 = (l236 + 1)) {
			fRec222[l236] = 0.0f;
		}
		for (int l237 = 0; (l237 < 2); l237 = (l237 + 1)) {
			fRec218[l237] = 0.0f;
		}
		for (int l238 = 0; (l238 < 1024); l238 = (l238 + 1)) {
			fRec214[l238] = 0.0f;
		}
		for (int l239 = 0; (l239 < 4); l239 = (l239 + 1)) {
			fRec212[l239] = 0.0f;
		}
		for (int l240 = 0; (l240 < 2048); l240 = (l240 + 1)) {
			fRec207[l240] = 0.0f;
		}
		for (int l241 = 0; (l241 < 2); l241 = (l241 + 1)) {
			fRec205[l241] = 0.0f;
		}
		for (int l242 = 0; (l242 < 2); l242 = (l242 + 1)) {
			iRec235[l242] = 0;
		}
		for (int l243 = 0; (l243 < 2); l243 = (l243 + 1)) {
			fRec236[l243] = 0.0f;
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
			fRec204[l248] = 0.0f;
		}
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec203[l249] = 0.0f;
		}
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec202[l250] = 0.0f;
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
			fRec298[l306] = 0.0f;
		}
		for (int l307 = 0; (l307 < 2); l307 = (l307 + 1)) {
			fRec297[l307] = 0.0f;
		}
		for (int l308 = 0; (l308 < 2); l308 = (l308 + 1)) {
			fRec296[l308] = 0.0f;
		}
		for (int l309 = 0; (l309 < 3); l309 = (l309 + 1)) {
			fRec299[l309] = 0.0f;
		}
		for (int l310 = 0; (l310 < 3); l310 = (l310 + 1)) {
			fRec243[l310] = 0.0f;
		}
		for (int l311 = 0; (l311 < 3); l311 = (l311 + 1)) {
			fRec242[l311] = 0.0f;
		}
		for (int l312 = 0; (l312 < 3); l312 = (l312 + 1)) {
			fRec241[l312] = 0.0f;
		}
		for (int l313 = 0; (l313 < 2); l313 = (l313 + 1)) {
			fRec302[l313] = 0.0f;
		}
		for (int l314 = 0; (l314 < 2); l314 = (l314 + 1)) {
			fRec301[l314] = 0.0f;
		}
		for (int l315 = 0; (l315 < 2); l315 = (l315 + 1)) {
			fRec300[l315] = 0.0f;
		}
		for (int l316 = 0; (l316 < 2); l316 = (l316 + 1)) {
			fRec305[l316] = 0.0f;
		}
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec304[l317] = 0.0f;
		}
		for (int l318 = 0; (l318 < 2); l318 = (l318 + 1)) {
			fRec303[l318] = 0.0f;
		}
		for (int l319 = 0; (l319 < 2); l319 = (l319 + 1)) {
			fRec308[l319] = 0.0f;
		}
		for (int l320 = 0; (l320 < 2); l320 = (l320 + 1)) {
			fRec307[l320] = 0.0f;
		}
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec306[l321] = 0.0f;
		}
		for (int l322 = 0; (l322 < 3); l322 = (l322 + 1)) {
			fRec309[l322] = 0.0f;
		}
		for (int l323 = 0; (l323 < 2); l323 = (l323 + 1)) {
			iRec314[l323] = 0;
		}
		for (int l324 = 0; (l324 < 3); l324 = (l324 + 1)) {
			fRec313[l324] = 0.0f;
		}
		for (int l325 = 0; (l325 < 2); l325 = (l325 + 1)) {
			iRec316[l325] = 0;
		}
		for (int l326 = 0; (l326 < 3); l326 = (l326 + 1)) {
			fRec315[l326] = 0.0f;
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
			fRec323[l331] = 0.0f;
		}
		for (int l332 = 0; (l332 < 2); l332 = (l332 + 1)) {
			fRec322[l332] = 0.0f;
		}
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			fRec321[l333] = 0.0f;
		}
		for (int l334 = 0; (l334 < 3); l334 = (l334 + 1)) {
			fRec324[l334] = 0.0f;
		}
		for (int l335 = 0; (l335 < 3); l335 = (l335 + 1)) {
			fRec312[l335] = 0.0f;
		}
		for (int l336 = 0; (l336 < 3); l336 = (l336 + 1)) {
			fRec311[l336] = 0.0f;
		}
		for (int l337 = 0; (l337 < 3); l337 = (l337 + 1)) {
			fRec310[l337] = 0.0f;
		}
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec327[l338] = 0.0f;
		}
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fRec326[l339] = 0.0f;
		}
		for (int l340 = 0; (l340 < 2); l340 = (l340 + 1)) {
			fRec325[l340] = 0.0f;
		}
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fRec330[l341] = 0.0f;
		}
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec329[l342] = 0.0f;
		}
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec328[l343] = 0.0f;
		}
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec333[l344] = 0.0f;
		}
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fRec332[l345] = 0.0f;
		}
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec331[l346] = 0.0f;
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
		ui_interface->declare(&fHslider31, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider31, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider30, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider30, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider32, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider32, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider33, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider33, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider14, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider13, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider15, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider16, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->declare(&fHslider51, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider51, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider50, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider50, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider52, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider52, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider53, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider53, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->declare(&fHslider68, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider68, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider67, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider67, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider69, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider69, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider70, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider70, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->declare(&fHslider83, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider83, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider82, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider82, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider84, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider84, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider85, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider85, 0.0f, -96.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->declare(&fHslider100, "style", "knob");
		ui_interface->addHorizontalSlider("Attack", &fHslider100, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider99, "style", "knob");
		ui_interface->addHorizontalSlider("Ratio", &fHslider99, 1.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fHslider101, "style", "knob");
		ui_interface->addHorizontalSlider("Release", &fHslider101, 0.100000001f, 0.00100000005f, 0.5f, 0.00100000005f);
		ui_interface->declare(&fHslider102, "style", "knob");
		ui_interface->addHorizontalSlider("Threshold", &fHslider102, 0.0f, -96.0f, 0.0f, 0.100000001f);
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
		ui_interface->declare(&fHslider35, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider35, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider34, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider34, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider27, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider27, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider36, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider36, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider37, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider37, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider26, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider26, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider38, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider38, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider39, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider39, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider25, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider25, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider24, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider24, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 2");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider18, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider18, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider17, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider5, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider19, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider20, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider20, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider4, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider21, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider22, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider22, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider3, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider2, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 3");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider55, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider55, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider54, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider54, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider44, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider44, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider56, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider56, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider57, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider57, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider43, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider43, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider58, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider58, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider59, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider59, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider42, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider42, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider41, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider41, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 4");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider72, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider72, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider71, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider71, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider64, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider64, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider73, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider73, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider74, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider74, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider63, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider63, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider75, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider75, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider76, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider76, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider62, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider62, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider61, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider61, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 5");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider87, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider87, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider86, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider86, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider81, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider81, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider88, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider88, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider89, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider89, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider80, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider80, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider90, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider90, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider91, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider91, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider79, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider79, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider78, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider78, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Track 6");
		ui_interface->openHorizontalBox("1_HPF");
		ui_interface->declare(&fHslider104, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider104, 50.0f, 50.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fHslider103, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider103, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2_Parametric_1");
		ui_interface->declare(&fHslider96, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider96, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider105, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider105, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider106, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider106, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider95, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider95, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider107, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider107, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider108, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider108, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider94, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider94, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider93, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider93, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
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
		ui_interface->declare(&fHslider113, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider113, 500.0f, 100.0f, 2000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider121, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider121, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider122, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider122, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3_Parametric_2");
		ui_interface->declare(&fHslider112, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider112, 2000.0f, 1500.0f, 15000.0f, 0.00999999978f);
		ui_interface->declare(&fHslider123, "style", "knob");
		ui_interface->addHorizontalSlider("Gain", &fHslider123, 0.0f, -24.0f, 24.0f, 0.00999999978f);
		ui_interface->declare(&fHslider124, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider124, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4_LPF");
		ui_interface->declare(&fHslider111, "style", "knob");
		ui_interface->addHorizontalSlider("Freq", &fHslider111, 20000.0f, 50.0f, 20000.0f, 0.100000001f);
		ui_interface->declare(&fHslider110, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider110, 0.699999988f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Mixer_And_Master");
		ui_interface->openVerticalBox("Master_Gain");
		ui_interface->addVerticalSlider("Master Gain", &fVslider0, -6.0f, -96.0f, 12.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Gain");
		ui_interface->addHorizontalSlider("Track 1", &fHslider23, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 2", &fHslider1, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 3", &fHslider40, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 4", &fHslider60, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 5", &fHslider77, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 6", &fHslider92, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("Track 7", &fHslider109, 0.0f, -96.0f, 24.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Track_Mute");
		ui_interface->addCheckButton("1", &fCheckbox1);
		ui_interface->addCheckButton("2", &fCheckbox0);
		ui_interface->addCheckButton("3", &fCheckbox2);
		ui_interface->addCheckButton("4", &fCheckbox3);
		ui_interface->addCheckButton("5", &fCheckbox4);
		ui_interface->addCheckButton("6", &fCheckbox5);
		ui_interface->addCheckButton("7", &fCheckbox6);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Sonification_Control");
		ui_interface->openHorizontalBox("Soni 1");
		ui_interface->addHorizontalSlider("Control 0", &fHslider29, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 10");
		ui_interface->addHorizontalSlider("Control 0", &fHslider7, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 11");
		ui_interface->addHorizontalSlider("Control 0", &fHslider10, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 12");
		ui_interface->addHorizontalSlider("Control 0", &fHslider97, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 13");
		ui_interface->addHorizontalSlider("Control 0", &fHslider98, 0.00999999978f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 14");
		ui_interface->addHorizontalSlider("Control 0", &fHslider9, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 15");
		ui_interface->addHorizontalSlider("Control 0", &fHslider28, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 16");
		ui_interface->addHorizontalSlider("Control 0", &fHslider66, 1.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 17");
		ui_interface->addHorizontalSlider("Control 0", &fHslider114, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 2");
		ui_interface->addHorizontalSlider("Control 0", &fHslider6, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 3");
		ui_interface->addHorizontalSlider("Control 0", &fHslider11, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 4");
		ui_interface->addHorizontalSlider("Control 0", &fHslider46, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 1", &fHslider47, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 2", &fHslider48, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("Control 3", &fHslider49, 440.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 5");
		ui_interface->addHorizontalSlider("Control 0", &fHslider45, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 6");
		ui_interface->addHorizontalSlider("Control 0", &fHslider8, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 7");
		ui_interface->addHorizontalSlider("Control 0", &fHslider0, 0.5f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 8");
		ui_interface->addHorizontalSlider("Control 0", &fHslider65, 0.0f, 0.0f, 2000.0f, 0.00100000005f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Soni 9");
		ui_interface->addHorizontalSlider("Control 0", &fHslider12, 7.0f, 0.0f, 2000.0f, 0.00100000005f);
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
		float fSlow4 = std::tan((fConst28 * float(fHslider3)));
		float fSlow5 = (1.0f / fSlow4);
		float fSlow6 = (((fSlow3 + fSlow5) / fSlow4) + 1.0f);
		float fSlow7 = ((std::pow(10.0f, (0.0500000007f * float(fHslider1))) * (1.0f - float(fCheckbox0))) / fSlow6);
		float fSlow8 = float(fHslider4);
		float fSlow9 = std::tan((fConst28 * fSlow8));
		float fSlow10 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow9))));
		float fSlow11 = float(fHslider5);
		float fSlow12 = std::tan((fConst28 * fSlow11));
		float fSlow13 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow12))));
		float fSlow14 = float(fHslider6);
		float fSlow15 = std::pow(10.0f, (2.0f * (std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider7))) + -0.5f)));
		float fSlow16 = (2.0f * (fSlow14 * fSlow15));
		float fSlow17 = std::min<float>(1.0f, std::max<float>(0.0f, float(fHslider8)));
		float fSlow18 = (0.5f * fSlow17);
		float fSlow19 = std::min<float>(1000.0f, std::max<float>(0.0f, float(fHslider9)));
		float fSlow20 = (1.0f - fSlow19);
		float fSlow21 = (4.65661287e-10f * fSlow19);
		float fSlow22 = std::min<float>(3.0f, std::max<float>(0.0f, float(fHslider10)));
		float fSlow23 = (fSlow22 + 5.0f);
		int iSlow24 = (fSlow23 < 23.0f);
		int iSlow25 = (fSlow23 < 24.0f);
		float fSlow26 = (fSlow22 + -18.0f);
		int iSlow27 = (fSlow23 < 22.0f);
		float fSlow28 = (fSlow22 + -17.0f);
		int iSlow29 = (fSlow23 < 21.0f);
		float fSlow30 = (fSlow22 + -16.0f);
		int iSlow31 = (fSlow23 < 20.0f);
		float fSlow32 = (fSlow22 + -15.0f);
		int iSlow33 = (fSlow23 < 19.0f);
		float fSlow34 = (fSlow22 + -14.0f);
		int iSlow35 = (fSlow23 < 18.0f);
		float fSlow36 = (fSlow22 + -13.0f);
		int iSlow37 = (fSlow23 < 17.0f);
		float fSlow38 = (fSlow22 + -12.0f);
		int iSlow39 = (fSlow23 < 16.0f);
		float fSlow40 = (fSlow22 + -11.0f);
		float fSlow41 = (80.0f * fSlow40);
		int iSlow42 = (fSlow23 < 15.0f);
		float fSlow43 = (fSlow22 + -10.0f);
		int iSlow44 = (fSlow23 < 14.0f);
		float fSlow45 = (fSlow22 + -9.0f);
		int iSlow46 = (fSlow23 < 13.0f);
		float fSlow47 = (fSlow22 + -8.0f);
		int iSlow48 = (fSlow23 < 12.0f);
		float fSlow49 = (fSlow22 + -7.0f);
		int iSlow50 = (fSlow23 < 11.0f);
		float fSlow51 = (fSlow22 + -6.0f);
		int iSlow52 = (fSlow23 < 10.0f);
		float fSlow53 = (fSlow22 + -5.0f);
		int iSlow54 = (fSlow23 < 9.0f);
		float fSlow55 = (fSlow22 + -4.0f);
		int iSlow56 = (fSlow23 < 8.0f);
		float fSlow57 = (fSlow22 + -3.0f);
		float fSlow58 = (50.0f * fSlow57);
		int iSlow59 = (fSlow23 < 7.0f);
		float fSlow60 = (fSlow22 + -2.0f);
		int iSlow61 = (fSlow23 < 6.0f);
		float fSlow62 = (fSlow22 + -1.0f);
		int iSlow63 = (fSlow23 < 5.0f);
		int iSlow64 = (fSlow23 < 4.0f);
		float fSlow65 = (fSlow22 + 1.0f);
		int iSlow66 = (fSlow23 < 3.0f);
		float fSlow67 = (fSlow22 + 2.0f);
		int iSlow68 = (fSlow23 < 2.0f);
		float fSlow69 = (fSlow22 + 3.0f);
		int iSlow70 = (fSlow23 < 1.0f);
		float fSlow71 = (fSlow22 + 4.0f);
		int iSlow72 = (fSlow23 < 0.0f);
		float fSlow73 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 800.0f : (iSlow70 ? (800.0f - (400.0f * fSlow23)) : 400.0f)) : (iSlow68 ? (400.0f - (50.0f * fSlow71)) : 350.0f)) : (iSlow66 ? ((100.0f * fSlow69) + 350.0f) : 450.0f)) : (iSlow64 ? (450.0f - (125.0f * fSlow67)) : 325.0f)) : (iSlow63 ? ((275.0f * fSlow65) + 325.0f) : 600.0f)) : (iSlow61 ? (600.0f - (200.0f * fSlow22)) : 400.0f)) : (iSlow59 ? (400.0f - (150.0f * fSlow62)) : 250.0f)) : (iSlow56 ? ((150.0f * fSlow60) + 250.0f) : 400.0f)) : (iSlow54 ? (400.0f - fSlow58) : 350.0f)) : (iSlow52 ? ((310.0f * fSlow55) + 350.0f) : 660.0f)) : (iSlow50 ? (660.0f - (220.0f * fSlow53)) : 440.0f)) : (iSlow48 ? (440.0f - (170.0f * fSlow51)) : 270.0f)) : (iSlow46 ? ((160.0f * fSlow49) + 270.0f) : 430.0f)) : (iSlow44 ? (430.0f - (60.0f * fSlow47)) : 370.0f)) : (iSlow42 ? ((430.0f * fSlow45) + 370.0f) : 800.0f)) : (iSlow39 ? (800.0f - (450.0f * fSlow43)) : 350.0f)) : (iSlow37 ? (350.0f - fSlow41) : 270.0f)) : (iSlow35 ? ((180.0f * fSlow38) + 270.0f) : 450.0f)) : (iSlow33 ? (450.0f - (125.0f * fSlow36)) : 325.0f)) : (iSlow31 ? (325.0f * (fSlow34 + 1.0f)) : 650.0f)) : (iSlow29 ? (650.0f - (250.0f * fSlow32)) : 400.0f)) : (iSlow27 ? (400.0f - (110.0f * fSlow30)) : 290.0f)) : (iSlow24 ? ((110.0f * fSlow28) + 290.0f) : 400.0f)) : (iSlow25 ? (400.0f - (50.0f * fSlow26)) : 350.0f));
		float fSlow74 = (20.0f * fSlow32);
		float fSlow75 = (20.0f * fSlow43);
		float fSlow76 = (10.0f * fSlow53);
		float fSlow77 = (10.0f * fSlow65);
		float fSlow78 = (20.0f * fSlow67);
		float fSlow79 = (20.0f * fSlow69);
		float fSlow80 = (20.0f * fSlow23);
		float fSlow81 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 80.0f : (iSlow70 ? (80.0f - fSlow80) : 60.0f)) : (iSlow68 ? (60.0f - (10.0f * fSlow71)) : 50.0f)) : (iSlow66 ? (fSlow79 + 50.0f) : 70.0f)) : (iSlow64 ? (70.0f - fSlow78) : 50.0f)) : (iSlow63 ? (fSlow77 + 50.0f) : 60.0f)) : (iSlow61 ? (60.0f - (20.0f * fSlow22)) : 40.0f)) : (iSlow59 ? ((20.0f * fSlow62) + 40.0f) : 60.0f)) : (iSlow56 ? (60.0f - (20.0f * fSlow60)) : 40.0f)) : 40.0f) : (iSlow52 ? (40.0f * (fSlow55 + 1.0f)) : 80.0f)) : (iSlow50 ? (80.0f - fSlow76) : 70.0f)) : (iSlow48 ? (70.0f - (30.0f * fSlow51)) : 40.0f)) : 40.0f) : 40.0f) : (iSlow42 ? (40.0f * (fSlow45 + 1.0f)) : 80.0f)) : (iSlow39 ? (80.0f - fSlow75) : 60.0f)) : 60.0f) : (iSlow35 ? (60.0f - (20.0f * fSlow38)) : 40.0f)) : (iSlow33 ? ((10.0f * fSlow36) + 40.0f) : 50.0f)) : 50.0f) : (iSlow29 ? (fSlow74 + 50.0f) : 70.0f)) : (iSlow27 ? (70.0f - (30.0f * fSlow30)) : 40.0f)) : (iSlow24 ? ((30.0f * fSlow28) + 40.0f) : 70.0f)) : (iSlow25 ? (70.0f - (30.0f * fSlow26)) : 40.0f));
		float fSlow82 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 1150.0f : (iSlow70 ? ((450.0f * fSlow23) + 1150.0f) : 1600.0f)) : (iSlow68 ? ((100.0f * fSlow71) + 1600.0f) : 1700.0f)) : (iSlow66 ? (1700.0f - (900.0f * fSlow69)) : 800.0f)) : (iSlow64 ? (800.0f - (100.0f * fSlow67)) : 700.0f)) : (iSlow63 ? ((340.0f * fSlow65) + 700.0f) : 1040.0f)) : (iSlow61 ? ((580.0f * fSlow22) + 1040.0f) : 1620.0f)) : (iSlow59 ? ((130.0f * fSlow62) + 1620.0f) : 1750.0f)) : (iSlow56 ? (1750.0f - (1000.0f * fSlow60)) : 750.0f)) : (iSlow54 ? (750.0f - (150.0f * fSlow57)) : 600.0f)) : (iSlow52 ? ((520.0f * fSlow55) + 600.0f) : 1120.0f)) : (iSlow50 ? ((680.0f * fSlow53) + 1120.0f) : 1800.0f)) : (iSlow48 ? ((50.0f * fSlow51) + 1800.0f) : 1850.0f)) : (iSlow46 ? (1850.0f - (1030.0f * fSlow49)) : 820.0f)) : (iSlow44 ? (820.0f - (190.0f * fSlow47)) : 630.0f)) : (iSlow42 ? ((520.0f * fSlow45) + 630.0f) : 1150.0f)) : (iSlow39 ? ((850.0f * fSlow43) + 1150.0f) : 2000.0f)) : (iSlow37 ? ((140.0f * fSlow40) + 2000.0f) : 2140.0f)) : (iSlow35 ? (2140.0f - (1340.0f * fSlow38)) : 800.0f)) : (iSlow33 ? (800.0f - (100.0f * fSlow36)) : 700.0f)) : (iSlow31 ? ((380.0f * fSlow34) + 700.0f) : 1080.0f)) : (iSlow29 ? ((620.0f * fSlow32) + 1080.0f) : 1700.0f)) : (iSlow27 ? ((170.0f * fSlow30) + 1700.0f) : 1870.0f)) : (iSlow24 ? (1870.0f - (1070.0f * fSlow28)) : 800.0f)) : (iSlow25 ? (800.0f - (200.0f * fSlow26)) : 600.0f));
		int iSlow83 = (fSlow82 >= 1300.0f);
		float fSlow84 = (fSlow82 + -1300.0f);
		float fSlow85 = (10.0f * fSlow28);
		float fSlow86 = (10.0f * fSlow32);
		float fSlow87 = (10.0f * fSlow55);
		float fSlow88 = (10.0f * fSlow22);
		float fSlow89 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 90.0f : (iSlow70 ? (90.0f - (10.0f * fSlow23)) : 80.0f)) : (iSlow68 ? ((20.0f * fSlow71) + 80.0f) : 100.0f)) : (iSlow66 ? (100.0f - fSlow79) : 80.0f)) : (iSlow64 ? (80.0f - fSlow78) : 60.0f)) : (iSlow63 ? (fSlow77 + 60.0f) : 70.0f)) : (iSlow61 ? (fSlow88 + 70.0f) : 80.0f)) : (iSlow59 ? ((10.0f * fSlow62) + 80.0f) : 90.0f)) : (iSlow56 ? (90.0f - (10.0f * fSlow60)) : 80.0f)) : 80.0f) : (iSlow52 ? (fSlow87 + 80.0f) : 90.0f)) : (iSlow50 ? (90.0f - fSlow76) : 80.0f)) : (iSlow48 ? ((10.0f * fSlow51) + 80.0f) : 90.0f)) : (iSlow46 ? (90.0f - (10.0f * fSlow49)) : 80.0f)) : (iSlow44 ? (80.0f - (20.0f * fSlow47)) : 60.0f)) : (iSlow42 ? ((30.0f * fSlow45) + 60.0f) : 90.0f)) : (iSlow39 ? ((10.0f * fSlow43) + 90.0f) : 100.0f)) : (iSlow37 ? (100.0f - (10.0f * fSlow40)) : 90.0f)) : (iSlow35 ? (90.0f - (10.0f * fSlow38)) : 80.0f)) : (iSlow33 ? (80.0f - (20.0f * fSlow36)) : 60.0f)) : (iSlow31 ? ((30.0f * fSlow34) + 60.0f) : 90.0f)) : (iSlow29 ? (90.0f - fSlow86) : 80.0f)) : (iSlow27 ? ((10.0f * fSlow30) + 80.0f) : 90.0f)) : (iSlow24 ? (90.0f - fSlow85) : 80.0f)) : (iSlow25 ? (80.0f - (20.0f * fSlow26)) : 60.0f));
		float fSlow90 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 0.630957007f : (iSlow70 ? (0.630957007f - (0.567861021f * fSlow23)) : 0.0630960017f)) : (iSlow68 ? ((0.0369039997f * fSlow71) + 0.0630960017f) : 0.100000001f)) : (iSlow66 ? ((0.254812986f * fSlow69) + 0.100000001f) : 0.35481301f)) : (iSlow64 ? (0.35481301f - (0.103624001f * fSlow67)) : 0.251188993f)) : (iSlow63 ? ((0.195494995f * fSlow65) + 0.251188993f) : 0.446684003f)) : (iSlow61 ? (0.446684003f - (0.195494995f * fSlow22)) : 0.251188993f)) : (iSlow59 ? (0.251188993f - (0.219566002f * fSlow62)) : 0.0316229984f)) : (iSlow56 ? ((0.250214994f * fSlow60) + 0.0316229984f) : 0.281838f)) : (iSlow54 ? (0.281838f - (0.181838006f * fSlow57)) : 0.100000001f)) : (iSlow52 ? ((0.401187003f * fSlow55) + 0.100000001f) : 0.501187027f)) : (iSlow50 ? (0.501187027f - (0.301661015f * fSlow53)) : 0.199525997f)) : (iSlow48 ? (0.199525997f - (0.136429995f * fSlow51)) : 0.0630960017f)) : (iSlow46 ? ((0.253131986f * fSlow49) + 0.0630960017f) : 0.316228002f)) : (iSlow44 ? (0.316228002f - (0.216227993f * fSlow47)) : 0.100000001f)) : (iSlow42 ? ((0.401187003f * fSlow45) + 0.100000001f) : 0.501187027f)) : (iSlow39 ? (0.501187027f - (0.401187003f * fSlow43)) : 0.100000001f)) : (iSlow37 ? ((0.151188999f * fSlow40) + 0.100000001f) : 0.251188993f)) : (iSlow35 ? ((0.0306490008f * fSlow38) + 0.251188993f) : 0.281838f)) : (iSlow33 ? (0.281838f - (0.123349003f * fSlow36)) : 0.158489004f)) : (iSlow31 ? ((0.342698008f * fSlow34) + 0.158489004f) : 0.501187027f)) : (iSlow29 ? (0.501187027f - (0.301661015f * fSlow32)) : 0.199525997f)) : (iSlow27 ? (0.199525997f - (0.0216979999f * fSlow30)) : 0.177827999f)) : (iSlow24 ? ((0.138400003f * fSlow28) + 0.177827999f) : 0.316228002f)) : (iSlow25 ? (0.316228002f - (0.216227993f * fSlow26)) : 0.100000001f));
		float fSlow91 = (100.0f * fSlow26);
		float fSlow92 = (50.0f * fSlow34);
		float fSlow93 = (50.0f * fSlow53);
		float fSlow94 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 2800.0f : (iSlow70 ? (2800.0f - (100.0f * fSlow23)) : 2700.0f)) : 2700.0f) : (iSlow66 ? ((130.0f * fSlow69) + 2700.0f) : 2830.0f)) : (iSlow64 ? (2830.0f - (300.0f * fSlow67)) : 2530.0f)) : (iSlow63 ? (2530.0f - (280.0f * fSlow65)) : 2250.0f)) : (iSlow61 ? ((150.0f * fSlow22) + 2250.0f) : 2400.0f)) : (iSlow59 ? ((200.0f * fSlow62) + 2400.0f) : 2600.0f)) : (iSlow56 ? (2600.0f - (200.0f * fSlow60)) : 2400.0f)) : 2400.0f) : (iSlow52 ? ((350.0f * fSlow55) + 2400.0f) : 2750.0f)) : (iSlow50 ? (2750.0f - fSlow93) : 2700.0f)) : (iSlow48 ? ((200.0f * fSlow51) + 2700.0f) : 2900.0f)) : (iSlow46 ? (2900.0f - (200.0f * fSlow49)) : 2700.0f)) : (iSlow44 ? ((50.0f * fSlow47) + 2700.0f) : 2750.0f)) : (iSlow42 ? ((150.0f * fSlow45) + 2750.0f) : 2900.0f)) : (iSlow39 ? (2900.0f - (100.0f * fSlow43)) : 2800.0f)) : (iSlow37 ? ((150.0f * fSlow40) + 2800.0f) : 2950.0f)) : (iSlow35 ? (2950.0f - (120.0f * fSlow38)) : 2830.0f)) : (iSlow33 ? (2830.0f - (130.0f * fSlow36)) : 2700.0f)) : (iSlow31 ? (2700.0f - fSlow92) : 2650.0f)) : (iSlow29 ? (2650.0f - (50.0f * fSlow32)) : 2600.0f)) : (iSlow27 ? ((200.0f * fSlow30) + 2600.0f) : 2800.0f)) : (iSlow24 ? (2800.0f - (200.0f * fSlow28)) : 2600.0f)) : (iSlow25 ? (fSlow91 + 2600.0f) : 2700.0f));
		float fSlow95 = std::tan((fConst28 * fSlow94));
		float fSlow96 = (1.0f / fSlow95);
		float fSlow97 = (20.0f * fSlow45);
		float fSlow98 = (20.0f * fSlow53);
		float fSlow99 = (20.0f * fSlow55);
		float fSlow100 = (60.0f * fSlow65);
		float fSlow101 = ((iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? 120.0f : (iSlow66 ? (120.0f - fSlow79) : 100.0f)) : (iSlow64 ? ((70.0f * fSlow67) + 100.0f) : 170.0f)) : (iSlow63 ? (170.0f - fSlow100) : 110.0f)) : (iSlow61 ? (110.0f - fSlow88) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow52 ? (fSlow99 + 100.0f) : 120.0f)) : (iSlow50 ? (120.0f - fSlow98) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) : (iSlow42 ? (fSlow97 + 100.0f) : 120.0f)) : 120.0f) : (iSlow37 ? (120.0f - (20.0f * fSlow40)) : 100.0f)) : 100.0f) : (iSlow33 ? ((70.0f * fSlow36) + 100.0f) : 170.0f)) : (iSlow31 ? (170.0f - fSlow92) : 120.0f)) : (iSlow29 ? (120.0f - fSlow74) : 100.0f)) : 100.0f) : 100.0f) : 100.0f) / fSlow94);
		float fSlow102 = (((fSlow96 - fSlow101) / fSlow95) + 1.0f);
		float fSlow103 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow95)));
		float fSlow104 = (((fSlow96 + fSlow101) / fSlow95) + 1.0f);
		float fSlow105 = (iSlow35 ? ((0.0293140002f * fSlow38) + 0.0501190014f) : 0.0794330016f);
		float fSlow106 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 0.100000001f : (iSlow70 ? (0.100000001f - (0.0683770031f * fSlow23)) : 0.0316229984f)) : 0.0316229984f) : (iSlow66 ? ((0.126865998f * fSlow69) + 0.0316229984f) : 0.158489004f)) : (iSlow64 ? (0.158489004f - (0.126865998f * fSlow67)) : 0.0316229984f)) : (iSlow63 ? ((0.323190004f * fSlow65) + 0.0316229984f) : 0.35481301f)) : 0.35481301f) : (iSlow59 ? (0.35481301f - (0.196324006f * fSlow62)) : 0.158489004f)) : (iSlow56 ? (0.158489004f - (0.0693639964f * fSlow60)) : 0.0891249999f)) : (iSlow54 ? (0.0891249999f - (0.0640060008f * fSlow57)) : 0.0251189992f)) : (iSlow52 ? ((0.0456760004f * fSlow55) + 0.0251189992f) : 0.0707949996f)) : (iSlow50 ? ((0.0550980009f * fSlow53) + 0.0707949996f) : 0.125892997f)) : (iSlow48 ? (0.125892997f - (0.0627970025f * fSlow51)) : 0.0630960017f)) : (iSlow46 ? (0.0630960017f - (0.0129770003f * fSlow49)) : 0.0501190014f)) : (iSlow44 ? ((0.020676f * fSlow47) + 0.0501190014f) : 0.0707949996f)) : (iSlow42 ? (0.0707949996f - (0.0456760004f * fSlow45)) : 0.0251189992f)) : (iSlow39 ? ((0.152709007f * fSlow43) + 0.0251189992f) : 0.177827999f)) : (iSlow37 ? (0.177827999f - (0.127709001f * fSlow40)) : 0.0501190014f)) : fSlow105) : (iSlow33 ? (0.0794330016f - (0.0616500005f * fSlow36)) : 0.0177829992f)) : (iSlow31 ? ((0.428900987f * fSlow34) + 0.0177829992f) : 0.446684003f)) : (iSlow29 ? (0.446684003f - (0.195494995f * fSlow32)) : 0.251188993f)) : (iSlow27 ? (0.251188993f - (0.125295997f * fSlow30)) : 0.125892997f)) : (iSlow24 ? ((0.125295997f * fSlow28) + 0.125892997f) : 0.251188993f)) : (iSlow25 ? (0.251188993f - (0.109935001f * fSlow26)) : 0.141253993f));
		float fSlow107 = (fSlow95 * fSlow104);
		float fSlow108 = (350.0f * fSlow22);
		float fSlow109 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 3500.0f : (iSlow70 ? (3500.0f - (200.0f * fSlow23)) : 3300.0f)) : (iSlow68 ? ((400.0f * fSlow71) + 3300.0f) : 3700.0f)) : (iSlow66 ? (3700.0f - (200.0f * fSlow69)) : 3500.0f)) : 3500.0f) : (iSlow63 ? (3500.0f - (1050.0f * fSlow65)) : 2450.0f)) : (iSlow61 ? (fSlow108 + 2450.0f) : 2800.0f)) : (iSlow59 ? ((250.0f * fSlow62) + 2800.0f) : 3050.0f)) : (iSlow56 ? (3050.0f - (450.0f * fSlow60)) : 2600.0f)) : (iSlow54 ? ((75.0f * fSlow57) + 2600.0f) : 2675.0f)) : (iSlow52 ? ((325.0f * fSlow55) + 2675.0f) : 3000.0f)) : 3000.0f) : (iSlow48 ? ((350.0f * fSlow51) + 3000.0f) : 3350.0f)) : (iSlow46 ? (3350.0f - (350.0f * fSlow49)) : 3000.0f)) : 3000.0f) : (iSlow42 ? ((900.0f * fSlow45) + 3000.0f) : 3900.0f)) : (iSlow39 ? (3900.0f - (300.0f * fSlow43)) : 3600.0f)) : (iSlow37 ? ((300.0f * fSlow40) + 3600.0f) : 3900.0f)) : (iSlow35 ? (3900.0f - (100.0f * fSlow38)) : 3800.0f)) : 3800.0f) : (iSlow31 ? (3800.0f - (900.0f * fSlow34)) : 2900.0f)) : (iSlow29 ? ((300.0f * fSlow32) + 2900.0f) : 3200.0f)) : (iSlow27 ? ((50.0f * fSlow30) + 3200.0f) : 3250.0f)) : (iSlow24 ? (3250.0f - (450.0f * fSlow28)) : 2800.0f)) : (iSlow25 ? (fSlow91 + 2800.0f) : 2900.0f));
		float fSlow110 = std::tan((fConst28 * fSlow109));
		float fSlow111 = (1.0f / fSlow110);
		float fSlow112 = ((iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 130.0f : (iSlow70 ? (fSlow80 + 130.0f) : 150.0f)) : 150.0f) : (iSlow66 ? (150.0f - fSlow79) : 130.0f)) : (iSlow64 ? ((50.0f * fSlow67) + 130.0f) : 180.0f)) : (iSlow63 ? (180.0f - fSlow100) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : 120.0f) : (iSlow52 ? (fSlow87 + 120.0f) : 130.0f)) : (iSlow50 ? (130.0f - fSlow76) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow42 ? ((10.0f * fSlow45) + 120.0f) : 130.0f)) : (iSlow39 ? (fSlow75 + 130.0f) : 150.0f)) : (iSlow37 ? (150.0f - (30.0f * fSlow40)) : 120.0f)) : 120.0f) : (iSlow33 ? ((60.0f * fSlow36) + 120.0f) : 180.0f)) : (iSlow31 ? (180.0f - fSlow92) : 130.0f)) : (iSlow29 ? (130.0f - fSlow86) : 120.0f)) : 120.0f) : (iSlow24 ? (fSlow85 + 120.0f) : 130.0f)) : (iSlow25 ? (130.0f - (10.0f * fSlow26)) : 120.0f)) / fSlow109);
		float fSlow113 = (((fSlow111 - fSlow112) / fSlow110) + 1.0f);
		float fSlow114 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow110)));
		float fSlow115 = (((fSlow111 + fSlow112) / fSlow110) + 1.0f);
		float fSlow116 = (iSlow48 ? (0.100000001f - (0.0841509998f * fSlow51)) : 0.0158489998f);
		float fSlow117 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 0.0158489998f : (iSlow70 ? ((0.00193400006f * fSlow23) + 0.0158489998f) : 0.0177829992f)) : (iSlow68 ? (0.0177829992f - (0.00193400006f * fSlow71)) : 0.0158489998f)) : (iSlow66 ? ((0.0239620004f * fSlow69) + 0.0158489998f) : 0.0398110002f)) : (iSlow64 ? (0.0398110002f - (0.0298110005f * fSlow67)) : 0.00999999978f)) : (iSlow63 ? ((0.344812989f * fSlow65) + 0.00999999978f) : 0.35481301f)) : (iSlow61 ? (0.35481301f - (0.103624001f * fSlow22)) : 0.251188993f)) : (iSlow59 ? (0.251188993f - (0.171755999f * fSlow62)) : 0.0794330016f)) : (iSlow56 ? ((0.0205669999f * fSlow60) + 0.0794330016f) : 0.100000001f)) : (iSlow54 ? (0.100000001f - (0.0601890013f * fSlow57)) : 0.0398110002f)) : (iSlow52 ? ((0.0232849997f * fSlow55) + 0.0398110002f) : 0.0630960017f)) : (iSlow50 ? ((0.0369039997f * fSlow53) + 0.0630960017f) : 0.100000001f)) : fSlow116) : (iSlow46 ? ((0.0635839999f * fSlow49) + 0.0158489998f) : 0.0794330016f)) : (iSlow44 ? (0.0794330016f - (0.0478099994f * fSlow47)) : 0.0316229984f)) : (iSlow42 ? ((0.0683770031f * fSlow45) + 0.0316229984f) : 0.100000001f)) : (iSlow39 ? (0.100000001f - (0.0900000036f * fSlow43)) : 0.00999999978f)) : (iSlow37 ? ((0.0401189998f * fSlow40) + 0.00999999978f) : 0.0501190014f)) : fSlow105) : (iSlow33 ? (0.0794330016f - (0.0694330037f * fSlow36)) : 0.00999999978f)) : (iSlow31 ? ((0.388107002f * fSlow34) + 0.00999999978f) : 0.398106992f)) : (iSlow29 ? (0.398106992f - (0.198580995f * fSlow32)) : 0.199525997f)) : (iSlow27 ? (0.199525997f - (0.099526003f * fSlow30)) : 0.100000001f)) : (iSlow24 ? ((0.151188999f * fSlow28) + 0.100000001f) : 0.251188993f)) : (iSlow25 ? (0.251188993f - (0.0516630001f * fSlow26)) : 0.199525997f));
		float fSlow118 = (fSlow110 * fSlow115);
		float fSlow119 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? 4950.0f : (iSlow63 ? (4950.0f - (2200.0f * fSlow65)) : 2750.0f)) : (iSlow61 ? (fSlow108 + 2750.0f) : 3100.0f)) : (iSlow59 ? ((240.0f * fSlow62) + 3100.0f) : 3340.0f)) : (iSlow56 ? (3340.0f - (440.0f * fSlow60)) : 2900.0f)) : (iSlow54 ? (fSlow58 + 2900.0f) : 2950.0f)) : (iSlow52 ? ((400.0f * fSlow55) + 2950.0f) : 3350.0f)) : (iSlow50 ? (3350.0f - fSlow93) : 3300.0f)) : (iSlow48 ? ((290.0f * fSlow51) + 3300.0f) : 3590.0f)) : (iSlow46 ? (3590.0f - (290.0f * fSlow49)) : 3300.0f)) : (iSlow44 ? ((100.0f * fSlow47) + 3300.0f) : 3400.0f)) : (iSlow42 ? ((1550.0f * fSlow45) + 3400.0f) : 4950.0f)) : 4950.0f) : 4950.0f) : 4950.0f) : 4950.0f) : (iSlow31 ? (4950.0f - (1700.0f * fSlow34)) : 3250.0f)) : (iSlow29 ? ((330.0f * fSlow32) + 3250.0f) : 3580.0f)) : (iSlow27 ? (3580.0f - (40.0f * fSlow30)) : 3540.0f)) : (iSlow24 ? (3540.0f - (540.0f * fSlow28)) : 3000.0f)) : (iSlow25 ? ((300.0f * fSlow26) + 3000.0f) : 3300.0f));
		float fSlow120 = std::tan((fConst28 * fSlow119));
		float fSlow121 = (1.0f / fSlow120);
		float fSlow122 = ((iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? (iSlow70 ? (iSlow72 ? 140.0f : (iSlow70 ? ((60.0f * fSlow23) + 140.0f) : 200.0f)) : 200.0f) : (iSlow66 ? (200.0f - (65.0f * fSlow69)) : 135.0f)) : (iSlow64 ? ((65.0f * fSlow67) + 135.0f) : 200.0f)) : (iSlow63 ? (200.0f - (70.0f * fSlow65)) : 130.0f)) : (iSlow61 ? (130.0f - fSlow88) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow52 ? (fSlow99 + 120.0f) : 140.0f)) : (iSlow50 ? (140.0f - fSlow98) : 120.0f)) : 120.0f) : 120.0f) : 120.0f) : (iSlow42 ? (fSlow97 + 120.0f) : 140.0f)) : (iSlow39 ? ((60.0f * fSlow43) + 140.0f) : 200.0f)) : (iSlow37 ? (200.0f - fSlow41) : 120.0f)) : 120.0f) : (iSlow33 ? ((80.0f * fSlow36) + 120.0f) : 200.0f)) : (iSlow31 ? (200.0f - (60.0f * fSlow34)) : 140.0f)) : (iSlow29 ? (140.0f - fSlow74) : 120.0f)) : 120.0f) : (iSlow24 ? ((15.0f * fSlow28) + 120.0f) : 135.0f)) : (iSlow25 ? (135.0f - (15.0f * fSlow26)) : 120.0f)) / fSlow119);
		float fSlow123 = (((fSlow121 - fSlow122) / fSlow120) + 1.0f);
		float fSlow124 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow120)));
		float fSlow125 = (((fSlow121 + fSlow122) / fSlow120) + 1.0f);
		float fSlow126 = (iSlow24 ? (iSlow27 ? (iSlow29 ? (iSlow31 ? (iSlow33 ? (iSlow35 ? (iSlow37 ? (iSlow39 ? (iSlow42 ? (iSlow44 ? (iSlow46 ? (iSlow48 ? (iSlow50 ? (iSlow52 ? (iSlow54 ? (iSlow56 ? (iSlow59 ? (iSlow61 ? (iSlow63 ? (iSlow64 ? (iSlow66 ? (iSlow68 ? 0.00100000005f : (iSlow66 ? ((0.000777999987f * fSlow69) + 0.00100000005f) : 0.00177800003f)) : (iSlow64 ? (0.00177800003f - (0.001147f * fSlow67)) : 0.000630999974f)) : (iSlow63 ? ((0.0993689969f * fSlow65) + 0.000630999974f) : 0.100000001f)) : (iSlow61 ? ((0.0258930009f * fSlow22) + 0.100000001f) : 0.125892997f)) : (iSlow59 ? (0.125892997f - (0.0860819966f * fSlow62)) : 0.0398110002f)) : (iSlow56 ? (0.0398110002f - (0.0298110005f * fSlow60)) : 0.00999999978f)) : (iSlow54 ? ((0.00584900007f * fSlow57) + 0.00999999978f) : 0.0158489998f)) : (iSlow52 ? (0.0158489998f - (0.00325999991f * fSlow55)) : 0.0125890002f)) : (iSlow50 ? ((0.0874110013f * fSlow53) + 0.0125890002f) : 0.100000001f)) : fSlow116) : (iSlow46 ? ((0.00410400005f * fSlow49) + 0.0158489998f) : 0.0199529994f)) : 0.0199529994f) : (iSlow42 ? (0.0199529994f - (0.0167909991f * fSlow45)) : 0.0031620001f)) : (iSlow39 ? (0.0031620001f - (0.00157700002f * fSlow43)) : 0.00158499996f)) : (iSlow37 ? ((0.00472499989f * fSlow40) + 0.00158499996f) : 0.00631000008f)) : (iSlow35 ? (0.00631000008f - (0.00314799999f * fSlow38)) : 0.0031620001f)) : (iSlow33 ? (0.0031620001f - (0.00216199993f * fSlow36)) : 0.00100000005f)) : (iSlow31 ? ((0.0784329996f * fSlow34) + 0.00100000005f) : 0.0794330016f)) : (iSlow29 ? ((0.0205669999f * fSlow32) + 0.0794330016f) : 0.100000001f)) : (iSlow27 ? (0.100000001f - (0.0683770031f * fSlow30)) : 0.0316229984f)) : (iSlow24 ? ((0.0184959993f * fSlow28) + 0.0316229984f) : 0.0501190014f)) : 0.0501190014f);
		float fSlow127 = (fSlow120 * fSlow125);
		float fSlow128 = float(fHslider11);
		float fSlow129 = float((fSlow128 > 0.0f));
		float fSlow130 = (0.0f - fSlow128);
		float fSlow131 = std::min<float>(10.0f, std::max<float>(7.0f, float(fHslider12)));
		float fSlow132 = (fConst47 * ((1.0f / float(fHslider13)) + -1.0f));
		float fSlow133 = float(fHslider14);
		int iSlow134 = (std::fabs(fSlow133) < 1.1920929e-07f);
		float fSlow135 = (iSlow134 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow134 ? 1.0f : fSlow133)))));
		float fSlow136 = float(fHslider15);
		int iSlow137 = (std::fabs(fSlow136) < 1.1920929e-07f);
		float fSlow138 = (iSlow137 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow137 ? 1.0f : fSlow136)))));
		float fSlow139 = (1.0f - fSlow138);
		float fSlow140 = (1.0f - fSlow135);
		float fSlow141 = float(fHslider16);
		float fSlow142 = (1.0f / float(fHslider17));
		float fSlow143 = std::tan((fConst28 * float(fHslider18)));
		float fSlow144 = (1.0f / fSlow143);
		float fSlow145 = (1.0f / (((fSlow142 + fSlow144) / fSlow143) + 1.0f));
		float fSlow146 = (((fSlow144 - fSlow142) / fSlow143) + 1.0f);
		float fSlow147 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow143))));
		float fSlow148 = (1.0f / fSlow12);
		float fSlow149 = float(fHslider19);
		int iSlow150 = (fSlow149 > 0.0f);
		float fSlow151 = (float(fHslider20) * std::sin((fConst48 * fSlow11)));
		float fSlow152 = (fConst28 * ((fSlow11 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow149)))) / fSlow151));
		float fSlow153 = (fConst28 * (fSlow11 / fSlow151));
		float fSlow154 = (iSlow150 ? fSlow153 : fSlow152);
		float fSlow155 = ((fSlow148 * (fSlow148 - fSlow154)) + 1.0f);
		float fSlow156 = ((fSlow148 * (fSlow148 + fSlow154)) + 1.0f);
		float fSlow157 = (iSlow150 ? fSlow152 : fSlow153);
		float fSlow158 = ((fSlow148 * (fSlow148 + fSlow157)) + 1.0f);
		float fSlow159 = ((fSlow148 * (fSlow148 - fSlow157)) + 1.0f);
		float fSlow160 = (1.0f / fSlow9);
		float fSlow161 = float(fHslider21);
		int iSlow162 = (fSlow161 > 0.0f);
		float fSlow163 = (float(fHslider22) * std::sin((fConst48 * fSlow8)));
		float fSlow164 = (fConst28 * ((fSlow8 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow161)))) / fSlow163));
		float fSlow165 = (fConst28 * (fSlow8 / fSlow163));
		float fSlow166 = (iSlow162 ? fSlow165 : fSlow164);
		float fSlow167 = ((fSlow160 * (fSlow160 - fSlow166)) + 1.0f);
		float fSlow168 = ((fSlow160 * (fSlow160 + fSlow166)) + 1.0f);
		float fSlow169 = (iSlow162 ? fSlow164 : fSlow165);
		float fSlow170 = ((fSlow160 * (fSlow160 + fSlow169)) + 1.0f);
		float fSlow171 = ((fSlow160 * (fSlow160 - fSlow169)) + 1.0f);
		float fSlow172 = (1.0f / fSlow6);
		float fSlow173 = (((fSlow5 - fSlow3) / fSlow4) + 1.0f);
		float fSlow174 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow4))));
		float fSlow175 = (1.0f / float(fHslider24));
		float fSlow176 = std::tan((fConst28 * float(fHslider25)));
		float fSlow177 = (1.0f / fSlow176);
		float fSlow178 = (((fSlow175 + fSlow177) / fSlow176) + 1.0f);
		float fSlow179 = ((std::pow(10.0f, (0.0500000007f * float(fHslider23))) * (1.0f - float(fCheckbox1))) / fSlow178);
		float fSlow180 = float(fHslider26);
		float fSlow181 = std::tan((fConst28 * fSlow180));
		float fSlow182 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow181))));
		float fSlow183 = float(fHslider27);
		float fSlow184 = std::tan((fConst28 * fSlow183));
		float fSlow185 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow184))));
		float fSlow186 = (std::min<float>(5.0f, std::max<float>(0.00999999978f, float(fHslider28))) + 1.0f);
		float fSlow187 = std::tan((fConst28 * ((1875.0f * fSlow186) + 500.0f)));
		float fSlow188 = (1.0f / fSlow187);
		float fSlow189 = (1.0f / (((fSlow188 + 1.41421354f) / fSlow187) + 1.0f));
		float fSlow190 = std::tan((fConst28 * ((62.5f * fSlow186) + 40.0f)));
		float fSlow191 = (1.0f / fSlow190);
		float fSlow192 = (1.0f / (((fSlow191 + 1.41421354f) / fSlow190) + 1.0f));
		float fSlow193 = mydsp_faustpower2_f(fSlow190);
		float fSlow194 = (1.0f / fSlow193);
		float fSlow195 = (((fSlow191 + -1.41421354f) / fSlow190) + 1.0f);
		float fSlow196 = (2.0f * (1.0f - fSlow194));
		float fSlow197 = (0.0f - (2.0f / fSlow193));
		float fSlow198 = (((fSlow188 + -1.41421354f) / fSlow187) + 1.0f);
		float fSlow199 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow187))));
		float fSlow200 = (1.0f / std::max<float>(1.0f, (fConst152 * fSlow186)));
		float fSlow201 = float(fHslider29);
		float fSlow202 = float((fSlow201 > 0.0f));
		float fSlow203 = (0.0f - fSlow201);
		float fSlow204 = (fConst47 * ((1.0f / float(fHslider30)) + -1.0f));
		float fSlow205 = float(fHslider31);
		int iSlow206 = (std::fabs(fSlow205) < 1.1920929e-07f);
		float fSlow207 = (iSlow206 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow206 ? 1.0f : fSlow205)))));
		float fSlow208 = float(fHslider32);
		int iSlow209 = (std::fabs(fSlow208) < 1.1920929e-07f);
		float fSlow210 = (iSlow209 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow209 ? 1.0f : fSlow208)))));
		float fSlow211 = (1.0f - fSlow210);
		float fSlow212 = (1.0f - fSlow207);
		float fSlow213 = float(fHslider33);
		float fSlow214 = (1.0f / float(fHslider34));
		float fSlow215 = std::tan((fConst28 * float(fHslider35)));
		float fSlow216 = (1.0f / fSlow215);
		float fSlow217 = (1.0f / (((fSlow214 + fSlow216) / fSlow215) + 1.0f));
		float fSlow218 = (((fSlow216 - fSlow214) / fSlow215) + 1.0f);
		float fSlow219 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow215))));
		float fSlow220 = (1.0f / fSlow184);
		float fSlow221 = float(fHslider36);
		int iSlow222 = (fSlow221 > 0.0f);
		float fSlow223 = (float(fHslider37) * std::sin((fConst48 * fSlow183)));
		float fSlow224 = (fConst28 * ((fSlow183 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow221)))) / fSlow223));
		float fSlow225 = (fConst28 * (fSlow183 / fSlow223));
		float fSlow226 = (iSlow222 ? fSlow225 : fSlow224);
		float fSlow227 = ((fSlow220 * (fSlow220 - fSlow226)) + 1.0f);
		float fSlow228 = ((fSlow220 * (fSlow220 + fSlow226)) + 1.0f);
		float fSlow229 = (iSlow222 ? fSlow224 : fSlow225);
		float fSlow230 = ((fSlow220 * (fSlow220 + fSlow229)) + 1.0f);
		float fSlow231 = ((fSlow220 * (fSlow220 - fSlow229)) + 1.0f);
		float fSlow232 = (1.0f / fSlow181);
		float fSlow233 = float(fHslider38);
		int iSlow234 = (fSlow233 > 0.0f);
		float fSlow235 = (float(fHslider39) * std::sin((fConst48 * fSlow180)));
		float fSlow236 = (fConst28 * ((fSlow180 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow233)))) / fSlow235));
		float fSlow237 = (fConst28 * (fSlow180 / fSlow235));
		float fSlow238 = (iSlow234 ? fSlow237 : fSlow236);
		float fSlow239 = ((fSlow232 * (fSlow232 - fSlow238)) + 1.0f);
		float fSlow240 = ((fSlow232 * (fSlow232 + fSlow238)) + 1.0f);
		float fSlow241 = (iSlow234 ? fSlow236 : fSlow237);
		float fSlow242 = ((fSlow232 * (fSlow232 + fSlow241)) + 1.0f);
		float fSlow243 = ((fSlow232 * (fSlow232 - fSlow241)) + 1.0f);
		float fSlow244 = (1.0f / fSlow178);
		float fSlow245 = (((fSlow177 - fSlow175) / fSlow176) + 1.0f);
		float fSlow246 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow176))));
		float fSlow247 = (1.0f / float(fHslider41));
		float fSlow248 = std::tan((fConst28 * float(fHslider42)));
		float fSlow249 = (1.0f / fSlow248);
		float fSlow250 = (((fSlow247 + fSlow249) / fSlow248) + 1.0f);
		float fSlow251 = ((std::pow(10.0f, (0.0500000007f * float(fHslider40))) * (1.0f - float(fCheckbox2))) / fSlow250);
		float fSlow252 = float(fHslider43);
		float fSlow253 = std::tan((fConst28 * fSlow252));
		float fSlow254 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow253))));
		float fSlow255 = float(fHslider44);
		float fSlow256 = std::tan((fConst28 * fSlow255));
		float fSlow257 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow256))));
		float fSlow258 = float(fHslider45);
		float fSlow259 = float((fSlow258 > 0.0f));
		float fSlow260 = (0.0f - fSlow258);
		float fSlow261 = std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider46)));
		float fSlow262 = ((fSlow261 * fSlow15) * ((0.0500000007f * fSlow17) + 1.0f));
		float fSlow263 = (fConst219 * ((0.200000003f * ((0.5f * fSlow131) + -5.0f)) + 1.0f));
		float fSlow264 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider47))) * fSlow15) * (1.0f - (0.0299999993f * fSlow17)));
		float fSlow265 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider48))) * fSlow15) * ((0.310000002f * fSlow17) + 1.0f));
		float fSlow266 = ((std::min<float>(20000.0f, std::max<float>(20.0f, float(fHslider49))) * fSlow15) * (1.0f - (0.270000011f * fSlow17)));
		float fSlow267 = (fConst47 * ((1.0f / float(fHslider50)) + -1.0f));
		float fSlow268 = float(fHslider51);
		int iSlow269 = (std::fabs(fSlow268) < 1.1920929e-07f);
		float fSlow270 = (iSlow269 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow269 ? 1.0f : fSlow268)))));
		float fSlow271 = float(fHslider52);
		int iSlow272 = (std::fabs(fSlow271) < 1.1920929e-07f);
		float fSlow273 = (iSlow272 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow272 ? 1.0f : fSlow271)))));
		float fSlow274 = (1.0f - fSlow273);
		float fSlow275 = (1.0f - fSlow270);
		float fSlow276 = float(fHslider53);
		float fSlow277 = (1.0f / float(fHslider54));
		float fSlow278 = std::tan((fConst28 * float(fHslider55)));
		float fSlow279 = (1.0f / fSlow278);
		float fSlow280 = (1.0f / (((fSlow277 + fSlow279) / fSlow278) + 1.0f));
		float fSlow281 = (((fSlow279 - fSlow277) / fSlow278) + 1.0f);
		float fSlow282 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow278))));
		float fSlow283 = (1.0f / fSlow256);
		float fSlow284 = float(fHslider56);
		int iSlow285 = (fSlow284 > 0.0f);
		float fSlow286 = (float(fHslider57) * std::sin((fConst48 * fSlow255)));
		float fSlow287 = (fConst28 * ((fSlow255 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow284)))) / fSlow286));
		float fSlow288 = (fConst28 * (fSlow255 / fSlow286));
		float fSlow289 = (iSlow285 ? fSlow288 : fSlow287);
		float fSlow290 = ((fSlow283 * (fSlow283 - fSlow289)) + 1.0f);
		float fSlow291 = ((fSlow283 * (fSlow283 + fSlow289)) + 1.0f);
		float fSlow292 = (iSlow285 ? fSlow287 : fSlow288);
		float fSlow293 = ((fSlow283 * (fSlow283 + fSlow292)) + 1.0f);
		float fSlow294 = ((fSlow283 * (fSlow283 - fSlow292)) + 1.0f);
		float fSlow295 = (1.0f / fSlow253);
		float fSlow296 = float(fHslider58);
		int iSlow297 = (fSlow296 > 0.0f);
		float fSlow298 = (float(fHslider59) * std::sin((fConst48 * fSlow252)));
		float fSlow299 = (fConst28 * ((fSlow252 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow296)))) / fSlow298));
		float fSlow300 = (fConst28 * (fSlow252 / fSlow298));
		float fSlow301 = (iSlow297 ? fSlow300 : fSlow299);
		float fSlow302 = ((fSlow295 * (fSlow295 - fSlow301)) + 1.0f);
		float fSlow303 = ((fSlow295 * (fSlow295 + fSlow301)) + 1.0f);
		float fSlow304 = (iSlow297 ? fSlow299 : fSlow300);
		float fSlow305 = ((fSlow295 * (fSlow295 + fSlow304)) + 1.0f);
		float fSlow306 = ((fSlow295 * (fSlow295 - fSlow304)) + 1.0f);
		float fSlow307 = (1.0f / fSlow250);
		float fSlow308 = (((fSlow249 - fSlow247) / fSlow248) + 1.0f);
		float fSlow309 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow248))));
		float fSlow310 = (1.0f / float(fHslider61));
		float fSlow311 = std::tan((fConst28 * float(fHslider62)));
		float fSlow312 = (1.0f / fSlow311);
		float fSlow313 = (((fSlow310 + fSlow312) / fSlow311) + 1.0f);
		float fSlow314 = ((std::pow(10.0f, (0.0500000007f * float(fHslider60))) * (1.0f - float(fCheckbox3))) / fSlow313);
		float fSlow315 = (0.300000012f * fSlow314);
		float fSlow316 = float(fHslider63);
		float fSlow317 = std::tan((fConst28 * fSlow316));
		float fSlow318 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow317))));
		float fSlow319 = float(fHslider64);
		float fSlow320 = std::tan((fConst28 * fSlow319));
		float fSlow321 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow320))));
		float fSlow322 = float(fHslider65);
		float fSlow323 = float((fSlow322 > 0.0f));
		float fSlow324 = (0.0f - fSlow322);
		float fSlow325 = (1.0f / std::max<float>(1.0f, (fConst152 * (std::min<float>(10.0f, std::max<float>(1.0f, float(fHslider66))) + 1.0f))));
		float fSlow326 = (fConst47 * ((1.0f / float(fHslider67)) + -1.0f));
		float fSlow327 = float(fHslider68);
		int iSlow328 = (std::fabs(fSlow327) < 1.1920929e-07f);
		float fSlow329 = (iSlow328 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow328 ? 1.0f : fSlow327)))));
		float fSlow330 = float(fHslider69);
		int iSlow331 = (std::fabs(fSlow330) < 1.1920929e-07f);
		float fSlow332 = (iSlow331 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow331 ? 1.0f : fSlow330)))));
		float fSlow333 = (1.0f - fSlow332);
		float fSlow334 = (1.0f - fSlow329);
		float fSlow335 = float(fHslider70);
		float fSlow336 = (1.0f / float(fHslider71));
		float fSlow337 = std::tan((fConst28 * float(fHslider72)));
		float fSlow338 = (1.0f / fSlow337);
		float fSlow339 = (1.0f / (((fSlow336 + fSlow338) / fSlow337) + 1.0f));
		float fSlow340 = (((fSlow338 - fSlow336) / fSlow337) + 1.0f);
		float fSlow341 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow337))));
		float fSlow342 = (1.0f / fSlow320);
		float fSlow343 = float(fHslider73);
		int iSlow344 = (fSlow343 > 0.0f);
		float fSlow345 = (float(fHslider74) * std::sin((fConst48 * fSlow319)));
		float fSlow346 = (fConst28 * ((fSlow319 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow343)))) / fSlow345));
		float fSlow347 = (fConst28 * (fSlow319 / fSlow345));
		float fSlow348 = (iSlow344 ? fSlow347 : fSlow346);
		float fSlow349 = ((fSlow342 * (fSlow342 - fSlow348)) + 1.0f);
		float fSlow350 = ((fSlow342 * (fSlow342 + fSlow348)) + 1.0f);
		float fSlow351 = (iSlow344 ? fSlow346 : fSlow347);
		float fSlow352 = ((fSlow342 * (fSlow342 + fSlow351)) + 1.0f);
		float fSlow353 = ((fSlow342 * (fSlow342 - fSlow351)) + 1.0f);
		float fSlow354 = (1.0f / fSlow317);
		float fSlow355 = float(fHslider75);
		int iSlow356 = (fSlow355 > 0.0f);
		float fSlow357 = (float(fHslider76) * std::sin((fConst48 * fSlow316)));
		float fSlow358 = (fConst28 * ((fSlow316 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow355)))) / fSlow357));
		float fSlow359 = (fConst28 * (fSlow316 / fSlow357));
		float fSlow360 = (iSlow356 ? fSlow359 : fSlow358);
		float fSlow361 = ((fSlow354 * (fSlow354 - fSlow360)) + 1.0f);
		float fSlow362 = ((fSlow354 * (fSlow354 + fSlow360)) + 1.0f);
		float fSlow363 = (iSlow356 ? fSlow358 : fSlow359);
		float fSlow364 = ((fSlow354 * (fSlow354 + fSlow363)) + 1.0f);
		float fSlow365 = ((fSlow354 * (fSlow354 - fSlow363)) + 1.0f);
		float fSlow366 = (1.0f / fSlow313);
		float fSlow367 = (((fSlow312 - fSlow310) / fSlow311) + 1.0f);
		float fSlow368 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow311))));
		float fSlow369 = (1.0f / float(fHslider78));
		float fSlow370 = std::tan((fConst28 * float(fHslider79)));
		float fSlow371 = (1.0f / fSlow370);
		float fSlow372 = (((fSlow369 + fSlow371) / fSlow370) + 1.0f);
		float fSlow373 = ((std::pow(10.0f, (0.0500000007f * float(fHslider77))) * (1.0f - float(fCheckbox4))) / fSlow372);
		float fSlow374 = float(fHslider80);
		float fSlow375 = std::tan((fConst28 * fSlow374));
		float fSlow376 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow375))));
		float fSlow377 = float(fHslider81);
		float fSlow378 = std::tan((fConst28 * fSlow377));
		float fSlow379 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow378))));
		float fSlow380 = (0.5f * fSlow261);
		float fSlow381 = (fConst486 * fSlow15);
		float fSlow382 = (27.0f * fSlow15);
		float fSlow383 = (9.0f * fSlow15);
		float fSlow384 = (3.0f * fSlow15);
		float fSlow385 = (fConst47 * ((1.0f / float(fHslider82)) + -1.0f));
		float fSlow386 = float(fHslider83);
		int iSlow387 = (std::fabs(fSlow386) < 1.1920929e-07f);
		float fSlow388 = (iSlow387 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow387 ? 1.0f : fSlow386)))));
		float fSlow389 = float(fHslider84);
		int iSlow390 = (std::fabs(fSlow389) < 1.1920929e-07f);
		float fSlow391 = (iSlow390 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow390 ? 1.0f : fSlow389)))));
		float fSlow392 = (1.0f - fSlow391);
		float fSlow393 = (1.0f - fSlow388);
		float fSlow394 = float(fHslider85);
		float fSlow395 = (1.0f / float(fHslider86));
		float fSlow396 = std::tan((fConst28 * float(fHslider87)));
		float fSlow397 = (1.0f / fSlow396);
		float fSlow398 = (1.0f / (((fSlow395 + fSlow397) / fSlow396) + 1.0f));
		float fSlow399 = (((fSlow397 - fSlow395) / fSlow396) + 1.0f);
		float fSlow400 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow396))));
		float fSlow401 = (1.0f / fSlow378);
		float fSlow402 = float(fHslider88);
		int iSlow403 = (fSlow402 > 0.0f);
		float fSlow404 = (float(fHslider89) * std::sin((fConst48 * fSlow377)));
		float fSlow405 = (fConst28 * ((fSlow377 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow402)))) / fSlow404));
		float fSlow406 = (fConst28 * (fSlow377 / fSlow404));
		float fSlow407 = (iSlow403 ? fSlow406 : fSlow405);
		float fSlow408 = ((fSlow401 * (fSlow401 - fSlow407)) + 1.0f);
		float fSlow409 = ((fSlow401 * (fSlow401 + fSlow407)) + 1.0f);
		float fSlow410 = (iSlow403 ? fSlow405 : fSlow406);
		float fSlow411 = ((fSlow401 * (fSlow401 + fSlow410)) + 1.0f);
		float fSlow412 = ((fSlow401 * (fSlow401 - fSlow410)) + 1.0f);
		float fSlow413 = (1.0f / fSlow375);
		float fSlow414 = float(fHslider90);
		int iSlow415 = (fSlow414 > 0.0f);
		float fSlow416 = (float(fHslider91) * std::sin((fConst48 * fSlow374)));
		float fSlow417 = (fConst28 * ((fSlow374 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow414)))) / fSlow416));
		float fSlow418 = (fConst28 * (fSlow374 / fSlow416));
		float fSlow419 = (iSlow415 ? fSlow418 : fSlow417);
		float fSlow420 = ((fSlow413 * (fSlow413 - fSlow419)) + 1.0f);
		float fSlow421 = ((fSlow413 * (fSlow413 + fSlow419)) + 1.0f);
		float fSlow422 = (iSlow415 ? fSlow417 : fSlow418);
		float fSlow423 = ((fSlow413 * (fSlow413 + fSlow422)) + 1.0f);
		float fSlow424 = ((fSlow413 * (fSlow413 - fSlow422)) + 1.0f);
		float fSlow425 = (1.0f / fSlow372);
		float fSlow426 = (((fSlow371 - fSlow369) / fSlow370) + 1.0f);
		float fSlow427 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow370))));
		float fSlow428 = (1.0f / float(fHslider93));
		float fSlow429 = std::tan((fConst28 * float(fHslider94)));
		float fSlow430 = (1.0f / fSlow429);
		float fSlow431 = (((fSlow428 + fSlow430) / fSlow429) + 1.0f);
		float fSlow432 = ((std::pow(10.0f, (0.0500000007f * float(fHslider92))) * (1.0f - float(fCheckbox5))) / fSlow431);
		float fSlow433 = float(fHslider95);
		float fSlow434 = std::tan((fConst28 * fSlow433));
		float fSlow435 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow434))));
		float fSlow436 = float(fHslider96);
		float fSlow437 = std::tan((fConst28 * fSlow436));
		float fSlow438 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow437))));
		float fSlow439 = (2.0f * fSlow15);
		float fSlow440 = float(fHslider97);
		float fSlow441 = float((fSlow440 > 0.0f));
		float fSlow442 = (0.0f - fSlow440);
		float fSlow443 = (2.0f * fSlow14);
		float fSlow444 = std::min<float>(1.0f, std::max<float>(0.00999999978f, float(fHslider98)));
		float fSlow445 = (1.0f - fSlow444);
		float fSlow446 = (fConst47 * ((1.0f / float(fHslider99)) + -1.0f));
		float fSlow447 = float(fHslider100);
		int iSlow448 = (std::fabs(fSlow447) < 1.1920929e-07f);
		float fSlow449 = (iSlow448 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow448 ? 1.0f : fSlow447)))));
		float fSlow450 = float(fHslider101);
		int iSlow451 = (std::fabs(fSlow450) < 1.1920929e-07f);
		float fSlow452 = (iSlow451 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow451 ? 1.0f : fSlow450)))));
		float fSlow453 = (1.0f - fSlow452);
		float fSlow454 = (1.0f - fSlow449);
		float fSlow455 = float(fHslider102);
		float fSlow456 = (1.0f / float(fHslider103));
		float fSlow457 = std::tan((fConst28 * float(fHslider104)));
		float fSlow458 = (1.0f / fSlow457);
		float fSlow459 = (1.0f / (((fSlow456 + fSlow458) / fSlow457) + 1.0f));
		float fSlow460 = (((fSlow458 - fSlow456) / fSlow457) + 1.0f);
		float fSlow461 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow457))));
		float fSlow462 = (1.0f / fSlow437);
		float fSlow463 = float(fHslider105);
		int iSlow464 = (fSlow463 > 0.0f);
		float fSlow465 = (float(fHslider106) * std::sin((fConst48 * fSlow436)));
		float fSlow466 = (fConst28 * ((fSlow436 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow463)))) / fSlow465));
		float fSlow467 = (fConst28 * (fSlow436 / fSlow465));
		float fSlow468 = (iSlow464 ? fSlow467 : fSlow466);
		float fSlow469 = ((fSlow462 * (fSlow462 - fSlow468)) + 1.0f);
		float fSlow470 = ((fSlow462 * (fSlow462 + fSlow468)) + 1.0f);
		float fSlow471 = (iSlow464 ? fSlow466 : fSlow467);
		float fSlow472 = ((fSlow462 * (fSlow462 + fSlow471)) + 1.0f);
		float fSlow473 = ((fSlow462 * (fSlow462 - fSlow471)) + 1.0f);
		float fSlow474 = (1.0f / fSlow434);
		float fSlow475 = float(fHslider107);
		int iSlow476 = (fSlow475 > 0.0f);
		float fSlow477 = (float(fHslider108) * std::sin((fConst48 * fSlow433)));
		float fSlow478 = (fConst28 * ((fSlow433 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow475)))) / fSlow477));
		float fSlow479 = (fConst28 * (fSlow433 / fSlow477));
		float fSlow480 = (iSlow476 ? fSlow479 : fSlow478);
		float fSlow481 = ((fSlow474 * (fSlow474 - fSlow480)) + 1.0f);
		float fSlow482 = ((fSlow474 * (fSlow474 + fSlow480)) + 1.0f);
		float fSlow483 = (iSlow476 ? fSlow478 : fSlow479);
		float fSlow484 = ((fSlow474 * (fSlow474 + fSlow483)) + 1.0f);
		float fSlow485 = ((fSlow474 * (fSlow474 - fSlow483)) + 1.0f);
		float fSlow486 = (1.0f / fSlow431);
		float fSlow487 = (((fSlow430 - fSlow428) / fSlow429) + 1.0f);
		float fSlow488 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow429))));
		float fSlow489 = (1.0f / float(fHslider110));
		float fSlow490 = std::tan((fConst28 * float(fHslider111)));
		float fSlow491 = (1.0f / fSlow490);
		float fSlow492 = (((fSlow489 + fSlow491) / fSlow490) + 1.0f);
		float fSlow493 = ((std::pow(10.0f, (0.0500000007f * float(fHslider109))) * (1.0f - float(fCheckbox6))) / fSlow492);
		float fSlow494 = float(fHslider112);
		float fSlow495 = std::tan((fConst28 * fSlow494));
		float fSlow496 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow495))));
		float fSlow497 = float(fHslider113);
		float fSlow498 = std::tan((fConst28 * fSlow497));
		float fSlow499 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow498))));
		float fSlow500 = float(fHslider114);
		float fSlow501 = float((fSlow500 > 0.0f));
		float fSlow502 = (0.0f - fSlow500);
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
		float fSlow519 = (1.0f / fSlow498);
		float fSlow520 = float(fHslider121);
		int iSlow521 = (fSlow520 > 0.0f);
		float fSlow522 = (float(fHslider122) * std::sin((fConst48 * fSlow497)));
		float fSlow523 = (fConst28 * ((fSlow497 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow520)))) / fSlow522));
		float fSlow524 = (fConst28 * (fSlow497 / fSlow522));
		float fSlow525 = (iSlow521 ? fSlow524 : fSlow523);
		float fSlow526 = ((fSlow519 * (fSlow519 - fSlow525)) + 1.0f);
		float fSlow527 = ((fSlow519 * (fSlow519 + fSlow525)) + 1.0f);
		float fSlow528 = (iSlow521 ? fSlow523 : fSlow524);
		float fSlow529 = ((fSlow519 * (fSlow519 + fSlow528)) + 1.0f);
		float fSlow530 = ((fSlow519 * (fSlow519 - fSlow528)) + 1.0f);
		float fSlow531 = (1.0f / fSlow495);
		float fSlow532 = float(fHslider123);
		int iSlow533 = (fSlow532 > 0.0f);
		float fSlow534 = (float(fHslider124) * std::sin((fConst48 * fSlow494)));
		float fSlow535 = (fConst28 * ((fSlow494 * std::pow(10.0f, (0.0500000007f * std::fabs(fSlow532)))) / fSlow534));
		float fSlow536 = (fConst28 * (fSlow494 / fSlow534));
		float fSlow537 = (iSlow533 ? fSlow536 : fSlow535);
		float fSlow538 = ((fSlow531 * (fSlow531 - fSlow537)) + 1.0f);
		float fSlow539 = ((fSlow531 * (fSlow531 + fSlow537)) + 1.0f);
		float fSlow540 = (iSlow533 ? fSlow535 : fSlow536);
		float fSlow541 = ((fSlow531 * (fSlow531 + fSlow540)) + 1.0f);
		float fSlow542 = ((fSlow531 * (fSlow531 - fSlow540)) + 1.0f);
		float fSlow543 = (1.0f / fSlow492);
		float fSlow544 = (((fSlow491 - fSlow489) / fSlow490) + 1.0f);
		float fSlow545 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow490))));
		float fSlow546 = (1.41400003f * std::sqrt(fSlow0));
		float fSlow547 = (0.949999988f * fSlow314);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec12[0] = (0.0f - (fConst24 * ((fConst25 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fConst20 * fRec11[1]) + (fConst21 * (fRec8[1] + (fConst22 * fRec12[0]))));
			fVec1[(IOTA & 32767)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fRec18[0] = (fConst33 + (fRec18[1] - std::floor((fConst33 + fRec18[1]))));
			float fTemp0 = ((0.00999999978f * ftbl0mydspSIG0[int((65536.0f * fRec18[0]))]) + 1.0f);
			fRec21[0] = (fConst36 + (fRec21[1] - std::floor((fConst36 + fRec21[1]))));
			fRec20[0] = ((0.999000013f * fRec20[1]) + (0.00100000005f * ((fSlow18 * ftbl0mydspSIG0[int((65536.0f * fRec21[0]))]) + 1.0f)));
			fRec19[0] = ((fConst34 * fRec19[1]) + (fConst35 * (std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow16 * fRec20[0]))) * fTemp0)));
			float fTemp1 = ((0.00366666657f * (400.0f - fRec19[0])) + 3.0f);
			float fTemp2 = std::max<float>(1.1920929e-07f, std::fabs(fRec19[0]));
			float fTemp3 = (fRec23[1] + (fConst5 * fTemp2));
			float fTemp4 = (fTemp3 + -1.0f);
			int iTemp5 = (fTemp4 < 0.0f);
			fRec23[0] = (iTemp5 ? fTemp3 : fTemp4);
			float fRec24 = (iTemp5 ? fTemp3 : (fTemp3 + ((1.0f - (fConst0 / fTemp2)) * fTemp4)));
			iRec25[0] = ((1103515245 * iRec25[1]) + 12345);
			float fTemp6 = float(iRec25[0]);
			float fTemp7 = (0.400000006f * ((fSlow20 * ((2.0f * fRec24) + -1.0f)) + (fSlow21 * fTemp6)));
			float fTemp8 = ((fSlow73 <= fRec19[0]) ? fRec19[0] : fSlow73);
			float fTemp9 = std::tan((fConst28 * fTemp8));
			float fTemp10 = (1.0f / fTemp9);
			float fTemp11 = (fSlow81 / fTemp8);
			float fTemp12 = (((fTemp10 + fTemp11) / fTemp9) + 1.0f);
			fRec22[0] = (fTemp7 - (((fRec22[2] * (((fTemp10 - fTemp11) / fTemp9) + 1.0f)) + (2.0f * (fRec22[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp9)))))) / fTemp12));
			float fTemp13 = (fTemp9 * fTemp12);
			float fTemp14 = ((2.0f * fRec19[0]) + 30.0f);
			float fTemp15 = ((iSlow83 & (fRec19[0] >= 200.0f)) ? (fSlow82 - (0.000952380942f * ((fRec19[0] + -200.0f) * fSlow84))) : ((fSlow82 <= fTemp14) ? fTemp14 : fSlow82));
			float fTemp16 = std::tan((fConst28 * fTemp15));
			float fTemp17 = (1.0f / fTemp16);
			float fTemp18 = (fSlow89 / fTemp15);
			float fTemp19 = (((fTemp17 + fTemp18) / fTemp16) + 1.0f);
			fRec26[0] = (fTemp7 - (((fRec26[2] * (((fTemp17 - fTemp18) / fTemp16) + 1.0f)) + (2.0f * (fRec26[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp16)))))) / fTemp19));
			float fTemp20 = (fTemp16 * fTemp19);
			fRec27[0] = (fTemp7 - (((fRec27[2] * fSlow102) + (2.0f * (fRec27[1] * fSlow103))) / fSlow104));
			fRec28[0] = (fTemp7 - (((fRec28[2] * fSlow113) + (2.0f * (fRec28[1] * fSlow114))) / fSlow115));
			fRec29[0] = (fTemp7 - (((fRec29[2] * fSlow123) + (2.0f * (fRec29[1] * fSlow124))) / fSlow125));
			fVec2[0] = fSlow128;
			float fTemp21 = (fSlow128 - fVec2[1]);
			fVec3[0] = fSlow130;
			float fTemp22 = (0.0f - (fSlow128 + fVec3[1]));
			float fTemp23 = std::min<float>(1.0f, ((fTemp21 * float((fTemp21 > 0.0f))) + (fSlow129 * (fTemp22 * float((fTemp22 > 0.0f))))));
			fVec4[0] = fTemp23;
			iRec30[0] = (((iRec30[1] + (iRec30[1] > 0)) * (fTemp23 <= fVec4[1])) + (fTemp23 > fVec4[1]));
			float fTemp24 = float(iRec30[0]);
			fRec16[0] = (((fTemp0 * ((fTemp1 * (((((fRec22[0] / fTemp13) + ((fRec26[0] * fSlow90) / fTemp20)) + ((fRec27[0] * fSlow106) / fSlow107)) + ((fRec28[0] * fSlow117) / fSlow118)) + ((fRec29[0] * fSlow126) / fSlow127))) + (((((fRec22[2] * (0.0f - (fTemp1 / fTemp13))) + (fRec26[2] * (0.0f - ((fTemp1 * fSlow90) / fTemp20)))) + (fRec27[2] * (0.0f - ((fTemp1 * fSlow106) / fSlow107)))) + (fRec28[2] * (0.0f - ((fTemp1 * fSlow117) / fSlow118)))) + (fRec29[2] * (0.0f - ((fTemp1 * fSlow126) / fSlow127)))))) * std::sqrt(std::max<float>(0.0f, std::min<float>((fConst38 * fTemp24), ((fConst39 * (fConst37 - fTemp24)) + 1.0f))))) - (fConst40 * ((fConst41 * fRec16[2]) + (fConst42 * fRec16[1]))));
			fRec31[0] = (int(fTemp23) ? fSlow131 : fRec31[1]);
			float fTemp25 = (((fRec16[2] + (fRec16[0] + (2.0f * fRec16[1]))) * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp24), ((fConst45 * (fConst43 - fTemp24)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec31[0] + -10.0f))));
			float fTemp26 = std::fabs((fConst32 * fTemp25));
			fRec34[0] = std::max<float>(fTemp26, ((fRec34[1] * fSlow138) + (fTemp26 * fSlow139)));
			fRec33[0] = ((fRec33[1] * fSlow135) + (fRec34[0] * fSlow140));
			fRec32[0] = ((fConst46 * fRec32[1]) + (fSlow132 * std::max<float>(((20.0f * std::log10(fRec33[0])) - fSlow141), 0.0f)));
			float fTemp27 = (fConst32 * (fTemp25 * std::pow(10.0f, (0.0500000007f * fRec32[0]))));
			fRec35[0] = (fTemp27 - (fSlow145 * ((fSlow146 * fRec35[2]) + (fSlow147 * fRec35[1]))));
			float fTemp28 = (fSlow13 * fRec15[1]);
			fRec15[0] = (fTemp27 - ((fSlow145 * (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])))) + (((fRec15[2] * fSlow155) + fTemp28) / fSlow156)));
			float fTemp29 = (fSlow10 * fRec14[1]);
			fRec14[0] = ((((fTemp28 + (fRec15[0] * fSlow158)) + (fRec15[2] * fSlow159)) / fSlow156) - (((fRec14[2] * fSlow167) + fTemp29) / fSlow168));
			fRec13[0] = ((((fTemp29 + (fRec14[0] * fSlow170)) + (fRec14[2] * fSlow171)) / fSlow168) - (fSlow172 * ((fSlow173 * fRec13[2]) + (fSlow174 * fRec13[1]))));
			float fTemp30 = (fSlow7 * (fRec13[2] + (fRec13[0] + (2.0f * fRec13[1]))));
			fVec5[(IOTA & 2047)] = fTemp30;
			float fTemp31 = (0.300000012f * fVec5[((IOTA - iConst49) & 2047)]);
			float fTemp32 = (((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst27) & 32767)]) - fTemp31);
			fVec6[(IOTA & 2047)] = fTemp32;
			fRec9[0] = fVec6[((IOTA - iConst50) & 2047)];
			float fRec10 = (0.0f - (0.600000024f * fTemp32));
			fRec39[0] = (0.0f - (fConst24 * ((fConst25 * fRec39[1]) - (fRec4[1] + fRec4[2]))));
			fRec38[0] = ((fConst59 * fRec38[1]) + (fConst60 * (fRec4[1] + (fConst61 * fRec39[0]))));
			fVec7[(IOTA & 32767)] = ((0.353553385f * fRec38[0]) + 9.99999968e-21f);
			float fTemp33 = (((0.600000024f * fRec36[1]) + fVec7[((IOTA - iConst63) & 32767)]) - fTemp31);
			fVec8[(IOTA & 4095)] = fTemp33;
			fRec36[0] = fVec8[((IOTA - iConst64) & 4095)];
			float fRec37 = (0.0f - (0.600000024f * fTemp33));
			fRec43[0] = (0.0f - (fConst24 * ((fConst25 * fRec43[1]) - (fRec6[1] + fRec6[2]))));
			fRec42[0] = ((fConst73 * fRec42[1]) + (fConst74 * (fRec6[1] + (fConst75 * fRec43[0]))));
			fVec9[(IOTA & 16383)] = ((0.353553385f * fRec42[0]) + 9.99999968e-21f);
			float fTemp34 = (fVec9[((IOTA - iConst77) & 16383)] + (fTemp31 + (0.600000024f * fRec40[1])));
			fVec10[(IOTA & 4095)] = fTemp34;
			fRec40[0] = fVec10[((IOTA - iConst78) & 4095)];
			float fRec41 = (0.0f - (0.600000024f * fTemp34));
			fRec47[0] = (0.0f - (fConst24 * ((fConst25 * fRec47[1]) - (fRec2[1] + fRec2[2]))));
			fRec46[0] = ((fConst87 * fRec46[1]) + (fConst88 * (fRec2[1] + (fConst89 * fRec47[0]))));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec46[0]) + 9.99999968e-21f);
			float fTemp35 = (fVec11[((IOTA - iConst91) & 32767)] + (fTemp31 + (0.600000024f * fRec44[1])));
			fVec12[(IOTA & 4095)] = fTemp35;
			fRec44[0] = fVec12[((IOTA - iConst92) & 4095)];
			float fRec45 = (0.0f - (0.600000024f * fTemp35));
			fRec51[0] = (0.0f - (fConst24 * ((fConst25 * fRec51[1]) - (fRec7[1] + fRec7[2]))));
			fRec50[0] = ((fConst101 * fRec50[1]) + (fConst102 * (fRec7[1] + (fConst103 * fRec51[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec50[0]) + 9.99999968e-21f);
			float fTemp36 = (fVec13[((IOTA - iConst105) & 16383)] - (fTemp31 + (0.600000024f * fRec48[1])));
			fVec14[(IOTA & 2047)] = fTemp36;
			fRec48[0] = fVec14[((IOTA - iConst106) & 2047)];
			float fRec49 = (0.600000024f * fTemp36);
			fRec55[0] = (0.0f - (fConst24 * ((fConst25 * fRec55[1]) - (fRec3[1] + fRec3[2]))));
			fRec54[0] = ((fConst115 * fRec54[1]) + (fConst116 * (fRec3[1] + (fConst117 * fRec55[0]))));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec54[0]) + 9.99999968e-21f);
			float fTemp37 = (fVec15[((IOTA - iConst119) & 16383)] - (fTemp31 + (0.600000024f * fRec52[1])));
			fVec16[(IOTA & 4095)] = fTemp37;
			fRec52[0] = fVec16[((IOTA - iConst120) & 4095)];
			float fRec53 = (0.600000024f * fTemp37);
			fRec59[0] = (0.0f - (fConst24 * ((fConst25 * fRec59[1]) - (fRec5[1] + fRec5[2]))));
			fRec58[0] = ((fConst129 * fRec58[1]) + (fConst130 * (fRec5[1] + (fConst131 * fRec59[0]))));
			fVec17[(IOTA & 16383)] = ((0.353553385f * fRec58[0]) + 9.99999968e-21f);
			float fTemp38 = ((fTemp31 + fVec17[((IOTA - iConst133) & 16383)]) - (0.600000024f * fRec56[1]));
			fVec18[(IOTA & 4095)] = fTemp38;
			fRec56[0] = fVec18[((IOTA - iConst134) & 4095)];
			float fRec57 = (0.600000024f * fTemp38);
			fRec63[0] = (0.0f - (fConst24 * ((fConst25 * fRec63[1]) - (fRec1[1] + fRec1[2]))));
			fRec62[0] = ((fConst143 * fRec62[1]) + (fConst144 * (fRec1[1] + (fConst145 * fRec63[0]))));
			fVec19[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp39 = ((fVec19[((IOTA - iConst147) & 16383)] + fTemp31) - (0.600000024f * fRec60[1]));
			fVec20[(IOTA & 2047)] = fTemp39;
			fRec60[0] = fVec20[((IOTA - iConst148) & 2047)];
			float fRec61 = (0.600000024f * fTemp39);
			float fTemp40 = (fRec61 + fRec57);
			float fTemp41 = (fRec49 + (fRec53 + fTemp40));
			fRec1[0] = (fRec9[1] + (fRec36[1] + (fRec40[1] + (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec60[1] + (fRec10 + (fRec37 + (fRec41 + (fRec45 + fTemp41))))))))))));
			fRec2[0] = ((fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec60[1] + fTemp41)))) - (fRec9[1] + (fRec36[1] + (fRec40[1] + (fRec44[1] + (fRec10 + (fRec37 + (fRec45 + fRec41))))))));
			float fTemp42 = (fRec53 + fRec49);
			fRec3[0] = ((fRec40[1] + (fRec44[1] + (fRec56[1] + (fRec60[1] + (fRec41 + (fRec45 + fTemp40)))))) - (fRec9[1] + (fRec36[1] + (fRec48[1] + (fRec52[1] + (fRec10 + (fRec37 + fTemp42)))))));
			fRec4[0] = ((fRec9[1] + (fRec36[1] + (fRec56[1] + (fRec60[1] + (fRec10 + (fRec37 + fTemp40)))))) - (fRec40[1] + (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec41 + (fRec45 + fTemp42)))))));
			float fTemp43 = (fRec61 + fRec53);
			float fTemp44 = (fRec57 + fRec49);
			fRec5[0] = ((fRec36[1] + (fRec44[1] + (fRec52[1] + (fRec60[1] + (fRec37 + (fRec45 + fTemp43)))))) - (fRec9[1] + (fRec40[1] + (fRec48[1] + (fRec56[1] + (fRec10 + (fRec41 + fTemp44)))))));
			fRec6[0] = ((fRec9[1] + (fRec40[1] + (fRec52[1] + (fRec60[1] + (fRec10 + (fRec41 + fTemp43)))))) - (fRec36[1] + (fRec44[1] + (fRec48[1] + (fRec56[1] + (fRec37 + (fRec45 + fTemp44)))))));
			float fTemp45 = (fRec61 + fRec49);
			float fTemp46 = (fRec57 + fRec53);
			fRec7[0] = ((fRec9[1] + (fRec44[1] + (fRec48[1] + (fRec60[1] + (fRec10 + (fRec45 + fTemp45)))))) - (fRec36[1] + (fRec40[1] + (fRec52[1] + (fRec56[1] + (fRec37 + (fRec41 + fTemp46)))))));
			fRec8[0] = ((fRec36[1] + (fRec40[1] + (fRec48[1] + (fRec60[1] + (fRec37 + (fRec41 + fTemp45)))))) - (fRec9[1] + (fRec44[1] + (fRec52[1] + (fRec56[1] + (fRec10 + (fRec45 + fTemp46)))))));
			fRec0[0] = ((0.370000005f * (fRec2[0] + fRec3[0])) - (fConst149 * ((fConst150 * fRec0[1]) + (fConst151 * fRec0[2]))));
			float fTemp47 = (4.65661287e-10f * fTemp6);
			fRec69[0] = (fTemp47 - (fSlow192 * ((fSlow195 * fRec69[2]) + (fSlow196 * fRec69[1]))));
			fRec68[0] = ((fSlow192 * (((fSlow194 * fRec69[0]) + (fSlow197 * fRec69[1])) + (fSlow194 * fRec69[2]))) - (fSlow189 * ((fSlow198 * fRec68[2]) + (fSlow199 * fRec68[1]))));
			fVec21[0] = fSlow201;
			float fTemp48 = (fSlow201 - fVec21[1]);
			fVec22[0] = fSlow203;
			float fTemp49 = (0.0f - (fSlow201 + fVec22[1]));
			float fTemp50 = std::min<float>(1.0f, ((fTemp48 * float((fTemp48 > 0.0f))) + (fSlow202 * (fTemp49 * float((fTemp49 > 0.0f))))));
			fVec23[0] = fTemp50;
			iRec70[0] = (((iRec70[1] + (iRec70[1] > 0)) * (fTemp50 <= fVec23[1])) + (fTemp50 > fVec23[1]));
			float fTemp51 = (fSlow200 * float(iRec70[0]));
			float fTemp52 = (fSlow189 * ((fRec68[2] + (fRec68[0] + (2.0f * fRec68[1]))) * std::max<float>(0.0f, std::min<float>(fTemp51, (2.0f - fTemp51)))));
			int iTemp53 = int(fTemp50);
			fRec71[0] = (iTemp53 ? fSlow201 : fRec71[1]);
			fRec67[0] = (fTemp52 - ((fConst154 * (fRec67[1] * std::cos((fConst48 * (fRec71[0] + 50.0f))))) + (fConst155 * fRec67[2])));
			fRec72[0] = (fTemp52 - ((fConst157 * (fRec72[1] * std::cos((fConst48 * (fRec71[0] + 250.0f))))) + (fConst158 * fRec72[2])));
			fRec73[0] = (fTemp52 - ((fConst160 * (fRec73[1] * std::cos((fConst48 * (fRec71[0] + 450.0f))))) + (fConst161 * fRec73[2])));
			fRec74[0] = (fTemp52 - ((fConst163 * (fRec74[1] * std::cos((fConst48 * (fRec71[0] + 650.0f))))) + (fConst164 * fRec74[2])));
			fRec75[0] = (fTemp52 - ((fConst166 * (fRec75[1] * std::cos((fConst48 * (fRec71[0] + 850.0f))))) + (fConst167 * fRec75[2])));
			fRec76[0] = (fTemp52 - ((fConst169 * (fRec76[1] * std::cos((fConst48 * (fRec71[0] + 1050.0f))))) + (fConst170 * fRec76[2])));
			fRec77[0] = (fTemp52 - ((fConst172 * (fRec77[1] * std::cos((fConst48 * (fRec71[0] + 1250.0f))))) + (fConst173 * fRec77[2])));
			fRec78[0] = (fTemp52 - ((fConst175 * (fRec78[1] * std::cos((fConst48 * (fRec71[0] + 1450.0f))))) + (fConst176 * fRec78[2])));
			fRec79[0] = (fTemp52 - ((fConst178 * (fRec79[1] * std::cos((fConst48 * (fRec71[0] + 1650.0f))))) + (fConst179 * fRec79[2])));
			fRec80[0] = (fTemp52 - ((fConst181 * (fRec80[1] * std::cos((fConst48 * (fRec71[0] + 1850.0f))))) + (fConst182 * fRec80[2])));
			fRec81[0] = (fTemp52 - ((fConst184 * (fRec81[1] * std::cos((fConst48 * (fRec71[0] + 2050.0f))))) + (fConst185 * fRec81[2])));
			fRec82[0] = (fTemp52 - ((fConst187 * (fRec82[1] * std::cos((fConst48 * (fRec71[0] + 2250.0f))))) + (fConst188 * fRec82[2])));
			fRec83[0] = (fTemp52 - ((fConst190 * (fRec83[1] * std::cos((fConst48 * (fRec71[0] + 2450.0f))))) + (fConst191 * fRec83[2])));
			fRec84[0] = (fTemp52 - ((fConst193 * (fRec84[1] * std::cos((fConst48 * (fRec71[0] + 2650.0f))))) + (fConst194 * fRec84[2])));
			fRec85[0] = (fTemp52 - ((fConst196 * (fRec85[1] * std::cos((fConst48 * (fRec71[0] + 2850.0f))))) + (fConst197 * fRec85[2])));
			fRec86[0] = (fTemp52 - ((fConst199 * (fRec86[1] * std::cos((fConst48 * (fRec71[0] + 3050.0f))))) + (fConst200 * fRec86[2])));
			fRec87[0] = (fTemp52 - ((fConst202 * (fRec87[1] * std::cos((fConst48 * (fRec71[0] + 3250.0f))))) + (fConst203 * fRec87[2])));
			fRec88[0] = (fTemp52 - ((fConst205 * (fRec88[1] * std::cos((fConst48 * (fRec71[0] + 3450.0f))))) + (fConst206 * fRec88[2])));
			fRec89[0] = (fTemp52 - ((fConst208 * (fRec89[1] * std::cos((fConst48 * (fRec71[0] + 3650.0f))))) + (fConst209 * fRec89[2])));
			fRec90[0] = (fTemp52 - ((fConst211 * (fRec90[1] * std::cos((fConst48 * (fRec71[0] + 3850.0f))))) + (fConst212 * fRec90[2])));
			fRec91[0] = (iTemp53 ? fSlow131 : fRec91[1]);
			float fTemp54 = (((((((((((((((((((((fRec67[0] + (0.25f * (fRec72[0] - fRec72[2]))) + (0.111111112f * (fRec73[0] - fRec73[2]))) + (0.0625f * (fRec74[0] - fRec74[2]))) + (0.0399999991f * (fRec75[0] - fRec75[2]))) + (0.027777778f * (fRec76[0] - fRec76[2]))) + (0.0204081628f * (fRec77[0] - fRec77[2]))) + (0.015625f * (fRec78[0] - fRec78[2]))) + (0.0123456791f * (fRec79[0] - fRec79[2]))) + (0.00999999978f * (fRec80[0] - fRec80[2]))) + (0.00826446246f * (fRec81[0] - fRec81[2]))) + (0.0069444445f * (fRec82[0] - fRec82[2]))) + (0.00591715984f * (fRec83[0] - fRec83[2]))) + (0.00510204071f * (fRec84[0] - fRec84[2]))) + (0.00444444455f * (fRec85[0] - fRec85[2]))) + (0.00390625f * (fRec86[0] - fRec86[2]))) + (0.00346020772f * (fRec87[0] - fRec87[2]))) + (0.00308641978f * (fRec88[0] - fRec88[2]))) + (0.00277008303f * (fRec89[0] - fRec89[2]))) + (0.00249999994f * (fRec90[0] - fRec90[2]))) - fRec67[2]) * std::pow(10.0f, (0.144444451f * (fRec91[0] + -10.0f))));
			float fTemp55 = std::fabs((0.0125000002f * fTemp54));
			fRec94[0] = std::max<float>(fTemp55, ((fRec94[1] * fSlow210) + (fTemp55 * fSlow211)));
			fRec93[0] = ((fRec93[1] * fSlow207) + (fRec94[0] * fSlow212));
			fRec92[0] = ((fConst46 * fRec92[1]) + (fSlow204 * std::max<float>(((20.0f * std::log10(fRec93[0])) - fSlow213), 0.0f)));
			float fTemp56 = (0.0125000002f * (fTemp54 * std::pow(10.0f, (0.0500000007f * fRec92[0]))));
			fRec95[0] = (fTemp56 - (fSlow217 * ((fSlow218 * fRec95[2]) + (fSlow219 * fRec95[1]))));
			float fTemp57 = (fSlow185 * fRec66[1]);
			fRec66[0] = (fTemp56 - ((fSlow217 * (fRec95[2] + (fRec95[0] + (2.0f * fRec95[1])))) + (((fRec66[2] * fSlow227) + fTemp57) / fSlow228)));
			float fTemp58 = (fSlow182 * fRec65[1]);
			fRec65[0] = ((((fTemp57 + (fRec66[0] * fSlow230)) + (fRec66[2] * fSlow231)) / fSlow228) - (((fRec65[2] * fSlow239) + fTemp58) / fSlow240));
			fRec64[0] = ((((fTemp58 + (fRec65[0] * fSlow242)) + (fRec65[2] * fSlow243)) / fSlow240) - (fSlow244 * ((fSlow245 * fRec64[2]) + (fSlow246 * fRec64[1]))));
			float fTemp59 = ((fSlow179 * (fRec64[2] + (fRec64[0] + (2.0f * fRec64[1])))) + fTemp30);
			fVec24[0] = fSlow258;
			float fTemp60 = (fSlow258 - fVec24[1]);
			fVec25[0] = fSlow260;
			float fTemp61 = (0.0f - (fSlow258 + fVec25[1]));
			float fTemp62 = std::min<float>(1.0f, ((fTemp60 * float((fTemp60 > 0.0f))) + (fSlow259 * (fTemp61 * float((fTemp61 > 0.0f))))));
			fVec26[0] = fTemp62;
			iRec100[0] = (((iRec100[1] + (iRec100[1] > 0)) * (fTemp62 <= fVec26[1])) + (fTemp62 > fVec26[1]));
			float fTemp63 = float(iRec100[0]);
			float fTemp64 = (fConst44 * fTemp63);
			float fTemp65 = (fConst43 - fTemp63);
			fRec99[0] = ((fConst213 * fRec99[1]) + (fConst214 * mydsp_faustpower6_f(std::max<float>(0.0f, std::min<float>(fTemp64, ((fConst45 * fTemp65) + 1.0f))))));
			int iTemp66 = int(fTemp62);
			fRec101[0] = (iTemp66 ? fSlow131 : fRec101[1]);
			float fTemp67 = std::pow(10.0f, (0.144444451f * (fRec101[0] + -10.0f)));
			float fTemp68 = (fRec99[0] * fTemp67);
			fRec104[0] = (iTemp66 ? fSlow262 : fRec104[1]);
			iRec103[0] = ((iVec0[1] + iRec103[1]) % int((fConst0 / fRec104[0])));
			float fTemp69 = float(iRec103[0]);
			int iTemp70 = (fTemp62 > 0.0f);
			float fTemp71 = (0.144717798f * (iTemp70 ? 0.00100000005f : 1.60000002f));
			int iTemp72 = (std::fabs(fTemp71) < 1.1920929e-07f);
			float fTemp73 = (iTemp72 ? 0.0f : std::exp((0.0f - (fConst5 / (iTemp72 ? 1.0f : fTemp71)))));
			fRec106[0] = ((fRec106[1] * fTemp73) + ((iTemp70 ? fTemp62 : 0.400000006f) * (1.0f - fTemp73)));
			fRec105[0] = ((fConst218 * fRec105[1]) + (fSlow263 * fRec106[0]));
			float fTemp74 = (fRec105[0] + 0.00999999978f);
			float fTemp75 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fRec104[0] * fTemp74) * (1.0f - (0.000500000024f * std::min<float>(fRec104[0], 1000.0f)))))))));
			float fTemp76 = (1.0f / fTemp75);
			float fTemp77 = (((fTemp76 + 1.41421354f) / fTemp75) + 1.0f);
			fRec102[0] = ((2.0f * (((float((fTemp69 < (fConst215 / fRec104[0]))) + -0.5f) + (float((fTemp69 < (fConst216 / fRec104[0]))) + -0.5f)) + (float((fTemp69 < (fConst217 / fRec104[0]))) + -0.5f))) - (((fRec102[2] * (((fTemp76 + -1.41421354f) / fTemp75) + 1.0f)) + (2.0f * (fRec102[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp75)))))) / fTemp77));
			fRec109[0] = (iTemp66 ? fSlow264 : fRec109[1]);
			iRec108[0] = ((iVec0[1] + iRec108[1]) % int((fConst0 / fRec109[0])));
			float fTemp78 = float(iRec108[0]);
			float fTemp79 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fRec109[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec109[0], 1000.0f)))))))));
			float fTemp80 = (1.0f / fTemp79);
			float fTemp81 = (((fTemp80 + 1.41421354f) / fTemp79) + 1.0f);
			fRec107[0] = ((2.0f * (((float((fTemp78 < (fConst215 / fRec109[0]))) + -0.5f) + (float((fTemp78 < (fConst216 / fRec109[0]))) + -0.5f)) + (float((fTemp78 < (fConst217 / fRec109[0]))) + -0.5f))) - (((fRec107[2] * (((fTemp80 + -1.41421354f) / fTemp79) + 1.0f)) + (2.0f * (fRec107[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp79)))))) / fTemp81));
			fRec112[0] = (iTemp66 ? fSlow265 : fRec112[1]);
			iRec111[0] = ((iVec0[1] + iRec111[1]) % int((fConst0 / fRec112[0])));
			float fTemp82 = float(iRec111[0]);
			float fTemp83 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fRec112[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec112[0], 1000.0f)))))))));
			float fTemp84 = (1.0f / fTemp83);
			float fTemp85 = (((fTemp84 + 1.41421354f) / fTemp83) + 1.0f);
			fRec110[0] = ((2.0f * (((float((fTemp82 < (fConst215 / fRec112[0]))) + -0.5f) + (float((fTemp82 < (fConst216 / fRec112[0]))) + -0.5f)) + (float((fTemp82 < (fConst217 / fRec112[0]))) + -0.5f))) - (((fRec110[2] * (((fTemp84 + -1.41421354f) / fTemp83) + 1.0f)) + (2.0f * (fRec110[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp83)))))) / fTemp85));
			fRec115[0] = (iTemp66 ? fSlow266 : fRec115[1]);
			iRec114[0] = ((iVec0[1] + iRec114[1]) % int((fConst0 / fRec115[0])));
			float fTemp86 = float(iRec114[0]);
			float fTemp87 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fRec115[0]) * (1.0f - (0.000500000024f * std::min<float>(fRec115[0], 1000.0f)))))))));
			float fTemp88 = (1.0f / fTemp87);
			float fTemp89 = (((fTemp88 + 1.41421354f) / fTemp87) + 1.0f);
			fRec113[0] = ((2.0f * (((float((fTemp86 < (fConst215 / fRec115[0]))) + -0.5f) + (float((fTemp86 < (fConst216 / fRec115[0]))) + -0.5f)) + (float((fTemp86 < (fConst217 / fRec115[0]))) + -0.5f))) - (((fRec113[2] * (((fTemp88 + -1.41421354f) / fTemp87) + 1.0f)) + (2.0f * (fRec113[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp87)))))) / fTemp89));
			float fTemp90 = (fTemp68 * (((((fRec102[2] + (fRec102[0] + (2.0f * fRec102[1]))) / fTemp77) + ((fRec107[2] + (fRec107[0] + (2.0f * fRec107[1]))) / fTemp81)) + ((fRec110[2] + (fRec110[0] + (2.0f * fRec110[1]))) / fTemp85)) + ((fRec113[2] + (fRec113[0] + (2.0f * fRec113[1]))) / fTemp89)));
			float fTemp91 = std::fabs((0.25f * fTemp90));
			fRec118[0] = std::max<float>(fTemp91, ((fRec118[1] * fSlow273) + (fTemp91 * fSlow274)));
			fRec117[0] = ((fRec117[1] * fSlow270) + (fRec118[0] * fSlow275));
			fRec116[0] = ((fConst46 * fRec116[1]) + (fSlow267 * std::max<float>(((20.0f * std::log10(fRec117[0])) - fSlow276), 0.0f)));
			float fTemp92 = (0.25f * (fTemp90 * std::pow(10.0f, (0.0500000007f * fRec116[0]))));
			fRec119[0] = (fTemp92 - (fSlow280 * ((fSlow281 * fRec119[2]) + (fSlow282 * fRec119[1]))));
			float fTemp93 = (fSlow257 * fRec98[1]);
			fRec98[0] = (fTemp92 - ((fSlow280 * (fRec119[2] + (fRec119[0] + (2.0f * fRec119[1])))) + (((fRec98[2] * fSlow290) + fTemp93) / fSlow291)));
			float fTemp94 = (fSlow254 * fRec97[1]);
			fRec97[0] = ((((fTemp93 + (fRec98[0] * fSlow293)) + (fRec98[2] * fSlow294)) / fSlow291) - (((fRec97[2] * fSlow302) + fTemp94) / fSlow303));
			fRec96[0] = ((((fTemp94 + (fRec97[0] * fSlow305)) + (fRec97[2] * fSlow306)) / fSlow303) - (fSlow307 * ((fSlow308 * fRec96[2]) + (fSlow309 * fRec96[1]))));
			iRec123[0] = 0;
			fRec127[0] = ((0.949999988f * fRec127[1]) + (0.0500000007f * fRec126[1]));
			float fTemp95 = ((0.99000001f * fRec127[0]) + float(iRec123[1]));
			fVec27[(IOTA & 2047)] = fTemp95;
			fVec28[0] = fSlow322;
			float fTemp96 = (fSlow322 - fVec28[1]);
			fVec29[0] = fSlow324;
			float fTemp97 = (0.0f - (fSlow322 + fVec29[1]));
			float fTemp98 = std::min<float>(1.0f, ((fTemp96 * float((fTemp96 > 0.0f))) + (fSlow323 * (fTemp97 * float((fTemp97 > 0.0f))))));
			fVec30[0] = fTemp98;
			int iTemp99 = int(fTemp98);
			fRec128[0] = (iTemp99 ? fSlow322 : fRec128[1]);
			float fTemp100 = (fRec128[0] + 100.0f);
			float fTemp101 = (fConst221 * ((340.0f / fTemp100) + -0.0399999991f));
			float fTemp102 = (fTemp101 + -1.49999499f);
			int iTemp103 = int(fTemp102);
			int iTemp104 = int(std::min<float>(fConst220, float(std::max<int>(0, iTemp103))));
			float fTemp105 = std::floor(fTemp102);
			float fTemp106 = (fTemp101 + (-1.0f - fTemp105));
			float fTemp107 = (0.0f - fTemp106);
			float fTemp108 = (fTemp101 + (-2.0f - fTemp105));
			float fTemp109 = (0.0f - (0.5f * fTemp108));
			float fTemp110 = (fTemp101 + (-3.0f - fTemp105));
			float fTemp111 = (0.0f - (0.333333343f * fTemp110));
			float fTemp112 = (fTemp101 + (-4.0f - fTemp105));
			float fTemp113 = (0.0f - (0.25f * fTemp112));
			float fTemp114 = (fTemp101 - fTemp105);
			int iTemp115 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp103 + 1)))));
			float fTemp116 = (0.0f - fTemp108);
			float fTemp117 = (0.0f - (0.5f * fTemp110));
			float fTemp118 = (0.0f - (0.333333343f * fTemp112));
			int iTemp119 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp103 + 2)))));
			float fTemp120 = (0.0f - fTemp110);
			float fTemp121 = (0.0f - (0.5f * fTemp112));
			float fTemp122 = (fTemp106 * fTemp108);
			int iTemp123 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp103 + 3)))));
			float fTemp124 = (0.0f - fTemp112);
			float fTemp125 = (fTemp122 * fTemp110);
			int iTemp126 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp103 + 4)))));
			fRec125[0] = (((((fVec27[((IOTA - iTemp104) & 2047)] * fTemp107) * fTemp109) * fTemp111) * fTemp113) + (fTemp114 * ((((((fVec27[((IOTA - iTemp115) & 2047)] * fTemp116) * fTemp117) * fTemp118) + (0.5f * (((fTemp106 * fVec27[((IOTA - iTemp119) & 2047)]) * fTemp120) * fTemp121))) + (0.166666672f * ((fTemp122 * fVec27[((IOTA - iTemp123) & 2047)]) * fTemp124))) + (0.0416666679f * (fTemp125 * fVec27[((IOTA - iTemp126) & 2047)])))));
			fRec129[0] = ((0.949999988f * fRec129[1]) + (0.0500000007f * fRec125[1]));
			fRec132[0] = (fTemp47 - (fConst227 * ((fConst230 * fRec132[2]) + (fConst231 * fRec132[1]))));
			float fTemp127 = (fConst227 * (((fConst229 * fRec132[0]) + (fConst232 * fRec132[1])) + (fConst229 * fRec132[2])));
			fRec131[0] = (fTemp127 - (fConst224 * ((fConst233 * fRec131[2]) + (fConst234 * fRec131[1]))));
			iRec133[0] = (((iRec133[1] + (iRec133[1] > 0)) * (fTemp98 <= fVec30[1])) + (fTemp98 > fVec30[1]));
			float fTemp128 = (fSlow325 * float(iRec133[0]));
			float fTemp129 = (fConst224 * ((fRec131[2] + (fRec131[0] + (2.0f * fRec131[1]))) * std::max<float>(0.0f, std::min<float>(fTemp128, (2.0f - fTemp128)))));
			fRec130[0] = (fTemp129 - ((fConst236 * (fRec130[1] * std::cos((fConst48 * fTemp100)))) + (fConst237 * fRec130[2])));
			fRec134[0] = (fTemp129 - ((fConst241 * (fRec134[1] * std::cos((fConst242 * fTemp100)))) + (fConst243 * fRec134[2])));
			fRec135[0] = (fTemp129 - ((fConst246 * (fRec135[1] * std::cos((fConst247 * fTemp100)))) + (fConst248 * fRec135[2])));
			fRec136[0] = (fTemp129 - ((fConst251 * (fRec136[1] * std::cos((fConst252 * fTemp100)))) + (fConst253 * fRec136[2])));
			fRec137[0] = (fTemp129 - ((fConst256 * (fRec137[1] * std::cos((fConst257 * fTemp100)))) + (fConst258 * fRec137[2])));
			fRec138[0] = (fTemp129 - ((fConst261 * (fRec138[1] * std::cos((fConst262 * fTemp100)))) + (fConst263 * fRec138[2])));
			fRec139[0] = (fTemp129 - ((fConst266 * (fRec139[1] * std::cos((fConst267 * fTemp100)))) + (fConst268 * fRec139[2])));
			fRec140[0] = (fTemp129 - ((fConst271 * (fRec140[1] * std::cos((fConst272 * fTemp100)))) + (fConst273 * fRec140[2])));
			fRec141[0] = (fTemp129 - ((fConst276 * (fRec141[1] * std::cos((fConst277 * fTemp100)))) + (fConst278 * fRec141[2])));
			fRec142[0] = (fTemp129 - ((fConst281 * (fRec142[1] * std::cos((fConst282 * fTemp100)))) + (fConst283 * fRec142[2])));
			fRec143[0] = (fTemp129 - ((fConst286 * (fRec143[1] * std::cos((fConst287 * fTemp100)))) + (fConst288 * fRec143[2])));
			fRec144[0] = (fTemp129 - ((fConst291 * (fRec144[1] * std::cos((fConst292 * fTemp100)))) + (fConst293 * fRec144[2])));
			fRec145[0] = (fTemp129 - ((fConst296 * (fRec145[1] * std::cos((fConst297 * fTemp100)))) + (fConst298 * fRec145[2])));
			fRec146[0] = (fTemp129 - ((fConst301 * (fRec146[1] * std::cos((fConst302 * fTemp100)))) + (fConst303 * fRec146[2])));
			fRec147[0] = (fTemp129 - ((fConst306 * (fRec147[1] * std::cos((fConst307 * fTemp100)))) + (fConst308 * fRec147[2])));
			fRec148[0] = (fTemp129 - ((fConst311 * (fRec148[1] * std::cos((fConst312 * fTemp100)))) + (fConst313 * fRec148[2])));
			fRec149[0] = (fTemp129 - ((fConst316 * (fRec149[1] * std::cos((fConst317 * fTemp100)))) + (fConst318 * fRec149[2])));
			fRec150[0] = (fTemp129 - ((fConst321 * (fRec150[1] * std::cos((fConst322 * fTemp100)))) + (fConst323 * fRec150[2])));
			fRec151[0] = (fTemp129 - ((fConst326 * (fRec151[1] * std::cos((fConst327 * fTemp100)))) + (fConst328 * fRec151[2])));
			fRec152[0] = (fTemp129 - ((fConst331 * (fRec152[1] * std::cos((fConst332 * fTemp100)))) + (fConst333 * fRec152[2])));
			fRec153[0] = (fTemp129 - ((fConst336 * (fRec153[1] * std::cos((fConst337 * fTemp100)))) + (fConst338 * fRec153[2])));
			fRec154[0] = (fTemp129 - ((fConst341 * (fRec154[1] * std::cos((fConst342 * fTemp100)))) + (fConst343 * fRec154[2])));
			fRec155[0] = (fTemp129 - ((fConst346 * (fRec155[1] * std::cos((fConst347 * fTemp100)))) + (fConst348 * fRec155[2])));
			fRec156[0] = (fTemp129 - ((fConst351 * (fRec156[1] * std::cos((fConst352 * fTemp100)))) + (fConst353 * fRec156[2])));
			fRec157[0] = (fTemp129 - ((fConst356 * (fRec157[1] * std::cos((fConst357 * fTemp100)))) + (fConst358 * fRec157[2])));
			fRec158[0] = (fTemp129 - ((fConst361 * (fRec158[1] * std::cos((fConst362 * fTemp100)))) + (fConst363 * fRec158[2])));
			fRec159[0] = (fTemp129 - ((fConst366 * (fRec159[1] * std::cos((fConst367 * fTemp100)))) + (fConst368 * fRec159[2])));
			fRec160[0] = (fTemp129 - ((fConst371 * (fRec160[1] * std::cos((fConst372 * fTemp100)))) + (fConst373 * fRec160[2])));
			fRec161[0] = (fTemp129 - ((fConst376 * (fRec161[1] * std::cos((fConst377 * fTemp100)))) + (fConst378 * fRec161[2])));
			fRec162[0] = (fTemp129 - ((fConst381 * (fRec162[1] * std::cos((fConst382 * fTemp100)))) + (fConst383 * fRec162[2])));
			fRec163[0] = (fTemp129 - ((fConst386 * (fRec163[1] * std::cos((fConst387 * fTemp100)))) + (fConst388 * fRec163[2])));
			fRec164[0] = (fTemp129 - ((fConst391 * (fRec164[1] * std::cos((fConst392 * fTemp100)))) + (fConst393 * fRec164[2])));
			fRec165[0] = (fTemp129 - ((fConst396 * (fRec165[1] * std::cos((fConst397 * fTemp100)))) + (fConst398 * fRec165[2])));
			fRec166[0] = (fTemp129 - ((fConst401 * (fRec166[1] * std::cos((fConst402 * fTemp100)))) + (fConst403 * fRec166[2])));
			fRec167[0] = (fTemp129 - ((fConst406 * (fRec167[1] * std::cos((fConst407 * fTemp100)))) + (fConst408 * fRec167[2])));
			fRec168[0] = (fTemp129 - ((fConst411 * (fRec168[1] * std::cos((fConst412 * fTemp100)))) + (fConst413 * fRec168[2])));
			fRec169[0] = (fTemp129 - ((fConst416 * (fRec169[1] * std::cos((fConst417 * fTemp100)))) + (fConst418 * fRec169[2])));
			fRec170[0] = (fTemp129 - ((fConst421 * (fRec170[1] * std::cos((fConst422 * fTemp100)))) + (fConst423 * fRec170[2])));
			fRec171[0] = (fTemp129 - ((fConst426 * (fRec171[1] * std::cos((fConst427 * fTemp100)))) + (fConst428 * fRec171[2])));
			fRec172[0] = (fTemp129 - ((fConst431 * (fRec172[1] * std::cos((fConst432 * fTemp100)))) + (fConst433 * fRec172[2])));
			fRec173[0] = (fTemp129 - ((fConst436 * (fRec173[1] * std::cos((fConst437 * fTemp100)))) + (fConst438 * fRec173[2])));
			fRec174[0] = (fTemp129 - ((fConst441 * (fRec174[1] * std::cos((fConst442 * fTemp100)))) + (fConst443 * fRec174[2])));
			fRec175[0] = (fTemp129 - ((fConst446 * (fRec175[1] * std::cos((fConst447 * fTemp100)))) + (fConst448 * fRec175[2])));
			fRec176[0] = (fTemp129 - ((fConst451 * (fRec176[1] * std::cos((fConst452 * fTemp100)))) + (fConst453 * fRec176[2])));
			fRec177[0] = (fTemp129 - ((fConst456 * (fRec177[1] * std::cos((fConst457 * fTemp100)))) + (fConst458 * fRec177[2])));
			fRec178[0] = (fTemp129 - ((fConst461 * (fRec178[1] * std::cos((fConst462 * fTemp100)))) + (fConst463 * fRec178[2])));
			fRec179[0] = (fTemp129 - ((fConst466 * (fRec179[1] * std::cos((fConst467 * fTemp100)))) + (fConst468 * fRec179[2])));
			fRec180[0] = (fTemp129 - ((fConst471 * (fRec180[1] * std::cos((fConst472 * fTemp100)))) + (fConst473 * fRec180[2])));
			fRec181[0] = (fTemp129 - ((fConst476 * (fRec181[1] * std::cos((fConst477 * fTemp100)))) + (fConst478 * fRec181[2])));
			fRec182[0] = (fTemp129 - ((fConst481 * (fRec182[1] * std::cos((fConst482 * fTemp100)))) + (fConst483 * fRec182[2])));
			fVec31[0] = (((((((((((((((((((((((((((((((((((((((((((((((((((fRec130[0] - fRec130[2]) * ((fTemp100 < fConst238) ? fConst239 : 0.0f)) + ((fRec134[0] - fRec134[2]) * (((3.31356001f * fTemp100) < fConst238) ? fConst244 : 0.0f))) + ((fRec135[0] - fRec135[2]) * (((3.83469009f * fTemp100) < fConst238) ? fConst249 : 0.0f))) + ((fRec136[0] - fRec136[2]) * (((8.06313038f * fTemp100) < fConst238) ? fConst254 : 0.0f))) + ((fRec137[0] - fRec137[2]) * (((9.44777966f * fTemp100) < fConst238) ? fConst259 : 0.0f))) + ((fRec138[0] - fRec138[2]) * (((14.1169004f * fTemp100) < fConst238) ? fConst264 : 0.0f))) + ((fRec139[0] - fRec139[2]) * (((18.3840008f * fTemp100) < fConst238) ? fConst269 : 0.0f))) + ((fRec140[0] - fRec140[2]) * (((21.0102005f * fTemp100) < fConst238) ? fConst274 : 0.0f))) + ((fRec141[0] - fRec141[2]) * (((26.1774998f * fTemp100) < fConst238) ? fConst279 : 0.0f))) + ((fRec142[0] - fRec142[2]) * (((28.9944f * fTemp100) < fConst238) ? fConst284 : 0.0f))) + ((fRec143[0] - fRec143[2]) * (((37.0727997f * fTemp100) < fConst238) ? fConst289 : 0.0f))) + ((fRec144[0] - fRec144[2]) * (((37.8703003f * fTemp100) < fConst238) ? fConst294 : 0.0f))) + ((fRec145[0] - fRec145[2]) * (((40.0634003f * fTemp100) < fConst238) ? fConst299 : 0.0f))) + ((fRec146[0] - fRec146[2]) * (((47.6439018f * fTemp100) < fConst238) ? fConst304 : 0.0f))) + ((fRec147[0] - fRec147[2]) * (((51.019001f * fTemp100) < fConst238) ? fConst309 : 0.0f))) + ((fRec148[0] - fRec148[2]) * (((52.4300003f * fTemp100) < fConst238) ? fConst314 : 0.0f))) + ((fRec149[0] - fRec149[2]) * (((58.2859993f * fTemp100) < fConst238) ? fConst319 : 0.0f))) + ((fRec150[0] - fRec150[2]) * (((63.5485992f * fTemp100) < fConst238) ? fConst324 : 0.0f))) + ((fRec151[0] - fRec151[2]) * (((65.3628006f * fTemp100) < fConst238) ? fConst329 : 0.0f))) + ((fRec152[0] - fRec152[2]) * (((66.9587021f * fTemp100) < fConst238) ? fConst334 : 0.0f))) + ((fRec153[0] - fRec153[2]) * (((74.530098f * fTemp100) < fConst238) ? fConst339 : 0.0f))) + ((fRec154[0] - fRec154[2]) * (((78.6920013f * fTemp100) < fConst238) ? fConst344 : 0.0f))) + ((fRec155[0] - fRec155[2]) * (((80.8375015f * fTemp100) < fConst238) ? fConst349 : 0.0f))) + ((fRec156[0] - fRec156[2]) * (((89.9779968f * fTemp100) < fConst238) ? fConst354 : 0.0f))) + ((fRec157[0] - fRec157[2]) * (((92.9661026f * fTemp100) < fConst238) ? fConst359 : 0.0f))) + ((fRec158[0] - fRec158[2]) * (((95.1913986f * fTemp100) < fConst238) ? fConst364 : 0.0f))) + ((fRec159[0] - fRec159[2]) * (((97.4806976f * fTemp100) < fConst238) ? fConst369 : 0.0f))) + ((fRec160[0] - fRec160[2]) * (((110.620003f * fTemp100) < fConst238) ? fConst374 : 0.0f))) + ((fRec161[0] - fRec161[2]) * (((112.069f * fTemp100) < fConst238) ? fConst379 : 0.0f))) + ((fRec162[0] - fRec162[2]) * (((113.825996f * fTemp100) < fConst238) ? fConst384 : 0.0f))) + ((fRec163[0] - fRec163[2]) * (((119.356003f * fTemp100) < fConst238) ? fConst389 : 0.0f))) + ((fRec164[0] - fRec164[2]) * (((127.044998f * fTemp100) < fConst238) ? fConst394 : 0.0f))) + ((fRec165[0] - fRec165[2]) * (((129.981995f * fTemp100) < fConst238) ? fConst399 : 0.0f))) + ((fRec166[0] - fRec166[2]) * (((132.259003f * fTemp100) < fConst238) ? fConst404 : 0.0f))) + ((fRec167[0] - fRec167[2]) * (((133.477005f * fTemp100) < fConst238) ? fConst409 : 0.0f))) + ((fRec168[0] - fRec168[2]) * (((144.548996f * fTemp100) < fConst238) ? fConst414 : 0.0f))) + ((fRec169[0] - fRec169[2]) * (((149.438004f * fTemp100) < fConst238) ? fConst419 : 0.0f))) + ((fRec170[0] - fRec170[2]) * (((152.033005f * fTemp100) < fConst238) ? fConst424 : 0.0f))) + ((fRec171[0] - fRec171[2]) * (((153.166f * fTemp100) < fConst238) ? fConst429 : 0.0f))) + ((fRec172[0] - fRec172[2]) * (((155.597f * fTemp100) < fConst238) ? fConst434 : 0.0f))) + ((fRec173[0] - fRec173[2]) * (((158.182999f * fTemp100) < fConst238) ? fConst439 : 0.0f))) + ((fRec174[0] - fRec174[2]) * (((168.104996f * fTemp100) < fConst238) ? fConst444 : 0.0f))) + ((fRec175[0] - fRec175[2]) * (((171.863007f * fTemp100) < fConst238) ? fConst449 : 0.0f))) + ((fRec176[0] - fRec176[2]) * (((174.464005f * fTemp100) < fConst238) ? fConst454 : 0.0f))) + ((fRec177[0] - fRec177[2]) * (((178.936996f * fTemp100) < fConst238) ? fConst459 : 0.0f))) + ((fRec178[0] - fRec178[2]) * (((181.481995f * fTemp100) < fConst238) ? fConst464 : 0.0f))) + ((fRec179[0] - fRec179[2]) * (((185.397995f * fTemp100) < fConst238) ? fConst469 : 0.0f))) + ((fRec180[0] - fRec180[2]) * (((190.369003f * fTemp100) < fConst238) ? fConst474 : 0.0f))) + ((fRec181[0] - fRec181[2]) * (((192.190002f * fTemp100) < fConst238) ? fConst479 : 0.0f))) + ((fRec182[0] - fRec182[2]) * (((195.505005f * fTemp100) < fConst238) ? fConst484 : 0.0f)));
			float fTemp130 = ((0.99000001f * fRec129[0]) + (0.0199999996f * fVec31[1]));
			fVec32[(IOTA & 2047)] = fTemp130;
			fRec126[0] = (((((fTemp107 * fTemp109) * fTemp111) * fTemp113) * fVec32[((IOTA - iTemp104) & 2047)]) + (fTemp114 * ((((((fTemp116 * fTemp117) * fTemp118) * fVec32[((IOTA - iTemp115) & 2047)]) + (0.5f * (((fTemp106 * fTemp120) * fTemp121) * fVec32[((IOTA - iTemp119) & 2047)]))) + (0.166666672f * ((fTemp122 * fTemp124) * fVec32[((IOTA - iTemp123) & 2047)]))) + (0.0416666679f * (fTemp125 * fVec32[((IOTA - iTemp126) & 2047)])))));
			float fRec124 = fRec126[0];
			fRec183[0] = (iTemp99 ? fSlow131 : fRec183[1]);
			float fTemp131 = (fRec124 * std::pow(10.0f, (0.144444451f * (fRec183[0] + -10.0f))));
			float fTemp132 = std::fabs((0.25f * fTemp131));
			fRec186[0] = std::max<float>(fTemp132, ((fRec186[1] * fSlow332) + (fTemp132 * fSlow333)));
			fRec185[0] = ((fRec185[1] * fSlow329) + (fRec186[0] * fSlow334));
			fRec184[0] = ((fConst46 * fRec184[1]) + (fSlow326 * std::max<float>(((20.0f * std::log10(fRec185[0])) - fSlow335), 0.0f)));
			float fTemp133 = (0.25f * (fTemp131 * std::pow(10.0f, (0.0500000007f * fRec184[0]))));
			fRec187[0] = (fTemp133 - (fSlow339 * ((fSlow340 * fRec187[2]) + (fSlow341 * fRec187[1]))));
			float fTemp134 = (fSlow321 * fRec122[1]);
			fRec122[0] = (fTemp133 - ((fSlow339 * (fRec187[2] + (fRec187[0] + (2.0f * fRec187[1])))) + (((fRec122[2] * fSlow349) + fTemp134) / fSlow350)));
			float fTemp135 = (fSlow318 * fRec121[1]);
			fRec121[0] = ((((fTemp134 + (fRec122[0] * fSlow352)) + (fRec122[2] * fSlow353)) / fSlow350) - (((fRec121[2] * fSlow361) + fTemp135) / fSlow362));
			fRec120[0] = ((((fTemp135 + (fRec121[0] * fSlow364)) + (fRec121[2] * fSlow365)) / fSlow362) - (fSlow366 * ((fSlow367 * fRec120[2]) + (fSlow368 * fRec120[1]))));
			float fTemp136 = (fRec120[2] + (fRec120[0] + (2.0f * fRec120[1])));
			fRec191[0] = ((0.999000013f * fRec191[1]) + (0.00100000005f * std::sqrt(std::max<float>(0.0f, std::min<float>(fTemp64, ((fConst485 * fTemp65) + 1.0f))))));
			fRec193[0] = (iTemp66 ? fSlow380 : fRec193[1]);
			float fTemp137 = (fRec197[1] + (fSlow381 * fRec193[0]));
			fRec197[0] = (fTemp137 - std::floor(fTemp137));
			float fTemp138 = (fRec196[1] + (fConst5 * ((315.0f * (fRec191[0] * ftbl0mydspSIG0[int((65536.0f * fRec197[0]))])) + (fSlow382 * fRec193[0]))));
			fRec196[0] = (fTemp138 - std::floor(fTemp138));
			float fTemp139 = (fRec195[1] + (fConst5 * ((315.0f * (fRec191[0] * ftbl0mydspSIG0[int((65536.0f * fRec196[0]))])) + (fSlow383 * fRec193[0]))));
			fRec195[0] = (fTemp139 - std::floor(fTemp139));
			float fTemp140 = (fRec194[1] + (fConst5 * ((315.0f * (fRec191[0] * ftbl0mydspSIG0[int((65536.0f * fRec195[0]))])) + (fSlow384 * fRec193[0]))));
			fRec194[0] = (fTemp140 - std::floor(fTemp140));
			float fTemp141 = (fRec192[1] + (fConst5 * ((fSlow15 * fRec193[0]) + (315.0f * (fRec191[0] * ftbl0mydspSIG0[int((65536.0f * fRec194[0]))])))));
			fRec192[0] = (fTemp141 - std::floor(fTemp141));
			float fTemp142 = ftbl0mydspSIG0[int((65536.0f * fRec192[0]))];
			float fTemp143 = std::max<float>(-1.0f, std::min<float>(1.0f, (25.1188641f * fTemp142)));
			float fTemp144 = ((fRec191[0] * fTemp67) * (fTemp142 + (0.0500000007f * (fTemp143 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp143)))))));
			float fTemp145 = std::fabs((0.25f * fTemp144));
			fRec200[0] = std::max<float>(fTemp145, ((fRec200[1] * fSlow391) + (fTemp145 * fSlow392)));
			fRec199[0] = ((fRec199[1] * fSlow388) + (fRec200[0] * fSlow393));
			fRec198[0] = ((fConst46 * fRec198[1]) + (fSlow385 * std::max<float>(((20.0f * std::log10(fRec199[0])) - fSlow394), 0.0f)));
			float fTemp146 = (0.25f * (fTemp144 * std::pow(10.0f, (0.0500000007f * fRec198[0]))));
			fRec201[0] = (fTemp146 - (fSlow398 * ((fSlow399 * fRec201[2]) + (fSlow400 * fRec201[1]))));
			float fTemp147 = (fSlow379 * fRec190[1]);
			fRec190[0] = (fTemp146 - ((fSlow398 * (fRec201[2] + (fRec201[0] + (2.0f * fRec201[1])))) + (((fRec190[2] * fSlow408) + fTemp147) / fSlow409)));
			float fTemp148 = (fSlow376 * fRec189[1]);
			fRec189[0] = ((((fTemp147 + (fRec190[0] * fSlow411)) + (fRec190[2] * fSlow412)) / fSlow409) - (((fRec189[2] * fSlow420) + fTemp148) / fSlow421));
			fRec188[0] = ((((fTemp148 + (fRec189[0] * fSlow423)) + (fRec189[2] * fSlow424)) / fSlow421) - (fSlow425 * ((fSlow426 * fRec188[2]) + (fSlow427 * fRec188[1]))));
			float fTemp149 = (fSlow373 * (fRec188[2] + (fRec188[0] + (2.0f * fRec188[1]))));
			float fRec211 = (0.0f - (0.997305274f * ((0.899999976f * fRec212[2]) + (0.0500000007f * (fRec212[1] + fRec212[3])))));
			fVec33[0] = fSlow440;
			float fTemp150 = (fSlow440 - fVec33[1]);
			fVec34[0] = fSlow442;
			float fTemp151 = (0.0f - (fSlow440 + fVec34[1]));
			float fTemp152 = std::min<float>(1.0f, ((fTemp150 * float((fTemp150 > 0.0f))) + (fSlow441 * (fTemp151 * float((fTemp151 > 0.0f))))));
			fVec35[0] = fTemp152;
			int iTemp153 = int(fTemp152);
			fRec230[0] = (iTemp153 ? fSlow443 : fRec230[1]);
			float fTemp154 = ((340.0f / std::min<float>(5000.0f, std::max<float>(20.0f, (fSlow439 * (fRec20[0] * fRec230[0]))))) + -0.109999999f);
			float fTemp155 = (fConst487 * fTemp154);
			float fTemp156 = (fTemp155 + -1.49999499f);
			int iTemp157 = int(fTemp156);
			int iTemp158 = int(std::min<float>(fConst220, float(std::max<int>(0, iTemp157))));
			float fTemp159 = std::floor(fTemp156);
			float fTemp160 = (fTemp155 + (-1.0f - fTemp159));
			float fTemp161 = (0.0f - fTemp160);
			float fTemp162 = (fTemp155 + (-2.0f - fTemp159));
			float fTemp163 = (0.0f - (0.5f * fTemp162));
			float fTemp164 = (fTemp155 + (-3.0f - fTemp159));
			float fTemp165 = (0.0f - (0.333333343f * fTemp164));
			float fTemp166 = (fTemp155 + (-4.0f - fTemp159));
			float fTemp167 = (0.0f - (0.25f * fTemp166));
			float fTemp168 = (fTemp155 - fTemp159);
			int iTemp169 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp157 + 1)))));
			float fTemp170 = (0.0f - fTemp162);
			float fTemp171 = (0.0f - (0.5f * fTemp164));
			float fTemp172 = (0.0f - (0.333333343f * fTemp166));
			int iTemp173 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp157 + 2)))));
			float fTemp174 = (0.0f - fTemp164);
			float fTemp175 = (0.0f - (0.5f * fTemp166));
			float fTemp176 = (fTemp160 * fTemp162);
			int iTemp177 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp157 + 3)))));
			float fTemp178 = (0.0f - fTemp166);
			float fTemp179 = (fTemp176 * fTemp164);
			int iTemp180 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp157 + 4)))));
			fRec226[0] = (((((fRec207[((IOTA - (iTemp158 + 1)) & 2047)] * fTemp161) * fTemp163) * fTemp165) * fTemp167) + (fTemp168 * ((((((fRec207[((IOTA - (iTemp169 + 1)) & 2047)] * fTemp170) * fTemp171) * fTemp172) + (0.5f * (((fTemp160 * fRec207[((IOTA - (iTemp173 + 1)) & 2047)]) * fTemp174) * fTemp175))) + (0.166666672f * ((fTemp176 * fRec207[((IOTA - (iTemp177 + 1)) & 2047)]) * fTemp178))) + (0.0416666679f * (fTemp179 * fRec207[((IOTA - (iTemp180 + 1)) & 2047)])))));
			fRec231[0] = ((fSlow444 * fRec231[1]) + (fSlow445 * fRec226[1]));
			float fRec227 = fRec231[0];
			fRec233[0] = fRec205[1];
			fRec234[(IOTA & 1023)] = (0.0f - (0.997305274f * ((0.899999976f * fRec233[2]) + (0.0500000007f * (fRec233[1] + fRec233[3])))));
			float fTemp181 = (fConst488 * fTemp154);
			float fTemp182 = (fTemp181 + -1.49999499f);
			int iTemp183 = int(fTemp182);
			int iTemp184 = int(std::min<float>(fConst220, float(std::max<int>(0, iTemp183))));
			float fTemp185 = std::floor(fTemp182);
			float fTemp186 = (fTemp181 + (-1.0f - fTemp185));
			float fTemp187 = (0.0f - fTemp186);
			float fTemp188 = (fTemp181 + (-2.0f - fTemp185));
			float fTemp189 = (0.0f - (0.5f * fTemp188));
			float fTemp190 = (fTemp181 + (-3.0f - fTemp185));
			float fTemp191 = (0.0f - (0.333333343f * fTemp190));
			float fTemp192 = (fTemp181 + (-4.0f - fTemp185));
			float fTemp193 = (0.0f - (0.25f * fTemp192));
			float fTemp194 = (fTemp181 - fTemp185);
			int iTemp195 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp183 + 1)))));
			float fTemp196 = (0.0f - fTemp188);
			float fTemp197 = (0.0f - (0.5f * fTemp190));
			float fTemp198 = (0.0f - (0.333333343f * fTemp192));
			int iTemp199 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp183 + 2)))));
			float fTemp200 = (0.0f - fTemp190);
			float fTemp201 = (0.0f - (0.5f * fTemp192));
			float fTemp202 = (fTemp186 * fTemp188);
			int iTemp203 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp183 + 3)))));
			float fTemp204 = (0.0f - fTemp192);
			float fTemp205 = (fTemp202 * fTemp190);
			int iTemp206 = int(std::min<float>(fConst220, float(std::max<int>(0, (iTemp183 + 4)))));
			fVec36[0] = (((((fRec234[((IOTA - (iTemp184 + 2)) & 1023)] * fTemp187) * fTemp189) * fTemp191) * fTemp193) + (fTemp194 * ((((((fRec234[((IOTA - (iTemp195 + 2)) & 1023)] * fTemp196) * fTemp197) * fTemp198) + (0.5f * (((fTemp186 * fRec234[((IOTA - (iTemp199 + 2)) & 1023)]) * fTemp200) * fTemp201))) + (0.166666672f * ((fTemp202 * fRec234[((IOTA - (iTemp203 + 2)) & 1023)]) * fTemp204))) + (0.0416666679f * (fTemp205 * fRec234[((IOTA - (iTemp206 + 2)) & 1023)])))));
			float fTemp207 = (fTemp152 + fVec36[1]);
			fVec37[0] = fTemp207;
			fRec232[(IOTA & 2047)] = ((fSlow444 * fRec232[((IOTA - 1) & 2047)]) + (fSlow445 * fVec37[2]));
			float fRec228 = (((((fTemp161 * fTemp163) * fTemp165) * fTemp167) * fRec232[((IOTA - iTemp158) & 2047)]) + (fTemp168 * ((((((fTemp170 * fTemp171) * fTemp172) * fRec232[((IOTA - iTemp169) & 2047)]) + (0.5f * (((fTemp160 * fTemp174) * fTemp175) * fRec232[((IOTA - iTemp173) & 2047)]))) + (0.166666672f * ((fTemp176 * fTemp178) * fRec232[((IOTA - iTemp177) & 2047)]))) + (0.0416666679f * (fTemp179 * fRec232[((IOTA - iTemp180) & 2047)])))));
			float fRec229 = (fVec37[1] + fRec222[1]);
			fRec222[0] = fRec227;
			float fRec223 = fRec222[1];
			float fRec224 = fRec228;
			float fRec225 = fRec229;
			fRec218[0] = fRec223;
			float fRec219 = (fTemp152 + fRec218[1]);
			float fRec220 = fRec224;
			float fRec221 = fRec225;
			fRec214[(IOTA & 1023)] = fRec219;
			float fRec215 = (((((fTemp187 * fTemp189) * fTemp191) * fTemp193) * fRec214[((IOTA - (iTemp184 + 1)) & 1023)]) + (fTemp194 * ((((((fTemp196 * fTemp197) * fTemp198) * fRec214[((IOTA - (iTemp195 + 1)) & 1023)]) + (0.5f * (((fTemp186 * fTemp200) * fTemp201) * fRec214[((IOTA - (iTemp199 + 1)) & 1023)]))) + (0.166666672f * ((fTemp202 * fTemp204) * fRec214[((IOTA - (iTemp203 + 1)) & 1023)]))) + (0.0416666679f * (fTemp205 * fRec214[((IOTA - (iTemp206 + 1)) & 1023)])))));
			float fRec216 = fRec220;
			float fRec217 = fRec221;
			fRec212[0] = fRec216;
			float fRec213 = fRec217;
			fRec207[(IOTA & 2047)] = fRec211;
			float fRec208 = fRec215;
			float fRec209 = fRec212[0];
			float fRec210 = fRec213;
			fRec205[0] = fRec208;
			float fRec206 = fRec210;
			iRec235[0] = (((iRec235[1] + (iRec235[1] > 0)) * (fTemp152 <= fVec35[1])) + (fTemp152 > fVec35[1]));
			float fTemp208 = float(iRec235[0]);
			fRec236[0] = (iTemp153 ? fSlow131 : fRec236[1]);
			float fTemp209 = ((fRec206 * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp208), ((fConst45 * (fConst43 - fTemp208)) + 1.0f)))) * std::pow(10.0f, (0.144444451f * (fRec236[0] + -10.0f))));
			float fTemp210 = std::fabs((0.375f * fTemp209));
			fRec239[0] = std::max<float>(fTemp210, ((fRec239[1] * fSlow452) + (fTemp210 * fSlow453)));
			fRec238[0] = ((fRec238[1] * fSlow449) + (fRec239[0] * fSlow454));
			fRec237[0] = ((fConst46 * fRec237[1]) + (fSlow446 * std::max<float>(((20.0f * std::log10(fRec238[0])) - fSlow455), 0.0f)));
			float fTemp211 = (0.375f * (fTemp209 * std::pow(10.0f, (0.0500000007f * fRec237[0]))));
			fRec240[0] = (fTemp211 - (fSlow459 * ((fSlow460 * fRec240[2]) + (fSlow461 * fRec240[1]))));
			float fTemp212 = (fSlow438 * fRec204[1]);
			fRec204[0] = (fTemp211 - ((fSlow459 * (fRec240[2] + (fRec240[0] + (2.0f * fRec240[1])))) + (((fRec204[2] * fSlow469) + fTemp212) / fSlow470)));
			float fTemp213 = (fSlow435 * fRec203[1]);
			fRec203[0] = ((((fTemp212 + (fRec204[0] * fSlow472)) + (fRec204[2] * fSlow473)) / fSlow470) - (((fRec203[2] * fSlow481) + fTemp213) / fSlow482));
			fRec202[0] = ((((fTemp213 + (fRec203[0] * fSlow484)) + (fRec203[2] * fSlow485)) / fSlow482) - (fSlow486 * ((fSlow487 * fRec202[2]) + (fSlow488 * fRec202[1]))));
			float fTemp214 = (fSlow432 * (fRec202[2] + (fRec202[0] + (2.0f * fRec202[1]))));
			fRec245[0] = (fTemp127 - (fConst492 * ((fConst493 * fRec245[2]) + (fConst494 * fRec245[1]))));
			fVec38[0] = fSlow500;
			float fTemp215 = (fSlow500 - fVec38[1]);
			fVec39[0] = fSlow502;
			float fTemp216 = (0.0f - (fSlow500 + fVec39[1]));
			float fTemp217 = std::min<float>(1.0f, ((fTemp215 * float((fTemp215 > 0.0f))) + (fSlow501 * (fTemp216 * float((fTemp216 > 0.0f))))));
			fVec40[0] = fTemp217;
			iRec246[0] = (((iRec246[1] + (iRec246[1] > 0)) * (fTemp217 <= fVec40[1])) + (fTemp217 > fVec40[1]));
			float fTemp218 = float(iRec246[0]);
			float fTemp219 = (fConst495 * fTemp218);
			float fTemp220 = (fConst492 * ((fRec245[2] + (fRec245[0] + (2.0f * fRec245[1]))) * std::max<float>(0.0f, std::min<float>(fTemp219, (2.0f - fTemp219)))));
			fRec244[0] = (fTemp220 - ((fConst497 * fRec244[1]) + (fConst498 * fRec244[2])));
			fRec247[0] = (fTemp220 - ((fConst501 * fRec247[1]) + (fConst502 * fRec247[2])));
			fRec248[0] = (fTemp220 - ((fConst505 * fRec248[1]) + (fConst506 * fRec248[2])));
			fRec249[0] = (fTemp220 - ((fConst509 * fRec249[1]) + (fConst510 * fRec249[2])));
			fRec250[0] = (fTemp220 - ((fConst513 * fRec250[1]) + (fConst514 * fRec250[2])));
			fRec251[0] = (fTemp220 - ((fConst517 * fRec251[1]) + (fConst518 * fRec251[2])));
			fRec252[0] = (fTemp220 - ((fConst521 * fRec252[1]) + (fConst522 * fRec252[2])));
			fRec253[0] = (fTemp220 - ((fConst525 * fRec253[1]) + (fConst526 * fRec253[2])));
			fRec254[0] = (fTemp220 - ((fConst529 * fRec254[1]) + (fConst530 * fRec254[2])));
			fRec255[0] = (fTemp220 - ((fConst533 * fRec255[1]) + (fConst534 * fRec255[2])));
			fRec256[0] = (fTemp220 - ((fConst537 * fRec256[1]) + (fConst538 * fRec256[2])));
			fRec257[0] = (fTemp220 - ((fConst541 * fRec257[1]) + (fConst542 * fRec257[2])));
			fRec258[0] = (fTemp220 - ((fConst545 * fRec258[1]) + (fConst546 * fRec258[2])));
			fRec259[0] = (fTemp220 - ((fConst549 * fRec259[1]) + (fConst550 * fRec259[2])));
			fRec260[0] = (fTemp220 - ((fConst553 * fRec260[1]) + (fConst554 * fRec260[2])));
			fRec261[0] = (fTemp220 - ((fConst557 * fRec261[1]) + (fConst558 * fRec261[2])));
			fRec262[0] = (fTemp220 - ((fConst561 * fRec262[1]) + (fConst562 * fRec262[2])));
			fRec263[0] = (fTemp220 - ((fConst565 * fRec263[1]) + (fConst566 * fRec263[2])));
			fRec264[0] = (fTemp220 - ((fConst569 * fRec264[1]) + (fConst570 * fRec264[2])));
			fRec265[0] = (fTemp220 - ((fConst573 * fRec265[1]) + (fConst574 * fRec265[2])));
			fRec266[0] = (fTemp220 - ((fConst577 * fRec266[1]) + (fConst578 * fRec266[2])));
			fRec267[0] = (fTemp220 - ((fConst581 * fRec267[1]) + (fConst582 * fRec267[2])));
			fRec268[0] = (fTemp220 - ((fConst585 * fRec268[1]) + (fConst586 * fRec268[2])));
			fRec269[0] = (fTemp220 - ((fConst589 * fRec269[1]) + (fConst590 * fRec269[2])));
			fRec270[0] = (fTemp220 - ((fConst593 * fRec270[1]) + (fConst594 * fRec270[2])));
			fRec271[0] = (fTemp220 - ((fConst597 * fRec271[1]) + (fConst598 * fRec271[2])));
			fRec272[0] = (fTemp220 - ((fConst601 * fRec272[1]) + (fConst602 * fRec272[2])));
			fRec273[0] = (fTemp220 - ((fConst605 * fRec273[1]) + (fConst606 * fRec273[2])));
			fRec274[0] = (fTemp220 - ((fConst609 * fRec274[1]) + (fConst610 * fRec274[2])));
			fRec275[0] = (fTemp220 - ((fConst613 * fRec275[1]) + (fConst614 * fRec275[2])));
			fRec276[0] = (fTemp220 - ((fConst617 * fRec276[1]) + (fConst618 * fRec276[2])));
			fRec277[0] = (fTemp220 - ((fConst621 * fRec277[1]) + (fConst622 * fRec277[2])));
			fRec278[0] = (fTemp220 - ((fConst625 * fRec278[1]) + (fConst626 * fRec278[2])));
			fRec279[0] = (fTemp220 - ((fConst629 * fRec279[1]) + (fConst630 * fRec279[2])));
			fRec280[0] = (fTemp220 - ((fConst633 * fRec280[1]) + (fConst634 * fRec280[2])));
			fRec281[0] = (fTemp220 - ((fConst637 * fRec281[1]) + (fConst638 * fRec281[2])));
			fRec282[0] = (fTemp220 - ((fConst641 * fRec282[1]) + (fConst642 * fRec282[2])));
			fRec283[0] = (fTemp220 - ((fConst645 * fRec283[1]) + (fConst646 * fRec283[2])));
			fRec284[0] = (fTemp220 - ((fConst649 * fRec284[1]) + (fConst650 * fRec284[2])));
			fRec285[0] = (fTemp220 - ((fConst653 * fRec285[1]) + (fConst654 * fRec285[2])));
			fRec286[0] = (fTemp220 - ((fConst657 * fRec286[1]) + (fConst658 * fRec286[2])));
			fRec287[0] = (fTemp220 - ((fConst661 * fRec287[1]) + (fConst662 * fRec287[2])));
			fRec288[0] = (fTemp220 - ((fConst665 * fRec288[1]) + (fConst666 * fRec288[2])));
			fRec289[0] = (fTemp220 - ((fConst669 * fRec289[1]) + (fConst670 * fRec289[2])));
			fRec290[0] = (fTemp220 - ((fConst673 * fRec290[1]) + (fConst674 * fRec290[2])));
			fRec291[0] = (fTemp220 - ((fConst677 * fRec291[1]) + (fConst678 * fRec291[2])));
			fRec292[0] = (fTemp220 - ((fConst681 * fRec292[1]) + (fConst682 * fRec292[2])));
			fRec293[0] = (fTemp220 - ((fConst685 * fRec293[1]) + (fConst686 * fRec293[2])));
			fRec294[0] = (fTemp220 - ((fConst689 * fRec294[1]) + (fConst690 * fRec294[2])));
			fRec295[0] = (fTemp220 - ((fConst693 * fRec295[1]) + (fConst694 * fRec295[2])));
			float fTemp221 = (((((((((((((((((((((((((((((((((((((((((((((((((((fConst489 * (fRec244[0] - fRec244[2])) + (fConst499 * (fRec247[0] - fRec247[2]))) + (fConst503 * (fRec248[0] - fRec248[2]))) + (fConst507 * (fRec249[0] - fRec249[2]))) + (fConst511 * (fRec250[0] - fRec250[2]))) + (fConst515 * (fRec251[0] - fRec251[2]))) + (fConst519 * (fRec252[0] - fRec252[2]))) + (fConst523 * (fRec253[0] - fRec253[2]))) + (fConst527 * (fRec254[0] - fRec254[2]))) + (fConst531 * (fRec255[0] - fRec255[2]))) + (fConst535 * (fRec256[0] - fRec256[2]))) + (fConst539 * (fRec257[0] - fRec257[2]))) + (fConst543 * (fRec258[0] - fRec258[2]))) + (fConst547 * (fRec259[0] - fRec259[2]))) + (fConst551 * (fRec260[0] - fRec260[2]))) + (fConst555 * (fRec261[0] - fRec261[2]))) + (fConst559 * (fRec262[0] - fRec262[2]))) + (fConst563 * (fRec263[0] - fRec263[2]))) + (fConst567 * (fRec264[0] - fRec264[2]))) + (fConst571 * (fRec265[0] - fRec265[2]))) + (fConst575 * (fRec266[0] - fRec266[2]))) + (fConst579 * (fRec267[0] - fRec267[2]))) + (fConst583 * (fRec268[0] - fRec268[2]))) + (fConst587 * (fRec269[0] - fRec269[2]))) + (fConst591 * (fRec270[0] - fRec270[2]))) + (fConst595 * (fRec271[0] - fRec271[2]))) + (fConst599 * (fRec272[0] - fRec272[2]))) + (fConst603 * (fRec273[0] - fRec273[2]))) + (fConst607 * (fRec274[0] - fRec274[2]))) + (fConst611 * (fRec275[0] - fRec275[2]))) + (fConst615 * (fRec276[0] - fRec276[2]))) + (fConst619 * (fRec277[0] - fRec277[2]))) + (fConst623 * (fRec278[0] - fRec278[2]))) + (fConst627 * (fRec279[0] - fRec279[2]))) + (fConst631 * (fRec280[0] - fRec280[2]))) + (fConst635 * (fRec281[0] - fRec281[2]))) + (fConst639 * (fRec282[0] - fRec282[2]))) + (fConst643 * (fRec283[0] - fRec283[2]))) + (fConst647 * (fRec284[0] - fRec284[2]))) + (fConst651 * (fRec285[0] - fRec285[2]))) + (fConst655 * (fRec286[0] - fRec286[2]))) + (fConst659 * (fRec287[0] - fRec287[2]))) + (fConst663 * (fRec288[0] - fRec288[2]))) + (fConst667 * (fRec289[0] - fRec289[2]))) + (fConst671 * (fRec290[0] - fRec290[2]))) + (fConst675 * (fRec291[0] - fRec291[2]))) + (fConst679 * (fRec292[0] - fRec292[2]))) + (fConst683 * (fRec293[0] - fRec293[2]))) + (fConst687 * (fRec294[0] - fRec294[2]))) + (fConst691 * (fRec295[0] - fRec295[2]))) * std::max<float>(0.0f, std::min<float>((fConst44 * fTemp218), ((fConst695 * (fConst43 - fTemp218)) + 1.0f))));
			float fTemp222 = std::fabs((0.00499999989f * fTemp221));
			fRec298[0] = std::max<float>(fTemp222, ((fRec298[1] * fSlow509) + (fTemp222 * fSlow510)));
			fRec297[0] = ((fRec297[1] * fSlow506) + (fRec298[0] * fSlow511));
			fRec296[0] = ((fConst46 * fRec296[1]) + (fSlow503 * std::max<float>(((20.0f * std::log10(fRec297[0])) - fSlow512), 0.0f)));
			float fTemp223 = (0.00499999989f * (fTemp221 * std::pow(10.0f, (0.0500000007f * fRec296[0]))));
			fRec299[0] = (fTemp223 - (fSlow516 * ((fSlow517 * fRec299[2]) + (fSlow518 * fRec299[1]))));
			float fTemp224 = (fSlow499 * fRec243[1]);
			fRec243[0] = (fTemp223 - ((fSlow516 * (fRec299[2] + (fRec299[0] + (2.0f * fRec299[1])))) + (((fRec243[2] * fSlow526) + fTemp224) / fSlow527)));
			float fTemp225 = (fSlow496 * fRec242[1]);
			fRec242[0] = ((((fTemp224 + (fRec243[0] * fSlow529)) + (fRec243[2] * fSlow530)) / fSlow527) - (((fRec242[2] * fSlow538) + fTemp225) / fSlow539));
			fRec241[0] = ((((fTemp225 + (fRec242[0] * fSlow541)) + (fRec242[2] * fSlow542)) / fSlow539) - (fSlow543 * ((fSlow544 * fRec241[2]) + (fSlow545 * fRec241[1]))));
			float fTemp226 = (fSlow493 * (fRec241[2] + (fRec241[0] + (2.0f * fRec241[1]))));
			float fTemp227 = ((fConst10 * fRec0[2]) + ((((((fTemp59 + (fSlow251 * (fRec96[2] + (fRec96[0] + (2.0f * fRec96[1]))))) + (fSlow315 * fTemp136)) + fTemp149) + fTemp214) + fTemp226) + (fConst9 * fRec0[0])));
			float fTemp228 = std::fabs(fTemp227);
			fRec302[0] = std::max<float>(fTemp228, ((fConst698 * fRec302[1]) + (fConst699 * fTemp228)));
			fRec301[0] = ((fConst696 * fRec301[1]) + (fConst697 * fRec302[0]));
			fRec300[0] = ((fConst46 * fRec300[1]) + (fConst47 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec301[0]) + 1.0f)), 0.0f)))));
			float fTemp229 = (fTemp227 * std::pow(10.0f, (0.0500000007f * fRec300[0])));
			float fTemp230 = std::fabs(fTemp229);
			fRec305[0] = std::max<float>(fTemp230, ((fConst702 * fRec305[1]) + (fConst703 * fTemp230)));
			fRec304[0] = ((fConst700 * fRec304[1]) + (fConst701 * fRec305[0]));
			fRec303[0] = ((fConst46 * fRec303[1]) + (fConst47 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec304[0])) + 5.0f), 0.0f)))));
			float fTemp231 = (fTemp229 * std::pow(10.0f, (0.0500000007f * fRec303[0])));
			float fTemp232 = std::fabs((fSlow2 * fTemp231));
			fRec308[0] = std::max<float>(fTemp232, ((fConst46 * fRec308[1]) + (fConst47 * fTemp232)));
			fRec307[0] = ((fConst704 * fRec307[1]) + (fConst705 * fRec308[0]));
			fRec306[0] = ((fConst46 * fRec306[1]) + (fConst47 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec307[0])), 0.0f)))));
			output0[i] = FAUSTFLOAT((fSlow1 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp231 * std::pow(10.0f, (0.0500000007f * fRec306[0]))))))));
			fRec309[0] = ((0.370000005f * (fRec2[0] - fRec3[0])) - (fConst149 * ((fConst150 * fRec309[1]) + (fConst151 * fRec309[2]))));
			float fTemp233 = (fRec104[0] + 0.5f);
			iRec314[0] = ((iVec0[1] + iRec314[1]) % int((fConst0 / fTemp233)));
			float fTemp234 = float(iRec314[0]);
			float fTemp235 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fTemp233) * (1.0f - (0.000500000024f * std::min<float>(fTemp233, 1000.0f)))))))));
			float fTemp236 = (1.0f / fTemp235);
			float fTemp237 = (((fTemp236 + 1.41421354f) / fTemp235) + 1.0f);
			fRec313[0] = ((2.0f * (((float((fTemp234 < (fConst215 / fTemp233))) + -0.5f) + (float((fTemp234 < (fConst216 / fTemp233))) + -0.5f)) + (float((fTemp234 < (fConst217 / fTemp233))) + -0.5f))) - (((fRec313[2] * (((fTemp236 + -1.41421354f) / fTemp235) + 1.0f)) + (2.0f * (fRec313[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp235)))))) / fTemp237));
			float fTemp238 = (fRec109[0] + 0.5f);
			iRec316[0] = ((iVec0[1] + iRec316[1]) % int((fConst0 / fTemp238)));
			float fTemp239 = float(iRec316[0]);
			float fTemp240 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fTemp238) * (1.0f - (0.000500000024f * std::min<float>(fTemp238, 1000.0f)))))))));
			float fTemp241 = (1.0f / fTemp240);
			float fTemp242 = (((fTemp241 + 1.41421354f) / fTemp240) + 1.0f);
			fRec315[0] = ((2.0f * (((float((fTemp239 < (fConst215 / fTemp238))) + -0.5f) + (float((fTemp239 < (fConst216 / fTemp238))) + -0.5f)) + (float((fTemp239 < (fConst217 / fTemp238))) + -0.5f))) - (((fRec315[2] * (((fTemp241 + -1.41421354f) / fTemp240) + 1.0f)) + (2.0f * (fRec315[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp240)))))) / fTemp242));
			float fTemp243 = (fRec112[0] + 0.5f);
			iRec318[0] = ((iVec0[1] + iRec318[1]) % int((fConst0 / fTemp243)));
			float fTemp244 = float(iRec318[0]);
			float fTemp245 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fTemp243) * (1.0f - (0.000500000024f * std::min<float>(fTemp243, 1000.0f)))))))));
			float fTemp246 = (1.0f / fTemp245);
			float fTemp247 = (((fTemp246 + 1.41421354f) / fTemp245) + 1.0f);
			fRec317[0] = ((2.0f * (((float((fTemp244 < (fConst215 / fTemp243))) + -0.5f) + (float((fTemp244 < (fConst216 / fTemp243))) + -0.5f)) + (float((fTemp244 < (fConst217 / fTemp243))) + -0.5f))) - (((fRec317[2] * (((fTemp246 + -1.41421354f) / fTemp245) + 1.0f)) + (2.0f * (fRec317[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp245)))))) / fTemp247));
			float fTemp248 = (fRec115[0] + 0.5f);
			iRec320[0] = ((iVec0[1] + iRec320[1]) % int((fConst0 / fTemp248)));
			float fTemp249 = float(iRec320[0]);
			float fTemp250 = std::tan((fConst28 * std::min<float>(20000.0f, std::max<float>(20.0f, (6.66666651f * ((fTemp74 * fTemp248) * (1.0f - (0.000500000024f * std::min<float>(fTemp248, 1000.0f)))))))));
			float fTemp251 = (1.0f / fTemp250);
			float fTemp252 = (((fTemp251 + 1.41421354f) / fTemp250) + 1.0f);
			fRec319[0] = ((2.0f * (((float((fTemp249 < (fConst215 / fTemp248))) + -0.5f) + (float((fTemp249 < (fConst216 / fTemp248))) + -0.5f)) + (float((fTemp249 < (fConst217 / fTemp248))) + -0.5f))) - (((fRec319[2] * (((fTemp251 + -1.41421354f) / fTemp250) + 1.0f)) + (2.0f * (fRec319[1] * (1.0f - (1.0f / mydsp_faustpower2_f(fTemp250)))))) / fTemp252));
			float fTemp253 = (fTemp68 * (((((fRec313[2] + (fRec313[0] + (2.0f * fRec313[1]))) / fTemp237) + ((fRec315[2] + (fRec315[0] + (2.0f * fRec315[1]))) / fTemp242)) + ((fRec317[2] + (fRec317[0] + (2.0f * fRec317[1]))) / fTemp247)) + ((fRec319[2] + (fRec319[0] + (2.0f * fRec319[1]))) / fTemp252)));
			float fTemp254 = std::fabs((0.25f * fTemp253));
			fRec323[0] = std::max<float>(fTemp254, ((fSlow273 * fRec323[1]) + (fSlow274 * fTemp254)));
			fRec322[0] = ((fSlow270 * fRec322[1]) + (fRec323[0] * fSlow275));
			fRec321[0] = ((fConst46 * fRec321[1]) + (fSlow267 * std::max<float>(((20.0f * std::log10(fRec322[0])) - fSlow276), 0.0f)));
			float fTemp255 = (0.25f * (fTemp253 * std::pow(10.0f, (0.0500000007f * fRec321[0]))));
			fRec324[0] = (fTemp255 - (fSlow280 * ((fSlow281 * fRec324[2]) + (fSlow282 * fRec324[1]))));
			float fTemp256 = (fSlow257 * fRec312[1]);
			fRec312[0] = (fTemp255 - ((fSlow280 * (fRec324[2] + (fRec324[0] + (2.0f * fRec324[1])))) + (((fSlow290 * fRec312[2]) + fTemp256) / fSlow291)));
			float fTemp257 = (fSlow254 * fRec311[1]);
			fRec311[0] = ((((fTemp256 + (fRec312[0] * fSlow293)) + (fSlow294 * fRec312[2])) / fSlow291) - (((fSlow302 * fRec311[2]) + fTemp257) / fSlow303));
			fRec310[0] = ((((fTemp257 + (fRec311[0] * fSlow305)) + (fSlow306 * fRec311[2])) / fSlow303) - (fSlow307 * ((fSlow308 * fRec310[2]) + (fSlow309 * fRec310[1]))));
			float fTemp258 = ((fConst10 * fRec309[2]) + (((fSlow251 * (fRec310[2] + (fRec310[0] + (2.0f * fRec310[1])))) + (fTemp226 + (fTemp214 + (fTemp149 + (fTemp59 + (fSlow547 * fTemp136)))))) + (fConst9 * fRec309[0])));
			float fTemp259 = std::fabs(fTemp258);
			fRec327[0] = std::max<float>(fTemp259, ((fConst698 * fRec327[1]) + (fConst699 * fTemp259)));
			fRec326[0] = ((fConst696 * fRec326[1]) + (fConst697 * fRec327[0]));
			fRec325[0] = ((fConst46 * fRec325[1]) + (fConst47 * (0.0f - (0.00990098994f * std::max<float>((20.0f * (std::log10(fRec326[0]) + 1.0f)), 0.0f)))));
			float fTemp260 = (fTemp258 * std::pow(10.0f, (0.0500000007f * fRec325[0])));
			float fTemp261 = std::fabs(fTemp260);
			fRec330[0] = std::max<float>(fTemp261, ((fConst702 * fRec330[1]) + (fConst703 * fTemp261)));
			fRec329[0] = ((fConst700 * fRec329[1]) + (fConst701 * fRec330[0]));
			fRec328[0] = ((fConst46 * fRec328[1]) + (fConst47 * (0.0f - (0.0909090936f * std::max<float>(((20.0f * std::log10(fRec329[0])) + 5.0f), 0.0f)))));
			float fTemp262 = (fTemp260 * std::pow(10.0f, (0.0500000007f * fRec328[0])));
			float fTemp263 = std::fabs((fSlow2 * fTemp262));
			fRec333[0] = std::max<float>(fTemp263, ((fConst46 * fRec333[1]) + (fConst47 * fTemp263)));
			fRec332[0] = ((fConst704 * fRec332[1]) + (fConst705 * fRec333[0]));
			fRec331[0] = ((fConst46 * fRec331[1]) + (fConst47 * (0.0f - (0.899999976f * std::max<float>((20.0f * std::log10(fRec332[0])), 0.0f)))));
			output1[i] = FAUSTFLOAT((fSlow546 * std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow2 * (fTemp262 * std::pow(10.0f, (0.0500000007f * fRec331[0]))))))));
			iVec0[1] = iVec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec23[1] = fRec23[0];
			iRec25[1] = iRec25[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			iRec30[1] = iRec30[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec31[1] = fRec31[0];
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
			fRec9[1] = fRec9[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec36[1] = fRec36[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec40[1] = fRec40[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec44[1] = fRec44[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec48[1] = fRec48[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec56[1] = fRec56[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
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
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fVec21[1] = fVec21[0];
			fVec22[1] = fVec22[0];
			fVec23[1] = fVec23[0];
			iRec70[1] = iRec70[0];
			fRec71[1] = fRec71[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec91[1] = fRec91[0];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fRec92[1] = fRec92[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fVec24[1] = fVec24[0];
			fVec25[1] = fVec25[0];
			fVec26[1] = fVec26[0];
			iRec100[1] = iRec100[0];
			fRec99[1] = fRec99[0];
			fRec101[1] = fRec101[0];
			fRec104[1] = fRec104[0];
			iRec103[1] = iRec103[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fRec109[1] = fRec109[0];
			iRec108[1] = iRec108[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec112[1] = fRec112[0];
			iRec111[1] = iRec111[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fRec115[1] = fRec115[0];
			iRec114[1] = iRec114[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec118[1] = fRec118[0];
			fRec117[1] = fRec117[0];
			fRec116[1] = fRec116[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			iRec123[1] = iRec123[0];
			fRec127[1] = fRec127[0];
			fVec28[1] = fVec28[0];
			fVec29[1] = fVec29[0];
			fVec30[1] = fVec30[0];
			fRec128[1] = fRec128[0];
			fRec125[1] = fRec125[0];
			fRec129[1] = fRec129[0];
			fRec132[2] = fRec132[1];
			fRec132[1] = fRec132[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			iRec133[1] = iRec133[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
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
			fRec143[2] = fRec143[1];
			fRec143[1] = fRec143[0];
			fRec144[2] = fRec144[1];
			fRec144[1] = fRec144[0];
			fRec145[2] = fRec145[1];
			fRec145[1] = fRec145[0];
			fRec146[2] = fRec146[1];
			fRec146[1] = fRec146[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec148[2] = fRec148[1];
			fRec148[1] = fRec148[0];
			fRec149[2] = fRec149[1];
			fRec149[1] = fRec149[0];
			fRec150[2] = fRec150[1];
			fRec150[1] = fRec150[0];
			fRec151[2] = fRec151[1];
			fRec151[1] = fRec151[0];
			fRec152[2] = fRec152[1];
			fRec152[1] = fRec152[0];
			fRec153[2] = fRec153[1];
			fRec153[1] = fRec153[0];
			fRec154[2] = fRec154[1];
			fRec154[1] = fRec154[0];
			fRec155[2] = fRec155[1];
			fRec155[1] = fRec155[0];
			fRec156[2] = fRec156[1];
			fRec156[1] = fRec156[0];
			fRec157[2] = fRec157[1];
			fRec157[1] = fRec157[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			fRec161[2] = fRec161[1];
			fRec161[1] = fRec161[0];
			fRec162[2] = fRec162[1];
			fRec162[1] = fRec162[0];
			fRec163[2] = fRec163[1];
			fRec163[1] = fRec163[0];
			fRec164[2] = fRec164[1];
			fRec164[1] = fRec164[0];
			fRec165[2] = fRec165[1];
			fRec165[1] = fRec165[0];
			fRec166[2] = fRec166[1];
			fRec166[1] = fRec166[0];
			fRec167[2] = fRec167[1];
			fRec167[1] = fRec167[0];
			fRec168[2] = fRec168[1];
			fRec168[1] = fRec168[0];
			fRec169[2] = fRec169[1];
			fRec169[1] = fRec169[0];
			fRec170[2] = fRec170[1];
			fRec170[1] = fRec170[0];
			fRec171[2] = fRec171[1];
			fRec171[1] = fRec171[0];
			fRec172[2] = fRec172[1];
			fRec172[1] = fRec172[0];
			fRec173[2] = fRec173[1];
			fRec173[1] = fRec173[0];
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
			fVec31[1] = fVec31[0];
			fRec126[1] = fRec126[0];
			fRec183[1] = fRec183[0];
			fRec186[1] = fRec186[0];
			fRec185[1] = fRec185[0];
			fRec184[1] = fRec184[0];
			fRec187[2] = fRec187[1];
			fRec187[1] = fRec187[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fRec191[1] = fRec191[0];
			fRec193[1] = fRec193[0];
			fRec197[1] = fRec197[0];
			fRec196[1] = fRec196[0];
			fRec195[1] = fRec195[0];
			fRec194[1] = fRec194[0];
			fRec192[1] = fRec192[0];
			fRec200[1] = fRec200[0];
			fRec199[1] = fRec199[0];
			fRec198[1] = fRec198[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec190[2] = fRec190[1];
			fRec190[1] = fRec190[0];
			fRec189[2] = fRec189[1];
			fRec189[1] = fRec189[0];
			fRec188[2] = fRec188[1];
			fRec188[1] = fRec188[0];
			fVec33[1] = fVec33[0];
			fVec34[1] = fVec34[0];
			fVec35[1] = fVec35[0];
			fRec230[1] = fRec230[0];
			fRec226[1] = fRec226[0];
			fRec231[1] = fRec231[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec233[j0] = fRec233[(j0 - 1)];
			}
			fVec36[1] = fVec36[0];
			fVec37[2] = fVec37[1];
			fVec37[1] = fVec37[0];
			fRec222[1] = fRec222[0];
			fRec218[1] = fRec218[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec212[j1] = fRec212[(j1 - 1)];
			}
			fRec205[1] = fRec205[0];
			iRec235[1] = iRec235[0];
			fRec236[1] = fRec236[0];
			fRec239[1] = fRec239[0];
			fRec238[1] = fRec238[0];
			fRec237[1] = fRec237[0];
			fRec240[2] = fRec240[1];
			fRec240[1] = fRec240[0];
			fRec204[2] = fRec204[1];
			fRec204[1] = fRec204[0];
			fRec203[2] = fRec203[1];
			fRec203[1] = fRec203[0];
			fRec202[2] = fRec202[1];
			fRec202[1] = fRec202[0];
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
			fRec298[1] = fRec298[0];
			fRec297[1] = fRec297[0];
			fRec296[1] = fRec296[0];
			fRec299[2] = fRec299[1];
			fRec299[1] = fRec299[0];
			fRec243[2] = fRec243[1];
			fRec243[1] = fRec243[0];
			fRec242[2] = fRec242[1];
			fRec242[1] = fRec242[0];
			fRec241[2] = fRec241[1];
			fRec241[1] = fRec241[0];
			fRec302[1] = fRec302[0];
			fRec301[1] = fRec301[0];
			fRec300[1] = fRec300[0];
			fRec305[1] = fRec305[0];
			fRec304[1] = fRec304[0];
			fRec303[1] = fRec303[0];
			fRec308[1] = fRec308[0];
			fRec307[1] = fRec307[0];
			fRec306[1] = fRec306[0];
			fRec309[2] = fRec309[1];
			fRec309[1] = fRec309[0];
			iRec314[1] = iRec314[0];
			fRec313[2] = fRec313[1];
			fRec313[1] = fRec313[0];
			iRec316[1] = iRec316[0];
			fRec315[2] = fRec315[1];
			fRec315[1] = fRec315[0];
			iRec318[1] = iRec318[0];
			fRec317[2] = fRec317[1];
			fRec317[1] = fRec317[0];
			iRec320[1] = iRec320[0];
			fRec319[2] = fRec319[1];
			fRec319[1] = fRec319[0];
			fRec323[1] = fRec323[0];
			fRec322[1] = fRec322[0];
			fRec321[1] = fRec321[0];
			fRec324[2] = fRec324[1];
			fRec324[1] = fRec324[0];
			fRec312[2] = fRec312[1];
			fRec312[1] = fRec312[0];
			fRec311[2] = fRec311[1];
			fRec311[1] = fRec311[0];
			fRec310[2] = fRec310[1];
			fRec310[1] = fRec310[0];
			fRec327[1] = fRec327[0];
			fRec326[1] = fRec326[0];
			fRec325[1] = fRec325[0];
			fRec330[1] = fRec330[0];
			fRec329[1] = fRec329[0];
			fRec328[1] = fRec328[0];
			fRec333[1] = fRec333[0];
			fRec332[1] = fRec332[0];
			fRec331[1] = fRec331[0];
		}
	}

};

#endif
