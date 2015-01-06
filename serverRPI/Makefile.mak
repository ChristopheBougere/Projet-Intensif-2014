CC = g++
CFLAGS = -Wall
EXEC_NAME = serverRPI
INCLUDES =
LIBS =
OBJ_FILES = main.o Utilities/kernel.o Utilities/observable.o Utilities/observer.o Modules/falldown.o Manager/kernelmanager.o
INSTALL_DIR = ./

all : $(EXEC_NAME)

clean :
  rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
  $(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
  $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
  $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
  gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
  cp $(EXEC_NAME) $(INSTALL_DIR)