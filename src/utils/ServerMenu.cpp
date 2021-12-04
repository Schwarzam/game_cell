//
// Created by gusta on 04/12/2021.
//
#define ARRAYSIZE(A) ( sizeof(A)/sizeof(A[0]) )
#include "ServerMenu.h"

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