#ifndef APPLICATION_HPP
#define APPLICATION_HPP

struct GLFWwindow;


class Application {
    public:
    Application();
    ~Application();

    bool Init(int width, int height, const char* title);
    void Run();
    void Shutdown();

    private:
    GLFWwindow* window;
    bool running;
    double lastFrameTime;
};

#endif