//
// Created by gusta on 13/11/2021.
//

#include <iostream>
#include "Server.h"
#include "../steam/steamtypes.h"
#include "../steam/steam_gameserver.h"

#define INADDR_ANY ((unsigned long int) 0x00000000);
#define MASTER_SERVER_UPDATER_PORT 27016
#define PORT 27016

Server::Server() {
    uint32 unIP = INADDR_ANY;
    uint16 usMasterServerUpdaterPort = MASTER_SERVER_UPDATER_PORT;
    uint16 port = PORT;
    EServerMode eMode = eServerModeAuthenticationAndSecure;

    if (!SteamGameServer_Init(unIP, port, usMasterServerUpdaterPort, eMode, "0.1")){

    }else{
        std::cout << "Created server" << std::endl;
    }

    if(SteamGameServer()){
        SteamGameServer()->SetProduct( "TheCellServer" );
        SteamGameServer()->SetGameDescription( "The Cell matchmaking server" );

        SteamGameServer()->LogOnAnonymous();

        SteamNetworkingUtils()->InitRelayNetworkAccess();

        SteamGameServer()->SetAdvertiseServerActive( true );

        m_hListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
        m_hNetPollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();
    }else{
        std::cout << "SteamGameServer() interface is invalid\n" << std::endl;
    }
}

Server::~Server() {
    SteamGameServerNetworkingSockets()->CloseListenSocket(m_hListenSocket);
    SteamGameServerNetworkingSockets()->DestroyPollGroup(m_hNetPollGroup);

    // Disconnect from the steam servers
    SteamGameServer()->LogOff();

    // release our reference to the steam client library
    SteamGameServer_Shutdown();
}

void Server::run() {
    SteamGameServer_RunCallbacks();


}

