#ifndef IRENDERADAPTER_HPP
#define IRENDERADAPTER_HPP

class IRenderAdapter {
public:
    virtual ~IRenderAdapter() = default;

    virtual bool Init() = 0;
    virtual void Clear(float r, float g, float b, float a) = 0;
    virtual void DrawTriangle() = 0;
    virtual void SwapBuffers() = 0;
    virtual void Shutdown() = 0;

    virtual void ReloadShaders() = 0;
};

#endif