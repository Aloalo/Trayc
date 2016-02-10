#ifndef ET_GUI_H
#define ET_GUI_H

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <Engine/Core/Updateable.h>
#include <Engine/Core/InputObserver.h>

#include <glm/glm.hpp>

class GUI : public engine::Updateable, public engine::InputObserver
{
public:
    void Init(const std::string &resourceDirectory);
    void Destroy();

    void Draw();
    virtual void Update(float dt) override;

    void SetMouseCursor(const std::string &imageFile);
    void ShowMouseCursor();
    void HideMouseCursor();

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;
    //virtual void TextInput(const TextInputEvent &e) override;

    void LoadScheme(const std::string &schemeFile);
    void SetFont(const std::string &fontFile);
    CEGUI::Window* CreateWidget(const std::string &type, const glm::vec4 &destRectPerc, const glm::vec4 &destRectPix, const std::string &name);
    CEGUI::Window* CreateWidget(CEGUI::Window *parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name);

    // Getters
    // static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
    // const CEGUI::GUIContext* GetContext() const { return m_context; }
private:
    static CEGUI::OpenGL3Renderer *mRenderer;
    CEGUI::GUIContext *mContext = nullptr;
    CEGUI::Window *mRootWindow = nullptr;
};

#endif
