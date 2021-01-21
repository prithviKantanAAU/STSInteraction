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

// Sonification Sliders and Preprocessing
soniSlider(idx,defaultVal) = tab_SoniControl(vgroup("FV_Values",hslider("Soni %idx",defaultVal,0,2000,0.001)));

// Single Horizontal Row of Soni Sliders for each feedback variable
array_soniSliders(idx,defaultVals) = finalGroup with
{
  finalGroup = tab_SoniControl(hgroup("Soni %idx",par(i,4,configuredSlider(i))));
  configuredSlider(x) = hslider("Control %x",defaultVal(x),0,2000,0.001);
  defaultVal(x) = defaultVals : ba.selectn(4,x);
};

// Definition of Sonification UI and Variables
SONI_1_DefaultVals = 0,0,0,0;
SONI_1_PercTr = array_soniSliders(1,SONI_1_DefaultVals);
SONI_2_DefaultVals = 440,0,0,0;
SONI_2_MelodyTrFr = array_soniSliders(2,SONI_2_DefaultVals);
SONI_3_DefaultVals = 440,440,440,440;
SONI_3_ChordTrFr = array_soniSliders(3,SONI_3_DefaultVals);
SONI_4_DefaultVals = 0,0,0,0;
SONI_4_Detune = array_soniSliders(4,SONI_4_DefaultVals);
SONI_5_DefaultVals = 0.5,0,0,0;
SONI_5_Pan = array_soniSliders(5,SONI_5_DefaultVals);
SONI_6_DefaultVals = 0,0,0,0;
SONI_6_Perc2Tr = array_soniSliders(6,SONI_6_DefaultVals);
SONI_7_DefaultVals = 0,0,0,0;
SONI_7_BassTrFr = array_soniSliders(7,SONI_7_DefaultVals);
SONI_8_DefaultVals = 7,0,0,0;
SONI_8_Dynamics = array_soniSliders(8,SONI_8_DefaultVals);
SONI_9_DefaultVals = 0.5,0,0,0;
SONI_9_Pitch = array_soniSliders(9,SONI_9_DefaultVals);

// TRIGGERS
TRG_PERC_MAIN = FVToTrigger(SONI_1_PercTr : ba.selectn(4,0));		// TRACK 1

FRQ_MEL = SONI_2_MelodyTrFr : ba.selectn(4,0);						// TRACK 2
TRG_MEL = FVToTrigger(FRQ_MEL);										

FRQ_CHORD_N1 = SONI_3_ChordTrFr : ba.selectn(4,0);					// TRACK 3
TRG_CHORD_N1 = FVToTrigger(FRQ_CHORD_N1);		
FRQ_CHORD_N2 = SONI_3_ChordTrFr : ba.selectn(4,1);
TRG_CHORD_N2 = FVToTrigger(FRQ_CHORD_N2);
FRQ_CHORD_N3 = SONI_3_ChordTrFr : ba.selectn(4,2);
TRG_CHORD_N3 = FVToTrigger(FRQ_CHORD_N3);
FRQ_CHORD_N4 = SONI_3_ChordTrFr : ba.selectn(4,3);
TRG_CHORD_N4 = FVToTrigger(FRQ_CHORD_N4);
TRG_LIST_CHORD = TRG_CHORD_N1,TRG_CHORD_N2,TRG_CHORD_N3,TRG_CHORD_N4;

TRG_PERC_2 = FVToTrigger(SONI_6_Perc2Tr : ba.selectn(4,0));			// TRACK 4

FRQ_BASS = SONI_7_BassTrFr: ba.selectn(4,0);						// TRACK 5
TRG_BASS = FVToTrigger(FRQ_BASS);

PARAM_VAL_DETUNE = SONI_4_Detune : ba.selectn(4,0);					// DETUNE
PARAM_VAL_PAN = SONI_5_Pan : ba.selectn(4,0);						// PAN
PARAM_VAL_DYNAMICS = SONI_8_Dynamics : ba.selectn(4,0);				// DYNAMICS
PARAM_VAL_PITCH = SONI_9_Pitch : ba.selectn(4,0);					// PITCH

// TRACK SYNTHESIS DEFINITION  

// TRACK 1 - MAIN PERCUSSION
Synth_T1_MainPerc = 
samplePlayer(PERC_MAIN_SMPL_1,TRG_PERC_MAIN),
samplePlayer(PERC_MAIN_SMPL_2,TRG_PERC_MAIN),
samplePlayer(PERC_MAIN_SMPL_3,TRG_PERC_MAIN),
samplePlayer(PERC_MAIN_SMPL_4,TRG_PERC_MAIN),
samplePlayer(PERC_MAIN_SMPL_5,TRG_PERC_MAIN),
samplePlayer(PERC_MAIN_SMPL_6,TRG_PERC_MAIN) : ba.selectn(6,TRG_PERC_MAIN) : applyVelocity(PARAM_VAL_DYNAMICS,TRG_PERC_MAIN,10) : monoChannel(1) : getPanFunction(0);

