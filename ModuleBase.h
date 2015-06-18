#ifndef __MODULE_BASE__
#define __MODULE_BASE__

#include "Thread.h"

class ModuleBase : public Thread {
  public:
    ModuleBase(){}
    virtual void execute(size_t) = 0;
};


#endif
