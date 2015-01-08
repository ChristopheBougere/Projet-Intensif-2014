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
                this->Statut();
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
                this->Statut();
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
                this->Statut();
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
            // TODO : Lancer le GetPosology du serveur
            rapidjson::Document doc;
            sc.getPosology(_user_id, doc);

            while(doc)


            firstTime = 1;
        } else {
            unsigned int res = timeNow->Difference(timeTwoHours);
            sleep(res);
        }
    }

}
