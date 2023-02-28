#pragma once

#include <functional>
#include <string>

namespace Erp
{
  using TaskType = std::string;

  struct TaskInfo
  {
    TaskType type;
  };

  struct TaskResult
  {
    enum class Status
    {
      IDLE,
      SUCCESSED,
      FAILED,
    };

    Status status;
    std::string details;

    explicit TaskResult(Status s = Status::IDLE, const std::string& d = std::string())
      : status(s)
      , details(d)
    {
    }

    static TaskResult MakeFailed(const std::string& details)
    {
      return TaskResult(Status::FAILED, details);
    }

    static TaskResult MakeSuccessed(const std::string& details)
    {
      return TaskResult(Status::SUCCESSED, details);
    }
  };

  using TaskResultListener = std::function<void(const TaskResult&)>;
}
