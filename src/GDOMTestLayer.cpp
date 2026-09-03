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
  // ------------------------------------------------------------
  // Helpers
  // ------------------------------------------------------------
  //

  auto text =
      [document](
          const std::string &value,
          const std::string &fontSize,
          ccColor4B color)
  {
    auto span =
        document->createElement("span");

    span->textContent =
        value;

    span->style.width =
        "auto";

    span->style.height =
        "auto";

    span->style.fontSize =
        fontSize;

    span->style.color =
        color;

    return span;
  };

  auto makeStatRow =
      [document, &text](
          const std::string &label,
          const std::string &value)
  {
    auto row =
        document->createElement("div");

    row->style.width =
        "100%";

    row->style.height =
        "auto";

    row->style.display =
        "flex";

    row->style.flexDirection =
        "row";

    row->style.justifyContent =
        "space-between";

    row->style.alignItems =
        "center";

    row->appendChild(
        text(
            label,
            "7px",
            {255,
             255,
             255,
             255}));

    row->appendChild(
        text(
            value,
            "6px",
            {220,
             220,
             220,
             255}));

    return row;
  };

  auto makeProgress =
      [document](
          const std::string &fillWidth,
          bool completed)
  {
    auto track =
        document->createElement("div");

    track->style.width =
        "100%";

    track->style.height =
        "4px";

    track->style.backgroundColor = {
        65,
        65,
        65,
        255};

    track->style.borderRadius =
        "2px";

    auto fill =
        document->createElement("div");

    fill->style.width =
        fillWidth;

    fill->style.height =
        "100%";

    fill->style.backgroundColor =
        completed
            ? ccColor4B{
                  82,
                  235,
                  75,
                  255}
            : ccColor4B{185, 185, 185, 255};

    fill->style.borderRadius =
        "2px";

    track->appendChild(
        fill);

    return track;
  };

  //
  // ------------------------------------------------------------
  // Range card
  // ------------------------------------------------------------
  //

  auto makeRangeCard =
      [document,
       &text,
       &makeStatRow,
       &makeProgress](
          const std::string &range,
          bool completed,
          const std::string &progress,
          const std::string &activity,
          const std::string &firstRun,
          const std::string &bestRun,
          const std::string &completion)
  {
    auto card =
        document->createElement("div");

    card->style.width =
        "198px";

    card->style.height =
        "88px";

    card->style.display =
        "flex";

    card->style.flexDirection =
        "column";

    card->style.padding =
        "6px";

    card->style.gap =
        "3px";

    card->style.backgroundColor =
        completed
            ? ccColor4B{
                  20,
                  47,
                  24,
                  250}
            : ccColor4B{31, 31, 31, 250};

    card->style.borderColor =
        completed
            ? ccColor4B{
                  64,
                  170,
                  63,
                  255}
            : ccColor4B{72, 72, 72, 255};

    card->style.borderWidth =
        "1px";

    card->style.borderRadius =
        "5px";

    //
    // Header
    //

    auto header =
        document->createElement("div");

    header->style.width =
        "100%";

    header->style.height =
        "18px";

    header->style.display =
        "flex";

    header->style.flexDirection =
        "row";

    header->style.alignItems =
        "center";

    header->style.gap =
        "5px";

    //
    // Checkbox
    //

    auto checkbox =
        document->createElement("div");

    checkbox->style.width =
        "15px";

    checkbox->style.height =
        "15px";

    checkbox->style.backgroundColor =
        completed
            ? ccColor4B{
                  100,
                  220,
                  65,
                  255}
            : ccColor4B{150, 150, 150, 255};

    checkbox->style.borderColor = {
        235,
        235,
        235,
        255};

    checkbox->style.borderWidth =
        "1px";

    checkbox->style.borderRadius =
        "2px";

    //
    // Range title
    //

    auto rangeTitle =
        text(
            range,
            "10px",
            completed
                ? ccColor4B{
                      80,
                      240,
                      90,
                      255}
                : ccColor4B{235, 170, 120, 255});

    //
    // Spacer
    //
    // У нас пока нет flex-grow,
    // поэтому резервируем место вручную.
    //

    auto spacer =
        document->createElement("div");

    spacer->style.width =
        "35px";

    spacer->style.height =
        "1px";

    //
    // Collapse button
    //

    auto collapse =
        document->createElement("button");

    collapse->textContent =
        "^";

    collapse->style.width =
        "17px";

    collapse->style.height =
        "17px";

    collapse->style.fontSize =
        "8px";

    collapse->style.backgroundColor = {
        205,
        130,
        225,
        255};

    collapse->style.borderColor = {
        255,
        255,
        255,
        255};

    collapse->style.borderWidth =
        "1px";

    collapse->style.borderRadius =
        "8px";

    collapse->onClick =
        [range]()
    {
      log::info(
          "Collapse: {}",
          range);
    };

    header->appendChild(
        checkbox);

    header->appendChild(
        rangeTitle);

    header->appendChild(
        spacer);

    header->appendChild(
        collapse);

    card->appendChild(
        header);

    //
    // Progress
    //

    card->appendChild(
        makeProgress(
            progress,
            completed));

    //
    // Stats
    //

    card->appendChild(
        makeStatRow(
            "ACTIVITY:",
            activity));

    card->appendChild(
        makeStatRow(
            "FIRST RUN:",
            firstRun));

    card->appendChild(
        makeStatRow(
            "BEST RUN:",
            bestRun));

    card->appendChild(
        makeStatRow(
            "COMPLETION:",
            completion));

    return card;
  };

  //
  // ------------------------------------------------------------
  // Floating / side button helper
  // ------------------------------------------------------------
  //

  auto makeRoundButton =
      [document](
          const std::string &label,
          const std::string &size,
          ccColor4B background,
          std::function<void()> callback)
  {
    auto button =
        document->createElement("button");

    button->textContent =
        label;

    button->style.width =
        size;

    button->style.height =
        size;

    button->style.fontSize =
        "8px";

    button->style.backgroundColor =
        background;

    button->style.borderColor = {
        235,
        235,
        235,
        255};

    button->style.borderWidth =
        "2px";

    button->style.borderRadius =
        "50%";

    button->onClick =
        std::move(callback);

    return button;
  };

  //
  // ------------------------------------------------------------
  // Main root
  // ------------------------------------------------------------
  //

  auto root =
      document->createElement("div");

  root->style.left =
      "72px";

  root->style.top =
      "18px";

  root->style.width =
      "430px";

  root->style.height =
      "auto";

  root->style.display =
      "flex";

  root->style.flexDirection =
      "column";

  //
  // ------------------------------------------------------------
  // Tabs
  // ------------------------------------------------------------
  //

  auto tabs =
      document->createElement("div");

  tabs->style.width =
      "100%";

  tabs->style.height =
      "36px";

  tabs->style.display =
      "flex";

  tabs->style.flexDirection =
      "row";

  tabs->style.gap =
      "4px";

  auto makeTab =
      [document](
          const std::string &label,
          bool active)
  {
    auto tab =
        document->createElement("button");

    tab->textContent =
        label;

    tab->style.width =
        "104px";

    tab->style.height =
        "36px";

    tab->style.fontSize =
        "8px";

    tab->style.backgroundColor =
        active
            ? ccColor4B{
                  170,
                  84,
                  40,
                  255}
            : ccColor4B{70, 70, 70, 255};

    tab->style.borderColor = {
        245,
        245,
        245,
        255};

    tab->style.borderWidth =
        "2px";

    tab->style.borderRadius =
        "9px";

    tab->onClick =
        [label]()
    {
      log::info(
          "Tab: {}",
          label);
    };

    return tab;
  };

  tabs->appendChild(
      makeTab(
          "PROFILES",
          true));

  tabs->appendChild(
      makeTab(
          "STAGE BROWSER",
          false));

  tabs->appendChild(
      makeTab(
          "STAGE GRAPH",
          true));

  tabs->appendChild(
      makeTab(
          "NEWS",
          true));

  root->appendChild(
      tabs);

  //
  // ------------------------------------------------------------
  // Main panel
  // ------------------------------------------------------------
  //

  auto panel =
      document->createElement("div");

  panel->style.width =
      "100%";

  panel->style.height =
      "245px";

  panel->style.display =
      "flex";

  panel->style.flexDirection =
      "column";

  panel->style.padding =
      "10px";

  panel->style.gap =
      "6px";

  panel->style.backgroundColor = {
      24,
      24,
      24,
      248};

  panel->style.borderColor = {
      225,
      225,
      225,
      255};

  panel->style.borderWidth =
      "2px";

  panel->style.borderRadius =
      "7px";

  //
  // ------------------------------------------------------------
  // Stage header
  // ------------------------------------------------------------
  //

  auto stageTop =
      document->createElement("div");

  stageTop->style.width =
      "100%";

  stageTop->style.height =
      "48px";

  stageTop->style.display =
      "flex";

  stageTop->style.flexDirection =
      "row";

  stageTop->style.alignItems =
      "center";

  stageTop->style.gap =
      "6px";

  //
  // Left title block
  //

  auto titleArea =
      document->createElement("div");

  titleArea->style.width =
      "340px";

  titleArea->style.height =
      "auto";

  titleArea->style.display =
      "flex";

  titleArea->style.flexDirection =
      "column";

  titleArea->style.gap =
      "2px";

  titleArea->appendChild(
      text(
          "STAGE: 1/5",
          "18px",
          {255,
           185,
           25,
           255}));

  auto meta =
      document->createElement("div");

  meta->style.width =
      "auto";

  meta->style.height =
      "auto";

  meta->style.display =
      "flex";

  meta->style.flexDirection =
      "row";

  meta->style.gap =
      "10px";

  meta->appendChild(
      text(
          "6 ATTEMPTS",
          "8px",
          {245,
           245,
           245,
           255}));

  meta->appendChild(
      text(
          "5s",
          "8px",
          {245,
           245,
           245,
           255}));

  titleArea->appendChild(
      meta);

  //
  // Right header buttons
  //

  auto headerActions =
      document->createElement("div");

  headerActions->style.width =
      "62px";

  headerActions->style.height =
      "28px";

  headerActions->style.display =
      "flex";

  headerActions->style.flexDirection =
      "row";

  headerActions->style.gap =
      "6px";

  auto completeStage =
      document->createElement("button");

  completeStage->textContent =
      "OK";

  completeStage->style.width =
      "28px";

  completeStage->style.height =
      "28px";

  completeStage->style.fontSize =
      "9px";

  completeStage->style.backgroundColor = {
      80,
      205,
      35,
      255};

  completeStage->style.borderColor = {
      255,
      255,
      255,
      255};

  completeStage->style.borderWidth =
      "1px";

  completeStage->style.borderRadius =
      "4px";

  completeStage->onClick =
      []()
  {
    log::info(
        "Complete current stage");
  };

  auto stageList =
      document->createElement("button");

  stageList->textContent =
      "LIST";

  stageList->style.width =
      "28px";

  stageList->style.height =
      "28px";

  stageList->style.fontSize =
      "6px";

  stageList->style.backgroundColor = {
      75,
      190,
      55,
      255};

  stageList->style.borderColor = {
      255,
      255,
      255,
      255};

  stageList->style.borderWidth =
      "1px";

  stageList->style.borderRadius =
      "4px";

  stageList->onClick =
      []()
  {
    log::info(
        "Open stage list");
  };

  headerActions->appendChild(
      completeStage);

  headerActions->appendChild(
      stageList);

  stageTop->appendChild(
      titleArea);

  stageTop->appendChild(
      headerActions);

  panel->appendChild(
      stageTop);

  //
  // ------------------------------------------------------------
  // Range area
  //
  // Сейчас это обычный layout.
  //
  // Когда добавим overflow/scroll:
  //
  // ranges->style.overflow = "scroll";
  //
  // Именно этот контейнер должен стать scroll viewport.
  // ------------------------------------------------------------
  //

  auto ranges =
      document->createElement("div");

  ranges->style.width =
      "100%";

  ranges->style.height =
      "180px";

  ranges->style.display =
      "flex";

  ranges->style.flexDirection =
      "column";

  ranges->style.gap =
      "6px";

  auto firstRow =
      document->createElement("div");

  firstRow->style.width =
      "100%";

  firstRow->style.height =
      "93px";

  firstRow->style.display =
      "flex";

  firstRow->style.flexDirection =
      "row";

  firstRow->style.gap =
      "6px";

  firstRow->appendChild(
      makeRangeCard(
          "0.00% - 14.59%",
          true,
          "28%",
          "5 ATTEMPTS / 3s",
          "0.00% - 28.77%",
          "0.00% - 28.77%",
          "1 ATTEMPT / 1 PASS"));

  firstRow->appendChild(
      makeRangeCard(
          "14.59% - 30.81%",
          true,
          "36%",
          "1 ATTEMPT / 2s",
          "0.00% - 100.00%",
          "0.00% - 100.00%",
          "1 ATTEMPT / 1 PASS"));

  auto secondRow =
      document->createElement("div");

  secondRow->style.width =
      "100%";

  secondRow->style.height =
      "93px";

  secondRow->style.display =
      "flex";

  secondRow->style.flexDirection =
      "row";

  secondRow->style.gap =
      "6px";

  secondRow->appendChild(
      makeRangeCard(
          "30.81% - 47.03%",
          false,
          "17%",
          "0 ATTEMPTS / 0s",
          "NONE",
          "NONE",
          "NOT COMPLETED"));

  secondRow->appendChild(
      makeRangeCard(
          "47.03% - 63.24%",
          false,
          "17%",
          "0 ATTEMPTS / 0s",
          "NONE",
          "NONE",
          "NOT COMPLETED"));

  ranges->appendChild(
      firstRow);

  ranges->appendChild(
      secondRow);

  panel->appendChild(
      ranges);

  root->appendChild(
      panel);

  document->appendChild(
      root);

  //
  // ------------------------------------------------------------
  // Left side controls
  // ------------------------------------------------------------
  //

  auto leftControls =
      document->createElement("div");

  leftControls->style.left =
      "22px";

  leftControls->style.top =
      "38px";

  leftControls->style.width =
      "34px";

  leftControls->style.height =
      "auto";

  leftControls->style.display =
      "flex";

  leftControls->style.flexDirection =
      "column";

  leftControls->style.gap =
      "8px";

  leftControls->appendChild(
      makeRoundButton(
          "V",
          "30px",
          {70,
           130,
           40,
           255},
          []()
          {
            log::info(
                "Toggle completed");
          }));

  leftControls->appendChild(
      makeRoundButton(
          "X",
          "30px",
          {100,
           170,
           45,
           255},
          []()
          {
            log::info(
                "Close stage");
          }));

  leftControls->appendChild(
      makeRoundButton(
          "SK",
          "30px",
          {70,
           110,
           40,
           255},
          []()
          {
            log::info(
                "Skull action");
          }));

  leftControls->appendChild(
      makeRoundButton(
          "S",
          "30px",
          {80,
           80,
           80,
           255},
          []()
          {
            log::info(
                "Sound action");
          }));

  leftControls->appendChild(
      makeRoundButton(
          "SP",
          "30px",
          {120,
           120,
           35,
           255},
          []()
          {
            log::info(
                "Start positions");
          }));

  document->appendChild(
      leftControls);

  //
  // ------------------------------------------------------------
  // Right side controls
  // ------------------------------------------------------------
  //

  auto rightControls =
      document->createElement("div");

  rightControls->style.left =
      "516px";

  rightControls->style.top =
      "40px";

  rightControls->style.width =
      "34px";

  rightControls->style.height =
      "auto";

  rightControls->style.display =
      "flex";

  rightControls->style.flexDirection =
      "column";

  rightControls->style.gap =
      "10px";

  rightControls->appendChild(
      makeRoundButton(
          "CFG",
          "30px",
          {130,
           140,
           35,
           255},
          []()
          {
            log::info(
                "Open config");
          }));

  rightControls->appendChild(
      makeRoundButton(
          "[]",
          "30px",
          {120,
           140,
           40,
           255},
          []()
          {
            log::info(
                "Secondary action");
          }));

  document->appendChild(
      rightControls);

  //
  // ------------------------------------------------------------
  // Large next arrow
  // ------------------------------------------------------------
  //

  auto next =
      document->createElement("button");

  next->textContent =
      ">";

  next->style.left =
      "510px";

  next->style.top =
      "160px";

  next->style.width =
      "35px";

  next->style.height =
      "48px";

  next->style.fontSize =
      "18px";

  next->style.backgroundColor = {
      225,
      100,
      220,
      255};

  next->style.borderColor = {
      255,
      255,
      255,
      255};

  next->style.borderWidth =
      "2px";

  next->style.borderRadius =
      "6px";

  next->onClick =
      []()
  {
    log::info(
        "Next stage");
  };

  document->appendChild(
      next);

  //
  // ------------------------------------------------------------
  // Bottom gear
  // ------------------------------------------------------------
  //

  auto gear =
      document->createElement("button");

  gear->textContent =
      "CFG";

  gear->style.left =
      "470px";

  gear->style.top =
      "266px";

  gear->style.width =
      "42px";

  gear->style.height =
      "42px";

  gear->style.fontSize =
      "8px";

  gear->style.backgroundColor = {
      170,
      210,
      30,
      255};

  gear->style.borderColor = {
      255,
      255,
      255,
      255};

  gear->style.borderWidth =
      "2px";

  gear->style.borderRadius =
      "50%";

  gear->onClick =
      []()
  {
    log::info(
        "Open settings");
  };

  document->appendChild(
      gear);

  //
  // ------------------------------------------------------------
  // Render
  // ------------------------------------------------------------
  //

  document->render();
}