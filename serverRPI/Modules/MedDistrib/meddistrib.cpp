#include "meddistrib.h"
#include "unistd.h"
#include <pthread.h>

#include <iostream>

#include <ctime>
#include "time.h"
#include "../../Web/serverConnector.h"
#include "../../Utilities/timeu.h"

#define DELAY_BEFORE_ACTION 30 //1200 // = 20min

bool MedDistrib::isDrawerOpen() {
    // Lire la valeur dans /sys/class/gpio/gpio21/value
    // Fermé = 1
    // Ouvert = 0
    string line;
    ifstream file("/sys/class/gpio/gpio21/value");
    char c = '2';
    if(file.is_open()) {
        if(getline(file, line)) {
            std::cout << line << std::endl;
            c = line[0];
        }
        file.close();
    } else {
        std::cout << "Unable to open file /sys/class/gpio/gpio21/value" << std::endl;
    }
    if(c == '0') {
        return true;
    } else if(c == '1') {
        return false;
    } else {
        std::cout << "Impossible de lire la réponse du fichier /sys/class/gpio/gpio21/value" << std::endl;
        return false;
    }

}

MedDistrib::MedDistrib()
{
    std::cout << "MedDistrib() : demarrage du module" << std::endl;

    _user_id = 1; // TODO

    std::thread t_posology(&MedDistrib::getPosology, this);
    t_posology.join();

    // TEST
    Posology *newPoso = new Posology();
    newPoso->setTimeU(15, 07, 00);
    newPoso->addPosology("Adrien", 1);

    Posology *newPoso1 = new Posology();
    newPoso1->setTimeU(15, 55, 00);
    newPoso1->addPosology("Bastien", 4);

    //_posologyList.push_back(*newPoso);
    _posologyList.push_back(*newPoso1);
    
    // END TEST

    if((int) _posologyList.size() != 0)
    {
        for(int c = 0 ; c < (int) _posologyList.size() ; c++)
        {
            Posology actPoso = _posologyList.at(c);
            std::thread t_taken(&MedDistrib::checkPosology, this, &actPoso);
            if(t_taken.joinable())
	    {
		//std::cout << c << std::endl;
		t_taken.join();
		//std::cout << c << "finished" << std::endl;
	    }

            _threadList.push_back(std::move(t_taken));
        }
    }

	//std::cout << "TEST" << std::endl;

    /*for(int c = 0 ; c < _posologyList.size() ; c++)
    {
        _threadList.at(c).detach();
    }*/


    t_posology.detach();

    // On initialise l'ecoute des GPIO
    std::system("[ -d /sys/class/gpio/gpio21 ] || echo 21 > /sys/class/gpio/export");
    std::system("echo out > /sys/class/gpio/gpio21/direction");
    std::system("echo 0 > /sys/class/gpio/gpio21/value");
}

std::string MedDistrib::Statut() const
{
    std::string res = "";
    for(int c = 0 ; c < (int) _posologyList.size() ; c++)
    {
        Posology actPoso = _posologyList.at(c);
        AlertLevel tmpAlert = actPoso.getAlert();
        if(tmpAlert.getAlert())
        {
            res = actPoso.getStringPosology();
            break;
        }

    }

    return res;
}

void MedDistrib::addPosology(std::string name, int quantity, int hour, int minutes, int seconds)
{
    Posology newPoso;
    newPoso.setTimeU(hour, minutes, seconds);
    newPoso.addPosology(name, quantity);

    _posologyList.push_back(newPoso);

}

