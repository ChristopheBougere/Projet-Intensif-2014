#include "screenmanager.h"

std::string ScreenManager::Statut(void) const {
	if(getHasPushedYes()) {
		return "oui";
	} else {
		return "non";
	}
}


ScreenManager::ScreenManager(ScreenCallback _med_notify, ScreenCallback _drawer_notify, ScreenCallback _fall_notify) {
	med_notify = _med_notify;
	drawer_notify = _drawer_notify;
	fall_notify = _fall_notify;
	setScreenManager(this);
}

ScreenManager::~ScreenManager() {}

void ScreenManager::Update(const Observable* observable) const
{
	std::string msg = observable->Statut();
	std::cout << "message re�u par screen manager  : " << msg << std::endl;
	char * S = new char[msg.length() + 1];
	std::strcpy(S, msg.c_str());
	if(msg == "") {
		drawer_notify(S);
	} else if(msg == "falldown") {
		fall_notify(S);
	} else {
		med_notify(S);
	}
}
