#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

struct ApplicationEntry {
  int id;
  std::string name;
  std::string exec;
  std::string icon;
  std::string comment;
  std::string desktop_file;
  bool terminal; // stored as INTEGER in DB
};

struct FileEntry {
  int id;
  std::string name;
  std::string path;
  bool is_dir; // stored as INTEGER in DB
  std::string mime_type;
  int64_t mtime; // modification time as integer
};

class DBManager {
public:
  explicit DBManager(const std::string &dbPath);
  ~DBManager();

  void initialize();

  // Application table
  bool insertOrUpdateApplication(const ApplicationEntry &app);
  std::vector<ApplicationEntry>
  searchApplicationsByName(const std::string &namePattern);

  // File table
  bool insertOrUpdateFile(const FileEntry &file);
  std::vector<FileEntry> searchFilesByName(const std::string &namePattern);

private:
  SQLite::Database m_db;

  ApplicationEntry rowToApplicationEntry(SQLite::Statement &query);
  FileEntry rowToFileEntry(SQLite::Statement &query);
};
