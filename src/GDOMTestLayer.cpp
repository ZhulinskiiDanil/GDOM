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

    auto root =
        document->createElement("div");

    root->style.left = "30px";
    root->style.top = "20px";

    root->style.width =
        "calc(100% - 60px)";

    root->style.height =
        "calc(100% - 40px)";

    root->style.display = "flex";
    root->style.flexDirection = "column";

    root->style.padding = "14px";
    root->style.gap = "10px";

    root->style.backgroundColor = {
        20,
        20,
        26,
        255};

    root->style.borderRadius =
        "12px";

    auto title =
        document->createElement("span");

    title->textContent =
        "CALC() TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "18px";
    title->style.flexShrink = 0.f;

    root->appendChild(title);

    auto fullMinus =
        document->createElement("div");

    fullMinus->style.width =
        "calc(100% - 40px)";

    fullMinus->style.height =
        "40px";

    fullMinus->style.flexShrink = 0.f;

    fullMinus->style.backgroundColor = {
        70,
        130,
        255,
        255};

    root->appendChild(fullMinus);

    auto halfPlus =
        document->createElement("div");

    halfPlus->style.width =
        "calc(50% + 40px)";

    halfPlus->style.height =
        "40px";

    halfPlus->style.flexShrink = 0.f;

    halfPlus->style.backgroundColor = {
        80,
        210,
        130,
        255};

    root->appendChild(halfPlus);

    auto remTest =
        document->createElement("div");

    remTest->style.width =
        "calc(100% - 20rem)";

    remTest->style.height =
        "40px";

    remTest->style.flexShrink = 0.f;

    remTest->style.backgroundColor = {
        230,
        100,
        90,
        255};

    root->appendChild(remTest);

    auto row =
        document->createElement("div");

    row->style.width = "100%";
    row->style.height = "60px";

    row->style.display = "flex";
    row->style.flexDirection = "row";
    row->style.gap = "8px";
    row->style.flexShrink = 0.f;

    auto left =
        document->createElement("div");

    left->style.width =
        "calc(50% - 4px)";

    left->style.height =
        "60px";

    left->style.backgroundColor = {
        160,
        90,
        220,
        255};

    auto right =
        document->createElement("div");

    right->style.width =
        "calc(50% - 4px)";

    right->style.height =
        "60px";

    right->style.backgroundColor = {
        220,
        150,
        60,
        255};

    row->appendChild(left);
    row->appendChild(right);

    root->appendChild(row);

    auto dynamic =
        document->createElement("div");

    dynamic->style.width =
        "calc(100% - 100px)";

    dynamic->style.height =
        "45px";

    dynamic->style.flexShrink = 0.f;

    dynamic->style.backgroundColor = {
        70,
        180,
        200,
        255};

    root->appendChild(dynamic);

    auto toggle =
        document->createElement("button");

    toggle->textContent =
        "TOGGLE CALC";

    toggle->style.width =
        "160px";

    toggle->style.height =
        "36px";

    toggle->style.flexShrink = 0.f;

    toggle->style.backgroundColor = {
        100,
        80,
        220,
        255};

    toggle->style.borderRadius =
        "7px";

    auto changed =
        new bool(false);

    toggle->onClick =
        [document,
         dynamic,
         changed]()
    {
        *changed =
            !*changed;

        dynamic->style.width =
            *changed
                ? "calc(50% + 60px)"
                : "calc(100% - 100px)";

        document->update();
    };

    root->appendChild(toggle);

    document->appendChild(root);
    document->render();
}