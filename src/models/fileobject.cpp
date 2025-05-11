#include "fileobject.h"

Glib::RefPtr<FileObject> FileObject::create(const FileResult &file_result) {
  auto obj = Glib::make_refptr_for_instance(new FileObject());
  obj->result = file_result;
  return obj;
}