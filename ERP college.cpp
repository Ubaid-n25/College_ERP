#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void pressAnyKey() {
    cout << "\n\n\tPress any key to continue...";
    _getch();
}

const int COL_DEFAULT = 7;
const int COL_MENU = 11;
const int COL_INPUT = 10;
const int COL_SUCCESS = 10;
const int COL_ERROR = 12;
const int COL_TITLE = 14;

const int MAX_STUDENTS = 100;
const int MAX_FACULTY = 50;
const int MAX_COURSES = 50;
const int MAX_ENROLLMENTS = 500;

class Person {
protected:
    int id;
    string name;
    int age;
    string gender;
public:
    Person() : id(0), age(0) {}
    Person(int i, string n, int a, string g) : id(i), name(n), age(a), gender(g) {}
    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getGender() const { return gender; }
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setGender(string g) { gender = g; }
    virtual void displayInfo() const = 0;
    virtual ~Person() {}
};

class Student : public Person {
private:
    string rollNumber;
    int marks[MAX_COURSES];
    float attendance[MAX_COURSES];
    double feePaid;
    double totalFee;
    int enrolledCourseCount;
    int enrolledCourseIds[MAX_COURSES];
public:
    Student() : Person(), feePaid(0), totalFee(50000), enrolledCourseCount(0) {
        for(int i=0; i<MAX_COURSES; i++) {
            marks[i] = -1;
            attendance[i] = -1;
            enrolledCourseIds[i] = -1;
        }
    }
    Student(int id, string name, int age, string gender, string roll)
        : Person(id, name, age, gender), rollNumber(roll), feePaid(0), totalFee(50000), enrolledCourseCount(0) {
        for(int i=0; i<MAX_COURSES; i++) {
            marks[i] = -1;
            attendance[i] = -1;
            enrolledCourseIds[i] = -1;
        }
    }
    string getRollNumber() const { return rollNumber; }
    double getFeePaid() const { return feePaid; }
    double getTotalFee() const { return totalFee; }
    double getRemainingFee() const { return totalFee - feePaid; }
    
    void payFee(double amount) {
        if(amount > 0 && amount <= getRemainingFee()) {
            feePaid += amount;
            setColor(COL_SUCCESS);
            cout << "\nPayment successful! Paid: Rs. " << amount;
            cout << "\nRemaining: Rs. " << getRemainingFee();
            setColor(COL_DEFAULT);
        } else {
            setColor(COL_ERROR);
            cout << "\nInvalid amount or exceeds remaining fee!";
            setColor(COL_DEFAULT);
        }
    }
    
    void enrollInCourse(int courseId) {
        if(enrolledCourseCount < MAX_COURSES) {
            enrolledCourseIds[enrolledCourseCount++] = courseId;
        }
    }
    
    bool isEnrolledInCourse(int courseId) const {
        for(int i=0; i<enrolledCourseCount; i++)
            if(enrolledCourseIds[i] == courseId) return true;
        return false;
    }
    
    void setMarks(int courseId, int m) {
        for(int i=0; i<enrolledCourseCount; i++) {
            if(enrolledCourseIds[i] == courseId) {
                if(m>=0 && m<=100) marks[i] = m;
                else cout << "Invalid marks!\n";
                return;
            }
        }
    }
    
    int getMarksForCourse(int courseId) const {
        for(int i=0; i<enrolledCourseCount; i++)
            if(enrolledCourseIds[i] == courseId) return marks[i];
        return -1;
    }
    
    void setAttendance(int courseId, float percent) {
        for(int i=0; i<enrolledCourseCount; i++) {
            if(enrolledCourseIds[i] == courseId) {
                if(percent>=0 && percent<=100) attendance[i] = percent;
                return;
            }
        }
    }
    
    float getAttendanceForCourse(int courseId) const {
        for(int i=0; i<enrolledCourseCount; i++)
            if(enrolledCourseIds[i] == courseId) return attendance[i];
        return -1;
    }
    
    int getEnrolledCourseCount() const { return enrolledCourseCount; }
    int getEnrolledCourseId(int idx) const { return enrolledCourseIds[idx]; }
    
    void displayInfo() const override {
        setColor(COL_TITLE);
        cout << "\n--- Student Details ---\n";
        setColor(COL_DEFAULT);
        cout << "ID: " << id << "\nName: " << name << "\nAge: " << age << "\nGender: " << gender
             << "\nRoll Number: " << rollNumber << "\nTotal Fee: Rs. " << totalFee
             << "\nFee Paid: Rs. " << feePaid << "\nRemaining: Rs. " << getRemainingFee();
    }
};

