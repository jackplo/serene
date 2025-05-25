#pragma once

#include "../lib/include/file_searcher.h"
#include "../lib/include/result.h"
#include <giomm/listmodel.h>

class FileObject : public Glib::Object {
public:
  FileResult result;

  static Glib::RefPtr<FileObject> create(const Result &file_result);

protected:
  FileObject() {}
  FileObject(const FileResult &file_result) : result(file_result) {}
  FileObject(const Result &file_result)
      : result{file_result.path, file_result.name, file_result.is_directory,
               file_result.mime_type, file_result.match_score} {}
};