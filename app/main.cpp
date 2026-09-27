#include <format>
#include <iostream>

#include "Database.cppm"
#include "MainMenuHandler.cppm"

using namespace EquipOps;

int main()
{

    Ui ui { };
    Database db { };
    MainMenuHandler mainMenuHandler { db };

    while (true)
    {
        try { mainMenuHandler.handleMainMenuOption(ui.getMainMenuOption()); }
        catch(const std::logic_error&) { ui.printError("Invalid option."); }
        catch(const std::exception& e)
        {
            ui.printError(std::format("An error occurred: {}", e.what()));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;

}