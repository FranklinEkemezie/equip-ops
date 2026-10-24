#include <stdexcept>

#include "App.cppm"

namespace EquipOps
{

    App::RecordType App::recordTypeFromInt(int recordType)
    {
        switch (recordType)
        {
            using enum RecordType;

            case 0: return EQUIPMENT;
            case 1: return INSPECTION;
            case 2: return MAINTENANCE;
        }

        throw std::logic_error { "Unknown record type" };
    }

    App::SearchFilter App::searchFilterFromInt(int searchFilter)
    {
        switch (searchFilter)
        {
            using enum SearchFilter;

            case 0: return CATEGORY;
            case 1: return STATUS;
            case 2: return DEPARTMENT;
            case 3: return ALL;
        }

        throw std::logic_error { "Unknown search filter" };
    }
}