import("stdfaust.lib");

// TABS
tab_SoniControl(x) = tgroup("Mapping_Tabs",vgroup("Sonification_Control",x));
tab_MixMaster(x) = tgroup("Mapping_Tabs",hgroup("Mixer_And_Master",x)); 	
tab_EQ(x) = tgroup("Mapping_Tabs",vgroup("EQ_Section",x));
tab_Comp(x) = tgroup("Mapping_Tabs",vgroup("Comp_Section",x));

// SLIDER GROUPS

// MIX AND MASTER
sliderGroup_TrackGain(x) = tab_MixMaster(vgroup("Track_Gain",x));
sliderGroup_TrackMute(x) = tab_MixMaster(vgroup("Track_Mute",x));
sliderGroup_MasterGain(x) = tab_MixMaster(vgroup("Master_Gain",x));
// EQ
sliderGroup_channelEQ(i,x) = tab_EQ(hgroup("Track %i",x));
// COMP
sliderGroup_channelComp(i,x) = tab_Comp(hgroup("Track %i",x));

// MASTER GAIN SLIDER
masterGain = sliderGroup_MasterGain(vslider("Master Gain",-6,-96,12,0.01) : ba.db2linear);
reverbOn = sliderGroup_MasterGain(checkbox("Reverb ON"));

// Sonification Sliders and Preprocessing
soniSlider(idx,defaultVal) = tab_SoniControl(vgroup("FV_Values",hslider("Soni %idx",defaultVal,0,2000,0.001)));

// Single Horizontal Row of Soni Sliders for each feedback variable
array_soniSliders(idx,defaultVals) = finalGroup with
{
  finalGroup = tab_SoniControl(hgroup("Soni %idx",par(i,4,configuredSlider(i))));
  configuredSlider(x) = hslider("Control %x",defaultVal(x),0,2000,0.001);
  defaultVal(x) = defaultVals : ba.selectn(4,x);
};

// Definition of Sonification UI and Variables - CURRENT TOTAL: 24
// Triggers
SONI_1_DefaultVals = 50,0,0,0;
SONI_1_PercTr = array_soniSliders(1,SONI_1_DefaultVals);
SONI_3_DefaultVals = 50,0,0,0;
SONI_3_MelodyTr = array_soniSliders(3,SONI_3_DefaultVals);
SONI_5_DefaultVals = 50,0,0,0;
SONI_5_ChordTr = array_soniSliders(5,SONI_5_DefaultVals);
SONI_24_DefaultVals = 50,0,0,0;
SONI_24_FluteTrg = array_soniSliders(24,SONI_24_DefaultVals);
SONI_12_DefaultVals = 50,0,0,0;
SONI_12_GtrTr = array_soniSliders(12,SONI_12_DefaultVals);
SONI_17_DefaultVals = 50,0,0,0;
SONI_17_WarningBell = array_soniSliders(17,SONI_17_DefaultVals);

// Fundamental Frequencies
SONI_9_DefaultVals = 7,0,0,0;
SONI_9_DjembeFr = array_soniSliders(9,SONI_9_DefaultVals);
SONI_2_DefaultVals = 440,0,0,0;
SONI_2_MelodyFr = array_soniSliders(2,SONI_2_DefaultVals);
SONI_4_DefaultVals = 440, 440, 440, 440;
SONI_4_ChordFr = array_soniSliders(4,SONI_4_DefaultVals);
SONI_8_DefaultVals = 440,0,0,0;
SONI_8_FluteFr = array_soniSliders(8,SONI_8_DefaultVals);
SONI_19_DefaultVals = 440,0,0,0;
SONI_19_GtrFr = array_soniSliders(19,SONI_19_DefaultVals);

// Dynamics
SONI_15_DefaultVals = 1,0,0,0;
SONI_15_DjembeSharp = array_soniSliders(15,SONI_15_DefaultVals);
SONI_14_DefaultVals = 0,0,0,0;
SONI_14_VoiceDyn = array_soniSliders(14,SONI_14_DefaultVals);
SONI_7_DefaultVals = 1,0,0,0;
SONI_7_ChordDyn = array_soniSliders(7,SONI_7_DefaultVals);
SONI_16_DefaultVals = 0.9,0,0,0;
SONI_16_FluteGn = array_soniSliders(16,SONI_16_DefaultVals);
SONI_18_DefaultVals = 1500,0,0,0;
SONI_18_GtrDyn = array_soniSliders(18,SONI_18_DefaultVals);

