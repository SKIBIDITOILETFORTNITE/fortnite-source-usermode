#pragma once

void TriggerbotLoop()
{
	if (Settings::Triggerbot)
	{
		if (Cache::ClosestPawn) {
			if (!Settings::AlwaysActive)
			{
				if (Syscalls::GetAsyncKeyState(Settings::Triggerkey))
				{
					if (Addresses::PlayerController->GetTargetedPawn())
						Syscalls::SendLeftClick();
				}
			}
			else
			{
				if (Addresses::PlayerController->GetTargetedPawn())
					Syscalls::SendLeftClick();
			}
		}
	}
}