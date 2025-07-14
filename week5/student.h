#pragma once

#include <iostream>
#include <string>

/*
This header file defines the Student class.
It declares member variables and function prototypes (no function bodies).
*/

class Student {
private:
    std::string college_id;
    int iq;
    double prev_sem_result;
    double cgpa;
    int academic_performance;
    bool internship_experience;
    int extra_curricular_score;
    int communication_skills;
    int projects_completed;
    bool placement;

public:
    // Constructor declaration (implemented in student.cpp)
    Student(const std::string& c_id,
            int iq_val,
            double prev_res,
            double c_gpa,
            int a_perf,
            bool intern_exp,
            int e_score,
            int com_skills,
            int proj_comp,
            bool place);

    // Display method declaration (implemented in student.cpp)
    void display() const;
};
