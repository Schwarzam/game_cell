//
// Created by gusta on 12/12/2021.
//

#ifndef THECELL_P2PSERVER_H
#define THECELL_P2PSERVER_H


#include "../steam/isteamnetworkingsockets.h"
#include "../steam/steamnetworkingtypes.h"
#include "../steam/isteammatchmaking.h"
#include "../steam/steam_api.h"

class P2Pserver {
public:
    P2Pserver();
    ~P2Pserver();

    void RunFrame();

private:

    STEAM_CALLBACK(P2Pserver, OnCreatedBeaconCall, CreateBeaconCallback_t, m_createdBeaconCallback);

    void createLobby();
    void onLobbyCreated( LobbyCreated_t *pCallback, bool status );
    CCallResult<P2Pserver, LobbyCreated_t>  m_LobbyCreated_t;

    void requestLobbyList();
    void onLobbyRequested( LobbyMatchList_t *pCallback, bool status );
    CCallResult<P2Pserver, LobbyMatchList_t> m_LobbyMatchList_t;

    HSteamListenSocket m_hListenSocket;
    HSteamNetPollGroup m_hNetPollGroup{};

    SteamNetworkingIdentity identity{};
};


#endif //THECELL_P2PSERVER_H
