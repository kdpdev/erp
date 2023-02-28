#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Utils
{
  template <typename F, typename ... Args>
  std::exception_ptr CallWithTryCatch(const std::string& context, std::ostream& err, const F& f, Args&&... args)
  {
    try
    {
      f(std::forward<Args>(args)...);
      return nullptr;
    }
    catch (const std::exception& e)
    {
      err << context << ": Exception: " << e.what() << std::endl;
      return std::current_exception();
    }
    catch (...)
    {
      err << "Unexpected exception." << std::endl;
      return std::current_exception();
    }
  }

  template <typename F, typename ... Args>
  auto CallWithTryCatch(const std::string& context, const F& f, Args&&... args)
  {
    return CallWithTryCatch(context, std::cerr, f, std::forward<Args>(args)...);
  }

  template <typename F>
  auto WrapWithTryCatch(const std::string& context, std::ostream& err, F f)
  {
    return [context, &err, f = std::move(f)](auto&&... args)
    {
      return CallWithTryCatch(
        context,
        err,
        f,
        std::forward<decltype(args)>(args)...);
    };
  }

  template <typename F>
  auto WrapWithTryCatch(const std::string& context, F f)
  {
    return WrapWithTryCatch(context, std::cerr, std::move(f));
  }

  template <typename T>
  decltype(auto) MakeNotDeletedShared(T* obj)
  {
    return std::shared_ptr<T>(obj, [](auto*) {});
  }

  template <typename Exception, typename T, typename ArgName>
  void CheckTrue(const T& arg, const ArgName& argName)
  {
    if (!arg)
    {
      throw Exception((std::string("Error: ") + argName).c_str());
    }
  }

  template <typename T, typename ArgName>
  void CheckArgTrue(const T& arg, const ArgName& argName)
  {
    CheckTrue<std::invalid_argument>(arg, argName);
  }

  template <typename T, typename ArgName>
  void CheckLogicTrue(const T& arg, const ArgName& argName)
  {
    CheckTrue<std::logic_error>(arg, argName);
  }
}
