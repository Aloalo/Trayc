#include "GUI.h"

using namespace std;
using namespace glm;
using namespace CEGUI;

OpenGL3Renderer* GUI::mRenderer = nullptr;

void GUI::Init(const string &resourceDirectory)
{
    // Check if the renderer and system were not already initialized
    if (mRenderer == nullptr) {
        mRenderer = &OpenGL3Renderer::bootstrapSystem();
    }

    DefaultResourceProvider *rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
    rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
    rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
    rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
    rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

    ImageManager::setImagesetDefaultResourceGroup("imagesets");
    Scheme::setDefaultResourceGroup("schemes");
    Font::setDefaultResourceGroup("fonts");
    WidgetLookManager::setDefaultResourceGroup("looknfeels");
    WindowManager::setDefaultResourceGroup("layouts");
    ScriptModule::setDefaultResourceGroup("lua_scripts");

    mContext = &System::getSingleton().createGUIContext(mRenderer->getDefaultRenderTarget());
    mRootWindow = WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    mContext->setRootWindow(mRootWindow);
}

void GUI::Destroy()
{
    System::getSingleton().destroyGUIContext(*mContext);
    WindowManager::getSingleton().destroyWindow(mRootWindow);
    mContext = nullptr;
    mRootWindow = nullptr;
}

