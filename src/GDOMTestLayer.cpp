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

    //
    // ROOT
    //
    // Важно:
    // testArea будет находиться НЕ в (0, 0).
    // Его позиция зависит от:
    // padding + margin + gap + flex layout.
    //

    auto root =
        document->createElement("div");

    root->style.left = "60px";
    root->style.top = "30px";

    root->style.width = "430px";
    root->style.height = "250px";

    root->style.display = "flex";
    root->style.flexDirection = "column";

    root->style.padding = "15px";
    root->style.gap = "12px";

    root->style.backgroundColor = {
        25,
        28,
        38,
        255};

    root->style.borderRadius =
        "8px";

    //
    // HEADER
    //
    // Нужен специально, чтобы testArea
    // получил ненулевой flowOffset.y.
    //

    auto header =
        document->createElement("div");

    header->style.width = "380px";
    header->style.height = "35px";
    header->style.flexShrink = 0.f;

    header->style.backgroundColor = {
        55,
        60,
        80,
        255};

    header->style.borderRadius =
        "5px";

    root->appendChild(
        header);

    //
    // TEST AREA
    //
    // Именно этот элемент должен локально
    // rerender-иться при изменениях внутри.
    //

    auto testArea =
        document->createElement("div");

    testArea->style.width = "380px";
    testArea->style.height = "100px";

    testArea->style.marginLeft =
        "8px";

    testArea->style.flexShrink =
        0.f;

    testArea->style.display =
        "flex";

    testArea->style.flexDirection =
        "row";

    testArea->style.alignItems =
        "center";

    testArea->style.gap =
        "10px";

    testArea->style.padding =
        "10px";

    testArea->style.backgroundColor = {
        40,
        45,
        60,
        255};

    testArea->style.borderRadius =
        "6px";

    //
    // BOX A
    //

    auto boxA =
        document->createElement("div");

    boxA->style.width = "70px";
    boxA->style.height = "50px";
    boxA->style.flexShrink = 0.f;

    boxA->style.backgroundColor = {
        180,
        80,
        80,
        255};

    boxA->style.borderRadius =
        "5px";

    //
    // BOX B
    //

    auto boxB =
        document->createElement("div");

    boxB->style.width = "70px";
    boxB->style.height = "50px";
    boxB->style.flexShrink = 0.f;

    boxB->style.backgroundColor = {
        80,
        150,
        220,
        255};

    boxB->style.borderRadius =
        "5px";

    testArea->appendChild(
        boxA);

    testArea->appendChild(
        boxB);

    root->appendChild(
        testArea);

    //
    // BUTTON ROW
    //

    auto buttons =
        document->createElement("div");

    buttons->style.width = "390px";
    buttons->style.height = "32px";

    buttons->style.display =
        "flex";

    buttons->style.flexDirection =
        "row";

    buttons->style.gap =
        "5px";

    buttons->style.flexShrink =
        0.f;

    //
    // WIDTH
    //

    auto widthButton =
        document->createElement("button");

    widthButton->textContent =
        "WIDTH";

    widthButton->style.width =
        "75px";

    widthButton->style.height =
        "32px";

    widthButton->style.flexShrink =
        0.f;

    //
    // HEIGHT
    //

    auto heightButton =
        document->createElement("button");

    heightButton->textContent =
        "HEIGHT";

    heightButton->style.width =
        "75px";

    heightButton->style.height =
        "32px";

    heightButton->style.flexShrink =
        0.f;

    //
    // GAP
    //

    auto gapButton =
        document->createElement("button");

    gapButton->textContent =
        "GAP";

    gapButton->style.width =
        "70px";

    gapButton->style.height =
        "32px";

    gapButton->style.flexShrink =
        0.f;

    //
    // ALIGN
    //

    auto alignButton =
        document->createElement("button");

    alignButton->textContent =
        "ALIGN";

    alignButton->style.width =
        "70px";

    alignButton->style.height =
        "32px";

    alignButton->style.flexShrink =
        0.f;

    //
    // RESET
    //

    auto resetButton =
        document->createElement("button");

    resetButton->textContent =
        "RESET";

    resetButton->style.width =
        "75px";

    resetButton->style.height =
        "32px";

    resetButton->style.flexShrink =
        0.f;

    buttons->appendChild(
        widthButton);

    buttons->appendChild(
        heightButton);

    buttons->appendChild(
        gapButton);

    buttons->appendChild(
        alignButton);

    buttons->appendChild(
        resetButton);

    root->appendChild(
        buttons);

    //
    // EVENTS
    //

    bool wide =
        false;

    widthButton->onClick =
        [document,
         boxA,
         &wide]()
    {
        wide =
            !wide;

        boxA->style.width =
            wide
                ? "140px"
                : "70px";

        log::info(
            "PARTIAL TEST: width -> {}",
            wide
                ? 140
                : 70);

        document->update();
    };

    bool tall =
        false;

    heightButton->onClick =
        [document,
         boxA,
         &tall]()
    {
        tall =
            !tall;

        boxA->style.height =
            tall
                ? "75px"
                : "50px";

        log::info(
            "PARTIAL TEST: height -> {}",
            tall
                ? 75
                : 50);

        document->update();
    };

    bool largeGap =
        false;

    gapButton->onClick =
        [document,
         testArea,
         &largeGap]()
    {
        largeGap =
            !largeGap;

        testArea->style.gap =
            largeGap
                ? "35px"
                : "10px";

        log::info(
            "PARTIAL TEST: gap changed");

        document->update();
    };

    bool centered =
        false;

    alignButton->onClick =
        [document,
         testArea,
         &centered]()
    {
        centered =
            !centered;

        testArea->style.justifyContent =
            centered
                ? "center"
                : "flex-start";

        log::info(
            "PARTIAL TEST: justify -> {}",
            centered
                ? "center"
                : "flex-start");

        document->update();
    };

    resetButton->onClick =
        [document,
         boxA,
         testArea,
         &wide,
         &tall,
         &largeGap,
         &centered]()
    {
        wide =
            false;

        tall =
            false;

        largeGap =
            false;

        centered =
            false;

        boxA->style.width =
            "70px";

        boxA->style.height =
            "50px";

        testArea->style.gap =
            "10px";

        testArea->style.justifyContent =
            "flex-start";

        log::info(
            "PARTIAL TEST: reset");

        document->update();
    };

    document->appendChild(
        root);

    document->render();

    log::info(
        "PARTIAL TEST: ready");
}