#ifndef EQUIP_OPS_UI_CPPM
#define EQUIP_OPS_UI_CPPM

#include <string>

#include "App.cppm"
#include "Equipment.cppm"
#include "Maintenance.cppm"

namespace EquipOps
{


    class Ui
    {

        public:

            using Option = std::byte;

            Ui();

            static void printMessage(std::string_view label, std::string_view message);
            static void printInfo(std::string_view message);
            static void printError(std::string_view message);

            static void printnl();
            static void println(std::string_view text);
            static void printnln(std::string_view text);
            static void printHeading(std::string_view text);

            static void showPrompt(std::string_view label);
            static std::string promptString(std::string_view label);
            static float promptFloat(std::string_view label);
            static int promptInt(std::string_view label);

            static Option getOption(int option);
            static Option promptOption(std::string_view label);
            static void showMenu(std::string_view menu);

            static void showMainMenu();
            static Option getMainMenuOption();

            static App::RecordType promptRecordType(std::string_view label);
            static std::pair<App::SearchFilter, int> promptSearchFilter(std::string_view label);

            static void showEquipmentCategoryMenu();
            static void showEquipmentStatusMenu();
            static void showEquipmentDepartmentMenu();

            // Following `isValid*` methods should be static methods in `Equipment` class. 
            // Better still, as methods that try to obtain the right enum case from int.

            static Equipment::Category getEquipmentCategoryOption(Option option);
            static Equipment::Status getEquipmentStatusOption(Option option);
            static Equipment::Department getEquipmentDepartmentOption(Option option);

            // Just like the methods above, the methods below couple `Ui` and
            // `Employee` classes tightly. But...

            static Equipment::Category promptEquipmentCategory();
            static Equipment::Status promptEquipmentStatus();
            static Equipment::Department promptEquipmentDepartment();

            static void showMaintenanceTypeOption();
            static Maintenance::Type getMaintenanceTypeOption(Option option);
            static Maintenance::Type promptMaintenanceTypeOption();

            static void printEquipmentSummary(EquipmentRepository::Summary summary);

        private:

            static constexpr int MIN_OPTION { 0 };
            static constexpr int MAX_OPTION { 100 };

            static inline std::string MAIN_MENU 
            {
                "1) Register equipment.\n"
                "2) View equipment information.\n"
                "3) Search for an equipment.\n"
                "4) Update equipment status.\n"
                "5) Remove equipment.\n"
                "6) Record maintenance activity.\n"
                "7) Record equipment inspection.\n"
                "8) Search workshop inventory.\n"
                "9) Display inventory summary.\n"
                "0) Exit the application.\n"
            };

            static inline std::string EQUIPMENT_CATEGORY_MENU
            {
                "1) Electric Motor\n"
                "2) Hand Tool\n"
                "3) Cutting Tool\n"
                "4) Welding Tool\n"
            };

            static inline std::string EQUIPMENT_DEPARTMENT_MENU
            {
                "1) Electrical\n"
                "2) Mechanical\n"
                "3) Welding\n"
                "4) Painting\n"
            };

            static inline std::string EQUIPMENT_STATUS_MENU
            {
                "1) Operational\n"
                "2) Outside Limits\n"
                "3) Under Maintenance\n"
                "4) Unavailable\n"
            };

            static inline std::string MAINTENANCE_TYPE_MENU
            {
                "1) WEEKLY\n"
                "2) BIWEEKLY\n"
                "3) MONTHLY\n"
                "4) QUARTERLY\n"
                "5) ANNUALLY\n"
            };

            static inline std::string RECORD_TYPE_MENU
            {
                "1) Equipment\n"
                "2) Inspection\n"
                "3) Maintenance\n"
            };

            static inline std::string SEARCH_FILTER_MENU
            {
                "1) Category\n"
                "2) Status\n"
                "3) Department\n"
                "0) None\n"
            };

    };

}