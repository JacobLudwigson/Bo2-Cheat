struct Vector2
{
public:
	float x;
	float y;
};

struct Vector3
{
public:
	float x;
	float y;
	float z;
};

// Created with ReClass.NET 1.2 by KN4CK3R

class LocalPlayer
{
public:
	float CurrentWeaponAmmo; //0x0000
	char pad_0004[8]; //0x0004
	float PercentAimedIn1; //0x000C
	char pad_0010[124]; //0x0010
	Vector3 Position; //0x008C
	char pad_0098[24]; //0x0098
	Vector2 ViewAngles; //0x00B0
	char pad_00B8[480]; //0x00B8
	float PercentAimedIn2; //0x0298
	char pad_029C[544]; //0x029C
	float PercentAimedIn3; //0x04BC
	char pad_04C0[536]; //0x04C0
	int32_t SecondaryReserve; //0x06D8
	int32_t PrimaryReserve; //0x06DC
	char pad_06E0[52]; //0x06E0
	int32_t SecondaryMagazine; //0x0714
	int32_t PrimaryMagazine; //0x0718
	int32_t PrimaryGrenade; //0x071C
	int32_t SecondaryGrenade; //0x0720
	char pad_0724[360]; //0x0724
}; //Size: 0x088C