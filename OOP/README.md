# Patient Care Manager

A terminal-based C++17 OOP mini project for managing:

- Patient records
- Doctor details
- Appointments

The application starts with sample data on the first run. On exit, it saves the
current records to `patient_care_data.txt` and loads them again the next time it
starts.

## Features

1. Register and list patients
2. Search patients by name or phone
3. Add and list doctors
4. Schedule appointments with date/time validation
5. Prevent double-booking the same doctor and time slot
6. View and cancel appointments
7. Save and load data automatically

## OOP concepts demonstrated

- **Encapsulation:** `HealthcareSystem` owns the data and operations.
- **Abstraction:** menu actions hide validation, lookup, and persistence details.
- **Composition:** the system contains collections of `Patient`, `Doctor`, and
  `Appointment` objects.
- **Structs/classes:** domain entities are represented as simple data structures,
  while application behavior is grouped in a class.

## Build and run

### With CMake

```powershell
cmake -S . -B build
cmake --build build
.\build\Debug\patient_care_manager.exe
```

For a single-configuration generator, the executable may be directly under
`build`.

### With g++

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o patient_care_manager.exe
.\patient_care_manager.exe
```

## Example data

The initial sample records use IDs `1001`, `1002` for patients and `501`, `502`
for doctors. Appointment dates use `YYYY-MM-DD`, and times use 24-hour `HH:MM`.
