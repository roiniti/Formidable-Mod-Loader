//
// Created by shirome on 19/04/2022.
//

#include <stdint.h>
#include <string>
#include <vector>


#ifndef ANDROID_HOOKING_PATCHING_TEMPLATE_MAIN_STRUCTS_H
#define ANDROID_HOOKING_PATCHING_TEMPLATE_MAIN_STRUCTS_H


typedef struct Il2CppString
{
    void* object;
    int32_t length;
    void* chars[];
} Il2CppString;

typedef struct Il2CppDomain
{
    void* domain;
    void* setup;
    void* default_context;
    const char* friendly_name;
    uint32_t domain_id;
} Il2CppDomain;

typedef struct Il2CppAssembly
{
    void* imageIndex;
    void* customAttributeIndex;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    void* aname;
} Il2CppAssembly;

typedef struct Il2CppImage
{
    const char* name;
    void* assemblyIndex;
    void* typeStart;
    uint32_t typeCount;
    void* entryPointIndex;
    void* nameToClassHashTable;
    uint32_t token;
} Il2CppImage;

typedef void(*Il2CppMethodPointer)();

typedef struct MethodInfo
{
    Il2CppMethodPointer* methodPointer;
    void* invoker_method;
    const char* name;
    void* declaring_type;
    const void* return_type;
    const void* parameters;

    union
    {
        const void* rgctx_data;
        const void* methodDefinition;
    };

    union
    {
        const void* genericMethod;
        const void* genericContainer;
    };

    void* customAttributeIndex;
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1;
    uint8_t is_inflated : 1;
} MethodInfo;


// mod structures
struct Config {
    struct AAircraft {
        bool Enabled = false;
        std::string Accuracy = "false";
        std::string AccuracyGrowth = "false";
        std::string AttackPower = "false";
        std::string AttackPowerGrowth = "false";
        std::string CrashDamage = "false";
        std::string Hp = "false";
        std::string HpGrowth = "false";
        std::string Speed = "false";
    };

    struct EEnemies {
        bool Enabled = false;
        std::string AntiAir = "false";
        std::string AntiAirGrowth = "false";
        std::string AntiSubmarine = "false";
        std::string Armor = "false";
        std::string ArmorGrowth = "false";
        std::string Cannon = "false";
        std::string CannonGrowth = "false";
        std::string Evasion = "false";
        std::string EvasionGrowth = "false";
        std::string Hit = "false";
        std::string HitGrowth = "false";
        std::string Hp = "false";
        std::string HpGrowth = "false";
        std::string Luck = "false";
        std::string LuckGrowth = "false";
        std::string Reload = "false";
        std::string ReloadGrowth = "false";
        bool RemoveBuffs = false;
        bool RemoveEquipment = false;
        bool RemoveSkill = false;
        std::string Speed = "false";
        std::string SpeedGrowth = "false";
        std::string Torpedo = "false";
        std::string TorpedoGrowth = "false";
    };

    struct MMisc {
        bool Enabled = false;
        bool ExerciseGodmode = false;
        bool FastStageMovement = false;
    };

    struct SSkins {
        bool Enabled = false;
    };

    struct WWeapons {
        bool Enabled = false;
        std::string Damage = "false";
        std::string ReloadMax = "false";
    };

    AAircraft Aircraft;
    EEnemies Enemies;
    SSkins Skins;
    MMisc Misc;
    WWeapons Weapons;
    bool Valid = true;
};
/*
std::vector<std::string> _default{
        ("# [*] Delete the file to reset it. Restart the game to apply any changes."),
        ("# [*] Options can be one of the following types:"),
        ("#      [1] Bool - e.g. \"true\" or \"false\" - Keys: All \"Enabled\"s, ExerciseGodmode, FastStageMovement, RemoveBuffs, RemoveEquipment and RemoveSkill."),
        ("#      [2] Whole numbers or false to disable changes - If you need an example you're five - Keys: All the ones not mentioned above."),
        ("# [*] \"Enabled\"s apply to the entirety of their section, if they're disabled everything will be."),
        ("# [*] If the game crashes, this file is most likely misconfigured."),
        (""),
        ("original repo: github.com/Egoistically/Perseus."),
        (""),
        ("[Aircraft]"),
        ("Enabled=false"),
        ("Accuracy=false"),
        ("AccuracyGrowth=false"),
        ("AttackPower=false"),
        ("AttackPowerGrowth=false"),
        ("CrashDamage=false"),
        ("Hp=false"),
        ("HpGrowth=false"),
        ("Speed=false"),
        (""),
        ("[Enemies]"),
        ("Enabled=false"),
        ("AntiAir=false"),
        ("AntiAirGrowth=false"),
        ("AntiSubmarine=false"),
        ("Armor=false"),
        ("ArmorGrowth=false"),
        ("Cannon=false"),
        ("CannonGrowth=false"),
        ("Evasion=false"),
        ("EvasionGrowth=false"),
        ("Hit=false"),
        ("HitGrowth=false"),
        ("Hp=false"),
        ("HpGrowth=false"),
        ("Luck=false"),
        ("LuckGrowth=false"),
        ("Reload=false"),
        ("ReloadGrowth=false"),
        ("RemoveBuffs=false"),
        ("RemoveEquipment=false"),
        ("RemoveSkill=false"),
        ("Speed=false"),
        ("SpeedGrowth=false"),
        ("Torpedo=false"),
        ("TorpedoGrowth=false"),
        (""),
        ("[Misc]"),
        ("Enabled=false"),
        ("ExerciseGodmode=false"),
        ("FastStageMovement=false"),
        (""),
        ("[Skins]"),
        ("Enabled=false"),
        (""),
        ("[Weapons]"),
        ("Enabled=false"),
        ("Damage=false"),
        ("ReloadMax=false")
};
*/

#endif //ANDROID_HOOKING_PATCHING_TEMPLATE_MAIN_STRUCTS_H