void MedDistrib::checkPosology(Posology *poso)
{
    int i = 0;
    ServerConnector sc;

    /*rapidjson::Document doc;
    sc.registerUser(doc);

    int user_id = doc["user_id"].GetInt();*/

    while(i != -1)
    {

        TimeU timePoso = poso->getTimeU();
        TimeU *timeNow = new TimeU();
        timeNow->setRealTime();

        int secPoso = timePoso.calculateSec();
        int secNow = timeNow->calculateSec();

        int total = secPoso - secNow;
        std::cout << total << std::endl;
	std::cout << secPoso << std::endl;
	std::cout << secNow << std::endl;

        AlertLevel tmpAlert = poso->getAlert();

        if( (secPoso - secNow) > 0) // Pas l'heure de prendre
        {
            std::cout << "MedDistrib() : aucune action a effectuer" << std::endl;
            sleep(secPoso - secNow);
        } else if ((secPoso - secNow) <= 0 && (secPoso - secNow) >= - DELAY_BEFORE_ACTION)
        {

            // TODO : check si médicaments pris : 1ere fois = non donc alert
            if(false) // Si medicaments prit
            {
                Notify();
            } else {
                tmpAlert.updateCriticity();
                Notify();
                sc.sendAlert(_user_id, tmpAlert.getType(), tmpAlert.getCriticityLevel());
                std::cout << "MedDistrib() : medicaments a prendre (1)" << std::endl;
                sleep(DELAY_BEFORE_ACTION); // Attente de 20 min
            }

        } else if ((secPoso - secNow) <= - DELAY_BEFORE_ACTION && (secPoso - secNow) >= - 2 * DELAY_BEFORE_ACTION)
        {
            // TODO : check si médicaments pris : 2eme fois = non donc alert
            if(false) // Si medicaments prit
            {
                 Notify();
            } else {
                tmpAlert.updateCriticity();
                Notify();
                sc.sendAlert(_user_id, tmpAlert.getType(), tmpAlert.getCriticityLevel());
                std::cout << "MedDistrib() : medicaments a prendre (2)" << std::endl;
                sleep(DELAY_BEFORE_ACTION); // Attente de 20 min
            }

        } else if ((secPoso - secNow) <= - 2 * DELAY_BEFORE_ACTION && (secPoso - secNow) >= - 3 * DELAY_BEFORE_ACTION)
        {
            // TODO : check si médicaments pris : 3eme fois = non donc alert
            if(false) // Si medicaments prit
            {
                 Notify();
            } else {
                tmpAlert.updateCriticity();
                Notify();
                sc.sendAlert(_user_id, tmpAlert.getType(), tmpAlert.getCriticityLevel());
                std::cout << "MedDistrib() : medicaments a prendre (3)" << std::endl;
                sleep(DELAY_BEFORE_ACTION); // Attente de 20 min
            }
        } else {
            break;
        }
    }
}

void MedDistrib::getPosology()
{
    static int firstTime = 0;
    int i = 0;
    ServerConnector sc;

    while(i != -1)
    {
        TimeU *timeTwoHours = new TimeU(2, 0, 0);
        TimeU *timeNow = new TimeU();
        timeNow->setRealTime();

        if(timeNow->getHours() == 2 || firstTime == 0)
        {
            std::cout << "MedDistrib() : Connexion au serveur" << std::endl;
            // TODO : Lancer le GetPosology du serveur
            rapidjson::Document doc;
            sc.getPosology(_user_id, doc);

            //rapidjson::SizeType i = 0;
	    //const rapidjson::Value& doc1 = doc[i];

	    const char json[] = "{ \"hello\" : \"world\" }";

	    rapidjson::Document d;
    		d.Parse<0>(json);

    		std::cout << d["hello"].GetString() << std::endl;
            /*for(rapidjson::SizeType j = 0 ; j < doc.Size() ; j++)
            {
		const rapidjson::Value& z = doc[j];

                const rapidjson::Value& a = z["hour"];
                std::cout << z["hour"].GetInt();
            
                const rapidjson::Value& b = z["minute"];
                std::cout << b["minute"].GetInt();

                const rapidjson::Value& c = z["drugs"];
                
                // rapidjson uses SizeType instead of size_t.
                for (rapidjson::SizeType i = 0; i < c.Size(); i++)
                {
                    const rapidjson::Value& d = c[i];
            
                    std::cout << d["name"].GetString();
                    std::cout << d["quantity"].GetInt();
                }        
            }*/

            firstTime = 1;
	    sleep(3600);
        } else {
            unsigned int res = timeNow->Difference(timeTwoHours);
            sleep(res);
        }
    }

}
