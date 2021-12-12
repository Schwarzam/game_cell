//
// Created by gusta on 13/11/2021.
//

#include <iostream>
#include <ctime>
#include "DedicatedServer.h"

#define INADDR_ANY (long)0x00000000
#define MASTER_SERVER_UPDATER_PORT 27024
#define PORT 27023


DedicatedServer::DedicatedServer() :
    m_sServerName("DedicatedServer Teste"),
    m_CallbackServersConnected(this, &DedicatedServer::OnSteamServersConnected),
    m_CallbackServersConnectFailure(this, &DedicatedServer::OnSteamServersConnectFailure),
    m_CallbackServerDisconnected(this, &DedicatedServer::OnSteamServersDisconnected),
    m_CallbackOnPolicyResponse(this, &DedicatedServer::OnPolicyResponse),
    m_CallbackAuthTicketResponse(this, &DedicatedServer::OnValidateAuthTicketResponse),
    m_CallbackConnectionStatusChange(this, &DedicatedServer::OnNetConnectionStatusChanged)
    {
    uint32 unIP = INADDR_ANY;
    uint16 usMasterServerUpdaterPort = MASTER_SERVER_UPDATER_PORT;
    uint16 port = PORT;
    EServerMode eMode = eServerModeAuthenticationAndSecure;


    if (!SteamGameServer_Init(0, port, usMasterServerUpdaterPort, eMode, "0.1")){
        std::cout << "Could not start server" << std::endl;
    }else{

    }

    if(SteamGameServer()){
        SteamGameServer()->SetModDir("thecell");

        SteamGameServer()->SetServerName("The Cell DedicatedServer");
        SteamGameServer()->SetProduct( "TheCellServer" );
        SteamGameServer()->SetGameDescription( "The Cell matchmaking server" );

        SteamGameServer()->LogOnAnonymous();

        //SteamGameServer()->LogOn("");

        SteamNetworkingUtils()->InitRelayNetworkAccess();

        SteamGameServer()->SetAdvertiseServerActive( true );


        m_hListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);

        m_hNetPollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();
        std::cout << "Created server connection" << std::endl;
    }else{
        std::cout << "SteamGameServer() interface is invalid\n" << std::endl;
    }

    // zero the client connection data
    memset( &m_rgClientData, 0, sizeof( m_rgClientData ) );
    memset( &m_rgPendingClientData, 0, sizeof( m_rgPendingClientData ) );

    // Seed random num generator
    srand( (uint32)time( NULL ) );
}

DedicatedServer::~DedicatedServer() {
    std::cout << "DedicatedServer Destroyed" << std::endl;
    SteamGameServerNetworkingSockets()->CloseListenSocket(m_hListenSocket);
    SteamGameServerNetworkingSockets()->DestroyPollGroup(m_hNetPollGroup);

    // Disconnect from the steam servers
    SteamGameServer()->LogOff();

    // release our reference to the steam client library
    SteamGameServer_Shutdown();
}

void DedicatedServer::RunFrame() {
    SteamGameServer_RunCallbacks();
    SendUpdatedServerDetailsToSteam();
}

void DedicatedServer::SendUpdatedServerDetailsToSteam() {
    // to send the player count.  The player count is maintained by steam and you should use the player
    // creation/authentication functions to maintain your player count.
    SteamGameServer()->SetMaxPlayerCount( MAX_PLAYERS_PER_SERVER );
    SteamGameServer()->SetPasswordProtected( false );
    SteamGameServer()->SetServerName( "The cell DedicatedServer" );
    SteamGameServer()->SetBotPlayerCount( 0 ); // optional, defaults to zero
    SteamGameServer()->SetMapName( "d2" );
}

