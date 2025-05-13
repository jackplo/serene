#include "itemdetails.h"

ItemDetails::ItemDetails() {
  set_orientation(Gtk::Orientation::VERTICAL);
  set_spacing(10);
  set_margin_start(10);
  set_margin_end(10);
  set_margin_top(10);
  set_margin_bottom(10);
  add_css_class("item-details");

  m_icon.set_icon_size(Gtk::IconSize::LARGE);
  m_icon.set_halign(Gtk::Align::CENTER);
  append(m_icon);

  m_name.set_halign(Gtk::Align::START);
  m_name.add_css_class("item-name");
  append(m_name);

  m_description.set_halign(Gtk::Align::START);
  m_description.set_wrap(true);
  m_description.set_wrap_mode(Pango::WrapMode::WORD_CHAR);
  m_description.set_max_width_chars(30);
  m_description.add_css_class("item-description");
  append(m_description);

  m_path.set_halign(Gtk::Align::START);
  m_path.set_wrap(true);
  m_path.set_wrap_mode(Pango::WrapMode::WORD_CHAR);
  m_path.set_max_width_chars(30);
  m_path.add_css_class("item-path");
  append(m_path);

  m_size.set_halign(Gtk::Align::START);
  m_size.add_css_class("item-size");
  append(m_size);

  m_type.set_halign(Gtk::Align::START);
  m_type.add_css_class("item-type");
  append(m_type);

  hide();
}

ItemDetails::~ItemDetails() {}

void ItemDetails::set_item(const Glib::RefPtr<Glib::Object> &item) {
  if (!item) {
    hide();
    return;
  }

  if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item)) {
    update_app_details(app_obj->app);
  } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(item)) {
    update_file_details(file_obj->result);
  }

  show();
}

void ItemDetails::update_app_details(const Application &app) {
  m_icon.set_from_icon_name(app.icon);
  m_name.set_text(app.name);
  m_description.set_text(app.comment);
  m_path.set_text(app.desktop_file);
  m_size.set_text("");
  m_type.set_text("Application");
}

void ItemDetails::update_file_details(const FileResult &file) {
  if (file.is_directory) {
    m_icon.set_from_icon_name("folder");
  } else {
    m_icon.set_from_icon_name(file.mime_type);
  }

  m_name.set_text(file.name);
  m_description.set_text("");
  m_path.set_text(file.path);

  if (file.is_directory) {
    m_size.set_text("Directory");
  } else {
    m_size.set_text("File");
  }

  m_type.set_text(file.mime_type);
}

std::string ItemDetails::format_file_size(size_t size) {
  const char *units[] = {"B", "KB", "MB", "GB", "TB"};
  int unit_index = 0;
  double size_d = static_cast<double>(size);

  while (size_d >= 1024.0 && unit_index < 4) {
    size_d /= 1024.0;
    unit_index++;
  }

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%.1f %s", size_d, units[unit_index]);
  return std::string(buffer);
}