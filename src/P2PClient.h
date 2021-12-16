//
// Created by gusta on 13/12/2021.
//

#ifndef THECELL_P2PCLIENT_H
#define THECELL_P2PCLIENT_H


#include "../steam/isteammatchmaking.h"
#include "P2Pserver.h"

class P2PClient {
public:
    P2PClient();
    ~P2PClient() = default;

    void processEvents();
    void requestLobbyList();
private:
    void onLobbyRequested( LobbyMatchList_t *pCallback, bool status );
    CCallResult<P2PClient, LobbyMatchList_t> m_LobbyMatchList_t;


};


#endif //THECELL_P2PCLIENT_H
