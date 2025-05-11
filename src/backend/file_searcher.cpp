#include "file_searcher.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

const std::vector<std::string> FileSearcher::COMMON_DIRS = {
    "Desktop", "Documents", "Music",    "Public",
    "Videos",  "Downloads", "Pictures", "Templates"};

FileSearcher::FileSearcher() = default;
FileSearcher::~FileSearcher() = default;

bool FileSearcher::should_skip_file(const std::string &filename) const {
  if (filename[0] == '.')
    return true;

  const std::vector<std::string> skip_extensions = {
      ".tmp",    ".temp",  ".swp",       ".git",      ".cache",
      ".config", ".local", ".unlinked2", ".resolved", ".linked"};

  for (const auto &ext : skip_extensions) {
    if (filename.find(ext) != std::string::npos)
      return true;
  }

  if (filename.length() >= 32 &&
      std::all_of(filename.begin(), filename.end(), ::isxdigit)) {
    return true;
  }

  return false;
}

bool FileSearcher::is_hidden_path(const fs::path &path) const {
  for (const auto &part : path) {
    if (part.string()[0] == '.')
      return true;
  }
  return false;
}

double
FileSearcher::calculate_file_type_score(const std::string &filename) const {
  std::string filename_lower = filename;
  std::transform(filename_lower.begin(), filename_lower.end(),
                 filename_lower.begin(), ::tolower);

  if (filename_lower.find(".") == std::string::npos) {
    return 1.1;
  }

  std::string extension =
      filename_lower.substr(filename_lower.find_last_of(".") + 1);

  const std::vector<std::string> common_types = {
      "pdf", "doc", "docx", "txt", "jpg", "jpeg", "png", "mp3",
      "mp4", "mov", "zip",  "rar", "xls", "xlsx", "ppt", "pptx"};

  const std::vector<std::string> programming_types = {
      "cpp", "h",   "hpp", "java", "py", "js",
      "ts",  "php", "rb",  "go",   "rs", "swift"};

  if (std::find(common_types.begin(), common_types.end(), extension) !=
      common_types.end()) {
    return 1.2;
  }
  if (std::find(programming_types.begin(), programming_types.end(),
                extension) != programming_types.end()) {
    return 0.8;
  }

  return 1.0;
}

bool FileSearcher::matches_query(const std::string &filename,
                                 const std::string &query, double &score) {
  if (query.empty())
    return false;

  std::string filename_lower = filename;
  std::string query_lower = query;
  std::transform(filename_lower.begin(), filename_lower.end(),
                 filename_lower.begin(), ::tolower);
  std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(),
                 ::tolower);

  if (filename_lower.find(query_lower) != std::string::npos) {
    score = filename_lower.find(query_lower) == 0
                ? 1.0
                : 0.5 + (0.5 * query_lower.length() / filename_lower.length());

    score *= calculate_file_type_score(filename);

    std::string lower_name = filename_lower;
    if (lower_name == "documents" || lower_name == "downloads" ||
        lower_name == "pictures" || lower_name == "music" ||
        lower_name == "videos" || lower_name == "desktop") {
      score *= 1.3;
    }

    return true;
  }
  return false;
}

void FileSearcher::add_result(const fs::path &path, const std::string &query,
                              std::vector<FileResult> &results,
                              double score_multiplier) {
  std::string filename = path.filename().string();
  double score = 0.0;

  if (matches_query(filename, query, score)) {
    std::string path_str = path.string();
    auto it = std::find_if(
        results.begin(), results.end(),
        [&path_str](const FileResult &r) { return r.path == path_str; });

    if (it == results.end()) {
      FileResult result;
      result.path = path_str;
      result.name = filename;
      result.is_directory = fs::is_directory(path);

      if (result.is_directory) {
        result.mime_type = "inode/directory";
      } else {
        auto mime_it = m_mime_cache.find(path_str);
        if (mime_it != m_mime_cache.end()) {
          result.mime_type = mime_it->second;
        } else {
          result.mime_type = get_mime_type(result.path);
          m_mime_cache[path_str] = result.mime_type;
        }
      }

      result.match_score = score * score_multiplier;
      results.push_back(result);
    }
  }
}

