#include <iostream>
#include "steam/steam_api.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
                SteamFriends()->ActivateGameOverlay( "friends" );

            SteamAPI_RunCallbacks();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;

}
