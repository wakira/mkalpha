
/*
 * WebSocket.h
 */

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "mbed.h"

#include "TCPSocket.h"
#include "EthernetInterface.h"

class WebSocket
{
    public:
        /**
        * Constructor
        *
        * @param url The Websocket url in the form "ws://ip_domain[:port]/path" (by default: port = 80)
        */
        WebSocket(EthernetInterface *eth, char * url);

        /**
        * Connect to the websocket url
        *
        *@return true if the connection is established, false otherwise
        */
        bool connect();

        /**
        * Send a string according to the websocket format (see rfc 6455)
        *
        * @param str string to be sent
        *
        * @returns the number of bytes sent
        */
        int send(char * str);

        /**
        * Read a websocket message
        *
        * @param message pointer to the string to be read (null if drop frame)
        *
        * @return true if a websocket frame has been read
        */
        bool read(char * message);

        /**
        * To see if there is a websocket connection active
        *
        * @return true if there is a connection active
        */
        bool is_connected();

        /**
        * Close the websocket connection
        *
        * @return true if the connection has been closed, false otherwise
        */
        bool close();

        /*
        * Accessor: get path from the websocket url
        *
        * @return path
        */
        char* getPath();

    private:

        void fillFields(char * url);
        int parseURL(const char* url, char* scheme, size_t maxSchemeLen, char* host, size_t maxHostLen, uint16_t* port, char* path, size_t maxPathLen); //Parse URL
        int sendOpcode(uint8_t opcode, char * msg);
        int sendLength(uint32_t len, char * msg);
        int sendMask(char * msg);
        int readChar(char * pC, bool block = true);
        
        char scheme[8];
        uint16_t port;
        char host[32];
        char path[64];
        
        EthernetInterface *ethptr;
        TCPSocket socket;

        int read(char * buf, int len, int min_len = -1);
        int write(char * buf, int len);
};

#endif
