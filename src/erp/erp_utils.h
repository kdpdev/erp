#pragma once

#include <erp/tasks.h>
#include <erp/reglament.h>

#include <iosfwd>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace Erp
{
  class Enterprise;
  class Department;
  class Employee;

  std::string ToHumanString(const Erp::TaskResult::Status obj);
  std::string ToHumanString(const Erp::TaskResult& obj);
  std::string ToHumanString(const Erp::Employee& obj);
  std::string ToHumanString(const Erp::Department& obj);
  std::string ToHumanString(const Erp::Enterprise& obj);

  TaskResultListener PrependDetails(const std::string& str, TaskResultListener listener);

  std::unique_ptr<Reglament> CreateEmptyReglament();

  std::unique_ptr<Reglament> CreateDefaultReglament(
    const std::set<TaskType>& common,
    const std::map<Reglament::Specialization, std::set<TaskType>>& specialized);

  Department CreateDepartmentFromCsvStream(
    const std::string& name,
    std::istream& csvStream);

  std::unique_ptr<Enterprise> CreateEnterpriseFromDepartmentCsvStreams(
    const std::map<std::string, std::unique_ptr<std::istream>>& streams);
}
