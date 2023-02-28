#pragma once

#include <erp/container.h>
#include <erp/employee.h>

#include <memory>
#include <string>

namespace Erp
{
  class Department final : public Container<Employee>
  {
  public:
    using ReglamentPtr = std::shared_ptr<Reglament>;

  public:
    explicit Department(const std::string& name);

    std::string GetName() const;
    void SetReglament(ReglamentPtr reglament);

    void Exec(const TaskInfo& task, const TaskResultListener& listener);
    void Exec(const TaskInfo& task, const std::string& employeeName, const TaskResultListener& listener);

  protected:
    virtual void OnObjAdded(Obj& obj) override;

  private:
    const std::string m_name;
    ReglamentPtr m_reglament;
  };
}
