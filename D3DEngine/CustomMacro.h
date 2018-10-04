#pragma once
#include <assert.h>

#define tassert(VALUE) assert(!(VALUE))
#define DTime TimeSystem::DeltaTime()