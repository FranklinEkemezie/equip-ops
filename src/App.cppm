#ifndef EQUIP_OPS_APP_CPPM
#define EQUIP_OPS_APP_CPPM

namespace EquipOps
{

    class App
    {


        public:
            enum class RecordType
            {
                EQUIPMENT,
                INSPECTION,
                MAINTENANCE
            };

            enum class SearchFilter
            {
                CATEGORY,
                STATUS,
                DEPARTMENT,
                NONE
            };

            struct SearchRequest
            {
                RecordType recordType;
                SearchFilter searchFilter;
            };

            static RecordType recordTypeFromInt(int recordType);
            static SearchFilter searchFilterFromInt(int searchFilter);


    };
}

#endif