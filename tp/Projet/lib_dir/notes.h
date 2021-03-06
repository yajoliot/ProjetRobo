#ifndef LIB_DIR_NOTES_H
#define LIB_DIR_NOTES_H

//twelve notes per octave except for octave 5 which has 8 notes excluding the last usual 4 starting from and including G#
//since this follows the midi key numbering there is no need for any mapping as it is already done here

const uint16_t note[]={
	61156,57724,54484,51426,48540,45815,43244,40817,38526,36364,34323,32396,	//octave -5
	30578,28862,27242,25713,24270,22908,21622,20408,19263,18182,17161,16198,	//octave -4 
	15289,14431,13621,12856,12135,11454,10811,10204,9631,9091,8581,8099,		//octave -3 
	7645,7215,6810,6428,6067,5727,5405,5102,4816,4545,4290,4050,				//octave -2 
	3822,3608,3405,3214,3034,2863,2703,2551,2408,2273,2145,2025,				//octave -1
	1911,1804,1703,1607,1517,1432,1351,1276,1204,1136,1073,1012,				//octave 0
	956,902,851,804,758,716,676,638,602,568,536,506,							//octave 1
	478,451,426,402,379,358,338,319,301,284,268,253,							//octave 2
	239,225,213,201,190,179,169,159,150,142,134,127,							//octave 3
	119,113,106,100,95,89,84,80,75,71,67,63,									//octave 4
	60,56,53,50,47,45,42,40														//octave 5
};

#endif