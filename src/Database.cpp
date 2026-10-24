
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

    std::optional<Equipment> Database::deleteEquipment(Equipment equipment)
    {
        return deleteEquipment(equipment.getId());
    }
     

}
