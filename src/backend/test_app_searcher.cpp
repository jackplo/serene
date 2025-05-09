#include "app_searcher.h"
#include <iomanip>
#include <iostream>

void print_application(const Application &app) {
  std::cout << std::left << std::setw(30) << app.name << std::setw(20)
            << (app.terminal ? "Terminal" : "GUI") << std::setw(30) << app.icon
            << "\n  Exec: " << app.exec << "\n  Comment: " << app.comment
            << "\n  Desktop File: " << app.desktop_file << "\n"
            << std::string(80, '-') << "\n";
}

int main() {
  std::cout << "Initializing AppSearcher...\n";
  AppSearcher searcher;

  // Test some searches
  std::vector<std::string> test_queries = {
      //"term",    // Should find terminal applications
      "firefox", // Should find Firefox
      //"edit",    // Should find text editors
      "game", // Should find games
              //"x"        // Should find many applications
  };

  for (const auto &query : test_queries) {
    std::cout << "\nSearching for: '" << query << "'\n";
    std::cout << std::string(80, '=') << "\n";

    auto results = searcher.search(query);
    std::cout << "Found " << results.size() << " results:\n\n";

    for (const auto &app : results) {
      print_application(app);
    }
  }

  return 0;
}