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
        return;
    }

    auto wrapper =
        document->createElement("div");

    wrapper->style.left = "40px";
    wrapper->style.top = "30px";
    wrapper->style.width = "420px";
    wrapper->style.height = "280px";
    wrapper->style.overflow = "auto";

    wrapper->style.display = "flex";
    wrapper->style.flexDirection = "column";
    wrapper->style.padding = "16px";
    wrapper->style.gap = "12px";

    wrapper->style.backgroundColor = {
        25,
        25,
        30,
        255};

    wrapper->style.borderRadius =
        "12px";

    auto title =
        document->createElement("span");

    title->textContent =
        "AUTO HEIGHT TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "16px";

    auto section =
        document->createElement("div");

    section->style.width = "360px";
    section->style.height = "auto";

    section->style.display = "flex";
    section->style.flexDirection = "column";
    section->style.padding = "10px";
    section->style.gap = "8px";

    section->style.backgroundColor = {
        40,
        40,
        48,
        255};

    section->style.borderRadius =
        "8px";

    auto boxA =
        document->createElement("div");

    boxA->style.width = "100%";
    boxA->style.height = "40px";

    boxA->style.backgroundColor = {
        80,
        140,
        255,
        255};

    boxA->style.borderRadius =
        "6px";

    auto boxB =
        document->createElement("div");

    boxB->style.width = "100%";
    boxB->style.height = "40px";

    boxB->style.backgroundColor = {
        100,
        220,
        140,
        255};

    boxB->style.borderRadius =
        "6px";

    auto footer =
        document->createElement("div");

    footer->style.width = "360px";
    footer->style.height = "30px";

    footer->style.backgroundColor = {
        70,
        70,
        80,
        255};

    footer->style.borderRadius =
        "6px";

    auto button =
        document->createElement("button");

    button->textContent =
        "CHANGE HEIGHT";

    button->style.width =
        "170px";

    button->style.height =
        "40px";

    button->style.backgroundColor = {
        80,
        100,
        220,
        255};

    button->style.borderRadius =
        "8px";

    bool *changed =
        new bool(false);

    button->onClick =
        [document,
         title,
         boxA,
         boxB,
         changed]()
    {
        *changed =
            !*changed;

        if (*changed)
        {
            title->style.fontSize =
                "24px";

            boxA->style.height =
                "90px";

            boxB->style.height =
                "70px";
        }
        else
        {
            title->style.fontSize =
                "16px";

            boxA->style.height =
                "40px";

            boxB->style.height =
                "40px";
        }

        document->update();
    };

    section->appendChild(
        boxA);

    section->appendChild(
        boxB);

    wrapper->appendChild(
        title);

    wrapper->appendChild(
        section);

    wrapper->appendChild(
        footer);

    wrapper->appendChild(
        button);

    document->appendChild(
        wrapper);

    document->render();
}