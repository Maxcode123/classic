#pragma once

#define DECL_TYPEDEF(cls) \
  class cls##_;           \
  typedef cls##_* cls;
