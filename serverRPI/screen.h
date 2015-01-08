#include "Web/serverConnector.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "geniePi.h"

#define OBJ_H 0
#define OBJ_M 1

char* getDateStr(struct tm *t);
//static void *clock_thread(void *data);
void screen(void);
void notify_med(char *msg);
void notify_drawer(char *msg);
