#pragma once

#include "applicationlistmodel.h"
#include "fileobject.h"
#include <giomm/liststore.h>
#include <gtkmm/listitem.h>

class CombinedListModel : public Glib::Object {
public:
  static Glib::RefPtr<CombinedListModel> create();

  void update(const std::vector<Application> &apps,
              const std::vector<FileResult> &files);
  Glib::RefPtr<Gio::ListStore<Glib::Object>> get_store() { return m_store; }

private:
  CombinedListModel();
  Glib::RefPtr<Gio::ListStore<Glib::Object>> m_store;
};