#include <format>

#include "Inspection.cppm"

namespace EquipOps
{

    Inspection::Inspection(std::string_view equipmentId):
        equipmentId { equipmentId }
    { };

    std::string Inspection::getEquipmentId() const { return equipmentId; };
    std::optional<Equipment> Inspection::getEquipment(const EquipmentRepository& equipment) const 
    {
        return equipment.findEquipmentById(equipmentId);
    }

    std::optional<float> Inspection::getTemperature() const { return temperature; }
    void Inspection::setTemperature(float temperature)
    {
        this->temperature = temperature;
    }

    std::optional<float> Inspection::getVibration() const { return vibration; }
    void Inspection::setVibration(float vibration)
    {
        this->vibration = vibration;
    }

    std::optional<unsigned int> Inspection::getOperatingHours() const { return operatingHours; }
    void Inspection::setOperatingHours(unsigned int operatingHours)
    {
        this->operatingHours = operatingHours;
    }

    std::optional<Equipment::Status> Inspection::getEquipmentOperatingStatus() const
    {
        using enum Equipment::Status;

        if (
            (temperature.has_value() && temperature.value() > MAX_TEMPERATURE) ||
            (vibration.has_value() && vibration.value() > MAX_VIBRATION)
        ) return OUTSIDE_LIMITS;

        if (
            operatingHours.has_value() && 
            operatingHours.value() < MIN_OPERATING_HOURS
        ) return UNAVAILABLE;

        if (
            temperature.has_value() || vibration.has_value() || 
            operatingHours.has_value()
        ) return OPERATIONAL;

        return std::nullopt;
    }

    std::string Inspection::toString() const
    {
        return std::format(
            "Equipment ID:      {}\n"
            "Temperature:       {}\n"
            "Vibration:         {}\n"
            "Operating Hours:   {}\n",

            equipmentId,
            temperature.has_value() ? std::format("{}°C", temperature.value()) : std::string{"N/A"},
            vibration.has_value() ? std::format("{}mm/s", vibration.value()) : std::string{"N/A"},
            operatingHours.has_value() ? std::format("{}hrs", operatingHours.value()) : std::string{"N/A"}
        );
    }


}