void GUI::Draw()
{
    glDisable(GL_DEPTH_TEST);
    mRenderer->beginRendering();
    mContext->draw();
    mRenderer->endRendering();
    // Clean up after CEGUI
    /*glBindVertexArray(0);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
}

void GUI::Update(float dt)
{
    mContext->injectTimePulse(dt);
}

void GUI::SetMouseCursor(const string &imageFile)
{
    mContext->getMouseCursor().setDefaultImage(imageFile);
}

void GUI::ShowMouseCursor()
{
    mContext->getMouseCursor().show();
}

void GUI::HideMouseCursor()
{
    mContext->getMouseCursor().hide();
}

static inline Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key)
{
    switch (key) {
        case SDLK_BACKSPACE:    return Key::Backspace;
        case SDLK_TAB:          return Key::Tab;
        case SDLK_RETURN:       return Key::Return;
        case SDLK_PAUSE:        return Key::Pause;
        case SDLK_ESCAPE:       return Key::Escape;
        case SDLK_SPACE:        return Key::Space;
        case SDLK_COMMA:        return Key::Comma;
        case SDLK_MINUS:        return Key::Minus;
        case SDLK_PERIOD:       return Key::Period;
        case SDLK_SLASH:        return Key::Slash;
        case SDLK_0:            return Key::Zero;
        case SDLK_1:            return Key::One;
        case SDLK_2:            return Key::Two;
        case SDLK_3:            return Key::Three;
        case SDLK_4:            return Key::Four;
        case SDLK_5:            return Key::Five;
        case SDLK_6:            return Key::Six;
        case SDLK_7:            return Key::Seven;
        case SDLK_8:            return Key::Eight;
        case SDLK_9:            return Key::Nine;
        case SDLK_COLON:        return Key::Colon;
        case SDLK_SEMICOLON:    return Key::Semicolon;
        case SDLK_EQUALS:       return Key::Equals;
        case SDLK_LEFTBRACKET:  return Key::LeftBracket;
        case SDLK_BACKSLASH:    return Key::Backslash;
        case SDLK_RIGHTBRACKET: return Key::RightBracket;
        case SDLK_a:            return Key::A;
        case SDLK_b:            return Key::B;
        case SDLK_c:            return Key::C;
        case SDLK_d:            return Key::D;
        case SDLK_e:            return Key::E;
        case SDLK_f:            return Key::F;
        case SDLK_g:            return Key::G;
        case SDLK_h:            return Key::H;
        case SDLK_i:            return Key::I;
        case SDLK_j:            return Key::J;
        case SDLK_k:            return Key::K;
        case SDLK_l:            return Key::L;
        case SDLK_m:            return Key::M;
        case SDLK_n:            return Key::N;
        case SDLK_o:            return Key::O;
        case SDLK_p:            return Key::P;
        case SDLK_q:            return Key::Q;
        case SDLK_r:            return Key::R;
        case SDLK_s:            return Key::S;
        case SDLK_t:            return Key::T;
        case SDLK_u:            return Key::U;
        case SDLK_v:            return Key::V;
        case SDLK_w:            return Key::W;
        case SDLK_x:            return Key::X;
        case SDLK_y:            return Key::Y;
        case SDLK_z:            return Key::Z;
        case SDLK_DELETE:       return Key::Delete;
        case SDLK_KP_PERIOD:    return Key::Decimal;
        case SDLK_KP_DIVIDE:    return Key::Divide;
        case SDLK_KP_MULTIPLY:  return Key::Multiply;
        case SDLK_KP_MINUS:     return Key::Subtract;
        case SDLK_KP_PLUS:      return Key::Add;
        case SDLK_KP_ENTER:     return Key::NumpadEnter;
        case SDLK_KP_EQUALS:    return Key::NumpadEquals;
        case SDLK_UP:           return Key::ArrowUp;
        case SDLK_DOWN:         return Key::ArrowDown;
        case SDLK_RIGHT:        return Key::ArrowRight;
        case SDLK_LEFT:         return Key::ArrowLeft;
        case SDLK_INSERT:       return Key::Insert;
        case SDLK_HOME:         return Key::Home;
        case SDLK_END:          return Key::End;
        case SDLK_PAGEUP:       return Key::PageUp;
        case SDLK_PAGEDOWN:     return Key::PageDown;
        case SDLK_F1:           return Key::F1;
        case SDLK_F2:           return Key::F2;
        case SDLK_F3:           return Key::F3;
        case SDLK_F4:           return Key::F4;
        case SDLK_F5:           return Key::F5;
        case SDLK_F6:           return Key::F6;
        case SDLK_F7:           return Key::F7;
        case SDLK_F8:           return Key::F8;
        case SDLK_F9:           return Key::F9;
        case SDLK_F10:          return Key::F10;
        case SDLK_F11:          return Key::F11;
        case SDLK_F12:          return Key::F12;
        case SDLK_F13:          return Key::F13;
        case SDLK_F14:          return Key::F14;
        case SDLK_F15:          return Key::F15;
        case SDLK_RSHIFT:       return Key::RightShift;
        case SDLK_LSHIFT:       return Key::LeftShift;
        case SDLK_RCTRL:        return Key::RightControl;
        case SDLK_LCTRL:        return Key::LeftControl;
        case SDLK_RALT:         return Key::RightAlt;
        case SDLK_LALT:         return Key::LeftAlt;
        case SDLK_SYSREQ:       return Key::SysRq;
        case SDLK_MENU:         return Key::AppMenu;
        case SDLK_POWER:        return Key::Power;
        default:                return Key::Unknown;
    }
}


static inline MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton)
{
    switch(sdlButton)
    {
    case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
    case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
    case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
    case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
    case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
    }
    return CEGUI::MouseButton::NoButton;
}

void GUI::KeyPress(const SDL_KeyboardEvent &e)
{
    mContext->injectKeyDown(SDLKeyToCEGUIKey(e.keysym.sym));
}

void GUI::MouseMotion(const SDL_MouseMotionEvent &e)
{
    mContext->injectMousePosition((float)e.x, (float)e.y);
}

void GUI::MouseButton(const SDL_MouseButtonEvent &e)
{
    if(e.state == SDL_PRESSED) {
        mContext->injectMouseButtonDown(SDLButtonToCEGUIButton(e.button));
    }
    else {
        mContext->injectMouseButtonUp(SDLButtonToCEGUIButton(e.button));
    }
}

/*void GUI::TextInput(const TextInputEvent &e)
{
    //TODO: This is wrong! We need to decode utf-8 and convert to utf-32.
    //Thanks to Spartan190 for figuring this out. You need to get a utf conversion library
    //or function that can convert the text, such as from UTF8-CPP: http://utfcpp.sourceforge.net/
    //If you use UTF8-CPP just use this code and it should work:

    //string evntText = string(evnt.text.text);
    //vector<int> utf32result;
    //utf8::utf8to32(evnt.text.text, evnt.text.text + evntText.size(), back_inserter(utf32result));
    //utf32`codePoint = (utf32)utf32result[0];
    //mContext->injectChar(codePoint);

    utf32 codePoint = 0;
    for(int i = 0; e.text[i] != '\0'; ++i) {
        codePoint |= (((utf32)*(unsigned char*)&e.text[i]) << (i * 8));
    }
    mContext->injectChar(codePoint);
}*/

void GUI::LoadScheme(const string &schemeFile)
{
    SchemeManager::getSingleton().createFromFile(schemeFile);
}

static inline void SetWidgetDestRect(Window *widget, const vec4 &destRectPerc, const vec4 &destRectPix)
{
    widget->setPosition(UVector2(UDim(destRectPerc.x, destRectPix.x), UDim(destRectPerc.y, destRectPix.y)));
    widget->setSize(USize(UDim(destRectPerc.z, destRectPix.z), UDim(destRectPerc.w, destRectPix.w)));
}

Window* GUI::CreateWidget(const string &type, const vec4 &destRectPerc, const vec4 &destRectPix, const string &name)
{
    Window *newWindow = WindowManager::getSingleton().createWindow(type, name);
    mRootWindow->addChild(newWindow);
    SetWidgetDestRect(newWindow, destRectPerc, destRectPix);
    return newWindow;
}

Window* GUI::CreateWidget(Window *parent, const string &type, const vec4 &destRectPerc, const vec4 &destRectPix, const string &name)
{
    Window *newWindow = WindowManager::getSingleton().createWindow(type, name);
    parent->addChild(newWindow);
    SetWidgetDestRect(newWindow, destRectPerc, destRectPix);
    return newWindow;
}

void GUI::SetFont(const string &fontFile)
{
    FontManager::getSingleton().createFromFile(fontFile + ".font");
    mContext->setDefaultFont(fontFile);
}