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
        log::error("REGRESSION: document failed");
        return;
    }

    auto resized =
        std::make_shared<bool>(false);

    auto painted =
        std::make_shared<bool>(false);

    auto hidden =
        std::make_shared<bool>(false);

    //
    // ROOT
    //

    auto root =
        document->createElement("div");

    root->style.left = "55px";
    root->style.top = "25px";

    root->style.width =
        "calc(100% - 110px)";

    root->style.height =
        "calc(100% - 50px)";

    root->style.display =
        "flex";

    root->style.flexDirection =
        "column";

    root->style.padding =
        "12px";

    root->style.gap =
        "8px";

    root->style.backgroundColor = {
        25,
        28,
        38,
        255};

    root->style.borderRadius =
        "8px";

    //
    // TITLE
    //

    auto title =
        document->createElement("span");

    title->textContent =
        "GDOM v0.1 REGRESSION";

    title->style.width =
        "auto";

    title->style.height =
        "auto";

    title->style.fontSize =
        "12px";

    title->style.flexShrink =
        0.f;

    root->appendChild(
        title);

    //
    // FLEX GROW TEST
    //

    auto growRow =
        document->createElement("div");

    growRow->style.width =
        "100%";

    growRow->style.height =
        "38px";

    growRow->style.display =
        "flex";

    growRow->style.flexDirection =
        "row";

    growRow->style.gap =
        "5px";

    growRow->style.flexShrink =
        0.f;

    auto growA =
        document->createElement("div");

    growA->style.width =
        "20px";

    growA->style.height =
        "38px";

    growA->style.flexGrow =
        1.f;

    growA->style.backgroundColor = {
        170,
        70,
        70,
        255};

    auto growB =
        document->createElement("div");

    growB->style.width =
        "20px";

    growB->style.height =
        "38px";

    growB->style.flexGrow =
        2.f;

    growB->style.backgroundColor = {
        70,
        150,
        210,
        255};

    auto growC =
        document->createElement("div");

    growC->style.width =
        "20px";

    growC->style.height =
        "38px";

    growC->style.flexGrow =
        1.f;

    growC->style.backgroundColor = {
        80,
        180,
        110,
        255};

    growRow->appendChild(growA);
    growRow->appendChild(growB);
    growRow->appendChild(growC);

    root->appendChild(
        growRow);

    //
    // PARTIAL RERENDER AREA
    //

    auto testArea =
        document->createElement("div");

    testArea->style.width =
        "100%";

    testArea->style.height =
        "70px";

    testArea->style.display =
        "flex";

    testArea->style.flexDirection =
        "row";

    testArea->style.alignItems =
        "center";

    testArea->style.padding =
        "8px";

    testArea->style.gap =
        "10px";

    testArea->style.flexShrink =
        0.f;

    testArea->style.backgroundColor = {
        40,
        44,
        58,
        255};

    testArea->style.borderRadius =
        "6px";

    auto dynamicBox =
        document->createElement("div");

    dynamicBox->style.width =
        "60px";

    dynamicBox->style.height =
        "45px";

    dynamicBox->style.flexShrink =
        0.f;

    dynamicBox->style.backgroundColor = {
        190,
        120,
        60,
        255};

    dynamicBox->style.borderRadius =
        "5px";

    auto hideBox =
        document->createElement("div");

    hideBox->style.width =
        "60px";

    hideBox->style.height =
        "45px";

    hideBox->style.flexShrink =
        0.f;

    hideBox->style.backgroundColor = {
        120,
        80,
        190,
        255};

    hideBox->style.borderRadius =
        "5px";

    testArea->appendChild(
        dynamicBox);

    testArea->appendChild(
        hideBox);

    root->appendChild(
        testArea);

    //
    // INPUT
    //

    auto input =
        document->createElement("input");

    input->placeholder =
        "Input regression test";

    input->style.width =
        "280px";

    input->style.height =
        "34px";

    input->style.paddingLeft =
        "8px";

    input->style.paddingRight =
        "8px";

    input->style.fontSize =
        "10px";

    input->style.flexShrink =
        0.f;

    input->style.backgroundColor = {
        50,
        55,
        70,
        255};

    input->style.borderRadius =
        "5px";

    input->onFocus =
        []()
    {
        log::info(
            "REGRESSION: INPUT FOCUS");
    };

    input->onBlur =
        []()
    {
        log::info(
            "REGRESSION: INPUT BLUR");
    };

    input->onInput =
        [](
            const std::string &value)
    {
        log::info(
            "REGRESSION: INPUT '{}'",
            value);
    };

    root->appendChild(
        input);

    //
    // BUTTONS
    //

    auto buttons =
        document->createElement("div");

    buttons->style.width =
        "100%";

    buttons->style.height =
        "32px";

    buttons->style.display =
        "flex";

    buttons->style.flexDirection =
        "row";

    buttons->style.gap =
        "5px";

    buttons->style.flexShrink =
        0.f;

    auto resizeButton =
        document->createElement("button");

    resizeButton->textContent =
        "RESIZE";

    resizeButton->style.width =
        "80px";

    resizeButton->style.height =
        "32px";

    resizeButton->style.flexShrink =
        0.f;

    auto hideButton =
        document->createElement("button");

    hideButton->textContent =
        "HIDE";

    hideButton->style.width =
        "70px";

    hideButton->style.height =
        "32px";

    hideButton->style.flexShrink =
        0.f;

    auto paintButton =
        document->createElement("button");

    paintButton->textContent =
        "PAINT";

    paintButton->style.width =
        "75px";

    paintButton->style.height =
        "32px";

    paintButton->style.flexShrink =
        0.f;

    auto focusButton =
        document->createElement("button");

    focusButton->textContent =
        "FOCUS";

    focusButton->style.width =
        "75px";

    focusButton->style.height =
        "32px";

    focusButton->style.flexShrink =
        0.f;

    buttons->appendChild(
        resizeButton);

    buttons->appendChild(
        hideButton);

    buttons->appendChild(
        paintButton);

    buttons->appendChild(
        focusButton);

    root->appendChild(
        buttons);

    //
    // EVENTS
    //

    resizeButton->onClick =
        [document,
         dynamicBox,
         resized]()
    {
        *resized =
            !*resized;

        dynamicBox->style.width =
            *resized
                ? "130px"
                : "60px";

        dynamicBox->style.height =
            *resized
                ? "55px"
                : "45px";

        document->update();

        log::info(
            "REGRESSION: RESIZE {}",
            *resized
                ? "LARGE"
                : "SMALL");
    };

    hideButton->onClick =
        [document,
         hideBox,
         hidden]()
    {
        *hidden =
            !*hidden;

        hideBox->style.display =
            *hidden
                ? "none"
                : "block";

        document->update();

        log::info(
            "REGRESSION: DISPLAY {}",
            *hidden
                ? "NONE"
                : "BLOCK");
    };

    paintButton->onClick =
        [document,
         dynamicBox,
         painted]()
    {
        *painted =
            !*painted;

        dynamicBox->style.backgroundColor =
            *painted
                ? ccColor4B{
                      220,
                      90,
                      90,
                      255}
                : ccColor4B{80, 190, 120, 255};

        document->update();

        log::info(
            "REGRESSION: PAINT {}",
            *painted
                ? "RED"
                : "GREEN");
    };

    focusButton->onClick =
        [input,
         document]()
    {
        const bool result =
            input->focus();

        log::info(
            "REGRESSION: FOCUS {} / tracked {}",
            result,
            document->getFocusedElement() ==
                input);
    };

    //
    // RENDER
    //

    document->appendChild(
        root);

    document->render();

    log::info(
        "REGRESSION: READY");
}