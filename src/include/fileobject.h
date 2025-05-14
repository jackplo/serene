#pragma once

#include "../lib/include/file_searcher.h"
#include <giomm/listmodel.h>

class FileObject : public Glib::Object {
public:
  FileResult result;

  static Glib::RefPtr<FileObject> create(const FileResult &file_result);

protected:
  FileObject() {}
  FileObject(const FileResult &file_result) : result(file_result) {}
};