#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __MOVE_FLAGS_H__
    #define __MOVE_FLAGS_H__
    #define k_MASK_ENV_MOVEMENT_TYPE  0xff000000
    #define k_SHIFT_ENV_MOVEMENT_TYPE 24

    #define k_MOVEMENT_TYPE_LAND          0x01
    #define k_MOVEMENT_TYPE_WATER         0x02
    #define k_MOVEMENT_TYPE_AIR           0x04
    #define k_MOVEMENT_TYPE_MOUNTAIN      0x08
    #define k_MOVEMENT_TYPE_TRADE         0x10
    #define k_MOVEMENT_TYPE_SHALLOW_WATER 0x20
    #define k_MOVEMENT_TYPE_SPACE         0x40
    #define k_BIT_MOVEMENT_TYPE_LAND          (uint32(k_MOVEMENT_TYPE_LAND << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_WATER         (uint32(k_MOVEMENT_TYPE_WATER << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_SPACE         (uint32(k_MOVEMENT_TYPE_SPACE << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_AIR           (uint32(k_MOVEMENT_TYPE_AIR << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_MOUNTAIN      (uint32(k_MOVEMENT_TYPE_MOUNTAIN << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_TRADE         (uint32(k_MOVEMENT_TYPE_TRADE << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_BIT_MOVEMENT_TYPE_SHALLOW_WATER (uint32(k_MOVEMENT_TYPE_SHALLOW_WATER << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_ATTACK_TYPE_UNDERWATER 0x01
    #define k_BIT_ATTACK_TYPE_UNDERWATER (uint32(k_ATTACK_TYPE_UNDERWATER << k_SHIFT_ENV_MOVEMENT_TYPE))
    #define k_MOVE_AIR_COST 100
    #define k_MOVE_COMBAT_COST 100.0
    #define k_MOVE_ENTER_TRANSPORT_COST 100.0
    #define k_MOVE_SPACE_LAUNCH_COST 100.0
    #define k_MOVE_ISDANGER_COST 1000.0
    #define k_MOVE_TREASPASSING_COST 1000.0
#endif
