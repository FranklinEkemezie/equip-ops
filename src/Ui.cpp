#include <iostream>
#include <limits>
#include <print>
#include <stdexcept>

#include "Prompt.cppm"
#include "Ui.cppm"

namespace EquipOps
{


    void Ui::print(std::string_view text)
    {
        std::print("{}", text);
    }

    void Ui::printnl()
    {
        std::println();
    }

    void Ui::println(std::string_view text)
    {
        std::println("{}", text);
    }

    void Ui::printnln(std::string_view text)
    {
        std::println("\n{}\n", text);
    }

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

    void Ui::printHeading(std::string_view text)
    {
        std::println("=== {}", text);
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

    std::optional<Ui::Option> Ui::promptMainMenuOption(std::string_view label)
    {
        while (true)
        {
            try {
                auto result { Prompt::promptInt(label) };
                if (result.status == Prompt::Status::FAILURE) return std::nullopt;

                return getOption(result.value);
            }
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

    std::optional<Ui::Option> Ui::getMainMenuOption()
    {
        showMainMenu();

        return promptMainMenuOption("Choose option");
    }

    std::optional<App::RecordType> Ui::promptRecordType(std::string_view label)
    {
        printnln(RECORD_TYPE_MENU);

        auto result { promptMainMenuOption(label) };
        if (! result) return std::nullopt;

        return App::recordTypeFromInt(static_cast<int>(result.value()) - 1);
    }

    std::optional<std::pair<App::SearchFilter, int>> Ui::promptSearchFilter(std::string_view label)
    {
        printnln(SEARCH_FILTER_MENU);

        auto result { promptMainMenuOption(label) };
        if (! result) return std::nullopt;

        auto option { static_cast<int>(result.value()) };
        auto searchFilterInt { option != 0 ? option - 1 
            : static_cast<int>(App::SearchFilter::ALL) };
    
        auto searchFilter { App::searchFilterFromInt(searchFilterInt) };

        switch (searchFilter)
        {
            using enum App::SearchFilter;

            case CATEGORY:
            {
                auto categoryOption { promptEquipmentCategory() };
                if (! categoryOption) return std::nullopt;

                return std::pair { searchFilter, static_cast<int>(categoryOption.value()) };
            }
        
            case STATUS:
            {
                auto statusOption { promptEquipmentStatus() };
                if (! statusOption) return std::nullopt;

                return std::pair { searchFilter, static_cast<int>(statusOption.value()) };
            }

            case DEPARTMENT:
            {
                auto departmentOption { promptEquipmentDepartment() };
                if (! departmentOption) return std::nullopt;

                return std::pair { searchFilter, static_cast<int>(departmentOption.value()) };
            }

            case ALL: return std::pair { searchFilter, 0 };
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

    /**
     * @throws: `std::logic_error` - `option` does not match any `Equipment::Category` enum case
     */
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

    /**
     * @throws: `std::logic_error` - `option` does not match any `Equipment::Status` enum case
     */
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

    /**
     * @throws: `std::logic_error` - `option` does not match any `Equipment::Department` enum case
     */
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

    /**
     * @throws: `std::logic_error` - `option` does not match any `Maintenance::Type` enum case
     */
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


    std::optional<Equipment::Category> Ui::promptEquipmentCategory()
    {
        showEquipmentCategoryMenu();

        while (true)
        {
            try 
            { 
                auto result { promptMainMenuOption("Choose equipment category") };
                if (! result) return std::nullopt;

                return getEquipmentCategoryOption(result.value());
            }
            catch (const std::logic_error&) 
            {
                printError("Invalid equipment category");
            }
        }
    }

    std::optional<Equipment::Status> Ui::promptEquipmentStatus()
    {
        showEquipmentStatusMenu();

        while (true)
        {
            try 
            {
                auto result { promptMainMenuOption("Choose equipment status") };
                if (! result) return std::nullopt;

                return getEquipmentStatusOption(result.value());
            }
            catch (const std::logic_error&) 
            { 
                printError("Invalid equipment status."); 
            }
        }
    }

    std::optional<Equipment::Department> Ui::promptEquipmentDepartment()
    {
        showEquipmentDepartmentMenu();

        while (true)
        {
            try 
            {
                auto result { promptMainMenuOption("Choose equipment department") };
                if (! result) return std::nullopt;

                return getEquipmentDepartmentOption(result.value());
            }
            catch (const std::logic_error&) 
            {
                printError("Invalid equipment department");
            }
        }
    }

    std::optional<Maintenance::Type> Ui::promptMaintenanceTypeOption()
    {
        showMaintenanceTypeOption();

        while (true)
        {
            try
            {
                auto result { promptMainMenuOption("Choose maintenance type") };
                if (! result) return std::nullopt;

                return getMaintenanceTypeOption(result.value());
            }
            catch (const std::logic_error&)
            {
                printError("Invalid maintenance type");
            }
        }
    }

    
}