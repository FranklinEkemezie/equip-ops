#include <format>
#include <stdexcept>

#include "Maintenance.cppm"

namespace EquipOps
{

    Maintenance::Maintenance(
        std::string_view equipmentId,
        Type type,
        std::string_view description,
        Equipment::Status beforeStatus,
        Equipment::Status afterStatus
    ):
        equipmentId { equipmentId },
        type { type },
        description { description },
        beforeStatus { beforeStatus },
        afterStatus { afterStatus }
    { };

    Maintenance::Maintenance(
        Equipment& equipment,
        Type type,
        std::string_view description,
        Equipment::Status status
    ):
        equipmentId { equipment.getId() },
        type { type },
        description { description },
        beforeStatus { equipment.getStatus() },
        afterStatus  { status }
    { };

    Maintenance::Type Maintenance::typeFromInt(int type)
    {
        switch (type)
        {
            using enum Maintenance::Type;
            case 0: return WEEKLY;
            case 1: return BIWEEKLY;
            case 2: return MONTHLY;
            case 3: return QUARTERLY;
            case 4: return ANNUALLY;
        }

        throw std::logic_error { "Unknown maintenance type" };
    }

    std::string Maintenance::typeToString(Type type)
    {
        switch (type)
        {
            using enum Maintenance::Type;
            case WEEKLY:    return "Weekly";
            case BIWEEKLY:  return "BiWeekly";
            case MONTHLY:   return "Monthly";
            case QUARTERLY: return "Quarterly";
            case ANNUALLY:  return "Annually";
        }

        throw std::logic_error { "Unknown maintenance type" };
    }

    std::string Maintenance::getEquipmentId() const { return equipmentId; }
    std::optional<Equipment> Maintenance::getEquipment(const EquipmentRepository& equipmentRepository) const
    {
        return equipmentRepository.findEquipmentById(equipmentId);
    }

    Maintenance::Type Maintenance::getType() const { return type; }
    std::string Maintenance::getDescription() const { return description; }
    Equipment::Status Maintenance::statusBeforeMaintenance() const { return beforeStatus; }
    Equipment::Status Maintenance::statusAfterMaintenance() const { return afterStatus; }

    std::string Maintenance::toString() const
    {
        return std::format(
            "Equipment ID:              {}\n"
            "Maintenance Type:          {}\n"
            "Description:               {}\n"
            "Status Before Maintenance: {}\n"
            "Status After  Maintenance: {}\n",

            equipmentId,
            typeToString(type),
            description,
            Equipment::statusToString(beforeStatus),
            Equipment::statusToString(afterStatus)
        );
    }
}