//-----------------------------------------------------------------------------
// Purpose: A new client that connected has had their authentication processed
//-----------------------------------------------------------------------------
void DedicatedServer::OnAuthCompleted( bool bAuthSuccessful, uint32 iPendingAuthIndex )
{
    std::cout << "TEI" << std::endl;
    if ( !m_rgPendingClientData[iPendingAuthIndex].m_bActive )
    {
        std::cout << "Got auth completed callback for client who is not pending\n" << std::endl;
        return;
    }

    if ( !bAuthSuccessful )
    {

        // Tell the GS the user is leaving the server
        SteamGameServer()->EndAuthSession( m_rgPendingClientData[iPendingAuthIndex].m_SteamIDUser );

        // Send a deny for the client, and zero out the pending data
        std::string msg = "Not possible to join!";
        int64 outMessage;
        SteamGameServerNetworkingSockets()->SendMessageToConnection(m_rgPendingClientData[iPendingAuthIndex].m_hConn, &msg, msg.size(), k_nSteamNetworkingSend_Reliable, &outMessage);
        memset( &m_rgPendingClientData[iPendingAuthIndex], 0, sizeof( ClientConnectionData_t ) );
        return;
    }

    bool bAddedOk = false;
    for( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i )
    {
        if ( !m_rgClientData[i].m_bActive )
        {
            // copy over the data from the pending array
            memcpy( &m_rgClientData[i], &m_rgPendingClientData[iPendingAuthIndex], sizeof( ClientConnectionData_t ) );
            memset( &m_rgPendingClientData[iPendingAuthIndex], 0, sizeof( ClientConnectionData_t	) );
            //m_rgClientData[i].m_ulTickCountLastData = m_pGameEngine->GetGameTickCount();

            // Add a new ship, make it dead immediately
            //AddPlayerShip( i );
            //m_rgpShips[i]->SetDisabled( true );

            //MsgServerPassAuthentication_t msg;
            //msg.SetPlayerPosition( i );
            //BSendDataToClient( i, (char*)&msg, sizeof( msg ) );

            bAddedOk = true;

            break;
        }
    }

    // If we just successfully added the player, check if they are #2 so we can restart the round
    if ( bAddedOk )
    {
        uint32 uPlayers = 0;
        for( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i )
        {
            if ( m_rgClientData[i].m_bActive )
                ++uPlayers;
        }

        // If we just got the second player, immediately reset round as a draw.  This will prevent
        // the existing player getting a win, and it will cause a new round to start right off
        // so that the one player can't just float around not letting the new one get into the game.
        if ( uPlayers == 2 )
        {
            //if ( m_eGameState != k_EServerWaitingForPlayers )
            //    SetGameState( k_EServerDraw );
        }
    }
}

void DedicatedServer::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t *pResponse) {
    std::cout << "TEI" << std::endl;
    if ( pResponse->m_eAuthSessionResponse == k_EAuthSessionResponseOK )
    {
        // This is the final approval, and means we should let the client play (find the pending auth by steamid)
        for ( uint32 i = 0; i<MAX_PLAYERS_PER_SERVER; ++i )
        {
            if ( !m_rgPendingClientData[i].m_bActive )
                continue;
            else if ( m_rgPendingClientData[i].m_SteamIDUser == pResponse->m_SteamID )
            {
                OnAuthCompleted( true, i );
                return;
            }
        }
    }
    else
    {
        // Looks like we shouldn't let this user play, kick them
        for ( uint32 i = 0; i<MAX_PLAYERS_PER_SERVER; ++i )
        {
            if ( !m_rgPendingClientData[i].m_bActive )
                continue;
            else if ( m_rgPendingClientData[i].m_SteamIDUser == pResponse->m_SteamID )
            {
                OnAuthCompleted( false, i );
                return;
            }
        }
    }
}

void DedicatedServer::OnPolicyResponse( GSPolicyResponse_t *pPolicyResponse )
{
    // Check if we were able to go VAC secure or not
    if ( SteamGameServer()->BSecure() )
    {
        std::cout <<  "Thecell is VAC Secure! " << pPolicyResponse->m_bSecure << "\n";
    }
    else
    {
        std::cout <<  ( "Thecell is not VAC Secure!\n" );
    }
    std::cout << SteamGameServer_GetSteamID() << std::endl;
    //GetSteamID();
}

