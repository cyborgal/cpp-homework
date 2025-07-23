// ────────────────────────────────────────────────────────────────
// main.cpp ― FocusTime overlay GUI using Dear ImGui + GLFW + OpenGL
// Implements a transparent overlay window for task tracking, featuring a main
// interface to manage tasks and a summary window for analytics. Uses GLFW for
// window management, OpenGL for rendering, and ImGui for GUI elements.
// ────────────────────────────────────────────────────────────────

/* 1. Standard & system headers */
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>

/* Windows-specific for region & layered window */
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

/* 2. OpenGL / GLFW / ImGui headers */
#include <glad/glad.h>       // Must come before glfw3 for OpenGL function loading
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

/* 3. Project headers */
#include "taskmanager.h"

/* 4. GLFW error callback */
static void glfw_error_callback(int error, const char* desc) {
    std::cerr << "GLFW Error " << error << ": " << desc << "\n";  // Log GLFW errors to console
}

/* 5. Helper function to format seconds into HH:MM:SS */
static std::string formatDuration(int64_t seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);  // Format time as HH:MM:SS
    return std::string(buf);
}

/* 6. Helper function to check if a session is from a specific date */
static bool isSessionOnDate(const Task::Session& session, const struct tm& target) {
    struct tm session_date = *localtime(&session.startTime);  // Convert session start time to tm struct
    return session_date.tm_year == target.tm_year &&         // Compare year
           session_date.tm_mon == target.tm_mon &&           // Compare month
           session_date.tm_mday == target.tm_mday;           // Compare day
}

/* 7. Helper function to format tm to YYYY-MM-DD */
static std::string formatDate(const struct tm& date) {
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &date);  // Format date as YYYY-MM-DD
    return std::string(buf);
}