// Timbre
SONI_11_DefaultVals = 0,0,0,0;
SONI_11_Vowel = array_soniSliders(11,SONI_11_DefaultVals);
SONI_13_DefaultVals = 0.01,0,0,0;
SONI_13_GtrStf = array_soniSliders(13,SONI_13_DefaultVals);
SONI_25_DefaultVals = 0.036,0,0,0;
SONI_25_FluteVib = array_soniSliders(25,SONI_25_DefaultVals);

// Negative Feedback
SONI_6_DefaultVals = 0,0,0,0;
SONI_6_Detune = array_soniSliders(6,SONI_6_DefaultVals);
SONI_10_DefaultVals = 0.5,0,0,0;
SONI_10_PitchWarp = array_soniSliders(10,SONI_10_DefaultVals);

// Sine Waves
SONI_20_DefaultVals = 10,0,0,0;
SONI_20_Sine1P = array_soniSliders(20,SONI_20_DefaultVals);
SONI_21_DefaultVals = 10,0,0,0;
SONI_21_Sine2P = array_soniSliders(21,SONI_21_DefaultVals);
SONI_22_DefaultVals = 10,0,0,0;
SONI_22_Sine3P = array_soniSliders(22,SONI_22_DefaultVals);
SONI_23_DefaultVals = 0.5,0,0,0;
SONI_23_FluteMouthPos = array_soniSliders(23,SONI_23_DefaultVals);


// TRIGGERS
TRG_PERC_MAIN = FVToTrigger(SONI_1_PercTr : ba.selectn(4,0));		// TRACK 1
FRQ_PERC_MAIN = SONI_9_DjembeFr : ba.selectn(4,0);

FRQ_MEL = SONI_2_MelodyFr : ba.selectn(4,0);						// TRACK 2
TRG_MEL = FVToTrigger(SONI_3_MelodyTr : ba.selectn(4,0));
DYN_MEL = SONI_14_VoiceDyn : ba.selectn(4,0);

FRQ_CHORD_N1 = SONI_4_ChordFr : ba.selectn(4,0) : limit(20,20000);					// TRACK 3
FRQ_CHORD_N2 = SONI_4_ChordFr : ba.selectn(4,1) : limit(20,20000);
FRQ_CHORD_N3 = SONI_4_ChordFr : ba.selectn(4,2) : limit(20,20000);
FRQ_CHORD_N4 = SONI_4_ChordFr : ba.selectn(4,3) : limit(20,20000);
TRG_CHORD = FVToTrigger(SONI_5_ChordTr : ba.selectn(4,0));		
DYN_CHORD = SONI_7_ChordDyn : ba.selectn(4,0);

TRG_FLUTE_RAW = SONI_24_FluteTrg : ba.selectn(4,0);
TRG_FLUTE = FVToTrigger(TRG_FLUTE_RAW);
FRQ_FLUTE = SONI_8_FluteFr : ba.selectn(4,0) : ba.sAndH(TRG_FLUTE) : si.smooth(ba.tau2pole(0.001));			// TRACK 4

FRQ_BASS = FRQ_CHORD_N1 / 2.0;										// TRACK 5

FRQ_GTR = SONI_19_GtrFr : ba.selectn(4,0);
TRG_GTR = FVToTrigger(SONI_12_GtrTr : ba.selectn(4,0));				// TRACK 6

TRG_BELL = FVToTrigger(SONI_17_WarningBell : ba.selectn(4,0));		// TRACK 7

FRQ_SINE1 = SONI_20_Sine1P : ba.selectn(4,0);
FRQ_SINE2 = SONI_21_Sine2P : ba.selectn(4,0);
FRQ_SINE3 = SONI_22_Sine3P : ba.selectn(4,0);

