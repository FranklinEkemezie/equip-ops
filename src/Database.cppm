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

            struct Summary
            {
                unsigned long 
                    registeredCount,        // number of registered equipment items
                    operationalCount,       // number currently operational
                    maintenanceCount,       // number under maintenance
                    unavailableCount,       // number unavailable
                    attentionRequiredCount; // number requiring attention based on latest inspection
            };

            Summary getSummary() const;
            static std::string summaryToString(Summary summary);

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