#include "../lib/include/engine.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
  std::string query = "repos";

  Engine *engine = new Engine();

  engine->search(query);
  delete engine;
  return 0;
}