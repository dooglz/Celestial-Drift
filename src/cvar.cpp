#pragma warning(disable : 4996)
#include "common.h"
#include "cvar.h"
#include <cassert>
#include <string>
#include <thread>

std::vector<ConVar *> ConVar::cvarList;

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags /* = 0 */) {
  Create(pName, pDefaultValue, flags);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString) {
  Create(pName, pDefaultValue, flags, pHelpString);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString,
               bool bMin, float fMin, bool bMax, float fMax) {
  Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString,
               FnChangeCallback_t callback) {
  Create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString,
               bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback) {
  Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
}

ConVar::~ConVar() {}

bool ConVar::IsFlagSet_(int flag) const { return (flag & flags_) ? true : false; }

const char *ConVar::GetHelpText() const { return m_pszHelpString; }

const char *ConVar::GetName() const { return m_pszName; }

void ConVar::AddFlags_(int flags) { flags_ |= flags; }

void ConVar::SetChangeCallback_(FnChangeCallback_t callback) {

  assert(!m_fnChangeCallback || !callback);
  m_fnChangeCallback = callback;

  if (m_fnChangeCallback) {
    // Call it immediately to set the initial value...
    m_fnChangeCallback(this, stringVal_, floatValue_);
  }
}

float ConVar::GetFloat_() const { return floatValue_; }

int ConVar::GetInt_() const { return intValue_; }

char const *ConVar::GetString_() const { return (stringVal_) ? stringVal_ : ""; }

int ConVar::GetFlags_() const { return flags_; }

void ConVar::SetValue_(const char *value) { InternalSetValue(value); }

void ConVar::SetValue_(float value) { InternalSetFloatValue(value); }

void ConVar::SetValue_(int value) { InternalSetIntValue(value); }

void ConVar::Revert_() { SetValue_(defaultValue_); }

bool ConVar::GetMin_(float &minVal) const {
  minVal = minVal_;
  return hasMin_;
}

bool ConVar::GetMax_(float &maxVal) const {
  maxVal = maxVal_;
  return hasMax_;
}

const char *ConVar::GetDefault_() const { return defaultValue_; }

void ConVar::SetDefault_(const char *pszDefault) {
  defaultValue_ = pszDefault ? pszDefault : "";
  assert(defaultValue_);
}

void ConVar::InternalSetValue(const char *value) {

  float fNewValue;
  char tempVal[32];
  char *val;

  float flOldValue = floatValue_;

  val = (char *)value;
  if (!value)
    fNewValue = 0.0f;
  else
    fNewValue = (float)atof(value);

  if (ClampValue(fNewValue)) {
    std::to_string(fNewValue).copy(tempVal, sizeof(tempVal), 0);
    // Q_snprintf(tempVal, sizeof(tempVal), "%f", fNewValue);
    val = tempVal;
  }

  // Redetermine value
  floatValue_ = fNewValue;
  intValue_ = (int)(fNewValue);

  // if (!(flags_ & FCVAR_NEVER_AS_STRING))
  // {
  ChangeStringValue(val, flOldValue);
  // }
}

void ConVar::InternalSetFloatValue(float fNewValue) {
  if (fNewValue == floatValue_)
    return;

  ClampValue(fNewValue);

  // Redetermine value
  float flOldValue = floatValue_;
  floatValue_ = fNewValue;
  intValue_ = (int)floatValue_;

  // if (!(flags_ & FCVAR_NEVER_AS_STRING))
  if (true) {
    char tempVal[32];
    const std::string s = std::to_string(fNewValue);
    s.copy(tempVal, sizeof(tempVal), 0);
    // manually add the null term back in
    if (s.length() < sizeof(tempVal)) {
      tempVal[s.length()] = 0;
    }

    // Q_snprintf(tempVal, sizeof(tempVal), "%f", floatValue_);
    ChangeStringValue(tempVal, flOldValue);
  } else {
    assert(!m_fnChangeCallback);
  }
}

void ConVar::InternalSetIntValue(int nValue) {
  if (nValue == intValue_)
    return;

  float fValue = (float)nValue;
  if (ClampValue(fValue)) {
    nValue = (int)(fValue);
  }

  // Redetermine value
  float flOldValue = floatValue_;
  floatValue_ = fValue;
  intValue_ = nValue;

  // if (!(flags_ & FCVAR_NEVER_AS_STRING))
  if (true) {
    char tempVal[32];
    const std::string s = std::to_string(intValue_);
    s.copy(tempVal, sizeof(tempVal), 0);
    // manually add the null term back in
    if (s.length() < sizeof(tempVal)) {
      tempVal[s.length()] = 0;
    }
    // Q_snprintf(tempVal, sizeof(tempVal), "%d", intValue_);
    ChangeStringValue(tempVal, flOldValue);
  } else {
    assert(!m_fnChangeCallback);
  }
}

