#ifndef EQUIP_OPS_PROMPT_CPPM
#define EQUIP_OPS_PROMPT_CPPM

#include <string>

namespace EquipOps
{


    class Prompt
    {

        public:

            enum Status
            {
                /** User input was captured successfully */
                SUCCESS = 0,

                /** User did not enter any input. */
                EMPTY   = 1,

                /** An error occurred while reading input e.g. EOF */
                FAILURE = 2
            };

            struct ResultString
            {
                std::string value;
                Status status;
            };

            struct ResultDouble
            {
                double value;
                Status status;
            };

            struct ResultInt
            {
                int value;
                Status status;
            };

            struct ResultUInt
            {
                unsigned int value;
                Status status;
            };

            using Option = std::byte;
            
            static void printPromptLabel(std::string_view label);

            static ResultString promptString(std::string_view label, bool isRequired = true);
            static ResultDouble promptDouble(std::string_view label, bool isRequired = true);
            static ResultInt    promptInt   (std::string_view label, bool isRequired = true);
            static ResultUInt   promptUInt  (std::string_view label, bool isRequired = true);


    };

}

#endif