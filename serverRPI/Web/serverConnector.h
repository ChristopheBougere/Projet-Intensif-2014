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
 * @version     0.0.1 - Jan 06, 2015
 * 
 * @todo 
 * @bug 
 */

#ifndef __SERVERCONNECTOR_H
#define __SERVERCONNECTOR_H

#include "../libs/rapidjson/rapidjson.h"
#include "../libs/rapidjson/document.h"
/**
 * @file serverConnector.h
 *  
 * Description of the program objectives.
 * 
 */

#include <iostream> 

class ServerConnector {

 public :
   ServerConnector(){}
   ~ServerConnector(){}
   
   bool getPosology(int /*userId*/,rapidjson::Document& /* outJson*/ );

   bool sendAlert(int /*userId*/, int /*alertType*/);

   bool registerUser(rapidjson::Document& /* outJson*/ );

 private :

   size_t static write(void *ptr, size_t size, size_t nmemb, std::string *s);

};

size_t serverConnectorWrite(void *ptr, size_t size, size_t nmemb, std::string *s);
#endif
