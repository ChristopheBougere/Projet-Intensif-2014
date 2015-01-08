#include "screen.h"

void notify_med() {
	printf("Il faut prendre les medicaments\n");
	// Changement d'écran
	genieWriteObj(GENIE_OBJ_FORM, 1, 0);
}

void notify_drawer() {
	printf("Le tiroir a été fermé.");
	genieWriteObj(GENIE_OBJ_FORM, 0, 0);	
}

char* getDateStr(struct tm *t) {
	int wday = t->tm_wday;
	int mday = t->tm_mday;
	int mon = t->tm_mon;
	char *jour_semaine;
	switch(wday) {
		case 0: jour_semaine = "Dimanche"; break;
		case 1: jour_semaine = "Lundi"; break;
		case 2: jour_semaine = "Mardi"; break;
                case 3: jour_semaine = "Mercredi"; break;
                case 4: jour_semaine = "Jeudi"; break;
                case 5: jour_semaine = "Vendredi"; break;
                case 6: jour_semaine = "Samedi"; break;
		default: jour_semaine = ""; break;
	}
	char *mois;
	switch(mon) {
		case 0: mois = "Janvier"; break;
		case 1: mois = "F�vrier"; break;
                case 2: mois = "Mars"; break;
                case 3: mois = "Avril"; break;
                case 4: mois = "Mai"; break;
                case 5: mois = "Juin"; break;
                case 6: mois = "Juillet"; break;
                case 7: mois = "Ao�t"; break;
                case 8: mois = "Septembre"; break;
                case 9: mois = "Octobre"; break;
                case 10: mois = "Novembre"; break;
                case 11: mois = "Décembre"; break;
                default: mois = ""; break;
	}
	char *ret = (char *) malloc((strlen(jour_semaine) + strlen(mois) + 4) * sizeof(char));
	sprintf(ret, "%s %d %s", jour_semaine, mday, mois);

	return ret;
}

static void *clock_thread(void *data) {
	while(1) {
		time_t ti = time(NULL);
                struct tm *t = localtime(&ti);
                int hour = t->tm_hour;
                int min = t->tm_min;
		genieWriteObj(GENIE_OBJ_LED_DIGITS, OBJ_H, hour);
		genieWriteObj(GENIE_OBJ_LED_DIGITS, OBJ_M, min);
                genieWriteStr(0, getDateStr(t));
		
                
       		usleep(1000000);
	}
	return (void *) NULL;
}

void screen(void) {
	if(genieSetup("/dev/ttyUSB0", 115200) < 0) {
		printf("Impossible d'accéder à l'écran.\n�");
		return;
	}
	printf("Connexion à l'écran... OK\n");
	
	struct genieReplyStruct reply;
	pthread_t myThread;
	pthread_create (&myThread, NULL, &clock_thread, NULL);
	
	for(;;) {
		while(genieReplyAvail()) {
			genieGetReply(&reply);
			printf("Commande recue: %d %d %d %d\n", reply.cmd, reply.object, reply
.index, reply.data);
			// Si c'est le bouton NON
			if(reply.cmd == 7 && reply.object == 6 && reply.index == 3) {
				ServerConnector sc;
				// USER ID A LA MAIN EN ATTENDANT LE NFC
				int user_id = 1;
				int alert_type = 3;
				int alert_level = 2;
				sc.sendAlert(user_id, alert_type, alert_level);
				genieWriteObj(GENIE_OBJ_FORM, 3, 0);
				sleep(60);
				genieWriteObj(GENIE_OBJ_FORM, 0, 0);
			}
		}
		usleep(10000);
	}
}
