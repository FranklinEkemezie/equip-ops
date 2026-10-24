#ifndef EQUIP_OPS_DATABASE_CPPM
#define EQUIP_OPS_DATABASE_CPPM

#include <vector>

#include "Equipment.cppm"
#include "EquipmentRepository.cppm"
#include "Inspection.cppm"
#include "InspectionRepository.cppm"
#include "Maintenance.cppm"
#include "MaintenanceRepository.cppm"

namespace EquipOps
{

    class Database
    {

        public:
            Database();

            EquipmentRepository& equipmentData();
            InspectionRepository& inspectionsData();
            MaintenanceRepository& maintenanceData();

            std::optional<Equipment> deleteEquipment(std::string_view equipmentId);
            std::optional<Equipment> deleteEquipment(Equipment equipment);

        private:
            std::vector<Equipment> equipment;
            std::vector<Inspection> inspections;
            std::vector<Maintenance> maintenance;

            EquipmentRepository equipmentRepository;
            InspectionRepository inspectionRepository;
            MaintenanceRepository maintenanceRepository;

    };
}

#endif