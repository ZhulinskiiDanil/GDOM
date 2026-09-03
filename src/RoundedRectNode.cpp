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

        float clampRadius(
            float radius,
            float width,
            float height)
        {
            if (
                width <= 0.f ||
                height <= 0.f)
            {
                return 0.f;
            }

            return std::clamp(
                radius,
                0.f,
                std::min(
                    width,
                    height) /
                    2.f);
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
            std::max(
                0.f,
                borderWidth);

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
            std::max(
                0.f,
                radius);

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
            std::max(
                0.f,
                m_size.width);

        const float height =
            std::max(
                0.f,
                m_size.height);

        if (
            width <= 0.f ||
            height <= 0.f)
        {
            return;
        }

        //
        // CSS-like radius limitation.
        //
        // 10px high element:
        // max radius = 5px
        //
        // 20px high element:
        // max radius = 10px
        //
        // This prevents opposite corners
        // from overlapping.
        //

        const float radius =
            clampRadius(
                m_radius,
                width,
                height);

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
        // Enough points for smooth corners,
        // but not excessive for small elements.
        //

        const int segments =
            std::clamp(
                static_cast<int>(
                    std::ceil(
                        radius *
                        1.5f)),
                3,
                16);

        //
        // Build convex rounded rectangle.
        //
        // Important:
        //
        // We DO NOT include the final point
        // of each quarter-circle.
        //
        // Previously every corner used:
        //
        //     i <= segments
        //
        // This can create duplicate vertices
        // when radius == height / 2 or
        // radius == width / 2.
        //
        // Example:
        //
        // height = 8
        // radius = 4
        //
        // Bottom-right and top-right arcs
        // share the same center.
        //
        // Their connecting point was inserted
        // twice, which can confuse CCDrawNode's
        // polygon triangulation and produce
        // spikes / arrows.
        //

        auto buildLoop =
            [segments](
                float x,
                float y,
                float w,
                float h,
                float requestedRadius)
        {
            std::vector<CCPoint>
                vertices;

            if (
                w <= 0.f ||
                h <= 0.f)
            {
                return vertices;
            }

            const float r =
                clampRadius(
                    requestedRadius,
                    w,
                    h);

            //
            // Radius zero does not need arcs.
            //

            if (r <= 0.001f)
            {
                vertices.reserve(4);

                vertices.push_back({x + w,
                                    y});

                vertices.push_back({x + w,
                                    y + h});

                vertices.push_back({x,
                                    y + h});

                vertices.push_back({x,
                                    y});

                return vertices;
            }

            vertices.reserve(
                static_cast<size_t>(
                    segments * 4));

            auto addCorner =
                [&](
                    float centerX,
                    float centerY,
                    float startAngle)
            {
                for (
                    int i = 0;
                    i < segments;
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

                    vertices.push_back({centerX +
                                            std::cos(angle) *
                                                r,

                                        centerY +
                                            std::sin(angle) *
                                                r});
                }
            };

            //
            // Counter-clockwise:
            //
            // bottom-right
            // top-right
            // top-left
            // bottom-left
            //

            addCorner(
                x + w - r,
                y + r,
                -static_cast<float>(
                    M_PI_2));

            addCorner(
                x + w - r,
                y + h - r,
                0.f);

            addCorner(
                x + r,
                y + h - r,
                static_cast<float>(
                    M_PI_2));

            addCorner(
                x + r,
                y + r,
                static_cast<float>(
                    M_PI));

            return vertices;
        };

        //
        // No border.
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
        // Outer geometry.
        //

        auto outer =
            buildLoop(
                0.f,
                0.f,
                width,
                height,
                radius);

        if (outer.size() < 3)
        {
            return;
        }

        const float innerWidth =
            width -
            borderWidth * 2.f;

        const float innerHeight =
            height -
            borderWidth * 2.f;

        //
        // Border completely fills the element.
        //

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

        //
        // Inner radius follows the outer curve.
        //
        // It is additionally clamped against
        // the inner box, so it can never overlap.
        //

        const float innerRadius =
            clampRadius(
                std::max(
                    0.f,
                    radius -
                        borderWidth),
                innerWidth,
                innerHeight);

        auto inner =
            buildLoop(
                borderWidth,
                borderWidth,
                innerWidth,
                innerHeight,
                innerRadius);

        if (inner.size() < 3)
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

        //
        // Both loops are generated with the
        // exact same number of vertices.
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
        // Fill.
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