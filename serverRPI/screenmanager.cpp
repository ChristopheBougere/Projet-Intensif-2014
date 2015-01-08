#include "screenmanager.h"


ScreenManager::ScreenManager(ScreenCallback _med_notify, ScreenCallback _drawer_notify) {
	med_notify = _med_notify;
	drawer_notify = _drawer_notify;
}

ScreenManager::~ScreenManager() {}

void ScreenManager::Update(const Observable* observable) const
{
	std::cout << "Onn prévient le gestionnaire d'écran qu'il faut prendre des més" << std::endl;
	std::string msg = observable->Statut();
	std::cout << "message: " << msg << std::endl;
	char * S = new char[msg.length() + 1];
	std::strcpy(S, msg.c_str());
	if(msg == "") {
		drawer_notify(S);
	} else {
		med_notify(S);
	}
}
