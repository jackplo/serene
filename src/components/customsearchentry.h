// src/components/customsearchentry.h
#pragma once
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/entry.h>

class CustomSearchEntry : public Gtk::Box {
public:
  CustomSearchEntry();
  virtual ~CustomSearchEntry();

  Glib::ustring get_text() const { return m_entry.get_text(); }
  void set_text(const Glib::ustring &text) { m_entry.set_text(text); }

  sigc::signal<void()> &signal_changed() { return m_signal_changed; }
  sigc::signal<void()> &signal_activate() { return m_signal_activate; }

  void grab_focus();

private:
  void on_text_changed();
  void on_search_clicked();
  void setup_layout();

  Gtk::Entry m_entry;
  Gtk::Button m_search_button;
  sigc::signal<void()> m_signal_changed;
  sigc::signal<void()> m_signal_activate;
};