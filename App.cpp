#include "App.h"

App::App()
{
    Running = true;
    window = NULL;
    screenSurface = NULL;
    renderer = NULL;

    active = false;
}

App::~App()
{
    //dtor
}

int App::OnExecute()
{
  if (OnInit() == false)
    {
      return -1;
    }

  SDL_Event Event;

  while (Running)
    {
      while (SDL_PollEvent(&Event))
        {
          OnEvent (&Event);
        }
      OnLoop();
      OnRender();
    }

  OnCleanup();

  return 0;
}

void print_SDL_version(char* preamble, SDL_version* v) {
   printf("%s %u.%u.%u\n", preamble, v->major, v->minor, v->patch);
}

void print_SDL_versions() {
    SDL_version ver;

    // Prints the compile time version
    SDL_VERSION(&ver);
    print_SDL_version("SDL compile-time version", &ver);

    // Prints the run-time version
//    ver = *SDL_Linked_Version();
//    print_SDL_version("SDL runtime version", &ver);
}

int main (int argc, char *argv[])
{
    print_SDL_versions();

    App theApp;

    return theApp.OnExecute();
}
