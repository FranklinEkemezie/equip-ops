#ifndef EQUIP_OPS_INSPECTION_CPPM
#define EQUIP_OPS_INSPECTION_CPPM

#include <optional>
#include <string>

#include "EquipmentRepository.cppm"

namespace EquipOps
{

    class Inspection
    {

        public:

            Inspection(std::string_view equipmentId);

            std::string getEquipmentId() const;
            std::optional<Equipment> getEquipment(const EquipmentRepository& equipment) const;

            std::optional<float> getTemperature() const;
            void setTemperature(float temperature);

            std::optional<float> getVibration() const;
            void setVibration(float vibration);

            std::optional<unsigned int> getOperatingHours() const;
            void setOperatingHours(unsigned int operatingHours);

            std::optional<Equipment::Status> getEquipmentOperatingStatus() const;

            std::string toString() const;

        private:
            std::string equipmentId;
            std::optional<float> temperature { std::nullopt };
            std::optional<float> vibration { std::nullopt };
            std::optional<unsigned int> operatingHours { std::nullopt };

            static constexpr float MAX_TEMPERATURE { 80.0f };
            static constexpr float MAX_VIBRATION { 5.0f };
            static constexpr unsigned int MIN_OPERATING_HOURS { 2 };
    };
}

#endif