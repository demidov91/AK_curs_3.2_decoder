#include "Decoding.h"
#include "FSRestorer.h"

void decode_async(void* arg)
{
	((FSRestorer*)arg) ->start();
}