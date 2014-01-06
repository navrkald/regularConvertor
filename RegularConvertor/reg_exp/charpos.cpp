#include "charpos.h"

bool operator==(const CharPos& c1, const CharPos& c2)
{
    return c1.charter == c2.charter && c1.pos == c2.pos && c1.terminal == c2.terminal &&
            c1.bold == c2.bold;
}
