#ifndef __InspectionRepository_cppm
#define __InspectionRepository_cppm

#include <vector>

#include "App.cppm"
#include "EquipmentRepository.cppm"
#include "Inspection.cppm"

namespace EquipOps
{

    class InspectionRepository
    {


        public:
            InspectionRepository(
                std::vector<Inspection>& inspections,
                const EquipmentRepository& equipmentRepository
            );

            void recordInspection(Inspection inspection);

            std::vector<Inspection> findAllInspections() const;
            std::vector<Inspection> findAllInspections(std::string_view equipmentId) const;
            std::vector<Inspection> findAllInspections(Equipment equipment) const;
            std::vector<Inspection> findAllInspections(Equipment::Category category) const;
            std::vector<Inspection> findAllInspections(Equipment::Status status) const;
            std::vector<Inspection> findAllInspections(Equipment::Department department) const;
            std::vector<Inspection> findAllInspections(App::SearchFilter filter, int value) const;

            void deleteAllInspections(std::string_view equipmentId);
            void deleteAllInspections(Equipment equipment);

           
        private:
            std::vector<Inspection>& inspections;
            const EquipmentRepository& equipmentRepository;

    };
}


#endif