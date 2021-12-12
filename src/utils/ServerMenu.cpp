//
// Created by gusta on 04/12/2021.
//
#define ARRAYSIZE(A) ( sizeof(A)/sizeof(A[0]) )

#include <debugapi.h>
#include "ServerMenu.h"
#include "../../steam/isteammatchmaking.h"
#include "../../steam/isteamutils.h"

//-----------------------------------------------------------------------------
// Purpose: Constructor -- initialize from steam gameserveritem_t
//-----------------------------------------------------------------------------
CGameServer::CGameServer( gameserveritem_t *pGameServerItem )
{
    m_unIPAddress = pGameServerItem->m_NetAdr.GetIP();
    m_nConnectionPort = pGameServerItem->m_NetAdr.GetConnectionPort();
    m_nPing = pGameServerItem->m_nPing;
    strncpy_s( m_szMap, pGameServerItem->m_szMap, ARRAYSIZE( m_szMap ) );
    strncpy_s( m_szGameDescription, pGameServerItem->m_szGameDescription, ARRAYSIZE( m_szGameDescription ) );
    m_nPlayers = pGameServerItem->m_nPlayers;
    m_nMaxPlayers = pGameServerItem->m_nMaxPlayers;
    m_nBotPlayers = pGameServerItem->m_nBotPlayers;
    m_bPassword = pGameServerItem->m_bPassword;
    m_bSecure = pGameServerItem->m_bSecure;
    m_nServerVersion = pGameServerItem->m_nServerVersion;
    strncpy_s( m_szServerName, pGameServerItem->GetName(), ARRAYSIZE( m_szServerName ) );
    sprintf_s( m_szServerString, "%s (%i/%i) at %s ping(%d)", pGameServerItem->GetName(), pGameServerItem->m_nPlayers, pGameServerItem->m_nMaxPlayers, pGameServerItem->m_NetAdr.GetConnectionAddressString(), pGameServerItem->m_nPing );
    m_steamID = pGameServerItem->m_steamID;
}


//-----------------------------------------------------------------------------
// Purpose: Initiate a refresh of internet servers
//-----------------------------------------------------------------------------
void ServerMenu::RefreshInternetServers()
{
    // If we are still finishing the previous refresh, then ignore this new request
    if ( m_bRequestingServers )
        return;

    // If another request is outstanding, make sure we release it properly
    if ( m_hServerListRequest )
    {
        SteamMatchmakingServers()->ReleaseRequest( m_hServerListRequest );
        m_hServerListRequest = NULL;
    }

    std::cout << "Refresing servers" << std::endl;

    OutputDebugString( "Refreshing internet servers\n" );
    // Track that we are now in a refresh, what type of refresh, and reset our server count
    m_bRequestingServers = true;
    m_nServers = 0;
    m_ListGameServers.clear();
    //SetHeading( "Internet DedicatedServer browser" );

    Rebuild( m_ListGameServers, m_bRequestingServers );


    // Allocate some filters, there are some common pre-defined values that can be used:
    //
    // "gamedir" -- this is used to specify mods inside or a single product/appid
    // "secure" -- this is used to specify whether anti-cheat is enabled for a server
    // "gametype" -- this is used to specify game type and is set to whatever your game server code sets

    MatchMakingKeyValuePair_t pFilters[2];
    MatchMakingKeyValuePair_t *pFilter = pFilters;

    strncpy_s( pFilters[ 0 ].m_szKey, "gamedir", sizeof(pFilters[ 0 ].m_szKey) );
    strncpy_s( pFilters[ 0 ].m_szValue, "thecell", sizeof(pFilters[ 0 ].m_szValue) );

    strncpy_s( pFilters[ 1 ].m_szKey, "secure", sizeof(pFilters[ 1 ].m_szKey) );
    strncpy_s( pFilters[ 1 ].m_szValue, "1", sizeof(pFilters[ 1 ].m_szValue) );

    //strncpy_safe( pFilters[ 2 ].m_szKey, "gametype", sizeof(pFilters[ 1 ].m_szValue) );
    //strncpy_safe( pFilters[ 2 ].m_szValue, "dm", sizeof(pFilters[ 1 ].m_szValue) );

    // bugbug jmccaskey - passing just the appid without filters results in getting all servers rather than
    // servers filtered by appid alone.  So, we'll use the filters to filter the results better.
    m_hServerListRequest = SteamMatchmakingServers()->RequestInternetServerList( SteamUtils()->GetAppID(), &pFilter, ARRAYSIZE(pFilters), this );
}


//-----------------------------------------------------------------------------
// Purpose: Callback from Steam telling us about a server that has responded
//-----------------------------------------------------------------------------
void ServerMenu::ServerResponded( HServerListRequest hReq, int iServer )
{
    // Assert( hReq == m_hServerListRequest );
    std::cout << "DedicatedServer list" << std::endl;
    gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( hReq, iServer );
    if ( pServer )
    {
        // Filter out servers that don't match our appid here (might get these in LAN calls since we can't put more filters on it)
        if ( pServer->m_nAppID == SteamUtils()->GetAppID() )
        {
            m_ListGameServers.emplace_back( pServer );
            m_nServers++;
        }
    }

    // Rebuild menu
    Rebuild( m_ListGameServers, m_bRequestingServers );
}


//-----------------------------------------------------------------------------
// Purpose: Callback from Steam telling us about a server that has failed to respond
//-----------------------------------------------------------------------------
void ServerMenu::ServerFailedToRespond( HServerListRequest hReq, int iServer )
{
    // Assert( hReq == m_hServerListRequest );
    std::cout << "DedicatedServer failed to respond" << std::endl;
    gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( hReq, iServer );
    std::cout << pServer->m_szGameDescription << pServer->m_nPing << std::endl;
    // bugbug jmccaskey - why would we ever need this?  Remove servers from our list I guess?
}


//-----------------------------------------------------------------------------
// Purpose: Callback from Steam telling us a refresh is complete
//-----------------------------------------------------------------------------
void ServerMenu::RefreshComplete( HServerListRequest hReq, EMatchMakingServerResponse response )
{
    // Assert( hReq == m_hServerListRequest );
    std::cout << "Refresh complete" << std::endl;
    // Doesn't really matter to us whether the response tells us the refresh succeeded or failed,
    // we just track whether we are done refreshing or not
    m_bRequestingServers = false;
}

ServerMenu::ServerMenu() = default;

