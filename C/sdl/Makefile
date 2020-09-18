ROOT_PATH := /data/nfs/HEVC3531D_TOOLCHAIN/ema
SDL_PATH := /data/work/tool/packet/arm-hisiv600/SDL1.2/



CC     := arm-hisiv600-linux-gcc
CFLAG  := -I${SDL_PATH}/include
LDFLAG := -L${SDL_PATH}/lib -lSDL -lSDL_image
TARGET := sdl_test

${TARGET}: sdl_jpeg.c
	${CC}  ${CFLAG} ${LDFLAG} $<  -o $@   

clean:
	rm *.o ${TARGET} 
