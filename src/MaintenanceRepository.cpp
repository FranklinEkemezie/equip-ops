#include <stdexcept>

#include "MaintenanceRepository.cppm"

namespace EquipOps
{


    MaintenanceRepository::MaintenanceRepository(
        std::vector<Maintenance>& maintenance,
        const EquipmentRepository& equipmentRepositorty

    ):
        maintenance { maintenance },
        equipmentRepository { equipmentRepositorty }
    { };

    void MaintenanceRepository::recordMaintenance(Maintenance maintenance)
    {
        this->maintenance.push_back(maintenance);
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance() const
    {
        return maintenance;
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(std::string_view equipmentId) const
    {
        std::vector<Maintenance> result { };

        for (const auto& m : maintenance)
        {
            if (m.getEquipmentId() == equipmentId) result.push_back(m);
        }

        return result;
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(Equipment equipment) const
    {
        return findAllMaintenance(equipment.getId());
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(Equipment::Category category) const
    {
        std::vector<Maintenance> result { };

        for (const auto& m : maintenance)
        {
            const auto& e { m.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().getCategory() == category) result.push_back(m);
        }

        return result; 
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(Equipment::Status status) const
    {
        std::vector<Maintenance> result { };

        for (const auto& m : maintenance)
        {
            const auto& e { m.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().getStatus() == status) result.push_back(m);
        }

        return result; 
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(Equipment::Department department) const
    {
        std::vector<Maintenance> result { };

        for (const auto& m : maintenance)
        {
            const auto& e { m.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().getDepartment() == department) result.push_back(m);
        }

        return result; 
    }

    std::vector<Maintenance> MaintenanceRepository::findAllMaintenance(
        App::SearchFilter filter, int value
    ) const
    {
        switch (filter)
        {
            using enum App::SearchFilter;
            
            case CATEGORY:      return findAllMaintenance(Equipment::categoryFromInt(value));
            case STATUS:        return findAllMaintenance(Equipment::statusFromInt(value));
            case DEPARTMENT:    return findAllMaintenance(Equipment::departmentFromInt(value));
            case NONE:          return findAllMaintenance();
        }

        throw std::logic_error { "Unknown search filter" };
    }

    void MaintenanceRepository::deleteAllMaintenance(std::string_view equipmentId)
    {

        for (auto m { maintenance.begin() }; m != maintenance.end(); )
        {
            if (m->getEquipmentId() != equipmentId) m++;
            else m = maintenance.erase(m);
        }
    }

    void MaintenanceRepository::deleteAllMaintenance(Equipment equipment)
    {
        deleteAllMaintenance(equipment.getId());
    }

}