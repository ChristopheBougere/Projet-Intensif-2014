#include "meddistrib.h"
#include "unistd.h"
#include <pthread.h>
 
#include <iostream>
#include <fstream>

#include <ctime>
#include "time.h"
#include "../../Web/serverConnector.h"
#include "../../Utilities/timeu.h"
#include "../../libs/rapidjson/writer.h"
#include "../../libs/rapidjson/stringbuffer.h"
#include "../../libs/rapidjson/rapidjson.h"
#include "../../libs/rapidjson/document.h"

#define DELAY_BEFORE_ACTION 30 //1200 // = 20min

void MedDistrib::isDrawerOpen() {
    // Lire la valeur dans /sys/class/gpio/gpio21/value
    // Fermé = 1
    // Ouvert = 0
    std::string line;
    
    char c = '2';
    while(true) {
      std::ifstream file("/sys/class/gpio/gpio21/value");
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
	std::cout << "open" << std::endl;
        _drawedHasBeenOpened = true;
      } else if(c == '1') {
	std::cout << "close" << std::endl;
	if (_drawedHasBeenOpened) {
	  Notify();
	  return;
	}
        _drawedHasBeenOpened |= false;
      } else {
        std::cout << "Impossible de lire la réponse du fichier /sys/class/gpio/gpio21/value" << std::endl;
      }
      sleep(5);
    }
}

MedDistrib::MedDistrib()
{
    std::cout << "MedDistrib() : demarrage du module" << std::endl;

    _user_id = 1; // TODO
    
    // On initialise l'ecoute des GPIO
     
    std::system("[ -d /sys/class/gpio/gpio21 ] || echo 21 > /sys/class/gpio/export");
    std::system("echo out > /sys/class/gpio/gpio21/direction");
    
    //std::thread toto(&MedDistrib::isDrawerOpen,this);
    //toto.detach();

    std::thread t_posology(&MedDistrib::getPosology, this);
    t_posology.detach();

    /*for(int c = 0 ; c < _posologyList.size() ; c++)
    {
        _threadList.at(c).detach();
    }*/

    ///t_posology.detach();

    
}

