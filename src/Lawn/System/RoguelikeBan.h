#pragma once

#include "../../ConstEnums.h"
#include <string>
#include <vector>

// Permanent plant bans for the roguelike mode (per profile).
class RoguelikeBan
{
public:
	static RoguelikeBan& Instance();

	// Call when a profile is selected / created
	void LoadForPlayer(int playerId);
	void Save();

	bool IsBanned(SeedType theSeedType) const;
	void Ban(SeedType theSeedType);

	// Fills out[3] with distinct non-banned pickable plants. Returns how many were rolled (0..3).
	int RollThree(SeedType out[3]);

	int CountAvailable() const;

	// Adventure levels that keep original awards (shovel, notes, almanac, etc.)
	// Uses the SAME level number AwardScreen::Draw uses (profile level AFTER finishing).
	static bool IsSpecialAwardLevel(int profileLevelAfterWin);

private:
	RoguelikeBan() = default;

	bool IsPickableSeed(SeedType t) const;

	int mPlayerId = -1;
	std::vector<char> mBanned; // index by SeedType, 0/1
	std::string mPath;
};
