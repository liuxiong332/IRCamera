#pragma once
#include <memory>

class MainWindow;

namespace camera {
class CameraImageLinkerList;
class CameraPrefLinker;
class MainLinker {
public:
  void  Init();
private:
  std::unique_ptr<MainWindow>     main_window_;
  std::unique_ptr<CameraImageLinkerList> linker_list_;

  std::unique_ptr<CameraPrefLinker>   camera_pref_linker_;
};
}