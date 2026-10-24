#include <format>

#include "MainMenuHandler.cppm"
#include "Prompt.cppm"

namespace EquipOps
{


    MainMenuHandler::MainMenuHandler(Database& db): db { db } { };


    /**
     * @throws `std::logic_error` - `option` is an unsupported menu option.
     */
    void MainMenuHandler::handleMainMenuOption(Ui::Option option)
    {
        switch (static_cast<int>(option))
        {
            case 1: return handleRegisterEquipment();
            case 2: return handleViewEquipment();
            case 3: return handleSearchEquipment();
            case 4: return handleUpdateEquipment();
            case 5: return handleRemoveEquipment();
            case 6: return handleRecordMaintenanceActivity();
            case 7: return handleRecordEquipmentInspection();
            case 8: return handleSearchWorkshopInventory();
            case 9: return handleDisplayInventorySummary();
            case 0: return handleExitApplication();
        }

        throw std::logic_error { "Invalid menu option" };
    }

    void MainMenuHandler::handleRegisterEquipment()
    {

        Ui::printHeading("Please enter equipment details");

        auto id         { Prompt::promptString("Equipment ID") };
        auto name       { Prompt::promptString("Equipment Name") };
        auto category   { Ui::promptEquipmentCategory() };
        auto department { Ui::promptEquipmentDepartment() };
        auto status     { Ui::promptEquipmentStatus() };

        using enum Prompt::Status;

        if (
            id.status   == FAILURE || 
            name.status == FAILURE || 
            ! category || ! department || ! status
        )
        {
            Ui::printError("An error occurred while registering equipment. Please try again");
            return;
        }

        Equipment e 
        { 
            id.value, 
            name.value, 
            category.value(), 
            department.value(), 
            status.value() 
        };

        try 
        { 
            db.equipmentData().registerEquipment(e);

            Ui::printInfo("Equipment registered successfully!");
        }
        catch (const std::invalid_argument& e) 
        { 
            Ui::printError(e.what()); 
        }
    }

    void MainMenuHandler::handleViewEquipment()
    {
        auto result { Prompt::promptString("Enter equipment ID") };
        if (result.status == Prompt::Status::FAILURE)
        {
            Ui::printError("Something went wrong. Please try again");
            return;
        }

        auto equipmentId { result.value };

        try
        {
            auto& e { db.equipmentData().findEquipmentById(equipmentId) };

            Ui::printInfo("Equipment found");
            Ui::printnln(e.toString());
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
        }
    }

    void MainMenuHandler::handleSearchEquipment()
    {
        
        auto result { Prompt::promptString("Enter equipment name") };
        if (result.status == Prompt::Status::FAILURE)
        {
            Ui::printError("Something went wrong. Please try again");
            return;
        }

        auto equipmentName { result.value };

        auto items { db.equipmentData().findAllEquipment(equipmentName) };
        if (items.empty())
        {
            Ui::printError(std::format(
                "Could not find any equipment with name: {}", equipmentName));
            return;
        }

        Ui::printError(std::format(
            "Found {} equipment with name: {}", items.size(), equipmentName));

        EquipmentRepository::printEquipmentList(items);
        
    }

