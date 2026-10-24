#ifndef EQUIP_OPS_EQUIPMENT_CPPM
#define EQUIP_OPS_EQUIPMENT_CPPM

#include <string>

namespace EquipOps
{

    class Equipment
    {

        public:

            enum class Category
            {
                ELECTRIC_MOTOR,
                HAND_TOOL,
                CUTTING_TOOL,
                WELDING_TOOL
            };

            enum class Department
            {
                ELECTRICAL,
                MECHANICAL,
                WELDING,
                PAINTING
            };

            enum class Status
            {
                OPERATIONAL,
                OUTSIDE_LIMITS,
                UNDER_MAINTENANCE,
                UNAVAILABLE
            };

            Equipment(
                std::string_view id,
                std::string_view name,
                Category category,
                Department department,
                Status status
            );

            static Category categoryFromInt(int category);
            static Department departmentFromInt(int department);
            static Status statusFromInt(int status);

            static std::string categoryToString(Category category);
            static std::string departmentToString(Department department);
            static std::string statusToString(Status status);

            std::string getId() const;

            std::string getName() const;
            void setName(std::string_view name);

            Category getCategory() const;
            std::string getCategoryLabel() const;
            void setCategory(Category category);

            Department getDepartment() const;
            std::string getDepartmentLabel() const;
            void setDepartment(Department department);

            Status getStatus() const;
            std::string getStatusLabel() const;
            void setStatus(Status status);

            std::string toString() const;


        private:
            std::string id;
            std::string name;
            Category category;
            Department department;
            Status status;
            // Relevant operating information???
    };
}

#endif