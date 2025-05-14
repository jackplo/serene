#include "../include/fileobject.h"

Glib::RefPtr<FileObject> FileObject::create(const FileResult &file) {
  return Glib::make_refptr_for_instance(new FileObject(file));
}
