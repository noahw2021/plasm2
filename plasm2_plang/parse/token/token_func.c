#include "token.h"
#include <stdlib.h>
#include <string.h>
/*
token_func.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6308 26451 28182)

void token_cat(tokens_t* Destination, tokens_t* Source) {
	Destination->Tokens = realloc(Destination->Tokens, (sizeof(*Destination->Tokens) * (Destination->TokenCount + Source->TokenCount)));
	memcpy(&Destination->Tokens[Destination->TokenCount], Source->Tokens, sizeof(*Destination->Tokens) * Source->TokenCount);

	/*free(Source->Tokens);
	free(Source);*/

	return;
}