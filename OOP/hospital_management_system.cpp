#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum class AppointmentStatus
{
    Scheduled,
    Cancelled
};

class Person
{
protected:
    int id;
    string name;
    string phone;

public:
    Person(int personId, const string &personName, const string &personPhone)
        : id(personId), name(personName), phone(personPhone) {}

    virtual ~Person() = default;

    int getId() const { return id; }
    string getName() const { return name; }

    virtual void display() const = 0;
    virtual string fileLine() const = 0;
};

class Patient : public Person
{
private:
    int age;
    string disease;

public:
    Patient(int patientId, const string &patientName, const string &patientPhone,
            int patientAge, const string &patientDisease)
        : Person(patientId, patientName, patientPhone),
          age(patientAge), disease(patientDisease) {}

    void display() const override
    {
        cout << "Patient ID: " << id << " | Name: " << name
             << " | Phone: " << phone << " | Age: " << age
             << " | Disease: " << disease << '\n';
    }

    string fileLine() const override
    {
        return "P|" + to_string(id) + "|" + name + "|" + phone + "|" +
               to_string(age) + "|" + disease;
    }
};

class Doctor : public Person
{
private:
    string specialization;

public:
    Doctor(int doctorId, const string &doctorName, const string &doctorPhone,
           const string &doctorSpecialization)
        : Person(doctorId, doctorName, doctorPhone),
          specialization(doctorSpecialization) {}

    void display() const override
    {
        cout << "Doctor ID: " << id << " | Name: " << name
             << " | Phone: " << phone
             << " | Specialization: " << specialization << '\n';
    }

    string fileLine() const override
    {
        return "D|" + to_string(id) + "|" + name + "|" + phone + "|" +
               specialization;
    }
};

class Appointment
{
private:
    int appointmentId;
    int patientId;
    int doctorId;
    string date;
    AppointmentStatus status;

public:
    Appointment(int appointmentNumber, int patientNumber, int doctorNumber,
                const string &appointmentDate)
        : appointmentId(appointmentNumber), patientId(patientNumber),
          doctorId(doctorNumber), date(appointmentDate),
          status(AppointmentStatus::Scheduled) {}

    int getId() const { return appointmentId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }

    void cancel() { status = AppointmentStatus::Cancelled; }
    void restoreStatus(AppointmentStatus appointmentStatus) { status = appointmentStatus; }

    bool operator<(const Appointment &other) const
    {
        return date < other.date;
    }

    friend ostream &operator<<(ostream &output, const Appointment &appointment)
    {
        output << "Appointment ID: " << appointment.appointmentId
               << " | Patient ID: " << appointment.patientId
               << " | Doctor ID: " << appointment.doctorId
               << " | Date: " << appointment.date
               << " | Status: "
               << (appointment.status == AppointmentStatus::Scheduled
                       ? "Scheduled"
                       : "Cancelled");
        return output;
    }

    string fileLine() const
    {
        return to_string(appointmentId) + "|" + to_string(patientId) + "|" +
               to_string(doctorId) + "|" + date + "|" +
               to_string(static_cast<int>(status));
    }
};

template <typename T>
void displayAll(const vector<T *> &records)
{
    if (records.empty())
    {
        cout << "No records found.\n";
        return;
    }
    for (const T *record : records)
    {
        record->display();
    }
}

class HospitalSystem
{
private:
    vector<unique_ptr<Person>> people;
    vector<Appointment> appointments;
    int nextPatientId = 1001;
    int nextDoctorId = 501;
    int nextAppointmentId = 1;
    const string fileName = "second_year_data.txt";

    static string lower(string value)
    {
        transform(value.begin(), value.end(), value.begin(),
                  [](unsigned char character) {
                      return static_cast<char>(tolower(character));
                  });
        return value;
    }

    static string readText(const string &prompt)
    {
        string value;
        do
        {
            cout << prompt;
            getline(cin, value);
            if (!value.empty())
            {
                for (char &character : value)
                {
                    if (character == '|')
                        character = '/';
                }
                return value;
            }
            cout << "Value cannot be empty.\n";
        } while (true);
    }

