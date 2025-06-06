#pragma once

enum class FactorType
{
    UNKNOWN = 0,

    PROJECTION = 1,
    STRUCTURE = 2,

    SMART_PROJECTION = 3,
    DUMB_PROJECTION = 31,

    ODOMETRY = 4,
    PRIOR = 5,

    ORIGIN = 6,

    INERTIAL = 7
};