std::string MedDistrib::Statut() const
{
  std::cout <<"statut - " << _drawedHasBeenOpened << std::endl;
  if (_drawedHasBeenOpened) {
    return "drugstaken";
  }
    std::string res = "";
    for(int c = 0 ; c < (int) _posologyList.size() ; c++)
    {
        Posology actPoso = _posologyList.at(c);
        AlertLevel* tmpAlert = actPoso.getAlert();
        if(tmpAlert->getAlert())
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
	std::cout << "enter checkPosology" << std::endl; 

    while(i != -1)
    {
      std::thread t_drawed;
        TimeU timePoso = poso->getTimeU();
        TimeU *timeNow = new TimeU();
        timeNow->setRealTime();

        int secPoso = timePoso.calculateSec();
        int secNow = timeNow->calculateSec();

        AlertLevel* tmpAlert = poso->getAlert();
	
 std::cout << "en secondes "<< (secPoso-secNow) << " - " << secPoso <<" " << secNow<< std::endl;
        if( (secPoso - secNow) > 0) // Pas l'heure de prendre
        {
	  std::cout << "MedDistrib() : aucune action a effectuer" << std::endl;
	     
	    sleep(secPoso - secNow);
        } else if ( secPoso <= secNow && secPoso + DELAY_BEFORE_ACTION > secNow)
	  {  std::cout << "!!!!!!!!!!!!!!!!!!!on doit lancer l'affichage !" << std::endl;
	  _drawedHasBeenOpened = false;
            // TODO : check si médicaments pris : 1ere fois = non donc alert
	  t_drawed = std::thread(&MedDistrib::isDrawerOpen,this); // Si medicaments prit
          t_drawed.detach();
	  tmpAlert->updateCriticity();
	  std::cout << "criticity level at initialisation "<< tmpAlert->getCriticityLevel() << std::endl;
	  Notify();
	  std::cout << "MedDistrib() : medicaments a prendre  : :-)" << std::endl;
	  sleep(DELAY_BEFORE_ACTION); // Attente de 20 min
            

        } else if ((secPoso - secNow) <= - DELAY_BEFORE_ACTION && (secPoso - secNow) >= - 2 * DELAY_BEFORE_ACTION)
        {
            // TODO : check si médicaments pris : 2eme fois = non donc alert
            if(_drawedHasBeenOpened) // Si medicaments prit
            {
		 return;
		 
            } else {
                tmpAlert->updateCriticity();
                Notify();
                sc.sendAlert(_user_id, tmpAlert->getType(), tmpAlert->getCriticityLevel()-1);
                std::cout << "MedDistrib() : medicaments a prendre =>" << tmpAlert->getCriticityLevel() << std::endl;
                sleep(DELAY_BEFORE_ACTION); // Attente de 20 min
            }

        } else if ((secPoso - secNow) <= - 2 * DELAY_BEFORE_ACTION && (secPoso - secNow) >= - 3 * DELAY_BEFORE_ACTION)
        {
            // TODO : check si médicaments pris : 3eme fois = non donc alert
            if(_drawedHasBeenOpened) // Si medicaments prit
            {
		 return;
            } else {
                tmpAlert->updateCriticity();
                Notify();
                sc.sendAlert(_user_id, tmpAlert->getType(), tmpAlert->getCriticityLevel()-1);
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
            if (!sc.getPosology(_user_id, doc)) {
		std::cout << "probleme reception getPosology" << std::endl;
		
	    }
		rapidjson::StringBuffer buf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
		doc.Accept(writer);
		std::cout << buf.GetString() << std::endl; 


		std::cout << "doc:"<<doc.IsArray() << std::endl;
	    _posologyList.clear();
            for(rapidjson::SizeType j = 0 ; j < doc.Size() ; j++)
            {
		const rapidjson::Value& time = doc[j];

                int hour = time["hour"].GetInt();
                std::cout << hour <<":";
		
                int minute = time["minute"].GetInt();
                std::cout << minute << std::endl;

		TimeU current;
		current.setRealTime();
		if (current.getHours() > hour || (current.getHours() == hour && current.getMinutes() > minute)) {
		  continue;
		}
		    		

                const rapidjson::Value& drugs = time["drugs"];
                std::cout << "drugs:" << drugs.IsArray() << std::endl;
		Posology posology;
		posology.setTimeU(hour,minute,0);
                // rapidjson uses SizeType instead of size_t.
                for (rapidjson::SizeType i = 0; i < drugs.Size(); i++)
                {
                    const rapidjson::Value& drug = drugs[i];
            
		    std::string drugName = drug["name"].GetString();
		    int drugQuantity = drug["quantity"].GetInt();

                    std::cout << drug["name"].GetString() << "-";
                    std::cout << drug["quantity"].GetInt() << std::endl;


		    posology.addPosology(drugName,drugQuantity);
                }
		std::cout << "before : " << _posologyList.size() << std::endl;
		_posologyList.push_back(posology); 
		std::cout << "after : " << _posologyList.size() << std::endl;
	    }
       if((int) _posologyList.size() != 0)  {
          for(int c = 0 ; c < (int) _posologyList.size() ; c++)
          {	std::cout << "before launch" << c <<std::endl;
             Posology actPoso = _posologyList.at(c);
             std::thread t_taken(&MedDistrib::checkPosology, this, &actPoso);
             t_taken.detach();

              _threadList.push_back(std::move(t_taken));
		std::cout << "after launch" << c << std::endl;
            }
        }
	
            firstTime = 1;
	    sleep(3600);
        } else {
            unsigned int res = timeNow->Difference(timeTwoHours);
            sleep(res);
        }
    }

}
