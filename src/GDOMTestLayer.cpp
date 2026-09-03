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
            "TEST: document create failed");

        return;
    }

    log::info(
        "TEST: document created");

    auto root =
        document->createElement("div");

    if (!root)
    {
        log::error(
            "TEST: root create failed");

        return;
    }

    root->style.left = "70px";
    root->style.top = "30px";

    root->style.width = "360px";
    root->style.height = "210px";

    root->style.display = "flex";
    root->style.flexDirection = "column";

    root->style.padding = "10px";
    root->style.gap = "8px";

    root->style.backgroundColor = {
        35,
        38,
        50,
        255};

    root->style.borderRadius =
        "8px";

    //
    // INPUT A
    //

    auto inputA =
        document->createElement("input");

    if (!inputA)
    {
        log::error(
            "TEST: input A create failed");

        return;
    }

    inputA->placeholder =
        "Input A";

    inputA->style.width = "300px";
    inputA->style.height = "36px";

    inputA->style.paddingLeft = "8px";
    inputA->style.paddingRight = "8px";

    inputA->style.fontSize = "11px";
    inputA->style.flexShrink = 0.f;

    inputA->style.backgroundColor = {
        55,
        60,
        75,
        255};

    inputA->style.borderRadius =
        "5px";

    //
    // INPUT B
    //

    auto inputB =
        document->createElement("input");

    if (!inputB)
    {
        log::error(
            "TEST: input B create failed");

        return;
    }

    inputB->placeholder =
        "Input B";

    inputB->style.width = "300px";
    inputB->style.height = "36px";

    inputB->style.paddingLeft = "8px";
    inputB->style.paddingRight = "8px";

    inputB->style.fontSize = "11px";
    inputB->style.flexShrink = 0.f;

    inputB->style.backgroundColor = {
        55,
        60,
        75,
        255};

    inputB->style.borderRadius =
        "5px";

    //
    // BUTTONS
    //

    auto row =
        document->createElement("div");

    row->style.width = "300px";
    row->style.height = "34px";

    row->style.display = "flex";
    row->style.flexDirection = "row";

    row->style.gap = "6px";
    row->style.flexShrink = 0.f;

    auto focusA =
        document->createElement("button");

    focusA->textContent =
        "FOCUS A";

    focusA->style.width = "90px";
    focusA->style.height = "34px";
    focusA->style.flexShrink = 0.f;

    auto focusB =
        document->createElement("button");

    focusB->textContent =
        "FOCUS B";

    focusB->style.width = "90px";
    focusB->style.height = "34px";
    focusB->style.flexShrink = 0.f;

    auto blur =
        document->createElement("button");

    blur->textContent =
        "BLUR";

    blur->style.width = "80px";
    blur->style.height = "34px";
    blur->style.flexShrink = 0.f;

    //
    // EVENTS
    //

    inputA->onFocus =
        [document, inputA]()
    {
        log::info(
            "TEST: A FOCUS");

        log::info(
            "TEST: focused == A: {}",
            document->getFocusedElement() ==
                inputA);
    };

    inputA->onBlur =
        []()
    {
        log::info(
            "TEST: A BLUR");
    };

    inputB->onFocus =
        [document, inputB]()
    {
        log::info(
            "TEST: B FOCUS");

        log::info(
            "TEST: focused == B: {}",
            document->getFocusedElement() ==
                inputB);
    };

    inputB->onBlur =
        []()
    {
        log::info(
            "TEST: B BLUR");
    };

    inputA->onInput =
        [](
            const std::string &value)
    {
        log::info(
            "TEST: A INPUT '{}'",
            value);
    };

    inputB->onInput =
        [](
            const std::string &value)
    {
        log::info(
            "TEST: B INPUT '{}'",
            value);
    };

    focusA->onClick =
        [inputA]()
    {
        log::info(
            "TEST: calling A.focus()");

        const bool result =
            inputA->focus();

        log::info(
            "TEST: A.focus() = {}",
            result);
    };

    focusB->onClick =
        [inputB]()
    {
        log::info(
            "TEST: calling B.focus()");

        const bool result =
            inputB->focus();

        log::info(
            "TEST: B.focus() = {}",
            result);
    };

    blur->onClick =
        [document]()
    {
        log::info(
            "TEST: calling blur");

        auto *focused =
            document->getFocusedElement();

        if (focused)
        {
            focused->blur();
        }
    };

    //
    // TREE
    //

    row->appendChild(
        focusA);

    row->appendChild(
        focusB);

    row->appendChild(
        blur);

    root->appendChild(
        inputA);

    root->appendChild(
        inputB);

    root->appendChild(
        row);

    document->appendChild(
        root);

    log::info(
        "TEST: rendering");

    document->render();

    log::info(
        "TEST: rendered");

    log::info(
        "TEST: root mounted = {}",
        root->isMounted());

    log::info(
        "TEST: input A mounted = {}",
        inputA->isMounted());

    log::info(
        "TEST: input B mounted = {}",
        inputB->isMounted());
}