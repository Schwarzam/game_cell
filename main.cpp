#include <iostream>
#include "steam/steam_api.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "src/GameManager.h"
#include "src/Game.h"

int main() {
    if ( SteamAPI_RestartAppIfNecessary( 1826930 ) ) // Substitua k_uAppIdInvalid com o AppID do seu aplicativo
        {
        return 1;
        }

    if ( !SteamAPI_Init() )
    {
        printf( "Erro fatal - O Steam precisa estar em execução para jogar (SteamAPI_Init() falhou).\n" );
        return 1;
    }

    SteamAPI_SetMiniDumpComment("Dump of The Cell");
    ISteamFriends * psteamfriends = SteamFriends();
    std::cout << psteamfriends->GetPersonaName() << std::endl;

    Game game;
    game.runWithMinimumTimeSteps();

    return 0;
}
