SRT5 : Victor GIRARD, Kévin Moreau, Luc GALLERAND

L'objectif du TP est de mettre en place un échange de données avec le protocole LoRa entre deux devices, en passant par un serveur MQTT.

Pour commencer, il nous faut définir les bibliothèques et définir des constantes.

Ensuite, on initialise la connexion avec le wifi (ssid et psswd) et on définit les paquets LoRa (union paquet).
On définit également la fonction "setup_wifi" et "setup_mqtt" pour définir la connexion au wifi et au serveur.

VOID SETUP
Par ailleurs, on définit la fonction "setup" il y aura le setup_wifi et le setup_mqtt.
Dans cette même fonction, on initialise des connexions avec le modem LoRa et les paramètres radio.

VOID LOOP
La fonction "loop" va permettre de se connecter au serveur et d'envoyer les données.
On publie notamment les paramètres LoRa au travers de MQTT.

SCHEMA DU PROTOTYPE
 __________________________________________________________________________________________
|		                                                                               |
| DEVICE1 -----------> Envoie paramètre LoRa sous MQTT --------------> DEVICE2             |
|    |											   |			 |
|    |											   |               |
|    |	 					          		 Application des données LoRa  |
|    |											   |			 |
|    |											   |               |
| DEVICE1 <------------ Échange avec le protocole LoRa --------------> DEVICE2             |
|__________________________________________________________________________________________|
