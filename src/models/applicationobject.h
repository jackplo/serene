#pragma once

#include "../backend/app_searcher.h"
#include <giomm/listmodel.h>

class ApplicationObject : public Glib::Object {
public:
  Application app;

  static Glib::RefPtr<ApplicationObject> create(const Application &application);

protected:
  ApplicationObject() {}
};
