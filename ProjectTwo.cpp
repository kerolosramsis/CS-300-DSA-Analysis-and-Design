#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Stores the information for one course.
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Removes extra spaces and carriage returns from a string.
string trim(string text) {
    while (!text.empty() && isspace(static_cast<unsigned char>(text.front()))) {
        text.erase(text.begin());
    }

    while (!text.empty() && isspace(static_cast<unsigned char>(text.back()))) {
        text.pop_back();
    }

    return text;
}

// Converts a course number to uppercase so searches are not case-sensitive.
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });

    return text;
}

// Binary search tree used to store the courses.
class BinarySearchTree {
private:

    // Each node stores one Course and pointers to its children.
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    // Recursively inserts a course in the correct position.
    void addNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {

            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {

            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    // In-order traversal prints courses in alphanumeric order.
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);

            cout << node->course.courseNumber << ", "
                << node->course.courseName << endl;

            inOrder(node->right);
        }
    }

    // Searches the tree for a course number.
    Node* searchNode(Node* node, string courseNumber) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseNumber == courseNumber) {
            return node;
        }

        if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        }

        return searchNode(node->right, courseNumber);
    }

    // Deletes all nodes when the tree is destroyed.
    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:

    // Starts with an empty tree.
    BinarySearchTree() {
        root = nullptr;
    }

    // Frees the memory used by the tree.
    ~BinarySearchTree() {
        deleteTree(root);
        root = nullptr;
    }

    // Inserts a course into the binary search tree.
    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    // Prints all courses in sorted order.
    void PrintCourseList() {
        if (root == nullptr) {
            cout << "No course data loaded." << endl;
            return;
        }

        inOrder(root);
    }

    // Searches for a course and returns it if found.
    Course* Search(string courseNumber) {
        Node* result = searchNode(root, courseNumber);

        if (result == nullptr) {
            return nullptr;
        }

        return &(result->course);
    }
};

// Splits one comma-separated line into separate fields.
vector<string> splitLine(string line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Checks whether a course number exists in the course-number list.
bool courseNumberExists(
    const vector<string>& courseNumbers,
    string courseNumber) {

    for (const string& number : courseNumbers) {
        if (number == courseNumber) {
            return true;
        }
    }

    return false;
}

// Reads the file, validates the data, creates Course objects,
// and stores them in the binary search tree.
bool loadCourses(string fileName, BinarySearchTree& courses) {

    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return false;
    }

    vector<string> fileLines;
    vector<string> allCourseNumbers;
    string line;

    // First pass: read each line and collect all course numbers.
    while (getline(inputFile, line)) {

        line = trim(line);

        if (line.empty()) {
            continue;
        }

        vector<string> tokens = splitLine(line);

        // Every line must have at least a course number and course name.
        if (tokens.size() < 2) {
            cout << "Error: Invalid course data format." << endl;
            inputFile.close();
            return false;
        }

        string courseNumber = toUpperCase(tokens[0]);
        string courseName = tokens[1];

        if (courseNumber.empty() || courseName.empty()) {
            cout << "Error: Course number and course name are required."
                << endl;
            inputFile.close();
            return false;
        }

        fileLines.push_back(line);
        allCourseNumbers.push_back(courseNumber);
    }

    inputFile.close();

    // Second pass: create each Course and validate prerequisites.
    for (const string& savedLine : fileLines) {

        vector<string> tokens = splitLine(savedLine);

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseName = tokens[1];

        // Any field after the course name is a prerequisite.
        for (size_t i = 2; i < tokens.size(); ++i) {

            string prerequisiteNumber = toUpperCase(trim(tokens[i]));

            // Ignore empty fields caused by trailing commas in the CSV file.
            if (prerequisiteNumber.empty()) {
                continue;
            }

            if (!courseNumberExists(
                allCourseNumbers,
                prerequisiteNumber)) {

                cout << "Error: Prerequisite "
                    << prerequisiteNumber
                    << " does not exist as a course in the file."
                    << endl;

                return false;
            }

            course.prerequisites.push_back(prerequisiteNumber);
        }

        courses.Insert(course);
    }

    return true;
}

// Prints information for one course and its prerequisites.
void printCourseInformation(
    BinarySearchTree& courses,
    string courseNumber) {

    // Allow the user to enter csci400, CSCI400, etc.
    courseNumber = toUpperCase(trim(courseNumber));

    Course* course = courses.Search(courseNumber);

    if (course == nullptr) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course->courseNumber << ", "
        << course->courseName << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites:" << endl;

    // Print each prerequisite course number and title.
    for (const string& prerequisiteNumber : course->prerequisites) {

        Course* prerequisite =
            courses.Search(prerequisiteNumber);

        if (prerequisite != nullptr) {
            cout << prerequisite->courseNumber << ", "
                << prerequisite->courseName << endl;
        }
    }
}

// Displays the required menu.
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
}

int main() {

    BinarySearchTree courses;

    bool dataLoaded = false;
    string input;

    cout << "Welcome to the course planner." << endl;

    while (true) {

        displayMenu();

        cout << "What would you like to do? ";
        getline(cin, input);

        int choice;

        // Check that the menu entry is actually a number.
        try {
            size_t position;
            choice = stoi(input, &position);

            if (position != input.length()) {
                throw invalid_argument("Invalid input");
            }
        }
        catch (...) {
            cout << input << " is not a valid option." << endl;
            continue;
        }

        // Option 1: Load the course file.
        if (choice == 1) {

            if (dataLoaded) {
                cout << "Course data has already been loaded." << endl;
                continue;
            }

            string fileName;

            cout << "Enter the course data file name: ";
            getline(cin, fileName);

            if (loadCourses(fileName, courses)) {
                dataLoaded = true;
                cout << "Course data loaded successfully." << endl;
            }
            else {
                cout << "Course data could not be loaded." << endl;
            }
        }

        // Option 2: Print all courses in alphanumeric order.
        else if (choice == 2) {

            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                cout << endl;

                courses.PrintCourseList();
            }
        }

        // Option 3: Search for one course.
        else if (choice == 3) {

            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                string courseNumber;

                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                printCourseInformation(courses, courseNumber);
            }
        }

        // Option 9: Exit.
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        // Handle any other numeric menu option.
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}