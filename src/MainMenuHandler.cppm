#ifndef EQUIP_OPS_MAIN_MENU_HANDLER_CPPM
#define EQUIP_OPS_MAIN_MENU_HANDLER_CPPM

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