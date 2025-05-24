#pragma once
#include "app_searcher.h"
#include "file_searcher.h"
#include <string>
#include <variant>

enum class ResultType { APPLICATION, FILE };

struct Result {
  ResultType type;

  // Common fields
  std::string name;
  std::string path;

  // Application-specific fields
  std::string icon;
  std::string exec;
  std::string comment;
  std::string desktop_file;
  bool terminal;

  // File-specific fields
  bool is_directory;
  std::string mime_type;
  double match_score;

  // Constructor for Application
  static Result from_application(const Application &app) {
    Result r;
    r.type = ResultType::APPLICATION;
    r.name = app.name;
    r.icon = app.icon;
    r.path = app.desktop_file;
    r.exec = app.exec;
    r.comment = app.comment;
    r.desktop_file = app.desktop_file;
    r.terminal = app.terminal;
    r.is_directory = false;
    r.mime_type = "application/x-desktop";
    r.match_score = 1.0;
    return r;
  }

  // Constructor for FileResult
  static Result from_file(const FileResult &file) {
    Result r;
    r.type = ResultType::FILE;
    r.icon = file.mime_type;
    r.name = file.name;
    r.path = file.path;
    r.is_directory = file.is_directory;
    r.mime_type = file.mime_type;
    r.match_score = file.match_score;
    r.terminal = false;
    return r;
  }
};