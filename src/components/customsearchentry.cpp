// src/components/customsearchentry.cpp
#include "customsearchentry.h"
#include <gtkmm/eventcontrollerkey.h>
#include <iostream>

CustomSearchEntry::CustomSearchEntry()
    : Gtk::Box(Gtk::Orientation::HORIZONTAL) {
  setup_layout();

  m_entry.signal_changed().connect(
      sigc::mem_fun(*this, &CustomSearchEntry::on_text_changed));
  m_search_button.signal_clicked().connect(
      sigc::mem_fun(*this, &CustomSearchEntry::on_search_clicked));

  // Connect to Entry's activate signal
  m_entry.signal_activate().connect([this]() { m_signal_activate.emit(); });
}

void CustomSearchEntry::setup_layout() {
  add_css_class("custom-search-container");

  m_entry.set_placeholder_text("Search...");
  m_entry.add_css_class("custom-search-entry");
  m_entry.set_hexpand(true);
  m_entry.set_halign(Gtk::Align::FILL);

  m_search_button.set_icon_name("avatar-default-symbolic");
  m_search_button.add_css_class("search-button");
  m_search_button.set_tooltip_text("Search");

  append(m_search_button);
  append(m_entry);

  set_spacing(0);
  set_margin_start(0);
  set_margin_end(0);
}

CustomSearchEntry::~CustomSearchEntry() {}

void CustomSearchEntry::on_text_changed() { m_signal_changed.emit(); }

void CustomSearchEntry::on_search_clicked() {
  std::cout << "Search button clicked!" << std::endl;
}

void CustomSearchEntry::grab_focus() { m_entry.grab_focus(); }