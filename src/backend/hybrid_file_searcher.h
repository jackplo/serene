#pragma once

#include "file_searcher.h"
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct CachedSearch {
  std::string query;
  std::vector<FileResult> results;
  time_t timestamp;
};

class HybridFileSearcher {
public:
  HybridFileSearcher();
  ~HybridFileSearcher();

  // Initialize the searcher with frequently accessed directories
  void initialize();

  // Search files with hybrid approach
  std::vector<FileResult> search(const std::string &query);

  // Get recent searches
  std::vector<CachedSearch> get_recent_searches() const;

private:
  // Index a specific directory
  void index_directory(const std::string &path);

  // Check if a path is in a frequently accessed directory
  bool is_frequent_directory(const std::string &path) const;

  // Cache management
  void add_to_cache(const std::string &query,
                    const std::vector<FileResult> &results);
  bool get_from_cache(const std::string &query,
                      std::vector<FileResult> &results);

  FileSearcher m_file_searcher;
  std::unordered_map<std::string, std::vector<FileResult>> m_index;
  std::deque<CachedSearch> m_search_cache;
  std::vector<std::string> m_frequent_dirs;
  static const size_t MAX_CACHE_SIZE = 100;
};