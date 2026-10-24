#include <format>
#include <iostream>
#include <stdexcept>

#include "Equipment.cppm"

namespace EquipOps
{

    Equipment::Equipment(
        std::string_view id,
        std::string_view name,
        Category category,
        Department department,
        Status status
    ): 
        id { id },
        name { name },
        category { category },
        department { department },
        status { status }
    { };

    Equipment::Category Equipment::categoryFromInt(int category)
    {
        switch (category)
        {
            using enum Equipment::Category;

            case 0: return ELECTRIC_MOTOR;
            case 1: return HAND_TOOL;
            case 2: return CUTTING_TOOL;
            case 3: return WELDING_TOOL;
            default: throw std::logic_error { "Unknown equipment category" };
        }
    }

    Equipment::Department Equipment::departmentFromInt(int department)
    {
        switch (department)
        {
            using enum Equipment::Department;

            case 0: return ELECTRICAL;
            case 1: return MECHANICAL;
            case 2: return WELDING;
            case 3: return PAINTING;
            default: throw std::logic_error { "Unknown equipment department" };
        }
    }

    Equipment::Status Equipment::statusFromInt(int status)
    {
        using enum Equipment::Status;

        switch (status) {
            case 0: return OPERATIONAL;
            case 1: return OUTSIDE_LIMITS;
            case 2: return UNDER_MAINTENANCE;
            case 3: return UNAVAILABLE;
        }

        throw std::logic_error { "Unknown equipment status" };

    }

    std::string Equipment::categoryToString(Category category)
    {
        switch (category)
        {
            using enum Category;

            case ELECTRIC_MOTOR:    return "Electric Motor";
            case HAND_TOOL:         return "Hand Tool";
            case CUTTING_TOOL:      return "Cutting Tool";
            case WELDING_TOOL:      return "Welding Tool";
        }

        throw std::logic_error { "Unknown category" };
    }

    std::string Equipment::departmentToString(Department department)
    {
        switch (department)
        {
            using enum Department;

            case ELECTRICAL:    return "Electrical";
            case MECHANICAL:    return "Mechanical";
            case WELDING:       return "Welding";
            case PAINTING:      return "Painting";
        }

        throw std::logic_error { "Unknown department" };
    }

    std::string Equipment::statusToString(Status status)
    {
        switch (status)
        {
            using enum Status;

            case OPERATIONAL:       return "Operational";
            case OUTSIDE_LIMITS:    return "Outside Limits";
            case UNDER_MAINTENANCE: return "Under Maintenance";
            case UNAVAILABLE:       return "Unavailable";
        }

        throw std::logic_error { "Unknown status" };
    }


    std::string Equipment::getId() const { return id; }

    std::string Equipment::getName() const { return name; }

    void Equipment::setName(std::string_view name) { this->name = name; }

    Equipment::Category Equipment::getCategory() const { return category; }

    std::string Equipment::getCategoryLabel() const { return categoryToString(category); }

    void Equipment::setCategory(Equipment::Category category) 
    { 
        this->category = category;
    }

    Equipment::Department Equipment::getDepartment() const { return department; }

    std::string Equipment::getDepartmentLabel() const { return departmentToString(department); }

    void Equipment::setDepartment(Equipment::Department department)
    {
        this->department = department;
    }

    Equipment::Status Equipment::getStatus() const { return status; }

    std::string Equipment::getStatusLabel() const { return statusToString(status); }

    void Equipment::setStatus(Equipment::Status status)
    {
        this->status = status;
    }

    std::string Equipment::toString() const
    {
        return std::format(
            "ID:            {}\n"
            "Name:          {}\n"
            "Category:      {}\n"
            "Department:    {}\n"
            "Status:        {}\n",
            
            getId(), getName(), getCategoryLabel(), 
                getDepartmentLabel(), getStatusLabel()
        );
    }

}
