#include "hybrid_file_searcher.h"
#include <ctime>
#include <iomanip>
#include <iostream>

void print_results(const std::vector<FileResult> &results) {
  std::cout << "Found " << results.size() << " results:" << std::endl;
  for (const auto &result : results) {
    std::cout << (result.is_directory ? "[DIR] " : "[FILE] ") << result.name
              << " (" << result.path << ")"
              << " - " << result.mime_type << std::endl;
  }
}

void print_recent_searches(const std::vector<CachedSearch> &searches) {
  std::cout << "\nRecent searches:" << std::endl;
  for (const auto &search : searches) {
    std::cout << "Query: " << search.query << " ("
              << std::put_time(std::localtime(&search.timestamp),
                               "%Y-%m-%d %H:%M:%S")
              << ") - " << search.results.size() << " results" << std::endl;
  }
}

int main() {
  HybridFileSearcher searcher;

  std::cout << "Initializing hybrid file searcher..." << std::endl;
  searcher.initialize();

  std::string query;
  while (true) {
    std::cout << "\nEnter search query (or 'q' to quit): ";
    std::getline(std::cin, query);

    if (query == "q") {
      break;
    }

    if (query.empty()) {
      continue;
    }

    std::cout << "\nSearching for: " << query << std::endl;
    auto results = searcher.search(query);
    print_results(results);

    // Show recent searches after each query
    print_recent_searches(searcher.get_recent_searches());
  }

  return 0;
}