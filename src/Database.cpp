
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

        // Delete associated inspections and maintenance records
        inspectionsData().deleteAllInspections(equipmentId);
        maintenanceData().deleteAllMaintenance(equipmentId);

        return equipmentData().removeEquipment(equipmentId);
    }

    std::optional<Equipment> Database::deleteEquipment(Equipment equipment)
    {
        return deleteEquipment(equipment.getId());
    }
     

}
