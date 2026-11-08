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

void BucketDoorMod_Apply(Zombie* z)
{
	if (!z)
		return;

	const ZombieType type = z->mZombieType;

	// Types that must not get normal zombie armor attachments
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
		// Double body HP (Giga-like toughness). No door — arms hold the pole.
		z->mBodyHealth *= 2;
		return;

	default:
		break;
	}

	const bool hasHelm = (z->mHelmType != HelmType::HELMTYPE_NONE);
	const bool hasShield = (z->mShieldType != ShieldType::SHIELDTYPE_NONE);

	// Head: only if nothing is already on the head
	if (!hasHelm)
	{
		z->mHelmType = HelmType::HELMTYPE_PAIL;
		z->mHelmHealth = 1100;
		// Same visual toggles as ZOMBIE_PAIL init (works on plain / door / cone bases)
		z->ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
		z->ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
	}

	// Hands: only if no shield already (newspaper / ladder / door keep theirs)
	if (!hasShield)
	{
		z->mShieldType = ShieldType::SHIELDTYPE_DOOR;
		z->mShieldHealth = 1100;
		z->AttachShield();
	}
}
