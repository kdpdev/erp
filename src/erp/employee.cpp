#include <erp/employee.h>

namespace Erp
{
  Employee::Employee(const std::string& name, const Specialization& specialization)
    : m_name(name)
    , m_specialization(specialization)
  {
  }

  std::string Employee::GetName() const
  {
    return m_name;
  }

  Employee::Specialization Employee::GetSpecialization() const
  {
    return m_specialization;
  }

  void Employee::SetReglament(ReglamentPtr reglament)
  {
    m_reglament = std::move(reglament);
  }

  TaskResult Employee::Exec(const TaskInfo& taskInfo)
  {
    if (m_reglament != nullptr &&  !m_reglament->CanExec(GetSpecialization(), taskInfo))
    {
      return TaskResult::MakeFailed("'" + taskInfo.type + "' is not my duty.");
    }
    return TaskResult::MakeSuccessed("'" + taskInfo.type + "' is done.");
  }
}
