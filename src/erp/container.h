#pragma once

#include <memory>
#include <vector>

namespace Erp
{
  template <typename T>
  class Container
  {
  public:
    using Obj = T;
    using ObjsList = std::vector<Obj>;

  protected:
    ~Container() = default;

  public:
    ObjsList GetObjs() const &&
    {
      return m_objs;
    }

    const ObjsList& GetObjs() const &
    {
      return m_objs;
    }

    bool Empty() const
    {
      return m_objs.empty();
    }

    auto GetObjsCount() const
    {
      return m_objs.size();
    }

    void AddObj(Obj obj)
    {
      m_objs.push_back(std::move(obj));
      OnObjAdded(m_objs.back());
    }

    template <typename Consumer>
    void EnumObjs(const Consumer& consumer) const
    {
      EnumObjs(m_objs, consumer);
    }

    template <typename Consumer>
    void EnumObjs(const Consumer& consumer)
    {
      EnumObjs(m_objs, consumer);
    }
    
    template <typename Consumer>
    void EnumAllObjs(const Consumer& consumer) const
    {
      EnumObjs([&consumer](auto&&... args)
      {
        consumer(std::forward<decltype(args)>(args)...);
        return true;
      });
    }

    template <typename Consumer>
    void EnumAllObjs(const Consumer& consumer)
    {
      EnumObjs([&consumer](auto&&... args)
      {
        consumer(std::forward<decltype(args)>(args)...);
        return true;
      });
    }

  protected:
    virtual void OnObjAdded(Obj&)
    {
    }

  private:
    template <typename Objs, typename F>
    static void EnumObjs(Objs& objs, const F consumer)
    {
      for (auto& obj : objs)
      {
        if (!consumer(obj))
        {
          break;
        }
      }
    }

  private:
    ObjsList m_objs;
  };
}
