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
    root->style.top = "15px";
    root->style.width = "520px";
    root->style.height = "270px";

    root->style.display = "flex";
    root->style.flexDirection = "column";
    root->style.gap = "4px";
    root->style.padding = "8px";

    root->style.backgroundColor = {
        20,
        20,
        26,
        255};

    auto title =
        document->createElement("span");

    title->textContent =
        "LENGTH RESOLVER TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "14px";
    title->style.flexShrink = 0.f;

    root->appendChild(title);

    auto createTest =
        [document,
         root](
            const std::string &labelText,
            const std::string &width,
            ccColor4B color)
    {
        auto row =
            document->createElement("div");

        row->style.width = "500px";
        row->style.height = "16px";
        row->style.flexShrink = 0.f;

        row->style.display = "flex";
        row->style.flexDirection = "row";
        row->style.alignItems = "center";
        row->style.gap = "4px";

        auto label =
            document->createElement("span");

        label->textContent =
            labelText;

        label->style.width = "155px";
        label->style.height = "auto";
        label->style.fontSize = "7px";
        label->style.flexShrink = 0.f;

        auto viewport =
            document->createElement("div");

        viewport->style.width = "330px";
        viewport->style.height = "12px";
        viewport->style.flexShrink = 0.f;

        viewport->style.backgroundColor = {
            35,
            35,
            45,
            255};

        auto block =
            document->createElement("div");

        block->style.width =
            width;

        block->style.height =
            "12px";

        block->style.flexShrink = 0.f;

        block->style.backgroundColor =
            color;

        viewport->appendChild(block);

        row->appendChild(label);
        row->appendChild(viewport);

        root->appendChild(row);
    };

    createTest(
        "100px",
        "100px",
        {70, 130, 255, 255});

    createTest(
        "50%",
        "50%",
        {80, 210, 130, 255});

    createTest(
        "2rem",
        "2rem",
        {230, 100, 90, 255});

    createTest(
        "unitless 100",
        "100",
        {180, 100, 220, 255});

    createTest(
        "calc(100%-20px)",
        "calc(100% - 20px)",
        {220, 150, 60, 255});

    createTest(
        "calc(50%+20px)",
        "calc(50% + 20px)",
        {60, 180, 210, 255});

    createTest(
        "calc(-20px+50%)",
        "calc(-20px + 50%)",
        {200, 90, 160, 255});

    createTest(
        "100%-20px-10px",
        "calc(100% - 20px - 10px)",
        {100, 180, 100, 255});

    createTest(
        "100%+10px-2rem+5px",
        "calc(100% + 10px - 2rem + 5px)",
        {210, 120, 80, 255});

    createTest(
        "100% - -20px",
        "calc(100% - -20px)",
        {100, 140, 230, 255});

    createTest(
        "100% + (-20px)",
        "calc(100% + (-20px))",
        {200, 110, 190, 255});

    createTest(
        "nested calc",
        "calc(100% - calc(20px + 10px))",
        {100, 210, 180, 255});

    createTest(
        "CALC + PX uppercase",
        "CALC(100% - 20PX)",
        {230, 170, 90, 255});

    createTest(
        "Calc with spaces",
        "Calc ( 100% - 20px )",
        {140, 110, 230, 255});

    //
    // Invalid expressions
    //
    // These should resolve to 0 and produce warnings.
    //

    createTest(
        "INVALID: abc",
        "calc(100% - abc)",
        {255, 70, 70, 255});

    createTest(
        "INVALID: missing rhs",
        "calc(100% -)",
        {255, 70, 70, 255});

    createTest(
        "INVALID: vw",
        "calc(100% + 20vw)",
        {255, 70, 70, 255});

    createTest(
        "INVALID: multiply",
        "calc(100% * 2)",
        {255, 70, 70, 255});

    document->appendChild(root);
    document->render();
}