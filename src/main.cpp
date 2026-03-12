#include "application/application.hpp"

int main() {
    Application app;
    if (!app.Init(1000, 800, "My game Engine")) {
        return -1;
    }

    app.Run();
    return 0;
}