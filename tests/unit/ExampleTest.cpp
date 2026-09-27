#include <iostream>

#include "Database.cppm"

using namespace EquipOps;

namespace
{
    int failures { 0 };

    void expect(bool condition, const char* message)
    {
        if (condition) return;
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }

    template <typename Exception, typename Function>
    void expectThrows(Function function, const char* message)
    {
        try
        {
            function();
            expect(false, message);
        }
        catch (const Exception&) {}
    }
}

int main()
{
    Equipment drill { "EQ-1", "Bench drill", Equipment::Category::ELECTRIC_MOTOR,
        Equipment::Department::MECHANICAL, Equipment::Status::OPERATIONAL };

    drill.setName("Drill press");
    expect(drill.getName() == "Drill press", "equipment names can be updated");
    expect(Equipment::statusFromInt(1) == Equipment::Status::OUTSIDE_LIMITS,
        "status menu value 1 means outside limits");
    expect(Equipment::statusFromInt(2) == Equipment::Status::UNDER_MAINTENANCE,
        "status menu value 2 means under maintenance");
    expect(Equipment::statusFromInt(3) == Equipment::Status::UNAVAILABLE,
        "status menu value 3 means unavailable");

    Database database;
    database.equipmentData().registerEquipment(drill);
    expectThrows<std::invalid_argument>([&] { database.equipmentData().registerEquipment(drill); },
        "duplicate equipment identifiers are rejected");
    database.equipmentData().findEquipmentById("EQ-1").setStatus(Equipment::Status::OUTSIDE_LIMITS);
    const auto summary { database.equipmentData().getSummary() };
    expect(summary.registeredCount == 1 && summary.attentionRequiredCount == 1,
        "summary counts equipment outside operating limits as requiring attention");

    Inspection inspection { "EQ-1" };
    inspection.setTemperature(81.0f);
    expect(inspection.getEquipmentOperatingStatus() == Equipment::Status::OUTSIDE_LIMITS,
        "high temperature requires attention");
    database.inspectionsData().recordInspection(inspection);

    Maintenance maintenance { "EQ-1", Maintenance::Type::MONTHLY, "Replace belt",
        Equipment::Status::OPERATIONAL, Equipment::Status::UNDER_MAINTENANCE };
    database.maintenanceData().recordMaintenance(maintenance);
    expectThrows<std::invalid_argument>([&] {
        database.maintenanceData().recordMaintenance(
            Maintenance { "missing", Maintenance::Type::WEEKLY, "Check", Equipment::Status::OPERATIONAL,
                Equipment::Status::OPERATIONAL });
    }, "maintenance for unknown equipment is rejected");
    expectThrows<std::invalid_argument>([&] { database.inspectionsData().recordInspection(Inspection { "missing" }); },
        "inspection for unknown equipment is rejected");

    auto removed { database.deleteEquipment("EQ-1") };
    expect(removed.has_value() && removed->getId() == "EQ-1", "equipment deletion returns the removed record");
    expect(database.inspectionsData().findAllInspections().empty(), "deletion removes related inspections");
    expect(database.maintenanceData().findAllMaintenance().empty(), "deletion removes related maintenance");
    expect(!database.deleteEquipment("missing").has_value(), "unknown equipment deletion is explicit");

    if (failures == 0) std::cout << "All unit tests passed\n";
    return failures == 0 ? 0 : 1;
}
