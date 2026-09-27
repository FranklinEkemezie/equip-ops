#ifndef __MainMenuHandler_cppm
#define __MainMenuHandler_cppm

#include "App.cppm"
#include "Database.cppm"
#include "Ui.cppm"

namespace EquipOps
{

    class MainMenuHandler
    {

        public:

            MainMenuHandler(Database& db);

            void handleMainMenuOption(Ui::Option option);

        private:
            void handleRegisterEquipment();
            void handleViewEquipment();
            void handleSearchEquipment();
            void handleUpdateEquipment();
            void handleRemoveEquipment();
            void handleRecordMaintenanceActivity();
            void handleRecordEquipmentInspection();
            void handleSearchWorkshopInventory();
            void handleDisplayInventorySummary();
            void handleExitApplication();

        private:
            Database& db;
    };
}

#endif