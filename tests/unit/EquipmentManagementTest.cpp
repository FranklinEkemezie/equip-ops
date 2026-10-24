#include <iostream>
#include <string>
#include <string_view>

#include "Database.cppm"

using namespace EquipOps;

namespace
{
    int failures { 0 };

    void expect(bool condition, std::string_view message)
    {
        if (condition) return;
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }

    template <typename Exception, typename Function>
    void expectThrows(Function function, std::string_view message)
    {
        try
        {
            function();
            expect(false, message);
        }
        catch (const Exception&) {}
    }

    Equipment makeEquipment(
        std::string_view id,
        std::string_view name,
        Equipment::Category category,
        Equipment::Department department,
        Equipment::Status status
    )
    {
        return Equipment { id, name, category, department, status };
    }

    void testEquipmentValues()
    {
        Equipment drill {
            "EQ-1", "Bench drill", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::MECHANICAL, Equipment::Status::OPERATIONAL
        };

        drill.setName("Drill press");
        expect(drill.getName() == "Drill press", "equipment names can be updated");

        drill.setCategory(Equipment::Category::HAND_TOOL);
        drill.setDepartment(Equipment::Department::PAINTING);
        drill.setStatus(Equipment::Status::UNDER_MAINTENANCE);
        expect(drill.getCategoryLabel() == "Hand Tool", "category labels are rendered");
        expect(drill.getDepartmentLabel() == "Painting", "department labels are rendered");
        expect(drill.getStatusLabel() == "Under Maintenance", "status labels are rendered");

        expect(Equipment::categoryFromInt(0) == Equipment::Category::ELECTRIC_MOTOR,
            "category menu value 0 means electric motor");
        expect(Equipment::categoryFromInt(3) == Equipment::Category::WELDING_TOOL,
            "category menu value 3 means welding tool");
        expect(Equipment::departmentFromInt(0) == Equipment::Department::ELECTRICAL,
            "department menu value 0 means electrical");
        expect(Equipment::departmentFromInt(3) == Equipment::Department::PAINTING,
            "department menu value 3 means painting");
        expect(Equipment::statusFromInt(0) == Equipment::Status::OPERATIONAL,
            "status menu value 0 means operational");
        expect(Equipment::statusFromInt(1) == Equipment::Status::OUTSIDE_LIMITS,
            "status menu value 1 means outside limits");
        expect(Equipment::statusFromInt(2) == Equipment::Status::UNDER_MAINTENANCE,
            "status menu value 2 means under maintenance");
        expect(Equipment::statusFromInt(3) == Equipment::Status::UNAVAILABLE,
            "status menu value 3 means unavailable");

        expectThrows<std::logic_error>(
            [] { (void) Equipment::categoryFromInt(4); },
            "invalid category menu values are rejected");
        expectThrows<std::logic_error>(
            [] { (void) Equipment::departmentFromInt(-1); },
            "invalid department menu values are rejected");
        expectThrows<std::logic_error>(
            [] { (void) Equipment::statusFromInt(4); },
            "invalid status menu values are rejected");
    }

