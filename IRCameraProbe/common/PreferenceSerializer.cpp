#include "PreferenceSerializer.h"
#include "PathService.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "common/StringConversion.h"
#include <tchar.h>
#include <fstream>
#include <string>


PreferenceSerializer::PreferenceSerializer() {
  file_name_ = StringConversion::UTF16ToANSI(PathService::GetConfigPath());
  file_name_ += "Pref.json";
 
  std::ifstream fstream;
  fstream.open(file_name_.c_str());
  typedef std::istream_iterator<char> StreamIterator;
  StreamIterator iter = StreamIterator(fstream);
  StreamIterator end_iter = StreamIterator();
  std::string file_content(iter,end_iter);
  doc_.Parse(file_content.c_str());
}

PreferenceSerializer::~PreferenceSerializer() {
  std::ofstream  fstream;
  fstream.open(file_name_.c_str());

  rapidjson::StringBuffer  buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc_.Accept(writer);

  fstream.write(buffer.GetString(), buffer.GetSize());
  fstream.flush();
}