#pragma once

#include <vector>
#include <memory>

class Shader;
struct LightManager;
class GeometryPass;
class OutlinePass;
class FinalPass;
class Renderable;
class RenderContext;

class Renderer {
public:
    Renderer(std::unique_ptr<GeometryPass> geo, std::unique_ptr<OutlinePass> outline, std::unique_ptr<FinalPass> final);
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void submit(Renderable* obj);
    void render(const RenderContext& rct);

    void beginFrame();
    void endFrame();

private:
    void setupGLState();

private:
    std::unique_ptr<GeometryPass> geometrypass;
    std::unique_ptr<OutlinePass> outlinepass;
    std::unique_ptr<FinalPass> finalpass;

    std::vector<Renderable*> renderQueue;
};