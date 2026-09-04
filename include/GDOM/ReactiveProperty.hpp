#pragma once

#include <functional>
#include <utility>

#include "DirtyFlags.hpp"

namespace gdom
{

  template <typename T>
  class ReactiveProperty
  {
  public:
    using Callback =
        std::function<void(DirtyFlags)>;

    ReactiveProperty() = default;

    ReactiveProperty(
        T value,
        DirtyFlags dirtyFlags)
        : m_value(std::move(value)),
          m_dirtyFlags(dirtyFlags)
    {
    }

    ReactiveProperty &operator=(
        T value)
    {
      m_value =
          std::move(value);

      notify();

      return *this;
    }

    operator const T &() const
    {
      return m_value;
    }

    const T &get() const
    {
      return m_value;
    }

    T &get()
    {
      return m_value;
    }

    void bind(
        Callback callback)
    {
      m_callback =
          std::move(callback);
    }

    void setDirtyFlags(
        DirtyFlags flags)
    {
      m_dirtyFlags =
          flags;
    }

    friend bool operator==(
        const ReactiveProperty &property,
        const T &value)
    {
      return property.m_value == value;
    }

    friend bool operator!=(
        const ReactiveProperty &property,
        const T &value)
    {
      return property.m_value != value;
    }

  private:
    void notify()
    {
      if (m_callback)
      {
        m_callback(
            m_dirtyFlags);
      }
    }

  private:
    T m_value{};
    DirtyFlags m_dirtyFlags{
        DirtyFlags::None};

    Callback m_callback;
  };

}