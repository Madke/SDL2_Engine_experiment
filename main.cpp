#include "controller.h"

int main(int argc, char** argv)
{
    controller control("./settings.ini");

    while(control.loop()) {}

    return control.exit();
}
