#ifndef __App_cppm
#define __App_cppm

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