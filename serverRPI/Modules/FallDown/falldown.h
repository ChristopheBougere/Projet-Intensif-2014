#ifndef FALLDOWN_H
#define FALLDOWN_H

#include "../../Utilities/observable.h"
#include "../../Utilities/observer.h"
#include "../../Web/serverConnector.h"
#include "../../libs/rapidjson/rapidjson.h"
#include "../../libs/rapidjson/document.h"
#include "../../Utilities/config.h"
#include <string>
#include <thread>

class Falldown : public Observer, public Observable
{
public:
    Falldown();
    std::string getStreamUrl() const;
    void Change(int valeur);
    std::string Statut(void) const;
    void Update(const Observable* observable) const;
private:
    
    void spoofServer();

    int _criticity;
};

#endif // FALLDOWN_H
