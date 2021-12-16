//
// Created by gusta on 12/12/2021.
//

#include <iostream>
#include "P2Pserver.h"
#include "../steam/isteamnetworkingutils.h"
#include "../steam/isteammatchmaking.h"
#include "utils/Chat.h"

P2Pserver::P2Pserver() :
        m_LobbyEnter_t(this, &P2Pserver::onLobbyEnter),
        m_LobbyChatUpdate_t(this, &P2Pserver::onLobbyChatUpdate)
    //m_createdBeaconCallback(this, &P2Pserver::OnCreatedBeaconCall)
{
    SteamNetworkingUtils()->InitRelayNetworkAccess();
    m_hListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr );

    if (SteamNetworkingSockets()->GetIdentity(&identity)){
        std::cout << "Identidade: " << identity.GetSteamID().ConvertToUint64() << std::endl;
        createLobby();
    }else{
        std::cout << "Failed to start server." << std::endl;
        Chat::addMessage("Failed to start server!", sf::Color::Red);
    }
}

P2Pserver::~P2Pserver() = default;

void P2Pserver::RunFrame() {
    SteamAPI_RunCallbacks();
}

void P2Pserver::createLobby() {
    SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 6);
    m_LobbyCreated_t.Set(hSteamAPICall, this, &P2Pserver::onLobbyCreated);
}


void P2Pserver::onLobbyCreated( LobbyCreated_t *pCallback, bool bIOFailure ) {
    if(pCallback->m_eResult == k_EResultOK){
        lobbyId = pCallback->m_ulSteamIDLobby;

        if (pCallback->m_eResult == k_EResultOK){
            Chat::addMessage("Lobby Created!", sf::Color::Yellow);
            SteamMatchmaking()->SetLobbyJoinable(pCallback->m_ulSteamIDLobby, true);
        }
    }
}

void P2Pserver::requestLobbyList() {
    SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
    m_LobbyMatchList_t.Set( hSteamAPICall, this, &P2Pserver::onLobbyRequested );
}

void P2Pserver::onLobbyRequested(LobbyMatchList_t  *pCallback, bool status) {
    std::cout << pCallback->m_nLobbiesMatching << std::endl;
}

void P2Pserver::onLobbyEnter(LobbyEnter_t *pParam) {
    std::cout << "Entered lobby: " << pParam->m_EChatRoomEnterResponse << std::endl;
    std::cout << "Members in lobby: " << SteamMatchmaking()->GetNumLobbyMembers(lobbyId) << std::endl;

    Chat::addMessage("Entered Lobby", sf::Color::Yellow);

    try{
        //SteamMatchmaking()->GetLobbyMemberByIndex(lobbyId, 0);
    }catch(std::exception& e) {

    }
}

void P2Pserver::onLobbyChatUpdate(LobbyChatUpdate_t *pParam) {
    std::cout << "Lobby chat update: " << pParam->m_ulSteamIDLobby << std::endl;

    Chat::addMessage("Lobby chat update");
}

