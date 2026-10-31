#include "RoguelikeBan.h"
#include "../../LawnApp.h"
#include "../Plant.h"
#include "misc/SexyString.h"
#include <fstream>
#include <random>
#include <algorithm>

// NUM_SEED_TYPES from ConstEnums — keep array large enough
static constexpr int kBanSlots = 64;

RoguelikeBan& RoguelikeBan::Instance()
{
	static RoguelikeBan inst;
	return inst;
}

void RoguelikeBan::LoadForPlayer(int playerId)
{
	mPlayerId = playerId;
	mBanned.assign(kBanSlots, 0);

	mPath = GetAppDataPath("userdata");
	MkDir(mPath);
	mPath += "/roguelike_ban_" + std::to_string(playerId) + ".dat";

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

bool RoguelikeBan::IsPickableSeed(SeedType t) const
{
	if (t < SeedType::SEED_PEASHOOTER)
		return false;
	// Base plants in chooser only; upgrades still need shop (and can be banned later if owned)
	if (Plant::IsUpgrade(t))
		return false;
	if (t == SeedType::SEED_IMITATER)
		return false;
	if (static_cast<int>(t) >= 40) // keep within normal adventure pool
		return false;
	if (IsBanned(t))
		return false;
	return true;
}

int RoguelikeBan::CountAvailable() const
{
	int n = 0;
	for (int i = 0; i < 40; ++i)
	{
		if (IsPickableSeed(static_cast<SeedType>(i)))
			++n;
	}
	return n;
}

int RoguelikeBan::RollThree(SeedType out[3])
{
	out[0] = out[1] = out[2] = SeedType::SEED_NONE;
	std::vector<SeedType> pool;
	pool.reserve(40);
	for (int i = 0; i < 40; ++i)
	{
		SeedType t = static_cast<SeedType>(i);
		if (IsPickableSeed(t))
			pool.push_back(t);
	}
	if (pool.empty())
		return 0;

	static std::mt19937 rng{ std::random_device{}() };
	std::shuffle(pool.begin(), pool.end(), rng);

	int n = std::min(3, static_cast<int>(pool.size()));
	for (int i = 0; i < n; ++i)
		out[i] = pool[i];
	return n;
}

bool RoguelikeBan::IsSpecialAwardLevel(int profileLevelAfterWin)
{
	// Matches AwardScreen::Draw branches that are NOT DrawAwardSeed
	switch (profileLevelAfterWin)
	{
	case 5:  // shovel
	case 10: // note
	case 15: // almanac
	case 20: // note
	case 25: // keys
	case 30: // note
	case 35: // taco
	case 40: // note
	case 45: // watering can
	case 50: // final note
		return true;
	default:
		return false;
	}
}
