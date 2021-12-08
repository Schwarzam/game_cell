//
// Created by gusta on 13/11/2021.
//

#ifndef GAME_CELL_SERVER_H
#define GAME_CELL_SERVER_H


#include "../steam/steamnetworkingtypes.h"
#include "../steam/isteamnetworkingsockets.h"
#include "../steam/isteamuser.h"
#include "../steam/isteamgameserver.h"
#include "../steam/steam_api_common.h"

#define MAX_PLAYERS_PER_SERVER 6

// Forward declaration
class Server;

struct ClientConnectionData_t{
    bool m_bActive;					// Is this slot in use? Or is it available for new connections?
    CSteamID m_SteamIDUser;			// What is the steamid of the player?
    uint64 m_ulTickCountLastData;	// What was the last time we got data from the player?
    HSteamNetConnection m_hConn;	// The handle for the connection to the player//
};


class Server {
public:
    Server();
    ~Server();

    void SendUpdatedServerDetailsToSteam();


    void run();

    bool IsConnectedToSteam()		{ return m_bConnectedToSteam; }
    CSteamID GetSteamID();

private:
    // Socket to listen for new connections on
    HSteamListenSocket m_hListenSocket;

    // Poll group used to receive messages from all clients at once
    HSteamNetPollGroup m_hNetPollGroup;

    //
    // Various callback functions that Steam will call to let us know about events related to our
    // connection to the Steam servers for authentication purposes.
    //


    // Tells us when we have successfully connected to Steam
    STEAM_GAMESERVER_CALLBACK( Server, OnSteamServersConnected, SteamServersConnected_t );

    // Tells us when there was a failure to connect to Steam
    STEAM_GAMESERVER_CALLBACK( Server, OnSteamServersConnectFailure, SteamServerConnectFailure_t );

    // Tells us when we have been logged out of Steam
    STEAM_GAMESERVER_CALLBACK( Server, OnSteamServersDisconnected, SteamServersDisconnected_t );

    // Tells us that Steam has set our security policy (VAC on or off)
    STEAM_GAMESERVER_CALLBACK( Server, OnPolicyResponse, GSPolicyResponse_t );

    //
    // Various callback functions that Steam will call to let us know about whether we should
    // allow clients to play or we should kick/deny them.
    //

    // Tells us a client has been authenticated and approved to play by Steam (passes auth, license check, VAC status, etc...)
    STEAM_GAMESERVER_CALLBACK( Server, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t );

    // client connection state
    // All connection changes are handled through this callback
    STEAM_GAMESERVER_CALLBACK( Server, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);


    // Vector to keep track of client connections
    ClientConnectionData_t m_rgClientData[MAX_PLAYERS_PER_SERVER];

    // Vector to keep track of client connections which are pending auth
    ClientConnectionData_t m_rgPendingClientData[MAX_PLAYERS_PER_SERVER];

    void OnAuthCompleted(bool bAuthSuccessful, uint32 iPendingAuthIndex);

    // Track whether our server is connected to Steam ok (meaning we can restrict who plays based on
    // ownership and VAC bans, etc...)
    bool m_bConnectedToSteam;

};


#endif //GAME_CELL_SERVER_H
