#pragma once
#include <windows.h>
#include "../../constants.hpp"
#include "./callbackContainer.hpp"
#include "./loading.hpp"
#include "./graphicElement.hpp"
#include "./label.hpp"

namespace owo
{
    class Input : public virtual GraphicElement
    {
    private:
        const short OUTLINE_THICKNESS = 1;
        std::string text;
        Label* label;
        StringCallbackContainer* cont;
        std::string highlight;
        Loading* loadLogo;
        sf::Color textColor;

        bool CTRL_pressed;
        bool loading;
        bool shouldLoad;

        int cursorPos;

        void init()
        {
            this->cursorPos = 0;
            this->CTRL_pressed = false;
            this->loading = false;
            this->shouldLoad = false;
            this->loadLogo = new Loading(sf::Vector2i(OUTLINE_THICKNESS, this->getSize().y-4-OUTLINE_THICKNESS), sf::Vector2i(this->getSize().x-OUTLINE_THICKNESS*2, 4));
            this->setReceiveEvents(true);
        }

        void copyToClipboard()
        {
            if (! OpenClipboard(nullptr))
            {
                std::cerr << "Error opening the clipboard" << std::endl;
                return;
            }
            HGLOBAL clipbuffer;
            char * buffer;
            EmptyClipboard();
            clipbuffer = GlobalAlloc(GMEM_DDESHARE, this->text.size()+1);
            buffer = (char*)GlobalLock(clipbuffer);
            strcpy(buffer, LPCSTR(this->text.c_str()));
            GlobalUnlock(clipbuffer);
            SetClipboardData(CF_TEXT,clipbuffer);
            CloseClipboard();
            CloseClipboard();
        }

        void pasteFromClipboard()
        {
            if (!OpenClipboard(nullptr))
            {
                std::cerr << "Error opening the clipboard" << std::endl;
                return;
            }
            char* data = (char*) GetClipboardData(CF_TEXT);
            if (data == nullptr)
            {
                std::cerr << "Error getting clipboard content" << std::endl;
                return;
            }

            std::string text(data);
            for (int i = 0; i < text.size(); i++)
                this->addLetter(text.at(i));
            
            CloseClipboard();
        }

        void addLetter(char letter)
        {
            this->text = this->text.substr(0, this->cursorPos) + letter + this->text.substr(this->cursorPos, this->text.size());
            this->cursorPos++;
        }

    public:
        static const int LEFT = Label::LEFT;
        static const int CENTER = Label::CENTER;
        static const int RIGHT = Label::RIGHT;

        Input()
        {
            this->setDimensions(0, 0, 200, 50);
            this->text = "Input";
            this->textColor = CONSTANT::COLOR_PRIMARY;
            this->label = new Label(this->text, 16);
            this->init();
            this->generateTexture();
        }

        Input(std::string text,
              int fontSize = 16, int placement = Input::CENTER,
              sf::Color textColor = CONSTANT::COLOR_PRIMARY, std::string highlight = "Input text")
        {
            this->setDimensions(0, 0, 200, 50);
            this->text = text;
            this->highlight = highlight;
            this->label = new Label(this->text, fontSize, placement, textColor);
            this->textColor = textColor;
            this->init();
            this->generateTexture();
        }

