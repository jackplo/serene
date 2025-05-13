#include "../components/itemdetails.h"
#include "../models/applicationobject.h"
#include "../models/fileobject.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>

class TestWindow : public Gtk::Window {
public:
  TestWindow() {
    set_title("ItemDetails Test");
    set_default_size(400, 600);

    auto box = Gtk::Box(Gtk::Orientation::VERTICAL, 6);
    box.set_margin_start(12);
    box.set_margin_end(12);
    box.set_margin_top(12);
    box.set_margin_bottom(12);

    m_itemDetails.set_hexpand(true);
    m_itemDetails.set_vexpand(true);
    box.append(m_itemDetails);

    set_child(box);

    // Test with an application
    Application app;
    app.name = "Test Application";
    app.exec = "test-app";
    app.icon = "applications-system";
    app.comment = "This is a test application";
    app.desktop_file = "/usr/share/applications/test.desktop";
    app.terminal = false;

    auto app_obj = ApplicationObject::create(app);
    m_itemDetails.set_item(app_obj);

    // Test with a file
    FileResult file;
    file.name = "test.txt";
    file.path = "/home/user/test.txt";
    file.is_directory = false;
    file.mime_type = "text/plain";

    auto file_obj = FileObject::create(file);
    m_itemDetails.set_item(file_obj);

    // Test with a directory
    FileResult dir;
    dir.name = "Documents";
    dir.path = "/home/user/Documents";
    dir.is_directory = true;
    dir.mime_type = "inode/directory";

    auto dir_obj = FileObject::create(dir);
    m_itemDetails.set_item(dir_obj);
  }

private:
  ItemDetails m_itemDetails;
};

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.spotlight.search");
  return app->make_window_and_run<TestWindow>(argc, argv);
}