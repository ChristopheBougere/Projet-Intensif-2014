#include "meddistrib.h"
#include "unistd.h"
#include <pthread.h>

#include <iostream>

#include <ctime>
#include "time.h"
#include "../../Web/serverConnector.h"
#include "../../Utilities/timeu.h"

#define DELAY_BEFORE_ACTION 30 //1200 // = 20min

MedDistrib::MedDistrib()
{
    std::cout << "MedDistrib() : demarrage du module" << std::endl;

    _alert = new AlertLevel(1);
    _user_id = 1; // TODO

    std::thread t_posology(&MedDistrib::getPosology, this);

    // TEST
    Posology *newPoso = new Posology();
    newPoso->setTimeU(12, 21, 00);
    newPoso->addPosology("A", 1);

    Posology *newPoso1 = new Posology();
    newPoso1->setTimeU(12, 22, 00);
    newPoso1->addPosology("A", 1);

    _posologyList.push_back(*newPoso);
    _posologyList.push_back(*newPoso1);
    
    // END TEST

    if((int) _posologyList.size() != 0)
    {
        for(int c = 0 ; c < (int) _posologyList.size() ; c++)
        {
            Posology actPoso = _posologyList.at(c);
            std::thread t_taken(&MedDistrib::checkPosology, this, &actPoso);
            t_taken.join();

            _threadList.push_back(std::move(t_taken));
        }
    }


    /*for(int c = 0 ; c < _posologyList.size() ; c++)
    {
        _threadList.at(c).detach();
    }*/


    t_posology.detach();
}

int MedDistrib::Statut() const
{
    return 0; //TODO
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
                _alert.activateAlert();
                sc.sendAlert(_user_id, _alert.getType(), _alert.getCriticityLevel());
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
                _alert.updateCriticity();
                sc.sendAlert(_user_id, _alert.getType(), _alert.getCriticityLevel());
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
                _alert.updateCriticity();
                sc.sendAlert(_user_id, _alert.getType(), _alert.getCriticityLevel());
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
    int i = 0;
    while(i != -1)
    {
        TimeU *timeTwoHours = new TimeU(2, 0, 0);
        TimeU *timeNow = new TimeU();
        timeNow->setRealTime();

        if(timeNow->getHours() == 2)
        {
            // TODO : Lancer le GetPosology du serveur
        } else {
            unsigned int res = timeNow->Difference(timeTwoHours);
            sleep(res);
        }
    }

}
