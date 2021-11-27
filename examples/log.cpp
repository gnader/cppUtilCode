#include "log.h"
#include <iostream>

#include <windows.h>

int main(int argc, char **argv)
{
  Log::set_verbose_level(Log::LVL_WARNING);

  PRINT_ERROR_ORIGIN("This should not have happened, but we can tell where it originates from.\n");
  PRINT_WARNING("Just be cautious\n");
  PRINT_DEBUG("This message will not appear !!\n");

  Log::set_verbose_level(Log::LVL_DETAIL);

  PRINT_COLORED(Log::TXT_MAGENTA, "this is a colored message.\n");
  PRINT_DEBUG("I can now see debug messages.\n");

  Log::set_verbose_level(Log::LVL_MUTE);

  PRINT("now everything is muted so no message will show up. \n");
  PRINT_ERROR("not even an error \n");
  PRINT_ALWAYS("Even if everything is muted, these messages will still show up. \n");

  return 0;
}