#include "../include/listview.h"
#include "../include/applicationobject.h"
#include <cstdlib>
#include <gdkmm/pixbuf.h>
#include <gdkmm/texture.h>
#include <giomm/contenttype.h>
#include <giomm/listmodel.h>
#include <gtk/gtkimage.h>
#include <gtkmm/box.h>
#include <gtkmm/eventcontroller.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <regex>

ListView::ListView() {
  set_vexpand(true);
  add_css_class("spotlight-list");
  set_single_click_activate(true);
  setup_factory();
  m_model = CombinedListModel::create();

  m_selection = Gtk::SingleSelection::create(m_model->get_store());

  signal_activate().connect(sigc::mem_fun(*this, &ListView::on_row_activated));

  set_model(m_selection);

  auto key_controller = Gtk::EventControllerKey::create();
  key_controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &ListView::on_key_pressed), false);
  add_controller(key_controller);

  auto click_controller = Gtk::GestureClick::create();
  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &ListView::on_click_pressed));
  add_controller(click_controller);
}

ListView::~ListView() {}

void ListView::update_model(const std::vector<Application> &applications,
                            const std::vector<FileResult> &files) {
  m_model->update(applications, files);
}

void ListView::launch_application(const std::string &exec_command) {
  std::string cmd = exec_command + " &";
  system(cmd.c_str());
}

void ListView::open_file(const std::string &path) {
  std::string cmd = "xdg-open \"" + path + "\" &";
  system(cmd.c_str());
}

void ListView::setup_factory() {
  m_factory = Gtk::SignalListItemFactory::create();

  m_factory->signal_setup().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &item) {
        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
        box->set_margin_start(6);
        box->set_margin_end(6);
        box->set_margin_top(6);
        box->set_margin_bottom(6);

        auto icon = Gtk::make_managed<Gtk::Image>();
        icon->set_icon_size(Gtk::IconSize::LARGE);
        box->append(*icon);

        auto label = Gtk::make_managed<Gtk::Label>();
        label->set_halign(Gtk::Align::START);
        label->set_hexpand(true);
        label->set_ellipsize(Pango::EllipsizeMode::END);
        box->append(*label);

        item->set_child(*box);
      });

  m_factory->signal_bind().connect(
      [this](const Glib::RefPtr<Gtk::ListItem> &item) {
        auto box = dynamic_cast<Gtk::Box *>(item->get_child());
        if (!box)
          return;

        auto icon = dynamic_cast<Gtk::Image *>(box->get_first_child());
        auto label = dynamic_cast<Gtk::Label *>(box->get_last_child());
        if (!icon || !label)
          return;

        if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(
                item->get_item())) {
          if (!app_obj->app.icon.empty()) {
            icon->set_from_icon_name(app_obj->app.icon);
          } else {
            icon->set_from_icon_name("applications-system");
          }
          label->set_text(app_obj->app.name);
        } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(
                       item->get_item())) {
          if (file_obj->result.is_directory) {
            icon->set_from_icon_name("folder");
          } else {
            // preview for image files
            if (file_obj->result.mime_type.find("image/") == 0) {
              try {
                auto pixbuf = Gdk::Pixbuf::create_from_file(
                    file_obj->result.path, 32, 32, true);
                if (pixbuf) {
                  auto texture = Gdk::Texture::create_for_pixbuf(pixbuf);
                  icon->set(texture);
                } else {
                  icon->set_from_icon_name(file_obj->result.mime_type);
                }
              } catch (const Glib::Error &) {
                icon->set_from_icon_name(file_obj->result.mime_type);
              }
            } else {
              // non-image files use system icon
              auto content_type =
                  Gio::content_type_from_mime_type(file_obj->result.mime_type);
              auto icon_name = Gio::content_type_get_icon(content_type);
              if (icon_name) {
                icon->set(icon_name);
              } else {
                icon->set_from_icon_name("text-x-generic");
              }
            }
          }
          label->set_text(file_obj->result.name);
        }
      });

  set_factory(m_factory);
}

bool ListView::on_key_pressed(guint keyval, guint keycode,
                              Gdk::ModifierType state) {
  if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      on_row_activated(selected_pos);
      return true;
    }
  }
  return false;
}

void ListView::on_click_pressed(int n_press, double x, double y) {
  if (n_press == 2) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      on_row_activated(selected_pos);
    }
  }
}

void ListView::on_row_activated(guint position) {
  auto item = m_selection->get_selected_item();
  if (!item)
    return;

  if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item)) {
    launch_application(app_obj->app.exec);
  } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(item)) {
    open_file(file_obj->result.path);
  }
}