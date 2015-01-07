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
#include <curl/curl.h>
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


   CURL *curl;
   CURLcode res;

   string jsonString;
 
   curl = curl_easy_init();
   if (curl) {
      string url = apiURL+"getPosology.php?user_id="+userIdString;
      char * cStrUrl = new char[url.length()+1];
      strcpy (cStrUrl, url.c_str());
      cStrUrl[url.length()]='\0';
      curl_easy_setopt(curl, CURLOPT_URL, cStrUrl);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, serverConnectorWrite);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonString);
      res = curl_easy_perform(curl);
      delete[] cStrUrl;

      if(res != CURLE_OK)
	 return false;
      json.Parse<0>(jsonString.c_str());
      if (!json.IsArray()) {
	 return false;
      }
      curl_easy_cleanup(curl);
      return true;
   }
   return false;
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

   CURL *curl;
   CURLcode res;
   curl = curl_easy_init();
   if (curl) {
      string url = apiURL+"sendAlert.php?user_id="+userIdString+"&alert_type="+alertTypeString+"&alert_level="+alertLevelString;
      cout << url << " " << url.length() << endl;
      char * cStrUrl = new char[url.length()+1];
      cout << cStrUrl << endl;
      strcpy (cStrUrl, url.c_str());
      cStrUrl[url.length()]='\0';
      curl_easy_setopt(curl, CURLOPT_URL, cStrUrl);
      res = curl_easy_perform(curl);
      delete[] cStrUrl;
      if(res != CURLE_OK)
	 return false;
      curl_easy_cleanup(curl);
      return true;
   }
   return false;
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

   CURL *curl;
   CURLcode res;
   curl = curl_easy_init();
   if (curl) {
      string url = apiURL+"sendAlert.php?user_id="+userIdString+"&alert_type="+alertTypeString+"&alert_level="+alertLevelString+"&stream__url="+streamUrl;
      cout << url << " " << url.length() << endl;
      char * cStrUrl = new char[url.length()+1];
      cout << cStrUrl << endl;
      strcpy (cStrUrl, url.c_str());
      cStrUrl[url.length()]='\0';
      curl_easy_setopt(curl, CURLOPT_URL, cStrUrl);
      res = curl_easy_perform(curl);
      delete[] cStrUrl;
      if(res != CURLE_OK)
	 return false;
      curl_easy_cleanup(curl);
      return true;
   }
   return false;
}


bool ServerConnector::registerUser(Document& json) {
   Config* conf = Config::Instance();
   string apiURL = conf->getConf("apiURL");

   CURL *curl;
   CURLcode res;

   string jsonString;
 
   curl = curl_easy_init();
   if (curl) {
      string url = apiURL+"registerUser.php";
      char * cStrUrl = new char[url.length()+1];
      strcpy (cStrUrl, url.c_str());
      cStrUrl[url.length()]='\0';
      curl_easy_setopt(curl, CURLOPT_URL, cStrUrl);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, serverConnectorWrite);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonString);
      res = curl_easy_perform(curl);
      delete[] cStrUrl;
      
      if(res != CURLE_OK) {
	 cerr << res << endl;
	 return false;
      }
      json.Parse<0>(jsonString.c_str());
      if (!json.IsObject()) {
	 cerr << "is not object" << endl;
	 return false;
      }
      curl_easy_cleanup(curl);
      return true;
   }
   return false;
}

size_t serverConnectorWrite(void *ptr, size_t size, size_t nmemb, string *s) {
   size_t new_len = size*nmemb;
   char* tmp = new char[new_len];
   memcpy(tmp, ptr, size*nmemb);
   s->append(tmp);
   delete[] tmp;
   return size*nmemb;
}
