#include "StdAfx.h"
#include "Logging.h"

using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System;

// logs the starttime of a single item
void Logging::LogStart() {
	lastLogStart = DateTime::Now;
}