//
// Created by gusta on 13/11/2021.
//

#ifndef GAME_CELL_SERVER_H
#define GAME_CELL_SERVER_H


#include "../steam/steamnetworkingtypes.h"

class Server {
public:
    Server();
    ~Server();

    void SendUpdatedServerDetailsToSteam();

    void run();

private:
    // Socket to listen for new connections on
    HSteamListenSocket m_hListenSocket;

    // Poll group used to receive messages from all clients at once
    HSteamNetPollGroup m_hNetPollGroup;

};


#endif //GAME_CELL_SERVER_H
