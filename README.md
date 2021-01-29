# Faust API

This API allows to interact with a Faust object and its associated audio engine at a high level. The idea is that all the audio part of the app is implemented in Faust allowing developers to focus on the design of the app itself. 

### App Set-Up

Import `DspFaust.h` and `DspFaust.cpp` in your project (this can be done simply by dragging these files in your project tree). Then, import `DspFaust.h` (`#include "DspFaust.h"`) in the file where you want to create/control the Faust object. 

### Using the C++ API

The current Faust API is designed to seamlessly integrate to the life cycle of an app. It is accessible through a single `DspFaust` object. The constructor of that object is used to set the sampling rate and the buffer size:

	DspFaust* dspFaust = new DspFaust(SR, buffer_size);

The `start()` method is used to start the audio computing. Similarly, `stop()` can be called to stop the audio computing.

It is possible to interact with the different parameters of the Faust object by using the `setParamValue` method. Two versions of this method exist: one where the parameter can be selected by its address and one where it can be selected using its ID. The [Parameters List](#parameters-list) section gives a list of the addresses and corresponding IDs of the current Faust object.

If your Faust object is polyphonic (e.g. if you used the `-nvoices` option when generating this API), then you can use the MIDI polyphony methods like `keyOn`, `keyOff`, etc.

It is possible to change the parameters of polyphonic voices independently using the `setVoiceParamValue` method. This method takes as one of its arguments the address to the voice returned by `keyOn` or `newVoice` when it is called. E.g:

	uintptr_t voiceAddress = dspFaust->keyOn(70, 100);
	dspFaust->setVoiceParamValue(1, voiceAddress, 214);
	dspFaust->keyOff(70);
	
In the example above, a new note is created and its parameter ID 1 is modified. This note is then terminated. Note that parameters addresses (path) are different for independent voices than when using `setParamValue`. The list of these addresses is provided in a separate sub-section of the [Parameters List](#parameters-list) section.

Finally, note that new voices don't necessarily have to be created using `keyOn`. Indeed, you might choose to just use the `newVoice` method for that:

	uintptr_t voiceAddress = dspFaust->newVoice();
	dspFaust->setVoiceParamValue(1, voiceAddress, 214);
	dspFaust->deleteVoice(voiceAddress);

This is particularly useful when making apps where each finger of the user is an independent sound that doesn't necessarily has a pitch.

In case you would like to use the built-in accelerometer or gyroscope of your device to control some of the parameters of your Faust object, all you have to do is to send the raw accelerometer data to it by using the `propagateAcc` or `propagateGyr` for the gyroscope. After that, mappings can be configured directly from the Faust code using [this technique](#using-built-in-sensors-to-control-parameters) or using the `setAccConverter` and `setGyrConverter` method.

## Parameters List

### Main Parameters

* **0**: `/Mapping_Tabs/Comp_Section/Track_2/Attack`
* **1**: `/Mapping_Tabs/Comp_Section/Track_2/Ratio`
* **2**: `/Mapping_Tabs/Comp_Section/Track_2/Release`
* **3**: `/Mapping_Tabs/Comp_Section/Track_2/Threshold`
* **4**: `/Mapping_Tabs/Comp_Section/Track_3/Attack`
* **5**: `/Mapping_Tabs/Comp_Section/Track_3/Ratio`
* **6**: `/Mapping_Tabs/Comp_Section/Track_3/Release`
* **7**: `/Mapping_Tabs/Comp_Section/Track_3/Threshold`
* **8**: `/Mapping_Tabs/EQ_Section/Track_1/1_HPF/Freq`
* **9**: `/Mapping_Tabs/EQ_Section/Track_1/1_HPF/Q`
* **10**: `/Mapping_Tabs/EQ_Section/Track_1/2_Parametric_1/Freq`
* **11**: `/Mapping_Tabs/EQ_Section/Track_1/2_Parametric_1/Gain`
* **12**: `/Mapping_Tabs/EQ_Section/Track_1/2_Parametric_1/Q`
* **13**: `/Mapping_Tabs/EQ_Section/Track_1/3_Parametric_2/Freq`
* **14**: `/Mapping_Tabs/EQ_Section/Track_1/3_Parametric_2/Gain`
* **15**: `/Mapping_Tabs/EQ_Section/Track_1/3_Parametric_2/Q`
* **16**: `/Mapping_Tabs/EQ_Section/Track_1/4_LPF/Freq`
* **17**: `/Mapping_Tabs/EQ_Section/Track_1/4_LPF/Q`
* **18**: `/Mapping_Tabs/EQ_Section/Track_2/1_HPF/Freq`
* **19**: `/Mapping_Tabs/EQ_Section/Track_2/1_HPF/Q`
* **20**: `/Mapping_Tabs/EQ_Section/Track_2/2_Parametric_1/Freq`
* **21**: `/Mapping_Tabs/EQ_Section/Track_2/2_Parametric_1/Gain`
* **22**: `/Mapping_Tabs/EQ_Section/Track_2/2_Parametric_1/Q`
* **23**: `/Mapping_Tabs/EQ_Section/Track_2/3_Parametric_2/Freq`
* **24**: `/Mapping_Tabs/EQ_Section/Track_2/3_Parametric_2/Gain`
* **25**: `/Mapping_Tabs/EQ_Section/Track_2/3_Parametric_2/Q`
* **26**: `/Mapping_Tabs/EQ_Section/Track_2/4_LPF/Freq`
* **27**: `/Mapping_Tabs/EQ_Section/Track_2/4_LPF/Q`
* **28**: `/Mapping_Tabs/EQ_Section/Track_3/1_HPF/Freq`
* **29**: `/Mapping_Tabs/EQ_Section/Track_3/1_HPF/Q`
* **30**: `/Mapping_Tabs/EQ_Section/Track_3/2_Parametric_1/Freq`
* **31**: `/Mapping_Tabs/EQ_Section/Track_3/2_Parametric_1/Gain`
* **32**: `/Mapping_Tabs/EQ_Section/Track_3/2_Parametric_1/Q`
* **33**: `/Mapping_Tabs/EQ_Section/Track_3/3_Parametric_2/Freq`
* **34**: `/Mapping_Tabs/EQ_Section/Track_3/3_Parametric_2/Gain`
* **35**: `/Mapping_Tabs/EQ_Section/Track_3/3_Parametric_2/Q`
* **36**: `/Mapping_Tabs/EQ_Section/Track_3/4_LPF/Freq`
* **37**: `/Mapping_Tabs/EQ_Section/Track_3/4_LPF/Q`
* **38**: `/Mapping_Tabs/EQ_Section/Track_4/1_HPF/Freq`
* **39**: `/Mapping_Tabs/EQ_Section/Track_4/1_HPF/Q`
* **40**: `/Mapping_Tabs/EQ_Section/Track_4/2_Parametric_1/Freq`
* **41**: `/Mapping_Tabs/EQ_Section/Track_4/2_Parametric_1/Gain`
* **42**: `/Mapping_Tabs/EQ_Section/Track_4/2_Parametric_1/Q`
* **43**: `/Mapping_Tabs/EQ_Section/Track_4/3_Parametric_2/Freq`
* **44**: `/Mapping_Tabs/EQ_Section/Track_4/3_Parametric_2/Gain`
* **45**: `/Mapping_Tabs/EQ_Section/Track_4/3_Parametric_2/Q`
* **46**: `/Mapping_Tabs/EQ_Section/Track_4/4_LPF/Freq`
* **47**: `/Mapping_Tabs/EQ_Section/Track_4/4_LPF/Q`
* **48**: `/Mapping_Tabs/Mixer_And_Master/Master_Gain/Master_Gain`
* **49**: `/Mapping_Tabs/Mixer_And_Master/Track_Gain/Track_1`
* **50**: `/Mapping_Tabs/Mixer_And_Master/Track_Gain/Track_2`
* **51**: `/Mapping_Tabs/Mixer_And_Master/Track_Gain/Track_3`
* **52**: `/Mapping_Tabs/Mixer_And_Master/Track_Gain/Track_4`
* **53**: `/Mapping_Tabs/Mixer_And_Master/Track_Mute/1`
* **54**: `/Mapping_Tabs/Mixer_And_Master/Track_Mute/2`
* **55**: `/Mapping_Tabs/Mixer_And_Master/Track_Mute/3`
* **56**: `/Mapping_Tabs/Mixer_And_Master/Track_Mute/4`
* **57**: `/Mapping_Tabs/Sonification_Control/Soni_10/Control_0`
* **58**: `/Mapping_Tabs/Sonification_Control/Soni_11/Control_0`
* **59**: `/Mapping_Tabs/Sonification_Control/Soni_2/Control_0`
* **60**: `/Mapping_Tabs/Sonification_Control/Soni_3/Control_0`
* **61**: `/Mapping_Tabs/Sonification_Control/Soni_4/Control_0`
* **62**: `/Mapping_Tabs/Sonification_Control/Soni_4/Control_1`
* **63**: `/Mapping_Tabs/Sonification_Control/Soni_4/Control_2`
* **64**: `/Mapping_Tabs/Sonification_Control/Soni_4/Control_3`
* **65**: `/Mapping_Tabs/Sonification_Control/Soni_5/Control_0`
* **66**: `/Mapping_Tabs/Sonification_Control/Soni_6/Control_0`
* **67**: `/Mapping_Tabs/Sonification_Control/Soni_7/Control_0`
* **68**: `/Mapping_Tabs/Sonification_Control/Soni_9/Control_0`


## API Reference

### `DspFaust()`
Default constructor, to be used wih audio drivers

 that impose their sample rate and buffer size

(like JACK and JUCE)



#### Arguments



* `auto_connect`: whether to automatically connect audio outpus to the hardware (usable with JACK)


---


### `DspFaust(int SR, int BS)`
Constructor.



#### Arguments



* `SR`: sampling rate

* `BS`: buffer size

* `auto_connect`: whether to automatically connect audio outputs to the hardware (usable with JACK)


---


### `DspFaust(const string& dsp_content, int SR, int BS)`
Constructor.



#### Arguments



* `dsp_content`: the DSP as a file or string

* `SR`: sampling rate

* `BS`: buffer size

* `auto_connect`: whether to automatically connect audio outputs to the hardware (usable with JACK)


---


### `bool start()`
Start the audio processing.



Returns `true` if successful and `false` if not.


---


### `void stop()`
Stop the audio processing.


---


### `bool isRunning()`
Returns `true` if audio is running.


---


### `long keyOn(int pitch, int velocity)`
Instantiate a new polyphonic voice. This method can

only be used if the `[style:poly]` metadata is used in

the Faust code or if the `-polyvoices` flag has been

provided before compilation.



`keyOn` will return 0 if the Faust object is not

polyphonic or the address to the allocated voice as

a `uintptr_t` otherwise. This value can be used later with

[`setVoiceParamValue`](#setvoiceparamvalue) or

[`getVoiceParamValue`](#getvoiceparamvalue) to access

the parameters of a specific voice.



#### Arguments



* `pitch`: MIDI note number (0-127)

* `velocity`: MIDI velocity (0-127)


---


### `int keyOff(int pitch)`
De-instantiate a polyphonic voice. This method can

only be used if the `[style:poly]` metadata is used in

the Faust code or if the `-nvoices` flag has been

provided before compilation.



`keyOff` will return 0 if the object is not polyphonic

and 1 otherwise.



#### Arguments



* `pitch`: MIDI note number (0-127), should be the same

as the one used for `keyOn`


---


### `uintptr_t newVoice()`
Instantiate a new polyphonic voice. This method can

only be used if the `[style:poly]` metadata is used in

the Faust code or if `-nvoices` flag has been

provided before compilation.



`newVoice` will return 0 if the Faust object is not

polyphonic or the address to the allocated voice as

a `uintptr_t` otherwise. This value can be used later with

`setVoiceParamValue`, `getVoiceParamValue` or

`deleteVoice` to access the parameters of a specific

voice.


---


### `int deleteVoice(uintptr_t voice)`
De-instantiate a polyphonic voice. This method can

only be used if the `[style:poly]` metadata is used in

the Faust code or if `-nvoices` flag has been

provided before compilation.



`deleteVoice` will return 0 if the object is not polyphonic

and 1 otherwise.



#### Arguments



* `voice`: the address of the voice given by `newVoice`


---


### `void allNotesOff()`
Terminates all the active voices, gently (with release when hard = false or immediately when hard = true).


---


### `void propagateMidi(int count, double time, int type, int channel, int data1, int data2)`
Take a raw MIDI message and propagate it to the Faust

DSP object. This method can be used concurrently with

[`keyOn`](#keyOn) and [`keyOff`](#keyOff).



`propagateMidi` can

only be used if the `[style:poly]` metadata is used in

the Faust code or if `-nvoices` flag has been

provided before compilation.



#### Arguments



* `count`: size of the message (1-3)

* `time`: time stamp

* `type`: message type (byte)

* `channel`: channel number

* `data1`: first data byte (should be `null` if `count<2`)

* `data2`: second data byte (should be `null` if `count<3`)


---


### `const char* getJSONUI()`
Returns the JSON description of the UI of the Faust object.


---


### `const char* getJSONMeta()`
Returns the JSON description of the metadata of the Faust object.


---


### `void buildUserInterface(UI* ui_interface)`
Calls the polyphonic or monophonic buildUserInterface with the ui_interface parameter.



#### Arguments



* `ui_interface`: an UI* object


---


### `int getParamsCount()`
Returns the number of parameters of the Faust object.


---


### `void setParamValue(const char* address, float value)`
Set the value of one of the parameters of the Faust

object in function of its address (path).



#### Arguments



* `address`: address (path) of the parameter

* `value`: value of the parameter


---


### `void setParamValue(int id, float value)`
Set the value of one of the parameters of the Faust

object in function of its id.



#### Arguments



* `id`: id of the parameter

* `value`: value of the parameter


---


### `float getParamValue(const char* address)`
Returns the value of a parameter in function of its

address (path).



#### Arguments



* `address`: address (path) of the parameter


---


### `float getParamValue(int id)`
Returns the value of a parameter in function of its

id.



#### Arguments



* `id`: id of the parameter


---


### `void setVoiceParamValue(const char* address, uintptr_t voice, float value)`
Set the value of one of the parameters of the Faust

object in function of its address (path) for a

specific voice.



#### Arguments



* `address`: address (path) of the parameter

* `voice`: address of the polyphonic voice (retrieved

from `keyOn`

* `value`: value of the parameter


---


### `void setVoiceValue(int id, uintptr_t voice, float value)`
Set the value of one of the parameters of the Faust

object in function of its id for a

specific voice.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved

from `keyOn`

* `value`: value of the parameter


---


### `float getVoiceParamValue(const char* address, uintptr_t voice)`
Returns the value of a parameter in function of its

address (path) for a specific voice.



#### Arguments



* `address`: address (path) of the parameter

* `voice`: address of the polyphonic voice (retrieved

from `keyOn`)


---


### `float getVoiceParamValue(int id, uintptr_t voice)`
Returns the value of a parameter in function of its

id for a specific voice.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved

from `keyOn`)


---


### `const char* getParamAddress(int id)`
Returns the address (path) of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `const char* getVoiceParamAddress(int id, uintptr_t voice)`
Returns the address (path) of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved

from `keyOn`)


---


### `float getParamMin(const char* address)`
Returns the minimum value of a parameter in function of

its address (path).



#### Arguments



* `address`: address (path) of the parameter


---


### `float getParamMin(int id)`
Returns the minimum value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `float getParamMax(const char* address)`
Returns the maximum value of a parameter in function of

its address (path).



#### Arguments



* `address`: address (path) of the parameter


---


### `float getParamMax(int id)`
Returns the maximum value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `float getParamInit(const char* address)`
Returns the default value of a parameter in function of

its address (path).



#### Arguments



* `address`: address (path) of the parameter


---


### `float getParamInit(int id)`
Returns the default value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `const char* getMetadata(const char* address, const char* key)`
Returns the metadataof a parameter in function of

its address (path) and the metadata key.



#### Arguments



* `address`: address (path) of the parameter


---


### `const char* getMetadata(int id, const char* key)`
Returns the metadataof a parameter in function of

its iD and the metadata key.



#### Arguments



* `id`: id of the parameter


---


### `void propagateAcc(int acc, float v)`
Propagate the RAW value of a specific accelerometer

axis to the Faust object.



#### Arguments



* `acc`: the accelerometer axis (**0**: x, **1**: y, **2**: z)

* `v`: the RAW acceleromter value in m/s


---


### `void setAccConverter(int p, int acc, int curve, float amin, float amid, float amax)`
Set the conversion curve for the accelerometer.



#### Arguments



* `p`: the UI parameter id

* `acc`: the accelerometer axis (**0**: x, **1**: y, **2**: z)

* `curve`: the curve (**0**: up, **1**: down, **2**: up and down)

* `amin`: mapping min point

* `amid`: mapping middle point

* `amax`: mapping max point


---


### `void propagateGyr(int gyr, float v)`
Propagate the RAW value of a specific gyroscope

axis to the Faust object.



#### Arguments



* `gyr`: the gyroscope axis (**0**: x, **1**: y, **2**: z)

* `v`: the RAW acceleromter value in m/s


---


### `void setGyrConverter(int p, int gyr, int curve, float amin, float amid, float amax)`
Set the conversion curve for the gyroscope.



#### Arguments



* `p`: the UI parameter id

* `gyr`: the gyroscope axis (**0**: x, **1**: y, **2**: z)

* `curve`: the curve (**0**: up, **1**: down, **2**: up and down)

* `amin`: mapping min point

* `amid`: mapping middle point

* `amax`: mapping max point


---


### `float getCPULoad()`
Returns the CPU load (between 0 and 1.0).


---


### `void configureOSC(int xmit, int inport, int outport, int errport, const char* address)`
Change the OSC configuration.



#### Arguments



* `xmit`: the xmit state (off-0 all-1 alias-2)

* `inport`: the input port number

* `outport`: the output port number

* `errport`: the error port number

* `address`: the destination IP address


---


### `bool isOSCOn()`
Return OSC Status.


---

