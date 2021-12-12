//
// Created by gusta on 12/12/2021.
//

#ifndef THECELL_P2PSERVER_H
#define THECELL_P2PSERVER_H


#include "../steam/isteamnetworkingsockets.h"
#include "../steam/steamnetworkingtypes.h"
#include "../steam/isteammatchmaking.h"

class P2Pserver {
public:
    P2Pserver();
    ~P2Pserver();

    void RunFrame();

private:

    STEAM_CALLBACK(P2Pserver, OnCreatedBeaconCall, CreateBeaconCallback_t, m_createdBeaconCallback);

    void createBeacon();


    HSteamListenSocket m_hListenSocket;
    HSteamNetPollGroup m_hNetPollGroup{};

    SteamNetworkingIdentity identity{};
};


#endif //THECELL_P2PSERVER_H
