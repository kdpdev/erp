#pragma once

#include <erp/container.h>
#include <erp/department.h>
#include <erp/reglament.h>

#include <memory>

namespace Erp
{
  class Enterprise final : public Container<Department>
  {
  public:
    using ReglamentPtr = std::shared_ptr<Reglament>;

  public:
    Enterprise();
    ReglamentPtr GetReglament() const;
    void SetReglament(ReglamentPtr reglament);

    void ExecGlobal(const TaskInfo& task, const TaskResultListener& listener);
    void ExecDepartment(const TaskInfo& task, const std::string& name, const TaskResultListener& listener);
    void ExecEmployee(const TaskInfo& task, const std::string& name, const TaskResultListener& listener);

  protected:
    virtual void OnObjAdded(Obj& obj) override;
    
  private:
    ReglamentPtr m_reglament;
  };
}
