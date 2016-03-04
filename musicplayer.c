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
__m128i reg;
__m128i reg_zero;
__m128i reg_one;
//mach_port_t clock_port;
//mach_timespec_t remain;

#define NSEC_PER_SEC	1000000000	/* nanoseconds per second */



/* Include this header file to use functions from libsndfile. */
#include	<sndfile.h>

/*    This will be the length of the buffer used to hold.frames while
**    we process them.
*/
#define		BUFFER_LEN	1024*1024*500

/* libsndfile can handle more than 6 channels but we'll restrict it to 6. */
#define		MAX_CHANNELS	6


/* Function prototype. */
static void process_data (double *data, int count, int channels) ;



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
	uint64_t period = NSEC_PER_SEC / (22000);
	int i = 0;
	for (i=0;i<len;i++){
		double time = 1.0 / 1000000.0;
		uint64_t start = mach_absolute_time();
		uint64_t end = start + time * NSEC_PER_SEC; //0.00004545454 * NSEC_PER_SEC;

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

int remap_level_to_signed_16_bit(float v)
{
    long r;

    // clamp values:
    if (v > 1.0) { v = 1.0; }
    else if (v < -1.0) { v = -1.0; }

    v += 1.0;      // remap from [-1.0,1.0] to [0.0,2.0]
    v /= 2;        // remap from [0.0,2.0] to [0.0,1.0]
    //r = v * 65535; // remap from [0.0,1.0] to [0,65535]
     r = v * 65535;
    return r;
    //return r - 32768;  // remap from [0,65535] to [-32768,32767]
}







int main()

{


  /* This is a buffer of double precision floating point values
    ** which will hold our data while we process it.
    */
    double *data = malloc(BUFFER_LEN*sizeof(double)) ;

    /* A SNDFILE is very much like a FILE in the Standard C library. The
    ** sf_open function return an SNDFILE* pointer when they sucessfully
	** open the specified file.
    */
    SNDFILE      *infile, *outfile ;

    /* A pointer to an SF_INFO stutct is passed to sf_open.
    ** On read, the library fills this struct with information about the file.
    ** On write, the struct must be filled in before calling sf_open.
    */
    SF_INFO		sfinfo ;
    int			readcount ;
    const char	*infilename = "Taylor2.wav" ;

    if (! (infile = sf_open (infilename, SFM_READ, &sfinfo)))
    {   /* Open failed so print an error message. */
        printf ("Not able to open input file %s.\n", infilename) ;
        /* Print the error message from libsndfile. */
        puts (sf_strerror (NULL)) ;
        return  1 ;
        } ;

    if (sfinfo.channels > MAX_CHANNELS)
    {   printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
        return  1 ;
        } ;
;
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
