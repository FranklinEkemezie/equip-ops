#include <format>

#include "MainMenuHandler.cppm"

namespace EquipOps
{


    MainMenuHandler::MainMenuHandler(Database& db): db { db } { };


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

        Equipment e 
        {
            Ui::promptString("Equipment ID"),
            Ui::promptString("Equipment Name"),
            Ui::promptEquipmentCategory(),
            Ui::promptEquipmentDepartment(),
            Ui::promptEquipmentStatus()
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

        auto equipmentId { Ui::promptString("Enter equipment ID") };

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
        
        auto equipmentName { Ui::promptString("Enter equipment name") };

        try
        {
            auto& e { db.equipmentData().findEquipmentByName(equipmentName) };

            Ui::printInfo("Equipment found");
            Ui::printnln(e.toString());
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with named: {}", equipmentName));
        }
    }

    void MainMenuHandler::handleUpdateEquipment()
    {
        auto equipmentId { Ui::promptString("Enter equipment ID") };

        try
        {
            db.equipmentData()
                .findEquipmentById(equipmentId)
                .setStatus(Ui::promptEquipmentStatus());

            Ui::printInfo("Status updated successfully.");
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
        }
        
    }

    void MainMenuHandler::handleRemoveEquipment()
    {
        auto equipmentId { Ui::promptString("Enter equipment ID") };

        auto e { db.deleteEquipment(equipmentId) };

        if (! e.has_value())
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
        }

        Ui::printInfo(std::format(
            "Equipment '{}' removed successfully", e.value().getName()));
        
    }

    void MainMenuHandler::handleRecordMaintenanceActivity()
    {
        auto equipmentId { Ui::promptString("Enter equipment ID") };

        try
        {
            auto& e { db.equipmentData().findEquipmentById(equipmentId) };

            Ui::printHeading("Please enter maintenance activity details");

            Maintenance m
            {
                e,
                Ui::promptMaintenanceTypeOption(),
                Ui::promptString("Description"),
                Ui::promptEquipmentStatus()
            };

            db.maintenanceData().recordMaintenance(m);
            e.setStatus(m.statusAfterMaintenance());

            Ui::printInfo("Maintenance activity recorded successfully!");
        }
        catch (const std::logic_error&)
        {
            Ui::printError(std::format(
                "Failed to find equipment with ID: {}", equipmentId));
        }
    }

    void MainMenuHandler::handleRecordEquipmentInspection()
    {
        auto equipmentId { Ui::promptString("Enter equipment ID") };

        try
        {
            auto& e { db.equipmentData().findEquipmentById(equipmentId) };
    
            Inspection inspection { e.getId() };

            Ui::printHeading("Please enter inspection records below");    
            Ui::printInfo("You may enter '0' if inspection data is unavailable");

            auto temperature { Ui::promptFloat("Temperature") };
            if (temperature != 0.0f) inspection.setTemperature(temperature);

            auto vibration { Ui::promptFloat("Vibration") };
            if (vibration != 0.0f) inspection.setVibration(vibration);

            auto operatingHours { Ui::promptInt("Operating Hours") };
            if (operatingHours != 0) inspection.setOperatingHours(operatingHours);

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

        switch (recordType)
        {
            using enum App::RecordType;

            case EQUIPMENT:
            {
                auto [searchFilter, value] { Ui::promptSearchFilter("Filter equipment records by") };
                auto result { db.equipmentData().findAllEquipment(searchFilter, value) };

                if (result.empty()) return Ui::printInfo("No equipment record found");

                Ui::printnl();
                Ui::printnln(std::format("Found {} equipment records", result.size()));
                Ui::printnl();

                for (const auto& e : result) Ui::println(e.get().toString());
                return;
            }

            case INSPECTION:
            {
                auto [searchFilter, value] { Ui::promptSearchFilter("Filter inspection records by") };
                auto result { db.inspectionsData().findAllInspections(searchFilter, value) };

                if (result.empty()) return Ui::printInfo("No inspection record found");

                Ui::printnl();
                Ui::printnln(std::format("Found {} inspection records", result.size()));
                Ui::printnl();

                for (const auto& i : result) Ui::println(i.toString());
                return;
            }
            case MAINTENANCE:
            {
                auto [searchFilter, value] { Ui::promptSearchFilter("Filter maintenance records by") };
                auto result { db.maintenanceData().findAllMaintenance(searchFilter, value) };
            
                if (result.empty()) return Ui::printInfo("No maintenance record found");

                Ui::printnl();
                Ui::printnln(std::format("Found {} maintenance records", result.size()));
                Ui::printnl();

                for (const auto& m : result) Ui::println(m.toString());
                return;
            }
        }

        throw std::logic_error { "Unknown database record type" };
    }

    void MainMenuHandler::handleDisplayInventorySummary()
    {
        Ui::printEquipmentSummary(db.equipmentData().getSummary());
    }

    void MainMenuHandler::handleExitApplication()
    {
        Ui::printInfo("Exiting application. Bye!");
        exit(EXIT_SUCCESS);
    }


}