//-----------------------------------------------------------------------------
// Purpose: Handle any connection status change
//-----------------------------------------------------------------------------
void DedicatedServer::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
    std::cout << "TEI" << std::endl;
    /// Connection handle
    HSteamNetConnection hConn = pCallback->m_hConn;

    /// Full connection info
    SteamNetConnectionInfo_t info = pCallback->m_info;

    /// Previous state.  (Current state is in m_info.m_eState)
    ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;

    // Parse information to know what was changed

    // Check if a client has connected
    if (info.m_hListenSocket &&
    eOldState == k_ESteamNetworkingConnectionState_None &&
    info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
    {
        // Connection from a new client
        // Search for an available slot
        for (uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i)
        {
            if (!m_rgClientData[i].m_bActive && !m_rgPendingClientData[i].m_hConn)
            {

                // Found one.  "Accept" the connection.
                EResult res = SteamGameServerNetworkingSockets()->AcceptConnection( hConn );
                if ( res != k_EResultOK )
                {
                    char msg[ 256 ];
                    sprintf( msg, "AcceptConnection returned %d", res );
                    std::cout << ( msg );
                    SteamGameServerNetworkingSockets()->CloseConnection( hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false );
                    return;
                }

                m_rgPendingClientData[i].m_hConn = hConn;

                // add the user to the poll group
                SteamGameServerNetworkingSockets()->SetConnectionPollGroup(hConn, m_hNetPollGroup);

                //Send them the server info as a reliable message
//                MsgServerSendInfo_t msg;
//                msg.SetSteamIDServer(SteamGameServer()->GetSteamID().ConvertToUint64());
//
//                // You can only make use of VAC when using the Steam authentication system
//                msg.SetSecure(SteamGameServer()->BSecure());
//
//                msg.SetServerName(m_sServerName.c_str());
//                SteamGameServerNetworkingSockets()->SendMessageToConnection( hConn, &msg, sizeof(MsgServerSendInfo_t), k_nSteamNetworkingSend_Reliable, nullptr );

                return;
            }
        }

        // No empty slots.  DedicatedServer full!
        std::cout << ("Rejecting connection; server full") << std::endl;
        SteamGameServerNetworkingSockets()->CloseConnection( hConn, k_ESteamNetConnectionEnd_AppException_Generic, "DedicatedServer full!", false );
    }
    // Check if a client has disconnected
    else if ((eOldState == k_ESteamNetworkingConnectionState_Connecting || eOldState == k_ESteamNetworkingConnectionState_Connected) &&
    info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
    {
        // Handle disconnecting a client
        for (uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i)
        {
            // If there is no ship, skip
            if (!m_rgClientData[i].m_bActive)
                continue;

            if (m_rgClientData[i].m_SteamIDUser == info.m_identityRemote.GetSteamID())//pCallback->m_steamIDRemote)
                {
                std::cout<< ("Disconnected dropped user\n");
                //RemovePlayerFromServer(i, EDisconnectReason::k_EDRClientDisconnect);
                break;
                }
        }
    }
}


//-----------------------------------------------------------------------------
// Purpose: Called when we were previously logged into steam but get logged out
//-----------------------------------------------------------------------------
void DedicatedServer::OnSteamServersDisconnected( SteamServersDisconnected_t *pLoggedOff )
{
    m_bConnectedToSteam = false;
    std::cout << ( "Thecell got logged out of Steam\n" );
}


//-----------------------------------------------------------------------------
// Purpose: Called when an attempt to login to Steam fails
//-----------------------------------------------------------------------------
void DedicatedServer::OnSteamServersConnectFailure( SteamServerConnectFailure_t *pConnectFailure ){
    m_bConnectedToSteam = false;
    std::cout << ( "Thecell failed to connect to Steam\n" );
}

//-----------------------------------------------------------------------------
// Purpose: Take any action we need to on Steam notifying us we are now logged in
//-----------------------------------------------------------------------------
void DedicatedServer::OnSteamServersConnected( SteamServersConnected_t *pLogonSuccess ){
    std::cout << ( "Thecell connected to Steam successfully\n" );
    m_bConnectedToSteam = true;

    // log on is not finished until OnPolicyResponse() is called

    // Tell Steam about our server details
    SendUpdatedServerDetailsToSteam();
}

void DedicatedServer::ReceiveNetworkData() {
    std::cout << "ReceiveNetworkData" << std::endl;
}

CSteamID DedicatedServer::GetSteamID(){
    std::cout << "GetSteamID" << std::endl;
    return SteamGameServer()->GetSteamID();
}

void DedicatedServer::OnClientBeginAuthentication(CSteamID steamIDClient, HSteamNetConnection connectionID, void *pToken,
                                         uint32 uTokenLen) {
    std::cout << "TEI" << std::endl;
}

bool DedicatedServer::BSendDataToClient(uint32 uShipIndex, char *pData, uint32 nSizeOfData) {
    std::cout << "TEI" << std::endl;
    return false;
}

bool DedicatedServer::BSendDataToPendingClient(uint32 uShipIndex, char *pData, uint32 nSizeOfData) {
    std::cout << "TEI" << std::endl;
    return false;
}

void DedicatedServer::SendMessageToAll(HSteamNetConnection hConnIgnore, const void *pubData, uint32 cubData) {
    std::cout << "TEI" << std::endl;
}

