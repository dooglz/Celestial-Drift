#pragma once
#include <mutex>
#include <string>
#include <vector>

class ConVar;
typedef void (*FnChangeCallback_t)(ConVar *var, const char *pOldValue, float flOldValue);

class ConVar {

public:
  // all these are thread safe
  ConVar(const char *pName, const char *pDefaultValue, int flags = 0);
  ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString);
  ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin,
         bool bMax, float fMax);
  ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback);
  ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin,
         bool bMax, float fMax, FnChangeCallback_t callback);

  ~ConVar(void);

  static std::vector<ConVar *> cvarList;
  static ConVar *FindCvar(const std::string &commandName);
  const char *GetName(void) const;
  const char *GetHelpText(void) const;

public:
  // all these funcs have to lock to be thread safe
  bool IsFlagSet(int flag) const;
  void AddFlags(int flags);
  void SetChangeCallback(FnChangeCallback_t callback);

  float GetFloat(void) const;
  int GetInt(void) const;
  bool GetBool() const;
  char const *GetString(void) const;
  int GetFlags(void) const;

  void SetValue(const char *value);
  void SetValue(float value);
  void SetValue(int value);

  // Reset to default value
  void Revert(void);

  // True if it has a min/max setting
  bool GetMin(float &minVal) const;
  bool GetMax(float &maxVal) const;
  const char *GetDefault(void) const;
  void SetDefault(const char *pszDefault);

private:
  // interal lock safe funcs
  bool IsFlagSet_(int flag) const;
  void AddFlags_(int flags);
  void SetChangeCallback_(FnChangeCallback_t callback);
  float GetFloat_(void) const;
  int GetInt_(void) const;
  bool GetBool_() const { return !!GetInt_(); }
  char const *GetString_(void) const;
  int GetFlags_(void) const;
  void SetValue_(const char *value);
  void SetValue_(float value);
  void SetValue_(int value);
  void Revert_(void);
  bool GetMin_(float &minVal) const;
  bool GetMax_(float &maxVal) const;
  const char *GetDefault_(void) const;
  void SetDefault_(const char *pszDefault);

private:
  // Called by CCvar when the value of a var is changing.
  virtual void InternalSetValue(const char *value);

  // For CVARs marked FCVAR_NEVER_AS_STRING
  void InternalSetFloatValue(float fNewValue);
  void InternalSetIntValue(int nValue);

  bool ClampValue(float &value);
  void ChangeStringValue(const char *tempVal, float flOldValue);

  void Create(const char *pName, const char *pDefaultValue, int flags = 0, const char *pHelpString = 0,
              bool bMin = false, float fMin = 0.0, bool bMax = false, float fMax = false,
              FnChangeCallback_t callback = 0);

  mutable std::mutex execution_mutex;

  const char *m_pszName;
  const char *m_pszHelpString;

  int flags_;
  const char *defaultValue_;
  char *stringVal_;
  size_t stringLength_;
  float floatValue_;
  int intValue_;
  bool hasMin_;
  float minVal_;
  bool hasMax_;
  float maxVal_;

  // Call this function when ConVar changes
  FnChangeCallback_t m_fnChangeCallback;
};