    void testInspectionStatusBoundaries()
    {
        using enum Equipment::Status;

        Inspection empty { "EQ-1" };
        expect(!empty.getEquipmentOperatingStatus().has_value(),
            "an inspection without measurements has no operating status");

        Inspection atTemperatureLimit { "EQ-1" };
        atTemperatureLimit.setTemperature(80.0f);
        expect(atTemperatureLimit.getEquipmentOperatingStatus() == OPERATIONAL,
            "temperature exactly at the limit is operational");

        Inspection aboveTemperatureLimit { "EQ-1" };
        aboveTemperatureLimit.setTemperature(80.1f);
        expect(aboveTemperatureLimit.getEquipmentOperatingStatus() == OUTSIDE_LIMITS,
            "temperature above the limit is outside limits");

        Inspection atVibrationLimit { "EQ-1" };
        atVibrationLimit.setVibration(5.0f);
        expect(atVibrationLimit.getEquipmentOperatingStatus() == OPERATIONAL,
            "vibration exactly at the limit is operational");

        Inspection aboveVibrationLimit { "EQ-1" };
        aboveVibrationLimit.setVibration(5.1f);
        expect(aboveVibrationLimit.getEquipmentOperatingStatus() == OUTSIDE_LIMITS,
            "vibration above the limit is outside limits");

        Inspection zeroHours { "EQ-1" };
        zeroHours.setOperatingHours(0);
        expect(zeroHours.getEquipmentOperatingStatus() == UNAVAILABLE,
            "zero operating hours can be recorded and mark the equipment unavailable");

        Inspection oneHour { "EQ-1" };
        oneHour.setOperatingHours(1);
        expect(oneHour.getEquipmentOperatingStatus() == UNAVAILABLE,
            "one operating hour is below the minimum");

        Inspection minimumHours { "EQ-1" };
        minimumHours.setOperatingHours(2);
        expect(minimumHours.getEquipmentOperatingStatus() == OPERATIONAL,
            "the minimum operating hours are operational");

        Inspection outsideWinsOverUnavailable { "EQ-1" };
        outsideWinsOverUnavailable.setTemperature(90.0f);
        outsideWinsOverUnavailable.setOperatingHours(1);
        expect(outsideWinsOverUnavailable.getEquipmentOperatingStatus() == OUTSIDE_LIMITS,
            "limit violations take precedence over low operating hours");

        Inspection allMeasurements { "EQ-1" };
        allMeasurements.setTemperature(50.0f);
        allMeasurements.setVibration(2.0f);
        allMeasurements.setOperatingHours(12);
        expect(allMeasurements.getEquipmentOperatingStatus() == OPERATIONAL,
            "an inspection within every limit is operational");
    }

    void testMaintenanceValues()
    {
        Maintenance maintenance {
            "EQ-2", Maintenance::Type::MONTHLY, "Replace belt",
            Equipment::Status::OPERATIONAL, Equipment::Status::UNDER_MAINTENANCE
        };

        expect(maintenance.getEquipmentId() == "EQ-2", "maintenance retains the equipment ID");
        expect(maintenance.getType() == Maintenance::Type::MONTHLY, "maintenance retains its type");
        expect(maintenance.getDescription() == "Replace belt", "maintenance retains its description");
        expect(maintenance.statusBeforeMaintenance() == Equipment::Status::OPERATIONAL,
            "maintenance retains the status before work");
        expect(maintenance.statusAfterMaintenance() == Equipment::Status::UNDER_MAINTENANCE,
            "maintenance retains the status after work");

        expect(Maintenance::typeFromInt(0) == Maintenance::Type::WEEKLY,
            "maintenance menu value 0 means weekly");
        expect(Maintenance::typeFromInt(4) == Maintenance::Type::ANNUALLY,
            "maintenance menu value 4 means annually");
        expectThrows<std::logic_error>(
            [] { (void) Maintenance::typeFromInt(5); },
            "invalid maintenance menu values are rejected");
        expect(Maintenance::typeToString(Maintenance::Type::BIWEEKLY) == "BiWeekly",
            "maintenance type labels are rendered");

        const auto rendered { maintenance.toString() };
        expect(rendered.find("Status Before Maintenance") != std::string::npos,
            "maintenance output spells 'Before' correctly");
        expect(rendered.find("Status After  Maintenance") != std::string::npos,
            "maintenance output spells 'After' correctly");
        expect(rendered.find("Bfter") == std::string::npos,
            "maintenance output no longer contains the old typo");
    }

    void testAppConversions()
    {
        expect(App::recordTypeFromInt(0) == App::RecordType::EQUIPMENT,
            "record menu value 0 means equipment");
        expect(App::recordTypeFromInt(1) == App::RecordType::INSPECTION,
            "record menu value 1 means inspection");
        expect(App::recordTypeFromInt(2) == App::RecordType::MAINTENANCE,
            "record menu value 2 means maintenance");
        expectThrows<std::logic_error>(
            [] { (void) App::recordTypeFromInt(3); },
            "invalid record menu values are rejected");

        expect(App::searchFilterFromInt(0) == App::SearchFilter::CATEGORY,
            "search menu value 0 means category");
        expect(App::searchFilterFromInt(1) == App::SearchFilter::STATUS,
            "search menu value 1 means status");
        expect(App::searchFilterFromInt(2) == App::SearchFilter::DEPARTMENT,
            "search menu value 2 means department");
        expect(App::searchFilterFromInt(3) == App::SearchFilter::ALL,
            "search menu value 3 means all records");
        expectThrows<std::logic_error>(
            [] { (void) App::searchFilterFromInt(4); },
            "invalid search menu values are rejected");
    }