    static bool isLeapYear(int year)
    {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    static bool isValidDate(const string &date)
    {
        if (date.size() != 10 || date[4] != '-' || date[7] != '-')
            return false;
        for (size_t index = 0; index < date.size(); ++index)
        {
            if (index != 4 && index != 7 &&
                !isdigit(static_cast<unsigned char>(date[index])))
                return false;
        }

        const int year = stoi(date.substr(0, 4));
        const int month = stoi(date.substr(5, 2));
        const int day = stoi(date.substr(8, 2));
        if (year < 1900 || month < 1 || month > 12 || day < 1)
            return false;

        const int daysInMonth[] = {
            31, isLeapYear(year) ? 29 : 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31};
        return day <= daysInMonth[month - 1];
    }

    static string readDate()
    {
        while (true)
        {
            const string date = readText("Date (YYYY-MM-DD): ");
            if (isValidDate(date))
                return date;
            cout << "Enter a valid date, for example 2026-11-05.\n";
        }
    }

    static int readInt(const string &prompt, int minimum, int maximum)
    {
        int value;
        while (true)
        {
            cout << prompt;
            if (cin >> value && value >= minimum && value <= maximum)
            {
                cin.ignore(10000, '\n');
                return value;
            }
            cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    Person *findPerson(int id)
    {
        for (const unique_ptr<Person> &person : people)
        {
            if (person->getId() == id)
                return person.get();
        }
        return nullptr;
    }

    Patient *findPatient(int id)
    {
        Person *person = findPerson(id);
        return dynamic_cast<Patient *>(person);
    }

    Doctor *findDoctor(int id)
    {
        Person *person = findPerson(id);
        return dynamic_cast<Doctor *>(person);
    }

    bool appointmentExists(int appointmentId) const
    {
        for (const Appointment &appointment : appointments)
        {
            if (appointment.getId() == appointmentId)
                return true;
        }
        return false;
    }

    void addPatient()
    {
        cout << "\n--- Add Patient ---\n";
        const int id = nextPatientId++;
        const string name = readText("Name: ");
        const string phone = readText("Phone: ");
        const int age = readInt("Age: ", 1, 120);
        const string disease = readText("Disease/problem: ");
        people.push_back(make_unique<Patient>(id, name, phone, age, disease));
        cout << "Patient added with ID " << id << ".\n";
    }

    void addDoctor()
    {
        cout << "\n--- Add Doctor ---\n";
        const int id = nextDoctorId++;
        const string name = readText("Name: ");
        const string phone = readText("Phone: ");
        const string specialization = readText("Specialization: ");
        people.push_back(make_unique<Doctor>(id, name, phone, specialization));
        cout << "Doctor added with ID " << id << ".\n";
    }

    void showPeople() const
    {
        cout << "\n--- People ---\n";
        vector<Person *> peopleToDisplay;
        for (const unique_ptr<Person> &person : people)
        {
            peopleToDisplay.push_back(person.get());
        }
        displayAll(peopleToDisplay);
    }

    void searchPatients() const
    {
        cout << "\n--- Search Patients ---\n";
        const string query = lower(readText("Enter patient name: "));
        bool found = false;
        for (const unique_ptr<Person> &person : people)
        {
            const Patient *patient = dynamic_cast<const Patient *>(person.get());
            if (patient != nullptr && lower(patient->getName()).find(query) != string::npos)
            {
                patient->display();
                found = true;
            }
        }
        if (!found)
        {
            cout << "No patient matched the search.\n";
        }
    }

    void bookAppointment()
    {
        cout << "\n--- Book Appointment ---\n";
        const int patientId = readInt("Patient ID: ", 1, 999999);
        const int doctorId = readInt("Doctor ID: ", 1, 999999);
        if (findPatient(patientId) == nullptr)
        {
            throw invalid_argument("The entered patient ID is not a patient ID.");
        }
        if (findDoctor(doctorId) == nullptr)
        {
            throw invalid_argument("The entered doctor ID is not a doctor ID.");
        }
        const string date = readDate();
        appointments.emplace_back(nextAppointmentId++, patientId, doctorId, date);
        cout << "Appointment booked.\n";
    }

    void showAppointments()
    {
        cout << "\n--- Appointments (sorted by date) ---\n";
        sort(appointments.begin(), appointments.end());
        if (appointments.empty())
        {
            cout << "No appointments found.\n";
            return;
        }
        for (const Appointment &appointment : appointments)
        {
            cout << appointment << '\n';
        }
    }

    void cancelAppointment()
    {
        cout << "\n--- Cancel Appointment ---\n";
        const int id = readInt("Appointment ID: ", 1, 999999);
        if (!appointmentExists(id))
        {
            throw invalid_argument("Appointment ID does not exist.");
        }
        for (Appointment &appointment : appointments)
        {
            if (appointment.getId() == id)
            {
                appointment.cancel();
                cout << "Appointment cancelled.\n";
                return;
            }
        }
    }

    void save() const
    {
        ofstream output(fileName);
        if (!output)
        {
            throw runtime_error("Could not open data file for writing.");
        }
        for (const unique_ptr<Person> &person : people)
        {
            output << person->fileLine() << '\n';
        }
        output << "APPOINTMENTS\n";
        for (const Appointment &appointment : appointments)
        {
            output << appointment.fileLine() << '\n';
        }
    }

    static vector<string> split(const string &line)
    {
        vector<string> fields;
        string field;
        size_t start = 0;
        size_t separator = line.find('|');
        while (separator != string::npos)
        {
            fields.push_back(line.substr(start, separator - start));
            start = separator + 1;
            separator = line.find('|', start);
        }
        fields.push_back(line.substr(start));
        return fields;
    }

    void loadSampleData()
    {
        people.push_back(make_unique<Patient>(
            1001, "Aarav Sharma", "9876543210", 20, "Fever"));
        people.push_back(make_unique<Doctor>(
            501, "Dr. Ananya Rao", "9000011111", "General Physician"));
        nextPatientId = max(nextPatientId, 1002);
        nextDoctorId = max(nextDoctorId, 502);
    }

    void load()
    {
        ifstream input(fileName);
        if (!input)
        {
            loadSampleData();
            return;
        }

        vector<unique_ptr<Person>> loadedPeople;
        vector<Appointment> loadedAppointments;
        string line;
        bool readingAppointments = false;

        try
        {
            while (getline(input, line))
            {
                if (line.empty())
                    continue;
                if (line == "APPOINTMENTS")
                {
                    readingAppointments = true;
                    continue;
                }

                const vector<string> fields = split(line);
                if (!readingAppointments && fields[0] == "P" && fields.size() == 6)
                {
                    loadedPeople.push_back(make_unique<Patient>(
                        stoi(fields[1]), fields[2], fields[3], stoi(fields[4]), fields[5]));
                }
                else if (!readingAppointments && fields[0] == "D" && fields.size() == 5)
                {
                    loadedPeople.push_back(make_unique<Doctor>(
                        stoi(fields[1]), fields[2], fields[3], fields[4]));
                }
                else if (readingAppointments && fields.size() == 5)
                {
                    const int status = stoi(fields[4]);
                    if (status < 0 || status > 1)
                        throw invalid_argument("Invalid appointment status.");
                    if (!isValidDate(fields[3]))
                        throw invalid_argument("Invalid appointment date.");
                    Appointment appointment(
                        stoi(fields[0]), stoi(fields[1]), stoi(fields[2]), fields[3]);
                    if (status == static_cast<int>(AppointmentStatus::Cancelled))
                        appointment.cancel();
                    loadedAppointments.push_back(appointment);
                }
                else
                {
                    throw invalid_argument("Invalid record in saved data.");
                }
            }
        }
        catch (const invalid_argument &error)
        {
            throw runtime_error(string("Could not load saved data: ") + error.what());
        }
        catch (const out_of_range &)
        {
            throw runtime_error("Could not load saved data: numeric value is out of range.");
        }

        people = move(loadedPeople);
        appointments = move(loadedAppointments);
        for (const unique_ptr<Person> &person : people)
        {
            if (dynamic_cast<const Patient *>(person.get()) != nullptr)
                nextPatientId = max(nextPatientId, person->getId() + 1);
            if (dynamic_cast<const Doctor *>(person.get()) != nullptr)
                nextDoctorId = max(nextDoctorId, person->getId() + 1);
        }
        for (const Appointment &appointment : appointments)
            nextAppointmentId = max(nextAppointmentId, appointment.getId() + 1);
    }

public:
    HospitalSystem()
    {
        try
        {
            load();
        }
        catch (const runtime_error &error)
        {
            cout << "File error: " << error.what() << '\n';
            cout << "Starting with sample data instead.\n";
            people.clear();
            appointments.clear();
            loadSampleData();
        }
    }

    ~HospitalSystem()
    {
        cout << "HospitalSystem object destroyed. Memory released automatically.\n";
    }

    void run()
    {
        int choice;
        do
        {
            cout << "\n===== SIMPLE HOSPITAL SYSTEM =====\n"
                 << "1. Add patient\n"
                 << "2. Add doctor\n"
                 << "3. Show patients and doctors\n"
                 << "4. Search patients\n"
                 << "5. Book appointment\n"
                 << "6. Show appointments\n"
                 << "7. Cancel appointment\n"
                 << "8. Save data\n"
                 << "0. Exit\n";
            choice = readInt("Choice: ", 0, 8);

            try
            {
                switch (choice)
                {
                case 1:
                    addPatient();
                    break;
                case 2:
                    addDoctor();
                    break;
                case 3:
                    showPeople();
                    break;
                case 4:
                    searchPatients();
                    break;
                case 5:
                    bookAppointment();
                    break;
                case 6:
                    showAppointments();
                    break;
                case 7:
                    cancelAppointment();
                    break;
                case 8:
                    save();
                    cout << "Data saved to " << fileName << ".\n";
                    break;
                case 0:
                    save();
                    cout << "Data saved. Goodbye.\n";
                    break;
                }
            }
            catch (const invalid_argument &error)
            {
                cout << "Input error: " << error.what() << '\n';
            }
            catch (const runtime_error &error)
            {
                cout << "File error: " << error.what() << '\n';
            }
        } while (choice != 0);
    }
};

int main()
{
    HospitalSystem hospital;
    hospital.run();
    return 0;
}
