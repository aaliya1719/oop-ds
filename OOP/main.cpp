#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Patient {
    int id{};
    string name;
    int age{};
    string gender;
    string phone;
    string address;
    string bloodGroup;
};

struct Doctor {
    int id{};
    string name;
    string specialization;
    string phone;
    string availability;
};

struct Appointment {
    int id{};
    int patientId{};
    int doctorId{};
    string date;
    string time;
    string reason;
    string status{"Scheduled"};
};

class HealthcareSystem {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    int nextPatientId{1001};
    int nextDoctorId{501};
    int nextAppointmentId{1};
    const string dataFile{"patient_care_data.txt"};

    static string lower(string value) {
        transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(tolower(ch));
        });
        return value;
    }

    static string clean(string value) {
        for (char& ch : value) {
            if (ch == '|') {
                ch = '/';
            }
        }
        return value;
    }

    Patient* findPatient(int id) {
        for (auto& patient : patients) {
            if (patient.id == id) return &patient;
        }
        return nullptr;
    }

    const Patient* findPatient(int id) const {
        for (const auto& patient : patients) {
            if (patient.id == id) return &patient;
        }
        return nullptr;
    }

    Doctor* findDoctor(int id) {
        for (auto& doctor : doctors) {
            if (doctor.id == id) return &doctor;
        }
        return nullptr;
    }

    const Doctor* findDoctor(int id) const {
        for (const auto& doctor : doctors) {
            if (doctor.id == id) return &doctor;
        }
        return nullptr;
    }

    static void printLine(char symbol = '-', int width = 78) {
        cout << string(width, symbol) << '\n';
    }

    static void printHeader(const string& title) {
        cout << "\n";
        printLine('=');
        cout << "  " << title << '\n';
        printLine('=');
    }

    static string inputText(const string& prompt, bool required = true) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            if (!required || !value.empty()) return clean(value);
            cout << "  This field is required.\n";
        } while (true);
    }

    static int inputInt(const string& prompt, int minimum, int maximum) {
        while (true) {
            cout << prompt;
            string value;
            getline(cin, value);
            stringstream stream(value);
            int number;
            char extra;
            if ((stream >> number) && !(stream >> extra) &&
                number >= minimum && number <= maximum) {
                return number;
            }
            cout << "  Enter a number between " << minimum << " and " << maximum << ".\n";
        }
    }

    static bool isDateFormat(const string& date) {
        if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
        for (size_t i = 0; i < date.size(); ++i) {
            if (i == 4 || i == 7) continue;
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        return true;
    }

    static bool isTimeFormat(const string& time) {
        if (time.size() != 5 || time[2] != ':') return false;
        if (!isdigit(static_cast<unsigned char>(time[0])) ||
            !isdigit(static_cast<unsigned char>(time[1])) ||
            !isdigit(static_cast<unsigned char>(time[3])) ||
            !isdigit(static_cast<unsigned char>(time[4]))) {
            return false;
        }
        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
    }

    static string inputDate() {
        while (true) {
            string date = inputText("Date (YYYY-MM-DD): ");
            if (isDateFormat(date)) return date;
            cout << "  Use the format YYYY-MM-DD, for example 2026-11-05.\n";
        }
    }

    static string inputTime() {
        while (true) {
            string time = inputText("Time (HH:MM): ");
            if (isTimeFormat(time)) return time;
            cout << "  Use 24-hour time, for example 09:30 or 14:15.\n";
        }
    }

    bool appointmentSlotTaken(int doctorId, const string& date, const string& time) const {
        for (const auto& appointment : appointments) {
            if (appointment.doctorId == doctorId && appointment.date == date &&
                appointment.time == time && appointment.status == "Scheduled") {
                return true;
            }
        }
        return false;
    }

    void seedData() {
        patients = {
            {1001, "Aarav Sharma", 29, "Male", "9876543210", "12 Lake Road", "B+"},
            {1002, "Meera Patel", 41, "Female", "9123456780", "8 Green Avenue", "O+"}
        };
        doctors = {
            {501, "Dr. Ananya Rao", "Cardiologist", "9000011111", "Mon-Fri, 09:00-13:00"},
            {502, "Dr. Rohan Mehta", "General Physician", "9000022222", "Mon-Sat, 14:00-18:00"}
        };
        appointments = {
            {1, 1001, 501, "2026-11-05", "10:30", "Routine heart check-up", "Scheduled"}
        };
    }

    void printPatient(const Patient& patient) const {
        cout << left << setw(7) << patient.id << setw(22) << patient.name
             << setw(6) << patient.age << setw(10) << patient.gender
             << setw(14) << patient.phone << setw(10) << patient.bloodGroup
             << patient.address << '\n';
    }

    void printDoctor(const Doctor& doctor) const {
        cout << left << setw(7) << doctor.id << setw(22) << doctor.name
             << setw(22) << doctor.specialization << setw(14) << doctor.phone
             << doctor.availability << '\n';
    }

    void printAppointment(const Appointment& appointment) const {
        const Patient* patient = findPatient(appointment.patientId);
        const Doctor* doctor = findDoctor(appointment.doctorId);
        cout << left << setw(6) << appointment.id
             << setw(20) << (patient ? patient->name : "Unknown")
             << setw(20) << (doctor ? doctor->name : "Unknown")
             << setw(13) << appointment.date << setw(8) << appointment.time
             << setw(15) << appointment.status << appointment.reason << '\n';
    }

    void addPatient() {
        printHeader("Register New Patient");
        Patient patient;
        patient.id = nextPatientId++;
        patient.name = inputText("Full name: ");
        patient.age = inputInt("Age: ", 0, 130);
        patient.gender = inputText("Gender: ");
        patient.phone = inputText("Phone: ");
        patient.address = inputText("Address: ");
        patient.bloodGroup = inputText("Blood group: ");
        patients.push_back(patient);
        cout << "\nPatient registered successfully. Patient ID: " << patient.id << "\n";
    }

    void listPatients() const {
        printHeader("Patient Records");
        if (patients.empty()) {
            cout << "No patient records found.\n";
            return;
        }
        cout << left << setw(7) << "ID" << setw(22) << "Name" << setw(6) << "Age"
             << setw(10) << "Gender" << setw(14) << "Phone" << setw(10) << "Blood"
             << "Address\n";
        printLine();
        for (const auto& patient : patients) printPatient(patient);
    }

    void searchPatients() const {
        printHeader("Search Patients");
        string query = lower(inputText("Name or phone: "));
        bool found = false;
        for (const auto& patient : patients) {
            if (lower(patient.name).find(query) != string::npos ||
                lower(patient.phone).find(query) != string::npos) {
                if (!found) {
                    cout << left << setw(7) << "ID" << setw(22) << "Name" << setw(6) << "Age"
                         << setw(10) << "Gender" << setw(14) << "Phone" << setw(10) << "Blood"
                         << "Address\n";
                    printLine();
                }
                printPatient(patient);
                found = true;
            }
        }
        if (!found) cout << "No matching patient was found.\n";
    }

    void addDoctor() {
        printHeader("Add Doctor");
        Doctor doctor;
        doctor.id = nextDoctorId++;
        doctor.name = inputText("Doctor name: ");
        doctor.specialization = inputText("Specialization: ");
        doctor.phone = inputText("Phone: ");
        doctor.availability = inputText("Availability: ");
        doctors.push_back(doctor);
        cout << "\nDoctor added successfully. Doctor ID: " << doctor.id << "\n";
    }

    void listDoctors() const {
        printHeader("Doctor Directory");
        if (doctors.empty()) {
            cout << "No doctor records found.\n";
            return;
        }
        cout << left << setw(7) << "ID" << setw(22) << "Name" << setw(22)
             << "Specialization" << setw(14) << "Phone" << "Availability\n";
        printLine();
        for (const auto& doctor : doctors) printDoctor(doctor);
    }

    void scheduleAppointment() {
        printHeader("Schedule Appointment");
        if (patients.empty() || doctors.empty()) {
            cout << "At least one patient and one doctor are required.\n";
            return;
        }
        listPatients();
        int patientId = inputInt("Patient ID: ", 1, 999999);
        if (!findPatient(patientId)) {
            cout << "Patient ID not found.\n";
            return;
        }
        listDoctors();
        int doctorId = inputInt("Doctor ID: ", 1, 999999);
        if (!findDoctor(doctorId)) {
            cout << "Doctor ID not found.\n";
            return;
        }
        Appointment appointment;
        appointment.id = nextAppointmentId++;
        appointment.patientId = patientId;
        appointment.doctorId = doctorId;
        appointment.date = inputDate();
        appointment.time = inputTime();
        if (appointmentSlotTaken(doctorId, appointment.date, appointment.time)) {
            cout << "That doctor already has a scheduled appointment at this time.\n";
            return;
        }
        appointment.reason = inputText("Reason for visit: ");
        appointments.push_back(appointment);
        cout << "\nAppointment scheduled successfully. Appointment ID: "
             << appointment.id << "\n";
    }

    void listAppointments() const {
        printHeader("Appointment Schedule");
        if (appointments.empty()) {
            cout << "No appointments found.\n";
            return;
        }
        cout << left << setw(6) << "ID" << setw(20) << "Patient" << setw(20) << "Doctor"
             << setw(13) << "Date" << setw(8) << "Time" << setw(15) << "Status"
             << "Reason\n";
        printLine();
        for (const auto& appointment : appointments) printAppointment(appointment);
    }

    void cancelAppointment() {
        printHeader("Cancel Appointment");
        listAppointments();
        int id = inputInt("Appointment ID: ", 1, 999999);
        for (auto& appointment : appointments) {
            if (appointment.id == id) {
                if (appointment.status == "Cancelled") {
                    cout << "This appointment is already cancelled.\n";
                } else {
                    appointment.status = "Cancelled";
                    cout << "Appointment cancelled successfully.\n";
                }
                return;
            }
        }
        cout << "Appointment ID not found.\n";
    }

    void save() const {
        ofstream file(dataFile);
        if (!file) {
            cout << "Warning: could not save data to " << dataFile << ".\n";
            return;
        }
        file << "PATIENTS\n";
        for (const auto& p : patients) {
            file << p.id << '|' << p.name << '|' << p.age << '|' << p.gender << '|'
                 << p.phone << '|' << p.address << '|' << p.bloodGroup << '\n';
        }
        file << "DOCTORS\n";
        for (const auto& d : doctors) {
            file << d.id << '|' << d.name << '|' << d.specialization << '|'
                 << d.phone << '|' << d.availability << '\n';
        }
        file << "APPOINTMENTS\n";
        for (const auto& a : appointments) {
            file << a.id << '|' << a.patientId << '|' << a.doctorId << '|' << a.date
                 << '|' << a.time << '|' << a.reason << '|' << a.status << '\n';
        }
    }

    void load() {
        ifstream file(dataFile);
        if (!file) {
            seedData();
            return;
        }
        vector<Patient> loadedPatients;
        vector<Doctor> loadedDoctors;
        vector<Appointment> loadedAppointments;
        string section;
        string line;
        try {
            while (getline(file, line)) {
                if (line == "PATIENTS" || line == "DOCTORS" || line == "APPOINTMENTS") {
                    section = line;
                    continue;
                }
                if (line.empty()) continue;
                stringstream stream(line);
                vector<string> fields;
                string field;
                while (getline(stream, field, '|')) fields.push_back(field);
                if (section == "PATIENTS" && fields.size() == 7) {
                    loadedPatients.push_back({stoi(fields[0]), fields[1], stoi(fields[2]),
                                              fields[3], fields[4], fields[5], fields[6]});
                } else if (section == "DOCTORS" && fields.size() == 5) {
                    loadedDoctors.push_back({stoi(fields[0]), fields[1], fields[2],
                                             fields[3], fields[4]});
                } else if (section == "APPOINTMENTS" && fields.size() == 7) {
                    loadedAppointments.push_back({stoi(fields[0]), stoi(fields[1]),
                                                 stoi(fields[2]), fields[3], fields[4],
                                                 fields[5], fields[6]});
                }
            }
        } catch (const invalid_argument&) {
            cout << "Warning: saved data was invalid. Starting with sample data.\n";
            seedData();
            return;
        } catch (const out_of_range&) {
            cout << "Warning: saved data was invalid. Starting with sample data.\n";
            seedData();
            return;
        }
        patients = loadedPatients;
        doctors = loadedDoctors;
        appointments = loadedAppointments;
        for (const auto& p : patients) nextPatientId = max(nextPatientId, p.id + 1);
        for (const auto& d : doctors) nextDoctorId = max(nextDoctorId, d.id + 1);
        for (const auto& a : appointments) nextAppointmentId = max(nextAppointmentId, a.id + 1);
    }

public:
    void run() {
        load();
        cout << "\n";
        printLine('=');
        cout << "        PATIENT CARE MANAGER - C++ OOP MINI PROJECT\n";
        cout << "        Manage patients, doctors, and appointments\n";
        printLine('=');

        while (true) {
            cout << "\nMAIN MENU\n";
            cout << "  1. Register patient\n";
            cout << "  2. View all patients\n";
            cout << "  3. Search patients\n";
            cout << "  4. Add doctor\n";
            cout << "  5. View doctor directory\n";
            cout << "  6. Schedule appointment\n";
            cout << "  7. View appointments\n";
            cout << "  8. Cancel appointment\n";
            cout << "  0. Save and exit\n";
            int choice = inputInt("Choose an option: ", 0, 8);

            switch (choice) {
                case 1: addPatient(); break;
                case 2: listPatients(); break;
                case 3: searchPatients(); break;
                case 4: addDoctor(); break;
                case 5: listDoctors(); break;
                case 6: scheduleAppointment(); break;
                case 7: listAppointments(); break;
                case 8: cancelAppointment(); break;
                case 0:
                    save();
                    cout << "Data saved. Goodbye!\n";
                    return;
            }
        }
    }
};

int main() {
    HealthcareSystem system;
    system.run();
    return 0;
}
