#include <iostream>
#include <format>

#include "Prompt.cppm"
#include "Ui.cppm"

namespace EquipOps
{

    void Prompt::printPromptLabel(std::string_view label)
    {
        Ui::print(std::format("{}: ", label));
    }

    Prompt::ResultString Prompt::promptString(std::string_view label, bool isRequired)
    {
        std::string buffer { };

        while (true)
        {
            printPromptLabel(label);

            if (! std::getline(std::cin, buffer)) return { "", FAILURE };
            if (! isRequired && buffer.empty()) return { "", EMPTY };

            if (isRequired && buffer.empty())
            {
                Ui::printError("This field is required");
                continue;
            }

            return { buffer, SUCCESS };
        }
    }


    Prompt::ResultDouble Prompt::promptDouble(std::string_view label, bool isRequired)
    {
        while (true)
        {
            try
            {
                auto result { promptString(label, isRequired) };

                switch (result.status)
                {
                    case FAILURE: return { 0.0, FAILURE };
                    case EMPTY:
                    {
                        if (! isRequired) return { 0.0, EMPTY };
                        continue;
                    }
                    case SUCCESS:
                    {
                        size_t idx { };
                        auto input { std::stod(result.value, &idx) };

                        if (idx != result.value.length())
                        {
                            Ui::printError("Invalid input. Please enter a number");
                            continue;
                        }

                        return { input, SUCCESS };
                    }
                }
            }
            catch(const std::invalid_argument&) { Ui::printError("Invalid input. Please enter a number"); }
            catch(const std::out_of_range&) { Ui::printError("Input is too large."); };
        }
        
    }

    Prompt::ResultInt Prompt::promptInt(std::string_view label, bool isRequired)
    {
        while (true)
        {
            try
            {
                auto result { promptString(label, isRequired) };

                switch (result.status)
                {
                    case FAILURE:return { 0, FAILURE };
                    case EMPTY:
                    {
                        if (! isRequired) return { 0, EMPTY };
                        continue;
                    }
                    case SUCCESS:
                    {
                        size_t idx { };
                        auto input { std::stoi(result.value, &idx) };
                        if (idx != result.value.length())
                        {
                            Ui::printError("Invalid input. Please enter a whole number");
                            continue;
                        }

                        return { input, SUCCESS };
                    }
                }
            }
            catch (const std::invalid_argument&) { Ui::printError("Invalid input. Please enter a whole number"); }
            catch (const std::out_of_range&) { Ui::printError("Input is too large"); }
        }
        
    }

    Prompt::ResultUInt Prompt::promptUInt(std::string_view label, bool isRequired)
    {
        while (true)
        {
            auto result { promptInt(label, isRequired) };

            switch (result.status)
            {
                case FAILURE: return { 0, FAILURE };
                case EMPTY:
                {
                    if (! isRequired) return { 0, EMPTY };
                    continue;
                }
                case SUCCESS:
                {
                    if (result.value < 0)
                    {
                        Ui::printError("Invalid input. Value must be a nonnegative whole number");
                        continue;
                    }

                    return { static_cast<unsigned int>(result.value), SUCCESS };
                }
            }
        }
        
    }


}