        Input(std::string text,
              sf::Vector2i position, sf::Vector2i size = sf::Vector2i(-1, -1),
              int fontSize = 16, int placement = Input::CENTER, sf::Color textColor = CONSTANT::COLOR_WHITE_LIGHT,
              std::string highlight = "Input text")
        {
            this->setDimensions(position.x, position.y, size.x, size.y);
            this->text = text;
            this->highlight = highlight;
            this->textColor = textColor;
            this->label = new Label(this->text, sf::Vector2i(1, 0), size+sf::Vector2i(-2, 0), fontSize, placement, textColor);
            this->init();
            this->generateTexture();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            if (this->text != "" || this->focused)
            {
                this->label->setTextColor(this->textColor);
                this->label->setText(this->text);
                this->renderTexture.draw(this->label->getSprite(0));
                if (this->focused)
                {
                    sf::RectangleShape cursor(sf::Vector2f(1, this->label->getFontSize()));
                    sf::Vector2f pos = this->label->calculateTextPos(this->text);
                    sf::Vector2u size = this->label->calculateTextSize(this->text.substr(0, this->cursorPos));
                    cursor.setPosition(pos.x+size.x+1, pos.y+2);
                    cursor.setFillColor(CONSTANT::COLOR_PRIMARY);
                    this->renderTexture.draw(cursor);
                }
            } else
            {
                this->label->setTextColor(CONSTANT::COLOR_GREY_DARK);
                this->label->setText(this->highlight);
                this->renderTexture.draw(this->label->getSprite(0));
            }
            sf::Color color = CONSTANT::COLOR_WHITE_DARK;
            if (this->focused) color = this->label->getTextColor();
            sf::RectangleShape highlightRect(sf::Vector2f(this->dimensions.width-4, this->dimensions.height-4));
            highlightRect.setPosition(sf::Vector2f(2, 2));
            highlightRect.setFillColor(CONSTANT::COLOR_TRANS);
            highlightRect.setOutlineColor( this->focused? CONSTANT::COLOR_PRIMARY: CONSTANT::COLOR_FORE );
            highlightRect.setOutlineThickness(this->OUTLINE_THICKNESS);
            this->renderTexture.draw(highlightRect);

            this->loading = this->shouldLoad;
            if (this->loading)
                this->renderTexture.draw(this->loadLogo->getSprite(0.16));

            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

        void onClick(int btn, bool clicked)
        {
            this->clicked = clicked;
        }

        void onFocus(bool focused)
        {
            this->focused = focused;
            if (this->focused)
                this->cursorPos = this->text.size();
            this->generateTexture();
            if (!this->focused && this->cont != nullptr)
                this->cont->func(this->text);
        }

        void onHover(bool hovered)
        {
            this->hovered = hovered;
        }
        
        void onKey(int key, char c, bool pressed)
        {
            switch (key)
            {
                case sf::Keyboard::Left:
                    if (!pressed) break;
                    if (this->CTRL_pressed)
                    {
                        while (this->cursorPos > 0)
                        {
                            this->cursorPos--;
                            if (this->cursorPos > 0 && this->text.at(this->cursorPos-1) == ' ')
                                break;
                        }
                    } else
                    {
                        if (this->cursorPos > 0)
                          this->cursorPos--;
                    }
                    break;
                case sf::Keyboard::Right:
                    if (!pressed) break;
                    if (this->CTRL_pressed)
                    {
                        while (this->cursorPos < this->text.size())
                        {
                            this->cursorPos++;
                            if (this->cursorPos < this->text.size() && this->text.at(this->cursorPos) == ' ')
                                break;
                        }
                    } else
                    {
                        if (this->cursorPos < this->text.size())
                            this->cursorPos++;
                    }
                    break;
                case sf::Keyboard::End:
                    this->cursorPos = this->text.size();
                    break;
                case sf::Keyboard::Home:
                    this->cursorPos = 0;
                    break;
                case sf::Keyboard::LControl:
                case sf::Keyboard::RControl:
                    this->CTRL_pressed = pressed;
                    break;
                case sf::Keyboard::Delete:
                    if (!pressed) break;
                    if (this->cursorPos < this->text.size())
                        this->text = this->text.substr(0, this->cursorPos) + this->text.substr(this->cursorPos+1, this->text.size());
                    break;
                case sf::Keyboard::BackSpace:
                    if (!pressed) break;
                    if (this->text.size() > 0 && this->cursorPos > 0)
                    {
                        this->text = this->text.substr(0, this->cursorPos-1) + this->text.substr(this->cursorPos, this->text.size());
                        this->cursorPos--;
                    }
                    break;
                default:
                    if (!pressed) break;
                    if (c == 3)
                    {
                        this->copyToClipboard();
                        break;
                    }
                    if (c == 22)
                    {
                        this->pasteFromClipboard();
                        break;
                    }
                    if (c == 13)
                        break;
                    if (c != CONSTANT::NO_CHAR)
                        this->addLetter(c);
                    break;
            }
            this->generateTexture();
        }

        void onScroll(int delta)
        {
            
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            
        }

        void setLoading(bool state)
        {
            this->shouldLoad = state;
        }

        bool isLoading()
        {
            return this->shouldLoad;
        }

        sf::Sprite getSprite(float dt)
        {
            if (this->shouldLoad || this->loading != this->shouldLoad)
                this->generateTexture();
            return this->sprite;
        }

        std::string getText()
        {
            return this->text;
        }

        template<class T> void setCallback(void (T::*callback)(std::string), T* instance)
        {
            this->cont = new TypedStringCallbackContainer<T>(callback, instance);
        }

        void setCallback(void (callback)(std::string))
        {
            this->cont = new VoidStringCallbackContainer(callback);
        }

        ~Input()
        {

        }

    };
}