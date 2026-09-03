#include <GDOMTestLayer.hpp>

#include <GDOM/GDOMDocument.hpp>

using namespace geode::prelude;

GDOMTestLayer *GDOMTestLayer::create()
{
    auto layer =
        new GDOMTestLayer();

    if (!layer)
    {
        return nullptr;
    }

    if (!layer->init())
    {
        delete layer;
        return nullptr;
    }

    layer->autorelease();

    return layer;
}

void GDOMTestLayer::open()
{
    auto scene =
        CCScene::create();

    if (!scene)
    {
        return;
    }

    auto layer =
        GDOMTestLayer::create();

    if (!layer)
    {
        return;
    }

    scene->addChild(
        layer);

    CCDirector::sharedDirector()
        ->pushScene(
            CCTransitionFade::create(
                0.25f,
                scene));
}

bool GDOMTestLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    const auto winSize =
        CCDirector::sharedDirector()
            ->getWinSize();

    setContentSize(
        winSize);

    //
    // Clean background
    //

    auto background =
        CCLayerColor::create(
            {18,
             20,
             28,
             255},
            winSize.width,
            winSize.height);

    if (background)
    {
        addChild(
            background,
            -100);
    }

    //
    // Native cocos close button.
    //
    // Intentionally NOT GDOM.
    // We should always be able to leave
    // the test layer if GDOM breaks.
    //

    auto closeSprite =
        CCSprite::createWithSpriteFrameName(
            "GJ_closeBtn_001.png");

    if (closeSprite)
    {
        auto closeButton =
            CCMenuItemSpriteExtra::create(
                closeSprite,
                this,
                menu_selector(
                    GDOMTestLayer::onClose));

        auto menu =
            CCMenu::create();

        menu->setPosition({0.f,
                           0.f});

        closeButton->setPosition({25.f,
                                  winSize.height -
                                      25.f});

        menu->addChild(
            closeButton);

        addChild(
            menu,
            1000);
    }

    buildTestUI();

    return true;
}

void GDOMTestLayer::onClose(
    CCObject *)
{
    CCDirector::sharedDirector()
        ->popScene();
}

void GDOMTestLayer::buildTestUI()
{
    auto document =
        gdom::GDOMDocument::create(this);

    if (!document)
    {
        log::error(
            "LIFECYCLE TEST: document create failed");

        return;
    }

    log::info(
        "LIFECYCLE TEST: document created");

    auto root =
        document->createElement("div");

    root->style.left = "80px";
    root->style.top = "40px";

    root->style.width = "350px";
    root->style.height = "180px";

    root->style.display = "flex";
    root->style.flexDirection = "column";

    root->style.padding = "15px";
    root->style.gap = "10px";

    root->style.backgroundColor = {
        35,
        40,
        55,
        255};

    root->style.borderRadius =
        "8px";

    auto input =
        document->createElement("input");

    input->placeholder =
        "Focus me before closing";

    input->style.width = "280px";
    input->style.height = "36px";
    input->style.flexShrink = 0.f;

    auto button =
        document->createElement("button");

    button->textContent =
        "TEST BUTTON";

    button->style.width = "140px";
    button->style.height = "34px";
    button->style.flexShrink = 0.f;

    input->onFocus =
        []()
    {
        log::info(
            "LIFECYCLE TEST: input focus");
    };

    input->onBlur =
        []()
    {
        log::info(
            "LIFECYCLE TEST: input blur");
    };

    input->onInput =
        [](
            const std::string &value)
    {
        log::info(
            "LIFECYCLE TEST: input '{}'",
            value);
    };

    button->onClick =
        []()
    {
        log::info(
            "LIFECYCLE TEST: button works");
    };

    root->appendChild(
        input);

    root->appendChild(
        button);

    document->appendChild(
        root);

    document->render();

    log::info(
        "LIFECYCLE TEST: rendered");
}