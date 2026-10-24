#ifndef EQUIP_OPS_MAINTENANCE_REPOSITORY_CPPM
#define EQUIP_OPS_MAINTENANCE_REPOSITORY_CPPM

#include <vector>

#include "App.cppm"
#include "Maintenance.cppm"

namespace EquipOps
{

    class MaintenanceRepository
    {

        public:
            MaintenanceRepository(
                std::vector<Maintenance>& maintenance,
                const EquipmentRepository& equipmentRepositorty
            );

            void recordMaintenance(Maintenance maintenance);

            std::vector<Maintenance> findAllMaintenance() const;
            std::vector<Maintenance> findAllMaintenance(std::string_view equipmentId) const;
            std::vector<Maintenance> findAllMaintenance(Equipment equipment) const;
            std::vector<Maintenance> findAllMaintenance(Equipment::Category category) const;
            std::vector<Maintenance> findAllMaintenance(Equipment::Status status) const;
            std::vector<Maintenance> findAllMaintenance(Equipment::Department department) const;
            std::vector<Maintenance> findAllMaintenance(App::SearchFilter filter, int value) const;

            void deleteAllMaintenance(std::string_view equipmentId);
            void deleteAllMaintenance(Equipment equipment);

        private:
            std::vector<Maintenance>& maintenance;
            const EquipmentRepository& equipmentRepository;

    };
}