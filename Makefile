
#This is a template to build your own project with the e-puck2_main-processor folder as a library.
#Simply adapt the lines below to be able to compile

# Define project name here
PROJECT = MazeRunner

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = e-puck2_main-processor

#Source files to include
CSRC += ./Src/main.c \
		./Src/audio_processing.c \
		./Src/fft.c \
		./Src/motor_control.c \
		./Src/proximity_control.c \
		
#Header folders to include
INCDIR += ./Inc/

#Jump to the main Makefile
include $(GLOBAL_PATH)/Makefile
