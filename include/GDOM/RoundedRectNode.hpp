#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace gdom
{

    class RoundedRectNode : public CCLayer
    {
    public:
        static RoundedRectNode *create(
            CCSize size,
            ccColor4B fillColor,
            float radius,
            float borderWidth = 0.f,
            ccColor4B borderColor = {
                0,
                0,
                0,
                0});

        bool init(
            CCSize size,
            ccColor4B fillColor,
            float radius,
            float borderWidth,
            ccColor4B borderColor);

        void setSize(
            CCSize size);

        void setFillColor(
            ccColor4B color);

        void setBorderColor(
            ccColor4B color);

        void setBorderWidth(
            float width);

        void setRadius(
            float radius);

    private:
        void redraw();

        CCSize m_size{
            0.f,
            0.f};

        ccColor4B m_fillColor{
            255,
            255,
            255,
            255};

        ccColor4B m_borderColor{
            0,
            0,
            0,
            0};

        float m_borderWidth =
            0.f;

        float m_radius =
            0.f;

        CCDrawNode *m_draw =
            nullptr;
    };

}