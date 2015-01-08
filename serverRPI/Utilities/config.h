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

#ifndef __CONFIG_H
#define __CONFIG_H

#include "../libs/tinyxml2.h"
#include <iostream> 
#include <map>
/**
 * @file config.h
 *  
 * Description of the program objectives.
 * 
 */

class Config
{
public:
    Config();
    ~Config();

    static Config* Instance();

    std::string getConf(std::string);

private:
    static Config _config;
    std::map<std::string,std::string> _configs;
    
    
};


#endif
