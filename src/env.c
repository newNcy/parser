#include "env.h"
#include "new.h"


void init_env(Env * env)
{
	const int strPoolSize = 4096;
	env->stringPool.size = strPoolSize;
	env->stringPool.use = 0;
	env->stringPool.pool = new(strPoolSize);
}
