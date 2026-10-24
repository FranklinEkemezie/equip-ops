#include <format>
#include <iostream>

#include "Database.cppm"
#include "MainMenuHandler.cppm"

using namespace EquipOps;

int main()
{

    Database db { };
    MainMenuHandler mainMenuHandler { db };

    while (true)
    {
        try { 

            auto result { Ui::getMainMenuOption() };
            if (! result)
            {
                Ui::printError("Something went wrong");
                return EXIT_SUCCESS;
            }

            auto option { result.value() };

            mainMenuHandler.handleMainMenuOption(option); 

            if (static_cast<int>(option) == 0) return EXIT_SUCCESS;
        }
        catch(const std::logic_error&) { Ui::printError("Invalid option."); }
        catch(const std::exception& e)
        {
            Ui::printError(std::format("An error occurred: {}", e.what()));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;

}