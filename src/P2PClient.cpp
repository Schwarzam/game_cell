//
// Created by gusta on 13/12/2021.
//

#include <iostream>
#include "P2PClient.h"


P2PClient::P2PClient() {

};

void P2PClient::requestLobbyList() {
    SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
    m_LobbyMatchList_t.Set( hSteamAPICall, this, &P2PClient::onLobbyRequested );
}

void P2PClient::onLobbyRequested(LobbyMatchList_t *pCallback, bool status) {
    std::cout << pCallback->m_nLobbiesMatching << std::endl;

    if (pCallback->m_nLobbiesMatching > 0){
//        CSteamID lobby = SteamMatchmaking()->GetLobbyByIndex( pCallback->m_nLobbiesMatching );
//
//        if (lobby != k_steamIDNil){
//            std::cout << lobby.ConvertToUint64() << std::endl;
//        }
    }
}

void P2PClient::processEvents() {
    SteamAPI_RunCallbacks();
}
