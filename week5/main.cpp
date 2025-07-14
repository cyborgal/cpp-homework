#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "student.h"

/*
This program reads student placement data from a CSV file,
creates Student objects for each row, and displays their information.
*/

int main() {
    std::ifstream file("college_student_placement_dataset.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::vector<Student> students; // Dynamic array to hold Student objects

    std::string line;
    std::getline(file, line); // Skip header row

    int count = 0;
    while (std::getline(file, line) && count < 10) {
        std::stringstream ss(line);

        // Temporary strings for each field
        std::string c_id, iq_str, prev_res_str, cgpa_str, a_perf_str,
                    intern_exp_str, e_score_str, com_skills_str,
                    proj_comp_str, placement_str;

        // Extract each comma-separated field
        std::getline(ss, c_id, ',');
        std::getline(ss, iq_str, ',');
        std::getline(ss, prev_res_str, ',');
        std::getline(ss, cgpa_str, ',');
        std::getline(ss, a_perf_str, ',');
        std::getline(ss, intern_exp_str, ',');
        std::getline(ss, e_score_str, ',');
        std::getline(ss, com_skills_str, ',');
        std::getline(ss, proj_comp_str, ',');
        std::getline(ss, placement_str, ',');

        // Convert strings to appropriate data types
        int iq = std::stoi(iq_str);
        double prev_res = std::stod(prev_res_str);
        double cgpa = std::stod(cgpa_str);
        int a_perf = std::stoi(a_perf_str);
        bool intern_exp = (intern_exp_str == "Yes");
        int e_score = std::stoi(e_score_str);
        int com_skills = std::stoi(com_skills_str);
        int proj_comp = std::stoi(proj_comp_str);
        bool place = (placement_str == "Yes");

        // Create Student object using constructor
        Student s(c_id, iq, prev_res, cgpa, a_perf,
                  intern_exp, e_score, com_skills, proj_comp, place);

        // Add Student to vector
        students.push_back(s);

        count++;
    }

    // Loop over all students and display their data
    for (const auto& s : students) {
        s.display();
    }

    return 0;
}
