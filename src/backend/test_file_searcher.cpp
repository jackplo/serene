#include "file_searcher.h"
#include <iomanip>
#include <iostream>

void print_results(const std::vector<FileResult> &results) {
  std::cout << "\nFound " << results.size() << " results:" << std::endl;
  std::cout << std::setw(8) << "Score" << " | " << std::setw(6) << "Type"
            << " | " << std::setw(40) << "Name" << " | "
            << "Path" << std::endl;
  std::cout << std::string(80, '-') << std::endl;

  for (const auto &result : results) {
    std::cout << std::fixed << std::setprecision(2) << std::setw(8)
              << result.match_score << " | " << std::setw(6)
              << (result.is_directory ? "DIR" : "FILE") << " | "
              << std::setw(40) << result.name << " | " << result.path
              << std::endl;
  }
}

int main() {
  FileSearcher searcher;
  std::vector<FileResult> current_results;
  std::string query;

  std::cout << "File Searcher Test Program" << std::endl;
  std::cout << "------------------------" << std::endl;
  std::cout << "Enter search queries to find files in your home directory."
            << std::endl;
  std::cout << "Type 'refine' to refine the current results." << std::endl;
  std::cout << "Type 'q' to quit." << std::endl;

  while (true) {
    std::cout << "\nEnter search query (or 'refine'/'q'): ";
    std::getline(std::cin, query);

    if (query == "q") {
      break;
    }

    if (query.empty()) {
      continue;
    }

    if (query == "refine") {
      if (current_results.empty()) {
        std::cout << "No results to refine. Please search first." << std::endl;
        continue;
      }

      std::cout << "Enter refinement query: ";
      std::getline(std::cin, query);

      if (query.empty()) {
        continue;
      }

      std::cout << "\nRefining search with: " << query << std::endl;
      current_results = searcher.refine_search(current_results, query);
    } else {
      std::cout << "\nSearching for: " << query << std::endl;
      current_results = searcher.search(query);
    }

    print_results(current_results);
  }

  return 0;
}