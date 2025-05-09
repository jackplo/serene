#include "listview.h"
#include "../models/applicationobject.h"
#include <cstdlib>
#include <giomm/listmodel.h>
#include <gtkmm/box.h>
#include <gtkmm/eventcontrollerkey.h>
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
  m_model = ApplicationListModel::create();

  auto key_controller = Gtk::EventControllerKey::create();

  key_controller->signal_key_pressed().connect(
      [this](guint keyval, guint keycode, Gdk::ModifierType state) -> bool {
        return this->on_key_pressed(keyval, keycode, state);
      },
      false);

  key_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);

  add_controller(key_controller);

  auto click_controller = Gtk::GestureClick::create();
  click_controller->set_button(GDK_BUTTON_PRIMARY); // Left mouse button
  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &ListView::on_click_pressed));
  add_controller(click_controller);

  m_selection = Gtk::SingleSelection::create(m_model->get_model());

  signal_activate().connect(sigc::mem_fun(*this, &ListView::on_row_activated));

  set_model(m_selection);
}

ListView::~ListView() {}

bool ListView::on_key_pressed(guint keyval, guint keycode,
                              Gdk::ModifierType state) {
  if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(
          m_selection->get_selected_item());
      if (app_obj) {
        launch_application(app_obj->app.exec);
        return true;
      }
    }
  }
  return false;
}

void ListView::on_click_pressed(int n_press, double x, double y) {
  if (n_press == 2) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(
          m_selection->get_selected_item());
      if (app_obj) {
        launch_application(app_obj->app.exec);
      }
    }
  }
}
void ListView::setup_factory() {
  m_factory = Gtk::SignalListItemFactory::create();

  m_factory->signal_setup().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &list_item) {
        auto box =
            Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
        box->set_margin_start(6);
        box->set_margin_end(6);
        box->set_margin_top(6);
        box->set_margin_bottom(6);

        auto icon = Gtk::make_managed<Gtk::Image>();
        icon->set_size_request(32, 32);

        auto name_label = Gtk::make_managed<Gtk::Label>();
        name_label->set_xalign(0);
        name_label->set_hexpand(true);

        box->append(*icon);
        box->append(*name_label);
        list_item->set_child(*box);
      });

  m_factory->signal_bind().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &list_item) {
        auto box = dynamic_cast<Gtk::Box *>(list_item->get_child());
        if (!box)
          return;

        auto app_obj =
            std::dynamic_pointer_cast<ApplicationObject>(list_item->get_item());
        if (!app_obj)
          return;

        const Application &app = app_obj->app;

        auto icon = dynamic_cast<Gtk::Image *>(box->get_first_child());
        auto label = dynamic_cast<Gtk::Label *>(box->get_last_child());

        if (icon && label) {
          if (!app.icon.empty()) {
            icon->set_from_icon_name(app.icon);
          } else {
            icon->set_from_icon_name("application-x-executable");
          }

          label->set_text(app.name);

          if (!app.comment.empty()) {
            label->set_tooltip_text(app.comment);
          }
        }
      });

  set_factory(m_factory);
}

void ListView::update_model(const std::vector<Application> &applications) {
  m_model->set_applications(applications);
}

void ListView::on_row_activated(guint position) {
  std::cout << "Row activated at position: " << position << std::endl;

  auto model = m_selection->get_model();
  auto item = model->get_object(position);

  auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item);

  if (app_obj) {
    std::cout << "Launching via activation: " << app_obj->app.name << std::endl;
    launch_application(app_obj->app.exec);
  }
}

void ListView::launch_application(const std::string &exec_command) {
  std::string cleaned_command =
      std::regex_replace(exec_command, std::regex(R"(%[uUfFcCikK])"), "");

  std::istringstream iss(cleaned_command);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  if (tokens.empty()) {
    std::cerr << "No executable found in command.\n";
    return;
  }

  std::vector<char *> argv;
  for (auto &t : tokens) {
    argv.push_back(&t[0]);
  }
  argv.push_back(nullptr);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(argv[0], argv.data());
    std::perror("execvp failed");
    std::exit(1);
  } else if (pid < 0) {
    std::perror("fork failed");
  }
}