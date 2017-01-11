#include "charpos.h"
#include <QDataStream>

bool operator==(const CharPos& c1, const CharPos& c2)
{
    return c1.charter == c2.charter && c1.pos == c2.pos && c1.terminal == c2.terminal &&
            c1.bold == c2.bold;
}


QDataStream &operator<<(QDataStream &out, const CharPos& ch)
{
    out << ch.charter << ch.pos << ch.terminal << ch.bold;
    return out;
}


QDataStream &operator>>(QDataStream &in, CharPos& ch)
{
    qint32 tmp_pos;
    in >> ch.charter;
    in >> tmp_pos;
    in >> ch.terminal;
    in >> ch.bold;
    ch.pos = (int)tmp_pos;
    return in;
}


//CharPos operator=(const CharPos c)
//{
//    CharPos r;
//    r.bold = c.bold;
//    r.charter = c.charter;
//    r.pos = c.pos;
//    r.terminal = c.terminal;
//    return r;
//}
