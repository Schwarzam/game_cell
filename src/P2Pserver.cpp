//
// Created by gusta on 12/12/2021.
//

#include <iostream>
#include "P2Pserver.h"
#include "../steam/isteamnetworkingutils.h"
#include "../steam/isteammatchmaking.h"

P2Pserver::P2Pserver() :
    m_createdBeaconCallback(this, &P2Pserver::OnCreatedBeaconCall)
{

    SteamNetworkingUtils()->InitRelayNetworkAccess();
    m_hListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr );


    if (SteamNetworkingSockets()->GetIdentity(&identity)){
        std::cout << "Identidade: " << identity.GetSteamID().ConvertToUint64() << std::endl;
        createBeacon();
    }else{
        std::cout << "Failed to start server." << std::endl;
    }



}

P2Pserver::~P2Pserver() {

}

void P2Pserver::RunFrame() {
    SteamAPI_RunCallbacks();
}

void P2Pserver::createBeacon() {
    uint32 numLocations{};

    if (SteamParties()->GetNumAvailableBeaconLocations(&numLocations)){
        SteamPartyBeaconLocation_t pList{};
        if (SteamParties()->GetAvailableBeaconLocations( &pList , 1 )){
            std::cout << pList.m_ulLocationID << std::endl;
            SteamParties()->CreateBeacon(6, &pList, "teste", "servidor teste");
        }
    }
}

void P2Pserver::OnCreatedBeaconCall( CreateBeaconCallback_t *pParam ){
    std::cout << pParam->m_eResult << std::endl;
}