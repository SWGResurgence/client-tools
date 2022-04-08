// ============================================================================
//
// InputActivityManager.cpp
// Copyright Sony Online Entertainment, Inc.
//
// ============================================================================

#include "clientGame/FirstClientGame.h"
#include "clientGame/InputActivityManager.h"

#include "clientGame/ClientCommandQueue.h"
#include "clientGame/Game.h"
#include "clientGame/GameNetwork.h"
#include "clientUserInterface/CuiConsoleHelper.h"
#include "clientUserInterface/CuiUtils.h"
#include "sharedDebug/InstallTimer.h"
#include "sharedFoundation/Crc.h"
#include "sharedFoundation/ExitChain.h"
#include "sharedFoundation/NetworkId.h"
#include "sharedGame/PlatformFeatureBits.h"
#include "sharedIoWin/IOWinManager.h"
#include "sharedNetworkMessages/GenericValueTypeMessage.h"
#include "sharedUtility/CurrentUserOptionManager.h"

// ============================================================================
//
// InputActivityManagerNamespace
//
// ============================================================================

//-----------------------------------------------------------------------------
namespace InputActivityManagerNamespace
{
	bool            s_installed = false;
	unsigned int    s_touchTime = 0;
	float           s_inactiveMinutes = 15.0f;
}

using namespace InputActivityManagerNamespace;

// ============================================================================
//
// InputActivityManager
//
// ============================================================================

//-----------------------------------------------------------------------------
void InputActivityManager::install()
{
	InstallTimer const installTimer("InputActivityManager::install");

	DEBUG_FATAL(s_installed, ("InputActivityManager::install() - Already installed."));
	s_installed = true;
	ExitChain::add(InputActivityManager::remove, "InputActivityManager::remove", 0, false);

	// obfuscation
	CurrentUserOptionManager::registerOption(s_inactiveMinutes, "b3b47ddc187911dc9bbd001143e269ee", "5797f6c6185811dc87f4000d56bf8c30");

	IoWinManager::registerInactivityCallback(setInactive, s_inactiveMinutes * 60.0f);
}

//-----------------------------------------------------------------------------
void InputActivityManager::remove()
{
	s_installed = false;
	IoWinManager::registerInactivityCallback(NULL, 0.0f);
}

//-----------------------------------------------------------------------------
bool InputActivityManager::setInactive(bool inactive)
{
	return true;
}


// ============================================================================
