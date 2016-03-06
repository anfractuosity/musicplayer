# musicplayer

Play .wav audio file via system bus 

Based on the work of https://github.com/fulldecent/system-bus-radio

You simply use sox to convert your audio to a high sample rate such as 1MHz, as seen below (This
high rate is necessary for PDM).

```
gcc musicplayer.c -lsndfile -o musicplayer
sox in.wav -r 1000000 out.wav
./musicplayer out.wav
```
##Video

https://youtu.be/xSj5skknXWg


###Requirements

* Sox
* libsndfile - http://www.mega-nerd.com/libsndfile/
