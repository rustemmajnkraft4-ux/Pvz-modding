#pragma once

class Zombie;

// Call once at the end of Zombie::ZombieInitialize (before MaxHealth is cached).
void BucketDoorMod_Apply(Zombie* zombie);
