//
// Created by gusta on 04/12/2021.
//

#ifndef THECELL_SERVERMENU_H
#define THECELL_SERVERMENU_H


#include <list>
#include <iostream>
#include "../../steam/steamclientpublic.h"
#include "../../steam/matchmakingtypes.h"
#include "../../steam/isteammatchmaking.h"

// Class to encapsulate game server data
class CGameServer
{
public:
    explicit CGameServer( gameserveritem_t *pGameServerItem );

    const char* GetName() { return m_szServerName; }

    const char* GetDisplayString() { return m_szServerString; }

    uint32 GetIP() const { return m_unIPAddress; }

    int32 GetPort() const { return m_nConnectionPort; }
    CSteamID GetSteamID()	{ return m_steamID; }

private:
    uint32 m_unIPAddress;			// IP address for the server
    int32 m_nConnectionPort;		// Port for game clients to connect to for this server
    int m_nPing;					// current ping time in milliseconds
    char m_szMap[32];				// current map
    char m_szGameDescription[64];	// game description
    int m_nPlayers;					// current number of players on the server
    int m_nMaxPlayers;				// Maximum players that can join this server
    int m_nBotPlayers;				// Number of bots (i.e simulated players) on this server
    bool m_bPassword;				// true if this server needs a password to join
    bool m_bSecure;					// Is this server protected by VAC
    int	m_nServerVersion;			// server version as reported to Steam
    char m_szServerName[64];		// Game server name
    char m_szServerString[128];		// String to show in server browser
    CSteamID m_steamID;
};

class ServerMenu : public ISteamMatchmakingServerListResponse {
public:
    ServerMenu();

    void RefreshInternetServers();

    void ServerResponded( HServerListRequest hReq, int iServer ) override;
    void ServerFailedToRespond( HServerListRequest hReq, int iServer ) override;
    void RefreshComplete( HServerListRequest hReq, EMatchMakingServerResponse response ) override;

    static void Rebuild( std::list<CGameServer> &List, bool bIsRefreshing )
    {
        std::list<CGameServer>::iterator iter;

        for( iter = List.begin(); iter != List.end(); ++iter )
        {
            std::cout << iter->GetDisplayString() << std::endl;
        }
    }

private:
    // Track the number of servers we know about
    int m_nServers{};

    // Track whether we are in the middle of a refresh or not
    bool m_bRequestingServers{};

    // Track what server list request is currently running
    HServerListRequest m_hServerListRequest{};

    // List of game servers
    std::list< CGameServer > m_ListGameServers;
};


#endif //THECELL_SERVERMENU_H
