#pragma once

#include <memory>
#include <string>
#include <vector>

struct Application {
  std::string name;         // Display name
  std::string exec;         // Command to execute
  std::string icon;         // Icon name/path
  std::string comment;      // Description
  std::string desktop_file; // Path to .desktop file
  bool terminal;            // Whether it's a terminal application
};

class AppSearcher {
public:
  AppSearcher();
  ~AppSearcher();

  // Search for applications matching the query
  std::vector<Application> search(const std::string &query);

  // Initialize the application database
  bool initialize();

private:
  // Load applications from the system
  bool load_applications();

  // Parse a .desktop file
  Application parse_desktop_file(const std::string &file_path);

  // Store the list of applications
  std::vector<Application> m_applications;
};