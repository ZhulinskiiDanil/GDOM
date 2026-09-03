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
    root->style.width = "500px";
    root->style.height = "auto";

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
        "DISPLAY: NONE TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "18px";

    root->appendChild(title);

    //
    // FLEX TEST
    //

    auto flexLabel =
        document->createElement("span");

    flexLabel->textContent =
        "Flex layout";

    flexLabel->style.width = "auto";
    flexLabel->style.height = "auto";
    flexLabel->style.fontSize = "11px";

    root->appendChild(flexLabel);

    auto flexRow =
        document->createElement("div");

    flexRow->style.width = "450px";
    flexRow->style.height = "50px";

    flexRow->style.display = "flex";
    flexRow->style.flexDirection = "row";
    flexRow->style.gap = "8px";

    auto flexA =
        document->createElement("div");

    flexA->style.width = "100px";
    flexA->style.height = "50px";

    flexA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto flexB =
        document->createElement("div");

    flexB->style.width = "100px";
    flexB->style.height = "50px";

    flexB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto flexC =
        document->createElement("div");

    flexC->style.width = "100px";
    flexC->style.height = "50px";

    flexC->style.backgroundColor = {
        230,
        100,
        90,
        255};

    flexRow->appendChild(flexA);
    flexRow->appendChild(flexB);
    flexRow->appendChild(flexC);

    root->appendChild(flexRow);

    //
    // BLOCK TEST
    //

    auto blockLabel =
        document->createElement("span");

    blockLabel->textContent =
        "Block layout";

    blockLabel->style.width = "auto";
    blockLabel->style.height = "auto";
    blockLabel->style.fontSize = "11px";

    root->appendChild(blockLabel);

    auto blockContainer =
        document->createElement("div");

    blockContainer->style.width = "450px";
    blockContainer->style.height = "auto";
    blockContainer->style.gap = "6px";

    auto blockA =
        document->createElement("div");

    blockA->style.width = "450px";
    blockA->style.height = "24px";

    blockA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto blockB =
        document->createElement("div");

    blockB->style.width = "450px";
    blockB->style.height = "24px";

    blockB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto blockC =
        document->createElement("div");

    blockC->style.width = "450px";
    blockC->style.height = "24px";

    blockC->style.backgroundColor = {
        230,
        100,
        90,
        255};

    blockContainer->appendChild(blockA);
    blockContainer->appendChild(blockB);
    blockContainer->appendChild(blockC);

    root->appendChild(blockContainer);

    //
    // BUTTON
    //

    auto toggle =
        document->createElement("button");

    toggle->textContent =
        "TOGGLE GREEN";

    toggle->style.width = "160px";
    toggle->style.height = "36px";

    toggle->style.backgroundColor = {
        100,
        80,
        220,
        255};

    toggle->style.borderRadius =
        "7px";

    auto hidden =
        new bool(false);

    toggle->onClick =
        [document,
         flexB,
         blockB,
         hidden]()
    {
        *hidden =
            !*hidden;

        flexB->style.display =
            *hidden
                ? "none"
                : "block";

        blockB->style.display =
            *hidden
                ? "none"
                : "block";

        document->update();
    };

    root->appendChild(toggle);

    //
    // FLEX + GROW + NONE
    //

    auto growLabel =
        document->createElement("span");

    growLabel->textContent =
        "Flex-grow + display:none";

    growLabel->style.width = "auto";
    growLabel->style.height = "auto";
    growLabel->style.fontSize = "11px";

    root->appendChild(growLabel);

    auto growRow =
        document->createElement("div");

    growRow->style.width = "450px";
    growRow->style.height = "35px";

    growRow->style.display = "flex";
    growRow->style.flexDirection = "row";
    growRow->style.gap = "6px";

    auto growA =
        document->createElement("div");

    growA->style.width = "20px";
    growA->style.height = "35px";
    growA->style.flexGrow = 1.f;

    growA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto growB =
        document->createElement("div");

    growB->style.width = "20px";
    growB->style.height = "35px";
    growB->style.flexGrow = 1.f;

    growB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto growC =
        document->createElement("div");

    growC->style.width = "20px";
    growC->style.height = "35px";
    growC->style.flexGrow = 1.f;

    growC->style.backgroundColor = {
        230,
        100,
        90,
        255};

    growRow->appendChild(growA);
    growRow->appendChild(growB);
    growRow->appendChild(growC);

    root->appendChild(growRow);

    auto toggleGrow =
        document->createElement("button");

    toggleGrow->textContent =
        "TOGGLE GROW";

    toggleGrow->style.width = "160px";
    toggleGrow->style.height = "36px";

    toggleGrow->style.backgroundColor = {
        180,
        80,
        90,
        255};

    toggleGrow->style.borderRadius =
        "7px";

    auto growHidden =
        new bool(false);

    toggleGrow->onClick =
        [document,
         growB,
         growHidden]()
    {
        *growHidden =
            !*growHidden;

        growB->style.display =
            *growHidden
                ? "none"
                : "block";

        document->update();
    };

    root->appendChild(toggleGrow);

    document->appendChild(root);
    document->render();
}