//COMMANDES DU BYTECODE
enum cmd{DBT=0x01, //commande de depart
         ATT=0x02, //commande d'attente
         DAL=0x44, DET=0x45, //commandes de del: allumer et fermer 
         SGO=0x48, SAR=0x09, //commandes de son: jouer et arreter
         MAR=0x60, MAR2=0x61, MAV=0x62, MRE=0x63, TRD=0x64, TRG=0x65, //commandes de moteurs
         DBC=0xC0, FBC=0xC1, //fonctions de bouclage:
         FIN=0xFF}; //commande de fin