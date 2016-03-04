// Music Player - Anfractuosity
// https://github.com/anfractuosity/musicplayer 
//
// based on the work from
// https://github.com/fulldecent/system-bus-radio Copyright 2016 William Entriken

#include <stdio.h>
#include <emmintrin.h>
//#include <mach/mach_traps.h>
//#include <mach/mach_time.h>
#include <math.h>
#include<stdint.h>
#include<time.h>
#include<math.h>
#include<sndfile.h>

__m128i reg;
__m128i reg_zero;
__m128i reg_one;

#define NSEC_PER_SEC	1000000000	/* nanoseconds per second */

/* Include this header file to use functions from libsndfile. */
#define		BUFFER_LEN	1024*1024*500


uint64_t mach_absolute_time(){
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */

	return (uint64_t)start.tv_sec * NSEC_PER_SEC + start.tv_nsec;
}

unsigned int * pdm2(unsigned int *x,int len){
	unsigned *y=malloc(len*sizeof(int));
	unsigned int n = 0; 
 	unsigned int quant_error = 0;
 
	for (n=0;n<len;n++){
		quant_error += x[n];
        	if (quant_error >= 65535) {
                	quant_error -= 65535;
                	y[n] = 1;
			continue;
        	}
        	y[n] = 0;
   	}

	return y;
}

static inline void pdm_signal(int *pdm,int len) {
	int i = 0;
	for (i=0;i<len;i++){
		double time = 1.0 / 1000000.0;
		uint64_t start = mach_absolute_time();
		uint64_t end = start + time * NSEC_PER_SEC; 

		if(pdm[i] == 1)
			while (mach_absolute_time() < end) {
				_mm_stream_si128(&reg, reg_one);
	     			_mm_stream_si128(&reg, reg_zero);
			}
		else
			while (mach_absolute_time() < end) {

			}
	}
}


// From http://beej.us/blog/data/digital-sound/
int remap_level_to_signed_16_bit(float v)
{
	long r;
	// clamp values:
	if (v > 1.0) { v = 1.0; }
	else if (v < -1.0) { v = -1.0; }
	v += 1.0;      // remap from [-1.0,1.0] to [0.0,2.0]
	v /= 2;        // remap from [0.0,2.0] to [0.0,1.0]
    	r = v * 65535; // remap from [0.0,1.0] to [0,65535]
    	return r;
}

	
int main ( int argc, char **argv ) {

	if(argc != 2){
		printf("Need to pass audio file sampled at 1 000 000 Hz\n");
		return 1;
	}else{
		printf("Attempting to load file: %s\n",argv[1]);
	}
	
	double *data = malloc(BUFFER_LEN*sizeof(double)) ;
	SNDFILE         *infile;
	SF_INFO		sfinfo ;
	int		readcount ;

	if (! (infile = sf_open (argv[1], SFM_READ, &sfinfo))){   
		/* Open failed so print an error message. */
        	printf ("Not able to open input file %s.\n", argv[1]) ;
        	/* Print the error message from libsndfile. */
        	puts (sf_strerror (NULL)) ;
        	return  1 ;
	} 

	if (sfinfo.channels > 1){
		printf ("Not able to process more than 1 channels\n") ;
        	return  1 ;
        } 

	unsigned int c;
   	readcount = sf_read_double (infile, data, BUFFER_LEN);
	unsigned int *nout = malloc(sizeof(int)*readcount);
	for(c=0;c<readcount;c++){
		nout[c] = remap_level_to_signed_16_bit(data[c]);
	}

	unsigned int* out = pdm2(nout,readcount);
	
	printf("Out>>>>>>>>\n");
	pdm_signal(out,readcount);

	exit(0);
}
