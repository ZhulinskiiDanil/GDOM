#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <GDOMTestLayer.hpp>

using namespace geode::prelude;

namespace
{

    class GDOMTestButtonTarget : public CCNode
    {
    public:
        static GDOMTestButtonTarget *create()
        {
            auto *target =
                new GDOMTestButtonTarget();

            if (!target)
            {
                return nullptr;
            }

            if (!target->init())
            {
                delete target;
                return nullptr;
            }

            target->autorelease();

            return target;
        }

        void onOpen(
            CCObject *)
        {
            GDOMTestLayer::open();
        }
    };

}

class $modify(MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
        {
            return false;
        }

        if (!Mod::get()->getSettingValue<bool>(
                "show-test-button"))
        {
            return true;
        }

        auto *sprite =
            ButtonSprite::create(
                "GDOM");

        if (!sprite)
        {
            return true;
        }

        auto *target =
            GDOMTestButtonTarget::create();

        if (!target)
        {
            return true;
        }

        auto *button =
            CCMenuItemSpriteExtra::create(
                sprite,
                target,
                menu_selector(
                    GDOMTestButtonTarget::onOpen));

        if (!button)
        {
            return true;
        }

        auto *menu =
            CCMenu::create();

        if (!menu)
        {
            return true;
        }

        menu->setPosition(
            CCPoint(
                0.f,
                0.f));

        button->setPosition(
            CCPoint(
                75.f,
                30.f));

        menu->addChild(
            button);

        addChild(
            target);

        addChild(
            menu,
            100);

        return true;
    }
};