bool ConVar::ClampValue(float &value) {
  if (hasMin_ && (value < minVal_)) {
    value = minVal_;
    return true;
  }

  if (hasMax_ && (value > maxVal_)) {
    value = maxVal_;
    return true;
  }

  return false;
}

void ConVar::ChangeStringValue(const char *tempVal, float flOldValue) {
  // Assert(!(flags_ & FCVAR_NEVER_AS_STRING));

  char *pszOldValue = (char *)stackalloc(stringLength_);
  memcpy(pszOldValue, stringVal_, stringLength_);

  if (tempVal) {
    size_t len = strlen(tempVal) + 1;

    if (len > stringLength_) {
      if (stringVal_) {
        delete[] stringVal_;
      }

      stringVal_ = new char[len];
      stringLength_ = len;
    }

    memcpy(stringVal_, tempVal, len);
  } else {
    *stringVal_ = 0;
  }

  // If nothing has changed, don't do the callbacks.
  if (strcmp(pszOldValue, stringVal_) != 0) {
    // Invoke any necessary callback function
    if (m_fnChangeCallback) {
      m_fnChangeCallback(this, pszOldValue, flOldValue);
    }

    // g_pCVar->CallGlobalChangeCallbacks(this, pszOldValue, flOldValue);
  }
}

void ConVar::Create(const char *pName, const char *pDefaultValue, int flags /*= 0*/,
                    const char *pHelpString /*= NULL*/, bool bMin /*= false*/, float fMin /*= 0.0*/,
                    bool bMax /*= false*/, float fMax /*= false*/,
                    FnChangeCallback_t callback /*= NULL*/) {
  // Name should be static data
  SetDefault(pDefaultValue);

  stringLength_ = strlen(defaultValue_) + 1;
  stringVal_ = new char[stringLength_];
  memcpy(stringVal_, defaultValue_, stringLength_);

  hasMin_ = bMin;
  minVal_ = fMin;
  hasMax_ = bMax;
  maxVal_ = fMax;

  m_fnChangeCallback = callback;

  floatValue_ = (float)atof(stringVal_);
  intValue_ = atoi(stringVal_); // dont convert from float to int and lose bits

  // Bounds Check, should never happen, if it does, no big deal
  if (hasMin_ && (floatValue_ < minVal_)) {
    assert(0);
  }

  if (hasMax_ && (floatValue_ > maxVal_)) {
    assert(0);
  }

  // BaseClass::CreateBase(pName, pHelpString, flags);
  assert(pName);
  m_pszName = pName;
  m_pszHelpString = pHelpString ? pHelpString : "";

  flags_ = flags;

  cvarList.push_back(this);
}

ConVar *ConVar::FindCvar(const std::string &commandName) {
  for (auto &it : cvarList) {
    if (std::string(it->GetName()) == commandName) {
      return it;
    }
  }
  return nullptr;
}

//
bool ConVar::IsFlagSet(int flag) const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return IsFlagSet_(flag);
}
float ConVar::GetFloat() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetFloat_();
}
int ConVar::GetInt() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetInt_();
}
bool ConVar::GetBool() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetBool_();
}
char const *ConVar::GetString() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetString_();
}
int ConVar::GetFlags() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetFlags_();
}
const char *ConVar::GetDefault() const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetDefault_();
}
bool ConVar::GetMin(float &minVal) const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetMin_(minVal);
}
bool ConVar::GetMax(float &maxVal) const {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return GetMax_(maxVal);
}

void ConVar::AddFlags(int flags) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return AddFlags_(flags);
}
void ConVar::SetChangeCallback(FnChangeCallback_t callback) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return SetChangeCallback_(callback);
}
void ConVar::SetValue(const char *value) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return SetValue_(value);
}
void ConVar::SetValue(float value) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return SetValue_(value);
}
void ConVar::SetValue(int value) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return SetValue_(value);
}
void ConVar::Revert() {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return Revert_();
}
void ConVar::SetDefault(const char *pszDefault) {
  std::lock_guard<std::mutex> lock(execution_mutex);
  return SetDefault_(pszDefault);
}