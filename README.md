# musicplayer

Play .wav audio file via system bus 

Based on the work of https://github.com/fulldecent/system-bus-radio

At the moment this program requires the audio passed to it to be sampled at
1 000 000 Hz, you simply use sox to convert your audio to this rate, as seen below (This
high rate is necessary for PDM).

```
gcc musicplayer.c -lm -lsndfile -o musicplayer
sox in.wav -r 1000000 out.wav
./musicplayer out.wav
```

Please can someone test this, I pick up a lot of noise, but i'm pretty sure I can hear Taylor Swift through my radio

###Requirements

* Sox
* libsndfile - http://www.mega-nerd.com/libsndfile/
