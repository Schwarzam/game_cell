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

    ISteamMatchmaking* pSteamMatchmaking = SteamMatchmaking();

    for ( uint32 iLobby = 0; iLobby < pCallback->m_nLobbiesMatching; iLobby++ ){
        if (pSteamMatchmaking){
            pSteamMatchmaking->GetLobbyByIndex( iLobby );
        }
    }

}

void P2PClient::processEvents() {
    SteamAPI_RunCallbacks();
}
