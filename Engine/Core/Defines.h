#ifndef EN_DEFINES_H
#define EN_DEFINES_H

#define PRODUCTION 1
#define DEPLOY !PRODUCTION

#define PROFILE_CPU 1
#define PROFILE_GPU (PROFILE_CPU && 0)

#endif
