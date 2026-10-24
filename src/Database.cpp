#include <format>

#include "Database.cppm"

namespace EquipOps
{

    Database::Database():
        equipment { },
        inspections { },
        maintenance { },
        equipmentRepository { EquipmentRepository { equipment } },
        inspectionRepository { InspectionRepository { inspections, equipmentRepository } },
        maintenanceRepository { MaintenanceRepository { maintenance, equipmentRepository } }
    { };

    EquipmentRepository& Database::equipmentData() { return equipmentRepository; }
    InspectionRepository& Database::inspectionsData() { return inspectionRepository; }
    MaintenanceRepository& Database::maintenanceData() { return maintenanceRepository; }

    std::optional<Equipment> Database::deleteEquipment(std::string_view equipmentId)
    {
        std::optional<Equipment> deletedEquipment { };
        for (auto &e : equipment)
        {
            if (e.getId() == equipmentId)
            {
                deletedEquipment = e;
                break;
            }
        }

        if (! deletedEquipment) return std::nullopt;

        inspectionsData().deleteAllInspections(equipmentId);
        maintenanceData().deleteAllMaintenance(equipmentId);

        return deletedEquipment;
    }

    
    Database::Summary Database::getSummary() const
    {

        using enum Equipment::Status;

        unsigned long 
            registeredCount         { equipmentRepository.findAllEquipment().size() },
            operationalCount        { equipmentRepository.findAllEquipment(OPERATIONAL).size() },
            maintenanceCount        { equipmentRepository.findAllEquipment(UNDER_MAINTENANCE).size() },
            unavailableCount        { equipmentRepository.findAllEquipment(UNAVAILABLE).size() },
            attentionRequiredCount  { inspectionRepository.findLatestInspections({ OUTSIDE_LIMITS, UNAVAILABLE }).size() };
        

        return Summary {
            .registeredCount = registeredCount,
            .operationalCount = operationalCount,
            .maintenanceCount = maintenanceCount,
            .unavailableCount = unavailableCount,
            .attentionRequiredCount = attentionRequiredCount
        };
    }

    std::string Database::summaryToString(Summary summary)
    {
        return std::format(
            "Registered:            {}\n"
            "Operational:           {}\n"
            "Under maintenance:     {}\n"
            "Unavailable:           {}\n"
            "Requires attention:    {}\n",

            summary.registeredCount,
            summary.operationalCount,
            summary.maintenanceCount,
            summary.unavailableCount,
            summary.attentionRequiredCount
        );
    }

     

}
