# Bug & Spec-Gap Tracker

Offline copy of the **open** issues (mirrors GitHub — see each section's link).
Re-run the repro before closing anything; checked boxes are not proof of a fix.

Integration PR: [#11](https://github.com/FranklinEkemezie/equip-ops/pull/11). Hold merge until #8 and #9 are fixed.

## Build and reproduce

```sh
make build     # build/debug/equip_ops
make test      # non-interactive test suite
```

All repro commands below assume the debug binary and are run from the project root.

## Index

- [ ] [#2 — Exception handling: over-broad `try` blocks remain](#2--exception-handling-over-broad-try-blocks-remain) — `low`
- [ ] [#5 — Reporting: attention count still wrong](#5--reporting-attention-count-still-wrong) — `high`
- [ ] [#6 — Code quality: remaining copies and dead finder](#6--code-quality-remaining-copies-and-dead-finder) — `low`
- [ ] [#8 — Deleting equipment leaves it in the inventory](#8--deleting-equipment-leaves-it-in-the-inventory) — `critical`
- [ ] [#9 — Latest-inspection tracking counts stale records](#9--latest-inspection-tracking-counts-stale-records) — `high`
- [ ] [#10 — UI feedback labels success/EOF as errors](#10--ui-feedback-labels-successeof-as-errors) — `low`

---

## 2 — Exception handling: over-broad `try` blocks remain

GitHub: [#2](https://github.com/FranklinEkemezie/equip-ops/issues/2)

Invalid department/maintenance choices now re-prompt, and menu errors are no longer confused with lookups in the update flow. The remaining problem is structural: `handleRecordMaintenanceActivity` (`src/MainMenuHandler.cpp:196-231`) and `handleRecordEquipmentInspection` (`src/MainMenuHandler.cpp:249-279`) still wrap their prompts inside the `try` whose `catch` reports `"Failed to find equipment with ID: ..."`.

**Expected**: only the lookup sits in that `try`; prompt/record failures report their own cause.

- [ ] Narrow both `try` blocks to the equipment lookup
- [ ] Verify an interrupted prompt does not produce a bogus "not found" message

---

## 5 — Reporting: attention count still wrong

GitHub: [#5](https://github.com/FranklinEkemezie/equip-ops/issues/5)

Name search now returns every match, and identifier lookup is documented as the direct-view flow (menu option 2) rather than an inventory filter. The remaining defect is the attention count, caused by [#9](#9--latest-inspection-tracking-counts-stale-records).

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n81\n\n\n7\nEQ-1\n50\n\n\n9\n0\n' | ./build/debug/equip_ops
```

Observed: `Requires attention: 1` even though the latest inspection is clean.

- [ ] Fix [#9](#9--latest-inspection-tracking-counts-stale-records), then re-run the repro — expect `Requires attention: 0`

---

## 6 — Code quality: remaining copies and dead finder

GitHub: [#6](https://github.com/FranklinEkemezie/equip-ops/issues/6)

Typos, reserved include guards, the missing `<stdexcept>` include, `exit()`, and most dead code are fixed. Remaining:

- `Maintenance::getEquipment` (`src/Maintenance.cpp:67`) still returns `std::optional<Equipment>` by value, so filtered maintenance searches still copy a full record per inspection. `Inspection::getEquipment` now returns an optional reference; make the two consistent.
- `EquipmentRepository::findEquipmentByName` (`src/EquipmentRepository.cpp:52`) is no longer used by the UI after the multi-match name search. Remove it or use it.
- `EquipmentRepository::findAllEquipment(std::string name)` takes its name by value; prefer `std::string_view`.

- [ ] Return a non-owning reference from `Maintenance::getEquipment`
- [ ] Remove or justify the unused `findEquipmentByName`
- [ ] Avoid the name-string copy in the name search

---

## 8 — Deleting equipment leaves it in the inventory

GitHub: [#8](https://github.com/FranklinEkemezie/equip-ops/issues/8)

`Database::deleteEquipment` (`src/Database.cpp:21-42`) returns the matching record and deletes related inspections/maintenance, but never erases the equipment from the inventory vector.

**Reproduce**

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n5\nEQ-1\n2\nEQ-1\n9\n0\n' | ./build/debug/equip_ops
```

Observed: removal reports success, then option 2 still finds `EQ-1` and the summary still reports `Registered: 1`.

**Expected**: removal erases the equipment; a later lookup reports it does not exist.

- [ ] Erase the matched equipment before/with the cascade
- [ ] Re-run the repro — lookup fails and the registered count drops
- [ ] Add a regression test asserting the equipment is gone

---

## 9 — Latest-inspection tracking counts stale records

GitHub: [#9](https://github.com/FranklinEkemezie/equip-ops/issues/9)

`InspectionRepository::findLatestInspections` (`src/InspectionRepository.cpp:115-150`) never appends matched equipment IDs to `equipmentIds`, so every matching inspection counts, including superseded ones; one equipment can contribute multiple times.

**Reproduce**: record a failing inspection, then a newer clean one, then show the summary.

```sh
printf '1\nEQ-1\nDrill\n1\n1\n1\n7\nEQ-1\n81\n\n\n7\nEQ-1\n50\n\n\n9\n0\n' | ./build/debug/equip_ops
```

Observed: `Requires attention: 1`; expected `0`.

- [ ] Track processed equipment IDs and keep only the newest usable inspection per equipment
- [ ] Replace the `static_cast<int>(size() - 1)` reverse loop with reverse iterators
- [ ] Add regression tests for stale-failure and multiple-failure cases

---

## 10 — UI feedback labels success/EOF as errors

GitHub: [#10](https://github.com/FranklinEkemezie/equip-ops/issues/10)

- A successful name search prints `[ERROR] Found N equipment...` (`src/MainMenuHandler.cpp:124-125`); use `printInfo`.
- EOF at the main menu prints `[ERROR] Something went wrong` before exiting successfully (`app/main.cpp:20-24`); a clean EOF should terminate quietly or informationally.

- [ ] Report successful searches as info
- [ ] Treat clean EOF as a normal exit
