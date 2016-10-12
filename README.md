# tesis-udp-2016

Para replicar la simulación se debe copiar la carpeta "aloha"

en el directorio samples/ del directorio raíz de la instalación

del software OMNET ++ 5.0.

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
