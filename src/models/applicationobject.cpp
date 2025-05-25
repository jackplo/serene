#include "../include/applicationobject.h"

Glib::RefPtr<ApplicationObject> ApplicationObject::create(const Result &app) {
  return Glib::make_refptr_for_instance(new ApplicationObject(app));
}