#pragma once
#include "common/TimeDelta.h"
#include "pref/SampleMode.h"
class TimeDelta;
namespace DuiLib {
class CContainerUI;
class CButtonUI;
class CRichEditUI;
class CComboUI;
class COptionUI;
}

class SampleModePrefUI {
public:

  enum TimeScale {
    SECOND = 0,
    MINUTE,
    HOUR,
    DAY
  };
  void Init(DuiLib::CContainerUI* ui);

  camera::SampleMode  GetSampleMode() const;
  void SetSampleMode(camera::SampleMode mode);

  TimeDelta GetSampleInterval() const;
  void SetSampleInterval(const TimeDelta& delta);
private:
  //make the content of richedit is synchronous with sample interval button 
  void SyncButtonToEdit();
  void SyncEditToButton();

  bool OnButtonClicked(void* param);
  bool OnSampleIntervalEditOKClicked(void* param);

  DuiLib::CContainerUI* container_ui_;

  DuiLib::COptionUI*  auto_sample_option_;
  DuiLib::COptionUI*  manual_sample_option_;

  DuiLib::CButtonUI*  sample_interval_button_;
  DuiLib::CContainerUI* sample_interval_ui_;
  DuiLib::CRichEditUI*  sample_interval_edit_;
  DuiLib::CComboUI*     sample_interval_combo_;
  TimeDelta time_delta_;
};