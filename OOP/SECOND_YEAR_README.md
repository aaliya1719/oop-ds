# Simple Hospital System - Second-Year OOP Version

This is a smaller, syllabus-oriented version of the patient-management project.
It is intentionally simpler than `main.cpp` and is suitable for a second-year
engineering OOP practical or mini-project.

## Compile and run

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic second_year_patient_system.cpp -o second_year_patient_system.exe
.\second_year_patient_system.exe
```

## What this version demonstrates

| Syllabus topic | Where it is used |
|---|---|
| Namespace | `using namespace std` |
| Structures/enumeration | `AppointmentStatus` |
| Classes and objects | `Patient`, `Doctor`, `Appointment`, `HospitalSystem` |
| Encapsulation | Private data and public methods |
| Data abstraction | Menu methods hide internal operations |
| Inheritance | `Patient` and `Doctor` inherit from `Person` |
| Protected members | `Person::id`, `name`, and `phone` |
| Runtime polymorphism | Virtual `display()` and `fileLine()` |
| Abstract class | `Person` has pure virtual methods |
| Virtual destructor | `virtual ~Person()` |
| Constructors/destructor | Constructors in all classes and `HospitalSystem::~HospitalSystem()` |
| Pointers to objects | `Person* findPerson(...)` |
| Smart pointers | `unique_ptr<Person>` |
| `this`-style object ownership | Objects are managed through class methods |
| Operator overloading | `Appointment::operator<` and `operator<<` |
| Function template | `displayAll()` is called by `showPeople()` |
| STL vector | `vector<unique_ptr<Person>>`, `vector<Appointment>` |
| STL algorithm | `sort()` |
| Exception handling | `invalid_argument` and `runtime_error` |
| File handling | `ofstream` in `save()`, `ifstream` in `load()` |

## Why it is easier than the first version

- Fewer fields
- Simple patient-name searching
- Simple but real `YYYY-MM-DD` date validation, including leap years
- Simple file loading parser
- No large menu system
- Clear base class and derived classes
- Each syllabus concept is visible in a small amount of code

This makes it easier to explain in a viva and easier to extend step by step.

## Validation and persistence improvements

- Booking checks that the patient ID belongs to a `Patient` and the doctor ID
  belongs to a `Doctor`; a valid ID of the wrong type is rejected.
- Appointment dates must use `YYYY-MM-DD` and must be real calendar dates.
- The pipe character is replaced with `/` in user text because `|` is the
  separator used by the simple text-file format.
- Patient, doctor, and appointment records are loaded from
  `second_year_data.txt` when the program starts.
- If no save file exists, the program uses sample data.
- Sample records advance the next patient and doctor IDs, so new records do not
  reuse IDs `1001` or `501`.
- The save file is updated when option `8` or option `0` is selected.

## Advanced features used intentionally

The program uses `unique_ptr`, `dynamic_cast`, an abstract base class, and
`move()` because they demonstrate the Unit II inheritance/pointers syllabus:

- `unique_ptr<Person>` owns each patient or doctor and releases memory
  automatically.
- `dynamic_cast` checks whether a base-class pointer refers to a `Patient` or a
  `Doctor`.
- `Person` is abstract because it has pure virtual functions.
- `move()` transfers the temporary loaded vectors into the system without
  copying every record.

For a viva, explain these as safety and polymorphism features rather than as
database features. The project still uses a text file, not a full database.
