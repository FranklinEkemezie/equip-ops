#include <format>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Ui.cppm"

namespace EquipOps
{

    Ui::Ui() { };


    void Ui::printMessage(std::string_view label, std::string_view message)
    {
        printnln(std::format("[{}] {}", label, message));
    }

    void Ui::printInfo(std::string_view message)
    {
        printMessage("INFO", message);
    }

    void Ui::printError(std::string_view message)
    {
        printMessage("ERROR", message);
    }

    void Ui::printnl()
    {
        std::cout << '\n';
    }

    void Ui::println(std::string_view text)
    {
        std::cout << text << '\n';
    }

    void Ui::printnln(std::string_view text)
    {
        std::cout << '\n' << text << '\n';
    }

    void Ui::printHeading(std::string_view text)
    {
        std::cout << std::format("=== {}", text) << std::endl;
    }

    void Ui::showPrompt(std::string_view label)
    {
        std::cout << std::format("{}: ", label);
    }

    std::string Ui::promptString(std::string_view label)
    {
        showPrompt(label);

        std::string buffer { };
        getline(std::cin, buffer);

        return buffer;
    }

    float Ui::promptFloat(std::string_view label)
    {
        while (true)
        {
            try { return std::stof(promptString(label)); }
            catch (const std::invalid_argument&) { printError("Invalid input"); }
            catch (const std::out_of_range&) { printError("Input is too large"); }
        }
    }

    int Ui::promptInt(std::string_view label)
    {
        while (true)
        {
            try { return std::stoi(promptString(label)); }
            catch (const std::invalid_argument&) { printError("Invalid input"); }
            catch (const std::out_of_range&) { printError("Input is too large"); }
        }
    }

    Ui::Option Ui::getOption(int option)
    {
        if (option < MIN_OPTION || option > MAX_OPTION)
        {
            throw std::out_of_range { std::format(
                "Value is outside of range: [{}, {}]", 
                Ui::MIN_OPTION, MAX_OPTION
            ) };
        }

        return static_cast<Option>(option);
    }

    Ui::Option Ui::promptOption(std::string_view label)
    {
        while (true)
        {
            try { return getOption(promptInt(label)); }
            catch (const std::out_of_range&) { printError("Invalid option."); }
        }
    }

    void Ui::showMenu(std::string_view menu)
    {
        printnln(menu);
    }

    void Ui::showMainMenu()
    {
        showMenu(MAIN_MENU);
    }

    Ui::Option Ui::getMainMenuOption()
    {
        showMainMenu();

        return promptOption("Choose option");
    }

    App::RecordType Ui::promptRecordType(std::string_view label)
    {
        printnln(RECORD_TYPE_MENU);

        return App::recordTypeFromInt(static_cast<int>(promptOption(label)) - 1);
    }

    std::pair<App::SearchFilter, int> Ui::promptSearchFilter(std::string_view label)
    {
        printnln(SEARCH_FILTER_MENU);

        auto option { static_cast<int>(promptOption(label)) };
        auto searchFilterInt { option != 0 ? option - 1 
            : static_cast<int>(App::SearchFilter::NONE) };
    
        auto searchFilter { App::searchFilterFromInt(searchFilterInt) };

        switch (searchFilter)
        {
            using enum App::SearchFilter;

            case CATEGORY: 
                return { 
                    searchFilter, 
                    static_cast<int>(promptEquipmentCategory()) 
                };
        
            case STATUS:
                return {
                    searchFilter,
                    static_cast<int>(promptEquipmentStatus())
                };

            case DEPARTMENT:
                return {
                    searchFilter,
                    static_cast<int>(promptEquipmentDepartment())
                };

            case NONE: return { searchFilter, 0 };
        }

        throw std::logic_error { "Unknown search filter" };
    }

    void Ui::showEquipmentCategoryMenu()
    {
        showMenu(EQUIPMENT_CATEGORY_MENU);
    }

    void Ui::showEquipmentDepartmentMenu()
    {
        showMenu(EQUIPMENT_DEPARTMENT_MENU);
    }

    void Ui::showEquipmentStatusMenu()
    {
        showMenu(EQUIPMENT_STATUS_MENU);
    }

    void Ui::showMaintenanceTypeOption()
    {
        showMenu(MAINTENANCE_TYPE_MENU);
    }

    Equipment::Category Ui::getEquipmentCategoryOption(Option option)
    {
        switch (static_cast<int>(option))
        {
            using enum Equipment::Category;

            case 1: return ELECTRIC_MOTOR;
            case 2: return HAND_TOOL;
            case 3: return CUTTING_TOOL;
            case 4: return WELDING_TOOL;
        }

        throw std::logic_error { "Unknown equipment category option" };
    }

    Equipment::Status Ui::getEquipmentStatusOption(Option option)
    {
        switch (static_cast<int>(option))
        {
            using enum Equipment::Status;

            case 1: return OPERATIONAL;
            case 2: return OUTSIDE_LIMITS;
            case 3: return UNDER_MAINTENANCE;
            case 4: return UNAVAILABLE;
        }

        throw std::logic_error { "Unknown equipment status option" };
    }

    Equipment::Department Ui::getEquipmentDepartmentOption(Option option)
    {
        switch (static_cast<int>(option))
        {
            using enum Equipment::Department;

            case 1: return ELECTRICAL;
            case 2: return MECHANICAL;
            case 3: return WELDING;
            case 4: return PAINTING;
        }

        throw std::logic_error { "Unknown equipment department option" };
    }

    Maintenance::Type Ui::getMaintenanceTypeOption(Option option)
    {
        switch (static_cast<int>(option))
        {
            using enum Maintenance::Type;

            case 1: return WEEKLY;
            case 2: return BIWEEKLY;
            case 3: return MONTHLY;
            case 4: return QUARTERLY;
            case 5: return ANNUALLY;
        }

        throw std::logic_error { "Unknown maintenance type" };
    }


    Equipment::Category Ui::promptEquipmentCategory()
    {
        showEquipmentCategoryMenu();

        while (true)
        {
            try 
            { 
                return getEquipmentCategoryOption(
                    promptOption("Choose equipment category"));
            }
            catch (const std::logic_error&) 
            {
                printError("Invalid equipment category");
            }
        }
    }

    Equipment::Status Ui::promptEquipmentStatus()
    {
        showEquipmentStatusMenu();

        while (true)
        {
            try 
            {
                return getEquipmentStatusOption(
                    promptOption("Choose equipment status"));
            }
            catch (const std::logic_error&) 
            { 
                printError("Invalid equipment status."); 
            }
        }
    }

    Equipment::Department Ui::promptEquipmentDepartment()
    {
        showEquipmentDepartmentMenu();

        while (true)
        {
            try 
            { 
                return getEquipmentDepartmentOption(
                    promptOption("Choose equipment department"));
            }
            catch (const std::invalid_argument&) 
            {
                printError("Invalid equipment department");
            }
        }
    }

    Maintenance::Type Ui::promptMaintenanceTypeOption()
    {
        showMaintenanceTypeOption();

        while (true)
        {
            try
            {
                return getMaintenanceTypeOption(
                    promptOption("Choose maintenance type"));
            }
            catch (const std::invalid_argument&)
            {
                printError("Invalid maintenance type");
            }
        }
    }

    void Ui::printEquipmentSummary(EquipmentRepository::Summary summary)
    {
        printnln(EquipmentRepository::summaryToString(summary));
    }

    
}