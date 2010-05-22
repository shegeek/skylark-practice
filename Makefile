
all: practice

practice: boomsketch.c boomerangs.h
	gcc -o practice boomsketch.c -lglut

