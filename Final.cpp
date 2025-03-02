#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
    // course structure
struct Course
{
    string courseNumber; //course id
    string courseTitle; //course name
    vector<string> prerequisites; //prereqs for course ID's
};

// Stucture for Binary Search Tree Nodes
struct Node
{
    Course course; //course data
    Node *left; //left child pointer
    Node *right; // right child pointer

    //constructor to initialize nodes with the course data
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// bst class creation
class BST
{
private:
    Node *root;

    void inOrderTraversal(Node *node) //using inorder traversal we display courses in a sorted order
    {
        if (node)
        {
            inOrderTraversal(node->left);
            cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
            inOrderTraversal(node->right);
        }
    }

    Node *insert(Node *node, Course course) //recursivly insert courses into bst
    {
        if (!node)
            return new Node(course); //create new node if tree empty
        if (course.courseNumber < node->course.courseNumber)
            node->left = insert(node->left, course); //insert to the left if course # smaller, otherwise insert right
        else
            node->right = insert(node->right, course);
        return node;
    }

    Node *search(Node *node, string courseNumber) //recursivly search for a course #
    {
        if (!node || node->course.courseNumber == courseNumber)
            return node; //return node if found or null if not
        if (courseNumber < node->course.courseNumber) //search left with smaller num, otherwise right
            return search(node->left, courseNumber);
        return search(node->right, courseNumber);
    }

public:
    BST() : root(nullptr) {} //initialize an empty bst

    void insert(Course course) //method to insert a course into bst
    {
        root = insert(root, course);
    }

    void displayCourses() //display all courses in sorted-order
    {
        inOrderTraversal(root);
    }

    void displayCourseInfo(string courseNumber) //dispaly course info and corresponding prereqs
    {
        Node *result = search(root, courseNumber);
        if (result)
        {
            cout << "Course: " << result->course.courseNumber << " - " << result->course.courseTitle << endl;
            cout << "Prerequisites: ";
            if (result->course.prerequisites.empty())
                cout << "none";
            else
                for (const string &prereq : result->course.prerequisites)
                    cout << prereq << " ";
            cout << endl;
        }
        else
        {
            cout << "Course not found." << endl;
        }
    }
};

// load courses from file
void loadCoursesFromFile(BST &bst, string filename)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file." << endl;
        return; //accidently had this infinetly display this error before so had to fix
        //changed approach to initial idea of file reading
    }

    cout << "Loading courses from file: " << filename << endl;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line);
        string courseNumber, courseTitle, prereq;
        
        if (!getline(ss, courseNumber, ',') || !getline(ss, courseTitle, ',')) {
            cerr << "Error: Invalid line format -> " << line << endl;
            continue;
        }

        Course course = {courseNumber, courseTitle, {}};
        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }

        bst.insert(course);
    }

    file.close();
    cout << "Courses loaded successfully!" << endl;
}

// menu display
void displayMenu()
{
    cout << "\nMenu:" << endl;
    cout << "1) Load course data from file" << endl;
    cout << "2) Print all courses in alphanumeric order" << endl;
    cout << "3) Print course information" << endl;
    cout << "9) Exit" << endl;
    cout << "Enter choice: ";
}

int main() //main function to handle user input
{
    BST bst;
    int choice;
    string filename, courseNumber;

    while (true)
    {
        displayMenu();
        cin >> choice;
        switch (choice)
        {
        case 1:
            filename = "/Users/georgelimajr/Desktop/CS 300/CS 300 ABCU_Advising_Program_Input.csv"; //this is the file path for my machine it would just need to changed per user, or embedded in a second file like previous, for this assignment i believe I was meant to keep things to one file
            cout << "Loading file: " << filename << endl;

            loadCoursesFromFile(bst, filename);
            break;

        case 2:
            bst.displayCourses(); //print sorted courses
            break;

        case 3:
            cout << "enter course number "; //display course info
            cin >> courseNumber;
            bst.displayCourseInfo(courseNumber);
            break;

        case 9:
            cout << "Bye bye" << endl;
            return 0; //exit

        default:
            cout << "Invalid, try again" << endl;
        }
    }
}