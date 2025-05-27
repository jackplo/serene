#include "../lib/include/db_manager.h"
#include "db_manager.h"
#include <iostream>

void testApplications(DBManager &db) {
  ApplicationEntry app{.id = 1,
                       .name = "Test App",
                       .exec = "/usr/bin/testapp",
                       .icon = "test-icon",
                       .comment = "A test application",
                       .desktop_file = "/usr/share/applications/test.desktop",
                       .terminal = false};

  if (db.insertOrUpdateApplication(app)) {
    std::cout << "Inserted test application.\n";
  }

  auto results = db.searchApplicationsByName("Test");
  std::cout << "Application search results:\n";
  for (const auto &result : results) {
    std::cout << " - " << result.name << " | " << result.exec << "\n";
  }
}

void testFiles(DBManager &db) {
  FileEntry file{.id = 1,
                 .name = "example.txt",
                 .path = "/home/user/example.txt",
                 .is_dir = false,
                 .mime_type = "text/plain",
                 .mtime = 1716800000};

  if (db.insertOrUpdateFile(file)) {
    std::cout << "Inserted test file.\n";
  }

  auto results = db.searchFilesByName("example");
  std::cout << "File search results:\n";
  for (const auto &result : results) {
    std::cout << " - " << result.name << " | " << result.path << "\n";
  }
}

int main() {
  DBManager db("test.db");

  std::cout << "=== Testing DBManager ===\n";
  testApplications(db);
  testFiles(db);
  std::cout << "=== Done ===\n";

  return 0;
}