    void MainMenuHandler::handleUpdateEquipment()
    {
        auto equipmentId { Prompt::promptString("Enter equipment ID") };
        if (equipmentId.status == Prompt::Status::FAILURE)
        {
            Ui::printError("Something went wrong");
            return;
        }

        auto equipmentStatus { Ui::promptEquipmentStatus() };
        if (! equipmentStatus)
        {
            Ui::printError("Something went wrong");
            return;
        }

        try
        {
            db.equipmentData()
                .findEquipmentById(equipmentId.value)
                .setStatus(equipmentStatus.value());

            Ui::printInfo("Status updated successfully.");
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId.value));
        }
        
    }

    void MainMenuHandler::handleRemoveEquipment()
    {
        auto result { Prompt::promptString("Enter equipment ID") };
        if (result.status == Prompt::Status::FAILURE)
        {
            Ui::printError("Something went wrong");
            return;
        }

        auto equipmentId { result.value };
        auto e { db.deleteEquipment(equipmentId) };

        if (! e.has_value())
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
            return;
        }

        Ui::printInfo(std::format(
            "Equipment '{}' removed successfully", e.value().getName()));
        
    }

    void MainMenuHandler::handleRecordMaintenanceActivity()
    {
        auto equipmentId { Prompt::promptString("Enter equipment ID") };
        if (equipmentId.status == Prompt::Status::FAILURE)
        {
            Ui::printError("Something went wrong");
            return;
        }

        try
        {

            auto& e { db.equipmentData().findEquipmentById(equipmentId.value) };

            Ui::printHeading("Please enter maintenance activity details");

            auto maintenanceTypeOption { Ui::promptMaintenanceTypeOption() };
            auto description { Prompt::promptString("Description") };
            auto equipmentStatus { Ui::promptEquipmentStatus() };

            if (
                ! maintenanceTypeOption || ! equipmentStatus ||
                description.status == Prompt::Status::FAILURE  
            )
            {
                Ui::printError("Something went wrong");
                return;
            }

            Maintenance m
            {
                e,
                maintenanceTypeOption.value(),
                description.value,
                equipmentStatus.value()
            };

            db.maintenanceData().recordMaintenance(m);
            e.setStatus(m.statusAfterMaintenance());

            Ui::printInfo("Maintenance activity recorded successfully!");
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId.value));
        }
    }

    void MainMenuHandler::handleRecordEquipmentInspection()
    {
        using enum Prompt::Status;

        auto result { Prompt::promptString("Enter equipment ID") };
        if (result.status == FAILURE)
        {
            Ui::printError("Something went wrong");
            return;
        }

        auto equipmentId { result.value };

        try
        {
            auto& e { db.equipmentData().findEquipmentById(equipmentId) };
    
            Inspection inspection { e.getId() };

            Ui::printHeading("Please enter inspection records below");    
            Ui::printInfo("Press 'Enter' to skip if inspection data is unavailable for the field");

            auto temperature { Prompt::promptDouble("Temperature", false) };
            if (temperature.status == FAILURE) { Ui::printError("Something went wrong"); return; }
            if (temperature.status != EMPTY) inspection.setTemperature(temperature.value);

            auto vibration { Prompt::promptDouble("Vibration", false) };
            if (vibration.status == FAILURE) { Ui::printError("Something went wrong"); return; }
            if (vibration.status != EMPTY) inspection.setVibration(vibration.value);

            auto operatingHours { Prompt::promptUInt("Operating Hours", false) };
            if (operatingHours.status == FAILURE) { Ui::printError("Something went wrong"); return; }
            if (operatingHours.status != EMPTY) inspection.setOperatingHours(operatingHours.value);

            db.inspectionsData().recordInspection(inspection);

            auto newStatus { inspection.getEquipmentOperatingStatus() };
            if (newStatus.has_value()) e.setStatus(newStatus.value());

            Ui::printInfo("Inspection recorded successfully!");
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
        }

    }

    void MainMenuHandler::handleSearchWorkshopInventory()
    {

        auto recordType { Ui::promptRecordType("Which record do you want to search?") };
        if (! recordType)
        {
            Ui::printError("Something went wrong");
            return;
        }

        switch (recordType.value())
        {
            using enum App::RecordType;

            case EQUIPMENT:
            {
                auto searchFilterResult { Ui::promptSearchFilter("Filter equipment records by") };
                if (! searchFilterResult)
                {
                    Ui::printError("Something went wrong");
                    return;
                }

                auto [searchFilter, value] { searchFilterResult.value() };
                auto result { db.equipmentData().findAllEquipment(searchFilter, value) };

                if (result.empty()) return Ui::printInfo("No equipment record found");

                Ui::printnln(std::format("Found {} equipment records", result.size()));
                EquipmentRepository::printEquipmentList(result);
                
                return;
            }

            case INSPECTION:
            {
                auto searchFilterResult { Ui::promptSearchFilter("Filter inspection records by") };
                if (! searchFilterResult)
                {
                    Ui::printError("Something went wrong");
                    return;
                }

                auto [searchFilter, value] { searchFilterResult.value() };
                auto result { db.inspectionsData().findAllInspections(searchFilter, value) };

                if (result.empty()) return Ui::printInfo("No inspection record found");

                Ui::printnln(std::format("Found {} inspection records", result.size()));
                for (const auto& i : result) Ui::println(i.toString());

                return;
            }
            case MAINTENANCE:
            {
                auto searchFilterResult { Ui::promptSearchFilter("Filter maintenance records by") };
                if (! searchFilterResult)
                {
                    Ui::printError("Something went wrong");
                    return;
                }

                auto [searchFilter, value] { searchFilterResult.value() };
                auto result { db.maintenanceData().findAllMaintenance(searchFilter, value) };
            
                if (result.empty()) return Ui::printInfo("No maintenance record found");

                Ui::printnln(std::format("Found {} maintenance records", result.size()));
                for (const auto& m : result) Ui::println(m.toString());

                return;
            }
        }

        throw std::logic_error { "Unknown database record type" };
    }

    void MainMenuHandler::handleDisplayInventorySummary()
    {
        Ui::printnln(db.summaryToString(db.getSummary()));
    }

    void MainMenuHandler::handleExitApplication()
    {
        Ui::printInfo("Exiting application. Bye!");
    }


}
