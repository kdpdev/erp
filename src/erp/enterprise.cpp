#include <erp/enterprise.h>
#include <erp/erp_utils.h>
#include <utils/common.h>

namespace Erp
{
  namespace
  {
    bool CheckNotEmpty(const Enterprise& enterprise, const TaskResultListener& listener)
    {
      if (enterprise.Empty())
      {
        listener(TaskResult::MakeFailed("Enterprise is empty."));
        return false;
      }
      return true;
    }
  }

  Enterprise::Enterprise()
    : m_reglament(CreateEmptyReglament())
  {
  }

  Enterprise::ReglamentPtr Enterprise::GetReglament() const
  {
    return m_reglament;
  }

  void Enterprise::SetReglament(ReglamentPtr reglament)
  {
    m_reglament = std::move(reglament);
    EnumAllObjs([this](auto& obj)
    {
      obj.SetReglament(m_reglament);
    });
  }

  void Enterprise::OnObjAdded(Obj& obj)
  {
    obj.SetReglament(m_reglament);
  }

  void Enterprise::ExecGlobal(const TaskInfo& task, const TaskResultListener& listener)
  {
    if (CheckNotEmpty(*this, listener))
    {
      EnumAllObjs([&task, &listener](auto& department)
      {
        department.Exec(task, PrependDetails(department.GetName(), listener));
      });
    }
  }

  void Enterprise::ExecDepartment(const TaskInfo& task, const std::string& name, const TaskResultListener& listener)
  {
    if (CheckNotEmpty(*this, listener))
    {
      bool executed = false;
      EnumAllObjs([&task, &name, &listener, & executed](auto& department)
      {
        if (department.GetName() == name)
        {
          department.Exec(task, PrependDetails(department.GetName(), listener));
          executed = true;
        }
      });

      if (!executed)
      {
        listener(TaskResult::MakeFailed("No department with the '" + name + "' name."));
      }
    }
  }

  void Enterprise::ExecEmployee(const TaskInfo& task, const std::string& name, const TaskResultListener& listener)
  {
    if (CheckNotEmpty(*this, listener))
    {
      EnumAllObjs([&task, &name, &listener](auto& department)
      {
        department.Exec(task, name, PrependDetails(department.GetName(), listener));
      });
    }
  }
}