PARAM_VAL_DETUNE = SONI_6_Detune : ba.selectn(4,0) : limit(0,1);					// DETUNE
PARAM_VAL_PAN = 0.5;
PARAM_VAL_DYNAMICS = 7;
PARAM_VAL_PITCH = SONI_10_PitchWarp : ba.selectn(4,0) : limit(0,1);				// PITCH
PARAM_VAL_VOWEL = SONI_11_Vowel : ba.selectn(4,0) : limit(0,3);					// VOWEL
PARAM_VAL_GTRSTF = SONI_13_GtrStf : ba.selectn(4,0) : limit(0.01,1);				// GUITAR STIFFNESS
PARAM_VAL_VOICEFRIC = 0;
PARAM_VAL_DJEMBESHARP = SONI_15_DjembeSharp : ba.selectn(4,0) : limit(0.01,5);		// DJEMBE STRIKE SHARPNESS
PARAM_VAL_FLUTEGN = SONI_16_FluteGn : ba.selectn(4,0);
PARAM_VAL_GTRDYN = SONI_18_GtrDyn : ba.selectn(4,0) : *(10) : si.smoo : limit(500,15000);				// GUITAR EXCITATION CUTOFF
PARAM_VAL_FLUTEMOUTHPOS = SONI_23_FluteMouthPos : ba.selectn(4,0);
PARAM_VAL_FLUTEVIB = SONI_25_FluteVib : ba.selectn(4,0);

// TRACK SYNTHESIS DEFINITION  

// TRACK 1 - MAIN PERCUSSION
DJ_FREQ = FRQ_PERC_MAIN : ba.sAndH(TRG_PERC_MAIN) + 50;
DJ_SHRPNS = 1 + PARAM_VAL_DJEMBESHARP * 2;
DJ_GAIN = 1 + ba.db2linear(PARAM_VAL_DJEMBESHARP);
Synth_T1_MainPerc = 
pm.djembe(DJ_FREQ,DJ_SHRPNS/8,DJ_SHRPNS,1,TRG_PERC_MAIN) * DJ_GAIN : monoChannel(1) : getPanFunction(0);

// TRACK 2 - MELODY
F0_M = FRQ_MEL : Soni_FreqWarpFactor;
synthFunc_Melody(freq) = voiceSynth_FormantBP;
Synth_T2_Melody = voiceSynth_FormantBP(F0_M,DYN_MEL,TRG_MEL,PARAM_VAL_DYNAMICS/2.0) : *(DYN_MEL) : monoChannel(2) : getPanFunction(0);
  
// TRACK 3 - CHORD
FREQ_LIST_C = FRQ_CHORD_N1,FRQ_CHORD_N2,FRQ_CHORD_N3,FRQ_CHORD_N4;																// LIST MIDI KEYS
F0_LIST_HZ_C = getChordFinalFreqs(FREQ_LIST_C);																					// MIDI KEYS TO FREQ
chordFreq(noteIdx) = F0_LIST_HZ_C : ba.selectn(4,noteIdx) : ba.sAndH(TRG_CHORD);																		// FREQ SELECTOR
chordTrg(noteIdx) =  TRG_CHORD;																								// TRG SELECTOR
synthFunc_Chord(trigger,freq) = pianoSim_singleNote(freq,trigger,PARAM_VAL_DYNAMICS/2);
chordSum = par(i,4,chordSingle_Synth(chordFreq(i), synthFunc_Chord(chordTrg(i)))) :> _,_;
Synth_T3_Chord = chordSum : stereoLinGain(DYN_CHORD) : stereoChannel(3);

// TRACK 4 - FLUTE
F0_FLUTE = FRQ_FLUTE : detuneFreq : Soni_FreqWarpFactor;
Synth_T4_Flute = fluteSimple(F0_FLUTE,PARAM_VAL_FLUTEGN,PARAM_VAL_FLUTEMOUTHPOS,PARAM_VAL_FLUTEVIB) : monoChannel(4) : getPanFunction(0);

// TRACK 5 - BASSLINE
F0_R = FRQ_BASS : ba.sAndH(TRG_CHORD) : Soni_FreqWarpFactor;
Synth_T5_Bass = fmSynth(F0_R,MOD_NUM_R,FREQ_FACTOR_R,RL_R,MOD_DEPTH_R,TRG_CHORD) : monoChannel(5) : getPanFunction(0);

// TRACK 6 - GUITAR
F0_GTR = FRQ_GTR * 4 : ba.sAndH(TRG_GTR) : detuneFreq : Soni_FreqWarpFactor;
Synth_T6_Guitar = guitarString(F0_GTR,PARAM_VAL_GTRSTF,TRG_GTR) : monoChannel(6) : getPanFunction(0);