int main() {
    // 5.1 Init GLFW
    glfwSetErrorCallback(glfw_error_callback);  // Set up error callback
    if (!glfwInit()) return -1;  // Exit if GLFW initialization fails

    // 5.2 Window hints for a transparent, borderless, always-on-top window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);  // Enable transparent framebuffer
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);                 // Keep window on top
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);               // Remove window decorations
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);                // Allow resizing
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);                // Start maximized

    // 5.3 Create full-screen GLFW window
    GLFWmonitor* mon = glfwGetPrimaryMonitor();  // Get primary monitor
    const GLFWvidmode* vm = glfwGetVideoMode(mon);
    GLFWwindow* window = glfwCreateWindow(vm->width, vm->height, "FocusTime Overlay", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }  // Exit if window creation fails
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // 5.4 Layered window so transparency works
    {
        HWND hwnd = glfwGetWin32Window(window);  // Get native Windows handle
        LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);  // Enable layered window
        SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);      // Set alpha for transparency
    }

    // 5.5 Load GL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";  // Log GLAD initialization failure
        return -1;
    }

    // 5.6 ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard navigation in ImGui
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 5.7 Load tasks and sessions
    TaskManager manager;
    manager.loadFromFile("tasks.csv");  // Load existing tasks
    manager.loadSessionsFromFile("sessions.csv");  // Load existing sessions

    // State for summary window
    bool show_summary = false;
    time_t now = time(nullptr);  // Get current time
    struct tm selected_date = *localtime(&now); // Default to today
    int selected_date_index = 0; // For combo box
    std::vector<struct tm> recent_dates(7); // Last 7 days
    std::vector<std::string> date_strings(7); // Persistent string storage
    std::vector<const char*> date_cstrings(7); // Persistent C-string pointers
    for (int i = 0; i < 7; ++i) {
        recent_dates[i] = *localtime(&now);  // Set initial date
        recent_dates[i].tm_mday -= i;        // Adjust for each of the last 7 days
        mktime(&recent_dates[i]);            // Normalize the date
        date_strings[i] = formatDate(recent_dates[i]);  // Format as string
        date_cstrings[i] = date_strings[i].c_str();     // Get C-string pointer
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // 6.1 Poll events
        glfwPollEvents();
        // 6.2 New frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();

        // Store regions for window region update
        std::vector<ImVec4> regions; // (x, y, x+width, y+height) for click-through regions

        // 6.3 Build main GUI
        ImGui::SetNextWindowPos(ImVec2(100,100), ImGuiCond_FirstUseEver);  // Initial position
        ImGui::SetNextWindowSize(ImVec2(400,300), ImGuiCond_FirstUseEver); // Initial size
        ImGui::Begin("FocusTime",
                     nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_AlwaysAutoResize);

        // Store main window rect
        ImVec2 main_pos = ImGui::GetWindowPos();
        ImVec2 main_size = ImGui::GetWindowSize();
        regions.push_back(ImVec4(main_pos.x, main_pos.y, main_pos.x + main_size.x, main_pos.y + main_size.y));

        // UI: add task
        static char newTask[64] = "";
        ImGui::InputText("##newtask", newTask, IM_ARRAYSIZE(newTask));  // Input field for new task name
        ImGui::SameLine();
        if (ImGui::Button("Add") && newTask[0]) {
            manager.addTask(newTask);  // Add new task
            manager.saveToFile("tasks.csv");  // Save to file
            newTask[0] = '\0';  // Clear input
        }
        ImGui::Separator();

        // UI: list tasks
        ImGui::Text("Tasks"); ImGui::Separator();
        for (int i = 0; i < manager.getCount(); ++i) {
            Task* t = manager.getTaskAt(i);
            if (!t) continue;
            ImGui::PushID(i);

            // Rename/Delete menu
            static int renaming = -1;
            static char buf[64] = "";
            if (renaming == i) {
                ImGui::InputText("##rename", buf, IM_ARRAYSIZE(buf));  // Input for renaming
                ImGui::SameLine();
                if (ImGui::Button("OK")) { manager.renameTask(i, buf); renaming = -1; }  // Confirm rename
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) renaming = -1;  // Cancel rename
            } else {
                ImGui::Text("%s", t->getName().c_str());  // Display task name
                ImGui::SameLine(200);
                ImGui::Text("%s", formatDuration(t->getTotalDuration()).c_str());  // Display total duration
                ImGui::SameLine(300);
                ImVec2 button_pos = ImGui::GetCursorScreenPos();
                if (ImGui::Button("⋮")) {
                    ImGui::OpenPopup("TaskMenu");  // Open context menu
                    float popup_width = 100.0f;
                    ImGui::SetNextWindowPos(ImVec2(button_pos.x - popup_width - 5, button_pos.y));  // Position menu
                }
                bool deleted = false;
                if (ImGui::BeginPopup("TaskMenu")) {
                    if (ImGui::MenuItem("Rename")) {
                        strcpy(buf, t->getName().c_str());  // Set current name for editing
                        renaming = i;
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Delete")) {
                        manager.deleteTask(i);  // Delete task
                        ImGui::CloseCurrentPopup();
                        deleted = true;
                    }
                    ImGui::EndPopup();
                }
                if (deleted) { ImGui::PopID(); continue; }
            }

            // Controls: Start/Stop, Pause, Reset
            if (ImGui::Button(t->isRunning() ? "Stop" : "Start")) {
                if (t->isRunning()) { t->stop(); manager.saveToFile("tasks.csv"); manager.saveSessionsToFile("sessions.csv"); }  // Stop and save
                else t->start();  // Start timer
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause")) { t->pause(); manager.saveToFile("tasks.csv"); manager.saveSessionsToFile("sessions.csv"); }  // Pause and save
            ImGui::SameLine();
            if (ImGui::Button("Reset")) { t->reset(); manager.saveToFile("tasks.csv"); manager.saveSessionsToFile("sessions.csv"); }  // Reset and save
            ImGui::Separator();
            ImGui::PopID();
        }

        // Summary button
        if (ImGui::Button("Show Summary")) {
            show_summary = true;  // Open summary window
            selected_date_index = 0; // Default to today
            selected_date = recent_dates[0];
        }

        ImGui::End(); // End main window

        // Summary window
        if (show_summary) {
            ImGui::SetNextWindowPos(ImVec2(600, 100), ImGuiCond_FirstUseEver);  // Initial position
            ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver); // Initial size
            ImGui::Begin("Task Summary", &show_summary, ImGuiWindowFlags_AlwaysAutoResize);

            // Store summary window rect
            ImVec2 summary_pos = ImGui::GetWindowPos();
            ImVec2 summary_size = ImGui::GetWindowSize();
            regions.push_back(ImVec4(summary_pos.x, summary_pos.y, summary_pos.x + summary_size.x, summary_pos.y + summary_size.y));

            // Date selection
            ImGui::Text("Select Date:");
            if (ImGui::Combo("##date", &selected_date_index, date_cstrings.data(), date_cstrings.size())) {  // Select date from dropdown
                selected_date = recent_dates[selected_date_index];
            }
            ImGui::SameLine();
            if (ImGui::Button("Today")) {
                selected_date_index = 0;  // Reset to first index
                selected_date = *localtime(&now);  // Set to current date
                recent_dates[0] = selected_date;
                date_strings[0] = formatDate(selected_date);  // Update date string
                date_cstrings[0] = date_strings[0].c_str();   // Update C-string pointer
            }

            // Calculate daily and cumulative totals
            int64_t daily_total = 0;
            std::vector<std::tuple<std::string, int64_t, int64_t>> task_data; // name, daily, cumulative
            for (int i = 0; i < manager.getCount(); ++i) {
                Task* t = manager.getTaskAt(i);
                if (!t) continue;
                int64_t task_daily = 0;
                for (const auto& session : t->getSessions()) {
                    if (isSessionOnDate(session, selected_date)) {
                        task_daily += session.duration;  // Sum durations for the selected date
                    }
                }
                daily_total += task_daily;
                task_data.push_back({t->getName(), task_daily, t->getTotalDuration()});
            }

            // Display summary in a table
            ImGui::Text("Summary for %s", formatDate(selected_date).c_str());
            ImGui::Separator();
            if (ImGui::BeginTable("SummaryTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Task");
                ImGui::TableSetupColumn("Daily Time");
                ImGui::TableSetupColumn("Daily %");
                ImGui::TableSetupColumn("Cumulative Time");
                ImGui::TableHeadersRow();
                for (const auto& [name, daily, cumulative] : task_data) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", name.c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", formatDuration(daily).c_str());
                    ImGui::TableSetColumnIndex(2);
                    float percentage = daily_total > 0 ? (daily * 100.0f) / daily_total : 0.0f;
                    ImGui::Text("%.1f%%", percentage);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", formatDuration(cumulative).c_str());
                }
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Total");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", formatDuration(daily_total).c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("100.0%%");
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("-");
                ImGui::EndTable();
            }

            // Close button
            if (ImGui::Button("Close")) {
                show_summary = false;  // Close summary window
            }

            ImGui::End(); // End summary window
        }

        // 6.4 Render
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);  // Set OpenGL viewport to window size
        glClearColor(0, 0, 0, 0);  // Clear with transparent black
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update native window region to include main and summary windows
        {
            HWND hwnd = glfwGetWin32Window(window);  // Get native window handle
            HRGN region = CreateRectRgn(0, 0, 0, 0); // Create empty region
            for (const auto& rect : regions) {
                HRGN temp = CreateRectRgn((int)rect.x, (int)rect.y, (int)rect.z, (int)rect.w);  // Create region for each window
                CombineRgn(region, region, temp, RGN_OR);  // Combine regions
                DeleteObject(temp);  // Free temporary region
            }
            SetWindowRgn(hwnd, region, TRUE);  // Apply region for click-through
        }

        glfwSwapBuffers(window);  // Swap buffers to display rendered frame
    }

    // 7. Cleanup: pause running tasks, save, shutdown
    for (int i = 0; i < manager.getCount(); ++i) {
        Task* t = manager.getTaskAt(i);
        if (t && t->isRunning()) t->pause();  // Pause any running tasks before exit
    }
    manager.saveToFile("tasks.csv");  // Save tasks
    manager.saveSessionsToFile("sessions.csv");  // Save sessions

    ImGui_ImplOpenGL3_Shutdown();  // Shutdown ImGui OpenGL backend
    ImGui_ImplGlfw_Shutdown();     // Shutdown ImGui GLFW backend
    ImGui::DestroyContext();       // Destroy ImGui context
    glfwDestroyWindow(window);     // Destroy GLFW window
    glfwTerminate();               // Terminate GLFW
    return 0;
}
