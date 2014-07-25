#include "StringConversion.h"
#include <windows.h>

std::string StringConversion::UTF16ToUTF8(const std::wstring& str) {
  return UTF16ToCodePage(str, CP_UTF8);
 }

std::string StringConversion::UTF16ToANSI(const std::wstring& str) {
  return UTF16ToCodePage(str, CP_ACP);
}

std::string StringConversion::UTF16ToCodePage(const std::wstring& str, int code_page) {
  int bytes = WideCharToMultiByte(code_page, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
  char* utf8_str = (char*)malloc(bytes + 1);
  WideCharToMultiByte(code_page, 0, str.c_str(), -1, utf8_str, bytes, NULL, NULL);
  utf8_str[bytes] = '\0';
  std::string res_str(utf8_str);
  free(utf8_str);
  return res_str;
}

std::wstring StringConversion::UTF8ToUTF16(const std::string& str) {
  int cch_size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  wchar_t* utf16_str = (wchar_t*)malloc((cch_size + 1)*sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, utf16_str, cch_size);
  utf16_str[cch_size] = L'\0';
  std::wstring wstr(utf16_str);
  free(utf16_str);
  return wstr;
}