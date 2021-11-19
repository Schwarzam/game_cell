#include <iostream>
#include "steam/steam_api.h"

S_API bool S_CALLTYPE SteamAPI_Init();

int main() {


    std::cout << SteamAPI_Init() << std::endl;
}
