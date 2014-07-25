#pragma once
#include "IRCameraBasic.h"
#include "rapidjson/document.h"

class PreferenceSerializer {
public:
  PreferenceSerializer();
  ~PreferenceSerializer();

  rapidjson::Document& GetDocument() {
    return doc_;
  }
private:
  std::string file_name_;
  rapidjson::Document doc_;
};