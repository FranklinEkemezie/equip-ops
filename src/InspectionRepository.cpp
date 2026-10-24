#include <stdexcept>

#include "InspectionRepository.cppm"

namespace EquipOps
{

    
    InspectionRepository::InspectionRepository(
        std::vector<Inspection>& inspections,
        const EquipmentRepository& equipmentRepository
    ):
        inspections { inspections },
        equipmentRepository { equipmentRepository }
    { };

    void InspectionRepository::recordInspection(Inspection inspection)
    {
        try
        {
            equipmentRepository.findEquipmentById(inspection.getEquipmentId());
        }
        catch (const std::logic_error&)
        {
            throw std::invalid_argument { "Cannot record an inspection for unknown equipment" };
        }

        inspections.push_back(inspection);
    }

    std::vector<Inspection> InspectionRepository::findAllInspections() const
    {
        return inspections;
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(std::string_view equipmentId) const
    {
        std::vector<Inspection> result { };

        for (const auto& i : inspections)
        {
            if (i.getEquipmentId() == equipmentId) result.push_back(i);
        }

        return result;
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(const Equipment& equipment) const
    {
        return findAllInspections(equipment.getId());
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(Equipment::Category category) const
    {
        std::vector<Inspection> result { };

        for (const auto& i : inspections)
        {
            const auto& e { i.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().get().getCategory() == category) result.push_back(i);
        }

        return result; 
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(Equipment::Status status) const
    {
        std::vector<Inspection> result { };

        for (const auto& i : inspections)
        {
            const auto& e { i.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().get().getStatus() == status) result.push_back(i);
        }

        return result; 
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(Equipment::Department department) const
    {
        std::vector<Inspection> result { };

        for (const auto& i : inspections)
        {
            const auto& e { i.getEquipment(equipmentRepository) };

            if (! e.has_value()) continue;
            if (e.value().get().getDepartment() == department) result.push_back(i);
        }

        return result; 
    }

    std::vector<Inspection> InspectionRepository::findAllInspections(
        App::SearchFilter filter, int value
    ) const
    {
        switch (filter)
        {
            using enum App::SearchFilter;

            case CATEGORY:      return findAllInspections(Equipment::categoryFromInt(value));
            case STATUS:        return findAllInspections(Equipment::statusFromInt(value));
            case DEPARTMENT:    return findAllInspections(Equipment::departmentFromInt(value));
            case ALL:           return findAllInspections();
        }

        throw std::logic_error { "Unknown search filter" };
    }

    void InspectionRepository::deleteAllInspections(std::string_view equipmentId)
    {
        for (auto i { inspections.begin() }; i != inspections.end(); )   
        {
            if (i->getEquipmentId() != equipmentId) i++;
            else i = inspections.erase(i);
        }
    }

    void InspectionRepository::deleteAllInspections(const Equipment& equipment)
    {
        deleteAllInspections(equipment.getId());
    }

}
