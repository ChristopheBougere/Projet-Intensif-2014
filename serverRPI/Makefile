CC = g++
CFLAGS = -Wall -std=c++0x
EXEC_NAME = serverRPI
INCLUDES =
LIBS = -Llibs -lgeniePi -lwiringPi -lpthread -lcurl
OBJ_FILES = main.o screen.o Utilities/kernel.o Utilities/observable.o Utilities/observer.o Modules/falldown.o \
            Modules/MedDistrib/meddistrib.o Modules/MedDistrib/posology.o Modules/MedDistrib/posologystruct.o \
            Manager/kernelmanager.o libs/tinyxml2.o Utilities/config.o \
            Web/serverConnector.o Utilities/timeu.o screenmanager.o Utilities/alertlevel.o
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
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