// TRACK 2 - MELODY
F0_M = FRQ_MEL : Soni_FreqWarpFactor;
synthFunc_Melody(freq) = fmSynth_Versatile(freq,MALLET_MRATIO,MALLET_I_FIXED,MALLET_I_ENV,MALLET_A,MALLET_D,MALLET_S,MALLET_R,MALLET_ENVTYPE,TRG_MEL,PARAM_VAL_DYNAMICS,PARAM_VAL_DYNAMICS/2.0);
Synth_T2_Melody = leadSynth(F0_M,synthFunc_Melody,PARAM_VAL_DYNAMICS,TRG_MEL,RL_M,FC_LP_M,PARAM_VAL_DYNAMICS/2.0) : stereoChannel(2);
  
// TRACK 3 - CHORD
FREQ_LIST_C = FRQ_CHORD_N1,FRQ_CHORD_N2,FRQ_CHORD_N3,FRQ_CHORD_N4;																// LIST MIDI KEYS
F0_LIST_HZ_C = getChordFinalFreqs(FREQ_LIST_C);																					// MIDI KEYS TO FREQ
chordFreq(noteIdx) = F0_LIST_HZ_C : ba.selectn(4,noteIdx);																		// FREQ SELECTOR
chordTrg(noteIdx) =  TRG_LIST_CHORD	  : ba.selectn(4,noteIdx);																	// TRG SELECTOR
synthFunc_Chord(trigger,freq) = pianoSim_singleNote(freq,trigger,PARAM_VAL_DYNAMICS/2);
chordSum = par(i,4,chordSingle_Synth(chordFreq(i), synthFunc_Chord(chordTrg(i))) : stereoEffect(applyVelocity(PARAM_VAL_DYNAMICS,chordTrg(i),10))) :> _,_;
Synth_T3_Chord = chordSum : stereoChannel(3);

// TRACK 4 - SECONDARY PERCUSSION
Synth_T4_SecPerc = 
samplePlayer(PERC_SEC_SMPL_1,TRG_PERC_2),
samplePlayer(PERC_SEC_SMPL_2,TRG_PERC_2) : ba.selectn(2, PARAM_VAL_DYNAMICS > 8.5) : applyVelocity(PARAM_VAL_DYNAMICS,TRG_PERC_2,10) : monoChannel(4) : getPanFunction(2);

// TRACK 5 - BASSLINE
F0_R = FRQ_BASS : Soni_FreqWarpFactor;
Synth_T5_Bass = fmSynth(F0_R,MOD_NUM_R,FREQ_FACTOR_R,RL_R,MOD_DEPTH_R,TRG_BASS) : applyVelocity(PARAM_VAL_DYNAMICS,TRG_BASS,10) : monoChannel(5) : getPanFunction(0);

track1 = Synth_T1_MainPerc : stereoMasterSection(1);
track2 = Synth_T2_Melody   : stereoMasterSection(2);
track3 = Synth_T3_Chord    : stereoMasterSection(3);
track4 = Synth_T4_SecPerc  : stereoMasterSection(4);
track5 = Synth_T5_Bass     : stereoMasterSection(5);

masterChannel = masterComp : stereoLinGain(masterGain) : stereoEffect(masterLimiter(0)) : stereoEffect(hard_clip(1)) : Soni_PAN;
  
process = track1,track2,track3,track4,track5 :> masterChannel;
//process = track2 :> masterChannel;
							    
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
FVToTrigger(quantInput) = trigger with
{
  trigger = (posTrig + negTrig);
  posTrig = quantInput : ba.impulsify;
  negTrig = - 1 * (quantInput) : ba.impulsify : *(quantInput > 0);
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

// SPATIALIZE
Soni_PAN = _,_ : _*(M_L),_*(M_R) with
{
  M_L = sqrt(1 - PARAM_VAL_PAN) * 1.414;
  M_R = sqrt(PARAM_VAL_PAN)  * 1.414;
};

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Sample Playback
samplePlayer(fileFunc,trigger) = fileFunc(ba.countup(96000,trigger));

PERC_MAIN_FILES = soundfile("PERC_MAIN_SMPL[url:{
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_1.wav'; 
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_2.wav';
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_3.wav';
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_4.wav';
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_5.wav';
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_MAIN_6.wav'
}]",1) : !,!,_;

PERC_SEC_FILES = soundfile("PERC_SEC_SMPL[url:{
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_SEC_1.wav';
'D:\\\\Audio Projects\\\\STSInteraction\\\\WAV SAMPLES\\\\PERC_SEC_2.wav'
}]",1) : !,!,_;