    void testRepositories()
    {
        using enum Equipment::Status;
        using enum Equipment::Category;
        using enum Equipment::Department;

        Database database;
        auto& equipment { database.equipmentData() };

        equipment.registerEquipment(makeEquipment(
            "EQ-1", "Drill Press", ELECTRIC_MOTOR, ELECTRICAL, OPERATIONAL));
        equipment.registerEquipment(makeEquipment(
            "EQ-2", "Bench Grinder", ELECTRIC_MOTOR, MECHANICAL, UNDER_MAINTENANCE));
        equipment.registerEquipment(makeEquipment(
            "EQ-3", "Welding Torch", WELDING_TOOL, WELDING, UNAVAILABLE));
        equipment.registerEquipment(makeEquipment(
            "EQ-4", "Drill Press", HAND_TOOL, PAINTING, OPERATIONAL));

        expectThrows<std::invalid_argument>(
            [&] {
                equipment.registerEquipment(makeEquipment(
                    "EQ-1", "Duplicate", HAND_TOOL, PAINTING, OPERATIONAL));
            },
            "duplicate equipment identifiers are rejected");

        expect(equipment.findAllEquipment().size() == 4, "all equipment can be listed");
        expect(equipment.findAllEquipment("Drill Press").size() == 2,
            "searching by name returns every matching equipment");
        expect(equipment.findAllEquipment("Missing").empty(),
            "searching by an unknown name returns no equipment");
        expect(equipment.findAllEquipment(ELECTRIC_MOTOR).size() == 2,
            "equipment can be filtered by category");
        expect(equipment.findAllEquipment(MECHANICAL).size() == 1,
            "equipment can be filtered by department");
        expect(equipment.findAllEquipment(OPERATIONAL).size() == 2,
            "equipment can be filtered by status");

        expect(equipment.findAllEquipment(App::SearchFilter::CATEGORY, 0).size() == 2,
            "the category search-filter overload is wired to the category lookup");
        expect(equipment.findAllEquipment(App::SearchFilter::STATUS, 2).size() == 1,
            "the status search-filter overload is wired to the status lookup");
        expect(equipment.findAllEquipment(App::SearchFilter::DEPARTMENT, 2).size() == 1,
            "the department search-filter overload is wired to the department lookup");
        expect(equipment.findAllEquipment(App::SearchFilter::ALL, 0).size() == 4,
            "the all search-filter overload lists every equipment");
        expectThrows<std::logic_error>(
            [&] { (void) equipment.findAllEquipment(static_cast<App::SearchFilter>(99), 0); },
            "an unknown search filter is rejected");

        expect(equipment.findEquipmentById("EQ-2").getName() == "Bench Grinder",
            "equipment can be found by identifier");
        expect(equipment.findEquipmentByName("Welding Torch").getId() == "EQ-3",
            "equipment can be found by unique name");
        expectThrows<std::logic_error>(
            [&] { (void) equipment.findEquipmentById("missing"); },
            "an unknown equipment identifier is rejected by the finder");
        expectThrows<std::logic_error>(
            [&] { (void) equipment.findEquipmentByName("missing"); },
            "an unknown equipment name is rejected by the finder");
    }

