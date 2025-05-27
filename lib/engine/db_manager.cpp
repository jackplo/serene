#include "../include/db_manager.h"
#include <iostream>

DBManager::DBManager(const std::string &dbPath)
    : m_db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
  initialize();
}

DBManager::~DBManager() = default;

void DBManager::initialize() {
  const char *createTablesSQL = R"(
        CREATE TABLE IF NOT EXISTS applications (
            id INTEGER PRIMARY KEY,
            name TEXT,
            exec TEXT,
            icon TEXT,
            comment TEXT,
            desktop_file TEXT,
            terminal INTEGER
        );

        CREATE TABLE IF NOT EXISTS files (
            id INTEGER PRIMARY KEY,
            name TEXT,
            path TEXT,
            is_dir INTEGER,
            mime_type TEXT,
            mtime INTEGER
        );

        CREATE INDEX IF NOT EXISTS idx_files_name ON files(name);
        CREATE INDEX IF NOT EXISTS idx_applications_name ON applications(name);
    )";

  m_db.exec(createTablesSQL);
}

// --- Applications ---

bool DBManager::insertOrUpdateApplication(const ApplicationEntry &app) {
  try {
    SQLite::Statement query(m_db, R"(
            INSERT OR REPLACE INTO applications 
                (id, name, exec, icon, comment, desktop_file, terminal)
            VALUES (?, ?, ?, ?, ?, ?, ?)
        )");

    query.bind(1, app.id);
    query.bind(2, app.name);
    query.bind(3, app.exec);
    query.bind(4, app.icon);
    query.bind(5, app.comment);
    query.bind(6, app.desktop_file);
    query.bind(7, app.terminal ? 1 : 0);

    query.exec();
    return true;
  } catch (const std::exception &e) {
    std::cerr << "DB insertOrUpdateApplication error: " << e.what() << "\n";
    return false;
  }
}

std::vector<ApplicationEntry>
DBManager::searchApplicationsByName(const std::string &namePattern) {
  std::vector<ApplicationEntry> results;
  try {
    SQLite::Statement query(
        m_db, "SELECT id, name, exec, icon, comment, desktop_file, terminal "
              "FROM applications WHERE name LIKE ? ORDER BY name LIMIT 100");
    std::string likePattern = "%" + namePattern + "%";
    query.bind(1, likePattern);

    while (query.executeStep()) {
      results.push_back(rowToApplicationEntry(query));
    }
  } catch (const std::exception &e) {
    std::cerr << "DB searchApplicationsByName error: " << e.what() << "\n";
  }
  return results;
}

ApplicationEntry DBManager::rowToApplicationEntry(SQLite::Statement &query) {
  ApplicationEntry app;
  app.id = query.getColumn(0).getInt();
  app.name = query.getColumn(1).getString();
  app.exec = query.getColumn(2).getString();
  app.icon = query.getColumn(3).getString();
  app.comment = query.getColumn(4).getString();
  app.desktop_file = query.getColumn(5).getString();
  app.terminal = query.getColumn(6).getInt() != 0;
  return app;
}

// --- Files ---

bool DBManager::insertOrUpdateFile(const FileEntry &file) {
  try {
    SQLite::Statement query(m_db, R"(
            INSERT OR REPLACE INTO files
                (id, name, path, is_dir, mime_type, mtime)
            VALUES (?, ?, ?, ?, ?, ?)
        )");

    query.bind(1, file.id);
    query.bind(2, file.name);
    query.bind(3, file.path);
    query.bind(4, file.is_dir ? 1 : 0);
    query.bind(5, file.mime_type);
    query.bind(6, static_cast<long>(file.mtime));

    query.exec();
    return true;
  } catch (const std::exception &e) {
    std::cerr << "DB insertOrUpdateFile error: " << e.what() << "\n";
    return false;
  }
}

std::vector<FileEntry>
DBManager::searchFilesByName(const std::string &namePattern) {
  std::vector<FileEntry> results;
  try {
    SQLite::Statement query(
        m_db, "SELECT id, name, path, is_dir, mime_type, mtime FROM files "
              "WHERE name LIKE ? ORDER BY name LIMIT 100");
    std::string likePattern = "%" + namePattern + "%";
    query.bind(1, likePattern);

    while (query.executeStep()) {
      results.push_back(rowToFileEntry(query));
    }
  } catch (const std::exception &e) {
    std::cerr << "DB searchFilesByName error: " << e.what() << "\n";
  }
  return results;
}

FileEntry DBManager::rowToFileEntry(SQLite::Statement &query) {
  FileEntry file;
  file.id = query.getColumn(0).getInt();
  file.name = query.getColumn(1).getString();
  file.path = query.getColumn(2).getString();
  file.is_dir = query.getColumn(3).getInt() != 0;
  file.mime_type = query.getColumn(4).getString();
  file.mtime = query.getColumn(5).getInt64();
  return file;
}
