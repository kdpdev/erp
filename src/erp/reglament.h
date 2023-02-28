#pragma once

#include <erp/tasks.h>

#include <string>

namespace Erp
{
  class Reglament
  {
  public:
    using Specialization = std::string;

  public:
    virtual ~Reglament() = default;

    virtual std::string GetDescription() const = 0;
    virtual bool CanExec(const Specialization& spec, const TaskType& taskType) const = 0;

    bool CanExec(const Specialization& spec, const TaskInfo& taskInfo) const
    {
      return CanExec(spec, taskInfo.type);
    }
  };
}
