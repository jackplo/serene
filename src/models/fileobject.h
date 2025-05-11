#pragma once

#include "../backend/file_searcher.h"
#include <giomm/listmodel.h>

class FileObject : public Glib::Object {
public:
  FileResult result;

  static Glib::RefPtr<FileObject> create(const FileResult &file_result);

protected:
  FileObject() {}
};