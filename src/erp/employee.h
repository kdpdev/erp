#pragma once

#include <erp/reglament.h>

#include <string>
#include <memory>

namespace Erp
{
  class Employee final
  {
  public:
    using Specialization = Reglament::Specialization;
    using ReglamentPtr = std::shared_ptr<Reglament>;

  public:
    Employee(const std::string& name, const Specialization& specialization);

    std::string GetName() const;
    Specialization GetSpecialization() const;
    void SetReglament(ReglamentPtr reglament);
    TaskResult Exec(const TaskInfo& taskInfo);

  private:
    const std::string m_name;
    const Specialization m_specialization;
    ReglamentPtr m_reglament;
  };
}