// TRACK 7 - WARNINGS
Synth_T7_Warning = 
  pm.churchBell(1,10000,0.8,1,TRG_BELL) * en.ar(0.001,2,TRG_BELL) 
  + ((FRQ_SINE1 > 310) : si.smoo) * 2 * os.osc(max(20,FRQ_SINE1))
  + ((FRQ_SINE2 > 610) : si.smoo) * 2 * os.osc(max(20,FRQ_SINE2))
  + ((FRQ_SINE3 > 1210) : si.smoo) * 2 * os.osc(max(20,FRQ_SINE3))
  : monoChannel(7) : getPanFunction(0);

track1 = Synth_T1_MainPerc : stereoMasterSection(1);
track2 = Synth_T2_Melody   : stereoMasterSection(2);
track3 = Synth_T3_Chord    : stereoMasterSection(3);
track4 = Synth_T4_Flute    : stereoMasterSection(4);
track5 = Synth_T5_Bass     : stereoMasterSection(5);
track6 = Synth_T6_Guitar   : stereoMasterSection(6);
track7 = Synth_T7_Warning  : stereoMasterSection(7);
reverbTrack = track1,track2,track3,track4,track6 :> reverbMaster;
dryBus = track1,track2,track3,track4,track5,track6,track7 :> stereodBGain(0 - 24 * sqrt(1 - PARAM_VAL_PROXIMITY));

masterChannel = masterComp : stereoLinGain(masterGain) : stereoEffect(masterLimiter(0)) : stereoEffect(hard_clip(1));
  
process = dryBus,reverbTrack :> masterChannel;
							    
/////////////////////////////////////////////////////////////////////////////////// FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////////////

// UTILITY
BT_SMPL = ba.tempo(tempo);
stereoLinGain(gain) = _,_ : *(gain),*(gain);
stereodBGain(gain) = _,_ : *(linGain),*(linGain) with { linGain = ba.db2linear(gain); };
stereoLinMult(gainPair) = _,_ : *(gainPair : select2(0)),*(gainPair : select2(1));
stereoEffect(effect) = _,_ : effect,effect;
limit(lower,higher) = _ : max(lower) : min(higher);
hard_clip(limit) = _ : min(limit) : max(-1*limit) : _;

// CONVERT QUANTIZED FEEDBACK VARIABLE VALUE TO IMPULSIVE TRIGGER
FVToTrigger(quantInput) = min(1,trigger) with
{
  trigger = (posTrig + negTrig);
  posTrig = quantInput : ba.impulsify;
  negTrig = - 1 * (quantInput) : ba.impulsify * (quantInput > 0);
};

// CONVERT 0-10 VELOCITY VALUE TO MONO GAIN MULTIPLIER
applyVelocity(velocity,trigger,maxVel) = _ : *(gainMult)	 with
{
  sampledVel = velocity : ba.sAndH(trigger);
  dBGain = (sampledVel - 10) * 26.0 / 9.0;
  gainMult = ba.db2linear(dBGain);
};

// PREPROCESS CHORD FREQUENCIES
LIST_FREQ_DISTFACTORS = 0.05,-0.03,0.31,-0.27;																// CHORD FREQ DIST FACTORS
getChordFinalFreqs(LIST) = freqs with 
{   
  freqs = par(i,4,freq(i));																							// CREATE FREQ LIST
  freq(i) = freqs_pre(i) : Soni_FreqWarpFactor : *(Soni_freqDistFactor(i));											// APPLY TUNING SONIFICATIONS
  Soni_freqDistFactor(i) = LIST_FREQ_DISTFACTORS : ba.selectn(4,i) : *(PARAM_VAL_DETUNE) : +(1);					// CALCULATE CHORD DIST AMOUNT
  freqs_pre(i) = LIST : ba.selectn(4,i);																			// GET INDIVIDUAL MIDI KEYS
};

// GENERATE SINGLE CHORD NOTE
chordSingle_Synth(freq,synthFunc) = noteOut with
{
  noteOut = synthFunc(freq),synthFunc(freq + 0.5);
};

// TONIC PITCH WARP
Soni_MaxWarpFactor = 10;
Soni_FreqWarpFactor = _* pow(Soni_MaxWarpFactor,(2 * (PARAM_VAL_PITCH - 0.5)));

// CHANNEL PROCESSORS - MONO AND STEREO
monoChannel(trackIndex) = *(0.25) : channelComp(sliderGroup_channelComp(trackIndex)) : parametricEQ(sliderGroup_channelEQ(trackIndex));
stereoChannel(trackIndex) = stereoLinGain(0.25) : stereoEffect(channelComp(sliderGroup_channelComp(trackIndex))) : stereoEffect(parametricEQ(sliderGroup_channelEQ(trackIndex)));

