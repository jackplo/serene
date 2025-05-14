#pragma once

#include "applicationobject.h"
#include <giomm/listmodel.h>
#include <giomm/liststore.h>
#include <gtkmm/listview.h>

class ApplicationListModel : public Glib::Object {
public:
  static Glib::RefPtr<ApplicationListModel> create();

  void set_applications(const std::vector<Application> &applications);

  const Application &get_application(int index) const;

  Glib::RefPtr<Gio::ListModel> get_model() const;

protected:
  ApplicationListModel();

private:
  Glib::RefPtr<Gio::ListStore<ApplicationObject>> m_store;
};