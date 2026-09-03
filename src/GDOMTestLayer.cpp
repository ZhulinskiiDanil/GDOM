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
    const auto document =
        gdom::GDOMDocument::create(this);

    auto list =
        document->createElement("div");

    list->style.left = "100px";
    list->style.top = "50px";

    list->style.width = "300px";
    list->style.height = "140px";

    list->style.display = "flex";
    list->style.flexDirection = "column";

    list->style.padding = "8px";
    list->style.gap = "6px";

    list->style.overflow = "auto";

    list->style.backgroundColor = {
        25,
        25,
        30,
        255};

    list->style.borderColor = {
        100,
        100,
        120,
        255};

    list->style.borderWidth = "1px";
    list->style.borderRadius = "8px";

    for (int i = 0; i < 12; ++i)
    {
        auto item =
            document->createElement("div");

        item->style.width = "100%";
        item->style.height = "32px";

        item->style.backgroundColor = {
            static_cast<GLubyte>(
                40 + i * 4),
            60,
            90,
            255};

        item->style.borderRadius = "5px";

        list->appendChild(item);
    }

    document->appendChild(list);
    document->render();
}