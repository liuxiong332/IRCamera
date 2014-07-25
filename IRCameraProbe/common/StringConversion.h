#pragma once
#include <string>

class StringConversion {
public:
  static std::string UTF16ToUTF8(const std::wstring& str);

  static std::string UTF16ToANSI(const std::wstring& str);
  static std::string UTF16ToCodePage(const std::wstring& str, int code_page);

  static std::wstring UTF8ToUTF16(const std::string& str);
};