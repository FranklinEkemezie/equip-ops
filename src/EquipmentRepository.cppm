#ifndef EQUIP_OPS_EQUIPMENT_REPOSITORY_CPPM
#define EQUIP_OPS_EQUIPMENT_REPOSITORY_CPPM

#include <optional>
#include <vector>

#include "App.cppm"
#include "Equipment.cppm"

namespace EquipOps
{


    class EquipmentRepository
    {

        public:
            EquipmentRepository(std::vector<Equipment>& equipment);

            void registerEquipment(Equipment equipment) const;

            Equipment& findEquipmentById(std::string_view id) const;
            Equipment& findEquipmentByName(std::string_view name) const;

            // Equipment data are heavily read, written and modified so a 
            // reference is a natural choice for this
            // Other "models" like Inspection and Maintenance are typically
            // readonly with delete when the associated equipment is deleted

            using EquipmentList = std::vector<std::reference_wrapper<Equipment>>;

            EquipmentList findAllEquipment() const;
            EquipmentList findAllEquipment(std::string name) const;
            EquipmentList findAllEquipment(Equipment::Category category) const;
            EquipmentList findAllEquipment(Equipment::Department department) const;
            EquipmentList findAllEquipment(Equipment::Status status) const;
            EquipmentList findAllEquipment(App::SearchFilter filter, int value) const;

            static void printEquipmentList(EquipmentList equipmentList);

        private:
            std::vector<Equipment>& equipment;
    };
}

#endif