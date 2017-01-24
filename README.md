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

-->Transmission Phase

-->Adaptative Rate Phase

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

# Joining Phase

Lo simulado con este modelo, comprende el envío de un mensaje "uplink window message"

al gateway o server, para luego recibir dos respuestas "downlink window message" que realizan

las configuraciones iniciales y las negociaciones de llaves aplicativas y de acceso a la red.

# Observaciones

Al ser una simulación de la capa física de los dispositivos LoRaWAN, sólo se envían paquetes Dummy

entre nodos y servers, la próxima fase será incluir condiciones de potencia de señal, spreading factor (ADR)

y retransmisión de datos por parte de los nodos (downlink window message), implementando timeouts y timestamps

para la implementación de acuse de recibo que posee LoRa.

# Fases Objetivo

-->Transmission Phase

-->Adaptative Rate Phase

-->Configuration Deployment

-->LoRaWAN/IPv6 transition Module (Coming soon!)