PERC_MAIN_SMPL_1(i) = 0,i : PERC_MAIN_FILES;
PERC_MAIN_SMPL_2(i) = 1,i : PERC_MAIN_FILES;
PERC_MAIN_SMPL_3(i) = 2,i : PERC_MAIN_FILES;
PERC_MAIN_SMPL_4(i) = 3,i : PERC_MAIN_FILES;
PERC_MAIN_SMPL_5(i) = 4,i : PERC_MAIN_FILES;
PERC_MAIN_SMPL_6(i) = 5,i : PERC_MAIN_FILES;

PERC_SEC_SMPL_1(i) = 0,i : PERC_SEC_FILES;
PERC_SEC_SMPL_2(i) = 1,i : PERC_SEC_FILES;

fmSynth_Versatile(fc,modRatio,I_fixed,I_ampDependent,a,d,s,r,envType,trigger,vel,acc) = output
  with
{
  fc_cooked = fc : si.smooth(ba.tau2pole(0.001));
  acc_cooked = 1 + (acc-5.0)/5.0;
  output = os.osc(fc_cooked + dev) * ampEnv;
  dev = I * ampEnv * modFreq * os.triangle(modFreq);
  I = I_fixed + (I_ampDependent * acc_cooked * (0.5 + velFactor) + I_freq) * (ampEnv);
  I_freq = 4 * (fc - 300)/300;
  modFreq = (modRatio + PARAM_VAL_DETUNE) * fc_cooked;
  release_basic = 2;
  env_basic = en.ar(a,release_basic,trigger);
  triggerCooked = (env_basic > 0.5) + (env_basic > env_basic');
  chosenEnv = en.adsr(a,d,s,r*1,triggerCooked); 
  ampEnv = chosenEnv : si.smooth(ba.tau2pole(a));
  velFactor = 1 : applyVelocity(vel,trigger,10);
};

leadSynth(fundamental,synthFunc,velocity,trigger,synthRelease,synthCutoff,acc) = output
with
{																			
  output = melSynth,melSynth;																					// OUTPUT SUMMING
  melSynth = synthFunc(fundamentalCooked) * env : applyVelocity(velocity,trigger,9);							// MEL COMPONENT
  fundamentalCooked = 2 * fundamental * soniVibratoLFO : limit(20,5000);
  env = en.ar(0.001,synthRelease * 1,trigger);													// MEL ENVELOPE
  vibLFO = os.osc(8);
  soniVibratoLFO = 1 + PARAM_VAL_DETUNE * vibLFO * 0.5 : si.smoo;								// CF DIST SONI - VIBRATO LFO
};

fmSynth(fundamental,numMod,freqFactor,release,depth,trigger) = (fmSynth + dirtyBus) * env 	with
{ 
  fmSynth = sy.fm(freqList,depthList);																			// SYNTH - FM 
  dirtyBus = fmSynth : ef.cubicnl(0.7,0) : _ *(FM_DISTMIX);														// DIST MIX
  freqList = par(i,numMod,fundamental * pow(freqFactor,i));														// (1)CARRIER + MOD FREQ LIST
  depthList = par(i,numMod-1,depthCooked);																		// MOD DEPTH LIST
  depthCooked = depth * env * 9;																				// COOKED DEPTH
  env = sqrt(en.ar(0.001,release * 1, trigger)) : si.smooth(ba.tau2pole(0.001));					// AMP ENVELOPE
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
  cutoff = (freqEnv + 0.01) * 4000 * freq / 600 * (1 - min(freq,1000)/2000) : limit(20,20000);							// FC
  freqEnv = (1 + (acc - 5.0)/5.0) * en.arfe(0.001,1.6,0.4 * 1,trigger) : si.smooth(ba.tau2pole(0.0001));	// FREQUENCY ENV
  ampEnv = pow(en.ar(0.001,4 * 1,trigger),6)  : si.smooth(ba.tau2pole(0.0001));							// AMPLITUDE ENV
};

voiceSynth_FormantBP(freq,vel,trigger,acc) = pm.SFFormantModelBP(2,vowel_H,fric,freqLow,0.04) * env : fi.resonlp(8000,3,1) with
{
  	fric = 0.13 - acc/10.0 * 0.13 : max(0);
	freqLow = freq / 2.0;
  	//vowel_idx = _~+(trigger) : %(4) : _ + 0.2;
  	vowel_idx = Soni_X_D1_Vowel;
	env = en.ar(0.02, 1.5  / tempo * 78.6 * 1 * (1 + acc/5.0), trigger);
  	vowel_H = vowel_idx : si.smooth(ba.tau2pole(0.01));
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



///////////////////////////////////////////////////////////////////////////// CONST PARAM DUMP ///////////////////////////////////////////////////////////////////////////////////////////////////

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