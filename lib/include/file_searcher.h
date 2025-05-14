#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct FileResult {
  std::string path;
  std::string name;
  bool is_directory;
  std::string mime_type;
  double match_score;
};

class FileSearcher {
public:
  FileSearcher();
  ~FileSearcher();

  std::vector<FileResult> search(const std::string &query,
                                 size_t max_results = 20);

  std::vector<FileResult>
  refine_search(const std::vector<FileResult> &current_results,
                const std::string &query, size_t max_results = 20);

  std::string get_mime_type(const std::string &path);

private:
  static const std::vector<std::string> COMMON_DIRS;

  bool matches_query(const std::string &filename, const std::string &query,
                     double &score);
  void search_directory(const std::filesystem::path &path,
                        const std::string &query,
                        std::vector<FileResult> &results, size_t max_results);
  bool should_skip_file(const std::string &filename) const;
  bool is_hidden_path(const std::filesystem::path &path) const;
  void add_result(const std::filesystem::path &path, const std::string &query,
                  std::vector<FileResult> &results,
                  double score_multiplier = 1.0);
  void search_common_directories(const std::string &home,
                                 const std::string &query,
                                 std::vector<FileResult> &results,
                                 size_t max_results,
                                 std::set<std::string> &searched_paths);
  void search_remaining_directories(
      const std::string &home, const std::string &query,
      std::vector<FileResult> &results, size_t max_results,
      const std::set<std::string> &searched_paths);
  double calculate_file_type_score(const std::string &filename) const;

  std::unordered_map<std::string, std::string> m_mime_cache;
};