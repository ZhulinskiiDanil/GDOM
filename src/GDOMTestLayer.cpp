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

    wrapper->style.left = "30px";
    wrapper->style.top = "20px";
    wrapper->style.width = "500px";
    wrapper->style.height = "260px";
    wrapper->style.overflow = "auto";

    wrapper->style.display = "flex";
    wrapper->style.flexDirection = "column";
    wrapper->style.padding = "14px";
    wrapper->style.gap = "10px";

    wrapper->style.backgroundColor = {
        20,
        20,
        26,
        255};

    wrapper->style.borderColor = {
        70,
        70,
        90,
        255};

    wrapper->style.borderWidth = "2px";
    wrapper->style.borderRadius = "12px";

    auto title =
        document->createElement("span");

    title->textContent =
        "GDOM STRESS TEST";

    title->style.width = "auto";
    title->style.height = "auto";
    title->style.fontSize = "20px";

    title->style.color = {
        255,
        255,
        255,
        255};

    wrapper->appendChild(
        title);

    auto status =
        document->createElement("span");

    status->textContent =
        "80 rows / nested flex / overflow / reactive updates";

    status->style.width = "auto";
    status->style.height = "auto";
    status->style.fontSize = "12px";

    status->style.color = {
        150,
        150,
        165,
        255};

    wrapper->appendChild(
        status);

    auto input =
        document->createElement("input");

    input->style.width = "100%";
    input->style.maxWidth = "420px";
    input->style.height = "38px";

    input->style.padding = "10px";

    input->placeholder =
        "Type while the stress test is running";

    input->style.backgroundColor = {
        35,
        35,
        45,
        255};

    input->style.borderColor = {
        80,
        80,
        105,
        255};

    input->style.borderWidth =
        "2px";

    input->style.borderRadius =
        "8px";

    input->style.color = {
        255,
        255,
        255,
        255};

    wrapper->appendChild(
        input);

    auto controls =
        document->createElement("div");

    controls->style.width = "100%";
    controls->style.height = "48px";

    controls->style.display = "flex";
    controls->style.flexDirection = "row";
    controls->style.alignItems = "center";
    controls->style.gap = "8px";

    wrapper->appendChild(
        controls);

    auto mutateButton =
        document->createElement("button");

    mutateButton->textContent =
        "MUTATE ALL";

    mutateButton->style.width = "130px";
    mutateButton->style.height = "36px";

    mutateButton->style.backgroundColor = {
        70,
        100,
        220,
        255};

    mutateButton->style.borderRadius =
        "7px";

    controls->appendChild(
        mutateButton);

    auto paintButton =
        document->createElement("button");

    paintButton->textContent =
        "PAINT ONLY";

    paintButton->style.width = "130px";
    paintButton->style.height = "36px";

    paintButton->style.backgroundColor = {
        160,
        70,
        180,
        255};

    paintButton->style.borderRadius =
        "7px";

    controls->appendChild(
        paintButton);

    std::vector<gdom::HTMLElement *>
        rows;

    std::vector<gdom::HTMLElement *>
        bars;

    std::vector<gdom::HTMLElement *>
        labels;

    for (int i = 0; i < 80; ++i)
    {
        auto row =
            document->createElement("div");

        row->style.width = "100%";
        row->style.maxWidth = "450px";
        row->style.height = "54px";

        row->style.display = "flex";
        row->style.flexDirection = "row";
        row->style.alignItems = "center";
        row->style.gap = "8px";
        row->style.padding = "7px";

        row->style.backgroundColor = {
            static_cast<GLubyte>(
                28 + (i % 3) * 5),
            static_cast<GLubyte>(
                28 + (i % 4) * 4),
            static_cast<GLubyte>(
                36 + (i % 5) * 3),
            255};

        row->style.borderRadius =
            "7px";

        auto index =
            document->createElement("span");

        index->textContent =
            "#" +
            std::to_string(
                i + 1);

        index->style.width = "34px";
        index->style.height = "auto";
        index->style.fontSize = "11px";

        index->style.color = {
            170,
            170,
            185,
            255};

        auto content =
            document->createElement("div");

        content->style.width = "250px";
        content->style.height = "40px";

        content->style.display = "flex";
        content->style.flexDirection = "column";
        content->style.gap = "4px";

        auto label =
            document->createElement("span");

        label->textContent =
            "Stress row " +
            std::to_string(
                i + 1);

        label->style.width = "auto";
        label->style.height = "auto";
        label->style.fontSize = "11px";

        auto track =
            document->createElement("div");

        track->style.width = "220px";
        track->style.height = "8px";

        track->style.backgroundColor = {
            50,
            50,
            65,
            255};

        track->style.borderRadius =
            "4px";

        auto bar =
            document->createElement("div");

        const int initialWidth =
            20 +
            (i % 10) *
                18;

        bar->style.width =
            std::to_string(
                initialWidth) +
            "px";

        bar->style.maxWidth =
            "220px";

        bar->style.height =
            "8px";

        bar->style.backgroundColor = {
            70,
            140,
            255,
            255};

        bar->style.borderRadius =
            "4px";

        track->appendChild(
            bar);

        content->appendChild(
            label);

        content->appendChild(
            track);

        auto badge =
            document->createElement("div");

        badge->style.width = "50px";
        badge->style.height = "28px";

        badge->style.backgroundColor = {
            50,
            55,
            70,
            255};

        badge->style.borderRadius =
            "6px";

        row->appendChild(
            index);

        row->appendChild(
            content);

        row->appendChild(
            badge);

        wrapper->appendChild(
            row);

        rows.push_back(
            row);

        bars.push_back(
            bar);

        labels.push_back(
            label);
    }

    auto endLabel =
        document->createElement("span");

    endLabel->textContent =
        "END OF STRESS TEST";

    endLabel->style.width = "auto";
    endLabel->style.height = "auto";
    endLabel->style.fontSize = "14px";

    endLabel->style.color = {
        100,
        255,
        150,
        255};

    wrapper->appendChild(
        endLabel);

    auto mutated =
        new bool(false);

    auto painted =
        new bool(false);

    mutateButton->onClick =
        [document,
         title,
         status,
         rows,
         bars,
         labels,
         mutated]()
    {
        *mutated =
            !*mutated;

        title->style.fontSize =
            *mutated
                ? "26px"
                : "20px";

        status->textContent =
            *mutated
                ? "MUTATED: layout + text + size"
                : "80 rows / nested flex / overflow / reactive updates";

        for (
            size_t i = 0;
            i < rows.size();
            ++i)
        {
            auto row =
                rows[i];

            auto bar =
                bars[i];

            auto label =
                labels[i];

            if (!row ||
                !bar ||
                !label)
            {
                continue;
            }

            row->style.height =
                *mutated
                    ? (
                          i % 5 == 0
                              ? "72px"
                              : "54px")
                    : "54px";

            const int width =
                *mutated
                    ? 210 -
                          static_cast<int>(
                              i % 10) *
                              14
                    : 20 +
                          static_cast<int>(
                              i % 10) *
                              18;

            bar->style.width =
                std::to_string(
                    width) +
                "px";

            label->style.fontSize =
                *mutated &&
                        i % 7 == 0
                    ? "15px"
                    : "11px";

            if (i % 9 == 0)
            {
                row->style.borderRadius =
                    *mutated
                        ? "16px"
                        : "7px";
            }
        }

        document->update();
    };

    paintButton->onClick =
        [document,
         wrapper,
         title,
         rows,
         bars,
         painted]()
    {
        *painted =
            !*painted;

        wrapper->style.backgroundColor =
            *painted
                ? ccColor4B{
                      32,
                      20,
                      42,
                      255}
                : ccColor4B{20, 20, 26, 255};

        title->style.color =
            *painted
                ? ccColor4B{
                      120,
                      255,
                      170,
                      255}
                : ccColor4B{255, 255, 255, 255};

        for (
            size_t i = 0;
            i < rows.size();
            ++i)
        {
            if (rows[i])
            {
                rows[i]
                    ->style
                    .backgroundColor =
                    *painted
                        ? ccColor4B{
                              static_cast<GLubyte>(
                                  35 +
                                  (i % 4) *
                                      6),
                              25,
                              static_cast<GLubyte>(
                                  45 +
                                  (i % 5) *
                                      5),
                              255}
                        : ccColor4B{static_cast<GLubyte>(28 + (i % 3) * 5), static_cast<GLubyte>(28 + (i % 4) * 4), static_cast<GLubyte>(36 + (i % 5) * 3), 255};
            }

            if (bars[i])
            {
                bars[i]
                    ->style
                    .backgroundColor =
                    *painted
                        ? ccColor4B{
                              220,
                              80,
                              160,
                              255}
                        : ccColor4B{70, 140, 255, 255};
            }
        }

        document->update();
    };

    document->appendChild(
        wrapper);

    document->render();
}