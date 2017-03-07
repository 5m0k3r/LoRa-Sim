## English Version

#  LoRa-Sim

Copy the "aloha" folder in the samples/ folder which is below

the installation root folder of OMNET++ 5.0. 

Download Links for OMNET++ 5.0:

Windows: https://omnetpp.org/omnetpp/send/30-omnet-releases/2307-omnetpp-50-windows

OSX: https://omnetpp.org/omnetpp/send/30-omnet-releases/2306-omnetpp-50-macosx

Linux/UNIX : https://omnetpp.org/omnetpp/send/30-omnet-releases/2305-omnetpp-50-linux
# Joining Phase

To date, the joining phase is in working condition. It sends the "uplink window message"

to the gateway/server and then waits for the two answers "downlink window message" to 

achieve initial configuration including application-level keys and network access.

# Remarks

Given that (at this time) this is a PHY and MAC simulation of LoRaWAN devices, only dummy

packets are sent between the nodes and the server. The following step will be to include signal

power values, spreading factors and packet retransmission from the nodes (uplink window message),

thus implementing timeouts and timestamps to generate LoRa Acknowledge packets.


# LoRa Phases (ToDo)


-->Configuration Deployment

-->LoRaWAN/IPv6 transition Module (Coming soon!)


## Versión en Español

#  LoRa-Sim

Para replicar la simulación se debe copiar la carpeta "aloha"

en el directorio samples/ del directorio raíz de la instalación

del software OMNET ++ 5.0.

Enlaces de descarga para OMNET++ 5.0:

Windows: https://omnetpp.org/omnetpp/send/30-omnet-releases/2307-omnetpp-50-windows

OSX: https://omnetpp.org/omnetpp/send/30-omnet-releases/2306-omnetpp-50-macosx

Linux/UNIX : https://omnetpp.org/omnetpp/send/30-omnet-releases/2305-omnetpp-50-linux

# Joining Phase -Transmission Phase - Adaptative Rate - Retransmission Phase - Sleep mode

Lo simulado con este modelo, comprende el envío de un mensaje "uplink window message"

al gateway o server, para luego recibir dos respuestas "downlink window message" que realizan

las configuraciones iniciales y las negociaciones de llaves aplicativas y de acceso a la red. Luego de que el dispositivo 

LoRa realiza el emparejamiento con el gateway objetivo, este envía un mensaje "request" para enviar los datos que posee el 

nodo. En caso de recibir una confirmación cn un mensaje ACK desde el gateway, el nodo envía el payload deseado, en caso 

contrario, entra en un modo de reposo hasta que consume el tiempo válido de espera (Timeout), lo que fuerza al nodo

a realizar una retransmisión del mensaje. Finalmente una vez que el payload es recibido y confirmado por el Gateway con un 

mensaje ACK, el nodo entra en un modo de reposo por el tiempo que determine el usuario. Durante este periodo no 

realizará operaciones o transmisiones, hasta que vuelva  estar activo de nuevo a causa del término del tiempo de reposo.


# Observaciones

Al ser una simulación de la capa física de los dispositivos LoRaWAN, sólo se envían paquetes Dummy

entre nodos y servers, la próxima fase será incluir condiciones de potencia de señal, spreading factor (ADR)

y retransmisión de datos por parte de los nodos (downlink window message), implementando timeouts y timestamps

para la implementación de acuse de recibo que posee LoRa.

# Fases Objetivo

-->Configuration Deployment

-->LoRaWAN/IPv6 transition Module (Coming soon!)
