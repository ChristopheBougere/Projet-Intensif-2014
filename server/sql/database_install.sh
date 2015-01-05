#!/bin/sh
#
# @author PHILIPPE Bastien <bastien.philippe@ecole.ensicaen.fr> 
# @author RIVEREAU Quentin <quentin.rivereau@ecole.ensicaen.fr> 
# @version     0.0.1 -  Jan 05, 2015
#
# @(#)database_install.sh
# 

# Installation de postgres si besoin
psql --version > /dev/null 2>&1 || sudo apt-get install postgresql

# Creation de l'utilisateur
echo "Création de l'utilisateur dbuser"
sudo -u postgres psql -c "create user dbuser with createdb nocreateuser encrypted password 'password'" 2> /dev/null

# Création de la base de données
echo "Création de la base de données"
sudo -u postgres createdb -O dbuser server_database 2> /dev/null

# Création des tables
echo "Création des tables, insertion des valeurs par défaut"
cat database.sql | sudo -u postgres psql -q server_database > /dev/null