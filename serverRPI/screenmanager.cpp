#include "screenmanager.h"


ScreenManager::ScreenManager(ScreenCallback _med_notify, ScreenCallback _drawer_notify) {
	//_alert = alert;
	//_drawer = drawer;
	med_notify = _med_notify;
	drawer_notify = _drawer_notify;
}

ScreenManager::~ScreenManager() {}

void ScreenManager::Update(const Observable* observable) const
{
  
}