// GET STEREO PAN GAINS
getPanFunction(panIndex) = panFunction	with	
{
  panFunction = _ <: _*(L_multiplier), _*(R_multiplier);																			// SPLIT AND MULTIPLY
  L_multiplier = 1,0.95,0.3,0.9,0.8,0.6,0.4 : ba.selectn(NUM_PANPOS,panIndex);														// L-MULTIPLIER
  R_multiplier = 1,0.3,0.95,0.4,0.6,0.8,0.9 : ba.selectn(NUM_PANPOS,panIndex);														// R-MULTIPLIER											
};

// CHANNEL COMP
channelComp(sliderGroup_channelComp) = compLimiter(0,ratio,thresh,att,rel,0.050)	 with	{
  ratio = sliderGroup_channelComp(hslider("Ratio[style:knob]",1,1,10,0.1));
  thresh = sliderGroup_channelComp(hslider("Threshold[style:knob]",0,-96,0,0.1));
  att = sliderGroup_channelComp(hslider("Attack[style:knob]",0.1,0.001,0.5,0.001));
  rel = sliderGroup_channelComp(hslider("Release[style:knob]",0.1,0.001,0.5,0.001));		};

// CHANNEL EQ
parametricEQ(sliderGroup_channelEQ) = fi.resonhp(fc_HP,Q_HP,1) : fi.peak_eq_cq(gain_P1,f_P1,Q_P1) : fi.peak_eq_cq(gain_P2,f_P2,Q_P2) : fi.resonlp(fc_LP,Q_LP,1)
  with	{ hpGroup(x) = hgroup("1_HPF",x);
  p1Group(x) = hgroup("2_Parametric_1",x);
  p2Group(x) = hgroup("3_Parametric_2",x);
  lpGroup(x) = hgroup("4_LPF",x);
  fc_HP = sliderGroup_channelEQ(hpGroup(hslider("Freq[style:knob]",50,50,1000,0.1)));
  Q_HP = sliderGroup_channelEQ(hpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  fc_LP = sliderGroup_channelEQ(lpGroup(hslider("Freq[style:knob]",20000,50,20000,0.1)));
  Q_LP = sliderGroup_channelEQ(lpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  gain_P1 = sliderGroup_channelEQ(p1Group(hslider("Gain[style:knob]",0,-24,24,0.01)));
  f_P1 = sliderGroup_channelEQ(p1Group(hslider("Freq[style:knob]",500,100,2000,0.01)));
  Q_P1 = sliderGroup_channelEQ(p1Group(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  gain_P2 = sliderGroup_channelEQ(p2Group(hslider("Gain[style:knob]",0,-24,24,0.01)));
  f_P2 = sliderGroup_channelEQ(p2Group(hslider("Freq[style:knob]",2000,1500,15000,0.01)));
  Q_P2 = sliderGroup_channelEQ(p2Group(hslider("Q[style:knob]",0.7,0.5,10,0.01))); };

// Compressor-Limiter
compLimiter(inputGain_dB,ratio,thresh,att,rel,kneeAtt) = _*inputGain_lin 
  <: _*(an.amp_follower_ud(att,rel) : ba.linear2db : outminusindb : kneesmooth : ba.db2linear)
with{inputGain_lin = ba.db2linear(inputGain_dB);
	 outminusindb(level) = max(level-thresh,0)*(1/ratio - 1);													// CALCULATE GAIN REDUCTION
	 kneesmooth = si.smooth(ba.tau2pole(kneeAtt));	};															// SMOOTH KNEE

// Master COMP
masterComp = stereoEffect(compLimiter(0,1.01,-20,100,300,0.05)) : stereoEffect(compLimiter(0,1.1,-5,50,200,0.05)) : stereodBGain(2);

// STEREO SECTION IN MASTER MIXER
stereoMasterSection(trackIndex) = stereoOut
  with
{
  stereoOut = stereoLinGain(trackGain) : stereoLinGain(1-trackMute);
  trackGain = sliderGroup_TrackGain(hslider("Track %trackIndex",0,-96,24,0.01) : ba.db2linear);
  trackMute = sliderGroup_TrackMute(checkbox("%trackIndex"));
};

// MASTER LIMITER
masterLimiter(ipGaindB) = _ : compLimiter(ipGaindB,10,0,0.001,0.05,0.050);

reverbMaster = _,_ <: *(reverbOn),*(reverbOn) : re.zita_rev1_stereo(REV_MST_PDEL_MS,REV_MST_F_DC_MID,REV_MST_F_MID_HI,REV_MST_T60_DC,rt_60,REV_MST_FsMax) : filter
with {
  rt_60 = 4;
LP_CUT = REV_MST_LPF_FC * (0.1 + 0.9 * PARAM_VAL_PROXIMITY) : si.smooth(ba.tau2pole(0.001));
filter = stereoEffect(fi.bandpass(1,REV_MST_HPF_FC,LP_CUT)) : stereodBGain(12 * sqrt(1 - PARAM_VAL_PROXIMITY));	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

detuneFreq(inputFreq) = inputFreq * soniVibratoLFO
with
{
  vibLFO = os.osc(8);
  soniVibratoLFO = 1 + PARAM_VAL_DETUNE * vibLFO * 0.5 : si.smoo;								// CF DIST SONI - VIBRATO LFO
};

// leadSynth(fundamental,synthFunc,velocity,trigger,synthRelease,synthCutoff,acc) = output

fmSynth(fundamental,numMod,freqFactor,release,depth,trigger) = (fmSynth + dirtyBus) * env 	with
{ 
  fmSynth = sy.fm(freqList,depthList);																			// SYNTH - FM 
  dirtyBus = fmSynth : ef.cubicnl(0.7,0) : _ *(FM_DISTMIX);														// DIST MIX
  freqList = par(i,numMod,fundamental * pow(freqFactor,i));														// (1)CARRIER + MOD FREQ LIST
  depthList = par(i,numMod-1,depthCooked);																		// MOD DEPTH LIST
  depthCooked = depth * env * 9;																				// COOKED DEPTH
  env = sqrt(en.ar(0.001,release, trigger)) : si.smoo;					// AMP ENVELOPE
};

pulseWave(freq,widthPercent) = output with
{
  output = 2 * (ba.pulsen(duration,interval) - 0.5);															// DC CORRECTED OUTPUT WAVE
  duration = widthPercent * interval / 100;																		// PULSE UP TIME
  interval = ma.SR / freq;																						// PUlSE TOTAL PERIOD
};

pianoSim_singleNote(freq,trigger,acc) = monoOut
  with
{
  monoOut = pulseWave(freq,PIANO_WAVEWIDTH1),pulseWave(freq,
			PIANO_WAVEWIDTH2),pulseWave(freq,PIANO_WAVEWIDTH3):> fi.lowpass(2,cutoff) * ampEnv;							// WAVESUMMING
  cutoff = (freqEnv + 0.01) * 7000 * freq / 600 * (1 - min(freq,1000)/2000) : limit(20,20000);							// FC
  freqEnv = (1 + (acc - 5.0)/5.0) * en.arfe(0.001,1.6,0.4 * 1,trigger) : si.smooth(ba.tau2pole(0.01));	// FREQUENCY ENV
  ampEnv = pow(en.ar(0.001,4 * 1,trigger),6)  : si.smooth(ba.tau2pole(0.002));							// AMPLITUDE ENV
};

voiceSynth_FormantBP(freq,vel,trigger,acc) = pm.SFFormantModelBP(1,vowel_H,PARAM_VAL_VOICEFRIC,freqFinal,0.4) * env : fi.resonlp(8000,3,1) with
{
  	trig_unFilt = SONI_3_MelodyTr : ba.selectn(4,0);
	vib = (1 + 0.01 * os.osc(6));
	freqFinal = 2 * freq : _* vib * soniVibratoLFO : si.smooth(ba.tau2pole(0.02));
  	soniVibratoLFO = 1 + PARAM_VAL_DETUNE * vibLFO * 0.5 : si.smoo;								// CF DIST SONI - VIBRATO LFO
  	vowel_idx = PARAM_VAL_VOWEL;
	env = vib * (trig_unFilt > 50) : si.smooth(ba.tau2pole(0.02));
  	vowel_H = vowel_idx;
  	vibLFO = os.osc(8);
};

fluteSimple(f_Hz,gain,mouthPos,vib) = pm.fluteModel(tubeLength,mouthPos,blow) * gain * gain
with{
  	gate = (f_Hz > 430) : si.smooth(ba.tau2pole(0.02));
    envelope = gate*gain : si.smooth(ba.tau2pole(0.001));
  	freqDiff = f_Hz - 440;
	freqFactor = 0.999999999999 + freqDiff * 0.0080951 - freqDiff*freqDiff * 0.00002777 + freqDiff*freqDiff*freqDiff * 0.00000004097;
    tubeLength = 440 + (f_Hz - 440) * freqFactor : pm.f2l;
    pressure = envelope;
    vibFreq = 5 * (1 + 0.4 * ((vib - 0.036)/ 0.036));
    fCut_Hz = 1200 + gain * 2500;
    blow = pm.blower(pressure,0.05,fCut_Hz,vibFreq,vib);
};

fullChordSynth(freqList,synthFunc,env) = stereoChordOut with
{ 
  freqSelector(n) = freqList : ba.selectn(4,n-1);																			// INDIVIDUAL FREQS
  strumDelay(dMax) = dMax * (tempo < 120) * (120 - tempo)/60;																// CALCULATE DELAY
  freq1Bus = synthFunc(freqSelector(1)),synthFunc(freqSelector(1) + FREQ_OFFSET_LR);										// F1
  freq2Bus = synthFunc(freqSelector(2)),synthFunc(freqSelector(2) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N1));	// F2	
  freq3Bus = synthFunc(freqSelector(3)),synthFunc(freqSelector(3) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N2)); 	// F3
  freq4Bus = synthFunc(freqSelector(4)),synthFunc(freqSelector(4) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N3)); 	// F4
  stereoChordOut = freq1Bus,freq2Bus,freq3Bus,freq4Bus :> stereoLinGain(env);												// SUM + ENVELOPE
};

guitarNuts = pm.lTermination(pm.bridgeFilter(0.8,0.6)*-1,pm.basicBlock);
guitarBridge = pm.rTermination(pm.basicBlock,pm.bridgeFilter(0.8,0.6)*-1);

guitarString(freq,stiffness,trigger) = pm.endChain(egChain) * 1.5 : fi.resonlp(PARAM_VAL_GTRDYN,0.7,1) * (1 + (PARAM_VAL_GTRDYN - 500)/15000.0)
with{
  	length = pm.speedOfSound / freq;
    maxStringLength = pm.maxLength;
    lengthTuning = 0.11;
    stringL = length-lengthTuning;
    egChain = pm.chain(guitarNuts : pm.openStringPick(stringL,stiffness,0.2,trigger) : guitarBridge);
};

///////////////////////////////////////////////////////////////////////////// CONST PARAM DUMP ///////////////////////////////////////////////////////////////////////////////////////////////////

PARAM_VAL_PROXIMITY = 1;

// Panner
NUM_PANPOS = 7;

// MALLET FM
MALLET_MRATIO = 2;		MALLET_I_FIXED = 2.4;	MALLET_I_ENV = 6.86;	MALLET_A = 0.001;	MALLET_D = 0.001;	MALLET_S = 0.5;		MALLET_R = 0.4;			MALLET_ENVTYPE = 0;

// PIANO SIM
PIANO_WAVEWIDTH1 = 33;				PIANO_WAVEWIDTH2 = 10;					PIANO_WAVEWIDTH3 = 66;

// 5 - BASSLINE
AT_R = 0.001;			RL_R = 3;
MOD_NUM_R = 5;			FREQ_FACTOR_R = 3;		MOD_DEPTH_R = 35;

FM_DISTMIX = 0.05;

// 6 - MAIN MELODY
AT_M = 0.001;			RL_M = 4;
FB_DEL_M = 0.5;							WET_DEL_M = 0.8;
FC_LP_M = 20000;						G_SIN_M = -90;				G_NS_M = -90;				RL_NS_M = 0.05;			FC_HP_NS_M = 3000;

// BUSS REVERB
REV_MST_PDEL_MS = 10;												// PRE-DELAY
REV_MST_F_DC_MID = 100;												// DC/MID CROSSOVER FREQ
REV_MST_F_MID_HI = 2000;											// MID/HI CROSSOVER FREQ
REV_MST_T60_DC = 1;													// RT60 - DC
REV_MST_FsMax = 48000;												// FS MAX
REV_MST_HPF_FC = 200;												// FC - LOWER
REV_MST_LPF_FC = 3500;												// FC - UPPER