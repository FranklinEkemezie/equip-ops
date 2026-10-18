# Bug & Spec-Gap Tracker

Offline copy of the reported issues (mirrors the GitHub issues — see each section's link).
Check boxes as you complete them. **Do not treat checked boxes as proof of a fix — re-run the repro.**

## How to build for reproduction

```sh
make build                      # binary: build/debug/equip_ops
make test                       # unit tests (currently pass)
```

All repro commands below assume the debug binary and are run from the project root.

## Index

- [ ] [#1 — Infinite error loop when stdin reaches EOF](#1--infinite-error-loop-when-stdin-reaches-EOF) — `critical`
- [ ] [#2 — Exception handling: invalid selections abort operations; errors misreported](#2--exception-handling-invalid-selections-abort-operations-errors-misreported) — `critical`
- [ ] [#3 — Input validation: wraparound, partial parses, 0-as-sentinel, empty fields](#3--input-validation-wraparound-partial-parses-0-as-sentinel-empty-fields) — `high`
- [ ] [#4 — `getEquipment` optional is always non-empty; not-found throws](#4--getequipment-optional-is-always-non-empty-not-found-throws) — `high`
- [ ] [#5 — Reporting & search correctness](#5--reporting--search-correctness) — `medium`
- [ ] [#6 — Code quality & polish](#6--code-quality--polish) — `low`
- [ ] [#7 — Specification gaps](#7--specification-gaps) — `spec`

---

## 1 — Infinite error loop when stdin reaches EOF

GitHub: [#1](https://github.com/FranklinEkemezie/equip-ops/issues/1)

When stdin reaches EOF (Ctrl-D, closed pipe, truncated piped input), the app loops forever printing `[ERROR] Invalid input` — never recovering or exiting.

**Locations**

- `src/Ui.cpp:54-62` — `promptString` returns `""` on failed `getline` without checking stream state.
- `src/Ui.cpp:64-82` — `promptFloat` / `promptInt` catch the `std::invalid_argument` from `std::stof`/`std::stoi("")`, print, and loop on an exhausted stream.

**Reproduce**

```sh
./build/debug/equip_ops < /dev/null
# or
printf '1\n' | ./build/debug/equip_ops
# or interactively: press Ctrl-D at any prompt
```

Observed: `[ERROR] Invalid input` / `Choose option:` repeats indefinitely (killed by timeout in testing).

**Expected**: EOF terminates the program cleanly instead of hanging it.

- [ ] Detect failed/EOF `getline` in `promptString` and exit (or throw a distinct handled error)
- [ ] Verify with both repro commands above — program exits, no error spam

---

## 2 — Exception handling: invalid selections abort operations; errors misreported

GitHub: [#2](https://github.com/FranklinEkemezie/equip-ops/issues/2)

### 2a. Wrong exception type in two prompts

`Ui::promptEquipmentDepartment` (`src/Ui.cpp:297`) and `Ui::promptMaintenanceTypeOption` (`src/Ui.cpp:315`) catch `std::invalid_argument`, but `getEquipmentDepartmentOption` (`src/Ui.cpp:218-231`) and `getMaintenanceTypeOption` (`src/Ui.cpp:233-247`) throw `std::logic_error`. An invalid-but-in-range choice escapes the re-prompt loop.
(`promptEquipmentCategory` at `src/Ui.cpp:261` and `promptEquipmentStatus` at `src/Ui.cpp:279` catch correctly — the four are inconsistent.)

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n9\n0\n' | ./build/debug/equip_ops
```

Observed: registration of `EQ-1` **aborts** with `[ERROR] Invalid option.` (from `main`). `"Invalid equipment department"` never prints and you're dumped back at the main menu instead of re-prompting.

**Expected**: re-prompt until a valid department/maintenance type is chosen (same as category/status prompts).

- [ ] Fix catch types in both prompts
- [ ] Re-run repro — expect re-prompt loop until valid input, registration completes

### 2b. `main` catch-all mislabels every escaped `logic_error`

`app/main.cpp:19` reports any escaped `logic_error` as `[ERROR] Invalid option.` regardless of cause (this is what masks 2a and issue #4).

**Expected**: messages match the actual failure.

- [ ] Distinguish menu-option errors from other failures at the top level

### 2c. Over-broad `try` blocks misattribute failures

`handleUpdateEquipment` (`src/MainMenuHandler.cpp:99-111`) and `handleRecordMaintenanceActivity` (`src/MainMenuHandler.cpp:137-160`) wrap their **prompts** inside a `try` that prints `"Failed to find equipment with ID: ..."`.

**Expected**: only the lookup itself sits in that `try`; prompt failures report their own cause.

- [ ] Narrow the try blocks around the actual lookups

---

## 3 — Input validation: wraparound, partial parses, 0-as-sentinel, empty fields

GitHub: [#3](https://github.com/FranklinEkemezie/equip-ops/issues/3)

### 3a. Negative operating hours wrap to ~4.29 billion

`src/MainMenuHandler.cpp:182-183` uses `Ui::promptInt` (accepts negatives) into `Inspection::setOperatingHours(unsigned int)` (`src/Inspection.cpp:31`).

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n50\n3\n-5\n9\n2\nEQ-1\n0\n' | ./build/debug/equip_ops
```

Observed: inspection records successfully; summary shows `Operational: 1` (hours became `4294967291`, which passes the ≥ 2 rule).

**Expected**: reject negative hours (range validation).

- [ ] Add bounds checking; re-run repro — expect a rejection/error, not success

### 3b. `std::stof` / `std::stoi` accept trailing garbage

`src/Ui.cpp:68, 78` — `"50abc"` silently parses as `50.0`.

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n50abc\nxyz3\n12\n2\nEQ-1\n0\n' | ./build/debug/equip_ops
```

Observed: temperature accepted as `50` (no error for `50abc`; `xyz3` correctly rejected).

**Expected**: whole string must be consumed, else re-prompt.

- [ ] Validate full-string parse; re-run repro — `50abc` rejected

### 3c. `0` doubles as "measurement unavailable" sentinel

`src/MainMenuHandler.cpp:176-183` — `if (temperature != 0.0f)` etc. A genuine 0 °C reading or 0 operating hours can never be recorded, and an all-zero inspection is stored as a valid (but empty) inspection.

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n0\n0\n0\n8\n2\n0\n0\n' | ./build/debug/equip_ops
```

Observed: `Inspection recorded successfully!` and inventory search reports `Found 1 inspection records` even though every measurement was "unavailable".

**Expected**: explicit skip mechanism (e.g. blank line = absent) driving `std::optional`; empty inspections either rejected or clearly distinguishable.

- [ ] Replace the 0-sentinel with explicit absent-measurement input
- [ ] Re-run repro — all-zero input should mean "absent", and a real `0` reading must be recordable

### 3d. Empty required fields accepted

`Ui::promptString` (`src/Ui.cpp:54-62`) accepts `""` for Equipment ID and Name.

**Reproduce**: choose option `1`, enter an empty ID/name — registration succeeds.

- [ ] Reject blank required fields at registration

---

## 4 — `getEquipment` optional is always non-empty; not-found throws

GitHub: [#4](https://github.com/FranklinEkemezie/equip-ops/issues/4)

`Inspection::getEquipment` (`src/Inspection.cpp:13-16`) and `Maintenance::getEquipment` (`src/Maintenance.cpp:67-70`) return `std::optional<Equipment>` but delegate to `findEquipmentById`, which **throws** `std::logic_error` when missing — so the optional is never empty, and the `if (!e.has_value()) continue;` guards at `src/InspectionRepository.cpp:61, 76, 91` and `src/MaintenanceRepository.cpp:62, 77, 92` are dead code for their intended case.

**Consequences**

1. Orphaned records make filtered searches throw → escapes to `app/main.cpp:19` → mislabeled `[ERROR] Invalid option.`
2. `EquipmentRepository::removeEquipment` (`src/EquipmentRepository.cpp:97-110`) is public and does **not** cascade-delete related records — only `Database::deleteEquipment` (`src/Database.cpp:20-34`) does. Direct calls create orphans.
3. Spec: not-found must be *represented explicitly*, not thrown; and "equipment does not exist" vs "no inspection recorded" must be distinguishable.

**Reproduce** (latent — not reachable through the current UI; write a temporary test or scratch code)

```cpp
Database db;
db.equipmentData().registerEquipment(
    Equipment { "EQ-1", "Drill", Equipment::Category::ELECTRIC_MOTOR,
                Equipment::Department::MECHANICAL, Equipment::Status::OPERATIONAL });
db.inspectionsData().recordInspection(Inspection { "EQ-1" });

db.equipmentData().removeEquipment("EQ-1");   // bypasses Database::deleteEquipment → orphan

// Dead guard in action — this THROWS instead of skipping:
db.inspectionsData().findAllInspections(Equipment::Category::ELECTRIC_MOTOR);
// → std::logic_error "Failed to find equipment with ID: EQ-1"
```

**Expected**: `getEquipment` returns `std::nullopt` for missing equipment; orphan path impossible or harmless.

- [ ] `getEquipment` returns `nullopt` instead of throwing (both classes)
- [ ] Remove/redirect the leaky `EquipmentRepository::removeEquipment` path so orphans can't be created
- [ ] Confirm the previously dead `has_value()` guards now work (or delete them if truly unnecessary)
- [ ] Add a unit test covering the scratch-code scenario above

---

## 5 — Reporting & search correctness

GitHub: [#5](https://github.com/FranklinEkemezie/equip-ops/issues/5)

### 5a. "Requires attention" ignores inspections (spec mismatch)

Spec (`SPECIFICATION.md:74`) requires attention count *"based on their latest inspection"*; `src/EquipmentRepository.cpp:144` just counts `OUTSIDE_LIMITS` status.

**Reproduce**

```sh
# Clean machine gets flagged without any inspection:
printf '1\nEQ-1\nDrill\n1\n1\n1\n4\nEQ-1\n2\n9\n0\n' | ./build/debug/equip_ops
# → "Requires attention: 1" despite no inspection ever recorded

# Bad inspection gets hidden by a manual status change:
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n81\n0\n0\n4\nEQ-1\n1\n9\n0\n' | ./build/debug/equip_ops
# → inspection recorded at 81°C (outside limits), status flipped back to
#    Operational via option 4 → "Requires attention: 0"
```

**Expected**: derive the count from each equipment's most recent inspection, with an explicit representation for "never inspected".

- [ ] Summary consults latest inspection per equipment
- [ ] Re-run both repros — results must reflect inspection history, not manual status

### 5b. Name search returns only the first match

`findEquipmentByName` (`src/EquipmentRepository.cpp:42-50`) returns one arbitrary match; only IDs are uniqueness-checked.

**Reproduce**: register two items with the same name, search (option 3) by that name — only one shown, no indication of duplicates.

- [ ] Enforce name uniqueness, or return/report all matches

### 5c. Inventory search has no ID filter

Spec (`SPECIFICATION.md:61-66`) lists equipment identifier as a search concept; menu option 8 only offers Category / Status / Department / None (`src/Ui.cpp:127-133`).

**Reproduce**: choose option `8` → filter menu lacks an ID choice.

- [ ] Add an ID filter to `App::SearchFilter` and the search-filter menu

---

## 6 — Code quality & polish

GitHub: [#6](https://github.com/FranklinEkemezie/equip-ops/issues/6)

Static findings — no runtime repro; locate with editor search.

- [ ] Typos: `src/Maintenance.cpp:84` `"Status Bfter"` (and misaligned `Before` label); `src/Equipment.cpp:62` `"Unkonwn"`; `src/MainMenuHandler.cpp:91` `"with named:"`
- [ ] Reserved include guards `__*_cppm` in every module (`src/*.cppm`) — rename to e.g. `EQUIP_OPS_APP_CPPM`
- [ ] `src/Equipment.cpp` throws `std::logic_error` without `#include <stdexcept>` (relies on transitive include)
- [ ] Dead code: unused `buffer` (`src/Inspection.cpp:60`); unused `App::SearchRequest` (`src/App.cppm:27-31`); unused `Database::deleteEquipment(Equipment)` overload (`src/Database.cpp:31-34`); pointless `Ui` instance/constructor (`app/main.cpp:12`, `src/Ui.cppm:18`)
- [ ] By-value `Equipment` parameters should be `const&`: `EquipmentRepository.cppm:40`, `InspectionRepository.cppm:27, 34`, `MaintenanceRepository.cppm:22, 29`, `Database.cppm:27`
- [ ] `getEquipment` returns `optional<Equipment>` by copy inside filter loops (`src/InspectionRepository.cpp:59, 74, 89`; `src/MaintenanceRepository.cpp:60, 75, 90`) — O(n) copies per search
- [ ] Replace `exit(EXIT_SUCCESS)` in `handleExitApplication` (`src/MainMenuHandler.cpp:265`) with a clean loop exit

**Verify after each**: `make build` (no new warnings with `-Wall -Wextra -Wpedantic`) and `make test`.

---

## 7 — Specification gaps

GitHub: [#7](https://github.com/FranklinEkemezie/equip-ops/issues/7)

- [ ] **"Relevant operating information" missing from equipment records** — spec `SPECIFICATION.md:26`; still a TODO comment at `src/Equipment.cppm:80`
- [ ] **Inspection limits are global, not per-equipment** — spec `SPECIFICATION.md:55` says *"its defined limits"*; hard-coded constants at `src/Inspection.cppm:41-43` apply to every category/equipment
- [ ] **Clarify `MIN_OPERATING_HOURS = 2` semantics** — `src/Inspection.cpp:45-48` marks a machine with 1 recorded hour `UNAVAILABLE`; intended meaning unclear
- [ ] **"Exists but has no inspection recorded" never surfaced** — spec `SPECIFICATION.md:101-111` requires this state to be distinguishable; no UI/query reports it
- [ ] **Test coverage for non-interactive core** — `tests/unit/ExampleTest.cpp` misses: `getEquipmentOperatingStatus` boundary values, `findAllEquipment(filter, value)` overloads, `*FromInt` conversion helpers

---

## Done criteria for any issue

- [ ] Repro steps above produce the *expected* behavior after the fix
- [ ] `make test` passes
- [ ] `make build` emits no new warnings
- [ ] GitHub issue closed (when working online)
