#include "../include/applicationobject.h"

Glib::RefPtr<ApplicationObject>
ApplicationObject::create(const Application &app) {
  return Glib::make_refptr_for_instance(new ApplicationObject(app));
}