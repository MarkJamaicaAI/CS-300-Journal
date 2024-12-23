#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <sstream>

using namespace std;
struct Course {
    string courseName;
    string courseNumber;
    vector <string> prereq;

    Course() : courseName(""), courseNumber(""), prereq({}) {}

    Course(string courseNumber, string courseName, vector<string> prereq) {
        this->courseName = courseName;
        this->courseNumber = courseNumber;
        this->prereq =  prereq;
    }
};
struct Node {
    Course course;
   
    Node* left;
    Node* right;
    Node(Course course) : course(course), left(nullptr), right(nullptr) {}

};
void displayCourse(Course& course);
void displayMenu();

class BinarySearchTree {
private:

    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:

    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);

    void DestroyBST(Node* node);
    int size;


};


BinarySearchTree::BinarySearchTree() {

    //root is equal to nullptr
    root = nullptr;
    size = 0;
}

void BinarySearchTree::DestroyBST(Node* node)
{
    if (node != nullptr) {
        DestroyBST(node->right);
        DestroyBST(node->left);
        delete node;
    }
}
BinarySearchTree::~BinarySearchTree() {
    // recursively destory all nodes
    DestroyBST(root);

}
void BinarySearchTree::Insert(Course course) {

    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);


        // increment size
        size++;
    }

    // else
    else {
        // add Node root and bid
        this->addNode(root, course);

        // increment size
        size++;
    }

}

void BinarySearchTree::InOrder() {
    inOrder(root);
}
void BinarySearchTree::addNode(Node* node, Course course) {
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

void parseFile(string fileName, BinarySearchTree* bst)
{

    // loading course into bst
    ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        cout << "ERROR: File could not be opened. Try adding to CWD." << endl;

    }
    else {
        string line = "";
        while (getline(inputFile, line)) {
            stringstream inputString(line);

            string courseNumber, courseName, prereq;
            string temp;
            vector<string> prerequisites;

            getline(inputString, courseNumber, ',');
            getline(inputString, courseName, ',');

            if (courseName.compare("") == 0 || courseNumber.compare("") == 0) {
                cout << endl << line << " imporper format less than two values. Not added to data structure." << endl << endl;
                continue;
            }
            // Get rest of the line to the EOL
            while (getline(inputString, temp, '\n')) {
                // Make the stream only concern the prerequisites 
                stringstream ss(temp);
                // Seperate temp by the ',' delimiter
                while (getline(ss, temp, ',')) {
                    // add it to prereq vector
                    prerequisites.push_back(temp);
                }

            }
            Course course(courseNumber, courseName, prerequisites);


            // Add the course to bst
            bst->Insert(course);
        }
    }


    inputFile.close();
}




void BinarySearchTree::inOrder(Node* node) {

    if (node != nullptr) {
        //InOrder left
        inOrder(node->left);
        //output courseId, title, and prerequisites 
        cout << node->course.courseNumber << " " << node->course.courseName;
        if (node->course.prereq.empty()) {
            cout << "No prerequisites" << endl;
        }
        else {
            for (const auto& prereq : node->course.prereq) {
                cout << prereq << " ";
            }
            cout << endl;
        }
        //InOder right
        inOrder(node->right);
    }
}


void displayCourse(Course& course) {
    cout << course.courseNumber;
    cout << course.courseName;

    if (course.prereq.empty()) {
        cout << "Prerequisites:None" << endl;
    }
    else {
        for (const string& prereq : course.prereq) {
            cout << prereq << " ";
        }
        cout << endl;
    }

}



Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* curr = root;
    // keep looping downwards until bottom reached or matching bidId found
    while (curr != nullptr) {
        // if match found, return current bid
        if (curr->course.courseNumber.compare(courseId )== 0) {
            Course course;
            course.courseNumber = curr->course.courseNumber;
            course.courseName = curr->course.courseName;
            course.prereq = curr->course.prereq;


            return course;
        }

        // if bid is smaller than current node then traverse left
        if (courseId.compare(curr->course.courseNumber) < 0) {
            curr = curr->left;
        }
        else {
            // else larger so traverse right
            curr = curr->right;
        }

    }
    Course course;
    return course;
}


void displayMenu() {
    cout << "Menu:" << endl;
    cout << "  1. Load Courses" << endl;
    cout << "  2. Print Schedule" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
}

int main(int argc, char** argv)
{
    Course course;

    BinarySearchTree* bst = new BinarySearchTree();

    // Set exception mask for cin stream
    cin.exceptions(ios::failbit);
    clock_t ticks;
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "C:\\Users\\missk\\Documents\\CS 300 ABCU_Advising_Program_Input.csv";
    }


    cout << "Welcome to Course Planner." << endl;
    int choice = 0;
    while (choice != 9) {
        displayMenu();
        cout << "Enter selection: ";

        // Verifies if input is a char or string - which is not valid 
        try {
            cin >> choice;
            // checking if input is valid
            if ((choice < 0 || choice > 3) && choice != 9) {
                cout << choice << " is not a valid menu option try again." << endl;
            }

            switch (choice) {
            case 1:

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the courses
                parseFile(csvPath, bst);

                cout << bst->size << " courses read" << endl;

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 2:
                cout << "Here is a sample schedule: " << endl << endl;
                bst->InOrder();
                break;

            case 3:
                string input;
                ticks = clock();
                cout << "What course do you want to know about?";
                cin >> input;

                courseId = input;
                // converting lowercase values to uppercase 
                for (int i = 0; i < courseId.length(); i++) {
                    courseId[i] = toupper(courseId[i]);
                }
                course = bst->Search(courseId);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course Id " << courseId << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            }
        }
        catch (ios_base::failure& ) {
            cout << "Not a valid input. Try a number 1-3 or 9" << endl;
            cin.clear();
            cin.ignore(1234, '\n');
        }
    }
    cout << endl << "Thank you for using the course planner." << endl;
}