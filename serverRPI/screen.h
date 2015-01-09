#ifndef SCREEN_H
#define SCREEN_H

#include "Web/serverConnector.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "geniePi.h"

class ScreenManager;

#define OBJ_H 0
#define OBJ_M 1

void setScreenManager(ScreenManager* manager);
char* getDateStr(struct tm *t);
//static void *clock_thread(void *data);
void screen(void);
void notify_med(char *msg);
void notify_drawer(char *msg);
void notify_fall(char *msg);
int getHasPushedYes();

#endif
