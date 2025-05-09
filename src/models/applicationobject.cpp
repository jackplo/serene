#include "applicationobject.h"

Glib::RefPtr<ApplicationObject>
ApplicationObject::create(const Application &application) {
  auto obj = Glib::make_refptr_for_instance<ApplicationObject>(
      new ApplicationObject());
  obj->app = application;
  return obj;
}