class Faculty : public Person {
private:
    string department;
    double salary;
public:
    Faculty() : Person(), salary(0) {}
    Faculty(int id, string name, int age, string gender, string dept, double sal)
        : Person(id, name, age, gender), department(dept), salary(sal) {}
    string getDepartment() const { return department; }
    double getSalary() const { return salary; }
    void setSalary(double s) { salary = s; }
    
    void displayInfo() const override {
        setColor(COL_TITLE);
        cout << "\n--- Faculty Details ---\n";
        setColor(COL_DEFAULT);
        cout << "ID: " << id << "\nName: " << name << "\nAge: " << age << "\nGender: " << gender
             << "\nDepartment: " << department << "\nSalary: Rs. " << salary;
    }
};

class Course {
private:
    int courseId;
    string courseName;
    int credits;
    int facultyId;
public:
    Course() : courseId(0), credits(0), facultyId(-1) {}
    Course(int id, string name, int cr) : courseId(id), courseName(name), credits(cr), facultyId(-1) {}
    int getId() const { return courseId; }
    string getName() const { return courseName; }
    int getCredits() const { return credits; }
    int getFacultyId() const { return facultyId; }
    void assignFaculty(int fId) { facultyId = fId; }
    void display() const {
        cout << "Course ID: " << courseId << " | Name: " << courseName << " | Credits: " << credits;
        if(facultyId != -1) cout << " | Faculty ID: " << facultyId;
        else cout << " | Faculty: Not Assigned";
    }
};

Student students[MAX_STUDENTS];
Faculty faculties[MAX_FACULTY];
Course courses[MAX_COURSES];
int studentCount = 0;
int facultyCount = 0;
int courseCount = 0;

int enrollmentCourseId[MAX_ENROLLMENTS];
int enrollmentStudentId[MAX_ENROLLMENTS];
int enrollmentCount = 0;

bool studentExists(int id) {
    for(int i=0; i<studentCount; i++)
        if(students[i].getId() == id) return true;
    return false;
}

bool facultyExists(int id) {
    for(int i=0; i<facultyCount; i++)
        if(faculties[i].getId() == id) return true;
    return false;
}

bool courseExists(int id) {
    for(int i=0; i<courseCount; i++)
        if(courses[i].getId() == id) return true;
    return false;
}

int findStudentIndex(int id) {
    for(int i=0; i<studentCount; i++)
        if(students[i].getId() == id) return i;
    return -1;
}

int findCourseIndex(int id) {
    for(int i=0; i<courseCount; i++)
        if(courses[i].getId() == id) return i;
    return -1;
}

