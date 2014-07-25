#include "PathService.h"
#include <tchar.h>
#include <windows.h>

TString PathService::GetConfigPath() {
  TCHAR module_name[MAX_PATH];
  GetModuleFileName(NULL, module_name, MAX_PATH);
  TString file_name = module_name;
  size_t n = file_name.rfind(_T('\\'));
  file_name.resize(n + 1);
  file_name.append(_T("config\\"));
  return file_name;
}