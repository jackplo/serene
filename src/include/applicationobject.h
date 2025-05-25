#pragma once

#include "../lib/include/app_searcher.h"
#include "../lib/include/result.h"
#include <giomm/listmodel.h>

class ApplicationObject : public Glib::Object {
public:
  Application app;

  static Glib::RefPtr<ApplicationObject> create(const Result &application);

protected:
  ApplicationObject() {}
  ApplicationObject(const Application &application) : app(application) {}
  ApplicationObject(const Result &application)
      : app{application.name,         application.exec,
            application.icon,         application.comment,
            application.desktop_file, application.terminal} {}
};
