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

    root->style.left = "20px";
    root->style.top = "20px";
    root->style.width = "420px";
    root->style.height = "220px";

    root->style.display = "flex";
    root->style.flexDirection = "column";
    root->style.gap = "8px";
    root->style.padding = "10px";

    root->style.backgroundColor = {
        20,
        20,
        26,
        255};

    auto title =
        document->createElement("span");

    title->textContent =
        "DOCUMENT LIFETIME TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "13px";
    title->style.flexShrink = 0.f;

    root->appendChild(title);

    auto status =
        document->createElement("span");

    status->textContent =
        "DOCUMENT ALIVE";

    status->style.width = "auto";
    status->style.height = "auto";
    status->style.fontSize = "10px";
    status->style.flexShrink = 0.f;

    root->appendChild(status);

    auto blocks =
        document->createElement("div");

    blocks->style.width = "100%";
    blocks->style.height = "60px";

    blocks->style.display = "flex";
    blocks->style.flexDirection = "row";
    blocks->style.gap = "3px";
    blocks->style.flexShrink = 0.f;

    for (int i = 0; i < 30; ++i)
    {
        auto item =
            document->createElement("div");

        item->style.width = "10px";
        item->style.height = "60px";
        item->style.flexShrink = 0.f;

        item->style.backgroundColor = {
            static_cast<GLubyte>(70 + i * 3),
            static_cast<GLubyte>(110 + i * 2),
            220,
            255};

        blocks->appendChild(item);
    }

    root->appendChild(blocks);

    auto destroyButton =
        document->createElement("button");

    destroyButton->textContent =
        "DELETE DOCUMENT";

    destroyButton->style.width = "160px";
    destroyButton->style.height = "32px";
    destroyButton->style.flexShrink = 0.f;

    root->appendChild(
        destroyButton);

    document->appendChild(root);
    document->render();

    destroyButton->onClick =
        [document]()
    {
        log::info(
            "GDOM TEST: deleting document");

        delete document;

        log::info(
            "GDOM TEST: document deleted");
    };
}