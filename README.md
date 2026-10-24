# Workshop Equipment Management System

A C++23 console application for registering workshop equipment, recording maintenance and inspections, searching inventory, and viewing operational summaries. Records are held in memory for the duration of one session.

## Requirements

- A C++23-capable compiler with `<print>` support (the project is tested with `g++` 14+)
- GNU Make

## Build and run

```sh
make build     # build/debug/equip_ops
make run       # start the interactive console program
```

## Quality checks

```sh
make test      # compile and run unit tests
make release   # optimized build at build/release/equip_ops
make sanitize  # address and undefined-behaviour sanitizer test run
make format    # format source with clang-format
```

The GitHub Actions workflow runs the test suite and creates a release build for every push and pull request.

## Project layout

- `app/` — program entry point
- `src/` — equipment, inspection, maintenance, repository, and UI code
- `tests/unit/` — non-interactive domain tests
- `SPECIFICATION.md` — project requirements

## Notes

The application rejects duplicate equipment identifiers and prevents inspection or maintenance records from being created for unregistered equipment. Removing equipment also removes its related records.
