#include "student.h"

/*
This source file implements the functions declared in student.h.
It contains the constructor and display() method definitions.
*/

// Constructor with initializer list
Student::Student(const std::string& c_id,
                 int iq_val,
                 double prev_res,
                 double c_gpa,
                 int a_perf,
                 bool intern_exp,
                 int e_score,
                 int com_skills,
                 int proj_comp,
                 bool place)
    : college_id(c_id),
      iq(iq_val),
      prev_sem_result(prev_res),
      cgpa(c_gpa),
      academic_performance(a_perf),
      internship_experience(intern_exp),
      extra_curricular_score(e_score),
      communication_skills(com_skills),
      projects_completed(proj_comp),
      placement(place)
{
    // Empty body since all initialization is done in the initializer list
}

// Display method
void Student::display() const {
    std::cout << "College ID: " << college_id
              << ", IQ: " << iq
              << ", Prev Semester: " << prev_sem_result
              << ", CGPA: " << cgpa
              << ", Academic Performance: " << academic_performance
              << ", Internship: " << (internship_experience ? "Yes" : "No")
              << ", Extra Curricular: " << extra_curricular_score
              << ", Communication Skills: " << communication_skills
              << ", Projects Completed: " << projects_completed
              << ", Placement: " << (placement ? "Yes" : "No")
              << std::endl;
}
