#include "RoguelikeBan.h"

#include "../../SexyAppFramework/SexyAppBase.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <string>

using namespace Sexy;

static constexpr int kBanSlots = 64;

RoguelikeBan& RoguelikeBan::Instance()
{
	static RoguelikeBan inst;
	return inst;
}

void RoguelikeBan::LoadForPlayer(int playerId)
{
	if (mPlayerId == playerId && !mBanned.empty())
		return;

	mPlayerId = playerId;
	mBanned.assign(kBanSlots, 0);

	std::string aDir = GetAppDataPath("userdata");
	MkDir(aDir);
	mPath = aDir + "/roguelike_ban_" + std::to_string(playerId) + ".dat";

	std::ifstream in(mPath, std::ios::binary);
	if (!in)
		return;

	std::vector<char> buf((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	for (size_t i = 0; i < buf.size() && i < mBanned.size(); ++i)
		mBanned[i] = buf[i] ? 1 : 0;
}

void RoguelikeBan::Save()
{
	if (mPlayerId < 0 || mPath.empty())
		return;

	std::ofstream out(mPath, std::ios::binary);
	if (!out)
		return;
	out.write(mBanned.data(), static_cast<std::streamsize>(mBanned.size()));
}

bool RoguelikeBan::IsBanned(SeedType theSeedType) const
{
	int i = static_cast<int>(theSeedType);
	if (i < 0 || i >= static_cast<int>(mBanned.size()))
		return false;
	return mBanned[i] != 0;
}

void RoguelikeBan::Ban(SeedType theSeedType)
{
	int i = static_cast<int>(theSeedType);
	if (i < 0 || i >= static_cast<int>(mBanned.size()))
		return;
	mBanned[i] = 1;
	Save();
}

bool RoguelikeBan::IsPickableSeed(SeedType theSeedType) const
{
	// Only normal adventure plants 0..39 (pea .. melon).
	int i = static_cast<int>(theSeedType);
	if (i < 0 || i >= 40)
		return false;
	if (IsBanned(theSeedType))
		return false;
	return true;
}

int RoguelikeBan::RollThree(SeedType outSeeds[3])
{
	outSeeds[0] = outSeeds[1] = outSeeds[2] = SeedType::SEED_NONE;

	std::vector<SeedType> pool;
	pool.reserve(40);
	for (int i = 0; i < 40; ++i)
	{
		SeedType aType = static_cast<SeedType>(i);
		if (IsPickableSeed(aType))
			pool.push_back(aType);
	}
	if (pool.empty())
		return 0;

	static std::mt19937 rng{ std::random_device{}() };
	std::shuffle(pool.begin(), pool.end(), rng);

	int n = std::min(3, static_cast<int>(pool.size()));
	for (int i = 0; i < n; ++i)
		outSeeds[i] = pool[i];
	return n;
}

bool RoguelikeBan::IsSpecialAwardLevel(int profileLevelAfterWin)
{
	switch (profileLevelAfterWin)
	{
	case 5:
	case 10:
	case 15:
	case 20:
	case 25:
	case 30:
	case 35:
	case 40:
	case 45:
	case 50:
		return true;
	default:
		return false;
	}
}
