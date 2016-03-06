CC=gcc
CFLAGS=-lsndfile

musicmake: musicplayer.c 
	$(CC) -o musicplayer musicplayer.c $(CFLAGS)