void FileSearcher::search_directory(const fs::path &path,
                                    const std::string &query,
                                    std::vector<FileResult> &results,
                                    size_t max_results) {
  try {
    if (!should_skip_file(path.filename().string())) {
      add_result(path, query, results);
    }

    if (fs::is_directory(path)) {
      for (const auto &entry : fs::directory_iterator(path)) {
        if (results.size() >= max_results)
          break;

        if (!is_hidden_path(entry.path()) &&
            !should_skip_file(entry.path().filename().string())) {
          add_result(entry.path(), query, results);
        }
      }

      if (results.size() < max_results) {
        for (const auto &entry : fs::directory_iterator(path)) {
          if (results.size() >= max_results)
            break;

          if (fs::is_directory(entry.path()) && !is_hidden_path(entry.path())) {
            for (const auto &subentry : fs::directory_iterator(entry.path())) {
              if (results.size() >= max_results)
                break;

              if (!is_hidden_path(subentry.path()) &&
                  !should_skip_file(subentry.path().filename().string())) {
                add_result(subentry.path(), query, results);
              }
            }
          }
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error searching directory " << path << ": " << e.what()
              << std::endl;
  }
}

void FileSearcher::search_common_directories(
    const std::string &home, const std::string &query,
    std::vector<FileResult> &results, size_t max_results,
    std::set<std::string> &searched_paths) {
  for (const auto &dir : COMMON_DIRS) {
    std::string full_path = home + "/" + dir;
    if (fs::exists(full_path)) {
      searched_paths.insert(full_path);
      add_result(fs::path(full_path), query, results, 1.5);

      if (results.size() < max_results) {
        search_directory(full_path, query, results, max_results);
      }
    }
  }
}

void FileSearcher::search_remaining_directories(
    const std::string &home, const std::string &query,
    std::vector<FileResult> &results, size_t max_results,
    const std::set<std::string> &searched_paths) {
  for (const auto &entry : fs::directory_iterator(home)) {
    if (results.size() >= max_results)
      break;

    std::string path = entry.path().string();
    if (searched_paths.find(path) == searched_paths.end()) {
      search_directory(path, query, results, max_results);
    }
  }
}

std::vector<FileResult> FileSearcher::search(const std::string &query,
                                             size_t max_results) {
  std::vector<FileResult> results;
  if (query.empty())
    return results;

  const char *home = std::getenv("HOME");
  if (!home)
    return results;

  std::set<std::string> searched_paths;
  search_common_directories(home, query, results, max_results, searched_paths);

  if (results.size() < max_results) {
    search_remaining_directories(home, query, results, max_results,
                                 searched_paths);
  }

  std::sort(results.begin(), results.end(),
            [](const FileResult &a, const FileResult &b) {
              return a.match_score > b.match_score;
            });

  return results;
}

std::vector<FileResult>
FileSearcher::refine_search(const std::vector<FileResult> &current_results,
                            const std::string &query, size_t max_results) {
  std::vector<FileResult> refined_results;
  if (query.empty())
    return current_results;

  for (const auto &result : current_results) {
    if (refined_results.size() >= max_results)
      break;

    double score = 0.0;
    if (matches_query(result.name, query, score)) {
      FileResult new_result = result;
      new_result.match_score = score;
      refined_results.push_back(new_result);
    }
  }

  std::sort(refined_results.begin(), refined_results.end(),
            [](const FileResult &a, const FileResult &b) {
              return a.match_score > b.match_score;
            });

  return refined_results;
}

std::string FileSearcher::get_mime_type(const std::string &path) {
  std::string cmd = "xdg-mime query filetype \"" + path + "\" 2>/dev/null";
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
                                                pclose);
  if (!pipe)
    return "application/octet-stream";

  std::string result;
  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
    result += buffer;
  }

  if (!result.empty() && result[result.length() - 1] == '\n') {
    result.erase(result.length() - 1);
  }

  return result.empty() ? "application/octet-stream" : result;
}