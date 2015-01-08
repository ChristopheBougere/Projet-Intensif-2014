/* -*- c-basic-offset: 3 -*- 
 *
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author PHILIPPE Bastien <bastien.philippe@ecole.ensicaen.fr> 
 * @version     0.0.1 -  Jan 06, 2015
 * 
 * @todo 
 * @bug
 */

#include <iostream>
#include <map>
#include "config.h"


using namespace std;
using namespace tinyxml2;

Config Config::_config=Config();

Config::Config() {
   XMLDocument confFile;
   confFile.LoadFile( "conf.xml");
   XMLNode * pRoot = confFile.FirstChild();
   if (pRoot == 0) {
      cerr << "error parsing pRoot" << endl;
      return;
   }
   XMLElement * pEntryElement = pRoot->FirstChildElement("entry");
   if (pEntryElement == 0) {
      cerr << "error parsing pEntryElement" << endl;
      return;
   }
   while (pEntryElement != 0) {
      string name = pEntryElement->FirstChildElement("name")->GetText();
      string value = pEntryElement->FirstChildElement("value")->GetText();
      _configs.insert(pair<string,string>(name,value));
      pEntryElement = pEntryElement->NextSiblingElement("entry");
   }
}

Config::~Config() {
}

Config* Config::Instance() {
   return &_config;
}

string Config::getConf(string key) {
   return _configs.find(key)->second;
}

