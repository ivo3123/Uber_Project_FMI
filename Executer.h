#pragma once

#include "Uber.h"

namespace {
    const String CURRENCY = "levs";
}


class Executer {
private:

    Uber& uberSystem;
    POV_CurrentUser currentUser;

public:

    Executer (Uber& uber);

    void goToMainScreen ();

    void goToClientScreen ();
    void goToDriverScreen ();
};