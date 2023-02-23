#
# Makefile for saucer shoot game using Dragonfly
#
# Copyright Mark Claypool and WPI, 2016-2023
#
# 'make' to build executable
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#   EXECUTABLE is the name of the runnable game
#   ENG is the name of the Dragonfly engine
#

# Compiler.
CC= g++ 

# Libraries and includes.
LINKDIR= -L../dragonfly/lib # path to dragonfly library
INCDIR= -I../dragonfly/include # path to dragonfly includes

### Uncomment and update below if using local SFML installation.
LOCALSFML= $(HOME)/src/SFML-2.5.1
LINKDIR:= $(LINKDIR) -L $(LOCALSFML)/lib
INCDIR:= $(INCDIR) -I$(LOCALSFML)/include

### Uncomment either 1) or 2) below! ###

## 1) For Linux:
CFLAGS= -Wall
LINKLIB= -l$(ENG) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lrt
ENG= dragonfly-x64-linux

## 2) For Mac:
# Note: for homebrew, sfml may be in: /usr/local/Cellar/sfml/2.5.1
#LOCALSFML= $(HOME)/src/SFML-2.5.1
#LINKDIR:= $(LINKDIR) -L $(LOCALSFML)/lib
#CFLAGS= -MD
#LINKLIB= -l$(ENG) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
## And uncomment ONE of two engine below based on the CPU arch:
#ENG= dragonfly-x64-mac
#ENG= dragonfly-arm64-mac

######

GAMESRC= \
         Bullet.cpp \
         EventNuke.cpp \
         Explosion.cpp \
         GameOver.cpp \
         GameStart.cpp \
         Hero.cpp \
         Points.cpp \
         Reticle.cpp \
         Saucer.cpp \
         Star.cpp \

GAME= game.cpp
EXECUTABLE= game
OBJECTS= $(GAMESRC:.cpp=.o)
CFLAGS:= $(CFLAGS) -std=c++11

.PHONY: all clean

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(OBJECTS) $(GAME) $(GAMESRC) 
	$(CC) $(CFLAGS) $(GAME) $(OBJECTS) -o $@ $(INCDIR) $(LINKDIR) $(LINKLIB) 

.cpp.o: 
	$(CC) $(CFLAGS) -c $< -o $@ $(INCDIR)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~

# DO NOT DELETE
