#pragma once

class CameraPrefLayoutUIObserver {
public:
  virtual void OnBackButtonClicked() = 0;
  virtual ~CameraPrefLayoutUIObserver() {}
};