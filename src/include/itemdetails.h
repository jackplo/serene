#pragma once

#include "applicationobject.h"
#include "fileobject.h"
#include "lib/include/app_searcher.h"
#include "lib/include/file_searcher.h"
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

class ItemDetails : public Gtk::Box {
public:
  ItemDetails();
  ~ItemDetails();

  void set_item(const Glib::RefPtr<Glib::Object> &item);

protected:
  Gtk::Image m_icon;
  Gtk::Label m_name;
  Gtk::Label m_description;
  Gtk::Label m_path;
  Gtk::Label m_size;
  Gtk::Label m_type;

private:
  void update_app_details(const Application &app);
  void update_file_details(const FileResult &file);
  std::string format_file_size(size_t size);
};
