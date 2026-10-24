#ifndef EQUIP_OPS_MAINTENANCE_CPPM
#define EQUIP_OPS_MAINTENANCE_CPPM

#include "Equipment.cppm"
#include "EquipmentRepository.cppm"

namespace EquipOps
{

    class Maintenance
    {

        public:
            enum class Type
            {
                WEEKLY,
                BIWEEKLY,
                MONTHLY,
                QUARTERLY,
                ANNUALLY
            };

            Maintenance(
                std::string_view equipmentId,
                Type type,
                std::string_view description,
                Equipment::Status beforeStatus,
                Equipment::Status afterStatus
            );

            Maintenance(
                Equipment& equipment,
                Type type,
                std::string_view description,
                Equipment::Status status
            );

            static Type typeFromInt(int type);
            static std::string typeToString(Type type);

            std::string getEquipmentId() const;
            std::optional<Equipment> getEquipment(const EquipmentRepository& equipmentRepository) const;

            Type getType() const;
            std::string getDescription() const;
            Equipment::Status statusBeforeMaintenance() const;
            Equipment::Status statusAfterMaintenance() const;

            std::string toString() const;

        private:
            std::string equipmentId;
            Type type;
            std::string description;
            Equipment::Status beforeStatus;
            Equipment::Status afterStatus;

    };
}

#endif