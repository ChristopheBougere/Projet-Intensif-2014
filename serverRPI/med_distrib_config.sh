#!/bin/sh
#
# @author PALU Adrien <adrien.palu@ecole.ensicaen.fr> 
# @version     0.0.1 -  Jan 06, 2015
#
# @(#)medical_distribution_configuration.sh
# 

# +3.3V ET PIN 16
# Configuration du PIN pour le module de distribution de medicaments
[ -d /sys/class/gpio/gpio21 ] || echo 21 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio21/direction
echo 0 > /sys/class/gpio/gpio21/value
