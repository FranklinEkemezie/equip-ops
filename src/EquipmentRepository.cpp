#include <format>
#include <stdexcept>

#include "EquipmentRepository.cppm"
#include "Ui.cppm"

namespace EquipOps
{


    EquipmentRepository::EquipmentRepository(
        std::vector<Equipment>& equipment
    ):
        equipment { equipment }
    {  };

    /**
     * @throws `std::invalid_argument` - An equipment with same identifier as the equipment to be inserted already exists
     */
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

    /**
     * @throws `std::logic_error` - An equipment with the `id` is not found
     */
    Equipment& EquipmentRepository::findEquipmentById(std::string_view id) const
    {
        for (auto& e : equipment)
        {
            if (e.getId() == id) return e;
        }

        throw std::logic_error { std::format("Failed to find equipment with ID: {}", id) };
    }

    /**
     * @throws `std::logic_error` - An equipment with the `id` is not found
     */
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

    EquipmentRepository::EquipmentList EquipmentRepository::findAllEquipment(std::string name) const 
    {
        EquipmentList result { };

        for (auto& e : equipment)
        {
            if (e.getName() == name) result.push_back(std::ref(e));
        }

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

    /**
     * @throws `std::logic_error` - The `filter` is not a valid `App::SearchFilter` enum case.
     */
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
            case ALL:           return findAllEquipment();
        };

        throw std::logic_error { "Unknown search filter" };
    }

    void EquipmentRepository::printEquipmentList(EquipmentList equipmentList)
    {
        constexpr auto row_format { "| {:8} | {:16} | {:32} | {:24} | {:24} | {:24} |" };
        const auto table_heading { std::format(row_format,
            "S/N", "ID", "Name", "Category", "Department", "Status") };
        const auto horizontal_border { std::format("{:-^{}}", "", table_heading.length()) };

        Ui::println(horizontal_border);
        Ui::println(table_heading);
        Ui::println(horizontal_border);

        for (size_t i { 0 }; i < equipmentList.size(); i++)
        {
            const auto& e { equipmentList[i].get() };

            Ui::println(std::format(row_format, 
                i + 1, 
                e.getId(), 
                e.getName(), 
                e.getCategoryLabel(), 
                e.getDepartmentLabel(), 
                e.getStatusLabel()  
            ));
            Ui::println(horizontal_border);

        }
    }

}