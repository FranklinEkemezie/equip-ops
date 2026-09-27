#include <format>
#include <stdexcept>

#include "EquipmentRepository.cppm"

namespace EquipOps
{


    EquipmentRepository::EquipmentRepository(
        std::vector<Equipment>& equipment
    ):
        equipment { equipment }
    {  };

    void EquipmentRepository::registerEquipment(Equipment equipment) const
    {
        for (auto& e : this->equipment)
        {
            if (e.getId() == equipment.getId())
            {
                throw std::invalid_argument { std::format(
                    "Failed to register equipment: identifier '{}' is already in use",
                    e.getId()
                ) };
            }
        }
        
        this->equipment.push_back(equipment);
    }

    Equipment& EquipmentRepository::findEquipmentById(std::string_view id) const
    {
        for (auto& e : equipment)
        {
            if (e.getId() == id) return e;
        }

        throw std::logic_error { std::format("Failed to find equipment with ID: {}", id) };
    }

    Equipment& EquipmentRepository::findEquipmentByName(std::string_view name) const
    {
        for (auto& e : equipment)
        {
            if (e.getName() == name) return e;
        }

        throw std::logic_error { std::format("Failed to find equipment with name: {}", name) };
    }

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment() const 
    {
        EquipmentList result { };

        for (auto& e : equipment) result.push_back(std::ref(e));

        return result;
    }

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment(Equipment::Category category) const
    {
        EquipmentList result { };

        for (auto& e : equipment)
        {
            if (e.getCategory() == category) result.push_back(std::ref(e));
        }

        return result;
    }

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment(Equipment::Department department) const
    {
        EquipmentList result { };

        for (auto& e : equipment)
        {
            if (e.getDepartment() == department) result.push_back(std::ref(e));
        }

        return result;
    }

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment(Equipment::Status status) const
    {
        EquipmentList result { };

        for (auto& e : equipment)
        {
            if (e.getStatus() == status) result.push_back(std::ref(e));
        }

        return result;
    }

    std::optional<Equipment> EquipmentRepository::removeEquipment(std::string_view id)
    {
        for (auto e { equipment.begin() }; e != equipment.end(); e++)
        {
            if (e->getId() == id)
            {
                equipment.erase(e);
                return *e;
            }
        }

        return std::nullopt;
    }

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment(
        App::SearchFilter filter, int value
    ) const
    {
        switch (filter)
        {
            using enum App::SearchFilter;

            case CATEGORY:      return findAllEquipment(Equipment::categoryFromInt(value));
            case STATUS:        return findAllEquipment(Equipment::statusFromInt(value));
            case DEPARTMENT:    return findAllEquipment(Equipment::departmentFromInt(value));
            case NONE:          return findAllEquipment();
        };

        throw std::logic_error { "Unknown search filter" };
    }

    std::optional<Equipment> EquipmentRepository::removeEquipment(Equipment equipment)
    {
        return removeEquipment(equipment.getId());
    }

    EquipmentRepository::Summary EquipmentRepository::getSummary() const
    {

        using enum Equipment::Status;

        unsigned long 
            registeredCount { findAllEquipment().size() },
            operationalCount { findAllEquipment(OPERATIONAL).size() },
            maintenanceCount { findAllEquipment(UNDER_MAINTENANCE).size() },
            unavailableCount { findAllEquipment(UNAVAILABLE).size() };

        return Summary {
            registeredCount: registeredCount,
            operationalCount: operationalCount,
            maintenanceCount: maintenanceCount,
            unavailableCount: unavailableCount,
            attentionRequiredCount: registeredCount - (operationalCount + maintenanceCount)
        };
    }

    std::string EquipmentRepository::summaryToString(Summary summary)
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