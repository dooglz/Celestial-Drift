#include "component_shipdriver.h"

namespace Components {

class CmPlayerShipDriver : public CmShipdriver {

private:
protected:
	const unsigned int playerID_;
public:
  CmPlayerShipDriver(const unsigned int playerID);
  ~CmPlayerShipDriver();
};
}