    void testInspectionRepository()
    {
        using enum Equipment::Status;

        Database database;
        auto& equipment { database.equipmentData() };
        auto& inspections { database.inspectionsData() };

        equipment.registerEquipment(makeEquipment(
            "EQ-1", "Drill Press", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::ELECTRICAL, OPERATIONAL));
        equipment.registerEquipment(makeEquipment(
            "EQ-2", "Bench Grinder", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::MECHANICAL, UNDER_MAINTENANCE));
        equipment.registerEquipment(makeEquipment(
            "EQ-3", "Welding Torch", Equipment::Category::WELDING_TOOL,
            Equipment::Department::WELDING, UNAVAILABLE));

        Inspection failedInspection { "EQ-1" };
        failedInspection.setTemperature(81.0f);
        inspections.recordInspection(failedInspection);

        Inspection lowHoursInspection { "EQ-2" };
        lowHoursInspection.setOperatingHours(1);
        inspections.recordInspection(lowHoursInspection);

        Inspection cleanInspection { "EQ-3" };
        cleanInspection.setTemperature(50.0f);
        inspections.recordInspection(cleanInspection);

        expectThrows<std::invalid_argument>(
            [&] { inspections.recordInspection(Inspection { "missing" }); },
            "inspection for unknown equipment is rejected");

        expect(inspections.findAllInspections().size() == 3, "all inspections can be listed");
        expect(inspections.findAllInspections("EQ-1").size() == 1,
            "inspections can be filtered by equipment identifier");
        expect(inspections.findAllInspections(equipment.findEquipmentById("EQ-2")).size() == 1,
            "inspections can be filtered by equipment record");
        expect(inspections.findAllInspections(Equipment::Category::ELECTRIC_MOTOR).size() == 2,
            "inspections can be filtered by equipment category");
        expect(inspections.findAllInspections(UNDER_MAINTENANCE).size() == 1,
            "inspections can be filtered by equipment status");
        expect(inspections.findAllInspections(Equipment::Department::WELDING).size() == 1,
            "inspections can be filtered by equipment department");
        expect(inspections.findAllInspections(App::SearchFilter::CATEGORY, 0).size() == 2,
            "the inspection search-filter overload is wired to the category lookup");
        expectThrows<std::logic_error>(
            [&] { (void) inspections.findAllInspections(static_cast<App::SearchFilter>(99), 0); },
            "an unknown inspection search filter is rejected");

        expect(inspections.findLatestInspections({ OUTSIDE_LIMITS, UNAVAILABLE }).size() == 2,
            "latest inspections are filtered by operating status");

        const auto existingEquipment { failedInspection.getEquipment(equipment) };
        expect(existingEquipment.has_value() &&
                existingEquipment->get().getId() == "EQ-1",
            "an inspection can resolve its existing equipment by reference");

        Inspection orphan { "missing" };
        expect(!orphan.getEquipment(equipment).has_value(),
            "an inspection returns an empty optional when its equipment is missing");
    }

    void testMaintenanceRepository()
    {
        Database database;
        auto& equipment { database.equipmentData() };
        auto& maintenanceRecords { database.maintenanceData() };

        equipment.registerEquipment(makeEquipment(
            "EQ-1", "Drill Press", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::ELECTRICAL, Equipment::Status::OPERATIONAL));
        equipment.registerEquipment(makeEquipment(
            "EQ-2", "Bench Grinder", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::MECHANICAL, Equipment::Status::UNDER_MAINTENANCE));

        Maintenance maintenance {
            "EQ-1", Maintenance::Type::MONTHLY, "Replace belt",
            Equipment::Status::OPERATIONAL, Equipment::Status::UNDER_MAINTENANCE
        };
        maintenanceRecords.recordMaintenance(maintenance);

        expectThrows<std::invalid_argument>(
            [&] {
                maintenanceRecords.recordMaintenance(Maintenance {
                    "missing", Maintenance::Type::WEEKLY, "Check",
                    Equipment::Status::OPERATIONAL, Equipment::Status::OPERATIONAL
                });
            },
            "maintenance for unknown equipment is rejected");

        expect(maintenanceRecords.findAllMaintenance().size() == 1,
            "all maintenance records can be listed");
        expect(maintenanceRecords.findAllMaintenance("EQ-1").size() == 1,
            "maintenance records can be filtered by equipment identifier");
        expect(maintenanceRecords.findAllMaintenance(equipment.findEquipmentById("EQ-1")).size() == 1,
            "maintenance records can be filtered by equipment record");
        expect(maintenanceRecords.findAllMaintenance(Equipment::Category::ELECTRIC_MOTOR).size() == 1,
            "maintenance records can be filtered by equipment category");
        expect(maintenanceRecords.findAllMaintenance(Equipment::Status::OPERATIONAL).size() == 1,
            "maintenance records can be filtered by equipment status");
        expect(maintenanceRecords.findAllMaintenance(Equipment::Department::ELECTRICAL).size() == 1,
            "maintenance records can be filtered by equipment department");
        expect(maintenanceRecords.findAllMaintenance(App::SearchFilter::ALL, 0).size() == 1,
            "the all maintenance search-filter overload lists every record");
        expectThrows<std::logic_error>(
            [&] { (void) maintenanceRecords.findAllMaintenance(static_cast<App::SearchFilter>(99), 0); },
            "an unknown maintenance search filter is rejected");

        const auto existingEquipment { maintenance.getEquipment(equipment) };
        expect(existingEquipment.has_value() && existingEquipment->getId() == "EQ-1",
            "maintenance can resolve its existing equipment");

        Maintenance orphan {
            "missing", Maintenance::Type::WEEKLY, "Check",
            Equipment::Status::OPERATIONAL, Equipment::Status::OPERATIONAL
        };
        expect(!orphan.getEquipment(equipment).has_value(),
            "maintenance returns an empty optional when its equipment is missing");
    }

