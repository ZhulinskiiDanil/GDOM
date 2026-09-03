#include <GDOM/RoundedRectNode.hpp>

#include <algorithm>
#include <cmath>
#include <vector>

using namespace geode::prelude;

namespace gdom
{

    namespace
    {

        ccColor4F toColor4F(
            const ccColor4B &color)
        {
            return {
                color.r / 255.f,
                color.g / 255.f,
                color.b / 255.f,
                color.a / 255.f};
        }

    }

    RoundedRectNode *RoundedRectNode::create(
        CCSize size,
        ccColor4B fillColor,
        float radius,
        float borderWidth,
        ccColor4B borderColor)
    {
        auto node =
            new RoundedRectNode();

        if (!node)
        {
            return nullptr;
        }

        if (!node->init(
                size,
                fillColor,
                radius,
                borderWidth,
                borderColor))
        {
            delete node;
            return nullptr;
        }

        node->autorelease();

        return node;
    }

    bool RoundedRectNode::init(
        CCSize size,
        ccColor4B fillColor,
        float radius,
        float borderWidth,
        ccColor4B borderColor)
    {
        if (!CCLayer::init())
        {
            return false;
        }

        m_size =
            size;

        m_fillColor =
            fillColor;

        m_radius =
            radius;

        m_borderWidth =
            borderWidth;

        m_borderColor =
            borderColor;

        m_draw =
            CCDrawNode::create();

        if (!m_draw)
        {
            return false;
        }

        setContentSize(
            m_size);

        m_draw->setContentSize(
            m_size);

        addChild(
            m_draw);

        redraw();

        return true;
    }

    void RoundedRectNode::setSize(
        CCSize size)
    {
        m_size =
            size;

        setContentSize(
            m_size);

        if (m_draw)
        {
            m_draw->setContentSize(
                m_size);
        }

        redraw();
    }

    void RoundedRectNode::setFillColor(
        ccColor4B color)
    {
        m_fillColor =
            color;

        redraw();
    }

    void RoundedRectNode::setBorderColor(
        ccColor4B color)
    {
        m_borderColor =
            color;

        redraw();
    }

    void RoundedRectNode::setBorderWidth(
        float width)
    {
        m_borderWidth =
            std::max(
                0.f,
                width);

        redraw();
    }

    void RoundedRectNode::setRadius(
        float radius)
    {
        m_radius =
            radius;

        redraw();
    }

    void RoundedRectNode::redraw()
    {
        if (!m_draw)
        {
            return;
        }

        m_draw->clear();

        const float width =
            m_size.width;

        const float height =
            m_size.height;

        if (
            width <= 0.f ||
            height <= 0.f)
        {
            return;
        }

        const float radius =
            std::clamp(
                m_radius,
                0.f,
                std::min(
                    width,
                    height) /
                    2.f);

        const float borderWidth =
            std::clamp(
                m_borderWidth,
                0.f,
                std::min(
                    width,
                    height) /
                    2.f);

        const auto fillColor =
            toColor4F(
                m_fillColor);

        const auto borderColor =
            toColor4F(
                m_borderColor);

        //
        // Number of segments per corner.
        //

        const int segments =
            std::clamp(
                static_cast<int>(
                    std::ceil(
                        std::max(
                            radius,
                            1.f) *
                        2.f)),
                3,
                16);

        //
        // Build rounded rectangle loop.
        //
        // Important:
        // outer and inner loops always contain
        // exactly the same number of vertices.
        //

        auto buildLoop =
            [segments](
                float x,
                float y,
                float w,
                float h,
                float r)
        {
            std::vector<CCPoint>
                vertices;

            vertices.reserve(
                static_cast<size_t>(
                    (segments + 1) * 4));

            const float safeRadius =
                std::clamp(
                    r,
                    0.f,
                    std::min(
                        w,
                        h) /
                        2.f);

            auto addCorner =
                [&](
                    const CCPoint &center,
                    float startAngle)
            {
                for (
                    int i = 0;
                    i <= segments;
                    ++i)
                {
                    const float progress =
                        static_cast<float>(i) /
                        static_cast<float>(
                            segments);

                    const float angle =
                        startAngle +
                        static_cast<float>(
                            M_PI_2) *
                            progress;

                    vertices.push_back({center.x +
                                            std::cos(angle) *
                                                safeRadius,

                                        center.y +
                                            std::sin(angle) *
                                                safeRadius});
                }
            };

            addCorner(
                {x + w - safeRadius,
                 y + safeRadius},
                -static_cast<float>(
                    M_PI_2));

            addCorner(
                {x + w - safeRadius,
                 y + h - safeRadius},
                0.f);

            addCorner(
                {x + safeRadius,
                 y + h - safeRadius},
                static_cast<float>(
                    M_PI_2));

            addCorner(
                {x + safeRadius,
                 y + safeRadius},
                static_cast<float>(
                    M_PI));

            return vertices;
        };

        //
        // No border:
        // just draw the normal background.
        //

        if (borderWidth <= 0.01f)
        {
            auto vertices =
                buildLoop(
                    0.f,
                    0.f,
                    width,
                    height,
                    radius);

            if (vertices.size() < 3)
            {
                return;
            }

            m_draw->drawPolygon(
                vertices.data(),
                static_cast<unsigned int>(
                    vertices.size()),
                fillColor,
                0.f,
                {0.f, 0.f, 0.f, 0.f});

            return;
        }

        //
        // Outer border geometry.
        //

        auto outer =
            buildLoop(
                0.f,
                0.f,
                width,
                height,
                radius);

        //
        // Inner background geometry.
        //

        const float innerWidth =
            std::max(
                0.f,
                width -
                    borderWidth * 2.f);

        const float innerHeight =
            std::max(
                0.f,
                height -
                    borderWidth * 2.f);

        const float innerRadius =
            std::max(
                0.f,
                radius -
                    borderWidth);

        if (
            innerWidth <= 0.f ||
            innerHeight <= 0.f)
        {
            m_draw->drawPolygon(
                outer.data(),
                static_cast<unsigned int>(
                    outer.size()),
                borderColor,
                0.f,
                {0.f, 0.f, 0.f, 0.f});

            return;
        }

        auto inner =
            buildLoop(
                borderWidth,
                borderWidth,
                innerWidth,
                innerHeight,
                innerRadius);

        //
        // Draw actual border ring.
        //
        // Each segment is a quad between
        // outer and inner polygons.
        //

        const size_t count =
            std::min(
                outer.size(),
                inner.size());

        for (
            size_t i = 0;
            i < count;
            ++i)
        {
            const size_t next =
                (i + 1) %
                count;

            CCPoint quad[] = {
                outer[i],
                outer[next],
                inner[next],
                inner[i]};

            m_draw->drawPolygon(
                quad,
                4,
                borderColor,
                0.f,
                {0.f, 0.f, 0.f, 0.f});
        }

        //
        // Draw fill ONLY inside the border.
        //
        // It never lies on top of the border,
        // so translucent backgrounds do not
        // mix with borderColor.
        //

        m_draw->drawPolygon(
            inner.data(),
            static_cast<unsigned int>(
                inner.size()),
            fillColor,
            0.f,
            {0.f, 0.f, 0.f, 0.f});
    }

}