# musicplayer
Play .wav audio file via system bus 

Based on the work of https://github.com/fulldecent/system-bus-radio

gcc musicplayer.c -lm -lsndfile -o musicplayer

sox in.wav -r 1000000 out.wav

./musicplayer
