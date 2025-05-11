#include "hybrid_file_searcher.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

HybridFileSearcher::HybridFileSearcher() {
  // Initialize with specific user directories
  const char *home = std::getenv("HOME");
  if (home) {
    std::vector<std::string> user_dirs = {"Desktop",  "Documents", "Music",
                                          "Public",   "Videos",    "Downloads",
                                          "Pictures", "Templates"};

    for (const auto &dir : user_dirs) {
      std::string full_path = std::string(home) + "/" + dir;
      if (fs::exists(full_path)) {
        m_frequent_dirs.push_back(full_path);
      }
    }
  }
}

HybridFileSearcher::~HybridFileSearcher() = default;

void HybridFileSearcher::initialize() {
  std::cout << "Initializing hybrid file searcher..." << std::endl;

  // Index each frequent directory
  for (const auto &dir : m_frequent_dirs) {
    if (fs::exists(dir)) {
      std::cout << "Indexing directory: " << dir << std::endl;
      index_directory(dir);
    }
  }
}

void HybridFileSearcher::index_directory(const std::string &path) {
  try {
    std::cout << "Indexing directory: " << path << std::endl;
    for (const auto &entry : fs::recursive_directory_iterator(path)) {
      if (entry.is_regular_file() || entry.is_directory()) {
        std::string filename = entry.path().filename().string();

        if (filename[0] == '.' || // Hidden files
            filename.find(".tmp") != std::string::npos ||
            filename.find(".temp") != std::string::npos ||
            filename.find(".swp") != std::string::npos ||
            filename.find(".git") != std::string::npos ||
            filename.find(".cache") != std::string::npos ||
            filename.find(".config") != std::string::npos ||
            filename.find(".local") != std::string::npos ||
            filename.find(".unlinked2") != std::string::npos ||
            filename.find(".resolved") != std::string::npos ||
            filename.find(".linked") != std::string::npos ||
            (filename.length() >= 32 &&
             std::all_of(filename.begin(), filename.end(), ::isxdigit))) {
          continue;
        }

        FileResult result;
        result.path = entry.path().string();
        result.name = filename;
        result.is_directory = entry.is_directory();
        result.mime_type = entry.is_regular_file()
                               ? m_file_searcher.get_mime_type(result.path)
                               : "inode/directory";

        m_index[filename].push_back(result);
        std::cout << "Indexed: " << filename << " ("
                  << (result.is_directory ? "dir" : "file") << ")" << std::endl;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error indexing directory " << path << ": " << e.what()
              << std::endl;
  }
}

bool HybridFileSearcher::is_frequent_directory(const std::string &path) const {
  return std::any_of(
      m_frequent_dirs.begin(), m_frequent_dirs.end(),
      [&path](const std::string &dir) { return path.find(dir) == 0; });
}

void HybridFileSearcher::add_to_cache(const std::string &query,
                                      const std::vector<FileResult> &results) {
  CachedSearch cached;
  cached.query = query;
  cached.results = results;
  cached.timestamp =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  // Add to front of cache
  m_search_cache.push_front(cached);

  // Remove oldest entry if cache is full
  if (m_search_cache.size() > MAX_CACHE_SIZE) {
    m_search_cache.pop_back();
  }
}

bool HybridFileSearcher::get_from_cache(const std::string &query,
                                        std::vector<FileResult> &results) {
  auto it = std::find_if(
      m_search_cache.begin(), m_search_cache.end(),
      [&query](const CachedSearch &cached) { return cached.query == query; });

  if (it != m_search_cache.end()) {
    results = it->results;
    return true;
  }
  return false;
}

std::vector<FileResult> HybridFileSearcher::search(const std::string &query) {
  // Check cache first
  std::vector<FileResult> results;
  if (get_from_cache(query, results)) {
    return results;
  }

  // Search in indexed directories first
  for (const auto &[filename, entries] : m_index) {
    if (filename.find(query) != std::string::npos) {
      results.insert(results.end(), entries.begin(), entries.end());
    }
  }

  // If no results in indexed directories, do a filesystem search
  if (results.empty()) {
    results = m_file_searcher.search(query);
  }

  // Cache the results
  add_to_cache(query, results);
  return results;
}

std::vector<CachedSearch> HybridFileSearcher::get_recent_searches() const {
  return std::vector<CachedSearch>(m_search_cache.begin(),
                                   m_search_cache.end());
}