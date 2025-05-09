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

  std::vector<Application> search(const std::string &query);
  bool initialize();

private:
  bool load_applications();
  Application parse_desktop_file(const std::string &file_path);
  std::vector<Application> m_applications;
};