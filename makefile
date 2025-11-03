CC = clang
LIBS_SDL2 = `sdl2-config --cflags --libs`
LIBS = -lm

ALL : visualizer 

visualizer : visualizer.o complex.o write_movie.o
	$(CC) $(LIBS_SDL2) visualizer.o complex.o write_movie.o -o visualizer $(LIBS)
	find -name '*.o' |xargs rm


write_movie.o : write_movie.c 
	$(CC) -c write_movie.c $(LIBS)


visualizer.o : visualizer.c
	$(CC) $(LIBS_SDL2) -c visualizer.c $(LIBS)

complex.o : complex.c
	$(CC) -c complex.c $(LIBS)



.PHONY : clean






clean :
	rm visualizer
	find -name '*.o' |xargs rm
