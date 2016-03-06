# musicplayer

Play .wav audio file via system bus. 

Based on the work of https://github.com/fulldecent/system-bus-radio

We make use of Pulse Density Modulation (PDM) (see https://en.wikipedia.org/wiki/Pulse-density_modulation for more
information).  The Pulse Code Modulation (PCM) data from a .wav file is converted to a PDM bit stream.  

Each PDM bit, is output at the same sample rate as the audio.  If we see a bit with a value of 1, we make use of the
_mm_stream_si128 instructions, otherwise we simply use a busy-wait loop.  We perform each of these operations for a duration
of 1/samplerate seconds.

You simply use sox to convert your audio to a high sample rate such as 1MHz, as seen below (This
high rate is necessary for PDM).

```
make
sox in.wav -r 1000000 out.wav
./musicplayer out.wav
```

##Video

https://youtu.be/xSj5skknXWg


###Requirements

* Sox
* libsndfile - http://www.mega-nerd.com/libsndfile/
