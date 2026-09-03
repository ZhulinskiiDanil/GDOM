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
    root->style.height = "280px";

    root->style.display = "flex";
    root->style.flexDirection = "column";
    root->style.padding = "14px";
    root->style.gap = "12px";

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
        "FLEX GROW / SHRINK TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "18px";

    root->appendChild(title);

    //
    // TEST 1
    //
    // flexGrow 1 : 2 : 1
    //

    auto growLabel =
        document->createElement("span");

    growLabel->textContent =
        "Grow: 1 / 2 / 1";

    growLabel->style.width = "auto";
    growLabel->style.height = "auto";
    growLabel->style.fontSize = "11px";

    root->appendChild(growLabel);

    auto growRow =
        document->createElement("div");

    growRow->style.width = "450px";
    growRow->style.height = "45px";

    growRow->style.display = "flex";
    growRow->style.flexDirection = "row";
    growRow->style.gap = "6px";

    growRow->style.backgroundColor = {
        35,
        35,
        45,
        255};

    growRow->style.borderRadius =
        "7px";

    auto growA =
        document->createElement("div");

    growA->style.width = "30px";
    growA->style.height = "45px";
    growA->style.flexGrow = 1.f;

    growA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto growB =
        document->createElement("div");

    growB->style.width = "30px";
    growB->style.height = "45px";
    growB->style.flexGrow = 2.f;

    growB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto growC =
        document->createElement("div");

    growC->style.width = "30px";
    growC->style.height = "45px";
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

    //
    // TEST 2
    //
    // Shrink.
    //
    // 200 + 200 + 200 > 450
    //

    auto shrinkLabel =
        document->createElement("span");

    shrinkLabel->textContent =
        "Shrink: 1 / 0 / 2";

    shrinkLabel->style.width = "auto";
    shrinkLabel->style.height = "auto";
    shrinkLabel->style.fontSize = "11px";

    root->appendChild(shrinkLabel);

    auto shrinkRow =
        document->createElement("div");

    shrinkRow->style.width = "450px";
    shrinkRow->style.height = "45px";

    shrinkRow->style.display = "flex";
    shrinkRow->style.flexDirection = "row";
    shrinkRow->style.gap = "6px";

    shrinkRow->style.backgroundColor = {
        35,
        35,
        45,
        255};

    shrinkRow->style.borderRadius =
        "7px";

    auto shrinkA =
        document->createElement("div");

    shrinkA->style.width = "200px";
    shrinkA->style.height = "45px";
    shrinkA->style.flexShrink = 1.f;

    shrinkA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto shrinkB =
        document->createElement("div");

    shrinkB->style.width = "200px";
    shrinkB->style.height = "45px";

    shrinkB->style.flexShrink = 0.f;

    shrinkB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto shrinkC =
        document->createElement("div");

    shrinkC->style.width = "200px";
    shrinkC->style.height = "45px";

    shrinkC->style.flexShrink = 2.f;

    shrinkC->style.backgroundColor = {
        230,
        100,
        90,
        255};

    shrinkRow->appendChild(shrinkA);
    shrinkRow->appendChild(shrinkB);
    shrinkRow->appendChild(shrinkC);

    root->appendChild(shrinkRow);

    //
    // TEST 3
    //
    // Grow + maxWidth.
    //

    auto limitLabel =
        document->createElement("span");

    limitLabel->textContent =
        "Grow + maxWidth";

    limitLabel->style.width = "auto";
    limitLabel->style.height = "auto";
    limitLabel->style.fontSize = "11px";

    root->appendChild(limitLabel);

    auto limitRow =
        document->createElement("div");

    limitRow->style.width = "450px";
    limitRow->style.height = "45px";

    limitRow->style.display = "flex";
    limitRow->style.flexDirection = "row";
    limitRow->style.gap = "6px";

    limitRow->style.backgroundColor = {
        35,
        35,
        45,
        255};

    auto limitA =
        document->createElement("div");

    limitA->style.width = "30px";
    limitA->style.height = "45px";
    limitA->style.flexGrow = 1.f;
    limitA->style.maxWidth = "100px";

    limitA->style.backgroundColor = {
        70,
        130,
        255,
        255};

    auto limitB =
        document->createElement("div");

    limitB->style.width = "30px";
    limitB->style.height = "45px";
    limitB->style.flexGrow = 1.f;

    limitB->style.backgroundColor = {
        80,
        210,
        130,
        255};

    auto limitC =
        document->createElement("div");

    limitC->style.width = "30px";
    limitC->style.height = "45px";
    limitC->style.flexGrow = 1.f;

    limitC->style.backgroundColor = {
        230,
        100,
        90,
        255};

    limitRow->appendChild(limitA);
    limitRow->appendChild(limitB);
    limitRow->appendChild(limitC);

    root->appendChild(limitRow);

    //
    // Dynamic test
    //

    auto button =
        document->createElement("button");

    button->textContent =
        "SWAP FLEX";

    button->style.width = "140px";
    button->style.height = "36px";

    button->style.backgroundColor = {
        100,
        80,
        220,
        255};

    button->style.borderRadius =
        "7px";

    auto changed =
        new bool(false);

    button->onClick =
        [document,
         growA,
         growB,
         growC,
         shrinkA,
         shrinkB,
         shrinkC,
         changed]()
    {
        *changed =
            !*changed;

        if (*changed)
        {
            growA->style.flexGrow = 3.f;
            growB->style.flexGrow = 1.f;
            growC->style.flexGrow = 1.f;

            shrinkA->style.flexShrink = 0.f;
            shrinkB->style.flexShrink = 1.f;
            shrinkC->style.flexShrink = 3.f;
        }
        else
        {
            growA->style.flexGrow = 1.f;
            growB->style.flexGrow = 2.f;
            growC->style.flexGrow = 1.f;

            shrinkA->style.flexShrink = 1.f;
            shrinkB->style.flexShrink = 0.f;
            shrinkC->style.flexShrink = 2.f;
        }

        document->update();
    };

    root->appendChild(button);

    document->appendChild(root);
    document->render();
}