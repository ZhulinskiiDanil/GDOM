#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GDOMTestLayer : public CCLayer
{
public:
  static GDOMTestLayer *create();

  bool init() override;

  static void open();

private:
  void onClose(
      CCObject *);

  void buildTestUI();
};