#include "component_shipdriver.h"

namespace Components {

  class CmNPCShipDriver : public CmShipdriver {

  private:
  protected:
  public:
    CmNPCShipDriver();
    ~CmNPCShipDriver();
    void Update(double delta);
  };
}