void addStudent() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ADD NEW STUDENT ==========\n";
    setColor(COL_DEFAULT);
    if(studentCount >= MAX_STUDENTS) {
        setColor(COL_ERROR); cout << "Student limit reached!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    int id, age;
    string name, gender, roll;
    cout << "Enter Student ID: "; cin >> id;
    if(studentExists(id)) {
        setColor(COL_ERROR); cout << "Student ID already exists!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter Age: "; cin >> age;
    cout << "Enter Gender (M/F): "; cin >> gender;
    cout << "Enter Roll Number: "; cin >> roll;
    students[studentCount] = Student(id, name, age, gender, roll);
    studentCount++;
    setColor(COL_SUCCESS);
    cout << "\nStudent added successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void viewAllStudents() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ALL STUDENTS ==========\n";
    setColor(COL_DEFAULT);
    if(studentCount == 0) cout << "No students found.\n";
    else {
        for(int i=0; i<studentCount; i++) {
            students[i].displayInfo();
            cout << "\n------------------------\n";
        }
    }
    pressAnyKey();
}

void addFaculty() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ADD NEW FACULTY ==========\n";
    setColor(COL_DEFAULT);
    if(facultyCount >= MAX_FACULTY) {
        setColor(COL_ERROR); cout << "Faculty limit reached!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    int id, age;
    string name, gender, dept;
    double salary;
    cout << "Enter Faculty ID: "; cin >> id;
    if(facultyExists(id)) {
        setColor(COL_ERROR); cout << "Faculty ID already exists!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter Age: "; cin >> age;
    cout << "Enter Gender: "; cin >> gender;
    cout << "Enter Department: "; cin.ignore(); getline(cin, dept);
    cout << "Enter Salary: "; cin >> salary;
    faculties[facultyCount] = Faculty(id, name, age, gender, dept, salary);
    facultyCount++;
    setColor(COL_SUCCESS);
    cout << "\nFaculty added successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void viewAllFaculty() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ALL FACULTY ==========\n";
    setColor(COL_DEFAULT);
    if(facultyCount == 0) cout << "No faculty found.\n";
    else {
        for(int i=0; i<facultyCount; i++) {
            faculties[i].displayInfo();
            cout << "\n------------------------\n";
        }
    }
    pressAnyKey();
}

void addCourse() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ADD NEW COURSE ==========\n";
    setColor(COL_DEFAULT);
    if(courseCount >= MAX_COURSES) {
        setColor(COL_ERROR); cout << "Course limit reached!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    int id, credits;
    string name;
    cout << "Enter Course ID: "; cin >> id;
    if(courseExists(id)) {
        setColor(COL_ERROR); cout << "Course ID already exists!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Course Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter Credits: "; cin >> credits;
    courses[courseCount] = Course(id, name, credits);
    courseCount++;
    setColor(COL_SUCCESS);
    cout << "\nCourse added successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void viewAllCourses() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ALL COURSES ==========\n";
    setColor(COL_DEFAULT);
    if(courseCount == 0) cout << "No courses found.\n";
    else {
        for(int i=0; i<courseCount; i++) {
            courses[i].display();
            cout << endl;
        }
    }
    pressAnyKey();
}

void assignFacultyToCourse() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ASSIGN FACULTY TO COURSE ==========\n";
    setColor(COL_DEFAULT);
    int cid, fid;
    cout << "Enter Course ID: "; cin >> cid;
    if(!courseExists(cid)) {
        setColor(COL_ERROR); cout << "Course not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Faculty ID: "; cin >> fid;
    if(!facultyExists(fid)) {
        setColor(COL_ERROR); cout << "Faculty not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    int idx = findCourseIndex(cid);
    courses[idx].assignFaculty(fid);
    setColor(COL_SUCCESS);
    cout << "Faculty assigned to course successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void enrollStudentInCourse() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== ENROLL STUDENT IN COURSE ==========\n";
    setColor(COL_DEFAULT);
    if(enrollmentCount >= MAX_ENROLLMENTS) {
        setColor(COL_ERROR); cout << "Enrollment limit reached!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    int sid, cid;
    cout << "Enter Student ID: "; cin >> sid;
    int sIdx = findStudentIndex(sid);
    if(sIdx == -1) {
        setColor(COL_ERROR); cout << "Student not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Course ID: "; cin >> cid;
    if(!courseExists(cid)) {
        setColor(COL_ERROR); cout << "Course not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    for(int i=0; i<enrollmentCount; i++) {
        if(enrollmentStudentId[i] == sid && enrollmentCourseId[i] == cid) {
            setColor(COL_ERROR); cout << "Student already enrolled in this course!\n"; setColor(COL_DEFAULT);
            pressAnyKey(); return;
        }
    }
    enrollmentCourseId[enrollmentCount] = cid;
    enrollmentStudentId[enrollmentCount] = sid;
    enrollmentCount++;
    students[sIdx].enrollInCourse(cid);
    setColor(COL_SUCCESS);
    cout << "Student enrolled successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void recordMarks() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== RECORD MARKS ==========\n";
    setColor(COL_DEFAULT);
    int sid, cid, marks;
    cout << "Enter Student ID: "; cin >> sid;
    int sIdx = findStudentIndex(sid);
    if(sIdx == -1) {
        setColor(COL_ERROR); cout << "Student not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Course ID: "; cin >> cid;
    if(!courseExists(cid)) {
        setColor(COL_ERROR); cout << "Course not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    bool enrolled = false;
    for(int i=0; i<enrollmentCount; i++) {
        if(enrollmentStudentId[i] == sid && enrollmentCourseId[i] == cid) {
            enrolled = true; break;
        }
    }
    if(!enrolled) {
        setColor(COL_ERROR); cout << "Student not enrolled in this course!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Marks (0-100): "; cin >> marks;
    students[sIdx].setMarks(cid, marks);
    setColor(COL_SUCCESS);
    cout << "Marks recorded successfully!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void recordAttendance() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== RECORD ATTENDANCE ==========\n";
    setColor(COL_DEFAULT);
    int sid, cid;
    float percent;
    cout << "Enter Student ID: "; cin >> sid;
    int sIdx = findStudentIndex(sid);
    if(sIdx == -1) {
        setColor(COL_ERROR); cout << "Student not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Course ID: "; cin >> cid;
    if(!courseExists(cid)) {
        setColor(COL_ERROR); cout << "Course not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    cout << "Enter Attendance Percentage (0-100): "; cin >> percent;
    students[sIdx].setAttendance(cid, percent);
    setColor(COL_SUCCESS);
    cout << "Attendance recorded!\n";
    setColor(COL_DEFAULT);
    pressAnyKey();
}

void viewTranscript() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== STUDENT TRANSCRIPT ==========\n";
    setColor(COL_DEFAULT);
    int sid;
    cout << "Enter Student ID: "; cin >> sid;
    int sIdx = findStudentIndex(sid);
    if(sIdx == -1) {
        setColor(COL_ERROR); cout << "Student not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    Student &s = students[sIdx];
    s.displayInfo();
    cout << "\n\n--- Enrolled Courses & Performance ---\n";
    for(int i=0; i<courseCount; i++) {
        int cid = courses[i].getId();
        if(s.isEnrolledInCourse(cid)) {
            cout << "Course: " << courses[i].getName() << " (ID:" << cid << ") | ";
            int marks = s.getMarksForCourse(cid);
            float att = s.getAttendanceForCourse(cid);
            if(marks != -1) cout << "Marks: " << marks << " | ";
            else cout << "Marks: Not recorded | ";
            if(att != -1) cout << "Attendance: " << att << "%";
            else cout << "Attendance: Not recorded";
            cout << endl;
        }
    }
    pressAnyKey();
}

void payStudentFee() {
    clearScreen();
    setColor(COL_MENU);
    cout << "\n========== PAY FEE ==========\n";
    setColor(COL_DEFAULT);
    int sid;
    double amount;
    cout << "Enter Student ID: "; cin >> sid;
    int sIdx = findStudentIndex(sid);
    if(sIdx == -1) {
        setColor(COL_ERROR); cout << "Student not found!\n"; setColor(COL_DEFAULT);
        pressAnyKey(); return;
    }
    students[sIdx].displayInfo();
    cout << "\n\nEnter amount to pay: Rs. "; cin >> amount;
    students[sIdx].payFee(amount);
    pressAnyKey();
}

void demoPolymorphism() {
    clearScreen();
    setColor(COL_TITLE);
    cout << "\n========== POLYMORPHISM DEMO ==========\n";
    setColor(COL_DEFAULT);
    cout << "Creating base class pointer to Student and Faculty objects...\n\n";
    Person* p1 = nullptr;
    Person* p2 = nullptr;
    if(studentCount > 0) p1 = &students[0];
    if(facultyCount > 0) p2 = &faculties[0];
    if(p1) {
        cout << "Calling displayInfo() via Person* (Student):\n";
        p1->displayInfo();
    } else {
        cout << "No student available for demo. Add a student first.\n";
    }
    cout << "\n\n";
    if(p2) {
        cout << "Calling displayInfo() via Person* (Faculty):\n";
        p2->displayInfo();
    } else {
        cout << "No faculty available for demo. Add a faculty first.\n";
    }
    pressAnyKey();
}

int main() {
    int choice;
    do {
        clearScreen();
        setColor(COL_MENU);
        cout << "\n\t===========================================\n";
        cout << "\t     COLLEGE ERP SYSTEM - OOP DEMO         \n";
        cout << "\t===========================================\n";
        setColor(COL_DEFAULT);
        cout << "\n\t1. Add Student\n";
        cout << "\t2. View All Students\n";
        cout << "\t3. Add Faculty\n";
        cout << "\t4. View All Faculty\n";
        cout << "\t5. Add Course\n";
        cout << "\t6. View All Courses\n";
        cout << "\t7. Assign Faculty to Course\n";
        cout << "\t8. Enroll Student in Course\n";
        cout << "\t9. Record Marks\n";
        cout << "\t10. Record Attendance\n";
        cout << "\t11. View Student Transcript\n";
        cout << "\t12. Pay Student Fee\n";
        cout << "\t13. DEMO: Polymorphism (Abstract Class)\n";
        cout << "\t0. Exit\n";
        cout << "\n\tEnter your choice: ";
        cin >> choice;
        switch(choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: addFaculty(); break;
            case 4: viewAllFaculty(); break;
            case 5: addCourse(); break;
            case 6: viewAllCourses(); break;
            case 7: assignFacultyToCourse(); break;
            case 8: enrollStudentInCourse(); break;
            case 9: recordMarks(); break;
            case 10: recordAttendance(); break;
            case 11: viewTranscript(); break;
            case 12: payStudentFee(); break;
            case 13: demoPolymorphism(); break;
            case 0:
                setColor(COL_SUCCESS);
                cout << "\nExiting ERP System. Goodbye!\n";
                setColor(COL_DEFAULT);
                break;
            default:
                setColor(COL_ERROR);
                cout << "\nInvalid choice! Try again.\n";
                setColor(COL_DEFAULT);
                pressAnyKey();
        }
    } while(choice != 0);
    return 0;
}