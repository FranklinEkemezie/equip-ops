# Project 2 — Workshop Equipment Management System

## Objective

Build a more substantial command-line workshop equipment management system for a small industrial maintenance workshop.

The workshop maintains a collection of equipment belonging to different departments. Equipment can be registered, inspected, assigned maintenance work, and marked as operational, under maintenance, or unavailable.

The system should allow a maintenance operator to manage the workshop's records during a program session and provide useful operational summaries.

The project should combine the C++ fundamentals from Chapter 1 with the design and organizational practices introduced throughout the chapter's larger Employee Records example.

## Core Requirements

### Equipment Management

The system must maintain multiple equipment records.

Each equipment record should contain information sufficient to identify and describe the equipment, such as:

- [x] Unique identifier.
- Name.
- Equipment category.
- Department/location.
- Operational status.
- Relevant operating information.

The system must allow the operator to:

- [x] Register equipment.
- [x] View equipment information.
- [x] Search for equipment.
- [x] Update its operational status.
- [x] Remove equipment from the active workshop inventory.

### Maintenance Records

The system must support recording maintenance activities against equipment.

A maintenance record should contain information such as:

- The equipment involved.
- The type of maintenance performed.
- A description of the work.
- The equipment's status before and after the work.

- [x] The system should prevent maintenance records from being associated with equipment that does not exist.

### Inspection

Equipment must be inspectable.

An inspection may record several measurements, but some measurements may be absent depending on the equipment being inspected.

- [x] The system must be able to determine whether the recorded inspection indicates that the equipment is operating within its defined limits.

### Searching and Reporting

The operator must be able to search the workshop inventory and obtain useful information from it.

- [x] At minimum, support searches or reports based on concepts such as:

- Equipment identifier.
- Equipment category.
- Operational status.
- Department.

- [x] The system should also provide summary information such as:

- Number of registered equipment items.
- Number currently operational.
- Number under maintenance.
- Number unavailable.
- Number requiring attention based on their latest inspection.

### User Interface

Provide a console interface through which an operator can perform the supported operations.

The interface should:

- Present available operations clearly.
- Validate user input.
- Handle invalid selections.
- Handle nonexistent equipment identifiers.
- Continue operating after recoverable errors.
- Provide meaningful feedback after operations.

## Additional Expectations

The application should demonstrate a deliberate separation between:

- Equipment/maintenance data.
- Operations performed on that data.
- User interaction.

Use appropriate value types and references rather than passing or copying large objects unnecessarily.

Where an operation may legitimately fail to produce a result—for example, searching for an unknown equipment identifier—the program should represent that situation explicitly rather than relying on special sentinel values.

The system should also distinguish between invalid input and legitimate operational states.

For example, these are different situations:

    Equipment does not exist.

and:

    Equipment exists but currently has no inspection recorded.

The program should represent those differences clearly.

## Constraints

- Console application only.
- No external libraries.
- No persistent database is required at this stage.
- Data only needs to remain available while the application is running.
- Use the C++ standard library where appropriate.
- Organize the application into multiple source/interface files.
- Do not use global mutable state as the application's data store.
- The system should remain usable after recoverable input errors.
- Do not introduce C++ features from later chapters as prerequisites.
- Do not implement persistence, networking, multithreading, or a graphical interface.

The application should be large enough to require meaningful organization, but not so large that its complexity comes from features unrelated to the material currently being studied.

## What You Decide

You decide:

- The overall architecture.
- The classes, structs, and other types.
- Which information belongs to which type.
- How equipment is stored and searched.
- How relationships between equipment, inspections, and maintenance records are represented.
- How operational states are represented.
- How measurement limits are represented.
- How inspection results are calculated.
- How failures and invalid operations are represented.
- How the user interface communicates with the underlying application logic.
- How source/interface files are organized.
- Where references, pointers, `const`, `std::optional`, `std::vector`, `std::array`, and other Chapter 1 facilities provide genuine value.
- What testing strategy you use for the non-interactive parts of the system.