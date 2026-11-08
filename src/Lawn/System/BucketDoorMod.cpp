/*
 * Bucket + Screen Door mod for PvZ-Portable
 * - Bare-headed zombies: pail helm + screen door
 * - Already have head armor: screen door only (if no shield yet)
 * - Already have a shield (door/ladder/paper): head pail only (if no helm)
 * - Gargantuar / Red-eye: body HP x2 (no door; model has no free hand)
 */

#include "BucketDoorMod.h"

#include "../Zombie.h"
#include "../../ConstEnums.h"

// Same values as in Zombie.cpp (there they are file-local constexpr, not in a header)
namespace {
constexpr int kRenderGroupNormal = 0;
constexpr int kRenderGroupHidden = -1;
}

void BucketDoorMod_Apply(Zombie* z)
{
	if (!z)
		return;

	const ZombieType type = z->mZombieType;

	switch (type)
	{
	case ZombieType::ZOMBIE_BOSS:
	case ZombieType::ZOMBIE_ZAMBONI:
	case ZombieType::ZOMBIE_CATAPULT:
	case ZombieType::ZOMBIE_BUNGEE:
	case ZombieType::ZOMBIE_BOBSLED:
		return;

	case ZombieType::ZOMBIE_GARGANTUAR:
	case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
		z->mBodyHealth *= 2;
		return;

	default:
		break;
	}

	const bool hasHelm = (z->mHelmType != HelmType::HELMTYPE_NONE);
	const bool hasShield = (z->mShieldType != ShieldType::SHIELDTYPE_NONE);

	if (!hasHelm)
	{
		z->mHelmType = HelmType::HELMTYPE_PAIL;
		z->mHelmHealth = 1100;
		z->ReanimShowPrefix("anim_bucket", kRenderGroupNormal);
		z->ReanimShowPrefix("anim_hair", kRenderGroupHidden);
	}

	if (!hasShield)
	{
		z->mShieldType = ShieldType::SHIELDTYPE_DOOR;
		z->mShieldHealth = 1100;
		z->AttachShield();
	}
}
