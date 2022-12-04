//
// Created by gusta on 13/11/2021.
//

#ifndef GAME_CELL_SERVER_H
#define GAME_CELL_SERVER_H


#include "../../steam/steamnetworkingtypes.h"
#include "../../steam/isteamnetworkingsockets.h"
#include "../../steam/isteamuser.h"
#include "../../steam/isteamgameserver.h"
#include "../../steam/steam_api_common.h"

#include "../../steam/steam_gameserver.h"


#define MAX_PLAYERS_PER_SERVER 6

// Forward declaration
class DedicatedServer;

struct ClientConnectionData_t{
    bool m_bActive{};					// Is this slot in use? Or is it available for new connections?
    CSteamID m_SteamIDUser;			// What is the steamid of the player?
    uint64 m_ulTickCountLastData{};	// What was the last time we got data from the player?
    HSteamNetConnection m_hConn{};	// The handle for the connection to the player//
};


class DedicatedServer
{
public:
    //Constructor
    DedicatedServer();

    // Destructor
    ~DedicatedServer();

    // Run a game frame
    void RunFrame();

    // Set game state
    //void SetGameState( EServerGameState eState );

    // Checks for any incoming network data, then dispatches it
    void ReceiveNetworkData();

    // Reset player scores (occurs when starting a new game)
    //void ResetScores();

    // Reset player positions (occurs in between rounds as well as at the start of a new game)
    //void ResetPlayerShips();

    // Checks various game objects for collisions and updates state appropriately if they have occurred
    //void CheckForCollisions();

    // Kicks a given player off the server
    //void KickPlayerOffServer( CSteamID steamID );

    // data accessors
    bool IsConnectedToSteam()		{ return m_bConnectedToSteam; }
    CSteamID GetSteamID();



private:
    //
    // Various callback functions that Steam will call to let us know about events related to our
    // connection to the Steam servers for authentication purposes.
    //

    // Tells us when we have successfully connected to Steam
    STEAM_GAMESERVER_CALLBACK( DedicatedServer, OnSteamServersConnected, SteamServersConnected_t, m_CallbackServersConnected );

    // Tells us when there was a failure to connect to Steam
    STEAM_GAMESERVER_CALLBACK( DedicatedServer, OnSteamServersConnectFailure, SteamServerConnectFailure_t, m_CallbackServersConnectFailure );

    // Tells us when we have been logged out of Steam
    STEAM_GAMESERVER_CALLBACK( DedicatedServer, OnSteamServersDisconnected, SteamServersDisconnected_t, m_CallbackServerDisconnected );

    // Tells us that Steam has set our security policy (VAC on or off)
    STEAM_GAMESERVER_CALLBACK( DedicatedServer, OnPolicyResponse, GSPolicyResponse_t, m_CallbackOnPolicyResponse );

    //
    // Various callback functions that Steam will call to let us know about whether we should
    // allow clients to play or we should kick/deny them.
    //

    // Tells us a client has been authenticated and approved to play by Steam (passes auth, license check, VAC status, etc...)
    STEAM_GAMESERVER_CALLBACK( DedicatedServer, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t, m_CallbackAuthTicketResponse );

    // client connection state
    // All connection changes are handled through this callback
    STEAM_GAMESERVER_CALLBACK(DedicatedServer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t, m_CallbackConnectionStatusChange);



    // Function to tell Steam about our servers details
    static void SendUpdatedServerDetailsToSteam();

    // Receive updates from client
    //void OnReceiveClientUpdateData( uint32 uShipIndex, ClientSpaceWarUpdateData_t *pUpdateData );

    // Send data to a client at the given ship index
    bool BSendDataToClient( uint32 uShipIndex, char *pData, uint32 nSizeOfData );

    // Send data to a client at the given pending index
    bool BSendDataToPendingClient( uint32 uShipIndex, char *pData, uint32 nSizeOfData );

    void OnClientBeginAuthentication(CSteamID steamIDClient, HSteamNetConnection connectionID, void* pToken, uint32 uTokenLen);
    // Handles authentication completing for a client
    void OnAuthCompleted( bool bAuthSuccess, uint32 iPendingAuthIndex );

    // Adds/initializes a new player ship at the given position
    //void AddPlayerShip( uint32 uShipPosition );

    // Removes a player from the server
    //void RemovePlayerFromServer( uint32 uShipPosition, EDisconnectReason reason);

    // Send world update to all clients
    //void SendUpdateDataToAllClients();

    // Send the same message to all clients, except the ignored connection if any
    void SendMessageToAll( HSteamNetConnection hConnIgnore, const void* pubData, uint32 cubData );

    // Track whether our server is connected to Steam ok (meaning we can restrict who plays based on
    // ownership and VAC bans, etc...)
    bool m_bConnectedToSteam{};

    // Ships for players, doubles as a way to check for open slots (pointer is NULL meaning open)
    //CShip *m_rgpShips[MAX_PLAYERS_PER_SERVER];

    // Player scores
    uint32 m_rguPlayerScores[MAX_PLAYERS_PER_SERVER]{};

    // server name
    std::string m_sServerName;

    // Who just won the game? Should be set if we go into the k_EGameWinner state
    uint32 m_uPlayerWhoWonGame{};

    // Last time state changed
    uint64 m_ulStateTransitionTime{};

    // Last time we sent clients an update
    uint64 m_ulLastServerUpdateTick{};

    // Number of players currently connected, updated each frame
    uint32 m_uPlayerCount{};

    // Current game state
    //EServerGameState m_eGameState;

    // Sun instance
    //CSun *m_pSun;

    // pointer to game engine instance we are running under
    //IGameEngine *m_pGameEngine;

    // Vector to keep track of client connections
    ClientConnectionData_t m_rgClientData[MAX_PLAYERS_PER_SERVER];

    // Vector to keep track of client connections which are pending auth
    ClientConnectionData_t m_rgPendingClientData[MAX_PLAYERS_PER_SERVER];

    // Socket to listen for new connections on
    HSteamListenSocket m_hListenSocket;

    // Poll group used to receive messages from all clients at once
    HSteamNetPollGroup m_hNetPollGroup;
};


#endif //GAME_CELL_SERVER_H
