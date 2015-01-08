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
 * @author FLEURIAUD Benoit <benoit.fleuriaud@ecole.ensicaen.fr> 
 * @version     0.0.1 -  Jan 06, 2015
 * 
 * @todo 
 * @bug
 */

/**
 * @file serverConnector.cpp
 *  
 * Description 
 * 
 */


#include <iostream> 
#include <cstring>
#include <string>
#include <sstream>
#include "httpDownloader.h"
#include "../Utilities/config.h"
#include "serverConnector.h"

using namespace std;
using namespace rapidjson;

bool ServerConnector::getPosology(int userId, Document& json) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");
   string userIdString;
   stringstream out;
   out << userId;
   userIdString = out.str();

   string jsonString;
 
   HTTPDownloader downloader;
   string url = apiURL+"getPosology.php?pi=true&user_id="+userIdString;
   jsonString = downloader.download(url);

   json.Parse<0>(jsonString.c_str());
   if (!json.IsArray()) {
      cout << "json is not array" << endl;
      return false;
   }
   return true;
}

bool ServerConnector::sendAlert(int userId, int alertType, int alertLevel) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");

   string userIdString;
   stringstream outUID;
   outUID << userId;
   userIdString = outUID.str();

   string alertTypeString;
   stringstream out;
   out << alertType;
   alertTypeString = out.str();

   string alertLevelString;
   stringstream outAL;
   outAL << alertLevel;
   alertLevelString = outAL.str();

   HTTPDownloader downloader;
   string url = apiURL+"sendAlert.php?user_id="+userIdString+"&alert_type="+alertTypeString+"&alert_level="+alertLevelString;
   downloader.download(url);
  
   return true;
}

bool ServerConnector::sendAlert(int userId, int alertType, int alertLevel, string streamUrl) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");

   string userIdString;
   stringstream outUID;
   outUID << userId;
   userIdString = outUID.str();

   string alertTypeString;
   stringstream out;
   out << alertType;
   alertTypeString = out.str();

   string alertLevelString;
   stringstream outAL;
   outAL << alertLevel;
   alertLevelString = outAL.str();

   HTTPDownloader downloader;
   string url = apiURL+"sendAlert.php?user_id="+userIdString+"&alert_type="+alertTypeString+"&alert_level="+alertLevelString+"&stream_url="+streamUrl;
   downloader.download(url);

   return true;
}


bool ServerConnector::registerUser(Document& json) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");

   CURL *curl;
   CURLcode res;

   string jsonString;
 
   HTTPDownloader downloader;
   string url = apiURL+"registerUser.php";
   jsonString = downloader.download(url);

   json.Parse<0>(jsonString.c_str());
   if (!json.IsObject()) {
      cerr << "is not object" << endl;
      return false;
   }
   return true;
}

bool ServerConnector::isRecentFalldown(int userId, rapidjson::Document&  json ) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");
   string userIdString;
   stringstream out;
   out << userId;
   userIdString = out.str();

   string jsonString;
 
   HTTPDownloader downloader;
   string url = apiURL+"isRecentFalldown.php?user_id="+userIdString;
   jsonString = downloader.download(url);

   json.Parse<0>(jsonString.c_str());
   if (!json.IsObject()) {
      return false;
   }
   return true;
}
