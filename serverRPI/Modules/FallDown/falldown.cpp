#include "falldown.h"

Falldown::Falldown()
{
	std::cout << "FallDown() : demarrage du module" << std::endl;

	std::thread spoofer(&Falldown::spoofServer,this);
	spoofer.detach();

}

std::string Falldown::getStreamUrl() const {
    // Launch stream
    // return stream url
    std::system("raspivid -o - -t 600000 -w 500 -h 500 | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8554}' :demux=h264 &");
    
    Config conf;
    std::string ip = conf.getConf("myIp");
    return "http://" + ip + ":8554";
}

void Falldown::Update(const Observable* observable) const {
   std::string msg = observable->Statut();
   if(msg == "oui") {
      ServerConnector sc;
      std::string url = getStreamUrl();
      sc.sendAlert(1, 2, 2, url);
   } else if(msg == "non") {
      
   }
}

void Falldown::Change(int valeur)
{
    _criticity = valeur;
}

std::string Falldown::Statut(void) const
{
   return "falldown";
}

void Falldown::spoofServer() {
   std::cout << "spoofer launched" << std::endl;
   while(1) {
      ServerConnector sc;
      rapidjson::Document doc;
      sc.isRecentFalldown(1, doc);
      std::cout << "= fall =" << doc["recentFalldown"].GetInt() << std::endl;
      if(doc["recentFalldown"].GetInt() == 1) {
	 // Changer d'écran
	 Notify();
      }
      sleep(10);
   }
}