    void testDatabaseSummaryAndDeletion()
    {
        using enum Equipment::Status;

        Database database;
        auto& equipment { database.equipmentData() };

        equipment.registerEquipment(makeEquipment(
            "EQ-1", "Drill Press", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::ELECTRICAL, OPERATIONAL));
        equipment.registerEquipment(makeEquipment(
            "EQ-2", "Bench Grinder", Equipment::Category::ELECTRIC_MOTOR,
            Equipment::Department::MECHANICAL, UNDER_MAINTENANCE));
        equipment.registerEquipment(makeEquipment(
            "EQ-3", "Welding Torch", Equipment::Category::WELDING_TOOL,
            Equipment::Department::WELDING, UNAVAILABLE));

        auto summary { database.getSummary() };
        expect(summary.registeredCount == 3, "summary counts registered equipment");
        expect(summary.operationalCount == 1, "summary counts operational equipment");
        expect(summary.maintenanceCount == 1, "summary counts equipment under maintenance");
        expect(summary.unavailableCount == 1, "summary counts unavailable equipment");
        expect(summary.attentionRequiredCount == 0,
            "equipment with no inspection does not require attention");

        Inspection failedInspection { "EQ-1" };
        failedInspection.setTemperature(81.0f);
        database.inspectionsData().recordInspection(failedInspection);

        summary = database.getSummary();
        expect(summary.attentionRequiredCount == 1,
            "summary counts a failing latest inspection as requiring attention");

        equipment.findEquipmentById("EQ-1").setStatus(OPERATIONAL);
        summary = database.getSummary();
        expect(summary.attentionRequiredCount == 1,
            "a manual status change does not hide a failing latest inspection");

        Inspection unavailableInspection { "EQ-2" };
        unavailableInspection.setOperatingHours(1);
        database.inspectionsData().recordInspection(unavailableInspection);
        expect(database.getSummary().attentionRequiredCount == 2,
            "an unavailable inspection requires attention");

        const auto rendered { Database::summaryToString(database.getSummary()) };
        expect(rendered.find("Registered:") != std::string::npos,
            "the summary output includes the registered count");
        expect(rendered.find("Requires attention:") != std::string::npos,
            "the summary output includes the attention count");

        Maintenance maintenance {
            "EQ-2", Maintenance::Type::MONTHLY, "Replace belt",
            UNDER_MAINTENANCE, OPERATIONAL
        };
        database.maintenanceData().recordMaintenance(maintenance);

        const auto removed { database.deleteEquipment("EQ-1") };
        expect(removed.has_value() && removed->getId() == "EQ-1",
            "equipment deletion returns the removed record");
        expect(database.inspectionsData().findAllInspections("EQ-1").empty(),
            "deletion removes related inspections");
        expect(database.maintenanceData().findAllMaintenance("EQ-2").size() == 1,
            "deletion does not remove unrelated maintenance records");
        expect(!database.deleteEquipment("missing").has_value(),
            "unknown equipment deletion is explicit");
    }
}

int main()
{
    testEquipmentValues();
    testInspectionStatusBoundaries();
    testMaintenanceValues();
    testAppConversions();
    testRepositories();
    testInspectionRepository();
    testMaintenanceRepository();
    testDatabaseSummaryAndDeletion();

    if (failures == 0) std::cout << "All unit tests passed\n";
    return failures == 0 ? 0 : 1;
}
