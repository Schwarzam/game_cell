//
// Created by gusta on 08/12/2021.
//

#ifndef THECELL_MESSAGE_H
#define THECELL_MESSAGE_H

#include <map>

#pragma pack( push, 1 )

typedef uint32 DWORD;
typedef DWORD HWND;
typedef DWORD HINSTANCE;
#define LittleInt16( val )	WordSwap( val )
#define LittleWord( val )	WordSwap( val )
#define LittleInt32( val )	DWordSwap( val )
#define LittleDWord( val )	DWordSwap( val )
#define LittleQWord( val )	QWordSwap( val )
#define LittleFloat( val )	DWordSwap( val )

template <typename T>
inline T WordSwap( T w )
{
    uint16 temp;

    temp  = ((*((uint16 *)&w) & 0xff00) >> 8);
    temp |= ((*((uint16 *)&w) & 0x00ff) << 8);

    return *((T*)&temp);
}

template <typename T>
inline T DWordSwap( T dw )
{
    uint32 temp;

    temp  =   *((uint32 *)&dw)               >> 24;
    temp |= ((*((uint32 *)&dw) & 0x00FF0000) >> 8);
    temp |= ((*((uint32 *)&dw) & 0x0000FF00) << 8);
    temp |= ((*((uint32 *)&dw) & 0x000000FF) << 24);

    return *((T*)&temp);
}

template <typename T>
inline T QWordSwap( T dw )
{
    uint64 temp;

    temp  =   *((uint64 *)&dw)                          >> 56;
    temp |= ((*((uint64 *)&dw) & 0x00FF000000000000ull) >> 40);
    temp |= ((*((uint64 *)&dw) & 0x0000FF0000000000ull) >> 24);
    temp |= ((*((uint64 *)&dw) & 0x000000FF00000000ull) >> 8);
    temp |= ((*((uint64 *)&dw) & 0x00000000FF000000ull) << 8);
    temp |= ((*((uint64 *)&dw) & 0x0000000000FF0000ull) << 24);
    temp |= ((*((uint64 *)&dw) & 0x000000000000FF00ull) << 40);
    temp |= ((*((uint64 *)&dw) & 0x00000000000000FFull) << 56);

    return *((T*)&temp);
}



// Network message types
enum EMessage
        {
    // Server messages
    k_EMsgServerBegin = 0,
    k_EMsgServerSendInfo = k_EMsgServerBegin+1,
    k_EMsgServerFailAuthentication = k_EMsgServerBegin+2,
    k_EMsgServerPassAuthentication = k_EMsgServerBegin+3,
    k_EMsgServerUpdateWorld = k_EMsgServerBegin+4,
    k_EMsgServerExiting = k_EMsgServerBegin+5,
    k_EMsgServerPingResponse = k_EMsgServerBegin+6,

    // Client messages
    k_EMsgClientBegin = 500,
    k_EMsgClientBeginAuthentication = k_EMsgClientBegin+2,
    k_EMsgClientSendLocalUpdate = k_EMsgClientBegin+3,

    // P2P authentication messages
    k_EMsgP2PBegin = 600,
    k_EMsgP2PSendingTicket = k_EMsgP2PBegin+1,

    // voice chat messages
    k_EMsgVoiceChatBegin = 700,
    //k_EMsgVoiceChatPing = k_EMsgVoiceChatBegin+1,	// deprecated keep alive message
    k_EMsgVoiceChatData = k_EMsgVoiceChatBegin+2,	// voice data from another player



    // force 32-bit size enum so the wire protocol doesn't get outgrown later
    k_EForceDWORD  = 0x7fffffff,
    };

// enums for use in
enum EDisconnectReason
        {
    k_EDRClientDisconnect = ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Min + 1,
    k_EDRServerClosed = ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Min + 2,
    k_EDRServerReject = ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Min + 3,
    k_EDRServerFull = ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Min + 4,
    k_EDRClientKicked = ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Min + 5
        };


// Msg from the server to the client which is sent right after communications are established
// and tells the client what SteamID the game server is using as well as whether the server is secure
struct MsgServerSendInfo_t
        {
    MsgServerSendInfo_t() : m_dwMessageType( LittleDWord( k_EMsgServerSendInfo ) ) {}
    DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }

    void SetSteamIDServer( uint64 SteamID ) { m_ulSteamIDServer = LittleQWord( SteamID ); }
    uint64 GetSteamIDServer() { return LittleQWord( m_ulSteamIDServer ); }

    void SetSecure( bool bSecure ) { m_bIsVACSecure = bSecure; }
    bool GetSecure() { return m_bIsVACSecure; }

    void SetServerName( const char *pchName ) { strncpy_s( m_rgchServerName, pchName, sizeof( m_rgchServerName ) ); }
    const char *GetServerName() { return m_rgchServerName; }

        private:
            const DWORD m_dwMessageType;
    uint64 m_ulSteamIDServer;
    bool m_bIsVACSecure;
    char m_rgchServerName[128];
        };


#pragma pack( pop )


#endif //THECELL_MESSAGE_H
