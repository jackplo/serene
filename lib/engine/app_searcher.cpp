#include "../include/app_searcher.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

AppSearcher::AppSearcher() { initialize(); }

AppSearcher::~AppSearcher() {}

bool AppSearcher::initialize() { return load_applications(); }

Application AppSearcher::parse_desktop_file(const std::string &file_path) {
  Application app;
  app.desktop_file = file_path;
  app.terminal = false;

  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open desktop file: " << file_path << std::endl;
    return app;
  }

  std::string line;
  bool in_desktop_entry = false;
  std::regex name_regex("^Name=(.*)$");
  std::regex exec_regex(R"(^Exec\s*=\s*(.*)$)");
  std::regex icon_regex("^Icon=(.*)$");
  std::regex comment_regex("^Comment=(.*)$");
  std::regex terminal_regex("^Terminal=(.*)$");

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    if (line[0] == '[' && line != "[Desktop Entry]") {
      break;
    }

    if (line == "[Desktop Entry]") {
      in_desktop_entry = true;
      continue;
    }

    if (!in_desktop_entry)
      continue;

    std::smatch matches;

    if (std::regex_match(line, matches, name_regex) && matches.size() > 1) {
      app.name = matches[1].str();
    } else if (std::regex_match(line, matches, exec_regex) &&
               matches.size() > 1) {
      app.exec = matches[1].str();
    } else if (std::regex_match(line, matches, icon_regex) &&
               matches.size() > 1) {
      app.icon = matches[1].str();
    } else if (std::regex_match(line, matches, comment_regex) &&
               matches.size() > 1) {
      app.comment = matches[1].str();
    } else if (std::regex_match(line, matches, terminal_regex) &&
               matches.size() > 1) {
      app.terminal = (matches[1].str() == "true");
    }
  }

  if (app.name.empty()) {
    app.name = fs::path(file_path).stem().string();
  }

  return app;
}

bool AppSearcher::load_applications() {
  std::vector<std::string> app_dirs = {
      "/usr/share/applications",
      std::string(getenv("HOME")) + "/.local/share/applications",
      "/usr/local/share/applications/",
  };

  for (const auto &dir : app_dirs) {
    if (!fs::exists(dir))
      continue;

    for (const auto &entry : fs::directory_iterator(dir)) {
      if (entry.path().extension() == ".desktop") {
        Application app = parse_desktop_file(entry.path().string());

        if (!app.exec.empty()) {
          m_applications.push_back(app);
        }
      }
    }
  }

  std::sort(m_applications.begin(), m_applications.end(),
            [](const Application &a, const Application &b) {
              return a.name < b.name;
            });

  return true;
}

std::vector<Application> AppSearcher::search(const std::string &query) {
  std::vector<Application> results;
  std::string lowercase_query = query;
  std::transform(lowercase_query.begin(), lowercase_query.end(),
                 lowercase_query.begin(), ::tolower);

  for (const auto &app : m_applications) {
    std::string lowercase_name = app.name;
    std::transform(lowercase_name.begin(), lowercase_name.end(),
                   lowercase_name.begin(), ::tolower);

    std::string lowercase_exec = app.exec;
    std::transform(lowercase_exec.begin(), lowercase_exec.end(),
                   lowercase_exec.begin(), ::tolower);

    if (lowercase_name.find(lowercase_query) != std::string::npos ||
        lowercase_exec.find(lowercase_query) != std::string::npos) {

      bool already_added = false;
      for (const auto &result : results) {
        if (result.name == app.name && result.exec == app.exec) {
          already_added = true;
          break;
        }
      }

      if (!already_added) {
        results.push_back(app);
      }
    }
  }

  return results;
}